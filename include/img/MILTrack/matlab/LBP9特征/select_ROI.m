%��������:ѡ�����Ȥ������(��ʼ)
%��������ס����϶�
function [Y1,X1,Y3,X3] = select_ROI(I);
figure,imshow(I);
%while 1 %��ѭ�����Ի��������
% set(gcf,'Position',[3,35,1200,800]);
k = waitforbuttonpress;%�ȴ��������
point1 = get(gca,'CurrentPoint');  %mouse pressed
rectregion = rbbox; %������ѡ�������
point2 = get(gca,'CurrentPoint');
point1 = point1(1,1:2);              % extract col/row min and maxs
point2 = point2(1,1:2);
zuoshang = min(point1, point2);
youxia = max(point1, point2);
X1 = floor(zuoshang(1));
X3 = floor(youxia(1));
Y1 = floor(zuoshang(2));
Y3 = floor(youxia(2));

c_wide=X3-X1;
r_height=Y3-Y1;

hold on;
rectangle('Position',[X1,Y1,c_wide,r_height],'Curvature',[0,0],'LineWidth',2,'LineStyle','-','EdgeColor','r');
hold off;
%end





