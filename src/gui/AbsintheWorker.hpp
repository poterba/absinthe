
#pragma once

#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>

#include "AbsintheMainWnd.hpp"

class AbsintheMainWnd;

class AbsintheWorker
{
  private:
    AbsintheMainWnd* _mainWnd;
    int _device_count;
    std::string _current_udid;

  public:
    AbsintheWorker(AbsintheMainWnd* main);
    ~AbsintheWorker(void);
    void setUDID(const char* udid);
    char* getUDID(void);
    void DeviceEventCB(const idevice_event_t* event, void* user_data);
    void checkDevice();
    void processStart(void);
    void processStatus(const char* msg, int progress);
    void processFinished(const char* error);
};