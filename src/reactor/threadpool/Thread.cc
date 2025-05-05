#include "../../../include/reactor/threadpool/Thread.h"
#include "../../../include/mylogger/MyLogger.h"

Thread::Thread(ThreadCallback&& cb)
:_thid(0)
,_is_running(false)
,_cb(cb)
{

}

Thread::~Thread(){
    if(_is_running){
        pthread_detach(_thid);
    }
}

void Thread::Create(){
    int ret = pthread_create(&_thid,nullptr,ThreadFunc,(void*)this);
    if(-1 == ret){
        LogError("ThreadCreate failed");
        return;
    }
    _is_running = true;
}

void Thread::Join(){
    if(_is_running){
        pthread_join(_thid,nullptr);
        _is_running = false;
    }
}

void* Thread::ThreadFunc(void* arg){
    Thread* pthread = static_cast<Thread*>(arg);
    if(pthread){
        pthread->_cb();
    }
    pthread_exit(nullptr);
} 
