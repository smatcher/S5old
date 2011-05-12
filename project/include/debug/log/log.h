#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QHash>
#include <QDebug>

#ifdef _DEBUG
	#define debugGL(message)\
	{\
		GLenum error = glGetError();\
		if(error != GL_NO_ERROR)\
		{\
			const char* msg = (char*)gluErrorString(error);\
			logError( message << QString(msg) );\
		}\
	}
	#define debug(topic, message)\
	{\
		if(Log::displaysTopic(topic))\
                        qDebug() << Log::format( Log::LOG_DEBUG, __FILE__ , __LINE__ , __FUNCTION__ , topic ).header.toStdString().c_str() << message;\
	}
	#define logInfo(message)\
	{\
                if(Log::infoPolicy == Log::POLICY_SHOW)\
                        qDebug() << Log::format( Log::LOG_INFO, __FILE__ , __LINE__ , __FUNCTION__ ).header.toStdString().c_str() << message;\
	}
	#define logWarn(message)\
	{\
                if(Log::warnPolicy == Log::POLICY_SHOW)\
                        qDebug() << Log::format( Log::LOG_WARN, __FILE__ , __LINE__ , __FUNCTION__ ).header.toStdString().c_str() << message;\
	}
	#define logError(message)\
	{\
                if(Log::errorPolicy == Log::POLICY_SHOW)\
                        qDebug() << Log::format( Log::LOG_ERROR, __FILE__ , __LINE__ , __FUNCTION__ ).header.toStdString().c_str() << message;\
	}
#else
	#define debugGL(message) {}
	#define debug(topic, message) {}
	#define logInfo(message)\
	{\
                if(Log::infoPolicy == Log::POLICY_SHOW)\
                        qDebug() << "[" << Log::format(Log::LOG_INFO).header.toStdString().c_str() << "]" << message;\
	}
	#define logWarn(message)\
	{\
                if(Log::warnPolicy == Log::POLICY_SHOW)\
                        qDebug() << "[" << Log::format(Log::LOG_WARN).header.toStdString().c_str() << "]" << message;\
	}
	#define logError(message)\
	{\
                if(Log::errorPolicy == Log::POLICY_SHOW)\
                        qDebug() << "[" << Log::format(Log::LOG_ERROR).header.toStdString().c_str() << "]" << message;\
	}
#endif

class Log
{
	public:
		enum LogType
		{
                        LOG_INFO,
                        LOG_WARN,
                        LOG_ERROR,
                        LOG_DEBUG
		};

		enum Policy
		{
                        POLICY_IGNORE,
                        POLICY_SHOW
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
