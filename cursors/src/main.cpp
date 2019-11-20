#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LEFT_PIN 4
#define RIGHT_PIN 5
#define UP_PIN 6
#define DOWN_PIN 7
#define KEY_PRESSED_PIN 2

#define MIN_X 0
#define MAX_X (SCREEN_WIDTH - 1)
#define MIN_Y 0
#define MAX_Y (SCREEN_HEIGHT - 1)

enum
{
  NONE = 0,
  UP = 1 << 0,
  DOWN = 1 << 1,
  LEFT = 1 << 2,
  RIGHT = 1 << 3,
  UP_LEFT = UP | LEFT,
  UP_RIGHT = UP | RIGHT,
  DOWN_RIGHT = DOWN | RIGHT,
  DOWN_LEFT = DOWN | LEFT
};

int current_action = NONE;
struct
{
  int x, y;
} current_position;

void displayDot()
{
  display.clearDisplay();
  display.drawPixel(current_position.x, current_position.y, WHITE);
  display.display();
}

struct key
{
  char pin;
  char action;
};

key keys[] = {
    {LEFT_PIN, LEFT},
    {RIGHT_PIN, RIGHT},
    {UP_PIN, UP},
    {DOWN_PIN, DOWN}};

char key_count = sizeof(keys) / sizeof(key);

void setDirection()
{
  char is_key_pressed = 0;
  int new_direction = NONE;
  for (int key_index = 0; key_index < key_count; key_index++)
  {
    if (!digitalRead(keys[key_index].pin))
    {
      new_direction |= keys[key_index].action;
      is_key_pressed = 1;
    }
  }
  if (is_key_pressed)
    current_action = new_direction;
}

void goUp()
{
  current_position.y = max(MIN_Y, current_position.y - 1);
}

void goDown()
{
  current_position.y = min(MAX_Y, current_position.y + 1);
}

void goLeft()
{
  current_position.x = max(MIN_X, current_position.x - 1);
}

void goRight()
{
  current_position.x = min(MAX_X, current_position.x + 1);
}

struct action_pair
{
  int direction;
  void (*processor)();
};

struct action_pair actions[] = {
    {UP, &goUp},
    {DOWN, &goDown},
    {LEFT, &goLeft},
    {RIGHT, &goRight}};

int action_count = sizeof(actions) / sizeof(action_pair);

void setPosition()
{
  for (int action_index = 0; action_index < action_count; action_index++)
  {
    action_pair pair = actions[action_index];
    if (current_action & pair.direction)
    {
      (*pair.processor)();
    }
  }
}

void loop()
{
  setDirection();
  setPosition();
  displayDot();
}

void showCenteredMessage(String message)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.cp437(true);
  int16_t *x = 0, *y = 0;
  uint16_t *w = 0, *h = 0;
  display.getTextBounds(message, 0, 0, x, y, w, h);
  display.setCursor((MAX_X - *w) / 2, (MAX_Y - *h) / 2);
  display.print(message);
  display.display();
}

void showSplash()
{
  showCenteredMessage("...Ready?..");
  delay(1000);
}

void setup()
{
  for (int key_index; key_index < key_count; key_index++)
  {
    pinMode(keys[key_index].pin, INPUT_PULLUP);
  }

  while (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
  }
  showSplash();

  current_position = {
      SCREEN_WIDTH / 2,
      SCREEN_HEIGHT / 2};
  displayDot();
}