#include "core/resources/xmlmaterial.h"
#include <QtXml>
#include <QVector>
#include "core/resources/managers.h"
#include "core/managers/rendermanager.h"

XmlMaterial::XmlMaterial(const QString& name, const QString& path, IResourceFactory* factory) :
	MaterialData(name, path, factory)
{
}

XmlMaterial::~XmlMaterial()
{
	/*
	for(int i=0 ; i<m_uniforms.length() ; i++)
	{
		delete m_uniforms[i];
	}
	*/
}

void XmlMaterial::apply(unsigned int layer)
{
	MaterialAttributes* target;

	if(m_layers.size() <= layer) {
		target = &m_default_attributes;
	} else {
		target = &m_layers[layer];
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  target->m_diffuse.coords);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, target->m_specular.coords);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  target->m_ambient.coords);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, target->m_emission.coords);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, target->m_shininess);

	if(target->m_doublesided) {
		glDisable(GL_CULL_FACE);
	}

	// Set ubershader parameters
	UberShader shader = RENDER_MANAGER.getRenderPassInfo()->ubershader_used;
	shader->setParamValue(UberShaderDefine::COLORMAPPED, target->m_colormap.isValid());
	shader->setParamValue(UberShaderDefine::NORMALMAPPED, target->m_normalmap.isValid());
	shader->setParamValue(UberShaderDefine::SPLATTING, target->m_splattingmap.isValid());
	// Bind UberShader
	shader->use();
	shader->setAllUniforms();

	if(target->m_colormap.isValid()) {
		target->m_colormap->bind(shader->getTexUnit(UberShaderTextureType::COLOR_MAP));
	}

	if(target->m_normalmap.isValid()) {
		target->m_normalmap->bind(shader->getTexUnit(UberShaderTextureType::NORMAL_MAP));
	}

	if(target->m_splattingmap.isValid()) {
		target->m_splattingmap->bind(shader->getTexUnit(UberShaderTextureType::SPLATTING));

		if(target->m_splatting_red.isValid()) {
			target->m_splatting_red->bind(shader->getTexUnit(UberShaderTextureType::SPLATTING_R));
		}
		if(target->m_splatting_green.isValid()) {
			target->m_splatting_green->bind(shader->getTexUnit(UberShaderTextureType::SPLATTING_G));
		}
		if(target->m_splatting_blue.isValid()) {
			target->m_splatting_blue->bind(shader->getTexUnit(UberShaderTextureType::SPLATTING_B));
		}
	}
}

void XmlMaterial::unset(unsigned int layer)
{
	MaterialAttributes* target;
	UberShader shader = RENDER_MANAGER.getRenderPassInfo()->ubershader_used;

	if(m_layers.size() <= layer) {
		target = &m_default_attributes;
	} else {
		target = &m_layers[layer];
	}

	if(target->m_doublesided) {
		glEnable(GL_CULL_FACE);
	}

	if(target->m_colormap.isValid()) {
		target->m_colormap->release(shader->getTexUnit(UberShaderTextureType::COLOR_MAP));
	}

	if(target->m_normalmap.isValid()) {
		target->m_normalmap->release(shader->getTexUnit(UberShaderTextureType::NORMAL_MAP));
	}

	if(target->m_splattingmap.isValid()) {
		target->m_splattingmap->release(shader->getTexUnit(UberShaderTextureType::SPLATTING));

		if(target->m_splatting_red.isValid()) {
			target->m_splatting_red->release(shader->getTexUnit(UberShaderTextureType::SPLATTING_R));
		}
		if(target->m_splatting_green.isValid()) {
			target->m_splatting_green->release(shader->getTexUnit(UberShaderTextureType::SPLATTING_G));
		}
		if(target->m_splatting_blue.isValid()) {
			target->m_splatting_blue->release(shader->getTexUnit(UberShaderTextureType::SPLATTING_B));
		}
	}

	shader->unset();
}

bool XmlMaterial::unload()
{
	return false;
}

void XmlMaterialFactory::load(ResourceData* resource)
{
	XmlMaterial* xmlresource = static_cast<XmlMaterial*>(resource);

	QFile file(xmlresource->m_path);

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		logWarn("XmlMaterial : Can't open file" << xmlresource->m_path);
		return;
	}

	QDomDocument doc;
	if(!doc.setContent(&file))
	{
		logWarn("XmlMaterial : Can't parse file" << xmlresource->m_path);
		return;
	}

	QDomNodeList materials = doc.elementsByTagName("material");

	if(materials.length() < 1)
	{
		logWarn("XmlMaterial : No material tag in file" << xmlresource->m_path);
		return;
	}

	QDomNodeList nodes = materials.at(0).childNodes();
	QDomNode layersNode;
	// Parse default attributes
	for(int i=0 ; i < nodes.length() ; i++)
	{
		QDomNode node = nodes.at(i);
		QString tag = node.nodeName();

		if(tag != "layers") {
			parseTag(tag, &node, xmlresource);
		} else {
			layersNode = node;
		}
	}
	// Parse layer attributes
	if(!layersNode.isNull()) {
		QDomNodeList layersNodes = layersNode.childNodes();
		for(int i=0 ; i< layersNodes.length() ; i++) {
			if(layersNodes.at(i).nodeName() == "layer") {
				QDomNodeList nodes = layersNodes.at(i).childNodes();
				int layer = xmlresource->m_layers.size();

				if(layersNodes.at(i).attributes().contains("id")) {
					QString value = layersNodes.at(i).attributes().namedItem("id").nodeValue();
					bool ok;
					int read = value.toInt(&ok);
					if(ok) {
						layer = read;
					} else {
						logWarn("failed to parse" << value << "as int in " << xmlresource->m_path);
					}
				}
				for(int j=0 ; j < nodes.length() ; j++) {
					QDomNode node = nodes.at(j);
					QString tag = node.nodeName();
					parseTag(tag, &node, xmlresource,layer);
				}
			}
		}
	}

	xmlresource->m_state = XmlMaterial::STATE_LOADED;

	logInfo("XmlMaterial loaded" << xmlresource->name());
}

void XmlMaterialFactory::parseTag(const QString& tag, QDomNode* node, XmlMaterial* xmlresource, int layer)
{
	XmlMaterial::MaterialAttributes* target;

	if(layer < 0) {
		target = &xmlresource->m_default_attributes;
	} else {
		while(layer >= xmlresource->m_layers.size()) {
			xmlresource->m_layers.push_back(xmlresource->m_default_attributes);
		}
		target = &xmlresource->m_layers[layer];
	}

	if(tag == "texture")
	{
		QDomNodeList content = node->childNodes();
		QString type = node->attributes().namedItem("type").nodeValue();
		Texture tex = TEXTURE_MANAGER.get(content.at(0).nodeValue());
		if(tex.isValid())
		{
			if(type == "colormap") {
				target->m_colormap = tex;
			} else if(type == "normalmap") {
				target->m_normalmap = tex;
			} else if(type == "splatting") {
				target->m_splattingmap = tex;
			} else if(type == "splat_R") {
				target->m_splatting_red = tex;
			} else if(type == "splat_G") {
				target->m_splatting_green = tex;
			} else if(type == "splat_B") {
				target->m_splatting_blue = tex;
			} else {
				logError("texture tag line "<< node->lineNumber() << "in file" << xmlresource->m_path << "misses the attribute type or has a wrong type");
				logInfo("valid texture types are colormap and normalmap");
			}
		}
		else
		{
			logError("Check texture tag line" << node->lineNumber() << "in file" << xmlresource->m_path << "texture ressource" << content.at(0).nodeValue() << "does not exist");
		}
	}
	else if(tag == "diffuse")
	{
		QDomNodeList content = node->childNodes();
		QStringList txtlist = content.at(0).nodeValue().split(" ",QString::SkipEmptyParts);
		for(int i=0 ; i<4 && i<txtlist.length() ; i++)
		{
			bool ok;
			float val = txtlist.at(i).toFloat(&ok);
			if(ok)
				target->m_diffuse[i] = val;
			else
				logWarn("format is 1.2 3.4 5.6 7.8 while reading" << content.at(0).nodeValue() << "in file" << xmlresource->m_path);
		}
	}
	else if(tag == "specular")
	{
		QDomNodeList content = node->childNodes();
		QStringList txtlist = content.at(0).nodeValue().split(" ",QString::SkipEmptyParts);
		for(int i=0 ; i<4 && i<txtlist.length() ; i++)
		{
			bool ok;
			float val = txtlist.at(i).toFloat(&ok);
			if(ok)
				target->m_specular[i] = val;
			else
				logWarn("format is 1.2 3.4 5.6 7.8 while reading" << content.at(0).nodeValue() << "in file" << xmlresource->m_path);
		}
	}
	else if(tag == "ambient")
	{
		QDomNodeList content = node->childNodes();
		QStringList txtlist = content.at(0).nodeValue().split(" ",QString::SkipEmptyParts);
		for(int i=0 ; i<4 && i<txtlist.length() ; i++)
		{
			bool ok;
			float val = txtlist.at(i).toFloat(&ok);
			if(ok)
				target->m_ambient[i] = val;
			else
				logWarn("format is 1.2 3.4 5.6 7.8 while reading" << content.at(0).nodeValue() << "in file" << xmlresource->m_path);
		}
	}
	else if(tag == "emission")
	{
		QDomNodeList content = node->childNodes();
		QStringList txtlist = content.at(0).nodeValue().split(" ",QString::SkipEmptyParts);
		for(int i=0 ; i<4 && i<txtlist.length() ; i++)
		{
			bool ok;
			float val = txtlist.at(i).toFloat(&ok);
			if(ok)
				target->m_emission[i] = val;
			else
				logWarn("format is 1.2 3.4 5.6 7.8 while reading" << content.at(0).nodeValue() << "in file" << xmlresource->m_path);
		}
	}
	else if(tag == "shininess")
	{
		QDomNodeList content = node->childNodes();
		bool ok;
		float val = content.at(0).nodeValue().toFloat(&ok);
		if(ok)
			target->m_shininess= val;
		else
			logWarn("expected float for shininess" << "in file" << xmlresource->m_path);
	}
	else if(tag == "transparent")
	{
		QDomNodeList content = node->childNodes();
		bool val = content.at(0).nodeValue() == "true" ||
				   content.at(0).nodeValue() == "1";
		target->m_transparent= val;
	}
	else if(tag == "receive_shadow")
	{
		QDomNodeList content = node->childNodes();
		bool val = content.at(0).nodeValue() == "true" ||
				   content.at(0).nodeValue() == "1";
		target->m_receive_shadow= val;
	}
	else if(tag == "cast_shadow")
	{
		QDomNodeList content = node->childNodes();
		bool val = content.at(0).nodeValue() == "true" ||
				   content.at(0).nodeValue() == "1";
		target->m_cast_shadow= val;
	}
	else if(tag == "doublesided")
	{
		QDomNodeList content = node->childNodes();
		bool val = content.at(0).nodeValue() == "true" ||
				   content.at(0).nodeValue() == "1";
		target->m_doublesided= val;
	}
	else if(tag == "uniform")
	{

	}
	else
	{
		logWarn("In file" << xmlresource->m_path << "Unknown tag" << tag);
	}

}

void XmlMaterialFactory::parseFile(const QString &path, QList<ResourceData *> &content, const QHash<QString,QString>&)
{
	QDir dir(path);
	QString name = dir.dirName();

	// Chop extension
	int id = name.lastIndexOf(".");
	if(id > 1)
	{
		name.chop(name.length() - id);
	}

	dir.cdUp();
	if(dir.exists())
	{
		debug( "RESOURCE PARSING" , "XmlMaterial found " << name);
		XmlMaterial* material = new XmlMaterial(name,path,this);
		content.push_back(material);
	}
	else
	{
		debug( "RESOURCE PARSING" , path << " : " << dir << " does not exist");
	}
}

bool XmlMaterial::isTransparent(unsigned int layer)
{
	MaterialAttributes* target;

	if(m_layers.size() <= layer) {
		target = &m_default_attributes;
	} else {
		target = &m_layers[layer];
	}
	return target->m_transparent;
}

bool XmlMaterial::receivesShadows(unsigned int layer)
{
	MaterialAttributes* target;

	if(m_layers.size() <= layer) {
		target = &m_default_attributes;
	} else {
		target = &m_layers[layer];
	}
	return target->m_receive_shadow;
}

bool XmlMaterial::castsShadows(unsigned int layer)
{
	MaterialAttributes* target;

	if(m_layers.size() <= layer) {
		target = &m_default_attributes;
	} else {
		target = &m_layers[layer];
	}
	return target->m_cast_shadow;
}

bool XmlMaterial::usesColorMap(unsigned int layer)
{
	MaterialAttributes* target;

	if(m_layers.size() <= layer) {
		target = &m_default_attributes;
	} else {
		target = &m_layers[layer];
	}
	return target->m_colormap.isValid();
}

bool XmlMaterial::usesNormalMap(unsigned int layer)
{
	MaterialAttributes* target;

	if(m_layers.size() <= layer) {
		target = &m_default_attributes;
	} else {
		target = &m_layers[layer];
	}
	return target->m_normalmap.isValid();
}

