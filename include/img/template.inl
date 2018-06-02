// ************************************************************************
//
// ͼ���������API�����⣺
//
// Threshold() - ͼ����ֵ�ָ�����
// AddMinus()  - ͼ��Ӽ�����
// Hproject() - ͼ��ˮƽͶӰ
// Vproject() - ͼ��ֱͶӰ
// Template() - ͼ��ģ��ƥ������
//
// ************************************************************************

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

//////////////////////////////////////////////////////////////////////
// ͼ��ģ��任����
//////////////////////////////////////////////////////////////////////

/*************************************************************************
* �������ƣ�
*   Template()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth          - Դͼ���ȣ���������
*   int lHeight         - Դͼ��߶ȣ���������
*   int  iTempH         - ģ��ĸ߶�
*   int  iTempW         - ģ��Ŀ��
*   int  iTempMX        - ģ�������Ԫ��X���� ( < iTempW - 1)
*   int  iTempMY        - ģ�������Ԫ��Y���� ( < iTempH - 1)
*   Float* fpArray      - ָ��ģ�������ָ��
*   Float fCoef         - ģ��ϵ��
* ����ֵ:
*   BOOL                - �ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú�����ָ����ģ�壨�����С������ͼ����в���������iTempHָ��ģ��
* �ĸ߶ȣ�����iTempWָ��ģ��Ŀ�ȣ�����iTempMX��iTempMYָ��ģ�������
* Ԫ�����꣬����fpArrayָ��ģ��Ԫ�أ�fCoefָ��ϵ����
 ************************************************************************/
BOOL Template(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int iTempH, int iTempW, int iTempMX, int iTempMY,
    Float* fpArray, Float fCoef)
{
  LPBYTE lpNewDIBBits;  // ָ����ͼ���ָ��
  LPBYTE lpSrc;    // ָ��Դͼ���ָ��
  LPBYTE lpDst;    // ָ��Ҫ���������ָ��
  int i, j, k, l;  // ѭ������
  Float fResult;   // ������

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lLineBytes * lHeight);

  // ��ʼ��ͼ��Ϊԭʼͼ��
  memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

  for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++) {   // ��(��ȥ��Ե����)
    for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++) {   // ��(��ȥ��Ե����)
      // ָ����DIB��i�У���j�����ص�ָ��
      lpDst = (LPBYTE) lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      fResult = 0;

      for (k = 0; k < iTempH; k++) {
        for (l = 0; l < iTempW; l++) {
          // ָ��DIB��i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
          lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1
              - i + iTempMY - k) + j - iTempMX + l;

          // ��������ֵ
          fResult += (* lpSrc) * fpArray[ k * iTempW + l ];
        }
      }

      fResult *= fCoef;     // ����ϵ��
      fResult = (Float) fabs(fResult);     // ȡ����ֵ

      if (fResult > 255) {
        * lpDst = 255;
      }
      else {
        * lpDst = (BYTE)(fResult + 0.5);
      }
    }
  }

  // ���Ʊ任���ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

  SAFEFREE(lpNewDIBBits);
  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   GetMedianNum()
* ����:
*   LPBYTE bpArray - ָ��Ҫ��ȡ��ֵ������ָ��
*   int  iFilterLen   - ���鳤��
* ����ֵ:
*   BYTE      - ����ָ���������ֵ��
* ˵��:
*   �ú�����ð�ݷ���һά����������򣬲���������Ԫ�ص���ֵ��
 ************************************************************************/
BYTE GetMedianNum(LPBYTE bArray, int iFilterLen)
{
  int i, j;   // ѭ������
  BYTE bTemp;

  // ��ð�ݷ��������������
  for (j = 0; j < iFilterLen - 1; j ++) {
    for (i = 0; i < iFilterLen - j - 1; i ++) {
      if (bArray[ i ] > bArray[ i + 1 ]) {
        // ����
        bTemp = bArray[ i ];
        bArray[ i ] = bArray[ i + 1 ];
        bArray[ i + 1 ] = bTemp;
      }
    }
  }

  // ������ֵ
  if ((iFilterLen & 1) > 0) {
    // ������������Ԫ�أ������м�һ��Ԫ��
    bTemp = bArray[(iFilterLen + 1) / 2 ];
  }
  else {
    // ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
    bTemp = (bArray[ iFilterLen / 2 ] + bArray[ iFilterLen / 2 + 1 ]) / 2;
  }

  return bTemp;
}

/*************************************************************************
* �������ƣ�
*   MedianFilter()
* ����:
*   LPBYTE lpDIBBits  - ָ��ԴDIBͼ��ָ��
*   int lWidth   - Դͼ���ȣ���������
*   int lHeight   - Դͼ��߶ȣ���������
*   int  iFilterH   - �˲����ĸ߶�
*   int  iFilterW   - �˲����Ŀ��
*   int  iFilterMX  - �˲���������Ԫ��X����
*   int  iFilterMY  - �˲���������Ԫ��Y����
* ����ֵ:
*   BOOL     - �ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú�����DIBͼ�������ֵ�˲���
 ************************************************************************/
BOOL MedianFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int iFilterH, int iFilterW, int iFilterMX, int iFilterMY)
{
  LPBYTE lpSrc;   // ָ��Դͼ���ָ��
  LPBYTE lpDst;   // ָ��Ҫ���������ָ��
  LPBYTE lpNewDIBBits; // ָ����ͼ���ָ��
  LPBYTE aValue;  // ָ���˲��������ָ��
  int i, j, k, l;  // ѭ������

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lLineBytes * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // ��ʼ��ͼ��Ϊԭʼͼ��
  memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

  // ��ʱ�����ڴ棬�Ա����˲�������
  aValue = MALLOC(BYTE, iFilterH * iFilterW);

  if (aValue == NULL) {
    SAFEFREE(lpNewDIBBits);
    return FALSE;
  }

  // ��ʼ��ֵ�˲�
  // ��(��ȥ��Ե����)
  for (i = iFilterMY; i < lHeight - iFilterH + iFilterMY + 1; i++) {
    // ��(��ȥ��Ե����)
    for (j = iFilterMX; j < lWidth - iFilterW + iFilterMX + 1; j++) {
      // ָ����DIB��i�У���j�����ص�ָ��
      lpDst = (LPBYTE) lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      // ��ȡ�˲�������
      for (k = 0; k < iFilterH; k++) {
        for (l = 0; l < iFilterW; l++) {
          // ָ��DIB��i - iFilterMY + k�У���j - iFilterMX + l�����ص�ָ��
          lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1
              - i + iFilterMY - k) + j - iFilterMX + l;

          // ��������ֵ
          aValue[ k * iFilterW + l ] = *lpSrc;
        }
      }

      // ��ȡ��ֵ
      * lpDst = GetMedianNum(aValue, iFilterH * iFilterW);
    }
  }

  // ���Ʊ任���ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

  SAFEFREE(lpNewDIBBits);
  SAFEFREE(aValue);
  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   MedianValue()
* ����:
*   LPBYTE FiltValue    - ָ��Ҫ��ȡ��ֵ������ָ��
*   int  iFilterLen             - ���鳤��
* ����ֵ:
*   BYTE                - ����ָ���������ֵ��
* ˵��:
*   �ú�����ð�ݷ���һά����������򣬲���������Ԫ�ص���ֵ��
************************************************************************/
BYTE MedianValue(LPBYTE FiltValue, int mFilterLen)
{
  int i;                                        // ѭ������
  int j;                                        // ѭ������
  BYTE Temp;                                    // �м����

  // ��ð�ݷ��������������
  for (j = 0; j < mFilterLen - 1; j ++) {
    for (i = 0; i < mFilterLen - j - 1; i ++) {
      if (FiltValue[ i ] > FiltValue[ i + 1 ]) {                     // ǰ���ֵ���ں����
        Temp = FiltValue[ i ];
        FiltValue[ i ] = FiltValue[ i + 1 ];                // ���ߵ�ֵ����
        FiltValue[ i + 1 ] = Temp;
      }
    }
  }

  if ((mFilterLen & 1) > 0) {                                   // ������ֵ
    //������������Ԫ�أ������м�һ��Ԫ��
    Temp = FiltValue[(mFilterLen + 1) / 2 ];
  }
  else {
    //������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
    Temp = (FiltValue[ mFilterLen / 2 ] + FiltValue[ mFilterLen / 2 + 1 ]) / 2;
  }

  return Temp;                                        // ������ֵ
}


/*************************************************************************
 *
* �������ƣ�
*   Project()
 *
* ����:
*   LPBYTE lpDIBBits        - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
*     int  direction        - ͶӰ����
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�����ͼ�����ˮƽ����ֱͶӰ���㡣
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ�񣬷���ǿ��ת����
 ************************************************************************/

BOOL Project(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int direction)
{
  // ��ʱ���ͼ�����ݵ�ָ��
  LPBYTE lpImage;
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;
  // ָ��Ҫ���������ָ��
  LPBYTE lpDst;
  //ѭ������
  int i;
  int j;
  int k;
  //ͼ����ÿ���ڵĺڵ����
  int BlackNumber;
  //����ֵ
  BYTE pixeltemp;
  // �����ڴ�
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // �ж��Ƿ��ڴ����ʧ��
  if (lpImage == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

  // ��Ŀ��ͼ���������س�ʼ��Ϊ�׵�
  lpDst = (LPBYTE) lpImage;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  //���ͼ���Ƕ�ֵͼ��ת��Ϊ��ֵͼ��
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixeltemp = (BYTE) * lpSrc;

      //Ŀ��ͼ���лҶ�ֵ����120������Ϊ255����������Ϊ0
      if (pixeltemp >= 120) {
        *lpSrc = (BYTE) 255;
      }
      else {
        *lpSrc = (BYTE) 0;
      }
    }
  }

  if (direction == 0) {   //ˮƽ����ͶӰ
    for (j = 0; j < lHeight; j++) {
      BlackNumber = 0;    //�ڵ���Ŀ��0

      for (i = 0; i < lWidth; i++) {
        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixeltemp = (BYTE) * lpSrc;

        if (pixeltemp == 0) { //������ֺڵ�
          BlackNumber++; //�ڵ���Ŀ��1
        }
      }

      for (k = 0; k < BlackNumber; k++) {
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = (LPBYTE) lpImage + lWidth * j + k;
        *lpDst = (BYTE) 0;   //��Ϊ�ڵ�
      }
    }
  }
  else if (direction == 1) {   //��ֱ����ͶӰ
    for (i = 0; i < lWidth; i++) {
      BlackNumber = 0;    //�ڵ���Ŀ��0

      for (j = 0; j < lHeight; j++) {
        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixeltemp = (BYTE) * lpSrc;

        if (pixeltemp == 0) { //������ֺڵ�
          BlackNumber++; //�ڵ���Ŀ��1
        }
      }

      for (k = 0; k < BlackNumber; k++) {
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = (LPBYTE) lpImage + lWidth * k + i;
        *lpDst = (BYTE) 0;   //��Ϊ�ڵ�
      }
    }
  }

  // ���������������ͼ��
  memcpy(lpDIBBits, lpImage, lWidth * lHeight);
  // �ͷ��ڴ�
  SAFEFREE(lpImage);
  // ����
  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Subtract()
* ����:
*   LPBYTE lpDIBBits1   - ָ��ԴDIBͼ��ָ��
*   LPBYTE lpDIBBits2   - ָ�򱳾�DIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
 *
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�����ͼ����в�Ӱ���㡣
* Ҫ��Ŀ��ͼ��Ϊ255���Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL Subtract(LPBYTE lpDIBBits1, LPBYTE lpDIBBits2, int lWidth, int lHeight)
{
  //ѭ������
  int i, j;
  // ��ʱ���ͼ�����ݵ�ָ��
  LPBYTE lpImage;
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc1;
  // ָ�򱳾�ͼ���ָ��
  LPBYTE lpSrc2;
  // ָ��Ҫ���������ָ��
  LPBYTE lpDst;
  //����ֵ
  BYTE pixeltemp1;
  BYTE pixeltemp2;
  // �����ڴ�
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // �ж��Ƿ��ڴ����ʧ��
  if (lpImage == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

  // ��Ŀ��ͼ���������س�ʼ��Ϊ�׵�
  lpDst = (LPBYTE) lpImage;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc1 = (LPBYTE) lpDIBBits1 + lWidth * j + i;
      // ָ�򱳾�ͼ������j�У���i�����ص�ָ��
      lpSrc2 = (LPBYTE) lpDIBBits2 + lWidth * j + i;
      // ָ�򻺳�ͼ������j�У���i�����ص�ָ��
      lpDst = (LPBYTE) lpImage + lWidth * j + i;
      //ȡ��Դͼ��ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixeltemp1 = (BYTE) * lpSrc1;
      //ȡ�ñ���ͼ��ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixeltemp2 = (BYTE) * lpSrc2;

      //���в�Ӱ����
      if (pixeltemp1 - pixeltemp2 < 0) {
        *lpDst = 0;
      }
      else {
        *lpDst = pixeltemp1 - pixeltemp2;
      }
    }
  }

  // ���������������ͼ��
  memcpy(lpDIBBits1, lpImage, lWidth * lHeight);
  // �ͷ��ڴ�
  SAFEFREE(lpImage);
  // ����
  return TRUE;
}


/*************************************************************************
* �������ƣ�
*   TemplateMatch()
* ����:
*   LPBYTE lpDIBBits1    - ָ��ԴDIBͼ��ָ��
*   LPBYTE lpDIBBits2    - ָ��ģ��DIBͼ��ָ��
*   int lWidth    - Դͼ���ȣ���������������4�ı�����
*   int lHeight   - Դͼ��߶ȣ���������
*   int TemplateWidth       - ģ��ͼ���ȣ���������������4�ı�����
*   int TemplateHeight      - ģ��ͼ��߶ȣ���������
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�ͼ�����ģ��ƥ�����㡣
* Ҫ��Ŀ��ͼ��Ϊ255���Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL TemplateMatch(int lHeight, int lWidth, LPBYTE lpDIBBits1,
    int TemplateHeight, int TemplateWidth, LPBYTE lpDIBBits2)
{
  // ��ʱ���ͼ�����ݵ�ָ��
  LPBYTE lpImage;
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc1;
  // ָ��ģ��ͼ���ָ��
  LPBYTE lpSrc2;
  // ָ��Ҫ���������ָ��
  LPBYTE lpDst;
  //����ֵ
  BYTE pixeltemp1; //Դͼ������
  BYTE pixeltemp2; //ģ��ͼ������
  //ѭ������
  int i, j, m, n;
  int cofT; //ģ�����������ֵ
  int cofS; //Դ���������ֵ
  int cofTS; //ģ�塢Դͼ�����ػҶȻ����ֵ
  Float R; //���R
  Float Rmax; //�����
  IPOINT matchpoint; //ƥ���
  // �����ڴ�
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // �ж��Ƿ��ڴ����ʧ��
  if (lpImage == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

  // ��Ŀ��ͼ���������س�ʼ��Ϊ�Ұ׵�
  lpDst = (LPBYTE) lpImage;
  memset(lpDst, (BYTE) 205, lWidth * lHeight);
  //����cofT
  cofT = 0;

  for (j = 0; j < TemplateHeight; j++) {
    for (i = 0; i < TemplateWidth; i++) {
      // ָ��ģ��ͼ������j�У���i�����ص�ָ��
      lpSrc2 = (LPBYTE) lpDIBBits2 + lWidth * j + i;
      //ȡ��ģ��ͼ��ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixeltemp2 = (BYTE) * lpSrc2;
      //�������������
      cofT = cofT + pixeltemp2 * pixeltemp2;
    }
  }

  //�ҵ�ͼ������������Եĳ���λ��
  Rmax = 0.0;

  for (j = 0; j < lHeight - TemplateHeight + 1; j++) {
    for (i = 0; i < lWidth - TemplateWidth + 1; i++) {
      cofS = 0; //��ʼ��Ϊ0
      cofTS = 0; //��ʼ��Ϊ0

      for (n = 0; n < TemplateHeight; n++) {
        for (m = 0; m < TemplateWidth; m++) {
          // ָ��Դͼ������j+n�У���i+m�����ص�ָ��
          lpSrc1 = (LPBYTE) lpDIBBits1 + lWidth * (j + n) + (i + m);
          // ָ��ģ��ͼ������n�У���m�����ص�ָ��
          lpSrc2 = (LPBYTE) lpDIBBits2 + TemplateWidth * n + m;
          //��ȡ
          pixeltemp1 = (BYTE) * lpSrc1;
          pixeltemp2 = (BYTE) * lpSrc2;

          cofS = (int)(cofS + (Float) pixeltemp1 * pixeltemp1);
          cofTS = (int)(cofTS + (Float) pixeltemp1 * pixeltemp2);
        }
      }

      //����������
      R = cofTS / (sqrt(cofS) * sqrt(cofT));

      //����������ԱȽ�
      if (R > Rmax) {
        Rmax = R;
        matchpoint.x = i;
        matchpoint.y = j;
      }
    }
  }


  //��Դͼ������ģ��ͼ��ƥ������������Ƶ�Ŀ��ͼ��
  for (n = 0; n < TemplateHeight; n++) {
    for (m = 0; m < TemplateWidth; m++) {
      lpSrc1 = (LPBYTE) lpDIBBits1 + lWidth * (n + matchpoint.y) + (m + matchpoint.x);
      lpDst = (LPBYTE) lpImage + lWidth * (n + matchpoint.y) + (m + matchpoint.x);
      *lpDst = *lpSrc1;
    }
  }

  // ����ͼ��
  memcpy(lpDIBBits1, lpImage, lWidth * lHeight);
  // �ͷ��ڴ�
  SAFEFREE(lpImage);
  // ����
  return TRUE;
}

/*************************************************************************
 *
* \�������ƣ�
*   TemplateMatch()
 *
* \�������:
*   CDib* pDibSrc  - ָ��CDib���ָ�룬���д�ƥ��ͼ����Ϣ
*   CDib* pDibTemplate - ָ��CDib���ָ�룬����ģ��ͼ����Ϣ
*   int nImageHeight   - ͼ��ĸ߶�
*   int nImageWidth    - ͼ��Ŀ��
*   int nTemplateHeight   - ģ��ͼ��ĸ߶�
*   int nTemplateWidth    - ģ��ͼ��Ŀ��
*   LPBYTE lpSrc    - ָ��Դͼ���ָ��
*   LPBYTE lpDst    - ָ��Դͼ���ָ��
 *
* \����ֵ:
*   BOOL   - �ɹ��򷵻�TRUE�����򷵻�FALSE
 *
* \˵��:
*   �ú�������ͼ�����ģ��ƥ���������Ҫע����ǣ��˳���ֻ����256�Ҷȼ���
 *ͼ��
 *
 *************************************************************************
 */
BOOL TemplateMatch1(int nImageHeight, int nImageWidth, LPBYTE lpSrc1, int sizeSaveImage_cx,
    int nTemplateHeight, int nTemplateWidth, LPBYTE lpTemplateSrc1, int sizeSaveTemplate_cx)
{
  // ָ�򻺴�ͼ���ָ��
  LPBYTE lpSrc, lpDst, lpDst1, lpTemplateSrc;

  //ѭ������
  int i, j, m, n;

  //�м���
  Float dSigmaST, dSigmaS, dSigmaT;

  //�����Բ��
  Float R;

  //��������Բ��
  Float dbMaxR;

  //��������Գ���λ��
  int nMaxWidth;
  int nMaxHeight;

  //����ֵ
  BYTE unchPixel;
  BYTE unchTemplatePixel;

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpDst1 = MALLOC(BYTE, nImageHeight * sizeSaveImage_cx);

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  memset(lpDst1, (BYTE) 255, nImageHeight * sizeSaveImage_cx);

  //����dSigmaT
  dSigmaT = 0;

  for (n = 0; n < nTemplateHeight; n++) {
    for (m = 0; m < nTemplateWidth; m++) {
      // ָ��ģ��ͼ������j�У���i�����ص�ָ��
      lpTemplateSrc = (LPBYTE) lpTemplateSrc1 + sizeSaveTemplate_cx * n + m;
      unchTemplatePixel = (BYTE) * lpTemplateSrc;
      dSigmaT += (Float) unchTemplatePixel * unchTemplatePixel;
    }
  }

  //�ҵ�ͼ������������Եĳ���λ��
  dbMaxR = 0.0;

  for (j = 0; j < nImageHeight - nTemplateHeight + 1; j++) {
    for (i = 0; i < nImageWidth - nTemplateWidth + 1; i++) {
      dSigmaST = 0;
      dSigmaS = 0;

      for (n = 0; n < nTemplateHeight; n++) {
        for (m = 0; m < nTemplateWidth; m++) {
          // ָ��Դͼ������j+n�У���i+m�����ص�ָ��
          lpSrc = (LPBYTE) lpSrc1 + sizeSaveImage_cx * (j + n) + (i + m);

          // ָ��ģ��ͼ������n�У���m�����ص�ָ��
          lpTemplateSrc = (LPBYTE) lpTemplateSrc1 + sizeSaveTemplate_cx * n + m;

          unchPixel = (BYTE) * lpSrc;
          unchTemplatePixel = (BYTE) * lpTemplateSrc;

          dSigmaS += (Float) unchPixel * unchPixel;
          dSigmaST += (Float) unchPixel * unchTemplatePixel;
        }
      }

      //����������
      R = dSigmaST / (sqrt(dSigmaS) * sqrt(dSigmaT));

      //����������ԱȽ�
      if (R > dbMaxR) {
        dbMaxR = R;
        nMaxWidth = i;
        nMaxHeight = j;
      }
    }
  }

  //����������Գ������򲿷ָ��Ƶ�Ŀ��ͼ��
  for (n = 0; n < nTemplateHeight; n++) {
    for (m = 0; m < nTemplateWidth; m++) {
      lpTemplateSrc = (LPBYTE) lpTemplateSrc1 + sizeSaveTemplate_cx * n + m;
      lpDst = (LPBYTE) lpDst1 + sizeSaveImage_cx * (n + nMaxHeight) + (m + nMaxWidth);
      *lpDst = *lpTemplateSrc;
    }
  }

  // ����ͼ��
  memcpy(lpSrc1, lpDst1, sizeSaveImage_cx * nImageHeight);

  // �ͷ��ڴ�
  SAFEFREE(lpDst1);

  // ����
  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   AddMinus()
 *
* ����:
*   LPBYTE lpDIBBitsBK  - ָ�򱳾�DIBͼ��ָ��
*   LPBYTE lpDIBBits   - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
*   int lLineBytes   - ͼ��ÿ�е��ֽ���
*  BOOL  bAddMinus - ΪTRUEʱִ�м����㣬����ִ�м����㡣
 *
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
* �ú������ڶ�����ͼ����мӼ����㡣
*
* Ҫ��Ŀ��ͼ��Ϊ255���Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL AddMinus(LPBYTE lpDIBBits, LPBYTE lpDIBBitsBK, int lWidth, int lHeight, int lLineBytes, BOOL bAddMinus)
{

  // ָ��Դͼ���ָ��
  LPBYTE lpSrc, lpSrcBK;

  // ָ�򻺴�ͼ���ָ��
  LPBYTE lpDst;

  // ָ�򻺴�DIBͼ���ָ��
  LPBYTE lpNewDIBBits;

  //ѭ������
  int i;
  int j;

  //����ֵ
  BYTE pixel, pixelBK;

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = (LPBYTE) lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;
      lpSrcBK = (LPBYTE) lpDIBBitsBK + lLineBytes * j + i;

      // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
      lpDst = (LPBYTE) lpNewDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;
      pixelBK = (BYTE) * lpSrcBK;

      if (bAddMinus) {
        * lpDst = pixel + pixelBK > 255 ? 255 : pixel + pixelBK;
      }
      else {
        *lpDst = pixel - pixelBK < 0 ? 0 : pixel - pixelBK;
      }


    }
  }


  // ���Ƹ�ʴ���ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  // �ͷ��ڴ�
  SAFEFREE(lpNewDIBBits);

  // ����
  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   Hproject()
 *
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
 *
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
* �ú������ڶ�����ͼ�����ˮƽͶӰ���㡣
*
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/

BOOL Hproject(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  // ָ�򻺴�ͼ���ָ��
  LPBYTE lpDst;

  // ָ�򻺴�DIBͼ���ָ��
  LPBYTE lpNewDIBBits;

  //ѭ������
  int i;
  int j;

  //ͼ����ÿ���ڵĺڵ����
  int lBlackNumber;

  //����ֵ
  BYTE pixel;

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = (LPBYTE) lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  for (j = 0; j < lHeight; j++) {
    lBlackNumber = 0;

    for (i = 0; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }

      if (pixel == 0) {
        lBlackNumber++;
      }
    }

    for (i = 0; i < lBlackNumber; i++) {
      // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
      lpDst = (LPBYTE) lpNewDIBBits + lLineBytes * j + i;

      *lpDst = (BYTE) 0;
    }
  }


  // ����ͶӰͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  // �ͷ��ڴ�
  SAFEFREE(lpNewDIBBits);

  // ����
  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   Vproject()
 *
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
 *
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
* �ú������ڶ�����ͼ����д�ֱͶӰ���㡣
*
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL Vproject(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  // ָ�򻺴�ͼ���ָ��
  LPBYTE lpDst;

  // ָ�򻺴�DIBͼ���ָ��
  LPBYTE lpNewDIBBits;

  // ѭ������
  int i, j;

  // ͼ����ÿ���ڵĺڵ����
  int lBlackNumber;

  // ����ֵ
  BYTE pixel;

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = (LPBYTE) lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  for (i = 0; i < lWidth; i++) {
    lBlackNumber = 0;

    for (j = 0; j < lHeight; j++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }

      if (pixel == 0) {
        lBlackNumber++;
      }
    }

    for (j = 0; j < lBlackNumber; j++) {
      // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
      lpDst = (LPBYTE) lpNewDIBBits + lLineBytes * j + i;

      *lpDst = (BYTE) 0;
    }
  }


  // ����ͶӰͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  // �ͷ��ڴ�
  SAFEFREE(lpNewDIBBits);

  // ����
  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   TemplateMatch()
 *
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   LPBYTE lpDIBBitsBK  - ָ�򱳾�DIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
*   int lTemplateWidth       - ģ��ͼ���ȣ���������
*   int lTemplateHeight      - ģ��ͼ��߶ȣ���������
 *
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
* �ú������ڶ�ͼ�����ģ��ƥ�����㡣
*
* Ҫ��Ŀ��ͼ��Ϊ255���Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL TemplateMatchDIB(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int lTemplateHeight, int lTemplateWidth,
    LPBYTE lpTemplateDIBBits, int lTemplateLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc, lpTemplateSrc;

  // ָ�򻺴�ͼ���ָ��
  LPBYTE lpDst;

  // ָ�򻺴�DIBͼ���ָ��
  LPBYTE lpNewDIBBits;

  //ѭ������
  int i, j, m, n;

  //�м���
  Float dSigmaST, dSigmaS, dSigmaT;

  //�����Բ��
  Float R;

  //��������Բ��
  Float MaxR;

  //��������Գ���λ��
  int lMaxWidth;
  int lMaxHeight;

  //����ֵ
  BYTE pixel;
  BYTE templatepixel;

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = (LPBYTE) lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  //����dSigmaT
  dSigmaT = 0;

  for (n = 0; n < lTemplateHeight; n++) {
    for (m = 0; m < lTemplateWidth; m++) {
      // ָ��ģ��ͼ������j�У���i�����ص�ָ��
      lpTemplateSrc = (LPBYTE) lpTemplateDIBBits + lTemplateLineBytes * n + m;
      templatepixel = (BYTE) * lpTemplateSrc;
      dSigmaT += (Float) templatepixel * templatepixel;
    }
  }

  //�ҵ�ͼ������������Եĳ���λ��
  MaxR = 0.0;

  for (j = 0; j < lHeight - lTemplateHeight + 1; j++) {
    for (i = 0; i < lWidth - lTemplateWidth + 1; i++) {
      dSigmaST = 0;
      dSigmaS = 0;

      for (n = 0; n < lTemplateHeight; n++) {
        for (m = 0; m < lTemplateWidth; m++) {
          // ָ��Դͼ������j+n�У���i+m�����ص�ָ��
          lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (j + n) + (i + m);

          // ָ��ģ��ͼ������n�У���m�����ص�ָ��
          lpTemplateSrc = (LPBYTE) lpTemplateDIBBits + lTemplateLineBytes * n + m;

          pixel = (BYTE) * lpSrc;
          templatepixel = (BYTE) * lpTemplateSrc;

          dSigmaS += (Float) pixel * pixel;
          dSigmaST += (Float) pixel * templatepixel;
        }
      }

      //����������
      R = dSigmaST / (sqrt(dSigmaS) * sqrt(dSigmaT));

      //����������ԱȽ�
      if (R > MaxR) {
        MaxR = R;
        lMaxWidth = i;
        lMaxHeight = j;
      }
    }
  }

  //����������Գ������򲿷ָ��Ƶ�Ŀ��ͼ��
  for (n = 0; n < lTemplateHeight; n++) {
    for (m = 0; m < lTemplateWidth; m++) {
      lpTemplateSrc = (LPBYTE) lpTemplateDIBBits + lTemplateLineBytes * n + m;
      lpDst = (LPBYTE) lpNewDIBBits + lLineBytes * (n + lMaxHeight) + (m + lMaxWidth);
      *lpDst = *lpTemplateSrc;
    }
  }

  // ����ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  // �ͷ��ڴ�
  SAFEFREE(lpNewDIBBits);

  // ����
  return TRUE;
}
