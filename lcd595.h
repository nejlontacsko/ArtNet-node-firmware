#pragma once

#include "Arduino.h"

/*** Uncomment to show DEBUG information on Serial Monitor ***/
//#define DEBUG
/****** ----- ******/

#define TOP_ROW HIGH
#define BOT_ROW LOW

#define DATA_REGISTER HIGH
#define CTRL_REGISTER LOW

#define ON HIGH
#define OFF LOW

#define RSpin 1
#define Epin 2
#define BLpin 7

class Lcd595
{
  public:
    Lcd595(int latchPin, int dataPin, int clockPin);
    void init();
    void sendData(int ms);
    
#ifdef DEBUG
    void printDebug();
#endif
        
    void setBacklight(bool l);
    void setCursorBlink(bool b);
    void moveCursor(bool row, int col);
    void clear();
    
    void createChar(uint8_t location, uint8_t charmap[]);
    
    void putc(uint8_t msg, bool row, int col);
    void puts(const char *msg, bool row);
    
    void write(const char *msg);
    void write(const char *top, const char *bot);
  
  private:
    uint8_t _value;
    
    int _latchPin;
    int _dataPin;
    int _clockPin;
    
    void _writeRegister(bool RS, uint8_t data, int ms);
};
