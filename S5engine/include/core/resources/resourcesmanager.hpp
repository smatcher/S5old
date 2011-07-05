#ifndef RESOURCESMANAGER_HPP
#define RESOURCESMANAGER_HPP

#include "resourcemanager.h"

#include "core/resources/iresourcefactory.h"
#include "core/log/log.h"


template <class Resource, class Handle>
Handle ResourceManager<Resource, Handle>::get(const QString& name)
{
	typename QHash<QString,Resource*>::iterator it = m_resources.find(name);

	if(it == m_resources.end())
		return Handle();

	Resource* res = it.value();
	if(res != NULL) {
		Handle ret(*res);
		if(ret->state() == Resource::STATE_UNLOADED) {
			ret->load();
		}
		return ret;
	} else {
		return Handle();
	}
}


template <class Resource, class Handle>
void ResourceManager<Resource, Handle>::addFactory(IResourceFactory* factory)
{
	m_factories.push_back(factory);
}

template <class Resource, class Handle>
void ResourceManager<Resource, Handle>::parseDir(const QString& path, bool recursive)
{
	for(typename QList< IResourceFactory * >::Iterator it = m_factories.begin() ; it != m_factories.end() ; it++)
	{
		QList<ResourceData*> found = (*it)->searchDir(path, recursive);

		for(typename QList<ResourceData*>::Iterator it2 = found.begin() ; it2 != found.end() ; it2++)
		{
			if(!m_resources.contains((*it2)->name()))
			{
				m_resources.insert((*it2)->name(),static_cast<Resource*>(*it2));
			}
			else
			{
				logError( "Resource named " << (*it2)->name() << " discarded because an other resource add the same name" );
			}
		}
	}
}

template <class Resource, class Handle>
void ResourceManager<Resource, Handle>::load(const QString& name)
{
	Resource* resource = m_resources.find(name);
	if(resource->state() == Resource::STATE_UNLOADED)
		resource->load();
}

template <class Resource, class Handle>
void ResourceManager<Resource, Handle>::loadAll()
{
	for(typename QHash<QString, Resource*>::Iterator it = m_resources.begin() ; it != m_resources.end() ; it++)
	{
		if((*it)->state() == Resource::STATE_UNLOADED)
			(*it)->load();
	}
}

template <class Resource, class Handle>
void ResourceManager<Resource, Handle>::loadFile(const QString& path)
{
}

template <class Resource, class Handle>
void ResourceManager<Resource, Handle>::unloadUnused()
{
	for(typename QMap<QString, Resource*>::Iterator it = m_resources.begin() ; it != m_resources.end() ; it++)
	{
		if((*it)->state() == Resource::STATE_LOADED && (*it)->refCount() < 1)
			(*it)->unload();
	}
}

template <class Resource, class Handle>
void ResourceManager<Resource, Handle>::unloadEverything()
{
}

template <class Resource, class Handle>
void ResourceManager<Resource, Handle>::unload(Handle resource)
{
}

template <class Resource, class Handle>
void ResourceManager<Resource, Handle>::add(Resource* resource)
{
	if(!m_resources.contains(resource->name()))
	{
		m_resources.insert(resource->name(),resource);
	}
	else
	{
		logError( "Resource named " << resource->name() << " discarded because an other resource add the same name" );
	}
}

#endif // RESOURCESMANAGER_HPP
