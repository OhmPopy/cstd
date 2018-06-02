% ���� AdaBoost �����������ϵ�����ѡ��
% ����˼��: ���Ȼ�÷��������õ���������,Ȼ���𲽼�����������,�Ի��Ч���Ϻõ��������
% ֱ�ӵ���AdaBoostѵ����������Ȼ��ͨ������AdaBoost��������÷�������ѡ�������
%
% ����:
% feaSelectFeatures     ��������ѡ���ѵ������
% feaSelectY            ��������ѡ���ѵ�����������
% T                     ѵ������
% SeleFeaSize           ��Ҫѡ�����������
%
% �����
% SelectFeaNo           ѡ�����������
% costTime              ����ѡ������������ѵ�ʱ��
% trainErrorRate        ��ѡ�������������ѡ����������ﵽ�Ĵ�����
% overlapNum            ��ѡ����������ظ���������
%
function [hypothesis,alphaT,SelectFeaNo,costTime]=...
    AdaBoostBasedCombineFeaSelect(feaSelectFeatures,feaSelectY,T,selectFeaSize,varargin)

outputTips=0;
if( nargin==5 )
    outputTips = varargin{1};
end


% ͨ��AdaBoostѵ����÷�����
if(selectFeaSize<=0)              % ��ѡ����������������Ч�������T����ѭ��
%     [hypothesis,alphaT,trainErrorRate,costTime]=AdaBoostTrain(feaSelectFeatures,feaSelectY,T);
    [hypothesis,alphaT,trainErrorRate,costTime,TPRate,FPRate]=trainAdaBoostLearner(feaSelectFeatures,feaSelectY,T);
else
%     [hypothesis,alphaT,trainErrorRate,costTime]=AdaBoostTrain(feaSelectFe
%     atures,feaSelectY,T,selectFeaSize);
     [hypothesis,alphaT,trainErrorRate,costTime,TPRate,FPRate]=trainAdaBoostLearner(feaSelectFeatures,feaSelectY,T,selectFeaSize);
end

% save('D:\�ҵ��ĵ�\liuhaimei\mat\OMdata\trainErrorRate.mat','trainErrorRate')
% save('D:\�ҵ��ĵ�\liuhaimei\mat\OMdata\TPRate.mat','TPRate')
% save('D:\�ҵ��ĵ�\liuhaimei\mat\OMdata\FPRate.mat','FPRate')%��ѵ�������ŵ�ָ���ļ�����
save resultRate trainErrorRate TPRate FPRate;    %��ѵ�������ŵ���ǰ����·���£������ƶ�����Ҫ���Ĵ��·��
 
% ͨ������AdaBoost��÷�������ѡ�������
% SelectFeaNo��ѡ������������;
% hypoFeaList��������������ִ����Ķ�Ӧ��ϵ
% overlapNum���ظ�����������
% �Ƿ������������ظ�����������������
[SelectFeaNo,hypoFeaList,overlapNum]=analysisHypothesisFeature(hypothesis,outputTips);

% �������������ѡ������������

% if(nargout==6)                   % �����������
%     varargout{1}=overlapNum;     % �ظ���������
% end
% if(nargout==7)                   % ����߸�����
%     varargout{1}=trainErrorRate; % ѵ��������
%     varargout{2}=overlapNum;     % �ظ���������
% end

if(outputTips==1)
    disp('���� AdaBoost ������ѡ��...');
    disp(strcat('ѡ������������',num2str(length(SelectFeaNo)),'.'));
    disp(strcat(' AdaBoost ѵ��������',num2str(size(hypothesis,1)),'.'));
    disp(strcat('�ظ�����������',num2str(overlapNum),'.'));
    disp('---------------------------------------');
end
