% 2011_7_20 ʢ��
% 8��ͨ��Ѱ��
function I_ind = my_connect_domain(I_bw)
[row, col] = size(I_bw);
% ��ΪI_bw���ܱ����о�Ϊ0  ����ǰ���������������

% �����Ͻǿ�ʼɨ��
I_ind = zeros(size(I_bw));   % �洢Ϊ1�ı��
ind = 1;
for ii = 2:row-1
    for jj = 2:col-1
        if I_bw(ii,jj) == 1
            Top_left = I_ind(ii-1,jj-1) + I_ind(ii-1,jj) + I_ind(ii-1,jj+1) + I_ind(ii,jj-1);
            if Top_left
                vect = [I_ind(ii-1,jj-1), I_ind(ii-1,jj), I_ind(ii-1,jj+1), I_ind(ii,jj-1)];
                for i = 1 : 4
                    if ~vect(i)
                        vect(i) = 100000;
                    end
                end
                I_ind(ii,jj) = min(vect);                
            else
                I_ind(ii,jj) = ind;
                ind = ind+1;
            end
        end
    end
end

% �����½ǿ�ʼɨ��
for ii = row-1 : -1 : 2
    for jj = col-1 : -1 : 2
        if I_bw(ii,jj) == 1
            Top_left = I_ind(ii+1,jj-1) + I_ind(ii+1,jj) + I_ind(ii+1,jj+1) + I_ind(ii,jj+1);
            if Top_left
                vect = [I_ind(ii+1,jj-1), I_ind(ii+1,jj), I_ind(ii+1,jj+1), I_ind(ii,jj+1)];
                for i = 1 : 4
                    if ~vect(i)
                        vect(i) = 100000;
                    end
                end
                I_ind(ii,jj) = min(vect);  
%            else
%                I_ind(ii,jj);
            end
        end
    end
end