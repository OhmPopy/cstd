clc;
%clear all;
fname='D:\pub\bin\iris\test1pic\S2271L04.jpg';
fname='D:\pub\bin\iris\test1pic\ttt.bmp';
fname='D:\pub\bin\iris\test1pic\S2005L05.jpg';

pathstr = 'D:\pub\bin\iris\test1pic\';
pathstr = 'D:\pub\bin\iris\testPic\';

DD = dir([pathstr '*.jpg']); % �Ѿ�ȥ��specular highlight��ͼƬ
pic_num = length(DD);
results = {pic_num};

pic_num=100;
z_bottom=0;

for i = 1:1:pic_num
    fname0 = DD(i).name;
    if 0
        fname0='S2094R08.jpg';
        fname0='S2320L02.jpg';
        fname0='S2409R04.jpg';
    end
    fname = strcat([pathstr fname0]);
    disp([num2str(i) ' ' fname]);
    Iris = imread(fname);
    ss=size(Iris, 1)/200;
    %Iris = imresize(Iris, [200, 200]);
    ss=1;
    Pic=Iris;
    Pic0=Iris;
    % ����ͼ��Pic�õ���Ӧiris���ֵļ������µ�ͼ��pic_normalize��360*30��
    % ��Ӧ��pic_normalize����ĤΪmask_normalize��0Ϊ��Ч�㣬1Ϊ��Ч��
    % ȥ��reflection
    %Iris = specular_remove(Pic);
    Iris = imresize(Iris, size(Iris)/2);
    Iris = glassRemoval(Iris);

    % ��ֵ�˲�
    % Iris1 = medfilt2(Iris,[5,5]);
    %Iris1 = Iris;

    Pic=Iris;
    Iris1 = Iris;

    % ��pupil��pp�㷨���õ�ԭʼͼ��ĺ�Ĥ��Բ����Ӧֱ�������µ�Բ��(nx,ny),�뾶nr
    [nx, ny, nr] = pp(Iris1);

    nr=nr*2;
    nx=nx*2;
    ny=ny*2;
    if 1
        % �õ������������ߣ�ֱ�������£�z_topÿ�ж�Ӧһ��ֵΪ��ǰ������������
        [z_top y_top]= TopEyelidLocation(Iris, nr, nx, ny);

        % �������������������ϵĲ��ּ��������ð�ɫ255��ʾ,��������Բ���ʱȥ��һ�󲿷�����
        ROI_rgb = Pic0;
        [ROI_row, ROI_col] = size(ROI_rgb);
        for ii = 1:ROI_col
            if (round(z_top(ii)) <= 0) || (round(z_top(ii)) > ROI_row)
                continue;
            end
            ROI_rgb(1:ceil(z_top(ii)),ii) = 255;
        end

        % pp�㷨���õ�ԭʼͼ��ĺ�Ĥ��Բ����Ӧֱ�������µ�Բ��(wx,wy),�뾶wr
        [wx, wy, wr] = wai_yuan_detection(nx/2, ny/2, nr/2, Iris1);

        wx=wx*2;
        wy=wy*2;
        wr=wr*2;
        % �õ������������ߣ�ֱ�������£�z_topÿ�ж�Ӧһ��ֵΪ��ǰ������������
        % �������������Բ�뾶�⣬�򲻽��м�⣬��Ϊ�Ժ����iris��ȡ��������
        %[z_bottom, y_bottom] = BottomEyelidLocation(Pic0, nr, wr, nx, ny);
    end

    % ����ֱ������ϵ�� ���õ�����Բ�Լ���������v_dw,  �Լ���Ĥmask_iris
    [v_dw, mask_iris] = iris_detect_show(Pic0, z_top, z_bottom, nx, ny, nr, wx, wy, wr);
    %[nx, ny, nr, wx, wy, wr]*ss  [y_top y_bottom]
    if 0
        v_nom = Daugmannormalized(nx, ny, nr, wx, wy, wr, Pic0);
        imshow(v_nom);
    end
    if 0
        imshow(v_dw);
    else
        outname = strcat([pathstr 'out_2701\' DD(i).name]);
        imwrite(v_dw, outname);
    end
end

