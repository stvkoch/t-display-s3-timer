// ST7789 using 8-bit Parallel

#define USER_SETUP_ID 206

#define ST7789_DRIVER
#define INIT_SEQUENCE_3 // Using this initialisation sequence improves the display image

#define CGRAM_OFFSET
#define TFT_RGB_ORDER TFT_RGB // Colour order Red-Green-Blue
// #define TFT_RGB_ORDER TFT_BGR // Colour order Blue-Green-Red

#define TFT_INVERSION_ON
// #define TFT_INVERSION_OFF

#define TFT_PARALLEL_8_BIT

#define TFT_WIDTH 170
#define TFT_HEIGHT 320

#define TFT_CS 6
#define TFT_DC 7
#define TFT_RST 5

#define TFT_WR 8
#define TFT_RD 9
#define TFT_D0 39
#define TFT_D1 40
#define TFT_D2 41
#define TFT_D3 42
#define TFT_D4 45
#define TFT_D5 46
#define TFT_D6 47
#define TFT_D7 48
#define TFT_BL 38
#define TFT_BACKLIGHT_ON HIGH
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

#define MENU_MODE_INACTIVE 0
#define MENU_MODE_ACTIVE 1
#define MENU_MODE_EDIT 2
#define MENU_ITEM_INTERVAL 1
#define MENU_ITEM_DURATION 2

#define MAX_INTERVAL 24
#define DEFAULT_INTERVAL 8

#define MAX_DURATION 60
#define DEFAULT_DURATION 3

#define DEBUG true
#ifdef DEBUG
#define MINUTE_S 2
#define HOUR_S 2
#endif
#ifndef DEBUG
#define MINUTE_S 60
#define HOUR_S 3600
#endif

// pallete THEME colors
// https://ankiewicz.com/color/palette/487f7f
#define C1 0x4bef
#define C2 0x75d7
#define C3 0xaedb
#define C4 0x832c
#define C5 0xd617
#define C6 0xef2f
#define C7 0xffbb

#define DEBUG false

#include "Countimer.h"
#include <PushButtonTaps.h>
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>

TFT_eSPI tft = TFT_eSPI();

PushButtonTaps pushBtn;

const int buttonPin = 14;
const int relayPin = 1;
int menuItem = 0;
int menuMode = MENU_MODE_INACTIVE;
int interval = DEFAULT_INTERVAL; // 8
int duration = DEFAULT_DURATION; // 3
int counter = 0;
Countimer timer;
bool status = 0;
bool screenState = HIGH;
float progressBarFill;

void setup()
{
  pushBtn.setButtonPin(buttonPin);
  pinMode(relayPin, OUTPUT);
  timer.setInterval(refreshClock, 1000);
  tft.begin();
  tft.setRotation(3);
  refreshClock();
  tft.fillScreen(C1);
  draw();
}

void refreshClock()
{
  // 1s
  counter++;
  // calculate the progress bar
  if (status)
  {
    progressBarFill = ((float)(counter / HOUR_S) / (float)duration) * 10;
  }
  else
  {
    progressBarFill = ((float)(counter / MINUTE_S) / (float)interval) * 10;
  }
  // clock
  if (counter % 2 == 0)
  {
    tft.fillCircle(20, 143, 7, C3);
  }
  else
  {
    tft.fillCircle(20, 143, 7, TFT_WHITE);
  }
  // progress bar
  for (int i = 1; i < 10; i++)
  {
    if (progressBarFill > i)
    {
      tft.fillCircle(20 + (i * 18), 143, 7, C3);
    }
    else
    {
      tft.fillCircle(20 + (i * 18), 143, 7, C2);
    }
  }
}

void toggleScreenOnOff()
{
  screenState = !screenState;
  digitalWrite(38, screenState);
}

void disable()
{
  if (menuMode == MENU_MODE_INACTIVE)
  {
    toggleScreenOnOff();
  }
  else
  {
    menuMode = MENU_MODE_INACTIVE;
    menuItem = 0;
  }
}

void draw()
{
  tft.fillScreen(C1);
  tft.fillRoundRect(10, 10, 300, 110, 4, C2);

  if (menuItem == 1)
  {
    if (menuMode == MENU_MODE_EDIT)
    {
      tft.fillRoundRect(30, 30, 100, 60, 4, TFT_RED);
      tft.setTextColor(TFT_BLACK, TFT_RED);
    }
    else
    {
      tft.fillRoundRect(30, 30, 100, 60, 4, TFT_WHITE);
      tft.setTextColor(C1, TFT_WHITE);
    }
  }
  else
  {
    tft.fillRoundRect(30, 30, 100, 60, 4, C1);
    tft.setTextColor(C2, C1);
  }
  tft.drawNumber(interval, 68, 40, 6);
  if (menuItem == 1)
  {
    if (menuMode == MENU_MODE_EDIT)
    {
      tft.setTextColor(TFT_RED, C2);
    }
    else
    {
      tft.setTextColor(TFT_WHITE, C2);
    }
  }
  else
  {
    tft.setTextColor(C1, C2);
  }
  tft.drawString("Interval (h)", 40, 90, 2);

  if (menuItem == 2)
  {
    if (menuMode == MENU_MODE_EDIT)
    {
      tft.fillRoundRect(190, 30, 100, 60, 4, TFT_RED);
      tft.setTextColor(TFT_BLACK, TFT_RED);
    }
    else
    {
      tft.fillRoundRect(190, 30, 100, 60, 4, TFT_WHITE);
      tft.setTextColor(C1, TFT_WHITE);
    }
  }
  else
  {
    tft.fillRoundRect(190, 30, 100, 60, 4, C1);
    tft.setTextColor(C2, C1);
  }
  tft.drawNumber(duration, 228, 40, 6);
  if (menuItem == 2)
  {
    if (menuMode == MENU_MODE_EDIT)
    {
      tft.setTextColor(TFT_RED, C2);
    }
    else
    {
      tft.setTextColor(TFT_WHITE, C2);
    }
  }
  else
  {
    tft.setTextColor(C1, C2);
  }
  tft.drawString("Duration (m)", 200, 90, 2);

  tft.setCursor(260, 135, 4);
  if (status)
  {
    tft.setTextColor(C6);
    tft.print("On");
  }
  else
  {
    tft.setTextColor(C7);
    tft.print("Off");
  }
}

void turnOn()
{
  status = 1;
  counter = 0;
  digitalWrite(relayPin, HIGH);
}

void turnOff()
{
  status = 0;
  counter = 0;
  digitalWrite(relayPin, LOW);
}

void loop()
{
  byte tap = pushBtn.checkButtonStatus();
  if (tap != 0)
  {
    switch (tap)
    {
    case 1:
      if (menuMode == MENU_MODE_ACTIVE)
      {
        menuItem++;
      }
      else if (menuMode == MENU_MODE_EDIT)
      {
        if (menuItem == MENU_ITEM_INTERVAL)
        {
          interval = interval + 1;
          if (interval > MAX_INTERVAL)
          {
            interval = 1;
          }
        }
        if (menuItem == MENU_ITEM_DURATION)
        {
          duration = duration + 1;
          if (duration > MAX_DURATION)
          {
            duration = 1;
          }
        }
      }
      else
      {
        if (!screenState)
        {
          toggleScreenOnOff();
        }
      }
      break;
    case 2:
      disable();
      break;
    case 3:
      if (menuMode == MENU_MODE_INACTIVE)
      {
        menuMode = MENU_MODE_ACTIVE;
        menuItem = 1;
      }
      else if (menuMode == MENU_MODE_ACTIVE)
      {
        menuMode = MENU_MODE_EDIT;
      }
      else
      {

        menuItem = 0;
        menuMode = MENU_MODE_INACTIVE;
      }
    }
    draw();
  }

  if (menuItem > 2)
  {
    menuItem = 1;
    draw();
  }
  timer.run();
  if (!timer.isCounterCompleted())
  {
    timer.start();
  }

  if (!status && counter / HOUR_S >= interval)
  {
    turnOn();
    draw();
  }
  if (status && counter / MINUTE_S >= duration)
  {
    turnOff();
    draw();
  }
}
