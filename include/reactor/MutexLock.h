#pragma once

#include "NonCopyable.h"

#include <pthread.h>

class MutexLock
:private NonCopyable
{
public:
    MutexLock();
    ~MutexLock();
    void Lock();
    void UnLock();
    void TryLock();
    pthread_mutex_t* GetMutexLockPtr();

private:
    pthread_mutex_t _mutex;
};

class MutexLockGuard{
public:
    MutexLockGuard(MutexLock& mutex);
    ~MutexLockGuard();

private:
    MutexLock& _mutex;
};
