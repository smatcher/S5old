#ifndef IRESOURCEFACTORY_H
#define IRESOURCEFACTORY_H

#include <QList>
#include <QString>

template <class Resource>
class IResourceFactory
{
public:
	virtual Resource* load(QString name) = 0;

	void addSearchPath(const QString& path);
	void removeSearchPath(const QString& path);
	void clearSearchPaths();

protected:
	QList<QString> m_search_paths;
};

#endif // IRESOURCEFACTORY_H
