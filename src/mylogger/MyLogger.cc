#include "../../include/mylogger/MyLogger.h"
#include "../../include/configuration/Configuration.h"

MyLogger *MyLogger::_pInstance = MyLogger::GetInstance();

MyLogger *MyLogger::GetInstance()
{
    if(nullptr == _pInstance)
    {
        _pInstance = new MyLogger();
        atexit(destroy);
    }
    return _pInstance;
}

void MyLogger::destroy()
{
    if(_pInstance)
    {
        delete _pInstance;
        _pInstance = nullptr;
    }
}

void MyLogger::error(const char *msg) const
{
    root.error(msg);
}

void MyLogger::info(const char *msg) const
{
    root.info(msg);
}

void MyLogger::warn(const char *msg) const
{
    root.warn(msg);
}

void MyLogger::debug(const char *msg) const
{
    root.debug(msg);
}

MyLogger::MyLogger()
:root(Category::getRoot().getInstance("MyLogger:"))
{
//    PatternLayout * ppl1 = new PatternLayout();
//    ppl1->setConversionPattern("%d [%p] %c %m %n");
//
//    PatternLayout * ppl2 = new PatternLayout();
//    ppl2->setConversionPattern("%d [%p] %c %m %n");
//
    PatternLayout * ppl3 = new PatternLayout();
    ppl3->setConversionPattern("%d [%p] %c %m %n");

//    OstreamAppender * poa = new OstreamAppender("OstreamAppender",&cout);
//    poa->setLayout(ppl1);
//
//    FileAppender * pfa = new FileAppender("File","File.txt",false);
//    pfa->setLayout(ppl2);
//
    string mylogger_path = 
        Configuration::GetInstance()->GetConfmap()["mylogger_path"];
    RollingFileAppender * prfa = 
        new RollingFileAppender("RollingFile",mylogger_path.c_str(),3*1024,2,false);
    prfa->setLayout(ppl3);

//    root.addAppender(poa);
//
//    root.addAppender(pfa);
//
    root.addAppender(prfa);

    root.setPriority(Priority::DEBUG);
}

MyLogger::~MyLogger()
{
    Category::shutdown();
}


