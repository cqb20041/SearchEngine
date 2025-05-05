#include "../../../include/reactor/tcpserver/EventLoop.h"
#include "../../../include/mylogger/MyLogger.h"

#include <unistd.h>
#include <sys/eventfd.h>

EventLoop::EventLoop(Acceptor& acceptor)
:_epfd(CreateEpollFd())
,_evfd(CreateEventFd())
,_is_looping(false)
,_event_list(1024)
,_acceptor(acceptor)
{
    AddEpollReadFd(_acceptor.fd());
    AddEpollReadFd(_evfd);
}

EventLoop::~EventLoop(){
    if(_epfd) close(_epfd);
    if(_evfd) close(_evfd);
}

void EventLoop::Loop(){
    _is_looping = true;
    while(_is_looping) WaitEpollFd();
}

void EventLoop::UnLoop(){
    _is_looping = false;
}

void EventLoop::SetNewConnectionCallback(TcpConnectionCallback&& cb){
    _on_connection = move(cb);
}

void EventLoop::SetMessageCallback(TcpConnectionCallback&& cb){
    _on_message = move(cb);
}

void EventLoop::SetCloseCallback(TcpConnectionCallback&& cb){
    _on_close = move(cb);
}

void EventLoop::WakeupEvfd(){
    uint64_t one = 1;
    int ret = write(_evfd,&one,sizeof(one));
    if(ret != sizeof(one)){
        LogError("Wakeup failed");
    }
}

void EventLoop::RunInLoop(SendTask&& send_task){
    MutexLockGuard autolock(_mutex);
    _send_tasks.push_back(move(send_task));
    WakeupEvfd();
}

void EventLoop::WaitEpollFd(){
    int nready = 
        epoll_wait(_epfd,&_event_list[0],_event_list.size(),5000);
    while(-1 == nready && errno == EINTR){
        nready = epoll_wait(_epfd,&_event_list[0],_event_list.size(),5000);
    }
    if(-1 == nready){
        LogError("WaitEpollFd failed");
    }
    else if(0 == nready){
        LogInfo("WaitEpollFd: timeout");
    }
    else{
        if(nready == int(_event_list.size())){
            _event_list.resize(2 * nready);
        }
        for(int idx = 0;idx < nready;++idx){
            int fd = _event_list[idx].data.fd;

            if(fd == _acceptor.fd()){
                if(_event_list[idx].events & EPOLLIN){
                    HandleNewConnection();
                }
            }
            else if(fd == _evfd){
                if(_event_list[idx].events & EPOLLIN){
                    HandleRead();
                    DoSendTasks();
                }
            }
            else{
                if(_event_list[idx].events & EPOLLIN){
                    HandleMessage(fd);
                }
            }
        }
    }
}

void EventLoop::HandleNewConnection(){
    int peerfd = _acceptor.Accept();
    if(-1 == peerfd){
        LogError("HandleNewConnection failed");
    }
    AddEpollReadFd(peerfd);
    TcpConnectionPtr con(new TcpConnection(peerfd,this));

    con->SetNewConnectionCallback(_on_connection);
    con->SetMessageCallback(_on_message);
    con->SetCloseCallback(_on_close);

    _cons.insert(make_pair(peerfd,con));
    con->HandleNewConnectionCallback();
}

void EventLoop::HandleMessage(int fd){
    auto it = _cons.find(fd);
    if(_cons.end() != it){
        bool flag = it->second->IsClosed();
        if(flag){
            it->second->HandleCloseCallback();
            DelEpollReadFd(fd);
            _cons.erase(it);
        }
        else{
            it->second->HandleMessageCallback();
        }
    }
    else{
        LogError("HandleMessage failed");
    }
}   

int EventLoop::CreateEpollFd(){
    int fd = ::epoll_create1(0);
    if(-1 == fd){
        LogError("CreateEpollFd failed");
    }
    return fd;
}

int EventLoop::CreateEventFd(){
    int fd = eventfd(0,0);
    if(-1 == fd){
        LogError("CreateEventFd failed");
        return -1;
    }
    return fd;
}

void EventLoop::AddEpollReadFd(int fd){
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;

    int ret = epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&event);
    if(-1 == ret){
        LogError("AddEpollReadFd failed");
    }
}

void EventLoop::DelEpollReadFd(int fd){
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;

    int ret = epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&event);
    if(-1 == ret){
        LogError("DelEpollReadFd failed");
    }
}

void EventLoop::HandleRead(){
    uint64_t one = 1;
    int ret = read(_evfd,&one,sizeof(one));
    if(-1 == ret){
        LogError("HandleRead failed");
    }
}

void EventLoop::DoSendTasks(){
    vector<SendTask> temp;
    MutexLockGuard autolock(_mutex);
    temp.swap(_send_tasks);
    for(auto& send_task:temp){
        send_task();
    }
}
