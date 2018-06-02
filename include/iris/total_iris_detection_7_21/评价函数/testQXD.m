clc
clear



%��ʵ�ֵĺ����У�
%��Ϣѧ����
%������˹��������
%�����ݶȺ���
%Brenner����
%����� 
%Tenengrad����




% %��Ϣѧ����
% I=imread('car2.bmp');
% I=double(I)/255;
% [M N]=size(I);
% begintime=clock;
% 
% EI=0;
% HI=0;
% for x=1:M
%     for y=1:N
%         EI=EI+I(x,y);
%         if I(x,y)~=0
%             HI=HI-I(x,y)*log2(I(x,y));
%         end
%     end
% end

%������˹��������
% I=imread('car2.bmp');
% I=rgb2gray(I);
% I=double(I);
% [M N]=size(I);
% begintime=clock;
% 
% FI=0;
% for x=2:M-1
%     for y=2:N-1
%        IXXIYY = -I(x-1,y-1) - 4*I(x-1,y) - I(x-1,y+1) ...
%                 -4*I(x,y-1) + 20*I(x,y)  - 4*I(x,y+1) ...
%                 -I(x+1,y-1) - 4*I(x+1,y) - I(x+1,y);
%             FI=FI+IXXIYY*IXXIYY;           
%     end
% end

%�����ݶȺ���
% I=imread('car2.bmp');
% I=rgb2gray(I);
% I=double(I);
% [M N]=size(I);
% begintime=clock;
% 
% FI=0;
% for x=1:M-1
%     for y=1:N-1
%         FI=FI+(I(x+1,y)-I(x,y))*(I(x+1,y)-I(x,y))+(I(x,y+1)-I(x,y))*(I(x,y+1)-I(x,y));
%     end
% end

%Brenner����
% I=imread('car2.bmp');
% I=rgb2gray(I);
% I=double(I);
% [M N]=size(I);
% begintime=clock;
% 
% FI=0;
% for x=1:M-2
%     for y=1:N
%         FI=FI+(I(x+2,y)-I(x,y))*(I(x+2,y)-I(x,y));
%     end
% end


%�����  ?ûЧ��
% I=imread('car2.bmp');
% I=rgb2gray(I);
% I=double(I);
% [M N]=size(I);
% begintime=clock;
% 
% gama=sum(sum(I))/(M*N);
% 
% FI=0;
% for x=1:M
%     for y=1:N
%         FI=FI+I(x,y)-gama;
%     end
% end

%Tenengrad����
% I=imread('car2.bmp');
% I=rgb2gray(I);
% I=double(I);
% [M N]=size(I);
% begintime=clock;
% 
% GX = 0;
% GY = 0;
% FI = 0;
% T  = 0;
% for x=2:M-1
%     for y=2:N-1
%         GX = I(x-1,y+1)+2*I(x,y+1)+I(x+1,y+1)-I(x-1,y-1)-2*I(x,y-1)-I(x+1,y-1);
%         GY = I(x+1,y-1)+2*I(x+1,y)+I(x+1,y+1)-I(x-1,y-1)-2*I(x-1,y)-I(x-1,y+1);
%         SXY= sqrt(GX*GX+GY*GY);
%         if SXY>T
%             FI = FI + SXY*SXY;
%         end
%     end
% end


%ACM
I=imread('D:\pub\bin\iris\���ۺ���\4.jpg');
I=rgb2gray(I);
I=double(I);
[M N]=size(I);
begintime=clock;

ACM=0;
p=zeros(1,256);
IXY=0;
for x=1:M
    for y=1:N
       p(I(x,y)+1)=p(I(x,y)+1)+1;
       IXY=IXY+I(x,y);
    end
end
p=p/(M*N);
u=IXY/(M*N);

for i=1:256
    ACM=ACM+abs(i-1-u)*p(i);
end



time=etime(clock,begintime);




