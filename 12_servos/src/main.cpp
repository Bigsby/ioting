#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN 150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600 // This is the 'maximum' pulse length count (out of 4096)

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver(0x40);

void setup()
{
  servos.begin();
  servos.setPWMFreq(60);
}

void setValueInAllServos(uint16_t value)
{
  for (int servo = 0; servo < 12; servo++)
  {
    servos.setPWM(servo, 0, value);
  }
}

void min_max()
{
  setValueInAllServos(SERVOMIN);
  delay(500);
  setValueInAllServos(SERVOMAX);
  delay(500);
}

void sametime()
{
  for (int value = SERVOMIN; value <= SERVOMAX; value += 50)
  {
    setValueInAllServos(value);
    delay(1000);
  }

  for (int value = SERVOMAX; value >= SERVOMIN; value -= 50)
  {
    setValueInAllServos(value);
    delay(1000);
  }

  for (int value = SERVOMIN; value <= SERVOMAX; value += 10)
  {
    setValueInAllServos(value);
    delay(200);
  }

  for (int value = SERVOMAX; value >= SERVOMIN; value -= 10)
  {
    setValueInAllServos(value);
    delay(200);
  }
}

void inverted()
{
  for (int servo = 0; servo < 12; servo++)
  {
    if (servo % 2 == 0)
    {
      servos.setPWM(servo, 0, SERVOMAX);
    }
    else
    {
      servos.setPWM(servo, 0, SERVOMIN);
    }
  }
  delay(1000);
  for (int servo = 0; servo < 12; servo++)
  {
    if (servo % 2 == 0)
    {
      servos.setPWM(servo, 0, SERVOMIN);
    }
    else
    {
      servos.setPWM(servo, 0, SERVOMAX);
    }
  }
  delay(1000);
}

void one_at_a_time()
{
  setValueInAllServos(SERVOMIN);
  for (int servo = 0; servo < 12; servo++)
  {
    servos.setPWM(servo, 0, SERVOMIN + (servo * 37));
    delay(100);
  }

  setValueInAllServos(SERVOMAX);
  for (int servo = 11; servo >= 0; servo--)
  {
    servos.setPWM(servo, 0, SERVOMIN + (servo * 37));
    delay(100);
  }

  setValueInAllServos(SERVOMIN);
  for (int servo = 0; servo < 12; servo++)
  {
    servos.setPWM(servo, 0, SERVOMIN + (servo * 37));
    delay(100);
  }

  setValueInAllServos(SERVOMAX);
  for (int servo = 11; servo >= 0; servo--)
  {
    servos.setPWM(servo, 0, SERVOMIN + (servo * 37));
    delay(100);
  }

  setValueInAllServos(SERVOMIN);
  for (int servo = 0; servo < 12; servo++) {
    servos.setPWM(servo, 0, SERVOMAX);
    delay(200);
  }

  for (int servo = 0; servo < 12; servo++) {
    servos.setPWM(servo, 0, SERVOMIN);
    delay(200);
  }

  for (int servo = 0; servo < 12; servo++) {
    servos.setPWM(servo, 0, SERVOMAX);
    delay(200);
  }

  for (int servo = 0; servo < 12; servo++) {
    servos.setPWM(servo, 0, SERVOMIN);
    delay(200);
  }
}

void loop()
{
  min_max();
  sametime();
  inverted();
  inverted();
  one_at_a_time();
}