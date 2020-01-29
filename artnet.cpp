#include "artnet.h"

ArtNet::ArtNet(uint32_t port)
{
    udp.begin(port);
}

bool ArtNet::receive(Lcd595 lcd)
{
  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    int len = udp.read(incomingPacket, 255);
    char proto[8];
    for (int i = 0; i < 8; i++)
      proto[i] = incomingPacket[i];
    if (!reference.compare(proto))
    {
      switch (incomingPacket[OpCodeHigh] << 8 + incomingPacket[OpCodeLow])
      {
        case OpPoll:
          lcd.write("     OpPoll", "    received");
        break;
        case OpCommand:
          lcd.write("   OpCommand", "    received");
        break;
        case OpDmx:
          lcd.write("     OpDmx", "    received");
        break;
        case OpAddress:
          lcd.write("   OpAddress", "    received");
        break;
        case OpInput:
          lcd.write("    OpInput", "    received");
        break;
        case OpIpProg:
          lcd.write("    OpIpProg", "    received");
        break;
      }
      return true;
    }
    return false;
  }
}