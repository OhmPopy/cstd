%%�ó�����������16*8*2=256��OM�˲���%%%%%
clear all;
clc;
delta1a=[0.8 1.7 2.8 3.5];%ˮƽ�����Ӷ����˲��Ŀ��ܳ߶Ȳ���ѡ��
delta1=[0.8 1.7 2.8 3.5];%ˮƽ�����Ӷ����˲��Ŀ��ܳ߶Ȳ���ѡ��
delta2=[1.1 1.8 2.8 3.5];%ˮƽ�����Ӷ����˲��Ŀ��ܳ߶Ȳ���ѡ��
width=[1:4]*6+7;%�����˲�ģ��Ĵ�Сѡ��
disl1=[2 4 6 8 10 12 14 16];%ˮƽ�����Ӷ����˲���ˮƽ֮��ľ���ѡ��
disl2=[4 6 8 10 12 14 16];%ˮƽ�����Ӷ����˲���ˮƽ֮��ľ���ѡ��
ordifilterall=cell(16,8);
%%%%������%%%%%%%%
for j=1:length(delta1)
    for k=1:length(width)
        for h=1:length(disl1)
            z=makekernel(width(k),disl1(h)/2,0,delta1(j));
            z1=makekernel(width(k),-disl1(h)/2,0,delta1(j));
            if j==0
                j, k, h, z
                ttt=1;
            end
            ordifilterall{4*(k-1)+j,h}=z1-z;
        end
    end
end
save ordifilter2L ordifilterall;
clear ordifilterall; clear disl1 delta1;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%������
ordifilterall=cell(16,7);
for j=1:length(delta2)
    for k=1:length(width)
        for h=1:length(disl2)
            z=makekernel(width(k),-disl2(h),0,delta2(j));
            z1=makekernel(width(k),0,0,delta2(j));
            z2=makekernel(width(k),disl2(h),0,delta2(j));
            ordifilter=z-2*z1+z2;
            ordifilterall{4*(k-1)+j,h}=ordifilter;
        end
    end
end
save ordifilter3L ordifilterall;
