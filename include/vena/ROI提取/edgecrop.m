%%%%��Ե���%%%%%% 
function BW2=edgecrop(shuru1,kk)
%%shuru1Ϊ��Ҫ����Ե��ͼ��kkΪlvboo�Ĳ���ֵ����ֵӰ��ܴ�ȥ��С��Ե
% [BW2,tv]=edge(double(shuru1),'sobel','vertical'); 
BW2=edge(shuru1);
% figure(2); 
% imshow(BW2)  
% se=strel('disk',2); 
% BW2=imclose(BW2,se);%������ 
% % % figure(3); 
% % % imshow(BW2) 
% %  BW2=bwmorph(BW2,'thin',Inf);%������ 
BW2=lvboo(BW2,kk);%ȥ������ 
% figure(4); 
% imshow(BW2) 