clear all;
clc;
%%%%%%��һ���裺�Ƚ�feat��mask������ͼƬ�����봦��,��д���µ��ļ�����%%%%%%%%
%ԭͼ��·��%%%%
% orimgset=dir('D:\�ҵ��ĵ�\feat(2)\V2_normalize\*.jpg');
% numberset=length(orimgset);
% %����ͼ��·��
% covimgset=dir('D:\�ҵ��ĵ�\feat(2)\V2_mask\*.jpg');
% % savefile='D:\�ҵ��ĵ�\feat(2)\V2_iris';
% for i=1:numberset
%     orifile=strcat('D:\�ҵ��ĵ�\feat(2)\V2_normalize\',orimgset(i).name);
%     covfile=strcat('D:\�ҵ��ĵ�\feat(2)\V2_mask\',covimgset(i).name);
%     orimg=imread(orifile);
%     coimg=imread(covfile);
%     fimg=makeom(orimg,coimg);
%     imname=orimgset(i).name;
%    savefile=strcat('D:\�ҵ��ĵ�\feat(2)\V2_iris\',num2str(imname));
%    imwrite(fimg,savefile);
% end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%�ڶ����裺�������Ԥ������ͼƬ�Ķ�������%%%%%%%%%%%
load OM3;%%%%��ȡһ��ˮƽ�����������ż���ӣ������˲���
imgset=dir('D:\�ҵ��ĵ�\feat(2)\V2_iris\*.jpg');
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
     allcell{i,1}=makeom(img,ordifilter);%��ź�Ĥ���϶�������ͼ����С��ԭͼһ����
     allcell{i,2}=imgname;   %��ź�Ĥ����ͼƬ����
     allcell{i,3}=masimg;    %����ڸ�ͼƬ
end
save allom3  allcell;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%�������裺�����ں����Ժ�������%%%%%%%%%%%%%
% load allom2;
% numimg=length(allcell);
% sw=[];
% sb=[];
% %minsb=1000;
% for i=1:(numimg-1)
%     for j=(i+1):numimg
% %           score=xor(allcell{i,1},allcell{j,1});
% %           morder1=find(allcell{i,3}==255);
% %           morder2=find(allcell{j,3}==255);
% %           score=score(:);
% %           score(morder1)=1;
% %           score(morder2)=1;
% %           dis=sum(score)/length(score);
% %           dis=single(dis);
% %           clear score morder1 morder2 ;
%       if  strcmp(allcell{i,2},allcell{j,2})==1 
%           score=xor(allcell{i,1},allcell{j,1});
%           morder1=find(allcell{i,3}==255);
%           morder2=find(allcell{j,3}==255);
%           score=score(:);
%           score(morder1)=1;
%           score(morder2)=1;
%           dis=sum(score)/length(score);
%           dis=single(dis);
%           clear score morder1 morder2 ;
%           sw=[sw;dis];
%       else
%           score=xor(allcell{i,1},allcell{j,1});
%           morder1=find(allcell{i,3}==255);
%           morder2=find(allcell{j,3}==255);
%           score=score(:);
%           score(morder1)=1;
%           score(morder2)=1;
%           dis=sum(score)/length(score);
%           dis=single(dis);
%           if dis<0.3945
%              % minsb=dis;
% %               indexi=allcell{i,2};
% %               indexj=allcell{j,2};
%                  indexi=i;
%                  indexj=j;
%           end         
%          sb=[sb;dis];
%       end         
%     end
% end
% % % save sw sw;
% % save sb sb;
% % save index indexi indexj;

     
     
     
     