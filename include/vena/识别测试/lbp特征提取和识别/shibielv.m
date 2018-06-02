clc;
clear all;

%solve('a+x=x*x')
syms x a b;
solve('(a*tan(200*x) + b*tan(156.6*x))*tan(28*x)=a*b')

return ;


% path='D:\�ҵ��ĵ�\finger_vein\FV_samples\';
% file=dir(path);
% count=0;
% allfeature=[];
% allname=[];
% mapping=getmapping(8,'u2');
% for tt=3:length(file)
%     Iname=strcat(path,file(tt).name);
%     file(tt).name
%     I=imread(Iname);
%     feaimg=fenkuailbp(I,2,8,mapping,'h',8);
%     count=count+1
%     feaimg=single(feaimg(:)');
%    allfeature=[allfeature;feaimg];
%    name=file(tt).name(2:3);
%    allname=[allname;name];
% end
% save allfeature allfeature
% save name  allname
%%%%%%%%%������Լ���ע�Ἧ
clear all;
load allfeature;
load name;
allfeature=single(allfeature);
allname=str2num(allname);
classnum=unique(allname);
perimg=[];
for i=1:length(classnum)
    tmp=length(find(allname==classnum(i)));
    perimg=[perimg;tmp];
end
%%����ѵ�����Ͳ��Լ���
cnt=2;
cnting=1:cnt;
gallery=zeros(length(perimg),cnt);
gallery(1,:)=cnting;
for i=1:length(perimg)-1
    gallery(i+1,:)=gallery(i,:)+perimg(i);
end
gallery=gallery';
gallery=gallery(:);%ע�Ἧ���к�
test=1:length(allname);
test(gallery)=[];
test=test';    %���Լ��к�
galleryID=allname(gallery);  %%%%ע�Ἧ�Ͳ��Լ����
testID=allname(test);
galleryfea=allfeature(gallery,:); %%ע�Ἧ�Ͳ��Լ�����
testfea=allfeature(test,:);
clear allfeature;
%%%%%%ʶ�����%%%
ra=0;
mdistance=zeros(length(galleryID),1);
for k=1:size(testID,1)
    testing=testfea(k,:);
    for h=1:size(galleryID,1)
        gallering=galleryfea(h,:);
        %score=sum(min(double(testing),double(gallering)));  %����ֱ��ͼ�ཻ����ʶ��
        score=sum(abs(testing-gallering));  %abs sub
        mdistance(h,1)=score;
    end
    [value,index]=min(mdistance);
    if galleryID(index)==testID(k)
        ra=ra+1;
    end
end
rate=ra/length(testID)


