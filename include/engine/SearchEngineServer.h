#pragma once

#include "../configuration/Configuration.h"

#include "../reactor/tcpserver/TcpServer.h"
#include "../reactor/threadpool/ThreadPool.h"

#include "../online/KeyRecommender.h"
#include "../online/PageSearcher.h"

class SearchEngineServer
{
public:
    SearchEngineServer(size_t thread_num,size_t que_size,
                       const string& ip,unsigned short port,
                       KeyRecommender &kr,PageSearcher &ps);
    ~SearchEngineServer();

    void Start();
    void Stop();

    void OnConnection(const TcpConnectionPtr& con);
    void OnMessage(const TcpConnectionPtr& con);
    void OnClose(const TcpConnectionPtr& con);

private:
    ThreadPool _pool;
    TcpServer _server;
    KeyRecommender _kr;
    PageSearcher _ps;
};

