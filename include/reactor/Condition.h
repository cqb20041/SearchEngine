#pragma once

#include "MutexLock.h"

class Condition
:private NonCopyable
{
public:
    Condition(MutexLock& mutex);
    ~Condition();
    void Wait();
    void Notify();
    void NotifyAll();

private:
    MutexLock& _mutex;
    pthread_cond_t _cond;
};

