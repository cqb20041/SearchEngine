#include "../../include/mylogger/ClientLogger.h"
#include "../../include/configuration/Configuration.h"

ClientLogger *ClientLogger::_pInstance = ClientLogger::GetInstance();

ClientLogger *ClientLogger::GetInstance()
{
    if(nullptr == _pInstance)
    {
        _pInstance = new ClientLogger();
        atexit(destroy);
    }
    return _pInstance;
}

void ClientLogger::destroy()
{
    if(_pInstance)
    {
        delete _pInstance;
        _pInstance = nullptr;
    }
}

void ClientLogger::error(const char *msg) const
{
    root.error(msg);
}

void ClientLogger::info(const char *msg) const
{
    root.info(msg);
}

void ClientLogger::warn(const char *msg) const
{
    root.warn(msg);
}

void ClientLogger::debug(const char *msg) const
{
    root.debug(msg);
}

ClientLogger::ClientLogger()
:root(Category::getRoot().getInstance("ClientLogger:"))
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
    string Clientlogger_path = 
        Configuration::GetInstance()->GetConfmap()["clientlogger_path"];
    RollingFileAppender * prfa = 
        new RollingFileAppender("ClientRollingFile",Clientlogger_path.c_str(),3*1024,2,false);
    prfa->setLayout(ppl3);

//    root.addAppender(poa);
//
//    root.addAppender(pfa);
//
    root.addAppender(prfa);

    root.setPriority(Priority::DEBUG);
}

ClientLogger::~ClientLogger()
{
    Category::shutdown();
}


