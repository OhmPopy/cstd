function H=householder(x)
%householder�任
n=length(x);
I=eye(n);
e1=I(1:n,1);%e1�ǵ�λ������
u=(x-norm(x)*e1)/norm(x-norm(x)*e1);%u�ǵ�λ������
H=I-2*(u)*u';