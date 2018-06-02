%������Ĺ������������������(��������)���õ��ĺ����ǻҶ�ͶӰ�㷨(GPA)
%�����ڵ�ǰ����֡����ڵ�ǰ�ο�֡���ƽ��20������(������)���ڵ������Ҳ����˵�������е�ǰ����֡���ƽ��20������(������)���ڵ����
%��������ͷ�볡������̫����ʵ�������̫���Ļ������ڵ�ǰ�����ƶ����죬Ҳ����ǰ����֡��ͬһ�ƶ������ƶ��÷��Ƚϴ����Ի�Ӱ������˶�ʸ��
%�ο����ף�
%(1)һ�ֻ��ڻҶ�ͶӰ�㷨�ĵ������񷽷������� ������ ������
%(2)��ȡ��̬ͼ��λ��ʸ���ĻҶ�ͶӰ�㷨��Ӧ��,�Ժ�ӱ���ܾ���,�й���ѧԺ������ѧ���ܻ�е�������о��������ֳ���
%(3)������ڵ�������ĻҶ�ͶӰ�㷨���ȵķ���,�� ƽ����ǰ�������ѣ��� ��(�й���ѧԺ������ѧ���ܻ�е�������о��������ֳ���130022)
clc;clear all;close all
offset_V = 20;%���±�Եȥ������ע�⣺offset_V �� offset_HҪ����ʵ�ʿ��ܵ���󶶶����ʵ�ȡֵ��������̫��̫��Ļ���ؼ���ʱ�ƶ�λ�õĺ�ѡλ�þ��٣����Ի�Ч�������ή�ͣ�ȡ20��30�Ƚ�����
offset_H = 30;%���ұ�Եȥ����

d1 = 'F:\pub\bin\GPA\DOUDONG\test\*.bmp';
d2 = dir(d1);
numframe = length(d2);
frame_name = num2str(d2(1).name);
frame_path = strcat('F:\pub\bin\GPA\DOUDONG\test\',frame_name);
referenceframe_rgb = imread(frame_path);
[row,col,d] = size(referenceframe_rgb);
referenceframe = rgb2gray(referenceframe_rgb);



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% �ο�֡����ͶӰ����ͶӰ�Լ������˲� %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%**********ͶӰ*********%
refprojrow = zeros(row,1);%�洢�ο�֡����ͶӰ
refprojrow     = sum(referenceframe,2);
refprojrowsum  = sum(refprojrow);
refprojrowmean = refprojrowsum / row;
refprojrow     = refprojrow - refprojrowmean;

refprojcol = zeros(1,col);%�洢�ο�֡����ͶӰ
refprojcol     = sum(referenceframe,1);
refprojcolsum  = sum(refprojcol);
refprojcolmean = refprojcolsum / col;
refprojcol     = refprojcol - refprojcolmean;


%**********���ڱ�Ե���������˲�*********%
for i=1:row
    if (i<offset_V)||(i>row-offset_V)
        refprojrow(i)=refprojrow(i)*((1+cos(pi*(offset_V-1-i)/offset_V))/2);%���߸�Ϊ��refprojrow(i)=refprojrow(i)*(1+cos(pi*(offset_V-1-i)/offset_V/2));�ĸ�Ч�����þ����ĸ����ĵĻ��������Ƶĵط�����
    end
end

for j=1:col
    if (j<offset_H)||(j>col-offset_H)
        refprojcol(j)=refprojcol(j)*((1+cos(pi*(offset_H-1-j)/offset_H))/2);
    end
end



%%%%%%%%%%%%%%%%%%%
% ��ǰ֡���������� %
%%%%%%%%%%%%%%%%%%%

for t = 2:numframe
    if mod(t,20) == 0
        clc;
        disp(strcat('���ڽ��������������',num2str(round(t*100/numframe)),'%'));
    end
    frame_name = num2str(d2(t).name);
    frame_path = strcat('F:\pub\bin\GPA\DOUDONG\test\',frame_name);
    currentframe_rgb = imread(frame_path);
    currentframe = rgb2gray(currentframe_rgb);
    
    
    %**********ͶӰ*********%
    curprojrow = zeros(row,1);
    curprojrow     = sum(currentframe,2);
    curprojrowsum  = sum(curprojrow);
    curprojrowmean = curprojrowsum / row;
    curprojrow     = curprojrow - curprojrowmean;

    curprojcol = zeros(1,col);
    curprojcol     = sum(currentframe,1);
    curprojcolsum  = sum(curprojcol);
    curprojcolmean = curprojcolsum / col;
    curprojcol     = curprojcol - curprojcolmean;
    
    
    %**********�����˲�*********%
    for i=1:row
        if (i<offset_V)||(i>row-offset_V)
            curprojrow(i)=curprojrow(i)*((1+cos(pi*(offset_V-1-i)/offset_V))/2);
        end
    end

    for j=1:col
        if (j<offset_H)||(j>col-offset_H)
            curprojcol(j)=curprojcol(j)*((1+cos(pi*(offset_H-1-j)/offset_H))/2);
        end
    end
    
    
    %**********�������������˶�ʸ��*********%
    correlation_row = zeros(1,2*offset_V+1);
    extend_row = zeros(2*offset_V+1,1);
    refprojrow = [refprojrow;extend_row];
    curprojrow = [curprojrow;extend_row];
    for j=1:2*offset_V+1
        for i=1:row-2*offset_V
            correlation_row(j)=correlation_row(j)+(refprojrow(j+i-1)-curprojrow(offset_V+i))^2;
        end
    end
    [temp,min_index]=min(correlation_row); 
    dy=(offset_V+1)-min_index;%��ֱ������(����),����ֱ�����˶�ʸ��
    
    correlation_col = zeros(1,2*offset_H+1);
    extend_col = zeros(1,2*offset_H+1);
    refprojcol = [refprojcol extend_col];
    curprojcol = [curprojcol extend_col];
    for i=1:2*offset_H+1
        for j=1:col-2*offset_H
            correlation_col(i)=correlation_col(i)+(refprojcol(j+i-1)-curprojcol(offset_H+j))^2;
        end
    end
    [temp,min_index]=min(correlation_col); 
    dx=(offset_H+1)-min_index;%ˮƽ������(����)����ˮƽ�����˶�ʸ��
    
    
    %**********�˶�����*********%
    if     dy < 0 %dy < 0��ǰ֡currentframe����ڲο�֡referenceframe�����˶���|dy|(�벻���׾ͻ�ͼ)
        temp_sub   = zeros(abs(dy),col,d);
        tempframe_rgb  = [temp_sub;currentframe_rgb(1:(row - abs(dy)),:,:)];
    elseif dy > 0 %dy > 0��ǰ֡currentframe����ڲο�֡referenceframe�����˶���|dy|
        temp_sub   = zeros(abs(dy),col,d);
        tempframe_rgb  = [currentframe_rgb((1+abs(dy)):row,:,:);temp_sub];
    else          %dy = 0��ǰ֡currentframe����ڲο�֡referenceframeû���˶�
        tempframe_rgb  = currentframe_rgb;
    end

    if     dx < 0 %dx < 0��ǰ֡currentframe����ڲο�֡referenceframe�����˶���|dx|(�벻���׾ͻ�ͼ)
        temp_sub   = zeros(row,abs(dx),d);
        tempframe_rgb  = [temp_sub tempframe_rgb(:,1:(col - abs(dx)),:)];
    elseif dx > 0 %dx > 0��ǰ֡currentframe����ڲο�֡referenceframe�����˶���|dx|
        temp_sub   = zeros(row,abs(dx),d);
        tempframe_rgb  = [tempframe_rgb(:,1+abs(dx):col,:) temp_sub];
    end
    compensateframe_rgb = tempframe_rgb;%��ǰ����֡(�ȶ�֡)
    
    
    %**********�ѵ�ǰ�ȶ�֡��Ϊ��һ֡�Ĳο�֡*********%
    referenceframe_rgb = compensateframe_rgb;
    [row,col,d] = size(referenceframe_rgb);
    referenceframe = rgb2gray(referenceframe_rgb);
    
    %**ͶӰ**%
    refprojrow = zeros(row,1);%�洢�ο�֡����ͶӰ
    refprojrow     = sum(referenceframe,2);
    refprojrowsum  = sum(refprojrow);
    refprojrowmean = refprojrowsum / row;
    refprojrow     = refprojrow - refprojrowmean;

    refprojcol = zeros(1,col);%�洢�ο�֡����ͶӰ
    refprojcol     = sum(referenceframe,1);
    refprojcolsum  = sum(refprojcol);
    refprojcolmean = refprojcolsum / col;
    refprojcol     = refprojcol - refprojcolmean;

    %**�����˲�**%
    for i=1:row
        if (i<offset_V)||(i>row-offset_V)
            refprojrow(i)=refprojrow(i)*((1+cos(pi*(offset_V-1-i)/offset_V))/2);
        end
    end

    for j=1:col
        if (j<offset_H)||(j>col-offset_H)
            refprojcol(j)=refprojcol(j)*((1+cos(pi*(offset_H-1-j)/offset_H))/2);
        end
    end

    
    resultframe_rgb = compensateframe_rgb(offset_V+1:row-offset_V,offset_H+1:col-offset_H,:);%�Ա�Ե���нض�
    fname_total = strcat('F:\pub\bin\GPA\DOUDONG\out\',frame_name);
    imwrite(resultframe_rgb,fname_total);
end
clc;


