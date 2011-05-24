#ifndef IRESOURCEFACTORY_H
#define IRESOURCEFACTORY_H

#include <QList>
#include <QHash>
#include <QString>

class ResourceData;

class IResourceFactory
{
public:
	virtual void load(ResourceData* resource) = 0;
	virtual QList<ResourceData*> searchDir(const QString& path, bool recursive);

protected:
	virtual void parseFile(const QString& path, QList<ResourceData*>& content, const QHash<QString,QString>& rules) = 0;

private:
	void parseRuleFile(QString path, QHash<QString,QString>& folder_rules, QHash<QString,QHash<QString,QString> >& files_rules);
};

#endif // IRESOURCEFACTORY_H
