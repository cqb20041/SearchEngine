#include "../../../include/reactor/tcpserver/InetAddress.h"
#include <string.h>

InetAddress::InetAddress(const string& ip,unsigned short port){
    ::bzero(&_addr,sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = ::htons(port);
    _addr.sin_addr.s_addr = ::inet_addr(ip.c_str());
}

InetAddress::InetAddress(const struct sockaddr_in& addr)
:_addr(addr)
{
    
}

InetAddress::~InetAddress(){

}

string InetAddress::ip() const{
    return string(::inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddress::port() const{
    return ::ntohs(_addr.sin_port);
}

const struct sockaddr_in* InetAddress::GetInetAddressPtr() const{
    return &_addr;
}

