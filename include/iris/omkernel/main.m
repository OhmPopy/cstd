%%%%ͼ��ͨ������Ķ��ֶ����˲�������ͼ��%%%%%%%%%
%%%%��֮ǰ�Ѿ�����Ķ��ֶ����˲�load���У��ļ�ordifilter2L��ordifilter3L��׺��23��ʾ�˸���,L��ʾˮƽ����%%%%%%%
%%%%%��������ֵallfimgΪһ��Ԫ�����������ж����˲���ͼƬ
%function allfimg=main(im)
%%%���Ըú���%%%%%%%%%%%
% clear all;
% clc;
% im=imread('1.bmp');
% im=double(im);
%%%%%%%%%%%%%%%%%%%%%%%
%im=imread('D:\pub\bin\iris\testPic\feat\S2001L01F.bmp');
tt=(15+20+25+30)*32;
im=imread('D:\pub\bin\iris\omkernel\1.bmp');
file='.\';
sita=cell(1,1);
sita{1}='L';
allfimg=[];
if 0
    size=15;
    x=[-size:size];
    delta1=5.5;
    d=exp(-x.*x/(2*delta1*delta1));
    d=d'*d;
    d=d/sum(sum(d));
    im2=imfilter(double(im)/256.,d,'replicate','same');
    imwrite(im2, 'C:\aaa.bmp');
    d1 = [-1 0 0 0 1];
    ttt=2^8;
    im2=floor(im2*ttt);
    im2=im2/ttt;
    %im2 = imread('C:\ttt.bmp'); im2 = double(im2)/256.;
    im3=imfilter(im2,d1,'replicate','same');
    imshow(im3<0);
end
if 1
    for k=1:2  %���ֺ�
        allimg=[];
        OM=load(strcat(file,'ordifilter',num2str(k+1),sita{1}));
        OM=OM.ordifilterall;
        [m n]=size(OM);
        fimg=cell(m,n);
        allimging=cell(m*n,1);
        tic;
        for i=1:m
            for j=1:n
                fimg{i,j}=makeom(im,OM{i,j});%%%�˲����ͼƬ%%
                imshow(fimg{i,j});
                allimging{(i-1)*n+j,1}=fimg{i,j};
            end
        end
        toc;
        allimg=[allimg; allimging];
        allfimg=[allfimg;allimg];
    end
end
%%����ͼƬ�˲���ͼ��%%
% for i=1:length(allfimg)
%     savefile=strcat('D:\�ҵ��ĵ�\om1\',num2str(i),'.bmp');
%     tt=allfimg{i,1};
%    imwrite(tt,savefile);
% end
%%%%%%%%%%%%%%%%%%%%



