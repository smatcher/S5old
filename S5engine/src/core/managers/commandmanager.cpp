#include "core/managers/commandmanager.h"
#include "core/log/log.h"

#ifdef WITH_TOOLS
	#include "tools/widgets/consolewidget.h"
#endif

CommandManager::CommandManager()
{
	m_console = NULL;

	// These commands are catched before, we only add them here for the autocompletion and the help
	m_commands.insert("help",NULL);
	m_commands_help.insert("help","displays a help message on a given command or list all the help messages");
	m_commands.insert("exec",NULL);
	m_commands_help.insert("exec","run every command in a given file");
}

CommandManager::~CommandManager()
{
}

QStringList CommandManager::autocomplete(QString from)
{
	QStringList ret;

	QList<QString> keys = m_commands.keys();
	for(int i=0 ; i<keys.count() ; i++) {
		QString key = keys.at(i);
		if(key.startsWith(from))
		{
			ret.push_back(key);
		}
	}
	return ret;
}

void CommandManager::registerCommand(QString name, Command command, QString helpmsg)
{
	m_commands_help.insert(name, helpmsg);
	registerCommand(name, command);
}

void CommandManager::registerCommand(QString name, Command command)
{
	m_commands.insert(name, command);
}

void CommandManager::registerConsole(ConsoleWidget *console)
{
	m_console = console;
}

void CommandManager::echo(QString msg)
{
	if(m_console)
	{
	#ifdef WITH_TOOLS
		m_console->echo(msg);
	#endif
	}
	else
	{
		logError("Could not echo" << msg << "no console registered");
	}
}

bool CommandManager::runCommand(QString cmd)
{
	bool ret = false;

	QStringList strlist = cmd.split(QRegExp("\\s+"));
	if(strlist.count()>0)
	{
		QString command = strlist.at(0);
		if(command == "help")
		{
			if(strlist.count()>1)
			{
				printHelp(strlist.at(1));
				ret = true;
			}
			else
			{
				printAllHelp();
				ret = true;
			}
		}
		else if(command == "exec")
		{
			if(strlist.count()>1)
			{
				ret = readFile(strlist.at(1));
			}
			else
			{
				echo("you need to specify a file, usage is \"exec some_file.cfg\"");
				ret = false;
			}
		}
		else if(m_commands.contains(command))
		{
			QHash<QString, Command>::iterator it = m_commands.find(strlist.at(0));
			ret = (it.value())(strlist);
		}
		else
		{
			echo("No command named " + command);
		}
	}

	return ret;
}

void CommandManager::printAllHelp()
{
	QString msg;
	for(QHash<QString, QString>::iterator  it = m_commands_help.begin() ;
														it != m_commands_help.end() ;
														it++)
	{
		if(it == m_commands_help.begin())
			msg += (it.key() + " : " + it.value());
		else
			msg += ("\n" + it.key() + " : " + it.value());
	}
	echo(msg);
}

void CommandManager::printHelp(QString command)
{
	if(m_commands_help.contains(command))
	{
		echo(command + " : " + m_commands_help.value(command));
	}
	else if(m_commands.contains(command))
	{
		echo("no help for command " + command);
	}
	else
	{
		echo("no command " + command);
	}
}

void CommandManager::logToConsole(Log::LogItem &log)
{
	if(m_console)
	{
	#ifdef WITH_TOOLS
		m_console->log(log);
	#endif
	}
}

bool CommandManager::readFile(QString path)
{
	bool ret = false;

	QFile file(path);
	if(file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream t(&file);
		while(!t.atEnd())
		{
			QString line = t.readLine();
			line = line.trimmed();

			if(!line.isEmpty() && !line.startsWith('#'))
			{
				runCommand(line);
			}
		}

		ret = true;
	}
	else
	{
		logError("Can't open the file" << path);
	}

	return ret;
}
