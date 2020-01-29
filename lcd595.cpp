#include "lcd595.h"

#pragma region PUBLIC

Lcd595::Lcd595(int l, int d, int c)
: _latchPin(l), _dataPin(d), _clockPin(c)
{
  pinMode(_latchPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);
}

void Lcd595::init()
{
  delay(41);
  _writeRegister(CTRL_REGISTER, 0b0011, 5);
  _writeRegister(CTRL_REGISTER, 0b0011, 1);
  _writeRegister(CTRL_REGISTER, 0b0011, 1);
  _writeRegister(CTRL_REGISTER, 0b0010, 0);
  _writeRegister(CTRL_REGISTER, 0b0010, 0);
  _writeRegister(CTRL_REGISTER, 0b1000, 1);
  _writeRegister(CTRL_REGISTER, 0b0000, 0);
  _writeRegister(CTRL_REGISTER, 0b1100, 1);
  _writeRegister(CTRL_REGISTER, 0b0000, 0);
  _writeRegister(CTRL_REGISTER, 0b0001, 1);
  _writeRegister(CTRL_REGISTER, 0b0000, 0);
  _writeRegister(CTRL_REGISTER, 0b0110, 5);
  setBacklight(ON);
  puts("Initializing...", TOP_ROW);
  delay(2000);
}

void Lcd595::sendData(int ms)
{
  digitalWrite(_latchPin, LOW);
  shiftOut(_dataPin, _clockPin, MSBFIRST, _value);
  digitalWrite(_latchPin, HIGH);
  #ifdef DEBUG
  printDebug();
  #endif
  delay(ms);
}

#ifdef DEBUG
void Lcd595::printDebug()
{
  bool currentBit = false;
  for (int i = 7; i > 0; i--)
  {
    currentBit = (_value >> i) & 1;
    Serial.print(currentBit);
  }
  Serial.print(" | 0x");
  Serial.println(_value, HEX);
}
#endif

void Lcd595::setBacklight(bool l)
{
  if (l) _value |= 1 << BLpin;
  else _value &= ~ (1 << BLpin);
  sendData(0);
}

void Lcd595::setCursorBlink(bool b)
{
  _writeRegister(CTRL_REGISTER, 0, 0);
  _writeRegister(CTRL_REGISTER, b ? 0xF : 0xC, 0);
}

void Lcd595::moveCursor(bool row, int col)
{
  _writeRegister(CTRL_REGISTER, row ? 0x8 : 0xC, 0);
  _writeRegister(CTRL_REGISTER, 0, 0);
  for (int i = 0; i < col; i++)
  {
    _writeRegister(CTRL_REGISTER, 0x1, 0);
    _writeRegister(CTRL_REGISTER, 0x4, 0);
  }
}

void Lcd595::clear()
{
  _writeRegister(CTRL_REGISTER, 0, 0);
  _writeRegister(CTRL_REGISTER, 0x01, 0);
}

void Lcd595::createChar(uint8_t location, uint8_t charmap[])
{
  uint8_t cgramadd = 0x40 | (location << 3);
  _writeRegister(CTRL_REGISTER, (cgramadd & 0xF0) >> 4, 0);
  _writeRegister(CTRL_REGISTER, (cgramadd & 0x0F), 0);
  for (int i = 0; i < 8; i++)
  {
    _writeRegister(DATA_REGISTER, (charmap[i] & 0xF0) >> 4, 0);
    _writeRegister(DATA_REGISTER, (charmap[i] & 0x0F), 0);
  }
}

void Lcd595::putc(uint8_t msg, bool row, int col)
{
  /*_writeRegister(CTRL_REGISTER, row ? 0x8 : 0xC, 0);
  _writeRegister(CTRL_REGISTER, 0, 0);
  for (int i = 0; i < col; i++)
  {
    _writeRegister(CTRL_REGISTER, 0x1, 0);
    _writeRegister(CTRL_REGISTER, 0x4, 0);
  }*/
  moveCursor(row, col);
  _writeRegister(DATA_REGISTER, (msg & 0xF0) >> 4, 0);
  _writeRegister(DATA_REGISTER, (msg & 0x0F), 0);
}

void Lcd595::puts(const char *msg, bool row)
{
  char c;
  _writeRegister(CTRL_REGISTER, row ? 0x8 : 0xC, 0);
  _writeRegister(CTRL_REGISTER, 0, 0);
  while (c = *msg)
  {
    _writeRegister(DATA_REGISTER, (c & 0xF0) >> 4, 0);
    _writeRegister(DATA_REGISTER, (c & 0x0F), 0);
    msg++;
  }
}

void Lcd595::write(const char *msg)
{
  clear();
  puts(msg, TOP_ROW);
}

void Lcd595::write(const char *top, const char *bot)
{
  write(top);
  puts(bot, BOT_ROW);
}

#pragma endregion PUBLIC

#pragma region PRIVATE

void Lcd595::_writeRegister(bool RS, uint8_t data, int ms)
{
  if (RS) _value |= 1 << RSpin;
  else _value &= ~ (1 << RSpin);
  sendData(0);
  _value &= ~ 0x78;
  _value |= (1 << Epin);
  _value |= (data << 3);
  sendData(2);
  _value &= ~ (1 << Epin);
  sendData(0);
  _value &= ~ 0x78;
  sendData(0);
  delay(ms);
}

#pragma endregion PRIVATE
