function t = func1()
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
t = 0:1/1000:0.4;
y = sin(20*pi*t);
plot(t,y,'r');
hold on;
t = 0:0.03125:0.4;
y = sin(20*pi*t);
stem(t,y,'b','filled');
end