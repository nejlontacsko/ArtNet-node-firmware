#ifndef DMX_H
#define DMX_H

#define OUTPUT_COUNT 1

enum DmxMode
{
    DM0 = 256,
    DM1 = 512,
    DM2 = 1024,
    DM3 = 2048
};

class DmxOutput
{
  private:
    DmxMode _mode;
    uint8_t* universes;
    
  public:
    DmxOutput(DmxMode mode);
    ~DmxOutput();
    
    void setMode(DmxMode mode);
    void setChannel(int ch, uint8_t val);
};

#endif