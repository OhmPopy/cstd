%%%%ͼ��ͨ������Ķ��ֶ����˲�������ͼ��%%%%%%%%%
%%%%��֮ǰ�Ѿ�����Ķ��ֶ����˲�load���У��ļ�ordifilter2L��ordifilter3L��׺��23��ʾ�˸���,L��ʾˮƽ����%%%%%%%
%%%%%��������ֵallfimgΪһ��Ԫ�����������ж����˲���ͼƬ
function allfimg=main(im)
%%%���Ըú���%%%%%%%%%%%
% clear all;
% clc;
% im=imread('1.bmp');
% im=double(im);
%%%%%%%%%%%%%%%%%%%%%%%  
file='C:\Documents and Settings\Administrator\����\�贫���ļ�\OMkernel\';
sita=cell(1,1);
sita{1}='L'; 
allfimg=[];
for k=1:2  %���ֺ�
    allimg=[];   
OM=load(strcat(file,'ordifilter',num2str(k+1),sita{1}));
OM=OM.ordifilterall;
[m n]=size(OM);
fimg=cell(m,n);
allimging=cell(m*n,1);
for i=1:m
    for j=1:n
        fimg{i,j}=makeom(im,OM{i,j});%%%�˲����ͼƬ%%
        allimging{(i-1)*n+j,1}=fimg{i,j};
    end
end
allimg=[allimg; allimging];
allfimg=[allfimg;allimg];
end
%%����ͼƬ�˲���ͼ��%%
% for i=1:length(allfimg)
%     savefile=strcat('D:\�ҵ��ĵ�\om1\',num2str(i),'.bmp');
%     tt=allfimg{i,1};
%    imwrite(tt,savefile);
% end
%%%%%%%%%%%%%%%%%%%%



