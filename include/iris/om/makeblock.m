
%clc
%%clear �ֿ���ȡ��������
function result=makeblock(im)
%im�Ĵ�СΪ360*30���ָ��ͼ��С���СΪ64*16
m=64;
n=16;
z=0;
result=[];
for i=1:8:360
    for j=1:2:30
        if (i+m)<=360&(j+n)<=30
            z=z+1;
            subimg=im(j:(j+n-1),i:(i+m-1));
            result=[result;sum(subimg(:))];
        end
    end
end

