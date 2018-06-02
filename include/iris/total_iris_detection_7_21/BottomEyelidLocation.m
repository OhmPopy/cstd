function  [z, y] = BottomEyelidLocation(img, nr, wr, polar_nx, polar_ny)

load BottomEyelidTrainreferPoint_100

length_point = 0;           % ѡȡ����iris �ڲ��ĵ�ĸ���
num_point = 0;              % �ж��Ƿ����ڵ��ļ���

BottomEyelidROI=img;
BottomEyelidROI_rgb(:,:,1)=img;
BottomEyelidROI_rgb(:,:,2)=img;
BottomEyelidROI_rgb(:,:,3)=img;

[ROI_row ROI_col]=size(BottomEyelidROI);

row_a = [];
col_a = [];
I5 = horizontal_filter(img,'bottom');
% I4Ϊdouble  I5Ϊlogical��

% 6_1 �����ж�����I4ͼ���pupil��iris���ڲ������ж��Ƿ������������ڵ�
% ע�⣺ ���ڵ�ʱ�ض��к�ǿ��canny���߼�⵽��Ҳ���ǿ��Զ�I4����(nr+10,wr+2)��Χ�ڽ���I4==1ͳ�Ƽ���
for jj = 5 : ROI_col-5
    for kk = round(2*ROI_row/3) : ROI_row
        diffdist=sqrt((kk - polar_nx)^2+(jj-polar_ny)^2);
        if (diffdist> nr + 10) && (I5(kk,jj) == 1) && (diffdist < wr + 2)
            num_point = num_point + 1;
        end
    end
end

if num_point <= 70              % �����и���Ҫ�Ĳ����趨��
    z = 1;
    a = [1;1];
else
    % ÿ��ѡ��һ����Ե��   û�еı�Ե�� all_edgepointΪ0
    for jj = 5 : ROI_col-5
        for kk = round(2*ROI_row/3) : ROI_row
            diffdist=sqrt((kk - polar_nx)^2+(jj-polar_ny)^2);
            if (diffdist> nr + 10) && (I5(kk,jj) == 1) && (diffdist < wr + 2)
                % diffabs(1,jj) = kk;
                % all_edgepoint(1,jj) = kk;
                row_a = [row_a,jj];
                col_a = [col_a,kk];
                BottomEyelidROI_rgb(kk,jj,:)=[0,255,0];
                length_point = length_point + 1;
                break;
            end
        end
    end

    a = [row_a;col_a];

    % y = polyfit(a(1, :), a(2, :), 2);
    y = my_polyfit(a(1, :), a(2, :), 2);
    canshu123(1, :) = y;
    x = 1 : 1 : ROI_col;
    z = canshu123(1, 1) * (x .* x) + canshu123(1, 2) * x + canshu123(1, 3);
    for xxx = 1 : 1 : ROI_col
        if(z(1, xxx) > ROI_row)
            z(1, xxx) = ROI_row;
        end
    end
end

