%this code is used after running "buck converter with PID control". It aims
%to use the data in workspace which is sent from scope in simulink to plot
%the graph for output current(from transfer function and switch model),
%output voltage and duty cycle.
T001 = ScopeData1.time*1e3;
I_L = ScopeData1.signals(1).values;
D = ScopeData1.signals(2).values;
i_L = ScopeData1.signals(3).values;
Vo = ScopeData1.signals(4).values;

%draw
figure(1)
subplot(4,1,1)%output current(from transfer function)
plot(T001,I_L)
box off;grid;axis tight
legend('\itI','Orientation','horizontal')%Ìí¼ÓÍ¼Àý
xlabel('ms','FontName','Times New Roman','FontSize',14);
ylabel('I','FontName','Times New Roman','FontSize',14);
ylim([0, 5.5])

subplot(4,1,2)%duty cycle
plot(T001,D)
box off;grid;axis tight
legend('\itD','Orientation','horizontal')
xlabel('ms','FontName','Times New Roman','FontSize',14);

subplot(4,1,3)%output current from switch mode
plot(T001,i_L)
box off;grid;axis tight
legend('\iti_{L}','Orientation','horizontal')
xlabel('ms','FontName','Times New Roman','FontSize',14);
ylabel('I','FontName','Times New Roman','FontSize',14);

subplot(4,1,4)%output voltage
plot(T001,Vo)
box off;grid;
legend('\itV_{o}','Orientation','horizontal')
xlabel('ms','FontName','Times New Roman','FontSize',14);
ylabel('V','FontName','Times New Roman','FontSize',14);
