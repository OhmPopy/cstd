// Alias/Wavefront RLAͼ���дģ��ʵ���ļ�
// ��ȡ���ܣ��ɶ�ȡ3ͨ��24λѹ��rgbɫϵ��ͼ�󣬲�֧��matte��auxͨ��
// ���湦�ܣ��ɽ�24λͼ�󱣴�Ϊ3ͨ��RLE8ѹ����rgbɫϵͼ��

/* �ļ��б��־ */
#define RLA_REVISION          0xfffe

/* ��������ṹ */
typedef struct {
  short left;
  short right;
  short bottom;
  short top;
} RLA_WINDOW_S, *LPRLA_WINDOW_S;


/* RLA��ͷ�ṹ���� */
typedef struct {
  RLA_WINDOW_S window;      /* ������������ */
  RLA_WINDOW_S active_window;   /* ͼ�󴰿����� */
  short frame;          /* ͼ��֡�� */
  short storage_type;       /* ͼ������ѹ������ */
  short num_chan;         /* ͼ��ͨ������������matte��auxͨ������һ��Ϊ3 */
  short num_matte;        /* matteͨ������һ��Ϊ1 */
  short num_aux;          /* ����ͨ���� */
  short revision;         /* ��ʽ�޶��ţ���ǰΪ0xfffe */
  char  gamma[16];        /* gammaֵ��һ��Ϊ2.2 */
  char  red_pri[24];
  char  green_pri[24];
  char  blue_pri[24];
  char  white_pt[24];
  long  job_num;
  char  name[128];
  char  desc[128];        /* ͼ���������� */
  char  program[64];        /* ����ͼ���������� */
  char  machine[32];        /* ����ͼ��Ļ������� */
  char  user[32];
  char  date[20];         /* ����ͼ������ڡ�ʱ�� */
  char  aspect[24];
  char  aspect_ratio[8];
  char  chan[32];         /* ����ɫϵ��һ��Ϊ"rgb" */
  short field;
  char  time[12];
  char  filter[32];
  short chan_bits;        /* ÿͨ����λ��� */
  short matte_type;
  short matte_bits;       /* matteͨ��λ��� */
  short aux_type;
  short aux_bits;         /* ����ͨ��λ��� */
  char  aux[32];
  char  space[36];
  long  next;           /* ��һ����ͼ�������ƫ�� */
} RLA_HEADER, *LPRLA_HEADER;



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* ����ɨ���гߴ�(���ֽڶ���) */
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* �жϴ����ͼ���Ƿ���Ա����棨�粻֧�ֱ��湦�ܣ��ɽ�������ȥ���� */
int CALLAGREEMENT _rla_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(rla_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* ��֧�ֵ�λ��ͼ�� */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (rla_irwp_info.irwp_save.img_num) {
    if (rla_irwp_info.irwp_save.img_num == 1) {
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* ͼ���������ȷ */
      }
    }
  }

  return 0;
}


/* ��RLA_HEADER�ṹ�е�short���������ת�� */
static void CALLAGREEMENT _swap_byte(LPRLA_HEADER prla)
{
  assert(prla);

  prla->window.bottom = EXCHANGE_WORD(prla->window.bottom);
  prla->window.left   = EXCHANGE_WORD(prla->window.left);
  prla->window.right  = EXCHANGE_WORD(prla->window.right);
  prla->window.top    = EXCHANGE_WORD(prla->window.top);

  prla->active_window.bottom = EXCHANGE_WORD(prla->active_window.bottom);
  prla->active_window.left   = EXCHANGE_WORD(prla->active_window.left);
  prla->active_window.right  = EXCHANGE_WORD(prla->active_window.right);
  prla->active_window.top    = EXCHANGE_WORD(prla->active_window.top);

  prla->frame         = EXCHANGE_WORD(prla->frame);
  prla->storage_type  = EXCHANGE_WORD(prla->storage_type);
  prla->num_chan      = EXCHANGE_WORD(prla->num_chan);
  prla->num_matte     = EXCHANGE_WORD(prla->num_matte);
  prla->num_aux       = EXCHANGE_WORD(prla->num_aux);
  prla->revision      = EXCHANGE_WORD(prla->revision);
  prla->job_num       = EXCHANGE_DWORD(prla->job_num);
  prla->field         = EXCHANGE_WORD(prla->field);
  prla->chan_bits     = EXCHANGE_WORD(prla->chan_bits);
  prla->matte_type    = EXCHANGE_WORD(prla->matte_type);
  prla->matte_bits    = EXCHANGE_WORD(prla->matte_bits);
  prla->aux_type      = EXCHANGE_WORD(prla->aux_type);
  prla->aux_bits      = EXCHANGE_WORD(prla->aux_bits);
  prla->next          = EXCHANGE_DWORD(prla->next);
}


/* ����һ��RLE8��ͨ��ѹ�����ݣ�һ��ɨ�����е�һ��ͨ���� */
static int CALLAGREEMENT _decode_scanline(unsigned char* src, int src_len, unsigned char* dec, int dec_len)
{
  unsigned char count;

  assert(src && dec && src_len && dec_len);

  while (src_len > 0) {
    /* ȡ����ֵ */
    count = *src++;
    src_len--;

    if (src_len == 0) {
      return 2;     /* Դ���ݲ��� */
    }

    if (count < (unsigned char)128) {
      count++;

      /* �Ϸ��Լ�� */
      if (((int)(unsigned int)count) > dec_len) {
        return 1;   /* Դ���ݷǷ� */
      }
      else {
        dec_len -= (int)(unsigned int)count;
      }

      /* �ظ������� */
      while (count-- > 0) {
        *dec++ = *src;
      }

      src++;
      src_len--;
    }
    else {
      /* ����δѹ�������� */
      for (count = (unsigned char)(256 - (int)(unsigned int)count); count > 0; src_len--, dec_len--, count--) {
        if (dec_len == 0) {
          return 1;
        }
        else {
          *dec++ = *src++;
        }
      }
    }
  }

  if (dec_len != 0) {
    return 3;     /* Դ������ȱ�� */
  }
  else {
    return 0;     /* �ɹ���ѹ�� */
  }
}


/* ѹ��һ��ͨ�������ݣ�һ��ɨ�����е�һ��ͨ���� */
static int CALLAGREEMENT _encode_scanline(unsigned char* src, int src_len, unsigned char* dec)
{
  int       len = 0;
  unsigned char count;

  assert(src && src_len && dec);

  while (src_len > 0) {
    if ((src_len > 1) && (src[0] == src[1])) {
      /* �ۼ��ظ��ֽڸ��� */
      for (count = 2; (int)(unsigned int)count < src_len; count++) {
        if (src[(int)(unsigned int)count] != src[(int)(unsigned int)count - 1]) {
          break;
        }

        if ((int)(unsigned int)count >= 127) {
          break;
        }
      }

      /* д�����ֵ */
      *dec++ = count - (unsigned char)1;
      len++;

      /* д���ظ�����ֵ */
      *dec++ = *src;
      len++;

      src_len -= (int)(unsigned int)count;
      src += (int)(unsigned int)count;
    }
    else {
      /* �ۼƲ��ظ��ֽڸ��� */
      for (count = 1; (int)(unsigned int)count < src_len; count++) {
        if (((src_len - (int)(unsigned int)count) > 1) && (src[(int)(unsigned int)count] == src[(int)(unsigned int)count + 1])) {
          break;
        }

        if ((int)(unsigned int)count >= 127) {
          break;
        }
      }

      /* д�����ֵ */
      *dec++ = (unsigned char)((int)256 - (int)(unsigned int)count);
      len++;

      /* ����δѹ������ */
      for (; count-- > 0; len++, src_len--) {
        *dec++ = *src++;
      }
    }
  }

  return len; /* ����ѹ�����ͨ�����ݳ��� */
}





/* ��ȡͼ��λ���� */
RLA_API int CALLAGREEMENT rla_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       i, linesize, j;
  unsigned int*  poffset = 0;
  unsigned char* punpack = 0, *pspl = 0, *p;
  unsigned char* pr, *pg, *pb;
  unsigned short  rl, gl, bl;

  RLA_HEADER    rlaheader;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&rla_load_img_critical);

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

      if (isio_read((void*)&rlaheader, sizeof(RLA_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ת������ */
      _swap_byte(&rlaheader);

      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        if (/* (rlaheader.revision != (short)RLA_REVISION) || */\
            ((rlaheader.matte_type != 0) && (rlaheader.matte_type != 4)) || \
            ((rlaheader.storage_type != 0) && (rlaheader.storage_type != 4)) || \
            (rlaheader.window.left > rlaheader.window.right) || \
            (rlaheader.window.bottom > rlaheader.window.top) || \
            (rlaheader.active_window.left > rlaheader.active_window.right) || \
            (rlaheader.active_window.bottom > rlaheader.active_window.top)) {
          b_status = ER_NONIMAGE;
          __leave;
        }

        if (strcmp((const char*)rlaheader.chan, "rgb")) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        if ((rlaheader.num_chan != 3) || (rlaheader.chan_bits != 8)) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        if (((rlaheader.active_window.right - rlaheader.active_window.left) < 0) || \
            ((rlaheader.active_window.top - rlaheader.active_window.bottom) < 0)) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        pinfo_str->imgtype    = IMT_RESSTATIC;  /* ͼ���ļ����� */
        pinfo_str->imgformat  = IMF_RLA;      /* ͼ���ļ���ʽ����׺���� */
        pinfo_str->compression  = ICS_RLE8;

        pinfo_str->width  = (unsigned long)(rlaheader.active_window.right - rlaheader.active_window.left + 1);
        pinfo_str->height = (unsigned long)(rlaheader.active_window.top - rlaheader.active_window.bottom + 1);
        pinfo_str->order  = 1;
        pinfo_str->bitcount = 24;         /* 3ͨ����8λ */

        pinfo_str->b_mask = 0xff;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->a_mask = 0x0;

        pinfo_str->data_state = 1;
      }

      /* �õ�ɫ����Ϊ 0 */
      pinfo_str->pal_count = 0;

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

      /* ��д���׵�ַ���� */
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

      /* �������ڴ洢ɨ����ƫ�Ʊ���ڴ� */
      if ((poffset = (int*)isirw_malloc(4 * pinfo_str->height)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* ��ȡƫ�Ʊ� */
      if (isio_read((void*)poffset, 4 * pinfo_str->height, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ת��ƫ��ֵ���� */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        poffset[i] = EXCHANGE_DWORD(poffset[i]);
      }

      /* ����ѹ�����ݻ����� */
      if ((punpack = (unsigned char*)isirw_malloc(pinfo_str->width * (pinfo_str->bitcount / 8) * 2)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      pr = punpack;
      pg = pr + pinfo_str->width * 2;
      pb = pg + pinfo_str->width * 2;

      /* �����ѹ������ */
      if ((pspl = (unsigned char*)isirw_malloc(pinfo_str->width * (pinfo_str->bitcount / 8))) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }


      /* ���ж��� */
      for (p = pinfo_str->p_bit_data, i = 0; i < (int)(pinfo_str->height); i++) {
        /* ��λɨ���ж�ȡλ�� */
        if (isio_seek(pfile, poffset[i], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ֻ��ȡǰ3��ͨ�����ݣ�R��G��B����matteͨ���ݲ���ȡ */

        /* ��R */
        if (isio_read((void*)&rl, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        rl = EXCHANGE_WORD(rl);

        if (isio_read((void*)pr, rl, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ��G */
        if (isio_read((void*)&gl, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        gl = EXCHANGE_WORD(gl);

        if (isio_read((void*)pg, gl, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ��B */
        if (isio_read((void*)&bl, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        bl = EXCHANGE_WORD(bl);

        if (isio_read((void*)pb, bl, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* �������ݺϷ��� */
        if ((rl == 0) || (gl == 0) || (bl == 0)) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* ��ѹ�� */
        if (_decode_scanline(pr, rl, pspl, (int)pinfo_str->width) != 0) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        if (_decode_scanline(pg, gl, pspl + pinfo_str->width, (int)pinfo_str->width) != 0) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        if (_decode_scanline(pb, bl, pspl + pinfo_str->width * 2, (int)pinfo_str->width) != 0) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        p = pinfo_str->p_bit_data + (linesize * i);

        /* �ϳ�Ŀ�����أ���д�����ݰ� */
        for (j = 0; j < (int)pinfo_str->width; j++) {
          *p++ = ((unsigned char*)(pspl + pinfo_str->width * 2))[j];
          *p++ = ((unsigned char*)(pspl + pinfo_str->width))[j];
          *p++ = ((unsigned char*)(pspl))[j];
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
      if (pspl) {
        isirw_free(pspl);
      }

      if (punpack) {
        isirw_free(punpack);
      }

      if (poffset) {
        isirw_free(poffset);
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

      LeaveCriticalSection(&rla_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* ����ͼ�� */
RLA_API int CALLAGREEMENT rla_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       i, j, off;
  unsigned int*  poffset = 0;
  unsigned char* ppack = 0, *pspl = 0, *p;
  unsigned char* pr, *pg, *pb;
  unsigned short  rl, gl, bl, l;

  RLA_HEADER    rlaheader;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&rla_save_img_critical);

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
      if (_rla_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* �������ͬ��������������������ȷ����ʾ */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      memset((void*)&rlaheader, 0, sizeof(RLA_HEADER));

      /* ��д��ͷ�ṹ */
      rlaheader.window.left   = 0;
      rlaheader.window.bottom = 0;
      rlaheader.window.right  = (short)(pinfo_str->width - 1);
      rlaheader.window.top    = (short)(pinfo_str->height - 1);

      rlaheader.active_window.left   = 0;
      rlaheader.active_window.bottom = 0;
      rlaheader.active_window.right  = (short)(pinfo_str->width - 1);
      rlaheader.active_window.top    = (short)(pinfo_str->height - 1);

      rlaheader.revision  = (short)RLA_REVISION;

      /* 3ͨ����R��G��B */
      strcpy(rlaheader.chan, "rgb");
      rlaheader.num_chan  = 3;
      rlaheader.chan_bits = 8;

      strcpy(rlaheader.desc, "Saved by ISee RLA plug-in.");
      strcpy(rlaheader.program, "ISee Explorer.");

      /* ͼ����� */
      rlaheader.frame = 1;

      strcpy(rlaheader.gamma, "2.2");

      strcpy(rlaheader.red_pri  , "0.670 0.330");
      strcpy(rlaheader.green_pri, "0.210 0.710");
      strcpy(rlaheader.blue_pri , "0.140 0.080");
      strcpy(rlaheader.white_pt , "0.310 0.316");

      strcpy(rlaheader.aspect, "user defined");

      /* תΪMOTO���� */
      _swap_byte(&rlaheader);

      /* д��ͷ�ṹ */
      if (isio_write((const void*)&rlaheader, sizeof(RLA_HEADER), 1, pfile) == 0) {
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

      /* ����ƫ�Ʊ� */
      if ((poffset = (unsigned int*)isirw_malloc(4 * pinfo_str->height)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* ����ѹ�����ݻ����� */
      if ((ppack = (unsigned char*)isirw_malloc(pinfo_str->width * (pinfo_str->bitcount / 8) * 2)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      pr = ppack;
      pg = pr + pinfo_str->width * 2;
      pb = pg + pinfo_str->width * 2;

      /* ���䵥ͨ�������� */
      if ((pspl = (unsigned char*)isirw_malloc(pinfo_str->width + 4)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* ��λͨ������д�����ʼ�㣨���ͷ�ṹ��ƫ�Ʊ� */
      if (isio_seek(pfile, sizeof(RLA_HEADER) + (4 * pinfo_str->height), SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      off = isio_tell(pfile);

      /* ����д�� */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        p = (unsigned char*)(pinfo_str->pp_line_addr[i]);

        poffset[(int)pinfo_str->height - i - 1] = EXCHANGE_DWORD(off);

        /* ��ȡ R ͨ������ */
        for (j = 0; j < (int)pinfo_str->width; j++) {
          pspl[j] = p[j * 3 + 2];
        }

        /* ѹ�� R ͨ������ */
        rl = (unsigned short)_encode_scanline(pspl, (int)pinfo_str->width, pr);

        /* ��ȡ G ͨ������ */
        for (j = 0; j < (int)pinfo_str->width; j++) {
          pspl[j] = p[j * 3 + 1];
        }

        /* ѹ�� G ͨ������ */
        gl = (unsigned short)_encode_scanline(pspl, (int)pinfo_str->width, pg);

        /* ��ȡ B ͨ������ */
        for (j = 0; j < (int)pinfo_str->width; j++) {
          pspl[j] = p[j * 3 + 0];
        }

        /* ѹ�� B ͨ������ */
        bl = (unsigned short)_encode_scanline(pspl, (int)pinfo_str->width, pb);


        /* д��ѹ��������ݣ�˳��Ϊ��R��G��B */
        l = EXCHANGE_WORD(rl);

        if (isio_write((const void*)&l, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_write((const void*)pr, rl, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        l = EXCHANGE_WORD(gl);

        if (isio_write((const void*)&l, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_write((const void*)pg, gl, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        l = EXCHANGE_WORD(bl);

        if (isio_write((const void*)&l, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_write((const void*)pb, bl, 1, pfile) == 0) {
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

        /* ��ȡ��һ�е�д��λ�� */
        off = isio_tell(pfile);
      }

      /* ��λƫ�Ʊ�д��� */
      if (isio_seek(pfile, sizeof(RLA_HEADER), SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* д��ƫ�Ʊ� */
      if (isio_write((const void*)poffset, 4 * pinfo_str->height, 1, pfile) == 0) {
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
      if (pspl) {
        isirw_free(pspl);
      }

      if (ppack) {
        isirw_free(ppack);
      }

      if (poffset) {
        isirw_free(poffset);
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

      LeaveCriticalSection(&rla_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


