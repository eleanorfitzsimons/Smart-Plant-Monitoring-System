#include "Sensors.h"
#include "Motor.h"
#include "Pins.h"

void setup() {
  Serial.begin(9600);

  pinMode(PRESSURE1, INPUT);
  pinMode(PRESSURE2, INPUT);
  pinMode(PRESSURE3, INPUT);
  pinMode(PRESSURE4, INPUT);

  servo_init(9);


}

void loop() {
  float P1, P2, P3, P4;
  float cogX, cogY;
  
  P1 = readSensor(PRESSURE1);
  P2 = readSensor(PRESSURE2);
  P3 = readSensor(PRESSURE3);
  P4 = readSensor(PRESSURE4);

  if (!computeCoG(P1, P2, P3, P4, cogX, cogY)) return;

  float magnitude = computeMagnitude();

  if (shouldRotate(magnitude)) {
    rotate180();
  }

  delay(5000);
}
 



