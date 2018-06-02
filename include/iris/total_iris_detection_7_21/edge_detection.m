function [p_row, p_col, Number_sample_edge] = edge_detection(polarv, Rmax)
% p_col Ϊ�������Բ��ȡ��ļ�����������
% p_row Ϊ�������Բ��ȡ��ļ�����������
% Number_sample_edge Ϊ�������Բ��ȡ��ĸ���
% polarv Ϊ������ͼ��, RmaxΪ������뾶���ֵ

% ��Ե��⼰��ʾ
polarv = im2bw(polarv,0.1161);
% polarv1 = bwareaopen(polarv, 400, 4);    % 7_20
polarv = my_areaopen(polarv, 400);
% I_edge = edge(polarv,'canny','vertical');   % 7_20

scaling = 1.0;    sigma = 1;    vert = 1.0;    horz = 0.0;
% generate the edge image
I_edge = canny(polarv, sigma, scaling, vert, horz, 1.9, 1.5);    % edge detection 4-19
I_edge = im2bw(I_edge,0.3);

%%%%%%%%%%%%%%%%��ͫ�ױ�Ե��%%%%%%%%%%%%%%%%%%%%
edge_number = 0;          % 4_11 edge_number Ϊȡ�ı�Ե������� Ҳ����ȡ�ĽǶȸ���
for i = 1 : 135         % 4_11 �����е�����һ���ֱ�Ե ÿ��Ҳ����ÿ���Ƕ���ֻȡһ����Ե��
    for j = 1 : Rmax
        if(I_edge(i,j) == 1)
            edge_number = edge_number + 1;
            edge_location_row(edge_number) = i;  % 4_8 �����ڼ������ϵ�λ��
            edge_location_col(edge_number) = j;  % 4_8 �����ڼ������ϵ�λ��
            break;
        end
    end
end
for i = 225 : 360  % 4_11 �����е�����һ���ֱ�Ե ÿ��Ҳ����ÿ���Ƕ���ֻȡһ����Ե��
    for j = 1 : Rmax
        if(I_edge(i,j) == 1)
            edge_number = edge_number + 1;
            edge_location_row(edge_number) = i;  % 4_8 �����ڼ������ϵ�λ��
            edge_location_col(edge_number) = j;  % 4_8 �����ڼ������ϵ�λ��
            break;
        end
    end
end

% ����ϵ�
duan_dian = [];  % ��������߶ε������ ����Ϊ��׼ �ڶ����õ���p_row_y2��������������2��Ϊһ���ϵ�
line_length = [];  % ��ŵ�ǰ�߶εĳ���
point = 1;
duan_dian(point) = 1;
for i = 2 : edge_number
    if abs((edge_location_col(i) - edge_location_col(i-1))) > 1
        point = point + 1;
        duan_dian(point) = i;
        line_length(point -1) = i - duan_dian(point-1);
    end
end
duan_dian(point+1) = edge_number + 1;
line_length(point) = edge_number + 1 - duan_dian(point);
N = 0;  % 4-13 ѡ�еı�Ե��
for i = 1 : length(line_length)
    if line_length(i) > 25     % 8 Ϊ�������ȣ�С��8���ȵ��߶ν�������
        for j = duan_dian(i) : duan_dian(i+1)-1
            N = N + 1;
            Final_good_edge_row(N) = edge_location_row(j);
            Final_good_edge_col(N) = edge_location_col(j);
        end
    end
end

% ȡ�ڱ�Ե��
Number_sample_edge = 0;
canshu2=5;
if N < canshu2
    i_cs = 1;
else
    i_cs = canshu2;
end
for i = i_cs : canshu2 : N         % 20 �ǿɵ���  ��Ҫ���ڸı�sample��Ķ���
    Number_sample_edge = Number_sample_edge + 1;
    p_col(Number_sample_edge) = Final_good_edge_col(i);
    p_row(Number_sample_edge) = Final_good_edge_row(i);
end


