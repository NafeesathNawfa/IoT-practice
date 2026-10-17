#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

const int BUTTON_PIN = 4;
const unsigned long SAMPLE_INTERVAL_MS = 20; // 50 Hz sampling rate (1 sample every 20ms)

unsigned long lastSampleTime = 0;
bool wasButtonPressed = false;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  // Use internal pull-up resistor (Button reads LOW when held)
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!mpu.begin()) {
    Serial.println("Error: MPU6050 not detected!");
    while (1) delay(10);
  }

  // Set ranges suited for motion gesture recognition
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("System Ready! Press and HOLD the button to stream motion data...");
}

void loop() {
  // Check if button is currently being held down (ACTIVE LOW)
  bool isButtonHeld = (digitalRead(BUTTON_PIN) == LOW);

  // If newly pressed, output header line once
  if (isButtonHeld && !wasButtonPressed) {
    Serial.println("ax,ay,az,gx,gy,gz");
    wasButtonPressed = true;
  }

  // Reset flag when button is released
  if (!isButtonHeld && wasButtonPressed) {
    wasButtonPressed = false;
  }

  // Stream continuous CSV data ONLY while button is actively held down
  if (isButtonHeld) {
    unsigned long now = millis();
    if (now - lastSampleTime >= SAMPLE_INTERVAL_MS) {
      lastSampleTime = now;

      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      // Print formatted CSV row
      Serial.print(a.acceleration.x); Serial.print(",");
      Serial.print(a.acceleration.y); Serial.print(",");
      Serial.print(a.acceleration.z); Serial.print(",");
      Serial.print(g.gyro.x);         Serial.print(",");
      Serial.print(g.gyro.y);         Serial.print(",");
      Serial.println(g.gyro.z);
    }
  }
}