
% function PH=getfhist(I)
%IΪһ��ͼƬ�����Ϊ����ֱ��ͼ
I=imread('3.bmp');
%I1=imresize(I,[50,100]);
%I=imresize(I,[100 100]);
[f, h1f, h2f, A, theta, psi] = monofilt(I, 3, 4, 2, 0.65, 0);
%[f, h1f, h2f, A, theta, psi] = monofilt(I, 3, 4, 2, 0.65, 0);



MAPPING=getmapping(8,'u2');
fimal=cell(3,1);
for sc=1:3;
    A{sc}=imresize(A{sc},[52 102]);
    LBPMAP=lbp(A{sc},1,8,MAPPING,' ');%��þֲ���ֵ�����ͼ��
  %  LBPMAP=imresize(LBPMAP,[90 100]);
    theta{sc}=thtod(theta{sc});
    final{sc}{1}=double(LBPMAP)+theta{sc};%%�������һ��ģʽ������MBPͼ6*372*60
    
 %   final{sc}{1}=imresize(final{sc}{1},[90 100]);
 fimal{sc}{1}=oversample(final{sc}{1});
end

for t=1:3

    for p=1:372
        PH(1,((372*(t-1)+p-1)*60+1):((372*(t-1)+p)*60))=zhifang60(fimal{t}{1}{p});
         
    end
   
end



% subplot(1,3,1),imshow(mat2gray(final{3}{1}));
% subplot(1,3,2),imshow(mat2gray(final{2}{1}));
% subplot(1,3,3),imshow(mat2gray(final{1}{1}));
%%%%%%%%%%%%%%%%%%%%%%%%%%%��ÿ��ģʽ������MBPͼ�ֿ�


% B=cell(5,5);
% B=fenkuai(final{sc}{1},5,5);
% hist=cell(5,5);
% for i=1:5
%     for j=1:5
%         hist{i}{j}=zhifang(B{i}{j});%%���ÿһ���ֱ��ͼ����Ԫ��hist
%     end
% end
% PEAH=cell(3,1);
% for sc=1:3;
%     evbh=cell(5,5);
%     evbh=toBeaH(final{sc}{1},5,5);%��ĳ��ģʽͼ�ֳ�5*5�飬������ÿ�����ֱ��ͼ,����Ԫ��evbh
%     PEAH{sc}{1}=evbh;%PEAH����3��Ԫ����ÿ��Ԫ������25��СԪ�����ֱ�洢25�����ֱ��ͼ
% end
% %%%%����ֱ��ͼ������������ʽΪ3�ŵ�n���ֱ��ͼ��һ��
% 
% PH=zeros(1,19200);
% 
%     for i=1:5
%         for j=1:5
%             for  k=1:3
%                 PH(1,(256*(i-1)*5*3+256*(j-1)*3+(k-1)*256+1):(256*(i-1)*5*3+256*(j-1)*3+k*256))=PEAH{k}{1}{i}{j};
%                 %PH Ϊһ��ͼƬ����ֱ��ͼ 19200
%             end
%         end
%     end
          