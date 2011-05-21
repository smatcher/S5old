#ifndef SHADER_H
#define SHADER_H

#include "core/resources/resource.h"

class ShaderProgram;
class VertexShader;
class FragmentShader;
template <class R, class H> class ResourceManager;

class ShaderProgramData : public ResourceData
{
	friend class ResourceHandle<ShaderProgramData>;
	friend class ResourceManager<ShaderProgramData, ShaderProgram>;

public:
	virtual void use() = 0;
};

class ShaderProgram : public ResourceHandle<ShaderProgramData>
{
public:
	ShaderProgram() {}
	ShaderProgram(const ShaderProgram& from) : ResourceHandle<ShaderProgramData>(from) {}
	ShaderProgram(ShaderProgramData& from) : ResourceHandle<ShaderProgramData>(from) {}
	virtual ~ShaderProgram() {}
};

class VertexShaderData : public ResourceData
{
	friend class ResourceHandle<VertexShaderData>;
	friend class ResourceManager<VertexShaderData, VertexShader>;
};

class VertexShader : public ResourceHandle<VertexShaderData>
{
public:
	VertexShader() {}
	VertexShader(const VertexShader& from) : ResourceHandle<VertexShaderData>(from) {}
	VertexShader(VertexShaderData& from) : ResourceHandle<VertexShaderData>(from) {}
	virtual ~VertexShader() {}
};

class FragmentShaderData : public ResourceData
{
	friend class ResourceHandle<FragmentShaderData>;
	friend class ResourceManager<FragmentShaderData, FragmentShader>;
};

class FragmentShader : public ResourceHandle<FragmentShaderData>
{
public:
	FragmentShader() {}
	FragmentShader(const FragmentShader& from) : ResourceHandle<FragmentShaderData>(from) {}
	FragmentShader(FragmentShaderData& from) : ResourceHandle<FragmentShaderData>(from) {}
	virtual ~FragmentShader() {}
};

#endif // SHADER_H
