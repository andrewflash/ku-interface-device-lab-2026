// Light unit demo for M5Stack (M5Unified)
// Connect the Light unit to Port B (black). The pins are taken from M5Unified,
// so this works on Basic/Gray (IN=36, OUT=26) and Core2 (IN=36, OUT=26).
//   Analog output  -> Port B input pin  (0..4095)
//   Digital output -> Port B output pin (threshold set by the knob on the unit)
#include <Arduino.h>
#include <M5Unified.h>

int analogPin;
int digitalPin;

void setup()
{
  M5.begin();
  Serial.begin(115200);

  analogPin  = M5.getPin(m5::pin_name_t::port_b_in);
  digitalPin = M5.getPin(m5::pin_name_t::port_b_out);
  pinMode(digitalPin, INPUT);
  Serial.printf("Port B analog=%d digital=%d\n", analogPin, digitalPin);

  M5.Display.setRotation(1);
  M5.Display.setTextSize(2);
}

void loop()
{
  int analogValue  = analogRead(analogPin);
  int digitalValue = digitalRead(digitalPin);

  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(10, 20);
  M5.Display.println("Light Sensor\n");
  M5.Display.printf(" Analog:  %d\n\n", analogValue);
  M5.Display.printf(" Digital: %d\n", digitalValue);

  // Bar graph of the analog value
  int w = map(analogValue, 0, 4095, 0, M5.Display.width() - 20);
  M5.Display.drawRect(10, 180, M5.Display.width() - 20, 30, TFT_WHITE);
  M5.Display.fillRect(10, 180, w, 30, TFT_YELLOW);

  Serial.printf("analog=%d digital=%d\n", analogValue, digitalValue);

  delay(200);
}
