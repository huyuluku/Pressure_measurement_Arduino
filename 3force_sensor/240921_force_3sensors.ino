#include <Arduino.h>

const int sensorPins[3] = {A0, A1, A2};  // Vout pins connected to A0, A1, A2
const int numReadings = 10;              // Number of readings to average
const int baselineValue = 20;            // Adjust this based on the raw reading when no force is applied

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 3; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  Serial.println("Setup complete, starting readings...");
}

void loop() {
  float totalPressure[3] = {0};  // Array to store total pressure for each sensor

  // Take 10 readings for each sensor
  for (int i = 0; i < numReadings; i++) {
    for (int j = 0; j < 3; j++) {
      int rawValue = analogRead(sensorPins[j]);  // Read sensor value for each sensor
      rawValue = rawValue - baselineValue;       // Subtract baseline to zero the sensor
      if (rawValue < 0) rawValue = 0;            // Ensure no negative readings
      
      float force = map(rawValue, 0, 1000, 0, 200);  // Map the raw sensor value to the force range
      totalPressure[j] += force;  // Sum up the mapped force values for each sensor
    }
    delay(10);  // Short delay between readings
  }

  // Calculate and output the average force for each sensor
  for (int j = 0; j < 3; j++) {
    float averagePressure = totalPressure[j] / numReadings;  // Calculate the average
    Serial.print("Sensor ");
    Serial.print(j + 1);
    Serial.print(" Average Force: ");
    Serial.print(averagePressure);
    Serial.println(" grams");
  }

  Serial.println("");  // Print a blank line for better readability between sets
  delay(100);  // Delay 1 second before the next set of readings
}
