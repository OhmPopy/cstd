// WMFͼ���дģ��ʵ���ļ�
// ��ȡ���ܣ�  �ɶ�ȡWindows��׼Ԫ�ļ���Aldus�ɷ���Ԫ�ļ���
// ���湦�ܣ�  ��֧�ֱ��湦�ܡ�

/* �ļ��ṹ���� */
#define ALDUSKEY        0x9AC6CDD7
#define ALDUSMETAHEADERSIZE   22

typedef struct {
  unsigned int  key;        /* �������ALDUSKEY */
  unsigned short  hmf;
  short     left;
  short     top;
  short     right;
  short     bottom;
  unsigned short  inch;
  unsigned int  reserved;
  unsigned short  checksum;
} ALDUSMETAHEADER;


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* ����ɨ���гߴ�(���ֽڶ���) */
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* �ж�ָ��ͼ���Ƿ���WMF */
static int _verify_file(ISFILE* pfile)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  unsigned int  is_aldus, filesize, wmf_offset = 0;
  METAHEADER    meta_hdr;

  __try {
    __try {
      /* ������֤�ļ����ȣ���Ӧ��С��METAHEADER�ṹ�����һ��6�ֽڵ�β��¼���� */
      if (isio_seek(pfile, 0, SEEK_END) == -1
          || (filesize = isio_tell(pfile)) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (filesize < (sizeof(METAHEADER) + 6)) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      /* ��ȡ�ļ���ͷ���ֽڣ��ж��ǲ���placeableԪ�ļ� */
      if (isio_seek(pfile, 0, SEEK_SET) == -1
          || isio_read(&is_aldus, sizeof(is_aldus), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (is_aldus == ALDUSKEY) {
        ALDUSMETAHEADER aldus_hdr;
        WORD* pw;
        WORD checksum = 0;

        /*  ����ǣ��ٴ���֤�ļ����� */
        if (filesize < (ALDUSMETAHEADERSIZE + sizeof(METAHEADER) + 6)) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* ��ȡplaceableԪ�ļ�ͷ����������ͼ�顣 */
        if (isio_seek(pfile, 0, SEEK_SET) == -1
            || isio_read(&aldus_hdr, ALDUSMETAHEADERSIZE, 1, pfile) != 1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        for (pw = (WORD*)&aldus_hdr; pw < (WORD*)&aldus_hdr.checksum; pw++) {
          checksum ^= *pw;
        }

        /* ��ʧ�ܣ���ʾ�ļ����� */
        if (checksum != aldus_hdr.checksum) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        wmf_offset = ALDUSMETAHEADERSIZE;
      }

      /* ��ȡwindows��׼Ԫ�ļ�ͷ�����м�¼��Ԫ�ļ���С��������placeableԪ�ļ�
         ͷ���ڣ���WORDΪ��λ����Ԫ�ļ����Ȳ�Ӧ��С�ڴ� */
      if (isio_seek(pfile, wmf_offset, SEEK_SET) == -1
          || isio_read(&meta_hdr, sizeof(METAHEADER), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (filesize < (meta_hdr.mtSize * 2 - wmf_offset)) {
        b_status = ER_BADIMAGE;
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


/* ��ȡͼ����Ϣ */
static int _get_info(ISFILE* pfile, LPINFOSTR pinfo_str)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  unsigned int  is_aldus;
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

      /* ����ͼ����߶ȣ���׼wmf�ļ�δ��¼�����Ϣ��Ĭ��Ϊ��Ļ���߶� */
      pinfo_str->width  = GetDeviceCaps(hdc, HORZRES);
      pinfo_str->height = GetDeviceCaps(hdc, VERTRES);

      /* ����ͼ��Ϊ������� */
      pinfo_str->order = 1;

      /* ����λ���,����wmf�ļ���δ��¼�����Ϣ��ֱ���趨Ϊ24bit */
      pinfo_str->bitcount = 24;

      /* ���ø���ɫ�������� */
      pinfo_str->r_mask = 0xff0000;
      pinfo_str->g_mask = 0xff00;
      pinfo_str->b_mask = 0xff;
      pinfo_str->a_mask = 0x0;

      /* ����ǿɷ���Ԫ�ļ������ļ�ͷ�л�ÿ�ȡ��߶� */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (isio_read(&is_aldus, sizeof(is_aldus), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (is_aldus == ALDUSKEY) {
        ALDUSMETAHEADER hdr;
        float fx, fy;

        if (isio_seek(pfile, 0, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read(&hdr, ALDUSMETAHEADERSIZE, 1, pfile) != 1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ���ȼ���ͼ��ĵ�λ�ߴ� */
        fx = (float)(hdr.right - hdr.left);
        fy = (float)(hdr.bottom - hdr.top);

        /* hdr.inch��ʾһӢ���൱�ڶ��ٵ�λ���ɴ˼������Ӣ���ʾ��ͼ��ߴ磬
           �ٳ�����Ļ��DPIֵ���Ӷ��õ�ͼ��������Ϊ��λ�ĳߴ�*/
        fx = (fx / (float)hdr.inch) * (float)GetDeviceCaps(hdc, LOGPIXELSX) + 0.5f;
        fy = (fy / (float)hdr.inch) * (float)GetDeviceCaps(hdc, LOGPIXELSY) + 0.5f;
        pinfo_str->width = (unsigned long)fx;
        pinfo_str->height = (unsigned long)fy;
      }

      /* ����ͼ�����͡��洢��ʽ������ѹ����ʽ */
      pinfo_str->imgtype     = IMT_VECTORSTATIC;
      pinfo_str->imgformat   = is_aldus == ALDUSKEY ? IMF_APM : IMF_WMF;
      pinfo_str->compression = ICS_GDIRECORD;
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


/* ��WMF���ݣ�������HMETAFILE��� */
static int _load_metafile(ISFILE* pfile, HMETAFILE* phout)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  METAHEADER    meta_hdr;
  unsigned long is_aldus;
  unsigned char* meta_bits = NULL;
  int       wmf_offset = 0;

  assert(pfile);

  __try {
    __try {
      /* ��ȡ�ļ�ͷ��ʶ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1
          || isio_read(&is_aldus, sizeof(is_aldus), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ����ǿɷ���Ԫ�ļ���APM��������APM�ļ�ͷ */
      if (is_aldus == ALDUSKEY) {
        wmf_offset = ALDUSMETAHEADERSIZE;
      }

      /* ����Ԫ�ļ����ݣ�����Ԫ�ļ�handle */
      memset(&meta_hdr, 0, sizeof(meta_hdr));

      if (isio_seek(pfile, wmf_offset, SEEK_SET) == -1
          || isio_read(&meta_hdr, sizeof(meta_hdr), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      meta_bits = (unsigned char*)isirw_malloc(2 * meta_hdr.mtSize);

      if (meta_bits == NULL) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      if (isio_seek(pfile, wmf_offset, SEEK_SET) == -1
          || isio_read(meta_bits, 2, meta_hdr.mtSize, pfile) != meta_hdr.mtSize) {
        b_status = ER_FILERWERR;
        __leave;
      }

      *phout = SetMetaFileBitsEx(2 * meta_hdr.mtSize, meta_bits);

      if (*phout == NULL) {
        b_status = ER_SYSERR;
        __leave;
      }

    }
    __finally {
      if (meta_bits != NULL) {
        isirw_free(meta_bits);
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


/* ��WMF����ת�����ݰ���ʸ������դ��*/
static int _meta_to_raster(HMETAFILE hwmf, LPINFOSTR pinfo_str)
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

  assert(hwmf);
  assert(pinfo_str);

  __try {
    __try {
      /* ������������ݵ�DC����Ԫ�ļ����ŵ���DC */
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

      /* �����DC����WMF���� */
      SetMapMode(hComDC, MM_ANISOTROPIC);
      EnumMetaFile(hComDC, hwmf, _EnumMetaFileProc, (LPARAM)pinfo_str);
      SelectObject(hComDC, oldbmp);

      /* ���õõ���λͼ����������ݰ������ */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);

      /* �������ػ����� */
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


/* WMF���Ź��̻ص����� */
static int CALLBACK _EnumMetaFileProc(HDC hDC,  HANDLETABLE* lpHTable,  METARECORD* lpMFR, int nObj, LPARAM lpClientData)
{
  static int viewport_ext_set = 0;
  LPINFOSTR  pinfo_str = (LPINFOSTR)lpClientData;

  PlayMetaFileRecord(hDC, lpHTable, lpMFR, nObj);

  switch (lpMFR->rdFunction) {
  case 0x20e: /* SetViewportExt() */
    viewport_ext_set = !0;
    break;

  case 0x20c: /* SetWindowExt() */

    /* ���Ԫ�ļ����趨��������(�߼����꣩��Χʱ����ǰû���޸�
       �����꣨�豸���꣩��Χ�����Ǿ������趨������ʲôҲ������
       �Է��޸��ļ����������� */
    if (!viewport_ext_set) {
      HDC hdc = GetDC(GetDesktopWindow());
      unsigned int cx, cy;
      float ratio;

      switch (pinfo_str->imgformat) {
      case IMF_APM:
        cx = pinfo_str->width;
        cy = pinfo_str->height;
        break;

        /* ����Ǳ�׼Ԫ�ļ������������귶Χ��ʹ�豸�����ݺ��
           ���߼�����һ�£����Ҳ�����ͼ��Ŀ��߳ߴ� */
      case IMF_WMF:
        cx = pinfo_str->width;
        cy = pinfo_str->height;

        /* ! ע����������ݺ�ȣ�y/x)�ķ���: ��Ԫ�ļ���¼�У�������
           ���Ժ����βα��෴��˳�򴢴�ģ����Ҳ�����hdc����˶���
           SetWindowExt(),rdParm[0]����Y��Χ��rdParm[1]����X��Χ��*/
        ratio = (float)lpMFR->rdParm[0] / (float)lpMFR->rdParm[1];
        cy = (unsigned int)((float)cx * ratio);

        if (cy > pinfo_str->height) {
          cy = pinfo_str->height;
          cx = (unsigned int)((float)cy / ratio);
        }

        break;

      default:
        assert(0);
      }

      SetViewportExtEx(hDC, cx, cy, NULL);
      ReleaseDC(GetDesktopWindow(), hdc);
    }

    break;

  default:
    break;
  }

  return !0;
}

/* ��ȡͼ��λ���� */
WMF_API int CALLAGREEMENT wmf_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
# ifdef WIN32

  ISFILE*      pfile = (ISFILE*)0;
  enum EXERESULT  b_status = ER_SUCCESS;
  HMETAFILE   hwmf = NULL;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&wmf_load_img_critical);

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

      /* ��֤wmf�ļ�����Ч�� */
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

      /* ����wmf���� */
      if ((b_status = _load_metafile(pfile, &hwmf)) != ER_SUCCESS) {
        __leave;
      }

      assert(hwmf);

      /* ת�����ݰ� */
      if ((b_status = _meta_to_raster(hwmf, pinfo_str)) != ER_SUCCESS) {
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
      if (hwmf) {
        DeleteMetaFile(hwmf);
      }

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

      LeaveCriticalSection(&wmf_load_img_critical);
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
