clear all;
clc;

aa=[
0.8 15 15 	0.8491	0.8841	0.9096	0.9241	0.9224	0.9182	0.9156	0.9062
1.7 15 15 	0.8977	0.9105	0.9165	0.9224	0.9241	0.9241	0.9199	0.9165
2.8 15 15 	0.9045	0.9028	0.9037	0.9096	0.9071	0.9071	0.9054	0.902
3.5 15 15 	0.8892	0.8909	0.8917	0.8971	0.8909	0.8866	0.8858	0.8849
0.8 20 20 	0.85	0.8806	0.9062	0.9261	0.9199	0.9207	0.9122	0.9003
1.7 20 20 	0.8977	0.9113	0.9165	0.9261	0.9224	0.9224	0.9096	0.8943
2.8 20 20 	0.9054	0.9062	0.9071	0.9045	0.902	0.8934	0.8892	0.873
3.5 20 20 	0.8841	0.8841	0.8832	0.8781	0.8781	0.8755	0.8687	0.8593
0.8 25 25 	0.8491	0.8849	0.9096	0.9241	0.9261	0.9173	0.9139	0.9037
1.7 25 25 	0.8977	0.9096	0.9165	0.9216	0.9224	0.9207	0.9062	0.8883
2.8 25 25 	0.902	0.9028	0.9097	0.8986	0.8968	0.8892	0.8687	0.8602
3.5 25 25 	0.8806	0.8781	0.8789	0.8747	0.8704	0.8508	0.8363	0.8278
0.8 30 30 	0.85	0.8806	0.9062	0.9261	0.9199	0.9207	0.9122	0.8994
1.7 30 30 	0.8977	0.9113	0.9165	0.9261	0.9224	0.9224	0.9054	0.8875
2.8 30 30 	0.9045	0.9054	0.9045	0.8986	0.8968	0.8875	0.873	0.8551
3.5 30 30 	0.8824	0.8772	0.8789	0.8738	0.8645	0.8448	0.8295	0.8124
];
mean(aa)
mean(aa(:,4:end), 2)


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






