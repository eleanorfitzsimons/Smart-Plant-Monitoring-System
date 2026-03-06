#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

//Function Declarations
float readSensor(int pin);
bool computeCOG(float S1, float S2, float S3, float S4, float &cogX, float &cogY);
//void filterCoG(float cogX, float cogY);
float computeMagnitude(float cogX, float cogY);
bool shouldRotate(float magnitude);


//Variable Declarations
const float COG_THRESHOLD = 0.1; ////////gentre of gravity has to be tuned
const float alpha = 0.001; //slow filter

extern float cogX_filtered;
extern float cogY_filtered;

const int d = 4; //half the distance between sensors

const unsigned long PERSIST_TIME = 5UL * 60UL * 60UL * 1000UL; // 5 hours (UL is unsigned long... matches milis type)
const unsigned long COOLDOWN_TIME = 24UL * 60UL * 60UL * 1000UL; // 24 hours
extern unsigned long thresholdStartTime;
extern unsigned long lastRotationTime;
extern bool thresholdActive;




#endif
