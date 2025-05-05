#include "../../include/reactor/Condition.h"

Condition::Condition(MutexLock& mutex)
:_mutex(mutex)
{
    pthread_cond_init(&_cond,nullptr);
}

Condition::~Condition(){
    pthread_cond_destroy(&_cond);
}

void Condition::Wait(){
    pthread_cond_wait(&_cond,_mutex.GetMutexLockPtr());
}

void Condition::Notify(){
    pthread_cond_signal(&_cond);
}

void Condition::NotifyAll(){
    pthread_cond_broadcast(&_cond);
}


