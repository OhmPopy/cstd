function  [MFeatures,NineBlocks_MFeatures] = randomMFeatures(sample_w,sample_h,M,K)
%�˺�������haar������MIL�ĺ���randomMFeatures��ͬ
%��������MFeaturesΪ���к�ѡ��������������
%NineBlocks_MFeatures�洢ÿ����ͼ���е�������������
Label     = zeros(sample_h,sample_w);%��ѡλ�õı��
for m = 1:M
    
    FeatureStPos.y = unidrnd(sample_h,1,1)+5;%����1�����������Χ��6:sample_h+5
    FeatureStPos.x = unidrnd(sample_w,1,1)+5;
    while((Label(FeatureStPos.y-5,FeatureStPos.x-5)) ~=0 )%��ֹ�ظ�ѡ��
        FeatureStPos.y = unidrnd(sample_h,1,1)+5;
        FeatureStPos.x = unidrnd(sample_w,1,1)+5;
    end
    MFeatures(m,1).StPos = FeatureStPos;
    Label(FeatureStPos.y-5,FeatureStPos.x-5) = m;%��m�����ѡ����������λ��
end

%Ҫ���߶�Ϊsample_h��sample_w��������Ϊ9��.ע�����µ�9��3����㷨���õ��������е�9��3����һ����,������Ϊ�˷�ֹ��������������Ķ���
%���ھֲ�(��ͼ�����Ͻ�)������������׵���ƫ������ʧ���٣����Խ�ͼ���Ϊ9����ͼ��ʹÿ����ͼ���ڶ���һЩ����������
sub_h = floor(sample_h/3);%��ͼ��߶�
sub_w = floor(sample_w/3);

block_num = 1;%��ͼ������
NineBlocks_MFeatures ={};%�洢ÿ����ͼ���е��������ģ���9*1��ceil����
for i = 1:3
    for j = 1:3
        temp_y1 = i*sub_h - (sub_h - 1);%(temp_y1,temp_x1)Ϊ��ͼ�����Ͻǵ����꣬(temp_y3,temp_x3)Ϊ��ͼ�����½ǵ�����
        temp_x1 = j*sub_w - (sub_w - 1);
        
        if i<=2
            temp_y3 = i*sub_h;
        else
            temp_y3 = sample_h;
        end
        
        if j<=2
            temp_x3 = j*sub_w;
        else
            temp_x3 = sample_w;
        end

        sub_Label = Label(temp_y1:temp_y3,temp_x1:temp_x3);%sub_LabelΪ��ͼ���е���ѡ�������ĵı��
        ind = find(sub_Label~=0);
        temp = (sub_Label(ind))';
        NineBlocks_MFeatures{block_num,1} = temp;  
        block_num = block_num + 1;
    end
end





