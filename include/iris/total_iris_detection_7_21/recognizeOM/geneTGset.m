clear all;
clc;
%%%%�������Լ��Ϻ�ע�Ἧ��%%%%%%%%
load allom3;
numimg=length(allcell); %�ܵ�ͼƬ����
nameall=cell(numimg,1);
numall=[];   %����������ÿ��ͼƬ�ı��
for i=1:numimg
    nameall{i,1}=allcell{i,2};
    nameall{i,1}=strrep(nameall{i,1},'L','0');
    nameall{i,1}=strrep(nameall{i,1},'R','1');
    numall=[numall;str2double(nameall{i,1})];
    temp=allcell{i,1};
end
classnum=unique(numall); %���������ͼƬ�����ţ���552�����
perimg=[];    %�ó�ÿ�����ͼƬ������
for i=1:length(classnum)
    temp=length(find(numall==classnum(i)));
    perimg=[perimg;temp];
end
%%%%����ÿ�������ѡȡ��ע�Ἧ�е�ͼƬ����
cnt=2;%ÿ�����ѡ����*******************************************   
cnting=1:cnt;
gallerynum=zeros(length(perimg),cnt);
gallerynum(1,:)=cnting;
for i=1:(length(perimg)-1)
       gallerynum(i+1,:)=gallerynum(i,:)+perimg(i);
end
gallerynum=gallerynum';
gallerynum=gallerynum(:); %%��Ҫѡ����ע�Ἧ���е��к�
testnum=1:length(numall);
testnum(gallerynum)=[];
testnum=testnum';       %%%ѡ���Ĳ��Լ��к�
save select2 gallerynum testnum %%%����Ĳ��Լ���ע�Ἧѡ�����к�
galleryID=numall(gallerynum);
testID=numall(testnum);
save ID2 galleryID testID    %%%%����Ĳ��Լ���ע�Ἧѡ����ID��
galleryfea=allcell(gallerynum,1);
testfea=allcell(testnum,1);
gallerymask=allcell(gallerynum,3);
testmask=allcell(testnum,3);
save set2 galleryfea testfea gallerymask testmask; %%%%������Լ���ע�Ἧ
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%






    