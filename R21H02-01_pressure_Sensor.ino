#include <Wire.h>

const int pressureSensorAddress = 0x28;  // I2C address of the pressure sensor

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  float pressure = readPressure();  // Read pressure data from the sensor
  Serial.print("Pressure: ");
  Serial.println(pressure);

  delay(1000);  // Delay before taking the next reading
}

float readPressure() {
  Wire.beginTransmission(pressureSensorAddress);
  // You might need to add commands to request pressure data from your specific sensor
  Wire.endTransmission();

  // Request 2 bytes of data
  Wire.requestFrom(pressureSensorAddress, 2);

  if (Wire.available() >= 2) {
    int rawData = Wire.read() << 8 | Wire.read();
    // Convert raw data to pressure using your specific sensor's data format
    // You might need to refer to the sensor's datasheet for conversion details
    // For demonstration purposes, we'll assume the conversion here
    float pressure = (rawData / 1024.0) * 100.0;  // Just an example conversion

    return pressure;
  }
  
  return -1.0;  // Return an error value if data couldn't be read
}
