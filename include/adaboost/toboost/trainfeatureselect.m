%%%%����ѡ��%%%%%%%%%%%%%
clear all;
clc;
T=100;
selectFeaSize=-1;%����ѡȡ����Ŀ��С��0�Ļ�����ѭ��������������ѡȡ
outputTips=1;
%%%%%%%%%%%%%%���Ը÷���������������ľ����еķ�������%%%%%%%%%%%%%%%%%
% aa=randn(100,100);
% bb=randn(100,100)+0.0009;
% ya=ones(1,100);
% yb=zeros(1,100);
% label=[ya yb];
% Y=label;
% feature=[aa;bb];
%%%%%%%%%%%%
% feature=zeros(5600,1475);%lbp����������Ϊ1475
% aa=floor(11000*rand(1,5000));%����������ѡȡ�����ڼ�ͼƬ����ȵ�����
% filesave=strcat('D:\�ҵ��ĵ�\MATLAB\swdislbp','.mat');
% interclass=load(filesave,'swdistance');
% interclass=interclass.swdistance;
% filesave2=strcat('D:\�ҵ��ĵ�\MATLAB\sbdislbp','.mat');
% extraclass=load(filesave2,'sbdistance');
% extraclass=extraclass.sbdistance;
% extraclass=extraclass(aa,:);
% numinter=size(interclass,1);
% numextra=size(extraclass,1);
% feature(1:600,:)=interclass;
% feature(601:(numinter+numextra),:)=extraclass;
% clear extraclass interclass;
% labelinter=ones(numinter,1);
% labelextra=zeros(numextra,1);
% Y=[labelinter;labelextra];
% Y=Y';
%%%%%%
% feature=zeros(200,1475);%lbp����������Ϊ1475
% aa=floor(11000*rand(1,100));%����������ѡȡ�����ڼ�ͼƬ����ȵ�����
% bb=floor(500*rand(1,100));
% filesave=strcat('D:\�ҵ��ĵ�\MATLAB\swdislbp','.mat');
% interclass=load(filesave,'swdistance');
% interclass=interclass.swdistance;
% interclass=interclass(bb,:);
% filesave2=strcat('D:\�ҵ��ĵ�\MATLAB\sbdislbp','.mat');
% extraclass=load(filesave2,'sbdistance');
% extraclass=extraclass.sbdistance;
% extraclass=extraclass(aa,:);
% numinter=size(interclass,1);
% numextra=size(extraclass,1);
% feature(1:100,:)=interclass;
% feature(101:(numinter+numextra),:)=extraclass;
% clear extraclass interclass;
% labelinter=ones(numinter,1);
% labelextra=zeros(numextra,1);
% Y=[labelinter;labelextra];
% Y=Y';         %%%����kerneldistri�����еĴ���Ϊ bandwith=0.0000015;
aa=load('C:\Documents and Settings\Administrator\����\toboost��\test\data');
Y=aa.Y;
feature=aa.feature;
%%%%%%%%%%%%%%%%%
disp('adboost��ʼ')
BoostSeriesFeaSelect(feature,Y,T,selectFeaSize,outputTips)
% [SelectFeaNo,costTime,SelectFeaName]=AdaBoostBasedSingleFeaSelect(feature,Y,T,selectFeaSize);
% ����selectFeaSize��Чʱ��ѡȡָ������������
% ���򣬰�����Tѭ��
% features: ѵ������
% Y: ѵ���������
% feaSelectRate: ѡ������ռ����������
% trainingRate: ѵ������ռ����������
% T: ѵ������
% selectFeaSize: Ҫѡ���������Ŀ
% testTimes: ���Դ���
