
//����Ҷ���任
void fft2(const img_t *src, img_t *dst)
{
  //ʵ�����鲿
  img_t image_Re[1] = {0};
  img_t image_Im[1] = {0};
  img_t Fourier[1] = {0};
  //   int i, j;
  cvSetMat(image_Re, cvGetSize(src), CC_64F, 1);  //ʵ��
  //Imaginary part
  cvSetMat(image_Im, cvGetSize(src), CC_64F, 1);  //�鲿
  //2 channels (image_Re, image_Im)
  cvSetMat(Fourier, cvGetSize(src), CC_64F, 2);
  // Real part conversion from u8 to 64f (double)
  cvConvertScale(src, image_Re, 1, 0);
  // Imaginary part (zeros)
  cvZeroMat(image_Im);
  // Join real and imaginary parts and stock them in Fourier image
  cvMerge(image_Re, image_Im, 0, 0, Fourier);
  // Application of the forward Fourier transform
  cvDFT(Fourier, dst, CC_DXT_FORWARD, 0);
  imfree(image_Re);
  imfree(image_Im);
  imfree(Fourier);
}

int ifftshift(const img_t *src, img_t *dst) {
  int nRow, nCol, i, j, cy, cx;
  const double* _src = img_ptr(double, src);
  double* _dst = img_ptr(double, dst);
  int sstep = src->s/src->c;
  int dstep = dst->s/dst->c;
  nRow = src->height;
  nCol = src->width;
  cy = nRow/2; // image center
  cx = nCol/2;
  for( j = 0; j < cy; j++ ) {
    for( i = 0; i < cx; i++ ) {
      //���Ļ���������ݳ��Ŀ���жԽǽ���
      double t1 = _src[j*sstep+i];
      double t2 = _src[(j+cy)*sstep+i];
      double t3 = _src[j*sstep+i+cx];
      double t4 = _src[(j+cy)*sstep+i+cx];

      _dst[j*dstep+i] = t4;
      _dst[(j+cy)*dstep+i] = t3;
      _dst[j*dstep+i+cx] = t2;
      _dst[(j+cy)*dstep+i+cx] = t1;
    }
  }
  return 0;
}
void fft2shift(img_t *src, img_t *dst)
{
  img_t image_Re[1] = {0};
  img_t image_Im[1] = {0};
  int nRow, nCol, i, j, cy, cx;
  double scale, shift, tmp13, tmp24;
  cvSetMat(image_Re, cvGetSize(src), CC_64F, 1);
  //Imaginary part
  cvSetMat(image_Im, cvGetSize(src), CC_64F, 1);
  cvSplit( src, image_Re, image_Im, 0, 0 );
  //����ԭ���������˹����ͼ����p123
  // Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
  //���㸵��Ҷ��
 // cvAddS( image_Re, cScalar(1.0, 0, 0, 0), image_Re, 0 ); // 1 + Mag
  cvPow( image_Re, image_Re, 2.0);
  cvPow( image_Im, image_Im, 2.0);
  cvAdd( image_Re, image_Im, image_Re, 0);
  cvPow( image_Re, image_Re, 0.5 );
  //�����任����ǿ�Ҷȼ�ϸ��(���ֱ任ʹ��խ���ͻҶ�����ͼ��ֵӳ��
  //һ������ֵ������ɼ�������˹����ͼ����p62)
  // Compute log(1 + Mag);
  cvAddS( image_Re, cScalar(1.0, 0, 0, 0), image_Re, 0 ); // 1 + Mag
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
      CC_IMAGE_ELEM( image_Re, double, j, i) = CC_IMAGE_ELEM( image_Re, double, j+cy, i+cx);
      CC_IMAGE_ELEM( image_Re, double, j+cy, i+cx) = tmp13;
      
      tmp24 = CC_IMAGE_ELEM( image_Re, double, j, i+cx);
      CC_IMAGE_ELEM( image_Re, double, j, i+cx) = CC_IMAGE_ELEM( image_Re, double, j+cy, i);
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
    imfree(image_Re);
    imfree(image_Im);
  }
}

#define cvReal(src, ImageRe) cvSplit(src,ImageRe,0,0,0)

#include "img/imgio.inl"

int test_fft2(int argc, char* argv[]) 
{
  int i = 0;
  img_t im[10] = {0};
  img_t* s = im+i++;
  img_t* d = im+i++;
  img_t* s2 = im+i++;
  cvSetMat(s, 1,32,CC_32F, 1); 
  cvSetMat(d, 1,32,CC_32F, 1); 
  cvSetMat(s2, 1,32,CC_32F, 1); 
  for(i=0;i<32;i++) {
    cvSetReal2D(s,0,i,i); 
  }
  
  //��ʾһ�������ź� 
  for(i=0;i<32;i++) 
    printf("%6.2f ", cvGetReal2D(s,0,i)); 
  printf("\n"); 
  
  //DFT ��ɢ����Ҷ�任 
  cvDFT(s, d, CC_DXT_FORWARD);     //CC_DXT_FORWARD ���������任�����򣭡�Ƶ�� 
  
  printf("The result of DFT: "); 
  for(i=0;i<32;i++) 
    printf("%6.2f ", cvGetReal2D(d,0,i)); 
  
  printf("\n"); 
  //DFT ��ɢ����Ҷ��任 
  cvDFT(d,s2,CC_DXT_INVERSE); //��任 
  printf(" The result of IDFT: ");
  for(i=0;i<32;i++) 
    printf("%6.2f ",cvGetReal2D(s2,0,i)); 
  printf(" "); 
  
  imfrees2(im);
  
  return 0; 
} 
int test_fft3()
{
  //aa+=1;
  //test_cpcode();
  IM_BEGIN(im, 10);
  double m, M;
  double scale;
  double shift;
  {
    double aaa[] = {1,2,3,4};
  }
  sys_chdir("C:/code/lib/opencv-3.2.0/samples/data");
  //����Դͼ�񣬵ڶ���������ʾ�������ͼƬתΪ���ŵ�
  img_t* src = im++;
  imread("lena.jpg", 1, 1, src);
  img_t* Fourier = cvSetMat(im++, cvGetSize(src),CC_64F,2);
  img_t* dst = cvSetMat(im++, cvGetSize(src),CC_64F,2);
  img_t* ImageRe = cvSetMat(im++, cvGetSize(src),CC_64F,1);
  img_t* ImageIm = cvSetMat(im++, cvGetSize(src),CC_64F,1);
  img_t* Image = cvSetMat(im++, cvGetSize(src), src->tid, 1);
  img_t* ImageDst = cvSetMat(im++, cvGetSize(src),src->tid,1);
  fft2(src,Fourier);                  //����Ҷ�任
  //cvOutMatToFile(Fourier, );
  fft2shift(Fourier, Image);          //���Ļ�
  cvDFT(Fourier,dst,CC_DXT_INV_SCALE, 0);//ʵ�ָ���Ҷ��任�����Խ����������
  cvSplit(dst,ImageRe,ImageIm,0,0);
  imshow2("Դͼ��",src);
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
  NamedWindow("����Ҷ��",0);
  imshow2("����Ҷ��",Image);
  NamedWindow("����Ҷ��任",0);
  imshow2("����Ҷ��任",ImageDst);
  //�ͷ�ͼ��
  WaitKey(-1);
  IM_END(im);
  return 0;
}
