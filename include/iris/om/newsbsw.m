clear all;
clc;
load ordifilter2L;
[M,N]=size(ordifilterall);
SW=[];
SB=[];
cnsw=0;cnsb=0;
%%%����adaboost����ǰʮ��ͼƬ%%%
for pp=1:36      %%%%���������SBNEW SWNEW
    for qq=(pp+1):37
        sw=[];
        sb=[];
       for k=1:4:16  %%%�˲�ˮƽ���˵�ѡ��
           for h=1:2
%%%load OMNEW;%%%%��ȡһ��ˮƽ�����������ż���ӣ������˲���
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
    imgname=imgname(2:8);   
     allcell{i,1}=makeom(img,ordifilterall{k,h});%��ź�Ĥ���϶�������ͼ����С��ԭͼһ����
     allcell{i,2}=imgname;   %��ź�Ĥ����ͼƬ����
     allcell{i,3}=masimg;    %����ڸ�ͼƬ
end
        score=xor(allcell{pp,1},allcell{qq,1});
        morder1=find(allcell{pp,3}==255);
        morder2=find(allcell{qq,3}==255);
        score(morder1)=1;
        score(morder2)=1;
        dis=makeblock(score);  %����ͼ�ֿ�
        dis=single(dis);
        if  strcmp(allcell{pp,2}(2:6),allcell{qq,2}(2:6))==1     %%���ھ���
%            swdis=makeblock(score);
           sw=[sw dis'];
           cnsw=cnsw+1
           swname{cnsw,1}=strcat(allcell{pp,2},allcell{qq,2});
           %SW=[SW;sw];
        else
            sb=[sb dis'];
            cnsb=cnsb+1
            sbname{cnsb,1}=strcat(allcell{pp,2},allcell{qq,2});
%             if dis<305
%                 indexi=allcell{i,2}
%                 indexj=allcell{j,2}
%             end
           % SB=[SB;sb];
        end
    end
       end
SW=[SW;sw];
SB=[SB;sb];
    end
end
            
           
                   
                    
            
        
        
