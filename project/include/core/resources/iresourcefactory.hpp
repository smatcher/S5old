#ifndef IRESOURCEFACTORY_HPP
#define IRESOURCEFACTORY_HPP

#include "iresourcefactory.h"

#include <QDir>

template <class Resource>
QList<Resource*> IResourceFactory<Resource>::searchPath(const QString& path, bool recursive)
{
	QList<Resource*> ret;
	QDir dir(path);

	QStringList content = dir.entryList(QDir::Files | QDir::Readable);
	for(QStringListIterator it = content.begin() ; it != content.end() ; it++)
	{
		parseFile(*it,ret);
	}

	if(recursive)
	{
		content = dir.entryList(QDir::Dir | QDir::Readable);
		for(QStringListIterator it = content.begin() ; it != content.end() ; it++)
		{
			ret += searchPath(*it, recursive);
		}
	}

	return ret;
}

#endif // IRESOURCEFACTORY_HPP
