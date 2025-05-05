#include "../../include/engine/QueryTask.h"
#include "../../include/engine/SearchEngineServer.h"
#include "../../include/mylogger/MyLogger.h"

QueryTask::QueryTask(const string &msg,const TcpConnectionPtr &con,                       
          KeyRecommender &kr,PageSearcher &ps)
    :_msg(msg),
    _con(con),
    _kr(kr),
    _ps(ps)
{

}

QueryTask::~QueryTask(){

}

void QueryTask::process(){
    nlohmann::json root = nlohmann::json::parse(_msg);
    string handled_msg;
    size_t msgID = root["msgID"];
    if(1 == msgID){
        string query_word = root["msg"];
        handled_msg = _kr.DoQuery(query_word);
        LogInfo("handling message successed");
    }else if(2 == msgID){
        string query_word = root["msg"];
        handled_msg = _ps.DoQuery(query_word);
        LogInfo("handling message successed");
    }
    _con->SendToLoop(handled_msg);
}

