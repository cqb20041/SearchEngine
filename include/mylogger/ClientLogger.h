#ifndef __CLIENTLOGGER_H__
#define __CLIENTLOGGER_H__

#include <iostream>
#include <string>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>

#define AddMsg(msg) (string("\n") + string(__FILE__) + string("    ") + \
                     string(__FUNCTION__) + string("    ") + \
                     string(to_string(__LINE__)) + string("    ") + \
                     string(msg)).c_str()

#define LogError(msg) ClientLogger::GetInstance()->error(AddMsg(msg))
#define LogInfo(msg) ClientLogger::GetInstance()->info(AddMsg(msg))
#define LogWarn(msg) ClientLogger::GetInstance()->warn(AddMsg(msg))
#define LogDebug(msg) ClientLogger::GetInstance()->debug(AddMsg(msg))

using namespace std;
using namespace log4cpp;

class ClientLogger
{
public:
    static ClientLogger *GetInstance();
    static void destroy();
	void error(const char *msg) const;
	void info(const char *msg) const;
	void warn(const char *msg) const;
	void debug(const char *msg) const;
private:
    ClientLogger();
    ~ClientLogger();
private:
    static ClientLogger *_pInstance;
    Category &root;
};
#endif
