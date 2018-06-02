function  [FAR FRR]=rocp(score,lable)
score=score(:);
lable=lable(:);
Nthr=100;
thresh=linspace(min(score),max(score),Nthr);%��min��max�ֳ�100���������
acc=zeros(Nthr,1);
FAR=acc;
FRR=acc;
for i=1:Nthr
    thr=thresh(i);
    result=score>thr;
    FAR(i)=(sum((result==1)&lable==0))/sum(lable==0);%�������  FARΪ���������������������  ���
    %lableΪ�����ǩ������Ϊ1�����Ϊ0.
    %�����������ֵ���������
    %������С����ֵ������ܾ�

    FRR(i)=(sum((result==0)&lable==1))/sum(lable==1);%����ܾ�  FRRΪ����ܾ��������ڲ�������  ����
end

%xlim([10^(-6) 10^(-2)]);
%ylim([10^(-0.2)     1]);
%plot((FAR),(FRR));

plot(FAR,FRR,'r');

xlabel('���������FAR');
ylabel('����ܾ���FRR');



