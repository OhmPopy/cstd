function rotaimg=correct_angle(shuru)
%%%%%%%%����бͼ�������б��У��%%%%%���룺ͼ���Ե%%%
if ndims(shuru)==3
        shuru=rgb2gray(shuru);
end 
% [mm nn]=size(shuru);
% if mm<nn
%     shuru=rot90(shuru);
% end
%  shuru=shuru(40:300,:);%%���þ��ο��ͼƬ
%%��ֵ�˲�
% h=fspecial('average'); 
% shuru1=filter2(h,shuru); 
% shuru1=medfilt2(shuru); %%��ֵ�˲�
%��Ե��� 
BW2=edge(shuru1);
se=strel('disk',2); 
BW2=imclose(BW2,se);%������ 
%BW2=bwmorph(BW2,'thin',Inf);%������ 
BW2=lvboo(BW2,100);%ȥ������ 
%%����б�ǣ���У��ͼƬ��������б�ǣ�%%%%%%%
angle=radon(BW2);%����radon�任����б��
if abs(angle)>1
% rotaimg=imrotate(shuru,-angle,'bilinear','crop');
rotaimg=imrotate(shuru,-angle,'bilinear','crop');  %%�ǶȾ���
 else
     rotaimg=shuru;
end

