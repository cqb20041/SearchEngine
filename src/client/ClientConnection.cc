#include "../../include/client/ClientConnection.h"
#include "../../include/mylogger/ClientLogger.h"

#include <sstream>

ClientConnection::ClientConnection(int fd)
:_sock_io(fd)
,_server_sock(fd)
,_local_address(GetLocalAddress())
,_peer_address(GetPeerAddress())
{

}

ClientConnection::~ClientConnection(){

}

void ClientConnection::Send(const string& msg){
    uint32_t len = msg.size();                                                                      
    uint32_t net_len = htonl(len);
    _sock_io.Writen((char*)&net_len,sizeof(net_len));
    _sock_io.Writen(msg.c_str(),msg.size());
}

string ClientConnection::Receive(){
    uint32_t net_len;
    if(_sock_io.Readn((char*)&net_len,sizeof(net_len)) != sizeof(net_len)){
        LogError("Receive failed");
    } 
    uint32_t len = ntohl(net_len);
                      
    string msg(len,'\0');
    _sock_io.Readn(&msg[0],len);
    return msg;
}

string ClientConnection::RecLine(){
    char buf[65535] = {0};
    _sock_io.ReadLine(buf,sizeof(buf));
    return string(buf);
}

string ClientConnection::Show() const{
    std::ostringstream oss;
    oss << "[" << _local_address.ip() << ":"
        << _local_address.port() << "---->"
        << _peer_address.ip() << ":"
        << _peer_address.port() << "]";
    return oss.str();
}

const InetAddress ClientConnection::GetLocalAddress() const{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr); 
    int ret = 
        getsockname(_server_sock.fd(),(struct sockaddr*)&addr,&len);
    if(-1 == ret){
        LogError("GetLocalAddress failed");
    }
    return InetAddress(addr);
}

const InetAddress ClientConnection::GetPeerAddress() const{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr); 
    int ret = 
        getpeername(_server_sock.fd(),(struct sockaddr*)&addr,&len);
    if(-1 == ret){
        LogError("GetPeerAddress failed");
    }
    return InetAddress(addr);
}
