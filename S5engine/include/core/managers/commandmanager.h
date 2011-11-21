#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "core/utils/singleton.h"

#include <QStringList>
#include <QHash>

class CommandManager
{
	friend class Singleton<CommandManager>;

public:
	typedef bool (*Command) (QStringList);
	enum Type
	{
		INT,
		FLOAT,
		STRING
	};

	QStringList autocomplete(QString);
	void registerCommand(QString name, Command command);
	void registerVariable(QString name, Type type);

	bool runCommand(QString cmd);
	void* getVariable(QString var);
	bool 	setVariable(QString var, void* value);

protected:
	CommandManager();
	~CommandManager();

private:

	QHash<QString, Command> m_commands;
	QHash<QString, Type> m_variables_type;
	QHash<QString, void*> m_variables_value;
};

typedef Singleton<CommandManager> SingletonCommandManager;
#define COMMAND_MANAGER SingletonCommandManager::getInstance()

#endif // COMMANDMANAGER_H
