function [natural_code, fold_code] = func3(n)
% 将所有信号量量化到16个相等的量化区间
% 信号量的范围为 -1:1/8:1 所规定的8个区间。
% 参数n表示采样信号的数量
natural_code = zeros(n, 4);
fold_code = zeros(n, 4);
nature = [0 0 0 0; 0 0 0 1; 0 0 1 0; 0 0 1 1; 0 1 0 0; 0 1 0 1; 0 1 1 0; 0 1 1 1;1 0 0 0; 1 0 0 1; 1 0 1 0; 1 0 1 1; 1 1 0 0; 1 1 0 1; 1 1 1 0; 1 1 1 1];
fold = [0 1 1 1; 0 1 1 0; 0 1 0 1; 0 1 0 0; 0 0 1 1; 0 0 1 0; 0 0 0 1; 0 0 0 0;1 0 0 0; 1 0 0 1; 1 0 1 0; 1 0 1 1; 1 1 0 0; 1 1 0 1; 1 1 1 0; 1 1 1 1];

S = func2(16);

for i = 1:n
    serial = (S(i) + 0.9375)*8;
    for k = 1:4
        natural_code(i, k) = nature(serial+1, k);
        fold_code(i, k) = fold(serial+1, k);
    end
    fprintf("量化值: %.4f, 自然码:", S(i));
    disp(natural_code(i,:));
    fprintf("折叠码: ");
    disp(fold_code(i,:));
    fprintf("\n");
end
end

