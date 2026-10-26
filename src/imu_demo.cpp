// IMU demo for M5Stack (M5Unified)
// Uses the built-in IMU (M5Stack Gray / M5GO / Fire / Core2).
// Shows accelerometer and gyro values, plus a "bubble level" dot that moves when you tilt it.
// Press button A to calibrate the gyro (keep the device still on a desk).
#include <Arduino.h>
#include <M5Unified.h>

static constexpr int LEVEL_X = 250;   // bubble level centre
static constexpr int LEVEL_Y = 140;
static constexpr int LEVEL_R = 55;

int dotX = LEVEL_X, dotY = LEVEL_Y;

void setup()
{
  M5.begin();
  Serial.begin(115200);

  M5.Display.setRotation(1);
  M5.Display.setTextSize(2);

  const char *name;
  switch (M5.Imu.getType()) {
  case m5::imu_none:    name = "not found"; break;
  case m5::imu_sh200q:  name = "SH200Q";    break;
  case m5::imu_mpu6050: name = "MPU6050";   break;
  case m5::imu_mpu6886: name = "MPU6886";   break;
  case m5::imu_mpu9250: name = "MPU9250";   break;
  case m5::imu_bmi270:  name = "BMI270";    break;
  default:              name = "unknown";   break;
  }
  Serial.printf("IMU: %s\n", name);

  if (M5.Imu.getType() == m5::imu_none) {
    M5.Display.println("IMU not found");
    M5.Display.println("(Basic has no IMU)");
    for (;;) delay(1);
  }

  M5.Imu.loadOffsetFromNVS();   // use saved calibration, if any

  M5.Display.setCursor(10, 10);
  M5.Display.printf("IMU Demo (%s)", name);
  M5.Display.setCursor(10, 220);
  M5.Display.setTextSize(1);
  M5.Display.print("BtnA: calibrate gyro (keep still)");
  M5.Display.setTextSize(2);
  M5.Display.drawCircle(LEVEL_X, LEVEL_Y, LEVEL_R, TFT_WHITE);
}

static void calibrateGyro()
{
  M5.Display.fillRect(0, 40, 180, 170, TFT_BLACK);
  M5.Display.setTextColor(TFT_YELLOW, TFT_BLACK);
  M5.Display.setCursor(10, 120);
  M5.Display.print("Calibrating...");

  M5.Imu.setCalibration(0, 128, 0);    // gyro only
  uint32_t start = millis();
  while (millis() - start < 3000) { M5.Imu.update(); delay(5); }
  M5.Imu.setCalibration(0, 0, 0);
  M5.Imu.saveOffsetToNVS();

  M5.Display.fillRect(0, 40, 180, 170, TFT_BLACK);
}

void loop()
{
  M5.update();
  if (M5.BtnA.wasClicked()) calibrateGyro();

  if (!M5.Imu.update()) return;
  auto data = M5.Imu.getImuData();

  static uint32_t lastDraw = 0;
  if (millis() - lastDraw < 50) return;   // redraw at ~20 fps
  lastDraw = millis();

  // Values, printed with a background colour so old digits are overwritten (no flicker)
  M5.Display.setTextColor(TFT_CYAN, TFT_BLACK);
  M5.Display.setCursor(10, 45);  M5.Display.print("Accel (g)");
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.setCursor(10, 65);  M5.Display.printf("X %+5.2f", data.accel.x);
  M5.Display.setCursor(10, 85);  M5.Display.printf("Y %+5.2f", data.accel.y);
  M5.Display.setCursor(10, 105); M5.Display.printf("Z %+5.2f", data.accel.z);

  M5.Display.setTextColor(TFT_ORANGE, TFT_BLACK);
  M5.Display.setCursor(10, 135); M5.Display.print("Gyro (dps)");
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.setCursor(10, 155); M5.Display.printf("X %+6.1f", data.gyro.x);
  M5.Display.setCursor(10, 175); M5.Display.printf("Y %+6.1f", data.gyro.y);
  M5.Display.setCursor(10, 195); M5.Display.printf("Z %+6.1f", data.gyro.z);

  // Bubble level: move the dot by the tilt (accel X/Y), clamped to the circle
  float dx = -data.accel.x, dy = data.accel.y;
  float len = sqrtf(dx * dx + dy * dy);
  if (len > 1.0f) { dx /= len; dy /= len; }
  int nx = LEVEL_X + dx * (LEVEL_R - 8);
  int ny = LEVEL_Y + dy * (LEVEL_R - 8);
  if (nx != dotX || ny != dotY) {
    M5.Display.fillCircle(dotX, dotY, 6, TFT_BLACK);
    dotX = nx; dotY = ny;
  }
  M5.Display.drawFastHLine(LEVEL_X - LEVEL_R + 1, LEVEL_Y, LEVEL_R * 2 - 1, TFT_DARKGREY);
  M5.Display.drawFastVLine(LEVEL_X, LEVEL_Y - LEVEL_R + 1, LEVEL_R * 2 - 1, TFT_DARKGREY);
  M5.Display.fillCircle(dotX, dotY, 6, TFT_GREEN);

  Serial.printf("ax=%.2f ay=%.2f az=%.2f  gx=%.1f gy=%.1f gz=%.1f\n",
                data.accel.x, data.accel.y, data.accel.z, data.gyro.x, data.gyro.y, data.gyro.z);
}
