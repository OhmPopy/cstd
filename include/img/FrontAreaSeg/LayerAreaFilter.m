function [FAI,LayerFilterI]=LayerAreaFilter(g,LayerThresh,AreaThresh,verson)
% g:ԭʼ����ͼƬ
% LayerThresh������˲���ֵ
% AreaThresh������˲���ֵ
% verson������ѡ���Ƿ����g�ĸ�ʴ
%         Y ��ʾ���и�ʴ��N ��ʾ�����и�ʴ
% ������˲����������˲�����������ͨ����
% edit time: 2013.07.02
Order_verson=1;
% Order_verson=1; ������˲�����ʴ��������
% Order_verson=1���ȸ�ʴ��������˲��������� 

if Order_verson==1
    Tg=g;
    LayThresh=LayerThresh; % �����ֵ
    for gsx=1:size(Tg,1)  % ��g����Ԫ��ֵ��С�˲�
        for gsy=1:size(Tg,2)
            if Tg(gsx,gsy)<LayThresh % С��һ������Ŀ��ȥ��
                Tg(gsx,gsy)=0;
            end
        end
    end
    LayerFilterI=Tg;    
    if strcmp(verson,'Y') % �� g ���и�ʴ
        Bg=LayerFilterI;
        Bg(LayerFilterI>0)=1; % �� g ���ж�ֵ��
        Se=strel('square',6);
        IME=imerode(Bg,Se); % ��ʴ��Ķ�ֵͼ
        Tg=IME.*LayerFilterI;
    end    
    if strcmp(verson,'N') % ���� g ���и�ʴ
        Tg=LayerFilterI;
    end
end
if Order_verson==2
    if strcmp(verson,'Y') % �� g ���и�ʴ
        Bg=g;
        Bg(g>0)=1; % �� g ���ж�ֵ��
        Se=strel('square',6);
        IME=imerode(Bg,Se); % ��ʴ��Ķ�ֵͼ
        Tg=IME.*g;
    end
    if strcmp(verson,'N') % ���� g ���и�ʴ
        Tg=g;
    end
    LayThresh=LayerThresh; % �����ֵ
    for gsx=1:size(Tg,1)  % ��g����Ԫ��ֵ��С�˲�
        for gsy=1:size(Tg,2)
            if Tg(gsx,gsy)<LayThresh % С��һ������Ŀ��ȥ��
                Tg(gsx,gsy)=0;
            end
        end
    end
    LayerFilterI=Tg;
end

Tg(Tg~=0)=1; % Ѱ�ҷ����
[L N]= bwlabel(Tg,4); % 4 ��ͨ��ǣ�LΪ��Ǻ�ı�Ե��NΪ���������Ŀ
Temp=zeros(N,2); % ���ڴ洢��������ĵ���
for ConNum=1:N % ͳ�Ƹ�������ĵ���,2��ŵ�����1�������
    Counts=L==ConNum;
    Temp(ConNum,2)=sum(Counts(:));
    Temp(ConNum,1)=ConNum;
end
ThreshArea=AreaThresh;% �����С��ֵ���ı�ֱ��Ӱ�������������Ŀ��gԪ�صĴ�С���������ս��
AreaNum=Temp(:,2);
AreaNum(AreaNum<ThreshArea)=0; % �˳��������С��ThreshArea��С����
NumLeft=sum(AreaNum~=0); % ��������Ŀ
[value index]=dsort(AreaNum); % �������Ӵ�С����
Tempsave=zeros(size(L));
for EraNum=1:NumLeft % ���������������
    Tempsave(L==index(EraNum))=1;
end
gArea=g.*Tempsave; % ��ԭʼg���м��У��ҳ����ϴ�С������
FAI=gArea;
end