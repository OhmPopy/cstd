%%%%%%��Ĥ��������ʶ��%%%%%%%%%
clear all;
clc;
% load  set;%��һ�������˲���
% tfeature=testfea;
% gfeature=galleryfea;
load ID2;
load set2;%�ڶ��������˲���
galleryperson=length(galleryID);
testperson=length(testID);
ra=0;
mdistance=zeros(galleryperson,1);
for k=1:size(testID,1)
    testing=testfea(k);
    testing=testing{1,1};
%     tmask=testmask(k);
%     tmask=tmask{1,1};
%     tmorder=(find(tmask==255));%����ͼƬ�����ǵ�����
   for h=1:size(galleryID,1)
       gallery=galleryfea(h);
       gallery=gallery{1,1};
%        gmask=gallerymask(h);
%        gmask=gmask{1,1};
%        gmorder=(find(gmask==255));%ע��ͼƬ�����ǵ�����
       score=xor(testing,gallery);
%       score(tmorder)=1;
%       score(gmorder)=1;
        score=score(:);
%         normval=length(score);
%         mdistance(h,1)=sum(score)/normval;
         mdistance(h,1)=sum(score);
   end
   [value,index]=min(mdistance);
   if galleryID(index)==testID(k)
       ra=ra+1;
   end
end
rate=ra/testperson
    
