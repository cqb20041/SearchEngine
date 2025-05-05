#pragma once

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"

class ClientConnection
{
public:
    ClientConnection(int fd);
    ~ClientConnection();

    void Send(const string& msg);
    string Receive();
    string RecLine();
    string Show() const;

private:
    const InetAddress GetLocalAddress() const;
    const InetAddress GetPeerAddress() const;

private:
    SocketIO _sock_io;
    Socket _server_sock;
    InetAddress _local_address;
    InetAddress _peer_address;
};

