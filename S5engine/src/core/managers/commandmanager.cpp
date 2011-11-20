#include "core/managers/commandmanager.h"

CommandManager::CommandManager()
{
}

CommandManager::~CommandManager()
{
}

QString CommandManager::autocomplete(QString from)
{
	return from + "lolTAB";
}

void CommandManager::registerCommand(QString name, Command command)
{
	m_commands.insert(name, command);
}

void CommandManager::registerVariable(QString name, Type type)
{
	m_variables_type.insert(name, type);
	// void* value = TODO
	// m_variables_value.insert(name, value);
}

bool CommandManager::runCommand(QString cmd)
{
	bool ret = false;

	QStringList strlist = cmd.split(QRegExp("\\s+"));
	if(strlist.count()>0 && m_commands.contains(strlist.at(0)))
	{
		QHash<QString, Command>::iterator it = m_commands.find(strlist.at(0));
		ret = (it.value())(strlist);
	}

	return ret;
}

void* CommandManager::getVariable(QString var)
{
	return NULL;
}

bool CommandManager::setVariable(QString var, void *value)
{
	return false;
}

