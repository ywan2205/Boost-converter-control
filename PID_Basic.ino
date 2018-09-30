
// This example uses the timer interrupt to sample feedback from boost converter.
// The sampling time is set as 5kHz
// After the calculation of PID is done, it will generate a PWM output with 100kHz frequency.

#include <PWM.h>

#include <avdweb_AnalogReadFast.h>

#include <PID_v1.h>

#define PIN_INPUT 0  //This is the feedback pin
#define PIN_OUTPUT 3 //PWM generated from this pin. It is connected to timer2

volatile int32_t i = 0;

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Exponential Filter
double alpha = 0.3;
double average = 0;

//Specify the links and initial tuning parameters
double Kp=0.012, Ki=66.6668, Kd=0.000261;        //consider the input varies from (0, 1024) duty cycle varies from (0, 255). The pid parameters should times 1024/255 = 4.0157
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

volatile double feedback = 0;//Store the feedback value

//For PWM library
int32_t frequency=100000;//PWM frequency 100kHz

void setup(void)
{
  pinMode(PIN_INPUT, INPUT);
  pinMode(PIN_OUTPUT, OUTPUT);
  Serial.begin(9600);

  //PID part: initialize the variables we're linked to
  Input = analogRead(PIN_INPUT);
  Setpoint = 300;//300/1024*5*3 = 4.39V output      4.04/1.35

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

  //minimize the Sampling time (1 micro second) 
  myPID.SetSampleTime(200);

  //Set output limits
  myPID.SetOutputLimits(0, 200);// for amti windup purpose. output will be keep as the limitation when the output outweigh the limitation

  
  //initialize the PWM pin and set the frequency.
  InitTimersSafe();
  bool success = SetPinFrequencySafe(PIN_OUTPUT,frequency); // set frequency

  cli();//stop interrupts

  //set timer1 interrupt at 8kHz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 49;// = (16*10^6) / (5000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 64 prescaler
  TCCR1B |= (1 << CS11| 1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts
}


ISR(TIMER1_COMPA_vect){  //change the 0 to 1 for timer1 and 2 for timer2
   //  PID part: 
  Input = analogReadFast(PIN_INPUT);
  average = average*alpha+Input*(1-alpha);
  myPID.Compute();
  pwmWrite(PIN_OUTPUT, Output);
   i++;
}

void loop(void)//to make sure PID actually run 5k times per second.
{
  int32_t icopy = 0;
  noInterrupts();
  Serial.print(Input);//For debug purpose
  Serial.print(" ");
  icopy = i;
  Serial.println(icopy);
  interrupts();
  delay(1000);
}
