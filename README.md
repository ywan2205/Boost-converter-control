# Boost-converter-control
Use this code to control a boost converter via current measurement. 

PID is used as the type of controller
IMC is selected as the method to tune the PID controller
sampling time 5kHz
PWM frequency 100kHz

The parameters of this boost converter are shown below:
inputVoltage = 6;
outputVoltage = 12;
inductor = 833e-5;
capacitor = 470e-6;
resistor = 47;

Kp = 0.003
Ki = 16.6667
Kd = 0.000065252

These PID parameters need to be multipled by 1024/255 to get the final value.
