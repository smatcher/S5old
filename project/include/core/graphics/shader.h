#ifndef SHADER_H
#define SHADER_H

#include "core/resources/resource.h"

class ShaderData : public ResourceData {};

class Shader : public ResourceHandle<ShaderData>
{
public:
	Shader() {}
	Shader(const Shader& from) : ResourceHandle<ShaderData>(from) {}
	Shader(ShaderData& from) : ResourceHandle<ShaderData>(from) {}
	virtual ~Shader() {}
};

#endif // SHADER_H
