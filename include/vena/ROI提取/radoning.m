function angle=radoning(img)
%%����radon�ں���Ѱ����б��%����Ϊһ��ͼ��ı�Եͼedge%
theta=1:179;
[R,XP]=radon(img,theta);
[index,jj]=find(R>=max(R(:)));
if jj<=90
    angle=jj;
else
    angle=jj-180;
end


