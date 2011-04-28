#include "debug/log/log.h"
#include <QDir>

Log::Policy Log::infoPolicy = Log::SHOW;
Log::Policy Log::warnPolicy = Log::SHOW;
Log::Policy Log::errorPolicy = Log::SHOW;
Log::Policy Log::defaultDebugPolicy = Log::SHOW;
QHash<QString, Log::Policy> Log::topicPolicy;

QString typeToString(Log::LogType type)
{
	QString stype;

	switch(type)
	{
		case Log::INFO:
			stype = "Info";
			break;
		case Log::WARN:
			stype = "Warning";
			break;
		case Log::ERROR:
			stype = "Error";
			break;
		case Log::DEBUG:
			stype = "Debug";
			break;
	}

	return stype;
}

Log::Format Log::format(LogType type)
{
	Format ret;

	ret.type = type;
	ret.header = typeToString(type);

	return ret;
}

Log::Format Log::format(LogType type, const char *file, int line, const char *function)
{
	Format ret;

	ret.type = type;
	ret.header = typeToString(type) + QChar('[') + QDir(file).dirName() + QChar(':') + QString().setNum(line) + QString(" in ") + QString(function) + QChar(']');

	return ret;
}

Log::Format Log::format(LogType type, const char *file, int line, const char *function, const char *topic)
{
	Format ret;

	ret.type = type;
	ret.header = typeToString(type) + QChar('(') + QString(topic) + QString(")[")
			   + QDir(file).dirName() + QChar(':') + QString().setNum(line) + QString(" in ") + QString(function) + QChar(']');

	return ret;
}

bool Log::displaysTopic(const char* topic)
{
	QHash<QString, Policy>::Iterator it = topicPolicy.find(QString(topic));

	if(it != topicPolicy.end())
		return it.value() == SHOW;
	else
		return defaultDebugPolicy;
}
