#ifndef ARTNET_H
#define ARTNET_H

#include <WiFiUDP.h>
#include <string>

#include "lcd595.h"
#include "opcodes.h"

#define ArtNetUdpPort 0x1936

#define OpCodeHigh 9
#define OpCodeLow 8

class ArtNet
{
private:
    WiFiUDP udp;
    byte incomingPacket[2114];
    
public:
    const std::string reference = "Art-Net\0";

    ArtNet(uint32_t port);
    //~ArtNet();
    
    bool receive(Lcd595 lcd);
};

#endif