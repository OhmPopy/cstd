% AdaBoost �������һ���������з��࣬���AdaBoost�����������������Ԥ�����
% AdaBoost ������Ϊ��ֵ������
% AdaBoostWeakLearnerClassfy
% ���룺
% Samples      �����������, cntSamples x cntFeatures ����
%              cntSamples    �����������������
%              cntFeatures   �����ռ�ά��
% weakLearner  �����������������˴����õ��Ƕ�ֵ������
% ���:
% predictOutput  AdaBoost����������ÿ��������Ԥ���������ֵΪ0����1
%                �����������1 x cntSamples
function [predictOutput]=AdaBoostWeakLearnerClassfy(Samples,hypo,weak)
j= hypo(1);              %���������
val=Samples(:,j);        %��Ӧ�������ϵ�����
nsample=length(val);
score=weak{1};         %ÿ�������ĵ÷ֲַ�
% index=weak{2};
% score=score(index);
% weight=weak{3};
% Y=Y(index);
% weight=weight(index);
for i=1:nsample
   predictOutput(i)=score(i)>0;
end
% erroRate=sum(weight(predictOutput~=Y));

