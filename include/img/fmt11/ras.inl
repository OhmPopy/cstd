// Sun RASͼ���дģ��ʵ���ļ�
// ��ȡ���ܣ��ɶ�ȡ1��8��24��32λѹ����δѹ��RASͼ��
// ���湦�ܣ��ṩ24λ��ѹ����ʽ�ı��湦��

/* �ļ��б��־ */
#define RAS_MAGIC           0x59a66a95

/* �ļ��ṹ���� */
#define RT_OLD          0     /* δѹ����ʽ */
#define RT_STANDARD       1     /* δѹ����ʽ */
#define RT_BYTE_ENCODED     2     /* RLEѹ����ʽ */
#define RT_FORMAT_RGB     3     /* RGB��ʽ */
#define RT_FORMAT_TIFF      4
#define RT_FORMAT_IFF     5

#define RT_EXPERIMENTAL     0xFFFF

#define RMT_NONE        0     /* �޵�ɫ������ */
#define RMT_EQUAL_RGB     1
#define RMT_RAW         2

#define RESC          0x80    /* RLEѹ����ʶ�� */
#define RAS_ROUNDROW      16      /* ɨ���ж���λ�� */


/* RASͼ��ͷ�ṹ */
typedef struct _SUNHeaderinfo {
  unsigned long ras_magic;        /* �б��־ */
  unsigned long ras_width;        /* ��� */
  unsigned long ras_height;       /* �߶� */
  unsigned long ras_depth;        /* λ��� */
  unsigned long ras_length;       /* ͼ�����ݳ��ȣ�������ͷ�ṹ�����ܵĵ�ɫ�壩*/
  unsigned long ras_type;       /* ��ʽ���� */
  unsigned long ras_maptype;      /* ��ɫ�����ݴ�ŷ�ʽ */
  unsigned long ras_maplength;      /* ��ɫ�峤�ȣ��ֽڼƣ�*/
} SUNHeaderinfo, *LPSUNHeaderinfo;



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* ����ɨ���гߴ�(���ֽڶ���) */
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* �жϴ����ͼ���Ƿ���Ա����棨�粻֧�ֱ��湦�ܣ��ɽ�������ȥ���� */
int CALLAGREEMENT _ras_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(ras_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* ��֧�ֵ�λ��ͼ�� */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (ras_irwp_info.irwp_save.img_num) {
    if (ras_irwp_info.irwp_save.img_num == 1) {
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* ͼ���������ȷ */
      }
    }
  }

  return 0;
}


/* ��ȡָ����Ŀ������������������*/
static enum EXERESULT CALLAGREEMENT _read_data(ISFILE* pfile,
    unsigned char* buf,
    long length,
    int rle,
    int init)
{
  static unsigned char repchar, remaining;

  enum EXERESULT  b_status = ER_SUCCESS;

  /* ��ʼ������ֵ */
  if (init) {
    repchar = remaining = 0;  /* ��̬�������ظ�����ʱ��Ҫ��ʱ�ĳ�ʼ�� */

    return b_status;
  }

  __try {
    __try {
      /* �����RLEѹ�����ݣ����Ƚ��� */
      if (rle) {
        while (length--) {
          if (remaining) {
            remaining--;
            *(buf++) = repchar;
          }
          else {
            /* ��ȡ��ʶ�ֽ� */
            if (isio_read((void*)&repchar, 1, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;
            }

            /* �ж��Ƿ�����ѹ�����ܵ��ֶ� */
            if (repchar == RESC) {
              if (isio_read((void*)&remaining, 1, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                __leave;
              }

              /* ѹ������Ϊ1�ֽڵ�0x80 */
              if (remaining == 0) {
                *(buf++) = RESC;
              }
              else { /* �ظ����� */
                /* ��ȡ�ظ��ֽ� */
                if (isio_read((void*)&repchar, 1, 1, pfile) == 0) {
                  b_status = ER_FILERWERR;
                  __leave;
                }

                *(buf++) = repchar;
              }
            }
            else { /* δѹ�����ֽ����� */
              *(buf++) = repchar;
            }
          }
        }
      }
      else { /* δѹ��ͼ�� */
        if (isio_read((void*)buf, length, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }
      }
    }
    __finally {
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_FILERWERR;
  }

  return b_status;
}

/* ��ȡͼ��λ���� */
RAS_API int CALLAGREEMENT ras_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       linesize, linelength, fill, i, j, numcolors, isrle, isrgb, fillchar;
  unsigned char c, *r = 0, *g, *b, *colormap = 0, *buf = 0, *bp, *bits;
  SUNHeaderinfo header;
  LPPALITEM   pal;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&ras_load_img_critical);

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

      if (isio_read((void*)&header, sizeof(SUNHeaderinfo), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ת��X86���� */
      header.ras_magic     = EXCHANGE_DWORD((header.ras_magic));
      header.ras_width     = EXCHANGE_DWORD((header.ras_width));
      header.ras_height    = EXCHANGE_DWORD((header.ras_height));
      header.ras_depth     = EXCHANGE_DWORD((header.ras_depth));
      header.ras_length    = EXCHANGE_DWORD((header.ras_length));
      header.ras_type      = EXCHANGE_DWORD((header.ras_type));
      header.ras_maptype   = EXCHANGE_DWORD((header.ras_maptype));
      header.ras_maplength = EXCHANGE_DWORD((header.ras_maplength));

      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        if (header.ras_magic != RAS_MAGIC) {
          b_status = ER_NONIMAGE;
          __leave;
        }

        if ((header.ras_type != RT_OLD) &&
            (header.ras_type != RT_STANDARD) &&
            (header.ras_type != RT_BYTE_ENCODED) &&
            (header.ras_type != RT_FORMAT_RGB)) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        if ((header.ras_width == 0) || (header.ras_height == 0) || (header.ras_depth == 0)) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        switch (header.ras_depth) {
        case  1:
        case  8:
        case  24:
        case  32:
          break;

        default:
          b_status = ER_BADIMAGE;
          __leave;  /* �������֧���������ָ�ʽ֮���ͼ��*/
        }

        pinfo_str->imgtype    = IMT_RESSTATIC;  /* ͼ���ļ����� */
        pinfo_str->imgformat  = IMF_RAS;      /* ͼ���ļ���ʽ����׺���� */

        switch (header.ras_type) {        /* ͼ���ѹ����ʽ */
        case RT_OLD:
        case RT_STANDARD:
        case RT_FORMAT_RGB:
          pinfo_str->compression = ICS_RGB;
          break;

        case RT_BYTE_ENCODED:
          pinfo_str->compression = ICS_RLE8;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          __leave;
        }

        pinfo_str->width  = header.ras_width;
        pinfo_str->height = header.ras_height;
        pinfo_str->order  = 0;
        pinfo_str->bitcount = header.ras_depth;

        switch (pinfo_str->bitcount) {
        case  1:
        case  8:
          SETMASK_8(pinfo_str);
          break;

        case  24:
          SETMASK_24(pinfo_str);
          break;

        case  32:
          SETMASK_32(pinfo_str);
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          __leave;
        }

        pinfo_str->data_state = 1;
      }

      isrle = 0;
      isrgb = 0;

      /* �趨�������ݴ�ŷ�ʽ���Ƿ�ѹ������RGB�ŷ�˳��*/
      switch (header.ras_type) {
      case RT_OLD:
      case RT_STANDARD:   /* ��ѹ����BGR���� */
        break;

      case RT_BYTE_ENCODED: /* ѹ�� */
        isrle = 1;
        break;

      case RT_FORMAT_RGB:   /* ��ѹ����RGB���� */
        isrgb = 1;
        break;

      case RT_FORMAT_TIFF:  /* ��֧�ֵĴ洢��ʽ */
      case RT_FORMAT_IFF:
      default:
        b_status = ER_BADIMAGE;
        __leave;
        break;
      }


      /* ���õ�ɫ������ */
      switch (pinfo_str->bitcount) {
      case  1:
      case  8:
        pinfo_str->pal_count = numcolors = 1 << header.ras_depth;

        switch (header.ras_maptype) {
        case RMT_NONE:      /* �˹��ϳɻҶȵ�ɫ�� */
          pal = (LPPALITEM)pinfo_str->palette;

          for (i = 0; i < numcolors; i++) {
            c = (unsigned char)((255 * i) / (numcolors - 1));

            pal[i].red   = c;
            pal[i].green = c;
            pal[i].blue  = c;
            pal[i].reserved = 0;
          }

          break;

        case RMT_EQUAL_RGB:   /* ��ȡ��ɫ������ */
          pal = (LPPALITEM)pinfo_str->palette;

          /* �ж�ͼ���ɫ�峤�����ֵ�Ƿ�Ϸ� */
          if (header.ras_maplength > (3 * numcolors * sizeof(unsigned char))) {
            b_status = ER_BADIMAGE;
            __leave;
            break;
          }

          /* �������ܷ����ɫ�建���� */
          if ((r = (unsigned char*)isirw_malloc(3 * numcolors * sizeof(unsigned char))) == 0) {
            b_status = ER_MEMORYERR;
            __leave;
            break;
          }

          /* ����ʵ�ʵ�ɫ������� */
          numcolors = header.ras_maplength / 3;

          g = r + numcolors;
          b = g + numcolors;

          /* �����ɫ������ */
          if (isio_read((void*)r, header.ras_maplength, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
            break;
          }

          for (i = 0; i < numcolors; i++) {
            pal[i].red   = r[i];
            pal[i].green = g[i];
            pal[i].blue  = b[i];
            pal[i].reserved = 0;
          }

          break;

        case RMT_RAW:     /* �������ɫ������ */

          /* δ�ܸ������ֵ�ɫ��ĸ�ʽ����ʱ���� */
          if ((colormap = (unsigned char*)isirw_malloc(header.ras_maplength * sizeof(unsigned char))) == 0) {
            b_status = ER_MEMORYERR;
            __leave;
            break;
          }

          if (isio_read((void*)colormap, header.ras_maplength, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
            break;
          }

          break;

        default:        /* �Ƿ��ĵ�ɫ���ǩֵ */
          b_status = ER_BADIMAGE;
          __leave;
          break;
        }

        break;

      case  24:
      case  32:

        /* ���ͼ���д��ڵ�ɫ�����ݣ���˵����һ����������ͼ�� */
        if (header.ras_maptype != RMT_NONE) {
          b_status = ER_BADIMAGE;
          __leave;
          break;
        }

        pinfo_str->pal_count = 0;

        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }


      /* ȡ��ɨ���гߴ� */
      linesize = DIBSCANLINE_WIDTHBYTES(pinfo_str->width * pinfo_str->bitcount);

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

      /* ��ʼ�����׵�ַ���飨RASͼ��Ϊ���� */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + (i * linesize));
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

      if (header.ras_depth == 1) {
        linelength = (int)((header.ras_width / 8) + (header.ras_width % 8 ? 1 : 0));
      }
      else {
        linelength = (int)header.ras_width;
      }

      /* RASɨ����Ϊ˫�ֽڶ��� */
      fill = (linelength % 2) ? 1 : 0;


      /* ��ʼ����ȡ���� */
      _read_data(pfile, 0, 0, 0, 1);

      /* ���ж��� */
      switch (pinfo_str->bitcount) {
      case 1:
      case 8:
        for (i = 0; i < (int)pinfo_str->height; i++) {
          bits = pinfo_str->p_bit_data + i * linesize;

          /* ����һ�е����� */
          if ((b_status = _read_data(pfile, bits, linelength, isrle, 0)) != ER_SUCCESS) {
            __leave;
            break;
          }

          /* ����еĻ�����ȡ��β����ֽ� */
          if (fill) {
            if ((b_status = _read_data(pfile, (unsigned char*)&fillchar, fill, isrle, 0)) != ER_SUCCESS) {
              __leave;
              break;
            }
          }

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

        break;

      case 24:

        /* ���仺���� */
        if ((buf = (unsigned char*)isirw_malloc(pinfo_str->width * 3)) == 0) {
          b_status = ER_MEMORYERR;
          __leave;
          break;
        }

        for (i = 0; i < (int)pinfo_str->height; i++) {
          bits = pinfo_str->p_bit_data + i * linesize;

          if ((b_status = _read_data(pfile, buf, pinfo_str->width * 3, isrle, 0)) != ER_SUCCESS) {
            __leave;
            break;
          }

          bp = buf;

          for (j = 0; j < (int)pinfo_str->width; j++) {
            bits[2] = *bp++; /* red */
            bits[1] = *bp++; /* green */
            bits[0] = *bp++; /* blue */

            bits += 3;
          }

#if 0

          /* ������δ�����ʵ��ͼ���ʽ��ͻ��isrgbΪ��ʱ������ΪRGB���У�*/
          /* ��ʱ������δ��� */
          if (isrgb) {  /* RGB���� */
            for (j = 0; j < (int)pinfo_str->width; j++) {
              bits[2] = *bp++; /* red */
              bits[1] = *bp++; /* green */
              bits[0] = *bp++; /* blue */

              bits += 3;
            }
          }
          else {    /* BGR���� */
            for (j = 0; j < (int)pinfo_str->width; j++) {
              bits[0] = *bp++; /* blue */
              bits[1] = *bp++; /* green */
              bits[2] = *bp++; /* red */

              bits += 3;
            }
          }

#endif

          if (fill) {
            if ((b_status = _read_data(pfile, (unsigned char*)&fillchar, fill, isrle, 0)) != ER_SUCCESS) {
              __leave;
              break;
            }
          }

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

        break;

      case 32:

        /* ���仺���� */
        if ((buf = (unsigned char*)isirw_malloc(pinfo_str->width * 4)) == 0) {
          b_status = ER_MEMORYERR;
          __leave;
          break;
        }

        for (i = 0; i < (int)pinfo_str->height; i++) {
          bits = pinfo_str->p_bit_data + i * linesize;

          if ((b_status = _read_data(pfile, buf, pinfo_str->width * 4, isrle, 0)) != ER_SUCCESS) {
            __leave;
            break;
          }

          bp = buf;

          for (j = 0; j < (int)pinfo_str->width; j++) {
            bits[3] = *bp++; /* empty */
            bits[2] = *bp++; /* red */
            bits[1] = *bp++; /* green */
            bits[0] = *bp++; /* blue */

            bits += 4;
          }

#if 0

          /* ע��ԭ����� */
          if (isrgb) {  /* RGB���� */
            for (j = 0; j < (int)pinfo_str->width; j++) {
              bits[3] = *bp++; /* empty */
              bits[2] = *bp++; /* red */
              bits[1] = *bp++; /* green */
              bits[0] = *bp++; /* blue */

              bits += 4;
            }
          }
          else {    /* BGR���� */
            for (j = 0; j < (int)pinfo_str->width; j++) {
              bits[3] = *bp++; /* empty */
              bits[0] = *bp++; /* blue */
              bits[1] = *bp++; /* green */
              bits[2] = *bp++; /* red */

              bits += 4;
            }
          }

#endif

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

      pinfo_str->data_state = 2;
    }
    __finally {
      if (buf) {
        isirw_free(buf);
      }

      if (colormap) {
        isirw_free(colormap);
      }

      if (r) {
        isirw_free(r);
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

      LeaveCriticalSection(&ras_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* ����ͼ�� */
RAS_API int CALLAGREEMENT ras_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       i, j, wlen;
  unsigned char* bits, *pbuf = 0;
  SUNHeaderinfo header;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&ras_save_img_critical);

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
      if (_ras_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* �������ͬ��������������������ȷ����ʾ */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ��дͷ�ṹ */
      header.ras_magic     = RAS_MAGIC;
      header.ras_width     = (int)pinfo_str->width;
      header.ras_height    = (int)pinfo_str->height;
      header.ras_depth     = (int)pinfo_str->bitcount;
      header.ras_length    = (header.ras_width * 3 + ((header.ras_width * 3) % 2)) * header.ras_height;
      header.ras_type      = RT_STANDARD;
      header.ras_maptype   = RMT_NONE;
      header.ras_maplength = 0;

      /* ת��68K���� */
      header.ras_magic     = EXCHANGE_DWORD((header.ras_magic));
      header.ras_width     = EXCHANGE_DWORD((header.ras_width));
      header.ras_height    = EXCHANGE_DWORD((header.ras_height));
      header.ras_depth     = EXCHANGE_DWORD((header.ras_depth));
      header.ras_length    = EXCHANGE_DWORD((header.ras_length));
      header.ras_type      = EXCHANGE_DWORD((header.ras_type));
      header.ras_maptype   = EXCHANGE_DWORD((header.ras_maptype));
      header.ras_maplength = EXCHANGE_DWORD((header.ras_maplength));

      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* д��ͷ�ṹ��Ŀ���� */
      if (isio_write((const void*)&header, sizeof(SUNHeaderinfo), 1, pfile) == 0) {
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

      wlen = pinfo_str->width * 3 + ((pinfo_str->width * 3) % 2);

      /* ����д��������ɨ���У�*/
      if ((pbuf = (unsigned char*)isirw_malloc(wlen + 4)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* д��ͼ�����ݣ������ͼ�����ݱ�����ISeeλ����ʽ��������ܻ����ʧ�棩*/
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        bits = (unsigned char*)pinfo_str->pp_line_addr[i];

        /* �ߵ�R��B������ע���ĵ�������ʵ��ͼ���ʽ����������ACDSee��ʾЧ��Ϊ׼ */
        for (j = 0; j < (int)pinfo_str->width; j++) {
          pbuf[j * 3 + 0] = bits[j * 3 + 2];
          pbuf[j * 3 + 1] = bits[j * 3 + 1];
          pbuf[j * 3 + 2] = bits[j * 3 + 0];
        }

        /* д��Ŀ���� */
        if (isio_write((const void*)pbuf, wlen, 1, pfile) == 0) {
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
      if (pbuf) {
        isirw_free(pbuf);
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

      LeaveCriticalSection(&ras_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


