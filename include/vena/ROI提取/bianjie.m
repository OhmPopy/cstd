 function [minn,maxx]=bianjie(h1) 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
%�����������h1Ϊһ����ֵͼ�� 
%�������minn��maxx�ֱ��ʾͼ������������е��������������ұ������е���С������ 
%h1=BW2;
m=[]; 
[g,num]=bwlabel(h1,8); 
% num 
for i=1:num 
    [r,c]=find(bwlabel(h1)==i); 
    m(i)=max(c);
    m(i+1)=min(c);
end 
%save('num.mat','num');
     
 
if m(1)>m(3) 
    minn=m(2); 
    maxx=m(4); 
else 
    minn=m(1); 
    maxx=m(3); 
end