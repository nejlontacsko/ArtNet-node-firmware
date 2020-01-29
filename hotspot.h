#ifndef HOTSPOT_H
#define HOTSPOT_H

#include <HardwareSerial.h>
#include <stdio.h>

class HotSpot
{
  private:
    String _ssid;
    int _ch;
    int _rssi;
    int _enc;
    
  public:
    static const char* encToStr(int enc);
    
    int getEnc() { return _enc; }
    const char* getEncStr() { return encToStr(getEnc()); }
    const String& getSsid() { return _ssid; }
    
    HotSpot(String ssid, int channel, int rssi, int encryption);
};

#endif