
//����Ҷ���任
void fft2(const IplImage *src, IplImage *dst)
{
  //ʵ�����鲿
  IplImage *image_Re = 0, *image_Im = 0, *Fourier = 0;
  //   int i, j;
  image_Re = cvCreateImage(cvGetSize(src), IMG_DEPTH_64F, 1);  //ʵ��
  //Imaginary part
  image_Im = cvCreateImage(cvGetSize(src), IMG_DEPTH_64F, 1);  //�鲿
  //2 channels (image_Re, image_Im)
  Fourier = cvCreateImage(cvGetSize(src), IMG_DEPTH_64F, 2);
  // Real part conversion from u8 to 64f (double)
  cvConvertScale(src, image_Re, 1, 0);
  // Imaginary part (zeros)
  cvZero(image_Im);
  // Join real and imaginary parts and stock them in Fourier image
  cvMerge(image_Re, image_Im, 0, 0, Fourier);
  // Application of the forward Fourier transform
  cvDFT(Fourier, dst, CC_DXT_FORWARD, 0);
  cvReleaseImage(&image_Re);
  cvReleaseImage(&image_Im);
  cvReleaseImage(&Fourier);
}

int ifftshift(IplImage *src, IplImage *dst) {
  int nRow, nCol, i, j, cy, cx;
  nRow = src->height;
  nCol = src->width;
  cy = nRow/2; // image center
  cx = nCol/2;
  for( j = 0; j < cy; j++ ) {
    for( i = 0; i < cx; i++ ) {
      //���Ļ���������ݳ��Ŀ���жԽǽ���
      double t1 = CC_IMAGE_ELEM( src, double, j   , i   );
      double t2 = CC_IMAGE_ELEM( src, double, j+cy, i   );
      double t3 = CC_IMAGE_ELEM( src, double, j   , i+cx);
      double t4 = CC_IMAGE_ELEM( src, double, j+cy, i+cx);

      CC_IMAGE_ELEM( dst, double, j   , i   ) = t4;
      CC_IMAGE_ELEM( dst, double, j+cy, i   ) = t3;
      CC_IMAGE_ELEM( dst, double, j   , i+cx) = t2;
      CC_IMAGE_ELEM( dst, double, j+cy, i+cx) = t1;
    }
  }
  return 0;
}
void fft2shift(IplImage *src, IplImage *dst)
{
  IplImage *image_Re = 0, *image_Im = 0;
  int nRow, nCol, i, j, cy, cx;
  double scale, shift, tmp13, tmp24;
  image_Re = cvCreateImage(cvGetSize(src), IMG_DEPTH_64F, 1);
  //Imaginary part
  image_Im = cvCreateImage(cvGetSize(src), IMG_DEPTH_64F, 1);
  cvSplit( src, image_Re, image_Im, 0, 0 );
  //����ԭ���������˹����ͼ����p123
  // Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
  //���㸵��Ҷ��
  cvPow( image_Re, image_Re, 2.0);
  cvPow( image_Im, image_Im, 2.0);
  cvAdd( image_Re, image_Im, image_Re, 0);
  cvPow( image_Re, image_Re, 0.5 );
  //�����任����ǿ�Ҷȼ�ϸ��(���ֱ任ʹ��խ���ͻҶ�����ͼ��ֵӳ��
  //һ������ֵ������ɼ�������˹����ͼ����p62)
  // Compute log(1 + Mag);
  cvAddS( image_Re, cvScalar(1.0, 0, 0, 0), image_Re, 0 ); // 1 + Mag
  cvLog( image_Re, image_Re ); // log(1 + Mag)
  //Rearrange the quadrants of Fourier image so that the origin is at the image center
  nRow = src->height;
  nCol = src->width;
  cy = nRow/2; // image center
  cx = nCol/2;
  //CC_IMAGE_ELEMΪOpenCV����ĺ꣬������ȡͼ�������ֵ����һ���־��ǽ������ı任
  for( j = 0; j < cy; j++ ){
    for( i = 0; i < cx; i++ ){
      //���Ļ���������ݳ��Ŀ���жԽǽ���
      tmp13 = CC_IMAGE_ELEM( image_Re, double, j, i);
      CC_IMAGE_ELEM( image_Re, double, j, i) = CC_IMAGE_ELEM(
        image_Re, double, j+cy, i+cx);
      CC_IMAGE_ELEM( image_Re, double, j+cy, i+cx) = tmp13;
      
      tmp24 = CC_IMAGE_ELEM( image_Re, double, j, i+cx);
      CC_IMAGE_ELEM( image_Re, double, j, i+cx) =
        CC_IMAGE_ELEM( image_Re, double, j+cy, i);
      CC_IMAGE_ELEM( image_Re, double, j+cy, i) = tmp24;
    }
  }
  {
  //��һ�����������Ԫ��ֵ��һΪ[0,255]
  //[(f(x,y)-minVal)/(maxVal-minVal)]*255
  double minVal = 0, maxVal = 0;
  // Localize minimum and maximum values
  cvMinMaxLoc( image_Re, &minVal, &maxVal, 0, 0, 0 );
  // Normalize image (0 - 255) to be observed as an u8 image
  scale = 255/(maxVal - minVal);
  shift = -minVal * scale;
  cvConvertScale(image_Re, dst, scale, shift);
  cvReleaseImage(&image_Re);
  cvReleaseImage(&image_Im);
  }
}

#define cvReal(src, ImageRe) cvSplit(src,ImageRe,0,0,0)

int test_fft()
{
  IplImage *src;  //Դͼ��
  IplImage *Fourier;   //����Ҷϵ��
  IplImage *dst ;
  IplImage *ImageRe;
  IplImage *ImageIm;
  IplImage *Image;
  IplImage *ImageDst;
  double m,M;
  double scale;
  double shift;
  //����Դͼ�񣬵ڶ���������ʾ�������ͼƬתΪ���ŵ�
  src = cvLoadImage("test_1_100x100.bmp",0);
  Fourier = cvCreateImage(cvGetSize(src),IMG_DEPTH_64F,2);
  dst = cvCreateImage(cvGetSize(src),IMG_DEPTH_64F,2);
  ImageRe = cvCreateImage(cvGetSize(src),IMG_DEPTH_64F,1);
  ImageIm = cvCreateImage(cvGetSize(src),IMG_DEPTH_64F,1);
  Image = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
  ImageDst = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
  fft2(src,Fourier);                  //����Ҷ�任
  fft2shift(Fourier, Image);          //���Ļ�
  cvDFT(Fourier,dst,CC_DXT_INV_SCALE, 0);//ʵ�ָ���Ҷ��任�����Խ����������
  cvSplit(dst,ImageRe,ImageIm,0,0);
  cvNamedWindow("Դͼ��",0);
  cvShowImage("Դͼ��",src);
  //������ÿ��Ԫ��ƽ�����洢�ڵڶ���������
  cvPow(ImageRe,ImageRe,2);
  cvPow(ImageIm,ImageIm,2);
  cvAdd(ImageRe,ImageIm,ImageRe,NULL);
  cvPow(ImageRe,ImageRe,0.5);
  cvMinMaxLoc(ImageRe,&m,&M,NULL,NULL, 0);
  scale = 255/(M - m);
  shift = -m * scale;
  //��shift����ImageRe��Ԫ�ذ��������ŵĽ���ϣ��洢ΪImageDst
  cvConvertScale(ImageRe,ImageDst,scale,shift);
  cvNamedWindow("����Ҷ��",0);
  cvShowImage("����Ҷ��",Image);
  cvNamedWindow("����Ҷ��任",0);
  cvShowImage("����Ҷ��任",ImageDst);
  //�ͷ�ͼ��
  cvWaitKey(10000);
  cvReleaseImage(&src);
  cvReleaseImage(&Image);
  cvReleaseImage(&ImageIm);
  cvReleaseImage(&ImageRe);
  cvReleaseImage(&Fourier);
  cvReleaseImage(&dst);
  cvReleaseImage(&ImageDst);
  return 0;
}

//����OpenCV�ĸ���Ҷ�任����任
#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <cxcore.h>
#include <highgui.h>
#pragma comment(lib, "Comctl32.lib")


int ddd=0;
IplImage*  cvCreateImage1( CvSize size, int depth, int channels ) {
  ++ddd;
  return cvCreateImage( size, depth, channels );
}
IplImage* cvLoadImage1( const char* filename, int iscolor =(CC_LOAD_IMAGE_COLOR)) {
  ++ddd;
  return cvLoadImage( filename, iscolor );
}
#define cvCreateImage   cvCreateImage1
#define cvReleaseImage(image)  (--ddd, cvReleaseImage(image))
//#include "..\\OpenCV1\\OpenCV1.inl"

#include "test_fft.inl"
#include "test_monofilt.inl"

//#include "demhist.inl"
//#include "test_hist.inl"
#include "test_wb.inl"
#include "test_whitebalance.inl"

int makerect() {
  int i, j;
  int cx=10, cy=10;
  for (i=0; i<100; i+=10) {
    for (j=0; j<100; j+=10) {
      printf("%d,%d,%d,%d\n", i, j, cy, cx);
    }
  }
  return 0;
}

FEAT_T aaa[]={
#include "E:/code/cstd/src/monofilt/6��ͼƬ/212/test.txt"
};
FEAT_T bbb[]={
#include "E:/code/cstd/src/monofilt/6��ͼƬ/212/3.txt"
};

int main()
{
  int di = CPM_AbsSub2(66960, aaa, bbb);
#if 1
  test_monofilt();
#else
  test_fft();
  //makerect();
  //test_wb();
  //IplImage* img = cvLoadImage1("E:\\pub\\bin\\feat\\asdfasf.jpg",0);
  //cvSaveImage( "E:\\pub\\bin\\feat\\asdfasf11.jpg", img );
  //test_faceRecognition5();
  //test_faceRecognition4();
  //test_faceRecognition3();
#endif
  return 0;
}

