    %%%�ó����ǲ��Գ���ֻ����һ�������˲�����ȡ����������������֤adboostѡȡ�����������ȷ��%%
    %clear all;
    clc;
    filename1=['D:\�ҵ��ĵ�\liuhaimei\mat\castrainpca'];
    filename1=load(filename1);
   trainall=filename1.trainhistall;
    %%%%%%%%%%%%%%%%%%%%%%%%%%%
    PerclassNum=4; %����ѵ��������ÿ����4��ͼƬ
    ClassNum=100;  %ѵ�������й���300��
    AllNum=PerclassNum*ClassNum; 
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%
    PerclassNum1=4; %���ͼƬ��
    ClassNum1=100; 
    AllNum1=PerclassNum1*ClassNum1;
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      ��������
    disp('�������ھ���')
    featurelength=size(trainall,2);
    swdistance=[]; % ���ھ������
    for i=1:ClassNum
     sample=trainall(((i-1)*PerclassNum+1):i*PerclassNum,:); % ÿһ���������� 
     count=0;
       sw=[];
     for j=1:(PerclassNum-1)
         for m=(j+1):PerclassNum
               count= count+1;
               sampledistance=abs(sample(j,:)-sample(m,:)); %  
                sw=[sw;sampledistance];
         end
     end
             swdistance=[swdistance;sw];  %��������ͼƬ�ľ���
    end
    filesave=strcat('D:\�ҵ��ĵ�\MATLAB\swdislbp','.mat');
    save(filesave,'swdistance');
% %     clear swdistance sample sampledistance;
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      % �������
%       disp('����������')
% %       sbdistance=zeros(45*16,featurelength); 
%         sbdistance=[];
%         counts_class=0;
%          n=0;
%        for k=1:(ClassNum1-1)
%           TP=trainall(((k-1)*PerclassNum1+1):k*PerclassNum1,:);
%          for h=(k+1):ClassNum1
%              counts_bclass=0;
%              counts_class=counts_class+1;
%              TQ=trainall(((h-1)*PerclassNum1+1):h*PerclassNum1,:);
%              for m=1:PerclassNum1
%                  for tk=1:PerclassNum1
%                   counts_bclass= counts_bclass+1;
%                    n=n+1;
%                  sbdistance=[sbdistance;abs(TP(m,:)-TQ(tk,:))];
%                  end
%              end
%          end
%        end
%      filesave2=strcat('D:\�ҵ��ĵ�\MATLAB\sbdislbp','.mat');
%      save(filesave2,'sbdistance');

    