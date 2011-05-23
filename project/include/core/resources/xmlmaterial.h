#ifndef XMLMATERIAL_H
#define XMLMATERIAL_H

#include "core/graphics/material.h"
#include "core/graphics/texture.h"
#include "core/graphics/shader.h"
#include "core/maths/vector4.h"

#include <QList>
#include <QtOpenGL>

class XmlMaterialFactory;

class XmlMaterial : public MaterialData
{
	friend class XmlMaterialFactory;

protected:
	QList<Texture> m_textures;
	Vector4f m_diffuse;
	Vector4f m_specular;
	Vector4f m_ambient;
	Vector4f m_emission;
	GLfloat  m_shininess;
	ShaderProgram m_program;
	QList<ShaderProgramData::UniformBase*> m_uniforms;

public:
	XmlMaterial(const QString& name, const QString& path, IResourceFactory* factory);
	virtual ~XmlMaterial();
	virtual void apply();
	virtual void unset();
	virtual bool unload();
};

class XmlMaterialFactory : public IResourceFactory
{
public:
	virtual void load(ResourceData* resource);

protected:
	virtual void parseFile(const QString& path, QList<ResourceData*>& content);
};

#endif // XMLMATERIAL_H
