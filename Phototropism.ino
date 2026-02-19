/***************** CONSTANTS *******************/
//PRESSURE SENSORS (oriented 90 deg around base)
const int PRESSURE1 = A1;
const int PRESSURE2 = A2;
const int PRESSURE3 = A3;
const int PRESSURE4 = A4;

//motor

//light sensors
const int LIGHT1 = A5;
const int LIGHT2 = A6;
//const int LIGHT3 = A7;
//const int LIGHT4 = A8;

const float COG_THRESHOLD = 0.1; ////////gentre of gravity has to be tuned
const unsigned long PERSIST_TIME = 5UL * 60UL * 60UL * 1000UL; // 6 hours (UL is unsigned long... matches milis type)
const unsigned long COOLDOWN_TIME = 24UL * 60UL * 60UL * 1000UL; // 24 hours

const float alpha = 0.001; //slow filter
/****************** GLOBAL VARIABLES *********************/
//PRESSURE SENSOR FORCE MEASUREMENT

float cogX_filtered = 0;
float cogY_filtered = 0;

unsigned long thresholdStartTime = 0;
unsigned long lastRotationTime = 0;

bool thresholdActive = false;

/************************ FUNCTIONS ************************/
//converting the bit val to voltage to lux for the light
float readSensor(int pin) {
  return analogRead(pin);
}
float luxVal(int pin){
  int adc = readSensor(pin);

  //prevent zero div
  if (adc == 0)
    adc = 1;
  
  //acd to voltage
  float voltage = adc * (5.0 / 1023.0);

  //voltage divider maths
  float R_fixed = 10000.0;   // 10k resistor... need to add
  float R_ldr = R_fixed * (5.0 / voltage - 1.0);

  ///////////calibration consts...... need to find
  float A = 500.0;
  float B = 1.4;

  float lux = A * pow(R_ldr, -B); //for lux formula

  return lux;

}

bool computeCoG(float S1, float S2, float S3, float S4, float &cogX, float &cogY) {

  float W = S1 + S2 + S3 + S4;

  if (W < 10) return false;  // avoid divide by zero

  cogX = ((S2 + S4) - (S1 + S3)) / W;
  cogY = ((S3 + S4) - (S1 + S2)) / W;

  return true;
}

void filterCoG(float cogX, float cogY) {
  cogX_filtered = alpha * cogX +
                 (1 - alpha) * cogX_filtered;

  cogY_filtered = alpha * cogY +
                 (1 - alpha) * cogY_filtered;
}

float computeMagnitude() {
  return sqrt(cogX_filtered * cogX_filtered +
              cogY_filtered * cogY_filtered);
}

bool shouldRotate(float magnitude) {

  unsigned long now = millis();

  if (magnitude > COG_THRESHOLD) {

    if (!thresholdActive) {
      thresholdActive = true;
      thresholdStartTime = now;
    }

    if ((now - thresholdStartTime > PERSIST_TIME) &&
        (now - lastRotationTime > COOLDOWN_TIME)) {
      return true;
    }

  } else {
    thresholdActive = false;
  }

  return false;
}

void rotate180() {
  Serial.println("Rotating 180 degrees...");
  // Then add motor control here
    lastRotationTime = millis();
  thresholdActive = false;
}

void setup() {
  Serial.begin(9600);

  pinMode(PRESSURE1, INPUT);
  pinMode(PRESSURE2, INPUT);

}

void loop() {
  //note: this will obvs be a VERY gradual process so will have to reduce frequency of polling in main code

  /*********** READ IN PRESSURE FORCES ****************/
  float P1, P2, P3, P4;
  float cogX, cogY;
  
  readSensors(P1, P2, P3, P4);

  if (!computeCoG(P1, P2, P3, P4, cogX, cogY))
    return;
  
  float magnitude = computeMagnitude();

  if (shouldRotate(magnitude)) {
    rotate180();
  }

  float W = P1 + P2 + P3 + P4;

  if (W < 10) return; //BY ZERO
  //ok so problem... when you water or whatever like the balance will shifr
  //ok so can we do some kind of spritzer where the water is distributed evenly
  //idea... pause after watering, like delay in the readings/recalc baseline afetr stabilised
  //only trigger rotation if the cog shift persists for many hours eg like 5 hour cog shift
  //also will want timer... limit osciations
  ///////////////////////////////
  //finding the centre of mass///
  ///////////////////////////////


  ///////////////////////////////
  //motor orientation////////////
  //rotate 180 deg///////////////
  ///////////////////////////////



  /*************READ IN LIGHT INTENSITY************/
  //note: i think for the overall thing just when weight shifts by certain deg, turn 180?
  //so then the light intensity kind of just for stats but could put a few arond the circumference and find average??
  float light1 = luxVal(LIGHT1);
  float light2 = luxVal(LIGHT2);

  //total light hitting surface... evenly spaced so just avg ig
  float total_light = light1/light2;
  Serial.print("Light hitting surface: ");
  Serial.print(total_light);

  delay(1000);
}




//Calibration 
/*
void loop() {
  int adc = analogRead(A0);

  float voltage = adc * (5.0 / 1023.0);
  float R_fixed = 10000.0;
  float R_ldr = R_fixed * (5.0 / voltage - 1.0);

  Serial.print("Resistance: ");
  Serial.println(R_ldr);

  delay(1000);
}
*/