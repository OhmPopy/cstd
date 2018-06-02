/********************************************************************
 xbm.c
 ���ļ���;�� XBMͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ�X10��X11��ʽXBMͼ�񣨶�ע����Ϣ�����ݴ��ԣ���
 ���湦�ܣ�X11��ʽXBM
 ���ļ���д�ˣ� YZ
 ���ļ��汾�� 30927
 ����޸��ڣ� 2003-9-27
 ----------------------------------------------------------------
 ������ʷ��
 2003-9 �޸ı��溯���Խ������͵��жϣ�����Ӧ�µ�ISeeIO�������ƣ�
 2003-4 ֧��ͼ���д�����ṩ�����ػ���������
 2002-9 ����ӿ�������2.2�������ڴ�������
 2002-1 ��һ�η������°汾����ʼ�汾��2.0��
********************************************************************/

#ifdef _WIN32
#define MAX_PATH_LEN _MAX_PATH
#define MAX_DRIVE_LEN _MAX_DRIVE
#define MAX_DIR_LEN _MAX_DIR
#define MAX_FNAME_LEN _MAX_FNAME
#define MAX_EXT_LEN _MAX_EXT
#else
#define MAX_PATH_LEN 512 /* Ӧ�޸�ΪLinux�е���Ӧֵ */
#define MAX_DRIVE_LEN 32
#define MAX_DIR_LEN 512
#define MAX_FNAME_LEN 512
#define MAX_EXT_LEN 512
#endif
//IRWP_INFO xbm_irwp_info; /* �����Ϣ�� */
static int hex_table[256]; /* �ַ�ת���ֱ� */

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */
/* �жϴ����ͼ���Ƿ���Ա����� */
int _xbm_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = 1UL};
  /* λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1; /* ��֧�ֵ�λ��ͼ�� */
  }
  assert(pinfo_str->imgnumbers);
  if (pinfo_str->imgnumbers != 1) {
    return -2; /* ͼ���������ȷ */
  }
  return 0;
}
/* ��ʼ���ַ�ֵ�� */
void _init_hex_table()
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
int _read_integer(ISFILE* fp)
{
  int c, flag = 0, value = 0;
  while (1) {
    if (isio_read((void*)&c, 1, 1, fp) == -1) {
      return -1; /* ��������ļ����� */
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
int _xbm_read_until_data(ISFILE* fp)
{
  char buff[4];
  long ioloc;
  int wanlev;
  while (1) {
    if (isio_read((void*)buff, 1, 1, fp) == -1) {
      return -1; /* ��������ļ����� */
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
      if (buff[0] == '*') { /* ע����Ϣ��ʼ */
        wanlev = 0;
        while (1) { /* ���ע����Ϣ */
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
        ioloc = isio_tell(fp); /* ����дλ�ú���һ�ֽ� */
        ioloc --;
        if (isio_seek(fp, ioloc, SEEK_SET) == -1) {
          return -1;
        }
      }
      continue;
    }
    else {
      ioloc = isio_tell(fp); /* ����дλ�ú���һ�ֽ� */
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
int _read_line(ISFILE* fp, char* buff)
{
  int i = 0;
  while (1) {
    if (isio_read((void*) & (buff[i]), 1, 1, fp) == -1) {
      return -1; /* ��������ļ����� */
    }
    if (buff[i] == '\r') {
      continue;
    }
    else if (buff[i] == '\n') { /* ȥ��/r/n���� */
      buff[i++] = (uchar)0;
      break;
    }
    else {
      i++;
    }
  }
  return i;
}


/* ��ȡͼ����Ϣ */
EXERESULT xbm_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  char rwbuff[512]; /* ��д������ */
  char agb_name[256];
  unsigned int w, h;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */
  
  do {
    /* ��ȡ�ļ�ͷ�ṹ */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* ȥ���ļ��ײ�����Ŀո�ע�͵��ַ� */
    if (_xbm_read_until_data(pfile) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* ����һ������ */
    if (_read_line(pfile, rwbuff) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* ��ȡͼ����ֵ */
    if ((sscanf(rwbuff, "#define %s %u", agb_name, &w) == 2) &&
      (strlen(agb_name) >= 6) &&
      (strcmp(agb_name + strlen(agb_name) - 6, "_width") == 0)) {
      pinfo_str->width = (ulong)w;
    }
    else {
      b_status = ER_NONIMAGE;
      break;
    }
    /* ����һ������ */
    if (_read_line(pfile, rwbuff) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* ��ȡͼ��߶�ֵ */
    if ((sscanf(rwbuff, "#define %s %u", agb_name, &h) == 2) &&
      (strlen(agb_name) >= 7) &&
      (strcmp(agb_name + strlen(agb_name) - 7, "_height") == 0)) {
      pinfo_str->height = (ulong)h;
    }
    else {
      b_status = ER_NONIMAGE;
      break;
    }
    if ((w == 0) || (h == 0)) {
      b_status = ER_NONIMAGE;
      break;
    }
    pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
    pinfo_str->imgformat = IMF_XBM; /* ͼ���ļ���ʽ����׺���� */
    pinfo_str->compression = ICS_XBM_TEXT;
    pinfo_str->order = 0; /* ���� */
    pinfo_str->bitcount = 1; /* XBMΪ��ɫͼ��λ��Ϊ 1 */
    pinfo_str->b_mask = 0; /* ����ͼ������������ */
    pinfo_str->g_mask = 0;
    pinfo_str->r_mask = 0;
    pinfo_str->a_mask = 0;
    /* �趨���ݰ�״̬ */
    pinfo_str->data_state = 1;
  }
  while (0);
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT xbm_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  char rwbuff[512]; /* ��д������ */
  char agb_name[256], *p;
  unsigned int w, h, version;
  int i, linesize, result, j;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */
  
  do {
    /* ���ļ���λ */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
    if (pinfo_str->data_state == 0) {
      /* ȥ���ļ��ײ�����Ŀո�ע�͵��ַ� */
      if (_xbm_read_until_data(pfile) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ����һ������ */
      if (_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ��ȡͼ����ֵ */
      if ((sscanf(rwbuff, "#define %s %u", agb_name, &w) == 2) &&
        (strlen(agb_name) >= 6) &&
        (strcmp(agb_name + strlen(agb_name) - 6, "_width") == 0)) {
        pinfo_str->width = (ulong)w;
      }
      else {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ����һ������ */
      if (_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ��ȡͼ��߶�ֵ */
      if ((sscanf(rwbuff, "#define %s %u", agb_name, &h) == 2) &&
        (strlen(agb_name) >= 7) &&
        (strcmp(agb_name + strlen(agb_name) - 7, "_height") == 0)) {
        pinfo_str->height = (ulong)h;
      }
      else {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ���ͼ����������Ƿ�Ϸ� */
      if ((w == 0) || (h == 0)) {
        b_status = ER_NONIMAGE;
        break;
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_XBM; /* ͼ���ļ���ʽ����׺���� */
      pinfo_str->compression = ICS_XBM_TEXT;
      pinfo_str->order = 0; /* ���� */
      pinfo_str->bitcount = 1; /* XBMΪ��ɫͼ��λ��Ϊ 1 */
      pinfo_str->b_mask = 0; /* ����ͼ������������ */
      pinfo_str->g_mask = 0;
      pinfo_str->r_mask = 0;
      pinfo_str->a_mask = 0;
      pinfo_str->data_state = 1; /* ���ñ�� */
    }
    else {
      /* ����ļ������Ϣ */
      if (_xbm_read_until_data(pfile) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
    }
    /* ����ȵ���Ϣ�� */
    while (1) {
      if (_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
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
    /* ��ʼ���������� */
    for (i = 0; i < (int)(pinfo_str->height); i++) {
      pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
    }
    version = 11; /* ȱʡ��X11 XBM ��ʽ */
    /* �ж�XBM�汾 */
    while (1) {
      if (sscanf(rwbuff, "static short %s = {", agb_name) == 1) {
        version = 10;
      }
      else {
        if (sscanf(rwbuff, "static uchar %s = {", agb_name) == 1) {
          version = 11;
        }
        else if (sscanf(rwbuff, "static char %s = {", agb_name) == 1) {
          version = 11;
        }
        else {
          continue;
        }
      }
      p = (char*)strrchr(agb_name, '_');
      /* ���_bits[]��� */
      if (p == NULL) {
        p = agb_name;
      }
      else {
        p++;
      }
      if (strcmp("bits[]", (char*)p) == 0) {
        break;
      }
      /* ����һ������ */
      if (_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
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
            break;
          }
          CVT_BITS1(result);
          *(uchar*)(pinfo_str->p_bit_data + (i * linesize) + j / 8) = (uchar)result;
        }
      }
    }
    else {
      uchar swa, swb;
      assert(version == 10);
      /* ���ж��� */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        for (j = 0; j < (int)(pinfo_str->width); j += 16) {
          result = _read_integer(pfile);
          if (result == -1) {
            b_status = ER_FILERWERR;
            break;
          }
          swa = (uchar)result;
          swb = (uchar)(result >> 8);
          CVT_BITS1(swa);
          CVT_BITS1(swb);
          result = (int)(unsigned int)swb;
          result <<= 8;
          result |= (int)(unsigned int)swa;
          *(ushort*)(pinfo_str->p_bit_data + (i * linesize) + j / 8) = (ushort)result;
        }
      }
    }
    pinfo_str->data_state = 2;
  }
  while (0);
  
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
EXERESULT xbm_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  static char path_buffer[MAX_PATH_LEN];
  static char drive[MAX_DRIVE_LEN];
  static char fname[MAX_FNAME_LEN];
  static char ext[MAX_EXT_LEN];
  static char dir[MAX_DIR_LEN];
  static char buffer[MAX_PATH_LEN];
  int i, j, linesize;
  uchar* p;
  uchar swa;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);
  
  do {
    /* �ж��Ƿ�����֧�ֵ�ͼ������ */
    if (_xbm_is_valid_img(pinfo_str) != 0) {
      b_status = ER_NSIMGFOR;
      break;
    }
#if 0
    /* �ֽ���ļ�������ȷ��XBM�п��߱��������� */
    if (pinfo_str->name) {
      strcpy(path_buffer, (const char*)pinfo_str->name); /* �ļ� */
    }
    else {
      strcpy(path_buffer, (const char*)"temp_name"); /* �������� */
    }
    _splitpath(path_buffer, drive, dir, fname, ext);
    /* �ų��ļ����ж����'.'�ַ������ַ��������ڱ�������*/
    p = (uchar*)strchr((const char*)fname, (int)'.');
    if (p) {
      *p = 0;
    }
#endif
    /* ��λ���ļ��ײ� */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* д����߱������� */
    sprintf(buffer, "#define %s_width %u\n", fname, pinfo_str->width);
    if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    sprintf(buffer, "#define %s_height %u\n", fname, pinfo_str->height);
    if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* ע����ģ�齫�������ȵ���Ϣ */
    /* �����ʽΪX11 */
    sprintf(buffer, "static uchar %s_bits[] = {\n", fname);
    if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    strcpy(buffer, (const char*)" ");
    if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    assert(pinfo_str->bitcount == 1);
    linesize = (pinfo_str->width + 7) / 8;
    /* д��ͼ������ */
    for (i = 0; i < (int)(pinfo_str->height); i++) {
      p = (uchar*)(pinfo_str->pp_line_addr[i]);
      for (j = 0; j < linesize; j++) {
        /* ��β�����ж� */
        if (((i + 1) == (int)(pinfo_str->height)) && ((j + 1) == linesize)) {
          swa = *p++;
          CVT_BITS1(swa);
          sprintf(buffer, "0x%02x };", (uchar)swa);
          if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
        else {
          swa = *p++;
          sprintf(buffer, "0x%02x, ", (uchar)swa);
          if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
      }
      strcpy(buffer, "\n ");
      if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
    }
  }
  while (0);
  return b_status;
}
