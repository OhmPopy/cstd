
#ifndef SUPPORT_IMA
#define SUPPORT_IMA


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include "_fmtimg.h"

//typedef unsigned char BYTE;
//typedef unsigned short WORD;

typedef struct {
  ihand_t m_strm;
  BYTE bBpp;
  WORD wLeft;
  WORD wTop;
  WORD wRight;
  WORD wBottom;
  WORD wXResolution;
  WORD wYResolution;
}
IMGHEADER;

typedef struct fmtima {
  PaletteEntry m_palette[ 256 ];
}
fmtima;

int fmtima_free(fmtima* reader)
{
  return 0;
}

#define fmtima_checkext(x)  0
int fmtima_checksign(const TCHAR* picname)
{
  return fmtima_checkext(picname);
}

// PNG Filter Factory
int fmtima_read_header(ihand_t m_strm, img_t* im, fmtbmp* fmt)
{
  int result = FALSE;
  IMGHEADER Header4IMG;  // IMGͷ�ļ�
  im->c = 1;

  // ����IMG�ļ�ͷ��Ϣ
  // ���Զ�ȡ�ļ�ͷ
  if (READ_NBYTE((LPBYTE) &Header4IMG, sizeof(IMGHEADER)) != sizeof(IMGHEADER)) {
    // ��С���ԣ�����NULL��
    return FALSE;
  }

  // ��ȡͼ��߶�
  im->h = Header4IMG.wBottom - Header4IMG.wTop + 1;

  // ��ȡͼ����
  im->w = Header4IMG.wRight - Header4IMG.wLeft + 1;

  im->f = 1;
  // ����ֵ
  return TRUE;
}

int fmtima_read(ihand_t m_strm, img_t* im, fmtbmp* fmt)
{
  unsigned char* data = im->tt.data;
  int step = im->s, nch = im->c;
  int result = FALSE;
  int color = nch > 1;
  int lWidth = im->w;
  int lHeight = im->h;
  int i, j, nTemp;
  PaletteEntry* lpbmc = fmt->m_palette;
  BYTE bChar; // �м����
  int* lpSrc; // ָ��Դͼ�����ص�ָ��
  LPBYTE lpDst; // ָ������ͼ�����ݵ�ָ��
  int* lpTemp; // ��ʱָ��
  // ��������Ԥ����������غ͵�ǰ��������
  BYTE bCharA, bCharB, bCharC;
  int lLineBytes = step; // ͼ��ÿ�е��ֽ���

  lpSrc = GMALLOC(int, lWidth * lHeight);
  lpTemp = lpSrc;

  // ��ȡ����������
  if ((int) READ_NBYTE(lpSrc, (lWidth * lHeight * sizeof(int)))
      != (int)(lWidth * lHeight * sizeof(int))) {
    return FALSE;
  }

  // ����λ��
  lpDst = data;

  // �����0��
  i = 0;

  for (j = 0; j < lWidth; j++) {
    if (j == 0) {
      // �����0��0�У�����ֵ������ʵֵ
      lpDst[ j + lLineBytes * (lHeight - 1 - i) ] = (BYTE)(*lpTemp);
      lpTemp ++;
    }
    else {
      // ���� D��A���в� �õ�ԭ��������
      lpDst[ j + lLineBytes * (lHeight - 1 - i) ]
        = (BYTE)(*lpTemp) + lpDst[ j + lLineBytes * (lHeight - 1 - i) - 1 ];
      lpTemp++;
    }
  }

  // �����1�е���lHeight��1��
  for (i = 1; i < lHeight; i++) {
    for (j = 0; j < lWidth; j++) {
      // �õ�����B��ֵ
      bCharB = lpDst[ j + lLineBytes * (lHeight - i) ];

      // �����һ��
      if (j == 0) {
        // ���� D��B���в� �õ�ԭ��������ֵ
        lpDst[ j + lLineBytes * (lHeight - 1 - i) ] = (BYTE)((*lpTemp) + bCharB);
        lpTemp++;
      }

      // ����ʣ�µ���
      else {
        // ���� D=(B-C)/2 + A + �в� �õ�ԭ��������ֵ
        bCharA = lpDst[ j - 1 + lLineBytes * (lHeight - 1 - i) ];
        bCharC = lpDst[ j - 1 + lLineBytes * (lHeight - i) ];

        // ����ʱ��Ԥ��
        nTemp = (int)((bCharB - bCharC) / 2 + bCharA);

        // Ԥ��ֵС��0��ֱ�Ӹ�0
        if (nTemp < 0) {
          nTemp = 0;
        }
        // Ԥ��ֵ����255��ֱ�Ӹ�ֵ255
        else if (nTemp > 255) {
          nTemp = 255;
        }
        else {
          nTemp = nTemp;
        }

        // Ԥ��ֵ���в�
        lpDst[ j + lLineBytes * (lHeight - 1 - i) ]
          = (BYTE)(*lpTemp + (BYTE) nTemp);
        lpTemp++;
      }
    }
  }

  // �ͷ��ڴ�
  GFREE(lpSrc);
  lpDst = NULL;

  // ����ɫ���־λ
  READ_NBYTE(&bChar, 1);

  if (bChar != 0x0C) {
    // ����NULL��
    return FALSE;
  }

  // �����ڴ��Զ�ȡ����������
  lpDst = GMALLOC(BYTE, 768);

  // ��ȡ��ɫ��
  if (READ_NBYTE(lpDst, 768) != 768) {
    return FALSE;
  }

  // ����ɫ�帳ֵ
  for (i = 0; i < 256; i++) {
    lpbmc[ i ].r = lpDst[ i * 3 + 2 ];
    lpbmc[ i ].g = lpDst[ i * 3 + 1 ];
    lpbmc[ i ].b = lpDst[ i * 3 ];
    lpbmc[ i ].a = 0;
  }

  GFREE(lpSrc);

  return result;
}

int fmtima_write(ihand_t m_strm, const img_t* im, const PaletteEntry* palette)
{
  int height = im->h, width = im->w, step = im->s, channels = im->c;
  const unsigned char* data = im->tt.data;
  int isBinary = FALSE;
  int result = FALSE;

  int channels0 = channels > 1 ? 3 : 1;
  int fileStep = width * channels0;

  assert(data && width > 0 && height > 0 && step >= fileStep);

  if (m_strm) {    // ָ��Դͼ���ָ��
    const unsigned char* lpSrc;
    PaletteEntry lpbmc[ 256 ] = {0};
    int i, j; // ѭ������
    // ����Ԥ������غ͵�ǰ���������
    BYTE bCharA, bCharB, bCharC, bCharD;
    int nTemp; // Ԥ��ֵ
    int nDpcm; // Ԥ���Ĳв�
    BYTE lpDst[769]; // ָ������ͼ�����ݵ�ָ��

    //д��IMG�ļ�ͷ��Ϣ
    IMGHEADER Header4IMG;
    // ���ļ�ͷ��ֵ
    Header4IMG.bBpp = 8;                // ����λ����256ɫΪ8λ��
    Header4IMG.wLeft = 0;               // ͼ���������Ļ�����Ͻ�X���꣨������Ϊ��λ��
    Header4IMG.wTop = 0;                // ͼ���������Ļ�����Ͻ�Y���꣨������Ϊ��λ��
    Header4IMG.wRight = width - 1;     // ͼ���������Ļ�����½�X���꣨������Ϊ��λ��
    Header4IMG.wBottom = height - 1;   // ͼ���������Ļ�����½�Y���꣨������Ϊ��λ��
    Header4IMG.wXResolution = (unsigned short) width;    // ͼ���ˮƽ�ֱ���
    Header4IMG.wYResolution = (unsigned short) height;   // ͼ��Ĵ�ֱ�ֱ���

    // д���ļ�ͷ
    WRITE_NBYTE((LPBYTE) & Header4IMG, sizeof(IMGHEADER));

    // �����0��
    i = 0;

    for (j = 0; j < width; j++) {
      // ָ��ͼ��0��j�����ص�ָ��
      lpSrc = data + step * (height - 1 - i) + j;

      // ��bCharD��ֵ
      bCharD = *lpSrc;

      // ����ǵ�0��0�У�ֱ�ӽ�����ֵд��
      if (j == 0) {
        nDpcm = (int) bCharD;
      }
      // ���� Dpcm ��D �� A ����в�
      else {
        bCharA = *(lpSrc - 1);
        nDpcm = (int) bCharD - (int) bCharA;
      }

      // ���в�д���ļ�
      WRITE_NBYTE(&nDpcm , sizeof(int));
    }

    // �����1�е�lHeight-1��
    for (i = 1; i < height; i++) {
      for (j = 0; j < width; j++) {
        // ָ��ǰ����Ԫ�ص�ָ��
        lpSrc = (LPBYTE) data + j + step * (height - 1 - i);

        // ��ֵ
        bCharD = *lpSrc;
        bCharB = *(lpSrc + step);

        // ����ǵ�һ�У����� �вD ��B ����Ԥ��
        if (j == 0) {
          nDpcm = (int) bCharD - (int) bCharB;
        }
        else {
          // ����(B-C)/2+A����Ԥ��ֵ
          bCharA = *(lpSrc - 1);
          bCharC = *(lpSrc + step - 1);
          nTemp = (int)((bCharB - bCharC) / 2 + bCharA);

          // ���Ԥ��ֵС��0��ֱ�Ӹ���
          if (nTemp < 0) {
            nTemp = 0;
          }
          // ���Ԥ��ֵ����255��ֱ�Ӹ�ֵ255
          else if (nTemp > 255) {
            nTemp = 255;
          }
          else {
            nTemp = nTemp;
          }

          // �õ��в�
          nDpcm = (int) bCharD - nTemp;
        }

        // ���в�д���ļ�
        WRITE_NBYTE(&nDpcm , sizeof(int));
      }
    }

    // ����һƬ�������Ա����ɫ��

    // ��ɫ����ʼ�ֽ�
    * lpDst = 0x0C;

    // ��ȡ��ǰͼ��ĵ�ɫ��
    for (i = 0; i < 256; i ++) {
      lpDst[ i * 3 + 1 ] = lpbmc[ i ].r; // ��ȡDIB��ɫ���ɫ����
      lpDst[ i * 3 + 2 ] = lpbmc[ i ].g; // ��ȡDIB��ɫ����ɫ����
      lpDst[ i * 3 + 3 ] = lpbmc[ i ].b; // ��ȡDIB��ɫ����ɫ����
    }

    // д���ɫ����Ϣ
    WRITE_NBYTE((LPBYTE) lpDst, 769);

    // ����ֵ
    result = TRUE;
  }

  return result;
}


#endif // SUPPORT_IMA
