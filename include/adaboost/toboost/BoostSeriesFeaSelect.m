% ���� AdaBoost ����ѡ��
% ����selectFeaSize��Чʱ��ѡȡָ������������
% ���򣬰�����Tѭ��
% features: ѵ������
% Y: ѵ���������
% feaSelectRate: ѡ������ռ����������
% trainingRate: ѵ������ռ����������
% T: ѵ������
% selectFeaSize: Ҫѡ���������Ŀ
% testTimes: ���Դ���
% outputTips:
function [SelectFeaNo costTime Hypothesis AlphaT]=BoostSeriesFeaSelect(features,Y,T,selectFeaSize,outputTips)

[Hypothesis,AlphaT,SelectFeaNo,costTime]=BoostSeriesFeaSelectCmpOnce(features,Y,T,selectFeaSize,outputTips);

save('D:\�ҵ��ĵ�\liuhaimei\mat\OMdata\Hypothesis.mat','Hypothesis') % Hypothesis   ������AdaBoostǿ������,��T�������������
save('D:\�ҵ��ĵ�\liuhaimei\mat\OMdata\AlphaT.mat','AlphaT')% AlphaT     AdaBoost��������Ȩ�� 
save('D:\�ҵ��ĵ�\liuhaimei\mat\OMdata\SelectFeaNo.mat','SelectFeaNo') %   ѡ�����������
save('D:\�ҵ��ĵ�\liuhaimei\mat\OMdata\costTime.mat','costTime') 
% ��������ѡ��Ȼ����з��� 
function [hypothesis alphaT SelectFeaNo costTime]...
    =BoostSeriesFeaSelectCmpOnce(features,Y,T,selectFeaSize,outputTips)
    feaSelectFeatures=features;  % �������ѡ������
    feaSelectY=Y;                % ����ѡ�����ݵ����
    
    [hypothesis,alphaT,SelectFeaNo,costTime]=AdaBoostBasedCombineFeaSelect(feaSelectFeatures,feaSelectY,T,selectFeaSize,outputTips);
    save feaselect602 hypothesis alphaT SelectFeaNo costTime ;
%     [trainError,testError,testTP,testFP,Hypothesis,AlphaT,classfyCostTime]=AdaBoostAfterFeaSelect(classfyFeatures,classfyY,trainingRate,T,SelectFeaNo);




