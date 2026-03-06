#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#incldue <Servo.h>



//Function Declarations
void rotate180();
void servo_init(int pin);
extern bool direction = false;

#endif
