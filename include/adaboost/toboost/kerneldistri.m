function  result=kerneldistri(inputscore,input,bandwith)
%%%%��˹���ܶȷֲ�%%
% clear all;
% clc;
% inputscore=0.99;
% input=rand(1,100);
% % [input,index]=sort(input);
% weight=ones(1,100);
% % weight=weight(index);
%%%%
%%%inputΪ�������inputscoreΪһ����ֵ
 nsample=length(input);
%  bandwith=0.0000015;%%0.0000015
 zongfkernel=0;
 for i=1:nsample
 distance=(inputscore-input(i)).^2;
 fkernel=1/(bandwith.*sqrt(2*pi))*exp(-(distance)./(2*bandwith^2));
 zongfkernel=zongfkernel+fkernel;
 end
 result=zongfkernel/nsample;
% result=zongfkernel;

