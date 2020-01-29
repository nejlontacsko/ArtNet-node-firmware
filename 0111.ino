#include "ui.h"
#include "dmx.h"
#include "net.h"
#include "menu.h"
#include "artnet.h"
#include "lcd595.h"
#include "buttons.h"

Lcd595 lcd(IC595_RCK, IC595_SER, IC595_SCK);

Button pressed = NONE, lastPressed = NONE;
bool isProgramMadeNone = false;

State state = INIT, nextState;
MenuState menuState = NO_MENU, nextMenuState;

DmxMode dmxMode = DM0;

char bufTop[16], bufBot[16];

bool tx_out = false;

byte upArrow[]   = { 0x00, 0x04, 0x04, 0x0E, 0x0E, 0x1F, 0x00, 0x00 };
byte downArrow[] = { 0x00, 0x00, 0x1F, 0x0E, 0x0E, 0x04, 0x04, 0x00 };
byte enter[]     = { 0x01, 0x01, 0x05, 0x0D, 0x1F, 0x0C, 0x04, 0x00 };
byte signalR[]   = { 0x02, 0x09, 0x05, 0x15, 0x15, 0x05, 0x09, 0x02 };
byte signalL[]   = { 0x08, 0x12, 0x14, 0x15, 0x15, 0x14, 0x12, 0x08 };
byte ap[]        = { 0x00, 0x00, 0x0E, 0x0E, 0x0E, 0x04, 0x04, 0x04 };

ArtNet artnet(ArtNetUdpPort);

//IPAddress staticIp1(192, 168, 1, 251), subnetMask1(255, 255, 255, 0), gateway1(192, 168, 1, 1);

void setup() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  
  pinMode(TX, OUTPUT);
  
  digitalWrite(LED_R, HIGH);
  
  lcd.init();
  lcd.clear();
  
  lcd.createChar(0, upArrow);
  lcd.createChar(1, downArrow);
  lcd.createChar(2, enter);
  lcd.createChar(3, signalL);
  lcd.createChar(4, signalR);
  lcd.createChar(5, ap);
  
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  wifi_set_sleep_type(NONE_SLEEP_T);
  WiFi.setAutoReconnect(true);
  WiFi.begin();
  //WiFi.config(staticIp1, gateway1, subnetMask1);
  //WiFi.begin("www.djfody.hu_450", "11783360");
  
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, HIGH);
}

void loop() {
  int button = analogRead(A0);
  if (button < 200)
  {
    digitalWrite(LED_R, LOW);
    pressed = NONE;
    isProgramMadeNone = false;
  }
  else
  {
    digitalWrite(LED_R, HIGH);
    if (button < 240) pressed = MENU;
      else if (button < 480) pressed = ENTER;
      else if (button < 700) pressed = UP;
      else pressed = DOWN;
  }
  if (lastPressed == pressed && pressed != NONE || isProgramMadeNone)
  {
      pressed = NONE;
      isProgramMadeNone = true;
  }
  lastPressed = pressed;
  
  navigateUi(state, &nextState, pressed);
  navigateMenu(menuState, &nextMenuState, pressed, &dmxMode);
  
  if (state == OPEN_MENU && menuState == NO_MENU)
  {
    delay(750);
    nextMenuState = IPCONF;
    nextState = MENU_ACTIVE;
  }
  if (state == MENU_ACTIVE && menuState == NO_MENU)
      nextState = READY; 
  
  stateMachineUi(&state, nextState, lcd, dmxMode);
  stateMachineMenu(&menuState, nextMenuState, lcd);
  
  digitalWrite(LED_G, artnet.receive(lcd));
  
  if (pressed == ENTER)
      digitalWrite(TX, tx_out = !tx_out);
  
  delay(2);
}