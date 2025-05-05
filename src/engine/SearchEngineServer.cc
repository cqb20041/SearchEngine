#include "../../include/engine/SearchEngineServer.h"
#include "../../include/engine/QueryTask.h"
#include "../../include/mylogger/MyLogger.h"

SearchEngineServer::SearchEngineServer(size_t thread_num,size_t que_size,
                       const string& ip,unsigned short port,
                       KeyRecommender& kr,PageSearcher& ps)
    :_pool(thread_num,que_size),
    _server(ip,port),
    _kr(kr),
    _ps(ps)
{

}

SearchEngineServer::~SearchEngineServer(){

}
    
void SearchEngineServer::Start(){
    _pool.Start();

    using namespace std::placeholders;
    _server.SetAllCallback(bind(&SearchEngineServer::OnConnection,this,_1),
                           bind(&SearchEngineServer::OnMessage,this,_1),
                           bind(&SearchEngineServer::OnClose,this,_1));
    _server.Start();
}

void SearchEngineServer::Stop(){
    _server.Stop();
    _pool.Stop();
}

void SearchEngineServer::OnConnection(const TcpConnectionPtr& con){
    cout << con->Show() + "has connected" << endl;
    LogInfo((con->Show() + "has connected").c_str());
}

void SearchEngineServer::OnMessage(const TcpConnectionPtr& con){
    string msg = con->Receive();
    LogInfo("Receiving message successed");
    QueryTask task(msg,con,_kr,_ps);
    _pool.AddTask(bind(&QueryTask::process,task));
}

void SearchEngineServer::OnClose(const TcpConnectionPtr& con){
    cout << con->Show() + "has closed" << endl;
    LogInfo((con->Show() + "has closed").c_str());
}

