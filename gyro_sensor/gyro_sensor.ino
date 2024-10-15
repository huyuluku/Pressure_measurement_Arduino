#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>

// Create instances for both sensors
MPU6050 mpu1(0x68);  // Address of the first MPU6050
MPU6050 mpu2(0x69);  // Address of the second MPU6050

bool dmpReady1 = false;  // Set true if DMP init was successful
bool dmpReady2 = false;  // Set true if DMP init was successful
uint8_t mpuIntStatus1;   // Holds actual interrupt status byte from MPU
uint8_t mpuIntStatus2;   // Holds actual interrupt status byte from MPU
uint8_t devStatus1;      // Return status after each device operation (0 = success, !0 = error)
uint8_t devStatus2;      // Return status after each device operation (0 = success, !0 = error)
uint16_t packetSize1;    // Expected DMP packet size (default is 42 bytes)
uint16_t packetSize2;    // Expected DMP packet size (default is 42 bytes)
uint16_t fifoCount1;     // Count of all bytes currently in FIFO
uint16_t fifoCount2;     // Count of all bytes currently in FIFO
uint8_t fifoBuffer1[64]; // FIFO storage buffer
uint8_t fifoBuffer2[64]; // FIFO storage buffer

Quaternion q1;           // [w, x, y, z] quaternion container
Quaternion q2;           // [w, x, y, z] quaternion container
VectorFloat gravity1;    // [x, y, z] gravity vector
VectorFloat gravity2;    // [x, y, z] gravity vector
float ypr1[3];           // [yaw, pitch, roll] yaw/pitch/roll container and gravity vector
float ypr2[3];           // [yaw, pitch, roll] yaw/pitch/roll container and gravity vector

unsigned long lastOutputTime = 0;
const int outputInterval = 500; // Output every 0.5 seconds

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000);  // Set I2C frequency to 400kHz

  // Initialize the first MPU6050 sensor
  mpu1.initialize();
  devStatus1 = mpu1.dmpInitialize();

  // Supply your own gyro offsets here, scaled for min sensitivity
  mpu1.setXGyroOffset(220);
  mpu1.setYGyroOffset(76);
  mpu1.setZGyroOffset(-85);
  mpu1.setZAccelOffset(1788); // 1688 factory default for my test chip

  // Make sure it worked (returns 0 if so)
  if (devStatus1 == 0) {
    // Turn on the DMP, now that it's ready
    mpu1.setDMPEnabled(true);

    // Enable Arduino interrupt detection
    attachInterrupt(digitalPinToInterrupt(2), dmpDataReady1, RISING);
    mpuIntStatus1 = mpu1.getIntStatus();

    // Set our DMP ready flag
    dmpReady1 = true;

    // Get expected DMP packet size for later comparison
    packetSize1 = mpu1.dmpGetFIFOPacketSize();
  } else {
    // ERROR! DMP initialization failed
    Serial.print("DMP Initialization failed (code ");
    Serial.print(devStatus1);
    Serial.println(")");
  }

  // Initialize the second MPU6050 sensor
  mpu2.initialize();
  devStatus2 = mpu2.dmpInitialize();

  // Supply your own gyro offsets here, scaled for min sensitivity
  mpu2.setXGyroOffset(220);
  mpu2.setYGyroOffset(76);
  mpu2.setZGyroOffset(-85);
  mpu2.setZAccelOffset(1788); // 1688 factory default for my test chip

  // Make sure it worked (returns 0 if so)
  if (devStatus2 == 0) {
    // Turn on the DMP, now that it's ready
    mpu2.setDMPEnabled(true);

    // Enable Arduino interrupt detection
    attachInterrupt(digitalPinToInterrupt(3), dmpDataReady2, RISING);
    mpuIntStatus2 = mpu2.getIntStatus();

    // Set our DMP ready flag
    dmpReady2 = true;

    // Get expected DMP packet size for later comparison
    packetSize2 = mpu2.dmpGetFIFOPacketSize();
  } else {
    // ERROR! DMP initialization failed
    Serial.print("DMP Initialization failed (code ");
    Serial.print(devStatus2);
    Serial.println(")");
  }
}

void loop() {
  if (!dmpReady1 || !dmpReady2) return;

  unsigned long currentTime = millis();

  // Check for DMP data ready interrupt
  mpuIntStatus1 = mpu1.getIntStatus();
  mpuIntStatus2 = mpu2.getIntStatus();

  // Get current FIFO count
  fifoCount1 = mpu1.getFIFOCount();
  fifoCount2 = mpu2.getFIFOCount();

  // Check for overflow
  if ((mpuIntStatus1 & 0x10) || fifoCount1 == 1024) {
    mpu1.resetFIFO();
    Serial.println("FIFO overflow!");
  } else if (mpuIntStatus1 & 0x02) {
    while (fifoCount1 < packetSize1) fifoCount1 = mpu1.getFIFOCount();
    mpu1.getFIFOBytes(fifoBuffer1, packetSize1);
    fifoCount1 -= packetSize1;

    mpu1.dmpGetQuaternion(&q1, fifoBuffer1);
    mpu1.dmpGetGravity(&gravity1, &q1);
    mpu1.dmpGetYawPitchRoll(ypr1, &q1, &gravity1);
  }

  if ((mpuIntStatus2 & 0x10) || fifoCount2 == 1024) {
    mpu2.resetFIFO();
    Serial.println("FIFO overflow!");
  } else if (mpuIntStatus2 & 0x02) {
    while (fifoCount2 < packetSize2) fifoCount2 = mpu2.getFIFOCount();
    mpu2.getFIFOBytes(fifoBuffer2, packetSize2);
    fifoCount2 -= packetSize2;

    mpu2.dmpGetQuaternion(&q2, fifoBuffer2);
    mpu2.dmpGetGravity(&gravity2, &q2);
    mpu2.dmpGetYawPitchRoll(ypr2, &q2, &gravity2);
  }

  // Output data every 0.5 seconds
  if (currentTime - lastOutputTime >= outputInterval) {
    lastOutputTime = currentTime;

    Serial.print("YPR #1: ");
    Serial.print(ypr1[0] * 180 / PI);
    Serial.print(", ");
    Serial.print(ypr1[1] * 180 / PI);
    Serial.print(", ");
    Serial.print(ypr1[2] * 180 / PI);
    Serial.print(", ");

    Serial.print("YPR #2: ");
    Serial.print(ypr2[0] * 180 / PI);
    Serial.print(", ");
    Serial.print(ypr2[1] * 180 / PI);
    Serial.print(", ");
    Serial.println(ypr2[2] * 180 / PI);
  }
}

void dmpDataReady1() {
  mpuIntStatus1 = mpu1.getIntStatus();
}

void dmpDataReady2() {
  mpuIntStatus2 = mpu2.getIntStatus();
}
