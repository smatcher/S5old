#include "core/resources/xmlmaterial.h"
#include <QtXml>
#include <QVector>
#include "core/resources/managers.h"

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

	if(target->m_textures.length() > 0)
	{
		glEnable(GL_TEXTURE_2D);
		for(int i = 0 ; i< target->m_textures.length() ; i++)
		{
			if(target->m_textures[i].isValid())
				target->m_textures[i]->bind(i);
		}
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	if(target->m_program.isValid())
	{
		target->m_program->use();
		int nb_uniforms = target->m_program->nbUniforms();
		for(int i=0 ; i<nb_uniforms ; i++)
		{
			target->m_program->setUniform(target->m_program->uniform(i));
		}
	}
}

void XmlMaterial::unset(unsigned int layer)
{
	MaterialAttributes* target;

	if(m_layers.size() <= layer) {
		target = &m_default_attributes;
	} else {
		target = &m_layers[layer];
	}

	if(target->m_doublesided) {
		glEnable(GL_CULL_FACE);
	}

	if(target->m_program.isValid())
		target->m_program->unset();

	for(int i = 0 ; i< target->m_textures.length() ; i++)
	{
		if(target->m_textures[i].isValid())
			target->m_textures[i]->release(i);
	}
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
		Texture tex = TEXTURE_MANAGER.get(content.at(0).nodeValue());
		if(tex.isValid())
		{
			target->m_textures.push_back(tex);
		}
		else
		{
			logError("Check texture tag" << node->lineNumber());
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
	else if(tag == "doublesided")
	{
		QDomNodeList content = node->childNodes();
		bool val = content.at(0).nodeValue() == "true" ||
				   content.at(0).nodeValue() == "1";
		target->m_doublesided= val;
	}
	else if(tag == "program")
	{
		QDomNodeList content = node->childNodes();
		QString program_name = content.at(0).nodeValue();

		ShaderProgram prog = SHADER_PROGRAM_MANAGER.get(program_name);

		if(prog.isValid())
		{
			target->m_program = prog;
		}
		else
		{
			logError("In file" << xmlresource->m_path << "require program" << program_name << "which can't be found");
		}
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

QGLShaderProgram* XmlMaterial::program(unsigned int layer)
{
	MaterialAttributes* target;

	if(m_layers.size() <= layer) {
		target = &m_default_attributes;
	} else {
		target = &m_layers[layer];
	}

	if(target->m_program.isValid())
		return target->m_program->program();
	else
		return NULL;
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
