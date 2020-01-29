#ifndef UI_H
#define UI_H

#include <ESP8266WiFi.h>

#include "dmx.h"
#include "lcd595.h"
#include "buttons.h"

enum State
{
    INIT,
    READY, READY_BUT_NO_UI,
    SHOW_IP, SHOW_MASK,
    SHOW_DMX_MODE,
    OPEN_MENU,
    MENU_ACTIVE
};

void navigateUi(State state, State *nextState, Button pressed);

void stateMachineUi(State *state, State nextState, Lcd595 lcd, DmxMode dmxMode);

#endif