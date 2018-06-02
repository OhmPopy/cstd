% Ѱ��I_bw��ֵͼ����Ϊ1�ĵ������ĸ����Լ���С  8��ͨ
% I_bw = [0 0 0 0 0 0 0 0; 
%     0 1 0 1 1 0 0 0;
%     0 1 0 1 0 0 1 0;
%     0 0 1 0 0 1 1 0;
%     0 0 0 0 1 1 0 0;
%     0 0 0 0 0 0 0 0];
% AREAΪ�趨��С��AREA���������ͨ��ʱ ��֮��Ϊ0
function I = my_conncomp(I_bw, AREA)

[row, col] = size(I_bw);
% ��ΪI_bw���ܱ����о�Ϊ0  ����ǰ���������������

% �����Ͻǿ�ʼɨ��
I_ind1 = zeros(size(I_bw));   % �洢Ϊ1�ı��
ind1 = 1;
for ii = 2:row-1
    for jj = 2:col-1
        if I_bw(ii,jj) == 1
            Top_left = I_ind1(ii-1,jj-1) + I_ind1(ii-1,jj) + I_ind1(ii-1,jj+1) + I_ind1(ii,jj-1);
            if Top_left
                vect = [I_ind1(ii-1,jj-1), I_ind1(ii-1,jj), I_ind1(ii-1,jj+1), I_ind1(ii,jj-1)];
                for i = 1 : 4
                    if ~vect(i)
                        vect(i) = 100000;
                    end
                end
                I_ind1(ii,jj) = min(vect);                
            else
                I_ind1(ii,jj) = ind1;
                ind1 = ind1+1;
            end
        end
    end
end

% �����½ǿ�ʼɨ��
I_ind2 = zeros(size(I_bw));   % �洢Ϊ1�ı��
ind2 = 1;
for ii = row-1 : -1 : 2
    for jj = col-1 : -1 : 2
        if I_bw(ii,jj) == 1
            Top_left = I_ind2(ii+1,jj-1) + I_ind2(ii+1,jj) + I_ind2(ii+1,jj+1) + I_ind2(ii,jj+1);
            if Top_left
                vect = [I_ind2(ii+1,jj-1), I_ind2(ii+1,jj), I_ind2(ii+1,jj+1), I_ind2(ii,jj+1)];
                for i = 1 : 4
                    if ~vect(i)
                        vect(i) = 100000;
                    end
                end
                I_ind2(ii,jj) = min(vect);                
            else
                I_ind2(ii,jj) = ind2;
                ind2 = ind2+1;
            end
        end
    end
end

if ind2 > ind1
    I_ind = I_ind1;
    ind = ind1-1;
else
    I_ind = I_ind2;
    ind = ind2-1;
end

I = I_bw;
% �ҳ�ÿ����ͨ��������С
for ii = 1:ind
    [r, c] = find(I_ind == ii);
    area = length(r);
    if area <= AREA
        I(r,c) = 0;
    end
end

