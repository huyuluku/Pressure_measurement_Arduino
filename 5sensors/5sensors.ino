#include <Wire.h>
#include <Adafruit_MPRLS.h>

#define TCAADDR 0x70
#define MPRLS_ADDR 0x18
#define NUM_SENSORS 5
#define NUM_READINGS 20
#define DELAY_BETWEEN_READINGS 1 // Time in milliseconds

Adafruit_MPRLS mprls[NUM_SENSORS];

void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    tcaselect(i);
    mprls[i] = Adafruit_MPRLS(MPRLS_ADDR);
    if (!mprls[i].begin()) {
      Serial.print("Failed to initialize sensor ");
      Serial.println(i);
    } else {
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.println(" initialized successfully");
    }
  }
}

void loop() {
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    tcaselect(i);
    
    float totalPressure = 0.0;
    for (uint8_t j = 0; j < NUM_READINGS; j++) {
      totalPressure += mprls[i].readPressure() / 68.947572932 - 14.875;
      delay(DELAY_BETWEEN_READINGS);
    }
    float averagePressure = totalPressure / NUM_READINGS;
    
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(averagePressure, 2);
    Serial.println(" psi");
  }
  Serial.println("--------------");
  delay(10);
}
