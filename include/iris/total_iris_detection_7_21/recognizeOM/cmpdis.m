function distance=cmpdis(im1,im2,mask1,mask2)
%%im1,im2Ϊ��һ���Ĳ���ͼƬ��ѵ��ͼƬ��������ͼ
%%mask1,mask2Ϊ����ͼƬ��ѵ��ͼƬ��maskԭʼͼƬ
mask1=255-mask1;%��תmask
mask2=255-mask2;
trmask=(find(mask1==255));
temask=(find(mask2==255));
result=xor(im1,im2);
result(trmask)=1;
result(temask)=1;
result=result(:);
%��һ������
normval=length(result);
distance=sum(result)/normval;


