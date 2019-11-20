#include <Arduino.h>
#include <SPI.h>              // include SPI library
#include <Adafruit_GFX.h>     // include adafruit graphics library
#include <Adafruit_PCD8544.h> // include adafruit PCD8544 (Nokia 5110) library

// source https://simple-circuit.com/esp8266-nodemcu-nokia-5110-lcd/
// Nokia 5110 LCD module connections (CLK, DIN, D/C, CS, RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(D4, D3, D2, D1, D0);

#define ENCODER_CLOCK D7
#define ENCODER_DT D6
#define ENCODER_SWITCH D5

char clock_state;
char click_state = 1;
int value;

void showState(char switch_clicked) {
  display.clearDisplay();
  display.println(value, DEC);
  if (switch_clicked) {
    display.print("CLICKED!");
  }
  display.display();
}

void setup()
{
  display.begin();
  display.setContrast(22);
  display.clearDisplay();
  display.setTextColor(BLACK);
  display.setTextSize(1);

  display.setRotation(2);
  
  display.drawRect(0, 0, display.width() - 1, display.height() - 1, BLACK);
  display.setCursor(2, 2);
  display.print("Here we go!");
  display.display();

  pinMode(ENCODER_CLOCK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(ENCODER_SWITCH, INPUT_PULLUP);
}

void loop()
{
  char showResult = 0;
  char switch_clicked = !digitalRead(ENCODER_SWITCH);
  if (click_state != switch_clicked) {
    showResult = 1;
    click_state = !click_state;
  }

  if (digitalRead(ENCODER_CLOCK) != clock_state) {
    if (digitalRead(ENCODER_DT) == clock_state) {
      value++;
    } else {
      value--;
    }
    clock_state = !clock_state;
    showResult = 1;
  }

  if (showResult) {
    showState(switch_clicked);
  }
}