#ifndef IRESOURCEMANAGER_H
#define IRESOURCEMANAGER_H

#include <QHash>
#include <QList>
#include <QString>

class IResourceFactory;

template <class Resource, class Handle>
class ResourceManager
{
public:
	Handle get(const QString& name);

	void addFactory(IResourceFactory* factory);

	void parseDir(const QString& path, bool recursive);

	void load(const QString& name);
	void loadAll();
	void loadFile(const QString& path);

	void unloadUnused();
	void unloadEverything();
	void unload(Handle resource);

	void add(Resource* resource);

protected:
	QHash<QString,Resource*> m_resources;
	QList<IResourceFactory*> m_factories;
};

#include "resourcesmanager.hpp"

#endif // IRESOURCEMANAGER_H
