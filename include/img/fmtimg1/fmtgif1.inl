#ifndef _FMTGIF_INL_
#define _FMTGIF_INL_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include "_fmtimg.h"
//#include "imcolor.h"
#include "sort.h"

typedef WORD* LPWORD;
typedef DWORD* LPDWORD;

typedef struct gifheader {
  BYTE bySignature[ 3 ];
  BYTE byVersion[ 3 ];
}
GIFHEADER;

typedef struct gifscrdesc {
  WORD wWidth;
  WORD wDepth;
  struct globalflag {
    BYTE PalBits : 3;
    BYTE SortFlag : 1;
    BYTE ColorRes : 3;
    BYTE GlobalPal : 1;
  }
  GlobalFlag;
  BYTE byBackground;
  BYTE byAspect;
}
GIFSCRDESC;

typedef struct gifimage {
  WORD wLeft;
  WORD wTop;
  WORD wWidth;
  WORD wDepth;
  struct localflag {
    BYTE PalBits : 3;
    BYTE Reserved : 2;
    BYTE SortFlag : 1;
    BYTE Interlace : 1;
    BYTE LocalPal : 1;
  }
  LocalFlag;
}
GIFIMAGE;

typedef struct gifcontrol {
  BYTE byBlockSize;
  struct flag {
BYTE Transparency :
    1;
BYTE UserInput :
    1;
BYTE DisposalMethod :
    3;
BYTE Reserved :
    3;
  }
  Flag;
  WORD wDelayTime;
  BYTE byTransparencyIndex;
  BYTE byTerminator;
}
GIFCONTROL;

typedef struct gifplaintext {
  BYTE byBlockSize;
  WORD wTextGridLeft;
  WORD wTextGridTop;
  WORD wTextGridWidth;
  WORD wTextGridDepth;
  BYTE byCharCellWidth;
  BYTE byCharCellDepth;
  BYTE byForeColorIndex;
  BYTE byBackColorIndex;
}
GIFPLAINTEXT;

typedef struct gifapplication {
  BYTE byBlockSize;
  BYTE byIdentifier[ 8 ];
  BYTE byAuthentication[ 3 ];
}
GIFAPPLICATION;

typedef struct gifd_var {
  LPBYTE lpDataBuff;
  LPBYTE lpBgnBuff;
  LPBYTE lpEndBuff;
  DWORD dwDataLen;
  WORD wMemLen;
  WORD wWidth;
  WORD wDepth;
  WORD wLineBytes;
  WORD wBits;
  int bEOF;
  int bInterlace;
}
GIFD_VAR;

typedef GIFD_VAR* LPGIFD_VAR;

typedef struct gifc_var {
  LPBYTE lpDataBuff;
  LPBYTE lpEndBuff;
  DWORD dwTempCode;
  WORD wWidth;
  WORD wDepth;
  WORD wLineBytes;
  WORD wBits;
  WORD wByteCnt;
  WORD wBlockNdx;
  BYTE byLeftBits;
}
GIFC_VAR;
typedef GIFC_VAR* LPGIFC_VAR;

// ������
#define DWORD_WBYTES(x)         ( (((x) + 31UL) >> 5) << 2 )
#define WORD_WBYTES(x)          ( (((x) + 15UL) >> 4) << 1 )
#define BYTE_WBYTES(x)          (  ((x) +  7UL) >> 3       )

//����
#define MAX_BUFF_SIZE           32768 /* 32K */
#define MAX_HASH_SIZE            5051
#define MAX_TABLE_SIZE           4096 /* 12-bit */
#define MAX_SUBBLOCK_SIZE         255

////////////////////////////////////////////////////////////////////////////////

typedef struct fmtgif {
  int m_bpp;
  int m_offset;
  int m_binary;
  int m_maxval;
  GIFD_VAR GIFDVar; // GIF�������
  PaletteEntry m_palette[ 256 ];
}
fmtgif;

int color_clustor(int h, int w, const unsigned char* A, int al, int ai, unsigned char* B, int bl,
    int color_num, unsigned char* colormap, int ci)
{
  int i, j, k, n, m, *iA, *iA2, **iiA, siz;
  typedef int* intptr;
  m = n = 0;
  siz = h * w;
  iA = MALLOC(int, siz);
  iA2 = MALLOC(int, siz);
  iiA = MALLOC(int*, siz);
  memset(colormap, 255, color_num * ci);

  for (i = 0; i < h; ++i) {
    const unsigned char* A1 = A + i * al;
    int* iA1 = iA + i * w;
    int** iiA1 = iiA + i * w;

    for (j = 0; j < w; ++j) {
      const unsigned char* A2 = A1 + j * ai;
      unsigned char a0 = A2[ 0 ], a1 = A2[ 1 ], a2 = A2[ 2 ];
      m = 0;
#define mmm(N) m<<=1; m|=((a0>>(N))&1); m<<=1; m|=((a1>>(N))&1); m<<=1; m|=((a2>>(N))&1);

      mmm(7);
      mmm(6);
      mmm(5);
      mmm(4);
      mmm(3);
      mmm(2);
      mmm(1);
      mmm(0);
#undef mmm

      iA1[ j ] = m;
      iiA1[ j ] = iA1 + j;
    }
  }

#define LESSPTR(a, b)   (*(a) < *(b))
  QSORT(iiA, iiA + siz, LESSPTR, int*);
#undef LESSPTR

  for (j = 0, i = 0; i < siz;) {
    iA2[ j++ ] = *iiA[ i++ ];

    while (i < siz && (*iiA[ i ] == *iiA[ i - 1 ])) {
      ++i;
    }
  }

  color_num = MIN(color_num, j);

  for (i = 0; i < color_num; ++i) {
    unsigned char a0, a1, a2;
    iA2[ i ] = iA2[ i * j / color_num ];
    k = m = iA2[ i ];
    a0 = a1 = a2 = 0;
#define mmm(N) a2|=((m&1)<<(N)); m>>=1; a1|=((m&1)<<(N)); m>>=1; a0|=((m&1)<<(N)); m>>=1;

    mmm(0);
    mmm(1);
    mmm(2);
    mmm(3);
    mmm(4);
    mmm(5);
    mmm(6);
    mmm(7);
#undef mmm

    colormap[ i * 3 + 0 ] = a2;
    colormap[ i * 3 + 1 ] = a1;
    colormap[ i * 3 + 2 ] = a0;
  }

  for (j = 1, i = 0; i < siz; ++i) {
    if (*(iiA[ i ]) > iA2[ j ] && j < (color_num - 1)) {
      ++j;
    }

    *(iiA[ i ]) = ((*(iiA[ i ]) - iA2[ j - 1 ]) < (iA2[ j ] - * (iiA[ i ]))) ? (j - 1) : j;
  }

  for (i = 0; i < h; ++i) {
    unsigned char* B1 = B + i * bl;
    int* iA1 = iA + i * w;

    for (j = 0; j < w; ++j) {
      B1[ j ] = iA1[ j ];
    }
  }

#if 0
  {
    unsigned char* lpDIBBits = MALLOC(BYTE, siz * 3);
    PaletteEntry m_palette[ 256 ];
    int fmtbmp_write(ihand_t m_strm, const img_t * im, const PaletteEntry * palette);

    for (i = 0; i < 256; ++i) {
      m_palette[ i ].b = colormap[ i * 3 + 0 ];
      m_palette[ i ].g = colormap[ i * 3 + 1 ];
      m_palette[ i ].r = colormap[ i * 3 + 2 ];
      m_palette[ i ].a = 0;
    }

    for (i = 0; i < h; ++i) {
      unsigned char* data1 = lpDIBBits + i * w * 3;
      unsigned char* src = B + i * bl;
      FillColorRow8(data1, src, w, m_palette);
    }

    fmtbmp_write("testa.bmp", h, w, lpDIBBits, w * 3, 3);
    fmtbmp_write("testa2.bmp", h, w, A, al, ai);
    SAFEFREE(lpDIBBits);
    //FPRINT2D("a.txt", "%3d, ", h, w, B, bl, 1);
    //FPRINT2D("m.txt", "%3d, ", 256, 3, colormap, 3, 1);
  }
#endif
  SAFEFREE(iA);
  SAFEFREE(iA2);
  SAFEFREE(iiA);
  return TRUE;
}

/*************************************************************************
* ��������:
*   GIF_LZW_WriteCode()
 *
* ����:
*   ihand_t m_strm   - Ҫ������ļ�
*  WORD wCode    - Ҫ��ӵı���
*  LPBYTE lpSubBlock  - �ӿ�
*  LPBYTE lpbyCurrentBits - ��ǰλ��
*  LPGIFC_VAR lpGIFCVar - ָ��GIFC_VAR�ṹ��ָ��
 *
 * ����ֵ:
 *   ��
 *
 * ˵��:
 *   �ú����������һ�����롣
 *
 *************************************************************************/
void GIF_LZW_WriteCode(ihand_t m_strm, WORD wCode, LPBYTE lpSubBlock,
    LPBYTE lpbyCurrentBits, LPGIFC_VAR lpGIFCVar)
{
  // ����ñ���
  lpGIFCVar->dwTempCode |= ((DWORD) wCode << lpGIFCVar->byLeftBits);
  lpGIFCVar->byLeftBits += (*lpbyCurrentBits);

  while (lpGIFCVar->byLeftBits >= 0x08) {
    lpSubBlock[ lpGIFCVar->wBlockNdx++ ] = (BYTE) lpGIFCVar->dwTempCode;

    // �ж��Ƿ񳬳�MAX_SUBBLOCK_SIZE
    if (lpGIFCVar->wBlockNdx > MAX_SUBBLOCK_SIZE) {
      // �ж�wByteCnt + 256�Ƿ񳬹�MAX_BUFF_SIZE
      if ((lpGIFCVar->wByteCnt + 256) >= MAX_BUFF_SIZE) {
        // ���
        WRITE_NBYTE(lpGIFCVar->lpDataBuff, lpGIFCVar->wByteCnt);
        lpGIFCVar->lpEndBuff = lpGIFCVar->lpDataBuff;
        lpGIFCVar->wByteCnt = 0;
      }

      lpSubBlock[ 0 ] = (BYTE)(lpGIFCVar->wBlockNdx - 1);
      memcpy(lpGIFCVar->lpEndBuff, lpSubBlock, lpGIFCVar->wBlockNdx);
      lpGIFCVar->lpEndBuff += lpGIFCVar->wBlockNdx;
      lpGIFCVar->wByteCnt += lpGIFCVar->wBlockNdx;
      lpGIFCVar->wBlockNdx = 1;
    }

    lpGIFCVar->dwTempCode >>= 8;
    lpGIFCVar->byLeftBits -= 0x08;
  }

  // ����
  return ;
}

/*************************************************************************
 *
* ��������:
*   EncodeGIF_LZW()
 *
* ����:
*   LPBYTE lpDIBBits  - ָ��ԴDIBͼ��ָ��
*   ihand_t m_strm   - Ҫ������ļ�
*   LPGIFC_VAR lpGIFCVar - ָ��GIFC_VAR�ṹ��ָ��
*  WORD wWidthBytes  - ÿ��ͼ���ֽ���
*  int bInterlace  - �Ƿ��ս���ʽ����
 *
* ����ֵ:
*   ��
 *
* ˵��:
*   �ú�����ָ��ͼ�����GIF_LZW���롣
 *
 *************************************************************************/
void EncodeGIF_LZW(LPBYTE lpDIBBits, ihand_t m_strm,
    LPGIFC_VAR lpGIFCVar, int wWidthBytes, int bInterlace)
{
  LPWORD lpwTableNdx; // ָ���ִ���ָ��

  // �����ִ�������������
  LPWORD lpwPrefix;
  LPBYTE lpbySuffix;

  LPBYTE lpImage; // ָ��ǰ�������ص�ָ��
  DWORD dwDataNdx; // ���㵱ǰ����ͼ���ƫ����
  WORD wLZW_CLEAR; // LZW_CLEAR
  WORD wLZW_EOI; // LZW_EOI
  BYTE byLZW_MinCodeLen; // LZW_MinCodeLen

  // �ִ�������
  WORD wPreTableNdx;
  WORD wNowTableNdx;
  WORD wTopTableNdx;

  // ��ϣ������
  WORD wHashNdx;
  WORD wHashGap;
  WORD wPrefix;
  WORD wShiftBits;

  // ��ǰͼ�������
  WORD wRowNum;
  WORD wWidthCnt;

  // ѭ������
  WORD wi;
  WORD wj;

  // ����ʽ�洢ʱÿ�����ӵ�����
  WORD wIncTable[ 5 ] = { 8, 8, 4, 2, 0 };

  // ����ʽ�洢ʱ��ʼ����
  WORD wBgnTable[ 5 ] = { 0, 4, 2, 1, 0 };

  int bStart;
  BYTE bySuffix;
  BYTE bySubBlock[ 256 ];
  BYTE byCurrentBits;
  BYTE byMask;
  BYTE byChar;
  BYTE byPass;

  // ��ʱ�ֽڱ���
  BYTE byTemp;

  // ���ִ�������ڴ�
  lpwTableNdx = MALLOC(WORD, MAX_HASH_SIZE);
  lpwPrefix = MALLOC(WORD, MAX_HASH_SIZE);
  lpbySuffix = MALLOC(BYTE, MAX_HASH_SIZE);

  // ����LZW_MinCodeLen
  byLZW_MinCodeLen = (BYTE)((lpGIFCVar->wBits > 1) ? lpGIFCVar->wBits : 0x02);

  // дGIF LZW��С�����С
  WRITE_NBYTE((LPBYTE) & byLZW_MinCodeLen, 1);

  wRowNum = 0;
  bStart = TRUE;
  byPass = 0x00;

  // ����LZW_CLEAR
  wLZW_CLEAR = 1 << byLZW_MinCodeLen;

  // ����LZW_EOI
  wLZW_EOI = wLZW_CLEAR + 1;

  // ��ʼ���ִ���
  byCurrentBits = byLZW_MinCodeLen + (BYTE) 0x01;
  wNowTableNdx = wLZW_CLEAR + 2;
  wTopTableNdx = 1 << byCurrentBits;

  for (wi = 0; wi < MAX_HASH_SIZE; wi++) {
    // ��ʼ��Ϊ0xFFFF
    *(lpwTableNdx + wi) = 0xFFFF;
  }

  // ���LZW_CLEAR
  GIF_LZW_WriteCode(m_strm, wLZW_CLEAR, (LPBYTE) bySubBlock,
      &byCurrentBits, lpGIFCVar);

  // ���б���
  for (wi = 0; wi < lpGIFCVar->wDepth; wi++) {
    // ���㵱ǰƫ����
    dwDataNdx = (DWORD)(wRowNum) * (DWORD) wWidthBytes;

    // ָ��ǰ��ͼ���ָ��
    lpImage = (LPBYTE)(((LPBYTE) lpDIBBits) + dwDataNdx);

    wWidthCnt = 0;
    wShiftBits = 8 - lpGIFCVar->wBits;
    byMask = (BYTE)((lpGIFCVar->wBits == 1) ? 0x80 : 0xF0);

    if (bStart) {
      // �ж��Ƿ���256ɫλͼ��һ������һ�ֽڣ�
      if (lpGIFCVar->wBits == 8) {
        // 256ɫ��ֱ�Ӹ�ֵ����
        byTemp = *lpImage++;
      }
      else {
        // ��256ɫ����Ҫ��λ��ȡ����ֵ
        wShiftBits = 8 - lpGIFCVar->wBits;
        byMask = (BYTE)((lpGIFCVar->wBits == 1) ? 0x80 : 0xF0);
        byTemp = (BYTE)((*lpImage & byMask) >> wShiftBits);
        byMask >>= lpGIFCVar->wBits;
        wShiftBits -= lpGIFCVar->wBits;
      }

      wPrefix = (WORD) byTemp;
      bStart = FALSE;
      wWidthCnt ++;
    }

    // ÿ�б���
    while (wWidthCnt < lpGIFCVar->wWidth) {
      // �ж��Ƿ���256ɫλͼ��һ������һ�ֽڣ�
      if (lpGIFCVar->wBits == 8) {
        // 256ɫ��ֱ�Ӹ�ֵ����
        byTemp = *lpImage++;
      }
      else {
        // ��256ɫ����Ҫ��λ��ȡ����ֵ
        byChar = *lpImage;
        byTemp = (BYTE)((byChar & byMask) >> wShiftBits);

        if (wShiftBits) {
          byMask >>= lpGIFCVar->wBits;
          wShiftBits -= lpGIFCVar->wBits;
        }
        else {
          wShiftBits = 8 - lpGIFCVar->wBits;
          byMask = (BYTE)((lpGIFCVar->wBits == 1) ? 0x80 : 0xF0);
          lpImage ++;
        }
      }

      bySuffix = byTemp;
      wWidthCnt ++;

      // ���ҵ�ǰ�ַ����Ƿ�������ִ�����
      wHashNdx = wPrefix ^ ((WORD) bySuffix << 4);
      wHashGap = (wHashNdx ? (MAX_HASH_SIZE - wHashNdx) : 1);

      // �жϵ�ǰ�ַ����Ƿ����ִ�����
      while (TRUE) {
        // ��ǰ�ַ��������ִ�����
        if (*(lpwTableNdx + wHashNdx) == 0xFFFF) {
          // ���ַ������˳�ѭ��
          break;
        }

        // �ж��Ƿ��ҵ����ַ���
        if ((*(lpwPrefix + wHashNdx) == wPrefix) &&
            (*(lpbySuffix + wHashNdx) == bySuffix)) {
          // �ҵ����˳�ѭ��
          break;
        }

        // �ڶ���ϣ��
        if (wHashNdx < wHashGap) {
          wHashNdx += MAX_HASH_SIZE;
        }

        wHashNdx -= wHashGap;
      }

      // �ж��Ƿ������ַ���
      if (*(lpwTableNdx + wHashNdx) != 0xFFFF) {
        // �������ַ���
        wPrefix = *(lpwTableNdx + wHashNdx);
      }
      else {
        // ���ַ���

        // ����ñ���
        GIF_LZW_WriteCode(m_strm, wPrefix, (LPBYTE) bySubBlock,
            &byCurrentBits, lpGIFCVar);

        // �������ַ�����ӵ��ִ�����
        wPreTableNdx = wNowTableNdx;

        // �ж��Ƿ�ﵽ����ִ����С
        if (wNowTableNdx < MAX_TABLE_SIZE) {
          *(lpwTableNdx + wHashNdx) = wNowTableNdx++;
          *(lpwPrefix + wHashNdx) = wPrefix;
          *(lpbySuffix + wHashNdx) = bySuffix;
        }

        if (wPreTableNdx == wTopTableNdx) {
          if (byCurrentBits < 12) {
            byCurrentBits ++;
            wTopTableNdx <<= 1;
          }
          else {
            // �ִ�������󳤶�

            // ���LZW_CLEAR
            GIF_LZW_WriteCode(m_strm, wLZW_CLEAR, (LPBYTE) bySubBlock,
                &byCurrentBits, lpGIFCVar);

            // ���³�ʼ���ִ���
            byCurrentBits = byLZW_MinCodeLen + (BYTE) 0x01;
            wLZW_CLEAR = 1 << byLZW_MinCodeLen;
            wLZW_EOI = wLZW_CLEAR + 1;
            wNowTableNdx = wLZW_CLEAR + 2;
            wTopTableNdx = 1 << byCurrentBits;

            for (wj = 0; wj < MAX_HASH_SIZE; wj++) {
              // ��ʼ��Ϊ0xFFFF
              *(lpwTableNdx + wj) = 0xFFFF;
            }
          }
        }

        wPrefix = (WORD) bySuffix;
      }
    }

    // �ж��Ƿ��ǽ���ʽ
    if (bInterlace) {
      // ����ʽ��������һ��λ��
      wRowNum += wIncTable[ byPass ];

      if (wRowNum >= lpGIFCVar->wDepth) {
        byPass ++;
        wRowNum = wBgnTable[ byPass ];
      }
    }
    else {
      // �ǽ���ʽ��ֱ�ӽ�������һ����
      wRowNum ++;
    }
  }

  // �����ǰ����
  GIF_LZW_WriteCode(m_strm, wPrefix, (LPBYTE) bySubBlock,
      &byCurrentBits, lpGIFCVar);

  // ���LZW_EOI
  GIF_LZW_WriteCode(m_strm, wLZW_EOI, (LPBYTE) bySubBlock,
      &byCurrentBits, lpGIFCVar);

  if (lpGIFCVar->byLeftBits) {
    // ������ַ�
    bySubBlock[ lpGIFCVar->wBlockNdx++ ] = (BYTE) lpGIFCVar->dwTempCode;

    // �ж��Ƿ񳬳�MAX_SUBBLOCK_SIZE
    if (lpGIFCVar->wBlockNdx > MAX_SUBBLOCK_SIZE) {
      // �ж�wByteCnt + 256�Ƿ񳬹�MAX_BUFF_SIZE
      if ((lpGIFCVar->wByteCnt + 256) >= MAX_BUFF_SIZE) {
        // ���
        WRITE_NBYTE(lpGIFCVar->lpDataBuff,
            lpGIFCVar->wByteCnt);
        lpGIFCVar->lpEndBuff = lpGIFCVar->lpDataBuff;
        lpGIFCVar->wByteCnt = 0;
      }

      bySubBlock[ 0 ] = (BYTE)(lpGIFCVar->wBlockNdx - 1);
      memcpy(lpGIFCVar->lpEndBuff, (LPBYTE) bySubBlock, lpGIFCVar->wBlockNdx);
      lpGIFCVar->lpEndBuff += lpGIFCVar->wBlockNdx;
      lpGIFCVar->wByteCnt += lpGIFCVar->wBlockNdx;
      lpGIFCVar->wBlockNdx = 1;
    }

    lpGIFCVar->dwTempCode = 0UL;
    lpGIFCVar->byLeftBits = 0x00;
  }

  if (lpGIFCVar->wBlockNdx > 1) {
    // �ж�wByteCnt + 256�Ƿ񳬹�MAX_BUFF_SIZE
    if ((lpGIFCVar->wByteCnt + 256) >= MAX_BUFF_SIZE) {
      // ���
      WRITE_NBYTE(lpGIFCVar->lpDataBuff,
          lpGIFCVar->wByteCnt);
      lpGIFCVar->lpEndBuff = lpGIFCVar->lpDataBuff;
      lpGIFCVar->wByteCnt = 0;
    }

    bySubBlock[ 0 ] = (BYTE)(lpGIFCVar->wBlockNdx - 1);
    memcpy(lpGIFCVar->lpEndBuff, (LPBYTE) bySubBlock, lpGIFCVar->wBlockNdx);
    lpGIFCVar->lpEndBuff += lpGIFCVar->wBlockNdx;
    lpGIFCVar->wByteCnt += lpGIFCVar->wBlockNdx;
    lpGIFCVar->wBlockNdx = 1;
  }

  // �ͷ��ڴ�
  SAFEFREE(lpwTableNdx);
  SAFEFREE(lpwPrefix);
  SAFEFREE(lpbySuffix);

  // �˳�
  return ;
}

/*************************************************************************
 * ��������:
 *   fmtgif_write()
 *
 * ����:
 *   LPBYTE lpDIB     - ָ��DIB�����ָ��
 *   ihand_t m_strm       - Ҫ������ļ�
 *   int bInterlace  - �Ƿ��ս���ʽ����
 *   LPBYTE lpDIBBits - ָ�����ص�ָ��
 *   int wHeight      - �߶�
 *   int wWidth       - ���
 *   int wWidthBytes  - ÿ�е��ֽ���
 *
 * ����ֵ:
 *   int               - �ɹ�����TRUE�����򷵻�False��
 *
 * ˵��:
 *   �ú�����ָ����ͼ��< 256ɫ������ΪGIF�ļ���
 *
 *************************************************************************/
int fmtgif_write(ihand_t m_strm, const img_t* im, const PaletteEntry* palette)
{
  // ѭ������
  int i, j;
  int wHeight = im->h, wWidth = im->w, wWidthBytes = im->s, channels = im->c;
  const unsigned char* data = im->tt.data;

  int isBinary = FALSE;
  int result = FALSE;
  int bInterlace = 0;

  // ָ��DIB����ָ��
  LPBYTE lpDIBBits = (LPBYTE) data;
  LPBYTE lpSrcBuff;
  GIFHEADER GIFH;   // GIF�ļ�ͷ
  GIFSCRDESC GIFS; // GIF�߼���Ļ������
  GIFIMAGE GIFI; // GIFͼ��������
  GIFC_VAR GIFCVar; // GIF�������
  WORD wColors; // ��ɫ��Ŀ
  BYTE byGIF_Pal[ 768 ]; // ��ɫ��
  BYTE byChar; // �ֽڱ���
  int bWinStyleDIB = channels > 1; // �����Ƿ���Win3.0 DIB�ı��

  // ��GIFCVar�ṹ��ֵ
  GIFCVar.wWidth = wWidth;
  GIFCVar.wDepth = wHeight;
  GIFCVar.wBits = 8;
  GIFCVar.wLineBytes = (WORD) BYTE_WBYTES((DWORD) GIFCVar.wWidth *
      (DWORD) GIFCVar.wBits);

  // ������ɫ��Ŀ
  wColors = 1 << GIFCVar.wBits;

  // ����ɫ�帳ֵ
  if (bWinStyleDIB) {
    lpDIBBits = MALLOC(BYTE, wHeight * wWidth);
    color_clustor(wHeight, wWidth, data, wWidthBytes, channels, lpDIBBits, wWidth, wColors, byGIF_Pal, 3);
    wWidthBytes = wWidth;
  }
  else {
    j = 0;

    for (i = 0; i < wColors; ++i, j += 3) {
      byGIF_Pal[ j + 0 ] = i; // ��ȡ��ɫ����
      byGIF_Pal[ j + 1 ] = i; // ��ȡ��ɫ����
      byGIF_Pal[ j + 2 ] = i; // ��ȡ��ɫ����
    }
  }

  // ��ʼдGIF�ļ�
  GIFH.bySignature[ 0 ] = 'G';
  GIFH.bySignature[ 1 ] = 'I';
  GIFH.bySignature[ 2 ] = 'F';
  GIFH.byVersion[ 0 ] = '8';
  GIFH.byVersion[ 1 ] = '9';
  GIFH.byVersion[ 2 ] = 'a';
  WRITE_NBYTE((LPBYTE) & GIFH, 6);

  // дGIF�߼���Ļ������
  GIFS.wWidth = GIFCVar.wWidth;
  GIFS.wDepth = GIFCVar.wDepth;
  GIFS.GlobalFlag.PalBits = (BYTE)(GIFCVar.wBits - 1);
  GIFS.GlobalFlag.SortFlag = 0x00;
  GIFS.GlobalFlag.ColorRes = (BYTE)(GIFCVar.wBits - 1);
  GIFS.GlobalFlag.GlobalPal = 0x01;
  GIFS.byBackground = 0x00;
  GIFS.byAspect = 0x00;
  WRITE_NBYTE((LPBYTE) & GIFS, 7);

  // дGIFȫ�ֵ�ɫ��
  WRITE_NBYTE((LPBYTE) byGIF_Pal, (wColors * 3));

  // дGIFͼ�����������
  byChar = 0x2C;
  WRITE_NBYTE((LPBYTE) & byChar, 1);

  // дGIFͼ��������
  GIFI.wLeft = 0;
  GIFI.wTop = 0;
  GIFI.wWidth = GIFCVar.wWidth;
  GIFI.wDepth = GIFCVar.wDepth;
  GIFI.LocalFlag.PalBits = 0x00;
  GIFI.LocalFlag.Reserved = 0x00;
  GIFI.LocalFlag.SortFlag = 0x00;
  GIFI.LocalFlag.Interlace = (BYTE)(bInterlace ? 0x01 : 0x00);
  GIFI.LocalFlag.LocalPal = 0x00;
  WRITE_NBYTE((LPBYTE) & GIFI, 9);

  // дGIFͼ��ѹ������
  lpSrcBuff = MALLOC(BYTE, MAX_BUFF_SIZE);
  GIFCVar.lpDataBuff = lpSrcBuff;
  GIFCVar.lpEndBuff = GIFCVar.lpDataBuff;
  GIFCVar.dwTempCode = 0UL;
  GIFCVar.wByteCnt = 0;
  GIFCVar.wBlockNdx = 1;
  GIFCVar.byLeftBits = 0x00;

  // ����GIF_LZW����
  EncodeGIF_LZW(lpDIBBits, m_strm, &GIFCVar, wWidthBytes, bInterlace);

  // �ж��Ƿ����ɹ�
  if (GIFCVar.wByteCnt) {
    // д���ļ�
    WRITE_NBYTE(GIFCVar.lpDataBuff, GIFCVar.wByteCnt);
  }

  // �ͷ��ڴ�
  SAFEFREE(lpSrcBuff);

  // дGIF Block Terminator
  byChar = 0x00;
  WRITE_NBYTE((LPBYTE) & byChar, 1);

  // дGIF�ļ���β��
  byChar = 0x3B;
  WRITE_NBYTE((LPBYTE) & byChar, 1);

  if (data != lpDIBBits) {
    SAFEFREE(lpDIBBits);
  }

  // ����
  return TRUE;
}

/*************************************************************************
* ��������:
*   ReadSrcData()
 *
* ����:
*   ihand_t m_strm   - ԴGIF�ļ�
*   LPWORD lpwMemLen  - ���������ȣ�ָ�룩
*   LPDWORD lpdwDataLen - ʣ�����ݳ��ȣ�ָ�룩
*   LPBYTE lpSrcBuff  - ������ָ��
*  LPBOOL lpbEOF   - ������־
 *
* ����ֵ:
*   ��
 *
* ˵��:
*   �ú���������ȡָ��GIF�ļ��е�ͼ����룬ÿ������ȡMAX_BUFF_SIZE
* �ֽڣ��Ƿ�����ɱ�־lpbEOFָ����
 *************************************************************************/
void ReadSrcData(ihand_t m_strm, LPWORD lpwMemLen, LPDWORD lpdwDataLen,
    LPBYTE lpSrcBuff, int* lpbEOF)
{
  // �ж����ݳ����Ƿ���Ȼ�����ڴ��С
  if ((*lpdwDataLen) > (DWORD)(*lpwMemLen)) {
    // ���ݳ��ȴ����ڴ��С����ʾû�н�����
    // ���ݳ��ȼ��ڴ��С
    (*lpdwDataLen) -= (DWORD)(*lpwMemLen);
  }
  else {
    // ���ݳ��Ȳ������ڴ��С����ʾ���뽫Ҫ����ڴ��С����ʣ�����ݳ���
    (*lpwMemLen) = (WORD)(*lpdwDataLen);
    // EOF��־����ΪTRUE
    (*lpbEOF) = TRUE;
  }

  // ��ȡ��������
  READ_NBYTE(lpSrcBuff, (*lpwMemLen));
  // ����
  return ;
}

/*************************************************************************
* ��������:
*   DecodeGIF_LZW()
 *
* ����:
*   ihand_t m_strm           - ԴGIF�ļ�
*   LPBYTE lpDIBBits     - ָ��Ҫ�����DIBͼ��ָ��
*   LPGIFD_VAR lpGIFDVar - ָ��GIFC_VAR�ṹ��ָ��
*   WORD wWidthBytes     - ÿ��ͼ���ֽ���
 *
* ����ֵ:
*   ��
 *
* ˵��:
*   �ú�����ָ��GIF_LZW�������ݽ��н��롣
 *************************************************************************/
void DecodeGIF_LZW(ihand_t m_strm, LPBYTE lpDIBBits, LPGIFD_VAR lpGIFDVar, int wWidthBytes)
{
  LPBYTE lpDst; // ָ������ͼ�����ݵ�ָ��
  // �����ִ�������������
  LPWORD lpwPrefix;
  LPBYTE lpbySuffix;
  LPBYTE lpbyStack;
  LPBYTE lpbyStackBgn;
  LPBYTE lpImageBgn;      // ָ��ͼ��ǰ�н�������ָ��
  LPBYTE lpImage;         // ָ��ǰ�������ص�ָ��
  DWORD dwDataNdx;       // ���㵱ǰ����ͼ���ƫ����
  WORD wLZW_CLEAR;      // LZW_CLEAR
  WORD wLZW_EOI;        // LZW_EOI
  BYTE byLZW_MinCodeLen; // LZW_MinCodeLen

  // �ִ�������
  WORD wNowTableNdx;
  WORD wTopTableNdx;

  WORD wRowNum; // ��ǰͼ�������

  // ����
  WORD wWidthCnt;
  WORD wBitCnt;
  WORD wRowCnt;
  WORD wi; // ѭ������
  // ����ʽ�洢ʱÿ�����ӵ�����
  WORD wIncTable[ 5 ] = { 8, 8, 4, 2, 0 };
  // ����ʽ�洢ʱ��ʼ����
  WORD wBgnTable[ 5 ] = { 0, 4, 2, 1, 0 };
  BYTE byBlockSize; // ���С
  BYTE byBlockNdx; // ������
  DWORD dwData;
  WORD wCode;   // ��ǰ����
  WORD wOldCode; // ��һ������
  WORD wTempNdx; // ��ʱ����

  WORD wCodeMask[ 13 ] = {   //
    0x0000,
    0x0001, 0x0003, 0x0007, 0x000F,
    0x001F, 0x003F, 0x007F, 0x00FF,
    0x01FF, 0x03FF, 0x07FF, 0x0FFF
  };

  BYTE byLeftBits;
  BYTE byFirstChar;
  BYTE byCode;
  BYTE byCurrentBits;
  BYTE byPass;
  BYTE byTempChar; // ��ʱ�ֽڱ���

  // ���ִ�������ڴ�
  lpwPrefix = MALLOC(WORD, MAX_TABLE_SIZE);
  lpbySuffix = MALLOC(BYTE, MAX_TABLE_SIZE);
  lpbyStack = MALLOC(BYTE, MAX_TABLE_SIZE);
  lpImage = MALLOC(BYTE, wWidthBytes);

  lpbyStackBgn = lpbyStack;
  lpImageBgn = lpImage;
  byLZW_MinCodeLen = *lpGIFDVar->lpBgnBuff++; // ��ȡGIF LZW��С�����С
  byCurrentBits = byLZW_MinCodeLen + (BYTE) 0x01;
  wLZW_CLEAR = 1 << byLZW_MinCodeLen; // ����LZW_CLEAR
  wLZW_EOI = wLZW_CLEAR + 1; // ����LZW_EOI

  // �����ִ�������
  wNowTableNdx = wLZW_CLEAR + 2;
  wTopTableNdx = 1 << byCurrentBits;

  // ����ֵ
  dwData = 0UL;
  wBitCnt = lpGIFDVar->wBits;
  wRowNum = 0;
  wRowCnt = 1;
  wWidthCnt = 0;
  wCode = 0;
  wOldCode = 0xFFFF;
  byBlockSize = 0x01;
  byBlockNdx = 0x00;
  byLeftBits = 0x00;
  byTempChar = 0x00;
  byPass = 0x00;

  // ��ȡ��һ������
  while (byLeftBits < byCurrentBits) {
    // ��ȡ��һ���ַ�
    // �ж��Ƿ����һ�����ݿ�
    if (++byBlockNdx == byBlockSize) {
      // ��ȡ��һ�����ݿ�
      byBlockSize = *lpGIFDVar->lpBgnBuff++;
      byBlockNdx = 0x00;

      // �ж��Ƿ����
      if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
          !lpGIFDVar->bEOF) {
        // ��ȡ��һ�����ݿ�
        ReadSrcData(m_strm, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen,
            lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);

        // ָ�����¸�ֵ
        lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
        lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
      }
    }

    // ��һ���ַ�
    byCode = *lpGIFDVar->lpBgnBuff++;

    // ��λ
    dwData |= ((DWORD) byCode << byLeftBits);
    byLeftBits += 0x08;

    // �ж��Ƿ����
    if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
        !lpGIFDVar->bEOF) {
      // ��ȡ��һ�����ݿ�
      ReadSrcData(m_strm, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen,
          lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);

      // ָ�����¸�ֵ
      lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
      lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
    }
  }

  wCode = (WORD) dwData & wCodeMask[ byCurrentBits ];
  dwData >>= byCurrentBits;
  byLeftBits -= byCurrentBits;

  // ����
  while (wCode != wLZW_EOI) {
    // ��ǰ���벻��LZW_EOI��
    // �ж��Ƿ���LZW_CLEAR��
    if (wCode == wLZW_CLEAR) {
      // ��LZW_CLEAR������ִ���
      // ���³�ʼ���ִ���
      for (wi = 0; wi < wLZW_CLEAR; wi++) {
        *(lpwPrefix + wi) = 0xFFFF;
        *(lpbySuffix + wi) = (BYTE) wi;
      }

      for (wi = wNowTableNdx; wi < MAX_TABLE_SIZE; wi++) {
        *(lpwPrefix + wi) = 0xFFFF;
        *(lpbySuffix + wi) = 0x00;
      }

      byCurrentBits = byLZW_MinCodeLen + (BYTE) 0x01;
      wNowTableNdx = wLZW_CLEAR + 2;
      wTopTableNdx = 1 << byCurrentBits;
      wOldCode = 0xFFFF;

      // ��ȡ��һ������
      while (byLeftBits < byCurrentBits) {
        // ��ȡ��һ���ַ�
        // �ж��Ƿ����һ�����ݿ�
        if (++byBlockNdx == byBlockSize) {
          // ��ȡ��һ�����ݿ�
          byBlockSize = *lpGIFDVar->lpBgnBuff++;
          byBlockNdx = 0x00;

          // �ж��Ƿ����
          if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) && !lpGIFDVar->bEOF) {
            // ��ȡ��һ�����ݿ�
            ReadSrcData(m_strm, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen,
                lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);

            // ָ�����¸�ֵ
            lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
            lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
          }
        }

        byCode = *lpGIFDVar->lpBgnBuff++;
        dwData |= ((DWORD) byCode << byLeftBits);
        byLeftBits += 0x08;

        // �ж��Ƿ����
        if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
            !lpGIFDVar->bEOF) {
          // ��ȡ��һ�����ݿ�
          ReadSrcData(m_strm, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen,
              lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);

          // ָ�����¸�ֵ
          lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
          lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
        }
      }

      wCode = (WORD) dwData & wCodeMask[ byCurrentBits ];
      dwData >>= byCurrentBits;
      byLeftBits -= byCurrentBits;

      // �жϱ����Ƿ�ΪLZW_EOI
      if (wCode != wLZW_EOI) {
        lpbyStack++; // ����û���õ�lpbyStack[0]

        // ������ѹ���ջ
        while ((*(lpwPrefix + wCode)) != 0xFFFF) {
          *lpbyStack++ = *(lpbySuffix + wCode);
          wCode = *(lpwPrefix + wCode);
        }

        *lpbyStack = *(lpbySuffix + wCode);
        byFirstChar = *lpbyStack;

        // �������
        while (lpbyStack > lpbyStackBgn) {
          byTempChar |= (*lpbyStack-- << (8 - wBitCnt));

          if (wBitCnt == 8) {
            *lpImage++ = byTempChar;
            byTempChar = 0x00;
            wBitCnt = lpGIFDVar->wBits;
          }
          else {
            wBitCnt += lpGIFDVar->wBits;
          }

          wWidthCnt++;

          if (wWidthCnt == lpGIFDVar->wWidth) {
            if (wBitCnt != lpGIFDVar->wBits) {
              *lpImage = byTempChar;
              byTempChar = 0x00;
              wBitCnt = lpGIFDVar->wBits;
            }


            dwDataNdx = (wRowNum) * wWidthBytes;   // ͼ��ǰ��ƫ����


            lpDst = (LPBYTE) lpDIBBits + dwDataNdx;   // ͼ��ǰ����ʼλ��

            // ��ֵ
            memcpy(lpDst, lpImageBgn, wWidthBytes);

            lpImage = lpImageBgn;

            // �ж��Ƿ��ս���ʽ����
            if (lpGIFDVar->bInterlace) {
              // ����ʽ
              // ������һ�е��к�
              wRowNum += wIncTable[ byPass ];

              if (wRowNum >= lpGIFDVar->wDepth) {
                byPass ++;
                wRowNum = wBgnTable[ byPass ];
              }
            }
            else {
              // �ǽ���ʽ���к�ֱ�Ӽ�1
              wRowNum ++;
            }

            wWidthCnt = 0;
          }
        }
      }
    }
    else {
      // ����û���õ�lpbyStack[0]
      lpbyStack++;

      // �ж��ַ����Ƿ����ִ�����
      if (wCode < wNowTableNdx) {
        // �����ִ�����
        wTempNdx = wCode;
      }
      else {
        // ���ִ�����
        wTempNdx = wOldCode;
        *lpbyStack++ = byFirstChar;
      }

      // ������ѹ���ջ
      while ((*(lpwPrefix + wTempNdx)) != 0xFFFF) {
        *lpbyStack++ = *(lpbySuffix + wTempNdx);
        wTempNdx = *(lpwPrefix + wTempNdx);
      }

      *lpbyStack = *(lpbySuffix + wTempNdx);
      byFirstChar = *lpbyStack;

      // ���ַ�����ӵ��ִ�����
      *(lpwPrefix + wNowTableNdx) = wOldCode;
      *(lpbySuffix + wNowTableNdx) = byFirstChar;

      if (++wNowTableNdx == wTopTableNdx && byCurrentBits < 12) {
        byCurrentBits ++;
        wTopTableNdx = 1 << byCurrentBits;
      }

      // �������
      while (lpbyStack > lpbyStackBgn) {
        byTempChar |= (*lpbyStack-- << (8 - wBitCnt));

        if (wBitCnt == 8) {
          *lpImage++ = byTempChar;
          byTempChar = 0x00;
          wBitCnt = lpGIFDVar->wBits;
        }
        else {
          wBitCnt += lpGIFDVar->wBits;
        }

        wWidthCnt++;

        if (wWidthCnt == lpGIFDVar->wWidth) {
          if (wBitCnt != lpGIFDVar->wBits) {
            *lpImage = byTempChar;
            byTempChar = 0x00;
            wBitCnt = lpGIFDVar->wBits;
          }


          dwDataNdx = (wRowNum) * wWidthBytes;   // ͼ��ǰ��ƫ����


          lpDst = (LPBYTE) lpDIBBits + dwDataNdx;   // ͼ��ǰ����ʼλ��


          memcpy(lpDst, lpImageBgn, wWidthBytes);   // ��ֵ

          lpImage = lpImageBgn;

          // �ж��Ƿ��ս���ʽ����
          if (lpGIFDVar->bInterlace) {
            // ����ʽ

            // ������һ�е��к�
            wRowNum += wIncTable[ byPass ];

            if (wRowNum >= lpGIFDVar->wDepth) {
              byPass ++;
              wRowNum = wBgnTable[ byPass ];
            }
          }
          else {
            // �ǽ���ʽ���к�ֱ�Ӽ�1
            wRowNum ++;
          }

          wWidthCnt = 0;
        }
      }
    }

    wOldCode = wCode;

    // ��ȡ��һ������
    while (byLeftBits < byCurrentBits) {
      // ��ȡ��һ���ַ�
      // �ж��Ƿ����һ�����ݿ�
      if (++byBlockNdx == byBlockSize) {
        // ��ȡ��һ�����ݿ�
        byBlockSize = *lpGIFDVar->lpBgnBuff++;
        byBlockNdx = 0x00;

        // �ж��Ƿ����
        if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
            !lpGIFDVar->bEOF) {
          // ��ȡ��һ�����ݿ�
          ReadSrcData(m_strm, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen,
              lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);

          // ָ�����¸�ֵ
          lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
          lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
        }
      }

      byCode = *lpGIFDVar->lpBgnBuff++;
      dwData |= ((DWORD) byCode << byLeftBits);
      byLeftBits += 0x08;

      // �ж��Ƿ����
      if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
          !lpGIFDVar->bEOF) {
        // ��ȡ��һ�����ݿ�
        ReadSrcData(m_strm, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen,
            lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);

        // ָ�����¸�ֵ
        lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
        lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
      }
    }

    wCode = (WORD) dwData & wCodeMask[ byCurrentBits ];
    dwData >>= byCurrentBits;
    byLeftBits -= byCurrentBits;
  }

  // �ͷ��ڴ�
  SAFEFREE(lpwPrefix);
  SAFEFREE(lpbySuffix);
  SAFEFREE(lpbyStack);

  // ����
  return ;
}

/*************************************************************************
* ��������:
*   GIFRead()
 *
* ����:
*   ihand_t m_strm        - Ҫ��ȡ���ļ�
 *
* ����ֵ:
*   HDIB               - �ɹ�����DIB�ľ�������򷵻�NULL��
 *
* ˵��:
*   �ú�������ȡָ����GIF�ļ�������ȡ�Ľ��������һ��δѹ��
* �����DIB�����С�
 *
 *************************************************************************/
int fmtgif_read_data(ihand_t m_strm, img_t* im, fmtgif* fmt)
{
  unsigned char* data = im->tt.data;
  int step = im->s, nch = im->c;
  int color = nch > 1;
  GIFD_VAR GIFDVar = fmt->GIFDVar;
  LPBYTE lpDIBBits = (LPBYTE) data;
  int wWidthBytes = step;
  PaletteEntry* m_palette = fmt->m_palette;
  unsigned char gray_palette[ 256 ];
  int i, m_bpp = fmt->m_bpp;
  int m_width = im->w;
  int m_height = im->h;

  // ����
  // ��ȡ�������ݳ���
  GIFDVar.dwDataLen = (DWORD)(xfsize(m_strm) - GetPos());

  // �����ڴ��С����󲻳���MAX_BUFF_SIZE��
  GIFDVar.wMemLen = ((GIFDVar.dwDataLen > (DWORD) MAX_BUFF_SIZE) ?
      (WORD) MAX_BUFF_SIZE : (WORD) GIFDVar.dwDataLen);

  // �����ڴ�
  GIFDVar.lpDataBuff = MALLOC(BYTE, GIFDVar.wMemLen);

  // ��ȡ��������
  ReadSrcData(m_strm, &GIFDVar.wMemLen, &GIFDVar.dwDataLen,
      GIFDVar.lpDataBuff, &GIFDVar.bEOF);

  // ��������ʼλ��
  GIFDVar.lpBgnBuff = GIFDVar.lpDataBuff;

  // ��������ֹλ��
  GIFDVar.lpEndBuff = GIFDVar.lpBgnBuff + GIFDVar.wMemLen;

  if (color) {
    lpDIBBits = MALLOC(BYTE, m_width * m_height);
    wWidthBytes = m_width;
  }

  // ����
  DecodeGIF_LZW(m_strm, lpDIBBits, &GIFDVar, wWidthBytes);

  if (!color) {
    if (m_bpp <= 8) {
      IMTRANS1D((1 << m_bpp), (unsigned char*) m_palette, 4, gray_palette, 1, BGRA2GRAY);
    }
  }

  for (i = 0; i < m_height; ++i) {
    unsigned char* data1 = data + i * step;
    unsigned char* src = lpDIBBits + i * wWidthBytes;

    if (color) {
      FillColorRow8(data1, src, m_width, m_palette);
    }
    else {
      FillGrayRow8(data1, src, m_width, gray_palette);
    }
  }

  // �ͷ��ڴ�
  SAFEFREE(GIFDVar.lpDataBuff);

  if (lpDIBBits != data) {
    SAFEFREE(lpDIBBits);
  }

  // ����DIB���
  return TRUE;
}

int fmtgif_checksign(ihand_t m_strm)
{
  int re = 0;
  int m_sign_len = 6;
  char psign[10];

  if (psign) {
    re = getsign(m_strm, psign, m_sign_len);

    if (re) {
      // �ж��Ƿ���GIF�ļ�
      if (memicmp(psign, "GIF", 3) != 0) {
        // ��GIF�ļ�������NULL
        re = FALSE;
      }

      // �жϰ汾���Ƿ���ȷ
      if ((memicmp(psign + 3, "87a", 3) != 0) &&
          (memicmp(psign + 3, "89a", 3) != 0)) {
        // ��֧�ָð汾������NULL
        re = FALSE;
      }
    }
  }

  return re;
}

// PNG Filter Factory
void fmtgif_init(fmtgif* fmt)
{
  //reader->m_sign_len = SIGN_LEN;
  //reader->m_signature = "";
  //reader->m_description = _T( "Portable image format (*.gif)" );
  fmt->m_offset = -1;
  fmt->m_bpp = 0;
  fmt->m_binary = fmt->m_maxval = 0;
}

int fmtgif_read_header(ihand_t m_strm, img_t* im, fmtgif* fmt)
{
  int result = FALSE;
  GIFHEADER GIFH; // GIF�ļ�ͷ
  GIFSCRDESC GIFS; // GIF�߼���Ļ������
  GIFIMAGE GIFI; // GIFͼ��������
  GIFCONTROL GIFC; // GIFͼ����������
  GIFPLAINTEXT GIFP; // GIFͼ��˵�������
  GIFAPPLICATION GIFA; // GIFӦ�ó��������
  GIFD_VAR GIFDVar; // GIF�������
  WORD wColors; // ��ɫ��Ŀ
  BYTE byGIF_Pal[ 768 ]; // ��ɫ��

  // 16ɫϵͳ��ɫ��
  BYTE bySysPal16[ 48 ] = {   //
    0, 0, 0, 0, 0, 128,
    0, 128, 0, 0, 128, 128,
    128, 0, 0, 128, 0, 128,
    128, 128, 0, 128, 128, 128,
    192, 192, 192, 0, 0, 255,
    0, 255, 0, 0, 255, 255,
    255, 0, 0, 255, 0, 255,
    255, 255, 0, 255, 255, 255
  };

  WORD wPalSize; // ��ɫ���С(�ֽ���)
  BYTE byTemp; // �ֽڱ���
  WORD wTemp; // �ֱ���
  WORD wi; // ѭ������
  BYTE byLabel; // ��ǩ
  BYTE byBlockSize; // ���С
  LPBYTE lpTemp; // �ڴ�ָ��

  if (!m_strm) {
    return FALSE;
  }

  if (!fmtgif_checksign(m_strm)) {
    return 0;
  }

  fmtgif_init(fmt);

  //if( setjmp( m_strm.JmpBuf()) == 0 )

  SetPos(0);
  // ��ȡGIF�ļ�ͷ
  READ_NBYTE((LPBYTE) & GIFH, sizeof(GIFH));

  // ��ȡGIF�߼���Ļ������
  READ_NBYTE((LPBYTE) & GIFS, 7);

  // ��ȡ��ɫ���λ��
  GIFDVar.wBits = (WORD) GIFS.GlobalFlag.PalBits + 1;

  // �ж��Ƿ���ȫ�ֵ�ɫ��
  if (GIFS.GlobalFlag.GlobalPal) {
    // ����ֵ
    memset((LPBYTE) byGIF_Pal, 0, 768);
    // ȫ�ֵ�ɫ���С
    wPalSize = 3 * (1 << GIFDVar.wBits);
    // ��ȡȫ�ֵ�ɫ��
    READ_NBYTE((LPBYTE) byGIF_Pal, wPalSize);
  }

  // ��ȡ��һ���ֽ�
  READ_NBYTE((LPBYTE) & byTemp, 1);

  // ��ÿһ��������ѭ��
  while (TRUE) {
    // �ж��Ƿ���ͼ��������
    if (byTemp == 0x2C) {
      // ��ͼ�������飬�˳�ѭ��
      break;
    }

    // �ж��Ƿ���GIF��չ��
    if (byTemp == 0x21) {
      // ��GIF��չ��
      // �����ڴ�
      lpTemp = MALLOC(BYTE, (DWORD) MAX_BUFF_SIZE);
      // ��ȡ��һ���ֽ�
      READ_NBYTE((LPBYTE) & byLabel, 1);

      // ��Ը�������飬���зֱ���
      switch (byLabel) {
      case 0xF9: {
        // ͼ����������
        READ_NBYTE((LPBYTE) & GIFC, 6);
        // ����
        break;
      }

      case 0x01: {
        // ͼ��˵�������
        READ_NBYTE((LPBYTE) & GIFP, sizeof(GIFP));
        // ��ȡ������С
        READ_NBYTE((LPBYTE) & byBlockSize, 1);

        // ��byBlockSize > 0ʱѭ����ȡ
        while (byBlockSize) {
          // ��ȡͼ��˵������飨����û�н����κδ���
          READ_NBYTE(lpTemp, byBlockSize);
          // ��ȡ������С
          READ_NBYTE((LPBYTE) & byBlockSize, 1);
        }

        // ����
        break;
      }

      case 0xFE: {
        // ע��˵�������
        // ��ȡ������С
        READ_NBYTE((LPBYTE) & byBlockSize, 1);

        // ��byBlockSize > 0ʱѭ����ȡ
        while (byBlockSize) {
          // ��ȡע��˵������飨����û�н����κδ���
          READ_NBYTE(lpTemp, byBlockSize);
          // ��ȡ������С
          READ_NBYTE((LPBYTE) & byBlockSize, 1);
        }

        // ����
        break;
      }

      case 0xFF: {
        // Ӧ�ó��������
        READ_NBYTE((LPBYTE) & GIFA, sizeof(GIFA));
        // ��ȡ������С
        READ_NBYTE((LPBYTE) & byBlockSize, 1);

        // ��byBlockSize > 0ʱѭ����ȡ
        while (byBlockSize) {
          // ��ȡӦ�ó�������飨����û�н����κδ���
          READ_NBYTE(lpTemp, byBlockSize);

          // ��ȡ������С
          READ_NBYTE((LPBYTE) & byBlockSize, 1);
        }

        // ����
        break;
      }

      default: {
        // ����δ֪�������

        // ��ȡ������С
        READ_NBYTE((LPBYTE) & byBlockSize, 1);

        // ��byBlockSize > 0ʱѭ����ȡ
        while (byBlockSize) {
          // ��ȡδ֪������飨����û�н����κδ���
          READ_NBYTE(lpTemp, byBlockSize);

          // ��ȡ������С
          READ_NBYTE((LPBYTE) & byBlockSize, 1);
        }

        // ����
        break;
      }

      // �ͷ��ڴ�
      SAFEFREE(lpTemp);
      }
    }

    // ��ȡ��һ���ֽ�
    READ_NBYTE((LPBYTE) & byTemp, 1);
  }

  // ��ȡGIFͼ��������
  READ_NBYTE((LPBYTE) & GIFI, 9);
  GIFDVar.wWidth = GIFI.wWidth; // ��ȡͼ����
  GIFDVar.wDepth = GIFI.wDepth; // ��ȡͼ��߶�

  // �ж��Ƿ��������ɫ��
  if (GIFI.LocalFlag.LocalPal) {
    // ����ֵ
    memset((LPBYTE) byGIF_Pal, 0, 768);

    // ��ȡ�����ɫ��λ��
    GIFDVar.wBits = (WORD) GIFI.LocalFlag.PalBits + 1;

    // �����ɫ���С
    wPalSize = 3 * (1 << GIFDVar.wBits);

    // ��ȡ�����ɫ��
    READ_NBYTE((LPBYTE) byGIF_Pal, wPalSize);
  }

  // ��GIFDVar��Ա��ֵ
  GIFDVar.wBits = ((GIFDVar.wBits == 1) ? 1 : (GIFDVar.wBits <= 4) ? 4 : 8);

  GIFDVar.wLineBytes = (WORD) BYTE_WBYTES((DWORD) GIFDVar.wWidth *
      (DWORD) GIFDVar.wBits);
  GIFDVar.bEOF = FALSE;

  // ����ʽ
  GIFDVar.bInterlace = (GIFI.LocalFlag.Interlace ? TRUE : FALSE);

  // ��ɫ��Ŀ
  wColors = 1 << GIFDVar.wBits;

  // ��ɫ���С
  wPalSize = wColors * sizeof(PaletteEntry);

  ////////////////////////////////////////////////////////////////////////////////
  // ����BITMAPINFOHEADER

#if 0
  // ��lpBIH��Ա��ֵ
  lpBIH->biSize = (DWORD) sizeof(BITMAPINFOHEADER);
  lpBIH->biWidth = (DWORD) GIFDVar.wWidth;
  lpBIH->biHeight = (DWORD) GIFDVar.wDepth;
  lpBIH->biPlanes = 1;
  lpBIH->biBitCount = GIFDVar.wBits;
  lpBIH->biCompression = BI_RGB;
  lpBIH->biSizeImage = (DWORD) wWidthBytes * GIFDVar.wDepth;
  lpBIH->biXPelsPerMeter = 0;
  lpBIH->biYPelsPerMeter = 0;
  lpBIH->biClrUsed = wColors;
  lpBIH->biClrImportant = 0;
#endif

  im->w = GIFDVar.wWidth;
  im->h = GIFDVar.wDepth;
  im->c = 3;
  fmt->m_bpp = (wColors >> 8) * 8;

  ////////////////////////////////////////////////////////////////////////////////
  // ���õ�ɫ��

  // �ж��Ƿ�ָ��ȫ�ֻ������ɫ��
  if (GIFS.GlobalFlag.GlobalPal || GIFI.LocalFlag.LocalPal) {
    wTemp = 0;

    for (wi = 0; wi < wColors; wi++) {
      fmt->m_palette[ wi ].r = byGIF_Pal[ wTemp++ ];
      fmt->m_palette[ wi ].g = byGIF_Pal[ wTemp++ ];
      fmt->m_palette[ wi ].b = byGIF_Pal[ wTemp++ ];
      fmt->m_palette[ wi ].a = 0x00;
    }
  }
  else {
    // û��ָ��ȫ�ֺ������ɫ�壬����ϵͳ��ɫ��

    // ������ɫ��Ŀ���ֱ����ɫ�帳ֵ
    switch (wColors) {
    case 2: {
      // ��ɫλͼ
      fmt->m_palette[ 0 ].r = 0x00;
      fmt->m_palette[ 0 ].g = 0x00;
      fmt->m_palette[ 0 ].b = 0x00;
      fmt->m_palette[ 0 ].a = 0x00;
      fmt->m_palette[ 1 ].r = 0xFF;
      fmt->m_palette[ 1 ].g = 0xFF;
      fmt->m_palette[ 1 ].b = 0xFF;
      fmt->m_palette[ 1 ].a = 0x00;

      // ����
      break;
    }

    case 16: {
      // 16ɫλͼ

      wTemp = 0;

      for (wi = 0; wi < wColors; wi++) {
        fmt->m_palette[ wi ].r = bySysPal16[ wTemp++ ];
        fmt->m_palette[ wi ].g = bySysPal16[ wTemp++ ];
        fmt->m_palette[ wi ].b = bySysPal16[ wTemp++ ];
        fmt->m_palette[ wi ].a = 0x00;
      }

      // ����
      break;
    }

    case 256: {
      // 256ɫλͼ
      for (wi = 0; wi < wColors; wi++) {
        fmt->m_palette[ wi ].r = (BYTE) wi;
        fmt->m_palette[ wi ].g = (BYTE) wi;
        fmt->m_palette[ wi ].b = (BYTE) wi;
        fmt->m_palette[ wi ].a = 0x00;
      }

      // ����
      break;
    }
    }
  }

  result = TRUE;
  fmt->GIFDVar = GIFDVar;


  if (!result) {
    fmt->m_offset = -1;
    im->w = im->h = -1;
  }

  return result;
}

#define fmtgif_free(x)  (0)

#endif // _FMTGIF_INL_
