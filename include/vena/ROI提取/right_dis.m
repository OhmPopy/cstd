function [result1,result2]=right_dis(colall)
%%����Ϊ�������aa����һ��Ϊ������ͼ���������ڶ���Ϊͼ������������߱�Ե��������Ϊͼ�����������ұ߱�Ե
[mm,nn]=size(colall);
mincol=min(colall(:,2));
maxcol=max(colall(:,3));
maxdis=abs(mincol-maxcol);
ddmin=maxdis/1000;
if maxdis>20&&maxdis<200
    aa=[];
    for i=1:mm
        tmp=abs(colall(i,2)-colall(i,3));
        if tmp>20&&tmp<180
            tmping=[i tmp];  %%%�����кźͶ�Ӧ�ľ���
            aa=[aa;tmping];
        end
    end
    d=unique(aa(:,2));       %����������
    for i=1:length(d)
        number(i)=length(find(aa(:,2)==d(i))); %ÿ�־������
    end
    index=find(number(:)==max(number(:)));
    resulting=d(index);%%%��ͬ���������ľ���
    if(length(resulting>2))
        resulting=min(resulting);
    end
    for i=1:size(aa,1)
        if(aa(i,2)==resulting)
            %%%&abs(colall(:,2)-mincol)<ddmin&abs(colall(:,3)-maxcol)<ddmin)
            result1=colall(aa(i,1),2);
            result2=colall(aa(i,1),3);
            break;
        end
    end
else
    result1=0;
    result2=0;
end


