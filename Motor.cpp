#include "Motor.h";
#include "Sensors.h";

Servo myServo;
bool direction = false;
void servo_init(int pin) {
  myServo.attach(pin);
}


void rotate180() {
  direction = !direction;
  Serial.println("Rotating 180 degrees...");

  if (direction)
    myServo.write(180);
  else
    myServo.write(0);
}