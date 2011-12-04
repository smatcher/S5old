#include "core/resources/resource.h"
#include "core/resources/iresourcefactory.h"

#include <QDir>
#include <QtXml>

void IResourceFactory::reload(ResourceData *resource)
{
	if(resource->unload()) {
		resource->load();
	}
}

QList<ResourceData*> IResourceFactory::searchDir(const QString& path, bool recursive)
{
	QList<ResourceData*> ret;
	QDir dir(path);

	QStringList content = dir.entryList(QDir::Files | QDir::Readable);

	// Build rule set
	QHash<QString,QString> folder_rules;
	QHash<QString,QHash<QString,QString> > files_rules;
	parseRuleFile(dir.filePath("rules.db"),folder_rules,files_rules);

	// parseFiles
	for(QStringList::Iterator it = content.begin() ; it != content.end() ; it++)
	{
		if(*it != "rules.db")
		{
			// Fuse folder rules and file rules
			QHash<QString,QString> rules;
			QHash<QString, QHash<QString,QString> >::iterator it2 = files_rules.find(*it);
			if(it2 != files_rules.end())
				rules = *it2;

			for(QHash<QString,QString>::iterator it3 = folder_rules.begin() ; it3 != folder_rules.end() ; it3++)
			{
				if(!rules.contains(it3.key()))
					rules.insert(it3.key(),it3.value());
			}

			// Parse the file
			parseFile(dir.filePath(*it),ret,rules);
		}
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

void IResourceFactory::parseRuleFile(QString path, QHash<QString,QString>& folder_rules, QHash<QString,QHash<QString,QString> >& files_rules)
{
	QFile file(path);

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}

	QDomDocument doc;
	if(!doc.setContent(&file))
	{
		logWarn("ResourceFactory : Can't parse rule file" << path);
		return;
	}

	QDomNodeList rules = doc.elementsByTagName("rules");

	if(rules.length() < 1)
	{
		logWarn("ResourceFactory : No rules tag in file" << path);
		return;
	}

	QDomNodeList nodes = rules.at(0).childNodes();

	for(int i=0 ; i < nodes.length() ; i++)
	{
		QString tag = nodes.at(i).nodeName();

		if(tag == "folder")
		{
			QDomNodeList folder_nodes = nodes.at(i).childNodes();
			for(int j=0 ; j<folder_nodes.length() ; j++)
			{
				if(folder_nodes.at(j).nodeName() == "rule")
				{
					QString name = folder_nodes.at(j).attributes().namedItem("name").nodeValue();
					QString value = folder_nodes.at(j).attributes().namedItem("value").nodeValue();
					folder_rules.insert(name,value);
				}
			}
		}
		else if(tag == "file")
		{
			QString file_name = nodes.at(i).attributes().namedItem("name").nodeValue();
			QHash<QString,QString> file_rules;
			QDomNodeList file_nodes = nodes.at(i).childNodes();
			for(int j=0 ; j<file_nodes.length() ; j++)
			{
				if(file_nodes.at(j).nodeName() == "rule")
				{
					QString name = file_nodes.at(j).attributes().namedItem("name").nodeValue();
					QString value = file_nodes.at(j).attributes().namedItem("value").nodeValue();
					file_rules.insert(name,value);
				}
			}
			files_rules.insert(file_name,file_rules);
		}
	}
}

