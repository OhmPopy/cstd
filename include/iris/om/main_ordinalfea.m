%%������ȡ��������ʶ��%%%%%%%%
clear all;
clc;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% load ordifilter2L;
% for k=1:4:16 %%k=13 % %%%�˲��˵�ѡ��
%       for h=1:2 %%h=2 %
%  %%%load OMNEW;%%%%��ȡһ��ˮƽ�����������ż���ӣ������˲���
% imgset=dir('D:\�ҵ��ĵ�\feat(2)\V2_iris\*.jpg');
% maskset=dir('D:\�ҵ��ĵ�\feat(2)\V2_mask\*.jpg');
% numberset=length(imgset);
% allcell=cell(numberset,3);
% for i=1:numberset
%      imgfile=strcat('D:\�ҵ��ĵ�\feat(2)\V2_iris\',imgset(i).name);
%      maskfile=strcat('D:\�ҵ��ĵ�\feat(2)\V2_mask\',maskset(i).name);
%      masimg=imread(maskfile);
%      masimg=255-masimg;
%      img=imread(imgfile);
%      imgname=imgset(i).name;
%     imgname=imgname(2:6);   
%      allcell{i,1}=single(makeom(img,ordifilterall{k,h}));%��ź�Ĥ���϶�������ͼ����С��ԭͼһ����
%      allcell{i,2}=(imgname);   %��ź�Ĥ����ͼƬ����
%      allcell{i,3}=single(masimg);    %����ڸ�ͼƬ
% end
% %%%%%%%%%%%%����ѵ�����ϣ����Լ���%%%%%%%%%%%%%%%%%%%%%%%%%%
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% numimg=length(allcell); %�ܵ�ͼƬ����
% nameall=cell(numimg,1);
% numall=[];   %����������ÿ��ͼƬ�ı��
% for i=1:numimg
%     nameall{i,1}=allcell{i,2};
%     nameall{i,1}=strrep(nameall{i,1},'L','0');
%     nameall{i,1}=strrep(nameall{i,1},'R','1');
%     numall=[numall;str2double(nameall{i,1})];
%     temp=allcell{i,1};
% end
% classnum=unique(numall); %���������ͼƬ�����ţ���552�����
% perimg=[];    %�ó�ÿ�����ͼƬ������
% for i=1:length(classnum)
%     temp=length(find(numall==classnum(i)));
%     perimg=[perimg;temp];
% end
% %%%%����ÿ�������ѡȡ��ע�Ἧ�е�ͼƬ����
% cnt=2;%ÿ�����ѡ����*******************************************   
% cnting=1:cnt;
% gallerynum=zeros(length(perimg),cnt);
% gallerynum(1,:)=cnting;
% for i=1:(length(perimg)-1)
%        gallerynum(i+1,:)=gallerynum(i,:)+perimg(i);
% end
% gallerynum=gallerynum';
% gallerynum=gallerynum(:); %%��Ҫѡ����ע�Ἧ���е��к�
% testnum=1:length(numall);
% testnum(gallerynum)=[];
% testnum=testnum';       %%%ѡ���Ĳ��Լ��к�
% % %%save select2 gallerynum testnum %%%����Ĳ��Լ���ע�Ἧѡ�����к�
% galleryID=numall(gallerynum);
% testID=numall(testnum);
% save ID2 galleryID testID    %%%%����Ĳ��Լ���ע�Ἧѡ����ID��
% galleryfea=allcell(gallerynum,1);
% galleryall{(ceil(k/4)-1)*2+h,1}=galleryfea;
% testfea=allcell(testnum,1);
% testall{(ceil(k/4)-1)*2+h,1}=testfea;
% gallerymask=allcell(gallerynum,3);
% testmask=allcell(testnum,3);
% clear allcell galleryfea testfea;
%            end
%   end
% %save set2 galleryfea testfea gallerymask testmask; %%%%������Լ���ע�Ἧ
%save gallerymask testmask;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 load ID2;
 load testall;
 load galleryall;
 load gmask;
 load tmask;
galleryperson=length(galleryID);
testperson=length(testID);
ra=0;
for tk=1:size(testID,1) 
    disall=[];
    for i=1:8
   dis=zeros(galleryperson,259);
    testsub=test{i,1}(tk);
    testing=testsub{1,1};
    tmask=testmask(tk);
    tmask=tmask{1,1};
    tmorder=(find(tmask==255));%����ͼƬ�����ǵ�����
   for th=1:size(galleryID,1)
       gallerysub=gallery{i,1}(th);
       gallerying=gallerysub{1,1};
       gmask=gallerymask(th);
       gmask=gmask{1,1};
       gmorder=(find(gmask==255));%ע��ͼƬ�����ǵ�����
       score=xor(testing,gallerying);
       score(tmorder)=1;
       score(gmorder)=1;
       dis(th,:)=single(makeblock(score));  %����ͼ�ֿ�
   end
   disall=[disall dis];
    end
    [value,index]=min(disall);
    if galleryID(index)==testID(tk)
        ra=ra+1;
    end    
end
rating=ra/testperson

  
