# Pressure_measurement_Arduino
This is a set of Arduino and Python codes for various sensors.

![IMG_85](https://github.com/user-attachments/assets/1612d689-cf3e-4063-be27-6b8834ddffff)

1. 3force_sensor includes an Arduino code that use 3 DF9-16 High Precise Force Sensitive Resistor Force Sensor to measure the force. It also contains python code that reads output from serial port and save as excel.

2. 5sensor includes an Arduino code that read pressure from 5 (maximum could be 7) Adafruit MPRLS Ported Pressure Sensors via a TCA9548A I2C Multiplexer. It also contains python code that reads output from serial port and save as excel.

![gyro_sensor](https://github.com/user-attachments/assets/b89e211b-8623-425b-a09c-bac5b3e1e66c)

3. gyro_sensor includes an Arduino code that read Yaw, Pitch, and Roll from 2 MPU6050 gyro sensors. It also contains python code that reads output from serial port and save as excel.

4. 20240402_LCD_monitor is an Arduino initialization code for a LC display.

5. LIS2MDL_magnetometer is an Arduino code that initialize LIS2MDL_magnetometer sense the magnetic fields that surround us with this handy triple-axis magnetometer (compass) module. This sensor tends to be paired with a 6-DoF (degree of freedom) accelerometer/gyroscope to create a 9-DoF inertial measurement unit that can detect its orientation in real-space thanks to Earth's stable magnetic field.

6. R21H02-01_pressure_Sensor is used to initialize R21H02-01 sensor for Aerotech customization.

7. SSCDANN150PGAA5_pressure_Sensor is used to initialize SSCDANN150PGAA5 sensor for Aerotech customization.
 
