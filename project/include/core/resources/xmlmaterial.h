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

	class UniformBase
	{
	public :
		virtual void sendTo(QGLShaderProgram& program) = 0;
	};

	template<class T>
	class Uniform: public UniformBase
	{
	public:
		Uniform(const QString& name, T* data, int width, int height) :
			name(name),
			data(data),
			width(width),
			height(height)
		{}
		~Uniform() {delete[] data;}

		virtual void sendTo(QGLShaderProgram &program)
		{
			program.setUniformValueArray(name, data, width, height);
		}

	private:
		QString name;
		T* data;
		int width;
		int height;
	};

protected:
	QList<Texture> m_textures;
	Vector4f m_diffuse;
	Vector4f m_specular;
	Vector4f m_ambient;
	Vector4f m_emission;
	GLfloat  m_shininess;
	ShaderProgram m_program;
	QList<UniformBase*> m_uniforms;

public:
	XmlMaterial(const QString& name, const QString& path, IResourceFactory* factory);
	virtual void apply();
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
