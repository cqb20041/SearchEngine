#pragma once

#include "Acceptor.h"
#include "TcpConnection.h"
#include "../MutexLock.h"

#include <vector>
#include <map>
#include <sys/epoll.h>

using std::vector;
using std::map;

using SendTask = function<void()>;

class EventLoop
{

public:
    EventLoop(Acceptor& acceptor);
    ~EventLoop();
    void Loop();
    void UnLoop();
    
    void SetNewConnectionCallback(TcpConnectionCallback&& cb);
    void SetMessageCallback(TcpConnectionCallback&& cb);
    void SetCloseCallback(TcpConnectionCallback&& cb);

    void WakeupEvfd();
    void RunInLoop(SendTask&& send_task);

private:
    void WaitEpollFd();
    void HandleNewConnection();
    void HandleMessage(int fd);

    int CreateEpollFd();
    int CreateEventFd();

    void AddEpollReadFd(int fd);
    void DelEpollReadFd(int fd);

    void HandleRead();
    void DoSendTasks();

private:
    int _epfd;
    int _evfd;
    bool _is_looping;

    vector<struct epoll_event> _event_list;
    Acceptor& _acceptor;
    map<int,shared_ptr<TcpConnection>> _cons;
    vector<SendTask> _send_tasks;
    
    TcpConnectionCallback _on_connection;
    TcpConnectionCallback _on_message;
    TcpConnectionCallback _on_close;

    MutexLock _mutex;
};

