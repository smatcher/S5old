#ifndef IRESOURCEMANAGER_H
#define IRESOURCEMANAGER_H

#include <QHash>
#include <QList>
#include <QString>

#ifdef WITH_TOOLS
	#include <QStringList>
	#include <QAbstractListModel>
#endif

class IResourceFactory;

#ifdef WITH_TOOLS
template <class Resource, class Handle>
class ResourceManagerModel;
#endif

template <class Resource, class Handle>
class ResourceManager
{

#ifdef WITH_TOOLS
	friend class ResourceManagerModel<Resource, Handle>;
	private:
		ResourceManagerModel<Resource, Handle>* m_model;
		QStringList m_sorted_keys;
#endif

public:
	ResourceManager();

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
	void remove(Resource* resource);

protected:
	QHash<QString,Resource*> m_resources;
	QList<IResourceFactory*> m_factories;
};

#ifdef WITH_TOOLS
	template <class Resource, class Handle>
	class ResourceManagerModel : public QAbstractListModel
	{
	private:
		ResourceManager<Resource, Handle>* m_manager;

	public:
		ResourceManagerModel();
		virtual int rowCount(const QModelIndex &parent) const;
		virtual QVariant data(const QModelIndex& parent, int role) const;

		void aboutToInsert();
		void finishedInserting();
	};
#endif

#include "resourcesmanager.hpp"

#endif // IRESOURCEMANAGER_H
