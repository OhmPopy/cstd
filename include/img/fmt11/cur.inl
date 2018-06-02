// ISeeͼ���������CURͼ���дģ��ʵ���ļ�
// ��ȡ���ܣ�1��4��8��16��24��32λ���ͼ��
// ���湦�ܣ���֧��



/* ������Ͷ��� */
#define CURTYPE   2


/* ������ͼ�����Ͷ��� */
enum CUR_DATA_TYPE {
  CUR_PRI_IMAGE,
  CUR_SUB_IMAGE
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
} CURDIRENTRY, *LPCURDIRENTRY;


/* �����Ϣͷ�ṹ */
typedef struct {
  unsigned short  reserved;   /* ���� */
  unsigned short  type;     /* ��Դ����(CURTYPEΪ���) */
  unsigned short  count;      /* ͼ����� */
  CURDIRENTRY   entries[1];   /* ÿһ��ͼ������ */
} CURDIR, *LPCURDIR;



/*****************************************************************************/
/* �ڲ��������� */

/* ����ɨ���гߴ�(���ֽڶ���) */
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* ��ȡCURͷ�ṹ������ͼ�������ͼ��ߴ�ȣ��������м򵥵ļ��� */
int CALLAGREEMENT _read_cur_header(ISFILE* file, CURDIR** cur)
{
  CURDIR    curdir;
  int     infosize, result = 0;
  LPCURDIR  lpir = 0;

  assert((file != 0) && (cur != 0));

  do {
    /* ��λ���ļ��ײ� */
    if (isio_seek(file, 0, SEEK_SET) == -1) {
      result = -1;
      break;   /* �ļ���дʧ�� */
    }

    /* ��ȡ����ļ���Ϣͷ�ṹ */
    if (isio_read((void*)&curdir, sizeof(CURDIR), 1, file) == 0) {
      result = -4;
      break;   /* ����һ������ļ� */
    }

    /* �ж��ļ���־ */
    if ((curdir.reserved != 0) || (curdir.type != CURTYPE)) {
      result = -4;
      break;   /* ����һ������ļ� */
    }

    /* ���ٻ����һ��ͼ�� */
    if (curdir.count < 1) {
      result = -2;
      break;   /* ����ļ����� */
    }


    /* ��������Ϣ��ĳߴ� */
    infosize = sizeof(CURDIR) + (curdir.count - 1) * sizeof(CURDIRENTRY);

    /* �������ڴ�Ź����Ϣ�Ļ������ڴ�� */
    if ((lpir = (LPCURDIR)isirw_malloc(infosize)) == 0) {
      result = -3;
      break;   /* �ڴ治�� */
    }

    /* ��ȡ�����Ϣ */
    if (isio_seek(file, 0, SEEK_SET) == -1) {
      result = -1;
      break;
    }

    if (isio_read((void*)lpir, infosize, 1, file) == 0) {
      result = -2;
      break;   /* ����ļ�����Ҳ�п������ļ���д����ͼ������Ŀ����Ը��� */
    }
  } while(0);
  
  if (result != 0) {
    if (lpir) {
      isirw_free(lpir);
      lpir = 0;/* ��������������쳣ʱ��*cur������ 0 */
    }
    
    if (result == 0) {
      result = -1;  /* ������쳣���ܶ������ļ���дʧ������� */
    }
  }
  *cur = lpir;  

  return result;  /* ���� 0 �ɹ����� 0 ֵʧ�ܡ�*curָ����ڴ���ڲ���ҪʱӦ�ͷš� */
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

  return result;  /* 0-�ɹ��� -1���Ƿ���ͼ���ʽ */
}


/* ��ȡһ�������������ݿ飨����BITMAPINFOHEADER�ṹ��XOR���ݺ�AND���ݣ�*/
int CALLAGREEMENT _read_image_block(ISFILE* file, unsigned long offset, unsigned long len, unsigned char** image)
{
  unsigned char*  tmp = 0;
  int       result = 0;
  int err = 1;

  assert((file != 0) && (image != 0));

  /* ���ݺϷ��Լ�� */
  if ((offset == 0) || (len == 0)) {
    return -2;    /* ����ļ��������ݲ��Ϸ���*/
  }

  do {
    /* ��λ��дλ�� */
    if (isio_seek(file, offset, SEEK_SET) == -1) {
      result = -1;
      break;   /* �ļ���дʧ�� */
    }

    if ((tmp = (unsigned char*)isirw_malloc(len)) == 0) {
      result = -3;
      break;   /* �ڴ治�� */
    }

    /* ��ȡͼ������ */
    if (isio_read((void*)tmp, len, 1, file) == 0) {
      result = -2;
      break;   /* ����ļ����� */
    }

    if (((wBITMAPINFOHEADER*)tmp)->biSize != sizeof(wBITMAPINFOHEADER)) {
      result = -2;
      break;   /* ����ļ����� */
    }
    err = 0;
  } while(0);

  if (err) {
    if (result != 0) {
      if (tmp) {
        isirw_free(tmp);
        tmp = 0;
      }

      if (result == 0) {
        result = -1;      /* �ļ���дʧ�� */
      }
    }
  }

  *image = tmp;

  return result;  /* ���� 0 �ɹ����� 0 ֵʧ�ܡ�*tmpָ����ڴ���ڲ���ҪʱӦ�ͷš� */
}


/* ��������ݿ�תΪ���ݰ��� */
int CALLAGREEMENT _conv_image_block(wBITMAPINFOHEADER* lpbmi, void* lpdest, enum CUR_DATA_TYPE mark)
{
  LPINFOSTR   pinfo = (LPINFOSTR)0;
  LPSUBIMGBLOCK psubinfo = (LPSUBIMGBLOCK)0;
  LPBITMAPINFO  pbi = (LPBITMAPINFO)lpbmi;

  unsigned char* p, *pxor, *pand;
  unsigned long pal_num = 0;
  int       i, linesize, linesize2;

  assert((lpbmi) && (lpdest));
  assert((mark == CUR_PRI_IMAGE) || (mark == CUR_SUB_IMAGE));

  if (mark == CUR_PRI_IMAGE) {
    pinfo = (LPINFOSTR)lpdest;

    /* ���õ�ɫ������ */
    if (lpbmi->biBitCount <= 8) {
      pinfo->pal_count = 1UL << pinfo->bitcount;

      pal_num = (lpbmi->biClrUsed == 0) ? pinfo->pal_count : lpbmi->biClrUsed;

      if (pal_num > pinfo->pal_count) {
        return -2;    /* ͼ������ */
      }

      memmove((void*)(pinfo->palette), (const void*)(pbi->bmiColors), sizeof(RGBQUAD)*pal_num);
    }
    else {
      pinfo->pal_count = 0;
    }

    /* XOR����ɨ���гߴ� */
    linesize  = _calcu_scanline_size(pinfo->width, pinfo->bitcount);
    /* AND����ɨ���гߴ� */
    linesize2 = _calcu_scanline_size(pinfo->width, 1);

    assert(pinfo->p_bit_data == (unsigned char*)0);

    /* ����Ŀ��ͼ���ڴ�飨+4 �� β������4�ֽڻ������� */
    pinfo->p_bit_data = (unsigned char*)isirw_orgpix_malloc(linesize * pinfo->height + 4);

    if (!pinfo->p_bit_data) {
      return -3;      /* �ڴ治�� */
    }

    assert(pinfo->pp_line_addr == (void**)0);
    /* �������׵�ַ���� */
    pinfo->pp_line_addr = (void**)isirw_malloc(sizeof(void*) * pinfo->height);

    if (!pinfo->pp_line_addr) {
      isirw_free(pinfo->p_bit_data);
      pinfo->p_bit_data = 0;
      return -3;      /* �ڴ治�� */
    }

    /* ��ʼ�����׵�ַ���飨���� */
    for (i = 0; i < (int)(pinfo->height); i++) {
      pinfo->pp_line_addr[i] = (void*)(pinfo->p_bit_data + ((pinfo->height - i - 1) * linesize));
    }

    /* ����XOR���������׵�ַ */
    p = (unsigned char*)lpbmi;
    p += lpbmi->biSize;
    p += pal_num * sizeof(RGBQUAD);

    /* ����XOR�������� */
    memmove((void*)pinfo->p_bit_data, (const void*)p, (linesize * pinfo->height));

    /* ȡ�� XOR �����׵�ַ */
    pxor = (unsigned char*)pinfo->p_bit_data;
    /* ���� AND �����׵�ַ */
    pand = p + linesize * pinfo->height;

    /* �ϳ�ͼ�� */
    for (i = 0; i < (int)pinfo->height; i++) {
      if (_compose_img(pinfo->bitcount, pinfo->width, pxor, pand) != 0) {
        isirw_free(pinfo->p_bit_data);
        pinfo->p_bit_data = 0;
        isirw_free(pinfo->pp_line_addr);
        pinfo->pp_line_addr = 0;

        return -2;    /* �����ͼ�� */
      }

      pxor += linesize;
      pand += linesize2;
    }
  }
  else {
    psubinfo = (LPSUBIMGBLOCK)lpdest;

    /* ��дͼ����Ϣ */
    psubinfo->width   = (int)lpbmi->biWidth;
    psubinfo->height  = (int)lpbmi->biHeight / 2; /* XOR��AND ͼ�ܸ߶� */
    psubinfo->order   = 1;  /* ���ǵ���ͼ */
    psubinfo->bitcount  = (int)lpbmi->biBitCount;

    if (_get_mask(psubinfo->bitcount,
        (unsigned long*)(&psubinfo->b_mask),
        (unsigned long*)(&psubinfo->g_mask),
        (unsigned long*)(&psubinfo->r_mask),
        (unsigned long*)(&psubinfo->a_mask)) == -1) {
      return -2;    /* ͼ������ */
    }

    psubinfo->left = psubinfo->top = 0;
    psubinfo->dowith = psubinfo->userinput = psubinfo->time = 0;
    psubinfo->colorkey = -1;  /* ��Ϊ�޹ؼ�ɫ */

    /* ���õ�ɫ������ */
    if (lpbmi->biBitCount <= 8) {
      psubinfo->pal_count = 1UL << psubinfo->bitcount;

      pal_num = (lpbmi->biClrUsed == 0) ? psubinfo->pal_count : lpbmi->biClrUsed;

      if (pal_num > (unsigned long)psubinfo->pal_count) {
        return -2;    /* ͼ������ */
      }

      memmove((void*)(psubinfo->palette), (const void*)(pbi->bmiColors), sizeof(RGBQUAD)*pal_num);
    }
    else {
      psubinfo->pal_count = 0;
    }


    /* ȡ��XOR����ɨ���гߴ� */
    linesize  = _calcu_scanline_size(psubinfo->width, psubinfo->bitcount);
    /* ����AND����ɨ���гߴ� */
    linesize2 = _calcu_scanline_size(psubinfo->width, 1);

    assert(psubinfo->p_bit_data == (unsigned char*)0);

    /* ����Ŀ��ͼ���ڴ�飨+4 �� β������4�ֽڻ������� */
    psubinfo->p_bit_data = (unsigned char*)isirw_malloc(linesize * psubinfo->height + 4);

    if (!psubinfo->p_bit_data) {
      return -3;      /* �ڴ治�� */
    }

    assert(psubinfo->pp_line_addr == (void**)0);
    /* �������׵�ַ���� */
    psubinfo->pp_line_addr = (void**)isirw_malloc(sizeof(void*) * psubinfo->height);

    if (!psubinfo->pp_line_addr) {
      isirw_free(psubinfo->p_bit_data);
      psubinfo->p_bit_data = 0;
      return -3;      /* �ڴ治�� */
    }

    /* ��ʼ�����׵�ַ���飨���� */
    for (i = 0; i < (int)(psubinfo->height); i++) {
      psubinfo->pp_line_addr[i] = (void*)(psubinfo->p_bit_data + ((psubinfo->height - i - 1) * linesize));
    }

    /* �������������׵�ַ */
    p = (unsigned char*)lpbmi;
    p += lpbmi->biSize;
    p += pal_num * sizeof(RGBQUAD);

    /* ������������ */
    memmove((void*)psubinfo->p_bit_data, (const void*)p, (linesize * psubinfo->height));

    /* ���� XOR �����׵�ַ */
    pxor = (unsigned char*)psubinfo->p_bit_data;
    /* ���� AND �����׵�ַ */
    pand = p + linesize * psubinfo->height;

    /* �ϳ�ͼ��ʹ��XOR������AND���ݣ�*/
    for (i = 0; i < psubinfo->height; i++) {
      if (_compose_img(psubinfo->bitcount, psubinfo->width, pxor, pand) != 0) {
        isirw_free(psubinfo->p_bit_data);
        psubinfo->p_bit_data = 0;
        isirw_free(psubinfo->pp_line_addr);
        psubinfo->pp_line_addr = 0;

        return -2;    /* �����ͼ�� */
      }

      pxor += linesize;
      pand += linesize2;
    }
  }

  return 0;   /* ���� 0 �ɹ����� 0 ʧ�� */
}


/* �ϳ�һ��ɨ���е�XORͼ��ANDͼ��������ISeeλ����ʽת�������������XORͼ�� */
int CALLAGREEMENT _compose_img(int bitcount, int width, unsigned char* pxor, unsigned char* pand)
{
  unsigned char buf[32], tmp, *p = pxor, isc = 0;
  int i, j, k, l, bytesize;

  assert(bitcount > 0);
  assert(pxor && pand);

  /* һ�δ��� 8 ������ */
  for (i = 0, k = 0; i < width; i += 8, k++) {
    /* ȡ�� 8 �����ص�AND���ݣ��� 1 ���ֽڣ� */
    tmp = *(pand + k);

    /* ��AND����תΪXOR���ظ�ʽ */
    switch (bitcount) {
    case  1:
      buf[0] = tmp;
      bytesize = 1;
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
      *p++ ^= buf[j];
    }
  }

  return 0; /* ���� 0 ��ʾ�ɹ������ط� 0 ֵ��ʾʧ�� */
}


/* ����һ����ͼ��ڵ㣬����ʼ���ڵ������� */
LPSUBIMGBLOCK CALLAGREEMENT _alloc_SUBIMGBLOCK(void)
{
  LPSUBIMGBLOCK p_subimg = 0;

  /* ������ͼ����Ϣ�ڵ�(SUBIMGBLOCK) */
  if ((p_subimg = (LPSUBIMGBLOCK)isirw_malloc(sizeof(SUBIMGBLOCK))) == 0) {
    return 0;  /* �ڴ治�� */
  }

  /* ��ʼ��Ϊ 0 */
  memset((void*)p_subimg, 0, sizeof(SUBIMGBLOCK));

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


/* ��ȡͼ��λ���� */
CC_INLINE int CALLAGREEMENT cur_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;
  LPCURDIR    lpir = 0;
  unsigned char* lpimg = 0;
  int       i;

  wBITMAPINFOHEADER*  lpbmi;
  LPSUBIMGBLOCK   lpsub = 0, lplast;
  LPSUBIMGBLOCK   subimg_tmp;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&cur_load_img_critical);

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

      /* ��ȡ�ļ�ͷ��Ϣ */
      switch (_read_cur_header(pfile, (CURDIR**)&lpir)) {
      case  -1:   /* �ļ���дʧ�� */
        b_status = ER_FILERWERR;
        __leave;
        break;

      case  -2:   /* ����ļ����� */
        b_status = ER_BADIMAGE;
        __leave;
        break;

      case  -3:   /* �ڴ治�� */
        b_status = ER_MEMORYERR;
        __leave;
        break;

      case  -4:   /* ����һ������ļ� */
        b_status = ER_NONIMAGE;
        __leave;
        break;

      case  0:    /* �ɹ� */
        break;

      default:
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* ��ȡ��ͼ�����ݿ� */
      switch (_read_image_block(pfile, (unsigned long)lpir->entries[0].image_offset, (unsigned long)lpir->entries[0].bytes_in_res, &lpimg)) {
      case  -1:   /* �ļ���дʧ�� */
        b_status = ER_FILERWERR;
        __leave;
        break;

      case  -2:   /* ����ļ����� */
        b_status = ER_BADIMAGE;
        __leave;
        break;

      case  -3:   /* �ڴ治�� */
        b_status = ER_MEMORYERR;
        __leave;
        break;

      case  0:    /* �ɹ� */
        break;

      default:
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      lpbmi = (wBITMAPINFOHEADER*)lpimg;

      assert(lpbmi);
      assert(lpir->count > 0);

      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        pinfo_str->imgtype    = IMT_RESSTATIC;  /* ͼ���ļ����� */
        pinfo_str->imgformat  = IMF_CUR;      /* ͼ���ļ���ʽ����׺���� */
        pinfo_str->compression  = ICS_RGB;      /* ͼ���ѹ����ʽ */

        /* ��дͼ����Ϣ */
        pinfo_str->width  = (unsigned long)lpbmi->biWidth;
        pinfo_str->height = (unsigned long)lpbmi->biHeight / 2; /* XOR��AND ͼ�ܸ߶� */
        pinfo_str->order  = 1;  /* ���ǵ���ͼ */
        pinfo_str->bitcount = (unsigned long)lpbmi->biBitCount;

        if (_get_mask(pinfo_str->bitcount,
            (unsigned long*)(&pinfo_str->b_mask),
            (unsigned long*)(&pinfo_str->g_mask),
            (unsigned long*)(&pinfo_str->r_mask),
            (unsigned long*)(&pinfo_str->a_mask)) == -1) {
          b_status = ER_BADIMAGE;
          __LEAVE;
        }

        /* �趨���ݰ�״̬ */
        pinfo_str->data_state = 1;
      }

      /* ����ͼ����� */
      pinfo_str->imgnumbers = (unsigned long)lpir->count;
      assert(pinfo_str->psubimg == 0);
      pinfo_str->psubimg  = (LPSUBIMGBLOCK)0;

      /* ����֡ͼ�����ݰ��� */
      switch (_conv_image_block(lpbmi, (void*)pinfo_str, CUR_PRI_IMAGE)) {
      case  -2:   /* ����ļ����� */
        b_status = ER_BADIMAGE;
        __leave;
        break;

      case  -3:   /* �ڴ治�� */
        b_status = ER_MEMORYERR;
        __leave;
        break;

      case  0:    /* �ɹ� */
        break;

      default:
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* ��ȡ����֡ͼ�����������ã��ͷ� */
      isirw_free(lpimg);
      lpimg = (unsigned char*)lpbmi = (unsigned char*)0;

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

      /* һ���Խ��ȵ��� */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_LOAD)) {
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


      lplast = 0;

      /* ������ͼ�� */
      for (i = 0; i < (int)(pinfo_str->imgnumbers - 1)/*��ͼ����*/; i++) {
        if ((lpsub = _alloc_SUBIMGBLOCK()) == 0) {
          b_status = ER_MEMORYERR;
          __leave;
        }

        lpsub->number = i + 1; /* ��ͼ������� 1 ��ʼ */
        lpsub->parents = pinfo_str;

        /* ������ͼ������β�� */
        if (lplast == 0) {
          pinfo_str->psubimg = lpsub;
          lplast = lpsub;

          lpsub->prev = 0;
          lpsub->next = 0;
        }
        else {
          lpsub->prev = lplast;
          lpsub->next = 0;
          lplast->next = lpsub;
          lplast = lpsub;
        }

        /* ��ȡ��ͼ�����ݿ� */
        switch (_read_image_block(pfile, (unsigned long)lpir->entries[i + 1].image_offset, (unsigned long)lpir->entries[i + 1].bytes_in_res, &lpimg)) {
        case  -1:   /* �ļ���дʧ�� */
          b_status = ER_FILERWERR;
          __leave;
          break;

        case  -2:   /* ����ļ����� */
          b_status = ER_BADIMAGE;
          __leave;
          break;

        case  -3:   /* �ڴ治�� */
          b_status = ER_MEMORYERR;
          __leave;
          break;

        case  0:    /* �ɹ� */
          break;

        default:
          b_status = ER_SYSERR;
          __leave;
          break;
        }

        lpbmi = (wBITMAPINFOHEADER*)lpimg;

        /* ת����ͼ�����ݰ��� */
        switch (_conv_image_block(lpbmi, (void*)lpsub, CUR_SUB_IMAGE)) {
        case  -2:   /* ����ļ����� */
          b_status = ER_BADIMAGE;
          __leave;
          break;

        case  -3:   /* �ڴ治�� */
          b_status = ER_MEMORYERR;
          __leave;
          break;

        case  0:    /* �ɹ� */
          break;

        default:
          b_status = ER_SYSERR;
          __leave;
          break;
        }

        isirw_free(lpimg);
        lpimg = (unsigned char*)lpbmi = (unsigned char*)0;

        /* �ص�֪ͨ */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_SERT_IMAGE, lpsub->number, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
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

        /* һ���Խ��ȵ��� */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, lpsub->number, 0, lpsub->height, IRWE_CALLBACK_FUNID_LOAD)) {
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

        /* һ����ͼ����Ƚ��� */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, lpsub->number, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
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

      pinfo_str->data_state = 2;
    }
    __finally {
      if (lpimg) {
        isirw_free(lpimg);
      }

      if (lpir) {
        isirw_free(lpir);
      }

      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        /* �ͷ���ͼ�� */
        if (pinfo_str->psubimg != 0) {
          while (subimg_tmp = pinfo_str->psubimg->next) {
            _free_SUBIMGBLOCK(pinfo_str->psubimg);
            pinfo_str->psubimg = subimg_tmp;
          }

          _free_SUBIMGBLOCK(pinfo_str->psubimg);
          pinfo_str->psubimg = 0;
        }

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
          if (pinfo_str->psubimg != 0) {
            while (subimg_tmp = pinfo_str->psubimg->next) {
              _free_SUBIMGBLOCK(pinfo_str->psubimg);
              pinfo_str->psubimg = subimg_tmp;
            }

            _free_SUBIMGBLOCK(pinfo_str->psubimg);
            pinfo_str->psubimg = 0;
          }

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
      }

      LeaveCriticalSection(&cur_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}
