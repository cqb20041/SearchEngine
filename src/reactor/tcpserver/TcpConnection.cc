#include "../../../include/reactor/tcpserver/TcpConnection.h"
#include "../../../include/reactor/tcpserver/EventLoop.h"
#include "../../../include/mylogger/MyLogger.h"

TcpConnection::TcpConnection(int fd,EventLoop* ploop)
:_ploop(ploop)
,_sock_io(fd)
,_client_sock(fd)
,_local_address(GetLocalAddress())
,_peer_address(GetPeerAddress())
{

}

TcpConnection::~TcpConnection(){

}

void TcpConnection::Send(const string& msg){
    uint32_t len = msg.size();
    uint32_t net_len = htonl(len);
    _sock_io.Writen((char*)&net_len,sizeof(net_len));
    _sock_io.Writen(msg.c_str(),msg.size());
}

void TcpConnection::SendToLoop(const string& msg){
    if(_ploop){
        _ploop->RunInLoop(bind(&TcpConnection::Send,this,msg));        
    }
}

string TcpConnection::Receive(){
    uint32_t net_len;
    if(_sock_io.Readn((char*)&net_len,sizeof(net_len)) != sizeof(net_len)){
        LogError("Receive failed");
    }
    uint32_t len = ntohl(net_len);
    
    string msg(len,'\0');
    _sock_io.Readn(&msg[0],len);
    return msg;
}

string TcpConnection::RecLine(){
    char buf[65535] = {0};
    _sock_io.ReadLine(buf,sizeof(buf));
    return string(buf);
}

bool TcpConnection::IsClosed() const{
    char temp_buf[128] = {0};
    int ret = ::recv(_client_sock.fd(), temp_buf, sizeof(temp_buf), MSG_PEEK);
    while (-1 == ret && errno == EINTR) {
        ret = ::recv(_client_sock.fd(), temp_buf, sizeof(temp_buf), MSG_PEEK);
    }
    return 0 == ret;
}

string TcpConnection::Show() const{
    ostringstream oss;
    oss << "[" << _local_address.ip() << ":"
        << _local_address.port() << "---->"
        << _peer_address.ip() << ":"
        << _peer_address.port() << "]";
    return oss.str();
}

void TcpConnection::SetNewConnectionCallback(const TcpConnectionCallback& cb){
    _on_connection = cb;
}

void TcpConnection::SetMessageCallback(const TcpConnectionCallback& cb){
    _on_message = cb;
}

void TcpConnection::SetCloseCallback(const TcpConnectionCallback& cb){
    _on_close = cb;
}

void TcpConnection::HandleNewConnectionCallback(){
    if(_on_connection) _on_connection(shared_from_this());
}

void TcpConnection::HandleMessageCallback(){
    if(_on_message) _on_message(shared_from_this());
}

void TcpConnection::HandleCloseCallback(){
    if(_on_close) _on_close(shared_from_this());
}

const InetAddress TcpConnection::GetLocalAddress() const{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr); 
    int ret = 
        getsockname(_client_sock.fd(),(struct sockaddr*)&addr,&len);
    if(-1 == ret){
        LogError("GetLocalAddress failed");
    }
    return InetAddress(addr);
}

const InetAddress TcpConnection::GetPeerAddress() const{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr); 
    int ret = 
        getpeername(_client_sock.fd(),(struct sockaddr*)&addr,&len);
    if(-1 == ret){
        LogError("GetPeerAddress failed");
    }
    return InetAddress(addr);
}

