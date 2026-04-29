#ifndef __RWLOCK_H__
#define __RWLOCK_H__
#include <semaphore.h>
#include <pthread.h>

class RWLock{
private:
    //Your solution to add more private fields
    pthread_mutex_t mutex;
    pthread_cond_t readers_ok;
    pthread_cond_t writers_ok;

    int active_readers;
    int waiting_writers;
    bool active_writer;
    
public:
    RWLock();
    ~RWLock();
    //Reader
    void startRead();
    void doneRead();
    // Writer
    void startWrite();
    void  doneWrite();
};

#endif
