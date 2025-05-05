#pragma once

#include "Socket.h"
#include "InetAddress.h"

class Accepted
{
public:
    Accepted(const string& ip,unsigned short port);
    ~Accepted();

    void Connect();
    int fd();

private:
    Socket _server_sock;
    InetAddress _server_addr;
};

