//
// threads.h - threaded garments for the bloopsaphone
//
#ifndef BLOOPSAPHONE_INTERNAL_H
#define BLOOPSAPHONE_INTERNAL_H

#ifdef _WIN32
#include <windows.h>
typedef CRITICAL_SECTION bloopsalock;
static inline void bloops_lock_init(bloopsalock *lock) {
    InitializeCriticalSection(lock);
}
static inline void bloops_lock_acquire(bloopsalock *lock) {
    EnterCriticalSection(lock);
}
static inline int bloops_lock_try_acquire(bloopsalock *lock) {
    return !!TryEnterCriticalSection(lock);
}
static inline void bloops_lock_release(bloopsalock *lock) {
    LeaveCriticalSection(lock);
}
static inline void bloops_lock_finalize(bloopsalock *lock) {
    DeleteCriticalSection(lock);
}
#else
#include <pthread.h>
#include <errno.h>
typedef pthread_mutex_t bloopsalock;
static inline void bloops_lock_init(bloopsalock *lock) {
    pthread_mutex_init(lock, NULL);
}
static inline void bloops_lock_acquire(bloopsalock *lock) {
    pthread_mutex_lock(lock);
}
static inline int bloops_lock_try_acquire(bloopsalock *lock) {
    return !pthread_mutex_trylock(lock);
}
static inline void bloops_lock_release(bloopsalock *lock) {
    pthread_mutex_unlock(lock);
}
static inline void bloops_lock_finalize(bloopsalock *lock) {
    pthread_mutex_destroy(lock);
}
#endif

#endif
