/********************************************************
   PID Basic Example
   Reading analog input 0 to control analog PWM output 3
 ********************************************************/

#include <PID_v1.h>
#include"PWM.h"

#define PIN_INPUT 0
#define PIN_OUTPUT 3

//Define Variables we'll be connecting to
double Setpoint, Input, Output, duty;//duty is the anti windup output

//Specify the links and initial tuning parameters
double Kp = 0.0000694, Ki = 200.67, Kd = 0.000088381;

//define some values that represent signals in the block doagran structure
double Eprevious = 0, dp = 0, feedback = 0 ;

int32_t frequency = 100000; //PWM frequency
PID myPID(&Eprevious, &duty, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup()
{
  Serial.begin(9600);

  //initialize the variables we're linked to
  Input = analogRead(PIN_INPUT);
  Setpoint = 3;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

  //initialize the PWM pin and set the frequency.
  InitTimersSafe();
  bool success = SetPinFrequencySafe(PIN_OUTPUT, frequency); // set frequency
}




//some function that will be used in loop:


void Readfeedback() {
  Input = analogRead(PIN_INPUT); //0-5V corresponding to 0-1023 digital value
  Input = Input / 1024 * 5; //transfer to volt
}

double U, Uminus1 = 0 , Uminus2 = 0 , Y, Yminus1 = 0, Yminus2 = 0; //used to calculate differential equation
void FromUtoYp() {
  Y = 0.2887 * U + 0.07534 * Uminus1 - 0.2134 * Uminus2 + 1.954 * Yminus1 - 0.9791 * Yminus2;
  Uminus2 = Uminus1;
  Uminus1 = U;
  Yminus2 = Yminus1;
  Yminus1 = Y;
}

double Yp[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 }; //to store the history value of Yp，Yp[0]为Yp(k);Yp[7]为Yp(k-7)
void FromYptoY1(){     //Y1 equals to Yp[7]
  for (int i = 7; i > 0; i--) {
    Yp[i] = Yp[i - 1];
  }
  Yp[0] = Y;
}


void loop()
{
  Readfeedback();
  //this function describe the PID block in smith predictor structure, here input is error signal; output is duty cycle
  myPID.Compute();
  duty = duty >= 200 ? 200 : duty; //for anti windup prupose
  pwmWrite(PIN_OUTPUT, duty);
  FromUtoYp();
  FromYptoY1();
  dp = Input - Yp[7];
  Eprevious = dp + Y;
}
