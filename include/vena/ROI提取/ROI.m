%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%�ó���������ȡ����ͼƬ����Ȥ���򣬵���ͼƬʵ��Ч���Ϻá�
%%��ȡ���裺
%1.��ȡͼƬ���Ҷȱ任����ת90��
%2.�����ü�ͼƬ����˹�˲�
%3.��Ե���
%4.���ݱ�Եͼ������ҵ���ָ�����ұ�Ե���Ӷ������ҳ�������ȥ������
%5.����д���ľ����ǶȺ���������ͼƬ�ĽǶ�
%6.�ؽڴ��߶Ƚ�ȡ���ҳ�ָ�ؽڴ����ٽ�ȡROI�߶ȡ�
%7.��ȡ��ȣ���ȡ��ROI����
%%%%%%%%%%%%%1.��ȡͼƬ+�Ҷȱ任%%%%%%%%%%%%%%%%%%%%%%%%%
%clear all;
clc;
tic;
shuru=imread('b.BMP');   %%��ȡͼƬ
%figure,imshow(shuru),title('ԭͼ��');
if ndims(shuru)==3
    shuru=rgb2gray(shuru);
end
[mm, nn]=size(shuru);
if mm<nn
    shuru=rot90(shuru);
end
%%%%%%%%%%%2.�����ü�ͼƬ����˹�˲�%%%%%%
shuru=shuru(80:480,:);
shuru2 = shuru;
shuru=double(shuru);
h=fspecial('gaussian');
shuru1=filter2(h,shuru);
%%%%%%%%%%%%%%3.��Ե���%%%%%%%%%%%%%%%%
BW2=edgecrop(shuru1,50);
% imshow(BW2);
% figure
% imshow(BW2,[]);
[MM, NN]=size(BW2);
colall=[];
%%%%4.���ݱ�Եͼ������ҵ���ָ�����ұ�Ե���Ӷ������ҳ�������ȥ������%%%%%
for i=1:MM
    COL=find(BW2(i,:)~=0);
    if length(COL)>1;
        col=[i COL(1) COL(2)];
        colall=[colall;col];
    end
end
oneing=zeros(MM,NN);
[result1,result2]=right_dis(colall);
if result1~=0&&result2~=0
    oneing(:,result1:result2)=1;
else
    oneing=ones(MM,NN);
end
shuru1=shuru1.*oneing;  %%ȥ����
shuru2=shuru1;
%%%%%%%%5.����д���ľ����ǶȺ���������ͼƬ�ĽǶ�%%%%%%%
%shuru1=correct_angle(shuru1);
% figure
% imshow(shuru1,[]);
% BW2=edgecrop(shuru1,100);
% figure
% imshow(BW2,[]);
%%%%%%%%6.�ؽڴ��߶Ƚ�ȡ���ҳ�ָ�ؽڴ����ٽ�ȡROI�߶�%%%%%%%
roih=roi_height(shuru1,BW2);
% figure
% imshow(roih,[]);
% title('��⵽�ĸ߶�');
% BW2=edgecrop(roih,50);
% figure
%imshow(BW2);
shuru=uint8(roih);
%%%%%%%%%%%%7.��ȡ��ȣ���ȡ��ROI����%%%%%%%%%%%%%%%%%%%%%
[min1, max1]=bianjie(BW2);
k=shuru(:,min1:max1);
%k=shuru(:,min1:max1);
%figure,imshow(k); title('ROI');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%�ߴ�Ҷȹ�һ��(����ͼ����) ��һ����С������������%%%%%%%
[m,n]=size(k);
s=[double(64/n) 0 0;
    0 double(96/m) 0;
    0 0 1];
tform=maketform('affine',double(s));
% k=imtransform(k,tform,'XData',[1 64],'YData',[1 96]); %%˫���Բ�ֵ
% k=imtransform(k,tform,'bilinear'); %%˫���Բ�ֵ
k=imtransform(k,tform,'XData',[1 64],'YData',[1 96],'FillValue',0);%%��0����
[m,n]=size(k);
%%%%%%%%%%%%%%%%%%%%%%%%%%
p=max(max(k(:,:)))-min(min(k(:,:)));
y=double(256/double(p));
k(:,:)=double((double(y)*double((k(:,:)-min(min(k(:,:)))))));  %%ֱ��ͼ��һ������
shuchu=k;
figure,imshow(shuchu,[]),title('ROIguiyihua');
toc;
