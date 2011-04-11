#ifndef IRESOURCEFACTORY_H
#define IRESOURCEFACTORY_H

#include <QList>
#include <QString>

template <class Resource>
class IResourceFactory
{
public:
	virtual void load(Resource* resource) = 0;
	QList<Resource*> searchPath(const QString& path, bool recursive);

protected:
	virtual void parseFile(const QString& path, QList<Resource*> content) = 0;
};

#include "iresourcefactory.hpp"

#endif // IRESOURCEFACTORY_H
