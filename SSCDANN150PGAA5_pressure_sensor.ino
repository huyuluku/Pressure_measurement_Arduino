const int pressurePin = A0;  // Analog input pin for the pressure sensor

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(pressurePin);  // Read analog value from the pressure sensor
  double pressureValue = sensorValue/1024.0 * 150.0;
  //Serial.println(sensorValue);
  Serial.print("Sensor Value: ");
  Serial.print(pressureValue);
  Serial.print("psi\n");

  delay(500);  // Delay before taking the next reading
}
