clear all;
clc;
load swdislbp;
load sbdislbp;

interclass=swdistance;
extraclass=sbdistance;

labelinter=ones(length(interclass),1);%�������ھ��볤��*1�ĵ�λ����
labelextra=zeros(length(extraclass),1);%���������볤��*1�������
Y=[labelinter;labelextra];
feature=[interclass;extraclass];
[FAR FRR]=rocp(feature,Y);%��ɫ
hold on
plot(0:1,0:1,':');
grid on;
box on
%rocplot(feature,Y)