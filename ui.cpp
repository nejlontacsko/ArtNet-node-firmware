#include "ui.h"

char uiBuf[9] = "\0";

void navigateUi(State state, State *nextState, Button pressed)
{
  switch (state)
  {
    case READY:
      switch (pressed)
      {
        case ENTER: *nextState = READY_BUT_NO_UI;       break;
        case UP:    *nextState = SHOW_DMX_MODE;         break;
        case DOWN:  *nextState = SHOW_IP;               break;
        case MENU:  *nextState = OPEN_MENU;             break;
      }
      break;
    case READY_BUT_NO_UI:
      if (pressed == ENTER)
        *nextState = READY;
      else
        *nextState = READY_BUT_NO_UI;
      break;
    case SHOW_IP:
      switch (pressed)
      {
        case ENTER: *nextState = READY_BUT_NO_UI;       break;
        case UP:    *nextState = READY;                 break;
        case DOWN:  *nextState = SHOW_MASK;             break;
        case MENU:  *nextState = OPEN_MENU;             break;
      }
      break;
    case SHOW_MASK:
      switch (pressed)
      {
        case ENTER: *nextState = READY_BUT_NO_UI;       break;
        case UP:    *nextState = SHOW_IP;               break;
        case DOWN:  *nextState = SHOW_DMX_MODE;         break;
        case MENU:  *nextState = OPEN_MENU;             break;
      }
      break;
    case SHOW_DMX_MODE:
      switch (pressed)
      {
        case ENTER: *nextState = READY_BUT_NO_UI;       break;
        case UP:    *nextState = SHOW_MASK;             break;
        case DOWN:  *nextState = READY;                 break;
        case MENU:  *nextState = OPEN_MENU;             break;
      }
      break;
    case OPEN_MENU:
      if (pressed == MENU) 
        *nextState = READY;
      else
        *nextState = OPEN_MENU;
      break;
    case MENU_ACTIVE:
      break;
    default:
      *nextState = READY;
      break;
  }
}

char apOkStr[] = {'R', 'e', 'a', 'd', 'y', '.', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 3, 5, 4, 0};
char apNoStr[] = {'R', 'e', 'a', 'd', 'y', '.', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 0xA5, 5, 0xA5, 0};

void stateMachineUi(State *state, State nextState, Lcd595 lcd, DmxMode dmxMode)
{
  if (*state != nextState)
  {
    *state = nextState;
  
    switch (*state)
    {
      case READY:
        lcd.write(WiFi.status() != WL_CONNECTED ? apNoStr : apOkStr);
        lcd.setBacklight(true);
        break;
      case READY_BUT_NO_UI:
        lcd.setBacklight(false);
        break;
      case SHOW_IP:
        lcd.write("My IP address:", WiFi.localIP().toString().c_str());
        break;
      case SHOW_MASK:
        lcd.write("My subnet mask:", WiFi.subnetMask().toString().c_str());
        break;
      case SHOW_DMX_MODE:
        sprintf(uiBuf, "    DMX-%d", dmxMode);
        lcd.write("DMX mode: ", uiBuf);
        break;
      case OPEN_MENU:
        lcd.write("   M e n u :", "  -----------");
        break;
      case MENU_ACTIVE:
        break;
      default:
        lcd.write("  StateMachine", "     ERROR!");
        break;
    }
  }
}