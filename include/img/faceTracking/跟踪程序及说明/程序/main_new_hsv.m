clc;clear;

t=200;%ͼƬ֡��
A=25;%����������
B=10;%���󴰿ڷ�Χ,���˶������Ҵ���̫С��ܿ���ʹM00=0;
B1=5;
D=20;%��������ߴ�,�汳���ı仯��ȡ��ͬ��ֵ��DԽ��Ϊ������ص��Խ��,ע��D��������probmapͼ������Ϊȫ��
F=500;%����ֱ��ͼʱ����ֵ


%(1)ͨ��������������ʼ������С(��ʼ)-----------------------------------
    N=2;%��ʼ��⵽N������

    y_min(1)=150;
    y_max(1)=210;
    x_min(1)=145;
    x_max(1)=190;

%     D=0.5*(y_max(1)-y_min(1));
%     B=0.5*(y_max(1)-y_min(1));
%     B1=0.5*(y_max(1)-y_min(1));

    y_min(2)=185;
    y_max(2)=229;
    x_min(2)=250;
    x_max(2)=286;

    Y_CEN=zeros(10,N);X_CEN=zeros(10,N);
    for i=1:N
        y1_center(i)=(y_max(i)+y_min(i))/2;
        x1_center(i)=(x_max(i)+x_min(i))/2;

        y_center_qian(i)=y1_center(i);
        x_center_qian(i)=x1_center(i);

        S(i)=(y_max(i)-y_min(i))*(x_max(i)-x_min(i));%//Ŀ���������
        Y_CEN(1,i)=y_center_qian(i);
        X_CEN(1,i)=x_center_qian(i);

    end
%   ͨ��������������ʼ������С(����)-----------------------------------


%(2)��ʼ����һ֡Ŀ����ɫֱ��ͼ(��ʼ)------------------------------------
    I=imread('E:\pic\duo\1.bmp');
    for i=1:N
        histogram_h(:,1,i)=my_histogram_h(y_min(i),y_max(i),x_min(i),x_max(i),I,D);%//histogram_h:256*1*1
        histogram_s(:,1,i)=my_histogram_s(y_min(i),y_max(i),x_min(i),x_max(i),I,D);
        histogram_v(:,1,i)=my_histogram_v(y_min(i),y_max(i),x_min(i),x_max(i),I,D);

        linshi1(:,1,i)=histogram_h(:,1,i);%//����ֱ��ͼʱ�õ�
        linshi2(:,1,i)=histogram_s(:,1,i);
        linshi3(:,1,i)=histogram_v(:,1,i);
    end
%   ��ʼ����һ֡Ŀ����ɫֱ��ͼ(��ʼ)------------------------------------


tic;
%(3)�����߸��¡�����camshift����(��ʼ)----------------------------------
    for k=2:t
        k
        for  ii=1:N
            %����ʷֲ�ͼ��(��ʼ)---------------------------
            filename = sprintf('E:/pic/duo/%.1d.bmp',k);
            I1=imread(filename);
            I1_hsv=rgb2hsv(I1);

            h=I1_hsv(:,:,1);s=I1_hsv(:,:,2);v=I1_hsv(:,:,3);
            h=h*255;s=s*255;v=v*255;
            h=uint8(h);s=uint8(s);v=uint8(v);
            [m,n]=size(h);
            probmap_h = zeros(m,n);probmap_s = zeros(m,n);probmap_v= zeros(m,n);
            for i=1:m
                for j=1:n
                        probmap_h(i,j)=histogram_h(h(i,j)+1,1,ii);
                        probmap_s(i,j)=histogram_s(s(i,j)+1,1,ii);
                        probmap_v(i,j)=histogram_v(v(i,j)+1,1,ii);
                end
            end
            probmap=probmap_h.*probmap_s.*probmap_v;
            probmap=bwmorph(probmap,'dilate');
            %figure,imshow(probmap),axis on;
            %����ʷֲ�ͼ��(����)---------------------------


            %����������(��ʼ)------------------------------
            count=0;
            rowcenter = 0;
            colcenter = 0;
            rowcenterold = 10;
            colcenterold = 10;

            aa=y_min(ii);bb=y_max(ii);cc=x_min(ii);dd=x_max(ii);
            gao=bb-aa;kuan=dd-cc;

            %while  (((abs(rowcenter - rowcenterold) > 1) && (abs(colcenter - colcenterold) > 1)) || (count < A))
            %while  (((abs(rowcenter - rowcenterold) > 1) && (abs(colcenter - colcenterold) > 1)))
            while  (count < A)%//ֻ��count < Aһ���������Է�ֹ��ȥ�ֲ���Сֵ��ȱ���ǿ���������������
                    ymin=floor(aa-B);ymax=floor(bb+B1);xmin=floor(cc-B);xmax=floor(dd+B);%//������������
                    rowcenterold = rowcenter;colcenterold = colcenter;

                    if ymin<1,a=ymax-ymin;ymin=1;ymax=ymin+a;if ymax>m,ymax=m;end;end%//��ֹԽ��
                    if ymax>m,b=ymax-ymin;ymax=m;ymin=ymax-b;if ymin<1,ymin=1;end;end
                    if xmin<1,c=xmax-xmin;xmin=1;xmax=xmin+c;if xmax>n,xmax=n;end;end
                    if xmax>n,d=xmax-xmin;xmax=n;xmin=xmax-d;if xmin<1,xmin=1;end;end

                    [rowcenter colcenter M00 probmap] = zhongxin(ymin,ymax,xmin,xmax,probmap,I1,linshi1(:,1,ii),linshi2(:,1,ii),linshi3(:,1,ii));

                    aa = floor(rowcenter - gao/2);%//���ϼ���˴���gao��kuanӦ��Ϊ����Ŀ���ʵ�ʳߴ�
                    bb = floor(rowcenter + gao/2);
                    cc = floor(colcenter - kuan/2);
                    dd = floor(colcenter + kuan/2);

                    count = count + 1;
            end
            %����������(����)------------------------------

            if abs(rowcenter-y_center_qian(ii))>60||abs(colcenter-x_center_qian(ii))>60%//��ֹ��������������������
                rowcenter=y_center_qian(ii);
                colcenter=x_center_qian(ii);

                ymin=floor(rowcenter-(ymax-ymin)/2);
                ymax=floor(rowcenter+(ymax-ymin)/2);
                xmin=floor(colcenter-(xmax-xmin)/2);
                xmax=floor(colcenter+(xmax-xmin)/2);
            end

            aa = floor(rowcenter - gao/2);
            bb = floor(rowcenter + gao/2);
            cc = floor(colcenter - kuan/2);
            dd = floor(colcenter + kuan/2);

            ymin=floor(aa-B);ymax=floor(bb+B1);xmin=floor(cc-B);xmax=floor(dd+B);

            if ymin<1,a=ymax-ymin;ymin=1;ymax=ymin+a;if ymax>m,ymax=m;end;end%��ֹԽ��
            if ymax>m,b=ymax-ymin;ymax=m;ymin=ymax-b;if ymin<1,ymin=1;end;end
            if xmin<1,c=xmax-xmin;xmin=1;xmax=xmin+c;if xmax>n,xmax=n;end;end
            if xmax>n,d=xmax-xmin;xmax=n;xmin=xmax-d;if xmin<1,xmin=1;end;end

            y_min(ii)=aa;y_max(ii)=bb;x_min(ii)=cc;x_max(ii)=dd;

            y_center=floor(rowcenter);
            x_center=floor(colcenter);

            %�����ο�(��ʼ)--------------------------------
            y_start=y_center-gao/2;x_start=x_center-kuan/2;
            if ii==1
                close;
                figure,imshow(I1),axis on;
                %figure,imshow(probmap),axis on;
                hold on;
                rectangle('Position',[x_start,y_start,kuan,gao],'Curvature',[0,0],'LineWidth',2,'LineStyle','-','EdgeColor','r');
                hold off;
                %pause(0.05);
            end
            if ii==2
                hold on;
                rectangle('Position',[x_start,y_start,kuan,gao],'Curvature',[0,0],'LineWidth',2,'LineStyle','-','EdgeColor','b');
                hold off;
                pause(0.05);
            end
            %�����ο�(����)--------------------------------
            y_center_qian(ii)=y_center;
            x_center_qian(ii)=x_center;

            num_cen=mod(k-1,10);
            num_cen=num_cen+1;
            Y_CEN(num_cen,ii)=y_center_qian(ii);
            X_CEN(num_cen,ii)=x_center_qian(ii);


            %����ֱ��ͼ(��ʼ)----------------------------------
            y_geng_min=floor(y_start);
            y_geng_max=floor(y_start+gao);
            x_geng_min=floor(x_start);
            x_geng_max=floor(x_start+kuan);

            linshi_histo1=my_histogram_h(y_geng_min,y_geng_max,x_geng_min,x_geng_max,I1,D);
            linshi_histo2=my_histogram_s(y_geng_min,y_geng_max,x_geng_min,x_geng_max,I1,D);
            linshi_histo3=my_histogram_v(y_geng_min,y_geng_max,x_geng_min,x_geng_max,I1,D);

            Dh=0;Ds=0;Dv=0;
            for i=1:256
                Dh=Dh+linshi_histo1(i)*log2(max(linshi_histo1(i),0.00001)/max(linshi1(i,1,ii),0.00001));%//��0.00001��Ϊ�˷�ֹ��ĸΪ��
                Ds=Ds+linshi_histo2(i)*log2(max(linshi_histo2(i),0.00001)/max(linshi1(i,1,ii),0.00001));
                Dv=Dv+linshi_histo3(i)*log2(max(linshi_histo3(i),0.00001)/max(linshi1(i,1,ii),0.00001));
            end
            if Dh<=F
                histogram_h(:,1,ii)=linshi_histo1;
            end
            if Ds<=F
                histogram_s(:,1,ii)=linshi_histo2;
            end
            if Dv<=F
                histogram_v(:,1,ii)=linshi_histo3;
            end
            %����ֱ��ͼ(����)----------------------------------

            %�жϸ�������ʧ(��ʼ)--------------------------------
            dis_cen=0;
            num_dis=0;
            if mod(k,10)==0
                for u=2:10
                    %dis_cen=sqrt((Y_CEN(u,ii)-Y_CEN(u-1,ii))^2+(X_CEN(u,ii)-X_CEN(u-1,ii))^2);
                    dis_cen1=abs(Y_CEN(u,ii)-Y_CEN(u-1,ii));
                    dis_cen2=abs(X_CEN(u,ii)-X_CEN(u-1,ii));
                    %if dis_cen<=3
                    if dis_cen1<=10&&dis_cen2<=10
                        num_dis=num_dis+1;
                    end
                end
                if  M00/S(ii)<0.1&&num_dis==5
                    kk=num2str(k);
                    iii=num2str(ii);
                    st=strcat('��',kk,'֡ʱ��','��',iii,'�˿���ʧ������ʧ');
                    disp(st);
                end
            end
            %�жϸ�������ʧ(��ʼ)--------------------------------

%             if ii==2
%             kk=num2str(k);
%             rusult_path=strcat('E:\result\',kk);
%             saveas(gcf,rusult_path,'jpg');
%             end
        end

    end

%  �����߸��¡�����camshift����(����)----------------------------------
tt=toc


