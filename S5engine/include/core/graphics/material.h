#ifndef MATERIAL_H
#define MATERIAL_H

#include "core/resources/resource.h"
class QGLShaderProgram;

class Material;
template <class R, class H> class ResourceManager;

class MaterialData : public ResourceData
{
	friend class ResourceHandle<MaterialData>;
	friend class ResourceManager<MaterialData,Material>;

public:
	MaterialData(const QString& name, const QString& path, IResourceFactory* factory) : ResourceData(name,path,factory) {}
	virtual void apply() = 0;
	virtual void unset() = 0;
	virtual QGLShaderProgram* program() = 0;
	virtual bool isTransparent() = 0;
};

class Material : public ResourceHandle<MaterialData>
{
public:
	Material() {}
	Material(const Material& from) : ResourceHandle<MaterialData>(from) {};
	Material(MaterialData& from) : ResourceHandle<MaterialData>(from) {};
	virtual ~Material() {}
};

#endif // MATERIAL_H
