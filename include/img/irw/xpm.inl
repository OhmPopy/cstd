/********************************************************************
 xpm.c
 ���ļ���;�� XPMͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ�XPM v3��ʽͼ�񣨶�ע����Ϣ�����ݴ��ԣ���
 ���湦�ܣ�1��4��8λͼ��ע���������ȵ���Ϣ��
 ���ļ���д�ˣ� YZ
 ���ļ��汾�� 30927
 ����޸��ڣ� 2003-09-27
 ----------------------------------------------------------------
 ������ʷ��
 2003-9 �޸ı��溯���Խ������͵��жϣ�����Ӧ�µ�ISeeIO�������ƣ�
 2003-6 ������ɫ�ַ������пո�ʱ���ֵĴ���()
 2003-4 ֧��ͼ���д�����ṩ�����ػ���������
 2002-9 ����ӿ�������2.2�������ڴ�������
 ����xpm_load_image()�����д��ڵ�һ��BUG��
 2002-1 ��һ�η������°汾����ʼ�汾��2.0��
********************************************************************/

/* XPM�ļ�ͷ��־�� */
#define XPM_MARK  "/* XPM */"

/* �������еĿո�� */
#define SKIPSPACE(p)  do{while(((*(p))==' ')||((*(p))=='\t')) (p)++;}while(0)
/* �������еķǿո�� */
#define SKIPNONSPACE(p) do{while(((*(p))!=' ')&&((*(p))!='\t')&&((*(p))!=0)&&((*(p))!='\"')) (p)++;}while(0)

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

/* XPM ͼ����ɫ�ṹ */
typedef struct _tagXPMCOLOR {
  char      pix[8];   /* ���������������Ϊ7���� */
  ulong rgb;    /* RGBA���� */
  int       attrib;   /* ��ɫ���ԣ�0����ͨ��ɫ��1��͸��ɫ */
} XPMCOLOR;



#define CTSIZE 16 /* ��ɫת�������ڱ��湦�� */
static const char ct1[17] = " *.o#+@O$81ugcms";
//static ushort clbuff[256]; /* ��ɫ�������� */

/* ���ݸ������������������Ҷ�Ӧ��RGB�������������ֵ */
static int _search_pix(XPMCOLOR* pcol, int ncol, char* pix, int cpp)
{
  int i;
  for (i = 0; i < ncol; i++) {
    if (memcmp((const void*)pcol[i].pix, (const void*)pix, cpp) == 0) {
      return i;
    }
  }
  return -1;
}

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */
/* �жϴ����ͼ���Ƿ���Ա����� */
int _xpm_is_valid_img(INFOSTR* pinfo_str)
{
  /* λ����һ������32λ��ʽ�޷���ʾ��?�LONGΪ32λ��*/
  enum {bitcount = 1UL | (1UL << (4 - 1)) | (1UL << (8 - 1)) };
  if (!(bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1; /* ��֧�ֵ�λ��ͼ�� */
  }
  assert(pinfo_str->imgnumbers);
      if (pinfo_str->imgnumbers != 1) {
        return -2; /* ͼ���������ȷ */
      }
  return 0;
}
/* ����հ��ַ��������ո�TAB�����س����з��� */
int _skip_spec(ISFILE* fp)
{
  char buff[4];
  long ioloc;
  while (1) {
    if (isio_read((void*)buff, 1, 1, fp) == 0) {
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
    else {
      ioloc = isio_tell(fp); /* ����дλ�ú���һ�ֽ� */
      ioloc --;
      if (isio_seek(fp, ioloc, SEEK_SET) == -1) {
        return -1;
      }
      break;
    }
  }
  return 0; /* �ɹ� */
}
/* ��ȡע����Ϣ */
int _distill_comm(ISFILE* fp, char* comm, int commsize)
{
  char buff[4];
  long ioloc;
  int wanlev, count;
  if (commsize < 5) {
    return -3; /* ������������С��������Ҫ5���ֽ� */
  }
  if (isio_read((void*)buff, 2, 1, fp) == 0) {
    return -1; /* ��������ļ����� */
  }
  buff[2] = 0;
  if (strcmp(buff, "/*")) {
    ioloc = isio_tell(fp); /* ����дλ�ú���2�ֽ� */
    ioloc -= 2;
    if (isio_seek(fp, ioloc, SEEK_SET) == -1) {
      return -1;
    }
    return -2; /* ��ǰλ�ò���ע����Ϣ */
  }
  strcpy(comm, buff);
  wanlev = 0;
  count = 2;
  while (1) { /* ���ע����Ϣ */
    if (isio_read((void*)buff, 1, 1, fp) == 0) {
      return -1;
    }
    comm[count++] = buff[0];
    if (count == commsize) {
      return -4; /* ע����Ϣ�������ѳ��������������ߴ� */
    }
    if (buff[0] == '*') {
      wanlev = 1;
    }
    else {
      if (buff[0] != '/') {
        wanlev = 0;
      }
      else if (wanlev == 1) {
        break;
      }
      continue;
    }
  }
  comm[count] = 0;
  return count; /* �ɹ� */
}
/* ��������ַ�������ע����Ϣ����ֱ����Ч����λ�� */
int _read_until_data(ISFILE* fp)
{
  char buff[4];
  long ioloc;
  int wanlev;
  while (1) {
    if (isio_read((void*)buff, 1, 1, fp) == 0) {
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
      if (isio_read((void*)buff, 1, 1, fp) == 0) {
        return -1;
      }
      if (buff[0] == '*') { /* ע����Ϣ��ʼ */
        wanlev = 0;
        while (1) { /* ���ע����Ϣ */
          if (isio_read((void*)buff, 1, 1, fp) == 0) {
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
int _xpm_read_line(ISFILE* fp, char* buff)
{
  int i = 0;
  while (1) {
    if (isio_read((void*) & (buff[i]), 1, 1, fp) == 0) {
      return -1; /* ��������ļ����� */
    }
    if (buff[i] == '\r') {
      continue;
    }
    else if (buff[i] == '\n') { /* ȥ��/r/n���� */
      buff[i++] = (char)0;
      break;
    }
    else {
      i++;
    }
  }
  return i;
}
#define XPM_MONO_MASK 1
#define XPM_COLOR_MASK 2
#define XPM_GRAY_MASK 4
/* ������ɫ���� */
int _parse_color_line(char* line, int cpp, char* colname, ulong* rgb)
{
  static char mbuf[128], cbuf[128], gbuf[128];
  ulong mrgb, crgb, grgb;
  int matt, catt, gatt;
  char* p, *coldat;
  char nametype;
  int result, bmask = 0;
  assert((line) && (strlen(line)));
  if ((p = strchr(line, (int)'\"')) == 0) {
    return -1; /* Դ���ݸ�ʽ����ȷ */
  }
  p++; /* ���ǰ������ */
  if (strlen(p) <= (size_t)cpp) {
    return -1;
  }
  memcpy((void*)colname, (const void*)p, cpp); /* �������������� */
  p += cpp; /* ������������� */
  matt = catt = gatt = 0;
  while (*p) {
    SKIPSPACE(p); /* �����հ׷� */
    if (*p == '\"') {
      break;
    }
    nametype = *p;
    SKIPNONSPACE(p);
    SKIPSPACE(p);
    coldat = p;
    SKIPNONSPACE(p);
    switch (nametype) {
    case 'S':
    case 's': /* �����������ƴ� */
      continue;
    case 'M':
    case 'm':
      matt = result = _color_to_rgb(coldat, p - coldat, &mrgb);
      if ((matt == 0) || (matt == 1)) {
        bmask |= XPM_MONO_MASK;
      }
      break;
    case 'C':
    case 'c':
      if ((p = strchr(coldat, (int)'\"')) == 0) {
        assert(0);
        p = coldat;
        SKIPNONSPACE(p);
      }
      catt = result = _color_to_rgb(coldat, p - coldat, &crgb);
      if ((catt == 0) || (catt == 1)) {
        bmask |= XPM_COLOR_MASK;
      }
      break;
    case 'G':
    case 'g':
      gatt = result = _color_to_rgb(coldat, p - coldat, &grgb);
      if ((gatt == 0) || (gatt == 1)) {
        bmask |= XPM_GRAY_MASK;
      }
      break;
    default:
      return -1; /* Դ���ݸ�ʽ����ȷ */
    }
    if ((result != 1) && (result != 0) && (result != -1)) {
      assert(0); /* ϵͳ���ȶ� :) */
      return -2;
    }
  }
  /* �˴�������ɫ��ԭ���ǣ��в�ɫ���ݾͲ��ûҶ����ݣ��лҶ�
  ���ݾͲ��úڰ����ݣ�������ڰ����ݶ�û���򷵻ش���*/
  if (bmask & XPM_COLOR_MASK) {
    *rgb = crgb;
    return catt;
  }
  else if (bmask & XPM_GRAY_MASK) {
    *rgb = grgb;
    return gatt;
  }
  else if (bmask & XPM_MONO_MASK) {
    *rgb = mrgb;
    return matt;
  }
  else {
    return -1; /* Դͼ��������ȱ�� */
  }
}
/* ��ȡͼ���ļ��е�������ɫ���� */
int _read_color_line(XPMCOLOR* pcol, int ncol, int cpp, ISFILE* fp)
{
  static char buff[256];
  int i, result;
  for (i = 0; i < ncol; i++) {
    if (_xpm_read_line(fp, (char*)buff) == -1) {
      return -1; /* �ļ���д�� */
    }
    result = _parse_color_line((char*)buff, cpp, pcol[i].pix, &(pcol[i].rgb));
    switch (result) {
    case -1:
      return -2; /* �ļ����ݴ��� */
    case 0:
      pcol[i].attrib = 0;
      break;
    case 1:
      pcol[i].attrib = 1;
      break;
    case -2:
    default:
      assert(0);
      return -3; /* ϵͳ���ȶ� */
    }
  }
  return i;
}
/* ��ȡһ���������� */
int _read_pix_line(ulong* pixbuf, int width, int bitcount, XPMCOLOR* pcol, int ncol, int cpp, ISFILE* fp)
{
  static char filebuf[256], buf[8];
  char* p;
  int i, index;
  /* ȥ������Ŀո�ע���ַ� */
  if (_read_until_data(fp) == -1) {
    return -1; /* �ļ���д�� */
  }
  if (_xpm_read_line(fp, filebuf) == -1) {
    return -1;
  }
  p = strchr(filebuf, '\"'); /* ��λ��ǰ�����Ŵ� */
  if (p == 0) {
    return -2; /* �Ƿ������ظ�ʽ */
  }
  if (strlen(p) < 3) {
    return -3; /* �������ż�һ�����ط�������г��� */
  }
  p++; /* ���ǰ������ */
  switch (bitcount) {
  case 1:
    for (i = 0; i < width; i++) {
      index = _search_pix(pcol, ncol, p, cpp);
      if (index == -1) {
        return -2; /* �Ƿ��������� */
      }
      assert(index < 2);
      if (i % 8) {
        ((uchar*)pixbuf)[i / 8] |= (uchar)index << (7 - (i % 8));
      }
      else {
        ((uchar*)pixbuf)[i / 8] = (uchar)((index & 0x1) << 7);
      }
      p += cpp;
    }
    break;
  case 4:
    for (i = 0; i < width; i++) {
      index = _search_pix(pcol, ncol, p, cpp);
      if (index == -1) {
        return -2; /* �Ƿ��������� */
      }
      assert(index < 16);
      if (i % 2) {
        ((uchar*)pixbuf)[i / 2] |= (uchar)index;
      }
      else {
        ((uchar*)pixbuf)[i / 2] = (uchar)((index & 0xf) << 4);
      }
      p += cpp;
    }
    break;
  case 8:
    for (i = 0; i < width; i++) {
      index = _search_pix(pcol, ncol, p, cpp);
      if (index == -1) {
        return -2; /* �Ƿ��������� */
      }
      assert(index < 256);
      ((uchar*)pixbuf)[i] = (uchar)index;
      p += cpp;
    }
    break;
  case 32:
    for (i = 0; i < width; i++) {
      index = _search_pix(pcol, ncol, p, cpp);
      if (index == -1) {
        return -2; /* �Ƿ��������� */
      }
      pixbuf[i] = pcol[index].rgb;
      p += cpp;
    }
    break;
  default:
    assert(0);
    return -3; /* ϵͳ���ȶ� */
  }
  return 0; /* �ɹ� */
}


/* ��ȡͼ����Ϣ */
EXERESULT xpm_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  char rwbuff[512]; /* ��д������ */
  char agb_name[256];
  int w, h, cpp, ncol, i;
  char* p;
  XPMCOLOR* pcol = 0;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */
  
    do {
      /* ��ȡ�ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ȥ���ļ��ײ�����Ŀո��ַ� */
      if (_skip_spec(pfile) != 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ��ȡע����Ϣ */
      if (_distill_comm(pfile, rwbuff, 512) < 0) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ���XPM�ļ��ײ���־ */
      if (strcmp(rwbuff, XPM_MARK) != 0) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ȥ���ļ��ײ�����Ŀո�ע�͵��ַ� */
      if (_read_until_data(pfile) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ����һ������ */
      if (_xpm_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ��ȡͼ�����ݱ����� */
      if (rwbuff[0] == 's') {
        if (sscanf(rwbuff, "static char * %s[] = {", agb_name) != 1) {
          b_status = ER_NONIMAGE;
          break;
        }
      }
      else if (rwbuff[0] == 'c') {
        if (sscanf(rwbuff, "char * %s[] = {", agb_name) != 1) {
          b_status = ER_NONIMAGE;
          break;
        }
      }
      else {
        b_status = ER_NONIMAGE;
        break;
      }
      /* �������Я����[]���� */
      p = strrchr(agb_name, (int)'[');
      if (p) {
        *p = '\0';
      }
      /* ȥ������Ŀո�ע�͵��ַ� */
      if (_read_until_data(pfile) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ����ͼ����ߵ����� */
      if (_xpm_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ��ȡͼ����ߡ�CPP����ɫֵ */
      if (sscanf(rwbuff, "\"%d %d %d %d", &w, &h, &ncol, &cpp) != 4) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ͼ�����ݺϷ��Լ�� */
      /* ע����CPPֵ����7ʱ��ģ����Ϊ���ǷǷ���������Ϊ����ʵ������
      ��Ӧ�û���������ͼ��CPPֵ��*/
      if ((w == 0) || (h == 0) || (cpp == 0) || (cpp > 7) || (ncol == 0)) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ȥ��?���Ŀո�ע�͵��ַ?*/
      if (_read_until_data(pfile) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ����ͼ����ɫ�� */
      pcol = (XPMCOLOR*)isirw_malloc(sizeof(XPMCOLOR) * ncol);
      if (!pcol) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* ��ȡ��ɫ��Ϣ */
      if (_read_color_line(pcol, ncol, cpp, pfile) < 0) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ��дͼ����Ϣ */
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_XPM; /* ͼ����?���ʽ����׺���?*/
      pinfo_str->compression = ICS_XPM_TEXT;
      pinfo_str->width = w;
      pinfo_str->height = h;
      pinfo_str->order = 0; /* ���� */
      /* �����Ƿ���͸��ɫ */
      for (i = 0; i < ncol; i++)
        if (pcol[i].attrib == 1) {
          break;
        }
      if (i < ncol) { /* ��͸��ɫ��ͼ��תΪ32λ��������͸��ɫ */
        pinfo_str->bitcount = 32;
        pinfo_str->b_mask = 0xff;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->a_mask = 0xff000000;
      }
      else {
        if (ncol <= 2) {
          pinfo_str->bitcount = 1;
        }
        else if (ncol <= 16) {
          pinfo_str->bitcount = 4;
        }
        else if (ncol <= 256) {
          pinfo_str->bitcount = 8;
        }
        else {
          pinfo_str->bitcount = 32;
        }
        if (pinfo_str->bitcount == 32) { /* ��͸��ɫ32λͼ�� */
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0x0;
        }
        else { /* ��ɫ��ͼ�� */
          pinfo_str->b_mask = 0x0;
          pinfo_str->g_mask = 0x0;
          pinfo_str->r_mask = 0x0;
          pinfo_str->a_mask = 0x0;
        }
      }
      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    while (0);

      if (pcol) {
        isirw_free(pcol);
      }
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT xpm_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  char rwbuff[512]; /* ��д������ */
  char agb_name[256];
  int w, h, cpp, ncol;
  int i, linesize, result;
  char* p;
  XPMCOLOR* pcol = 0;
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
        /* ȥ���ļ��ײ�����Ŀո��ַ� */
        if (_skip_spec(pfile) != 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ��ȡע����Ϣ */
        if (_distill_comm(pfile, rwbuff, 512) < 0) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* ���XPM�ļ��ײ���־ */
        if (strcmp(rwbuff, XPM_MARK) != 0) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* ȥ���ļ��ײ�����Ŀո�ע�͵��ַ� */
        if (_read_until_data(pfile) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ����һ������ */
        if (_xpm_read_line(pfile, rwbuff) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ��ȡͼ�����ݱ����� */
        if (rwbuff[0] == 's') {
          if (sscanf(rwbuff, "static char * %s[] = {", agb_name) != 1) {
            b_status = ER_NONIMAGE;
            break;
          }
        }
        else if (rwbuff[0] == 'c') {
          if (sscanf(rwbuff, "char * %s[] = {", agb_name) != 1) {
            b_status = ER_NONIMAGE;
            break;
          }
        }
        else {
          b_status = ER_NONIMAGE;
          break;
        }
        /* �������Я����[]���� */
        p = strrchr(agb_name, (int)'[');
        if (p) {
          *p = '\0';
        }
        /* ȥ������Ŀո�ע�͵��ַ� */
        if (_read_until_data(pfile) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ����һ������ */
        if (_xpm_read_line(pfile, rwbuff) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ��ȡͼ����ߡ�CPP����ɫֵ */
        if (sscanf(rwbuff, "\"%d %d %d %d", &w, &h, &ncol, &cpp) != 4) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* ͼ�����ݺϷ��Լ�� */
        /* ע����CPPֵ����7ʱ��ģ����Ϊ���ǷǷ���������Ϊ����ʵ������
        û��������ͼ��CPPֵ��*/
        if ((w == 0) || (h == 0) || (cpp == 0) || (cpp > 7) || (ncol == 0)) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* ȥ������Ŀո�ע�͵��ַ� */
        if (_read_until_data(pfile) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ����ͼ����ɫ�� */
        pcol = (XPMCOLOR*)isirw_malloc(sizeof(XPMCOLOR) * ncol);
        if (!pcol) {
          b_status = ER_MEMORYERR;
          break;
        }
        /* ��ȡ��ɫ��Ϣ */
        if (_read_color_line(pcol, ncol, cpp, pfile) < 0) {
          b_status = ER_NONIMAGE;
          break;
        }
        pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
        pinfo_str->imgformat = IMF_XPM; /* ͼ���ļ���ʽ����׺���� */
        pinfo_str->compression = ICS_XPM_TEXT;
        pinfo_str->width = w;
        pinfo_str->height = h;
        pinfo_str->order = 0; /* ���� */
        /* �����Ƿ���͸��ɫ */
        for (i = 0; i < ncol; i++)
          if (pcol[i].attrib == 1) {
            break;
          }
        if (i < ncol) { /* ��͸��ɫ��ͼ��תΪ32λ��������͸��ɫ */
          pinfo_str->bitcount = 32;
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0xff000000;
        }
        else {
          if (ncol <= 2) {
            pinfo_str->bitcount = 1;
          }
          else if (ncol <= 16) {
            pinfo_str->bitcount = 4;
          }
          else if (ncol <= 256) {
            pinfo_str->bitcount = 8;
          }
          else {
            pinfo_str->bitcount = 32;
          }
          if (pinfo_str->bitcount == 32) { /* ��͸��ɫ32λͼ�� */
            pinfo_str->b_mask = 0xff;
            pinfo_str->g_mask = 0xff00;
            pinfo_str->r_mask = 0xff0000;
            pinfo_str->a_mask = 0x0;
          }
          else { /* ��ɫ��ͼ�� */
            pinfo_str->b_mask = 0x0;
            pinfo_str->g_mask = 0x0;
            pinfo_str->r_mask = 0x0;
            pinfo_str->a_mask = 0x0;
          }
        }
        pinfo_str->data_state = 1;
      }
      else {
        /* ȥ���ļ��ײ�����Ŀո��ַ� */
        if (_skip_spec(pfile) != 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ��ȡע����Ϣ */
        if (_distill_comm(pfile, rwbuff, 512) < 0) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* ȥ���ļ��ײ�����?Ŀո�ע�͵��ַ?*/
        if (_read_until_data(pfile) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ��������������� */
        if (_xpm_read_line(pfile, rwbuff) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ȥ������Ŀո�ע�͵��ַ� */
        if (_read_until_data(pfile) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ����ͼ����ߡ�CPP������ */
        if (_xpm_read_line(pfile, rwbuff) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ��ȡͼ����ߡ�CPP����ɫֵ */
        if (sscanf(rwbuff, "\"%d %d %d %d", &w, &h, &ncol, &cpp) != 4) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* ���ߵ����ݵĺϷ����Ѿ��жϹ��ˣ����Դ˴��������ж� */
        /* ȥ������Ŀո�ע�͵��ַ� */
        if (_read_until_data(pfile) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ����ͼ����ɫ�� */
        pcol = (XPMCOLOR*)isirw_malloc(sizeof(XPMCOLOR) * ncol);
        if (!pcol) {
          b_status = ER_MEMORYERR;
          break;
        }
        /* ��ȡ��ɫ��Ϣ */
        if (_read_color_line(pcol, ncol, cpp, pfile) < 0) {
          b_status = ER_NONIMAGE;
          break;
        }
      }
      /* ȥ��?���Ŀո�ע�͵��ַ�����λ���������ݴ?*/
      if (_read_until_data(pfile) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ���õ�ɫ������ */
      switch (pinfo_str->bitcount) {
      case 1:
        pinfo_str->pal_count = 2;
        break;
      case 4:
        pinfo_str->pal_count = 16;
        break;
      case 8:
        pinfo_str->pal_count = 256;
        break;
      default:
        pinfo_str->pal_count = 0;
        break;
      }
      if (pinfo_str->pal_count) {
        for (i = 0; i < ncol; i++) {
          pinfo_str->palette[i] = pcol[i].rgb;
        }
      }
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

      /* ���ж��� */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        result = _read_pix_line((ulong*)pinfo_str->pp_line_addr[i], (int)pinfo_str->width, (int)pinfo_str->bitcount, pcol, ncol, cpp, pfile);
        if (result == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        else if ((result == -2) || (result == -3)) {
          b_status = ER_BADIMAGE;
          break;
        }
      }
      pinfo_str->data_state = 2;
    }
    while (0);
    
      if (pcol) {
        isirw_free(pcol);
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
            pinfo_str->data_state = 1; /* �Զ����� */
          }
        }
      }

  return b_status;
}
/* ����ͼ�� */
EXERESULT xpm_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  static char path_buffer[MAX_PATH_LEN];
  static char drive[MAX_DRIVE_LEN];
  static char fname[MAX_FNAME_LEN];
  static char ext[MAX_EXT_LEN];
  static char dir[MAX_DIR_LEN];
  static char buffer[MAX_PATH_LEN];
  int i, j, k, linesize, ncol, cpp, ctloc0, ctloc1;
  uchar* p, tmpc;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);
  
    do {
      /* �ж��Ƿ�����֧�ֵ�ͼ������ */
      if (_xpm_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        break;
      }
#if 0
      /* �ֽ���ļ�������ȷ��XPM�п��߱��������� */
      if (pinfo_str->name) {
        strcpy(path_buffer, (const char*)pinfo_str->name); /* �ļ� */
      }
      else {
        strcpy(path_buffer, (const char*)"image"); /* �������� */
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
      /* д���ļ��ײ���־�� */
      sprintf(buffer, "%s\n", XPM_MARK);
      if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* д����������� */
      sprintf(buffer, "static char * %s_xpm[] = {\n", fname);
      if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ֻ�����ɫ��ͼ����Ϊ��ɫ��Ŀ�����ͼ���ʺ���XPM��ʽ���� */
      assert(pinfo_str->pal_count <= 256);
      ncol = pinfo_str->pal_count; /* ȡ����ɫ�� */
      cpp = (ncol <= CTSIZE) ? 1 : 2; /* ȷ����������� */
      /* д����ߵȱ��������� */
      sprintf(buffer, "\"%d %d %d %d\",\n", pinfo_str->width, pinfo_str->height, ncol, cpp);
      if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* д����ɫ���� */
      for (i = 0; i < ncol; i++) {
        ctloc0 = i / CTSIZE;
        ctloc1 = i % CTSIZE;
        if (cpp == 1) {
          sprintf(buffer, "\"%c c #%06X\",\n", ct1[ctloc1], pinfo_str->palette[i]);
          if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
        else { /* cpp == 2 */
          sprintf(buffer, "\"%c%c c #%06X\",\n", ct1[ctloc0], ct1[ctloc1], pinfo_str->palette[i]);
          if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
      }
      /* ȡ��ɨ���гߴ� */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      /* д��ͼ������ */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        p = (uchar*)(pinfo_str->pp_line_addr[i]);
        /* д��ǰ������ */
        buffer[0] = '\"';
        buffer[1] = '\0';
        if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* д��ͼ�������� */
        switch (pinfo_str->bitcount) {
        case 1:
          assert(cpp == 1); /* ��ɫͼCPPֵ�ض�Ϊ1 */
          for (j = 0; j < (int)pinfo_str->width; j += 8) {
            tmpc = *p++;
            for (k = 0; ((k < 8) && ((j + k) < (int)pinfo_str->width)); k++) {
              if ((tmpc >> (7 - k)) & 1) {
                buffer[0] = ct1[1];
              }
              else {
                buffer[0] = ct1[0];
              }
              buffer[1] = 0;
              if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                break;
              }
            }
          }
          break;
        case 4:
          assert(cpp == 1);
          for (j = 0; j < (int)pinfo_str->width; j += 2) {
            tmpc = *p++;
            for (k = 0; ((k < 2) && ((j + k) < (int)pinfo_str->width)); k++) {
              buffer[0] = ct1[((tmpc >> ((1 - k) * 4)) & 0xf)];
              buffer[1] = 0;
              if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                break;
              }
            }
          }
          break;
        case 8: /* 8λ��ͼ��CPPֵΪ2 */
          assert(cpp == 2);
          for (j = 0; j < (int)pinfo_str->width; j++) {
            tmpc = *p++;
            buffer[0] = ct1[tmpc / CTSIZE];
            buffer[1] = ct1[tmpc % CTSIZE];
            buffer[2] = 0;
            if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
            }
          }
          break;
        default:
          assert(0); /* ϵͳ���ȶ� */
          b_status = ER_SYSERR;
          break;
        }
        /* ��β�����ж� */
        if ((i + 1) == (int)(pinfo_str->height)) {
          buffer[0] = '\"';
          buffer[1] = '}';
          buffer[2] = ';';
          buffer[3] = '\n';
          buffer[4] = 0;
        }
        else {
          buffer[0] = '\"';
          buffer[1] = ',';
          buffer[2] = '\n';
          buffer[3] = 0;
        }
        /* д���׺���� */
        if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
      }
    }
    while (0);
  return b_status;
}
