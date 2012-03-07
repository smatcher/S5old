#ifndef RESOURCESMANAGER_HPP
#define RESOURCESMANAGER_HPP

#include "resourcemanager.h"

#include "core/resources/iresourcefactory.h"
#include "core/log/log.h"

#ifdef WITH_TOOLS
	#include "core/utils/singleton.h"
#endif

#include <QApplication>
#include <QPalette>

template <class Resource, class Handle>
ResourceManager<Resource, Handle>::ResourceManager()
{
#ifdef WITH_TOOLS
	m_model = 0;
#endif
}

template <class Resource, class Handle>
Handle ResourceManager<Resource, Handle>::get(const QString& name)
{
	typename QHash<QString,Resource*>::iterator it = m_resources.find(name);

	if(it == m_resources.end())
		return Handle();

	Resource* res = it.value();
	if(res != 0) {
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
#ifdef WITH_TOOLS
	if(m_model)
		m_model->aboutToInsert();
#endif

	for(typename QList< IResourceFactory * >::Iterator it = m_factories.begin() ; it != m_factories.end() ; it++)
	{
		QList<ResourceData*> found = (*it)->searchDir(path, recursive);

		for(typename QList<ResourceData*>::Iterator it2 = found.begin() ; it2 != found.end() ; it2++)
		{
			if(!m_resources.contains((*it2)->name()))
			{
				m_resources.insert((*it2)->name(),static_cast<Resource*>(*it2));
				#ifdef WITH_TOOLS
					m_sorted_keys.push_back((*it2)->name());
				#endif
			}
			else
			{
				logError( "Resource named " << (*it2)->name() << " discarded because an other resource had the same name" );
			}
		}
	}

#ifdef WITH_TOOLS
	m_sorted_keys.sort();
	if(m_model)
		m_model->finishedInserting();
#endif
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
#ifdef WITH_TOOLS
	if(m_model)
		m_model->aboutToInsert();
#endif

	if(!m_resources.contains(resource->name()))
	{
		m_resources.insert(resource->name(),resource);
	}
	else
	{
		logError( "Resource named " << resource->name() << " discarded because an other resource add the same name" );
	}

#ifdef WITH_TOOLS
	m_sorted_keys.push_back(resource->name());
	m_sorted_keys.sort();
	if(m_model)
		m_model->finishedInserting();
#endif
}

template <class Resource, class Handle>
void ResourceManager<Resource, Handle>::remove(Resource* resource)
{
	if(!m_resources.contains(resource->name()))
	{
		m_resources.remove(resource->name());
	}
	else
	{
		logError( "Resource named " << resource->name() << " not removed because no resource add this name" );
	}
}


#ifdef WITH_TOOLS
template <class Resource, class Handle>
ResourceManagerModel<Resource, Handle>::ResourceManagerModel()
{
	m_manager = &Singleton< ResourceManager<Resource, Handle > >::getInstance();
	m_manager->m_model = this;
}

template <class Resource, class Handle>
int ResourceManagerModel<Resource, Handle>::rowCount(const QModelIndex &parent) const
{
	if(parent.isValid())
		return 0;
	else
		return m_manager->m_sorted_keys.count();
}

template <class Resource, class Handle>
QVariant ResourceManagerModel<Resource, Handle>::data(const QModelIndex &parent, int role) const
{
	if(parent.isValid())
	{
		if(role == Qt::DisplayRole)
		{
			if(parent.row() < m_manager->m_sorted_keys.count())
				return m_manager->m_sorted_keys.at(parent.row());
		}
		else if(role == Qt::TextColorRole)
		{
			const QHash<QString,Resource*>& resources = m_manager->m_resources;
			if(parent.row() < m_manager->m_sorted_keys.count())
			{
				if(resources.value(m_manager->m_sorted_keys.at(parent.row()))->isLoaded())
				{
					return QApplication::palette().text().color();
				}
				else
				{
					/*
					QColor textColor = QApplication::palette().text().color();
					QColor backgroundColor = QApplication::palette().background().color();
					*/
					QColor mix;
					mix.setRgb( 255, 0, 0);
					/*
					 static_cast<int>(0.5 * textColor.green() + 0.5 * backgroundColor.green()),
					 static_cast<int>(0.5 * textColor.blue()  + 0.5 * backgroundColor.blue()));
					 */
					return mix;
				}
			}
		}
		else if(role == Qt::UserRole)
		{
			const QHash<QString,Resource*>& resources = m_manager->m_resources;
			if(parent.row() < m_manager->m_sorted_keys.count())
				return qVariantFromValue((void*)resources.value(m_manager->m_sorted_keys.at(parent.row()))->getWidget());
		}
	}

	return QVariant();
}

template <class Resource, class Handle>
void ResourceManagerModel<Resource, Handle>::aboutToInsert()
{
	beginInsertRows(QModelIndex(), m_manager->m_resources.count(), m_manager->m_resources.count());
}

template <class Resource, class Handle>
void ResourceManagerModel<Resource, Handle>::finishedInserting()
{
	endInsertRows();
}

#endif

#endif // RESOURCESMANAGER_HPP
