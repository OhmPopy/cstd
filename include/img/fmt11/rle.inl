// Utah RLEͼ���дģ��ʵ���ļ�
// ��ȡ���ܣ��ɶ�ȡ8λ��24λ��32λRLEͼ�������������ɫ��ͨ����һ��ALPHAͨ����
// ���湦�ܣ��ɽ�8λ��24λͼ�󱣴�ΪRLEͼ�󣨲�ѹ����ʽ��

/* �ļ��б��־ */
#define RLE_MAGIC           ((unsigned short)0xcc52)

/* ���������б�λ */
#define     LONGOPCODE      0x40

/* RLE�������붨�� */
#define     RLE_SKIPLINE_OP   1
#define     RLE_SETCOLOR_OP   2
#define     RLE_SKIPPIXELS_OP 3
#define     RLE_BYTEDATA_OP   5
#define     RLE_RUNDATA_OP    6
#define     RLE_EOF_OP      7

#define     RLE_CLEARFIRST_F  0x1   /* ������õĻ�����ѹǰ�������������ʹ�ñ���ɫ�� */
#define     RLE_NO_BACKGROUND_F 0x2   /* ������õĻ���������δ�ṩ����ɫ��Ϣ */
#define     RLE_ALPHA_F     0x4   /* ������õĻ����������д���ALPHAͨ����ͨ����-1�� */
#define     RLE_COMMENT_F   0x8   /* ������õĻ����������д���ע����Ϣ */

#define   RLE_OPCODE(s)   (s[0] & ~(unsigned char)LONGOPCODE)
#define   RLE_LONGP(s)    (s[0] & (unsigned char)LONGOPCODE)
#define   RLE_DATUM(s)    (s[1] & (unsigned char)0xff)


/* RLEͷ�ṹ���� */
typedef struct {
  unsigned short  magic;        /* RLE�б��־��0xcc52�� */
  unsigned short  xpos;       /* ͼ���x��yԭ�����꣨���½ǣ���һ���ޣ�*/
  unsigned short  ypos;
  unsigned short  width;        /* ͼ��Ŀ�Ⱥ͸߶ȣ����ص�λ��*/
  unsigned short  height;
  unsigned char flags;        /* ��־λ */
  unsigned char channels;     /* ��ɫͨ������������ALPHAͨ�� */
  unsigned char chan_bits;      /* ÿ����ɫͨ����λ��ȣ���ǰ��Чֵֻ��8��*/
  unsigned char pal_chan_count;   /* ��ɫ�����ͨ���� */
  unsigned char pal_chan_bits;    /* ��ɫ��ͨ����λ��� */
} RLE_HEADER, *LPRLE_HEADER;


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* ����ɨ���гߴ�(���ֽڶ���) */
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* �жϴ����ͼ���Ƿ���Ա����棨�粻֧�ֱ��湦�ܣ��ɽ�������ȥ���� */
int CALLAGREEMENT _rle_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(rle_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* ��֧�ֵ�λ��ͼ�� */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (rle_irwp_info.irwp_save.img_num) {
    if (rle_irwp_info.irwp_save.img_num == 1) {
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* ͼ���������ȷ */
      }
    }
  }

  return 0;
}


#define RLE_UNKNOW    0
#define RLE_REPEAT    1
#define RLE_NONREPEAT 2


/* ѹ��һ������ */
/* �ú���ʵ���ϲ�δѹ�����ݣ���Ϊ����ѹ������ԭʼ�������õĻ��ͻ������Ч�� */
static int CALLAGREEMENT _rle_pack(unsigned char* psou, int len, unsigned char* pdec)
{
  assert(psou && len && pdec);

  *pdec++ = RLE_BYTEDATA_OP | LONGOPCODE;
  *pdec++ = 0;
  *(unsigned short*)pdec = (unsigned short)(len - 1);

  pdec += 2;

  memcpy((void*)pdec, (const void*)psou, len);

  return (len + 2 + 2 + 1) & 0xfffffffe; /* ѹ��������ݳ��ȣ�ż�ֽڶ��� */
}




/* ��ȡͼ��λ���� */
RLE_API int CALLAGREEMENT rle_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       i, j, k, linesize, end_mark;

  RLE_HEADER    rle_header;
  char      bg[4], filler, bgmark;
  unsigned short*  pal_buf = 0, comm_len;
  unsigned char* pcomm = 0, *p, oper[2], *pbuf = 0;
  unsigned int  xinc;
  unsigned short  skiplines, skipcol, nc, cor;
  unsigned char currchann;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&rle_load_img_critical);

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

      if (isio_read((void*)&rle_header, sizeof(RLE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        if ((rle_header.magic != RLE_MAGIC) || \
            ((rle_header.channels != 1) && (rle_header.channels != 3)) || \
            (rle_header.chan_bits != 8)) {
          b_status = ER_NONIMAGE;
          __leave;
        }

        pinfo_str->imgtype    = IMT_RESSTATIC;  /* ͼ���ļ����� */
        pinfo_str->imgformat  = IMF_RLE;      /* ͼ���ļ���ʽ����׺���� */

        /* ��дͼ����Ϣ */
        pinfo_str->width  = rle_header.width;
        pinfo_str->height = rle_header.height;
        pinfo_str->order  = 1;

        if (rle_header.flags & RLE_ALPHA_F) {
          if (rle_header.channels == 1) {
            b_status = ER_NONIMAGE;
            __leave;
          }
          else {
            assert(rle_header.channels == 3);
            pinfo_str->bitcount = (rle_header.channels + 1) * 8;
          }

          assert(pinfo_str->bitcount == 32);
        }
        else {
          pinfo_str->bitcount =  rle_header.channels * 8;
        }

        pinfo_str->compression = ICS_RLE8;

        /* ����ͼ���������� */
        switch (pinfo_str->bitcount) {
        case  8:
          pinfo_str->r_mask = 0;
          pinfo_str->g_mask = 0;
          pinfo_str->b_mask = 0;
          pinfo_str->a_mask = 0;
          break;

        case  24:
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->b_mask = 0xff;
          pinfo_str->a_mask = 0;
          break;

        case  32:
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->b_mask = 0xff;
          pinfo_str->a_mask = 0xff000000;
          break;
        }

        pinfo_str->data_state = 1;
      }

      /* ��ȡ����ɫ��Ϣ */
      if (rle_header.flags & RLE_NO_BACKGROUND_F) {
        /* �������ֽ� */
        if (isio_read((void*)&filler, 1, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        bgmark = 0;
      }
      else {
        assert((rle_header.channels == 3) || (rle_header.channels == 1));

        memset((void*)bg, 0, 4);

        /* ��ȡ����ɫ */
        if (isio_read((void*)bg, rle_header.channels, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ȷ���Ƿ���Ҫ������䱳��ɫ */
        if (rle_header.flags & RLE_CLEARFIRST_F) {
          bgmark = 1;
        }
        else {
          bgmark = 0;
        }
      }

      /* ���õ�ɫ������ */
      if (pinfo_str->bitcount <= 8) {
        assert(rle_header.channels == 1);
        assert(pinfo_str->bitcount == 8);   /* ��ǰֻ֧��8λ����ͼ */

        if ((rle_header.pal_chan_count != 3) || (rle_header.pal_chan_bits != 8)) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* �����ɫ�����ݻ����� */
        if ((pal_buf = (unsigned short*)isirw_malloc(256 * 3 * 2)) == 0) { /* 256�3ͨ����2�ֽ� */
          b_status = ER_MEMORYERR;
          __leave;
        }

        /* ��ȡ��ɫ������ */
        if (isio_read((void*)pal_buf, (256 * 3 * 2), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ת�������ݰ� */
        for (i = 0; i < 256; i++) {
          pinfo_str->palette[i] = (((unsigned long)(pal_buf[i] >> 8)) << 16) | \
              (((unsigned long)(pal_buf[256 + i] >> 8)) << 8) | \
              ((unsigned long)(pal_buf[512 + i] >> 8));
        }

        pinfo_str->pal_count = 256;
      }
      else {
        if (rle_header.pal_chan_count != 0) {
          b_status = ER_BADIMAGE;
          __leave;  /* ������ͼ��Ӧ���ڵ�ɫ������ */
        }

        pinfo_str->pal_count = 0;
      }


      /* ��ȡע����Ϣ */
      if (rle_header.flags & RLE_COMMENT_F) {
        /* ��ȡע����Ϣ����ֵ��2�ֽڣ�*/
        if (isio_read((void*)&comm_len, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ż������ */
        comm_len = (comm_len + 1) & (unsigned short)0xfffe;

        if (comm_len) {
          /* ����ע����Ϣ������ */
          if ((pcomm = (unsigned char*)isirw_malloc(comm_len)) == 0) {
            b_status = ER_MEMORYERR;
            __leave;
          }

          /* ��ע����Ϣ */
          if (isio_read((void*)pcomm, comm_len, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
          }
        }
      }


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

      /* ��д���׵�ַ���飨����ͼ��*/
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
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

      /* ��䱳��ɫ */
      if (bgmark) {
        k = (int)pinfo_str->bitcount / 8;

        for (i = 0; i < (int)pinfo_str->height; i++) {
          for (j = 0; j < (int)pinfo_str->width; j++) {
            memcpy((void*)((unsigned char*)pinfo_str->pp_line_addr[i] + j * k), (const void*)bg, k);
          }
        }
      }

      /* ����ɨ���л����� */
      if ((pbuf = (unsigned char*)isirw_malloc(pinfo_str->width * 3 + 4)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      currchann = 0;
      nc = 0;
      k = pinfo_str->bitcount / 8;
      end_mark = 0;

      /* ���벢��ѹ�������������ݰ� */
      for (i = 0; i < (int)pinfo_str->height; i += skiplines) {
        if (end_mark) {
          b_status = ER_BADIMAGE;
          __leave;      /* Դͼ��ȱ������ */
        }

        skiplines = 0;
        xinc = 0;

        p = pinfo_str->p_bit_data + i * linesize;

        while (!skiplines) {
          /* ���Դ�����Ƿ�Ϸ� */
          if (xinc > pinfo_str->width) {
            b_status = ER_BADIMAGE;
            __leave;
          }

          /* ��ȡ�����������������͵��ֽڲ�������*/
          if (isio_read((void*)oper, 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
          }

          /* �ֽⲢִ������ */
          switch (RLE_OPCODE(oper)) {
          case  RLE_SKIPLINE_OP:    /* 1 �� ����ָ���� */
            if (RLE_LONGP(oper)) {
              if (isio_read((void*)&skiplines, 2, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                __leave;
                break;
              }
            }
            else {
              skiplines = (unsigned short)RLE_DATUM(oper);
            }

            break;

          case  RLE_SETCOLOR_OP:    /* 2 �� ���õ�ǰͨ�� */
            currchann = RLE_DATUM(oper);

            switch (k) {
            case  1:
              if (currchann != 0) { /* ֻ����0��ͨ������ */
                b_status = ER_BADIMAGE;
                __leave;
                break;
              }

              break;

            case  3:
              if (currchann == 0) { /* ����ǰͨ��ֵתΪ����д��λ�� */
                currchann = 2;
              }
              else if (currchann == 1) {
                currchann = 1;
              }
              else if (currchann == 2) {
                currchann = 0;
              }
              else {        /* ������������ͨ���� */
                b_status = ER_BADIMAGE;
                __leave;
                break;
              }

              break;

            case  4:
              if (currchann == 0) {
                currchann = 2;
              }
              else if (currchann == 1) {
                currchann = 1;
              }
              else if (currchann == 2) {
                currchann = 0;
              }
              else if (currchann == (unsigned char)255) { /* ALPHAͨ��תΪ����д��λ�� */
                currchann = 3;
              }
              else {
                b_status = ER_BADIMAGE;
                __leave;
                break;
              }

              break;

            default:
              assert(0);
              b_status = ER_SYSERR;
              __leave;
              break;
            }

            assert(currchann <= 3);

            xinc = 0;         /* ����������0 */

            break;

          case  RLE_SKIPPIXELS_OP:    /* 3 �� ����ָ������������ */
            if (RLE_LONGP(oper)) {
              if (isio_read((void*)&skipcol, 2, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                __leave;
                break;
              }
            }
            else {
              skipcol = (unsigned short)RLE_DATUM(oper);
            }

            /* ��ֹ����д��� */
            if ((xinc + (unsigned int)skipcol) > pinfo_str->width) {
              b_status = ER_BADIMAGE;
              __leave;
              break;
            }

            xinc += (unsigned int)skipcol;
            break;

          case  RLE_BYTEDATA_OP:    /* 5 �� δѹ������ */
            if (RLE_LONGP(oper)) {
              if (isio_read((void*)&nc, 2, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                __leave;
                break;
              }
            }
            else {
              nc = RLE_DATUM(oper);
            }

            nc++;

            /* ����δѹ�����ݣ�������ܣ�Ҳ��������ֽڣ� */
            if (isio_read((void*)pbuf, ((nc + 1) / 2) * 2, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;
              break;
            }

            /* ��ֹ����д��� */
            if ((xinc + (unsigned int)nc) > pinfo_str->width) {
              b_status = ER_BADIMAGE;
              __leave;
              break;
            }

            /* д���������ݰ� */
            for (j = 0; j < (int)(unsigned int)nc; j++) {
              p[k * (xinc + j) + (int)(unsigned int)currchann] = pbuf[j];
            }

            xinc += (unsigned int)nc;
            break;

          case  RLE_RUNDATA_OP:     /* 6 �� ѹ������ */
            if (RLE_LONGP(oper)) {
              if (isio_read((void*)&nc, 2, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                __leave;
                break;
              }
            }
            else {
              nc = RLE_DATUM(oper);
            }

            nc++;

            /* ��ѹ���ֽڣ���һ�ֽڵ�����룩*/
            if (isio_read((void*)&cor, 2, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;
              break;
            }

            /* ��ֹ����д��� */
            if ((xinc + (unsigned int)nc) > pinfo_str->width) {
              b_status = ER_BADIMAGE;
              __leave;
              break;
            }

            /* д���������ݰ� */
            for (j = 0; j < (int)(unsigned int)nc; j++) {
              p[k * (xinc + j) + (int)(unsigned int)currchann] = (unsigned char)cor;
            }

            xinc += (unsigned int)nc;
            break;

          case  RLE_EOF_OP:       /* 7 �� ���ս�� */
            skiplines = 1;        /* ��ֵ��ʹwhileѭ����ֹ */
            end_mark  = 1;
            break;

          default:
            b_status = ER_BADIMAGE;
            __leave;
            break; /* δ����Ĳ����� */
          }
        }

        /* ���ȵ��� */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, i, skiplines, IRWE_CALLBACK_FUNID_LOAD)) {
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

      if (pcomm) {
        isirw_free(pcomm);
      }

      if (pal_buf) {
        isirw_free(pal_buf);
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

      LeaveCriticalSection(&rle_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* ����ͼ�� */
RLE_API int CALLAGREEMENT rle_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;

  RLE_HEADER    rle_header;
  unsigned char filler = 0;
  unsigned short  commlen, skipline, chann;
  int       i, j, l, k, packlen;
  unsigned char* psou = 0, *pdec = 0, *p;

  static char*       ptag = "manufacturer=ISee";  /* ע�ʹ���17�ֽ� */
  static unsigned short pal[3][256];          /* ��ɫ������ */

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&rle_save_img_critical);

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
      if (_rle_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* �������ͬ��������������������ȷ����ʾ */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ��дͷ�ṹ */
      rle_header.magic    = (unsigned short)RLE_MAGIC;
      rle_header.xpos     = 0;
      rle_header.ypos     = 0;
      rle_header.width    = (unsigned short)pinfo_str->width;
      rle_header.height   = (unsigned short)pinfo_str->height;
      rle_header.channels = (unsigned char)pinfo_str->bitcount / 8;
      assert((rle_header.channels == 1) || (rle_header.channels == 3)); /* ֻ����8λ��24λͼ�� */
      rle_header.chan_bits = 8;
      rle_header.flags    = RLE_NO_BACKGROUND_F | RLE_COMMENT_F;

      if (pinfo_str->bitcount == 8) {
        rle_header.pal_chan_count = 3;
        rle_header.pal_chan_bits  = 8;
      }
      else {
        rle_header.pal_chan_count = 0;
        rle_header.pal_chan_bits  = 8;
      }

      /* д��ͷ�ṹ */
      if (isio_write((const void*)&rle_header, sizeof(RLE_HEADER), 1, pfile) == 0) {
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

      /* д������ֽ� */
      if (isio_write((const void*)&filler, 1, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* д���ɫ������ */
      if (rle_header.pal_chan_count) {
        assert(pinfo_str->bitcount == 8);

        /* �ֽ�ԭ��ɫ������ */
        for (i = 0; i < 3; i++) {
          for (j = 0; j < 256; j++) {
            pal[i][j] = (unsigned short)(((pinfo_str->palette[j] >> ((2 - i) * 8)) & 0xff) << 8);
          }
        }

        /* д��ͨ�������ĵ�ɫ������ */
        for (i = 0; i < 3; i++) {
          if (isio_write((const void*)pal[i], 256 * 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
          }
        }
      }


      commlen = strlen(ptag) + 1; /* ����ע�ʹ����ȣ�������β0����*/

      /* д��ע����Ϣ */
      if (isio_write((const void*)&commlen, 2, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (isio_write((const void*)ptag, commlen, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ȷ��ż�ֽڶ��� */
      if (commlen & 0x1) {
        /* д������ֽ� */
        if (isio_write((const void*)&filler, 1, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }
      }

      /* ����ѹ�������� */
      if ((psou = (unsigned char*)isirw_malloc(pinfo_str->width + 4)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      if ((pdec = (unsigned char*)isirw_malloc(pinfo_str->width * 3 + 4)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      k = pinfo_str->bitcount / 8;

      /* ����д������ */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        /* �Ե���ͼ��ʽ����ͼ�� */
        p = (unsigned char*)pinfo_str->pp_line_addr[pinfo_str->height - i - 1];

        for (j = 0; j < k; j++) {
          chann = ((j & 0xff) << 8) | (RLE_SETCOLOR_OP & 0xff);

          /* д��ͨ��ֵ */
          if (isio_write((const void*)&chann, 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
          }

          /* ��ȡͨ������ */
          for (l = 0; l < (int)pinfo_str->width; l++) {
            psou[l] = p[k * l + (k - 1 - j)];
          }

          packlen = _rle_pack(psou, (int)pinfo_str->width, pdec);

          /* д��ͨ������ */
          if (isio_write((const void*)pdec, packlen, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
          }
        }

        if ((i + 1) == (int)pinfo_str->height) { /* ���һ���Ѿ�д�� */
          skipline = RLE_EOF_OP;

          /* д��EOF��־ */
          if (isio_write((const void*)&skipline, 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
          }
        }
        else {
          skipline = (0x1 << 8) | (RLE_SKIPLINE_OP & 0xff);

          /* д��ͨ��ֵ */
          if (isio_write((const void*)&skipline, 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
          }
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
      if (psou) {
        isirw_free(psou);
      }

      if (pdec) {
        isirw_free(pdec);
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

      LeaveCriticalSection(&rle_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}



