#ifndef IRESOURCEMANAGER_H
#define IRESOURCEMANAGER_H

#include <QHash>
#include <QString>

template <class Resource>
class IResourceManager
{
public:
	Resource* get(const QString& name);
	void unloadUnused();

private:
	void unload(Resource* resource);

protected:
	QHash<QString,Resource*> m_resources;
};

#endif // IRESOURCEMANAGER_H
