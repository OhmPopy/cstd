function FrontRepairI=FrontAreaSeg(RDI,BI,ImerodeThresh,ImdilateThresh,AreaThresh)
% RDI: ���޸�������ͼ
% BI��ǰ����ֵͼ
% ImerodeThresh����ʴ�ṹԪ�ش�С , ���� 10
% ImdilateThresh�����ͽṹԪ�ش�С ������ 15
% AreaThresh: ���������ֵ, ���� 1500
% FrontRepairI�����޸����ǰ�����ͼ
% ˼�룺�Ƚ��и�ʴ����ȥ��С����������Ȼ���ʣ�µ���ͷ����������ͣ�����Ӧ�޸�������ͼ
% edit time: 2013.07.05
RDI=double(RDI);
BI=double(BI);
% ��ʴȥ��С����
TI=BI;
TI(BI==255)=1; % ת����0��1
Se=strel('square',ImerodeThresh); % ��ʴ�ĽṹԪ�ض���
IME=imerode(TI,Se); % ��ʴ��Ķ�ֵͼ
[L CNum]= bwlabel(IME,4); % 4 ��ͨ��ǣ�LΪ��Ǻ�ı�Ե��NΪ���������Ŀ
NTemp=zeros(CNum,2); % ���ڴ洢��������ĵ���
for ConNum=1:CNum % ͳ�Ƹ�������ĵ���,2��ŵ�����1�������
    Counts=L==ConNum; % Ѱ����ͨ����
    NTemp(ConNum,2)=sum(Counts(:)); % ���ҵ�����ͨ�����������ͳ��
    NTemp(ConNum,1)=ConNum; % ����
end
ThreshArea=AreaThresh; % ���������ֵ
AreaNum=NTemp(:,2);
AreaNum(AreaNum<ThreshArea)=0; % �˳��������С��ThreshArea��С����
NumLeft=sum(AreaNum~=0); % ��������Ŀ
[value index]=dsort(AreaNum); % �������Ӵ�С����
Tempsave=zeros(size(L));
for EraNum=1:NumLeft % ���������������
    Tempsave(L==index(EraNum))=1;
end

SI=strel('square',ImdilateThresh); % ���ͽṹԪ��
IME=imdilate(Tempsave,SI); % ���ͺ�Ķ�ֵͼ

FrontRepairI=IME.*double(RDI); % ���޸����ǰ������
end