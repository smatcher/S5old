#include "core/log/log.h"
#include "core/managers/commandmanager.h"
#include <QDir>

Log::Policy Log::infoPolicy = Log::POLICY_SHOW;
Log::Policy Log::warnPolicy = Log::POLICY_SHOW;
Log::Policy Log::errorPolicy = Log::POLICY_SHOW;
Log::Policy Log::defaultDebugPolicy = Log::POLICY_SHOW;
QHash<QString, Log::Policy> Log::topicPolicy;

QString typeToString(Log::LogType type)
{
	QString stype;

	switch(type)
	{
				case Log::LOG_INFO:
			stype = "Info";
			break;
				case Log::LOG_WARN:
			stype = "Warning";
			break;
				case Log::LOG_ERROR:
			stype = "Error";
			break;
				case Log::LOG_DEBUG:
			stype = "Debug";
			break;
	}

	return stype;
}

bool typeFromString(QString string, Log::LogType& type)
{
	bool ret = true;

	if(string == "Info")
		type = Log::LOG_INFO;
	else if(string == "Warning")
		type = Log::LOG_WARN;
	else if(string == "Error")
		type = Log::LOG_ERROR;
	else if(string == "Debug")
		type = Log::LOG_DEBUG;
	else
		ret = false;

	return ret;
}

Log::LogItem::LogItem(LogType type)
{
	this->type = type;

	has_file = false;
	has_topic = false;
}

Log::LogItem::LogItem(LogType type, const char *file, int line, const char *function)
{
	this->type = type;
	this->file = QDir(file);
	this->line = line;
	this->function = QString(function);

	has_file = true;
	has_topic = false;
}

Log::LogItem::LogItem(LogType type, const char *file, int line, const char *function, const char *topic)
{
	this->type = type;
	this->file = QDir(file);
	this->line = line;
	this->function = QString(function);
	this->topic = QString(topic);

	has_file = true;
	has_topic = true;
}

Log::LogItem::~LogItem()
{
	QString header = typeToString(type);

	if(has_file)
	{
		if(has_topic)
		{
			header += QChar('(') + QString(topic) + QString(")[")
			+ QDir(file).dirName() + QChar(':') + QString().setNum(line) + QString(" in ") + QString(function) + QChar(']');
		}
		else
		{
			header += QChar('[') + QDir(file).dirName() + QChar(':') + QString().setNum(line) + QString(" in ") + QString(function) + QChar(']');
		}
	}
	else
	{
		if(has_topic)
		{
			header += QChar('(') + QString(topic) + QChar(')');
		}
	}

	switch(type)
	{
		case LOG_INFO:
		case LOG_DEBUG:
			qDebug() << (header + " " + message).toAscii().data();
			break;
		case LOG_WARN:
			qWarning() << (header + " " + message).toAscii().data();
			break;
		case LOG_ERROR:
			qCritical() << header + " " + message.toAscii().data();
			break;
	}

	COMMAND_MANAGER.logToConsole(*this);
}

QDebug Log::LogItem::debugStream()
{
	return QDebug(&message);
}

bool Log::displaysTopic(const char* topic)
{
	QHash<QString, Policy>::Iterator it = topicPolicy.find(QString(topic));

	if(it != topicPolicy.end())
				return it.value() == POLICY_SHOW;
	else
		return defaultDebugPolicy;
}
