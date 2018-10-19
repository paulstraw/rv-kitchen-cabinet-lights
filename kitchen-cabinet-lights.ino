#include <Adafruit_DotStar.h>
#include <avr/power.h>

#define NUMPIXELS 30
#define CLOCKPIN 3
#define DATAPIN 4

int configModeTimeout = 3000;
bool configMode = false;
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

int red = 255;
int green = 143;
int blue = 79;
int colorIncrementAmount = 16;

int leftDoorPin = 0;
int leftDoorLightOffset = 22;
int leftDoorLightCount = 8;
bool leftDoorOpen = false;
bool leftDoorJustOpened = false;

int centerDoorPin = 1;
int centerDoorLightOffset = 10;
int centerDoorLightCount = 12;
bool centerDoorOpen = false;
bool centerDoorJustOpened = false;

int rightDoorPin = 2;
int rightDoorLightOffset = 0;
int rightDoorLightCount = 10;
bool rightDoorOpen = false;
bool rightDoorJustOpened = false;

bool allDoorsOpen = false;
unsigned long allDoorsOpenedAt = 0;

void setup() {
  pinMode(leftDoorPin, INPUT_PULLUP);
  pinMode(centerDoorPin, INPUT);
  pinMode(rightDoorPin, INPUT_PULLUP);

  strip.begin();
  strip.show();
}

void loop() {
  setDoorStates();
  updateMode();

  if (configMode) {
    configLoop();
  } else {
    standardLoop();
  }
}

void setDoorStates() {
  // Center door pin reversed:
  // https://learn.adafruit.com/tap-tempo-trinket/things-learned

  leftDoorJustOpened = !leftDoorOpen && digitalRead(leftDoorPin) == LOW;
  centerDoorJustOpened = !centerDoorOpen && digitalRead(centerDoorPin) == HIGH;
  rightDoorJustOpened = !rightDoorOpen && digitalRead(rightDoorPin) == LOW;

  leftDoorOpen = digitalRead(leftDoorPin) == LOW;
  centerDoorOpen = digitalRead(centerDoorPin) == HIGH;
  rightDoorOpen = digitalRead(rightDoorPin) == LOW;
}

void updateMode() {
  if (configMode)
  {
    if (!leftDoorOpen && !centerDoorOpen && !rightDoorOpen)
    {
      allDoorsOpen = false;
      allDoorsOpenedAt = 0;
    }
  }
  else
  {
    if (leftDoorOpen && centerDoorOpen && rightDoorOpen)
    {
      allDoorsOpen = true;

      if (allDoorsOpenedAt == 0)
      {
        allDoorsOpenedAt = millis();
      }
    }
    else
    {
      allDoorsOpen = false;
      allDoorsOpenedAt = 0;
    }
  }


  if (
    allDoorsOpen &&
    allDoorsOpenedAt != 0 &&
    (millis() - allDoorsOpenedAt > configModeTimeout)
  )
  {
    // Flash lights when entering config mode
    if (!configMode) {
      for(size_t i = 0; i < 3; i++)
      {
        strip.clear();
        strip.show();
        delay(100);

        for(size_t i = 0; i < NUMPIXELS; i++)
        {
          strip.setPixelColor(i, red, green, blue);
        }
        strip.show();
        delay(100);
      }
    }


    configMode = true;
  }
  else
  {
    configMode = false;
  }
}

void configLoop() {
  if (leftDoorJustOpened) {
    red = (red + colorIncrementAmount) % 255;
  }

  if (centerDoorJustOpened) {
    green = (green + colorIncrementAmount) % 255;
  }

  if (rightDoorJustOpened) {
    blue = (blue + colorIncrementAmount) % 255;
  }

  for(size_t i = 0; i < NUMPIXELS; i++)
  {
    strip.setPixelColor(i, red, green, blue);
  }
  strip.show();
}

void standardLoop() {
  strip.clear();

  if (leftDoorOpen) {
    int leftDoorMax = leftDoorLightCount + leftDoorLightOffset;
    for (size_t i = leftDoorLightOffset; i < leftDoorMax; i++)
    {
      strip.setPixelColor(i, red, green, blue);
    }
  }

  if (centerDoorOpen) {
    int centerDoorMax = centerDoorLightCount + centerDoorLightOffset;
    for (size_t i = centerDoorLightOffset; i < centerDoorMax; i++)
    {
      strip.setPixelColor(i, red, green, blue);
    }
  }

  if (rightDoorOpen) {
    int rightDoorMax = rightDoorLightCount + rightDoorLightOffset;
    for(size_t i = rightDoorLightOffset; i < rightDoorMax; i++)
    {
      strip.setPixelColor(i, red, green, blue);
    }
  }

  strip.show();
}
