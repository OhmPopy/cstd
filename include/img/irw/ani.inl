/********************************************************************
 ani.c
 ���ļ���;�� ANIͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ�1��4��8��16��24��32λANIͼ��
 ���湦�ܣ���֧��
 ���ļ���д�ˣ�
 YZ
 ���ļ��汾�� 30401
 ����޸��ڣ� 2003-4-1
 ----------------------------------------------------------------
 ������ʷ��
 2003-4 ֧��ͼ���д�����ṩ�����ػ���������
 2002-8 ����ӿ�������2.2�������ڴ�������
 2002-3 ������һ���汾���°棬֧��ISeeIOϵͳ��
********************************************************************/
#ifndef WIN32
#if defined(_WIN32)||defined(_WINDOWS)
#define WIN32
#endif
#endif /* WIN32 */
/* ������Ͷ��� */
#define ANI_CURTYPE 2
/* ������ͼ�����Ͷ��� */
enum ANI_CUR_DATA_TYPE {
  ANI_CUR_PRI_IMAGE,
  ANI_CUR_SUB_IMAGE
};
#pragma pack(1)
/* ����ͼ�����Ϣͷ�ṹ */
typedef struct {
  uchar width; /* ͼ���� */
  uchar height; /* ͼ��߶� */
  uchar color_count; /* ��ɫ����(���λ��ȴ���8ʱΪ0) */
  uchar reserved;
  ushort x_hotspot; /* �ȵ�X���� */
  ushort y_hotspot; /* �ȵ�Y���� */
  ulong bytes_in_res; /* ͼ����������Դ����ռ���ֽ��� */
  ulong image_offset; /* ͼ�����ݵ�ƫ�� */
} ANI_CURDIRENTRY, *LPANI_CURDIRENTRY;
/* �����Ϣͷ�ṹ */
typedef struct {
  ushort reserved; /* ���� */
  ushort type; /* ��Դ����(CURTYPEΪ���) */
  ushort count; /* ͼ����� */
  ANI_CURDIRENTRY entries[1]; /* ÿһ��ͼ������ */
} ANI_CURDIR, *LPANI_CURDIR;
#pragma pack()
/* RIFF ������ID���� */
#define RIFF_STR "RIFF"
#define LIST_STR "LIST"
#define ACON_STR "ACON"
#define INFO_STR "INFO"
#define INAM_STR "INAM"
#define IART_STR "IART"
#define FRAM_STR "fram"
#define ANIH_STR "anih"
#define RATE_STR "rate"
#define SEQ_STR "seq "
#define ICON_STR "icon"
#define NULL_STR "\0\0\0\0"
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
  ANICHUNK_UNKONW /* δ֪���� */
} ANICHUNK, *LPANICHUNK;
/* RIFF ���ʹ����� */
#define FORMTYPESEIZE 4
/* ANI�ļ�ͷ�ṹ */
typedef struct _tagAnihead {
  uchar riff[4];
  ulong imgsize;
  uchar acon[4];
} ANIHEAD, PANIHEAD;
/* ANIͨ�ÿ�ͷ���ṹ�����+���С��*/
typedef struct _tagAnitag {
  char ck_id[4]; /* ���� */
  ulong ck_size; /* ���С���ֽڼƣ� */
} ANITAG, *PANITAG;
/* ANI�ļ���Ϣ��������anih����ṹ */
typedef struct _tagAniheader {
  ulong cbSizeof; /* ���С����36�ֽ� */
  ulong cFrames; /* �����ͼ������ */
  ulong cSteps; /* ��ʾ��ͼ������ */
  ulong cx; /* ͼ���� */
  ulong cy; /* ͼ��߶� */
  ulong cBitCount; /* ��ɫλ�� */
  ulong cPlanes;
  ulong jifRate; /* JIF���� */
  ulong fl; /* AF_ICON/AF_SEQUENCE���ñ�� */
} ANIHEADER, *PANIHEADER;
/* ��60��֮һ��ĵ�λ����תΪǧ��֮һ������� */
#define FPS_TO_MS(r) (r*(1000/60))
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */
/* �ж�ָ�����������Ƿ�����Ч��ANI���� */
int _is_ani(uchar* pani)
{
  assert(pani);
  if ((memcmp((const char*)pani, (const char*)RIFF_STR, FORMTYPESEIZE) == 0) && \
      (memcmp((const char*)(pani + FORMTYPESEIZE + sizeof(ulong)), (const char*)ACON_STR, FORMTYPESEIZE) == 0)) {
    return 0; /* ��ANI������ */
  }
  else {
    return -1; /* ��ANI���� */
  }
}
/* ת�����ʾ��Ϊ��ʾ�� */
ANICHUNK _str_to_value(uchar* pstr)
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
    return ANICHUNK_OUT; /* ������Ч���ݷ�Χ����������β���� */
  }
  else {
    return ANICHUNK_UNKONW; /* δ֪��RIFF���־ */
  }
}
/* ����һ���� */
uchar* _jump_and_jump_and_jump(uchar* ptag)
{
  int size;
  assert(ptag);
  size = (int) * (ulong*)(uchar*)(ptag + FORMTYPESEIZE);
  size += size % 2; /* ��ߴ���������������׷��һ���ֽڣ�RIFF��ʽ�涨�� */
  return (uchar*)(ptag + FORMTYPESEIZE + sizeof(ulong) + size);
}
/* ��ȡ�׷����ĸ�Ҫ��Ϣ */
EXERESULT _get_info(uchar* pani, INFOSTR* pinfo_str)
{
  enum EXERESULT b_status = ER_SUCCESS;
  int mark = 0;
  ANICHUNK type;
  ANIHEADER aniheader;
  LPANI_CURDIR pdirect;
  wBITMAPINFOHEADER* pbmi;
  assert((pani != 0) && (pinfo_str != 0));
  while ((type = _str_to_value(pani)) != ANICHUNK_OUT) {
    switch (type) {
    case ANICHUNK_RIFF:
      return ER_BADIMAGE; /* �Ƿ����ݡ�pani��ָ��RIFF���ڲ������Բ�Ӧ�ٳ���RIFF���־ */
    case ANICHUNK_LIST:
      pani += FORMTYPESEIZE + sizeof(ulong) + FORMTYPESEIZE;
      break;
    case ANICHUNK_INAM:
    case ANICHUNK_IART:
    case ANICHUNK_RATE:
    case ANICHUNK_SEQ:
    case ANICHUNK_UNKONW:
      pani = _jump_and_jump_and_jump(pani);
      break;
    case ANICHUNK_ANIH:
      if ((mark & 0x2) != 0) {
        return ER_BADIMAGE; /* ֻ�ܴ���һ��ANIH�� */
      }
      else {
        mark |= 0x2;
      }
      memcpy((void*)&aniheader, (const void*)(pani + FORMTYPESEIZE + sizeof(ulong)), sizeof(ANIHEADER));
      pani = _jump_and_jump_and_jump(pani);
      break;
    case ANICHUNK_ICON:
      if ((mark & 0x1) == 0) { /* ֻ��ȡ��һ�����ͼ����Ϣ */
        /* ��������ǹ�����ݣ�Ҳ������ͼ�����ݣ������ǲ�ʹ����Щ���ݣ����� */
        /* ֱ�Ӷ�ȡ����BMPINFOHEADER�еĲ��������Բ������� */
        pdirect = (LPANI_CURDIR)(uchar*)(pani + FORMTYPESEIZE + sizeof(ulong));
        pbmi = (wBITMAPINFOHEADER*)(uchar*)(((uchar*)pdirect) + pdirect->entries[0].image_offset);
        if (pbmi->biSize != sizeof(wBITMAPINFOHEADER)) {
          return ER_BADIMAGE; /* �Ƿ����� */
        }
        /* ��ȡ��һ��ͼ�����Ϣ�������ݰ���֡ */
        pinfo_str->width = (ulong)pbmi->biWidth;
        pinfo_str->height = (ulong)pbmi->biHeight / 2; /* XOR��AND ͼ�ܸ߶� */
        pinfo_str->order = 1; /* ���ǵ���ͼ */
        pinfo_str->bitcount = (ulong)pbmi->biBitCount;
        if ((pinfo_str->width == 0) || (pinfo_str->height == 0)) {
          return ER_BADIMAGE;
        }
        if (_get_mask(pinfo_str->bitcount,
            (&pinfo_str->b_mask),
            (&pinfo_str->g_mask),
            (&pinfo_str->r_mask),
            (&pinfo_str->a_mask)) == -1) {
          return ER_BADIMAGE;
        }
        mark |= 0x1; /* ������ȡ����ͼ����Ϣ */
      }
      pani = _jump_and_jump_and_jump(pani);
      break;
    default:
      assert(0);
      return ER_SYSERR; /* ϵͳ�쳣 */
      break;
    }
  }
  if (mark == 3) {
    return ER_SUCCESS; /* �ɹ� */
  }
  else {
    return ER_BADIMAGE; /* ͼ���������� */
  }
}
/* �ϳ�һ��ɨ���е�XORͼ��ANDͼ�����������XORͼ�� */
int _ani_compose_img(int bitcount, int width, uchar* pxor, uchar* pand)
{
  uchar buf[32], tmp, *p = pxor, isc = 0;
  int i, j, k, l, bytesize;
  assert(bitcount > 0);
  assert(pxor && pand);
  /* һ�δ��� 8 ������ */
  for (i = 0, k = 0; i < width; i += 8, k++) {
    /* ȡ�� 8 �����ص�AND���ݣ��� 1 ���ֽڣ� */
    tmp = *(pand + k);
    /* ��AND����תΪXOR���ظ�ʽ */
    switch (bitcount) {
    case 1:
      buf[0] = tmp;
      bytesize = 1; /* ʵ��Ҫ�������ֽ��� */
      break;
    case 4:
      for (l = 0; l < 8; l += 2) {
        buf[l / 2] = ((tmp >> (7 - l)) & 0x1) ? 0xf0 : 0x0;
        buf[l / 2] |= ((tmp >> (7 - l - 1)) & 0x1) ? 0xf : 0x0;
      }
      bytesize = 4;
      break;
    case 8:
      for (l = 0; l < 8; l++) {
        buf[l] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
      }
      bytesize = ((width - i) < 8) ? (width - i) : 8;
      break;
    case 16:
      for (l = 0; l < 8; l++) {
        buf[l * 2] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 2 + 1] = buf[l * 2] & 0x7f; /* 555��ʽ */
      }
      bytesize = ((width - i) < 8) ? (width - i) * 2 : 16;
      break;
    case 24:
      for (l = 0; l < 8; l++) {
        buf[l * 3] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 3 + 1] = buf[l * 3];
        buf[l * 3 + 2] = buf[l * 3];
      }
      bytesize = ((width - i) < 8) ? (width - i) * 3 : 24;
      break;
    case 32:
      for (l = 0; l < 8; l++) {
        buf[l * 4] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 4 + 1] = buf[l * 4];
        buf[l * 4 + 2] = buf[l * 4];
        buf[l * 4 + 3] = 0; /* 888��ʽ */
      }
      bytesize = ((width - i) < 8) ? (width - i) * 4 : 32;
      break;
    default:
      return -1; /* ��֧�ֵ�λ��� */
      break;
    }
    /* �ϳ�ͼ��һ�κϳ� 8 �����أ�*/
    for (j = 0; j < bytesize; j++) {
      *p++ ^= buf[j];
    }
  }
  return 0; /* ���� 0 ��ʾ�ɹ������ط� 0 ֵ��ʾʧ�� */
}
/* ��������ݿ�תΪ���ݰ��� */
int _ani_conv_image_block(wBITMAPINFOHEADER* lpbmi, void* lpdest, enum ANI_CUR_DATA_TYPE mark)
{
  INFOSTR* pinfo = (INFOSTR*)0;
  SUBIMGBLOCK* psubinfo = (SUBIMGBLOCK*)0;
  wBITMAPINFO* pbi = (wBITMAPINFO*)lpbmi;
  uchar* p, *pxor, *pand;
  ulong pal_num = 0;
  int i, linesize, linesize2;
  assert((lpbmi) && (lpdest));
  assert((mark == ANI_CUR_PRI_IMAGE) || (mark == ANI_CUR_SUB_IMAGE));
  if (mark == ANI_CUR_PRI_IMAGE) {
    pinfo = (INFOSTR*)lpdest;
    /* ���õ�ɫ������ */
    if (lpbmi->biBitCount <= 8) {
      pinfo->pal_count = 1UL << pinfo->bitcount;
      pal_num = (lpbmi->biClrUsed == 0) ? pinfo->pal_count : lpbmi->biClrUsed;
      if (pal_num > pinfo->pal_count) {
        return -2; /* ͼ������ */
      }
      memmove((void*)(pinfo->palette), (const void*)(pbi->bmiColors), sizeof(wRGBQUAD)*pal_num);
    }
    else {
      pinfo->pal_count = 0;
    }
    /* XOR����ɨ���гߴ� */
    linesize = _calcu_scanline_size(pinfo->width, pinfo->bitcount);
    /* AND����ɨ���гߴ� */
    linesize2 = _calcu_scanline_size(pinfo->width, 1);
    assert(pinfo->p_bit_data == NULL);
    /* ����Ŀ��ͼ���ڴ�飨+4 �� β������4�ֽڻ������� */
    pinfo->p_bit_data = (uchar*)isirw_malloc(linesize * pinfo->height + 4);
    if (!pinfo->p_bit_data) {
      return -3; /* �ڴ治�� */
    }
    assert(pinfo->pp_line_addr == NULL);
    /* �������׵�ַ���� */
    pinfo->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * pinfo->height);
    if (!pinfo->pp_line_addr) {
      isirw_free(pinfo->p_bit_data);
      pinfo->p_bit_data = 0;
      return -3; /* �ڴ治�� */
    }
    /* ��ʼ�����׵�ַ���飨���� */
    for (i = 0; i < (int)(pinfo->height); i++) {
      pinfo->pp_line_addr[i] = (pinfo->p_bit_data + ((pinfo->height - i - 1) * linesize));
    }
    /* ����XOR���������׵�ַ */
    p = (uchar*)lpbmi;
    p += lpbmi->biSize;
    p += pal_num * sizeof(wRGBQUAD);
    /* ����XOR�������� */
    memmove((void*)pinfo->p_bit_data, (const void*)p, (linesize * pinfo->height));
    /* ȡ�� XOR �����׵�ַ */
    pxor = (uchar*)pinfo->p_bit_data;
    /* ���� AND �����׵�ַ */
    pand = p + linesize * pinfo->height;
    /* �ϳ�ͼ�� */
    for (i = 0; i < (int)pinfo->height; i++) {
      if (_ani_compose_img(pinfo->bitcount, pinfo->width, pxor, pand) != 0) {
        isirw_free(pinfo->p_bit_data);
        pinfo->p_bit_data = 0;
        isirw_free(pinfo->pp_line_addr);
        pinfo->pp_line_addr = 0;
        return -2; /* �����ͼ�� */
      }
      pxor += linesize;
      pand += linesize2;
    }
  }
  else {
    psubinfo = (SUBIMGBLOCK*)lpdest;
    /* ��дͼ����Ϣ */
    psubinfo->width = (int)lpbmi->biWidth;
    psubinfo->height = (int)lpbmi->biHeight / 2; /* XOR��AND ͼ�ܸ߶� */
    psubinfo->order = 1; /* ���ǵ���ͼ */
    psubinfo->bitcount = (int)lpbmi->biBitCount;
    if (_get_mask(psubinfo->bitcount,
        (ulong*)(&psubinfo->b_mask),
        (ulong*)(&psubinfo->g_mask),
        (ulong*)(&psubinfo->r_mask),
        (ulong*)(&psubinfo->a_mask)) == -1) {
      return -2; /* ͼ������ */
    }
    psubinfo->left = psubinfo->top = 0;
    psubinfo->dowith = psubinfo->userinput = psubinfo->time = 0;
    psubinfo->colorkey = -1; /* ��Ϊ�޹ؼ�ɫ */
    /* ���õ�ɫ������ */
    if (lpbmi->biBitCount <= 8) {
      psubinfo->pal_count = 1UL << psubinfo->bitcount;
      pal_num = (lpbmi->biClrUsed == 0) ? psubinfo->pal_count : lpbmi->biClrUsed;
      if (pal_num > (ulong)psubinfo->pal_count) {
        return -2; /* ͼ������ */
      }
      memmove((void*)(psubinfo->palette), (const void*)(pbi->bmiColors), sizeof(wRGBQUAD)*pal_num);
    }
    else {
      psubinfo->pal_count = 0;
    }
    /* ȡ��XOR����ɨ���гߴ� */
    linesize = _calcu_scanline_size(psubinfo->width, psubinfo->bitcount);
    /* ����AND����ɨ���гߴ� */
    linesize2 = _calcu_scanline_size(psubinfo->width, 1);
    assert(psubinfo->p_bit_data == (uchar*)0);
    /* ����Ŀ��ͼ���ڴ�飨+4 �� β������4�ֽڻ������� */
    psubinfo->p_bit_data = (uchar*)isirw_malloc(linesize * psubinfo->height + 4);
    if (!psubinfo->p_bit_data) {
      return -3; /* �ڴ治�� */
    }
    assert(psubinfo->pp_line_addr == NULL);
    /* �������׵�ַ���� */
    psubinfo->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * psubinfo->height);
    if (!psubinfo->pp_line_addr) {
      isirw_free(psubinfo->p_bit_data);
      psubinfo->p_bit_data = 0;
      return -3; /* �ڴ治�� */
    }
    /* ��ʼ�����׵�ַ���飨���� */
    for (i = 0; i < (int)(psubinfo->height); i++) {
      psubinfo->pp_line_addr[i] = (psubinfo->p_bit_data + ((psubinfo->height - i - 1) * linesize));
    }
    /* �������������׵�ַ */
    p = (uchar*)lpbmi;
    p += lpbmi->biSize;
    p += pal_num * sizeof(wRGBQUAD);
    /* ������������ */
    memmove((void*)psubinfo->p_bit_data, (const void*)p, (linesize * psubinfo->height));
    /* ���� XOR �����׵�ַ */
    pxor = (uchar*)psubinfo->p_bit_data;
    /* ���� AND �����׵�ַ */
    pand = p + linesize * psubinfo->height;
    /* �ϳ�ͼ��ʹ��XOR������AND���ݣ�*/
    for (i = 0; i < psubinfo->height; i++) {
      if (_ani_compose_img(psubinfo->bitcount, psubinfo->width, pxor, pand) != 0) {
        isirw_free(psubinfo->p_bit_data);
        psubinfo->p_bit_data = 0;
        isirw_free(psubinfo->pp_line_addr);
        psubinfo->pp_line_addr = 0;
        return -2; /* �����ͼ�� */
      }
      pxor += linesize;
      pand += linesize2;
    }
  }
  return 0; /* ���� 0 �ɹ����� 0 ʧ�� */
}
/* ����ͼ����ʾ�ӳٲ�����ֻ��_get_image�������ã� */
int _set_rate(int index, ulong rate, INFOSTR* pinfo_str)
{
  if (index >= (int)pinfo_str->imgnumbers) {
    return -1; /* �Ƿ������� */
  }
  if (index == 0) { /* ��֡ͼ�� */
    pinfo_str->time = FPS_TO_MS(rate);
  }
  else { /* ��ͼ�� */
    int i;
    for (i = 0; i<pinfo_str->imgnumbers; ++i) {
      SUBIMGBLOCK* lpsub = lpsub = pinfo_str->psubimg + i;;
      if (lpsub->number != index) {
        continue;
      }
      else {
        lpsub->time = FPS_TO_MS(rate);
        break;
      }
    }
    assert(i<pinfo_str->imgnumbers); /* �����ܳ����Ҳ�����Ӧ��ͼ������ */
  }
  return 0;
}
/* ����һ����ͼ��ڵ㣬����ʼ���ڵ������� */
SUBIMGBLOCK* _ani_alloc_SUBIMGBLOCK(INFOSTR* pinfo_str)
{
  SUBIMGBLOCK* p_subimg = _alloc_SUBIMGBLOCK(pinfo_str);
  if (p_subimg) {
    p_subimg->number = 1;
    p_subimg->colorkey = -1;
  }
  return p_subimg;
}
/* ��ȡ������� */
int _get_image(uchar* pani, INFOSTR* pinfo_str)
{
  int mark = 0, imgcount = 0, i, imgnum = 0;
  ulong seqcount, ratecount;
  ulong* rate = 0; /* ���ʱ� */
  ANICHUNK type;
  ANIHEADER aniheader;
  LPANI_CURDIR pdirect;
  wBITMAPINFOHEADER* pbmi;
  SUBIMGBLOCK* lpsub = 0;
  assert((pani != 0) && (pinfo_str != 0));
  while ((type = _str_to_value(pani)) != ANICHUNK_OUT) {
    switch (type) {
    case ANICHUNK_RIFF:
      if (rate) {
        isirw_free(rate);
      }
      return -1; /* �Ƿ����ݡ�pani��ָ��RIFF���ڲ������Բ�Ӧ�ٳ���RIFF���־ */
    case ANICHUNK_LIST:
      pani += FORMTYPESEIZE + sizeof(ulong) + FORMTYPESEIZE;
      break;
    case ANICHUNK_RATE:
      ratecount = *(ulong*)(uchar*)(pani + FORMTYPESEIZE);
      /* ��?䲥�����ʱ?*/
      rate = (ulong*)isirw_malloc(ratecount);
      if (rate == 0) {
        return -3;
      }
      /* ���Ʊ����� */
      memcpy((void*)rate, (const void*)(uchar*)(pani + FORMTYPESEIZE + sizeof(ulong)), ratecount);
      /* ת��ΪԪ�ص�λ */
      ratecount /= sizeof(ulong);
      pani = _jump_and_jump_and_jump(pani);
      break;
    case ANICHUNK_SEQ:
      seqcount = *(ulong*)(uchar*)(pani + FORMTYPESEIZE);
      /* ���䲥��˳��� */
      pinfo_str->play_order = (ulong*)isirw_malloc(seqcount);
      if (pinfo_str->play_order == 0) {
        if (rate) {
          isirw_free(rate);
        }
        return -3; /* �ڴ治�� */
      }
      /* ���Ʊ����� */
      memcpy((void*)pinfo_str->play_order, (const void*)(uchar*)(pani + FORMTYPESEIZE + sizeof(ulong)), seqcount);
      /* ת��ΪԪ�ص�λ */
      seqcount /= sizeof(ulong);
      /* ���ű���Ԫ�ظ��� */
      pinfo_str->play_number = seqcount;
      pani = _jump_and_jump_and_jump(pani);
      break;
    case ANICHUNK_INAM: /* ��ʱ����ͼ��������Ϣ */
    case ANICHUNK_IART:
    case ANICHUNK_UNKONW:
      pani = _jump_and_jump_and_jump(pani);
      break;
    case ANICHUNK_ANIH:
      if ((mark & 0x2) != 0) {
        if (rate) {
          isirw_free(rate);
        }
        return -1; /* ֻ�ܴ���һ��ANIH�� */
      }
      else {
        mark |= 0x2;
      }
      memcpy((void*)&aniheader, (const void*)(pani + FORMTYPESEIZE + sizeof(ulong)), sizeof(ANIHEADER));
      if ((aniheader.fl & 0x1) == 0) {
        if (rate) {
          isirw_free(rate);
        }
        return -1; /* ��������������������ݣ�RAW����ʽ�洢ͼ���ANI */
      }
      pani = _jump_and_jump_and_jump(pani);
      break;
    case ANICHUNK_ICON:
      if ((mark & 0x2) == 0) {
        if (rate) {
          isirw_free(rate);
        }
        return -1; /* anih �������ICON���ǰ�棬������Ϊ�Ƿ����� */
      }
      pdirect = (LPANI_CURDIR)(uchar*)(pani + FORMTYPESEIZE + sizeof(ulong));
      pbmi = (wBITMAPINFOHEADER*)(uchar*)(((uchar*)pdirect) + pdirect->entries[0].image_offset);
      if (pbmi->biSize != sizeof(wBITMAPINFOHEADER)) {
        if (rate) {
          isirw_free(rate);
        }
        return -1; /* �Ƿ����� */
      }
      imgcount = pdirect->count;
      if (imgcount < 1) { /* ����Ӧ��һ?�ͼ�?*/
        if (rate) {
          isirw_free(rate);
        }
        return -1;
      }
      if ((mark & 0x1) == 0) { /* ��ȡ��֡���ͼ�� */
        switch (_ani_conv_image_block(pbmi, (void*)pinfo_str, ANI_CUR_PRI_IMAGE)) {
        case -2: /* ����ļ����� */
          if (rate) {
            isirw_free(rate);
          }
          return -1;
        case -3: /* �ڴ治�� */
          if (rate) {
            isirw_free(rate);
          }
          return -3;
        case 0: /* �ɹ� */
          break;
        default:
          if (rate) {
            isirw_free(rate);
          }
          return -2; /* ϵͳ�쳣 */
        }
        imgcount--; /* ����������ͼ��������� */
        imgnum = 0; /* ��ͼ��������� */
        mark |= 0x1;
      }
      /* ������ͼ�� */
      while (imgcount) {
        if ((lpsub = _ani_alloc_SUBIMGBLOCK(pinfo_str)) == 0) {
          if (rate) {
            isirw_free(rate);
          }
          return -3;
        }
        lpsub->number = ++imgnum; /* ��ͼ������� 1 ��ʼ */
        /* ����ͼ�����ݵ�ַ */
        pbmi = (wBITMAPINFOHEADER*)(uchar*)(((uchar*)pdirect) + pdirect->entries[pdirect->count - imgcount].image_offset);
        /* ת����ͼ�����ݰ��� */
        switch (_ani_conv_image_block(pbmi, (void*)lpsub, ANI_CUR_SUB_IMAGE)) {
        case -2: /* ����ļ����� */
          if (rate) {
            isirw_free(rate);
          }
          return -1;
        case -3: /* �ڴ治�� */
          if (rate) {
            isirw_free(rate);
          }
          return -3;
        case 0: /* �ɹ� */
          break;
        default:
          if (rate) {
            isirw_free(rate);
          }
          return -2;
        }
        imgcount--;
      }
      pani = _jump_and_jump_and_jump(pani);
      break;
    default:
      assert(0);
      if (rate) {
        isirw_free(rate);
      }
      return -2; /* ϵͳ�쳣 */
      break;
    }
  }
  if (mark == 3) {
    pinfo_str->imgnumbers = imgnum + 1; /* ͼ���ܸ�����������֡ͼ��*/
    /* ����ÿ֡ͼ�����ʾ�ӳ�ʱ�� */
    if (rate) {
      if (pinfo_str->play_order) {
        for (i = 0; i < (int)pinfo_str->play_number; i++) {
          if (_set_rate((int)pinfo_str->play_order[i], rate[i], pinfo_str) != 0) {
            isirw_free(rate);
            return -1;
          }
        }
      }
      else {
        for (i = 0; i < (int)pinfo_str->imgnumbers; i++) {
          if (_set_rate((int)i, rate[i], pinfo_str) != 0) {
            isirw_free(rate);
            return -1;
          }
        }
      }
    }
    else {
      for (i = 0; i < (int)pinfo_str->imgnumbers; i++) {
        if (_set_rate((int)i, aniheader.jifRate, pinfo_str) != 0) {
          return -1;
        }
      }
    }
    pinfo_str->colorkey = -1;
    if (rate) {
      isirw_free(rate);
    }
    /* ת����ͼ�� */
    return 0; /* �ɹ� */
  }
  else {
    if (rate) {
      isirw_free(rate);
    }
    return -1; /* ͼ���������� */
  }
}
/* ��ȡͼ����Ϣ */
EXERESULT ani_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  uchar* pani = 0, *p;
  ulong ani_len;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */
  do {
    /* ���ANI���������� */
    if ((ani_len = isio_length(pfile)) < (FORMTYPESEIZE * 4)) {
      b_status = ER_NONIMAGE;
      break;
    }
    /* ��λ���ļ�ͷ */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* �����ڴ��Դ��������ANIͼ�����ݣ�+8��Ϊ�˷�ֹ���뺯��������Խ�磩 */
    if ((pani = (uchar*)isirw_malloc(ani_len + 8)) == 0) {
      b_status = ER_MEMORYERR;
      break; /* �ڴ治�� */
    }
    memset((void*)(pani + ani_len), 0, 8);
    /* ��ȡ������ANI���ݵ��ڴ��� */
    if (isio_read((void*)pani, ani_len, 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* �ж��Ƿ���ANIͼ�� */
    if (_is_ani(pani) != 0) {
      b_status = ER_NONIMAGE;
      break;
    }
    /* ��λ�������������RIFF��־�ײ��� */
    p = (uchar*)(pani + FORMTYPESEIZE + sizeof(ulong) + FORMTYPESEIZE);
    /* ��ȡ��һ�����ĸ�Ҫ��Ϣ */
    IF_LEAVE(_get_info(p, pinfo_str));
    pinfo_str->imgtype = IMT_RESDYN; /* ͼ���ļ����� */
    pinfo_str->imgformat = IMF_ANI; /* ͼ���ļ���ʽ����׺���� */
    pinfo_str->compression = ICS_RGB; /* ͼ���ѹ����ʽ */
    /* �趨���ݰ�״̬ */
    pinfo_str->data_state = 1;
  }
  while (0);
  if (b_status != ER_SUCCESS) {
    if (pani) {
      isirw_free(pani);
    }
    if (pfile) {
    }
    pinfo_str->data_state = 0;
  }
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT ani_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  uchar* pani = 0, *p;
  ulong ani_len;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */
  do {
    /* ���ANI���������� */
    if ((ani_len = isio_length(pfile)) < (FORMTYPESEIZE * 4)) {
      b_status = ER_NONIMAGE;
      break;
    }
    /* ��λ���ļ�ͷ */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* �����ڴ��Դ��������ANIͼ�����ݣ�+8��Ϊ�˷�ֹ���뺯��������Խ�磩 */
    if ((pani = (uchar*)isirw_malloc(ani_len + 8)) == 0) {
      b_status = ER_MEMORYERR;
      break; /* �ڴ治�� */
    }
    memset((void*)(pani + ani_len), 0, 8);
    /* ��ȡ������ANI���ݵ��ڴ��� */
    if (isio_read((void*)pani, ani_len, 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* �ж��Ƿ���ANIͼ�� */
    if (_is_ani(pani) != 0) {
      b_status = ER_NONIMAGE;
      break;
    }
    /* ��λ�������������RIFF��־�ײ��� */
    p = (uchar*)(pani + FORMTYPESEIZE + sizeof(ulong) + FORMTYPESEIZE);
    /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
    if (pinfo_str->data_state == 0) {
      /* ��ȡ��һ�����ĸ�Ҫ��Ϣ */
      IF_LEAVE(_get_info(p, pinfo_str));
      pinfo_str->imgtype = IMT_RESDYN; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_ANI; /* ͼ���ļ���ʽ����׺���� */
      pinfo_str->compression = ICS_RGB; /* ͼ���ѹ����ʽ */
      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    /* ��ȡͼ������ */
    switch (_get_image(p, pinfo_str)) {
    case 0: /* �ɹ� */
      break;
    case -1: /* �����ͼ�� */
      b_status = ER_BADIMAGE;
      break;
    case -3: /* �ڴ治�� */
      b_status = ER_MEMORYERR;
      break;
    case -4: /* �û��ж� */
      b_status = ER_USERBREAK;
      break;
    case -2: /* ϵͳ�쳣 */
    default:
      assert(0);
      b_status = ER_SYSERR;
      break;
    }
    __LEAVE;
    pinfo_str->data_state = 2;
  }
  while (0);
  if (pani) {
    isirw_free(pani);
  }
  if (b_status != ER_SUCCESS) {
    INFOSTR_free(pinfo_str);
  }
  return b_status;
}
/* ����ͼ�� */
EXERESULT ani_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);
  b_status = ER_NOTSUPPORT; /* ��֧�ֱ��湦�� */
  return b_status;
}

