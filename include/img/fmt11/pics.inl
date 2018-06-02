// SoftImage PICͼ���дģ��ʵ���ļ�
// ��ȡ���ܣ��ɶ�ȡ24λ��32λ����һ��ALPHAͨ������
// ѹ����δѹ�� SoftImage PIC ͼ��
// ���湦�ܣ��ɽ�24λͼ���Բ�ѹ���ķ�ʽ����Ϊ3ͨ��
// SoftImage PIC ͼ��


/* �ļ��б��־ */
#define PICS_MAGIC            0x5380F634
#define PICS_PICT_STR         0x54434950    /* 'PICT' �ĸ��ַ� */

/* �ļ�ͷ�ṹ���� */
typedef struct {
  unsigned int  magic;           /* PICͼ���ǩ             */
  float     version;         /* ʹ�õĸ�ʽ�汾��        */
  char      comment[80];     /* ͼ�������ִ�            */
  char      id[4];           /* "PICT"�ĸ��ַ�          */
  unsigned short  width;           /* ͼ���ȣ����ص�λ      */
  unsigned short  height;          /* ͼ��߶ȣ����ص�λ      */
  float     ratio;           /* ͼ��Ŀ�߱�            */
  unsigned short  fields;          /* ͼ���־��              */
  unsigned short  padding;         /* ����                    */
} PICS_HEADER, *LPPICS_HEADER;


/* ͨ����Ϣ�ṹ */
typedef struct {
  unsigned char chained;
  unsigned char size;     /* ÿͨ��λ�� */
  unsigned char type;     /* ѹ����ʽ */
  unsigned char channel;    /* ������Щͨ��(R��G��B��A...) */
} PICS_CHANNEL_INFO, *LPPICS_CHANNEL_INFO;



/* �������� */
#define PIC_UNSIGNED_INTEGER  0x00
#define PIC_SIGNED_INTEGER    0x10  /* δʵ�� */
#define PIC_SIGNED_FLOAT      0x20  /* δʵ�� */


/* ѹ������ */
#define PIC_UNCOMPRESSED      0x00
#define PIC_PURE_RUN_LENGTH   0x01
#define PIC_MIXED_RUN_LENGTH  0x02

/* ͨ������ */
#define PIC_RED_CHANNEL     0x80
#define PIC_GREEN_CHANNEL   0x40
#define PIC_BLUE_CHANNEL    0x20
#define PIC_ALPHA_CHANNEL   0x10
#define PIC_SHADOW_CHANNEL    0x08  /* δʵ�� */
#define PIC_DEPTH_CHANNEL   0x04  /* δʵ�� */
#define PIC_AUXILIARY_1_CHANNEL 0x02  /* δʵ�� */
#define PIC_AUXILIARY_2_CHANNEL 0x01  /* δʵ�� */



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* ����ɨ���гߴ�(���ֽڶ���) */
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* �жϴ����ͼ���Ƿ���Ա����棨�粻֧�ֱ��湦�ܣ��ɽ�������ȥ���� */
int CALLAGREEMENT _pics_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(pics_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* ��֧�ֵ�λ��ͼ�� */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (pics_irwp_info.irwp_save.img_num) {
    if (pics_irwp_info.irwp_save.img_num == 1) {
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* ͼ���������ȷ */
      }
    }
  }

  return 0;
}


/* ��ȡ���е�ͨ����Ϣ������4��ͨ������������Ϊ4ͨ��ͼ��*/
static enum EXERESULT CALLAGREEMENT _read_channel_info(ISFILE* pfile, LPPICS_CHANNEL_INFO* ppchannel, int* channel_count)
{
  int         i, j;
  LPPICS_CHANNEL_INFO p = 0;
  LPPICS_CHANNEL_INFO ptmp;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(pfile && ppchannel && channel_count);

  __try {
    *ppchannel = p;
    *channel_count = 0;

    if (isio_seek(pfile, sizeof(PICS_HEADER), SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      __leave;
    }

    if ((p = ptmp = (LPPICS_CHANNEL_INFO)isirw_malloc(sizeof(PICS_CHANNEL_INFO) * 3)) == 0) {
      b_status = ER_MEMORYERR;
      __leave;
    }

    j = i = 0;

    do {
      /* ��һ��ͨ������ */
      if (isio_read((void*)ptmp, sizeof(PICS_CHANNEL_INFO), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      i++;
      j++;

      /* ���ͨ�������Ƿ�Ϸ� */
      if ((ptmp->size != 8) || (ptmp->type > 0x2) || ((ptmp->channel != 0xe0) && (ptmp->channel != 0x10))) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      if (i > 2) {    /* ֻ����ǰ4��ͨ�������ݣ�RGB����ͨ������һ�� */
        i--;
        ptmp--;  /* ������ȡ��һ��ͨ�������ݣ��������� */

        if (j > 6) {  /* ����8��ͨ�����п����ǷǷ���PICͼ�� */
          b_status = ER_BADIMAGE;
          __leave;
        }
      }
    }
    while ((ptmp->chained != 0) && (ptmp++));

    *ppchannel    = p;  /* ����ͨ������ */
    *channel_count  = i;
  }
  __finally {
    if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
      if (p) {
        isirw_free(p);
      }
    }
  }

  return b_status;
}


/* ��ȡһ��ɨ���е����ݣ��������ɨ���е�����ͨ����*/
static unsigned long read_scanline(ISFILE* pfile,
    unsigned char* scan,
    unsigned long width,
    LPPICS_CHANNEL_INFO channel,
    unsigned long chan_count)
{
  int       i;
  unsigned long noCol, status = 0;
  unsigned long off[4];
  unsigned long bytes;

  assert(pfile && scan && width && channel && chan_count);
  assert((chan_count == 1) || (chan_count == 2));

  bytes = (chan_count == 1) ? 3 : 4;

  for (i = 0; i < (int)chan_count; i++) {
    noCol = 0;

    /* ȷ��R��G��B��������ISee���е��������е�λ�� */
    if (channel[i].channel & PIC_RED_CHANNEL) {
      off[noCol] = 2;
      noCol++;
    }

    if (channel[i].channel & PIC_GREEN_CHANNEL) {
      off[noCol] = 1;
      noCol++;
    }

    if (channel[i].channel & PIC_BLUE_CHANNEL) {
      off[noCol] = 0;
      noCol++;
    }

    if (channel[i].channel & PIC_ALPHA_CHANNEL) {
      off[noCol] = 3;
      noCol++;
    }

    /* ����ѹ����ʽ������Ӧ�Ķ�ȡ���� */
    switch (channel[i].type & 0xf) {
    case PIC_UNCOMPRESSED:    /* δѹ�� */
      status = channel_read_raw(pfile, scan, width, noCol, off, bytes);
      break;

    case PIC_PURE_RUN_LENGTH: /* ��׼RLE8ѹ�� */
      status = channel_read_pure(pfile, scan, width, noCol, off, bytes);
      break;

    case PIC_MIXED_RUN_LENGTH:  /* ���RLE8ѹ���� */
      status = channel_read_mixed(pfile, scan, width, noCol, off, bytes);
      break;

    default:          /* ϵͳ�쳣 */
      assert(0);
      return 0;
    }

    if (status != 0) {      /* ���������ֹͣ��ȡ���� */
      break;
    }
  }

  return status;
}


/* ��һδѹ������������ */
static unsigned long channel_read_raw(ISFILE* file,
    unsigned char* scan,
    unsigned long width,
    unsigned long noCol,
    unsigned long* off,
    unsigned long bytes)
{
  int     i, j;

  for (i = 0; i < (int)width; i++) {
    if (isio_eof(file)) {
      return 1;   /* Դ����ȱ�� */
    }

    for (j = 0; j < (int)noCol; j++) {
      /* ��ISee����ʽ���ж�������ط��� */
      if (isio_read((void*) & (scan[off[j]]), 1, 1, file) == 0) {
        return 2; /* ����д�쳣 */
      }
    }

    scan += bytes;
  }

  return 0;
}


/* ��һ��׼RLEѹ������������ */
static unsigned long channel_read_pure(ISFILE* file,
    unsigned char* scan,
    unsigned long width,
    unsigned long noCol,
    unsigned long* off,
    unsigned long bytes)
{
  unsigned char col[4];
  unsigned long count = 0;
  int       i, j, k;

  for (i = (int)width; i > 0;) {
    /* �������ֵ */
    if (isio_read((void*)&count, 1, 1, file) == 0) {
      return 1;   /* Դ����ȱ�� */
    }

    /* �ݴ��� */
    if (count > width) {
      count = width;
    }

    i -= (int)count;

    if (isio_eof(file)) {
      return 1;
    }

    /* �����ظ����� */
    for (j = 0; j < (int)noCol; j++) {
      if (isio_read((void*) & (col[j]), 1, 1, file) == 0) {
        return 2; /* ����д�쳣 */
      }
    }

    /* չ�������ݰ� */
    for (k = 0; k < (int)count; k++, scan += bytes) {
      for (j = 0; j < (int)noCol; j++) {
        scan[off[j] + k] = col[j];
      }
    }
  }

  return 0;
}


/* ��һ���RLEѹ���������ݣ�������ѹ������*/
static unsigned long channel_read_mixed(ISFILE* file,
    unsigned char* scan,
    unsigned long width,
    unsigned long noCol,
    unsigned long* off,
    unsigned long bytes)
{
  unsigned long count = 0;
  int       i, j, k;
  unsigned char col[4];

  for (i = 0; i < (int)width; i += count) {
    if (isio_eof(file)) {
      return 1;     /* Դ����ȱ�� */
    }

    /* �������ֵ */
    if (isio_read((void*)&count, 1, 1, file) == 0) {
      return 2;     /* ����д�쳣 */
    }

    if (count >= 128) { /* �ظ�����ѹ�������� */
      if (count == 128) {
        if (isio_read((void*)&count, 2, 1, file) == 0) {
          return 2;
        }

        count = (unsigned long)EXCHANGE_WORD(((unsigned short)count));
      }
      else {
        count -= 127;
      }

      if ((i + (int)count) > (int)width) {
        return 3;   /* Դ���ݷǷ� */
      }

      /* ����ѹ������ */
      for (j = 0; j < (int)noCol; j++) {
        if (isio_read((void*) & (col[j]), 1, 1, file) == 0) {
          return 2;
        }
      }

      for (k = 0; k < (int)count; k++, scan += bytes) {
        for (j = 0; j < (int)noCol; j++) {
          scan[off[j]] = col[j];
        }
      }
    }
    else {
      /* δ��ѹ�������� */
      count++;

      if ((i + (int)count) > (int)width) {
        return 3;   /* Դ���ݷǷ� */
      }

      /* ֱ�Ӹ���Դ���������ݰ� */
      for (k = (int)count; k > 0; k--, scan += bytes) {
        for (j = 0; j < (int)noCol; j++) {
          if (isio_read((void*) & (scan[off[j]]), 1, 1, file) == 0) {
            return 2;
          }
        }
      }
    }
  }

  return 0;
}



/* ��ȡͼ��λ���� */
PICS_API int CALLAGREEMENT pics_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*        pfile = (ISFILE*)0;

  int         i, linesize;
  int         channel_count;
  unsigned char*   pb;

  PICS_HEADER     picheader;
  LPPICS_CHANNEL_INFO pchannel = 0;

  enum EXERESULT    b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&pics_load_img_critical);

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

      if (isio_read((void*)&picheader, sizeof(PICS_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* תΪX86���� */
      picheader.fields = EXCHANGE_WORD(picheader.fields);
      picheader.height = EXCHANGE_WORD(picheader.height);
      picheader.magic  = EXCHANGE_DWORD(picheader.magic);
      picheader.ratio  = (float)EXCHANGE_DWORD(((unsigned long)picheader.ratio));
      picheader.version = (float)EXCHANGE_DWORD(((unsigned long)picheader.version));
      picheader.width  = EXCHANGE_WORD(picheader.width);

      /* �ж�ͼ�����Ƿ���SoftImage PIC��ʽ */
      if ((picheader.magic != PICS_MAGIC) || \
          (*((unsigned long*)(&(picheader.id))) != PICS_PICT_STR) || \
          (picheader.width == 0) || \
          (picheader.height == 0)) {
        b_status = ER_NONIMAGE;
        __leave;
      }

      /* ��ȡͨ������ */
      if ((b_status = _read_channel_info(pfile, &pchannel, &channel_count)) != ER_SUCCESS) {
        __leave;
      }

      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        if ((channel_count != 1) && (channel_count != 2)) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        pinfo_str->imgtype    = IMT_RESSTATIC;  /* ͼ���ļ����� */
        pinfo_str->imgformat  = IMF_PIC;      /* ͼ���ļ���ʽ����׺���� */

        pinfo_str->compression  = ICS_RGB;      /* ��������ѹ������ */

        for (i = 0; i < channel_count; i++) { /* �����һ��ͨ��������RLE8������ΪRLE8ѹ�� */
          if (pchannel[i].type != PIC_UNCOMPRESSED) {
            pinfo_str->compression  = ICS_RLE8;
            break;
          }
        }

        pinfo_str->width  = (unsigned long)picheader.width;
        pinfo_str->height = (unsigned long)picheader.height;
        pinfo_str->order  = 0;          /* ����ͼ */
        pinfo_str->bitcount = (unsigned long)(channel_count == 1) ? 24 : 32;

        if (pinfo_str->bitcount == 24) {      /* ������������ */
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0x0;
        }
        else if (pinfo_str->bitcount == 32) {
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0xff000000;
        }
        else {
          assert(0);
        }

        pinfo_str->data_state = 1;
      }

      /* ���õ�ɫ����ĸ���Ϊ0 */
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

      /* ��ʼ�����׵�ַ���飨PICΪ����ͼ��*/
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

      /* ���ж����������� */
      for (pb = (unsigned char*)pinfo_str->p_bit_data, i = 0; i < (int)(pinfo_str->height); i++) {
        switch (read_scanline(pfile, pb, pinfo_str->width, pchannel, channel_count)) {
        case  0:
          break;

        case  1:
        case  3:
          b_status = ER_BADIMAGE;
          __leave;
          break;

        case  2:
          b_status = ER_FILERWERR;
          __leave;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          __leave;
          break;
        }

        pb += linesize;

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
      if (pchannel) {
        isirw_free(pchannel);
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

      LeaveCriticalSection(&pics_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* ����ͼ�� */
PICS_API int CALLAGREEMENT pics_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*        pfile = (ISFILE*)0;

  int         i, j, linesize;
  unsigned char*   pb;
  unsigned char   buf[4];

  PICS_HEADER     picheader;
  PICS_CHANNEL_INFO channel_info;

  enum EXERESULT    b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&pics_save_img_critical);

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
      if (_pics_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* �������ͬ��������������������ȷ����ʾ */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ��д��ͷ�ṹ */
      strcpy((char*)picheader.comment, (const char*)"Saved by ISee PICS plug-in.");
      *((unsigned long*) & (picheader.id[0])) = PICS_PICT_STR;
      picheader.padding = 0;

      picheader.magic  = PICS_MAGIC;
      picheader.width  = (unsigned short)pinfo_str->width;
      picheader.height = (unsigned short)pinfo_str->height;
      picheader.fields = 0x3;
      picheader.ratio  = (float)pinfo_str->width / (float)pinfo_str->height;
      picheader.version = (float)0.0;

      /* ת����MOTO���� */
      picheader.magic  = EXCHANGE_DWORD((picheader.magic));
      picheader.width  = EXCHANGE_WORD((picheader.width));
      picheader.height = EXCHANGE_WORD((picheader.height));
      picheader.fields = EXCHANGE_WORD((picheader.fields));
      picheader.ratio  = (float)EXCHANGE_DWORD(((unsigned long)picheader.ratio));
      picheader.version = (float)EXCHANGE_DWORD(((unsigned long)picheader.version));

      /* ȡ��ɨ���гߴ� */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);

      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* д���ļ�ͷ�ṹ */
      if (isio_write((const void*)&picheader, sizeof(PICS_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      assert(pinfo_str->bitcount == 24);

      /* ��дͨ����Ϣ��ֻ��һ��ͨ����*/
      channel_info.chained = 0;
      channel_info.channel = 0xe0;
      channel_info.size    = 8;
      channel_info.type    = PIC_UNCOMPRESSED;

      /* д��ͨ����Ϣ */
      if (isio_write((const void*)&channel_info, sizeof(PICS_CHANNEL_INFO), 1, pfile) == 0) {
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

      /* �Բ�ѹ���ķ�ʽ���б����������� */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        pb = (unsigned char*)pinfo_str->pp_line_addr[i];

        for (j = 0; j < (int)pinfo_str->width; j++) {
          /* ��ȡ��R��G��B����ֵ��PIC˳����ISee����ʽ�෴��*/
          buf[2] = *pb++;
          buf[1] = *pb++;
          buf[0] = *pb++;

          if (isio_write((const void*)buf, 3, 1, pfile) == 0) {
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
      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }
      else {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }

      LeaveCriticalSection(&pics_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}



