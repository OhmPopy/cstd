/********************************************************************
 pics.c
 ���ļ���;�� SoftImage PICͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ��ɶ�ȡ24λ��32λ����һ��ALPHAͨ������
 ѹ����δѹ�� SoftImage PIC ͼ��
 ���湦�ܣ��ɽ�24λͼ���Բ�ѹ���ķ�ʽ����Ϊ3ͨ��
 SoftImage PIC ͼ��
 ���ļ���д�ˣ�
 YZ yzfree##yeah.net
 ���ļ��汾�� 30401
 ����޸��ڣ� 2003-4-1
 ----------------------------------------------------------------
 ������ʷ��
 2003-4 ֧��ͼ���д�����ṩ�����ػ���������
 2002-8 ����ӿ�������2.2�������ڴ�������
 2002-5 ������һ���汾���°棩
********************************************************************/

/* �ļ��б��־ */
#define PICS_MAGIC            0x5380F634
#define PICS_PICT_STR         0x54434950    /* 'PICT' �ĸ��ַ� */

/* �ļ�ͷ�ṹ���� */
typedef struct {
  unsigned int  magic;           /* PICͼ���ǩ             */
  float     version;         /* ʹ�õĸ�ʽ�汾��        */
  char      comment[80];     /* ͼ�������ִ�            */
  char      id[4];           /* "PICT"�ĸ��ַ�          */
  ushort  width;           /* ͼ���ȣ����ص�λ      */
  ushort  height;          /* ͼ��߶ȣ����ص�λ      */
  float     ratio;           /* ͼ��Ŀ�߱�            */
  ushort  fields;          /* ͼ���־��              */
  ushort  padding;         /* ����                    */
} PICS_HEADER, *LPPICS_HEADER;


/* ͨ����Ϣ�ṹ */
typedef struct {
  uchar chained;
  uchar size;     /* ÿͨ��λ�� */
  uchar type;     /* ѹ����ʽ */
  uchar channel;    /* ������Щͨ��(R��G��B��A...) */
} PICS_CHANNEL_INFO, *LPPICS_CHANNEL_INFO;



/* �������� */
#define PIC_UNSIGNED_INTEGER  0x00
#define PIC_SIGNED_INTEGER    0x10  /* δʵ�� */
#define PIC_SIGNED_FLOAT    0x20  /* δʵ�� */


/* ѹ������ */
#define PIC_UNCOMPRESSED    0x00
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

/* �ڲ����ֺ��� */
int _pics_is_valid_img(INFOSTR* pinfo_str);
static enum EXERESULT _read_channel_info(ISFILE* pfile, LPPICS_CHANNEL_INFO* ppchannel, int* channel_count);
static ulong read_scanline(ISFILE*, uchar*, ulong, LPPICS_CHANNEL_INFO, ulong);
static ulong channel_read_mixed(ISFILE*, uchar*, ulong, ulong, ulong*, ulong);
static ulong channel_read_pure(ISFILE*, uchar*, ulong, ulong, ulong*, ulong);
static ulong channel_read_raw(ISFILE*, uchar*, ulong, ulong, ulong*, ulong);

/* ��ȡͼ����Ϣ */
EXERESULT pics_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  int channel_count, i;
  PICS_HEADER picheader;
  LPPICS_CHANNEL_INFO pchannel = 0;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */

    do {
      /* ��ȡ�ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&picheader, sizeof(PICS_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* תΪX86���� */
      picheader.fields = EXCHANGE_WORD(picheader.fields);
      picheader.height = EXCHANGE_WORD(picheader.height);
      picheader.magic = EXCHANGE_DWORD(picheader.magic);
      picheader.ratio = (float)EXCHANGE_DWORD(((ulong)picheader.ratio));
      picheader.version = (float)EXCHANGE_DWORD(((ulong)picheader.version));
      picheader.width = EXCHANGE_WORD(picheader.width);
      /* �ж�ͼ�����Ƿ���SoftImage PIC��ʽ */
      if ((picheader.magic != PICS_MAGIC) || \
          (*((ulong*)(&(picheader.id))) != PICS_PICT_STR) || \
          (picheader.width == 0) || \
      (picheader.height == 0)) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ��ȡͨ������ */
      if ((b_status = _read_channel_info(pfile, &pchannel, &channel_count)) != ER_SUCCESS) {
        break;
      }
      /* ֻ֧��3��4ͨ��ͼ�󣨵�ǰ��SoftImage PICͼ����󲿷���������ͨ����ʽ��*/
      /* RGB����ͨ���ϲ���һ��ALPHAͨ���������� */
      if ((channel_count != 1) && (channel_count != 2)) {
        b_status = ER_BADIMAGE;
        break;
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_PIC; /* ͼ���ļ���ʽ����׺���� */
      pinfo_str->compression = ICS_RGB; /* ��������ѹ������ */
      for (i = 0; i < channel_count; i++) { /* �����һ��ͨ��������RLE8������ΪRLE8ѹ�� */
        if (pchannel[i].type != PIC_UNCOMPRESSED) {
          pinfo_str->compression = ICS_RLE8;
          break;
        }
      }
      pinfo_str->width = (ulong)picheader.width;
      pinfo_str->height = (ulong)picheader.height;
      pinfo_str->order = 0; /* ����ͼ */
      pinfo_str->bitcount = (ulong)(channel_count == 1) ? 24 : 32;
      if (pinfo_str->bitcount == 24) { /* ������������ */
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
        assert(0); /* ϵͳ�쳣��ֻ����3ͨ����4ͨ���� */
      }
      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    while (0);
    
      if (pchannel) {
        isirw_free(pchannel);
      }
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT pics_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  int i, linesize;
  int channel_count;
  uchar* pb;
  PICS_HEADER picheader;
  LPPICS_CHANNEL_INFO pchannel = 0;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */

    do {
      /* ���ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&picheader, sizeof(PICS_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* תΪX86���� */
      picheader.fields = EXCHANGE_WORD(picheader.fields);
      picheader.height = EXCHANGE_WORD(picheader.height);
      picheader.magic = EXCHANGE_DWORD(picheader.magic);
      picheader.ratio = (float)EXCHANGE_DWORD(((ulong)picheader.ratio));
      picheader.version = (float)EXCHANGE_DWORD(((ulong)picheader.version));
      picheader.width = EXCHANGE_WORD(picheader.width);
      /* �ж�ͼ�����Ƿ���SoftImage PIC��ʽ */
      if ((picheader.magic != PICS_MAGIC) || \
          (*((ulong*)(&(picheader.id))) != PICS_PICT_STR) || \
          (picheader.width == 0) || \
      (picheader.height == 0)) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ��ȡͨ������ */
      if ((b_status = _read_channel_info(pfile, &pchannel, &channel_count)) != ER_SUCCESS) {
        break;
      }
      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        if ((channel_count != 1) && (channel_count != 2)) {
          b_status = ER_BADIMAGE;
          break;
        }
        pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
        pinfo_str->imgformat = IMF_PIC; /* ͼ���ļ���ʽ����׺���� */
        pinfo_str->compression = ICS_RGB; /* ��������ѹ������ */
        for (i = 0; i < channel_count; i++) { /* �����һ��ͨ��������RLE8������ΪRLE8ѹ�� */
          if (pchannel[i].type != PIC_UNCOMPRESSED) {
            pinfo_str->compression = ICS_RLE8;
            break;
          }
        }
        pinfo_str->width = (ulong)picheader.width;
        pinfo_str->height = (ulong)picheader.height;
        pinfo_str->order = 0; /* ����ͼ */
        pinfo_str->bitcount = (ulong)(channel_count == 1) ? 24 : 32;
        if (pinfo_str->bitcount == 24) { /* ������������ */
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
      pinfo_str->psubimg = (SUBIMGBLOCK*)0;
      assert(pinfo_str->p_bit_data == (uchar*)0);
      /* ����Ŀ��ͼ���ڴ�飨+4 �� β������4�ֽڻ������� */
      pinfo_str->p_bit_data = (uchar*)isirw_malloc(linesize * pinfo_str->height + 4);
      if (!pinfo_str->p_bit_data) {
        b_status = ER_MEMORYERR;
        break;
      }
      assert(pinfo_str->pp_line_addr == NULL);
      /* �������׵�ַ��??*/
      pinfo_str->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * pinfo_str->height);
      if (!pinfo_str->pp_line_addr) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* ��ʼ�����׵�ַ���飨PICΪ����ͼ��*/
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
      }
      /* ���ж����������� */
      for (pb = (uchar*)pinfo_str->p_bit_data, i = 0; i < (int)(pinfo_str->height); i++) {
        switch (read_scanline(pfile, pb, pinfo_str->width, pchannel, channel_count)) {
        case 0:
          break;
        case 1:
        case 3:
          b_status = ER_BADIMAGE;
          break;
        case 2:
          b_status = ER_FILERWERR;
          break;
        default:
          assert(0);
          b_status = ER_SYSERR;
          break;
        }
        pb += linesize;
      }
      pinfo_str->data_state = 2;
    }
    while (0);
    
      if (pchannel) {
        isirw_free(pchannel);
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
EXERESULT pics_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  int i, j, linesize;
  uchar* pb;
  uchar buf[4];
  PICS_HEADER picheader;
  PICS_CHANNEL_INFO channel_info;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);

    do {
      /* �ж��Ƿ�����֧�ֵ�ͼ������ */
      if (_pics_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        break;
      }
      /* ��д��ͷ�ṹ */
      strcpy((char*)picheader.comment, (const char*)"Saved by ISee PICS plug-in.");
      *((ulong*) & (picheader.id[0])) = PICS_PICT_STR;
      picheader.padding = 0;
      picheader.magic = PICS_MAGIC;
      picheader.width = (ushort)pinfo_str->width;
      picheader.height = (ushort)pinfo_str->height;
      picheader.fields = 0x3;
      picheader.ratio = (float)pinfo_str->width / (float)pinfo_str->height;
      picheader.version = (float)0.0;
      /* ת����MOTO���� */
      picheader.magic = EXCHANGE_DWORD((picheader.magic));
      picheader.width = EXCHANGE_WORD((picheader.width));
      picheader.height = EXCHANGE_WORD((picheader.height));
      picheader.fields = EXCHANGE_WORD((picheader.fields));
      picheader.ratio = (float)EXCHANGE_DWORD(((ulong)picheader.ratio));
      picheader.version = (float)EXCHANGE_DWORD(((ulong)picheader.version));
      /* ȡ��ɨ���гߴ� */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* д���ļ�ͷ�ṹ */
      if (isio_write((const void*)&picheader, sizeof(PICS_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      assert(pinfo_str->bitcount == 24);
      /* ��дͨ����Ϣ��ֻ��һ��ͨ����*/
      channel_info.chained = 0;
      channel_info.channel = 0xe0;
      channel_info.size = 8;
      channel_info.type = PIC_UNCOMPRESSED;
      /* д��ͨ����Ϣ */
      if (isio_write((const void*)&channel_info, sizeof(PICS_CHANNEL_INFO), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* �Բ�ѹ���ķ�ʽ���б����������� */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        pb = (uchar*)pinfo_str->pp_line_addr[i];
        for (j = 0; j < (int)pinfo_str->width; j++) {
          /* ��ȡ��R��G��B����ֵ��PIC˳����ISee����ʽ�෴��*/
          buf[2] = *pb++;
          buf[1] = *pb++;
          buf[0] = *pb++;
          if (isio_write((const void*)buf, 3, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
      }
    }
    while (0);
    
  return b_status;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */
/* �жϴ����ͼ���Ƿ���Ա����棨�粻֧�ֱ��湦�ܣ��ɽ�������ȥ���� */
int _pics_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = 1UL << (24 - 1)};
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
/* ��ȡ���е�ͨ����Ϣ������4��ͨ������������Ϊ4ͨ��ͼ��*/
static enum EXERESULT _read_channel_info(ISFILE* pfile, LPPICS_CHANNEL_INFO* ppchannel, int* channel_count)
{
  int i, j;
  LPPICS_CHANNEL_INFO p = 0;
  LPPICS_CHANNEL_INFO ptmp;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && ppchannel && channel_count);
  do {
    *ppchannel = p;
    *channel_count = 0;
    if (isio_seek(pfile, sizeof(PICS_HEADER), SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    if ((p = ptmp = (LPPICS_CHANNEL_INFO)isirw_malloc(sizeof(PICS_CHANNEL_INFO) * 3)) == 0) {
      b_status = ER_MEMORYERR;
      break;
    }
    j = i = 0;
    do {
      /* ��һ��ͨ������ */
      if (isio_read((void*)ptmp, sizeof(PICS_CHANNEL_INFO), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      i++;
      j++;
      /* ���ͨ�������Ƿ�Ϸ� */
      if ((ptmp->size != 8) || (ptmp->type > 0x2) || ((ptmp->channel != 0xe0) && (ptmp->channel != 0x10))) {
        b_status = ER_BADIMAGE;
        break;
      }
      if (i > 2) { /* ֻ����ǰ4��ͨ�������ݣ�RGB����ͨ������һ�� */
        i--;
        ptmp--; /* ������ȡ��һ��ͨ�������ݣ��������� */
        if (j > 6) { /* ����8��ͨ�����п����ǷǷ���PICͼ�� */
          b_status = ER_BADIMAGE;
          break;
        }
      }
    }
    while ((ptmp->chained != 0) && (ptmp++));
    *ppchannel = p; /* ����ͨ������ */
    *channel_count = i;
  }
  while (0);
  
    if (b_status != ER_SUCCESS) {
      if (p) {
        isirw_free(p);
      }
    }
  
  return b_status;
}
/* ��ȡһ��ɨ���е����ݣ��������ɨ���е�����ͨ����*/
static ulong read_scanline(ISFILE* pfile,
    uchar* scan,
    ulong width,
    LPPICS_CHANNEL_INFO channel,
    ulong chan_count)
{
  int i;
  ulong noCol, status = 0;
  ulong off[4];
  ulong bytes;
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
    case PIC_UNCOMPRESSED: /* δѹ�� */
      status = channel_read_raw(pfile, scan, width, noCol, off, bytes);
      break;
    case PIC_PURE_RUN_LENGTH: /* ��׼RLE8ѹ�� */
      status = channel_read_pure(pfile, scan, width, noCol, off, bytes);
      break;
    case PIC_MIXED_RUN_LENGTH: /* ���RLE8ѹ���� */
      status = channel_read_mixed(pfile, scan, width, noCol, off, bytes);
      break;
    default: /* ϵͳ�쳣 */
      assert(0);
      return 0;
    }
    if (status != 0) { /* ���������ֹͣ��ȡ���� */
      break;
    }
  }
  return status;
}
/* ��һδѹ������������ */
static ulong channel_read_raw(ISFILE* file,
    uchar* scan,
    ulong width,
    ulong noCol,
    ulong* off,
    ulong bytes)
{
  int i, j;
  for (i = 0; i < (int)width; i++) {
    if (isio_eof(file)) {
      return 1; /* Դ����ȱ�� */
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
static ulong channel_read_pure(ISFILE* file,
    uchar* scan,
    ulong width,
    ulong noCol,
    ulong* off,
    ulong bytes)
{
  uchar col[4];
  ulong count = 0;
  int i, j, k;
  for (i = (int)width; i > 0;) {
    /* �������ֵ */
    if (isio_read((void*)&count, 1, 1, file) == 0) {
      return 1; /* Դ����ȱ�� */
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
static ulong channel_read_mixed(ISFILE* file,
    uchar* scan,
    ulong width,
    ulong noCol,
    ulong* off,
    ulong bytes)
{
  ulong count = 0;
  int i, j, k;
  uchar col[4];
  for (i = 0; i < (int)width; i += count) {
    if (isio_eof(file)) {
      return 1; /* Դ����ȱ�� */
    }
    /* �������ֵ */
    if (isio_read((void*)&count, 1, 1, file) == 0) {
      return 2; /* ����д�쳣 */
    }
    if (count >= 128) { /* �ظ�����ѹ�������� */
      if (count == 128) {
        if (isio_read((void*)&count, 2, 1, file) == 0) {
          return 2;
        }
        count = (ulong)EXCHANGE_WORD(((ushort)count));
      }
      else {
        count -= 127;
      }
      if ((i + (int)count) > (int)width) {
        return 3; /* Դ���ݷǷ� */
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
        return 3; /* Դ���ݷǷ� */
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

