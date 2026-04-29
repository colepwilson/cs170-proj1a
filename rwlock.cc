#include<stdio.h>
#include <iostream> 


#include "rwlock.h"

//Your solution to implement each of the following methods
//
RWLock::RWLock() : active_readers(0), waiting_writers(0), active_writer(false) {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&readers_ok, NULL);
    pthread_cond_init(&writers_ok, NULL);
}

RWLock::~RWLock() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&readers_ok);
    pthread_cond_destroy(&writers_ok);
}

void RWLock::startRead() {
    pthread_mutex_lock(&mutex);
    while (active_writer || waiting_writers > 0) {
        pthread_cond_wait(&readers_ok, &mutex);
    }
    ++active_readers;
    pthread_mutex_unlock(&mutex);
}

void RWLock::doneRead() {
    pthread_mutex_lock(&mutex);
    --active_readers;
    if (active_readers == 0) {
        pthread_cond_signal(&writers_ok);
    }
    pthread_mutex_unlock(&mutex);
}

void RWLock::startWrite() {
    pthread_mutex_lock(&mutex);
    ++waiting_writers;
    while (active_writer || active_readers > 0) {
        pthread_cond_wait(&writers_ok, &mutex);
    }
    --waiting_writers;
    active_writer = true;
    pthread_mutex_unlock(&mutex);
}

void RWLock::doneWrite() {
    pthread_mutex_lock(&mutex);
    active_writer = false;
    if (waiting_writers > 0) {
        pthread_cond_signal(&writers_ok);
    } else {
        pthread_cond_broadcast(&readers_ok);
    }
    pthread_mutex_unlock(&mutex);
}
