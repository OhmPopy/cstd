function feature = GetOneFeatureValue_2(FeatureStPos,InterData )
%�˺�������haar������MIL�ĺ���GetOneFeatureValue_2��ͬ
%���������������InterData�ĳ߶��Ǹ߶ȣ�sample_h+9,��ȣ�sample_w+9��
%               FeatureStPosΪ������(������Ҫ��Ϊ9*9��������������)
%���featureΪ����ֵ

yc = FeatureStPos.y;%9*9�����������
xc = FeatureStPos.x;

col1 = [-3;-3;-3;0;0;0;3;3;3];
col2 = [-3; 0; 3;-3; 0; 3;-3; 0; 3];

YX = [yc - col1,xc - col2];%9*9����ÿһ��3*3�ӿ����������
blockValues = zeros(9,1);

LTopY = YX(:,1) - 2;%����ͼ�õ���9*9�������9��3*3�ӿ�����Ͻ�����
LTopX = YX(:,2) - 2;

RTopY = YX(:,1) - 2;%����ͼ�õ���9*9�������9��3*3�ӿ�����Ͻ�����
RTopX = YX(:,2) + 1;

LBtmY = YX(:,1) + 1;%����ͼ�õ���9*9�������9��3*3�ӿ�����½�����
LBtmX = YX(:,2) - 2;

RBtmY = YX(:,1) + 1;%����ͼ�õ���9*9�������9��3*3�ӿ�����½�����
RBtmX = YX(:,2) + 1;

for i =1:9%����9��3*3�ӿ��Ӧ�ĸ������غ�
    blockValues(i,1) = InterData(RBtmY(i),RBtmX(i)) + InterData(LTopY(i),LTopX(i)) - InterData(RTopY(i),RTopX(i)) - InterData(LBtmY(i),LBtmX(i));      
end

Label = ((blockValues - blockValues(5,1)) >= 0);%���8��3*3�ӿ���Ե�blockValuesֵ��������3*3��blockValuesֵ����Ϊ1��������Ϊ0

%����ֵΪ2^7 * blockValues(1,1) + 2^6 * blockValues(2,1) + 2^5 * blockValues(3,1) + 2^4 * blockValues(4,1) + 2^3 * blockValues(6,1) + 2^2 * blockValues(7,1) + 2^1 * blockValues(8,1) +  2^0 *blockValues(9,1) ;
feature  = 128 * blockValues(1,1) + 64 * blockValues(2,1) + 32 * blockValues(3,1) + 16 * blockValues(4,1) + ...
           8 * blockValues(6,1) + 4 * blockValues(7,1) + 2 * blockValues(8,1) +  blockValues(9,1) ;
