function L_ind = SelectFeatures(h_candidateWeakClf,NineBlocks_MFeatures,tempnum_neg,tempnum_pos,K,M)
%�������ܣ���M����ѡ����(����)��ѡ���һЩ��ѵ�,�������ص�������ֵ��ʵ��ѡ������������������ں���������
%������
%h_candidateWeakClfΪ�����������������������������
%NineBlocks_MFeatures�洢ÿ����ͼ���е�������������
%tempnum_neg��tempnum_posΪ����������������
%��������L_ind��Ϊѡ���������������(����)����

h_candidateWeakClf = h_candidateWeakClf/max(max(h_candidateWeakClf));%��һ������ֹh_candidateWeakClf��Ԫ�ع���ʱ��ʹ1/(1+exp(-x))Ϊ0
Label_y = zeros(tempnum_neg + 1,1);
Label_y(end,1) = 1;%�����������Ϊ0,�����������Ϊ1


KM_Rate = K/M;
L_ind = [];
for numB = 1:size(NineBlocks_MFeatures,1)%������ͼ���е�ÿһ��
    one_ninthB_Features = NineBlocks_MFeatures{numB,1};%��ͼ���е���������
    subM       = size(one_ninthB_Features,2);%��ͼ���е��������ĵĸ���
    L = zeros(subM ,1);%subM��������(��ʧ����)
    for ind_subM = 1:subM
        subm =  one_ninthB_Features(ind_subM);%submΪ��ͼ���е�����(����)��������������������������ͼ�����������(����)����������

        P_main     = zeros(tempnum_neg + 1,1);%tempnum_neg + 1Ϊ�ܰ�����ÿһ��������������Ϊһ������������������Ϊһ����
        for i = 1:tempnum_neg%ÿһ��������������Ϊһ����
            temp_neg_p      = 1/(1 + exp(-h_candidateWeakClf(i,subm)));
            P_main(i)       = temp_neg_p;
        end
        temp_pos_p = zeros(tempnum_pos,1);
        temp_multi = 1;
        for j = 1:tempnum_pos%������������Ϊһ����
            temp_pos_p(j,1) = 1/(1 + exp(-h_candidateWeakClf(tempnum_neg+j,subm)));
            temp_multi = temp_multi * (1 - temp_pos_p(j,1));
        end
        P_main(end,1) = 1 - temp_multi;
        L(ind_subM,1) = abs(sum(Label_y.*log(P_main + 0.000000001) + (1-Label_y).*log(1-P_main + 0.000000001),1));

    end
    [L_sort,L_ind_sub] = sort(L);
    selectNumBlock =floor(KM_Rate*subM);%��ÿ����ͼ���е������ѱ�ǵ�����������ѡ���selectNumBlock��
    if selectNumBlock>=1
        for temp_ind = 1:selectNumBlock
            L_ind = [L_ind;one_ninthB_Features(L_ind_sub(temp_ind))];%ѡ���������������(����)����
        end
    else
        continue;
    end
end



