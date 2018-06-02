% ����֡�ľ��룬Frame1��Frame2����Ϊ�ṹ�壬��������ΪCentroid
% ���صľ���d������֮��ľ���
function d=Frame_Euler(Frame1,Frame2)
N1=length(Frame1);
N2=length(Frame2);
d=zeros(N2,N1);
for i=1:N2
    for j=1:N1
        d(i,j)=norm(Frame2(i).Centroid-Frame1(j).Centroid);
    end
end
end