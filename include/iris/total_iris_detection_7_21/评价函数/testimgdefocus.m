clear 
clc
close all
%��ȡͼƬ
I = imread('1.bmp');%����
if ndims(I)==3    
    I=rgb2gray(I);
end
I=double(I);%������������Ҫ
I1 = imread('2.bmp');%��ɢ��
if ndims(I1)==3    
    I1=rgb2gray(I1);
end
I1=double(I1);%�������Ҫ��Ϊdouble
I2 = imread('pic11.jpg');
if ndims(I2)==3    
    I2=rgb2gray(I2);
end
I2=double(I2);
%�õ������
a=ones(5,5)*(-1);
b=ones(3,3)*2;
b(2,2)=0;
a(2:4,2:4)=b;
%ͼƬ����˲�
Img_n1 = conv2(I,a,'same');
Img_n2 = conv2(I1,a,'same');
Img_n3 = conv2(I2,a,'same');
%ͼƬ�Ҷ����ű��ں������÷�
img1=mat2gray(Img_n1);
img2=mat2gray(Img_n2);
img3=mat2gray(Img_n3);
%�����Ƶ�����÷�
score1=mean2(img1);
score2=mean2(img2);
score3=mean2(img3);
%score3>score1>score2;����Խ������ͼƬ����Խ��.ͨ��ʵ����Եõ���ֵ
% figure;imshow(Img_n1,[]);title('�������ͼ')
% figure;imshow(Img_n2,[]);title('�������ͼ')
% figure;imshow(Img_n3,[]);title('�������ͼ')