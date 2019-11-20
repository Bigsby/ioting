#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LED_PIN 5
#define COLOR_ORDER RGB
#define CHIPSET WS2812B
#define NUM_LEDS 10
#define button1 2
#define button2 3

#define BRIGHTNESS 200
#define FRAMES_PER_SECOND 30

bool gReverseDirection = false;

CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define COOLING 55

#define SPARKING 120

void Fire2012WithPalette()
{
 
 
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for (int i = 0; i < NUM_LEDS; i++)
  {
    heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for (int k = NUM_LEDS - 1; k >= 2; k--)
  {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if (random8() < SPARKING)
  {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }

  // Step 4.  Map from heat cells to LED colors
  for (int j = 0; j < NUM_LEDS; j++)
  {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8(heat[j], 240);
    CRGB color = ColorFromPalette(gPal, colorindex);
    int pixelnumber;
    if (gReverseDirection)
    {
      pixelnumber = (NUM_LEDS - 1) - j;
    }
    else
    {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}

CRGB current_color = CRGB::Blue;

void test_colors()
{
  FastLED.showColor(current_color);
}

void writeDisplay(int color) {
  display.clearDisplay();
  display.setCursor(0,0);
  if (color == 0) {
    display.print("Red");
  } else if (color == 1) {
    display.print("Green");
  } else {
    display.print("No Color");
  }
  display.display();
}

void changeColorRed()
{
  writeDisplay(0);
  FastLED.showColor(CRGB::Red);
}
void changeColorGreen()
{
  writeDisplay(1);
  FastLED.showColor(CRGB::Green);
}


void setup()
{
  delay(1000); // sanity delay
  


  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);//.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  FastLED.showColor(CRGB::Blue);
  attachInterrupt(digitalPinToInterrupt(2), changeColorRed, LOW);
  attachInterrupt(digitalPinToInterrupt(3), changeColorGreen, LOW);

   if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    // flashLed();
    // flashLed();
  }
  else
  {
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.cp437(true);
  }

  display.display();
  delay(1000);
  
}

int count = 0;
void do_flame()
{
  switch (count % 10000)
  {
  case 0:
    gPal = HeatColors_p;
    break;
    // case 1:
    //gPal = CRGBPalette16(CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
    //   break;
    // case 2:
    //   gPal = CRGBPalette16(CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White);
    //   break;
    // case 3:
    //   gPal = CRGBPalette16(CRGB::Black, CRGB::Red, CRGB::White);
    //   break;
  case 4:
    gPal = CRGBPalette16(CRGB::Black, CRGB::Red);
    break;
    // default:
    //   gPal = HeatColors_p;
    //   break;
  }
  count++;

  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random());

  // Fourth, the most sophisticated: this one sets up a new palette every
  // time through the loop, based on a hue that changes every time.
  // The palette is a gradient from black, to a dark color based on the hue,
  // to a light color based on the hue, to white.
  //
  //   static uint8_t hue = 0;
  //   hue++;
  //   CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
  //   CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
  //   gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);

  Fire2012WithPalette(); // run simulation frame, using palette colors

  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}


void (*test[])(void) = {
    &test_colors};

void loop()
{
  // FastLED.showColor(CRGB::Red);
  // delay(2000);
  // FastLED.showColor(CRGB::Black);
  // delay(1000);
  // FastLED.showColor(CRGB::Green);
  // delay(2000);
  // FastLED.showColor(CRGB::Black);
  // delay(1000);
  // FastLED.showColor(CRGB::Blue);
  // delay(2000);
  // FastLED.showColor(CRGB::Black);
  // delay(1000);

  //test_colors();
  // delay(1000);
  // for (int func = 0; func < 1; func++)
  //   (*test[func])();
}
