// GIFͼ���дģ��ʵ���ļ�
// ��ȡ���ܣ�1~8λ����̬��̬GIFͼ��
// ���湦�ܣ��ݲ��ṩ���湦��

/* GIF�汾��ʶ������ */
#define GIFVERSIZE          6

struct _tagGIFHEADER;
typedef struct _tagGIFHEADER    GIFHEADER;
typedef struct _tagGIFHEADER*    LPGIFHEADER;

struct _tagGIFINFO;
typedef struct _tagGIFINFO      GIFINFO;
typedef struct _tagGIFINFO*      LPGIFINFO;

struct _tagGIFRGB;
typedef struct _tagGIFRGB     GIFRGB;
typedef struct _tagGIFRGB*     LPGIFRGB;

struct _tagISEERGB;
typedef struct _tagISEERGB      ISEERGB;
typedef struct _tagISEERGB*      LPISEERGB;

struct _tagIMAGEDATAINFO;
typedef struct _tagIMAGEDATAINFO  IMAGEDATAINFO;
typedef struct _tagIMAGEDATAINFO*  LPIMAGEDATAINFO;

struct _tagGIFINSIDEINFO;
typedef struct _tagGIFINSIDEINFO  GIFINSIDEINFO;
typedef struct _tagGIFINSIDEINFO*  LPGIFINSIDEINFO;

struct _tagLZWTABLE;
typedef struct _tagLZWTABLE     LZWTABLE;
typedef struct _tagLZWTABLE*     LPLZWTABLE;

union _tagCODEWORD;
typedef union _tagCODEWORD      CODEWORD;
typedef union _tagCODEWORD*      LPCODEWORD;

struct _tagGRAPHCTRL;
typedef struct _tagGRAPHCTRL    GRAPHCTRL;
typedef struct _tagGRAPHCTRL*    LPGRAPHCTRL;

struct _tagNOTEHCTRL;
typedef struct _tagNOTEHCTRL    NOTEHCTRL;
typedef struct _tagNOTEHCTRL*    LPNOTEHCTRL;

struct _tagTEXTCTRL;
typedef struct _tagTEXTCTRL     TEXTCTRL;
typedef struct _tagTEXTCTRL*     LPTEXTCTRL;

struct _tagAPPCTRL;
typedef struct _tagAPPCTRL      APPCTRL;
typedef struct _tagAPPCTRL*      LPAPPCTRL;


/*@@@@@@@@@@@@@@@GIF87a �� GIF89a ���õĽṹ@@@@@@@@@@@@@@@@@@*/

struct _tagGIFINFO {
  unsigned short FileType;    /* 89a or 87a */
  unsigned short ColorNum;    /* ��ɫ�� */
  unsigned short BitCount;    /* ͼ���λ��� */
  unsigned short Width;
  unsigned short Height;
  unsigned short FrameCount;    /* ���м���ͼ */
  unsigned char  bkindex;     /* ����ɫ���� */
  unsigned char  InitPixelBits; /* ѹ��������ʼ����Bitsλ�� */
};


/* GIF��ɫ��Ԫ�ؽṹ */
struct _tagGIFRGB {
  unsigned char bRed;
  unsigned char bGreen;
  unsigned char bBlue;
};

/* ISee��ɫ��Ԫ�ؽṹ */
struct _tagISEERGB {
  unsigned char bBlue;
  unsigned char bGreen;
  unsigned char bRed;
  unsigned char rev;
};

/* GIFͼ��ͷ�ṹ */
struct _tagGIFHEADER {
  unsigned char gif_type[GIFVERSIZE]; /* ͼ��汾(87a or 89a) */
  unsigned short  scr_width;        /* �߼������ */
  unsigned short  scr_height;       /* �߼����߶� */
  unsigned char global_flag;      /* ȫ�ֱ�� */
  unsigned char bg_color;       /* ����ɫ���� */
  unsigned char aspect_radio;     /* 89a��ĳ���� */
};


/* ͼ�������� */
struct _tagIMAGEDATAINFO {
  unsigned char label;          /* ��ǩֵ��0x2c */
  unsigned short  left;           /* ��ͼ�����Ͻ����߼���Ļ�е�λ�� */
  unsigned short  top;
  unsigned short  width;
  unsigned short  height;
  unsigned char local_flag;       /* ����λ�� */
};

#define GIF_MAXCOLOR  256

/* GIF�ۺ���Ϣ�ṹ */
struct _tagGIFINSIDEINFO {
  int       type;         /* GIF�汾��87a = 2 89a = 1 */

  unsigned long scr_width;        /* �߼�����Ϣ */
  unsigned long scr_height;
  unsigned long scr_bitcount;
  GIFRGB      p_pal[GIF_MAXCOLOR];  /* ȫ�ֵ�ɫ������ */

  unsigned long first_img_width;    /* �׷�ͼ����Ϣ */
  unsigned long first_img_height;
  unsigned long first_img_bitcount;
  GIFRGB      p_first_pal[GIF_MAXCOLOR];

  int       img_count;        /* ���м���ͼ */
  int       bk_index;       /* ����ɫ���� */
};


#define LZWTABLESIZE  4096  /* GIF-LZW ����ߴ� */

/*  GIF-LZW �ֵ䵥Ԫ�ṹ */
struct _tagLZWTABLE {
  short prefix;     /* ǰ׺����������ǰ׺��ʱΪ-1�� */
  short code;     /* β�� */
  int   used;     /* �Ƿ�ռ�ñ�־��0��δռ�ã�1����ռ�� */
};


/* GIF-LZW���뵥Ԫ */
union _tagCODEWORD {
  unsigned char bcode[sizeof(unsigned long)];
  unsigned long lcode;
};


/*--------------------------------------------------------
  ע�����½ṹֻ������GIF89a��ʽ��ͼ���ļ�
--------------------------------------------------------*/

/* ͼ�ο�����չ�� */
struct _tagGRAPHCTRL {
  unsigned char extintr;      /* 0x21 */
  unsigned char label;        /* 0xF9 */
  unsigned char block_size;     /* 0x04 */
  unsigned char field;
  unsigned short  delay_time;     /* �ӳ�ʱ�� */
  unsigned char tran_color_index; /* ͸��ɫ���� */
  unsigned char block_terminator; /* 0x00 */
};

/* ע����չ�� */
struct _tagNOTEHCTRL {
  unsigned char extintr;        /* 0x21 */
  unsigned char label;        /* 0xfe */
};

/* �ı���չ�� */
struct _tagTEXTCTRL {
  unsigned char extintr;      /* 0x21 */
  unsigned char label;        /* 0x01 */
  unsigned char block_size;     /* 0x0c */
  unsigned short  left;
  unsigned short  top;
  unsigned short  width;
  unsigned short  height;
  unsigned char fore_color_index; /* ǰ��ɫ���� */
  unsigned char bk_color_index;   /* ����ɫ���� */
};

/* Ӧ�ó�����չ�� */
struct _tagAPPCTRL {
  unsigned char extintr;        /* 0x21 */
  unsigned char label;        /* 0xff */
  unsigned char block_size;     /* 0xb  */
  unsigned char identifier[8];
  unsigned char authentication[3];
};

/* add by menghui 2003.06.18 */
#define MAXGIFBITS 12


/*==================================================================*/
/* �ڲ����ֺ��� */
/*==================================================================*/


/* ���뾲̬GIFͼ������ */
int CALLAGREEMENT _load_static_gif(
    LPINFOSTR pinfo_str,
    unsigned char* stream,
    unsigned long stream_len,
    LPGIFINSIDEINFO p_info)
{
  assert(pinfo_str->imgtype == IMT_RESSTATIC);

  /* ����ͼ��ʽ������ͼ�� */
  if (_load_subimg_serial(pinfo_str, stream, stream_len) == -1) {
    return -1;
  }

  assert(pinfo_str->imgnumbers == 1);

  /* ��ͼ�����ݴ���ͼ��ṹ���Ƶ���ͼ��ṹ */
  memcpy((void*)pinfo_str->palette, (const void*)pinfo_str->psubimg->palette, sizeof(unsigned long)*MAX_PALETTE_COUNT);
  pinfo_str->pal_count = pinfo_str->psubimg->pal_count;
  pinfo_str->pp_line_addr = pinfo_str->psubimg->pp_line_addr;
  pinfo_str->p_bit_data = pinfo_str->psubimg->p_bit_data;

  memset((void*)pinfo_str->psubimg, 0, sizeof(SUBIMGBLOCK));

  /* �ͷ���ͼ��ṹ */
  _free_SUBIMGBLOCK(pinfo_str->psubimg);

  pinfo_str->psubimg = 0;

  /* �ص�֪ͨ */
  switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_MAST_IMAGE, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
  case  0:
    break;

  case  1:
    return -2;
    break;   /* �û��ж� */

  case  3:
    return -4;
    break;   /* �ڴ治�� */

  case  2:
  default:
    return -3;
    break;   /* ϵͳ�쳣 */
  }

  /* �ص�֪ͨ */
  switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_LOAD)) {
  case  0:
    break;

  case  1:
    return -2;
    break;   /* �û��ж� */

  case  3:
    return -4;
    break;   /* �ڴ治�� */

  case  2:
  default:
    return -3;
    break;   /* ϵͳ�쳣 */
  }

  switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
  case  0:
    break;

  case  1:
    return -2;
    break;   /* �û��ж� */

  case  3:
    return -4;
    break;   /* �ڴ治�� */

  case  2:
  default:
    return -3;
    break;   /* ϵͳ�쳣 */
  }

  return 0;
}



/* ���붯̬GIFͼ������ */
int CALLAGREEMENT _load_dyn_gif(
    LPINFOSTR pinfo_str,
    unsigned char* stream,
    unsigned long stream_len,
    LPGIFINSIDEINFO p_info)
{
  LPGIFHEADER   pgif_header = (LPGIFHEADER)stream;
  LPSUBIMGBLOCK subimg_tmp;
  int       i;

  assert(pinfo_str->imgtype == IMT_RESDYN);

  /* ���뱳��ͼ */
  if (_build_bkimg(pinfo_str, p_info) == -1) {
    return -1;
  }

  /* ����ͼҲ������ͼ������� */
  pinfo_str->imgnumbers = 1;

  /* �ص�֪ͨ */
  switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_MAST_IMAGE, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
  case  0:
    break;

  case  1:
    return -2;
    break;   /* �û��ж� */

  case  3:
    return -4;
    break;   /* �ڴ治�� */

  case  2:
  default:
    return -3;
    break;   /* ϵͳ�쳣 */
  }

  /* �ص�֪ͨ */
  switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_LOAD)) {
  case  0:
    break;

  case  1:
    return -2;
    break;   /* �û��ж� */

  case  3:
    return -4;
    break;   /* �ڴ治�� */

  case  2:
  default:
    return -3;
    break;   /* ϵͳ�쳣 */
  }

  switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
  case  0:
    break;

  case  1:
    return -2;
    break;   /* �û��ж� */

  case  3:
    return -4;
    break;   /* �ڴ治�� */

  case  2:
  default:
    return -3;
    break;   /* ϵͳ�쳣 */
  }

  /* ������ͼ������ */
  if (_load_subimg_serial(pinfo_str, stream, stream_len) == -1) {
    return -1;
  }

  assert(pinfo_str->imgnumbers > 1);

  subimg_tmp = pinfo_str->psubimg;
  assert(subimg_tmp);

  while (subimg_tmp) {
    /* �ص�֪ͨ */
    switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_SERT_IMAGE, subimg_tmp->number, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
    case  0:
      break;

    case  1:
      return -2;
      break;   /* �û��ж� */

    case  3:
      return -4;
      break;   /* �ڴ治�� */

    case  2:
    default:
      return -3;
      break;   /* ϵͳ�쳣 */
    }

    /* �ص�֪ͨ */
    switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, subimg_tmp->number, 0, subimg_tmp->height, IRWE_CALLBACK_FUNID_LOAD)) {
    case  0:
      break;

    case  1:
      return -2;
      break;   /* �û��ж� */

    case  3:
      return -4;
      break;   /* �ڴ治�� */

    case  2:
    default:
      return -3;
      break;   /* ϵͳ�쳣 */
    }

    switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, subimg_tmp->number, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
    case  0:
      break;

    case  1:
      return -2;
      break;   /* �û��ж� */

    case  3:
      return -4;
      break;   /* �ڴ治�� */

    case  2:
    default:
      return -3;
      break;   /* ϵͳ�쳣 */
    }

    subimg_tmp = subimg_tmp->next;
  }

  // ���䲥��˳���
  pinfo_str->play_order = (unsigned long*)isirw_malloc((pinfo_str->imgnumbers - 1) * sizeof(unsigned long*));

  if (pinfo_str->play_order) {
    for (i = 1; i < (int)pinfo_str->imgnumbers; i++) {
      pinfo_str->play_order[i - 1] = i;     // ���ò���˳�򣨿����ͼ��
    }

    pinfo_str->play_number = pinfo_str->imgnumbers - 1;
  }
  else {
    pinfo_str->play_number = 0;           // ����ȱʡ����˳��
  }

  return 0;
}


/* ��ȡGIFͼ����Ϣ
/
/ ����p_filecontentָ��Ļ������������������GIF������
/ ����filesize��ָ��p_filecontent��ָ��Ļ������ĳߴ磨��GIF���ĳߴ磩
/ �ɹ�����0��ʧ�ܷ���-1��ͼ��������Щ�����棬���ѳɹ���ȡ������1
/ ע�������һ����̬GIF��ͼ���������1����ͼ��Ŀ������ݽ���
/   �߼����Ŀ���Ϊ׼������Ǿ�̬GIF������ͼ�񣩣�����ͼ��
/   �Ŀ�������Ϊ׼��
*/
int CALLAGREEMENT _get_imginfo(unsigned char* p_filecontent, unsigned long filesize, LPGIFINSIDEINFO p_info)
{
  unsigned char*  p = p_filecontent;
  int       type = 0, i;
  unsigned char data_len;
  int       color_num;
  int       first_img_mark = 0;
  unsigned long len = 0;

  GIFHEADER   gifHeader;
  IMAGEDATAINFO image_data; /* ͼ�����ݿ� */


  assert(p_filecontent);
  assert(p_info);

  memcpy(&gifHeader, p, sizeof(GIFHEADER));

  if (!memcmp((const void*)gifHeader.gif_type, (const char*)GIF_MARKER_89a, strlen((const char*)GIF_MARKER_89a))) {
    type = 1;
  }
  else if (!memcmp((const void*)gifHeader.gif_type, (const char*)GIF_MARKER_87a, strlen((const char*)GIF_MARKER_87a))) {
    type = 2;
  }
  else {
    return -1;  /* �Ƿ�ͼ������ */
  }

  /* ��д�������� */
  p_info->type = type;
  p_info->scr_width    = (unsigned long)gifHeader.scr_width;
  p_info->scr_height   = (unsigned long)gifHeader.scr_height;
  /*p_info->scr_bitcount = (unsigned long)(((gifHeader.global_flag&0x70)>>4)+1);*/ /* GIF�ĵ��ڴ˴���˵����Щģ�� */
  p_info->bk_index     = (int)gifHeader.bg_color;

  if ((p_info->scr_width == 0) || (p_info->scr_height == 0)) {
    return -1;  /* �Ƿ�ͼ������ */
  }

  if (gifHeader.global_flag & 0x80) {
    p_info->scr_bitcount = (unsigned long)((gifHeader.global_flag & 0x7) + 1);

    /* ���Ա���ɫ�����ĺϷ��� */
    if (p_info->bk_index >= (1L << p_info->scr_bitcount)) {
      p_info->bk_index = (1L << p_info->scr_bitcount) - 1;  /* �Ƿ����ݴ�������������� */
    }

    /* return -1; �����Դͼ������Ҫ���ϸ�Ļ����ɽ����� */
    /* һ�еĴ����ñ���return -1;����滻��*/
  }
  else {
    p_info->scr_bitcount = (unsigned long)0;
    p_info->bk_index = 0;
  }


  /* ���GIF��ͷ���߼���Ļ������ */
  p += sizeof(GIFHEADER);
  len += sizeof(GIFHEADER);

  /* �ж��Ƿ����ȫ�ֵ�ɫ�壬�����������ȡ���ݲ�����õ�ɫ�� */
  if (gifHeader.global_flag & 0x80) {
    color_num = 1 << (int)(unsigned int)((gifHeader.global_flag & 0x7) + 1);
    /*color_num = 1 << p_info->scr_bitcount;*/

    if (color_num > GIF_MAXCOLOR) {
      return -1;  /* �����ͼ�� */
    }

    if ((sizeof(GIFHEADER) + color_num * sizeof(GIFRGB)) >= (int)filesize) {
      return -1;  /* �����ͼ�� */
    }

    len += color_num * sizeof(GIFRGB);

    for (i = 0; i < color_num; i++) {
      p_info->p_pal[i].bRed = *p++;
      p_info->p_pal[i].bGreen = *p++;
      p_info->p_pal[i].bBlue  = *p++;
    }
  }
  else {
    /* ���û��ȫ�ֵ�ɫ�����ݣ����ûҶ�ͼ���ݳ�ʼ����ɫ������ */
    for (i = 0; i < GIF_MAXCOLOR; i++) {
      p_info->p_pal[i].bRed   = (unsigned char)i;
      p_info->p_pal[i].bGreen = (unsigned char)i;
      p_info->p_pal[i].bBlue  = (unsigned char)i;
    }
  }

  /* �ֽ������� */
  while (1) {
    if (p[0] == 0x2c) { /* ͼ����������[�ֲ���ɫ��]��ͼ�����ݿ� */
      if ((len + sizeof(IMAGEDATAINFO)) >= filesize) {
        return -1;  /* �����ͼ�� */
      }

      /* ���ͼ���������ṹ */
      memcpy(&image_data, p, sizeof(IMAGEDATAINFO));
      p += sizeof(IMAGEDATAINFO);
      len += sizeof(IMAGEDATAINFO);

      if (image_data.local_flag & 0x80) {
        if ((len + (1 << (int)(unsigned int)((image_data.local_flag & 0x7) + 1))*sizeof(GIFRGB)) > filesize) {
          return -1;
        }
        else {
          len += (1 << (int)(unsigned int)((image_data.local_flag & 0x7) + 1)) * sizeof(GIFRGB);
        }
      }

      /* ��д�׷�ͼ����Ϣ */
      if (first_img_mark == 0) {
        p_info->first_img_width = (unsigned long)image_data.width;
        p_info->first_img_height = (unsigned long)image_data.height;

        if (image_data.local_flag & 0x80) {
          p_info->first_img_bitcount = (int)(unsigned int)((image_data.local_flag & 0x7) + 1);
        }
        else if (p_info->scr_bitcount) {
          p_info->first_img_bitcount = p_info->scr_bitcount;
        }
        else {
          p_info->first_img_bitcount = 1;   /* �ݴ��� */
        }

        /*
        if (image_data.local_flag&0x80)
          p_info->first_img_bitcount = (unsigned long)((image_data.local_flag&0x7)+1);
        else
          p_info->first_img_bitcount = p_info->scr_bitcount;
        */

        if ((p_info->first_img_width == 0) || (p_info->first_img_height == 0)) {
          return -1;  /* �����ͼ�� */
        }

        p_info->img_count = 0;

        /* ��ȡ�ֲ���ɫ������ */
        if (image_data.local_flag & 0x80) {
          color_num = 1 << (int)(unsigned int)((image_data.local_flag & 0x7) + 1);
          //color_num = 1 << p_info->first_img_bitcount;

          if (color_num > GIF_MAXCOLOR) {
            return -1;    /* �����ͼ�� */
          }

          for (i = 0; i < color_num; i++) {
            p_info->p_first_pal[i].bRed   = *p++;
            p_info->p_first_pal[i].bGreen = *p++;
            p_info->p_first_pal[i].bBlue  = *p++;
          }
        }
        else {
          memmove((void*)p_info->p_first_pal, (const void*)p_info->p_pal, sizeof(GIFRGB)*GIF_MAXCOLOR);
        }

        first_img_mark = 1;
      }
      else {
        /* ���׷�ͼ�����ӵ�оֲ���ɫ�����ݣ����� */
        if (image_data.local_flag & 0x80) {
          p += (1 << (int)(unsigned int)((image_data.local_flag & 0x7) + 1)) * sizeof(GIFRGB);
        }
      }

      if ((len + 1) > filesize) {
        return -1;
      }

      p++;        /* Խ��GIF-LZW��С����ֵ */
      len++;
      data_len = 1;

      /* �����ѹ����ͼ�������ӿ� */
      while (data_len) {
        if ((len + 1) > filesize) {
          return -1;  /* �����ͼ���� */
        }

        data_len = *p++;
        p += data_len;
        len += data_len + 1;
      }

      /* ͼ�������1 */
      p_info->img_count++;
    }
    else if (p[0] == 0x21 && p[1] == 0xf9 && p[2] == 0x4) { /* ͼ�ο�����չ�� */
      if ((len + sizeof(GRAPHCTRL)) > filesize) {
        return -1;  /* �����ͼ���� */
      }

      p += sizeof(GRAPHCTRL);
      len += sizeof(GRAPHCTRL);
    }
    else if (p[0] == 0x21 && p[1] == 0x1 && p[2] == 0xc) { /* �ı���չ�� */
      if ((len + sizeof(TEXTCTRL)) > filesize) {
        return -1;  /* �����ͼ���� */
      }

      p += sizeof(TEXTCTRL);
      len += sizeof(TEXTCTRL);

      data_len = 1;

      /* ����ı��ӿ�����  */
      while (data_len) {
        if ((len + 1) > filesize) {
          return -1;  /* �����ͼ���� */
        }

        data_len = *p++;
        p += data_len;
        len += data_len + 1;
      }
    }
    else if (p[0] == 0x21 && p[1] == 0xff && p[2] == 0xb) { /* Ӧ�ó�����չ�� */
      if ((len + sizeof(APPCTRL)) > filesize) {
        return -1;  /* �����ͼ���� */
      }

      p += sizeof(APPCTRL);
      len += sizeof(APPCTRL);

      data_len = 1;

      /* ����ı��ӿ�����  */
      while (data_len) {
        if ((len + 1) > filesize) {
          return -1;  /* �����ͼ���� */
        }

        data_len = *p++;
        p += data_len;
        len += data_len + 1;
      }
    }
    else if (p[0] == 0x21 && p[1] == 0xfe) {        /* ע����չ�� */
      if ((len + sizeof(NOTEHCTRL)) > filesize) {
        return -1;  /* �����ͼ���� */
      }

      p += sizeof(NOTEHCTRL);
      len += sizeof(NOTEHCTRL);

      data_len = 1;

      /* ����ı��ӿ�����  */
      while (data_len) {
        if ((len + 1) > filesize) {
          return -1;  /* �����ͼ���� */
        }

        data_len = *p++;
        p += data_len;
        len += data_len + 1;
      }
    }
    else if (p[0] == 0x0) {               /* �˳�������ӿ��β������������ĵ���û��˵�������ֿ��β�����˴�Ϊ������� */
      p++;
      len++;
    }
    else if (p[0] == 0x3b) {                /* ���� */
      break;
    }
    else if (first_img_mark == 1) {           /* �ݴ���������Щͼ������β��û�н�����־��GIF��*/
      break;
    }
    else {
      return -1;  /* �����ͼ���� */
    }

    if (len > filesize) {
      return -1;  /* �����ͼ���� */
    }
  }

  if ((len + 1) != filesize) {
    return 1;   /* ��ͼ����β���������ݣ���������GIF�����ݣ����� */
    /* ��ͼ�񱻸������������Ϣ�������ⲻӰ��ͼ���  */
    /* ������ȡ�������Դ���:)�����Է���1������-1     */
  }

  /* �ɹ����� */
  return 0;
}


/* ��������LZWѹ�����ݽ�ѹΪBMP���ݣ�������Щ���ݱ�����һ��������������ݽڵ��� */
LPSUBIMGBLOCK CALLAGREEMENT _decomp_LZW_to_BMP(
    LPINFOSTR pinfo_str,      /* ��ͼ����ַ */
    unsigned char* plzw,      /* ͼ����׵�ַ */
    unsigned char* stream,      /* GIF���׵�ַ */
    LPGRAPHCTRL p_imgctrl,      /* �����Ч��ͼ�������չ�� */
    unsigned long* block_size)    /* ����ͼ�����ݿ��ʵ�ʳߴ� */
{
  LZWTABLE    str_table[LZWTABLESIZE];        /* GIF-LZW ���� */
  unsigned char cw_buf[LZWTABLESIZE];         /* ����ִ�ʱʹ�õĻ����� */

  LPGIFHEADER   p_header = (LPGIFHEADER)stream;     /* GIF��ͷ�ṹ */
  LPIMAGEDATAINFO p_imginfo = (LPIMAGEDATAINFO)plzw;
  unsigned char*   p_data, *p;

  unsigned char first_char;
  int       cw_buf_len, percw;
  int       linesize, init_bit_len;
  unsigned long imgdata_size, imgblock_size;

  int       lzw_clear_code, lzw_eoi_code, cur_bit_len;
  int       cur_insert_pos, cur_code, old_code, old_code_mark;
  /* Ŀ�껺������ǰдλ����ز��� */
  int       cur_row, cur_x, cur_row_bit_count, intr, pass;
  unsigned short  code_buf;
  unsigned long bit_count;

  unsigned char*   p_lzwdata = 0;
  LPSUBIMGBLOCK p_subimg   = 0;

  assert(plzw && ((*plzw) == 0x2c));
  assert(stream && ((*stream) == 'G'));

  __try {
    /* ����һ����ͼ��ڵ� */
    if ((p_subimg = _alloc_SUBIMGBLOCK(pinfo_str, plzw, stream, p_imgctrl)) == 0) {
      return 0;  /* �ڴ治���ͼ�����ݷǷ� */
    }

    assert((p_subimg->p_bit_data) && (p_subimg->pp_line_addr));

    /* ����ͼ������ƫ�� */
    if (p_imginfo->local_flag & 0x80) {
      p_data = plzw + sizeof(IMAGEDATAINFO) + (1 << (int)(unsigned int)((p_imginfo->local_flag & 0x7) + 1)) * sizeof(GIFRGB);
    }
    else {
      p_data = plzw + sizeof(IMAGEDATAINFO);
    }

    /* DIB�гߴ� */
    linesize = DIBSCANLINE_WIDTHBYTES(p_subimg->width * p_subimg->bitcount);

    p = p_data;

    /* ��ȡ��ʼλ����ֵ */
    init_bit_len = (int) * p++;

    /* ��ȡ��LZW���ݳ��Ⱥ�LZW���ݿ鳤�� */
    _get_imgdata_size(p, &imgdata_size, &imgblock_size);

    /* ����ʵ��ͼ��飨LZW�飩�ĳ��� */
    *block_size = imgblock_size;

    /* ��LZW������ȡ��һ���µġ��������ڴ���� */
    if ((p_lzwdata = _get_lzw_datablock(p, imgdata_size)) == 0) {
      _free_SUBIMGBLOCK(p_subimg);
      return 0;         /* �ڴ治�� */
    }


    /* ����ǰ�ĳ�ʼ������ */
    p = p_lzwdata;

    intr = (p_imginfo->local_flag & 0x40) ? 1 : 0; /* �����־ */
    cur_row = cur_x = cur_row_bit_count = 0;  /* Ŀ��ͼ�񻺳����С���λ�� */
    pass = 1;                 /* ����ͨ��ֵ */

    lzw_clear_code = 1 << init_bit_len;   /* ����� */
    lzw_eoi_code   = lzw_clear_code + 1;    /* �ս��� */
    cur_bit_len    = init_bit_len + 1;    /* ��ǰ�볤�� */
    cur_insert_pos = lzw_eoi_code + 1;    /* ��ǰ�������λ�� */
    bit_count      = 0;             /* �����ж�ȡ������λ���� */
    old_code       = 0;             /* ǰ׺�� */
    old_code_mark  = 0;             /* �ױ�־ */


    /* ��ʼ���� */
    while ((cur_code = (int)_get_next_codeword(p, imgdata_size, bit_count / 8, bit_count % 8, cur_bit_len)) != lzw_eoi_code) {
      if (cur_code == lzw_clear_code) {     /* ����� */
        bit_count      += cur_bit_len;
        cur_bit_len    = init_bit_len + 1;
        cur_insert_pos = lzw_eoi_code + 1;

        old_code       = 0;
        old_code_mark  = 0;

        /* ��ʼ������ */
        _init_lzw_table((LZWTABLE*)str_table, LZWTABLESIZE, init_bit_len);
        continue;
      }
      else if (cur_code < cur_insert_pos) {   /* �����ڴ����� */
        /* ������� */
        cw_buf_len = 0;
        percw = cur_code;

        /* �����ֶ�Ӧ��ͼ�����ݴ����뻺���� */
        while (percw != -1) {
          /* assert(str_table[percw].used == 1); */
          cw_buf[cw_buf_len++] = (unsigned char)str_table[percw].code;
          percw = (int)str_table[percw].prefix;
        }

        /* д��Ŀ��ͼ�񻺳��� */
        while (cw_buf_len > 0) {
          if (intr) { /* �Ƿ��ǽ���ģʽ */
            if (cur_x == p_subimg->width) {
              cur_x = 0;
              cur_row_bit_count = 0;

              /* �������� */
              if (pass == 1) {
                cur_row += 8;
              }

              if (pass == 2) {
                cur_row += 8;
              }

              if (pass == 3) {
                cur_row += 4;
              }

              if (pass == 4) {
                cur_row += 2;
              }

              if (cur_row >= p_subimg->height) {
                pass++;
                cur_row = 0x10 >> pass;
              }
            }
          }
          else {
            if (cur_x == p_subimg->width) {
              cur_x = 0;
              cur_row_bit_count = 0;
              cur_row++;
            }
          }

          /* �ݴ�����cur_row < heightʱ���� by menghui 2003.06.18 */
          if (cur_row < p_subimg->height) {
            /* ���ͼ�����ݲ��ƶ���Ԥ��λ�� */
            code_buf = (unsigned short)(unsigned char)(cw_buf[cw_buf_len - 1]);
            code_buf &= (unsigned short)((1 << (p_subimg->bitcount)) - 1);
            code_buf <<= cur_row_bit_count % 8;

            /* ���������ݰ�����λд��ͼ�񻺳��� */
            *((unsigned char*)(((unsigned char*)(p_subimg->pp_line_addr[cur_row])) + cur_row_bit_count / 8)) |= (unsigned char)code_buf;

            if (((cur_row_bit_count % 8) + p_subimg->bitcount) > 8) {
              *((unsigned char*)(((unsigned char*)(p_subimg->pp_line_addr[cur_row])) + cur_row_bit_count / 8 + 1)) |= (unsigned char)(code_buf >> 8);
            }
          }

          cur_x++;
          cur_row_bit_count += p_subimg->bitcount;

          cw_buf_len--;
        }

        /* �Ƿ��ǵ�һ�ζ��� */
        if (old_code_mark) {
          /* �򴮱�����������֣�������������£� */
          percw = cur_code;

          /* ȡ�õ�ǰ���ֵ�һ���ַ� */
          while (percw != -1) {
            /* assert(str_table[percw].used == 1); */
            first_char = (unsigned char)str_table[percw].code;
            percw = (int)str_table[percw].prefix;
          }

          /* ��������ֵ�LZW������ */
          str_table[cur_insert_pos].used = 1;
          str_table[cur_insert_pos].code = (short)first_char;
          str_table[cur_insert_pos].prefix = old_code;

          /* ����λ�ú��� */
          cur_insert_pos++;
        }
        else {
          old_code_mark = 1;  /* ��һ�ν��� */
        }

        old_code = cur_code;
        bit_count += cur_bit_len;
      }
      else {                  /* �����ڵı��� */
        /* �򴮱�����������֣���������������£� */
        percw = old_code;

        /* ȡ��ǰ׺���ֵ�һ���ַ� */
        while (percw != -1) {
          /* assert(str_table[percw].used == 1); */
          first_char = (unsigned char)str_table[percw].code;
          percw = (int)str_table[percw].prefix;
        }

        /* ��������� */
        str_table[cur_insert_pos].used = 1;
        str_table[cur_insert_pos].code = (short)first_char;
        str_table[cur_insert_pos].prefix = old_code;

        cur_insert_pos++;

        /* ������� */
        cw_buf_len = 0;
        /* ����ո����ɵ������� */
        percw = cur_insert_pos - 1;

        /* �����ֶ�Ӧ��ͼ�����ݴ����뻺���� */
        while (percw != -1) {
          /* assert(str_table[percw].used == 1); */
          cw_buf[cw_buf_len++] = (unsigned char)str_table[percw].code;
          percw = (int)str_table[percw].prefix;
        }

        while (cw_buf_len > 0) {
          if (intr) { /* �Ƿ��ǽ���ģʽ */
            if (cur_x == p_subimg->width) {
              cur_x = 0;
              cur_row_bit_count = 0;

              if (pass == 1) {
                cur_row += 8;
              }

              if (pass == 2) {
                cur_row += 8;
              }

              if (pass == 3) {
                cur_row += 4;
              }

              if (pass == 4) {
                cur_row += 2;
              }

              if (cur_row >= p_subimg->height) {
                pass++;
                cur_row = 0x10 >> pass;
              }
            }
          }
          else {
            if (cur_x == p_subimg->width) {
              cur_x = 0;
              cur_row_bit_count = 0;
              cur_row++;
            }
          }

          /* ֻ��˵�Ǳ�׼��GIF̫����!!!
          // ��Ȼ��ʵ�ʸ߶ȱ��Լ������Ҫ�������������
          // by menghui 2003.06.18 */
          if (cur_row < p_subimg->height) {
            code_buf = (unsigned short)(cw_buf[cw_buf_len - 1]);
            code_buf &= (unsigned short)((1 << (p_subimg->bitcount)) - 1);
            code_buf <<= cur_row_bit_count % 8;

            /* ���������ݰ�����λд��ͼ�񻺳��� */
            *((unsigned char*)(((unsigned char*)(p_subimg->pp_line_addr[cur_row])) + cur_row_bit_count / 8)) |= (unsigned char)code_buf;

            if (((cur_row_bit_count % 8) + p_subimg->bitcount) > 8) {
              *((unsigned char*)(((unsigned char*)(p_subimg->pp_line_addr[cur_row])) + cur_row_bit_count / 8 + 1)) |= (unsigned char)(code_buf >> 8);
            }
          }

          cur_x++;
          cur_row_bit_count += p_subimg->bitcount;

          cw_buf_len--;
        }

        old_code = cur_code;
        bit_count += cur_bit_len;
      }

      if ((cur_insert_pos >> cur_bit_len) > 0) {
        cur_bit_len++;
      }

      if (cur_bit_len > MAXGIFBITS) {
        cur_bit_len = MAXGIFBITS;
      }

      /*
       *  �����һ��Ϊ if(bit_count / 8 > imgblock_size) break;
       *  ��֪���ǲ�����������ô˵�ģ���ʵ��Ӧ���г������⣬�ҽ������ⱨ���yzfree����
       *  Ҳ��yzfree̫æ�������ԭ����û�з��ִ˴��Ĵ�����Ǽ�����һ��warninglevel�ı�������ʱ������ѹ��
       *  ��Ҳʹ��һЩGIFͼ�޷���ʾ��
       *  �������������ľ���ϸ�Ŀ�����δ��룬���ִ��󼴸�֮��
       *  by menghui 2003.06.18
       */
      if (bit_count / 8 >= imgdata_size) {
        break;
      }
    }
  }
  __finally {
    if (AbnormalTermination()) {
      if (p_subimg) {
        _free_SUBIMGBLOCK(p_subimg);
      }

      p_subimg = 0;
    }

    if (p_lzwdata) {
      isirw_free(p_lzwdata);
    }

    p_lzwdata = 0;
  }

  return p_subimg;
}


int CALLAGREEMENT _load_subimg_serial(LPINFOSTR pinfo_str, unsigned char* stream, unsigned long stream_len)
{
  unsigned char* p = stream;
  unsigned char* pimg, *ptmp;
  int       type = 0, i, j;
  unsigned char data_len;
  unsigned long block_size;

  GIFHEADER   gifHeader;
  IMAGEDATAINFO image_data;     /* ͼ�����ݿ� */
  GRAPHCTRL   cur_craph_ctrl;   /* ��ǰ��ͼ��������� */

  LPSUBIMGBLOCK subimg_list = 0;  /* ��ͼ������ */
  LPSUBIMGBLOCK subimg_tail = 0;
  LPSUBIMGBLOCK subimg_tmp  = 0;


  assert(p);
  assert(pinfo_str);

  memset((void*)&cur_craph_ctrl, 0, sizeof(GRAPHCTRL));
  memcpy(&gifHeader, p, sizeof(GIFHEADER));

  /* ���GIF��ͷ���߼���Ļ������ */
  p += sizeof(GIFHEADER);

  /* ���ȫ�ֵ�ɫ�� */
  if (gifHeader.global_flag & 0x80) {
    p += (1 << ((gifHeader.global_flag & 0x7) + 1)) * sizeof(GIFRGB);
  }

  /* ��ʼ��ͼ��������� */
  cur_craph_ctrl.extintr      = 0x21;
  cur_craph_ctrl.label      = 0xf9;
  cur_craph_ctrl.block_size   = 0x4;
  cur_craph_ctrl.delay_time   = 0;
  cur_craph_ctrl.field      = 0;
  cur_craph_ctrl.tran_color_index = -1;
  cur_craph_ctrl.block_terminator = 0;


  /* �ֽ������� */
  while (1) {
    if (p[0] == 0x2c) { /* ͼ����������[�ֲ���ɫ��]��ͼ�����ݿ� */
      /* ����ͼ���������׵�ַ */
      pimg = p;

      /* ���ͼ���������ṹ */
      memcpy(&image_data, p, sizeof(IMAGEDATAINFO));
      p += sizeof(IMAGEDATAINFO);

      /* ���ӵ�оֲ���ɫ�����ݣ����� */
      if (image_data.local_flag & 0x80) {
        p += (1 << (int)(unsigned int)((image_data.local_flag & 0x7) + 1)) * sizeof(GIFRGB);
      }

      /* ����ͼ����н��� */
      subimg_tmp = _decomp_LZW_to_BMP(pinfo_str, pimg, stream, &cur_craph_ctrl, &block_size);

      /* ֧���û��жϲ��� */
      if (pinfo_str->pater->s_break) {
        _free_SUBIMGBLOCK(subimg_tmp);
        subimg_tmp = 0;
      }

      /* �������ʧ�ܣ��ͷ���ͼ���������ش��� */
      if (subimg_tmp == 0) {
        if (subimg_list != 0) {
          while (subimg_tmp = subimg_list->next) {
            _free_SUBIMGBLOCK(subimg_list);
            subimg_list = subimg_tmp;
          }

          _free_SUBIMGBLOCK(subimg_list);
          subimg_list = 0;
        }

        return -1;
      }

      /* ��1��4λͼ��תΪISee����ʾ��ʽ(�Է���2.2�ӿڱ�׼) */
      switch (subimg_tmp->bitcount) {
      case  1:
        for (i = 0; i < (int)subimg_tmp->height; i++) {
          ptmp = (unsigned char*)(subimg_tmp->pp_line_addr[i]);

          for (j = 0; j < (int)subimg_tmp->width; j += 8) {
            CVT_BITS1((*ptmp));       /* �ߵ�λ�� */
            ptmp++;
          }
        }

        break;

      case  4:
        for (i = 0; i < (int)subimg_tmp->height; i++) {
          ptmp = (unsigned char*)(subimg_tmp->pp_line_addr[i]);

          for (j = 0; j < (int)subimg_tmp->width; j += 2) {
            CVT_BITS4((*ptmp));       /* �ߵ�λ�� */
            ptmp++;
          }
        }

        break;

      default:
        break;
      }

      /* ����ͼ�������ͼ������β�� */
      if (subimg_list == 0) {
        subimg_list = subimg_tmp; /* ����ͷ */
        subimg_tail = subimg_tmp; /* ����β */

        subimg_tmp->prev = 0;
        subimg_tmp->next = 0;
        subimg_tmp->number = 1;
        subimg_tmp->parents = pinfo_str;
      }
      else {
        subimg_tmp->prev = subimg_tail;
        subimg_tmp->next = 0;
        subimg_tmp->number = subimg_tail->number + 1;
        subimg_tmp->parents = pinfo_str;

        assert(subimg_tail->next == 0);

        subimg_tail->next = subimg_tmp;
        subimg_tail = subimg_tmp;
      }

      /* ͼ�������1 */
      pinfo_str->imgnumbers++;

      /* ���ͼ������ */
      p += block_size;
    }
    else if (p[0] == 0x21 && p[1] == 0xf9 && p[2] == 0x4) { /* ͼ�ο�����չ�� */
      memcpy((void*)&cur_craph_ctrl, (const void*)p, sizeof(GRAPHCTRL));
      p += sizeof(GRAPHCTRL);
    }
    else if (p[0] == 0x21 && p[1] == 0x1 && p[2] == 0xc) { /* �ı���չ�� */
      p += sizeof(TEXTCTRL);
      data_len = 1;

      /* ����ı��ӿ�����  */
      while (data_len) {
        data_len = *p++;
        p += data_len;
      }
    }
    else if (p[0] == 0x21 && p[1] == 0xff && p[2] == 0xb) { /* Ӧ�ó�����չ�� */
      p += sizeof(APPCTRL);
      data_len = 1;

      /* ����ı��ӿ�����  */
      while (data_len) {
        data_len = *p++;
        p += data_len;
      }
    }
    else if (p[0] == 0x21 && p[1] == 0xfe) {        /* ע����չ�� */
      p += sizeof(NOTEHCTRL);
      data_len = 1;

      /* ����ı��ӿ�����  */
      while (data_len) {
        data_len = *p++;
        p += data_len;
      }
    }
    else if (p[0] == 0x0) {               /* �˳�������ӿ��β������������ĵ���û��˵�������ֿ��β�����˴�Ϊ������� */
      p++;
    }
    else if (p[0] == 0x3b) {                /* ���� */
      break;
    }
    else if (pinfo_str->imgnumbers >= 1) {        /* �ݴ����������Щͼ������β��û�н�����־��GIF��*/
      break;
    }
    else {
      /* �ͷ���ͼ������ */
      if (subimg_list != 0) {
        while (subimg_tmp = subimg_list->next) {
          _free_SUBIMGBLOCK(subimg_list);
          subimg_list = subimg_tmp;
        }

        _free_SUBIMGBLOCK(subimg_list);
        subimg_list = 0;
      }

      pinfo_str->imgnumbers = 0;

      return -1;  /* �����ͼ���� */
    }
  }

  pinfo_str->psubimg = subimg_list;

  /* �ɹ����� */
  return 0;
}



/* ��ʼ��LZW���� */
void CALLAGREEMENT _init_lzw_table(LZWTABLE* pt, int table_size, int init_bit_len)
{
  int i, len;

  assert(init_bit_len <= MAXGIFBITS);

  memset((void*)pt, 0, sizeof(LZWTABLE)*table_size);

  len = 1 << init_bit_len;

  /* ���� */
  for (i = 0; i < len; i++) {
    pt[i].prefix = -1;
    pt[i].code   = (short)i;
    pt[i].used   = 1;
  }

  /* ����� */
  pt[len].prefix = -1;
  pt[len].code   = -1;
  pt[len].used   = 1;

  /* ������ */
  pt[len + 1].prefix = -1;
  pt[len + 1].code   = -1;
  pt[len + 1].used   = 1;
}


/* ����һ����ͼ��ڵ㣬����ʼ���ڵ������� */
LPSUBIMGBLOCK CALLAGREEMENT _alloc_SUBIMGBLOCK(
    LPINFOSTR pinfo_str,
    unsigned char* plzw,       /* ͼ����׵�ַ */
    unsigned char* stream,     /* GIF���׵�ַ */
    LPGRAPHCTRL p_imgctrl)     /* �����Ч��ͼ�������չ�� */
{
  LPGIFHEADER   p_header = (LPGIFHEADER)stream;
  LPIMAGEDATAINFO p_imginfo = (LPIMAGEDATAINFO)plzw;
  LPGIFRGB    p_pal;

  int       linesize, i, colorkey;

  LPSUBIMGBLOCK p_subimg = 0;


  /* ������ͼ����Ϣ�ڵ�(SUBIMGBLOCK) */
  if ((p_subimg = (LPSUBIMGBLOCK)isirw_malloc(sizeof(SUBIMGBLOCK))) == 0) {
    return 0;  /* �ڴ治�� */
  }

  memset((void*)p_subimg, 0, sizeof(SUBIMGBLOCK));

  /* ������ͼ��ڵ����� */
  p_subimg->left = (int)p_imginfo->left;
  p_subimg->top = (int)p_imginfo->top;
  p_subimg->width = (int)p_imginfo->width;
  p_subimg->height = (int)p_imginfo->height;

  /* �ݴ���ĳЩ������ɵ�GIFͼ����global_flag�Ǵ���ģ�����ֻ���Լ���ȫ��bitcount��ȡ����ȷ������ */
  /* p_subimg->bitcount = (int)(unsigned int)(p_header->global_flag&0x7)+1; */
  p_subimg->bitcount = pinfo_str->bitcount;

  if ((p_subimg->width == 0) || (p_subimg->height == 0)) {
    isirw_free(p_subimg);   /* �Ƿ�ͼ������ */
    return 0;
  }

  /* �����ɫ��λ�� */
  if (p_imginfo->local_flag & 0x80) {
    p_pal = (LPGIFRGB)(unsigned char*)(plzw + sizeof(IMAGEDATAINFO));
  }
  else {
    if (!(p_header->global_flag & 0x80)) {
      isirw_free(p_subimg);
      return 0;       /* �Ƿ�ͼ�񣨼�û����ͼ��λ��Ҳû��ȫ��λ��ֵ��*/
    }
    else {
      p_pal = (LPGIFRGB)(unsigned char*)(p_header + 1);
    }
  }

  /* DIB�гߴ� */
  linesize = DIBSCANLINE_WIDTHBYTES(p_subimg->width * p_subimg->bitcount);

  /* ����Ŀ��ͼ�񻺳�����+4 �� ���ӵ�4�ֽڻ������� */
  if ((p_subimg->p_bit_data = (unsigned char*)isirw_malloc(linesize * p_subimg->height + 4)) == 0) {
    isirw_free(p_subimg);
    return 0;         /* �ڴ治�� */
  }

  /* �����е�ַ���� */
  if ((p_subimg->pp_line_addr = (void**)isirw_malloc(sizeof(void*)*p_subimg->height)) == 0) {
    isirw_free(p_subimg->p_bit_data);
    isirw_free(p_subimg);
    return 0;         /* �ڴ治�� */
  }

  /* ��ʼ��ͼ�񻺳��� */
  memset((void*)p_subimg->p_bit_data, 0, linesize * p_subimg->height);

  /* GIFΪ����ͼ�� */
  p_subimg->order   = 0;

  for (i = 0; i < p_subimg->height; i++) {
    p_subimg->pp_line_addr[i] = (void*)(p_subimg->p_bit_data + (i * linesize));
  }

  /* GIFΪ����ͼ������������ */
  p_subimg->r_mask = 0;
  p_subimg->g_mask = 0;
  p_subimg->b_mask = 0;
  p_subimg->a_mask = 0;


  /* ���÷�ʽ��0 - δָ�����÷�ʽ
         1 - �������κδ�����
         2 - �Ա���ɫ����
         3 - �ظ�ԭ���ı���ͼ�� */
  p_subimg->dowith = (int)((p_imgctrl->field & 0xc) >> 2);
  /* ����ʾ��һ����ͼ��ǰ�Ƿ���Ҫ�ȴ�ĳһ�¼� */
  p_subimg->userinput = (int)((p_imgctrl->field & 0x2) >> 1);
  /* �ӳ�ʱ�䣨1/100s תΪ 1/1000s��ȱʡ�ӳ�ʱ������Ϊ100ms��*/
  /* ��л zggeng##163.com ���ѵ����ѣ���2003-6��*/
  p_subimg->time = (p_imgctrl->delay_time == 0) ? 100 : ((int)(unsigned int)p_imgctrl->delay_time * 10);
  /* ����͸��ɫ���������û��͸��ɫ������Ϊ-1 */
  colorkey = (p_imgctrl->field & 0x1) ? p_imgctrl->tran_color_index : -1;


  /* ���Ƶ�ɫ������ */
  p_subimg->pal_count = 1 << p_subimg->bitcount;

  for (i = 0; i < p_subimg->pal_count; i++) {
    ((LPISEERGB)(&(p_subimg->palette[i])))->bRed  = p_pal[i].bRed;
    ((LPISEERGB)(&(p_subimg->palette[i])))->bGreen  = p_pal[i].bGreen;
    ((LPISEERGB)(&(p_subimg->palette[i])))->bBlue = p_pal[i].bBlue;
    ((LPISEERGB)(&(p_subimg->palette[i])))->rev   = 0;
  }

  /* ����͸��ɫ��ʵ��RGBֵ */
  if (colorkey != -1) {
    p_subimg->colorkey = p_subimg->palette[colorkey];

    do {
      /* �ж�͸��ɫ�Ƿ��ǵ�ɫ����Ψһ����ɫ */
      for (i = 0; i < p_subimg->pal_count; i++) {
        if ((p_subimg->palette[i] == (unsigned long)p_subimg->colorkey) && (i != colorkey)) {
          break;
        }
      }

      /* ��������ص��������޸�͸��ɫ */
      if (i < p_subimg->pal_count) {
        p_subimg->colorkey += 1;
        p_subimg->colorkey &= 0xffffff;
      }
      else {
        /* ������͸��ɫ��Ӧ�ĵ�ɫ�������� */
        p_subimg->palette[colorkey] = p_subimg->colorkey;
        break;
      }

    }
    while (i < p_subimg->pal_count);

  }
  else {
    p_subimg->colorkey = (unsigned long) - 1;
  }

  p_subimg->number  = 0;      /* ��ͼ�������к�����Ϊ0 */

  p_subimg->parents = 0;
  p_subimg->prev    = 0;
  p_subimg->next    = 0;

  return p_subimg;
}


/* �ͷ�һ����ͼ��ڵ㣬�������е�λ�����������׵�ַ���� */
void CALLAGREEMENT _free_SUBIMGBLOCK(LPSUBIMGBLOCK p_node)
{
  assert(p_node);

  if (p_node->pp_line_addr) {
    isirw_free(p_node->pp_line_addr);
  }

  if (p_node->p_bit_data) {
    isirw_free(p_node->p_bit_data);
  }

  isirw_free(p_node);
}


/* ��������ͼ����Զ�̬ͼ�� */
int CALLAGREEMENT _build_bkimg(LPINFOSTR pinfo_str, LPGIFINSIDEINFO p_info)
{
  int linesize, i;
  unsigned char* pline = 0;
  unsigned char* pimg  = 0;
  void** ppaddr = 0;
  unsigned char bkindex = p_info->bk_index;

  int byte_pos, bit_pos, bit_count;
  unsigned char c;

  assert((pinfo_str->width > 0) && (pinfo_str->height > 0));

  linesize = DIBSCANLINE_WIDTHBYTES(pinfo_str->width * pinfo_str->bitcount);

  /* ���������л����� */
  if ((pline = (unsigned char*)isirw_malloc(linesize)) == 0) {
    return -1;
  }

  /* ��ʼ��Ϊ0 */
  memset((void*)pline, 0, linesize);

  /* ���䱳��ͼ�񻺳�����+4 �� ���ӵ�4�ֽڻ������� */
  if ((pimg = (unsigned char*)isirw_orgpix_malloc(linesize * pinfo_str->height + 4)) == 0) {
    isirw_free(pline);
    return -1;
  }

  /* �������׵�ַ���� */
  if ((ppaddr = (void**)isirw_malloc(sizeof(void*)*pinfo_str->height)) == 0) {
    isirw_free(pimg);
    isirw_free(pline);
    return -1;
  }

  /* ��ʼ�����׵�ַ��GIF���� */
  for (i = 0; i < (int)pinfo_str->height; i++) {
    ppaddr[i] = (void*)(pimg + (i * linesize));
  }

  assert(pinfo_str->bitcount <= 8);
  assert(bkindex < (1 << pinfo_str->bitcount));

  /* ��д������ */
  byte_pos = bit_pos = bit_count = 0;

  for (i = 0; i < (int)pinfo_str->width; i++) {
    byte_pos = bit_count / 8;
    bit_pos  = bit_count % 8;

    c = (unsigned char)bkindex;
    c <<= bit_pos;
    pline[byte_pos] |= c;

    if ((bit_pos + pinfo_str->bitcount) > 8) {
      c = (unsigned char)bkindex;
      c >>= 8 - bit_pos;
      pline[byte_pos + 1] |= c;
    }

    bit_count += pinfo_str->bitcount;
  }

  /* ������ͼ��ʼ��Ϊ����ɫ�ĵ�ɫͼ */
  for (i = 0; i < (int)pinfo_str->height; i++) {
    memmove((void*)ppaddr[i], (const void*)pline, linesize);
  }


  pinfo_str->pp_line_addr = ppaddr;
  pinfo_str->p_bit_data   = pimg;
  pinfo_str->pal_count = 1 << pinfo_str->bitcount;

  /* ���Ƶ�ɫ������ */
  for (i = 0; i < (int)pinfo_str->pal_count; i++) {
    ((LPISEERGB)(&(pinfo_str->palette[i])))->bRed   = p_info->p_pal[i].bRed;
    ((LPISEERGB)(&(pinfo_str->palette[i])))->bGreen = p_info->p_pal[i].bGreen;
    ((LPISEERGB)(&(pinfo_str->palette[i])))->bBlue  = p_info->p_pal[i].bBlue;
    ((LPISEERGB)(&(pinfo_str->palette[i])))->rev    = 0;
  }

  isirw_free(pline);

  return 0;
}


/* ����ͼ��λ���ݵĳߴ磨������ͼ�����ݳߴ��ͼ�����ݿ������ߴ磩
/
/ ע����ߴ罫�����ײ��ĳ�ʼλ�õ��ֽڡ�
*/
void CALLAGREEMENT _get_imgdata_size(
    unsigned char* stream,      /* ָ��ͼ�����ݿ��׵�ַ��ָ�루�ѿ���˳�ʼλ�����ֽڣ�*/
    unsigned long* imgdata_size,
    unsigned long* imgblock_size)
{
  unsigned char* p = stream;
  unsigned long data_count, block_count;
  unsigned char data_len;

  assert(p);

  data_count = block_count = 0UL;

  while (1) {
    data_len = *p++;        /* ��ȡ���������ֽ�ֵ��������ָ�� */
    p += data_len;
    data_count += (unsigned long)data_len;
    block_count += ((unsigned long)data_len + 1);

    /* ��1�Ǹ����������ӿ�ĳ����ֽ� */
    if (data_len == 0) {
      break;
    }
  }

  block_count++;            /* ���LZW��ʼλֵ��һ���ֽ� */

  *imgdata_size  = data_count;
  *imgblock_size = block_count;
}


/* ��GIF�еĴ�LZWͼ��������ȡ���µ��ڴ����
/
/ ����ڴ����ʧ�ܣ�����0
*/
unsigned char* CALLAGREEMENT _get_lzw_datablock(
    unsigned char* stream,        /* ͼ����׵�ַ���ѿ����ʼλ�����ֽڣ�*/
    unsigned long lzw_len)
{
  unsigned char* p1, *p2, *pdata, data_len;
  unsigned long count = 0UL;

  assert(stream);

  /* �����ڴ�飬���ڴ�Ŵ�LZW���� */
  if ((pdata = (unsigned char*)isirw_malloc(lzw_len + 1)) == (unsigned char*)0) {
    return (unsigned char*)0;
  }

  p1 = stream;
  p2 = pdata;

  while (1) {
    data_len = *p1++;

    if (data_len == 0x00) {
      break;
    }

    /* ��ֹ�������ݸ��ų��� */
    count += (unsigned long)data_len;

    if (count > lzw_len) {
      isirw_free(pdata);
      return (unsigned char*)0;
    }

    /* ��LZW���ݸ��Ƶ��·�����ڴ���� */
    memcpy(p2, p1, data_len);

    p1 += data_len;
    p2 += data_len;
  }

  assert(count == lzw_len);

  return pdata;
}


/* ȡ��LZW����һ��������  */
unsigned short CALLAGREEMENT _get_next_codeword(
    unsigned char* code_stream,     /* LZW�������׵�ַ */
    unsigned long code_stream_len,    /* ������ */
    unsigned long offset_byte,      /* ��ǰ�������ֽ�ƫ�� */
    unsigned long offset_bits,      /* ��ǰ������λƫ�� */
    unsigned long code_word_bit_size) /* ����λ���� */
{
  CODEWORD  cw;
  LPCODEWORD  p_code_word;

  assert(code_word_bit_size <= MAXGIFBITS);

  /*
  * ��ʼ��lcode = 0 ���򣬾ͻ�������Ŷ��add by menghui 2003.06.18
   */
  cw.lcode = 0;

  p_code_word = (LPCODEWORD)(unsigned char*)(code_stream + offset_byte);

  /* 12λ��ȡ3���ֽڼ��� */
  if ((offset_byte + 0) < code_stream_len) {
    cw.bcode[0] = p_code_word->bcode[0];
  }

  if ((offset_byte + 1) < code_stream_len) {
    cw.bcode[1] = p_code_word->bcode[1];
  }

  if ((offset_byte + 2) < code_stream_len) {
    cw.bcode[2] = p_code_word->bcode[2];
  }

  /* �����������ֵͶ� */
  cw.lcode >>= offset_bits;
  cw.lcode  &= ((1 << code_word_bit_size) - 1);

  return (unsigned short)cw.lcode;
}



/* ��ȡͼ��λ���� */
GIF_API int CALLAGREEMENT gif_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;
  unsigned long stream_length;
  void*      ptmp = (void*)0;
  int       result;

  GIFINSIDEINFO inside;
  LPSUBIMGBLOCK subimg_tmp;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&gif_load_img_critical);

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

      /* �����ڴ棬���ڱ�������GIF�� */
      ptmp = isirw_malloc(stream_length);

      if (!ptmp) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ��ȡ����GIF�����ڴ� */
      if (isio_read(ptmp, stream_length, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ��ȡͼ����Ϣ */
      if (_get_imginfo((unsigned char*)ptmp, stream_length, &inside) == -1) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      inside.img_count = (inside.img_count > 1) ? inside.img_count + 1 : 1;

      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        /* ͼ�������� */
        pinfo_str->imgtype  = (inside.img_count > 1) ? IMT_RESDYN : IMT_RESSTATIC;
        pinfo_str->imgformat = IMF_GIF;     /* ͼ������ʽ����׺���� */
        pinfo_str->compression = ICS_GIFLZW;  /* ͼ��ѹ����ʽ */

        /* ����Ƕ�̬GIF����ȡ���߼��������� */
        if (pinfo_str->imgtype == IMT_RESDYN) {
          pinfo_str->width  = inside.scr_width;
          pinfo_str->height = inside.scr_height;
          pinfo_str->order  = 0;      /* GIFͼ��Ϊ���� */

          /* ���û��ȫ�ֵ�ɫ�����ݣ���ȡ�׷�ͼ���Ӧ���� */
          if (inside.scr_bitcount == 0) {
            pinfo_str->bitcount = inside.first_img_bitcount;
          }
          else {
            pinfo_str->bitcount = inside.scr_bitcount;
          }
        }
        else {
          pinfo_str->width  = inside.first_img_width;
          pinfo_str->height = inside.first_img_height;
          pinfo_str->order  = 0;      /* GIFͼ��Ϊ���� */
          pinfo_str->bitcount = inside.first_img_bitcount;
        }

        /* GIF��Ϊ����ͼ������û���������� */
        pinfo_str->b_mask = 0x0;
        pinfo_str->g_mask = 0x0;
        pinfo_str->r_mask = 0x0;
        pinfo_str->a_mask = 0x0;

        /* �趨���ݰ�״̬ */
        pinfo_str->data_state = 1;
      }


      /* ����GIFͼ�� */
      if (pinfo_str->imgtype == IMT_RESSTATIC) { /* ��̬ */
        result = _load_static_gif(pinfo_str, ptmp, stream_length, &inside);
      }
      else if (pinfo_str->imgtype == IMT_RESDYN) { /* ��̬ */
        result = _load_dyn_gif(pinfo_str, ptmp, stream_length, &inside);
      }
      else {
        assert(0);  /* ��ƴ��� */
      }


      switch (result) {
      case  0:
        break;

      case  -1:
        b_status = ER_BADIMAGE;
        __leave;
        break;

      case  -2:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  -3:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  -4:
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
      if (ptmp) {
        isirw_free(ptmp);
      }

      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        /* �ͷ���ͼ������ */
        if (pinfo_str->psubimg != 0) {
          while (subimg_tmp = pinfo_str->psubimg->next) {
            _free_SUBIMGBLOCK(pinfo_str->psubimg);
            pinfo_str->psubimg = subimg_tmp;
          }

          _free_SUBIMGBLOCK(pinfo_str->psubimg);
          pinfo_str->psubimg = 0;
        }

        /* �ͷ���ͼ�� */
        if (pinfo_str->pp_line_addr != 0) {
          isirw_free(pinfo_str->pp_line_addr);
        }

        if (pinfo_str->p_bit_data != 0) {
          isirw_free(pinfo_str->p_bit_data);
        }

        pinfo_str->pp_line_addr = 0;
        pinfo_str->p_bit_data   = 0;
        pinfo_str->pal_count    = 0;

        pinfo_str->imgnumbers = 0;

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
          /* �ͷ���ͼ������ */
          if (pinfo_str->psubimg != 0) {
            while (subimg_tmp = pinfo_str->psubimg->next) {
              _free_SUBIMGBLOCK(pinfo_str->psubimg);
              pinfo_str->psubimg = subimg_tmp;
            }

            _free_SUBIMGBLOCK(pinfo_str->psubimg);
            pinfo_str->psubimg = 0;
          }

          /* �ͷ���ͼ�� */
          if (pinfo_str->pp_line_addr != 0) {
            isirw_free(pinfo_str->pp_line_addr);
          }

          if (pinfo_str->p_bit_data != 0) {
            isirw_free(pinfo_str->p_bit_data);
          }

          pinfo_str->pp_line_addr = 0;
          pinfo_str->p_bit_data   = 0;
          pinfo_str->pal_count    = 0;

          pinfo_str->imgnumbers = 0;

          if (pinfo_str->data_state == 2) {
            pinfo_str->data_state = 1;  /* �Զ����� */
          }

          (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD);
        }
      }

      LeaveCriticalSection(&gif_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}

