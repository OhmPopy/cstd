% Canny��Ե���ĺ���
% Input:
%   a: input image
%   sigma: Gaussian�ľ�����
function e=canny_edge(a,sigma)

a = double(a);     % ��ͼ����������ת��Ϊdouble����
[m,n] = size(a)
e = repmat(logical(uint8(0)),m,n);  % ���ɳ�ʼ����

GaussianDieOff = .0001;
PercentOfPixelsNotEdges = .7; % Used for selecting thresholds
ThresholdRatio = .4;          % Low thresh is this fraction of the high.

% Design the filters - a gaussian and its derivative

pw = 1:30; % possible widths
ssq = sigma^2;
width = find(exp(-(pw.*pw)/(2*ssq))>GaussianDieOff,1,'last');
if isempty(width)
    width = 1;  % the user entered a really small sigma
end

t = (-width:width);
gau = exp(-(t.*t)/(2*ssq))/(2*pi*ssq);     % the gaussian 1D filter

% Find the directional derivative of 2D Gaussian (along X-axis)
% Since the result is symmetric along X, we can get the derivative along
% Y-axis simply by transposing the result for X direction.
[x,y]=meshgrid(-width:width,-width:width);
%[x,y]=meshgrid(-1:1,-1:1);
dgau2D=-x.*exp(-(x.*x+y.*y)/(2*ssq))/(pi*ssq);

% Convolve the filters with the image in each direction
% The canny edge detector first requires convolution with
% 2D gaussian, and then with the derivitave of a gaussian.
% Since gaussian filter is separable, for smoothing, we can use
% two 1D convolutions in order to achieve the effect of convolving
% with 2D Gaussian.  We convolve along rows and then columns.

%smooth the image out
aSmooth=imfilter(a,gau,'conv','replicate');   % run the filter accross rows
aSmooth=imfilter(aSmooth,gau','conv','replicate'); % and then accross columns

%apply directional derivatives
ax = imfilter(aSmooth, dgau2D, 'conv','replicate');
ay = imfilter(aSmooth, dgau2D', 'conv','replicate');
mag = sqrt((ax.*ax)+(ay.*ay));  % ÿ�����ص���ݶ�ǿ��

magmax = max(mag(:));
if magmax>0
    mag = mag/magmax;  % ��һ��
end

%[counts,x] = imhist(mag,64);    % ֱ��ͼͳ��
%high = min(find(cumsum(counts)>Perc*m*n))/64;
%low = Th*high;
high=0.1;
low=0.04;

thresh = [low high];   % ����ֱ��ͼͳ��ȷ��������

% Nonmax-suppression
idxStrong = [];
for dir = 1:4
    Localmax1 = Findlocalmax(dir,ax,ay,mag);
    idxWeak = Localmax1(mag(Localmax1)>low);
    e(idxWeak) = 1;
    idxStrong = [idxStrong; idxWeak(mag(idxWeak)>high)];
end

rstrong = rem(idxStrong-1,m)+1;
cstrong = floor((idxStrong-1)/m)+1;
e = bwselect(e,cstrong,rstrong,8);  %
e = bwmorph(e,'thin',1);   % ʹ����̬ѧ�ķ����ѱ�Ե��ϸ
