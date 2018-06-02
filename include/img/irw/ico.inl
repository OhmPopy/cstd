/********************************************************************
 ico.c
 ���ļ���;�� ICOͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ�1��4��8��16��24��32λICOͼ��
 ���湦�ܣ���֧��
 ���ļ���д�ˣ�
 YZ yzfree##yeah.net
 ���ļ��汾�� 30401
 ����޸��ڣ� 2003-4-1
 ----------------------------------------------------------------
 ������ʷ��
 2003-4 ֧��ͼ���д�����ṩ�����ػ���������
 2002-8 ����ӿ�������2.2�������ڴ�������
 2002-3 �����°�ģ�顣ȥ����������Windowsϵͳ�й�
 ��API���ã�ʹ�����������ֲ��
 2001-1 ������ģ����ڵ�һЩBUG��
 2000-8 ��ǿ��ģ����ݴ����ܡ�
 2000-8 ��һ�������档
********************************************************************/

/* ͼ�����Ͷ��� */
#define ICONTYPE  1

/* ������ͼ�����Ͷ��� */
enum ICO_DATA_TYPE {
  ICO_PRI_IMAGE,
    ICO_SUB_IMAGE
};


/* ͼ�����Ϣͷ�ṹ */
typedef struct {
  uchar width;      /* ͼ���� */
  uchar height;     /* ͼ��߶� */
  uchar color_count;  /* ��ɫ����(���λ��ȴ���8ʱΪ0) */
  uchar reserved;
  ushort  planes;     /* λ���� */
  ushort  bit_count;    /* ÿ����λ�� */
  ulong bytes_in_res; /* ͼ����������Դ����ռ���ֽ��� */
  ulong image_offset; /* ͼ�����ݵ�ƫ�� */
} ICONDIRENTRY, *LPICONDIRENTRY;


/* ͼ���ļ���Ϣͷ�ṹ */
typedef struct {
  ushort  reserved;   /* ���� */
  ushort  type;     /* ��Դ����(ICONTYPEΪͼ��) */
  ushort  count;      /* ͼ����� */
  ICONDIRENTRY  entries[1];   /* ÿһ��ͼ������ */
} ICONDIR, *LPICONDIR;

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */
/* ��ȡICOͷ�ṹ������ͼ�������ͼ��ߴ�ȣ��������м򵥵ļ��� */
EXERESULT _read_icon_header(ISFILE* file, ICONDIR** icon)
{
  enum EXERESULT b_status = ER_SUCCESS;
  ICONDIR icondir;
  int infosize, result = 0;
  LPICONDIR lpir = 0;
  assert((file != 0) && (icon != 0));
  do {
    /* ��λ���ļ��ײ� */
    if (isio_seek(file, 0, SEEK_SET) == -1) {
      result = -1;
      break; /* �ļ���дʧ�� */
    }
    /* ��ȡͼ���ļ���Ϣͷ�ṹ */
    if (isio_read((void*)&icondir, sizeof(ICONDIR), 1, file) == 0) {
      result = -4;
      break; /* ����һ��ͼ���ļ� */
    }
    /* �ж��ļ���־ */
    if ((icondir.reserved != 0) || (icondir.type != ICONTYPE)) {
      result = -4;
      break; /* ����һ��ͼ���ļ� */
    }
    /* ���ٻ����һ��ͼ�� */
    if (icondir.count < 1) {
      result = -2;
      break; /* ͼ���ļ����� */
    }
    /* ����ͼ����Ϣ��ĳߴ� */
    infosize = sizeof(ICONDIR) + (icondir.count - 1) * sizeof(ICONDIRENTRY);
    /* �������ڴ��ͼ����Ϣ�Ļ������ڴ�� */
    if ((lpir = (LPICONDIR)isirw_malloc(infosize)) == 0) {
      result = -3;
      break; /* �ڴ治�� */
    }
    /* ��ȡͼ����Ϣ */
    if (isio_seek(file, 0, SEEK_SET) == -1) {
      result = -1;
      break;
    }
    if (isio_read((void*)lpir, infosize, 1, file) == 0) {
      result = -2;
      break; /* ͼ���ļ�����Ҳ�п������ļ���д����ͼ������Ŀ����Ը��� */
    }
  }
  while (0);
  {
    if (result != 0) {
      if (lpir) {
        isirw_free(lpir);
        lpir = 0;
      }
      if (result == 0) {
        result = -1; /* ������쳣���ܶ������ļ���дʧ������� */
      }
    }
  }
  *icon = lpir; /* ��������������쳣ʱ��*icon������ 0 */
  
  switch (result) {
  case -1: /* �ļ���дʧ�� */
    b_status = ER_FILERWERR;
    break;
  case -2: /* ͼ���ļ����� */
    b_status = ER_BADIMAGE;
    break;
  case -3: /* �ڴ治�� */
    b_status = ER_MEMORYERR;
    break;
  case -4: /* ����һ��ͼ���ļ� */
    b_status = ER_NONIMAGE;
    break;
  case 0: /* �ɹ� */
    break;
  default:
    b_status = ER_SYSERR;
    break;
      }

  return b_status; /* ���� 0 �ɹ����� 0 ֵʧ�ܡ�*iconָ����ڴ���ڲ���ҪʱӦ�ͷš� */
}
/* ��ȡһ��ͼ���������?ݿ飨����BITMAPINFOHEADER�ṹ��XOR���ݺ�AND���ݣ�*/
EXERESULT _ico_read_image_block(ISFILE* file, ulong offset, ulong len, uchar** image)
{
  enum EXERESULT b_status = ER_SUCCESS;
  uchar* tmp = 0;
  int result = 0;
  assert((file != 0) && (image != 0));
  /* ���ݺϷ��Լ�� */
  if ((offset == 0) || (len == 0)) {
    return ER_BADIMAGE; /* ͼ���ļ��������ݲ��Ϸ���*/
  }
  do {
    /* ��λ��дλ�� */
    if (isio_seek(file, offset, SEEK_SET) == -1) {
      result = -1;
      break; /* �ļ���дʧ�� */
    }
    if ((tmp = (uchar*)isirw_malloc(len)) == 0) {
      result = -3;
      break; /* �ڴ治�� */
    }
    /* ��ȡͼ������ */
    if (isio_read((void*)tmp, len, 1, file) == 0) {
      result = -2;
      break; /* ͼ���ļ����� */
    }
    if (((wBITMAPINFOHEADER*)tmp)->biSize != sizeof(wBITMAPINFOHEADER)) {
      result = -2;
      break; /* ͼ���ļ����� */
    }
  }
  while (0);
  {
    if (result != 0) {
      if (tmp) {
        isirw_free(tmp);
        tmp = 0;
      }
      if (result == 0) {
        result = -1; /* �ļ���дʧ�� */
      }
    }
  }
  *image = tmp;
  
  switch (result) {
  case -1: /* �ļ���дʧ�� */
    b_status = ER_FILERWERR;
    break;
  case -2: /* ͼ���ļ����� */
    b_status = ER_BADIMAGE;
    break;
  case -3: /* �ڴ治�� */
    b_status = ER_MEMORYERR;
    break;
  case 0: /* �ɹ� */
    break;
  default:
    b_status = ER_SYSERR;
    break;
      }

  return b_status; /* ���� 0 �ɹ����� 0 ֵʧ�ܡ�*tmpָ����ڴ���ڲ���ҪʱӦ�ͷš� */
}
/* �ϳ�һ��ɨ���е�XORͼ��ANDͼ��������ISeλ����ʽת�������������XORͼ�� */
int _ico_compose_img(int bitcount, int width, uchar* pxor, uchar* pand)
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
      bytesize = 1;
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
/* ��ͼ�����ݿ�תΪ���ݰ��� */
EXERESULT _ico_conv_image_block(wBITMAPINFOHEADER* lpbmi, void* lpdest, enum ICO_DATA_TYPE mark)
{
  INFOSTR* pinfo = (INFOSTR*)0;
  SUBIMGBLOCK* psubinfo = (SUBIMGBLOCK*)0;
  wBITMAPINFO* pbi = (wBITMAPINFO*)lpbmi;
  uchar* p, *pxor, *pand;
  ulong pal_num = 0;
  int i, linesize, linesize2;
  assert((lpbmi) && (lpdest));
  assert((mark == ICO_PRI_IMAGE) || (mark == ICO_SUB_IMAGE));
  if (mark == ICO_PRI_IMAGE) {
    pinfo = (INFOSTR*)lpdest;
    /* ���õ�ɫ������ */
    if (lpbmi->biBitCount <= 8) {
      pinfo->pal_count = 1UL << pinfo->bitcount;
      pal_num = (lpbmi->biClrUsed == 0) ? pinfo->pal_count : lpbmi->biClrUsed;
      if (pal_num > pinfo->pal_count) {
        return ER_BADIMAGE; /* ͼ������ */
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
    assert(pinfo->p_bit_data == (uchar*)0);
    /* ����Ŀ��ͼ���ڴ�飨+4 �� β������4�ֽڻ������� */
    pinfo->p_bit_data = (uchar*)isirw_malloc(linesize * pinfo->height + 4);
    if (!pinfo->p_bit_data) {
      return ER_MEMORYERR; /* �ڴ治�� */
    }
    assert(pinfo->pp_line_addr == NULL);
    /* �������׵�ַ���� */
    pinfo->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * pinfo->height);
    if (!pinfo->pp_line_addr) {
      isirw_free(pinfo->p_bit_data);
      pinfo->p_bit_data = 0;
      return ER_MEMORYERR; /* �ڴ治�� */
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
      if (_ico_compose_img(pinfo->bitcount, pinfo->width, pxor, pand) != 0) {
        isirw_free(pinfo->p_bit_data);
        pinfo->p_bit_data = 0;
        isirw_free(pinfo->pp_line_addr);
        pinfo->pp_line_addr = 0;
        return ER_BADIMAGE; /* �����ͼ�� */
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
      return ER_BADIMAGE; /* ͼ������ */
    }
    psubinfo->left = psubinfo->top = 0;
    psubinfo->dowith = psubinfo->userinput = psubinfo->time = 0;
    psubinfo->colorkey = -1; /* ��Ϊ�޹ؼ�ɫ */
    /* ���õ�ɫ������ */
    if (lpbmi->biBitCount <= 8) {
      psubinfo->pal_count = 1UL << psubinfo->bitcount;
      pal_num = (lpbmi->biClrUsed == 0) ? psubinfo->pal_count : lpbmi->biClrUsed;
      if (pal_num > (ulong)psubinfo->pal_count) {
        return ER_BADIMAGE; /* ͼ������ */
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
      return ER_MEMORYERR; /* �ڴ治�� */
    }
    assert(psubinfo->pp_line_addr == NULL);
    /* �������׵�ַ���� */
    psubinfo->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * psubinfo->height);
    if (!psubinfo->pp_line_addr) {
      isirw_free(psubinfo->p_bit_data);
      psubinfo->p_bit_data = 0;
      return ER_MEMORYERR; /* �ڴ治�� */
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
      if (_ico_compose_img(psubinfo->bitcount, psubinfo->width, pxor, pand) != 0) {
        isirw_free(psubinfo->p_bit_data);
        psubinfo->p_bit_data = 0;
        isirw_free(psubinfo->pp_line_addr);
        psubinfo->pp_line_addr = 0;
        return ER_BADIMAGE; /* �����ͼ�� */
      }
      pxor += linesize;
      pand += linesize2;
    }
  }
  return ER_SUCCESS; /* ���� 0 �ɹ����� 0 ʧ�� */
}
/* ��ȡͼ����Ϣ */
EXERESULT ico_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LPICONDIR lpir = 0;
  wBITMAPINFOHEADER* lpbmi = 0;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */
  
  do {
    /* ��ȡ�ļ�ͷ��Ϣ */
    IF_LEAVE(_read_icon_header(pfile, (ICONDIR**)&lpir));
    pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
    pinfo_str->imgformat = IMF_ICO; /* ͼ���ļ���ʽ����׺���� */
    pinfo_str->compression = ICS_RGB; /* ͼ���ѹ����ʽ */
    /* ��ȡ��ͼ�����ݿ� */
    IF_LEAVE(_ico_read_image_block(pfile, (ulong)lpir->entries[0].image_offset, (ulong)lpir->entries[0].bytes_in_res, (uchar**)&lpbmi));
    /* ��дͼ����Ϣ */
    pinfo_str->width = (ulong)lpbmi->biWidth;
    pinfo_str->height = (ulong)lpbmi->biHeight / 2; /* XOR��AND ͼ�ܸ߶� */
    pinfo_str->order = 1; /* ���ǵ���ͼ */
    pinfo_str->bitcount = (ulong)lpbmi->biBitCount;
    if (_get_mask(pinfo_str->bitcount, (&pinfo_str->b_mask), (&pinfo_str->g_mask), (&pinfo_str->r_mask), (&pinfo_str->a_mask)) == -1) {
      b_status = ER_BADIMAGE;
      break;
    }
    /* �趨���ݰ�״̬ */
    pinfo_str->data_state = 1;
  }
  while (0);
  
  if (lpbmi) {
    isirw_free(lpbmi);
  }
  if (lpir) {
    isirw_free(lpir);
  }
  
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT ico_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LPICONDIR lpir = 0;
  uchar* lpimg = 0;
  int i;
  wBITMAPINFOHEADER* lpbmi;
  SUBIMGBLOCK* lpsub = 0, *lplast;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */
  
  do {
    /* ��ȡ�ļ�ͷ��Ϣ */
    IF_LEAVE(_read_icon_header(pfile, (ICONDIR**)&lpir));
    /* ��ȡ��ͼ�����ݿ� */
    IF_LEAVE(_ico_read_image_block(pfile, (ulong)lpir->entries[0].image_offset, (ulong)lpir->entries[0].bytes_in_res, &lpimg));
    lpbmi = (wBITMAPINFOHEADER*)lpimg;
    assert(lpbmi);
    assert(lpir->count > 0);
    /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
    if (pinfo_str->data_state == 0) {
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_ICO; /* ͼ���ļ���ʽ����׺���� */
      pinfo_str->compression = ICS_RGB; /* ͼ���ѹ����ʽ */
      /* ��дͼ����Ϣ */
      pinfo_str->width = (ulong)lpbmi->biWidth;
      pinfo_str->height = (ulong)lpbmi->biHeight / 2; /* XOR��AND ͼ�ܸ߶� */
      pinfo_str->order = 1; /* ���ǵ���ͼ */
      pinfo_str->bitcount = (ulong)lpbmi->biBitCount;
      if (_get_mask(pinfo_str->bitcount,
        (&pinfo_str->b_mask),
        (&pinfo_str->g_mask),
        (&pinfo_str->r_mask),
        (&pinfo_str->a_mask)) == -1) {
        b_status = ER_BADIMAGE;
        break;
      }
      pinfo_str->data_state = 1;
    }
    /* ����ͼ����� */
    assert(pinfo_str->psubimg == 0);
    pinfo_str->imgnumbers = (ulong)lpir->count;
    pinfo_str->psubimg = (SUBIMGBLOCK*)0;
    /* ����֡ͼ�����ݰ��� */
    IF_LEAVE(_ico_conv_image_block(lpbmi, (void*)pinfo_str, ICO_PRI_IMAGE));
    /* ��ȡ����֡ͼ�����������ã��ͷ� */
    isirw_free(lpimg);
    lpimg = NULL;lpbmi = NULL;
    lplast = 0;
    /* ������ͼ�� */
    for (i = 0; i < (int)(pinfo_str->imgnumbers - 1)/*��ͼ����*/; i++) {
      if ((lpsub = _alloc_SUBIMGBLOCK(pinfo_str)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      lpsub->number = i + 1; /* ��ͼ������� 1 ��ʼ */
      /* ��ȡ��ͼ�����ݿ� */
      switch (_ico_read_image_block(pfile, (ulong)lpir->entries[i + 1].image_offset, (ulong)lpir->entries[i + 1].bytes_in_res, &lpimg)) {
      case -1: /* �ļ���дʧ�� */
        b_status = ER_FILERWERR;
        break;
      case -2: /* ͼ���ļ����� */
        b_status = ER_BADIMAGE;
        break;
      case -3: /* �ڴ治�� */
        b_status = ER_MEMORYERR;
        break;
      case 0: /* �ɹ� */
        break;
      default:
        b_status = ER_SYSERR;
        break;
      }
      lpbmi = (wBITMAPINFOHEADER*)lpimg;
      /* ת����ͼ�����ݰ��� */
      IF_LEAVE(_ico_conv_image_block(lpbmi, (void*)lpsub, ICO_SUB_IMAGE));
      isirw_free(lpimg);
      lpimg = NULL;lpbmi = NULL;
    }
    pinfo_str->data_state = 2;
  }
  while (0);
  
  if (lpimg) {
    isirw_free(lpimg);
  }
  if (lpir) {
    isirw_free(lpir);
  }
  if (b_status != ER_SUCCESS) {
    INFOSTR_free(pinfo_str);
  }
  return b_status;
}
/* ����ͼ�� */
EXERESULT ico_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  return ER_NOTSUPPORT;
}