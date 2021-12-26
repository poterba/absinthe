
#pragma once

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "AbsintheWorker.hpp"

class AbsintheJailbreaker
{
  private:
    AbsintheWorker* worker;
#ifdef WIN32
    HANDLE thread;
#else
    pthread_t thread;
#endif

  public:
    AbsintheJailbreaker(AbsintheWorker* worker);
    void Start(void);
    void statusCallback(const char* message, int progress);
    void* Entry(void* data);
};