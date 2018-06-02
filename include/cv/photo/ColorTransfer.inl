
#ifndef _COLORTRANSFER_INL_
#define _COLORTRANSFER_INL_

#include "cstd.h"
#include "img/color.inl"


// Welsh������Reinhard���˵Ĳ�ɫͼ���ɫ��Ǩ���㷨�Ļ����ϣ�����˻Ҷ�ͼ���ɫ����˼�룬���������Ӧ���㷨�����㷨��Ҫ���ò���ƥ��������ʵ�ֻҶ�ͼ���ɫ��Ǩ�ƣ���Ϊ�Ҷ�ͼ��ֻ��������Ϣ�����Ը��㷨��Ҫͨ�����ص�����ֵƥ�䣬��ʵ�ֻҶ�ͼ����Զ���ɫ����
// 
// ���岽�����£�
// 
// (1)���ο�ͼ��ͻҶ�ͼ��ֱ���RGB�ռ�ת����l����ɫ�ʿռ䡣
// 
// (2)���ݻҶ�ͼ������ȼ���׼��Բο�ͼ�����������ӳ�䡣
// 
// ���ڲο�ͼ��ͻҶ�ͼ���ֱ��ͼȡֵ��һ����ͬһ��Χ����ô�����һ������ֵ�ܵ͵Ĳο�ͼ����һ������ֵ�ܸߵ�Ŀ��ͼ���ֱ��ͼ���жԱȽ�����ֽϴ������ˣ���Ҫ�Բο�ͼ�����������ӳ�䣺
// 
// L = (nl�� / nl)* (l �C ml) + ml��
// 
// ���У�l��Դͼ��lͨ�������ݣ�L�Ǳ任��õ��µ�Դͼ��lͨ����ֵ��ml��ml�� �ֱ���Դͼ�����ɫͼ���lͨ���ľ�ֵ��nl��nl����ʾ���ǵ�lͨ����׼���
// 
// (3)�Ӳο�ͼ�������ѡȡһ�������㣬�����ص�����Ⱥ�����Χ�����ȵı�׼����������ֵ��ΪȨֵ�����㹫ʽ���£�
// 
// W = l/2 + ��/2
// 
// ���У�wΪȨֵ��lΪ���ص������ֵ����Ϊ�����ص���Χĳ������������ֵ�ı�׼���������Ĵ�С��Welshָ��һ��ȡ5X5���Ը���ͼ��ȡ���������Ч������á�
// 
// (4)����ɨ��Ҷ�ͼ�񣬶���ÿ�����ص㣬�ɹ�ʽ������Ȩֵ�����ڲο�ͼ������������ҵ�һ����ӽ��������㣬�����ƥ��㣬��ƥ���Ľ��Ϳ�ͨ����ֵ�����Ҷ�ͼ��Ķ�Ӧ���ص㣬�Ӷ�ʵ��ɫ�ʵĴ��ݡ�
// 
// (5)���ο�ͼ��ͻҶ�ͼ���l���¿ռ�ת����RGB�ռ䡣
// 

#define max_uchar(a, b)    (((a) > (b)) ? (a) : (b))  
#define min_uchar(a, b)    (((a) < (b)) ? (a) : (b))  

// �����ɫͼ���ֵ�ͱ�׼��  
void CompMeanAndVariance(const img_t* img, double* mean3f, double* variance3f)  
{
  int i, j;
  int row = img->h;
  int col = img->w;
  int total = row * col;  
  double sum[3] = { 0, 0, 0 };
  // ��ֵ
  for (i = 0; i < row; i++) {
    const uchar *pImg = img_row(uchar, img, i);
    for (j = 0; j < col; ++j, pImg+=img->c) {
      sum[0] += pImg[0];  
      sum[1] += pImg[1];  
      sum[2] += pImg[2];
    }
  }  
  
  mean3f[0] = sum[0] / total;  
  mean3f[1] = sum[1] / total;  
  mean3f[2] = sum[2] / total;  
  
  sum[0] = sum[1] = sum[2] = 0; 
  // ��׼��
  for (i = 0; i < row; i++) {
    const uchar *pImg = img_row(uchar, img, i);
    for (j = 0; j < col; ++j, pImg+=img->c) {
      sum[0] += (pImg[0] - mean3f[0]) * (pImg[0] - mean3f[0]);  
      sum[1] += (pImg[1] - mean3f[1]) * (pImg[1] - mean3f[1]);  
      sum[2] += (pImg[2] - mean3f[2]) * (pImg[2] - mean3f[2]);  
    }
  }  
  
  variance3f[0] = sqrt(sum[0] / total);
  variance3f[1] = sqrt(sum[1] / total);
  variance3f[2] = sqrt(sum[2] / total);
}  

// ��ɫת��  
int ColorTransfer(const img_t* src, const img_t* tar, img_t* dst)
{
  img_t im[10] = {0};
  int i=0, j;
  img_t* srcLab = im+i++;
  img_t* tarLab = im+i++;
  double srcMean3f[4] = {0}, tarMean3f[4] = {0};// Դ/Ŀ��ͼ���ֵ  
  double srcVariance3f[4] = {0}, tarVariance3f[4] = {0};// Դ/Ŀ��ͼ���׼��  
  double ratioVariance3f[4] = {0};// ��׼�����  
  
  // BGR�ռ�תLab�ռ�  
  imcolorcvt(src, srcLab, T_BGR, T_Lab);
  imcolorcvt(tar, tarLab, T_BGR, T_Lab);
  if(0) {
    img_t* src2 = im+i++;
    img_t* tar2 = im+i++;
    imcolorcvt(srcLab, src2, T_Lab, T_BGR);
    imcolorcvt(tarLab, tar2, T_Lab, T_BGR);
    imshow(srcLab);
    imshow(tarLab);
    imshow(src2);
    imshow(tar2);
    cvWaitKey(-1);
  }
  // ���㵱ǰͼ����Ŀ��ͼ���ֵ����׼��
  CompMeanAndVariance(srcLab, srcMean3f, srcVariance3f);  
  CompMeanAndVariance(tarLab, tarMean3f, tarVariance3f);  
  
  // ��׼���  
  ratioVariance3f[0] = tarVariance3f[0] / srcVariance3f[0];  
  ratioVariance3f[1] = tarVariance3f[1] / srcVariance3f[1];  
  ratioVariance3f[2] = tarVariance3f[2] / srcVariance3f[2];  
  
  // ������ɫת��ֵ  
  int row = srcLab->h;  
  int col = srcLab->w;  
  for (i = 0; i < row; i++) {
    uchar *pImg = img_row(uchar, srcLab, i);
    for (j = 0; j < col; ++j, pImg+=srcLab->c) {
      double t0 = ratioVariance3f[0] * (pImg[0] - srcMean3f[0]) + tarMean3f[0];
      double t1 = ratioVariance3f[1] * (pImg[1] - srcMean3f[1]) + tarMean3f[1];
      double t2 = ratioVariance3f[2] * (pImg[2] - srcMean3f[2]) + tarMean3f[2];

      pImg[0] = (uchar)min_uchar(255, max_uchar(0, t0));
      pImg[1] = (uchar)min_uchar(255, max_uchar(0, t1));
      pImg[2] = (uchar)min_uchar(255, max_uchar(0, t2));
    }
  }  
  
  // Lab�ռ�תBGR�ռ�  
  imcolorcvt(srcLab, dst, T_Lab, T_BGR);
  imfrees(im, 10);
  return 0;
}


#endif // _COLORTRANSFER_INL_
