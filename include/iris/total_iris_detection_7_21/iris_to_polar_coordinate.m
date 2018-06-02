% ������Բ�ļ�����ת��
function polarv = iris_to_polar_coordinate(pic, x0, y0, Rmax, pic_xsize, pic_ysize)

% ���弫����ͼ���С �� ��λ��ࣨ��360�ݣ�
np = 446;  dp = pi / 180; 

polarv = zeros(np, Rmax);
pic = double(pic);
num_i = 1;
for i = 1 : 2 : 45          % ��0��44����Ϊ2�㣬���б仯    
    for j = 1 : Rmax
        theta = dp * i;
        % ��ֱ�������ж�Ӧ�ĵ�
        x = x0 + j*cos(theta);
        y = y0 + j*sin(theta);
        if x < 0
            x = 1;
        else if x > pic_xsize
                x = pic_xsize-1;
            end
        end                
        if y < 0
            y = 1;
        else if y > pic_ysize
                y = pic_ysize-1;
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
        polarv(num_i, j) = round(abs(e * (y - y1) + f * (y2 - y)));       
    end
    num_i = num_i + 1;
end 
for i = 46 : 0.5 : 134          % ��0��44����Ϊ2�㣬���б仯    
    for j = 1 : Rmax
%         if num_i == 112 && j == 1
%                num_i  = 112;
%         end
        theta = dp * i;
        % ��ֱ�������ж�Ӧ�ĵ�
        x = x0 + j*cos(theta);
        y = y0 + j*sin(theta);
        if x < 0
            x = 1;
        else if x > pic_xsize
                x = pic_xsize-1;
            end
        end                
        if y < 0
            y = 1;
        else if y > pic_ysize
                y = pic_ysize-1;
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
        polarv(num_i, j) = round(abs(e * (y - y1) + f * (y2 - y)));
    end
    num_i = num_i + 1;
end
for i = 135 : 2 : 225          % ��0��44����Ϊ2�㣬���б仯    
    for j = 1 : Rmax
%         if i == 161 && j == 101
%             i = 161;
%         end
        theta = dp * i;
        % ��ֱ�������ж�Ӧ�ĵ�
        x = x0 + j*cos(theta);
        y = y0 + j*sin(theta);
        if x < 0
            x = 1;
        else if x > pic_xsize
                x = pic_xsize-1;
            end
        end                
        if y < 0
            y = 1;
        else if y > pic_ysize
                y = pic_ysize-1;
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
        polarv(num_i, j) = round(abs(e * (y - y1) + f * (y2 - y)));        
    end
    num_i = num_i + 1;
end
for i = 226 : 0.5 : 314          % ��0��44����Ϊ2�㣬���б仯    
    for j = 1 : Rmax
%         if i == 161 && j == 101
%             i = 161;
%         end
        theta = dp * i;
        % ��ֱ�������ж�Ӧ�ĵ�
        x = x0 + j*cos(theta);
        y = y0 + j*sin(theta);
        if x < 0
            x = 1;
        else if x > pic_xsize
                x = pic_xsize-1;
            end
        end                
        if y < 0
            y = 1;
        else if y > pic_ysize
                y = pic_ysize-1;
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
        polarv(num_i, j) = round(abs(e * (y - y1) + f * (y2 - y)));       
    end
     num_i = num_i + 1;
end
for i = 315 : 2 : 360          % ��0��44����Ϊ2�㣬���б仯    
    for j = 1 : Rmax
%         if i == 161 && j == 101
%             i = 161;
%         end
        theta = dp * i;
        % ��ֱ�������ж�Ӧ�ĵ�
        x = x0 + j*cos(theta);
        y = y0 + j*sin(theta);
        if x < 0
            x = 1;
        else if x > pic_xsize
                x = pic_xsize-1;
            end
        end                
        if y < 0
            y = 1;
        else if y > pic_ysize
                y = pic_ysize-1;
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
        polarv(num_i, j) = round(abs(e * (y - y1) + f * (y2 - y)));        
    end
    num_i = num_i + 1;
end
% ��ʾ�������µ�ͼ��
% figure; imshow(polarv,[]);  