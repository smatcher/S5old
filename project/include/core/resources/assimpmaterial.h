#ifndef ASSIMPMATERIAL_H
#define ASSIMPMATERIAL_H

#include "core/graphics/material.h"

class aiMaterial;
class AssimpMaterial : public MaterialData
{
private:
	const aiMaterial* m_material;

public :
	AssimpMaterial(const QString& name, const QString& path, IResourceFactory* factory, const aiMaterial* material);
	virtual bool unload();

	virtual void apply();
};


#endif // ASSIMPMATERIAL_H
