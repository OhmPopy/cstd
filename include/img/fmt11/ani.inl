//ANIͼ���дģ��ʵ���ļ�
//          ��ȡ���ܣ�1��4��8��16��24��32λANIͼ��

/* ������Ͷ��� */
#define ANI_CURTYPE   2


/* ������ͼ�����Ͷ��� */
enum CUR_DATA_TYPE {
  ANI_CUR_PRI_IMAGE,
  ANI_CUR_SUB_IMAGE
};


/* ����ͼ�����Ϣͷ�ṹ */
typedef struct {
  unsigned char width;      /* ͼ���� */
  unsigned char height;     /* ͼ��߶� */
  unsigned char color_count;  /* ��ɫ����(���λ��ȴ���8ʱΪ0) */
  unsigned char reserved;
  unsigned short  x_hotspot;    /* �ȵ�X���� */
  unsigned short  y_hotspot;    /* �ȵ�Y���� */
  unsigned long bytes_in_res; /* ͼ����������Դ����ռ���ֽ��� */
  unsigned long image_offset; /* ͼ�����ݵ�ƫ�� */
} ANI_CURDIRENTRY, *LPANI_CURDIRENTRY;


/* �����Ϣͷ�ṹ */
typedef struct {
  unsigned short  reserved;   /* ���� */
  unsigned short  type;     /* ��Դ����(CURTYPEΪ���) */
  unsigned short  count;      /* ͼ����� */
  ANI_CURDIRENTRY entries[1];   /* ÿһ��ͼ������ */
} ANI_CURDIR, *LPANI_CURDIR;



/* RIFF ������ID���� */
#define RIFF_STR  "RIFF"
#define LIST_STR  "LIST"

#define ACON_STR  "ACON"
#define INFO_STR  "INFO"
#define INAM_STR  "INAM"
#define IART_STR  "IART"

#define FRAM_STR  "fram"
#define ANIH_STR  "anih"
#define RATE_STR  "rate"
#define SEQ_STR   "seq "
#define ICON_STR  "icon"

#define NULL_STR  "\0\0\0\0"


typedef enum {
  ANICHUNK_OUT = 0,
  ANICHUNK_RIFF,
  ANICHUNK_LIST,
  ANICHUNK_INAM,
  ANICHUNK_IART,
  ANICHUNK_ANIH,
  ANICHUNK_RATE,
  ANICHUNK_SEQ,
  ANICHUNK_ICON,
  ANICHUNK_UNKONW       /* δ֪���� */
} ANICHUNK, *LPANICHUNK;

/* RIFF ���ʹ����� */
#define FORMTYPESEIZE 4


/* ANI�ļ�ͷ�ṹ */
typedef struct _tagAnihead {
  unsigned char riff[4];
  unsigned long imgsize;
  unsigned char acon[4];
} ANIHEAD, PANIHEAD;


/* ANIͨ�ÿ�ͷ���ṹ�����+���С��*/
typedef  struct  _tagAnitag {
  char      ck_id[4]; /* ���� */
  unsigned long ck_size;  /* ���С���ֽڼƣ� */
} ANITAG, *PANITAG;


/* ANI�ļ���Ϣ��������anih����ṹ */
typedef  struct  _tagAniheader {
  unsigned long cbSizeof;   /* ���С����36�ֽ� */
  unsigned long cFrames;    /* �����ͼ������ */
  unsigned long cSteps;     /* ��ʾ��ͼ������ */
  unsigned long cx;       /* ͼ���� */
  unsigned long cy;       /* ͼ��߶� */
  unsigned long cBitCount;    /* ��ɫλ�� */
  unsigned long cPlanes;
  unsigned long jifRate;    /* JIF���� */
  unsigned long fl;       /* AF_ICON/AF_SEQUENCE���ñ�� */
} ANIHEADER, *PANIHEADER;


/* ��60��֮һ��ĵ�λ����תΪǧ��֮һ������� */
#define   FPS_TO_MS(r)    (r*(1000/60))


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* �ж�ָ�����������Ƿ�����Ч��ANI���� */
int CALLAGREEMENT _is_ani(unsigned char* pani)
{
  assert(pani);

  if ((memcmp((const char*)pani, (const char*)RIFF_STR, FORMTYPESEIZE) == 0) && \
      (memcmp((const char*)(pani + FORMTYPESEIZE + sizeof(unsigned long)), (const char*)ACON_STR, FORMTYPESEIZE) == 0)) {
    return 0; /* ��ANI������ */
  }
  else {
    return -1;  /* ��ANI���� */
  }
}

/* ת�����ʾ��Ϊ��ʾ�� */
int CALLAGREEMENT _str_to_value(unsigned char* pstr)
{
  assert(pstr);

  if (memcmp((const char*)pstr, (const char*)ICON_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_ICON;
  }
  else if (memcmp((const char*)pstr, (const char*)LIST_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_LIST;
  }
  else if (memcmp((const char*)pstr, (const char*)ANIH_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_ANIH;
  }
  else if (memcmp((const char*)pstr, (const char*)RATE_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_RATE;
  }
  else if (memcmp((const char*)pstr, (const char*)SEQ_STR , FORMTYPESEIZE) == 0) {
    return ANICHUNK_SEQ;
  }
  else if (memcmp((const char*)pstr, (const char*)RIFF_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_RIFF;
  }
  else if (memcmp((const char*)pstr, (const char*)INAM_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_INAM;
  }
  else if (memcmp((const char*)pstr, (const char*)IART_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_IART;
  }
  else if (memcmp((const char*)pstr, (const char*)NULL_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_OUT;  /* ������Ч���ݷ�Χ����������β���� */
  }
  else {
    return ANICHUNK_UNKONW;  /* δ֪��RIFF���־ */
  }
}


/* ����һ���� */
unsigned char* CALLAGREEMENT _jump_and_jump_and_jump(unsigned char* ptag)
{
  int size;

  assert(ptag);

  size = (int) * (unsigned long*)(unsigned char*)(ptag + FORMTYPESEIZE);

  size += size % 2; /* ��ߴ���������������׷��һ���ֽڣ�RIFF��ʽ�涨�� */

  return (unsigned char*)(ptag + FORMTYPESEIZE + sizeof(unsigned long) + size);
}


/* ��ȡ����ɫ������ֵ */
int CALLAGREEMENT _get_mask(int bitcount,
    unsigned long* pb_mask,
    unsigned long* pg_mask,
    unsigned long* pr_mask,
    unsigned long* pa_mask)
{
  int result = 0;

  switch (bitcount) {
  case  1:
  case  4:
  case  8:
    /* ��ɫ��λͼ������ */
    *pr_mask = 0;
    *pg_mask = 0;
    *pb_mask = 0;
    *pa_mask = 0;
    break;

  case  16:   /* 555 ��ʽ */
    *pr_mask = 0x7c00;
    *pg_mask = 0x3e0;
    *pb_mask = 0x1f;
    *pa_mask = 0;
    break;

  case  24:
    *pr_mask  = 0xff0000;
    *pg_mask  = 0xff00;
    *pb_mask  = 0xff;
    *pa_mask  = 0x0;
    break;

  case  32:   /* 888 ��ʽ */
    *pr_mask  = 0xff0000;
    *pg_mask  = 0xff00;
    *pb_mask  = 0xff;
    *pa_mask  = 0x0;
    break;

  default:    /* �Ƿ���ʽ */
    result = -1;
    *pr_mask = 0;
    *pg_mask = 0;
    *pb_mask = 0;
    *pa_mask = 0;
    break;
  }

  return result;  // 0-�ɹ��� -1���Ƿ���ͼ���ʽ
}

// �ϳ�һ��ɨ���е�XORͼ��ANDͼ�����������XORͼ�� 
int CALLAGREEMENT _ani_compose_img(int bitcount, int width, const unsigned char* pxor, const unsigned char* pand, int cn, uchar* p_bit_data)
{
  unsigned char buf[32], tmp, isc = 0;
  const uchar* p = pxor;
  int i, j, k, l, bytesize;

  assert(bitcount > 0);
  assert(pxor && pand);

  // һ�δ��� 8 ������
  for (i = 0, k = 0; i < width; i += 8, k++) {
    // ȡ�� 8 �����ص�AND���ݣ��� 1 ���ֽڣ�
    tmp = *(pand + k);

    // ��AND����תΪXOR���ظ�ʽ
    switch (bitcount) {
    case  1:
      buf[0] = tmp;
      bytesize = 1; // ʵ��Ҫ�������ֽ���
      break;

    case  4:
      for (l = 0; l < 8; l += 2) {
        buf[l / 2]  = ((tmp >> (7 - l)) & 0x1) ? 0xf0 : 0x0;
        buf[l / 2] |= ((tmp >> (7 - l - 1)) & 0x1) ? 0xf : 0x0;
      }

      bytesize = 4;
      break;

    case  8:
      for (l = 0; l < 8; l++) {
        buf[l] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
      }

      bytesize = ((width - i) < 8) ? (width - i) : 8;
      break;

    case  16:
      for (l = 0; l < 8; l++) {
        buf[l * 2] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 2 + 1] = buf[l * 2] & 0x7f; /* 555��ʽ */
      }

      bytesize = ((width - i) < 8) ? (width - i) * 2 : 16;
      break;

    case  24:
      for (l = 0; l < 8; l++) {
        buf[l * 3] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 3 + 1] = buf[l * 3];
        buf[l * 3 + 2] = buf[l * 3];
      }

      bytesize = ((width - i) < 8) ? (width - i) * 3 : 24;
      break;

    case  32:
      for (l = 0; l < 8; l++) {
        buf[l * 4] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 4 + 1] = buf[l * 4];
        buf[l * 4 + 2] = buf[l * 4];
        buf[l * 4 + 3] = 0;     /* 888��ʽ */
      }

      bytesize = ((width - i) < 8) ? (width - i) * 4 : 32;
      break;

    default:
      return -1;              /* ��֧�ֵ�λ��� */
      break;
    }

    /* �ϳ�ͼ��һ�κϳ� 8 �����أ�*/
    for (j = 0; j < bytesize; j++) {
      buf[j] = p[j] ^ buf[j];
    }
  }

  return 0; /* ���� 0 ��ʾ�ɹ������ط� 0 ֵ��ʾʧ�� */
}

/* ��������ݿ�תΪ���ݰ��� */
CC_INLINE int _ani_conv_image_block(LPBITMAPINFOHEADER lpbmi, INFOSTR* pinfo, int mark, int cn, uchar* p_bit_data)
{
  LPBITMAPINFO  pbi = (LPBITMAPINFO)lpbmi;

  unsigned char* p, *pxor, *pand;
  unsigned long pal_num = 0;
  int       i, linesize, linesize2;

  assert(lpbmi);
  assert((mark == ANI_CUR_PRI_IMAGE) || (mark == ANI_CUR_SUB_IMAGE));

  if (mark == ANI_CUR_PRI_IMAGE) {
    /* ���õ�ɫ������ */
    if (lpbmi->biBitCount <= 8) {
      pinfo->pal_count = 1UL << pinfo->bitcount;

      pal_num = (lpbmi->biClrUsed == 0) ? pinfo->pal_count : lpbmi->biClrUsed;

      if (pal_num > pinfo->pal_count) {
        return ER_BADIMAGE;   /* ͼ������ */
      }

      memmove((void*)(pinfo->palette), (const void*)(pbi->bmiColors), sizeof(RGBQUAD)*pal_num);
    }
    else {
      pinfo->pal_count = 0;
    }

    /* XOR����ɨ���гߴ� */
    linesize  = xpm_calcu_scanline_size(pinfo->width, pinfo->bitcount);
    /* AND����ɨ���гߴ� */
    linesize2 = xpm_calcu_scanline_size(pinfo->width, 1);

    /* ����XOR���������׵�ַ */
    p = (unsigned char*)lpbmi;
    p += lpbmi->biSize;
    p += pal_num * sizeof(RGBQUAD);

    /* ����XOR�������� */
    //memmove((void*)p_bit_data, (const void *)p, (linesize*pinfo->height));

    /* ȡ�� XOR �����׵�ַ */
    pxor = (unsigned char*)p;
    /* ���� AND �����׵�ַ */
    pand = p + linesize * pinfo->height;

    /* �ϳ�ͼ�� */
    for (i = 0; i < (int)pinfo->height; i++) {
      if (_ani_compose_img(pinfo->bitcount, pinfo->width, pxor, pand, cn, p_bit_data) != 0) {
        return ER_BADIMAGE;   /* �����ͼ�� */
      }

      pxor += linesize;
      pand += linesize2;
    }
  }
  else {
    /* ��дͼ����Ϣ */
    int width   = (int)lpbmi->biWidth;
    int height  = (int)lpbmi->biHeight / 2; /* XOR��AND ͼ�ܸ߶� */
    int order   = 1;  /* ���ǵ���ͼ */
    int bitcount  = (int)lpbmi->biBitCount;
    unsigned long b_mask, g_mask, r_mask, a_mask;
    int left = 0, top = 0;
    int colorkey;
    int pal_count = 0;
    unsigned long palette[MAX_PALETTE_COUNT];

    if (_get_mask(bitcount,
        (unsigned long*)(&b_mask),
        (unsigned long*)(&g_mask),
        (unsigned long*)(&r_mask),
        (unsigned long*)(&a_mask)) == -1) {
      return ER_BADIMAGE;   /* ͼ������ */
    }

    colorkey = -1;  /* ��Ϊ�޹ؼ�ɫ */

    /* ���õ�ɫ������ */
    if (lpbmi->biBitCount <= 8) {
      pal_count = 1UL << bitcount;

      pal_num = (lpbmi->biClrUsed == 0) ? pal_count : lpbmi->biClrUsed;

      if (pal_num > (unsigned long)pal_count) {
        return ER_BADIMAGE;   /* ͼ������ */
      }

      memmove((void*)(palette), (const void*)(pbi->bmiColors), sizeof(RGBQUAD)*pal_num);
    }
    else {
      pal_count = 0;
    }

    /* ȡ��XOR����ɨ���гߴ� */
    linesize  = xpm_calcu_scanline_size(width, bitcount);
    /* ����AND����ɨ���гߴ� */
    linesize2 = xpm_calcu_scanline_size(width, 1);

    /* �������������׵�ַ */
    p = (unsigned char*)lpbmi;
    p += lpbmi->biSize;
    p += pal_num * sizeof(RGBQUAD);

    /* ������������ */
    memmove((void*)p_bit_data, (const void*)p, (linesize * height));

    /* ���� XOR �����׵�ַ */
    pxor = (unsigned char*)p_bit_data;
    /* ���� AND �����׵�ַ */
    pand = p + linesize * height;

    /* �ϳ�ͼ��ʹ��XOR������AND���ݣ�*/
    for (i = 0; i < height; i++) {
      if (_ani_compose_img(bitcount, width, pxor, pand, cn, p_bit_data) != 0) {
        return ER_BADIMAGE;   /* �����ͼ�� */
      }

      pxor += linesize;
      pand += linesize2;
    }
  }

  return ER_SUCCESS;    /* ���� 0 �ɹ����� 0 ʧ�� */
}

/* ����ͼ����ʾ�ӳٲ��� */
int CALLAGREEMENT _set_rate(int index, unsigned long rate, INFOSTR* pinfo_str)
{
#if 0

  if (index >= (int)pinfo_str->imgnumbers) {
    return -1;    /* �Ƿ������� */
  }

  if (index == 0) { /* ��֡ͼ�� */
    pinfo_str->time = FPS_TO_MS(rate);
  }
  else {      /* ��ͼ�� */
    for (lpsub = pinfo_str->psubimg; lpsub != 0; lpsub = lpsub->next) {
      if (lpsub->number != index) {
        continue;
      }
      else {
        lpsub->time = FPS_TO_MS(rate);
        break;
      }
    }

    assert(lpsub);  /* �����ܳ����Ҳ�����Ӧ��ͼ������ */
  }

#endif

  return ER_SUCCESS;
}


/* ��ȡͼ��λ���� */
CC_INLINE int CALLAGREEMENT ani_load(ISFILE* pfile, int cn_req, img_t* im)
{
  unsigned char* pani  = 0;
  unsigned long ani_len;
  INFOSTR pinfo_str[1] = {0};
  int b_status = ER_SUCCESS;
  int       mark = 0, imgcount = 0, i, imgnum = 0;
  unsigned long ratecount;

  int     type;
  ANIHEADER     aniheader;
  LPANI_CURDIR    pdirect;
  LPBITMAPINFOHEADER  pbmi;

  /* ���ANI���������� */
  if ((ani_len = isio_length(pfile)) < (FORMTYPESEIZE * 4)) {
    b_status = ER_NONIMAGE;
    return b_status;
  }

  /* ��λ���ļ�ͷ */
  if (isio_seek(pfile, 0, SEEK_SET) == -1) {
    b_status = ER_FILERWERR;
    return b_status;
  }

  /* �����ڴ��Դ��������ANIͼ�����ݣ�+8��Ϊ�˷�ֹ���뺯��������Խ�磩 */
  if ((pani = (unsigned char*)isirw_malloc(ani_len + 8)) == 0) {
    b_status = ER_MEMORYERR;
    return b_status;  /* �ڴ治�� */
  }

  memset((void*)(pani + ani_len), 0, 8);

  /* ��ȡ������ANI���ݵ��ڴ��� */
  if (isio_read((void*)pani, ani_len, 1, pfile) == 0) {
    b_status = ER_FILERWERR;
    return b_status;
  }

  /* �ж��Ƿ���ANIͼ�� */
  if (_is_ani(pani) != 0) {
    b_status = ER_NONIMAGE;
    return b_status;
  }

  /* ��λ�������������RIFF��־�ײ��� */
  pani = (unsigned char*)(pani + FORMTYPESEIZE + sizeof(unsigned long) + FORMTYPESEIZE);

  /* ��ȡ��һ�����ĸ�Ҫ��Ϣ */
  {
    int   mark = 0;
    int     type;
    ANIHEADER     aniheader;
    LPANI_CURDIR    pdirect;
    LPBITMAPINFOHEADER  pbmi;

    assert((pani != 0));

    while ((type = _str_to_value(pani)) != ANICHUNK_OUT) {
      switch (type) {
      case  ANICHUNK_RIFF:
        b_status = ER_BADIMAGE; /* �Ƿ����ݡ�pani��ָ��RIFF���ڲ������Բ�Ӧ�ٳ���RIFF���־ */
        break;

      case  ANICHUNK_LIST:
        pani += FORMTYPESEIZE + sizeof(unsigned long) + FORMTYPESEIZE;
        break;

      case  ANICHUNK_INAM:
      case  ANICHUNK_IART:
      case  ANICHUNK_RATE:
      case  ANICHUNK_SEQ:
      case  ANICHUNK_UNKONW:
        pani = _jump_and_jump_and_jump(pani);
        break;

      case  ANICHUNK_ANIH:
        if ((mark & 0x2) != 0) {
          b_status = ER_BADIMAGE;     /* ֻ�ܴ���һ��ANIH�� */
        }
        else {
          mark |= 0x2;
        }

        memcpy((void*)&aniheader, (const void*)(pani + FORMTYPESEIZE + sizeof(unsigned long)), sizeof(ANIHEADER));

        pani = _jump_and_jump_and_jump(pani);
        break;

      case  ANICHUNK_ICON:
        if ((mark & 0x1) == 0) {
          /* ֻ��ȡ��һ�����ͼ����Ϣ */
          /* ��������ǹ�����ݣ�Ҳ������ͼ�����ݣ������ǲ�ʹ����Щ���ݣ����� */
          /* ֱ�Ӷ�ȡ����BMPINFOHEADER�еĲ��������Բ������� */
          pdirect = (LPANI_CURDIR)(unsigned char*)(pani + FORMTYPESEIZE + sizeof(unsigned long));
          pbmi = (LPBITMAPINFOHEADER)(unsigned char*)(((unsigned char*)pdirect) + pdirect->entries[0].image_offset);

          if (pbmi->biSize != sizeof(BITMAPINFOHEADER)) {
            b_status = ER_BADIMAGE;   /* �Ƿ����� */
            break;
          }

          /* ��ȡ��һ��ͼ�����Ϣ�������ݰ���֡ */
          pinfo_str->width  = (unsigned long)pbmi->biWidth;
          pinfo_str->height = (unsigned long)pbmi->biHeight / 2; /* XOR��AND ͼ�ܸ߶� */
          pinfo_str->order  = 1;  /* ���ǵ���ͼ */
          pinfo_str->bitcount = (unsigned long)pbmi->biBitCount;

          if ((pinfo_str->width == 0) || (pinfo_str->height == 0)) {
            b_status = ER_BADIMAGE;
            break;
          }

          if (_get_mask(pinfo_str->bitcount,
              (unsigned long*)(&pinfo_str->b_mask),
              (unsigned long*)(&pinfo_str->g_mask),
              (unsigned long*)(&pinfo_str->r_mask),
              (unsigned long*)(&pinfo_str->a_mask)) == -1) {
            b_status = ER_BADIMAGE;
            break;
          }

          mark |= 0x1;  /* ������ȡ����ͼ����Ϣ */
        }

        pani = _jump_and_jump_and_jump(pani);
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;   /* ϵͳ�쳣 */
        break;
      }
    }

    b_status = (mark == 3) ? ER_SUCCESS : ER_BADIMAGE;  /* �ɹ� */
  }

  if (b_status != ER_SUCCESS) {
    return b_status;
  }

  /* ��ȡ������� */
  assert((pani != 0) && (pinfo_str != 0));

  while ((type = _str_to_value(pani)) != ANICHUNK_OUT) {
    switch (type) {
    case  ANICHUNK_RIFF:
      b_status = ER_BADIMAGE; /* �Ƿ����ݡ�pani��ָ��RIFF���ڲ������Բ�Ӧ�ٳ���RIFF���־ */
      return b_status;

    case  ANICHUNK_LIST:
      pani += FORMTYPESEIZE + sizeof(unsigned long) + FORMTYPESEIZE;
      break;

    case  ANICHUNK_RATE:
      ratecount = *(unsigned long*)(unsigned char*)(pani + FORMTYPESEIZE);
      /* ���䲥�����ʱ� */

      //rate = (unsigned long *)isirw_malloc(ratecount);
      /* ���Ʊ����� */
      //memcpy((void*)rate, (const void *)(unsigned char *)(pani+FORMTYPESEIZE+sizeof(unsigned long)), ratecount);

      /* ת��ΪԪ�ص�λ */
      ratecount /= sizeof(unsigned long);

      pani = _jump_and_jump_and_jump(pani);
      break;

    case  ANICHUNK_SEQ:
#if 0
      seqcount = *(unsigned long*)(unsigned char*)(pani + FORMTYPESEIZE);
      /* ���䲥��˳��� */
      pinfo_str->play_order = (unsigned long*)isirw_malloc(seqcount);

      if (pinfo_str->play_order == 0) {
        b_status = ER_MEMORYERR;
        return b_status;      /* �ڴ治�� */
      }

      /* ���Ʊ����� */
      memcpy((void*)pinfo_str->play_order, (const void*)(unsigned char*)(pani + FORMTYPESEIZE + sizeof(unsigned long)), seqcount);

      /* ת��ΪԪ�ص�λ */
      seqcount /= sizeof(unsigned long);
      /* ���ű���Ԫ�ظ��� */
      pinfo_str->play_number = seqcount;
#endif
      pani = _jump_and_jump_and_jump(pani);
      break;

    case  ANICHUNK_INAM:    /* ��ʱ����ͼ��������Ϣ */
    case  ANICHUNK_IART:
    case  ANICHUNK_UNKONW:
      pani = _jump_and_jump_and_jump(pani);
      break;

    case  ANICHUNK_ANIH:
      if ((mark & 0x2) != 0) {
        b_status = ER_BADIMAGE;     /* ֻ�ܴ���һ��ANIH�� */
        return b_status;
      }
      else {
        mark |= 0x2;
      }

      memcpy((void*)&aniheader, (const void*)(pani + FORMTYPESEIZE + sizeof(unsigned long)), sizeof(ANIHEADER));

      if ((aniheader.fl & 0x1) == 0) {
        b_status = ER_BADIMAGE;     /* ��������������������ݣ�RAW����ʽ�洢ͼ���ANI */
        return b_status;
      }

      pani = _jump_and_jump_and_jump(pani);
      break;

    case  ANICHUNK_ICON:
      if ((mark & 0x2) == 0) {
        b_status = ER_BADIMAGE;     /* anih �������ICON���ǰ�棬������Ϊ�Ƿ����� */
        return b_status;
      }

      pdirect = (LPANI_CURDIR)(unsigned char*)(pani + FORMTYPESEIZE + sizeof(unsigned long));
      pbmi = (LPBITMAPINFOHEADER)(unsigned char*)(((unsigned char*)pdirect) + pdirect->entries[0].image_offset);

      if (pbmi->biSize != sizeof(BITMAPINFOHEADER)) {
        b_status = ER_BADIMAGE;   /* �Ƿ����� */
        return b_status;
      }

      imgcount = pdirect->count;

      if (imgcount < 1) { /* ����Ӧ��һ��ͼ�� */
        b_status = ER_BADIMAGE;
        return b_status;
      }

      imsetsize(im, pinfo_str->height, pinfo_str->width, cn_req, imgcount);

      if ((mark & 0x1) == 0) { /* ��ȡ��֡���ͼ�� */
        if ((b_status = _ani_conv_image_block(pbmi, pinfo_str, ANI_CUR_PRI_IMAGE, cn_req, im->tt.data))!=0) {
          return b_status;
        }

        imgnum = 0;   /* ��ͼ��������� */
        mark |= 0x1;
      }

      /* ������ͼ�� */
      for (i = 0; i < imgcount; ++i) {
        /* ����ͼ�����ݵ�ַ */
        pbmi = (LPBITMAPINFOHEADER)(unsigned char*)(((unsigned char*)pdirect) + pdirect->entries[pdirect->count - imgcount].image_offset);

        /* ת����ͼ�����ݰ��� */
        if ((b_status = _ani_conv_image_block(pbmi, pinfo_str, ANI_CUR_SUB_IMAGE, cn_req, im->tt.data + i * im->h * im->s))!=0) {
          return b_status;
        }
      }

      pani = _jump_and_jump_and_jump(pani);
      break;

    default:
      assert(0);
      b_status = ER_SYSERR;
      return b_status;    /* ϵͳ�쳣 */
      break;
    }
  }

  if (mark == 3) {
    pinfo_str->imgnumbers = imgnum + 1; /* ͼ���ܸ�����������֡ͼ��*/

    /* ����ÿ֡ͼ�����ʾ�ӳ�ʱ�� */
    pinfo_str->colorkey = -1;
    b_status = ER_SUCCESS;  /* �ɹ� */
  }
  else {
    b_status = ER_BADIMAGE; /* ͼ���������� */
    return b_status;
  }


  return b_status;
}


#define ani_load_filename(fname, cn_req, im, pbpp, palette) ani_load_file_close(fopen(fname, "rb"), cn_req, im, pbpp, palette)
CC_INLINE int ani_load_file_close(FILE* pf, int cn_req, img_t* im, int* pbpp, uchar* palette)
{
  int ret = 0;

  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = ani_load(s, cn_req, im);
    fclose(pf);
  }

  return ret;
}

