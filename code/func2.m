function S = func2(n)
% 对10Hz的正弦波进行32Hz的抽样，分别进行8级、256级和2048级均匀量化
% 参数n表示量化级数
%   Detailed explanation goes here
t = 0:0.03125:0.5;
y = sin(20*pi*t);
a = -1:2/n:1;
S = zeros(1,17);
T = zeros(1,17);
for i = 1:17
    %fprintf("i = %d, y(i) = %.4f\n",i,y(i));
    for j = 1:n
        %fprintf("a(j) = %.4f\n", a(j));
        if (y(i) >= a(j) && y(i) <= a(j+1))
            %fprintf("y(i) = %.4f, j = %d\n",y(i), j);
            S(i) = (a(j)+a(j+1))/2;
            T(i) = S(i) - y(i);
            break;
        end
    end
end
%stem(t,y,'r');
%hold on;
%stairs(t,S);
%stairs(t,T);
end