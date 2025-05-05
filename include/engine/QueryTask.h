#pragma once

#include <functional>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>

using std::function;
using std::shared_ptr;
using std::string;

class TcpConnection;
class KeyRecommender;
class PageSearcher;

using TcpConnectionPtr = shared_ptr<TcpConnection>;

class QueryTask
{
public:
    QueryTask(const string &msg,const TcpConnectionPtr &con,
              KeyRecommender &kr,PageSearcher &ps);
    ~QueryTask();

    void process();

private:
    string _msg;
    TcpConnectionPtr _con;
    KeyRecommender &_kr;
    PageSearcher &_ps;
};

