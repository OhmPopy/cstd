
% A 10Ԫ�ı� Ӯ�� 60%
% B 1000Ԫ�ı� Ӯ�� 40%

clc
c=0;
n=100;
for i=1:n,
    a=1;
    b=10;
    while a>0 && b>0,
        r = rand(1);
        t = double(r<0.51)*2-1;
        a = a+t;
        b = b-t;
    end
    c = c + (a>0);
end
c*1./n