clc;clear;close all
%�˺�������haar������MIL�ĺ���main�����в�ͬ����ͬ�ĵط�����%������������������������ǳ�

%Ӱ����پ��ȵ���Ҫ����ΪM��K(��Ϊ��Ҫ)������������������������ѧϰ��v(ͨ����ѧϰ�ʵ��ڵ��ӽ�1����0.99���ɴ������߸�����������������ʹ������ֵ�ͱ�׼��ĸ����ٶȱ��������ܲ����ڱ����ںܶ�ʱ���ڱ仯�ϴ�����)
if_face_detect = 0;%�Ƿ����������⣬Ϊ0�򲻽���������⣬�ø���Ȥ����ѡ����
if_replace_clf = 0;%�Ƿ��滻(�����������)������,Ϊ0���滻

work_path = pwd;
d1 = 'test_pic\*.bmp';
d2 = dir(d1);
num_pic = size(d2,1);%ͼƬ����
M = 250;%��ѡ������(��ѡ��������)%����������������������
K = 60; %ѡ��������(ѡ����������)%����������������������
beita1 = 50;%��������������õ��ľ���
beita2 = 80;%��������������õ��ľ���
gama  = 5; %��������������õ��ľ���
s = 30;%��ѡ��������,Ҫ>=gama,����ñ�֤s > (num_fix_cr/10)/2
num_neg = 65;%��������
num_pos = 45;%��������
num_candidate_result =500;%����������,Ҫ>=num_fix_cr
num_fix_cr = 300;%Ҫ��ԭλ�ø����̶���������,ĿǰȡֵΪ100��200��300��������randomSelectSample_for_result.m


MIU1_last_f = zeros(1,M);DELTA1_last_f = zeros(1,M);%��˹�ֲ���ֵ�ͱ�׼��
MIU2_last_f = zeros(1,M);DELTA2_last_f = zeros(1,M);
v = 0.95;%ѧϰ��,0.85~1,vԽС������ֵ�ͱ�׼��͸��µ�Խ�죬v = 1�򲻸���������ֵ�ͱ�׼����ڱ��治����߱仯�ܻ����Ķ���vӦ��ȡ���ӽ�1

for t = 1:num_pic
    tic
    pic_path = strcat(work_path,'\test_pic\',num2str(d2(t).name));%����ͼƬ����Ŀ¼
    I = imread(pic_path);
    I_rgb=I;
    if ndims(I)==3,I = rgb2gray(I);end
    [pic_h,pic_w] = size(I);


    %     if t == 1%��һֻ֡�õ�ǰλ��һ��������
    %         tempnum_neg = num_neg;
    %         tempnum_pos = 1;
    %     else%��num_pos��������
    tempnum_neg = num_neg;
    tempnum_pos = num_pos;
    %     end
    totalnum = tempnum_neg + tempnum_pos;
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % step1:��������Լ����ѡ��M������ %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if t==1
        if if_face_detect ==1
            fp1 = fopen('face_detect\face_detect.bat','w');
            fprintf(fp1,'%s\n','@echo off');
            path_for_bat = strcat('..\test_pic\',num2str(d2(t).name));
            fprintf(fp1,'%s %s','face_detect.exe',path_for_bat);
            fclose(fp1);

            cd(strcat(work_path,'\face_detect'));
            system('face_detect.bat');
            %clc;
            cd(work_path);

            fp2 = fopen('face_detect\facePoints.txt','r');
            facePoints = fscanf(fp2,'%f');%��Ϊc++����ϵ
            fclose(fp2);
            clc
            if facePoints(1) == -1,disp('��һ֡û�м�⵽�κ������������˳�');break;end

            x1_f = facePoints(1);y1_f = facePoints(2);
            x3_f = facePoints(3);y3_f = facePoints(4);
            y1_f = y1_f - abs((y3_f - y1_f)/2);%�����⵽������(����ȥ��)
            x2_f = x3_f;y2_f = y1_f;
            x4_f = x1_f;y4_f = y3_f;
            %imshow(I_rgb),axis on;
            X1 = round(x1_f+1);X2 = round(x2_f+1);X3 = round(x3_f+1);X4 = round(x4_f+1);%(X1,Y1)Ϊ�������Ͻǵ�����
            Y1 = round(y1_f+1);Y2 = round(y2_f+1);Y3 = round(y3_f+1);Y4 = round(y4_f+1);
            Xm = round(((x2_f+1) + (x1_f+1))/2);%�������ľ��ο������λ��

            %Ym = round(((y3_f+1) + (y1_f+1))/2);
            %hold on
            %plot([X1;X2;X3;X4;X1],[Y1;Y2;Y3;Y4;Y1],'r');%���������ο�
            %plot(Xm,Ym,'r.', 'MarkerSize',10);
            %hold off
            %rusult_path=strcat('E:\result\',strtok(num2str(d2(t).name),'.'),'.jpg');%�����״
            %f=getframe;f_r=f.cdata;
            %imwrite(f_r,rusult_path);
            %pause(0.1);
            plot_location(X1,Y1,X3,Y3,I_rgb,d2,t,1);%����Ϊ������ٽ����1��ʾ��ɫ
        else
            [Y1,X1,Y3,X3] = select_ROI(I_rgb);
            pause(0.1);
            close all;
        end

        patch_pos_sample = I(Y1:Y3,X1:X3);%��һ֡��������
        [sample_h,sample_w] = size(patch_pos_sample);

        [MFeatures,NineBlocks_MFeatures] = randomMFeatures(sample_w,sample_h,M,K);%�������M������(���͡�λ�á��߶�)��%����������������������
    end

    %%%%%%%%%%%%%%
    % step2:���� %
    %%%%%%%%%%%%%%
    if t>=2

        YX_candidate = randomSelectSample_for_result(X1,Y1,sample_w,sample_h,pic_w,pic_h,s,num_candidate_result,num_fix_cr);
        [X1_result,Y1_result] = compute_reult(YX_candidate,sample_w,sample_h,SelectKFeatures,I,SelectMIU1,SelectDELTA1,SelectMIU2,SelectDELTA2);

        X1 = X1_result;
        Y1 = Y1_result;
        X3 = X1 + sample_w - 1;
        Y3 = Y1 + sample_h - 1;

        plot_location(X1,Y1,X3,Y3,I_rgb,d2,t,2);%����Ϊ������ٽ����2��ʾ��ɫ

    end

    %%%%%%%%%%%%%%%%%%%%%
    % step3:����������� %
    %%%%%%%%%%%%%%%%%%%%%
    [YX_neg,YX_pos] = randomSelectSample(X1,Y1,sample_w,sample_h,pic_w,pic_h,beita1,beita2,gama,tempnum_neg,tempnum_pos,-1);%YX_neg,YX_posֻ��¼�������ο�����Ͻ����꣬����һ֡�ͺ���֡һ������ֻ��һ������������˴�����-1����ԭ����t

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % step4:����ÿһ��������Ӧ��M����ѡ�������� %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    [h_candidateWeakClf,MIU1,DELTA1,MIU2,DELTA2] = candidateWeakClf(YX_neg,YX_pos,sample_w,sample_h,MFeatures,I,MIU1_last_f,DELTA1_last_f,MIU2_last_f,DELTA2_last_f,v,t);%�������������������������˺����ڲ��в�ͬ
    MIU1_last_f    = MIU1;
    DELTA1_last_f  = DELTA1;
    MIU2_last_f    = MIU2;
    DELTA2_last_f  = DELTA2;


    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % step5:��M����ѡ����������ѡ��K������������� %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    L_ind = SelectFeatures(h_candidateWeakClf,NineBlocks_MFeatures,tempnum_neg,tempnum_pos,K,M);%����������������������

    for k =1:size(L_ind,1)
        SelectKFeatures(k,1).StPos = MFeatures(L_ind(k),1).StPos;%����������������������
        SelectMIU1(1,k)            = MIU1(1,L_ind(k));
        SelectDELTA1(1,k)          = DELTA1(1,L_ind(k));
        SelectMIU2(1,k)            = MIU2(1,L_ind(k));
        SelectDELTA2(1,k)          = DELTA2(1,L_ind(k));
    end
    toc
end
