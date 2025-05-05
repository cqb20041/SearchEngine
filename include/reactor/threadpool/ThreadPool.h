#pragma once

#include "Thread.h"
#include "TaskQueue.h"

#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

class ThreadPool
{
public:
    ThreadPool(size_t thread_num,size_t que_size);
    ~ThreadPool();
    void Start();
    void Stop();
    void AddTask(Task&& task);

private:
    Task GetTask();
    void DoTask();

private:
    size_t _thread_num;
    size_t _que_size;
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _task_queue;
    bool _is_exit;
};

