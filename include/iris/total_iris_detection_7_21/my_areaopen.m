% 2011_7_20 ʢ��
% Ѱ��I_bw��ֵͼ����Ϊ1�ĵ������ĸ����Լ���С8��ͨ
% I_bw = [0 0 0 0 0 0 0 0; 
%     0 1 0 1 1 0 0 0;
%     0 1 0 1 0 0 1 0;
%     0 0 1 0 0 1 1 0;
%     0 0 0 0 1 1 0 0;
%     0 0 0 0 0 0 0 0];
% AREAΪ�趨��С��AREA���������ͨ��ʱ ��֮��Ϊ0
function I = my_areaopen(I_bw, AREA)

I_ind = my_connect_domain(I_bw);
I = I_bw;
% �ҳ�ÿ����ͨ��������С
for ii = 1:max(max(I_ind))
    [r, c] = find(I_ind == ii);
    area = length(r);
    if area <= AREA
        I(r,c) = 0;
    end
end

