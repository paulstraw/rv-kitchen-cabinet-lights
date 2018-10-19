#include <Adafruit_DotStar.h>
#include <avr/power.h>

#define NUMPIXELS 30
#define CLOCKPIN 3
#define DATAPIN 4

int ledPin = 1;
int configModeTimeout = 3000;
bool configMode = false;
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN);

int red = 255;
int green = 239;
int blue = 223;
int colorIncrementAmount = 16;

int leftDoorPin = 0;
int leftDoorLightOffset = 20;
int leftDoorLightCount = 10;
bool leftDoorOpen = false;
bool leftDoorJustClosed = false;

int centerDoorPin = 1;
int centerDoorLightOffset = 10;
int centerDoorLightCount = 10;
bool centerDoorOpen = false;
bool centerDoorJustClosed = false;

int rightDoorPin = 2;
int rightDoorLightOffset = 10;
int rightDoorLightCount = 10;
bool rightDoorOpen = false;
bool rightDoorJustClosed = false;

bool allDoorsOpen = false;
unsigned long allDoorsOpenedAt = 0;

void setup() {
  pinMode(leftDoorPin, INPUT_PULLUP);
  pinMode(centerDoorPin, INPUT_PULLUP);
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
  leftDoorJustClosed = leftDoorOpen && digitalRead(leftDoorPin) == HIGH;
  centerDoorJustClosed = centerDoorOpen && digitalRead(centerDoorPin) == HIGH;
  rightDoorJustClosed = rightDoorOpen && digitalRead(rightDoorPin) == HIGH;

  leftDoorOpen = digitalRead(leftDoorPin) == LOW;
  centerDoorOpen = digitalRead(centerDoorPin) == LOW;
  rightDoorOpen = digitalRead(rightDoorPin) == LOW;
}

void updateMode() {
  if (leftDoorOpen && centerDoorOpen && rightDoorOpen)
  {
    allDoorsOpen = true;
    allDoorsOpenedAt = millis();
  }
  else
  {
    allDoorsOpen = false;
  }

  if (allDoorsOpen && (millis() - allDoorsOpenedAt > configModeTimeout))
  {
    configMode = true;

    // Flash lights when entering config mode
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
  else
  {
    configMode = false;
  }
}

void configLoop() {
  if (leftDoorJustClosed) {
    red = (red + colorIncrementAmount) % 255;
  }

  if (centerDoorJustClosed) {
    green = (green + colorIncrementAmount) % 255;
  }

  if (rightDoorJustClosed) {
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
