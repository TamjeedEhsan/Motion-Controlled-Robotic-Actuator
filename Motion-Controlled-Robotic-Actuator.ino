#include <Wire.h>
#include <Servo.h>

const int MPU_ADDR = 0x68;
const int SERVO_PIN = 9;

Servo myServo;

float accelX, accelY, accelZ;
float roll;

int servoAngle = 90;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1C);
  Wire.write(0x08);
  Wire.endTransmission();

  myServo.attach(SERVO_PIN);
  myServo.write(90);

  Serial.println("Motion-Controlled Actuator Started");
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU_ADDR, 6, true);

  if (Wire.available() < 6) {
    return;
  }

  int16_t rawX = (Wire.read() << 8) | Wire.read();
  int16_t rawY = (Wire.read() << 8) | Wire.read();
  int16_t rawZ = (Wire.read() << 8) | Wire.read();

  accelX = rawX / 8192.0;
  accelY = rawY / 8192.0;
  accelZ = rawZ / 8192.0;

  roll = atan2(accelY, accelZ) * 180.0 / PI;

  servoAngle = map((int)roll, -45, 45, 0, 180);
  servoAngle = constrain(servoAngle, 0, 180);

  myServo.write(servoAngle);

  Serial.print("Roll: ");
  Serial.print(roll);
  Serial.print(" | Servo Angle: ");
  Serial.println(servoAngle);

  delay(20);
}
