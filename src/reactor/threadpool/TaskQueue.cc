#include "../../../include/reactor/threadpool/TaskQueue.h"

TaskQueue::TaskQueue(size_t que_size)
:_que_size(que_size)
,_mutex()
,_notempty(_mutex)
,_notfull(_mutex)
,_is_exit(false)
{

}

TaskQueue::~TaskQueue(){

}

bool TaskQueue::IsEmpty() const{
    return _que.empty();
}

bool TaskQueue::IsFull() const{
    return _que_size == _que.size();
}

void TaskQueue::Push(Task&& task){
    MutexLockGuard autolock(_mutex);

    while(IsFull()) _notfull.Wait();
    _que.push(move(task));
    _notempty.Notify();

    if(!IsEmpty()) _notfull.NotifyAll();
}

Task TaskQueue::Pop(){
    MutexLockGuard autolock(_mutex);

    while(IsEmpty() && !_is_exit) _notempty.Wait();

    if(!_is_exit){
        Task task = _que.front();
        _que.pop();
        _notfull.Notify();
        return task;
    }else{
        return nullptr;
    }
}

void TaskQueue::Wakeup(){
    _is_exit = true;
    _notempty.NotifyAll();
}

