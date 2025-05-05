#include "../../include/client/Accepted.h"
#include "../../include/mylogger/ClientLogger.h"

Accepted::Accepted(const string &ip,unsigned short port)
:_server_sock(),
_server_addr(ip,port)
{

}

Accepted::~Accepted(){

}

void Accepted::Connect(){
    int ret = connect(_server_sock.fd(),
                      (struct sockaddr*)_server_addr.GetInetAddressPtr(),
                      sizeof(struct sockaddr)); 
    if(-1 == ret){
        LogError("Connect failed");
    }
}

int Accepted::fd(){
    return _server_sock.fd();
}
