function [z, y] = TopEyelidLocation(Iris, nr, nx, ny)
% z Ϊ�������ĺ�����ÿ�ж�ӦROI���е�������
% TopEyelidROIΪ����Ȥ���� ROI
% a Ϊѡ������ployfit��������������Ե��
% I5 ΪROI�ĺ����Ե�����
% TopEyelidROI_rgb ΪROI���ʼѡ�����������Ե��
load TopEyelidTrainM1
TopEyelidTrainM1 = y;
load TopEyelidTrainM2
TopEyelidTrainM2 = y;
load TopEyelidTrainM3
TopEyelidTrainM3 = y;
clear y
nr = ceil(nr);
TopEyelidROI = Iris;
% ����ȥ����ë ˮƽ�˲� �õ���Ե���Ľ��
I5 = horizontal_filter(TopEyelidROI,'top');
I5 = imresize(I5, size(I5)*2);
% figure,imshow(I5);
[ROI_row, ROI_col] = size(I5);

diffabs = zeros(1, ROI_col);
all_edgepoint = zeros(1, ROI_col);

% 5_31 ��pupilֱ���Ϸ� �� pupilֱ������зֱ���
for jj = 5 : ny - nr
    for kk = round(3*ROI_row/5) : -1 : 10
        diffdist = sqrt((kk -  nx)^2 + (jj - ny)^2);
        if (diffdist > (nr + 5)) && (I5(kk,jj)==1)
            diffabs(1, jj) = kk + 1;
            all_edgepoint(1, jj) = kk + 1;
            break;
        end
    end
end
for jj = ny+nr : ROI_col-5
    for kk = round(3*ROI_row/5) : -1 : 10
        diffdist = sqrt((kk -  nx)^2 + (jj - ny)^2);
        if (diffdist > (nr + 5)) && (I5(kk,jj)==1)
            diffabs(1, jj) = kk + 1;
            all_edgepoint(1, jj) = kk + 1;
            break;
        end
    end
end
for jj = ny-nr+1 : ny+nr-1
    for kk = round(3*ROI_row/5) : -1 : 10
        diffdist = sqrt((kk -  nx)^2 + (jj - ny)^2);
        if (diffdist < (nr - 5)) && (I5(kk,jj)==1)
            diffabs(1, jj) = kk + 1;
            all_edgepoint(1, jj) = kk + 1;
            break;
        elseif (abs(diffdist) > (nr + 5)) && (I5(kk,jj)==1)
            diffabs(1, jj) = kk + 1;
            all_edgepoint(1, jj) = kk + 1;
            break;
        end
    end
end

row_col = size(diffabs, 2);
xx3 = 1 : row_col;
zz1 = TopEyelidTrainM1(1, 1) * (xx3 .* xx3) + TopEyelidTrainM1(1, 2) * xx3 + TopEyelidTrainM1(1, 3);
zz2 = TopEyelidTrainM2(1, 1) * (xx3 .* xx3) + TopEyelidTrainM2(1, 2) * xx3 + TopEyelidTrainM2(1, 3);
zz3 = TopEyelidTrainM3(1, 1) * (xx3 .* xx3) + TopEyelidTrainM3(1, 2) * xx3 + TopEyelidTrainM3(1, 3);

newdiffabs1 = zeros(row_col, row_col);
newdiffabs2 = newdiffabs1;
newdiffabs3 = newdiffabs1;

new_h_sum1 = zeros(row_col, 1);
new_h_sum2 = zeros(row_col, 1);
new_h_sum3 = zeros(row_col, 1);

for KKK = 1 : row_col
    if KKK == 188
        KKK = 188;
    end
    newdiffabs1(KKK, :) = diffabs + KKK - round(row_col/2) - zz1;         % 6_1�޸�
    newdiffabs2(KKK, :) = diffabs + KKK - round(row_col/2) - zz2;
    newdiffabs3(KKK, :) = diffabs + KKK - round(row_col/2) - zz3;
    yyy = newdiffabs1(KKK, :);
    yyyy = newdiffabs2(KKK, :);
    yyyyy = newdiffabs3(KKK, :);

    for iii = 1 : row_col
        if yyy(iii) >= -2 && yyy(iii) < 6
            new_h_sum1(KKK, 1) = new_h_sum1(KKK, 1) + 1;
        end

        if yyyy(iii) >= -2 && yyyy(iii) < 6
            new_h_sum2(KKK, 1) = new_h_sum2(KKK, 1) + 1;
        end
        if yyyyy(iii) >= -2 && yyyyy(iii) < 6
            new_h_sum3(KKK, 1) = new_h_sum3(KKK, 1) + 1;
        end
    end
end

[H_SUM(1, 1), diffabs_indx1] = max(new_h_sum1);
[H_SUM(1, 2), diffabs_indx2] = max(new_h_sum2);
[H_SUM(1, 3), diffabs_indx3] = max(new_h_sum3);
diffabs_tot(1, :) = newdiffabs1(diffabs_indx1, :);
diffabs_tot(2, :) = newdiffabs2(diffabs_indx2, :);
diffabs_tot(3, :) = newdiffabs3(diffabs_indx3, :);
[aa, M_indx] = max(H_SUM);

[aa, N_N] = (find(abs(diffabs_tot(M_indx, :)) <= 10));

genpoint = all_edgepoint(1, N_N);
a(1, :) = N_N;
a(2, :) = genpoint;
% y = polyfit(a(1, :), a(2, :), 2);
% y = my_polyfit(a(1, :), a(2, :), 2);
y = my_two_polyfit(a(1, :), a(2, :));       % 8_2���޸�
canshu123(1, :) = y;
x = 1 : 1 : ROI_col;
z = canshu123(1, 1) * (x .* x) + canshu123(1, 2) * x + canshu123(1, 3);
for xxx = 1 : 1 : ROI_col
    if(z(1, xxx) > ROI_row)
        z(1, xxx) = ROI_row;
    end
end
