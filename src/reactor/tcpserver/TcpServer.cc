#include "../../../include/reactor/tcpserver/TcpServer.h"
#include "../../../include/mylogger/MyLogger.h"

TcpServer::TcpServer(const string& ip,unsigned short port)
:_acceptor(ip,port)
,_loop(_acceptor)
{

}

TcpServer::~TcpServer(){

}

void TcpServer::Start(){
    _acceptor.ready();
    _loop.Loop();
}

void TcpServer::Stop(){
    _loop.UnLoop();
}

void TcpServer::SetAllCallback(TcpConnectionCallback&& on_connection,
                               TcpConnectionCallback&& on_message,
                               TcpConnectionCallback&& on_close)  
{
    _loop.SetNewConnectionCallback(move(on_connection));
    _loop.SetMessageCallback(move(on_message));
    _loop.SetCloseCallback(move(on_close));
}
