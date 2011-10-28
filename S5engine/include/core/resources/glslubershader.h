#ifndef GLSLUBERSHADER_H
#define GLSLUBERSHADER_H

#include "core/graphics/ubershader.h"
#include  <QtOpenGL>

class GLSLUberShaderFactory;

class GLSLUberShader: public UberShaderData
{
	friend class GLSLUberShaderFactory;

public:
	GLSLUberShader(const QString& name, const QString& path, IResourceFactory* factory);
	virtual ~GLSLUberShader();

	virtual bool unload();

	virtual void setUniform(const UniformBase* uniform);
	virtual void setEngineUniforms();
	virtual void setAllUniforms();
	virtual const UniformBase* uniform(int nb);
	virtual int nbUniforms();

	virtual void use();
	virtual void unset();

protected:
	QList<ShaderProgramData::UniformBase*> m_uniforms;
	QString m_vertex_shader;
	QString m_fragment_shader;

private:
	virtual void compileCurrentState();
};

class GLSLUberShaderFactory : public IResourceFactory
{
public:
	virtual void load(ResourceData* resource);

protected:
	virtual void parseFile(const QString& path, QList<ResourceData*>& content, const QHash<QString,QString>& rules);
};

#endif // GLSLUBERSHADER_H
