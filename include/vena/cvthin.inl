
#if 0

opencvʵ�ֶ�ֵͼ��ϸ�����㷨



ϸ���㷨ͨ���͹��������Ǽܻ��㷨����ͬ����˼��Ҳ����thin�㷨����skeleton�㷨����Ȼ�ܶ�ͼ����Ľ̲��ϲ�����ôд�ģ�����ԭ����Կ���ƪ���ģ�Louisa Lam, Seong-Whan Lee, Ching Y. Suen����Thinning Methodologies-A Comprehensive Survey ����IEEE TRANSACTIONS ON PATTERN ANALYSIS AND MACHINE INTELLIGENCE, VOL. 14, NO. 9, SEPTEMBER 1992 ���ܽ��˼�������92����ǰ�ľ���ϸ���㷨��

������void cvThin( img_t* src, img_t* dst, int iterations=1)
���ܣ���IPL_DEPTH_8U�Ͷ�ֵͼ�����ϸ��
������src��ԭʼIPL_DEPTH_8U�Ͷ�ֵͼ��
dst��Ŀ��洢�ռ䣬�������ȷ���ã��Һ�ԭͼ���С����һ��
iterations����������
�ο����ף�T. Y. Zhang and C. Y. Suen, ��A fast parallel algorithm for thinning digital patterns,�� Comm. ACM, vol. 27, no. 3, pp. 236-239, 1984.

#endif
#undef CV_IMAGE_ELEM
#define CV_IMAGE_ELEM(im, T, y, x)  im->tt.data[(y)*im->s + (x)]

int cvThin( const img_t* src, img_t* dst, int iterations )
{
  CvSize size = imsize(src);
  
  int n = 0,i = 0,j = 0, p2, p3, p4, p5, p6, p7, p8, p9;
  img_t t_image[1] = {0};
  imclone2(src, dst);
  for(n=0; n<iterations; n++)
  {
    imclone2(dst, t_image);
    for(i=0; i<size.h;  i++)
    {
      for(j=0; j<size.w; j++)
      {
        if(CV_IMAGE_ELEM(t_image,byte,i,j))
        {
          int ap=0;
          p2 = (i==0)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j);
          p3 = (i==0 || j==size.w-1)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j+1);
          if (p2==0 && p3==1)
          {
            ap++;
          }
          p4 = (j==size.w-1)?0:CV_IMAGE_ELEM(t_image,byte,i,j+1);
          if(p3==0 && p4==1)
          {
            ap++;
          }
          p5 = (i==size.h-1 || j==size.w-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j+1);
          if(p4==0 && p5==1)
          {
            ap++;
          }
          p6 = (i==size.h-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j);
          if(p5==0 && p6==1)
          {
            ap++;
          }
          p7 = (i==size.h-1 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j-1);
          if(p6==0 && p7==1)
          {
            ap++;
          }
          p8 = (j==0)?0:CV_IMAGE_ELEM(t_image,byte,i,j-1);
          if(p7==0 && p8==1)
          {
            ap++;
          }
          p9 = (i==0 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i-1,j-1);
          if(p8==0 && p9==1)
          {
            ap++;
          }
          if(p9==0 && p2==1)
          {
            ap++;
          }
          if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7)
          {
            if(ap==1)
            {
              if(!(p2 && p4 && p6))
              {
                if(!(p4 && p6 && p8))
                {
                  CV_IMAGE_ELEM(dst,byte,i,j)=0;
                }
              }
            }
          }
          
        }
      }
    }
    imfree(t_image);
    imclone2(dst, t_image);
    for(i=0; i<size.h;  i++)
    {
      for(j=0; j<size.w; j++)
      {
        if(CV_IMAGE_ELEM(t_image,byte,i,j)==1)
        {
          int ap=0;
          p2 = (i==0)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j);
          p3 = (i==0 || j==size.w-1)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j+1);
          if (p2==0 && p3==1)
          {
            ap++;
          }
          p4 = (j==size.w-1)?0:CV_IMAGE_ELEM(t_image,byte,i,j+1);
          if(p3==0 && p4==1)
          {
            ap++;
          }
          p5 = (i==size.h-1 || j==size.w-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j+1);
          if(p4==0 && p5==1)
          {
            ap++;
          }
          p6 = (i==size.h-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j);
          if(p5==0 && p6==1)
          {
            ap++;
          }
          p7 = (i==size.h-1 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j-1);
          if(p6==0 && p7==1)
          {
            ap++;
          }
          p8 = (j==0)?0:CV_IMAGE_ELEM(t_image,byte,i,j-1);
          if(p7==0 && p8==1)
          {
            ap++;
          }
          p9 = (i==0 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i-1,j-1);
          if(p8==0 && p9==1)
          {
            ap++;
          }
          if(p9==0 && p2==1)
          {
            ap++;
          }
          if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7)
          {
            if(ap==1)
            {
              if(p2*p4*p8==0)
              {
                if(p2*p6*p8==0)
                {
                  CV_IMAGE_ELEM(dst, byte,i,j)=0;
                }
              }
            }
          }                   
        }
        
      }
      
    }           
    //imfree(t_image);
 }
 imfree(t_image);
 return 0;
}



//ʹ�þ���

#if 0
int test_cvthin()
{
  if(argc!=2)
  {
    return 0;
  }
  img_t *pSrc = NULL,*pDst = NULL,*pTmp = NULL;
  
  //����һ���Ҷ�ͼ��
  pSrc = cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  if(!pSrc)
  {
    return 0;
  }
  pTmp = cvCloneImage(pSrc);
  pDst = cvCreateImage(cvGetSize(pSrc),pSrc->depth,pSrc->nChannels);
  cvZero(pDst);
  cvThreshold(pSrc,pTmp,128,1,CV_THRESH_BINARY_INV);//����ֵ������ͼ��ת����0��1��ʽ
  //cvSaveImage("c://Threshold.bmp",pTmp,0);
  cvThin(pTmp,pDst,8);//ϸ����ͨ���޸�iterations������һ��ϸ��
  cvNamedWindow("src",1);
  cvNamedWindow("dst",1);
  cvShowImage("src",pSrc);
  //����ֵͼ��ת���ɻҶȣ��Ա���ʾ
  int i = 0,j = 0;
  CvSize size = cvGetSize(pDst);
  for(i=0; i<size.h;  i++)
  {
    for(j=0; j<size.w; j++)
    {
      if(CV_IMAGE_ELEM(pDst,uchar,i,j)==1)
      {
        CV_IMAGE_ELEM(pDst,uchar,i,j) = 0;
      }
      else
      {
        CV_IMAGE_ELEM(pDst,uchar,i,j) = 255;
      }
    }
  }
  //cvSaveImage("c://thin.bmp",pDst);
  cvShowImage("dst",pDst);
  cvWaitKey(0);
  cvReleaseImage(&pSrc);
  cvReleaseImage(&pDst);
  cvReleaseImage(&pTmp);
  cvDestroyWindow("src");
  cvDestroyWindow("dst");
  return 0;
} 

#endif
