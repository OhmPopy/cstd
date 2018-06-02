// TGAͼ���дģ��ʵ���ļ�
// ��ȡ���ܣ�δѹ����8��15��16��24��32λͼ�񣬼�
// ����RLEѹ����8��15��16��24��32λͼ��
// ���湦�ܣ�δѹ����8��15��16��24��32λͼ��


/**************TGA ͼ������ *************/
typedef enum {
  TGA_NULL    = 0,
  TGA_UNCPSPAL  = 1,
  TGA_UNCPSCOLOR  = 2,
  TGA_UNCPSGRAY = 3,
  TGA_RLEPAL    = 9,
  TGA_RLECOLOR  = 10,
  TGA_RLEGRAY   = 11
} TGATYPE;

/**********TGA ͼ�����ݴ洢����(��bDescriptor��Ӧλ��) ***********/
typedef enum {
  TGA_HORZMIRROR = 0x10,
  TGA_VERTMIRROR = 0x20
} TGAMIRROR;

/**************TGA �ļ�ͷ���� ***************/
typedef struct {
  unsigned char  bID_Length;    /* ������Ϣ���� */
  unsigned char  bPalType;    /* ��ɫ����Ϣ */
  unsigned char  bImageType;    /* ͼ������(0,1,2,3,9,10,11) */
  unsigned short wPalFirstNdx;  /* ��ɫ���һ������ֵ */
  unsigned short wPalLength;    /* ��ɫ��������(�Ե�ɫ�嵥ԪΪ��λ) */
  unsigned char  bPalBits;    /* һ����ɫ�嵥λλ��(15,16,24,32) */
  unsigned short wLeft;     /* ͼ���������(��������) */
  unsigned short wBottom;     /* ͼ��׶�����(��������) */
  unsigned short wWidth;      /* ͼ���� */
  unsigned short wDepth;      /* ͼ�󳤶� */
  unsigned char  bBits;     /* һ������λ�� */
  unsigned char  bDescriptor;   /* ������������  */
} TGAHEADER, *LPTGAHEADER;

/**************TGA �ļ�β���� ***************/
typedef struct {
  unsigned long  eao;       /* ��չ��ƫ�� */
  unsigned long  ddo;       /* ��������ƫ�� */
  unsigned char  info[16];    /* TRUEVISION-XFILE �̱��ַ��� */
  unsigned char  period;      /* �ַ�"." */
  unsigned char  zero;      /* 0 */
} TGAFOOTER, *LPTGAFOOTER;



IRWP_INFO     tga_irwp_info;      /* �����Ϣ�� */
unsigned char*   tga_save_idinfo = "ISeeExplorer";
/* ����ͼ��ʱע��ı�־����Ϣ */



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* ��ȡ����ɫ������ֵ */
int CALLAGREEMENT _get_mask(LPTGAHEADER pInfo,
    unsigned long* pb_mask,
    unsigned long* pg_mask,
    unsigned long* pr_mask,
    unsigned long* pa_mask
                           )
{
  int result = 0;

  assert(pInfo->bBits);

  switch (pInfo->bBits) {
  case  8:
    /* ��ɫ��λͼ������ */
    *pr_mask = 0;
    *pg_mask = 0;
    *pb_mask = 0;
    *pa_mask = 0;
    break;

  case  15:
    /*******************************************
        15��16λ����ʾ��ͼ - TGA

        ���ֽ�      ���ֽ�
      0000 0000   0000 0000
               1 1111--B  15λ��ʽ
           11 111 -------G
       111 11  --------------R
      0
               1 1111--B  16λ��ʽ
           11   111 -------G
       111 11  --------------R
      1  --------------------A
    *******************************************/
    *pr_mask = 0x7c00;
    *pg_mask = 0x3e0;
    *pb_mask = 0x1f;
    *pa_mask = 0;
    break;

  case  16:
    *pr_mask = 0x7c00;
    *pg_mask = 0x3e0;
    *pb_mask = 0x1f;
    *pa_mask = 0x8000;
    break;

  case  24:
    *pr_mask  = 0xff0000;
    *pg_mask  = 0xff00;
    *pb_mask  = 0xff;
    *pa_mask  = 0x0;
    break;

  case  32:
    /******************************************************
            32λ����ʾ��ͼ - TGA
          ��               ->                 ��
    0000 0000   0000 0000   0000 0000   0000 0000   888��ʽ
    1111 1111  ------------------------------------A
                1111 1111  ------------------------R
                            1111 1111 -------------G
                                        1111 1111--B
    ******************************************************/
    *pr_mask  = 0xff0000;
    *pg_mask  = 0xff00;
    *pb_mask  = 0xff;
    *pa_mask  = 0x0;
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


/* ����ɨ���гߴ�(���ֽڶ���) */
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* �н�ѹ������psrcʵ�ʽ�ѹ�ֽ�������Դ�������󣬷���-1 */
int CALLAGREEMENT _decoding_line(
    unsigned char* pdest,     /* Ŀ�껺���� */
    unsigned char* psrc,      /* RLEԴ����  */
    int rleunit,          /* rleunitΪ��ѹ��λ(in Bytes) */
    int destlen           /* destlenΪ��ѹĿ�곤�� */
)
{
  int i, j, nCount;
  unsigned char* pdestEnd = pdest + destlen, *psrcOrg = psrc, *pdestOrg = pdest;
  unsigned char* ptmp;

  assert(psrc);
  assert(pdest);

  while (pdest < pdestEnd) {
    if (psrc[0] & 0x80) {       /* �ظ����� */
      nCount = (psrc[0] & 0x7F) + 1;
      psrc++;

      for (i = 0; i < nCount; i++) {
        ptmp = psrc;

        for (j = 0; j < rleunit; j++) { /* ��ѹһ������(length equ rleunit) */
          *pdest++ = *ptmp++;
        }
      }

      psrc += rleunit;
    }
    else {                /* ���ظ����� */
      nCount = (psrc[0] & 0x7F) + 1;
      psrc++;
      memcpy(pdest, psrc, nCount * rleunit); /* ���ظ�����ĳߴ���ܻ�Ƚϴ�����memcpy��Ч�ʿ��ܸ��� */
      pdest += nCount * rleunit;
      psrc += nCount * rleunit;
    }
  }

  if (pdest != pdestEnd) {        /* ͼ�����ݷǷ������� -1 */
    return -1;
  }

  return psrc - psrcOrg;          /* ��ѹ�ɹ������ر���ѹ�ֽ��� */
}


/* �жϴ����ͼ���Ƿ���Ա����� */
int CALLAGREEMENT _tga_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* ## λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(tga_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* ��֧�ֵ�λ��ͼ�� */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (tga_irwp_info.irwp_save.img_num)
    if (tga_irwp_info.irwp_save.img_num == 1)
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* ͼ���������ȷ */
      }

  return 0;
}


/* ��ȡͼ��λ���� */
TGA_API int CALLAGREEMENT tga_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;
  unsigned long stream_length;
  TGAHEADER   tgaHeader;

  int       i, k, j, linesize, decoded;
  int       pallen = 0, firstindex = 0, newlinepos = 0, ile = 0;
  unsigned short  palentry;
  unsigned char* pbuf = 0;
  unsigned char* pbits;

  unsigned char bpix;
  unsigned short  wpix;
  unsigned long lpix;

  static unsigned char tmp_pal[1024]; /* ��ʱ��ɫ�建���� */

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&tga_load_img_critical);

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

      /* ��ȡ������ */
      stream_length = isio_length(pfile);

      /* ���ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (isio_read((void*)&tgaHeader, sizeof(TGAHEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        /* �ж��ļ�ͷ��ͼ�����ͱ�־ */
        switch (tgaHeader.bImageType) {
        case  TGA_RLEPAL:
        case  TGA_RLECOLOR:
        case  TGA_RLEGRAY:

          /* ��Ч��ͼ��������Ӧ����ͷ�ṹ�ߴ磨�����жϣ� */
          if (stream_length < sizeof(TGAHEADER)) {
            b_status = ER_NONIMAGE;
            __leave;
          }

          break;

        case  TGA_UNCPSPAL:
        case  TGA_UNCPSCOLOR:
        case  TGA_UNCPSGRAY:

          /* δѹ��ͼ��ɼ����ͼ�����ݵĳߴ磬�����жϵ��ܸ���ȷһЩ */
          if (stream_length < (sizeof(TGAHEADER) + tgaHeader.bID_Length + ((tgaHeader.bBits + 7) >> 3)*tgaHeader.wDepth * tgaHeader.wWidth + tgaHeader.wPalLength * ((tgaHeader.bPalBits + 7) >> 3))) {
            b_status = ER_NONIMAGE;
            __leave;
          }

          break;

        case  TGA_NULL: /* δ����ͼ�����ݵ��ļ�����Ϊ��Чͼ�� */
        default:      /* δ֪�ļ����� */
          b_status = ER_NONIMAGE;
          __leave;
          break;
        }

        /* ��������λ���Ƿ�Ϸ� */
        if ((tgaHeader.bBits != 8) && (tgaHeader.bBits != 15) && \
            (tgaHeader.bBits != 16) && (tgaHeader.bBits != 24) && \
            (tgaHeader.bBits != 32)) {
          b_status = ER_BADIMAGE;
          __leave;
        }


        /* ��дͼ����Ϣ�ṹ */
        pinfo_str->imgtype    = IMT_RESSTATIC;  /* ͼ���ļ����� */
        pinfo_str->imgformat  = IMF_TGA;      /* ͼ���ļ���ʽ����׺���� */

        switch (tgaHeader.bImageType) {     /* ͼ���ѹ����ʽ */
        case  TGA_UNCPSPAL:
        case  TGA_UNCPSCOLOR:
        case  TGA_UNCPSGRAY:
          pinfo_str->compression = ICS_RGB;
          break;

        case  TGA_RLEPAL:
        case  TGA_RLECOLOR:
        case  TGA_RLEGRAY:
          switch (tgaHeader.bBits) {
          case  8:
            pinfo_str->compression = ICS_RLE8;
            break;

          case  15:
          case  16:
            pinfo_str->compression = ICS_RLE16;
            break;

          case  24:
            pinfo_str->compression = ICS_RLE24;
            break;

          case  32:
            pinfo_str->compression = ICS_RLE32;
            break;
          }

          break;

        default:
          assert(0);    /* ϵͳ���� */
          b_status = ER_SYSERR;
          __leave;
          break;
        }


        /* ��дͼ����Ϣ */
        pinfo_str->width  = (unsigned long)tgaHeader.wWidth;
        pinfo_str->height = (unsigned long)tgaHeader.wDepth;
        pinfo_str->order  = (tgaHeader.bDescriptor & TGA_VERTMIRROR) ? 0 : 1;
        pinfo_str->bitcount = (unsigned long)(tgaHeader.bBits == 15) ? 16 : tgaHeader.bBits;

        /* ��д������Ϣ */
        if (_get_mask(&tgaHeader,
            (unsigned long*)(&pinfo_str->b_mask),
            (unsigned long*)(&pinfo_str->g_mask),
            (unsigned long*)(&pinfo_str->r_mask),
            (unsigned long*)(&pinfo_str->a_mask)) == 1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        pinfo_str->data_state = 1;
      }


      /* ���õ�ɫ������ */
      if (pinfo_str->bitcount == 8) {
        pinfo_str->pal_count = 256;

        /* ���ɻҶ�ͼ��ɫ�� */
        if ((tgaHeader.bImageType == TGA_UNCPSGRAY) || (tgaHeader.bImageType == TGA_RLEGRAY)) {
          for (i = 0; i < (int)pinfo_str->pal_count; i++) {
            pinfo_str->palette[i] = (i << 16) | (i << 8) | (i);
          }
        }
        else { /* ������ת����ɫ�����ݴ��ļ� */
          if (isio_seek(pfile, sizeof(TGAHEADER) + tgaHeader.bID_Length, SEEK_SET) == -1) {
            b_status = ER_FILERWERR;
            __leave;
          }

          /* TGA��ɫ���������ͨ��ARGB��ʽ��������Ҫ������жϹ��� */
          pallen = tgaHeader.wPalLength * ((tgaHeader.bPalBits + 7) >> 3);
          firstindex = tgaHeader.wPalFirstNdx;

          if (isio_read((void*)tmp_pal, pallen, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
          }

          /* ת����ɫ������ */
          switch (tgaHeader.bPalBits) {
          case  15:
          case  16:   /* ת��2�ֽڵ�ɫ������ */
            for (i = 0; i < pallen; i += 2) {
              if (firstindex >= 256) {
                b_status = ER_BADIMAGE;
                __leave;
              }

              palentry = *((unsigned short*)(tmp_pal + i));
              ((LPPALITEM)(&pinfo_str->palette[firstindex]))->blue  = (palentry & 0x1f) << 3;
              ((LPPALITEM)(&pinfo_str->palette[firstindex]))->green = ((palentry >> 5) & 0x1f) << 3;
              ((LPPALITEM)(&pinfo_str->palette[firstindex]))->red   = ((palentry >> 10) & 0x1f) << 3;
              pinfo_str->palette[firstindex++] &= 0xffffff; /* ����ֽ����� */

            }

            break;

          case  24:   /* ת��3�ֽڵ�ɫ������ */
            for (i = 0; i < pallen; i += 3) {
              if (firstindex >= 256) {
                b_status = ER_BADIMAGE;
                __leave;
              }

              ((LPPALITEM)(&pinfo_str->palette[firstindex]))->blue  = tmp_pal[i];
              ((LPPALITEM)(&pinfo_str->palette[firstindex]))->green = tmp_pal[i + 1];
              ((LPPALITEM)(&pinfo_str->palette[firstindex]))->red   = tmp_pal[i + 2];
              pinfo_str->palette[firstindex++] &= 0xffffff;
            }

            break;

          case  32:   /* ת��4�ֽڵ�ɫ������ */
            for (i = 0; i < pallen; i += 4) {
              if (firstindex >= 256) {
                b_status = ER_BADIMAGE;
                __leave;
              }

              pinfo_str->palette[firstindex] = *(unsigned long*)(tmp_pal + i);
              pinfo_str->palette[firstindex++] &= 0xffffff;
            }

            break;

          default:
            assert(0);    /* ϵͳ���� */
            b_status = ER_SYSERR;
            __leave;
            break;
          }
        }
      }
      else {
        pinfo_str->pal_count = 0; /* �޵�ɫ������ */
      }

      /* ȡ��ɨ���гߴ� */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);

      /* ����ͼ����� */
      pinfo_str->imgnumbers = 1;    /* TGA�ļ���ֻ��һ��ͼ��*/
      /* Ӧ������ԭʼͼ����ͼ(Postage Stamp)��*/
      /* �����¶���δ����ʵ�� */
      pinfo_str->psubimg  = (LPSUBIMGBLOCK)0;   /* ��ͼ�����ÿ� */

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

      /* ����ͼ��ɨ���л����� */
      pbuf = (unsigned char*)isirw_malloc(pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3) + pinfo_str->width); /* ��������RLEѹ����û��һ���ظ����� */

      if (!pbuf) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* ��ʼ�����׵�ַ���� */
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

      /* ��ȡͼ������ */
      /*     ����ͼ���ļ���λ��ȵĲ�ͬ�����ò�ͬ�Ķ�ȡ���� */
      switch (tgaHeader.bImageType) {
      case  TGA_UNCPSPAL:
      case  TGA_UNCPSGRAY:
      case  TGA_UNCPSCOLOR:
        /* ��һ��ͼ���������ļ��е���ʼλ�� */
        newlinepos = sizeof(TGAHEADER) + tgaHeader.bID_Length + pallen;

        if (isio_seek(pfile, newlinepos, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* �ļ�ɨ���г��� */
        ile = pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3);

        pbits = pinfo_str->p_bit_data;

        /* ����Ϊ��λ��ȡͼ������ */
        if (tgaHeader.bDescriptor & TGA_HORZMIRROR) { /* ���ҷ��� */
          for (i = 0; i < (int)pinfo_str->height; i++) {
            if (isio_read((void*)pbuf, ile, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;
            }

            for (k = 0; k < (int)pinfo_str->width; k++) {
              switch (pinfo_str->bitcount) {
              case  8:
                ((unsigned char*)pbits)[k] = pbuf[pinfo_str->width - 1 - k];
                break;

              case  15:
              case  16:
                ((unsigned short*)pbits)[k] = ((unsigned short*)pbuf)[pinfo_str->width - 1 - k];
                break;

              case  24:
                for (j = 0; j < 3; j++) {
                  ((unsigned char*)pbits)[k * 3 + j] = pbuf[(pinfo_str->width - 1 - k) * 3 + j];
                }

                break;

              case  32:
                ((unsigned long*)pbits)[k] = ((unsigned long*)pbuf)[pinfo_str->width - 1 - k];
                break;

              default:
                assert(0);  /* ϵͳ���� */
                b_status = ER_SYSERR;
                __leave;
                break;
              }
            }

            pbits = (unsigned char*)(pbits + linesize);

            /* ���ȵ��� */
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
        }
        else {
          for (i = 0; i < (int)pinfo_str->height; i++) {
            if (isio_read((void*)pbits, ile, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;
            }

            pbits = (unsigned char*)(pbits + linesize);

            /* ���ȵ��� */
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
        }

        break;

      case  TGA_RLEGRAY:
      case  TGA_RLEPAL:
      case  TGA_RLECOLOR:
        /* ��һ��ͼ���������ļ��е���ʼλ�� */
        newlinepos = sizeof(TGAHEADER) + tgaHeader.bID_Length + pallen;

        /* �ļ�ɨ���г��ȣ������������㣩 */
        ile = pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3) + pinfo_str->width;

        pbits = pinfo_str->p_bit_data;

        /* ����Ϊ��λ��ȡͼ������ */
        if (tgaHeader.bDescriptor & TGA_HORZMIRROR) { /* ���ҷ��� */
          for (i = 0; i < (int)pinfo_str->height; i++) {
            if (isio_seek(pfile, newlinepos, SEEK_SET) == -1) {
              b_status = ER_FILERWERR;
              __leave;
            }

            if (isio_read((void*)pbuf, 1, ile, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;
            }

            /* ��ѹһRLEͼ���� */
            decoded = _decoding_line(pbits, pbuf, (pinfo_str->bitcount + 7) >> 3, pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3));

            if (decoded == -1) {
              b_status = ER_BADIMAGE;
              __leave;
            }

            assert(decoded);

            /* ��һ��ͼ���������ļ��е���ʼλ�� */
            newlinepos += decoded;

            /* ���ҵߵ�ͼ������ */
            for (k = 0; k < (int)pinfo_str->width; k++) {
              switch (pinfo_str->bitcount) {
              case  8:
                bpix = ((unsigned char*)pbits)[k];
                ((unsigned char*)pbits)[k] = ((unsigned char*)pbits)[pinfo_str->width - 1 - k];
                ((unsigned char*)pbits)[pinfo_str->width - 1 - k] = bpix;
                break;

              case  15:
              case  16:
                wpix = ((unsigned short*)pbits)[k];
                ((unsigned short*)pbits)[k] = ((unsigned short*)pbits)[pinfo_str->width - 1 - k];
                ((unsigned short*)pbits)[pinfo_str->width - 1 - k] = wpix;
                break;

              case  24:
                for (j = 0; j < 3; j++) {
                  bpix = ((unsigned char*)pbits)[k * 3 + j];
                  ((unsigned char*)pbits)[k * 3 + j] = ((unsigned char*)pbits)[(pinfo_str->width - 1 - k) * 3 + j];
                  ((unsigned char*)pbits)[(pinfo_str->width - 1 - k) * 3 + j] = bpix;
                }

                break;

              case  32:
                lpix = ((unsigned long*)pbits)[k];
                ((unsigned long*)pbits)[k] = ((unsigned long*)pbits)[pinfo_str->width - 1 - k];
                ((unsigned long*)pbits)[pinfo_str->width - 1 - k] = lpix;
                break;

              default:
                assert(0);  /* ϵͳ���� */
                b_status = ER_SYSERR;
                __leave;
                break;
              }
            }

            pbits = (unsigned char*)(pbits + linesize);

            /* ���ȵ��� */
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
        }
        else {
          for (i = 0; i < (int)pinfo_str->height; i++) {
            if (isio_seek(pfile, newlinepos, SEEK_SET) == -1) {
              b_status = ER_FILERWERR;
              __leave;
            }

            if (isio_read((void*)pbuf, 1, ile, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;
            }

            /* ��ѹһRLEͼ���� */
            decoded = _decoding_line(pbits, pbuf, (pinfo_str->bitcount + 7) >> 3, pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3));

            if (decoded == -1) {
              b_status = ER_BADIMAGE;
              __leave;
            }

            assert(decoded);

            /* ��һ��ͼ���������ļ��е���ʼλ�� */
            newlinepos += decoded;

            pbits = (unsigned char*)(pbits + linesize);

            /* ���ȵ��� */
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
        }

        break;

      default:
        assert(0);  /* ϵͳ���� */
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

      pinfo_str->data_state = 2;
    }
    __finally {
      if (pbuf) {
        isirw_free(pbuf);
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

      LeaveCriticalSection(&tga_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* ����ͼ�� */
TGA_API int CALLAGREEMENT tga_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;

  TGAHEADER   tgaHeader;
  TGAFOOTER   tgaFooter;
  int       i, wrlinesize;
  unsigned char*   pbits;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&tga_save_img_critical);

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
      if (_tga_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* �������ͬ��������������������ȷ����ʾ */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ������Ϣ���ȣ��˴�������һ��"ISeeExplorer"�ַ��� */
      tgaHeader.bID_Length   = (unsigned char)strlen((const char*)tga_save_idinfo);
      /* ��ɫ����Ϣ���Ҷ�ͼ������Ҳ����������ɫ��ͼ�� */
      tgaHeader.bPalType     = (unsigned char)(pinfo_str->bitcount == 8) ? 1 : 0;
      /* ͼ������(ֻ��1��2��������) */
      tgaHeader.bImageType   = (unsigned char)(pinfo_str->bitcount == 8) ? TGA_UNCPSPAL : TGA_UNCPSCOLOR;
      /* ��ɫ���һ������ֵ */
      tgaHeader.wPalFirstNdx = 0;
      /* ��ɫ��������(�Ե�ɫ�嵥ԪΪ��λ) */
      tgaHeader.wPalLength   = (unsigned short)(pinfo_str->bitcount == 8) ? 256 : 0;
      /* һ����ɫ�嵥λλ��(ISee��ɫ������32λ��) */
      tgaHeader.bPalBits     = 32;
      /* ͼ�����¶����� */
      tgaHeader.wLeft        = 0;
      tgaHeader.wBottom      = 0;
      /* ͼ����߶� */
      tgaHeader.wWidth       = (unsigned short)pinfo_str->width;
      tgaHeader.wDepth       = (unsigned short)pinfo_str->height;
      /* ����λ�� */
      tgaHeader.bBits        = (unsigned char)pinfo_str->bitcount;
      /* ������������(������Aͨ����һ��Ϊ����ͼ) */
      tgaHeader.bDescriptor  = 0;


      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* д���ļ�ͷ�ṹ */
      if (isio_write((const void*)&tgaHeader, sizeof(TGAHEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* д����Ϣ */
      if (isio_write((const void*)tga_save_idinfo, strlen(tga_save_idinfo), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* д���ɫ������ */
      if (tgaHeader.bPalType == 1) {
        if (isio_write((const void*)pinfo_str->palette, pinfo_str->pal_count * 4, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }
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

      /* TGAδѹ��ɨ���гߴ粢����4�ֽڶ���ģ�������Ҫ��ʵ�����ؼ����гߴ� */
      wrlinesize = pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3);

      /* д��ͼ��λ����
        ע�������ͼ�����ݱ�����ISeeλ����ʽ��������ܻ����ʧ��
      */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        /* һ�ɱ���Ϊ����ͼ */
        pbits = (unsigned char*)(pinfo_str->pp_line_addr[pinfo_str->height - 1 - i]);

        if (isio_write((const void*)pbits, wrlinesize, 1, pfile) == 0) {
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

      /* ��д�ļ�β����Ϣ���°�TGA��ʽ��*/
      tgaFooter.eao = 0;
      tgaFooter.ddo = 0;
      memcpy((void*)tgaFooter.info, (const void*)"TRUEVISION-XFILE", 16);
      tgaFooter.period = '.';
      tgaFooter.zero = 0;

      if (isio_write((const void*)&tgaFooter, sizeof(TGAFOOTER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
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
      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }
      else {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }

      LeaveCriticalSection(&tga_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


