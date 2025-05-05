#ifndef __MYLOGGER_H__
#define __MYLOGGER_H__

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

#define LogError(msg) MyLogger::GetInstance()->error(AddMsg(msg))
#define LogInfo(msg) MyLogger::GetInstance()->info(AddMsg(msg))
#define LogWarn(msg) MyLogger::GetInstance()->warn(AddMsg(msg))
#define LogDebug(msg) MyLogger::GetInstance()->debug(AddMsg(msg))

using namespace std;
using namespace log4cpp;

class MyLogger
{
public:
    static MyLogger *GetInstance();
    static void destroy();
	void error(const char *msg) const;
	void info(const char *msg) const;
	void warn(const char *msg) const;
	void debug(const char *msg) const;
private:
    MyLogger();
    ~MyLogger();
private:
    static MyLogger *_pInstance;
    Category &root;
};
#endif
