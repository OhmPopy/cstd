
function [Hypothesis,AlphaT,trainErrorRate,costTime,TPRate,FPRate]=trainAdaBoostLearner(X,Y,T,varargin)
error(nargchk(3,4,nargin)); % ��������3-4������,������ֹ����
iptcheckinput(X,{'numeric'},{'2d','real','nonsparse'}, mfilename,'X',1);
iptcheckinput(Y,{'logical','numeric'},{'row','nonempty','integer'},mfilename, 'Y', 2);
iptcheckinput(T,{'numeric'},{'row','nonempty','integer'},mfilename, 'T',3);
if( length(T) > 1 )              % ָ��ѵ�������Ĳ���T����ӦΪ1������Ϊ������
    error(['ָ��ѵ�������Ĳ���(T)����(' num2str(length(T)) ')ӦΪ1(����Ϊ����).']);
end
[cntSamples,cntFeatures]=size(X); % cntSamples  ѵ�����ݼ�����������
inverseControl=0;           % ����ѭ���˳���ʶ��Ϊ0��ʾѵ��T�֣�������Ҫѵ����ָ������������
cntSelectFeatures=0;        % ��Ҫѡ����������� 
if( nargin>3 )              % ������4�����������ʾ��������ѵ������ΪT�Ĺ���
    cntSelectFeatures=varargin{1};
    inverseControl=1;
    iptcheckinput(cntSelectFeatures,{'numeric'},{'row','nonempty','integer'},mfilename, 'cntSelectFeatures',4);
    if( length(cntSelectFeatures) > 1 ) % ָ�����������Ĳ�������ӦΪ1������Ϊ������
        error(['ָ�����������Ĳ���(cntSelectFeatures)����(' num2str(length(cntSelectFeatures)) ')ӦΪ1.']);
    end
    if( cntSelectFeatures>=cntFeatures )
        error('��Ҫѡ�����������(cntSelectFeatures)����');
    end
end
if( cntSamples~=length(Y) ) % ѵ������X���뱻�ල
    error('ѵ������X���ȱ������������������ͬ') ;
end
computeCosttimeFlag=1;      % ��ʱ��ʶ����Ϊ1��ʾ��ѵ��ʱ���ʱ
if(computeCosttimeFlag==1)
    tic
end
X=ceil(X*10000)/10000;          % ѵ����������ɾ��β�����ݣ���ֹ�������
positiveCols=find(Y==1);        % ��ѵ���������
negativeCols=find(Y==0);        % ��ѵ���������
if(isempty(positiveCols))     % �����������
    error('����������Ϊ0,�޷�ѵ��.');
end
if(isempty(negativeCols))     % ��鸺������
    error('����������Ϊ0,�޷�ѵ��.');
end
weight=ones(cntSamples,1);       % Ȩֵ����;������;һ�д���һ��������Ȩ��
weight(positiveCols)=1/(2*length(positiveCols));      % ��ѵ�������ĳ�ʼȨֵ
weight(negativeCols)=1/(2*length(negativeCols));      % ��ѵ�������ĳ�ʼȨֵ
AlphaT=zeros(1,T);              % T��ѵ����ȡ��ÿ���������Ȩֵ
trainErrorRate=zeros(1,T);      % �� 1 ������ T �ֵ�ѵ��������
costTime=zeros(1,T);            % �� 1 ������ T �ֻ��ѵ�ʱ��
trainOutput=zeros(1,cntSamples); % ��ʱ����:ʹ��ǰt����������ѵ�����ݽ��з���Ľ��  
h=zeros(1,cntSamples);           % ��ʱ����:ʹ�õ�t�����ŷ��������з��� �Ľ��      
TPRate = zeros(1,T);             % ��1��T�ּ����
FPRate = zeros(1,T);             % ��1��T�������
t=1; 
curFeaSize=0;                   % ��ǰ�Ѿ�ѡ�������������
count_time=0;
%%%%%%%%%
while(true)                     % ѵ������
    t_times=cputime;
    minauc=10000;
    weight=weight/(sum(weight));% Ȩֵ��һ��
    for j=1:cntFeatures         % ��ÿ������ֵ,��ȡ���ŵķ�����(����)
        [tempauc,tempscore]=searchBestWeakLearner(X(:,j),Y',weight); % ������ȡj�����ŵļ���
         if(tempauc<minauc)                        % ѡ�������ж�Ӧ��ɵķ�����
              minauc=tempauc;   %��Ӧ�������ϵ�����ͳ��ֱ��ͼK-Lɢ��ֵ   
              score=tempscore;
%               index=tempindex;
%               tweight=tempweight;
                Hypothesis(t,:)=[j];
%               Hypothesis(t,:)=[j,value,histdisval];  % ��t�� ������������
              weak{t,1}=score; 
           %%weak{t,2}=index; 
%          weak{t,3}=tweight;
        end
    end
    [h]=AdaBoostWeakLearnerClassfy(X, Hypothesis(t,:),weak(t,:));   % ʹ�õ�t�����������������з���   
%     h=predict;
    errorRate=sum(weight(h~=Y));                 % �����t�ַ��������  
%     AlphaT(t)=aw;    % �����t��Ȩ��
    AlphaT(t)=0.5*log((1-errorRate)/(errorRate+eps));    % �����t��Ȩ��   
%       AlphaT(t)=(1+(0.6).^t)*log((1-errorRate)/(errorRate+eps));
%     if(errorRate>eps)                                  % ��С����ȷ�����������Ȩֵ
% %         % �����Ƿ��������������⣬��ĳ��ѵ�����Ϊ0��ʱ��Ȩ�ز��ٸ��£�����û������ѵ��
% %         % ���յ���ѵ�������һ�������½��Ĺ��̣�ĳ��Ϊ0�������������������һ����0.  12.03.2013
%         weight(h==Y)=weight(h==Y)*(errorRate/(1-errorRate)); 
          weight(h==Y)=weight(h==Y)*exp(- AlphaT(t));
%           weight(h~=Y)=weight(h~=Y)*exp(AlphaT(t));
           
%     end
%          weight=weight*exp(-AlphaT(t).*h.*Y);
    % �����Ǽ��㵱ǰ����(��t��)��ѵ��������
    [trainOutput]=AdaBoostClassfy(X,Hypothesis,weak,AlphaT,t);% yes!2013.04.09
    [curErrorRate,curTPRate,curFPRate]=calPredictErrorRate(Y,trainOutput);
    trainErrorRate(t) = curErrorRate;
    TPRate(t) = curTPRate;
    FPRate(t) = curFPRate;
   
    if(inverseControl==0)            % ����ѵ����������ѭ��
        if(computeCosttimeFlag==1)
            costTime(t)=toc;
        end
        if(t>=T)                     % �ﵽ������ѵ������                       
             break;
        end
    else                             % ������ȡ��������������ѭ��
        [SelectFeaNo]=analysisHypothesisFeature(Hypothesis(1:t,:),0);
        if( length(SelectFeaNo)>curFeaSize )
            curFeaSize=length(SelectFeaNo);
            if( computeCosttimeFlag==1 )
                costTime(curFeaSize)=toc;
            end
        end
        if( curFeaSize>=cntSelectFeatures )% �ﵽ��������������
            break;
        end
    end
    elaspetime=cputime-t_times;
    count_time=count_time+elaspetime;
    disp(['�Ѿ�ɸѡ�� ',num2str(t),' ������','��ʣ�� ',num2str(T-t),' ������'])
    disp(['����������ʱ�䣺',num2str(elaspetime),' ��'])
    disp(['��Ŀǰ�ܹ�����ʱ�䣺',num2str(count_time),' ��'])
     %%%%��ʾÿ��ѡ���ʶ���ʺ���ʶ����
     disp(['��',num2str(t),'��ѵ��������trainErrorRateΪ��',num2str(trainErrorRate(t))]);
     disp(['��',num2str(t),'�μ����TPRateΪ��',num2str(TPRate(t))]);
     disp(['��',num2str(t),'����ʶ����FPRateΪ��',num2str(FPRate(t))]);
     disp(['ѡ���ĵ�',num2str(t),'���������Ϊ��',num2str(Hypothesis(t,1))]);
     %%%%%%%%%%%%%%%%%%%%%%
    fprintf('\n')
    t=t+1;
end

if(computeCosttimeFlag==1)
    costTime=costTime(costTime~=0);% ���ѵ��ʱ��
else
    costTime=0;
end

if(t<T)     
    Hypothesis=Hypothesis(1:t,:);      % ǿ������
    AlphaT=AlphaT(1:t);                % ǿ������Ȩ��
    trainErrorRate=trainErrorRate(1:t);% ѵ��������
end

