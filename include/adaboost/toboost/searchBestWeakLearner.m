% ���������ϻ�����ŵ���ֵ������
% searchBestWeakLearner
%��KL-adboost��ͨ��Klɢ����������������������ֱ��ͳ��ͼ
% ����:
% FeatureVector ���������������������������������;������      
% Y             ÿ�������������ı�ʶ;������
% weight        Ȩ������,��ŵ�ǰÿ��������Ȩ��ֵ;������
% [bestError,bestThresh,bestBias]=searchBestWeakLearner(FeatureVector,Y,weight)
function [auc,score]=searchBestWeakLearner(FeatureVector,Y,weight)
% function [auc,minfea,disval,score]=searchBestWeakLearner(FeatureVector,Y,weight)
error(nargchk(3,3,nargin));         % ����3������,������ֹ����
% ����������������������Ϊ������
iptcheckinput(FeatureVector,{'logical','numeric'},{'column','nonempty','real'},mfilename, 'FeatureVector',1);
iptcheckinput(Y,{'logical','numeric'},{'column','nonempty','integer'},mfilename, 'Y',2);
iptcheckinput(weight,{'numeric'},{'column','nonempty','real'},mfilename, 'weight',3);

cntSamples=length(FeatureVector);    % ��������
if( length(Y)~=cntSamples || length(weight)~=cntSamples ) % ��鳤��
    error('����������������ꡢ������Ȩ�ر���߱���ȵĳ���.');
end
%%%%%%%%
% % [FeatureVector,Findex]=sort(FeatureVector);
% % weight=weight(Findex);
% % Y=Y(Findex);
% [pfeature,pindex]=sort(pfeature);
% pweight=pweight(pindex);
% [nfeature,nindex]=sort(nfeature);
% nweight=nweight(nindex);
positiveCols= find(Y==1);        % ��ѵ���������
negativeCols= find(Y==0);        % ��ѵ���������
pweight=weight(positiveCols);%������Ȩ��
nweight=weight(negativeCols);%������Ȩ��
pfeature=FeatureVector(positiveCols);%����������
nfeature=FeatureVector(negativeCols);%����������
%%%%���ܶȹ����㷨%%%%%%%%%%%%%%%%%%%%%%%
po=zeros(1,cntSamples);
ne=zeros(1,cntSamples);

%%%%
for i=1:cntSamples
%        po(i)=kerneldistri(FeatureVector(i),pfeature,pweight);
%        ne(i)=kerneldistri(FeatureVector(i),nfeature,nweight);
         po(i)=kerneldistri(FeatureVector(i),pfeature);
         ne(i)=kerneldistri(FeatureVector(i),nfeature);       
end
% po=po./length(FeatureVector);
% ne=ne./length(FeatureVector);
score=1/2*log((po+eps)./(ne+eps));
% score=2./(1+exp(-8*(po-ne)))-1;
% score=score(Findex);
auc=newroc(score,Y,weight);
% score=score(Findex);
% Y=Y(Findex);
% weight=weight(Findex);

