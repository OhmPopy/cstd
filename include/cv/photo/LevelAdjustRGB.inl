nShadow=159  nHighLight=238  dMidtone=0.51

#define  CLIP8(a)  (((a) & 0xFFFFFF00) ? (((a) < 0) ? 0 : 255 ) : (a))      ///<�ж�8λ���ݷ�Χ  
void LevelAdjustRGB(unsigned char *pSrc, unsigned char *pDest, int nWidth, int nHeight,int nShadow,int nHighLight,double dMidtone)  
{  
  //�ֲ���������  
  int i = 0;   
  int nLength  = nWidth * nHeight;  
  double dDiff = nHighLight-nShadow;  
  int *nRgbDiff = new int[3 * nLength];  
  
  for(i = 0;i < 3 * nLength; i++)  
  {  
    nRgbDiff[i] = pSrc[i] - nShadow;  
    if(nRgbDiff[i] < 0)  
    {  
      nRgbDiff[i] = 0;  
    }  
    
    //�˴�������int��Ϊ�м���� ������char  �������ݽض�  
    int nTemp = static_cast<int>(pow((nRgbDiff[i] / dDiff),1 / dMidtone) * 255);  
    pDest[i] = CLIP8(nTemp);  
  }  
  //�ͷſռ�  
  if(!nRgbDiff)  
  {  
    delete []nRgbDiff;  
    nRgbDiff = NULL;  
  }  
    }
