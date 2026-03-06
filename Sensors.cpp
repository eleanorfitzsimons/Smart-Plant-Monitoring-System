#include "Sensors.h"

/******** VARIABLES **********/
float cogX_filtered = 0;
float cogY_filtered = 0;

unsigned long thresholdStartTime = 0;
unsigned long lastRotationTime = 0;
bool thresholdActive = false;

float readSensor(int pin) {
  return analogRead(pin);
}

bool computeCOG(float S1, float S2, float S3, float S4, float &cogX, float &cogY) {

  float W = S1 + S2 + S3 + S4; //total force, for normalisation
  //should i have a check to avoid zero division
  if (W == 0) return false;

  //find x and y coords of centre of gravity
  cogX = ((S2 + S4) - (S1 + S3)) * d / W;
  cogY = ((S3 + S4) - (S1 + S2)) * d / W;

  //d is half distance between sensors

  return true;
}

//will possibly have to amplify the load cells

/**
//exponential moving average filter
void filterCoG(float cogX, float cogY) {
  cogX_filtered = alpha * cogX + (1 - alpha) * cogX_filtered;
  cogY_filtered = alpha * cogY + (1 - alpha) * cogY_filtered;
}
*/

float computeMagnitude(float cogX, float cogY) {
  //gives overall lean amount... distance from origin to point
  return sqrt(cogX * cogX + cogY * cogY);
}

bool shouldRotate(float magnitude) {

  unsigned long now = millis();

  if (magnitude > COG_THRESHOLD) {

    //need to ensure that temporsaty disturbances dont trigget motor
    if (!thresholdActive) {
      thresholdActive = true;
      thresholdStartTime = now;
    }

    if ((now - thresholdStartTime > PERSIST_TIME) &&
        (now - lastRotationTime > COOLDOWN_TIME)) {

      lastRoationTime = now;
      thresholdActive = false;
      return true;
    }

  } 
  else {
    thresholdActive = false;
  }


  return false;
}



///////////////////////////////////////////
//note: the pressure sensors not always equal... 
//so once we have the sensors we have to compute the baseline and subtract it
////////////////////////////////////////////////////