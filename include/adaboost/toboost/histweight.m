function [yresult,xresult,disval]=histweight(input,n,weight)
%input ��һ�����������
%nΪ�������������Ϊn�����䣬ͳ��ÿ������ķֲ�ֱ��ͼ
%weightΪ��������������Ȩ��ֵ���������û��weight��������ֱ��ͼͳ�Ʒֲ�����ֱ���õ���hist.m����
lengthin=length(input);
 maxin=max(input);
 minin=min(input);
 disval=(maxin-minin)/n;
 for i=1:n
     xresult(i)=minin+disval*i;
 end
 yresult=zeros(1,n);
    for j=1:lengthin  
        if input(j)<xresult(1)
           yresulting=1*weight(j);
           yresult(1)=yresult(1)+yresulting;
        else  
           i=floor((input(j)-xresult(1))/disval)+1;
           yresulting2=1*weight(j);
           yresult(i)=yresult(i)+yresulting2;
        end       
    end