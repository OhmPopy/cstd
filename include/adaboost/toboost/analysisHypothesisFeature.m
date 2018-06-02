% ����ÿ����ȡ���������е������ֲ������������ֵĴ����ɶൽ������
% �������룺[selectFeaSequence,hypoFeaList,overlapNum]=analysisHypothesisFeature(hypothesis)
%          ����: hypothesis�Ǵ������ļ���
%          ���أ�selectFeaSequence��ѡ������������;hypoFeaList��������������ִ����Ķ�Ӧ��ϵ
%               overlapNum���ظ�����������

function [selectFeaSequence,hypoFeaList,overlapNum]=analysisHypothesisFeature(varargin)
if nargin==1              
    hypothesis=varargin{1};    
    isDisplay=1;               %Ĭ������ʾ��������
elseif(nargin==2)
    hypothesis=varargin{1};
    isDisplay=varargin{2};
else
    error('����������󣡷���.');
end
    

% ���������裬���������ͬ
%  hypothesis  ��ֵ     ƫ�� ������
hypoNum=size(hypothesis,1); % ����������
hypoFeaList=zeros(hypoNum,2); % ͳ���������ִ���
hypoFeaList(:,1)=hypothesis(:,1);%����������б�ʾ���������ڵ���Ϊ��һ��
hypoFeaList(:,2)=1;

% Ѱ�ҳ���ͬ�������У���ͳ����ͬ�Ĵ���.
% ���У�hypoFeaList(:,1)�����������
% hypoFeaList(:,2)������������ظ��Ĵ������Ե�һ�γ���Ϊ׼����ͳ��
% �磺û��ͳ��֮ǰ�� hypoFeaList  ��ͳ��֮��� hypoFeaList
%  hypoFeaList(ͳ��ǰ)    hypoFeaList(ͳ�ƺ�)
%      1     1                  1     3
%      2     1                  2     2
%      9     1                  9     1
%      2     1                  2     0
%      8     1                  8     1
%      5     1                  5     1
%     10     1                 10     1
%      1     1                  1     0
%      4     1                  4     1
%      1     1                  1     0
% Noted by Li.12.03.2013
for i=1:(hypoNum-1)
    for j=(i+1):hypoNum
        if ((hypoFeaList(i,2)~=0)&&(hypoFeaList(j,2)~=0)&&(hypoFeaList(i,1)==hypoFeaList(j,1)))
            hypoFeaList(i,2)=hypoFeaList(i,2)+1;
            hypoFeaList(j,2)=0;
        end
    end
end

    
overlapIndex=find(hypoFeaList(:,2)==0);   % �ظ�������
overlapNum=length(overlapIndex);          % �ظ�����������
hypoFeaList(overlapIndex,:)=[];           % ɾ���ظ����������

hypoFeaList=sortrows(hypoFeaList,2); % �����������ֵĴ����ɶൽ������
if(isDisplay==1)
    overlapInfo=strcat('�ظ�����������:',num2str(overlapNum));
    disp(overlapInfo); % ��ʾ�ظ�������������
end
hypoFeaList=sortrows(hypoFeaList,1);  % ������������С��������
selectFeaSequence=hypoFeaList(:,1).'; % ����ѡ�����������ֵ
    