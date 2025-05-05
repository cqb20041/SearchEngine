#pragma once

#include <functional>
#include <memory>

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"
#include "../NonCopyable.h"

using std::function;
using std::shared_ptr;
using std::enable_shared_from_this;

class TcpConnection;
class EventLoop;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr&)>;

class TcpConnection
:private NonCopyable
,public enable_shared_from_this<TcpConnection>
{

public:
    explicit TcpConnection(int fd,EventLoop* EventLoop);
    ~TcpConnection();
    void Send(const string& msg);
    void SendToLoop(const string& msg);
    string Receive();
    string RecLine();
    bool IsClosed() const;
    string Show() const;

    void SetNewConnectionCallback(const TcpConnectionCallback& cb);
    void SetMessageCallback(const TcpConnectionCallback& cb);
    void SetCloseCallback(const TcpConnectionCallback& cb);

    void HandleNewConnectionCallback();
    void HandleMessageCallback();
    void HandleCloseCallback();

private:
    const InetAddress GetLocalAddress() const;
    const InetAddress GetPeerAddress() const;

private:
    EventLoop* _ploop;
    SocketIO _sock_io;
    Socket _client_sock;
    InetAddress _local_address;
    InetAddress _peer_address;

    TcpConnectionCallback _on_connection;
    TcpConnectionCallback _on_message;
    TcpConnectionCallback _on_close;
};

