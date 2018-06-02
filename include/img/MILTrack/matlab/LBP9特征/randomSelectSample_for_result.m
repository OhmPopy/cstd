function YX_candidate = randomSelectSample_for_result(X,Y,sample_w,sample_h,pic_w,pic_h,s,num_candidate_result,num_fix_cr);

YX_candidate = [];
Label = zeros(pic_h,pic_w);%���ڱ���Ѿ�ѡ������꣬�����ظ�ѡ��
%**��[Y X]�ڽ���100(200��300)��������̶����������ѡ,�����ɱ�֤��[Y X]����100(200��300)���ڽ���ѡ�㣬��������ȫ���ѡ���ʹ�õ�[Y X]�ڽ���ѡ����Ŀ���ٵ����**%
if num_fix_cr == 100
    tempY1 = Y - 4;tempX1 = X - 4;
    tempY3 = Y + 5;tempX3 = X + 5;
end
if num_fix_cr == 200
    tempY1 = Y - 4;tempX1 = X - 9;
    tempY3 = Y + 5;tempX3 = X + 10;
end

if num_fix_cr == 300
    tempY1 = Y - 4;tempX1 = X - 14;
    tempY3 = Y + 5;tempX3 = X + 15;
end

if num_fix_cr == 100 || num_fix_cr == 200 || num_fix_cr == 300
    if tempX1 < 1,     tempX1 =     1;end
    if tempY1 < 1,     tempY1 =     1;end
    if tempX3 > pic_w, tempX3 = pic_w;end
    if tempY3 > pic_h, tempY3 = pic_h;end
    for i = tempY1:tempY3
        for j = tempX1:tempX3
            if i + sample_h -1 <= pic_h && j + sample_w -1 <= pic_w
                YX_candidate = [YX_candidate;[i j]];
                Label(i,j)   = 1;
            end
        end
    end
end
%**ʣ�µ�����Ҫ�ڵ�[Y X]�ڽ���100���������ⷶΧs�����ѡ��**%
domainX1 = X - s;
domainY1 = Y - s;
domainX3 = X + s;
domainY3 = Y + s;

if domainX1 < 1,     domainX1 =     1;end
if domainY1 < 1,     domainY1 =     1;end
if domainX3 > pic_w, domainX3 = pic_w;end
if domainY3 > pic_h, domainY3 = pic_h;end

YX_candidate_all = [];
for i = domainY1:domainY3
    for j = domainX1:domainX3
        if Label(i,j) == 1,continue;end
        dis = (Y - i)*(Y - i) + (X - j)*(X - j);
        tempY = i + sample_h -1;
        tempX = j + sample_w -1;
        if  dis <= s * s && tempY <= pic_h && tempX <= pic_w
            YX_candidate_all = [YX_candidate_all;[i j]];
        end
    end
end


if length(YX_candidate_all)~=0
    candidate_ind = unidrnd(size(YX_candidate_all,1),num_candidate_result-num_fix_cr,1);
    for j = 1:size(candidate_ind,1)
        YX(j,1:2) = YX_candidate_all(candidate_ind(j),:);
    end
    YX_candidate = [YX_candidate;YX];
end
if length(YX_candidate)==0,disp('��������߶ȹ���,���²��ܲ�������������������ͣ,����С��������߶�');pause;end











