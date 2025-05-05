#pragma once

#include "../Condition.h"

#include <functional>
#include <queue>

using std::function;
using std::queue;

using Task = function<void()>; 

class TaskQueue
{
public:
    TaskQueue(size_t que_size);
    ~TaskQueue();
    bool IsEmpty() const;
    bool IsFull() const;
    void Push(Task&& task);
    Task Pop();
    void Wakeup();

private:
    size_t _que_size;
    queue<Task> _que;
    MutexLock _mutex;
    Condition _notempty;
    Condition _notfull;
    bool _is_exit;
};

