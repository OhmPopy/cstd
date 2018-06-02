/********************************************************************
 pix.c
 ���ļ���;�� Alias PIXͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ��ɶ�ȡ8��24λRLEѹ��Alias PIXͼ��
 ���湦�ܣ�24λRLEѹ����ʽ
 ���ļ���д�ˣ�
 YZ yzfree##yeah.net
 ���ļ��汾�� 30401
 ����޸��ڣ� 2003-4-1
 ----------------------------------------------------------------
 ������ʷ��
 2003-4 ֧��ͼ���д�����ṩ�����ػ���������
 2002-8 ����ӿ�������2.2�������ڴ�������
 2002-5 ��һ�������棨�°棩
********************************************************************/

/* ��ͷ�ṹ���� */
typedef struct {
  ushort  width;      /* ͼ����߶ȣ����ص�λ��*/
  ushort  height;
  ushort  rev0;     /* �ѷ����������֣���ֵ������ */
  ushort  rev1;
  ushort  bitcount;   /* ͼ��λ��ȣ�ֻ����24��8��*/
} PIX_HEADER, *LPPIX_HEADER;

/* �ڲ����ֺ��� */
int _pix_is_valid_img(INFOSTR* pinfo_str);
static enum EXERESULT _read_line_alias24(ISFILE*, uchar*, int);
static enum EXERESULT _read_line_alias8(ISFILE*, uchar*, int);
static enum EXERESULT _write_line_alias24(ISFILE*, uchar*, int);

/* ��ȡͼ����Ϣ */
EXERESULT pix_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  PIX_HEADER pixheader;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */

    do {
      /* ��ȡ�ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&pixheader, sizeof(PIX_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* תΪIntel���� */
      pixheader.width = EXCHANGE_WORD((pixheader.width));
      pixheader.height = EXCHANGE_WORD((pixheader.height));
      pixheader.bitcount = EXCHANGE_WORD((pixheader.bitcount));
      /* �ж��Ƿ�����Ч��Alias PIXͼ�����ָ�ʽ�ǳ��򵥣�����
      �޷������ϸ���жϣ��������еĿ����Խ�������ʽ�Դ�һЩ��*/
      if ((pixheader.width == 0) || (pixheader.height == 0) || \
      ((pixheader.bitcount != 24) && (pixheader.bitcount != 8))) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ��д���ݰ� */
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_PIX; /* ͼ���ļ���ʽ����׺���� */
      pinfo_str->compression = ICS_RLE8;
      pinfo_str->width = (ulong)pixheader.width;
      pinfo_str->height = (ulong)pixheader.height;
      pinfo_str->order = 0; /* PIX ͼ��Ϊ����ͼ */
      pinfo_str->bitcount = (ulong)pixheader.bitcount;
      /* ��д������Ϣ */
      switch (pixheader.bitcount) {
      case 24:
        pinfo_str->b_mask = 0xff;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->a_mask = 0x0;
        break;
      case 8:
        pinfo_str->b_mask = 0x0;
        pinfo_str->g_mask = 0x0;
        pinfo_str->r_mask = 0x0;
        pinfo_str->a_mask = 0x0;
        break;
      default:
        assert(0);
        b_status = ER_SYSERR;
        break;
      }
      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    while (0);
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT pix_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  int i, linesize;
  uchar* p;
  PIX_HEADER pixheader;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */

    do {
      /* ���ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&pixheader, sizeof(PIX_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* תΪIntel���� */
      pixheader.width = EXCHANGE_WORD((pixheader.width));
      pixheader.height = EXCHANGE_WORD((pixheader.height));
      pixheader.bitcount = EXCHANGE_WORD((pixheader.bitcount));
      /* �����ͼ��δ���ù���ȡ��Ϣ��?�������д�?����?*/
      if (pinfo_str->data_state == 0) {
        if ((pixheader.width == 0) || (pixheader.height == 0) || \
        ((pixheader.bitcount != 24) && (pixheader.bitcount != 8))) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* ��д���ݰ� */
        pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
        pinfo_str->imgformat = IMF_PIX; /* ͼ���ļ���ʽ����׺���� */
        pinfo_str->compression = ICS_RLE8;
        pinfo_str->width = (ulong)pixheader.width;
        pinfo_str->height = (ulong)pixheader.height;
        pinfo_str->order = 0; /* PIX ͼ��Ϊ����ͼ */
        pinfo_str->bitcount = (ulong)pixheader.bitcount;
        switch (pixheader.bitcount) {
        case 24:
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0x0;
          break;
        case 8:
          pinfo_str->b_mask = 0x0;
          pinfo_str->g_mask = 0x0;
          pinfo_str->r_mask = 0x0;
          pinfo_str->a_mask = 0x0;
          break;
        default:
          assert(0);
          b_status = ER_SYSERR;
          break;
        }
        pinfo_str->data_state = 1;
      }
      /* �˹��ϳɻҶ�ͼ��ɫ�� */
      if (pinfo_str->bitcount == 8) {
        pinfo_str->pal_count = 256;
        for (i = 0; i < (int)pinfo_str->pal_count; i++) {
          pinfo_str->palette[i] = (ulong)((i << 16) | (i << 8) | (i << 0));
        }
      }
      else {
        pinfo_str->pal_count = 0;
      }
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
      /* ��д���׵�ַ���飨����*/
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
      }
      /* ����..... */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        p = pinfo_str->p_bit_data + linesize * i;
        /* ���ж�ȡ����ѹ�� */
        if (pinfo_str->bitcount == 24) {
          if ((b_status = _read_line_alias24(pfile, p, (int)pinfo_str->width)) != ER_SUCCESS) {
            break;
          }
        }
        else {
          if ((b_status = _read_line_alias8(pfile, p, (int)pinfo_str->width)) != ER_SUCCESS) {
            break;
          }
        }
      }
      pinfo_str->data_state = 2;
    }
    while (0);
    
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
/* ����ͼ�� */
EXERESULT pix_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  int i;
  uchar* p;
  PIX_HEADER pixheader;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);
  do {
      /* �ж��Ƿ�����֧�ֵ�ͼ������ */
      if (_pix_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        break;
      }
      /* ��д��ͷ�ṹ */
      pixheader.width = (ushort)pinfo_str->width;
      pixheader.height = (ushort)pinfo_str->height;
      pixheader.rev0 = 0;
      pixheader.rev1 = 0;
      assert(pinfo_str->bitcount == 24);
      pixheader.bitcount = 24;
      /* ת��ΪMOTO���� */
      pixheader.width = EXCHANGE_WORD((pixheader.width));
      pixheader.height = EXCHANGE_WORD((pixheader.height));
      pixheader.bitcount = EXCHANGE_WORD((pixheader.bitcount));
      /* д����ͷ�ṹ */
      if (isio_write((const void*)&pixheader, sizeof(PIX_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ���б��루RLE8����д��Ŀ���� */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        p = (uchar*)pinfo_str->pp_line_addr[i];
        if ((b_status = _write_line_alias24(pfile, p, (int)pinfo_str->width)) != ER_SUCCESS) {
          break;
        }
      }
    }
    while (0);
    
  return b_status;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */
/* �жϴ����ͼ���Ƿ���Ա����� */
int _pix_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = (1UL << (24 - 1))};
  /* λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1; /* ��֧�ֵ�λ��ͼ�� */
  }
  assert(pinfo_str->imgnumbers);
      if (pinfo_str->imgnumbers != 1) {
        return -2; /* ͼ���������ȷ */
      }
  return 0;
}
/* ��ȡһ��24λRLEѹ������ */
static enum EXERESULT _read_line_alias24(ISFILE* pfile, uchar* pix, int size)
{
  int i;
  int count = 0;
  uchar buffer[4];
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pix && size);
  do {
    while (count < size) {
      /* ��ȡһ��ѹ�����ݣ�ÿ��4�ֽڣ���һ���ֽ�Ϊ�ظ�����ֵ����
      �����ֽ�ΪB��G��R���� */
      if (isio_read((void*)buffer, 1, 4, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* �ظ�����ֵ */
      count += (int)(unsigned int)(uchar)(buffer[0]);
      if (count > size) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* �����ݰ���д�ظ����� */
      for (i = 0; i < (int)(unsigned int)(uchar)buffer[0]; i++) {
        *pix++ = buffer[1];
        *pix++ = buffer[2];
        *pix++ = buffer[3];
      }
    }
  }
  while (0);
  {
    ;
  }
  return b_status;
}
/* ��ȡһ��8λRLEѹ������ */
static enum EXERESULT _read_line_alias8(ISFILE* pfile, uchar* pix, int size)
{
  int i;
  int count = 0;
  uchar buffer[2];
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pix && size);
  do {
    while (count < size) {
      /* ��ȡһ��ѹ�����ݣ�ÿ��2�ֽڣ���һ���ֽ�Ϊ�ظ�����ֵ����
      һ���ֽ�Ϊ��ɫ����ֵ */
      if (isio_read((void*)buffer, 1, 2, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* �ظ�����ֵ */
      count += (int)(unsigned int)(uchar)(buffer[0]);
      if (count > size) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* �����ݰ���д�ظ����� */
      for (i = 0; i < (int)(unsigned int)(uchar)buffer[0]; i++) {
        *pix++ = buffer[1];
      }
    }
  }
  while (0);
  {
    ;
  }
  return b_status;
}
/* ѹ��һ�����ݵ�24λRLE */
static enum EXERESULT _write_line_alias24(ISFILE* pfile, uchar* pix, int size)
{
  int i;
  int count;
  uchar buffer[4];
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pix && size);
  do {
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
          buffer[0] = (uchar)count;
          pix += 3;
        }
        else {
          /* д��һ��ѹ������ */
          if (isio_write((const void*)buffer, 1, 4, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
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
        break;
      }
    }
  }
  while (0);
  {
    ;
  }
  return b_status;
}

