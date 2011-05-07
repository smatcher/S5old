#ifndef SHADER_H
#define SHADER_H

#include "core/resources/resource.h"

class Shader;
template <class R, class H> class ResourceManager;

class ShaderData : public ResourceData
{
	friend class ResourceHandle<ShaderData>;
	friend class ResourceManager<ShaderData, Shader>;
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
