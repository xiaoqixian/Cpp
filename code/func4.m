function code = func4()
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
t = 0:1/8000:1;
x1 = 1024*cos(900*pi*t);
z = sign(x1);
Maxs = max(abs(x1));
x1 = abs(x1/Maxs);
Q = 2048*x1;
code = zeros(length(x1), 8);
paras = [0 0 0;0 0 1;0 1 0;0 1 1;1 0 0;1 0 1;1 1 0;1 1 1];
areas = [0 0 0 0;0 0 0 1;0 0 1 0;0 0 1 1;0 1 0 0;0 1 0 1;0 1 1 0;0 1 1 1;1 0 0 0;1 0 0 1;1 0 1 0;1 0 1 1;1 1 0 0;1 1 0 1;1 1 1 0;1 1 1 1];
for i = 1:8000
    if z(i) > 0
        code(i,1) = 1;
    else
        code(i,1) = 0;
    end
    index = calc_para(Q(i));
    code(i,2) = paras(index,1);
    code(i,3) = paras(index,2);
    code(i,4) = paras(index,3);
    num = calc_area(Q(i) - 2^index, index);
    code(i,5) = areas(num, 1);
    code(i,6) = areas(num, 2);
    code(i,7) = areas(num, 3);
    code(i,8) = areas(num, 4);
    fprintf("Q(i): %.4f. PCM码:", Q(i));
    disp(code(i,:));
    fprintf("\n");
end
%plot(t,x1);
end

function index = calc_para(val)
% 根据值val计算相应的段落
index = 1;
for i = 4:11
    if 2^i > val
        index = i - 3;
        break;
    end
end
end

function num = calc_area(val, index)
    len = 2^index/16;
    num = 1;
    for i = 1:16
        if val < len*i
            num = i;
            break;
        end
    end
end