#ifndef GLSLSHADER_H
#define GLSLSHADER_H

#include "core/graphics/shader.h"
#include  <QtOpenGL>

class GLSLShaderProgramFactory;

class GLSLShaderProgram : public ShaderProgramData
{
	friend class GLSLShaderProgramFactory;

public:
	GLSLShaderProgram(const QString& name, const QString& path, IResourceFactory* factory);
	virtual ~GLSLShaderProgram();

	virtual bool unload();

	virtual void use();
	virtual void unset();

protected:
	QGLShaderProgram* m_program;
};

class GLSLShaderProgramFactory : public IResourceFactory
{
public:
	virtual void load(ResourceData* resource);

protected:
	virtual void parseFile(const QString& path, QList<ResourceData*>& content);
};

class GLSLShaderFactory;

class GLSLShader : public ShaderData
{
	friend class GLSLShaderFactory;

public:
	GLSLShader(const QString& name, const QString& path, IResourceFactory* factory, ShaderType type);
	virtual bool unload();
	virtual QGLShader* shader();

protected:
	QGLShader* m_shader;
};

class GLSLShaderFactory : public IResourceFactory
{
public:
	virtual void load(ResourceData* resource);

protected:
	virtual void parseFile(const QString& path, QList<ResourceData*>& content);
};

#endif // GLSLSHADER_H
