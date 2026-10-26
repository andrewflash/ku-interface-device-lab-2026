// PIR motion unit demo for M5Stack (M5Unified)
// Connect the PIR unit to Port B (black). Its output goes to the Port B input pin
// (GPIO36 on Basic/Gray/Core2): 1 = motion detected, 0 = no motion.
// After power-on the sensor needs a few seconds to settle.
#include <Arduino.h>
#include <M5Unified.h>

int pirPin;
int lastState = -1;
unsigned int motionCount = 0;

void setup()
{
  M5.begin();
  Serial.begin(115200);

  pirPin = M5.getPin(m5::pin_name_t::port_b_in);
  pinMode(pirPin, INPUT);
  Serial.printf("PIR on pin %d\n", pirPin);

  M5.Display.setRotation(1);
  M5.Display.setTextSize(2);
}

void loop()
{
  int state = digitalRead(pirPin);
  if (state == lastState) { delay(50); return; }

  if (state == HIGH && lastState != -1) motionCount++;
  lastState = state;

  // Redraw only when the state changes
  M5.Display.fillScreen(state ? TFT_RED : TFT_BLACK);
  M5.Display.setTextColor(TFT_WHITE);
  M5.Display.setCursor(10, 20);
  M5.Display.println("PIR Motion Sensor");

  M5.Display.setTextSize(4);
  M5.Display.setCursor(20, 100);
  M5.Display.print(state ? "MOTION!" : "No motion");

  M5.Display.setTextSize(2);
  M5.Display.setCursor(10, 200);
  M5.Display.printf("Detections: %u", motionCount);

  Serial.printf("%s (count=%u)\n", state ? "Motion detected" : "No motion", motionCount);
}
