/********************************************************************
 cur.c
 ���ļ���;�� CURͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ�1��4��8��16��24��32λ���ͼ��
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
 2000-8 ��һ�������档
********************************************************************/
#ifndef WIN32
#if defined(_WIN32)||defined(_WINDOWS)
#define WIN32
#endif
#endif /* WIN32 */
/* ������Ͷ��� */
#define CURTYPE 2
/* ������ͼ�����Ͷ��� */
enum CUR_DATA_TYPE {
  CUR_PRI_IMAGE,
  CUR_SUB_IMAGE
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
} CURDIRENTRY, *LPCURDIRENTRY;
/* �����Ϣͷ�ṹ */
typedef struct {
  ushort reserved; /* ���� */
  ushort type; /* ��Դ����(CURTYPEΪ���) */
  ushort count; /* ͼ����� */
  CURDIRENTRY entries[1]; /* ÿһ��ͼ������ */
} CURDIR, *LPCURDIR;
#pragma pack()
//IRWP_INFO cur_irwp_info; /* �����Ϣ�� */
/* �ڲ����ֺ��� */
/*****************************************************************************/
/* �ڲ��������� */
/* ��ȡCURͷ�ṹ������ͼ�������ͼ��ߴ�ȣ��������м򵥵ļ��� */
EXERESULT _read_cur_header(ISFILE* file, CURDIR** cur)
{
  CURDIR curdir;
  int infosize;
  EXERESULT result = ER_SUCCESS;
  LPCURDIR lpir = 0;
  assert((file != 0) && (cur != 0));
  do {
    /* ��λ���ļ��ײ� */
    if (isio_seek(file, 0, SEEK_SET) == -1) {
      result = ER_FILERWERR;
      break; /* �ļ���дʧ�� */
    }
    /* ��ȡ����ļ���Ϣͷ�ṹ */
    if (isio_read((void*)&curdir, sizeof(CURDIR), 1, file) == 0) {
      result = ER_NONIMAGE;
      break; /* ����һ������ļ� */
    }
    /* �ж��ļ���־ */
    if ((curdir.reserved != 0) || (curdir.type != CURTYPE)) {
      result = ER_NONIMAGE;
      break; /* ����һ������ļ� */
    }
    /* ���ٻ����һ��ͼ�� */
    if (curdir.count < 1) {
      result = ER_BADIMAGE;
      break; /* ����ļ����� */
    }
    /* ��������Ϣ��ĳߴ� */
    infosize = sizeof(CURDIR) + (curdir.count - 1) * sizeof(CURDIRENTRY);
    /* �������ڴ�Ź����Ϣ�Ļ������ڴ�� */
    if ((lpir = (LPCURDIR)isirw_malloc(infosize)) == 0) {
      result = ER_MEMORYERR;
      break; /* �ڴ治�� */
    }
    /* ��ȡ�����Ϣ */
    if (isio_seek(file, 0, SEEK_SET) == -1) {
      result = ER_FILERWERR;
      break;
    }
    if (isio_read((void*)lpir, infosize, 1, file) == 0) {
      result = ER_BADIMAGE;
      break; /* ����ļ�����Ҳ�п������ļ���д����ͼ������Ŀ����Ը��� */
    }
  }
  while (0);
  if (result != ER_SUCCESS) {
    if (lpir) {
      isirw_free(lpir);
      lpir = 0;
    }
  }
  *cur = lpir; /* ��������������쳣ʱ��*cur������ 0 */
  return result; /* ���� 0 �ɹ����� 0 ֵʧ�ܡ�*curָ����ڴ���ڲ���ҪʱӦ�ͷš� */
}
/* ��ȡһ�������������ݿ飨����BITMAPINFOHEADER�ṹ��XOR���ݺ�AND���ݣ�*/
EXERESULT _read_image_block(ISFILE* file, ulong offset, ulong len, uchar** image)
{
  uchar* tmp = 0;
  EXERESULT result = ER_SUCCESS;
  assert((file != 0) && (image != 0));
  /* ���ݺϷ��Լ�� */
  if ((offset == 0) || (len == 0)) {
    return ER_BADIMAGE; /* ����ļ��������ݲ��Ϸ���*/
  }
  do {
    /* ��λ��дλ�� */
    if (isio_seek(file, offset, SEEK_SET) == -1) {
      result = ER_FILERWERR;
      break; /* �ļ���дʧ�� */
    }
    if ((tmp = (uchar*)isirw_malloc(len)) == 0) {
      result = ER_MEMORYERR;
      break; /* �ڴ治�� */
    }
    /* ��ȡͼ������ */
    if (isio_read((void*)tmp, len, 1, file) == 0) {
      result = ER_BADIMAGE;
      break; /* ����ļ����� */
    }
    if (((wBITMAPINFOHEADER*)tmp)->biSize != sizeof(wBITMAPINFOHEADER)) {
      result = ER_BADIMAGE;
      break; /* ����ļ����� */
    }
  }
  while (0);
  if (result != 0) {
    if (tmp) {
      isirw_free(tmp);
      tmp = 0;
    }
  }
  *image = tmp;
  return result; /* ���� 0 �ɹ����� 0 ֵʧ�ܡ�*tmpָ����ڴ���ڲ���ҪʱӦ�ͷš� */
}
/* �ϳ�һ��ɨ���е�XORͼ��ANDͼ��������ISeeλ����ʽת�������������XORͼ�� */
int _compose_img(int bitcount, int width, uchar* pxor, uchar* pand)
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
/* ��������ݿ�תΪ���ݰ��� */
EXERESULT _conv_image_block(wBITMAPINFOHEADER* lpbmi, void* lpdest, enum CUR_DATA_TYPE mark)
{
  EXERESULT b_status = ER_SUCCESS;
  INFOSTR* pinfo = (INFOSTR*)0;
  SUBIMGBLOCK* psubinfo = (SUBIMGBLOCK*)0;
  wBITMAPINFO* pbi = (wBITMAPINFO*)lpbmi;
  uchar* p, *pxor, *pand;
  ulong pal_num = 0;
  int i, linesize, linesize2;
  assert((lpbmi) && (lpdest));
  assert((mark == CUR_PRI_IMAGE) || (mark == CUR_SUB_IMAGE));
  if (mark == CUR_PRI_IMAGE) {
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
      if (_compose_img(pinfo->bitcount, pinfo->width, pxor, pand) != 0) {
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
    if(ER_SUCCESS!=(b_status = _get_mask(psubinfo->bitcount,(&psubinfo->b_mask),(&psubinfo->g_mask),
      &psubinfo->r_mask,&psubinfo->a_mask))) {
      return b_status;
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
      if (_compose_img(psubinfo->bitcount, psubinfo->width, pxor, pand) != 0) {
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
EXERESULT cur_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LPCURDIR lpir = 0;
  wBITMAPINFOHEADER* lpbmi = 0;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */
  do {
    /* ��ȡ�ļ�ͷ��Ϣ */
    IF_LEAVE(_read_cur_header(pfile, (CURDIR**)&lpir));
    pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
    pinfo_str->imgformat = IMF_CUR; /* ͼ���ļ���ʽ����׺���� */
    pinfo_str->compression = ICS_RGB; /* ͼ���ѹ����ʽ */
    /* ��ȡ��ͼ�����ݿ� */
    IF_LEAVE(_read_image_block(pfile, (ulong)lpir->entries[0].image_offset, (ulong)lpir->entries[0].bytes_in_res, (uchar**)&lpbmi));
    /* ��дͼ����Ϣ */
    pinfo_str->width = (ulong)lpbmi->biWidth;
    pinfo_str->height = (ulong)lpbmi->biHeight / 2; /* XOR��AND ͼ�ܸ߶� */
    pinfo_str->order = 1; /* ���ǵ���ͼ */
    pinfo_str->bitcount = (ulong)lpbmi->biBitCount;
    IF_LEAVE(_get_mask(pinfo_str->bitcount, &pinfo_str->b_mask, &pinfo_str->g_mask, &pinfo_str->r_mask, &pinfo_str->a_mask));
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
EXERESULT cur_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LPCURDIR lpir = 0;
  uchar* lpimg = 0;
  int i;
  wBITMAPINFOHEADER* lpbmi;
  SUBIMGBLOCK* lpsub = 0;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */
  do {
    /* ��ȡ�ļ�ͷ��Ϣ */
    IF_LEAVE(_read_cur_header(pfile, (CURDIR**)&lpir));
    /* ��ȡ��ͼ�����ݿ� */
    IF_LEAVE(_read_image_block(pfile, (ulong)lpir->entries[0].image_offset, (ulong)lpir->entries[0].bytes_in_res, &lpimg));
    lpbmi = (wBITMAPINFOHEADER*)lpimg;
    assert(lpbmi);
    assert(lpir->count > 0);
    /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
    if (pinfo_str->data_state == 0) {
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_CUR; /* ͼ���ļ���ʽ����׺���� */
      pinfo_str->compression = ICS_RGB; /* ͼ���ѹ����ʽ */
      /* ��дͼ����Ϣ */
      pinfo_str->width = (ulong)lpbmi->biWidth;
      pinfo_str->height = (ulong)lpbmi->biHeight / 2; /* XOR��AND ͼ�ܸ߶� */
      pinfo_str->order = 1; /* ���ǵ���ͼ */
      pinfo_str->bitcount = (ulong)lpbmi->biBitCount;
      IF_LEAVE(_get_mask(pinfo_str->bitcount,
          (&pinfo_str->b_mask),
          (&pinfo_str->g_mask),
          (&pinfo_str->r_mask),
          (&pinfo_str->a_mask)));
      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    /* ����ͼ����� */
    pinfo_str->imgnumbers = (ulong)lpir->count;
    assert(pinfo_str->psubimg == 0);
    pinfo_str->psubimg = (SUBIMGBLOCK*)0;
    /* ����֡ͼ�����ݰ��� */
    IF_LEAVE(_conv_image_block(lpbmi, (void*)pinfo_str, CUR_PRI_IMAGE));
    /* ��ȡ����֡ͼ�����������ã��ͷ� */
    isirw_free(lpimg);
    lpimg = NULL;
    lpbmi = NULL;
    /* ������ͼ�� */
    for (i = 0; i < (int)(pinfo_str->imgnumbers - 1); i++) { /*��ͼ����*/
      if ((lpsub = _alloc_SUBIMGBLOCK(pinfo_str)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      lpsub->number = i + 1; /* ��ͼ������� 1 ��ʼ */

      /* ��ȡ��ͼ�����ݿ� */
      IF_LEAVE(_read_image_block(pfile, (ulong)lpir->entries[i + 1].image_offset, (ulong)lpir->entries[i + 1].bytes_in_res, &lpimg));
      lpbmi = (wBITMAPINFOHEADER*)lpimg;
      /* ת����ͼ�����ݰ��� */
      IF_LEAVE(_conv_image_block(lpbmi, (void*)lpsub, CUR_SUB_IMAGE))
      isirw_free(lpimg);
      lpimg = NULL;
      lpbmi = NULL;
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
  if ((b_status != ER_SUCCESS)) {
    INFOSTR_free(pinfo_str);
  }
  return b_status;
}


