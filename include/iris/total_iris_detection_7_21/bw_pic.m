% ��ֵ��ͼ��, ���õ���Ӧ��ɫ���
function [I_bw, area] = bw_pic(I,thres)

I_bw = false(size(I));
[row, col] = size(I);
for ii = 1:row
    for jj = 1:col
        if I(ii,jj) > thres
            I_bw(ii,jj) = 1;
        end
    end
end
% I_bw1 = bwareaopen(I_bw, 400, 8); % ȥ����С�ĵ�
% I_bw = I_bw - I_bw1; % ȥ����С�ĵ�
area = 0;
% cc = bwconncomp(I_bw, 8);
% for i = 1:length(cc.PixelIdxList)
%     area = area + length(cc.PixelIdxList{i});
% end
cc = my_connect_domain(I_bw);
for i = 1:max(max(cc))
    area = area + length(find(cc == i));
end
