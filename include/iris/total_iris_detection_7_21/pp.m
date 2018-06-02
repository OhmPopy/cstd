%%%%%%%%%%%%%%%%%%%%%%%%��Ĥ��Բ��λ%%%%%%%%%%%%%%%%%%%%%%%%%    
function [pupil_x, pupil_y, pupil_radius] = pp(img_gray)
   
    % ͼ���ȡ
    I_source = im2double(img_gray);    
    
    % ������ԭ�㣬ͼ������ĵ�
    [pic_xsize, pic_ysize] = size(I_source);
    [x0, y0] = rough_center_7_20(I_source);      % �õ�pupil�Ĵ������ĵ�
    
    % �뾶���ֵ
    Rmax_p = min(round(pic_xsize - x0), round(pic_ysize - y0)) - 1;    
    
    % 4_8 ת��Ϊ�������µ�ͼ��
    polarv = pupil_to_polar_coordinate(I_source, x0, y0, Rmax_p);
    % figure,imshow(polarv)
    
    % 4_8 ��Ե��⣬ ÿ������Ӧһ����Ե��, �������õ��ڱ�Ե�㼰���ڼ������µ�λ��
    [p_row, p_col, N] = edge_detection(polarv, Rmax_p); 

    % ���˶���
    [pupil_radius, pupil_x, pupil_y] = Hooke_law_total(x0, y0, p_col, p_row, N, 100, 0.0051, 'NeiYuan');
%     [pupil_radius, pupil_x, pupil_y] = Hooke_law(x0, y0, p_col, p_row, N, 100, 0.0051);

end
