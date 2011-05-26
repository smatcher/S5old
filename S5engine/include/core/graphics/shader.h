#ifndef SHADER_H
#define SHADER_H

#include "core/resources/resource.h"
#include <QtOpenGL>

class ShaderProgram;
class Shader;
template <class R, class H> class ResourceManager;

class ShaderProgramData : public ResourceData
{
	friend class ResourceHandle<ShaderProgramData>;
	friend class ResourceManager<ShaderProgramData, ShaderProgram>;

public:
	class UniformBase
	{
	public :
		virtual void sendTo(QGLShaderProgram& program) const = 0;
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

		virtual void sendTo(QGLShaderProgram &program) const
		{
			//program.setUniformValueArray((const char*)name.toAscii(), data, width, height); // problème template, on va passer qu'une valeur
			program.setUniformValue((const char*)name.toAscii(), *data);
		}

	private:
		QString name;
		T* data;
		int width;
		int height;
	};


public:
	ShaderProgramData(const QString& name, const QString& path, IResourceFactory* factory) : ResourceData(name,path,factory) {}
	virtual const UniformBase* uniform(int nb) = 0;
	virtual int nbUniforms() = 0;
	virtual void setUniform(const UniformBase* uniform) = 0;
	virtual void use() = 0;
	virtual void unset() = 0;
};

class ShaderProgram : public ResourceHandle<ShaderProgramData>
{
public:
	ShaderProgram() {}
	ShaderProgram(const ShaderProgram& from) : ResourceHandle<ShaderProgramData>(from) {}
	ShaderProgram(ShaderProgramData& from) : ResourceHandle<ShaderProgramData>(from) {}
	virtual ~ShaderProgram() {}
};

class ShaderData : public ResourceData
{
	friend class ResourceHandle<ShaderData>;
	friend class ResourceManager<ShaderData, Shader>;

	public:
		enum ShaderType
		{
			FRAGMENT_SHADER,
			VERTEX_SHADER
		};

		ShaderData(const QString& name, const QString& path, IResourceFactory* factory, ShaderType type) : ResourceData(name,path,factory), m_type(type) {}
		ShaderType shaderType() {return m_type;}
		virtual QGLShader* shader() = 0;

	protected:
		ShaderType m_type;
};

class Shader : public ResourceHandle<ShaderData>
{
public:
	Shader() {}
	Shader(const Shader& from) : ResourceHandle<ShaderData>(from) {}
	Shader(ShaderData& from) : ResourceHandle<ShaderData>(from) {}
	virtual ~Shader() {}
};

#endif // SHADER_H
