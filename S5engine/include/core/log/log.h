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
    static int id = Log::getLogId();\
					if(Log::displaysTopic(topic))\
                                { Log::LogItem( Log::LOG_DEBUG, id, __FILE__ , __LINE__ , __FUNCTION__ , topic ).debugStream() << message; }\
	}
	#define logInfo(message)\
	{\
    static int id = Log::getLogId();\
					 if(Log::infoPolicy == Log::POLICY_SHOW)\
                                { Log::LogItem( Log::LOG_INFO, id, __FILE__ , __LINE__ , __FUNCTION__ ).debugStream() << message; }\
	}
	#define logWarn(message)\
	{\
    static int id = Log::getLogId();\
					 if(Log::warnPolicy == Log::POLICY_SHOW)\
                                { Log::LogItem( Log::LOG_WARN, id, __FILE__ , __LINE__ , __FUNCTION__ ).debugStream() << message; }\
	}
	#define logError(message)\
	{\
    static int id = Log::getLogId();\
					 if(Log::errorPolicy == Log::POLICY_SHOW)\
                                { Log::LogItem( Log::LOG_ERROR, id , __FILE__ , __LINE__ , __FUNCTION__ ).debugStream() << message; }\
	}
#else
	#define debugGL(message) {}
	#define debug(topic, message) {}
	#define logInfo(message)\
	{\
    static int id = Log::getLogId();\
					 if(Log::infoPolicy == Log::POLICY_SHOW)\
                        { Log::LogItem(Log::LOG_INFO, id).debugStream() << message; }\
	}
	#define logWarn(message)\
	{\
    static int id = Log::getLogId();\
					 if(Log::warnPolicy == Log::POLICY_SHOW)\
                        { Log::LogItem(Log::LOG_WARN, id).debugStream() << message; }\
	}
	#define logError(message)\
	{\
    static int id = Log::getLogId();\
					if(Log::errorPolicy == Log::POLICY_SHOW)\
                        { Log::LogItem(Log::LOG_ERROR, id).debugStream() << message; }\
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

            LogItem( LogType type, int id);
            LogItem( LogType type, int id , const char* file , int line , const char* function);
            LogItem( LogType type, int id , const char* file , int line , const char* function , const char* topic);
			~LogItem();

			QDebug debugStream();

			bool has_file;
			bool has_topic;
			LogType type;
			QDir file;
			int line;
            int id;
			QString function;
			QString topic;
			QString message;
		};

		static Policy infoPolicy;
		static Policy warnPolicy;
		static Policy errorPolicy;
		static Policy defaultDebugPolicy;
		static QHash<QString, Policy> topicPolicy;
        static int next_id;

		static bool displaysTopic(const char* topic);
        static int getLogId();
};

#endif // LOG_H
