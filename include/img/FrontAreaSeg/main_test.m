% main
% edit: 2013.07.24
%% ǰ��������ȡ��������
ImerodeThresh=10; % ��ʴ��ֵ
ImdilateThresh=15; % ������ֵ
AreaThresh=1500; % �����ֵ

BRsize=12; % ǰ���޸���ֵ

%% �������
RN=25;
RD=5;

%% �˲�����
FilterLayerThresh=170; % ����˲�
FilterAreaThresh=700; % ����˲�
verson='Y'; % ѡ����и�ʴ

BI=imread('G:\matlab_test\peopledetect\testlargedata\depthBW\depth00221.JPG_fg_dep.bmp'); % ǰ����ֵͼ
DI=imread('G:\matlab_test\peopledetect\testlargedata\depth\depth00221.png'); % ���ͼ
DI=double(DI); % ����ת�� 
CI=imread('G:\matlab_test\peopledetect\testlargedata\color\color00221.jpg'); % ��ɫͼ
RDI=depthIrepair(DI,BRsize); % ǰ���޸�
FrontI=FrontAreaSeg(BI,ImerodeThresh,ImdilateThresh,AreaThresh); % ǰ��������ȡ
FRDI=RDI.*FrontI; % ǰ������
g=Rainclusteredit(FRDI,RN,RD); % ����
[FAI,LayerFilterI]=LayerAreaFilter(g,FilterLayerThresh,FilterAreaThresh,verson); % �˲�
