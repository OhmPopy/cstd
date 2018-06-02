// PBM��PGM��PPMͼ���дģ��ʵ���ļ�
// ��ȡ���ܣ�PBM��PGM��PPM �ı���������ͼ��
// ���湦�ܣ��ɽ�1��8��24λͼ�񱣴�Ϊ��Ӧ��
// PBM��PGM��PPM �ı��������ͼ���ʽ


#include "pnm.h"

static enum pnm_type            /* PNM ���Ͷ��� */
{
  PBM = 1,
  PGM,
  PPM
};




/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* ����ɨ���гߴ�(���ֽڶ���) */
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* �жϴ����ͼ���Ƿ���Ա����� */
int CALLAGREEMENT _pnm_is_valid_img(LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  /* ################################################################# */
  /* λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(pnm_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* ��֧�ֵ�λ��ͼ�� */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (pnm_irwp_info.irwp_save.img_num) {
    if (pnm_irwp_info.irwp_save.img_num == 1) {
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* ͼ���������ȷ */
      }
    }
  }

  if ((lpsave->para_value[0] < 0) || (lpsave->para_value[0] >= pnm_irwp_info.irwp_save.para[0].count)) {
    return -3;      /* -3:��Ч�Ĵ洢��ʽ */
  }

  return 0;
}


/* ��ָ�����ж�ȡһ���Ǹ����� */
int CALLAGREEMENT _read_number(ISFILE* fp)
{
  unsigned char buff[4];      /* �߽���룬ʵ��ֻʹ�õ�һ���ֽ� */
  int number = 0;

  /* ����հ׷��ţ�����ACSII��0x9~0xd��0x20��*/
  do {
    if (isio_read((void*)buff, 1, 1, fp) == 0) {
      return -1;  /* ������������� */
    }

    /* ���ע����Ϣ��PNM��ע����Ϣ��#�ſ�ʼ��ֱ����β��*/
    if (buff[0] == '#') {
      do {
        if (isio_read((void*)buff, 1, 1, fp) == 0) {
          return -1;
        }
      }
      while ((buff[0] != '\r') && (buff[0] != '\n'));
    }

  }
  while (isspace((int)(buff[0])));

  /* ��ȡ��ֵ */
  do {
    number *= 10;
    number += buff[0] - '0';

    if (isio_read((void*)buff, 1, 1, fp) == 0) {
      return -1;
    }

  }
  while (isdigit(buff[0]));

  return(number);   /* ���ظ�����-1����ʾ������ȡʧ�� */
}


/* дһ���Ǹ���������markΪ��׺��־��0 ��׺�ո����1��׺ \n�� */
int CALLAGREEMENT _write_number(ISFILE* fp, unsigned int num, int mark)
{
  char buff[32];

  assert(num >= 0);

  _itoa(num, (char*)buff, 10);  /* ��ֲʱӦת��ΪUNIX��Ӧ���� */

  if (mark == 1) {
    strcat((char*)buff, (const char*)"\n");
  }
  else {
    strcat((char*)buff, (const char*)" ");
  }

  if (isio_write((const void*)buff, strlen(buff), 1, fp) == 0) {
    return -1;          /* д�ļ�ʧ�� */
  }

  return 0;           /* �ɹ� */
}


/* PNM ����־�б� */
int CALLAGREEMENT _is_pnm_hahaha(char* pmc)
{
  char c;

  assert(pmc != 0);

  if (*pmc++ != 'P') {
    return -1;  /* ��PNMͼ�� */
  }

  c = (*pmc) - '0';

  /*
   * P1 PBM, ascii
   * P2 PGM, ascii
   * P3 PPM, ascii
   * P4 PBM, binary
   * P5 PGM, binary
   * P6 PPM, binary
   */

  if ((c < 1) || (c > 6)) {
    return -1;  /* ��PNMͼ�� */
  }

  return (int)c;    /* ����PNM��ʽ��ʶ�� 1~6 */
}




/* ��ȡͼ��λ���� */
PNM_API int CALLAGREEMENT pnm_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;

  char      buff[4], *p;
  int       kind, ascii_mark, i, y, x, r, linesize, cr, cb, cg;
  int       width, height, maxval;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&pnm_load_img_critical);

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

      /* ������λ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ��PNMͼ���ײ���־��Ϣ */
      if (isio_read((void*)buff, 2, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* �ж����Ƿ���һ��PNMͼ�� */
      if ((kind = _is_pnm_hahaha(buff)) == -1) {
        b_status = ER_NONIMAGE;
        __leave;
      }

      if (kind > 3) {
        ascii_mark = 0;     /* �����Ƹ�ʽ */
        kind -= 3;
      }
      else {
        ascii_mark = 1;     /* ASCII ��ʽ */
      }

      /* ��ȡͼ��������� */
      if ((width = _read_number(pfile)) == -1) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      if ((height = _read_number(pfile)) == -1) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      /* PGM��PPM������һ�������ɫ��������ֵ */
      if (kind != PBM) {
        if ((maxval = _read_number(pfile)) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* ISeeϵͳ��֧�ֵ�����ɫ��������8λ��ͼ�� */
        if ((maxval <= 0) || (maxval > 255)) {
          b_status = ER_BADIMAGE;
          __leave;
        }
      }
      else {
        maxval = 255;
      }

      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        /* ����ֵ��� */
        if ((width <= 0) || (height <= 0)) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* ��дͼ����Ϣ */
        pinfo_str->imgtype     = IMT_RESSTATIC; /* ͼ�������� */

        if (kind == PBM) {            /* ͼ������ʽ����׺���� */
          pinfo_str->imgformat = IMF_PBM;
        }
        else if (kind == PGM) {
          pinfo_str->imgformat = IMF_PGM;
        }
        else if (kind == PPM) {
          pinfo_str->imgformat = IMF_PPM;
        }
        else {
          assert(0);
        }

        if (ascii_mark == 1) {          /* ͼ������ѹ����ʽ */
          pinfo_str->compression  = ICS_PNM_TEXT;
        }
        else if (ascii_mark == 0) {
          pinfo_str->compression  = ICS_PNM_BIN;
        }
        else {
          assert(0);
        }

        pinfo_str->width    = width;
        pinfo_str->height   = height;
        pinfo_str->order    = 0;        /* ���� */

        pinfo_str->b_mask = 0x0;
        pinfo_str->g_mask = 0x0;
        pinfo_str->r_mask = 0x0;
        pinfo_str->a_mask = 0x0;

        if (kind == PBM) {            /* ͼ��λ�� */
          pinfo_str->bitcount = 1;
        }
        else if (kind == PGM) {
          pinfo_str->bitcount = 8;  /* ��ɫ��ͼ */
        }
        else if (kind == PPM) {
          pinfo_str->bitcount = 24;

          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0x0;
        }
        else {
          assert(0);
        }

        pinfo_str->data_state = 1;
      }


      /* ���õ�ɫ������ */
      switch (pinfo_str->bitcount) {
      case  1:
        pinfo_str->pal_count = 2;
        pinfo_str->palette[0] = 0xffffff;
        pinfo_str->palette[1] = 0x0;
        break;

      case  8:
        pinfo_str->pal_count = 256;

        for (i = 0; i < 256; i++) {
          pinfo_str->palette[i] = (i << 16) | (i << 8) | (i << 0);
        }

        break;

      case  24:
        pinfo_str->pal_count = 0;
        break;

      default:
        assert(0);
        pinfo_str->pal_count = 0;
        break;
      }


      /* ȡ��ɨ���гߴ� */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);

      /* ����ͼ����� */
      pinfo_str->imgnumbers = 1;
      pinfo_str->psubimg  = (LPSUBIMGBLOCK)0;

      assert(pinfo_str->p_bit_data == (unsigned char*)0);
      /* ����Ŀ��ͼ���ڴ�飨+4 �� β������4�ֽڻ������� */
      pinfo_str->p_bit_data = (unsigned char*)isirw_orgpix_malloc(linesize * pinfo_str->height + 4);

      if (!pinfo_str->p_bit_data) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      assert(pinfo_str->pp_line_addr == (void**)0);
      /* �������׵�ַ���� */
      pinfo_str->pp_line_addr = (void**)isirw_malloc(sizeof(void*) * pinfo_str->height);

      if (!pinfo_str->pp_line_addr) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* ��ʼ���������� */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + (i * linesize));
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

      /* ������ɫ�������������� */
      r = 255 / maxval;

      /* ���ж��� */
      if (pinfo_str->compression == ICS_PNM_TEXT) {
        for (y = 0; y < height; y++) {
          /* ȡ�����׵�ַ */
          p = (char*)pinfo_str->pp_line_addr[y];

          if (pinfo_str->imgformat == IMF_PBM) {
            memset((void*)p, 0, linesize);  /* ��Ϊ����ֻ�С��򡯲�����������Ҫ�ȳ�ʼ��Ϊ0 */

            for (x = 0; x < width; x++) {
              do {
                if (isio_read((void*)buff, 1, 1, pfile) == 0) {
                  b_status = ER_FILERWERR;
                  __leave;
                }
              }
              while (isspace((int)buff[0]));

              /* ���̵�Դ���ݼ��� :) */
              if ((buff[0] != '0') && (buff[0] != '1')) {
                b_status = ER_BADIMAGE;
                __leave;
              }

              buff[0] -= '0';

              p[x / 8] |= buff[0] << (7 - (x % 8)); /* ͷ�������ֽڵͶ�(ISee format) */
            }
          }
          else if (pinfo_str->imgformat == IMF_PGM) {
            for (x = 0; x < width; x++) {
              if ((i = _read_number(pfile)) == -1) {
                b_status = ER_BADIMAGE;
                __leave;
              }

              if (i > maxval) {
                b_status = ER_BADIMAGE;
                __leave;
              }

              i *= r; /* ������ɫ������Χ�� 0��255 */

              p[x] = (char)i;
            }
          }
          else if (pinfo_str->imgformat == IMF_PPM) {
            for (x = 0; x < width; x++) {
              if (((cr = _read_number(pfile)) == -1) ||
                  ((cg = _read_number(pfile)) == -1) ||
                  ((cb = _read_number(pfile)) == -1)) {
                b_status = ER_BADIMAGE;
                __leave;
              }

              if ((cr > maxval) || (cg > maxval) || (cb > maxval)) {
                b_status = ER_BADIMAGE;
                __leave;
              }

              /* ������ɫ������Χ�� 0��255 */
              cr *= r;
              cg *= r;
              cb *= r;

              p[x * 3]   = (char)cb;
              p[x * 3 + 1] = (char)cg;
              p[x * 3 + 2] = (char)cr; /* BGR ˳�� (ISee format) */
            }
          }
          else {
            assert(0);
          }

          /* ���ȵ��� */
          switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, y, 1, IRWE_CALLBACK_FUNID_LOAD)) {
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
      }
      else { /* ICS_PNM_BIN */
        for (y = 0; y < height; y++) {
          /* ȡ�����׵�ַ */
          p = (char*)pinfo_str->pp_line_addr[y];

          if (pinfo_str->imgformat == IMF_PBM) {
            if (isio_read((void*)p, (size_t)((width + 7) >> 3), 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;
            }
          }
          else if (pinfo_str->imgformat == IMF_PGM) {
            if (isio_read((void*)p, width, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;
            }

            /* ������������ˣ��õ�0��255��Χ����ɫ����ֵ */
            for (x = 0; x < width; x++) {
              p[x] *= (char)r;
            }
          }
          else if (pinfo_str->imgformat == IMF_PPM) {
            if (isio_read((void*)p, (size_t)(width * 3), 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;
            }

            /* ����R��B����λ�ã�ʹ֮����ISee��ʽ */
            for (x = 0; x < width; x++) {
              buff[0] = p[x * 3];
              p[x * 3]  = p[x * 3 + 2];
              p[x * 3 + 2] = buff[0];

              p[x * 3]   *= (char)r;  /* ���ŷ��� */
              p[x * 3 + 1] *= (char)r;
              p[x * 3 + 2] *= (char)r;
            }
          }
          else {
            assert(0);
          }

          /* ���ȵ��� */
          switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, y, 1, IRWE_CALLBACK_FUNID_LOAD)) {
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
            pinfo_str->data_state = 1;
          }

          (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD);
        }
      }

      LeaveCriticalSection(&pnm_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* ����ͼ�� */
PNM_API int CALLAGREEMENT pnm_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       ascii_mark;     /* �洢��ʽ */
  int       kind;       /* �����ʽ����׺����*/
  char      buffer[64], *p;
  int       x, y, mark = 0;
  unsigned int  num;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&pnm_save_img_critical);

      /* ����ϵͳ�ص����� */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_INTO_PLUG, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
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

      /* �ж��Ƿ�����֧�ֵ�ͼ������ */
      if (_pnm_is_valid_img(pinfo_str, lpsave) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* �������ͬ��������������������ȷ����ʾ */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ��λ�����ײ� */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 0 �����ı���ʽ��1��������Ʒ�ʽ */
      ascii_mark = (pnm_irwp_info.irwp_save.para[0].value[lpsave->para_value[0]] == 0) ? 1 : 0;

      /* ȷ�������ʽ */
      switch (pinfo_str->bitcount) {
      case  1:
        kind = PBM;
        break;

      case  8:
        kind = PGM;
        break;

      case  24:
        kind = PPM;
        break;

      default:
        assert(0);      /* ��ģ�鲻�ܱ����������ָ�ʽ�����ͼ�� */
        b_status = ER_NSIMGFOR;
        __leave;
        break;
      }

      /* д����ͷ����Ϣ�ṹ */
      sprintf((char*)buffer, "P%d\n#Created by ISeeExplorer PNM Plug-in 2.2\n%d %d", ((ascii_mark == 1) ? kind : (kind + 3)), (int)pinfo_str->width, (int)pinfo_str->height);

      switch (kind) {
      case  PBM:
        strcat((char*)buffer, (const char*)"\n");
        break;

      case  PGM:
      case  PPM:
        strcat((char*)buffer, (const char*)" 255\n");
        break;

      default:
        assert(0);
        b_status = ER_NSIMGFOR;
        __leave;
        break;
      }

      if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_MAST_IMAGE, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
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

      /* д��ͼ������ */
      if (ascii_mark == 1) { /* �ı���ʽ */
        if (kind == PBM) {
          for (y = 0; y < (int)pinfo_str->height; y++) {
            p = (char*)pinfo_str->pp_line_addr[y];

            for (x = 0; x < (int)pinfo_str->width; x++) {
              mark++;

              if (_write_number(pfile, (unsigned int)(unsigned char)((p[x / 8] & (1 << (7 - (x % 8)))) >> (7 - (x % 8))), ((mark < 32) ? 0 : 1)) == -1) {
                b_status = ER_FILERWERR;
                __leave;
              }

              /* ��ΪPNM˵�����й涨ÿ�����ݵ��ַ������ܳ���70���ַ���������mark���� */
              if (mark >= 32) {
                mark = 0;
              }
            }
          }
        }
        else if (kind == PGM) {
          for (y = 0; y < (int)pinfo_str->height; y++) {
            p = (char*)pinfo_str->pp_line_addr[y];

            for (x = 0; x < (int)pinfo_str->width; x++) {
              ((unsigned long*)buffer)[0] = pinfo_str->palette[(unsigned int)(unsigned char)p[x]];
              /* ��RGB����ת��Ϊ�Ҷ����ݡ��Ҷ�ת����ʽ (3*r+6*g+b)/10 */
              num = (((unsigned int)(unsigned char)buffer[2]) * 3 + ((unsigned int)(unsigned char)buffer[1]) * 6 + ((unsigned int)(unsigned char)buffer[0]) * 1) / (unsigned int)10;
              mark++;

              if (_write_number(pfile, num, ((mark < 16) ? 0 : 1)) == -1) {
                b_status = ER_FILERWERR;
                __leave;
              }

              if (mark >= 16) {
                mark = 0;
              }
            }
          }
        }
        else { /* if (kind == PPM) */
          for (y = 0; y < (int)pinfo_str->height; y++) {
            p = (char*)pinfo_str->pp_line_addr[y];

            for (x = 0; x < (int)pinfo_str->width; x++) {
              mark++;

              /* д��R��G��B���� */
              if ((_write_number(pfile, (unsigned int)(unsigned char)p[x * 3 + 2], 0) == -1) ||
                  (_write_number(pfile, (unsigned int)(unsigned char)p[x * 3 + 1], 0) == -1) ||
                  (_write_number(pfile, (unsigned int)(unsigned char)p[x * 3 + 0], ((mark < 5) ? 0 : 1)) == -1)) {
                b_status = ER_FILERWERR;
                __leave;
              }

              if (mark >= 5) {
                mark = 0;
              }
            }
          }
        }
      }
      else {        /* �����Ʒ�ʽ */
        assert(ascii_mark == 0);

        /* �����Ʒ�ʽ��ÿ���ַ�����Ҫ�� */

        if (kind == PBM) {
          for (y = 0; y < (int)pinfo_str->height; y++) {
            p = (char*)pinfo_str->pp_line_addr[y];

            if (isio_write((const void*)p, (int)(pinfo_str->width + 7) / 8, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;/* д�ļ�ʧ�� */
            }
          }
        }
        else if (kind == PGM) {
          for (y = 0; y < (int)pinfo_str->height; y++) {
            p = (char*)pinfo_str->pp_line_addr[y];

            if (isio_write((const void*)p, pinfo_str->width, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;/* д�ļ�ʧ�� */
            }
          }
        }
        else { /* if (kind == PPM) */
          for (y = 0; y < (int)pinfo_str->height; y++) {
            p = (char*)pinfo_str->pp_line_addr[y];

            for (x = 0; x < (int)pinfo_str->width; x++) {
              /* д��R��G��B���� */
              if ((isio_write((const void*) & (p[x * 3 + 2]), 1, 1, pfile) == 0) ||
                  (isio_write((const void*) & (p[x * 3 + 1]), 1, 1, pfile) == 0) ||
                  (isio_write((const void*) & (p[x * 3 + 0]), 1, 1, pfile) == 0)) {
                b_status = ER_FILERWERR;
                __leave;
              }
            }
          }
        }
      }

      /* һ���Խ��ȵ��� */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, (int)pinfo_str->height, IRWE_CALLBACK_FUNID_SAVE)) {
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

      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
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
    __finally {
      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }
      else {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }

      LeaveCriticalSection(&pnm_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}

