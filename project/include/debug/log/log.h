#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QHash>
#include <QDebug>

#ifdef _DEBUG
	#define debug(topic, message)\
	{\
		if(Log::displaysTopic(topic))\
			qDebug() << Log::format( Log::DEBUG, __FILE__ , __LINE__ , __FUNCTION__ , topic ).header.toStdString().c_str() << message;\
	}
	#define logInfo(message)\
	{\
		if(Log::infoPolicy == Log::SHOW)\
			qDebug() << Log::format( Log::INFO, __FILE__ , __LINE__ , __FUNCTION__ ).header.toStdString().c_str() << message;\
	}
	#define logWarn(message)\
	{\
		if(Log::warnPolicy == Log::SHOW)\
			qDebug() << Log::format( Log::WARN, __FILE__ , __LINE__ , __FUNCTION__ ).header.toStdString().c_str() << message;\
	}
	#define logError(message)\
	{\
		if(Log::errorPolicy == Log::SHOW)\
			qDebug() << Log::format( Log::ERROR, __FILE__ , __LINE__ , __FUNCTION__ ).header.toStdString().c_str() << message;\
	}
#else
	#define debug(topic, message) {}
	#define logInfo(message)\
	{\
		if(Log::infoPolicy == Log::SHOW)\
			qDebug() << "[" << Log::format(Log::INFO).header.toStdString().c_str() << "]" << message;\
	}
	#define logWarn(message)\
	{\
		if(Log::warnPolicy == Log::SHOW)\
			qDebug() << "[" << Log::format(Log::WARN).header.toStdString().c_str() << "]" << message;\
	}
	#define logError(message)\
	{\
		if(Log::errorPolicy == Log::SHOW)\
			qDebug() << "[" << Log::format(Log::ERROR).header.toStdString().c_str() << "]" << message;\
	}
#endif

class Log
{
	public:
		enum LogType
		{
			INFO,
			WARN,
			ERROR,
			DEBUG
		};

		enum Policy
		{
			IGNORE,
			SHOW
		};

		struct Format
		{
			LogType type;
			QString header;
		};

		static Policy infoPolicy;
		static Policy warnPolicy;
		static Policy errorPolicy;
		static Policy defaultDebugPolicy;
		static QHash<QString, Policy> topicPolicy;

		static Format format( LogType type);
		static Format format( LogType type , const char* file , int line , const char* function);
		static Format format( LogType type , const char* file , int line , const char* function , const char* topic);
		static bool displaysTopic(const char* topic);
};

#endif // LOG_H
