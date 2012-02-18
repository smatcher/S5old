#ifndef XMLMATERIAL_H
#define XMLMATERIAL_H

#include "core/graphics/material.h"
#include "core/graphics/texture.h"
#include "core/graphics/shader.h"
#include "core/maths/vector4.h"

#include <QList>
#include <QtOpenGL>

class XmlMaterialFactory;
class QDomNode;

class XmlMaterial : public MaterialData
{
	friend class XmlMaterialFactory;

protected:
	struct MaterialAttributes {
		Vector4f m_diffuse;
		Vector4f m_specular;
		Vector4f m_ambient;
		Vector4f m_emission;
		GLfloat  m_shininess;
		bool     m_transparent;
		bool     m_receive_shadow;
		bool     m_cast_shadow;
		bool     m_doublesided;
		bool     m_uses_sss;
		Texture  m_colormap;
		Texture  m_normalmap;
		Texture  m_specularmap;
		Texture  m_gradientmap;
		Texture  m_splattingmap;
		Texture  m_splatting_red;
		Texture  m_splatting_green;
		Texture  m_splatting_blue;
		Texture  m_sssmap;
		QList<ShaderProgramData::UniformBase*> m_uniforms;

		MaterialAttributes() :
			m_diffuse(0.7,0.7,0.7,1.0),
			m_specular(0.7,0.7,0.7,1.0),
			m_ambient(0.7,0.7,0.7,1.0),
			m_emission(0.0,0.0,0.0,0.0),
			m_shininess(0.7),
			m_transparent(false),
			m_receive_shadow(true),
			m_cast_shadow(true),
			m_doublesided(false),
			m_uses_sss(false) {}
	};

	MaterialAttributes m_default_attributes;
	QVector<MaterialAttributes> m_layers;

public:
	XmlMaterial(const QString& name, const QString& path, IResourceFactory* factory);
	virtual ~XmlMaterial();
	virtual void apply(unsigned int layer);
	virtual void unset(unsigned int layer);
	virtual bool unload();
	virtual bool isTransparent(unsigned int layer);
	virtual bool receivesShadows(unsigned int layer);
	virtual bool castsShadows(unsigned int layer);
	virtual bool usesSSS(unsigned int layer);
	virtual bool usesColorMap(unsigned int layer);
	virtual bool usesNormalMap(unsigned int layer);
	virtual bool usesSpecularMap(unsigned int layer);
	virtual bool usesGradientMap(unsigned int layer);
	virtual bool usesSSSMap(unsigned int layer);
};

class XmlMaterialFactory : public IResourceFactory
{
public:
	virtual void load(ResourceData* resource);

protected:
	virtual void parseFile(const QString& path, QList<ResourceData*>& content, const QHash<QString,QString>& rules);
	virtual void parseTag(const QString& tag, QDomNode* node, XmlMaterial* xmlresource, int layer = -1);
};

#endif // XMLMATERIAL_H
