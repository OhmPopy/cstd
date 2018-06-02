// XBMͼ���дģ��ʵ���ļ�
// ��ȡ���ܣ�X10��X11��ʽXBMͼ�񣨶�ע����Ϣ�����ݴ��ԣ���
//���湦�ܣ�X11��ʽXBM

#ifdef WIN32
#define MAX_PATH_LEN  _MAX_PATH
#define MAX_DRIVE_LEN _MAX_DRIVE
#define MAX_DIR_LEN   _MAX_DIR
#define MAX_FNAME_LEN _MAX_FNAME
#define MAX_EXT_LEN   _MAX_EXT
#else
#define MAX_PATH_LEN  512       /* Ӧ�޸�ΪLinux�е���Ӧֵ */
#define MAX_DRIVE_LEN 32
#define MAX_DIR_LEN   512
#define MAX_FNAME_LEN 512
#define MAX_EXT_LEN   512
#endif

static int       hex_table[256];    /* �ַ�ת���ֱ� */
static unsigned char rwbuff[512];     /* ��д������ */



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* ����ɨ���гߴ�(���ֽڶ���) */
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* �жϴ����ͼ���Ƿ���Ա����� */
int CALLAGREEMENT _xbm_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(xbm_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* ��֧�ֵ�λ��ͼ�� */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (xbm_irwp_info.irwp_save.img_num)
    if (xbm_irwp_info.irwp_save.img_num == 1)
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* ͼ���������ȷ */
      }

  return 0;
}


/* ��ʼ���ַ�ֵ�� */
void CALLAGREEMENT _init_hex_table()
{
  int i;

  for (i = (int)'0'; i <= (int)'9'; i++) {
    hex_table[i] = i - (int)'0';
  }

  for (i = (int)'a'; i <= (int)'f'; i++) {
    hex_table[i] = i - (int)'a' + 10;
  }

  for (i = (int)'A'; i <= (int)'F'; i++) {
    hex_table[i] = i - (int)'A' + 10;
  }

  hex_table['x'] = 0;
  hex_table['X'] = 0;

  hex_table[' '] = -1;
  hex_table[','] = -1;
  hex_table['}'] = -1;
  hex_table['\n'] = -1;
  hex_table['\t'] = -1;
}


/* ��ȡһ������ */
int CALLAGREEMENT _read_integer(ISFILE* fp)
{
  int c, flag = 0, value = 0;

  while (1) {
    if (isio_read((void*)&c, 1, 1, fp) == -1) {
      return -1;  /* ��������ļ����� */
    }

    c &= 0xff;

    if (isxdigit(c)) {
      value = (value << 4) + hex_table[c];
      flag++;
      continue;
    }

    if ((hex_table[c] < 0) && flag) { /* ','�Ϳո�ı�ֵС��0 */
      break;
    }
  }

  return value;
}


/* ��������ַ�������ע����Ϣ����ֱ����Ч����λ�� */
int CALLAGREEMENT _read_until_data(ISFILE* fp)
{
  char buff[4];
  long ioloc;
  int  wanlev;

  while (1) {
    if (isio_read((void*)buff, 1, 1, fp) == -1) {
      return -1;  /* ��������ļ����� */
    }

    if (buff[0] == ' ') {
      continue;
    }
    else if (buff[0] == '\t') {
      continue;
    }
    else if (buff[0] == '\n') {
      continue;
    }
    else if (buff[0] == '\r') {
      continue;
    }
    else if (buff[0] == '/') {
      if (isio_read((void*)buff, 1, 1, fp) == -1) {
        return -1;
      }

      if (buff[0] == '*') {     /* ע����Ϣ��ʼ */
        wanlev = 0;

        while (1) {       /* ���ע����Ϣ */
          if (isio_read((void*)buff, 1, 1, fp) == -1) {
            return -1;
          }

          if (buff[0] == '*') {
            wanlev = 1;
          }
          else {
            if (buff[0] != '/') {
              wanlev = 0;
            }
            else if ((buff[0] == '/') && (wanlev == 1)) {
              break;
            }

            continue;
          }
        }
      }
      else {
        ioloc = isio_tell(fp);    /* ����дλ�ú���һ�ֽ� */
        ioloc --;

        if (isio_seek(fp, ioloc, SEEK_SET) == -1) {
          return -1;
        }
      }

      continue;
    }
    else {
      ioloc = isio_tell(fp);    /* ����дλ�ú���һ�ֽ� */
      ioloc --;

      if (isio_seek(fp, ioloc, SEEK_SET) == -1) {
        return -1;
      }

      break;
    }
  }

  return 0;
}


/* ��һ������ */
int CALLAGREEMENT _read_line(ISFILE* fp, unsigned char* buff)
{
  int i = 0;

  while (1) {
    if (isio_read((void*) & (buff[i]), 1, 1, fp) == -1) {
      return -1;  /* ��������ļ����� */
    }

    if (buff[i] == '\r') {
      continue;
    }
    else if (buff[i] == '\n') {       /* ȥ��/r/n���� */
      buff[i++] = (unsigned char)0;
      break;
    }
    else {
      i++;
    }
  }

  return i;
}

/* ��ȡͼ��λ���� */
XBM_API int CALLAGREEMENT xbm_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;

  unsigned char agb_name[256], *p;
  unsigned int  w, h, version;
  int       i, linesize, result, j;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&xbm_load_img_critical);

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

      /* ���ļ���λ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        /* ȥ���ļ��ײ�����Ŀո�ע�͵��ַ� */
        if (_read_until_data(pfile) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ����һ������ */
        if (_read_line(pfile, rwbuff) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ��ȡͼ����ֵ */
        if ((sscanf(rwbuff, "#define %s %u", agb_name, &w) == 2) &&
            (strlen(agb_name) >= 6) &&
            (strcmp(agb_name + strlen(agb_name) - 6, "_width") == 0)) {
          pinfo_str->width = (unsigned long)w;
        }
        else {
          b_status = ER_NONIMAGE;
          __leave;
        }


        /* ����һ������ */
        if (_read_line(pfile, rwbuff) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ��ȡͼ��߶�ֵ */
        if ((sscanf(rwbuff, "#define %s %u", agb_name, &h) == 2) &&
            (strlen(agb_name) >= 7) &&
            (strcmp(agb_name + strlen(agb_name) - 7, "_height") == 0)) {
          pinfo_str->height = (unsigned long)h;
        }
        else {
          b_status = ER_NONIMAGE;
          __leave;
        }

        /* ���ͼ����������Ƿ�Ϸ� */
        if ((w == 0) || (h == 0)) {
          b_status = ER_NONIMAGE;
          __leave;
        }

        pinfo_str->imgtype    = IMT_RESSTATIC;  /* ͼ���ļ����� */
        pinfo_str->imgformat  = IMF_XBM;      /* ͼ���ļ���ʽ����׺���� */
        pinfo_str->compression  = ICS_XBM_TEXT;

        pinfo_str->order    = 0;  /* ���� */
        pinfo_str->bitcount   = 1;  /* XBMΪ��ɫͼ��λ��Ϊ 1 */

        pinfo_str->b_mask = 0;      /* ����ͼ������������ */
        pinfo_str->g_mask = 0;
        pinfo_str->r_mask = 0;
        pinfo_str->a_mask = 0;

        pinfo_str->data_state = 1;    /* ���ñ�� */
      }
      else {
        /* ����ļ������Ϣ */
        if (_read_until_data(pfile) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_read_line(pfile, rwbuff) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_read_line(pfile, rwbuff) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }
      }

      /* ����ȵ���Ϣ�� */
      while (1) {
        if (_read_line(pfile, rwbuff) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (sscanf(rwbuff, "#define %s %u", agb_name, &w) == 2) {
          continue;
        }
        else {
          break;
        }
      }

      /* ���õ�ɫ������ */
      pinfo_str->pal_count = 2;
      /* ʵ��λ����Ϊ1������Ϊ0������0����Ϊ��ɫ��1����Ϊ��ɫ */
      pinfo_str->palette[0] = 0xffffffUL;
      pinfo_str->palette[1] = 0UL;

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

      version = 11;   /* ȱʡ��X11 XBM ��ʽ */

      /* �ж�XBM�汾 */
      while (1) {
        if (sscanf(rwbuff, "static short %s = {", agb_name) == 1) {
          version = 10;
        }
        else {
          if (sscanf(rwbuff, "static unsigned char %s = {", agb_name) == 1) {
            version = 11;
          }
          else if (sscanf(rwbuff, "static char %s = {", agb_name) == 1) {
            version = 11;
          }
          else {
            continue;
          }
        }

        p = (unsigned char*)strrchr(agb_name, '_');

        /* ���_bits[]��� */
        if (p == (unsigned char*)0) {
          p = (unsigned char*)agb_name;
        }
        else {
          p++;
        }

        if (strcmp((const char*)"bits[]", (char*)p) == 0) {
          break;
        }

        /* ����һ������ */
        if (_read_line(pfile, rwbuff) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }
      }

      /* ��ʼ��ת��ֵ�� */
      _init_hex_table();

      if (version == 11) {
        /* ���ж��� */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          for (j = 0; j < (int)(pinfo_str->width); j += 8) {
            result = _read_integer(pfile);

            if (result == -1) {
              b_status = ER_FILERWERR;
              __leave;
            }

            CVT_BITS1(result);
            *(unsigned char*)(pinfo_str->p_bit_data + (i * linesize) + j / 8) = (unsigned char)result;
          }

          /* ���ȵ��� */
          switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, i, 1, IRWE_CALLBACK_FUNID_LOAD)) {
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
      else {
        unsigned char swa, swb;

        assert(version == 10);

        /* ���ж��� */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          for (j = 0; j < (int)(pinfo_str->width); j += 16) {
            result = _read_integer(pfile);

            if (result == -1) {
              b_status = ER_FILERWERR;
              __leave;
            }

            swa = (unsigned char)result;
            swb = (unsigned char)(result >> 8);

            CVT_BITS1(swa);
            CVT_BITS1(swb);

            result = (int)(unsigned int)swb;
            result <<= 8;
            result |= (int)(unsigned int)swa;

            *(unsigned short*)(pinfo_str->p_bit_data + (i * linesize) + j / 8) = (unsigned short)result;
          }

          /* ���ȵ��� */
          switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, i, 1, IRWE_CALLBACK_FUNID_LOAD)) {
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

      LeaveCriticalSection(&xbm_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* ����ͼ�� */
XBM_API int CALLAGREEMENT xbm_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;

  static char   path_buffer[MAX_PATH_LEN];
  static char   drive[MAX_DRIVE_LEN];
  static char   fname[MAX_FNAME_LEN];
  static char   ext[MAX_EXT_LEN];
  static char   dir[MAX_DIR_LEN];

  static char   buffer[MAX_PATH_LEN];

  int       i, j, linesize;
  unsigned char* p;
  unsigned char swa;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&xbm_save_img_critical);

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
      if (_xbm_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* �������ͬ��������������������ȷ����ʾ */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* �ֽ���ļ�������ȷ��XBM�п��߱��������� */
      if ((psct->type == ISEEIO_CONFER_LOC_FILE) || (psct->type == ISEEIO_CONFER_LOC_FILE_SEQUENTIAL)) {
        strcpy(path_buffer, (const char*)psct->name);  /* �ļ� */
      }
      else {
        strcpy(path_buffer, (const char*)"temp_name");  /* �������� */
      }

      _splitpath(path_buffer, drive, dir, fname, ext);

      /* �ų��ļ����ж����'.'�ַ������ַ��������ڱ�������*/
      p = (unsigned char*)strchr((const char*)fname, (int)'.');

      if (p) {
        *p = 0;
      }


      /* ��λ���ļ��ײ� */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* д����߱������� */
      sprintf(buffer, "#define %s_width %u\n", fname, pinfo_str->width);

      if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      sprintf(buffer, "#define %s_height %u\n", fname, pinfo_str->height);

      if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ע����ģ�齫�������ȵ���Ϣ */

      /* �����ʽΪX11 */
      sprintf(buffer, "static unsigned char %s_bits[] = {\n", fname);

      if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      strcpy(buffer, (const char*)"   ");

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

      assert(pinfo_str->bitcount == 1);

      linesize = (pinfo_str->width + 7) / 8;

      /* д��ͼ������ */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        p = (unsigned char*)(pinfo_str->pp_line_addr[i]);

        for (j = 0; j < linesize; j++) {
          /* ��β�����ж� */
          if (((i + 1) == (int)(pinfo_str->height)) && ((j + 1) == linesize)) {
            swa = *p++;
            CVT_BITS1(swa);
            sprintf(buffer, "0x%02x };", (unsigned char)swa);

            if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;
            }
          }
          else {
            swa = *p++;
            sprintf(buffer, "0x%02x, ", (unsigned char)swa);

            if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;
            }
          }
        }

        strcpy(buffer, "\n   ");

        if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, i, 1, IRWE_CALLBACK_FUNID_SAVE)) {
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

      LeaveCriticalSection(&xbm_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


