/********************************************************************
 emf.c
 ���ļ���;�� EMFͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ� �ɶ�ȡ��ǿ��Ԫ�ļ���EMF��
 ���湦�ܣ� ��֧�ֱ��湦��
 ���ļ���д�ˣ� YZ yzfree##yeah.net
 swstudio swstudio##sohu.com
 ���ļ��汾�� 30401
 ����޸��ڣ� 2003-4-1
 ----------------------------------------------------------------
 ������ʷ��
 2003-4 ֧��ͼ���д�����ṩ�����ػ���������
 2002-8 ����ӿ�������2.2�������ڴ�������
 2002-6 �ڶ������������°棩
 2001-1 ��������ע����Ϣ
 2000-7 �����ദBUG������ǿ��ģ����ݴ���
 2000-6 ��һ���汾����
********************************************************************/

/* �ڲ����ֺ��� */
static int _verify_file(ISFILE* pfile);
static int _get_info(ISFILE* pfile, INFOSTR* pinfo_str);
static int _load_enh_metafile(ISFILE* pfile, HENHMETAFILE* phout);
static int _enh_meta_to_raster(HENHMETAFILE hemf, INFOSTR* pinfo_str);

/* ��ȡͼ����Ϣ */
EXERESULT emf_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
# ifdef WIN32
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */

    do {
      /* ��ȡ�ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if ((b_status = _verify_file(pfile)) != ER_SUCCESS) {
        break;
      }
      if ((b_status = _get_info(pfile, pinfo_str)) != ER_SUCCESS) {
        break;
      }
      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    while (0);
  
# else
  b_status = ER_NOTSUPPORT;
# endif /* WIN32 */
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT emf_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
# ifdef WIN32
  enum EXERESULT b_status = ER_SUCCESS;
  HENHMETAFILE hemf;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */
  do {
    do {
      /* ���ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ��֤emf�ļ�����Ч�� */
      if ((b_status = _verify_file(pfile)) != ER_SUCCESS) {
        break;
      }
      /* ����ǿյ����ݰ������Ȼ�ȡͼ���Ҫ��Ϣ���ɹ����ٶ�ȡͼ�� */
      if (pinfo_str->data_state == 0) {
        b_status = _get_info(pfile, pinfo_str);
        if (b_status != ER_SUCCESS) {
          break;
        }
        pinfo_str->data_state = 1;
      }
      assert(pinfo_str->data_state == 1);
      /* ��ȡemf���� */
      if ((b_status = _load_enh_metafile(pfile, &hemf)) != ER_SUCCESS) {
        break;
      }
      assert(hemf);
      /* ת�������ݰ� */
      if ((b_status = _enh_meta_to_raster(hemf, pinfo_str)) != ER_SUCCESS) {
        break;
      }
      /* �ص�֪ͨ */
       
      /* һ���н��ȵ��� */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_LOAD)) 
      /* ��ͼ����Ƚ��� */
       
      pinfo_str->data_state = 2;
    }
    while (0);
    {
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
            pinfo_str->data_state = 1; /* �Զ����� */
          }
          
        }
      }
      //LeaveCriticalSection(&emf_load_img_critical);
    }
  }
# else
  b_status = ER_NOTSUPPORT;
# endif /* WIN32 */
  return b_status;
}
/* ����ͼ�� */
EXERESULT emf_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  return ER_NOTSUPPORT;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */
/* ����ɨ���гߴ�(���ֽڶ���) */
int _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}
static int _verify_file(ISFILE* pfile)
{
  enum EXERESULT b_status = ER_SUCCESS;
  unsigned int filesize;
  ENHMETAHEADER emh;
  do {
    do {
      /* ������֤�ļ���С����Ӧ��С��emf�ļ�ͷ�Ĵ�С */
      if (isio_seek(pfile, 0, SEEK_END) == -1
      || (filesize = isio_tell(pfile)) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (filesize < sizeof(emh)) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* ��ȡemf�ļ�ͷ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1
      || isio_read(&emh, sizeof(emh), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ȷ�����ļ�ͷ��¼,����֤�ļ����� */
      if (emh.iType != EMR_HEADER
          || emh.dSignature != ENHMETA_SIGNATURE
      || emh.nBytes > filesize) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* ��֤�汾�� */
      if (emh.nVersion != 0x10000) {
        b_status = ER_NONIMAGE;
        break;
      }
    }
    while (0);
    {
    }
  }
  return b_status;
}
static int _get_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  enum EXERESULT b_status = ER_SUCCESS;
  HDC hdc = NULL;
  assert(pfile);
  assert(pinfo_str);
  do {
    do {
      hdc = GetDC(GetDesktopWindow());
      if (hdc == NULL) {
        b_status = ER_SYSERR;
        break;
      }
      /* ����ͼ����߶� */
      {
        int width, height, t;
        ENHMETAHEADER emh;
        if (isio_seek(pfile, 0, SEEK_SET) == -1
        || isio_read(&emh, sizeof(emh), 1, pfile) != 1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ����� 0.01 mm Ϊ��λ��ͼ��ߴ� */
        width = emh.rclFrame.right - emh.rclFrame.left;
        height = emh.rclFrame.bottom - emh.rclFrame.top;
        /* ת��Ϊ������Ϊ��λ�ĳߴ� */
        t = GetDeviceCaps(hdc, HORZSIZE) * 100;
        width = (width * GetDeviceCaps(hdc, HORZRES) + t / 2) / t;
        t = GetDeviceCaps(hdc, VERTSIZE) * 100;
        height = (height * GetDeviceCaps(hdc, VERTRES) + t / 2) / t;
        pinfo_str->width = width;
        pinfo_str->height = height;
      }
      /* ����ͼ�����͡��洢��ʽ������ѹ����ʽ */
      pinfo_str->imgtype = IMT_VECTORSTATIC;
      pinfo_str->imgformat = IMF_EMF;
      pinfo_str->compression = ICS_GDIRECORD;
      /* ����ͼ��Ϊ������� */
      pinfo_str->order = 1;
      /* ����λ��ȣ�ֱ���趨Ϊ24bit */
      pinfo_str->bitcount = 24;
      /* ���ø���ɫ�������� */
      pinfo_str->r_mask = 0xff0000;
      pinfo_str->g_mask = 0xff00;
      pinfo_str->b_mask = 0xff;
      pinfo_str->a_mask = 0x0;
    }
    while (0);
    {
      if (hdc != NULL) {
        ReleaseDC(GetDesktopWindow(), hdc);
      }
    }
  }
  return b_status;
}
static int _load_enh_metafile(ISFILE* pfile, HENHMETAFILE* phout)
{
  enum EXERESULT b_status = ER_SUCCESS;
  ENHMETAHEADER emh;
  uchar* embits = NULL;
  assert(pfile);
  do {
    do {
      /* ��ȡemf�ļ�ͷ������ļ���С */
      if (isio_seek(pfile, 0, SEEK_SET) == -1
      || isio_read(&emh, sizeof(emh), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ��ȡ�ļ����ݣ�����emf handle */
      embits = (uchar*)isirw_malloc(emh.nBytes);
      if (embits == NULL) {
        b_status = ER_MEMORYERR;
        break;
      }
      if (isio_seek(pfile, 0, SEEK_SET) == -1
      || isio_read(embits, 1, emh.nBytes, pfile) != emh.nBytes) {
        b_status = ER_FILERWERR;
        break;
      }
      *phout = SetEnhMetaFileBits(emh.nBytes, embits);
      if (*phout == NULL) {
        b_status = ER_SYSERR;
        break;
      }
    }
    while (0);
    {
      if (embits != NULL) {
        isirw_free(embits);
      }
      if (b_status != ER_SUCCESS || 1) {
        *phout = NULL;
      }
    }
  }
  return b_status;
}
static int _enh_meta_to_raster(HENHMETAFILE hemf, INFOSTR* pinfo_str)
{
  enum EXERESULT b_status = ER_SUCCESS;
  uchar* dib_bits = NULL;
  HDC hdc = NULL, hComDC = NULL;
  HBITMAP bmp = NULL, oldbmp = NULL;
  BITMAPINFO bmi;
  RECT rect;
  int linesize;
  unsigned i;
  uchar** ppt;
  assert(hemf);
  assert(pinfo_str);
  do {
    do {
      /* ������������ݵ�DC */
      if ((hdc = GetDC(GetDesktopWindow())) == NULL) {
        b_status = ER_SYSERR;
        break;
      }
      memset(&bmi, 0, sizeof(bmi));
      bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
      bmi.bmiHeader.biWidth = pinfo_str->width;
      bmi.bmiHeader.biHeight = pinfo_str->height;
      bmi.bmiHeader.biPlanes = 1;
      bmi.bmiHeader.biBitCount = (WORD)pinfo_str->bitcount;
      bmi.bmiHeader.biCompression = BI_RGB;
      bmi.bmiHeader.biXPelsPerMeter = GetDeviceCaps(hdc, HORZRES) * 1000 / GetDeviceCaps(hdc, HORZSIZE);
      bmi.bmiHeader.biYPelsPerMeter = GetDeviceCaps(hdc, VERTRES) * 1000 / GetDeviceCaps(hdc, VERTSIZE);
      /* ����λͼ��� */
      bmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &dib_bits, NULL, 0);
      if (bmp == NULL) {
        b_status = ER_SYSERR;
        break;
      }
      /* ��������DC */
      if ((hComDC = CreateCompatibleDC(hdc)) == NULL) {
        b_status = ER_SYSERR;
        break;
      }
      oldbmp = SelectObject(hComDC, bmp);
      /* ��ͼ�󱳾����Ϊ��ɫ */
      rect.left = rect.top = 0;
      rect.right = pinfo_str->width;
      rect.bottom = pinfo_str->height;
      FillRect(hComDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
      /* �����DC����emf���� */
      PlayEnhMetaFile(hComDC, hemf, &rect);
      /* ���õõ���λͼ����������ݰ������ */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      if ((pinfo_str->p_bit_data = (uchar*)isirw_orgpix_malloc(linesize * pinfo_str->height + 4)) == NULL
      || (pinfo_str->pp_line_addr = isirw_malloc(sizeof(uchar*)*pinfo_str->height)) == NULL) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* ���������ݿ��������ݰ� */
      memcpy(pinfo_str->p_bit_data, dib_bits, linesize * pinfo_str->height);
      /* ��д���׵�ַ���� */
      ppt = (uchar**)pinfo_str->pp_line_addr;
      for (i = 0; i < pinfo_str->height; i++) {
        ppt[i] = (void*)(pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
      }
      /* ��д���ݰ��������� */
      pinfo_str->pal_count = 0;
      pinfo_str->imgnumbers = 1;
      pinfo_str->psubimg = NULL;
    }
    while (0);
    {
      if (hdc != NULL) {
        ReleaseDC(GetDesktopWindow(), hdc);
      }
      if (hComDC != NULL) {
        DeleteDC(hComDC);
      }
      if (bmp != NULL) {
        DeleteObject(bmp);
      }
    }
  }
  return b_status;
}

