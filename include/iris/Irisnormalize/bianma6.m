%%�����˲���,���Ƕ���ת��
%%�ȼ���һ������˹�˲�����Ȼ��ͼ���˲������ͨ���еľ������
function A_f=bianma6(A)

[a b]=size(A);

fn=5;
alf=1.7;
dist=6;%%��Ҫ�����Ĳ���

H1=fspecial('gaussian',fn,alf);


A_j1 = imfilter(A,H1);


A_j2=A_j1;
for i=1:(b-dist)
    for j=1:(a-dist)
        A_j2(j,i)=A_j1(j,i)-A_j1(j+dist,i+dist);
    end
end

%%��ֵ��
for i=1:a
    for j=1:b
        if A_j2(i,j)>0
            A_f(i,j)=1;
        else
            A_f(i,j)=0;
        end
    end
end
