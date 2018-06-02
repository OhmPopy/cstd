clear all;
clc;
%%%%%%�ڶ����裺�������Ԥ������ͼƬ�Ķ�������%%%%%%%%%%%
load ordifilter2L;
[M,N]=size(ordifilterall);
rate=zeros(M,N);
for k=1:M
    for h=1:N
%%%load OMNEW;%%%%��ȡһ��ˮƽ�����������ż���ӣ������˲���
imgset=dir('D:\�ҵ��ĵ�\feat(2)\v2_iris\*.jpg');
maskset=dir('D:\�ҵ��ĵ�\feat(2)\V2_mask\*.jpg');
numberset=length(imgset);
allcell=cell(numberset,3);
for i=1:numberset
     imgfile=strcat('D:\�ҵ��ĵ�\feat(2)\V2_iris\',imgset(i).name);
     maskfile=strcat('D:\�ҵ��ĵ�\feat(2)\V2_mask\',maskset(i).name);
     masimg=imread(maskfile);
     masimg=255-masimg;
     img=imread(imgfile);
     imgname=imgset(i).name;
    imgname=imgname(2:6);   
     allcell{i,1}=makeom(img,ordifilterall{k,h});%��ź�Ĥ���϶�������ͼ����С��ԭͼһ����
     allcell{i,2}=imgname;   %��ź�Ĥ����ͼƬ����
     allcell{i,3}=masimg;    %����ڸ�ͼƬ
end
save allom3  allcell;
%%%%%%%%%%%%����ѵ�����ϣ����Լ���%%%%%%%%%%%%%%%%%%%%%%%%%%
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
load ID2;
load set2;%�ڶ��������˲���
galleryperson=length(galleryID);
testperson=length(testID);
ra=0;
mdistance=zeros(galleryperson,1);
for k=1:size(testID,1)
    testing=testfea(k);
    testing=testing{1,1};
    tmask=testmask(k);
    tmask=tmask{1,1};
    tmorder=(find(tmask==255));%����ͼƬ�����ǵ�����
   for h=1:size(galleryID,1)
       gallery=galleryfea(h);
       gallery=gallery{1,1};
       gmask=gallerymask(h);
       gmask=gmask{1,1};
       gmorder=(find(gmask==255));%ע��ͼƬ�����ǵ�����
       score=xor(testing,gallery);
       score(tmorder)=1;
       score(gmorder)=1;
        score=score(:);
        normval=length(score);
        mdistance(h,1)=sum(score)/normval;
   end
   [value,index]=min(mdistance);
   if galleryID(index)==testID(k)
       ra=ra+1;
   end
end
rate(k,h)=ra/testperson;
    end
end

     
     
     
     