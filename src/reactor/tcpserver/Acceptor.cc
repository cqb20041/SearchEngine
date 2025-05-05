#include "../../../include/reactor/tcpserver/Acceptor.h"
#include "../../../include/mylogger/MyLogger.h"

Acceptor::Acceptor(const string& ip,unsigned short port)
:_listen_sock()    
,_server_addr(ip,port)
{

}

Acceptor::~Acceptor(){

}

void Acceptor::ready(){
    SetReuseAddr();
    SetReusePort();
    Bind();
    Listen();
}

int Acceptor::Accept(){
    int peerfd = ::accept(_listen_sock.fd(),nullptr,nullptr);
    if(-1 == peerfd){
        LogError("Accept failed");
    }
    return peerfd;
}

int Acceptor::fd(){
    return _listen_sock.fd();
}

void Acceptor::SetReuseAddr(){
    int opt = 1;
    int ret = 
        ::setsockopt(_listen_sock.fd(),SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    if(-1 == ret){
        LogError("SetReuseAddr failed");
    }
}

void Acceptor::SetReusePort(){
    int opt = 1;
    int ret = 
        ::setsockopt(_listen_sock.fd(),SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt));
    if(-1 == ret){
        LogError("SetReusePort failed");
    }
}

void Acceptor::Bind(){
    int ret = ::bind(_listen_sock.fd(),
                     (struct sockaddr*) _server_addr.GetInetAddressPtr(),
                     sizeof(struct sockaddr));
    if(-1 == ret){
        LogError("Bind failed");
    }
}

void Acceptor::Listen(){
    int ret = ::listen(_listen_sock.fd(),256);
    if(-1 == ret){
        LogError("Listen failed");
    }
}

