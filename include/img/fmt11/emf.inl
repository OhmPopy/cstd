// EMFͼ���дģ��ʵ���ļ�
// ��ȡ���ܣ�  �ɶ�ȡ��ǿ��Ԫ�ļ���EMF��
// ���湦�ܣ�  ��֧�ֱ��湦��


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* ����ɨ���гߴ�(���ֽڶ���) */
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}



static int _verify_file(ISFILE* pfile)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  unsigned int  filesize;
  ENHMETAHEADER emh;

  __try {
    __try {
      /* ������֤�ļ���С����Ӧ��С��emf�ļ�ͷ�Ĵ�С */
      if (isio_seek(pfile, 0, SEEK_END) == -1
          || (filesize = isio_tell(pfile)) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (filesize < sizeof(emh)) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      /* ��ȡemf�ļ�ͷ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1
          || isio_read(&emh, sizeof(emh), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ȷ�����ļ�ͷ��¼,����֤�ļ����� */
      if (emh.iType != EMR_HEADER
          || emh.dSignature != ENHMETA_SIGNATURE
          || emh.nBytes > filesize) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      /* ��֤�汾�� */
      if (emh.nVersion != 0x10000) {
        b_status = ER_NONIMAGE;
        __leave;
      }
    }
    __finally {

    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}



static int _get_info(ISFILE* pfile, LPINFOSTR pinfo_str)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  HDC       hdc = NULL;

  assert(pfile);
  assert(pinfo_str);

  __try {
    __try {
      hdc = GetDC(GetDesktopWindow());

      if (hdc == NULL) {
        b_status = ER_SYSERR;
        __leave;
      }

      /* ����ͼ����߶� */
      {
        int width, height, t;
        ENHMETAHEADER emh;

        if (isio_seek(pfile, 0, SEEK_SET) == -1
            || isio_read(&emh, sizeof(emh), 1, pfile) != 1) {
          b_status = ER_FILERWERR;
          __leave;
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
      pinfo_str->imgtype     = IMT_VECTORSTATIC;
      pinfo_str->imgformat   = IMF_EMF;
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
    __finally {
      if (hdc != NULL) {
        ReleaseDC(GetDesktopWindow(), hdc);
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}

static int _load_enh_metafile(ISFILE* pfile, HENHMETAFILE* phout)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  ENHMETAHEADER emh;
  unsigned char*  embits = NULL;

  assert(pfile);

  __try {
    __try {
      /* ��ȡemf�ļ�ͷ������ļ���С */
      if (isio_seek(pfile, 0, SEEK_SET) == -1
          || isio_read(&emh, sizeof(emh), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ��ȡ�ļ����ݣ�����emf handle */
      embits = (unsigned char*)isirw_malloc(emh.nBytes);

      if (embits == NULL) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      if (isio_seek(pfile, 0, SEEK_SET) == -1
          || isio_read(embits, 1, emh.nBytes, pfile) != emh.nBytes) {
        b_status = ER_FILERWERR;
        __leave;
      }

      *phout = SetEnhMetaFileBits(emh.nBytes, embits);

      if (*phout == NULL) {
        b_status = ER_SYSERR;
        __leave;
      }

    }
    __finally {
      if (embits != NULL) {
        isirw_free(embits);
      }

      if (b_status != ER_SUCCESS || AbnormalTermination()) {
        *phout = NULL;
      }

    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}



static int _enh_meta_to_raster(HENHMETAFILE hemf, LPINFOSTR pinfo_str)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  unsigned char* dib_bits = NULL;
  HDC       hdc = NULL, hComDC = NULL;
  HBITMAP     bmp = NULL, oldbmp = NULL;
  BITMAPINFO    bmi;
  RECT      rect;
  int       linesize;
  unsigned    i;
  unsigned char** ppt;

  assert(hemf);
  assert(pinfo_str);

  __try {
    __try {
      /* ������������ݵ�DC */
      if ((hdc = GetDC(GetDesktopWindow())) == NULL) {
        b_status = ER_SYSERR;
        __leave;
      }

      memset(&bmi, 0, sizeof(bmi));
      bmi.bmiHeader.biSize          = sizeof(bmi.bmiHeader);
      bmi.bmiHeader.biWidth         = pinfo_str->width;
      bmi.bmiHeader.biHeight        = pinfo_str->height;
      bmi.bmiHeader.biPlanes        = 1;
      bmi.bmiHeader.biBitCount      = (WORD)pinfo_str->bitcount;
      bmi.bmiHeader.biCompression   = BI_RGB;
      bmi.bmiHeader.biXPelsPerMeter = GetDeviceCaps(hdc, HORZRES) * 1000 / GetDeviceCaps(hdc, HORZSIZE);
      bmi.bmiHeader.biYPelsPerMeter = GetDeviceCaps(hdc, VERTRES) * 1000 / GetDeviceCaps(hdc, VERTSIZE);

      /* ����λͼ��� */
      bmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &dib_bits, NULL, 0);

      if (bmp == NULL) {
        b_status = ER_SYSERR;
        __leave;
      }

      /* ��������DC */
      if ((hComDC = CreateCompatibleDC(hdc)) == NULL) {
        b_status = ER_SYSERR;
        __leave;
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

      if ((pinfo_str->p_bit_data = (unsigned char*)isirw_orgpix_malloc(linesize * pinfo_str->height + 4)) == NULL
          || (pinfo_str->pp_line_addr = isirw_malloc(sizeof(unsigned char*)*pinfo_str->height)) == NULL) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* ���������ݿ��������ݰ� */
      memcpy(pinfo_str->p_bit_data, dib_bits, linesize * pinfo_str->height);

      /* ��д���׵�ַ���� */
      ppt = (unsigned char**)pinfo_str->pp_line_addr;

      for (i = 0; i < pinfo_str->height; i++) {
        ppt[i] = (void*)(pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
      }

      /* ��д���ݰ��������� */
      pinfo_str->pal_count  = 0;
      pinfo_str->imgnumbers = 1;
      pinfo_str->psubimg    = NULL;
    }
    __finally {
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
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}




/* ��ȡͼ��λ���� */
EMF_API int CALLAGREEMENT emf_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
# ifdef WIN32
  ISFILE*      pfile = (ISFILE*)0;

  enum EXERESULT  b_status = ER_SUCCESS;

  HENHMETAFILE hemf;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&emf_load_img_critical);

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

      /* ���ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ��֤emf�ļ�����Ч�� */
      if ((b_status = _verify_file(pfile)) != ER_SUCCESS) {
        __leave;
      }

      /* ����ǿյ����ݰ������Ȼ�ȡͼ���Ҫ��Ϣ���ɹ����ٶ�ȡͼ�� */
      if (pinfo_str->data_state == 0) {
        b_status = _get_info(pfile, pinfo_str);

        if (b_status != ER_SUCCESS) {
          __leave;
        }

        pinfo_str->data_state = 1;
      }

      assert(pinfo_str->data_state == 1);

      /* ��ȡemf���� */
      if ((b_status = _load_enh_metafile(pfile, &hemf)) != ER_SUCCESS) {
        __leave;
      }

      assert(hemf);

      /* ת�������ݰ� */
      if ((b_status = _enh_meta_to_raster(hemf, pinfo_str)) != ER_SUCCESS) {
        __leave;
      }

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

      /* һ���н��ȵ��� */
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

      pinfo_str->data_state = 2;
    }
    __finally {
      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
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

      LeaveCriticalSection(&emf_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

# else

  b_status = ER_NOTSUPPORT;

# endif /* WIN32 */

  return (int)b_status;
}

