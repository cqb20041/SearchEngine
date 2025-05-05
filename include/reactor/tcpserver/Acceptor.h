#pragma once

#include "Socket.h"
#include "InetAddress.h"

class Acceptor
{
public:
    Acceptor(const string& ip,unsigned short port);
    ~Acceptor();
    void ready();

    int Accept();
    int fd();

private:
    void SetReuseAddr();
    void SetReusePort();
    void Bind();
    void Listen();

private:
    Socket _listen_sock;
    InetAddress _server_addr;
};

