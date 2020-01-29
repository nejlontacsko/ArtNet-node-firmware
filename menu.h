#ifndef MENU_H
#define MENU_H

#include <list>
#include <ESP8266WiFi.h>

#include "dmx.h"
#include "lcd595.h"
#include "buttons.h"
#include "hotspot.h"

extern "C" {
#include "lwip/dhcp.h"
}

#define MIN_ASCII 0x20
#define MAX_ASCII 0x7E

#define MIN_PASS_LENGTH 8
#define MAX_PASS_LENGTH 12

using namespace std;

enum MenuState
{
    NO_MENU,
    IPCONF, DHCP, FIXIP, IP_INPUT, MASK_INPUT, REQUEST,
    ACCESS_POINT, DISCONNECT_NO, DISCONNECT_YES, DISCONNECTED, SCAN, CHOOSE_HOTSPOT, PASS_INPUT, CONNECT, SAVE_PASS_YES, SAVE_PASS_NO,
    UNIVERSE_SIZE, DMX256, DMX512, DMX1024, DMX2048, SAVE_SIZE_YES, SAVE_SIZE_NO,
    RESTART, RUSURE_NO, RUSURE_YES, RESET,
    EXIT
};

extern "C" void esp_yield();

void arrow(bool showUp, bool showDown, Lcd595 lcd);

void navigateMenu(MenuState menuState, MenuState *nextMenuState, Button pressed, DmxMode *dmxMode);

void stateMachineMenu(MenuState *menuState, MenuState nextMenuState, Lcd595 lcd);

#endif