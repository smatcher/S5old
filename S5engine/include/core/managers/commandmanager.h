#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "core/utils/singleton.h"
#include "core/log/log.h"

#include <QStringList>
#include <QHash>

class ConsoleWidget;

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
	void registerCommand(QString name, Command command, QString helpmsg);
	void registerConsole(ConsoleWidget* console);

	bool readFile(QString path);

	bool runCommand(QString cmd);
	void echo(QString msg);
	void logToConsole(Log::LogItem& log);

protected:
	CommandManager();
	~CommandManager();

private:

	QHash<QString, Command> m_commands;
	QHash<QString, QString> m_commands_help;

	ConsoleWidget* m_console;

	void printAllHelp();
	void printHelp(QString command);
};

typedef Singleton<CommandManager> SingletonCommandManager;
#define COMMAND_MANAGER SingletonCommandManager::getInstance()

#endif // COMMANDMANAGER_H
