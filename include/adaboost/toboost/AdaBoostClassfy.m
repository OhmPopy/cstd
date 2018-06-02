% AdaBoost ǿѧϰ�㷨 ��һ���������з��࣬���AdaBoost�㷨��������Ԥ�����
% AdaBoostClassfy �� AdaBoostDecisionForSample ���ƣ��䲻֮ͬ������
% AdaBoostClassfy �б�һ������, AdaBoostDecisionForSample �б𵥸��������
% AdaBoostClassfy  ����������д���������������ֵ
% AdaBoostDecisionForSample ������ǵ�������������ֵ 
% ���룺
% Samples      �����������, cntSamples x cntFeatures ����
%              cntSamples    �����������������
%              cntFeatures   �����ռ�ά��
% Hypothesis   ������AdaBoostǿ������,��T�������������
% AlphaT       AdaBoost��������Ȩ��
% T            ���������ʹ�õ�AdaBoost��������������
% boostthresh  AdaBoostǿ����������ֵ��Ĭ��Ϊ0.5
% ���:
% predictOutput      AdaBoost��ÿ��������Ԥ���������ֵΪ0����1
%                    ��������1 x cntSamples
% predictConfidence  AdaBoost��ÿ��������Ԥ����������Ŷȣ�ֵ��[0 1]��
%                    ��������1 x cntSamples
% ���ø�ʽ:
% [predictOutput,predictConfidence]=AdaBoostClassfy(Samples,Hypothesis,AlphaT,T)
% [predictOutput,predictConfidence]=AdaBoostClassfy(Samples,Hypothesis,AlphaT,T,boostThresh)
% 
% function [predictOutput,predictConfidence]=AdaBoostClassfy(Samples,Hypothesis,weak,AlphaT,T)
function [predictOutput]=AdaBoostClassfy(Samples,Hypothesis,weak,AlphaT,T)
cntSamples=size(Samples,1);        % �����������������
predictOutput=zeros(1,cntSamples); % ÿ��������ǿ���������б������0����1
Hypothesis=Hypothesis(1:T,:);      % ȡǰT���������� 
weak=weak(1:T,:);
AlphaT=AlphaT(1:T);                % ǰT����������Ȩ��
for i=1:cntSamples              
    h=zeros(1,T);                  % ���з�����ÿ�������ķ�������� 
    hzong1(i)=0;  
    for t=1:T
           j=Hypothesis(t,1);           % ��t������������ ������   
            score=weak{t,1};    
%             index=weak{t,2};
%             alpha=AlphaT(t);  
%             score=score(index);
            hzong1(i)=hzong1(i)+score(i);
           
     end
%      tempH=sum(AlphaT.*h);      
%      if(tempH>=(boostthresh*sum(AlphaT)))          % ��T�������������ǿ������,���������i����� 
%           predictOutput(i)=1;
%      end
%      predictConfidence(i)=tempH/(sum(AlphaT)+eps); % ���Ŷ�
% %       if(hzong1(i)>0)
% %           predictOutput(i)=1;
% %       end
%       if(hzong2(i)<hzong1(i))
%           predictOutput(i)=1;
%       end
%    allhzong(i)=hzong1(i);
end
sortall=sort(hzong1(1:100));  %��600���������÷ֽ������� 
for i=1:cntSamples  
    if (hzong1(i)>sortall(1))
         predictOutput(i)=1;
    end
end


