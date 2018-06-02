/********************************************************************
 bmp.c
 ���ļ���;�� BMPͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ�Windows ��� ��1��32λλͼ��RLEλͼ
 OS/2 ��� �� 1��4��8��24λλͼ
 ���湦�ܣ�Windows ��� ��1��4��8��16(555)��24��
 32(888)λλͼ
 ���ļ���д�ˣ� YZ
 TearShark
 ����
 ���ļ��汾�� 30618
 ����޸��ڣ� 2003-06-18
 ----------------------------------------------------------------
 ������ʷ��
 2003-6 �޸�RLE4ͼ��λ��Ϊ4λ()
 2003-3 ֧��ͼ���д�����ṩ�����ػ���������
 2002-11 ����TearShark����һ��BUG��OS2���λͼ��ȷ�ϴ���
 ���ڵĴ��󣩡�
 2002-11 ȥ��������չ��"rle"����������չ�����ٱ�ʹ�á�
 2002-8 ����ӿ�������2.2��
 �����ڴ�������
 2001-7 ����RLE�����е�һ������
 1��4λͼ���������ISeeλ����ʽ
 ����Ŀ��ͼ�񻺳��������ֽ�
 ��ǿ���ݴ���
 2001-6 �����°����
 2001-5 ���¶���ģ��Ľӿڼ��������
 2000-10 ������һ����Ϊ�ش��BUG
 2000-7 ��һ�����԰淢��
********************************************************************/
/* �ļ��б��־ (windows - 'BM', OS/2 - 'BA')*/
#define DIB_HEADER_MARKER ((ushort)('M'<<8)|'B')
#define DIB_HEADER_MARKER_OS2 ((ushort)('A'<<8)|'B')
/* λͼ��ɫ����ṹ ��Windows ���*/
typedef struct _tag_rgb_quad {
  uchar rgbBlue;
  uchar rgbGreen;
  uchar rgbRed;
  uchar rgbReserved;
} RGB_QUAD, *LPRGB_QUAD;
/* λͼ��ɫ����ṹ ��OS/2 ���*/
typedef struct _tag_rgb_triple {
  uchar rgbtBlue;
  uchar rgbtGreen;
  uchar rgbtRed;
} RGB_TRIPLE, *LPRGB_TRIPLE;
/* λͼ��Ϣͷ�ṹ ��Windows ���*/
typedef struct _tag_bmp_info_header {
  ulong biSize;
  long biWidth;
  long biHeight;
  ushort biPlanes;
  ushort biBitCount;
  ulong biCompression;
  ulong biSizeImage;
  long biXPelsPerMeter;
  long biYPelsPerMeter;
  ulong biClrUsed;
  ulong biClrImportant;
} BMP_INFO_HEADER, *LPBMP_INFO_HEADER;
/* λͼ��Ϣͷ�ṹ ��OS/2 ���*/
typedef struct _tag_bmp_core_header {
  ulong bcSize;
  ushort bcWidth;
  ushort bcHeight;
  ushort bcPlanes;
  ushort bcBitCount;
} BMP_CORE_HEADER, *LPBMP_CORE_HEADER;
/* λͼ��Ϣ�ṹ ��Windows ��� */
typedef struct _tag_bmp_info {
  BMP_INFO_HEADER bmiHeader;
  RGB_QUAD bmiColors[1];
} BMP_INFO, *LPBMP_INFO;
/* λͼ��Ϣ�ṹ ��OS/2 ��� */
typedef struct _tag_bmp_core {
  BMP_CORE_HEADER bmciHeader;
  RGB_TRIPLE bmciColors[1];
} BMP_CORE, *LPBMP_CORE;
/* λͼ�ļ�ͷ�ṹ ��Windows ��OS/2 ���ã�*/
typedef struct _tag_bmp_file_header {
  ushort bfType;
  ulong bfSize;
  ushort bfReserved1;
  ushort bfReserved2;
  ulong bfOffBits;
} BMP_FILE_HEADER, *LPBMP_FILE_HEADER;
/*****************************************************************************/
/* �ڲ��������� */
/* ��ȡ����ɫ������ֵ */
int _bmp_get_mask(LPBMP_INFO pInfo, ulong* pb_mask, ulong* pg_mask, ulong* pr_mask, ulong* pa_mask)
{
  int result = 0;
  ulong* pMark;
  assert(pInfo->bmiHeader.biSize);
  switch (pInfo->bmiHeader.biBitCount) {
  case 1:
  case 4:
  case 8:
    /* ��ɫ��λͼ������ */
    *pr_mask = 0;
    *pg_mask = 0;
    *pb_mask = 0;
    *pa_mask = 0;
    break;
  case 16:
    /*******************************************
    16λ����ʾ��ͼ
    ���ֽ� ���ֽ�
    0000 0000 0000 0000
    1 1111--B 555��ʽ
    11 111 -------G
    111 11 --------------R
    0
    1 1111--B 565��ʽ
    111 111 -------G
    1111 1 --------------R
    * Win95 ϵͳֻ֧���������ָ�ʽ��NT֧�ֶ��ָ�ʽ��
    *******************************************/
    if (pInfo->bmiHeader.biCompression == 0) {
      *pr_mask = 0x7c00;
      *pg_mask = 0x3e0;
      *pb_mask = 0x1f;
      *pa_mask = 0;
      break;
    }
    else {
      pMark = (ulong*)((LPBMP_INFO_HEADER)pInfo + 1);
      *pr_mask = pMark[0];
      *pg_mask = pMark[1];
      *pb_mask = pMark[2];
      *pa_mask = 0;
      break;
    }
    break;
  case 24:
    *pr_mask = 0xff0000;
    *pg_mask = 0xff00;
    *pb_mask = 0xff;
    *pa_mask = 0x0;
    break;
  case 32:
    /******************************************************
    32λ����ʾ��ͼ
    �� -> ��
    0000 0000 0000 0000 0000 0000 0000 0000 888��ʽ
    1111 1111 ------------------------R
    1111 1111 -------------G
    1111 1111--B
    * Win95 ϵͳֻ֧����һ�ָ�ʽ
    ******************************************************/
    if (pInfo->bmiHeader.biCompression == 0) {
      *pr_mask = 0xff0000;
      *pg_mask = 0xff00;
      *pb_mask = 0xff;
      *pa_mask = 0x0;
    }
    else {
      pMark = (ulong*)((LPBMP_INFO_HEADER)pInfo + 1);
      *pr_mask = pMark[0];
      *pg_mask = pMark[1];
      *pb_mask = pMark[2];
      *pa_mask = 0;
    }
    break;
  default:
    result = 1;
    *pr_mask = 0;
    *pg_mask = 0;
    *pb_mask = 0;
    *pa_mask = 0;
    break;
  }
  return result; /* 0-�ɹ��� 1���Ƿ���ͼ���ʽ */
}
/* ��4λ������д����
 �β�mark���⣺0-��ʼ������ 1-д���� 2-�ƶ���ָ���� 3-�������� 4-��ȡ��ǰ��ַ
 ע����mark����2ʱ������num����Ϊ���س�2���������룬����iCurOff��ֵ
*/
uchar* _write2buff4RLE(int mark, int num, uchar pix, uchar* lpDesData)
{
  static uchar* lpDes; /* ����������ַ */
  static uchar* lpCur; /* ��������ǰд��ַ */
  static int iCurOff; /* ���ֽ��еĵ�ǰλ�ã�0-�߰��ֽ� 1-�Ͱ��ֽ� */
  int i;
  uchar pixa[2];
  switch (mark) {
  case 0: /* ��ʼ������ */
    lpDes = lpDesData;
    lpCur = lpDes;
    iCurOff = 0;
    break;
  case 1: /* д���� */
    pixa[0] = pix >> 4;
    pixa[1] = pix & 0xf;
    for (i = 0; i < num; i++) {
      if (iCurOff == 0) { /* ������ */
        *lpCur = pixa[i % 2] << 4;
        iCurOff = 1;
      }
      else { /* ������ */
        assert(iCurOff == 1);
        *lpCur |= pixa[i % 2];
        lpCur++;
        iCurOff = 0;
      }
    }
    break;
  case 2: /* �ƶ���ָ���� */
    lpCur = lpDesData;
    iCurOff = num;
    break;
  case 3: /* �������� */
    lpDes = (uchar*)0;
    lpCur = lpDes;
    iCurOff = 0;
    break;
  case 4: /* ��ȡ��ǰ��ַ */
    break;
  default:
    assert(0);
    break;
  }
  return lpCur;
}
/* ��8λ������д���أ�ֻ��_from_8_RLE_read()�����ڲ����ã�
 �β�mark���⣺0-��ʼ������ 1-д���� 2-�ƶ���ָ���� 3-�������� 4-��ȡ��ǰ��ַ
*/
uchar* _write2buff8RLE(int mark, int num, uchar pix, uchar* lpDesData)
{
  static uchar* lpDes; /* ����������ַ */
  static uchar* lpCur; /* ��������ǰд��ַ */
  int i;
  switch (mark) {
  case 0: /* ��ʼ������ */
    lpDes = lpDesData;
    lpCur = lpDes;
    break;
  case 1: /* д���� */
    for (i = 0; i < num; i++) {
      *lpCur++ = pix;
    }
    break;
  case 2: /* �ƶ���ָ���� */
    lpCur = lpDesData;
    break;
  case 3: /* �������� */
    lpDes = (uchar*)0;
    lpCur = lpDes;
    break;
  case 4: /* ��ȡ��ǰ��ַ */
    break;
  default:
    assert(0);
    break;
  }
  return lpCur;
}
/* �жϴ����ͼ���Ƿ���Ա����� */
int _bmp_is_valid_img(INFOSTR* pinfo_str)
{
  enum { bitcount = 1UL | (1UL<<(4-1)) | (1UL<<(8-1)) | (1UL<<(15-1)) | (1UL<<(16-1)) | (1UL<<(24-1)) | (1UL<<(32-1)) };
  /* ## 2001-8-9 YZ �޸ģ�λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1; /* ��֧�ֵ�λ��ͼ�� */
  }
  assert(pinfo_str->imgnumbers);
  if (pinfo_str->imgnumbers != 1) {
    return -2; /* ͼ���������ȷ */
  }
  return 0;
}
/* ��ȡͼ����Ϣ */
EXERESULT bmp_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  BMP_FILE_HEADER bfh;
  LPBMP_INFO pdibinfo = (LPBMP_INFO)0;
  ulong dib_info_len = 0;
  int style; /* λͼ���0��WINDOWS��1��OS/2 */
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */
  do {
    /* ��ȡ�ļ�ͷ�ṹ */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    if (isio_read((void*)&bfh, sizeof(BMP_FILE_HEADER), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* �ж��Ƿ�����Ч��λͼ�ļ� */
    if ((bfh.bfType != DIB_HEADER_MARKER) && (bfh.bfType != DIB_HEADER_MARKER_OS2)) {
      b_status = ER_NONIMAGE;
      break;
    }
    /* ��Ϣͷ����ɫ������OS/2��� */
    dib_info_len = bfh.bfOffBits - sizeof(BMP_FILE_HEADER);
    if ((pdibinfo = (LPBMP_INFO)isirw_malloc(dib_info_len)) == (LPBMP_INFO)0) {
      b_status = ER_MEMORYERR;
      break;
    }
    if (isio_seek(pfile, sizeof(BMP_FILE_HEADER), SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* ��ȡͼ����Ϣ�ṹ */
    if (isio_read((void*)pdibinfo, dib_info_len, 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* ȷ����windows���λͼ����OS/2���λͼ */
    if (pdibinfo->bmiHeader.biSize == sizeof(BMP_CORE_HEADER)) {
      style = 1; /* OS/2 */
    }
    else {
      style = 0; /* windows */
    }
    /* ������Ч�Լ��� */
    if (style == 0) {
      if ((pdibinfo->bmiHeader.biHeight == 0) || (pdibinfo->bmiHeader.biWidth == 0)) {
        b_status = ER_NONIMAGE;
        break;
      }
    }
    else {
      assert(style == 1);
      if ((((LPBMP_CORE)pdibinfo)->bmciHeader.bcHeight == 0) || (((LPBMP_CORE)pdibinfo)->bmciHeader.bcWidth == 0)) {
        b_status = ER_NONIMAGE;
        break;
      }
    }
    pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
    pinfo_str->imgformat = IMF_BMP; /* ͼ���ļ���ʽ����׺���� */
    if (style == 0) {
      switch (pdibinfo->bmiHeader.biCompression) { /* ͼ���ѹ����ʽ */
      case 0:
        pinfo_str->compression = ICS_RGB;
        break;
      case 1:
        pinfo_str->compression = ICS_RLE8;
        break;
      case 2:
        pinfo_str->compression = ICS_RLE4;
        break;
      case 3:
        pinfo_str->compression = ICS_BITFIELDS;
        break;
      default:
        b_status = ER_NONIMAGE;
        break;
      }
    }
    else {
      pinfo_str->compression = ICS_RGB; /* OS/2 ��ѹ��λͼ��ʽ */
    }
    /* ��дͼ����Ϣ */
    if (style == 0) {
      pinfo_str->width = pdibinfo->bmiHeader.biWidth;
      pinfo_str->height = (ulong)abs(pdibinfo->bmiHeader.biHeight);
      pinfo_str->order = (pdibinfo->bmiHeader.biHeight < 0) ? 0 : 1;
      /* ��Щ��ɫRLE4��ͼbitcount�����ó�1����Windows 3.0��logo��������һ���ݴ��� */
      if (pinfo_str->compression == ICS_RLE4) {
        pinfo_str->bitcount = 4;
      }
      else if (pinfo_str->compression == ICS_RLE8) {
        pinfo_str->bitcount = 8;
      }
      else {
        pinfo_str->bitcount = pdibinfo->bmiHeader.biBitCount;
      }
      if (_bmp_get_mask(pdibinfo,
          (&pinfo_str->b_mask),
          (&pinfo_str->g_mask),
          (&pinfo_str->r_mask),
          (&pinfo_str->a_mask)) == 1) {
        b_status = ER_NONIMAGE;
        break;
      }
    }
    else {
      pinfo_str->width = (ulong)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcWidth);
      pinfo_str->height = (ulong)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcHeight);
      pinfo_str->order = 1; /* OS/2λͼ���ǵ���� */
      pinfo_str->bitcount = (ulong)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcBitCount);
      /* OS/2 λͼ����Чλ��ֻ�У�1��4��8��24 */
      if (pinfo_str->bitcount == 24) {
        pinfo_str->b_mask = 0xff;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->a_mask = 0x0;
      }
      else {
        pinfo_str->b_mask = 0x0;
        pinfo_str->g_mask = 0x0;
        pinfo_str->r_mask = 0x0;
        pinfo_str->a_mask = 0x0;
      }
    }
    /* �趨���ݰ�״̬ */
    pinfo_str->data_state = 1;
  }
  while (0);
  if (pdibinfo) {
    isirw_free(pdibinfo);
  }
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT bmp_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  BMP_FILE_HEADER bfh;
  LPBMP_INFO pdibinfo = (LPBMP_INFO)0;
  ulong dib_info_len; /* DIB��Ϣ�ṹ�ߴ� */
  ulong dib_data_len; /* DIBλ���ݳߴ� */
  unsigned int pal_num; /* ��?�������?*/
  int i, linesize;
  int style; /* λͼ���0��WINDOWS��1��OS/2 */
  int bEOF = 0; /* ͼ���β��־ */
  int line = 0; /* ��ǰ�� */
  uchar pix, pix2; /* ��ǰֵ */
  uchar* pbit = (uchar*)0;
  uchar* prle = (uchar*)0;
  uchar* prlesou = (uchar*)0;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */
  do {
    /* ���ļ�ͷ�ṹ */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    if (isio_read((void*)&bfh, sizeof(BMP_FILE_HEADER), 1, pfile) == 0) {
      b_status = (pinfo_str->data_state == 1) ? ER_FILERWERR : ER_NONIMAGE;
      break;
    }
    /* �ж��Ƿ�����Ч��λͼ�ļ� */
    if ((bfh.bfType != DIB_HEADER_MARKER) && (bfh.bfType != DIB_HEADER_MARKER_OS2)) {
      b_status = ER_NONIMAGE;
      break;
    }
    /* ���� BMP_INFO �ĳߴ� */
    dib_info_len = bfh.bfOffBits - sizeof(BMP_FILE_HEADER);
    /* ������ BMP_INFO ��Ϣ���ڴ�� */
    if ((pdibinfo = (LPBMP_INFO)isirw_malloc(dib_info_len)) == (LPBMP_INFO)0) {
      b_status = ER_MEMORYERR;
      break;
    }
    /* ����Ϣͷ�ṹ */
    if (isio_seek(pfile, sizeof(BMP_FILE_HEADER), SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    if (isio_read((void*)pdibinfo, dib_info_len, 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* ȷ����windows���λͼ����OS/2���λͼ */
    if (pdibinfo->bmiHeader.biSize == sizeof(BMP_CORE_HEADER)) {
      style = 1; /* OS/2 */
    }
    else {
      style = 0; /* windows */
    }
    /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
    if (pinfo_str->data_state == 0) {
      /* ������Ч�Լ��� */
      if (style == 0) {
        if ((pdibinfo->bmiHeader.biHeight == 0) || (pdibinfo->bmiHeader.biWidth == 0)) {
          b_status = ER_NONIMAGE;
        }
      }
      else {
        assert(style == 1);
        if ((((LPBMP_CORE)pdibinfo)->bmciHeader.bcHeight == 0) || (((LPBMP_CORE)pdibinfo)->bmciHeader.bcWidth == 0)) {
          b_status = ER_NONIMAGE;
        }
      }
      __LEAVE;
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_BMP; /* ͼ���ļ���ʽ����׺���� */
      if (style == 0) {
        switch (pdibinfo->bmiHeader.biCompression) { /* ͼ���ѹ����ʽ */
        case 0:
          pinfo_str->compression = ICS_RGB;
          break;
        case 1:
          pinfo_str->compression = ICS_RLE8;
          break;
        case 2:
          pinfo_str->compression = ICS_RLE4;
          break;
        case 3:
          pinfo_str->compression = ICS_BITFIELDS;
          break;
        default:
          b_status = ER_NONIMAGE;
          break;
        }
      }
      else {
        pinfo_str->compression = ICS_RGB;
      }
      if (style == 0) {
        pinfo_str->width = pdibinfo->bmiHeader.biWidth;
        pinfo_str->height = (ulong)abs(pdibinfo->bmiHeader.biHeight);
        pinfo_str->order = (pdibinfo->bmiHeader.biHeight < 0) ? 0 : 1;
        if (pinfo_str->compression == ICS_RLE4) {
          pinfo_str->bitcount = 4;
        }
        else if (pinfo_str->compression == ICS_RLE8) {
          pinfo_str->bitcount = 8;
        }
        else {
          pinfo_str->bitcount = pdibinfo->bmiHeader.biBitCount;
        }
        if (_bmp_get_mask(pdibinfo,
            (&pinfo_str->b_mask),
            (&pinfo_str->g_mask),
            (&pinfo_str->r_mask),
            (&pinfo_str->a_mask)) == 1) {
          b_status = ER_NONIMAGE;
        }
        __LEAVE;
      }
      else {
        pinfo_str->width = (ulong)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcWidth);
        pinfo_str->height = (ulong)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcHeight);
        pinfo_str->order = 1; /* OS/2λͼ���ǵ���� */
        pinfo_str->bitcount = (ulong)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcBitCount);
        /* OS/2 λͼ����Чλ��ֻ�У�1��4��8��24 */
        if (pinfo_str->bitcount == 24) {
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0x0;
        }
        else {
          pinfo_str->b_mask = 0x0;
          pinfo_str->g_mask = 0x0;
          pinfo_str->r_mask = 0x0;
          pinfo_str->a_mask = 0x0;
        }
      }
      pinfo_str->data_state = 1;
    }
    /* ���õ�ɫ������ */
    if (pinfo_str->bitcount <= 8) {
      pinfo_str->pal_count = 1UL << pinfo_str->bitcount;
      if (style == 0) {
        pal_num = (pdibinfo->bmiHeader.biClrUsed == 0) ? pinfo_str->pal_count : pdibinfo->bmiHeader.biClrUsed;
        if (pal_num > pinfo_str->pal_count) {
          b_status = ER_NONIMAGE;
          break;
        }
        memmove((void*)(pinfo_str->palette), (const void*)(pdibinfo->bmiColors), sizeof(RGB_QUAD)*pal_num);
      }
      else {
        for (i = 0; i < (int)pinfo_str->pal_count; i++) {
          ((LPRGB_TRIPLE)(&pinfo_str->palette[i]))->rgbtBlue = ((LPBMP_CORE)pdibinfo)->bmciColors[i].rgbtBlue;
          ((LPRGB_TRIPLE)(&pinfo_str->palette[i]))->rgbtGreen = ((LPBMP_CORE)pdibinfo)->bmciColors[i].rgbtGreen;
          ((LPRGB_TRIPLE)(&pinfo_str->palette[i]))->rgbtRed = ((LPBMP_CORE)pdibinfo)->bmciColors[i].rgbtRed;
          pinfo_str->palette[i] &= 0xffffff; /* ����ֽ����� */
        }
      }
    }
    else {
      pinfo_str->pal_count = 0;
    }
    /* ȡ��λ���ݳ��� */
    dib_data_len = isio_length(pfile) - bfh.bfOffBits;
    /* ȡ��ɨ���гߴ� */
    linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
    /* ����ͼ����� */
    pinfo_str->imgnumbers = 1;
    pinfo_str->psubimg = (SUBIMGBLOCK*)0;
    assert(pinfo_str->p_bit_data == (uchar*)0);
    /* ����Ŀ��ͼ���ڴ�飨+4 �� β������4�ֽڻ������� */
    pinfo_str->p_bit_data = (uchar*)isirw_malloc(linesize * pinfo_str->height + 4);
    if (!pinfo_str->p_bit_data) {
      b_status = ER_MEMORYERR;
      break;
    }
    assert(pinfo_str->pp_line_addr == NULL);
    /* �������׵�ַ���� */
    pinfo_str->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * pinfo_str->height);
    if (!pinfo_str->pp_line_addr) {
      b_status = ER_MEMORYERR;
      break;
    }
    __LEAVE;
    /* �Ƿ���RLE */
    if ((pinfo_str->compression == ICS_RLE4) || (pinfo_str->compression == ICS_RLE8)) {
      assert(style == 0);
      /* �����ڴ棬���ڴ��RLEԭʼ���� */
      prlesou = prle = (uchar*)isirw_malloc(dib_data_len);
      if (!prle) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* ��ȡRLEԭʼ���� */
      if (isio_seek(pfile, bfh.bfOffBits, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)prle, dib_data_len, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ΪRLE��ѹ��׼������ʱ��Ϊдָ�����飩 */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
      }
      if (pinfo_str->compression == ICS_RLE4) {
        _write2buff4RLE(0, 0, 0, (uchar*)pinfo_str->pp_line_addr[line]); /* ��ʼ��д���� */
        /* �Ƚ�RLE��?��ݽ�ѹ�ɱ�׼��BMP���� */
        while (!bEOF) {
          pix = *prle++;
          if (pix == 0) { /* ���Է�ʽ */
            pix2 = *prle++;
            switch (pix2) {
            case 0: /* �н�β */
              line++;
              _write2buff4RLE(2, 0, 0, (uchar*)pinfo_str->pp_line_addr[line]); /* ָ���µ�ƫ�Ƶ�ַ */
              break;
            case 1: /* ͼ���β */
              bEOF = 1;
              break;
            case 2: /* ��ǰ��ַƫ�� */
              pix = *prle++;
              pix2 = *prle++;
              line += pix2; /* ����pix2�� */
              _write2buff4RLE(2, pix % 2, 0, (uchar*)((uchar*)pinfo_str->pp_line_addr[line] + (ulong)pix / 2));
              break;
            default: /* ����ѹ�������� */
              for (i = 0; i < pix2 / 2; i++) {
                pix = *(prle + i);
                _write2buff4RLE(1, 2, pix, (uchar*)0);
              }
              if (pix2 % 2) {
                /* ������©�ĵ������� */
                pix = *(prle + i);
                _write2buff4RLE(1, 1, pix, (uchar*)0);
              }
              /* (pix2+1)/2�ǽ����ض��뵽�ֽڱ߽��ϣ��ټ�1����2�Ƕ��뵽�ֱ߽��� */
              prle = (uchar*)(prle + ((pix2 + 1) / 2 + 1) / 2 * 2); /* �ֶ��� */
              break;
            }
          }
          else { /* ���뷽ʽ */
            /* pix == ��ͬ���صĸ��� */
            pix2 = *prle++; /* pix2 == ���� */
            _write2buff4RLE(1, (int)pix, pix2, (uchar*)0);
          }
        }
        _write2buff4RLE(3, 0, 0, (uchar*)0); /* ����д���� */
      }
      else {
        _write2buff8RLE(0, 0, 0, (uchar*)pinfo_str->pp_line_addr[line]); /* ��ʼ��д���� */
        while (!bEOF) {
          pix = *prle++;
          if (pix == 0) { /* ���Է�ʽ */
            pix2 = *prle++;
            switch (pix2) {
            case 0: /* �н�β */
              line++;
              _write2buff8RLE(2, 0, 0, (uchar*)pinfo_str->pp_line_addr[line]); /* ָ���µ�ƫ�Ƶ�ַ */
              break;
            case 1: /* ͼ���β */
              bEOF = 1;
              break;
            case 2: /* ��ǰ��ַƫ�� */
              pix = *prle++;
              pix2 = *prle++;
              line += pix2; /* ����pix2�� */
              _write2buff8RLE(2, 0, 0, (uchar*)((uchar*)pinfo_str->pp_line_addr[line] + (ulong)pix));
              break;
            default: /* ����ѹ�������� */
              for (i = 0; i < pix2; i++) {
                pix = *(prle + i);
                _write2buff8RLE(1, 1, pix, (uchar*)0);
              }
              /* (pix2+1)/2�ǽ����ض��뵽�ֽڱ߽��ϣ��ټ�1����2�Ƕ��뵽�ֱ߽��� */
              prle = (uchar*)(prle + ((pix2 + 1) / 2) * 2); /* �ֶ��� */
              break;
            }
          }
          else { /* ���뷽ʽ */
            /* pix == ��ͬ���صĸ��� */
            pix2 = *prle++; /* pix2 == ���� */
            _write2buff8RLE(1, (int)pix, pix2, (uchar*)0);
          }
        }
        _write2buff8RLE(3, 0, 0, (uchar*)0); /* ����д���� */
      }
      /* ��ʼ�����׵�ַ��RLEλͼ���ǵ���ģ� */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
      }
    }
    else {
      if (pinfo_str->order == 0) { /* ���� */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
        }
      }
      else { /* ���� */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
        }
      }
      /* ��ȡλ���ݵ��ڴ� */
      if (isio_seek(pfile, bfh.bfOffBits, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      pbit = pinfo_str->p_bit_data;
      /* ���ж��� */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        if (isio_read((void*)pbit, linesize, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        pbit = (uchar*)(pbit + linesize);
      }
      __LEAVE;
    }
    pinfo_str->data_state = 2;
  }
  while (0);
  if (prlesou) {
    isirw_free(prlesou);
  }
  if (pdibinfo) {
    isirw_free(pdibinfo);
  }
  if (b_status != ER_SUCCESS) {
    if (pinfo_str->p_bit_data) {
      isirw_free(pinfo_str->p_bit_data);
      pinfo_str->p_bit_data = (uchar*)0;
    }
    if (pinfo_str->pp_line_addr) {
      isirw_free(pinfo_str->pp_line_addr);
      pinfo_str->pp_line_addr = (uchar**)0;
    }
    if (pinfo_str->data_state == 2) {
      pinfo_str->data_state = 1; /* �Զ����� */
    }
  }
  else {
    if (b_status != ER_SUCCESS) {
      if (pinfo_str->p_bit_data) {
        isirw_free(pinfo_str->p_bit_data);
        pinfo_str->p_bit_data = (uchar*)0;
      }
      if (pinfo_str->pp_line_addr) {
        isirw_free(pinfo_str->pp_line_addr);
        pinfo_str->pp_line_addr = (uchar**)0;
      }
      if (pinfo_str->data_state == 2) {
        pinfo_str->data_state = 1;
      }
    }
  }
  return b_status;
}
/* ����ͼ�� ��ֻ�ܱ���Windows����λͼ��*/
EXERESULT bmp_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  BMP_FILE_HEADER bfh;
  LPBMP_INFO pdibinfo = (LPBMP_INFO)0;
  ulong dib_info_len; /* DIB��Ϣ�ṹ�ߴ� */
  uchar* plinebuf = (uchar*)0;
  int i, linesize;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);
  do {
    /* �ж��Ƿ�����֧�ֵ�ͼ������ */
    if (_bmp_is_valid_img(pinfo_str) != 0) {
      b_status = ER_NSIMGFOR;
      break;
    }
    /* ����λͼ��Ϣͷ�ṹ�ĳߴ磨������ɫ�壩 */
    switch (pinfo_str->bitcount) {
    case 1:
    case 4:
    case 8:
      dib_info_len = sizeof(BMP_INFO_HEADER) + pinfo_str->pal_count * sizeof(RGB_QUAD);
      break;
    case 24:
      dib_info_len = sizeof(BMP_INFO_HEADER);
      break;
    case 16:
    case 32:
      dib_info_len = sizeof(BMP_INFO_HEADER) + 3 * sizeof(ulong);
      break;
    default:
      assert(0);
      b_status = ER_SYSERR;
      break;
    }
    /* ������ BMP_INFO ��Ϣ���ڴ�� */
    if ((pdibinfo = (LPBMP_INFO)isirw_malloc(dib_info_len)) == (LPBMP_INFO)0) {
      b_status = ER_MEMORYERR;
      break;
    }
    pdibinfo->bmiHeader.biSize = sizeof(BMP_INFO_HEADER);
    pdibinfo->bmiHeader.biPlanes = 1;
    pdibinfo->bmiHeader.biBitCount = (ushort)pinfo_str->bitcount;
    pdibinfo->bmiHeader.biCompression = ((pinfo_str->bitcount == 16) || (pinfo_str->bitcount == 32)) ? 3 : 0;
    pdibinfo->bmiHeader.biWidth = pinfo_str->width;
    /* ȫ������Ϊ����ͼ */
    pdibinfo->bmiHeader.biHeight = (long)pinfo_str->height;
    pdibinfo->bmiHeader.biSizeImage = 0;
    pdibinfo->bmiHeader.biClrImportant = 0;
    pdibinfo->bmiHeader.biClrUsed = 0;
    pdibinfo->bmiHeader.biXPelsPerMeter = 0;
    pdibinfo->bmiHeader.biYPelsPerMeter = 0;
    if (pdibinfo->bmiHeader.biCompression == 0) {
      memmove((void*)pdibinfo->bmiColors, (const void*)pinfo_str->palette, pinfo_str->pal_count * sizeof(RGB_QUAD));
    }
    else {
      *((ulong*) & (pdibinfo->bmiColors[0])) = pinfo_str->r_mask;
      *((ulong*) & (pdibinfo->bmiColors[1])) = pinfo_str->g_mask;
      *((ulong*) & (pdibinfo->bmiColors[2])) = pinfo_str->b_mask;
    }
    /* ��дλͼ�ļ�ͷ�ṹ */
    bfh.bfSize = sizeof(BMP_FILE_HEADER);
    bfh.bfType = DIB_HEADER_MARKER;
    bfh.bfOffBits = bfh.bfSize + dib_info_len;
    bfh.bfReserved1 = bfh.bfReserved2 = 0;
    /* ȡ��ɨ���гߴ� */
    linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
    /* ����д���ݻ����� */
    if ((plinebuf = (uchar*)isirw_malloc(linesize)) == 0) {
      b_status = ER_MEMORYERR;
      break;
    }
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* д���ļ�ͷ�ṹ */
    if (isio_write((const void*)&bfh, sizeof(BMP_FILE_HEADER), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* д����Ϣͷ�ṹ */
    if (isio_write((const void*)pdibinfo, dib_info_len, 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* д��ͼ��λ����
    ע�������ͼ�����ݱ�����ISeeλ����ʽ��������ܻ����ʧ��
    */
    for (i = 0; i < (int)(pinfo_str->height); i++) {
      memcpy((void*)plinebuf, (const void*)(uchar*) \
          (pinfo_str->pp_line_addr[pinfo_str->height - 1 - i]), linesize);
      /* һ�ɱ���Ϊ����ͼ */
      if (isio_write((const void*)plinebuf, linesize, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
    }
  }
  while (0);
  if (plinebuf) {
    isirw_free(plinebuf);
  }
  if (pdibinfo) {
    isirw_free(pdibinfo);
  }
  return b_status;
}

