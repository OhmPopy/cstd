% Edit by hudalikm
% Time��2013.06.09
% Second edit time: 2013.07.04
% ��������ĺô�����������Ҫ����̫��Ĳ�������Բ�ͬ�߶ȵ��˺Ͳ�ͬ�߶ȵ������Ҳ�кܺõ�³���ԣ�
% �������������κε�������ֱ��ʹ�á�ȱ�����Ҫ���˲�����̫��Ķ�����
% ������ֵȣ������������������Ҳ���ܴ����������������д����⡣�������Ȥ��
% �������ⷽ�������Ľ���
% �����㷨�Ľ�
% �����д���֤
% ע��ĳЩ���ݵ�ļ��㣬�п��ܳ����߽�

function [ClusterI Time_use]=Rainclusteredit(I,RN,RD)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ��������Ϊ�ȵ�ԭ�򣬸��費ͬ��Ȩ�أ�����ѭ������ Edit by hudalikm.2013.09.05
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
OW=[8 6 4 2 1 3 5 7;
    7 8 6 4 2 1 3 5;
    5 7 8 6 4 2 1 3;
    3 5 7 8 6 4 2 1;
    1 3 5 7 8 6 4 2;
    2 1 3 5 7 8 6 4;
    4 2 1 3 5 7 8 6;
    6 4 2 1 3 5 7 8];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

if ndims(I)==3
    I=rgb2gray(I);
end
[m,n]=size(I);
II=zeros(m+2,n+2);
II(2:m+1,2:n+1)=I;
I=II;
I=double(I);
index=find(I>10); % �ҳ�ͼ���еķ����
[xq,yq]=ind2sub(size(I),index); % �ҳ�ǰ��������λ��
MaxValue=max(I(:)); % �ҳ�ͼ�����ֵ
FillValue=2*MaxValue+10; % ���÷�ǰ���������ֵ(��ɫ0�������ֵ)��Ŀ�ķ���߽紦�������빹��һ��ˮ�ص�Χǽ
I(I<10)=FillValue; % ��ǰ���������
sizeforeground=length(xq); % �ҳ�ǰ������Ŀ
RainNum=50*sizeforeground/25; % �����Ŀ��Ҳ�����ܵ���ѭ������
disp(['�����Ŀ��',num2str(RainNum)])
g=zeros(size(I)); % ��������
t_start=cputime;
NUMCOUNT=0;
for k=1:RainNum
    Rands=ceil(rand*sizeforeground); % ģ����ε��������
    xs=xq(Rands); % �����������
    ys=yq(Rands);
    W=RN;
    r=RD;
    orietind=zeros(1,8); % ������
    findt=0; % ����ָʾ�ǳ�ʼ��λ������µ����
%     disp(['xs: ',num2str(xs),'  ys: ',num2str(ys)])
%     disp('')
    if mod(k,10000)==0
        NUMCOUNT=NUMCOUNT+1;
        disp(['�� ',num2str(NUMCOUNT),' ��ˮ�μ�����ϣ�'])
    end
    while W>0 % �ֲ�Ѱ��
        d(1,8)=0;
        d(1)=I(xs-1,ys-1)+g(xs-1,ys-1)-(I(xs,ys)+g(xs,ys)); % 1
        d(2)=I(xs  ,ys-1)+g(xs  ,ys-1)-(I(xs,ys)+g(xs,ys)); % 2
        d(3)=I(xs+1,ys-1)+g(xs+1,ys-1)-(I(xs,ys)+g(xs,ys)); % 3
        d(4)=I(xs+1,ys  )+g(xs+1,ys  )-(I(xs,ys)+g(xs,ys)); % 4
        d(5)=I(xs+1,ys+1)+g(xs+1,ys+1)-(I(xs,ys)+g(xs,ys)); % 5
        d(6)=I(xs  ,ys+1)+g(xs  ,ys+1)-(I(xs,ys)+g(xs,ys)); % 6
        d(7)=I(xs-1,ys+1)+g(xs-1,ys+1)-(I(xs,ys)+g(xs,ys)); % 7
        d(8)=I(xs-1,ys  )+g(xs-1,ys  )-(I(xs,ys)+g(xs,ys)); % 8
        if findt==0 % ��ʼ���
            Index=find(d==min(d)); % Ѱ����d����Сֵ������
            repnum=length(Index); % ��Сֵ�ĸ���
            Flg=Index(ceil(rand*repnum)); % �����ǳ�ʼ��Σ���ô����Сֵ�������ѡһ������
            Value=d(Flg); % ����������Сֵ
            if Value+r<0 %%%%%%%%%%%%%%%%%%%%%%%%%   ����ѭ���Ĵ���,���� +r 
                                                 %   Edit by hudalikm.2013.09.05
                if Flg==1 % ����1������ת 
                    xt=xs-1;
                    yt=ys-1;
                elseif Flg==2 % ����2������ת 
                    xt=xs;
                    yt=ys-1;
                elseif Flg==3 % ����3������ת 
                    xt=xs+1;
                    yt=ys-1;
                elseif Flg==4 % ����4������ת 
                    xt=xs+1;
                    yt=ys;
                elseif Flg==5 % ����5������ת 
                    xt=xs+1;
                    yt=ys+1;
                elseif Flg==6 % ����6������ת 
                    xt=xs;
                    yt=ys+1;
                elseif Flg==7 % ����7������ת 
                    xt=xs-1;
                    yt=ys+1;
                elseif Flg==8 % ����8������ת 
                    xt=xs-1;
                    yt=ys;
                end
                xs=xt; % ������ת
                ys=yt;
%                 findt=findt+1;
            else
                g(xs,ys)=g(xs,ys)+min(r,W);
                W=W-r;
%                 findt=findt+1;
            end
        else % �ǳ�ʼ���
            td=d==min(d);
            Index=find(td); % �ҳ���Сֵ���ڵķ���
            repnum=length(Index); % �ҳ���Сֵ�ĸ���
            NSFlg=Index; % ��Сֵ���򱣴�
            Indicate=find(NSFlg==Flg); % �ҳ���һ����Сֵ������ͬ����Сֵ��������Flg����һ���ķ���
            DirectInd=0; % ����ָʾ��Сֵ����ֵ֮�Ƿ�Ϊ0
            if ~isempty(Indicate) % ���������һ����Сֵ������ͬ�ķ�������ѡ��÷���
                Flg=NSFlg(Indicate); % �ҳ���Сֵ����
                Value=d(Flg); % �ҳ���Сֵ
                if Value==0 %�������Сֵ���ڣ�����DirectInd=1
                    DirectInd=1;
                end
            elseif repnum>1 % �������һ������ͬ������Сֵ������ֹһ����ʱ�������ѡȡһ��������Ϊ��������(0��С��0������Ƿ���)
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                % ����ͬ������Ϊ�ȣ�Edit by hudalikm.2013.09.05
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                % ������
                %                 Flg=Index(ceil(rand*repnum)); % ���ѡȡ����
                %                 Value=d(Flg); % ��Ӧ��Сֵ�������Сֵ
                % �޸�Ϊ
                tod=OW(Flg,:).*td; 
                [Vtod Flg]=max(tod);
                Value=d(Flg);
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            else % ����һ������ͬ��������Сֵֻ��һ��
                [Value,Flg]=min(d);
            end
            if DirectInd~=1 % ������Сֵ����0
                if Value+r<0 % ��ô����СֵС��0ʱ��ִ��������ת %%%%%%%%%%%%%%%%%% ����ѭ���Ĵ��������� +r
                                                                               % Edit by hudalikm.2013.09.05
                    if Flg==1
                        xt=xs-1;
                        yt=ys-1;
                    elseif Flg==2
                        xt=xs;
                        yt=ys-1;
                    elseif Flg==3
                        xt=xs+1;
                        yt=ys-1;
                    elseif Flg==4
                        xt=xs+1;
                        yt=ys;
                    elseif Flg==5
                        xt=xs+1;
                        yt=ys+1;
                    elseif Flg==6
                        xt=xs;
                        yt=ys+1;
                    elseif Flg==7
                        xt=xs-1;
                        yt=ys+1;
                    elseif Flg==8
                        xt=xs-1;
                        yt=ys;
                    end
                    xs=xt;
                    ys=yt;
%                     findt=findt+1;
                else % ����Сֵ����0ʱ��������g�ı�
                    g(xs,ys)=g(xs,ys)+min(r,W);
                    W=W-r;
%                     findt=findt+1;
                end
            else % ����СֵΪ0(����ֻ���������һ��������ͬ��0ֵ��������һ������ͬ��0ֵ�������ڴ���)
                    if Flg==1
                        xt=xs-1;
                        yt=ys-1;
                    elseif Flg==2
                        xt=xs;
                        yt=ys-1;
                    elseif Flg==3
                        xt=xs+1;
                        yt=ys-1;
                    elseif Flg==4
                        xt=xs+1;
                        yt=ys;
                    elseif Flg==5
                        xt=xs+1;
                        yt=ys+1;
                    elseif Flg==6
                        xt=xs;
                        yt=ys+1;
                    elseif Flg==7
                        xt=xs-1;
                        yt=ys+1;
                    elseif Flg==8
                        xt=xs-1;
                        yt=ys;
                    end
                    xs=xt;
                    ys=yt;
%                     findt=findt+1;
            end
        end
        findt=findt+1; % ��ǵ�ǰ���Ѱ�ŵĲ���������ָʾ�Ƿ��ǳ�ʼ���
        %         disp(['�� ',num2str(findt),'��ѭ��'])
    end
    %         disp(['�� ',num2str(k),'��ˮ�μ������'])
end
end_time=cputime-t_start;
disp(['����ʱ�䣺',num2str(end_time),' second,��ԼΪ��',num2str(round(end_time/60)),' ����!'])
ClusterI=g;
Time_use=end_time;
end

