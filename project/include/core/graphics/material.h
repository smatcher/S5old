#ifndef MATERIAL_H
#define MATERIAL_H

#include "core/resources/resource.h"

class MaterialData : public ResourceData
{
public:
	MaterialData(const QString& name, const QString& path, IResourceFactory* factory) : ResourceData(name,path,factory) {}
	virtual void apply() = 0;
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
