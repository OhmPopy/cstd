%%%��-pi��+pi�ĽǶ�ת��Ϊ��Ӧ��2λ�����Ʊ���,�����6λ���㣬ת��Ϊʮ������
function pic=thtod(A)
% A=imread('1.bmp');
D=size(A);
sx=D(1);
sy=D(2);
A=A*180/pi;
for i=1:sx;
    for j=1:sy;
      %  A(i,j)=A(i,j)*180/pi;
        if ((0<=A(i,j))&&(A(i,j)<90))
            A(i,j)=0;
        end   
         if ((90<=A(i,j))&&(A(i,j)<=180))
             A(i,j)=128;
         end
        
         if ((-90<=A(i,j))&&(A(i,j)<0))
             A(i,j)=64;
         end
          if ((-180<=A(i,j))&&(A(i,j)<-90))
              A(i,j)=192;
          end
    end
end
pic=A;