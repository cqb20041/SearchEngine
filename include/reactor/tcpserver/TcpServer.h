#pragma once

#include "Acceptor.h"
#include "EventLoop.h"

class TcpServer
{
public:
    TcpServer(const string& ip,unsigned short port);
    ~TcpServer();
    void Start();
    void Stop();
    void SetAllCallback(TcpConnectionCallback&& on_connection,
                        TcpConnectionCallback&& on_message,
                        TcpConnectionCallback&& on_close);

private:
    Acceptor _acceptor;
    EventLoop _loop;
};

