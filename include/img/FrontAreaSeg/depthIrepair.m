function RI=depthIrepair(DI,BRsize)
% ���ͼ�������
% �����ͼ��0ֵ��Ϊ���ģ���ȡ(2*BRsize+1) x (2*BRsize+1)��С�Ŀ飬��ȡ���з����ֵ��Ϊ���ĵ�����ֵ
% DI : ԭʼ���ͼ
% BRsize : ��뾶�����������˿��С������BRsizeӦ��С�ڻ��ߵ���12
% ����ĳЩ�߽ǿն��������޷����
% edit time: 2013.07.05
if BRsize>12
    error('���С�����������趨�����ޣ����СӦ��С�ڻ��ߵ���12');
end
if BRsize<0
    error('���С������Ҫ�󣬿��СӦ�ô���0');
end

DI=double(DI); % �����ͼת��Ϊdouble����
[m,n]=size(DI); % ԭʼ���ͼ��С
TDI=ones(size(DI)+26); % ����һ����������Ѱ�����ͼ�пն����ص�����
TDI(13:m+12,13:n+12)=DI; 
index=find(TDI==0); % Ѱ��ԭʼ���ͼ�еĿն���������
[xc,yc]=ind2sub(size(TDI),index); % Ѱ��ԭʼ���ͼ�еĿն���������
N=length(xc); % �ն�������Ŀ
TDIT=TDI; % ����һ����������
TDIT(TDI==1)=0; % �����е����ֵΪ1�����ص�ȫ����ֵΪ0
for p=1:N  %���������пն�����
    IP=TDIT(xc(p)-BRsize:xc(p)+BRsize,yc(p)-BRsize:yc(p)+BRsize); % �����򻮶�
    A=sum(IP(:)); % ��ȡ�������������ֵ    
    AB=IP~=0; % Ѱ�ҷ����
    B=sum(AB(:)); % ��������
    if B==0 % ���������û�з���㣬Ϊ�˱����ĸΪ�㣬��B=1
        B=1;
    end
    TDIT(xc(p),yc(p))=round(A/B); % ������ֵ
end
RI=TDIT(13:m+12,13:n+12); % ���ؼ�����������ͼ����С��ԭʼ���ͼһ��
end
