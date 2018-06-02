%
clear all
clc

use_haar = 1;
if use_haar ==1
    XX1 = load('E:\luobo\�ο�����\AdaBoost\boost - ����\haar+adaboost+cascade\F_Haar_0_20_4000_0104.mat');
    X1 = XX1.FEA(1:1000,:);
    % clear XX1;
    XX0 = load('E:\luobo\�ο�����\AdaBoost\boost - ����\haar+adaboost+cascade\F_Haar_neg_1-4226_0104.mat');
    X0 = XX0.FEA(1:3000,:);
    %clear XX0;
else
    XX1 = load('E:\luobo\download_files\piotr_toolbox_luobo\toolbox\channels\feature channels\F_pitor_left_0_20_4000.mat');
    X1 = XX1.FEA;
    XX0 = load('E:\luobo\download_files\piotr_toolbox_luobo\toolbox\channels\feature channels\F_pitor_neg659_8080_15445(1_5000).mat');
    X0 = XX0.FEA;
end
clear XX1 XX0;
X1 = double(X1);
X0 = double(X0);
Y = zeros(1,size(X1,1)+size(X0,1));
Y(1:size(X1,1)) = 1;

T = 500;%��Ҫ����ָ������������
tic;
%
disp(['��ʼѵ��',num2str(T),'��������������']);

pBoost=struct('nWeak',T,'verbose',1,'discrete',1,'pTree',struct('maxDepth',2));%Ӧ���Լ�ָ��pTree������
model = adaBoostTrain( X0, X1, pBoost);


%[Hypothesis,AlphaT,trainErrorRate,costTime,TPRate,FPRate]=trainAdaBoostLearner(X,Y,T);%����AdaBoostѵ����T����������
%[Hypothesis,left_Hypothesis,AlphaT,c]=new_Adaboost(X,Y,T);
%disp([num2str(T),'����������','ѵ����������']);



toc;

%���·�
MODEL = model;

disp('��ʼ����soft��������');

%function  [c_qt,r_t,F,D] = soft_cascade(X,Y,V,C)
%
%Ŀ�����ڵõ�ÿһ����������������˳��q_t������ֵr_t
%
% �������ø�������������������AdaBoost������C{c1,c2,c3...cT}
% ��C��Ԫ�����򣬸�����ֵ����˳�����е�c�͹���ǿ��������
% C{c1,c2,c3...cT}��
%ct=[Hypothesis,left_Hypothesis,right_Hypothesis,AlphaT,left_AlphaT,right_AlphaT]
%(���2�ľ����������ڵ㣬�����ӽڵ��Լ���Ӧ��Ȩֵ����û����Ȩֵ��);
%
%
%��˳��洢ѡȡ��ÿ����������
H1 = zeros(T,3);
%l_H1 = zeros(T,3);
%r_H1 = zeros(T,3);
AT1 = zeros(T,1);
%l_T1 = zeros(T,1);
%r_T1 = zeros(T,1);

%��ʼ��
a1 = sum(Y==0);%����������
b1 = sum(Y==1);%����������
N1 = a1+b1;%��������

P = b1;
N = a1;

%T = length(C); %������ʹ��2048.
%T = 2048;
d = zeros(N1,1);% N*T��ÿt����ÿ������ͨ��ǰt����������������Ӧ����ֱ�Ӿ����ۼӺ;����ˣ����ü�¼ÿһ�����ۼӺͣ�
p = 0; % �����ܾ�����
m = 0; %ִ��ʱ��
A = 0; %ĿǰΪֹ�õĸ������������ظ�ʹ�õ�Ҳ����,��ֵ��Ϊ0
nn = 0;
nnn = 0;

q = zeros(T,1);
r = zeros(T,1);
Q = zeros(T,1);
QQ = zeros(T,T);tic;
V = zeros(1,T);
boostrp_flag = 0;
%��ʼѵ��soft���������T��������������˳���Լ�ÿ��������������ֵ��
for t = 1:T %ͨ��ѵ���������ĸ���TΪѭ������������Ҳ����ʹ�����������������ռ���ʻ�����������
    t_qt = 0;%��¼����ʱ��

    tic

    V(t) = 0.01;%ÿһ��������������©����,
    %����soft���ĸ������㷨����⣬����kֵ��kֵ��֤v����Ԫ��֮�͵���1-D�������Ҫ����D,T��aֵ����ǰ�����������alphaֵ��
    %if a<0
    %   V(t) = k*exp(-alpha*(1-t/T));
    %else
    %   V(t) = k*exp(alpha*t/T);
    %end

    p = p+V(t); %�ۼ�©���ʣ�©��������������ۼӣ�����ÿһ���涨���©��10����������1��©����8������ڶ�������©��12�����Դ����ơ����������ۼӣ�����������˵�ģ�
    a(t) = sum(Y==0);%��t���ĸ�������
    b(t) = sum(Y==1);
    %N(t) = a(t) + b(t); %��t������������

    A = A+a(t);

    %Q = zeros(T,1);
    Q(t) = 0;
    %Q_t = -1000;
    %q_t = 1;
    ar_1 = 0;
    ar_2 = 0;
    tt =T-t+1;%�������q_t��ѭ����������t�֣������������ﻹʣ��tt������������ѭ��tt�Σ��ҳ���ѷ��������±�Ϊq_t��
    %if t==1
    %    d(:,1) = 0;
    %else
    % for jj=1:t-1
    %   c(jj,:) = tree2(H1(jj,:),l_H1(jj,:),r_H1(jj,:),...
    %      AT1(jj),l_T1(jj),r_T1(jj),X,Y);
    %   d(:,jj) = d(:,jj)+c(jj,:)';%�����������d����Ϊÿһ������Ӧ�����ᷢ���ı䣬����ֱ��forѭ����Ӧ���ڴ�ѭ����ÿ����⣡���±�Ҳ���󣡣�
    % end
    %end
    d = d(1:P+N,1);
    %��ѭ���������q_t
    for j = 1:tt

        ids(1:size(X1,1),:) = forestInds(X1,model.thrs(:,j),model.fids(:,j),model.child(:,j),1e5);
        ids(size(X1,1)+1:size(X1,1)+size(X0,1),:) = forestInds(X0,model.thrs(:,j),model.fids(:,j),model.child(:,j),1e5);
        %ids��Ҷ�ӽڵ�ı��
        hs =  model.hs(ids,j);

        f_tij = d + hs;
        clear ids hs;
        ar_1 = sum(f_tij.*Y')/b(t);
        ar_2 = sum(f_tij.*(1-Y)')/a(t);
        QQ(t,j) = ar_1 - ar_2;

        if j==1 %���ΪQ(t)����ֵ����ֱ�Ӹ���ֵ����Щ
            Q(t) = QQ(t,j);
            q_t = 1;
        end
        if Q(t) < QQ(t,j)  %Q_t < Q(j,:) %�ҳ����Ľ��
            Q(t) = QQ(t,j); %Q_t = Q(j,:);
            q_t = j;%�ô������⣡������q_tӦ��ԭ���ı�š����ñ����Ϊ���ҵ�����������������ž�����q_t,��ȡ������һ������ѷ������������ǲ��ǳ�ʼ�����������еı�ţ�����ν����
        end

    end

    % toc;

    q(t) = q_t;%�洢ÿһ���ڼ��������ţ��治������ν��û�����������壩

    disp(['��',num2str(t),'��������ѡȡ������������Ϊ��',num2str(model.fids(1,q_t)),',',num2str(model.fids(2,q_t)),',',num2str(model.fids(3,q_t))]);

    %��˳��洢ÿһ����������
    Model.fids(:,t) = model.fids(:,q_t);
    Model.thrs(:,t) = model.thrs(:,q_t);
    Model.child(:,t)= model.child(:,q_t);
    Model.hs(:,t) = model.hs(:,q_t);
    Model.weights(:,t) = model.weights(:,q_t);
    Model.depth(:,t) = model.depth(:,q_t);


    %t_qt = time;
    %����������Ӧ

    ids(1:size(X1,1),:) = forestInds(X1,model.thrs(:,q_t),model.fids(:,q_t),model.child(:,q_t),1e5);
    ids(size(X1,1)+1:size(X1,1)+size(X0,1),:) = forestInds(X0,model.thrs(:,q_t),model.fids(:,q_t),model.child(:,q_t),1e5);
    hs =  model.hs(ids,q_t);

    %ע��˴�����������Ӧʱ��Ӧ�ø���d��size��
    d = d + hs;
    clear ids hs;

    if 0
        %����t����Ӧ������ֵr_t
        %������ֵʱ�����õķ����뱣֤������ͨ�������ķ�������
        %����ֵӦ�ò���֮ǰ��Ϊ������ֵ�ı仯

        % tempH(i)=sum(AlphaT.*h);
        % �����dӦ���൱�� tempH������dӦ���Ǿ�������Ϊ������
        % sorttemp=sort(tempH(1:600)); %��600����������������
        % Ӧ��d�������򣬰���ʧȥ�涨������������ȷ������ֵ�������Է����������㱣֤ͨ���������������õ���ֵ��


        %����������е�������Ϊ��p*b(t),��Ҫ����ȷ�ж�����������Ϊ��1-p��*b(t);
        %num(t) = b(t)-floor(p*b(t)); %ÿһ��Ҫ��ͨ��������������
        num(t) = floor(p*b(t)); %ÿһ������©���������������Ǹ���
        %sorttemp=sort(d(1:b(t)),'descend'); %�����е�������Ӧֵ���н�������
        sorttemp=sort(d(1:b(t)),'ascend'); %�����е�����������������Ӧֵ������������

        %Ϊr_t����ֵ
        if num(t)>0
            r_t = sorttemp(num(t)); %��֤ͨ�������������� %ֱ��ʹ�����������������Ҳ��֤�˸�����ֵҲ��������������������������Ϊp*b(t)��������ֵ������
        else
            r_t =  sorttemp(1)-0.0001;%������ֵ����С����ֵ��С
            %r_t =  sorttemp(b(t));%������ֵΪ��С����������ֵ
        end

        pred = (d <=r_t).*Y'; %ͳ����ӦֵС����ֵr_t������
        %pred = (d >=r_t).*Y';

        %ȷ�����յ�r_tֵ
        %��������ֵ���ڱ�֤����ʽ����������£�r_tȡֵ���
        i=num(t);
        while(sum(pred) > num(t))
            i=i-1;
            if i<=0
                r_t =  sorttemp(1)-0.0001;%���ڲ�ȥ���������������ʱ����ֵ����������ֵ�𣿣�
                %r_t =  sorttemp(b(t));
                break;
            else
                r_t = sorttemp(i);
            end
            pred = (d <=r_t).*Y';
        end

        r(t) = r_t;

        p = p-sum((d <=r_t).*Y')/b(t); %��������ȥ�������ķ���
        if p<0 %©����ӦΪ�Ǹ�������ʵ�����Ͻ���pû�и�ֵ�Ŀ��ܣ�����ʵ�ʼ����У�����ָ�ֵ����Ӧ���Ǽ��������⣬Ӧ���޸ģ�����
            p=0;
        end


        %sum_pred = 0;
        %r_t = X(1,Hypothesis(q_t,3));
        %R = X(:,Hypothesis(q_t,3)); %��ֵ�Ƚ�
        %for j = 1:N
        %   pred = d<=R(j);
        %   sum_pred = sum(pred.*Y');
        %if sum_pred < p*b
        %    if r_t < R(j)
        %        r_t = R(j);
        %    end
        %end
        %end
    end

    sorttemp=sort(d(1:b(t)),'ascend');
    r_t = (sorttemp(ceil(b(t)*0.01)-1)+sorttemp(ceil(b(t)*0.01)))/2;
    %sorttemp=sort(d(1:b(t)),'descend');
    r(t) = r_t;
    clear sorttemp;

    %����δ����������������,ȥ����ѡȡ����������
    model.fids(:,q_t) = [];
    model.thrs(:,q_t) = [];
    model.child(:,q_t) = [];
    model.hs(:,q_t) = [];
    model.weights(:,q_t) = [];
    model.depth(:,q_t) = [];

    % m = m+t_qt*a(t);%���¼�����ʱ��
    m = m+1*a(t);%������˵ʱ�亯��Ϊ����1

    output = d < r(t);%ǰt�������������������
    output = ~output;
    %Ϊʲôoutput����Ϊ1����r(t)�ĵó����ǻᱣ֤��������ͨ���ʣ���������û�ж��壬ÿ�θ���������Ӧ��ô�������ѡȡ������ֵ����żȻ����

    TRUE(t) = sum(output'.*Y)/P;
    FALSE(t) = sum(output'.*(~Y))/N;
    DD(t) = sum(output'.*Y)/b1;
    FF(t) = sum(output'.*(~Y))/a1;
    %disp(['��',num2str(t),'��������������ȷ��Ϊ��',num2str(TRUE(t)),'����  ','False Positive��Ϊ��',num2str(FALSE(t)),'����']);
    disp(['           ������ȷ��Ϊ��',num2str(TRUE(t)),'����  ','False Positive��Ϊ��',num2str(FALSE(t)),'����']);
    disp(['ǰ',num2str(t),'��������������ȷ��Ϊ��',num2str(DD(t)),'����  ','FP��Ϊ��',num2str(FF(t)),'����']);
    disp(['ͨ��������������Ϊ��',num2str(sum(output'.*Y)),'��',]);
    disp(['���и���������Ϊ��',num2str(N),'ͨ���ĸ���������Ϊ��',num2str(sum(output'.*(~Y))),'��  ']);
    fprintf('\n');
    %�������������ҲӦ��ע�⣬ÿ������ۼӺ͵�size

    %X = X(~mark,:); %ȥ��d<r(t)����������������ֻҪ����������
    X1 = X1(output(1:P),:);
    X0 = X0(output(1+P:N+P),:);
    Y = Y(output);
    d = d(output); %����d��size.

    clear output;

    if boostrp_flag == 0
        if isempty(X0)
            disp('������Ϊ�գ���ѵ����������');
            break;
        end
    else
        if t>2
            nn = nn+1;
            XX0  = load ('E:\luobo\download_files\piotr_toolbox_luobo\toolbox\channels\feature channels\F_pitor_neg659_8080_15445(5001_10000).mat');
            if 1000+(nn-1)*1000 <= size(XX0.FEA,1)
                X00 = XX0.FEA(1+(nn-1)*1000:1000+(nn-1)*1000,:);
                boostrp_flag = boostrp_flag +1;
            else
                clear XX0;
                nnn = nnn+1;
                XX0  = load ('E:\luobo\download_files\piotr_toolbox_luobo\toolbox\channels\feature channels\F_pitor_neg659_8080_15445(10001_15000).mat');
                if 1000+(nnn-1)*1000 < size(XX0.FEA,1)
                    X00 = XX0.FEA(1+(nnn-1)*1000:1000+(nnn-1)*1000,:);
                    boostrp_flag = boostrp_flag +1;
                else
                    X00 = XX0.FEA(1+(nnn-1)*1000:1000+(nnn-1)*1000,:);
                    boostrp_flag = 1;
                end

            end
            clear XX0;
            X00 = double(X00);
            HS_x = 0;
            for nweak = 1:size(Model.fids,2)
                ids = forestInds(X00,Model.thrs(:,nweak),Model.fids(:,nweak),Model.child(:,nweak),1e5);
                hs =  Model.hs(ids,nweak);
                HS_x = HS_x+hs;
            end
            d = [d;HS_x];
            Y = [Y,zeros(1,size(X00,1))];
            X0 = [X0;double(X00)];
            %num_F(t) = num_F(t)+ size(X00,1);
            %num_FF = num_FF+ size(X00,1);
            a1 = a1 + size(X00,1);
            clear X00;
            boostrp_flag = boostrp_flag-1;
            %F(t+1) = 1;
            %FF(t+1) = num_F(t)/num_FF;
            %break;
        end
    end
    P = sum(Y==1);
    N = sum(Y==0);



end
toc;

if t<T
    q = q(1:t);

    Model.fids = Model.fids(:,1:t);
    Model.thrs = Model.thrs(:,1:t);
    Model.child= Model.child(:,1:t);
    Model.hs = Model.hs(:,1:t);
    Model.weights = Model.weights(:,1:t);
    Model.depth = Model.depth(:,1:t);
end

[selectFeaSequence,hypoFeaList,overlapNum] = analysisHypothesisFeature_soft( Model);
disp(['ѡ�������������',num2str(length(hypoFeaList(:,1)))]);
%disp(['�ظ�����������',num2str(overlapNum)]);
disp(['ѡ�������Ϊ��',num2str(selectFeaSequence)]);



