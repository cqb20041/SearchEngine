#include "../../include/reactor/MutexLock.h"
#include "../../include/mylogger/MyLogger.h"

MutexLock::MutexLock(){
    int ret = pthread_mutex_init(&_mutex,nullptr);
    if(ret < 0){
        LogError("Pthread_mutex_init failed");
    }
}

MutexLock::~MutexLock(){
    int ret = pthread_mutex_destroy(&_mutex);
    if(ret < 0){
        LogError("Pthread_mutex_destroy failed");
    }
}

void MutexLock::Lock(){
    int ret = pthread_mutex_lock(&_mutex);
    if(ret < 0){
        LogError("Pthread_mutex_lock failed");
    }
}

void MutexLock::UnLock(){
    int ret = pthread_mutex_unlock(&_mutex);
    if(ret < 0){
        LogError("Pthread_mutex_unlock failed");
    }
}

void MutexLock::TryLock(){
    int ret = pthread_mutex_trylock(&_mutex);
    if(ret < 0){
        LogError("Pthread_mutex_trylock failed");
    }
}

pthread_mutex_t* MutexLock::GetMutexLockPtr(){
    return &_mutex;
}


MutexLockGuard::MutexLockGuard(MutexLock& mutex)
:_mutex(mutex)
{
    _mutex.Lock();
}

MutexLockGuard::~MutexLockGuard(){
    _mutex.UnLock();
}
