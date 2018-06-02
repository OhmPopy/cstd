% ���ݱ�׼������ǣ�����������Ĵ�����
% calPredictErrorRate
% �� testAdaBoostLearner/testBoostLearner/testCascadeAdaBoost �ȼ�����������ʱ����
% 
% ���룺
% stdOutput     ��׼���������
% predictOutput Ԥ�����������
% 
% �����
% errorRate  ������ ����������ʵ���������
% TPRate     ����� ��True-Positive Rate
% FPRate     ����ʣ� Negative-True Rate
% 
% 
function [errorRate,TPRate,FPRate]=calPredictErrorRate(stdOutput,predictOutput)
TPSamples=((predictOutput.*stdOutput)==1); % Positive����ΪTrue����������
ErrorSamples=stdOutput-predictOutput;      % ����������������
errorRate=length(find(ErrorSamples~=0))/length(stdOutput);
                                           % ���������
TPRate=length(find(TPSamples==1))/length(find(stdOutput==1));  
                                           % ����Positive����ΪTrue�ı���
FPRate=length(find(ErrorSamples==-1))/length(find(stdOutput==0));    
                                           % ����Negative����ΪTrue�ı���


