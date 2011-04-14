#include "core/resources/resource.h"
#include "core/resources/iresourcefactory.h"

#include <QDir>

QList<ResourceData*> IResourceFactory::searchDir(const QString& path, bool recursive)
{
	QList<ResourceData*> ret;
	QDir dir(path);

	QStringList content = dir.entryList(QDir::Files | QDir::Readable);
	for(QStringList::Iterator it = content.begin() ; it != content.end() ; it++)
	{
		parseFile(dir.filePath(*it),ret);
	}

	if(recursive)
	{
		content = dir.entryList(QDir::Dirs | QDir::Readable | QDir::NoDotAndDotDot);
		for(QStringList::Iterator it = content.begin() ; it != content.end() ; it++)
		{
			QList<ResourceData*> rec = searchDir(dir.filePath(*it), recursive);
			ret.append(rec);
		}
	}

	return ret;
}

