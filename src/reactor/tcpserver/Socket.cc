#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include "../../../include/reactor/tcpserver/Socket.h"
#include "../../../include/mylogger/MyLogger.h"

Socket::Socket(){
    _fd = ::socket(AF_INET,SOCK_STREAM,0);
    if(_fd < 0){
        LogError("Socket created unsuccessfully");
        return;
    } 
}   

Socket::Socket(int fd)
:_fd(fd)
{

}

Socket::~Socket(){
    close(_fd);
}

int Socket::fd() const{
    return _fd;
} 

