function  [row_sample, col_sample, N] = edge_detection_waiyuan(polarv,pupil_radius)
% ��ֱ��ͼͳ�Ƶķ���ȥ����� 

% ��Ե���
[ROI_row, ROI_col] = size(polarv);
% ȥ����ë �Ա�Ե����Ӱ��
if 0
polarv1 = polarv;
for i = 1 : ROI_row
    for j = 4 : ROI_col - 4
        b = polarv(i, j - 3 : j + 3);
        [d aa] = sort(b);
        polarv1(i, j) = d(1, 6);
    end
end
polarv = medfilt2(polarv1,[3,3]);
end

% �õ���polarv ������һ���е���255�ĵ�, ����һ�еı�Ե���ڿ��ǵķ�Χ
r_first = ROI_row;
r_last = 1;
ind = find(polarv(:,ROI_col) == 255);
if length(ind) >= 1
    r_first = ind(1);
    r_last = ind(length(ind));
end

% for j = 1:ROI_col
%     ind = find(polarv(:,j) == 255);
%     if length(ind) >= 1
%         row_first = [row_first,ind(1)];
%         row_last = [row_last,ind(length(ind))];
%     end
% end
% if isempty(row_first)
%     r_first = ROI_row;
% else
%     r_first = min(row_first);
% end
%     
% if isempty(row_last)
%     r_last = 1;
% else
%     r_last = max(row_last);
% end

scaling = 1.0;    sigma = 2;    vert = 1.0;    horz = 0.0;
% generate the edge image    
I_edge = canny(polarv, sigma, scaling, vert, horz, 1.9, 1.5);    % edge detection 4-19
v_edge_y = false(size(I_edge));
% num_line = 0;
num_up = 0;
num_down = 0;

% if r_first == 1 && r_last == ROI_row  
%     for i = 1:ROI_row
%         max_num = max(I_edge(i, ceil(pupil_radius)+10:end-10));
%         ind = find(I_edge(i,:) == max_num);
%         if max_num > 0.1 % && polarv(i,ind-2:ind+2) ~= 255
%              v_edge_y(i, ind) = 1;
%              num_line = num_line+1;
%         end
%     end 
% else
for i = 1:r_first
    max_num = max(I_edge(i, ceil(pupil_radius)+10:end-10));
    ind = find(I_edge(i,:) == max_num);
    if max_num > 0.1 % && polarv(i,ind-2:ind+2) ~= 255
         v_edge_y(i, ind) = 1;
         num_up = num_up+1;
    end
end 
for i = r_last : ROI_row
    max_num = max(I_edge(i, ceil(pupil_radius)+10:end-10));
    ind = find(I_edge(i,:) == max_num);
    if max_num > 0.1 % && polarv(i,ind-2:ind+2) ~= 255
         v_edge_y(i, ind) = 1;
         num_down = num_down + 1;
    end
end
% figure,imshow(v_edge_y)   figure,imshow(edge_up)

% ����ֱ��ͼͳ�ƣ��õ���Ե�����ļ��У�ȥ�����
edge_up = v_edge_y(1:r_first,:);
counts_up = sum(edge_up); % �洢ÿһ����Ϊ1�ĵ�ĸ���
RR = 3;     % RR Ϊ�ۻ��������غ�
counts_up_leiji = zeros(1, ROI_col - RR + 1);    
for jj = 1 : ROI_col-RR+1
    counts_up_leiji(jj) = sum(counts_up(jj:jj+RR-1));
end
[max_up_num, max_up_ind] = max(counts_up_leiji);

edge_down = v_edge_y(r_last:end,:);
counts_down = sum(edge_down); % �洢ÿһ����Ϊ1�ĵ�ĸ���
RR = 3;     % RR Ϊ�ۻ��������غ�
counts_down_leiji = zeros(1, ROI_col - RR + 1);    
for jj = 1 : ROI_col-RR+1
    counts_down_leiji(jj) = sum(counts_down(jj:jj+RR-1));
end
[max_down_num, max_down_ind] = max(counts_down_leiji);

% ȡ��Ч�� Ϊ max_ind : max_ind+RR-1
edge = false(ROI_row, ROI_col);
for jj = max_up_ind : max_up_ind+RR-1
    for ii = 1:r_first
        edge(ii,jj) = v_edge_y(ii,jj);
        if v_edge_y(ii,jj) == 1
            polarv(ii,jj) = 255;
        end
    end
end
for jj = max_down_ind : max_down_ind+RR-1
    for ii = r_last : ROI_row
        edge(ii,jj) = v_edge_y(ii,jj);
        if v_edge_y(ii,jj) == 1
            polarv(ii,jj) = 255;
        end
    end
end

% ��ȡ�õı�Ե����� ����ȡ��
[edge_row, edge_col] = find(edge);

row_up_sample = edge_row(1 : floor(max_up_num/15) : max_up_num)';
row_down_sample = edge_row(max_down_num : floor(max_down_num/15) : end)';
row_sample = [row_up_sample,row_down_sample];
col_up_sample = edge_col(1 : floor(max_up_num/15) : max_up_num)';
col_down_sample = edge_col(max_down_num : floor(max_down_num/15) : end)';
col_sample = [col_up_sample,col_down_sample];
N = length(row_sample);
