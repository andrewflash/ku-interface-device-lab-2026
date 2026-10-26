// ENV II sensor demo for M5Stack (M5Unified + M5Unit-ENV)
// Connect the ENV II unit to Port A (red). The pins are taken from M5Unified,
// so this works on Basic/Gray (SDA=21, SCL=22) and Core2 (SDA=32, SCL=33).
#include <Arduino.h>
#include <M5Unified.h>
#include <M5UnitENV.h>

SHT3X  sht3x;   // temperature + humidity
BMP280 bmp;     // pressure

void setup()
{
  M5.begin();
  Serial.begin(115200);
  int sda = M5.getPin(m5::pin_name_t::port_a_sda);
  int scl = M5.getPin(m5::pin_name_t::port_a_scl);
  Serial.printf("Board id: %d, Port A SDA=%d SCL=%d\n", (int)M5.getBoard(), sda, scl);

  M5.Display.setRotation(1);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting...");

  if (!sht3x.begin(&Wire, SHT3X_I2C_ADDR, sda, scl, 400000U)) {
    M5.Display.println("SHT30 not found");
    Serial.println("SHT30 not found");
  }
  if (!bmp.begin(&Wire, BMP280_I2C_ADDR, sda, scl, 400000U)) {
    M5.Display.println("BMP280 not found");
    Serial.println("BMP280 not found");
  }
  bmp.setSampling(BMP280::MODE_NORMAL, BMP280::SAMPLING_X2, BMP280::SAMPLING_X16,
                  BMP280::FILTER_X16, BMP280::STANDBY_MS_500);
  delay(2000);
}

void loop()
{
  sht3x.update();
  bmp.update();

  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(10, 20);
  M5.Display.println("ENV II Sensor\n");
  M5.Display.printf(" Temp:     %.1f C\n\n", sht3x.cTemp);
  M5.Display.printf(" Humidity: %.1f %%\n\n", sht3x.humidity);
  M5.Display.printf(" Pressure: %.1f hPa\n", bmp.pressure / 100);
  Serial.printf("T=%.1fC H=%.1f%% P=%.1fhPa\n", sht3x.cTemp, sht3x.humidity, bmp.pressure / 100);

  delay(1000);
}
