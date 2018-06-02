function [h_candidateWeakClf,MIU1,DELTA1,MIU2,DELTA2] = candidateWeakClf(YX_neg,YX_pos,sample_w,sample_h,MFeatures,I,MIU1_last_f,DELTA1_last_f,MIU2_last_f,DELTA2_last_f,v,frame)
%�������ܣ�����M����ѡ�����������Լ���������ֵ�ľ�ֵ�ͱ�׼��
%YX_neg��YX_pos�洢������������������(�������Ͻǵ�)
%pic_w��pic_hΪ֡�Ŀ�͸�
%MFeaturesΪ���ѡ���M������������λ�á����͡��߶�
%IΪ��ǰ����֡
%MIU1_last_f,DELTA1_last_f,MIU2_last_f,DELTA2_last_f�洢���Ӹ�˹�ֲ�������ֵ�ľ�ֵ�ͱ�׼��
%vΪѧϰ��(��ֵ�ͱ�׼��ĸ�����)


M = size(MFeatures,1);
tempnum_neg = size(YX_neg,1);
tempnum_pos = size(YX_pos,1);
totalnum = tempnum_neg + tempnum_pos;%��������
AllFeatures = zeros(totalnum,M);%�洢������������������ֵ
YX_all = [YX_neg;YX_pos];
I = double(imadjust(I));%���ӶԱȶȣ�
for i = 1:totalnum%����ÿһ������
    X1 = YX_all(i,2);
    Y1 = YX_all(i,1);
    X3 = X1 + sample_w - 1;
    Y3 = Y1 + sample_h - 1;
    patch_I = I(Y1:Y3,X1:X3);
    intg_im = ImageToIntg(patch_I,sample_w,sample_h);%�����ͼ
    for j = 1:M%����ÿһ������
        FeatureStPos = MFeatures(j,1).StPos;
        FeatureScale = MFeatures(j,1).Scale;
        FeatureType  = MFeatures(j,1).Type;
        AllFeatures(i,j) = GetOneFeatureValue_2(FeatureType, FeatureStPos, FeatureScale, intg_im);%��������ֵ
    end   
end
   
%���¼��м���������ֵ�ͱ�׼��(ÿ�������������������������и������ľ�ֵ�ͱ�׼,Ҫ������������)
if frame ==1,v=0;end%��һ֡ʱ�ĸ�����Ϊ0
MIU2   = v*MIU2_last_f   + (1-v)*sum(AllFeatures(1:tempnum_neg,:),1)/tempnum_neg;%���ڸ�����
temp = repmat(MIU2,tempnum_neg,1);
DELTA2 = v*DELTA2_last_f + (1-v)*sqrt(sum((AllFeatures(1:tempnum_neg,:)-temp).^2,1)/tempnum_neg);
MIU1   = v*MIU1_last_f   + (1-v)*sum(AllFeatures(1+tempnum_neg:end,:),1)/tempnum_pos;%����������
temp = repmat(MIU1,tempnum_pos,1);
DELTA1 = v*DELTA1_last_f + (1-v)*sqrt(sum((AllFeatures(1+tempnum_neg:end,:)-temp).^2,1)/tempnum_pos);


h_candidateWeakClf = zeros(totalnum,M);%�洢���������ĺ�ѡ��������
for j = 1:M
    miu1 = MIU1(j);delta1 = DELTA1(j);
    miu2 = MIU2(j);delta2 = DELTA2(j);
    for i = 1:totalnum
        tempFeature = AllFeatures(i,j);
        P_F_1 = (1/sqrt(2*pi)) * (1/(delta1+0.000000001)) * exp(-(tempFeature-miu1)*(tempFeature-miu1)/(2*delta1*delta1+0.000000001));
        P_F_0 = (1/sqrt(2*pi)) * (1/(delta2+0.000000001)) * exp(-(tempFeature-miu2)*(tempFeature-miu2)/(2*delta2*delta2+0.000000001));
        h_candidateWeakClf(i,j) = log((P_F_1+0.000000001)/(P_F_0+0.000000001));
    end
end



