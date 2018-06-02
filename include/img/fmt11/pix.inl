// Alias PIXͼ���дģ��ʵ���ļ�
// ��ȡ���ܣ��ɶ�ȡ8��24λRLEѹ��Alias PIXͼ��
// ���湦�ܣ�24λRLEѹ����ʽ

/* ��ͷ�ṹ���� */
typedef struct {
  unsigned short  width;      /* ͼ����߶ȣ����ص�λ��*/
  unsigned short  height;
  unsigned short  rev0;     /* �ѷ����������֣���ֵ������ */
  unsigned short  rev1;
  unsigned short  bitcount;   /* ͼ��λ��ȣ�ֻ����24��8��*/
} PIX_HEADER, *LPPIX_HEADER;


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* ����ɨ���гߴ�(���ֽڶ���) */
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* �жϴ����ͼ���Ƿ���Ա����� */
int CALLAGREEMENT _pix_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(pix_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* ��֧�ֵ�λ��ͼ�� */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (pix_irwp_info.irwp_save.img_num)
    if (pix_irwp_info.irwp_save.img_num == 1)
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* ͼ���������ȷ */
      }

  return 0;
}


/* ��ȡһ��24λRLEѹ������ */
static enum EXERESULT CALLAGREEMENT _read_line_alias24(ISFILE* pfile, unsigned char* pix, int size)
{
  int       i;
  int       count = 0;
  unsigned char buffer[4];

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(pfile && pix && size);

  __try {
    while (count < size) {
      /* ��ȡһ��ѹ�����ݣ�ÿ��4�ֽڣ���һ���ֽ�Ϊ�ظ�����ֵ����
         �����ֽ�ΪB��G��R���� */
      if (isio_read((void*)buffer, 1, 4, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* �ظ�����ֵ */
      count += (int)(unsigned int)(unsigned char)(buffer[0]);

      if (count > size) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      /* �����ݰ���д�ظ����� */
      for (i = 0; i < (int)(unsigned int)(unsigned char)buffer[0]; i++) {
        *pix++ = buffer[1];
        *pix++ = buffer[2];
        *pix++ = buffer[3];
      }
    }
  }
  __finally {
    ;
  }

  return b_status;
}


/* ��ȡһ��8λRLEѹ������ */
static enum EXERESULT CALLAGREEMENT _read_line_alias8(ISFILE* pfile, unsigned char* pix, int size)
{
  int       i;
  int       count = 0;
  unsigned char buffer[2];

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(pfile && pix && size);

  __try {
    while (count < size) {
      /* ��ȡһ��ѹ�����ݣ�ÿ��2�ֽڣ���һ���ֽ�Ϊ�ظ�����ֵ����
         һ���ֽ�Ϊ��ɫ����ֵ */
      if (isio_read((void*)buffer, 1, 2, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* �ظ�����ֵ */
      count += (int)(unsigned int)(unsigned char)(buffer[0]);

      if (count > size) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      /* �����ݰ���д�ظ����� */
      for (i = 0; i < (int)(unsigned int)(unsigned char)buffer[0]; i++) {
        *pix++ = buffer[1];
      }
    }
  }
  __finally {
    ;
  }

  return b_status;
}


/* ѹ��һ�����ݵ�24λRLE */
static enum EXERESULT CALLAGREEMENT _write_line_alias24(ISFILE* pfile, unsigned char* pix, int size)
{
  int       i;
  int       count;
  unsigned char buffer[4];

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(pfile && pix && size);

  __try {
    for (count = 0, i = 0; i < size; i++) {
      if (count == 0) {
        buffer[0] = 1;
        buffer[1] = *pix++; /* B */
        buffer[2] = *pix++; /* G */
        buffer[3] = *pix++; /* R */
        count = 1;
      }
      else {
        if ((*(pix + 0) == buffer[1]) && (*(pix + 1) == buffer[2]) && \
            (*(pix + 2) == buffer[3]) && (count < 255)) {
          count++;
          buffer[0] = (unsigned char)count;
          pix += 3;
        }
        else {
          /* д��һ��ѹ������ */
          if (isio_write((const void*)buffer, 1, 4, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
          }

          count = 0;

          /* �ղ�����ѭ��ֵ���� */
          i--;
        }
      }
    }

    /* д��ʣ������� */
    if (count != 0) {
      if (isio_write((const void*)buffer, 1, 4, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }
    }
  }
  __finally {
    ;
  }

  return b_status;
}

/* ��ȡͼ��λ���� */
PIX_API int CALLAGREEMENT pix_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       i, linesize;
  unsigned char* p;

  PIX_HEADER    pixheader;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&pix_load_img_critical);

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

      if (isio_read((void*)&pixheader, sizeof(PIX_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* תΪIntel���� */
      pixheader.width   = EXCHANGE_WORD((pixheader.width));
      pixheader.height  = EXCHANGE_WORD((pixheader.height));
      pixheader.bitcount = EXCHANGE_WORD((pixheader.bitcount));

      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        if ((pixheader.width == 0) || (pixheader.height == 0) || \
            ((pixheader.bitcount != 24) && (pixheader.bitcount != 8))) {
          b_status = ER_NONIMAGE;
          __leave;
        }

        /* ��д���ݰ� */
        pinfo_str->imgtype    = IMT_RESSTATIC;  /* ͼ���ļ����� */
        pinfo_str->imgformat  = IMF_PIX;      /* ͼ���ļ���ʽ����׺���� */

        pinfo_str->compression  = ICS_RLE8;

        pinfo_str->width  = (unsigned long)pixheader.width;
        pinfo_str->height = (unsigned long)pixheader.height;
        pinfo_str->order  = 0;          /* PIX ͼ��Ϊ����ͼ */
        pinfo_str->bitcount = (unsigned long)pixheader.bitcount;

        switch (pixheader.bitcount) {
        case  24:
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0x0;
          break;

        case  8:
          pinfo_str->b_mask = 0x0;
          pinfo_str->g_mask = 0x0;
          pinfo_str->r_mask = 0x0;
          pinfo_str->a_mask = 0x0;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          __leave;
          break;
        }

        pinfo_str->data_state = 1;
      }

      /* �˹��ϳɻҶ�ͼ��ɫ�� */
      if (pinfo_str->bitcount == 8) {
        pinfo_str->pal_count = 256;

        for (i = 0; i < (int)pinfo_str->pal_count; i++) {
          pinfo_str->palette[i] = (unsigned long)((i << 16) | (i << 8) | (i << 0));
        }
      }
      else {
        pinfo_str->pal_count = 0;
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

      /* ��д���׵�ַ���飨����*/
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

      /* ����..... */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        p = pinfo_str->p_bit_data + linesize * i;

        /* ���ж�ȡ����ѹ�� */
        if (pinfo_str->bitcount == 24) {
          if ((b_status = _read_line_alias24(pfile, p, (int)pinfo_str->width)) != ER_SUCCESS) {
            __leave;
          }
        }
        else {
          if ((b_status = _read_line_alias8(pfile, p, (int)pinfo_str->width)) != ER_SUCCESS) {
            __leave;
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

      LeaveCriticalSection(&pix_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* ����ͼ�� */
PIX_API int CALLAGREEMENT pix_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       i;
  unsigned char* p;

  PIX_HEADER    pixheader;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&pix_save_img_critical);

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
      if (_pix_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* �������ͬ��������������������ȷ����ʾ */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ��д��ͷ�ṹ */
      pixheader.width   = (unsigned short)pinfo_str->width;
      pixheader.height  = (unsigned short)pinfo_str->height;
      pixheader.rev0    = 0;
      pixheader.rev1    = 0;
      assert(pinfo_str->bitcount == 24);
      pixheader.bitcount = 24;

      /* ת��ΪMOTO���� */
      pixheader.width   = EXCHANGE_WORD((pixheader.width));
      pixheader.height  = EXCHANGE_WORD((pixheader.height));
      pixheader.bitcount = EXCHANGE_WORD((pixheader.bitcount));


      /* д����ͷ�ṹ */
      if (isio_write((const void*)&pixheader, sizeof(PIX_HEADER), 1, pfile) == 0) {
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

      /* ���б��루RLE8����д��Ŀ���� */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        p = (unsigned char*)pinfo_str->pp_line_addr[i];

        if ((b_status = _write_line_alias24(pfile, p, (int)pinfo_str->width)) != ER_SUCCESS) {
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
      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }
      else {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }

      LeaveCriticalSection(&pix_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}



