#include <Wire.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_Sensor.h>

Adafruit_LIS2MDL lis2mdl;

void setup() {
  Serial.begin(9600);
  Serial.println("LIS2MDL Magnetometer Test");

  if (!lis2mdl.begin()) {
    Serial.println("Failed to find LIS2MDL chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("LIS2MDL Found!");
}

void loop() {
  sensors_event_t event;
  lis2mdl.getEvent(&event);

  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print(" uT ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print(" uT ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.println(" uT");

  delay(500);
}
