#include "../../../include/reactor/threadpool/ThreadPool.h"
#include "../../../include/mylogger/MyLogger.h"

#include <unistd.h>

using std::bind;

ThreadPool::ThreadPool(size_t thread_num,size_t que_size)
:_thread_num(thread_num)
,_que_size(que_size)
,_task_queue(que_size)
,_is_exit(false)
{
    _threads.reserve(_thread_num);
}

ThreadPool::~ThreadPool(){
    Stop();
}

void ThreadPool::Start(){
    for(size_t idx = 0;idx < _thread_num;idx++){
        unique_ptr<Thread> pthread(new Thread(bind(&ThreadPool::DoTask,this)));
        _threads.push_back(move(pthread));
    }

    for(auto& pthread:_threads){
        pthread->Create();
    }
}

void ThreadPool::Stop(){
    while(!_task_queue.IsEmpty()){
        sleep(1);
    }
    _is_exit = true;
    _task_queue.Wakeup();

    for(auto& pthread:_threads){
        pthread->Join();
    }
}

void ThreadPool::AddTask(Task&& task){
    if(task){
        _task_queue.Push(move(task));
    }
}

Task ThreadPool::GetTask(){
    return _task_queue.Pop();
}

void ThreadPool::DoTask(){
    while(!_is_exit){
        Task task = GetTask();
        if(task){
            task();
        }
    }
}
