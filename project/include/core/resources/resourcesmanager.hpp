#ifndef RESOURCESMANAGER_HPP
#define RESOURCESMANAGER_HPP

#include "resourcemanager.h"

#include "core/resources/iresourcefactory.h"


template <class Resource, class Handle>
Handle ResourceManager<Resource, Handle>::get(const QString& name)
{
	return Handle(m_resources.find(name));
}


template <class Resource, class Handle>
void ResourceManager<Resource, Handle>::addFactory(IResourceFactory* factory)
{
	m_factories.push_back(factory);
}

template <class Resource, class Handle>
void ResourceManager<Resource, Handle>::parseDir(const QString& path, bool recursive)
{
	for(QList<IResourceFactory*>::iterator it = m_factories.begin() ; it != m_factories.end() ; it++)
	{
		(*it)->searchDir(path, recursive);
	}
}

template <class Resource, class Handle>
void ResourceManager<ResourceManager, Handle>::load(const QString& name)
{
	Resource* resource = m_resources.find(name);
	if(resource->state() == Resource::STATE_UNLOADED)
		resource->factory()->load(resource);
}

template <class Resource, class Handle>
void ResourceManager<ResourceManager, Handle>::loadAll()
{
}

template <class Resource, class Handle>
void ResourceManager<ResourceManager, Handle>::loadFile(const QString& path)
{
}

template <class Resource, class Handle>
void ResourceManager<ResourceManager, Handle>::loadUnused()
{
}

#endif // RESOURCESMANAGER_HPP
