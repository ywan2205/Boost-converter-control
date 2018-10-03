inputVoltage = 6;
outputVoltage = 12;
dutyCycle = 1-inputVoltage/outputVoltage;
inductor = 833e-3;
capacitor = 470e-6;
resistor = 10;
inductorCurrent = outputVoltage/resistor/(1-dutyCycle);
openLoopGain = inputVoltage/inductor/capacitor;
openLoopZero = -inductorCurrent*inductor/inputVoltage;

undampedNaturalFreq = sqrt((1-dutyCycle)^2/inductor/capacitor);
%undamped natural frequency
dampingRatio = 1/resistor/capacitor/2/undampedNaturalFreq;
%damping ratio
G = tf([openLoopGain*openLoopZero openLoopGain],[1 1/resistor/capacitor (1-dutyCycle)^2/inductor/capacitor]);

timeconst = 0.1;
Kp_proposed = 2*undampedNaturalFreq*dampingRatio/(openLoopGain*timeconst);
Ti_proposed = 2*dampingRatio/undampedNaturalFreq;
Ki_proposed = Kp_proposed/Ti_proposed;
Td_proposed = 1/(2*dampingRatio*undampedNaturalFreq);
Kd_proposed = Kp_proposed*Td_proposed;
Clag = tf([1],[1 2/dampingRatio]);
Controller_proposed = pid(Kp_proposed,Ki_proposed,Kd_proposed);


openLoop = series(Controller_proposed*Clag,G);
figure('Name','step response of IMC controller')
step(feedback(openLoop,1));
figure('Name','frequency response of IMC controller')
margin(openLoop);

%model mismatch test(double the resistor)
resistor_changed = resistor*2;
G_loadchanged = tf([openLoopGain*openLoopZero openLoopGain],[1 1/resistor_changed/capacitor 1/inductor/capacitor]);
openLoop_changed = series(Controller_proposed,G_loadchanged);
figure('Name','step response of IMC controller with model mismatch (load doubled)')
step(feedback(openLoop_changed,1));

%compare with affine parameterization
%affine parameterization for 2nd order system with 1 zero.
G_minimalphase = tf([openLoopGain],[1 1/resistor_changed/capacitor (1-dutyCycle)^2/inductor/capacitor]);%system transfer function
closeLoopDampingRatio = 0.7;
closeLoopUndampedFreq = 0.1*undampedNaturalFreq;
alpha2 = 1/closeLoopUndampedFreq^2;
alpha1 = 2*closeLoopDampingRatio/closeLoopUndampedFreq;
closeloop_affine = tf([1],[alpha2 alpha1 1]);
Kp_affine = (2*undampedNaturalFreq*dampingRatio*alpha1-alpha2*undampedNaturalFreq)/(openLoopGain*alpha1^2);
Ki_affine = undampedNaturalFreq^2/openLoopGain/alpha1;
Kd_affine = (alpha1^2-2*undampedNaturalFreq*dampingRatio*alpha1*alpha2+alpha2^2*undampedNaturalFreq^2)/openLoopGain/alpha1^3;

Controller_affine = pid(Kp_affine,Ki_affine,Kd_affine);
figure('Name','step response of Affine parameterization')
step(feedback(Controller_affine*G,1));
figure('Name','frequency response of IMC controller')
margin(Controller_affine*G);

%simulink simulation
R = resistor;
L = inductor;
C = capacitor;
Vin = inputVoltage;
Vo = outputVoltage;
Fsw = 1e5;
Tsample = 1/Fsw/100;
Ttotal = 0.1;