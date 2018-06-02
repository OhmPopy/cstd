/********************************************************************
 pcx.c
 ���ļ���;�� PCXͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ�1��4��8��24λͼ��
 ���湦�ܣ�1��4��8��24λͼ��
 ���ļ���д�ˣ� freedvlp feelfree##263.net���������д�ˣ�
 ChK ChK##163.net��д�����д�ˣ�
 YZ ���İ湤����
 (�����޸�)
 ���ļ��汾�� 30813
 ����޸��ڣ� 2003-8-13
 ----------------------------------------------------------------
 ������ʷ��
 2003-8 ����4λͼδ����1λ���Ĵ���()
 2003-7 ���ӿ������ݴ�����루Ehomsoft webmaster##ehom.net��
 2003-4 ֧��ͼ���д�����ṩ�����ػ���������
 2002-8 ����ӿ�������2.2�������ڴ�������
 2001-8 �����°���롣����չ�˸�ģ��ı��湦�ܣ�
 ��ǿ��ģ���ݴ��ԣ������ģ��Ķ�д�ٶȡ�
 2001-4 �����ͼ��ı��湦�ܣ�24λ��
 2000-8 ����˶�ȡͼ�񲿷ֵ�ȫ������
 2000-6 ��һ�����԰淢��
********************************************************************/


/* �ļ��б��־ */
#define PCX_FLAG            0xa

/* PCX��ɫ����ṹ */
typedef struct _tag_pcx_palette_item {
  uchar red;
  uchar green;
  uchar blue;
} PCX_PALITEM, *LPPCX_PALITEM;


/* PCX��Ϣ�ṹ */
typedef struct _tag_pcx_header {
  uchar  byManufacturer;
  uchar  byVersion;
  uchar  byEncoding;
  uchar  byBits;
  ushort wLeft;
  ushort wTop;
  ushort wRight;
  ushort wBottom;
  ushort wXResolution;
  ushort wYResolution;
  PCX_PALITEM    ptPalette[16];
  uchar  byReserved;
  uchar  byPlanes;
  ushort wLineBytes;
  ushort wPaletteType;
  ushort wScrWidth;
  ushort wScrDepth;
  uchar  byFiller[54];
}  PCX_HEADER;

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */
/* ����һ��PCX_RLE���ݣ������뵽ָ���Ļ������� */
uchar* _pcx_read_line(char* p_buf, char* p_sou, int linesize, char* p_base, int data_size)
{
  int cur_offset = 0;
  uchar count, conv;
  while (cur_offset < linesize) {
    count = (uchar) * p_sou++;
    if ((count & 0xc0) == 0xc0) { /* ����ֵ */
      count &= 0x3f;
      count = MIN(count, (linesize - cur_offset)); /* �ݴ��� */
      conv = (uchar) * p_sou++; /* ���� */
      while (count--) {
        p_buf[cur_offset++] = conv; /* �ظ�д������ */
      }
    }
    else { /* ���� */
      p_buf[cur_offset++] = count;
    }
  }
  return (uchar*)p_sou; /* ������һ�еĶ�ȡ��ַ������������ݴ�����0 */
}
/* дһ�����ݵ�PCX_RLE������ */
int _write_line(char* p_buf, char* p_sou, int linesize)
{
  int m = 0;
  int commsize = 0;
  uchar v, n;
  while (m < linesize) {
    v = p_sou[m];
    /* ȡ����������ͬ���ֽ���n */
    n = 1;
    while (((m + n) < linesize) && (n < 63) && (v == (uchar)(p_sou[m + n]))) {
      n++;
    }
    if (n == 1) { /* δ��?��?*/
      if (v < 0xc0) {
        p_buf[commsize++] = v;
      }
      else {
        p_buf[commsize++] = (char)0xc1;
        p_buf[commsize++] = v;
      }
    }
    else { /* ѹ�� */
      p_buf[commsize++] = (char)(n + 0xc0);
      p_buf[commsize++] = v;
    }
    m += n;
  }
  return commsize; /* ѹ����������гߴ� */
}
/* ��ȡ��ɫ������ */
void _get_palette(INFOSTR* pinfo_str, PCX_HEADER* p_header, ISFILE* pfile)
{
  int i;
  PCX_PALITEM tmp_palbuf[256];
#if 0
  PALETTEENTRY palcolor[256];
  HDC hdc;
#endif
  assert(pfile);
  if (pinfo_str->bitcount <= 8) {
    pinfo_str->pal_count = 1UL << pinfo_str->bitcount;
    if (p_header->wPaletteType <= 1) { /* ��ɫ��ɫ��YZ: 0������Ϊ��ɫ�� */
      if (p_header->byVersion == 3) { /* ʹ��ϵͳ��ɫ�� */
#if 0
        hdc = GetDC((HWND)0);
        GetSystemPaletteEntries(hdc, 0, pinfo_str->pal_count, (LPPALETTEENTRY)palcolor);
        ReleaseDC((HWND)0, hdc);
        for (i = 0; i < (int)pinfo_str->pal_count; i++) {
          ((PALITEM*)(&pinfo_str->palette[i]))->blue = palcolor[i].peBlue;
          ((PALITEM*)(&pinfo_str->palette[i]))->green = palcolor[i].peGreen;
          ((PALITEM*)(&pinfo_str->palette[i]))->red = palcolor[i].peRed;
          ((PALITEM*)(&pinfo_str->palette[i]))->reserved = 0;
        }
#else
        /* ȱʡΪ�Ҷȵ�ɫ�壬�ɸ���ΪLinux GUIϵͳ��ȱʡ��ɫ�� */
        switch (pinfo_str->bitcount) {
        case 1:
          pinfo_str->palette[0] = 0;
          pinfo_str->palette[1] = 0xffffff;
          break;
        case 2:
          pinfo_str->palette[0] = 0;
          pinfo_str->palette[1] = 0x555555;
          pinfo_str->palette[2] = 0xaaaaaa;
          pinfo_str->palette[3] = 0xffffff;
          break;
        case 3:
          pinfo_str->palette[0] = 0;
          pinfo_str->palette[1] = 0x242424;
          pinfo_str->palette[2] = 0x484848;
          pinfo_str->palette[3] = 0x6c6c6c;
          pinfo_str->palette[4] = 0x909090;
          pinfo_str->palette[5] = 0xb4b4b4;
          pinfo_str->palette[6] = 0xd8d8d8;
          pinfo_str->palette[7] = 0xffffff;
          break;
        case 4:
          pinfo_str->palette[0] = 0;
          pinfo_str->palette[1] = 0x111111;
          pinfo_str->palette[2] = 0x222222;
          pinfo_str->palette[3] = 0x333333;
          pinfo_str->palette[4] = 0x444444;
          pinfo_str->palette[5] = 0x555555;
          pinfo_str->palette[6] = 0x666666;
          pinfo_str->palette[7] = 0x777777;
          pinfo_str->palette[8] = 0x888888;
          pinfo_str->palette[9] = 0x999999;
          pinfo_str->palette[10] = 0xaaaaaa;
          pinfo_str->palette[11] = 0xbbbbbb;
          pinfo_str->palette[12] = 0xcccccc;
          pinfo_str->palette[13] = 0xdddddd;
          pinfo_str->palette[14] = 0xeeeeee;
          pinfo_str->palette[15] = 0xffffff;
          break;
        case 8:
          for (i = 0; i < (int)256; i++) {
            pinfo_str->palette[i] = i | i << 8 | i << 16;
          }
          break;
        }
#endif
      }
      else { /* �Դ���ɫ�� */
        if (pinfo_str->bitcount == 8) {
          /* ��ȡ256ɫ��ɫ�����ݣ���β����*/
          isio_seek(pfile, -256L * 3, SEEK_END);
          isio_read((void*)tmp_palbuf, 256 * 3, 1, pfile);
          /* ���Ƶ�ɫ�����ݣ�8λ�� */
          for (i = 0; i < (int)pinfo_str->pal_count; i++) {
            ((PALITEM*)(&pinfo_str->palette[i]))->blue = tmp_palbuf[i].blue;
            ((PALITEM*)(&pinfo_str->palette[i]))->green = tmp_palbuf[i].green;
            ((PALITEM*)(&pinfo_str->palette[i]))->red = tmp_palbuf[i].red;
            ((PALITEM*)(&pinfo_str->palette[i]))->reserved = 0;
          }
        }
        else {
          /* ���Ƶ�ɫ�����ݣ�1��2��3��4λ�� */
          for (i = 0; i < (int)pinfo_str->pal_count; i++) {
            ((PALITEM*)(&pinfo_str->palette[i]))->blue = p_header->ptPalette[i].blue;
            ((PALITEM*)(&pinfo_str->palette[i]))->green = p_header->ptPalette[i].green;
            ((PALITEM*)(&pinfo_str->palette[i]))->red = p_header->ptPalette[i].red;
            ((PALITEM*)(&pinfo_str->palette[i]))->reserved = 0;
          }
        }
      }
    }
    else { /* �Ҷȣ�2�� */
      switch (pinfo_str->bitcount) {
      case 1:
        pinfo_str->palette[0] = 0;
        pinfo_str->palette[1] = 0xffffff;
        break;
      case 2:
        pinfo_str->palette[0] = 0;
        pinfo_str->palette[1] = 0x555555;
        pinfo_str->palette[2] = 0xaaaaaa;
        pinfo_str->palette[3] = 0xffffff;
        break;
      case 3:
        pinfo_str->palette[0] = 0;
        pinfo_str->palette[1] = 0x242424;
        pinfo_str->palette[2] = 0x484848;
        pinfo_str->palette[3] = 0x6c6c6c;
        pinfo_str->palette[4] = 0x909090;
        pinfo_str->palette[5] = 0xb4b4b4;
        pinfo_str->palette[6] = 0xd8d8d8;
        pinfo_str->palette[7] = 0xffffff;
        break;
      case 4:
        pinfo_str->palette[0] = 0;
        pinfo_str->palette[1] = 0x111111;
        pinfo_str->palette[2] = 0x222222;
        pinfo_str->palette[3] = 0x333333;
        pinfo_str->palette[4] = 0x444444;
        pinfo_str->palette[5] = 0x555555;
        pinfo_str->palette[6] = 0x666666;
        pinfo_str->palette[7] = 0x777777;
        pinfo_str->palette[8] = 0x888888;
        pinfo_str->palette[9] = 0x999999;
        pinfo_str->palette[10] = 0xaaaaaa;
        pinfo_str->palette[11] = 0xbbbbbb;
        pinfo_str->palette[12] = 0xcccccc;
        pinfo_str->palette[13] = 0xdddddd;
        pinfo_str->palette[14] = 0xeeeeee;
        pinfo_str->palette[15] = 0xffffff;
        break;
      case 8:
        for (i = 0; i < (int)256; i++) {
          pinfo_str->palette[i] = i | i << 8 | i << 16;
        }
        break;
      }
    }
  }
  else {
    pinfo_str->pal_count = 0; /* �޵�ɫ������ */
  }
}
/* �жϴ����ͼ���Ƿ���Ա����� */
int _pcx_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = 1UL | (1UL << (4 - 1)) | (1UL << (8 - 1)) | (1UL << (24 - 1)) };
  if (!(bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1; /* ��֧�ֵ�λ��ͼ�� */
  }
  assert(pinfo_str->imgnumbers);
  if (pinfo_str->imgnumbers != 1) {
    return -2; /* ͼ���������ȷ */
  }
  return 0;
}


/* ��ȡͼ����Ϣ */
EXERESULT pcx_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  PCX_HEADER phr;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */
  
    do {
      /* ��ȡ�ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&phr, sizeof(PCX_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ����ļ���־ */
      if (phr.byManufacturer != (uchar)PCX_FLAG) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ������Ч�Լ��� */
      if (((phr.wRight - phr.wLeft) == 0) || ((phr.wBottom - phr.wTop) == 0)) {
        b_status = ER_NONIMAGE;
        break;
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_PCX; /* ͼ���ļ���ʽ����׺���� */
      pinfo_str->compression = ICS_PCXRLE; /* ͼ���ѹ����ʽ */
      /* ��дͼ����Ϣ */
      pinfo_str->width = (ulong)((phr.wRight - phr.wLeft) + 1);
      pinfo_str->height = (ulong)((phr.wBottom - phr.wTop) + 1);
      pinfo_str->order = 0;
      pinfo_str->bitcount = phr.byBits * phr.byPlanes;
      /* ��д�������� */
      if (pinfo_str->bitcount > 8) { /* 24λ */
        pinfo_str->b_mask = 0xff;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->a_mask = 0x0;
      }
      else {
        pinfo_str->b_mask = 0x0;
        pinfo_str->g_mask = 0x0;
        pinfo_str->r_mask = 0x0;
        pinfo_str->a_mask = 0x0;
      }
      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    while (0);
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT pcx_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  PCX_HEADER phr;
  int i, j, k, linesize, bufsize;
  ulong data_len;
  uchar* pbit = NULL;
  uchar* ptmp = NULL;
  uchar* plinebuf = NULL;
  uchar* psou, *pdesline;
  uchar* pla3, *pla2, *pla1, *pla0;
  uchar pb3, pb2, pb1, pb0, tbuf, tofs;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ?��?*/

    do {
      /* ��ȡ�ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&phr, sizeof(PCX_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ����ļ���־ */
      if (phr.byManufacturer != (uchar)PCX_FLAG) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ������Ч�Լ��� */
      if (((phr.wRight - phr.wLeft) == 0) || ((phr.wBottom - phr.wTop) == 0)) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
        pinfo_str->imgformat = IMF_PCX; /* ͼ���ļ���ʽ����׺���� */
        pinfo_str->compression = ICS_PCXRLE; /* ͼ���ѹ����ʽ */
        /* ��дͼ����Ϣ */
        pinfo_str->width = (ulong)((phr.wRight - phr.wLeft) + 1);
        pinfo_str->height = (ulong)((phr.wBottom - phr.wTop) + 1);
        pinfo_str->order = 0;
        pinfo_str->bitcount = phr.byBits * phr.byPlanes;
        /* ��д�������� */
        if (pinfo_str->bitcount > 8) { /* 24λ */
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0x0;
        }
        else {
          pinfo_str->b_mask = 0x0;
          pinfo_str->g_mask = 0x0;
          pinfo_str->r_mask = 0x0;
          pinfo_str->a_mask = 0x0;
        }
        pinfo_str->data_state = 1;
      }
      /* ���õ�ɫ������ */
      _get_palette(pinfo_str, &phr, pfile);
      /* ȡ��ͼ�����ݳ��� */
      data_len = isio_length(pfile) - sizeof(PCX_HEADER);
      if ((pinfo_str->bitcount == 8) && (phr.wPaletteType == 1)) {
        data_len -= 256 * 3 + 1; /* +1 : 0xc0 */
      }
      /* ȡ��ɨ���гߴ� */
      linesize = DIBSCANLINE_WIDTHBYTES(pinfo_str->width * pinfo_str->bitcount);
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
      /* ��ʼ�����׵�ַ���飨PCXͼ��Ϊ���� */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
      }
      bufsize = (int)phr.wLineBytes * phr.byPlanes;
      /* �������ڴ����ʱ��ѹ���ݵĻ����� */
      plinebuf = (uchar*)isirw_malloc(bufsize);
      if (plinebuf == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* ��ȡλ���ݵ��ڴ� */
      if (isio_seek(pfile, sizeof(PCX_HEADER), SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ������ʱ�ڴ�飬���ڴ������PCXͼ������ */
      ptmp = (uchar*)isirw_malloc(data_len);
      if (ptmp == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* ��ԭʼͼ������ */
      if (isio_read((void*)ptmp, data_len, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      psou = ptmp;
      pbit = pinfo_str->p_bit_data;
      /* ���ж��� */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        if ((psou = _pcx_read_line((char*)plinebuf, (char*)psou, bufsize, (char*)ptmp, data_len)) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* Ŀ�껺������ַ */
        pdesline = (uchar*)pinfo_str->pp_line_addr[i];
        switch (pinfo_str->bitcount) {
        case 1:
          /* 1λ��1��� */
          memcpy((void*)pdesline, (const void*)plinebuf, bufsize);
          break;
        case 2:
        case 3:
          /* ���޸ġ���δ�ҵ���ص�����������ϣ�����Ϊ1����壩*/
          memcpy((void*)pdesline, (const void*)plinebuf, bufsize);
          break;
        case 4:
          /* 1λ��4��� */
          /* 1λ��崦�� add by yuhe */ /* who is yuhe? */ /* ��������������С��д�������ˣ�����??*/
          if (phr.byPlanes == 1) {
            memcpy((void*)pdesline, (const void*)plinebuf, bufsize);
            break;
          }
          pla0 = (uchar*)(plinebuf + ((int)phr.wLineBytes * 0));
          pla1 = (uchar*)(plinebuf + ((int)phr.wLineBytes * 1));
          pla2 = (uchar*)(plinebuf + ((int)phr.wLineBytes * 2));
          pla3 = (uchar*)(plinebuf + ((int)phr.wLineBytes * 3));
          for (k = 0; k < (int)pinfo_str->width; k += 8) {
            pb0 = *pla0++;
            pb1 = *pla1++;
            pb2 = *pla2++;
            pb3 = *pla3++;
            /* ��λΪ��ߵ����� */
            for (j = 7; (j >= 0) && ((k + (7 - j)) < (int)pinfo_str->width); j--) {
              tofs = 1 << j;
              tbuf = 0;
              tbuf |= (pb0 & tofs) ? 0x1 : 0;
              tbuf |= (pb1 & tofs) ? 0x2 : 0;
              tbuf |= (pb2 & tofs) ? 0x4 : 0;
              tbuf |= (pb3 & tofs) ? 0x8 : 0;
              if (j % 2) {
                tbuf <<= 4;
                *pdesline = tbuf;
              }
              else {
                *pdesline++ |= tbuf;
              }
            }
          }
          break;
        case 8:
          /* 8λͼ������ֱ�ӿ��� */
          memcpy((void*)pdesline, (const void*)plinebuf, bufsize);
          break;
        case 24:
          /* 8λ��3��� */
          pla0 = (uchar*)(plinebuf + ((int)phr.wLineBytes * 0));
          pla1 = (uchar*)(plinebuf + ((int)phr.wLineBytes * 1));
          pla2 = (uchar*)(plinebuf + ((int)phr.wLineBytes * 2));
          /* BGR˳�� */
          for (k = 0; k < (int)pinfo_str->width; k++) {
            *pdesline++ = *pla2++; /* B */
            *pdesline++ = *pla1++; /* G */
            *pdesline++ = *pla0++; /* R */
          }
          break;
        default:
          b_status = ER_BADIMAGE;
          break;
        }
      }
      pinfo_str->data_state = 2;
    }
    while (0);
      if (plinebuf) {
        isirw_free(plinebuf);
      }
      if (ptmp) {
        isirw_free(ptmp);
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
EXERESULT pcx_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  PCX_HEADER phr;
  int i, j, k, expsize, commsize;
  uchar* psou, *pdes;
  uchar* pexpend = (uchar*)0;
  uchar* pexpend2 = (uchar*)0;
  uchar* pdesbuf = (uchar*)0;
  LPPCX_PALITEM p8pal = (LPPCX_PALITEM)0;
  uchar* pla3, *pla2, *pla1, *pla0;
  uchar pb3, pb2, pb1, pb0, tbuf;
  int cnv_count;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);
  
  do {
    /* �ж��Ƿ�����֧�ֵ�ͼ������ */
    if (_pcx_is_valid_img(pinfo_str) != 0) {
      b_status = ER_NSIMGFOR;
      break;
    }
    /* ����PCXͼ����Ϣ�ṹ */
    phr.byManufacturer = (uchar)PCX_FLAG;
    phr.byVersion = 5;
    phr.byEncoding = 1;
    phr.wLeft = 0;
    phr.wTop = 0;
    assert(pinfo_str->width);
    assert(pinfo_str->height);
    phr.wRight = (ushort)(pinfo_str->width - 1);
    phr.wBottom = (ushort)(pinfo_str->height - 1);
    phr.byReserved = 0;
    /* ȱʡ����ֵ�����޸� */
    phr.wXResolution = 300; /* DPI */
    phr.wYResolution = 300;
    phr.wScrWidth = 800; /* PIXEL */
    phr.wScrDepth = 600;
    memset((void*)phr.byFiller, 0, 54);
    switch (pinfo_str->bitcount) {
    case 1:
      phr.byBits = 1;
      phr.byPlanes = 1;
      phr.wLineBytes = (ushort)(((pinfo_str->width + 15) / 16) * 2);
      phr.wPaletteType = 1; /* ��ɫ */
      for (i = 0; i < 2; i++) {
        phr.ptPalette[i].blue = ((PALITEM*)(&pinfo_str->palette[i]))->blue;
        phr.ptPalette[i].green = ((PALITEM*)(&pinfo_str->palette[i]))->green;
        phr.ptPalette[i].red = ((PALITEM*)(&pinfo_str->palette[i]))->red;
      }
      expsize = phr.wLineBytes * 1;
      pexpend = (uchar*)isirw_malloc(expsize);
      pexpend2 = (uchar*)0;
      pdesbuf = (uchar*)isirw_malloc(phr.wLineBytes * 1 * 2); /* �����������������ڴ� */
      if ((pexpend == 0) || (pdesbuf == 0)) {
        b_status = ER_MEMORYERR;
        break;
      }
      break;
    case 4:
      phr.byBits = 1;
      phr.byPlanes = 4;
      phr.wLineBytes = (ushort)(((pinfo_str->width + 15) / 16) * 2);
      phr.wPaletteType = 1; /* ��ɫ */
      for (i = 0; i < 16; i++) {
        phr.ptPalette[i].blue = ((PALITEM*)(&pinfo_str->palette[i]))->blue;
        phr.ptPalette[i].green = ((PALITEM*)(&pinfo_str->palette[i]))->green;
        phr.ptPalette[i].red = ((PALITEM*)(&pinfo_str->palette[i]))->red;
      }
      expsize = phr.wLineBytes * 4;
      pexpend = (uchar*)isirw_malloc(expsize);
      pexpend2 = (uchar*)isirw_malloc(DIBSCANLINE_WIDTHBYTES(pinfo_str->width * 4));
      pdesbuf = (uchar*)isirw_malloc(phr.wLineBytes * 4 * 2);
      if ((pexpend == 0) || (pdesbuf == 0) || (pexpend2 == 0)) {
        b_status = ER_MEMORYERR;
        break;
      }
      break;
    case 8:
      phr.byBits = 8;
      phr.byPlanes = 1;
      phr.wLineBytes = (ushort)(((pinfo_str->width + 1) / 2) * 2);
      phr.wPaletteType = 0; /* ��ɫ */
      expsize = phr.wLineBytes * 1;
      pexpend = (uchar*)isirw_malloc(expsize);
      pexpend2 = (uchar*)0;
      p8pal = (LPPCX_PALITEM)isirw_malloc(sizeof(PCX_PALITEM) * 256);
      pdesbuf = (uchar*)isirw_malloc(phr.wLineBytes * 1 * 2);
      if ((pexpend == 0) || (pdesbuf == 0) || (p8pal == 0)) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* ��ɫ�����ݽ���дͼ�����ݺ��??*/
      break;
    case 24:
      phr.byBits = 8;
      phr.byPlanes = 3;
      phr.wLineBytes = (ushort)(((pinfo_str->width + 1) / 2) * 2);
      phr.wPaletteType = 0; /* ��ɫ */
      expsize = phr.wLineBytes * 3;
      pexpend = (uchar*)isirw_malloc(expsize);
      pexpend2 = (uchar*)isirw_malloc(DIBSCANLINE_WIDTHBYTES(pinfo_str->width * 3 * 8));
      pdesbuf = (uchar*)isirw_malloc(phr.wLineBytes * 3 * 2);
      if ((pexpend == 0) || (pdesbuf == 0) || (pexpend2 == 0)) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* �޵�ɫ������ */
      break;
    default:
      assert(0);
      break;
    }
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* д����Ϣ�ṹ */
    if (isio_write((const void*)&phr, sizeof(PCX_HEADER), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* д��?������?*/
    for (i = 0; i < (int)(pinfo_str->height); i++) {
      psou = (uchar*)pinfo_str->pp_line_addr[i];
      pdes = pexpend;
      /* չ��Ϊ����ʽ */
      switch (pinfo_str->bitcount) {
      case 1:
        memcpy((void*)pdes, (const void*)psou, phr.wLineBytes);
        break;
      case 4:
        memcpy((void*)pexpend2, (const void*)psou, (int)((pinfo_str->width + 1) / 2));
        /* ��ʼ��չ�������� */
        memset((void*)pexpend, 0, phr.wLineBytes * 4);
        pla0 = (uchar*)(pexpend + (phr.wLineBytes * 0));
        pla1 = (uchar*)(pexpend + (phr.wLineBytes * 1));
        pla2 = (uchar*)(pexpend + (phr.wLineBytes * 2));
        pla3 = (uchar*)(pexpend + (phr.wLineBytes * 3));
        /* ��ͼ������չ��Ϊ4����� */
        for (k = 0, j = 7; k < (int)pinfo_str->width; k++, j--) {
          tbuf = *(uchar*)(pexpend2 + k / 2);
          if (k % 2) {
            tbuf &= 0xf;
          }
          else {
            tbuf >>= 4;
          }
          pb0 = ((tbuf & 0x1) >> 0) << j;
          pb1 = ((tbuf & 0x2) >> 1) << j;
          pb2 = ((tbuf & 0x4) >> 2) << j;
          pb3 = ((tbuf & 0x8) >> 3) << j;
          *pla0 |= pb0;
          *pla1 |= pb1;
          *pla2 |= pb2;
          *pla3 |= pb3;
          if (j == 0) {
            j = 8;
            pla0++;
            pla1++;
            pla2++;
            pla3++;
          }
        }
        break;
      case 8:
        /* 8λͼ��ֱ�ӿ��� */
        memcpy((void*)pdes, (const void*)psou, phr.wLineBytes);
        break;
      case 24:
        /* �ֽ���˳��ת */
        for (cnv_count = 0; cnv_count < (int)pinfo_str->width; cnv_count++) {
          *(uchar*)(pexpend2 + cnv_count * 3 + 2) = *(uchar*)(psou + cnv_count * 3 + 0);
          *(uchar*)(pexpend2 + cnv_count * 3 + 1) = *(uchar*)(psou + cnv_count * 3 + 1);
          *(uchar*)(pexpend2 + cnv_count * 3 + 0) = *(uchar*)(psou + cnv_count * 3 + 2);
        }
        /* ��ʼ��չ�������� */
        memset((void*)pexpend, 0, phr.wLineBytes * 3);
        pla0 = (uchar*)(pexpend + (phr.wLineBytes * 0)); /*R*/
        pla1 = (uchar*)(pexpend + (phr.wLineBytes * 1)); /*G*/
        pla2 = (uchar*)(pexpend + (phr.wLineBytes * 2)); /*B*/
        /* ��ͼ������չ��Ϊ3�����--RGB */
        for (k = 0; k < (int)pinfo_str->width; k++) {
          *pla0++ = *(uchar*)(pexpend2 + k * 3 + 0);
          *pla1++ = *(uchar*)(pexpend2 + k * 3 + 1);
          *pla2++ = *(uchar*)(pexpend2 + k * 3 + 2);
        }
        break;
      }
      /* ʹ��RLE�㷨��չ����������ݽ���ѹ�� */
      commsize = _write_line((char*)pdesbuf, (char*)pexpend, expsize);
      /* д��Ŀ���� */
      if (isio_write((const void*)pdesbuf, commsize, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* �����8λͼ����������������256ɫ��ɫ������ */
      if ((pinfo_str->bitcount == 8) && ((i + 1) == (int)(pinfo_str->height))) {
        pdesbuf[0] = (uchar)0xc; /* 0xc Ϊ��־�� */
        if (isio_write((const void*)pdesbuf, 1, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        for (k = 0; k < 256; k++) {
          p8pal[k].blue = ((PALITEM*)(&pinfo_str->palette[k]))->blue;
          p8pal[k].green = ((PALITEM*)(&pinfo_str->palette[k]))->green;
          p8pal[k].red = ((PALITEM*)(&pinfo_str->palette[k]))->red;
        }
        if (isio_write((const void*)p8pal, sizeof(PCX_PALITEM) * 256, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
      }
    }
    }
    while (0);
    
    if (p8pal) {
      isirw_free(p8pal);
    }
    if (pexpend2) {
      isirw_free(pexpend2);
    }
    if (pexpend) {
      isirw_free(pexpend);
    }
    if (pdesbuf) {
      isirw_free(pdesbuf);
    }
    return b_status;
}