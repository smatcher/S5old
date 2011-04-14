#ifndef ASSIMPFACTORY_H
#define ASSIMPFACTORY_H

#include "core/resources/iresourcefactory.h"
#include "core/graphics/mesh.h"
#include <assimp.hpp>

class AssimpFactory : public IResourceFactory
{
public:
	virtual void load(ResourceData* resource);

protected:
	virtual void parseFile(const QString& path, QList<ResourceData*>& content);

private:
	Assimp::Importer m_importer;
};

#endif // ASSIMPFACTORY_H
