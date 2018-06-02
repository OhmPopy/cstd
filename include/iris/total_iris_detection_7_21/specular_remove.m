% specular highlight remove
% ���� %5������thres ��ֵ��ͼ��,�������ֵ�������������Ϊͼ��������
% �仯�Ƚ�С�������𲽼�Сthresֱ���� ���������С��һ��ֵ���ж���ֵ��
% ������ Ȼ�����ö�ֵ��ͼ�� �������ͣ��õ���ֵ��ͼ
% Ȼ�󽫸�������������Χ���ص��Ȩ��䡣
function I_inpaint = specular_remove(I)
    [row,col] = size(I);
    % ͳ�ƻҶ�ͼ��5% �ĸ��������ط�Χ
    All_num_pix_I = row * col;
    [n, xout] = imhist(I);
    n_fanzhuan = fliplr(n);  % ������ n ��ת  255�����ظ�����ǰ��
    n_leijihe = zeros(1,length(xout));  % �ۼƺͣ���������������ı�
    for ii = 1:length(xout)
        n_leijihe(ii) = sum(n_fanzhuan(1:ii))/All_num_pix_I;
    end
    ind = find(n_leijihe>0.05,1,'first');
    thres = 256 - ind;
    [aa, area_before] = bw_pic(I, thres);

    for threshold = thres-2 : -2 : 1
        [I_bw_last, area_last] = bw_pic(I, threshold);
%         if area_last/area_before < 1.05
        if (area_last-area_before)/area_before > 0.03 && (area_last-area_before)>10
%         if (area_last-area_before)>10
            I_bw = I_bw_last;
            break;
        end
        area_before = area_last;
    end

%     I_bw1 = bwareaopen(I_bw, 400, 8); % ȥ����С�ĵ�
    I_bw1 = my_areaopen(I_bw, 400);   % 7_20 ȥ����С�ĵ�
    I_bw = I_bw - I_bw1; % ȥ���ܴ�ĵ㣬Ϊ������ٶ�
    
%     se = strel('disk', 2);            % 7_20
%     I_bw2 = imdilate(I_bw, se);
    I_bw2 = mydilate(I_bw,'se2');
    % ��Ϊ��Ĥ����ͼƬ���Ĵ������԰Ѷ�ֵ��ͼ���ܱ߱�Ϊ0,�����������
    for ii= 1 : 15 
        for jj = 1:col
            I_bw2(ii,jj) = 0;
        end
    end
    for ii = row - 14 :row
        for jj = 1:col
            I_bw2(ii,jj) = 0;
        end
    end
    for ii = 1 : row
        for jj = 1:15
            I_bw2(ii,jj) = 0;
        end
    end
    for ii = 1 : row
        for jj = col-14 : col
            I_bw2(ii,jj) = 0;
        end
    end

    I_inpaint = I;
    I = double(I);
    for ii = 5 : row - 5
        for jj = 5 : col - 5
            if I_bw2(ii, jj) == 1 % �ҵ�sqecular��
                % �ҵ��˵�������ĸ��㣨left,right,top,down��
                % ��left�ڽ���
                for iii =  ii : -1 : 1
                    if sum(I_bw2(iii-2:iii-1, jj)) == 0 && I_bw2(iii, jj) == 1
                        top = iii-1;
                        break;
                    end
                end
                for iii = ii : 1 : row
                    if sum(I_bw2(iii+1:iii+2, jj)) == 0 && I_bw2(iii, jj) == 1
                        down = iii+1;
                        break;
                    end
                end
                for jjj = jj : -1 : 1
                    if sum(I_bw2(ii, jjj-2:jjj-1)) == 0 && I_bw2(ii, jjj) == 1
                        left = jjj-1;
                        break;
                    end
                end
                for jjj = jj : 1 : col
                    if sum(I_bw2(ii, jjj+1:jjj+2)) == 0 && I_bw2(ii, jjj) == 1
                        right = jjj+1;
                        break;
                    end
                end
                % ע���ʱ��I��ȡֵ��Χ��Ҫ������int8�ͣ��ܿ��ܳ��ִ���
                I_inpaint(ii,jj) = floor((I(ii,left)*(right-jj) + I(ii,right)*(jj-left))/(2*(right-left))...
                    + (I(top,jj)*(down-ii) + I(down,jj)*(ii-top))/(2*(down-top)));         
            end
        end
    end
    