function ROIH=roi_height(img,BW2)
%%%%%%�ú������Ǹ�����ָ�ؽ�λ�òü���Ȥ����ĸ߶�
img=double(img);
cropimage=img;
%[L, N]=bwlabel(BW2,8);%%%%�ҳ�BW2��8�������ͨ����������L���󣬸���ΪN��
[m,n]=size(BW2); %%%%����������m������n��
if mod(n,2)==0 % ��Եͼ���е�    n��2���ȡ����
    mid=n/2;
else
    mid=(n+1)/2;
end
% ĩ�ҹؽ�
linesum=sum(cropimage,2); % ���ۻ���������ÿ��֮�ͣ���Ϊ1
LN=length(linesum); % ����Ŀ
LM=zeros(LN,1); %����LN*1�������
ST=LM; % ���ڱ��洰���ۻ�ֵ
LM(1:50,1)=1; % ����һ��50��1������ΪLN�ľ���
for i=1:mid-49 % ȷ��ĩ�ҹؽ�λ��
    winsum=sum(LM.*linesum); % �����ۻ����
    ST(i+49,1)=winsum; %�����ڶ�Ӧ��λ��
    LM(i,1)=0; % ���������ƶ�һ��
    LM(i+50,1)=1;
end
[v, ind]=max(ST); % Ѱ�Ҿ���ST�е����ֵv��������ind��Ϊ��Ӧ��ĩ�ҹؽ�λ��,indΪ���ڵ�����
[m,n]=size(cropimage); % �����ü���ͼ���С
r1=ind;
% ���˹ؽ�
[m,n]=size(cropimage); % �����ü���ͼ���С
LN=length(linesum); % ����Ŀ
LM=zeros(LN,1);
ST=LM; % ���ڱ��洰���ۻ�ֵ
scor=r1+80; % ���˹ؽ���������ʼ����
LM=zeros(LN,1);
LM(scor:scor+49,1)=1; % ����һ��50��1������ΪLN�ľ���
for i=1:m-(scor-1)-49 % ȷ�����˹ؽ�λ��
    winsum=sum(LM.*linesum);
    ST(i+scor+49-1,1)=winsum;
    LM(i+scor-1,1)=0;
    LM(i+scor+49,1)=1;
end
[v, ind]=max(ST);
[m,n]=size(cropimage);
r2=ind;
d1=r2-r1; % ���˹ؽں�ĩ�ҹؽڵľ���
d2=m-r2;  % ���˹ؽں�ͼƬ����еľ���
if r1<d1*0.3 % Ϊ�˱���������Ϣ����Ӧ�ؽ�ĩ�ҹؽ�λ�����ƣ����˹ؽ�λ������
    h1=5; % ��ֵ�д�����
else
    h1=r1-d1*0.3;
end
if  d2<d1*0.1
    h2=m-5;
else
    h2=r2+d1*0.1;
end
if(ceil(h1)==0)
    ROIH=cropimage((ceil(h1)+1):ceil(h2),:); % ��Ȥ���򰴸߶Ȳü�
else
    ROIH=cropimage(ceil(h1):ceil(h2),:);
end