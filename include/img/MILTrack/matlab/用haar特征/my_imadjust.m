function OutImage = my_imadjust(InImage)
%�������ܣ����ӻҶ�ͼ��ĶԱȶȣ���Ч��matlab�Դ�����imadjust,����Ч��OutImage = imadjust(InImage);
%InImageΪ����ĻҶ�ͼ��
%OutImageΪ���ӶԱȶȺ�ĻҶ�ͼ��

%���´����MATLAB�Դ�����stretchlim��ͬ������Ч��lowhigh_in = stretchlim(InImage);%
nbins = 256;
tol = [.01 .99];
tol_low = tol(1);
tol_high = tol(2);
N = imhist(InImage,nbins);%��256��bin��ֱ��ͼ
cdf = cumsum(N)/sum(N);%cumsum(N)Ϊ��N��Ԫ�ص��ۼӺ�
low = find(cdf>tol_low, 1, 'first');%�˾���˼�Ƿ���cdf�еĵ�һ������tol_low��Ԫ������
high = find(cdf>=tol_high, 1, 'first');
if low==high
    lowhigh(:,1) = [1; nbins];
else
    lowhigh(:,1) = [low;high];
end
lowhigh_in = (lowhigh - 1)/(nbins-1);  % convert to range [0 1]
%�ϴ����MATLAB�Դ�����lowhigh_in = stretchlim(InImage);��ͬ%


low_in   = lowhigh_in(1);
high_in  = lowhigh_in(2);
low_out  = 0;
high_out = 1;

temp1 = linspace(0,1,256);
temp1(:) = max(low_in,min(high_in,temp1));

temp2 = ((temp1-low_in)./(high_in-low_in));
temp2(:) = temp2.*(high_out-low_out) + low_out;

lut = im2uint8(temp2);%���ұ����õ��ı���lut = uint8(round(temp2.*255));
OutImage = zeros(size(InImage,1),size(InImage,2));
OutImage= uintlut(InImage,lut);%��InImage�е�100��λ�õĵ�Ԫ��ֵΪa,��OutImage��ͬλ�õ�Ԫ��ֵΪlut(a+1)

