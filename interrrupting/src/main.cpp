#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
volatile int count = 0;
volatile int state = HIGH;
volatile int button_number = 0;

void displayMessage()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Button ");
  display.print(button_number, DEC);
  display.print(" pressed");
  display.setCursor(0, 15);
  display.print(count, DEC);
  display.display();
}

volatile char button_pressed = 0;

void loop()
{
  digitalWrite(9, state);
  if (button_pressed)
  {
    char *message;
    sprintf(message, "Button %d pressed", button_number);
    displayMessage();
    button_pressed = 0;
    button_number = 0;
  }
  // put your main code here, to run repeatedly:
}

void interruptTwo()
{
  count++;
  state = HIGH;
  button_pressed = 1;
  button_number = 2;
}

void interruptThree()
{
  count--;
  state = LOW;
  button_pressed = 1;
  button_number = 3;
}

void setup()
{

  pinMode(9, OUTPUT);
  digitalWrite(9, state);

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), interruptTwo, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), interruptThree, FALLING);

  if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    display.setTextSize(1);      
    display.setTextColor(WHITE); 
    display.setCursor(0, 0);     
    display.cp437(true);
    display.display();
  }

  delay(1000);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Ready...");
  display.setCursor(0, 15);
  display.print(count, DEC);
  display.display();
}