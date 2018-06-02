/********************************************************************
 pcd.c
 ���ļ���;�� PCDͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ��ɶ�ȡ24λPCDͼ��ǰ������
 ���湦�ܣ���֧��
 ���ļ���д�ˣ�
 YZ yzfree##yeah.net
 ���ļ��汾�� 30401
 ����޸��ڣ� 2003-4-1
 ----------------------------------------------------------------
 ������ʷ��
 2003-4 ֧��ͼ���д�����ṩ�����ػ���������
 2002-8 ����ӿ�������2.2�������ڴ�������
 2002-6 ��һ�������棨�°棩
********************************************************************/


/* �ļ��б��־��PCD����ϸ�ĸ�ʽ�ĵ����´�����û�з�����Щ
 * �ĵ�������ֻ��ͨ���ж�ͼ����С���������ȷ�����Ƿ���һ��
 * PCDͼ�󡣵�Ȼ�����ַ�ʽ�Ǻܲ���ȷ�ģ�
 */
#define PCD_MIN_SIZE          (1048576UL)
/* PCDͼ������Ϣλ�� */
#define PCD_VERTICAL_LOC        72
#define PCD_VERTICAL_MASK       (0x3f)
/* PCD��ͼ��ߴ磨PCDͼ���ǹ̶��ߴ�ģ�*/
#define PCD_1_W             192
#define PCD_1_H             128
#define PCD_2_W             384
#define PCD_2_H             256
#define PCD_3_W             768
#define PCD_3_H             512
#define PCD_4_W             1536
#define PCD_4_H             1024
#define PCD_5_W             3072
#define PCD_5_H             2048
/* PCDȱʡͼ��λ��� */
#define PCD_DEF_BITCOUNT        24
/* PCD��ͼ��������ͼ���е�ƫ�ƣ�������ͼ���ƫ�ƿ´﹫˾û�й����� */
#define PCD_1_OFF           0x2000
#define PCD_2_OFF           0xb800
#define PCD_3_OFF           0x30000


/* �ڲ����ֺ��� */
static enum EXERESULT _read_img(void* pinfo, int img_num, ISFILE* pfile);
static SUBIMGBLOCK* _allsub(int num, INFOSTR* pinfo_str);

/* ��ȡͼ����Ϣ */
EXERESULT pcd_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  uchar ord;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */

    do {
      /* ȷ��PCDͼ�󳤶� */
      if (isio_seek(pfile, 0, SEEK_END) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* �򵥵�ͨ��ͼ�󳤶���ȷ���Ƿ���PCDͼ�� */
      if (isio_tell(pfile) < PCD_MIN_SIZE) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ��ȡͼ������Ϣ */
      if (isio_seek(pfile, PCD_VERTICAL_LOC, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&ord, sizeof(uchar), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_PCD; /* ͼ���ļ���ʽ����׺���� */
      pinfo_str->compression = ICS_JPEGYCbCr; /* ͼ���ѹ����ʽ */
      /* ��дͼ����Ϣ */
      pinfo_str->width = PCD_1_W; /* ȡ��һ��ͼ��ĳߴ����� */
      pinfo_str->height = PCD_1_H;
      pinfo_str->order = ((ord & PCD_VERTICAL_MASK) != 8);
      pinfo_str->bitcount = PCD_DEF_BITCOUNT; /* ��֧��8λ��PCD */
      /* ��д�������ݣ�24λ��*/
      SETMASK_24(pinfo_str);
      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    while (0);
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT pcd_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  int linesize, i;
  uchar ord;
  SUBIMGBLOCK* subimg_tmp;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */

    do {
      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        if (isio_seek(pfile, 0, SEEK_END) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* �򵥵�ͨ��ͼ�󳤶���ȷ���Ƿ���PCDͼ�� */
        if (isio_tell(pfile) < PCD_MIN_SIZE) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* ��ȡͼ������Ϣ */
        if (isio_seek(pfile, PCD_VERTICAL_LOC, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        if (isio_read((void*)&ord, sizeof(uchar), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
        pinfo_str->imgformat = IMF_PCD; /* ͼ���ļ���ʽ����׺���� */
        pinfo_str->compression = ICS_JPEGYCbCr; /* ͼ���ѹ����ʽ */
        pinfo_str->width = PCD_1_W; /* ȡ��һ��ͼ��ĳߴ����� */
        pinfo_str->height = PCD_1_H;
        pinfo_str->order = ((ord & PCD_VERTICAL_MASK) != 8);
        pinfo_str->bitcount = PCD_DEF_BITCOUNT; /* ��֧��8λ��PCD */
        SETMASK_24(pinfo_str);
        pinfo_str->data_state = 1;
      }
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
      /* �������׵�ַ���� */
      pinfo_str->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * pinfo_str->height);
      if (!pinfo_str->pp_line_addr) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* ��д���׵�ַ���� */
      if (pinfo_str->order == 0) { /* ���� */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
        }
      }
      else { /* ���� */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
        }
      }
      /* ������ͼ�� */
      if ((b_status = _read_img((void*)pinfo_str, 0, pfile)) != ER_SUCCESS) {
        break;
      }
      /* ����ڶ���ͼ����ӽڵ� */
      if ((subimg_tmp = _allsub(1, pinfo_str)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* ����ڶ���ͼ�� */
      if ((b_status = _read_img((void*)subimg_tmp, 1, pfile)) != ER_SUCCESS) {
        break;
      }
      /* ���������ͼ����ӽڵ� */
      if ((subimg_tmp = _allsub(2, pinfo_str)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* ���������ͼ�� */
      if ((b_status = _read_img((void*)subimg_tmp, 2, pfile)) != ER_SUCCESS) {
        break;
      }
      pinfo_str->data_state = 2;
    }
    while (0);
    
    if (b_status != ER_SUCCESS) {
      INFOSTR_free(pinfo_str);
    }
  return b_status;
}
/* ����ͼ�� */
EXERESULT pcd_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  return ER_NOTSUPPORT;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */
/* ��ָ���������ӣ�ͼ��������ݰ� */
static enum EXERESULT _read_img(void* pinfo, int img_num, ISFILE* pfile)
{
  INFOSTR* pinfo_str = (INFOSTR*)pinfo;
  SUBIMGBLOCK* psub_str = (SUBIMGBLOCK*)pinfo;
  int w, h, bitcount, linesize, i, j;
  long pos;
  uchar* y1 = 0, *y2 = 0, *cbcr = 0, *pbits, *p;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pinfo && pfile);
  assert((img_num < 3) && (img_num >= 0)); /* �����ʱֻ�ܶ�ȡǰ����ͼ�� */

    do {
      /* ��ȡͼ��ߴ����� */
      if (img_num == 0) { /* ��һ��ͼ����ͼ��*/
        w = (int)(pinfo_str->width);
        h = (int)(pinfo_str->height);
        bitcount = (int)(pinfo_str->bitcount);
        pbits = (uchar*)(pinfo_str->p_bit_data);
      }
      else { /* ����ͼ����ͼ��*/
        w = (int)(psub_str->width);
        h = (int)(psub_str->height);
        bitcount = (int)(psub_str->bitcount);
        pbits = (uchar*)(psub_str->p_bit_data);
      }
      /* ����ɨ���гߴ� */
      linesize = _calcu_scanline_size(w, bitcount);
      /* ����ϳɻ����� */
      y1 = (uchar*)isirw_malloc(w);
      y2 = (uchar*)isirw_malloc(w);
      cbcr = (uchar*)isirw_malloc(w);
      if ((y1 == 0) || (y2 == 0) || (cbcr == 0)) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* ����ͼ������ƫ�� */
      switch (img_num) {
      case 0:
        pos = PCD_1_OFF;
        break;
      case 1:
        pos = PCD_2_OFF;
        break;
      case 2:
        pos = PCD_3_OFF;
        break;
      default:
        assert(0);
        b_status = ER_SYSERR;
        break;
      }
      /* �趨��ȡλ�� */
      if (isio_seek(pfile, pos, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* PCDͼ��߶�һ����ż�� */
      assert((h % 2) == 0);
      /* ���ж������� */
      for (i = 0; i < h; i += 2) {
        uchar* r, *g, *b;
        /* ��ȡ���е�YCbCr���ݣ�CbCr���ݺ��ã� */
        if (isio_read((void*)y1, w, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        if (isio_read((void*)y2, w, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        if (isio_read((void*)cbcr, w, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ȷ������д��λ�� */
        p = pbits + (i + 0) * linesize;
        /* ת��YUV������RGB���� */
        for (j = 0; j < w; j++) {
          b = p + j * 3 + 0;
          g = p + j * 3 + 1;
          r = p + j * 3 + 2;
          YUV2RGB(y1[j], cbcr[j / 2], cbcr[(w / 2) + (j / 2)], r, g, b);
        }
        /* ָ����һ�е�ַ */
        p = pbits + (i + 1) * linesize;
        /* ת��YUV������RGB���� */
        for (j = 0; j < w; j++) {
          b = p + j * 3 + 0;
          g = p + j * 3 + 1;
          r = p + j * 3 + 2;
          YUV2RGB(y2[j], cbcr[j / 2], cbcr[(w / 2) + (j / 2)], r, g, b);
        }
      }
    }
    while (0);
    
      if (y1) {
        isirw_free(y1);
      }
      if (y2) {
        isirw_free(y2);
      }
      if (cbcr) {
        isirw_free(cbcr);
      }
  return b_status;
}
/* ���벢��ʼ����ͼ��ڵ� */
static SUBIMGBLOCK* _allsub(int num, INFOSTR* pinfo_str)
{
  SUBIMGBLOCK* subimg_tmp;
  int i, linesize;
  /* ����ڵ� */
  if ((subimg_tmp = _alloc_SUBIMGBLOCK(pinfo_str)) == 0) {
    return 0;
  }
  subimg_tmp->number = 1; /* ��ͼ����ʼ���Ϊ 1 */
  subimg_tmp->colorkey = -1; /* ͸��ɫֵ��Ϊ�� */
  subimg_tmp->number = num; /* ��ͼ������� 1 ��ʼ */
  //subimg_tmp->parents = pinfo_str; /* ��ͼ���ַ */
  if (num == 1) {
    pinfo_str->psubimg = subimg_tmp;
    pinfo_str->imgnumbers = 2;
    subimg_tmp->width = PCD_2_W;
    subimg_tmp->height = PCD_2_H;
  }
  else if (num == 2) {
    pinfo_str->imgnumbers = 3;
    subimg_tmp->width = PCD_3_W;
    subimg_tmp->height = PCD_3_H;
  }
  else {
    assert(0); /* ֻ�ܴ���ڶ����͵�����ͼ�� */
    _free_SUBIMGBLOCK(subimg_tmp);
    return 0;
  }
  subimg_tmp->order = pinfo_str->order; /* ��ͬ����ͼ���� */
  subimg_tmp->bitcount = PCD_DEF_BITCOUNT;
  SETMASK_24(subimg_tmp);
  subimg_tmp->left = subimg_tmp->top = 0;
  subimg_tmp->dowith = subimg_tmp->userinput = subimg_tmp->time = 0;
  subimg_tmp->colorkey = -1; /* ��Ϊ�޹ؼ�ɫ */
  subimg_tmp->pal_count = 0;
  linesize = _calcu_scanline_size(subimg_tmp->width, subimg_tmp->bitcount);
  assert(subimg_tmp->p_bit_data == (uchar*)0);
  /* ����Ŀ��ͼ���ڴ�飨+4 �� β������4�ֽڻ������� */
  subimg_tmp->p_bit_data = (uchar*)isirw_malloc(linesize * subimg_tmp->height + 4);
  if (!subimg_tmp->p_bit_data) {
    _free_SUBIMGBLOCK(subimg_tmp);
    return 0;
  }
  assert(subimg_tmp->pp_line_addr == NULL);
  /* �������׵�ַ���� */
  subimg_tmp->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * subimg_tmp->height);
  if (!subimg_tmp->pp_line_addr) {
    _free_SUBIMGBLOCK(subimg_tmp);
    return 0;
  }
  /* ��ʼ�����׵�ַ���飨���� */
  if (subimg_tmp->order == 0) { /* ���� */
    for (i = 0; i < (int)(subimg_tmp->height); i++) {
      subimg_tmp->pp_line_addr[i] = (subimg_tmp->p_bit_data + (i * linesize));
    }
  }
  else { /* ���� */
    for (i = 0; i < (int)(subimg_tmp->height); i++) {
      subimg_tmp->pp_line_addr[i] = (subimg_tmp->p_bit_data + ((subimg_tmp->height - i - 1) * linesize));
    }
  }
  return subimg_tmp;
}

