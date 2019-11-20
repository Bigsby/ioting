#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define STEER 3
#define MOTOR_FORWARD 10  // Motor Red Wire - L293D IN1
#define MOTOR_BACKWARD 9 // Motor Black Wire - L293D IN2
#define MOTOR_SPEED 11
#define SPEED_START 100
#define LED 13

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Servo steer;

void goTo(int a);
void flashLed();

#define FAR_RIGHT 146
#define MIDDLE 115
#define FAR_LEFT 65

void testServo()
{
  goTo(MIDDLE);
  goTo(FAR_RIGHT);
  goTo(MIDDLE);
  goTo(FAR_LEFT);
}

void printSpeed(int speed, int motor)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Going ");
  display.print(speed, DEC);
  display.print(" on ");
  display.println(motor, DEC);
  display.display();
  // Serial.print("Going ");
  // Serial.print(speed, DEC);
  // Serial.print(" on ");
  // Serial.println(motor, DEC);
}

#define SPEED_WAIT 50
void testMotor(int on_motor, int off_motor)
{
  //
  digitalWrite(off_motor, LOW);
  digitalWrite(on_motor, HIGH);
  //digitalWrite(MOTOR_SPEED, HIGH);
  for (int speed = SPEED_START; speed < 256; speed++)
  {
    analogWrite(MOTOR_SPEED, speed);
    printSpeed(speed, on_motor);
    delay(SPEED_WAIT);
  }
  flashLed();

  for (int speed = 255; speed >= SPEED_START; speed--)
  {
    analogWrite(MOTOR_SPEED, speed);
    printSpeed(speed, on_motor);
    delay(SPEED_WAIT);
  }
  flashLed();
  digitalWrite(MOTOR_SPEED, LOW);
}

void testDisplay()
{
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);
  for (int a = 0; a < 255; a++)
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(a, DEC);
    display.display();
    delay(100);
  }
}

void setup()
{
  pinMode(LED, OUTPUT);

  steer.attach(STEER);

  pinMode(MOTOR_FORWARD, OUTPUT);
  pinMode(MOTOR_BACKWARD, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    flashLed();
    flashLed();
  }
  else
  {
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.cp437(true);
  }
}

#define FLASH_REPEATS 5
#define FLASH_HIGH 250
#define FLASH_LOW 100
#define FLASH_WAIT 1000

void showPosition(int pos) {

}

void goTo(int pos)
{
  steer.write(pos);
  
  // Serial.print("Went to ");
  // Serial.println(steer.read(), DEC);
  delay(1000);
}

void flashLed()
{
  for (int i = 0; i < FLASH_REPEATS; i++)
  {
    digitalWrite(LED, HIGH);
    delay(FLASH_HIGH);
    digitalWrite(LED, LOW);
    delay(FLASH_LOW);
  }
  delay(FLASH_WAIT);
}

void loop()
{
  //testDisplay();
  testMotor(MOTOR_FORWARD, MOTOR_BACKWARD);
  testMotor(MOTOR_BACKWARD, MOTOR_FORWARD);
}