%%%%�����Ϊ3�����ӣ�����Ϊ10
clear all;
clc;
delta=1.7;  %�߶�
dis=10;     %����
width=31;
z=makekernel(width,-dis,0,delta);
z1=makekernel(width,0,0,delta);
z2=makekernel(width,dis,0,delta);
ordifilter=z-2*z1+z2;
save OM3 ordifilter;
%%%%��ȡ����%%ʾ������%%
load OM3;
im=imread('1.bmp');
fimg=makeom(im,ordifilter); %��ȡ��������ͼ
aa=ordifilter(16, :);
bb=ordifilter./(ones(31, 1)*aa);
round([bb(:,16)';aa]*(2^8))
imshow(fimg);
