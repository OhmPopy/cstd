
#ifndef SUPPORT_PCX
#define SUPPORT_PCX

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include "_fmtimg.h"

// P?M filter factory

typedef struct fmtpcx {
  int m_bpp;
  int m_offset;
  int m_binary;
  int m_maxval;
  PaletteEntry m_palette[ 256 ];
}
fmtpcx;

int fmtpcx_checksign(ihand_t m_strm)
{
  return 0;
}

int fmtpcx_free(fmtpcx* reader)
{
  return 0;
}

int fmtpcx_read_header(ihand_t m_strm, img_t* im, fmtpcx* fmt)
{
  int result = FALSE;
  int m_sign_len = 0;
  const char* m_signature = "";
  const char* m_description = _T("Portable image format (*.pcx)");
  fmt->m_offset = -1;
  fmt->m_bpp = 0;
  fmt->m_binary = fmt->m_maxval = 0;

  if (!m_strm) {
    return FALSE;
  }

  if (!result) {
    fmt->m_offset = -1;
    im->w = im->h = -1;
  }

  return result;
}

int fmtpcx_read_data(ihand_t m_strm, img_t* im, fmtpcx* fmt)
{
  return 0;
}

/*************************************************************************
* ��������:
*     PCXCoding()
* ����:
*   uchar* lpDIBBits  - ָ��DIB�������ص�ָ��
*   int lWidth   - Դͼ���ȣ���������������4�ı�����
*   int lHeight  - Դͼ��߶ȣ���������
*   ihand_t file         - Ҫ������ļ�
* ����ֵ:
*   int               - �ɹ�����TRUE�����򷵻�False��
* ˵��:
*   �ú�����ָ����256ɫDIB���󱣴�Ϊ256ɫPCX�ļ���
 *************************************************************************/
int PCXCoding(int lHeight, int lWidth, uchar* lpDIBBits, int lLineBytes, ihand_t m_strm)
{
  typedef struct {
    char manufacturer;
    char version;
    char encoding;
    char bits_per_pixel;
    WORD xmin, ymin;
    WORD xmax, ymax;
    WORD hres;
    WORD vres;
    char palette[ 48 ];
    char reserved;
    char colour_planes;
    WORD bytes_per_line;
    WORD palette_type;
    char filler[ 58 ];
  }
  PCXHEAD;

  // ѭ������
  int i, j;
  // �м����
  uint8 bChar1, bChar2;

  // ָ��Դͼ�����ص�ָ��
  uchar* lpSrc;
  // ָ������ͼ�����ݵ�ָ��
  uchar* lpDst;
  // �ظ����ؼ���
  int iCount;
  // ��������ʹ�õ��ֽ���
  uint32 dwBuffUsed;
  // PCX�ļ�ͷ
  PCXHEAD pcxHeadr;
  // PCX��ʶ��
  pcxHeadr.manufacturer = 0x0A;
  // PCX�汾��
  pcxHeadr.version = 5;
  // PCX���뷽ʽ��1��ʾRLE���룩
  pcxHeadr.encoding = 1;
  // ����λ����256ɫΪ8λ��
  pcxHeadr.bits_per_pixel = 8;
  // ͼ���������Ļ�����Ͻ�X���꣨������Ϊ��λ��
  pcxHeadr.xmin = 0;
  // ͼ���������Ļ�����Ͻ�Y���꣨������Ϊ��λ��
  pcxHeadr.ymin = 0;
  // ͼ���������Ļ�����½�X���꣨������Ϊ��λ��
  pcxHeadr.xmax = (unsigned short)(lWidth - 1);
  // ͼ���������Ļ�����½�Y���꣨������Ϊ��λ��
  pcxHeadr.ymax = (unsigned short)(lHeight - 1);
  // ͼ���ˮƽ�ֱ���
  pcxHeadr.hres = (unsigned short) lWidth;
  // ͼ��Ĵ�ֱ�ֱ���
  pcxHeadr.vres = (unsigned short) lHeight;

  // ��ɫ�����ݣ�����256ɫPCX�����壬ֱ�Ӹ�ֵΪ0��
  for (i = 0; i < 48; i ++) {
    pcxHeadr.palette[ i ] = 0;
  }

  // �������趨Ϊ0��
  pcxHeadr.reserved = 0;
  // ͼ��ɫ��ƽ����Ŀ������256ɫPCX�趨Ϊ1����
  pcxHeadr.colour_planes = 1;
  // ͼ��Ŀ�ȣ��ֽ�Ϊ��λ��������Ϊż����
  pcxHeadr.bytes_per_line = (unsigned short) lWidth;
  // ͼ���ɫ������ͣ�1��ʾ��ɫ���ߵ�ɫͼ��2��ʾͼ���ǻҶ�ͼ��
  pcxHeadr.palette_type = 1;

  // ������ͼ�����Ļ��ȣ�����Ϊ��λ��
  //    pcxHeadr.wSrcWidth = 0;
  // ������ͼ�����Ļ�߶ȣ�����Ϊ��λ��
  //    pcxHeadr.wSrcDepth = 0;
  // ������ȡֵ�趨Ϊ0��
  for (i = 0; i < 54; i ++) {
    pcxHeadr.filler[ i ] = 0;
  }

  // д���ļ�ͷ
  WRITE_NBYTE((uchar*) & pcxHeadr, sizeof(PCXHEAD));
  // ��ʼ����

  // ����һƬ������(2��ԭʼͼ���С)�Ա��������
  lpDst = GMALLOC(uint8, lHeight * lWidth * 2);
  // ָ����ǰ�Ѿ����˶��ٻ��������ֽ�����
  dwBuffUsed = 0;

  // ÿ��
  for (i = 0; i < lHeight; i++) {
    // ָ��DIB��i�У���0�����ص�ָ��
    lpSrc = (uchar*) lpDIBBits + lLineBytes * (i);
    // ��bChar1��ֵ
    bChar1 = *lpSrc;
    // ����iCountΪ1
    iCount = 1;

    // ʣ����
    for (j = 1; j < lWidth; j ++) {
      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrc++;
      // ��ȡ��һ������
      bChar2 = *lpSrc;

      // �ж��Ƿ��bChar1��ͬ����iCount < 63
      if ((bChar1 == bChar2) && (iCount < 63)) {
        // ��ͬ��������1
        iCount ++;
        // ��������һ��
      }
      else {
        // ��ͬ������iCount = 63
        // д�뻺����
        if ((iCount > 1) || (bChar1 >= 0xC0)) {
          // �����볤��Ϣ
          lpDst[ dwBuffUsed ] = (uint8)(iCount | 0xC0);
          // ����bChar1
          lpDst[ dwBuffUsed + 1 ] = bChar1;
          // ����dwBuffUsed
          dwBuffUsed += 2;
        }
        else {
          // ֱ�ӱ����ֵ
          lpDst[ dwBuffUsed ] = bChar1;
          // ����dwBuffUsed
          dwBuffUsed ++;
        }

        // ���¸�bChar1��ֵ
        bChar1 = bChar2;
        // ����iCountΪ1
        iCount = 1;
      }
    }

    // ����ÿ�����һ���ֱ���
    if ((iCount > 1) || (bChar1 >= 0xC0)) {
      // �����볤��Ϣ
      lpDst[ dwBuffUsed ] = (uint8)(iCount | 0xC0);
      // ����bChar1
      lpDst[ dwBuffUsed + 1 ] = bChar1;
      // ����dwBuffUsed
      dwBuffUsed += 2;
    }
    else {
      // ֱ�ӱ����ֵ
      lpDst[ dwBuffUsed ] = bChar1;
      // ����dwBuffUsed
      dwBuffUsed ++;
    }
  }

  // д�������
  WRITE_NBYTE((uchar*) lpDst, dwBuffUsed);
  // �ͷ��ڴ�
  GFREE(lpDst) ;
  // ����һƬ�������Ա����ɫ��
  lpDst = GMALLOC(uint8, 769);
  // ��ɫ����ʼ�ֽ�
  * lpDst = 0x0C;

  for (i = 0; i < 256; i ++) {
    lpDst[ i * 3 + 1 ] = (uint8)(i);      // ��ȡDIB��ɫ���ɫ����
    lpDst[ i * 3 + 2 ] = (uint8)(i);      // ��ȡDIB��ɫ����ɫ����
    lpDst[ i * 3 + 3 ] = (uint8)(i);      // ��ȡDIB��ɫ����ɫ����
  }

  // д���ɫ����Ϣ
  WRITE_NBYTE((uchar*) lpDst, 769);
  // ����
  GFREE(lpDst);
  return TRUE;
}

int fmtpcx_write(ihand_t m_strm, const img_t* im, const PaletteEntry* palette)
{
  int height = im->h, width = im->w, step = im->s, channels = im->c;
  const unsigned char* data = im->tt.data;
  int isBinary = FALSE;
  int result = FALSE;

  int channels0 = channels > 1 ? 3 : 1;
  int fileStep = width * channels0;

  assert(data && width > 0 && height > 0 && step >= fileStep);

  if (m_strm) {}

  return result;
}


#endif // SUPPORT_PCX
