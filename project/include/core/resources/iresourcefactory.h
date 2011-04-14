#ifndef IRESOURCEFACTORY_H
#define IRESOURCEFACTORY_H

#include <QList>
#include <QString>

class ResourceData;

class IResourceFactory
{
public:
	virtual void load(ResourceData* resource) = 0;
	virtual QList<ResourceData*> searchDir(const QString& path, bool recursive);

protected:
	virtual void parseFile(const QString& path, QList<ResourceData*>& content) = 0;
};

#endif // IRESOURCEFACTORY_H
