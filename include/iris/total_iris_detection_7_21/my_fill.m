% 2011_7_20 ʢ��
% ����ֵ��ͼ�� ������Ϊ0ʱ�������ܾ���һ�Ĵ����������Ϊ1
function I = my_fill(I_bw)
% �ն����
% ������ͨ��Ȼ����ͨ���ڵĵ������0�㣬����䣿��
% I2 =[0     0     0     0     0;
%      0     1     1     1     0;
%      0     1     0     1     0;
%      0     1     1     1     0;
%      0     0     0     0     0];

I_ind = my_connect_domain(I_bw);
I = I_bw;
[row, col] = size(I_bw); 
for ii = 2 : row-1
    for jj = 2 : col-1
        if ~I_bw(ii,jj)
            [aa,bb,left] = find(I_ind(ii, jj-1:-1:1),1,'first');
            [aa,bb,right] = find(I_ind(ii,jj+1:1:end),1,'first');
            [aa,bb,top] = find(I_ind(ii-1:-1:1,jj),1,'first');
            [aa,bb,bottom] = find(I_ind(ii+1:1:end,jj),1,'first');
            if (~isempty(left))&&(~isempty(right))&&(~isempty(top))&&(~isempty(bottom))
                if (left == right) && (left == top) && (left == bottom)
                    I(ii,jj) = 1;
                end
            end
        end
    end
end
