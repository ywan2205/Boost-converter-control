/********************************************************
 * PID Basic Example
 * Reading analog input 0 to control analog PWM output 3
 ********************************************************/

#include <PID_v1.h>
#include"PWM.h"

#define PIN_INPUT 0
#define PIN_OUTPUT 3

//Define Variables we'll be connecting to
double Setpoint, Input, Output, duty;//duty is the anti windup output

//Specify the links and initial tuning parameters
double Kp=0.000406738, Ki=0.4883, Kd=0.0000019117;//just change these numbers

int32_t frequency=100000;//PWM frequency
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int const MeasurementsToAverage = 16;
double inputaverage[MeasurementsToAverage] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

double voltage1, current1;
double scope = 1.254, offset = 57.117; //to indicate the relationship between measured voltage and inductor current 

void setup()
{
  Serial.begin(9600);
  
  //initialize the variables we're linked to
  Input = analogRead(PIN_INPUT);
  Setpoint = 4;//reference value in volt

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

  //initialize the PWM pin and set the frequency.
  InitTimersSafe();
  bool success = SetPinFrequencySafe(PIN_OUTPUT,frequency); // set frequency
}

void loop()
{
  //Input = analogRead(PIN_INPUT); //0-5V corresponding to 0-1023 digital value
  //Input = Input/1024*5; //transfer to volt
  //myPID.Compute();
  //duty = antiWindup(Output);
  duty = 125;
  pwmWrite(PIN_OUTPUT,duty);
  voltage1 = analogRead(0);
  current1 = scope * voltage1 + offset;
  //ReadCurrent();
  //Input = average(inputaverage,MeasurementsToAverage);
  Serial.println(current1);
}

double antiWindup(double Output){
duty=Output;
if (duty>=200)
{duty=200;}
else
{duty=duty;}
return duty;
}

float AverageCurrent = 0;

float average(double inputaverage[],int MeasurementsToAverage){
for(int i = 0; i < MeasurementsToAverage; i++)
{
  AverageCurrent += inputaverage[i];
}
AverageCurrent /= MeasurementsToAverage;
return AverageCurrent;
}

int j = 0;
void ReadCurrent(){
    inputaverage[j] = analogRead(1);
  if(j<MeasurementsToAverage){
    j++;
  }
  else
  j=0;
}
