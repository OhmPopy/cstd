// BMPͼ���дģ��ʵ���ļ�
// ��ȡ���ܣ�Windows ��� ��1��32λλͼ��RLEλͼ
// OS/2 ��� �� 1��4��8��24λλͼ
// ���湦�ܣ�Windows ��� ��1��4��8��16(555)��24��32(888)λλͼ

/* �ļ��б��־ (windows - 'BM', OS/2 - 'BA')*/
#define DIB_HEADER_MARKER         ((unsigned short)('M'<<8)|'B')
#define DIB_HEADER_MARKER_OS2     ((unsigned short)('A'<<8)|'B')

/* λͼ��ɫ����ṹ ��Windows ���*/
typedef struct _tag_rgb_quad {
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbRed;
  unsigned char rgbReserved;
} RGB_QUAD, *LPRGB_QUAD;

/* λͼ��ɫ����ṹ ��OS/2 ���*/
typedef struct _tag_rgb_triple {
  unsigned char rgbtBlue;
  unsigned char rgbtGreen;
  unsigned char rgbtRed;
} RGB_TRIPLE, *LPRGB_TRIPLE;



/* λͼ��Ϣͷ�ṹ ��Windows ���*/
typedef struct _tag_bmp_info_header {
  unsigned long biSize;
  long      biWidth;
  long      biHeight;
  unsigned short  biPlanes;
  unsigned short  biBitCount;
  unsigned long biCompression;
  unsigned long biSizeImage;
  long      biXPelsPerMeter;
  long      biYPelsPerMeter;
  unsigned long biClrUsed;
  unsigned long biClrImportant;
} BMP_INFO_HEADER, *LPBMP_INFO_HEADER;

/* λͼ��Ϣͷ�ṹ ��OS/2 ���*/
typedef struct _tag_bmp_core_header {
  unsigned long bcSize;
  unsigned short  bcWidth;
  unsigned short  bcHeight;
  unsigned short  bcPlanes;
  unsigned short  bcBitCount;
} BMP_CORE_HEADER, *LPBMP_CORE_HEADER;



/* λͼ��Ϣ�ṹ ��Windows ��� */
typedef struct _tag_bmp_info {
  BMP_INFO_HEADER bmiHeader;
  RGB_QUAD    bmiColors[1];
} BMP_INFO, *LPBMP_INFO;

/* λͼ��Ϣ�ṹ ��OS/2 ��� */
typedef struct _tag_bmp_core {
  BMP_CORE_HEADER bmciHeader;
  RGB_TRIPLE      bmciColors[1];
} BMP_CORE, *LPBMP_CORE;



/* λͼ�ļ�ͷ�ṹ ��Windows ��OS/2 ���ã�*/
typedef struct _tag_bmp_file_header {
  unsigned short  bfType;
  unsigned long bfSize;
  unsigned short  bfReserved1;
  unsigned short  bfReserved2;
  unsigned long bfOffBits;
} BMP_FILE_HEADER, *LPBMP_FILE_HEADER;


/*****************************************************************************/
/* �ڲ��������� */

/* ��ȡ����ɫ������ֵ */
int CALLAGREEMENT _get_mask(LPBMP_INFO pInfo,
    unsigned long* pb_mask,
    unsigned long* pg_mask,
    unsigned long* pr_mask,
    unsigned long* pa_mask)
{
  int result = 0;
  unsigned long* pMark;

  assert(pInfo->bmiHeader.biSize);

  switch (pInfo->bmiHeader.biBitCount) {
  case  1:
  case  4:
  case  8:
    /* ��ɫ��λͼ������ */
    *pr_mask = 0;
    *pg_mask = 0;
    *pb_mask = 0;
    *pa_mask = 0;
    break;

  case  16:

    /*******************************************
        16λ����ʾ��ͼ

        ���ֽ�      ���ֽ�
      0000 0000   0000 0000
               1 1111--B  555��ʽ
           11 111 -------G
       111 11  --------------R
      0
               1 1111--B  565��ʽ
          111   111 -------G
      1111 1   --------------R

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
      pMark = (unsigned long*)((LPBMP_INFO_HEADER)pInfo + 1);

      *pr_mask = pMark[0];
      *pg_mask = pMark[1];
      *pb_mask = pMark[2];
      *pa_mask = 0;
      break;
    }

    break;

  case  24:
    *pr_mask  = 0xff0000;
    *pg_mask  = 0xff00;
    *pb_mask  = 0xff;
    *pa_mask  = 0x0;
    break;

  case  32:

    /******************************************************
            32λ����ʾ��ͼ
          ��               ->                 ��
    0000 0000   0000 0000   0000 0000   0000 0000   888��ʽ
                1111 1111  ------------------------R
                            1111 1111 -------------G
                                        1111 1111--B
    * Win95 ϵͳֻ֧����һ�ָ�ʽ
    ******************************************************/
    if (pInfo->bmiHeader.biCompression == 0) {
      *pr_mask  = 0xff0000;
      *pg_mask  = 0xff00;
      *pb_mask  = 0xff;
      *pa_mask  = 0x0;
    }
    else {
      pMark = (unsigned long*)((LPBMP_INFO_HEADER)pInfo + 1);

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

  return result;  /* 0-�ɹ��� 1���Ƿ���ͼ���ʽ */
}


/* ��4λ������д����
   �β�mark���⣺0-��ʼ������  1-д����  2-�ƶ���ָ����  3-��������  4-��ȡ��ǰ��ַ
   ע����mark����2ʱ������num����Ϊ���س�2���������룬����iCurOff��ֵ
*/
unsigned char* CALLAGREEMENT _write2buff4RLE(int mark, int num, unsigned char pix, unsigned char* lpDesData)
{
  static unsigned char* lpDes;    /* ����������ַ */
  static unsigned char* lpCur;    /* ��������ǰд��ַ */
  static int        iCurOff;    /* ���ֽ��еĵ�ǰλ�ã�0-�߰��ֽ�  1-�Ͱ��ֽ� */
  int i;
  unsigned char pixa[2];

  switch (mark) {
  case  0:    /* ��ʼ������ */
    lpDes = lpDesData;
    lpCur = lpDes;
    iCurOff = 0;
    break;

  case  1:    /* д���� */
    pixa[0] = pix >> 4;
    pixa[1] = pix & 0xf;

    for (i = 0; i < num; i++) {
      if (iCurOff == 0) { /* ������ */
        *lpCur = pixa[i % 2] << 4;
        iCurOff = 1;
      }
      else {      /* ������ */
        assert(iCurOff == 1);
        *lpCur |= pixa[i % 2];
        lpCur++;
        iCurOff = 0;
      }
    }

    break;

  case  2:    /* �ƶ���ָ���� */
    lpCur = lpDesData;
    iCurOff = num;
    break;

  case  3:    /* �������� */
    lpDes = (unsigned char*)0;
    lpCur = lpDes;
    iCurOff = 0;
    break;

  case  4:    /* ��ȡ��ǰ��ַ */
    break;

  default:
    assert(0);
    break;
  }

  return lpCur;
}



/* ��8λ������д���أ�ֻ��_from_8_RLE_read()�����ڲ����ã�
   �β�mark���⣺0-��ʼ������  1-д����  2-�ƶ���ָ����  3-��������  4-��ȡ��ǰ��ַ
*/
unsigned char* CALLAGREEMENT _write2buff8RLE(int mark, int num, unsigned char pix, unsigned char* lpDesData)
{
  static unsigned char* lpDes;    /* ����������ַ */
  static unsigned char* lpCur;    /* ��������ǰд��ַ */
  int i;

  switch (mark) {
  case  0:    /* ��ʼ������ */
    lpDes = lpDesData;
    lpCur = lpDes;
    break;

  case  1:    /* д���� */
    for (i = 0; i < num; i++) {
      *lpCur++ = pix;
    }

    break;

  case  2:    /* �ƶ���ָ���� */
    lpCur = lpDesData;
    break;

  case  3:    /* �������� */
    lpDes = (unsigned char*)0;
    lpCur = lpDes;
    break;

  case  4:    /* ��ȡ��ǰ��ַ */
    break;

  default:
    assert(0);
    break;
  }

  return lpCur;
}


/* ����ɨ���гߴ�(���ֽڶ���) */
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* �жϴ����ͼ���Ƿ���Ա����� */
int CALLAGREEMENT _bmp_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* ## 2001-8-9  YZ �޸ģ�λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(bmp_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* ��֧�ֵ�λ��ͼ�� */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (bmp_irwp_info.irwp_save.img_num)
    if (bmp_irwp_info.irwp_save.img_num == 1)
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* ͼ���������ȷ */
      }

  return 0;
}

/* ��ȡͼ��λ���� */
BMP_API int CALLAGREEMENT bmp_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;
  BMP_FILE_HEADER bfh;
  LPBMP_INFO    pdibinfo = (LPBMP_INFO)0;

  unsigned long dib_info_len;   /* DIB��Ϣ�ṹ�ߴ� */
  unsigned long dib_data_len;   /* DIBλ���ݳߴ� */
  unsigned int  pal_num;      /* ��ɫ������� */
  int       i, linesize;
  int       style;        /* λͼ���0��WINDOWS��1��OS/2 */

  int       bEOF = 0;     /* ͼ���β��־ */
  int       line = 0;     /* ��ǰ�� */
  unsigned char pix, pix2;      /* ��ǰֵ */

  unsigned char* pbit = (unsigned char*)0;
  unsigned char* prle = (unsigned char*)0;
  unsigned char* prlesou = (unsigned char*)0;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&bmp_load_img_critical);

      /* ����ϵͳ�ص����� */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_INTO_PLUG, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* ���� */
      if ((pfile = isio_open((const char*)psct, "rb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ���ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (isio_read((void*)&bfh, sizeof(BMP_FILE_HEADER), 1, pfile) == 0) {
        b_status = (pinfo_str->data_state == 1) ? ER_FILERWERR : ER_NONIMAGE;
        __leave;
      }

      /* �ж��Ƿ�����Ч��λͼ�ļ� */
      if ((bfh.bfType != DIB_HEADER_MARKER) && (bfh.bfType != DIB_HEADER_MARKER_OS2)) {
        b_status = ER_NONIMAGE;
        __leave;
      }

      /* ���� BMP_INFO �ĳߴ� */
      dib_info_len = bfh.bfOffBits - sizeof(BMP_FILE_HEADER);

      /* ������ BMP_INFO ��Ϣ���ڴ�� */
      if ((pdibinfo = (LPBMP_INFO)isirw_malloc(dib_info_len)) == (LPBMP_INFO)0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* ����Ϣͷ�ṹ */
      if (isio_seek(pfile, sizeof(BMP_FILE_HEADER), SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (isio_read((void*)pdibinfo, dib_info_len, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ȷ����windows���λͼ����OS/2���λͼ */
      if (pdibinfo->bmiHeader.biSize == sizeof(BMP_CORE_HEADER)) {
        style = 1;                /* OS/2 */
      }
      else {
        style = 0;                /* windows */
      }

      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        /* ������Ч�Լ��� */
        if (style == 0) {
          if ((pdibinfo->bmiHeader.biHeight == 0) || (pdibinfo->bmiHeader.biWidth == 0)) {
            b_status = ER_NONIMAGE;
            __leave;
          }
        }
        else {
          assert(style == 1);

          if ((((LPBMP_CORE)pdibinfo)->bmciHeader.bcHeight == 0) || (((LPBMP_CORE)pdibinfo)->bmciHeader.bcWidth == 0)) {
            b_status = ER_NONIMAGE;
            __leave;
          }
        }

        pinfo_str->imgtype    = IMT_RESSTATIC;  /* ͼ���ļ����� */
        pinfo_str->imgformat  = IMF_BMP;      /* ͼ���ļ���ʽ����׺���� */

        if (style == 0) {
          switch (pdibinfo->bmiHeader.biCompression) { /* ͼ���ѹ����ʽ */
          case  0:
            pinfo_str->compression = ICS_RGB;
            break;

          case  1:
            pinfo_str->compression = ICS_RLE8;
            break;

          case  2:
            pinfo_str->compression = ICS_RLE4;
            break;

          case  3:
            pinfo_str->compression = ICS_BITFIELDS;
            break;

          default:
            b_status = ER_NONIMAGE;
            __leave;
          }
        }
        else {
          pinfo_str->compression = ICS_RGB;
        }

        if (style == 0) {
          pinfo_str->width  = pdibinfo->bmiHeader.biWidth;
          pinfo_str->height = (unsigned long)abs(pdibinfo->bmiHeader.biHeight);
          pinfo_str->order  = (pdibinfo->bmiHeader.biHeight < 0) ? 0 : 1;

          if (pinfo_str->compression == ICS_RLE4) {
            pinfo_str->bitcount = 4;
          }
          else if (pinfo_str->compression == ICS_RLE8) {
            pinfo_str->bitcount = 8;
          }
          else {
            pinfo_str->bitcount = pdibinfo->bmiHeader.biBitCount;
          }

          if (_get_mask(pdibinfo,
              (unsigned long*)(&pinfo_str->b_mask),
              (unsigned long*)(&pinfo_str->g_mask),
              (unsigned long*)(&pinfo_str->r_mask),
              (unsigned long*)(&pinfo_str->a_mask)) == 1) {
            b_status = ER_NONIMAGE;
            __leave;
          }
        }
        else {
          pinfo_str->width  = (unsigned long)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcWidth);
          pinfo_str->height = (unsigned long)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcHeight);
          pinfo_str->order  = 1;  /* OS/2λͼ���ǵ���� */
          pinfo_str->bitcount = (unsigned long)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcBitCount);

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
            __leave;
          }

          memmove((void*)(pinfo_str->palette), (const void*)(pdibinfo->bmiColors), sizeof(RGB_QUAD)*pal_num);
        }
        else {
          for (i = 0; i < (int)pinfo_str->pal_count; i++) {
            ((LPRGB_TRIPLE)(&pinfo_str->palette[i]))->rgbtBlue = ((LPBMP_CORE)pdibinfo)->bmciColors[i].rgbtBlue;
            ((LPRGB_TRIPLE)(&pinfo_str->palette[i]))->rgbtGreen = ((LPBMP_CORE)pdibinfo)->bmciColors[i].rgbtGreen;
            ((LPRGB_TRIPLE)(&pinfo_str->palette[i]))->rgbtRed = ((LPBMP_CORE)pdibinfo)->bmciColors[i].rgbtRed;
            pinfo_str->palette[i] &= 0xffffff;  /* ����ֽ����� */
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
      pinfo_str->psubimg  = (LPSUBIMGBLOCK)0;

      assert(pinfo_str->p_bit_data == (unsigned char*)0);
      /* ����Ŀ��ͼ���ڴ�飨+4 �� β������4�ֽڻ������� */
      pinfo_str->p_bit_data = (unsigned char*)isirw_orgpix_malloc(linesize * pinfo_str->height + 4);

      if (!pinfo_str->p_bit_data) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      assert(pinfo_str->pp_line_addr == (void**)0);
      /* �������׵�ַ���� */
      pinfo_str->pp_line_addr = (void**)isirw_malloc(sizeof(void*) * pinfo_str->height);

      if (!pinfo_str->pp_line_addr) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* �ص�֪ͨ */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_MAST_IMAGE, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* �Ƿ���RLE */
      if ((pinfo_str->compression == ICS_RLE4) || (pinfo_str->compression == ICS_RLE8)) {
        assert(style == 0);

        /* �����ڴ棬���ڴ��RLEԭʼ���� */
        prlesou = prle = (unsigned char*)isirw_malloc(dib_data_len);

        if (!prle) {
          b_status = ER_MEMORYERR;
          __leave;
        }

        /* ��ȡRLEԭʼ���� */
        if (isio_seek(pfile, bfh.bfOffBits, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)prle, dib_data_len, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ΪRLE��ѹ��׼������ʱ��Ϊдָ�����飩 */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + (i * linesize));
        }

        if (pinfo_str->compression == ICS_RLE4) {
          _write2buff4RLE(0, 0, 0, (unsigned char*)pinfo_str->pp_line_addr[line]);  /* ��ʼ��д���� */

          /* �Ƚ�RLE�����ݽ�ѹ�ɱ�׼��BMP���� */
          while (!bEOF) {
            pix = *prle++;

            if (pix == 0) { /* ���Է�ʽ */
              pix2 = *prle++;

              switch (pix2) {
              case  0:    /* �н�β */
                line++;
                _write2buff4RLE(2, 0, 0, (unsigned char*)pinfo_str->pp_line_addr[line]); /* ָ���µ�ƫ�Ƶ�ַ */
                break;

              case  1:    /* ͼ���β */
                bEOF = 1;
                break;

              case  2:    /* ��ǰ��ַƫ�� */
                pix = *prle++;
                pix2 = *prle++;
                line += pix2; /* ����pix2�� */
                _write2buff4RLE(2, pix % 2, 0, (unsigned char*)((unsigned char*)pinfo_str->pp_line_addr[line] + (unsigned long)pix / 2));
                break;

              default:    /* ����ѹ�������� */
                for (i = 0; i < pix2 / 2; i++) {
                  pix = *(prle + i);
                  _write2buff4RLE(1, 2, pix, (unsigned char*)0);
                }

                if (pix2 % 2) {
                  /* ������©�ĵ������� */
                  pix = *(prle + i);
                  _write2buff4RLE(1, 1, pix, (unsigned char*)0);
                }

                /* (pix2+1)/2�ǽ����ض��뵽�ֽڱ߽��ϣ��ټ�1����2�Ƕ��뵽�ֱ߽��� */
                prle = (unsigned char*)(prle + ((pix2 + 1) / 2 + 1) / 2 * 2); /* �ֶ��� */
                break;
              }
            }
            else {    /* ���뷽ʽ */
              /* pix == ��ͬ���صĸ��� */
              pix2 = *prle++; /* pix2 == ���� */
              _write2buff4RLE(1, (int)pix, pix2, (unsigned char*)0);
            }
          }

          _write2buff4RLE(3, 0, 0, (unsigned char*)0);  /* ����д���� */
        }
        else {
          _write2buff8RLE(0, 0, 0, (unsigned char*)pinfo_str->pp_line_addr[line]);  /* ��ʼ��д���� */

          while (!bEOF) {
            pix = *prle++;

            if (pix == 0) { /* ���Է�ʽ */
              pix2 = *prle++;

              switch (pix2) {
              case  0:    /* �н�β */
                line++;
                _write2buff8RLE(2, 0, 0, (unsigned char*)pinfo_str->pp_line_addr[line]); /* ָ���µ�ƫ�Ƶ�ַ */
                break;

              case  1:    /* ͼ���β */
                bEOF = 1;
                break;

              case  2:    /* ��ǰ��ַƫ�� */
                pix = *prle++;
                pix2 = *prle++;
                line += pix2; /* ����pix2�� */
                _write2buff8RLE(2, 0, 0, (unsigned char*)((unsigned char*)pinfo_str->pp_line_addr[line] + (unsigned long)pix));
                break;

              default:    /* ����ѹ�������� */
                for (i = 0; i < pix2; i++) {
                  pix = *(prle + i);
                  _write2buff8RLE(1, 1, pix, (unsigned char*)0);
                }

                /* (pix2+1)/2�ǽ����ض��뵽�ֽڱ߽��ϣ��ټ�1����2�Ƕ��뵽�ֱ߽��� */
                prle = (unsigned char*)(prle + ((pix2 + 1) / 2) * 2); /* �ֶ��� */
                break;
              }
            }
            else {    /* ���뷽ʽ */
              /* pix == ��ͬ���صĸ��� */
              pix2 = *prle++; /* pix2 == ���� */
              _write2buff8RLE(1, (int)pix, pix2, (unsigned char*)0);
            }
          }

          _write2buff8RLE(3, 0, 0, (unsigned char*)0);  /* ����д���� */
        }

        /* ��ʼ�����׵�ַ��RLEλͼ���ǵ���ģ� */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
        }

        /* һ���н��ȵ��� */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_LOAD)) {
        case  0:
          break;

        case  1:
          b_status = ER_USERBREAK;
          __leave;
          break;

        case  2:
          b_status = ER_SYSERR;
          __leave;
          break;

        case  3:
          b_status = ER_MEMORYERR;
          __leave;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          __leave;
          break;
        }

        /* ��ͼ����Ƚ��� */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
        case  0:
          break;

        case  1:
          b_status = ER_USERBREAK;
          __leave;
          break;

        case  2:
          b_status = ER_SYSERR;
          __leave;
          break;

        case  3:
          b_status = ER_MEMORYERR;
          __leave;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          __leave;
          break;
        }
      }
      else {
        if (pinfo_str->order == 0) {  /* ���� */
          for (i = 0; i < (int)(pinfo_str->height); i++) {
            pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + (i * linesize));
          }
        }
        else {            /* ���� */
          for (i = 0; i < (int)(pinfo_str->height); i++) {
            pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
          }
        }

        /* ��ȡλ���ݵ��ڴ� */
        if (isio_seek(pfile, bfh.bfOffBits, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        pbit = pinfo_str->p_bit_data;

        /* ���ж��� */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          if (isio_read((void*)pbit, linesize, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
          }

          pbit = (unsigned char*)(pbit + linesize);

          /* ���Ȼص� */
          switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, i, 1, IRWE_CALLBACK_FUNID_LOAD)) {
          case  0:
            break;

          case  1:
            b_status = ER_USERBREAK;
            __leave;
            break;

          case  2:
            b_status = ER_SYSERR;
            __leave;
            break;

          case  3:
            b_status = ER_MEMORYERR;
            __leave;
            break;

          default:
            assert(0);
            b_status = ER_SYSERR;
            __leave;
            break;
          }
        }

        /* ��ͼ����Ȼص����� */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
        case  0:
          break;

        case  1:
          b_status = ER_USERBREAK;
          __leave;
          break;

        case  2:
          b_status = ER_SYSERR;
          __leave;
          break;

        case  3:
          b_status = ER_MEMORYERR;
          __leave;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          __leave;
          break;
        }
      }

      pinfo_str->data_state = 2;
    }
    __finally {
      if (prlesou) {
        isirw_free(prlesou);
      }

      if (pdibinfo) {
        isirw_free(pdibinfo);
      }

      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        if (pinfo_str->p_bit_data) {
          isirw_free(pinfo_str->p_bit_data);
          pinfo_str->p_bit_data = (unsigned char*)0;
        }

        if (pinfo_str->pp_line_addr) {
          isirw_free(pinfo_str->pp_line_addr);
          pinfo_str->pp_line_addr = (void**)0;
        }

        if (pinfo_str->data_state == 2) {
          pinfo_str->data_state = 1;  /* �Զ����� */
        }

        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD);
      }
      else {
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
        case  0:
          break;

        case  1:
          b_status = ER_USERBREAK;
          break;

        case  2:
          b_status = ER_SYSERR;
          break;

        case  3:
          b_status = ER_MEMORYERR;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          break;
        }

        if (b_status != ER_SUCCESS) {
          if (pinfo_str->p_bit_data) {
            isirw_free(pinfo_str->p_bit_data);
            pinfo_str->p_bit_data = (unsigned char*)0;
          }

          if (pinfo_str->pp_line_addr) {
            isirw_free(pinfo_str->pp_line_addr);
            pinfo_str->pp_line_addr = (void**)0;
          }

          if (pinfo_str->data_state == 2) {
            pinfo_str->data_state = 1;
          }

          (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD);
        }
      }

      LeaveCriticalSection(&bmp_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* ����ͼ�� ��ֻ�ܱ���Windows����λͼ��*/
BMP_API int CALLAGREEMENT bmp_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;
  BMP_FILE_HEADER bfh;
  LPBMP_INFO    pdibinfo = (LPBMP_INFO)0;

  unsigned long dib_info_len;   /* DIB��Ϣ�ṹ�ߴ� */
  unsigned char* plinebuf = (unsigned char*)0;
  int       i, linesize;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&bmp_save_img_critical);

      /* ����ϵͳ�ص����� */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_INTO_PLUG, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* �ж��Ƿ�����֧�ֵ�ͼ������ */
      if (_bmp_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* �������ͬ��������������������ȷ����ʾ */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }


      /* ����λͼ��Ϣͷ�ṹ�ĳߴ磨������ɫ�壩 */
      switch (pinfo_str->bitcount) {
      case  1:
      case  4:
      case  8:
        dib_info_len = sizeof(BMP_INFO_HEADER) + pinfo_str->pal_count * sizeof(RGB_QUAD);
        break;

      case  24:
        dib_info_len = sizeof(BMP_INFO_HEADER);
        break;

      case  16:
      case  32:
        dib_info_len = sizeof(BMP_INFO_HEADER) + 3 * sizeof(unsigned long);
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
      }

      /* ������ BMP_INFO ��Ϣ���ڴ�� */
      if ((pdibinfo = (LPBMP_INFO)isirw_malloc(dib_info_len)) == (LPBMP_INFO)0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      pdibinfo->bmiHeader.biSize      = sizeof(BMP_INFO_HEADER);
      pdibinfo->bmiHeader.biPlanes    = 1;
      pdibinfo->bmiHeader.biBitCount    = (unsigned short)pinfo_str->bitcount;
      pdibinfo->bmiHeader.biCompression = ((pinfo_str->bitcount == 16) || (pinfo_str->bitcount == 32)) ? 3 : 0;
      pdibinfo->bmiHeader.biWidth     = pinfo_str->width;
      /* ȫ������Ϊ����ͼ */
      pdibinfo->bmiHeader.biHeight    = (long)pinfo_str->height;
      pdibinfo->bmiHeader.biSizeImage   = 0;
      pdibinfo->bmiHeader.biClrImportant  = 0;
      pdibinfo->bmiHeader.biClrUsed   = 0;
      pdibinfo->bmiHeader.biXPelsPerMeter = 0;
      pdibinfo->bmiHeader.biYPelsPerMeter = 0;

      if (pdibinfo->bmiHeader.biCompression == 0) {
        memmove((void*)pdibinfo->bmiColors, (const void*)pinfo_str->palette, pinfo_str->pal_count * sizeof(RGB_QUAD));
      }
      else {
        *((unsigned long*) & (pdibinfo->bmiColors[0])) = pinfo_str->r_mask;
        *((unsigned long*) & (pdibinfo->bmiColors[1])) = pinfo_str->g_mask;
        *((unsigned long*) & (pdibinfo->bmiColors[2])) = pinfo_str->b_mask;
      }

      /* ��дλͼ�ļ�ͷ�ṹ */
      bfh.bfSize    = sizeof(BMP_FILE_HEADER);
      bfh.bfType    = DIB_HEADER_MARKER;
      bfh.bfOffBits = bfh.bfSize + dib_info_len;
      bfh.bfReserved1 = bfh.bfReserved2 = 0;

      /* ȡ��ɨ���гߴ� */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);

      /* ����д���ݻ����� */
      if ((plinebuf = (unsigned char*)isirw_malloc(linesize)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* д���ļ�ͷ�ṹ */
      if (isio_write((const void*)&bfh, sizeof(BMP_FILE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* д����Ϣͷ�ṹ */
      if (isio_write((const void*)pdibinfo, dib_info_len, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_MAST_IMAGE, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* д��ͼ��λ����
        ע�������ͼ�����ݱ�����ISeeλ����ʽ��������ܻ����ʧ��
      */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        memcpy((void*)plinebuf, (const void*)(unsigned char*) \
            (pinfo_str->pp_line_addr[pinfo_str->height - 1 - i]), linesize);

        /* һ�ɱ���Ϊ����ͼ */
        if (isio_write((const void*)plinebuf, linesize, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, i, 1, IRWE_CALLBACK_FUNID_SAVE)) {
        case  0:
          break;

        case  1:
          b_status = ER_USERBREAK;
          __leave;
          break;

        case  2:
          b_status = ER_SYSERR;
          __leave;
          break;

        case  3:
          b_status = ER_MEMORYERR;
          __leave;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          __leave;
          break;
        }
      }

      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }
    }
    __finally {
      if (plinebuf) {
        isirw_free(plinebuf);
      }

      if (pdibinfo) {
        isirw_free(pdibinfo);
      }

      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }
      else {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }

      LeaveCriticalSection(&bmp_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}

