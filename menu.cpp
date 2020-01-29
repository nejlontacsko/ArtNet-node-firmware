#include "menu.h"

vector<HotSpot> hotspots;
int hotspot_count, selected = 0, nextSelected = 0, ipPos = 1, passPos = 1;
IPAddress staticIp(192, 168, 1, 250), subnetMask(255, 255, 255, 0), gateway(0, 0, 0, 0);
char buf[17] = "\0", pass[13] = "********\0";
bool doneIp = false, ipUpdate = false, donePass = false, passUpdate = false, wantDhcp = true;

void arrow(bool showUp, bool showDown, Lcd595 lcd)
{
    if (showUp)
        lcd.putc(0, TOP_ROW, 15);
    if (showDown)
        lcd.putc(1, BOT_ROW, 15);
}

void navigateMenu(MenuState menuState, MenuState *nextMenuState, Button pressed, DmxMode *dmxMode)
{
  switch (menuState)
  {
    #pragma region MenuItem0
    case IPCONF:
      switch (pressed)
      {
          case ENTER: *nextMenuState = DHCP;         break;
          case DOWN:  *nextMenuState = ACCESS_POINT; break;
          case MENU:  *nextMenuState = NO_MENU;      break;
          default: break;
      }
      break;
    case DHCP:
      switch (pressed)
      {
          case ENTER:
            *nextMenuState = REQUEST;
            wantDhcp = true;
            break;
          case DOWN:  *nextMenuState = FIXIP;   break;
          case MENU:  *nextMenuState = IPCONF;  break;
          default: break;
      }
      break;
    case FIXIP:
      switch (pressed)
      {
          case ENTER:
            *nextMenuState = IP_INPUT;
            wantDhcp = false;
            break;
          case UP:    *nextMenuState = DHCP;     break;
          case MENU:  *nextMenuState = IPCONF;   break;
          default: break;
      }
      break;
    case IP_INPUT:
      ipUpdate = pressed != NONE;
      switch (pressed)
      {
          case ENTER:
            ipPos++;
            if (doneIp)
            {
              *nextMenuState = MASK_INPUT;
              doneIp = false;
              ipPos = 1;
              ipUpdate = false;
              gateway = staticIp;
              gateway[3] = 1;
            }
            break;
          case UP:
            staticIp[ipPos - 1] = (staticIp[ipPos - 1] + 1) > 255 ? 255 : (staticIp[ipPos - 1] + 1);
            break;
          case DOWN:
            staticIp[ipPos - 1] = (staticIp[ipPos - 1] - 1) < 0 ? 0 : (staticIp[ipPos - 1] - 1);
            break;
          case MENU:
            if (ipPos == 1)
            {
              *nextMenuState = FIXIP;
              ipUpdate = false;
            }
            else
              ipPos--;
      }
      break;
    case MASK_INPUT:
      ipUpdate = pressed != NONE;
      switch (pressed)
      {
          case ENTER:
            ipPos++;
            if (doneIp)
            {
              *nextMenuState = REQUEST;
              doneIp = false;
              ipPos = 1;
              ipUpdate = false;
            }
            break;
          case UP:
            subnetMask[ipPos - 1] = (subnetMask[ipPos - 1] + 1) > 255 ? 255 : (subnetMask[ipPos - 1] + 1);
            break;
          case DOWN:
            subnetMask[ipPos - 1] = (subnetMask[ipPos - 1] - 1) < 0 ? 0 : (subnetMask[ipPos - 1] - 1);
            break;
          case MENU:
            if (ipPos == 1)
            {
              *nextMenuState = IP_INPUT;
              ipUpdate = false;
            }
            else
              ipPos--;
      }
      break;
    case REQUEST:
      if (pressed == ENTER)
          *nextMenuState = IPCONF;
      break;
    #pragma endregion MenuItem0
    #pragma region MenuItem1
    case ACCESS_POINT:
      switch (pressed)
      {
          case ENTER:
            if (WiFi.status() == WL_CONNECTED)
              *nextMenuState = DISCONNECT_NO;
            else
              *nextMenuState = SCAN;
          break;
          case UP:    *nextMenuState = IPCONF;        break;
          case DOWN:  *nextMenuState = UNIVERSE_SIZE; break;
          case MENU:  *nextMenuState = NO_MENU;       break;
      }
      break;
    case DISCONNECT_NO:
      switch (pressed)
      {
        case ENTER: *nextMenuState = ACCESS_POINT;   break;
        case DOWN:  *nextMenuState = DISCONNECT_YES; break;
        case MENU:  *nextMenuState = ACCESS_POINT;   break;
        default: break;
      }
      break;
    case DISCONNECT_YES:
      switch (pressed)
      {
        case ENTER: *nextMenuState = DISCONNECTED;  break;
        case UP:    *nextMenuState = DISCONNECT_NO; break;
        case MENU:  *nextMenuState = ACCESS_POINT;  break;
        default: break;
      }
      break;
    case DISCONNECTED:
      switch (pressed)
      {
        case ENTER: *nextMenuState = SCAN;         break;
        case MENU:  *nextMenuState = ACCESS_POINT; break;
        default: break;
      }
      break;
    case SCAN:
      if (pressed == ENTER)
        *nextMenuState = CHOOSE_HOTSPOT;
      break;
    case CHOOSE_HOTSPOT:
      switch (pressed)
      {
          case ENTER: *nextMenuState = PASS_INPUT;                                                    break;
          case UP: 
          nextSelected = (selected - 1) < 0 ? 0 : (selected - 1);                                     break;
          case DOWN: 
          nextSelected = (selected + 1) > (hotspot_count - 1) ? (hotspot_count - 1) : (selected + 1); break;
          case MENU:  nextSelected = -1; *nextMenuState = ACCESS_POINT;                               break;
      }
      break;
    case PASS_INPUT:
      passUpdate = pressed != NONE;
      switch (pressed)
      {
          case ENTER:
            if(pass[passPos - 1] == 0x7F)
            {
              pass[passPos - 1] = '\0';
              donePass = true;
            }
            passPos++;
            if (donePass)
            {
              *nextMenuState = CONNECT;
              donePass = false;
              passPos = 1;
              passUpdate = false;
            }
            break;
          case UP:
            pass[passPos - 1] = (pass[passPos - 1] + 1) > MAX_ASCII + 1 ? MIN_ASCII : (pass[passPos - 1] + 1);
            break;
          case DOWN:
            pass[passPos - 1] = (pass[passPos - 1] - 1) < MIN_ASCII ? MAX_ASCII + 1 : (pass[passPos - 1] - 1);
            break;
          case MENU:
            if (passPos == 1)
            {
              *nextMenuState = CHOOSE_HOTSPOT;
              passUpdate = false;
            }
            else
              passPos--;
      }
      break;
    case CONNECT:
      if (pressed == ENTER && WiFi.status() == WL_CONNECTED)
        *nextMenuState = SAVE_PASS_YES;
      else if (pressed == MENU)
        *nextMenuState = ACCESS_POINT;
      break;
    case SAVE_PASS_YES: //TODO!!
      //store to EEPROM or SPIFFS?
      if (pressed == ENTER)
        *nextMenuState = ACCESS_POINT;
      break;
    case SAVE_PASS_NO:
      if (pressed == ENTER)
        *nextMenuState = ACCESS_POINT;
      break;
    #pragma endregion MenuItem1
    #pragma region MenuItem2
    case UNIVERSE_SIZE:
      switch (pressed)
      {
        case ENTER: *nextMenuState = DMX256;        break;
        case UP:    *nextMenuState = ACCESS_POINT;  break;
        case DOWN:  *nextMenuState = RESTART;       break;
        case MENU:  *nextMenuState = NO_MENU;       break;
      }
      break;
    case DMX256:
      switch (pressed)
      {
        case ENTER:
          *nextMenuState = SAVE_SIZE_YES;
          *dmxMode = DM0;
        break;
        case DOWN:  *nextMenuState = DMX512;        break;
        case MENU:  *nextMenuState = UNIVERSE_SIZE; break;
        default: break;
      }
      break;
    case DMX512:
      switch (pressed)
      {
        case ENTER:
          *nextMenuState = SAVE_SIZE_YES;
          *dmxMode = DM1;
        break;
        case UP:    *nextMenuState = DMX256;        break;
        case DOWN:  *nextMenuState = DMX1024;       break;
        case MENU:  *nextMenuState = UNIVERSE_SIZE; break;
      }
      break;
    case DMX1024:
      switch (pressed)
      {
        case ENTER:
          *nextMenuState = SAVE_SIZE_YES;
          *dmxMode = DM2;
          break;
        case UP:    *nextMenuState = DMX512;        break;
        case DOWN:  *nextMenuState = DMX2048;       break;
        case MENU:  *nextMenuState = UNIVERSE_SIZE; break;
      }
      break;
    case DMX2048:
      switch (pressed)
      {
        case ENTER:
          *nextMenuState = SAVE_SIZE_YES;
          *dmxMode = DM3;
          break;
        case UP:    *nextMenuState = DMX1024;       break;
        case MENU:  *nextMenuState = UNIVERSE_SIZE; break;
        default: break;
      }
      break;
    case SAVE_SIZE_YES: //TODO!!
      switch (pressed)
      {
        case ENTER: *nextMenuState = UNIVERSE_SIZE; break;
        case DOWN:  *nextMenuState = SAVE_SIZE_NO;  break;
        default: break;
      }
      break;
    case SAVE_SIZE_NO:
      switch (pressed)
      {
        case ENTER: *nextMenuState = UNIVERSE_SIZE; break;
        case UP:    *nextMenuState = SAVE_SIZE_YES; break;
        default: break;
      }
      break;
    #pragma endregion MenuItem2
    #pragma region MenuItem3
    case RESTART:
      switch (pressed)
      {
        case ENTER: *nextMenuState = RUSURE_NO;     break;
        case UP:    *nextMenuState = UNIVERSE_SIZE; break;
        case DOWN:  *nextMenuState = EXIT;          break;
        case MENU:  *nextMenuState = NO_MENU;       break;
      }
      break;
    case RUSURE_NO: //TODO!!
      switch (pressed)
      {
        case ENTER: *nextMenuState = RESTART;    break;
        case DOWN:  *nextMenuState = RUSURE_YES; break;
        case MENU:  *nextMenuState = RESTART;    break;
        default: break;
      }
      break;
    case RUSURE_YES: //TODO!!
      switch (pressed)
      {
        case ENTER: *nextMenuState = RESET;     break;
        case UP:    *nextMenuState = RUSURE_NO; break;
        case MENU:  *nextMenuState = RESTART;   break;
        default: break;
      }
      break;
    #pragma endregion MenuItem3
    #pragma region MenuItem4
    case EXIT:
      switch (pressed)
      {
        case ENTER: *nextMenuState = NO_MENU; break;
        case UP:    *nextMenuState = RESTART; break;
        case MENU:  *nextMenuState = NO_MENU; break;
        default: break;
      }
      break;
    default:
      break;
    #pragma endregion MenuItem4
  }
}

void stateMachineMenu(MenuState *menuState, MenuState nextMenuState, Lcd595 lcd)
{
  if (*menuState != nextMenuState ||
       selected != nextSelected && *menuState == CHOOSE_HOTSPOT ||
       ipUpdate || passUpdate)
  {
    
    *menuState = nextMenuState;
    selected = nextSelected;
    
    switch (*menuState)
    {
    #pragma region MenuItem0
    case IPCONF:
      lcd.write("1. IP config");
      arrow(false, true, lcd);
      break;
    case DHCP:
      lcd.write("a) Obtain IP ", " via DHCP");
      arrow(false, true, lcd);
      break;
    case FIXIP:
      lcd.write("b) Set static", " IP address");
      arrow(true, false, lcd);
      break;
    case IP_INPUT:
      lcd.setCursorBlink(true);
      sprintf(buf, "%03d.%03d.%03d.%03d%c",
        staticIp[0], staticIp[1], staticIp[2], staticIp[3], ipPos > 3 ? 2 : ' ');
      lcd.write("New IP:", buf);
      lcd.moveCursor(BOT_ROW, ipPos * 4 - 2);
      if (ipPos > 4)
      {
        lcd.setCursorBlink(false);
        doneIp = true;
        lcd.write("Press ENTER to", " continue!");
      }
      break;
    case MASK_INPUT:
      lcd.setCursorBlink(true);
      sprintf(buf, "%03d.%03d.%03d.%03d%c", 
        subnetMask[0], subnetMask[1], subnetMask[2], subnetMask[3], ipPos > 3 ? 2 : ' ');
      lcd.write("New IP:", buf);
      lcd.moveCursor(BOT_ROW, ipPos * 4 - 2);
      if (ipPos > 4)
      {
        lcd.setCursorBlink(false);
        doneIp = true;
        lcd.write("Press ENTER to", " continue!");
      }
      break;
    case REQUEST:
      lcd.write("Finished. Press ", "  ENTER to exit!");
      break;
    #pragma endregion MenuItem0
    #pragma region MenuItem1
    case ACCESS_POINT:
      lcd.write("2. Connent to", "a WiFi network");
      arrow(true, true, lcd);
      break;
    case DISCONNECT_NO:
      lcd.write("Disconnect", "now?        NO");
      arrow(false, true, lcd);
      break;
    case DISCONNECT_YES:
      lcd.write("Disconnect", "now?       YES");
      arrow(true, false, lcd);
      break;
    case DISCONNECTED:
      WiFi.disconnect();
      lcd.write(" Disconnected,", "  please choose!");
      delay(1250);
      lcd.write("Scan Wifi: ENTER", "or offline: MENU");
      break;
    case SCAN:
      lcd.write("Looking for", "WiFi networks...");

      hotspot_count = WiFi.scanNetworks(false, true);
  
      for (int i = 0; i < hotspot_count; i++)
        hotspots.emplace_back(WiFi.SSID(i), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i));
      
      lcd.write("Press ENTER to", "  show the list!");
      break;
    case CHOOSE_HOTSPOT:
      lcd.setCursorBlink(false);
      if (hotspots.size() > 0)
      {
        lcd.write(hotspots[selected].getSsid().c_str(), hotspots[selected].getEncStr());
        arrow(selected > 0, selected < hotspots.size() - 1, lcd);
      }
      else
        lcd.write("Didn't find any.");
      break;
    case PASS_INPUT:
      lcd.setCursorBlink(true);
      sprintf(buf, "%s%c", pass, passPos > MAX_PASS_LENGTH ? 2 : ' ');
      lcd.write("Password:", buf);
      if (pass[passPos - 1] == MAX_ASCII + 1)
        lcd.putc(2, BOT_ROW, passPos - 1);
      lcd.moveCursor(BOT_ROW, passPos - 1);
      if (passPos > MAX_PASS_LENGTH || donePass)
      {
        lcd.setCursorBlink(false);
        donePass = true;
        lcd.write("Press ENTER to", " continue!");
      }
      break;
    case CONNECT:
      lcd.setCursorBlink(false);
      if (wantDhcp)
        lcd.write("   Connecting", "  with DHCP...");
      else
      {
        WiFi.config(staticIp, gateway, subnetMask);
        lcd.write("   Connecting", " with Fix IP...");
      }
      WiFi.mode(WIFI_STA);
      WiFi.setSleepMode(WIFI_NONE_SLEEP);
      wifi_set_sleep_type(NONE_SLEEP_T);
      WiFi.begin(hotspots[selected].getSsid(), pass);
      switch (WiFi.waitForConnectResult())
      {
        case WL_CONNECTED:
          lcd.write("Finished. Press ", "  ENTER to exit!");
          break;
        case WL_NO_SSID_AVAIL:
          lcd.write("AP cannot be", "    reached!");
          break;
        case WL_CONNECT_FAILED:
          lcd.write("   Incorrect", "   password!");
          break;
        case WL_IDLE_STATUS:
          lcd.write("   WiFi is in", "  IDLE status!");
          delay(2000);
          nextMenuState = RESET;
          break;
        case WL_DISCONNECTED:
          lcd.write(" Module not in station mode");
          delay(2000);
          nextMenuState = RESTART;
          break;
        case -1:
          lcd.write("   Connection", "    timeout!");
          break;
      }
      break;
    break;
    case SAVE_PASS_YES: //TODO!!
      lcd.write("Save pass for", "this AP?   YES");
      arrow(false, true, lcd);
      //store to EEPROM or SPIFFS?
      break;
    case SAVE_PASS_NO:
      lcd.write("Save pass for", "this AP?    NO");
      arrow(true, false, lcd);
      break;
    #pragma endregion MenuItem1
    #pragma region MenuItem2
    case UNIVERSE_SIZE:
      lcd.write("3. Choose DMX", "universe mode");
      arrow(true, true, lcd);
      break;
    case DMX256:
      lcd.write("a) DMX-256");
      arrow(false, true, lcd);
      break;
    case DMX512:
      lcd.write("b) DMX-512");
      arrow(true, true, lcd);
      break;
    case DMX1024:
      lcd.write("c) DMX-1024");
      arrow(true, true, lcd);
      break;
    case DMX2048:
      lcd.write("d) DMX-2048");
      arrow(true, false, lcd);
      break;
    case SAVE_SIZE_YES: //TODO!!
      lcd.write("Save length?", "           YES");
      arrow(false, true, lcd);
      break;
    case SAVE_SIZE_NO:
      lcd.write("Save length?", "            NO");
      arrow(true, false, lcd);
      break;
    #pragma endregion MenuItem2
    #pragma region MenuItem3
    case RESTART:
      lcd.write("4. Restart", "this node");
      arrow(true, true, lcd);
      break;
    case RUSURE_NO: //TODO!!
      lcd.write("Are you sure?", "            NO");
      arrow(false, true, lcd);
      break;
    case RUSURE_YES: //TODO!!
      lcd.write("Are you sure?", "           YES");
      arrow(true, false, lcd);
      break;
    case RESET:
      digitalWrite(16, LOW);
      lcd.write("Restart...");
      system_restart();
      esp_yield();
      break;
    #pragma endregion MenuItem3
    #pragma region MenuItem4
    case EXIT:
      lcd.write("5. Exit");
      arrow(true, false, lcd);
      break;
    default:
      break;
    #pragma endregion MenuItem4
    }
  }
}