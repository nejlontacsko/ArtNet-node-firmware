#include "hotspot.h"

HotSpot::HotSpot(String ssid, int channel, int rssi, int encryption) : _ssid(ssid), _ch(channel), _rssi(rssi), _enc(encryption) {}

/*const char* HotSpot::toString()
{
    char str[100], enc[20];
    sprintf(str, "%s, Ch:%d (%ddBm) %s", _ssid.c_str(), _ch, _rssi, encToStr());
    return str;
}

void HotSpot::dump()
{
    Serial.println(toString());
}*/

const char* HotSpot::encToStr(int enc)
{
    switch (enc)
    {
        case 2:
            return "WPA / PSK";
        case 4:
            return "WPA2 / PSK";
        case 5:
            return "WEP";
        case 7:
            return "Open";
        case 8:
            return "WPA / WPA2 / PSK";
        default:
            return "Unknown.";
    }
}