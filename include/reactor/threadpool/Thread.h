#pragma once

#include <pthread.h>
#include <functional>

using std::function;
using ThreadCallback = function<void()>;

class Thread
{
public:
    Thread(ThreadCallback&& cb);
    ~Thread();
    void Create();
    void Join();

private:
    static void *ThreadFunc(void* arg);

private:
    pthread_t _thid;
    bool _is_running;
    ThreadCallback _cb;
};

