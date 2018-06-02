
function [h,grad_m,luv] = getfeature_10(img,bin_size,nbin,sample_factor)
%sample_factor = 4;
%bin_size = 4;
%nbin =6;
%img = single(imread('E:\luobo\databases\AFLW\aflw-images-3\aflw\data\data\flickr\0\image00002.jpg'));

filter = filterBinomial1d( 1 ); % �뾶r=1��1ά����ʽ�˲���
%filter2 = filter*filter';% �뾶r=1��2ά����ʽ�˲���
%img = imfilter(img,filter,'conv'); %����pre-smoothing
im = img;
im = img/255.;
%im(:,:,3) = img(:,:,3)/max(max(img(:,:,3)));
img = im;
[m,n,k] = size(img);

%[H,Grad_M] = hog(single(im),bin_size,nbin);
%LUV = rgbConvert(single(im),'luv');

[H,Grad_M] = hog(single(img),bin_size,nbin);
LUV = rgbConvert(img,'luv');

h = H(:,:,1:nbin);
grad_m = imResample(Grad_M,[m,n]/sample_factor);
luv = imResample(LUV,[m,n]/sample_factor);


%h = imfilter(h,filter,'conv');
%grad_m = imfilter(grad_m,filter,'conv');
%luv = imfilter(luv,filter,'conv');

