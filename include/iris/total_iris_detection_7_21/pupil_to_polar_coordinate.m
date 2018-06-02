% 4_8 ת��Ϊ�������µ�ͼ��
% 4-13 ֱ�ӵĴ�(x0,y0)��ֱ���� ��ʱ����תΪ��Ӧ�� theta�Ƕ�
function polarv = pupil_to_polar_coordinate(pic, x0, y0, Rmax)

% ������Բ pupil������ͼ���С �� ��λ��ࣨ��360�ݣ�
np = 360;  dp = pi / 180;
[pic_xsize, pic_ysize] = size(pic);

polarv = zeros(np, Rmax);
pic = double(pic);
for i = 0 : np - 1
    for j = 1 : Rmax
        theta = dp * i;
        % ��ֱ�������ж�Ӧ�ĵ�
        x = x0 + j*cos(theta);
        y = y0 + j*sin(theta);
        if x < 0
            x = 1;
        else if x > pic_xsize
                x = pic_xsize - 1;
            end
        end

        if y < 0
            y = 1;
        else if y > pic_ysize
                y = pic_ysize - 1;
            end
        end
        % ��ֱ�������е���Χ�ĵ�
        x1 = floor(x);
        y1 = floor(y);
        x2 = x1+1;
        y2 = y1+1;
        % ����Խ�����
        [x1, x2, y1, y2] = value_yue_jie(x1, x2, y1, y2, pic_xsize, pic_ysize);
        %��X��Y����ֱ�������Բ�ֵ
        e = pic(x1, y1) * (x - x1) + pic(x2, y1) * (x2 - x);
        f = pic(x1, y2) * (x - x1) + pic(x2, y2) * (x2 - x);
        polarv(i+1, j) = abs(e * (y - y1) + f * (y2 - y));
    end
end
