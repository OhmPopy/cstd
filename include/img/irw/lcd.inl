/********************************************************************
 lcd.c
 ���ļ���;�� LCDͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ���ȡ�Զ���ṹ -����(8λһ���ֽ�)�ж��뷽ʽ�洢��
 Һ����������
 ���湦�ܣ��Զ���ṹ -����(8λһ���ֽ�)�ж��뷽ʽ�洢��Һ��
 �������ݣ���.h�ļ��ж��壩
 ���ļ���д�ˣ� zhaojinpo (zhaojinpo##sina.com)
 ���ļ��汾�� 30902
 ����޸��ڣ� 2003-9-2
 ----------------------------------------------------------------
 ������ʷ��
 2003-9 ��һ�η���
********************************************************************/




/* �ļ��ṹ���� */

/* MAGIC���� */
#define LCD_HEADER_MARKER 0x0064636c          //"LCD "

/* ���ݿ�λ�� */
#define LCD_data_offset   0x010

/* ����ͼ�ļ�ͷ�ṹ ��Windows��*/
typedef struct _tag_lcd_file_header {
  ulong bfType;
  ulong Size;
  ushort  width;
  ushort  hight;
  ushort  bSize;                //Һ�����ڲ���˫���ֽڴ洢
  uchar bwidth;               //Һ�����ڲ��õ��ֽڴ洢
  uchar bhight;               //Һ�����ڲ��õ��ֽڴ洢
} LCD_FILE_HEADER;

/* ���ݴ洢��ʽ�����ݴ洢1�ֽڴ���һ����8���㡣

   --------lcd_file_header-------------
   ----ulong  bfType;   lcd�ļ���ʾ
   ----ulong  Size;   Һ��ͼ�δ洢���ݴ�С������ͷ�ṹ���ȣ�
   ----ushort width;    ͼ�ο��
   ----ushort hight;    ͼ�θ߶�
   ----ushort bSize;      �ҵ�Һ�����ڲ���˫���ֽڴ洢
   ----uchar  bwidth;     �ҵ�Һ�����ڲ��õ��ֽڴ洢
   ----uchar  bhight;     �ҵ�Һ�����ڲ��õ��ֽڴ洢
   ---------lcd�д洢����-------------

   ----uchar  data;       Һ�����ֽڰ��д洢����

     �ֽڴ洢��1��----����11������ȡ��

     �������е�1�� ��|0|1| 2 3 4 5 6 7 8 9 10
     |1|
     |2|
     |3|
     |4|
     |5|
     |6|
     �������е�8�� ��|7|

*/


/* �ڲ����ֺ��� */
int _lcd_is_valid_img(INFOSTR* pinfo_str);
uchar _get_byte_bit(uchar my_byte, uchar bit);

/* ��ȡͼ����Ϣ */
EXERESULT lcd_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LCD_FILE_HEADER bfh;
  int style; /* ���0��WINDOWS��1��OS/2 */
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */

    do {
      /* ��ȡ�ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&bfh, sizeof(LCD_FILE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      if (bfh.bfType == LCD_HEADER_MARKER) {
        style = 0;
      }
      else {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ������Ч�Լ��� */
      if (style == 0) {
        if ((bfh.width == 0) || (bfh.hight == 0)) {
          b_status = ER_NONIMAGE;
          break;
        }
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_LCD; /* ͼ���ļ���ʽ����׺���� */
      pinfo_str->compression = ICS_RGB;
      pinfo_str->width = bfh.width;
      pinfo_str->height = bfh.hight;
      pinfo_str->order = 0;
      pinfo_str->bitcount = 1;
      pinfo_str->b_mask = 0x0;
      pinfo_str->g_mask = 0x0;
      pinfo_str->r_mask = 0x0;
      pinfo_str->a_mask = 0x0;
      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    while (0);
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT lcd_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LCD_FILE_HEADER bfh;
  int style; /* ���0��WINDOWS */
  ulong LCD_data_len; /* LCDλ���ݳߴ� */
  int i, linesize, line = 0; /* ��ǰ�� */
  unsigned int k;
  uchar tmp , j;
  uchar* pbit = (uchar*)0;
  uchar* p_pre_bit = (uchar*)0;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */

    do {
      /* ���ļ���λ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&bfh, sizeof(LCD_FILE_HEADER), 1, pfile) == 0) {
        b_status = (pinfo_str->data_state == 1) ? ER_FILERWERR : ER_NONIMAGE;
        break;
      }
      /* �ж��Ƿ�����Ч��λͼ�ļ� */
      if (bfh.bfType == LCD_HEADER_MARKER) {
        style = 0;
      }
      else {
        b_status = ER_NONIMAGE;
        break;
      }
      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        /* ������Ч�Լ��� */
        if (style == 0) {
          if ((bfh.width == 0) || (bfh.hight == 0)) {
            b_status = ER_NONIMAGE;
            break;
          }
        }
        pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ�?��?*/
        pinfo_str->imgformat = IMF_LCD; /* ͼ���ļ���ʽ����׺���� */
        pinfo_str->compression = ICS_RGB;
        pinfo_str->width = bfh.width;
        pinfo_str->height = bfh.hight;
        pinfo_str->order = 0;
        pinfo_str->bitcount = 1;
        pinfo_str->b_mask = 0x0;
        pinfo_str->g_mask = 0x0;
        pinfo_str->r_mask = 0x0;
        pinfo_str->a_mask = 0x0;
        pinfo_str->data_state = 1;
      }
      /* ���õ�ɫ������ */
      if (pinfo_str->bitcount <= 8) {
        pinfo_str->pal_count = 1UL << pinfo_str->bitcount;
        if (style == 0) {
          pinfo_str->palette[0] = 0xff00; /* ��ɫ������ֽ����� */
          pinfo_str->palette[1] = 0xffffff;
        }
      }
      else {
        pinfo_str->pal_count = 0;
      }
      /* ȡ��λ���ݳ��� */
      LCD_data_len = isio_length(pfile) - 1;
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
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
      }
      /* ��ȡλ���ݵ��ڴ� */
      if (isio_seek(pfile, LCD_data_offset, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      pbit = pinfo_str->p_bit_data;
      /* ÿ�ζ�ȡ8������ ����8�����ݻ�����(�����б�������+8�ֽڱ���) */
      p_pre_bit = (uchar*)isirw_malloc(pinfo_str->width + 8);
      if (p_pre_bit == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      for (i = 0; i < (int)(pinfo_str->height);) {
        /* ��һ�����ݵ������� */
        if (isio_read(p_pre_bit, pinfo_str->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        j = 0;
        /* ���������ʾ���� */
        while (i < (int)(pinfo_str->height) && (j < 8)) {
          /* ���г�ȡ��������ƴ��Ϊ������ */
          for (k = 0; k < (int)(pinfo_str->width); k = k + 8) {
            tmp = (_get_byte_bit(*(p_pre_bit + k), j) << 7) | (_get_byte_bit(*(p_pre_bit + 1 + k), j) << 6) | (_get_byte_bit(*(p_pre_bit + 2 + k), j) << 5) | (_get_byte_bit(*(p_pre_bit + k + 3), j) << 4) | (_get_byte_bit(*(p_pre_bit + k + 4), j) << 3) | (_get_byte_bit(*(p_pre_bit + k + 5), j) << 2) | (_get_byte_bit(*(p_pre_bit + k + 6), j) << 1) | (_get_byte_bit(*(p_pre_bit + k + 7), j) << 0);
            *(uchar*)(pbit + (k >> 3)) = tmp;
          }
          i++;
          j++;
          /* �ƶ����µ�λ��4�ֽڶ��� */
          pbit = (uchar*)(pbit + linesize);
        }
      }
      /* ����ڴ� */
      isirw_free(p_pre_bit);
      p_pre_bit = 0;
      pinfo_str->data_state = 2;
    }
    while (0);
    
      if (p_pre_bit) {
        isirw_free(p_pre_bit);
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
/* ����ͼ�� */
EXERESULT lcd_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LCD_FILE_HEADER bfh;
  uchar* p_pre_bit = (uchar*)0;
  int i, k, linesize;
  typedef struct _tag_byte_bit {
    unsigned bit0: 1;
    unsigned bit1: 1;
    unsigned bit2: 1;
    unsigned bit3: 1;
    unsigned bit4: 1;
    unsigned bit5: 1;
    unsigned bit6: 1;
    unsigned bit7: 1;
  } BIT_DATA;
  union {
    uchar uc_data;
    BIT_DATA bit_data;
  } mylcd_bit;
  enum EXERESULT b_status = ER_SUCCESS;
      assert(pfile && pinfo_str);
      assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
      assert(pinfo_str->p_bit_data);

    do {
      /* �ж��Ƿ�����֧�ֵ�ͼ������ */
      if (_lcd_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        break;
      }
      /* ����ļ�ͷ */
      bfh.bfType = LCD_HEADER_MARKER;
      bfh.width = (ushort)pinfo_str->width;
      bfh.hight = (ushort)pinfo_str->height;
      bfh.Size = pinfo_str->width * ((pinfo_str->height + 7) / 8);
      bfh.bwidth = (uchar)pinfo_str->width;
      bfh.bhight = (uchar)pinfo_str->height;
      bfh.bSize = ((((ushort)bfh.Size) << 4) & 0x0f) | ((((ushort)bfh.Size) >> 4) & 0xf0);
      /* д���ļ�ͷ�ṹ */
      if (isio_write((const void*)&bfh, sizeof(LCD_FILE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ȡ��ɨ���гߴ� */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      /* ������Ϊ8�ı���--�����ڴ� */
      p_pre_bit = (uchar*)isirw_malloc(8L * linesize * ((pinfo_str->height + 7) / 8) + 4);
      if (p_pre_bit == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        memmove((void*)(p_pre_bit + linesize * i), (const void*)(uchar*)(pinfo_str->pp_line_addr[i]), linesize);
      }
      /* ��-8���ƶ� */
      for (i = 0; i < (int)(pinfo_str->height); i = i + 8) {
        /* ���ƶ� */
        for (k = 0; k < (int)(pinfo_str->width); k++) {
          mylcd_bit.uc_data = 0;
          /* ���г�ȡ��������ƴ��Ϊ������ */
          mylcd_bit.bit_data.bit0 = _get_byte_bit(*(p_pre_bit + linesize * i + k / 8) , (uchar)((7 - (k % 8))));
          mylcd_bit.bit_data.bit1 = _get_byte_bit(*(p_pre_bit + linesize * (i + 1) + k / 8) , (uchar)((7 - (k % 8))));
          mylcd_bit.bit_data.bit2 = _get_byte_bit(*(p_pre_bit + linesize * (i + 2) + k / 8) , (uchar)((7 - (k % 8))));
          mylcd_bit.bit_data.bit3 = _get_byte_bit(*(p_pre_bit + linesize * (i + 3) + k / 8) , (uchar)((7 - (k % 8))));
          mylcd_bit.bit_data.bit4 = _get_byte_bit(*(p_pre_bit + linesize * (i + 4) + k / 8) , (uchar)((7 - (k % 8))));
          mylcd_bit.bit_data.bit5 = _get_byte_bit(*(p_pre_bit + linesize * (i + 5) + k / 8) , (uchar)((7 - (k % 8))));
          mylcd_bit.bit_data.bit6 = _get_byte_bit(*(p_pre_bit + linesize * (i + 6) + k / 8) , (uchar)((7 - (k % 8))));
          mylcd_bit.bit_data.bit7 = _get_byte_bit(*(p_pre_bit + linesize * (i + 7) + k / 8) , (uchar)((7 - (k % 8))));
          if (isio_write((const void*)&mylcd_bit.uc_data, 1, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
      }
      /* ����ڴ� */
      isirw_free(p_pre_bit);
      p_pre_bit = 0;
    }
    while (0);
    
      if (p_pre_bit) {
        isirw_free(p_pre_bit);
      }
  return b_status;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */
/* �жϴ����ͼ���Ƿ���Ա����棨�粻֧�ֱ��湦�ܣ��ɽ�������ȥ���� */
int _lcd_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = 1UL};
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
uchar _get_byte_bit(uchar my_byte, uchar bit)
{
  uchar result = 0;
  typedef struct _tag_byte_bit {
    unsigned bit0: 1;
    unsigned bit1: 1;
    unsigned bit2: 1;
    unsigned bit3: 1;
    unsigned bit4: 1;
    unsigned bit5: 1;
    unsigned bit6: 1;
    unsigned bit7: 1;
  } BIT_DATA;
  union {
    uchar uc_data;
    BIT_DATA bit_data;
  } my_bit;
  my_bit.uc_data = my_byte;
  switch (bit) {
  case 0x0:
    result = my_bit.bit_data.bit0;
    break;
  case 0x1:
    result = my_bit.bit_data.bit1;
    break;
  case 0x2:
    result = my_bit.bit_data.bit2;
    break;
  case 0x3:
    result = my_bit.bit_data.bit3;
    break;
  case 0x4:
    result = my_bit.bit_data.bit4;
    break;
  case 0x5:
    result = my_bit.bit_data.bit5;
    break;
  case 0x6:
    result = my_bit.bit_data.bit6;
    break;
  case 0x7:
    result = my_bit.bit_data.bit7;
    break;
  }
  return result; /* λ���� */
}

