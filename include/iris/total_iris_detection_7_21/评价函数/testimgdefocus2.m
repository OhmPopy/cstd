clear 
clc
close all
%��ȡͼƬ
I = imread('D:\pub\bin\iris\���ۺ���\1.jpg');%����
if ndims(I)==3    
    I=rgb2gray(I);
end
I=double(I);
%�õ������
a=ones(5,5)*(-1);
b=ones(3,3)*2;
b(2,2)=0;
a(2:4,2:4)=b;
%ͼƬ����˲�
Img_n1 = conv2(I,a,'valid');
Img_n11=uint8(Img_n1);
Img_n11=abs(Img_n1);
score1=mean2(Img_n11)
%ͼƬ�Ҷ����ű��ں������÷�
%Img_n1=Img_n1(3:end-3, 3:end-3);
%img1=mat2gray(Img_n1);
%�����Ƶ�����÷�
%score1=mean2(img1)
%score3>score1>score2;����Խ������ͼƬ����Խ��.ͨ��ʵ����Եõ���ֵ
% figure;imshow(Img_n1,[]);title('�������ͼ')
