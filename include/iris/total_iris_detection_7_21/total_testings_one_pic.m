% ����ͼ��Pic�õ���Ӧiris���ֵļ������µ�ͼ��pic_normalize��360*30��
% ��Ӧ��pic_normalize����ĤΪmask_normalize��0Ϊ��Ч�㣬1Ϊ��Ч��
function [pic_normalize, mask_normalize] = total_testings_one_pic(Pic)

% ȥ��reflection
Iris = specular_remove(Pic);
% ��ֵ�˲�
Iris1 = medfilt2(Iris,[5,5]);

% ��pupil��pp�㷨���õ�ԭʼͼ��ĺ�Ĥ��Բ����Ӧֱ�������µ�Բ��(nx,ny),�뾶nr
[nx, ny, nr] = pp(Iris1);

% �õ������������ߣ�ֱ�������£�z_topÿ�ж�Ӧһ��ֵΪ��ǰ������������
z_top = TopEyelidLocation(Iris, nr, nx, ny);

% �������������������ϵĲ��ּ��������ð�ɫ255��ʾ,��������Բ���ʱȥ��һ�󲿷�����
ROI_rgb = Iris;
[ROI_row, ROI_col] = size(ROI_rgb);
for ii = 1:ROI_col
    if (round(z_top(ii)) <= 0) || (round(z_top(ii)) > ROI_row)
        continue;
    end
    ROI_rgb(1:ceil(z_top(ii)),ii) = 255;
end

% pp�㷨���õ�ԭʼͼ��ĺ�Ĥ��Բ����Ӧֱ�������µ�Բ��(wx,wy),�뾶wr
[wx, wy, wr] = wai_yuan_detection(nx, ny, nr, ROI_rgb);

% �õ������������ߣ�ֱ�������£�z_topÿ�ж�Ӧһ��ֵΪ��ǰ������������
% �������������Բ�뾶�⣬�򲻽��м�⣬��Ϊ�Ժ����iris��ȡ��������
z_bottom = BottomEyelidLocation(Iris, nr, wr, nx, ny);

% ����ֱ������ϵ�� ���õ�����Բ�Լ���������v_dw,  �Լ���Ĥmask_iris
[v_dw, mask_iris] = iris_detect_show(Pic, z_top, z_bottom, nx, ny, nr, wx, wy, wr);

% �� iris���ֱ�ʾ����  ��һ�� Daugman�ķ���
pic_normalize = Daugmannormalized(nx, ny, nr, wx, wy, wr, Pic);
mask_normalize = Daugmannormalized(nx, ny, nr, wx, wy, wr, mask_iris);

