function  AUC=newroc(output,Y,weight)
%%output��ʾÿ��������ֱ��ͼ�ֲ��ϵ����յ÷֣�Y��ʾÿ�������ı�ǩ���������������
%%%%��������%%%%
% clear all;
% clc;
% % output=[1 2 3 4];
% % Y=[1 0 1 0];
% % weight=[0.25 0.25 0.25 0.25];
% output=rand(1,10);
% Y=[1  1 1 1 1 0 0 0 0 1];
% weight=[ 1/(12) 1/(12) 1/(12) 1/(12) 1/(12) 1/8 1/8 1/8 1/8 1/(12) ];
%%%%%%%%%
maxout=max(output);
minout=min(output);
sort_output = sort(output);

thresh = ([minout-1 sort_output] + [sort_output maxout+1])*0.5;

