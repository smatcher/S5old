#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QHash>
#include <QDebug>
#include <QDir>

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
								{ Log::LogItem( Log::LOG_DEBUG, __FILE__ , __LINE__ , __FUNCTION__ , topic ).debugStream() << message; }\
	}
	#define logInfo(message)\
	{\
					 if(Log::infoPolicy == Log::POLICY_SHOW)\
								{ Log::LogItem( Log::LOG_INFO, __FILE__ , __LINE__ , __FUNCTION__ ).debugStream() << message; }\
	}
	#define logWarn(message)\
	{\
					 if(Log::warnPolicy == Log::POLICY_SHOW)\
								{ Log::LogItem( Log::LOG_WARN, __FILE__ , __LINE__ , __FUNCTION__ ).debugStream() << message; }\
	}
	#define logError(message)\
	{\
					 if(Log::errorPolicy == Log::POLICY_SHOW)\
								{ Log::LogItem( Log::LOG_ERROR, __FILE__ , __LINE__ , __FUNCTION__ ).debugStream() << message; }\
	}
#else
	#define debugGL(message) {}
	#define debug(topic, message) {}
	#define logInfo(message)\
	{\
					 if(Log::infoPolicy == Log::POLICY_SHOW)\
						{ Log::LogItem(Log::LOG_INFO).debugStream() << message; }\
	}
	#define logWarn(message)\
	{\
					 if(Log::warnPolicy == Log::POLICY_SHOW)\
						{ Log::LogItem(Log::LOG_WARN).debugStream() << message; }\
	}
	#define logError(message)\
	{\
					if(Log::errorPolicy == Log::POLICY_SHOW)\
						{ Log::LogItem(Log::LOG_ERROR).debugStream() << message; }\
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

		class LogItem
		{
		public:

			LogItem( LogType type);
			LogItem( LogType type , const char* file , int line , const char* function);
			LogItem( LogType type , const char* file , int line , const char* function , const char* topic);
			~LogItem();

			QDebug debugStream();

			bool has_file;
			bool has_topic;
			LogType type;
			QDir file;
			int line;
			QString function;
			QString topic;
			QString message;
		};

		static Policy infoPolicy;
		static Policy warnPolicy;
		static Policy errorPolicy;
		static Policy defaultDebugPolicy;
		static QHash<QString, Policy> topicPolicy;

		static bool displaysTopic(const char* topic);
};

#endif // LOG_H
