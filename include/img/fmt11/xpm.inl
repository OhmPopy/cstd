// XPMͼ���дģ�鶨���ļ�

// ��ȡ���ܣ�XPM v3��ʽͼ�񣨶�ע����Ϣ�����ݴ��ԣ���
//          ���湦�ܣ�1��4��8λͼ��ע���������ȵ���Ϣ��

/* XPM�ļ�ͷ��־�� */
#define XPM_MARK  "/* XPM */"

/* �������еĿո�� */
#define SKIPSPACE1(p) do{while(((*(p))==' ')||((*(p))=='\t')) (p)++;}while(0)
/* �������еķǿո�� */
#define SKIPNONSPACE(p) do{while(((*(p))!=' ')&&((*(p))!='\t')&&((*(p))!=0)&&((*(p))!='\"')) (p)++;}while(0)



#ifdef WIN32
#define MAX_PATH_LEN _MAX_PATH
#define MAX_DRIVE_LEN _MAX_DRIVE
#define MAX_DIR_LEN  _MAX_DIR
#define MAX_FNAME_LEN _MAX_FNAME
#define MAX_EXT_LEN  _MAX_EXT
#else
#define MAX_PATH_LEN 512    /* Ӧ�޸�ΪLinux�е���Ӧֵ */
#define MAX_DRIVE_LEN 32
#define MAX_DIR_LEN  512
#define MAX_FNAME_LEN 512
#define MAX_EXT_LEN  512
#endif


#define CTSIZE   92      /* ��ɫת�������ڱ��湦�� */
//static char    ct1[17] = " *.o#+@O$81ugcms";
const char* xpm_ct92 = " .+@#$%&*=-;>,')!~{]^/(_:<[}|1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`";

typedef COLOR Fl_Color;

int fl_parse_color(const char* p, uchar* c)
{
  size_t n, m;
  int R = 0, G = 0, B = 0;
  const char* pattern = 0;

  if (*p == '#') {
    p++;
  }

  n = strlen(p);
  m = n / 3;

  switch (m) {
  case 1:
    pattern = "%1x%1x%1x";
    break;

  case 2:
    pattern = "%2x%2x%2x";
    break;

  case 3:
    pattern = "%3x%3x%3x";
    break;

  case 4:
    pattern = "%4x%4x%4x";
    break;

  default:
    return 0;
  }

  if (sscanf(p, pattern, &R, &G, &B) == 3) {
    switch (m) {
    case 1:
      R *= 0x11;
      G *= 0x11;
      B *= 0x11;
      break;

    case 3:
      R >>= 4;
      G >>= 4;
      B >>= 4;
      break;

    case 4:
      R >>= 8;
      G >>= 8;
      B >>= 8;
      break;
    }

    c[0] = (uchar)B;
    c[1] = (uchar)G;
    c[2] = (uchar)R;
  }

  return 1;
}

int xpm_load_from_str(const char* const* xpm, int cn_req, img_t* im, int* pbpp, uchar* palette)
{
  int w, h, i;
  int ncolors;  // Number of colors in image
  int chars_per_pixel;  // Number of colors in image
  //typedef uchar uchar4[4];
  uchar _colors[1024] = {0};
  const uchar** data = NULL;
  int color_count;
  static unsigned char xmap[128] = {0};

  if (0 == xmap['z']) {
    for (i = 0; xpm_ct92[i]; ++i) {
      xmap[(uchar)xpm_ct92[i]] = i;
    }
  }

  if (NULL == palette) {
    palette = _colors;
  }

  sscanf(xpm[0], "%d %d %d %d", &w, &h, &ncolors, &chars_per_pixel);
  //height + 2);
  // height + ncolors + 1);

  if ((chars_per_pixel < 1) || (chars_per_pixel > 2)) {
    return 0;
  }

  color_count = 0;
  data = (const uchar**)(xpm + 1);

  // normal XPM colormap with names
  for (i = 0; i < ncolors; i++) {
    uchar* c;
    const uchar* p = *data++;
    // the first 1 or 2 characters are the color index:
    int ind = xmap[*p++];
    const uchar* previous_word;
    int parse;

    if (chars_per_pixel > 1) {
      ind += (xmap[*p++] * CTSIZE);
    }

    ASSERT(ind < 256);
    c = palette + ind * 4;
    // look for "c word", or last word if none:
    previous_word = p;

    for (;;) {
      uchar what;

      while (*p && isspace(*p)) {
        p++;
      }

      what = *p++;

      while (*p && !isspace(*p)) {
        p++;
      }

      while (*p && isspace(*p)) {
        p++;
      }

      if (!*p) {
        p = previous_word;
        break;
      }

      if (what == 'c') {
        break;
      }

      previous_word = p;

      while (*p && !isspace(*p)) {
        p++;
      }
    }

    parse = fl_parse_color((const char*)p, c);

    if (parse) {
      c[3] = 255;
    }
    else {
      // assume "None" or "#transparent" for any errors
      // "bg" should be transparent...
      c[0] = c[1] = c[2] = c[3] = 0;
    } // if parse
  } // for ncolors

  if (pbpp) {
    *pbpp = 4;
  }

  cn_req = (cn_req <= 0) ? 4 : cn_req;
  imsetsize(im, h, w, cn_req, 1);
  {
    int X, Y;

    for (Y = 0; Y < h; Y++) {
      const uchar* p = data[Y];
      uchar* q = im->tt.data + Y * im->s;

      switch (cn_req) {
      case 1:
        if (chars_per_pixel <= 1) {
          for (X = 0; X < w; X++) {
            *q++ = (uchar)xmap[*p++];
          }
        }
        else {
          for (X = 0; X < w; X++) {
            int ind = xmap[*p++];
            ind += xmap[*p++] * CTSIZE;
            *q++ = (uchar)ind;
          }
        }

        break;

      case 3:
      case 4:
        if (chars_per_pixel <= 1) {
          for (X = 0; X < w; X++, q += cn_req) {
            memcpy(q, palette + 4 * xmap[*p++], cn_req);
          }
        }
        else {
          for (X = 0; X < w; X++, q += cn_req) {
            int ind = xmap[*p++];
            ind += xmap[*p++] * CTSIZE;
            memcpy(q, palette + 4 * ind, cn_req);
          }
        }

        break;
      }
    }
  }
  return 0;
}



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* ����ɨ���гߴ�(���ֽڶ���) */
CC_INLINE int xpm_calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}

/* ����հ��ַ��������ո�TAB�����س����з��� */
CC_INLINE int xpm_skip_spec(stream_t* fp)
{
  char buff[4];
  long ioloc;

  while (1) {
    if (isio_read((void*)buff, 1, 1, fp) == 0) {
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
    else {
      ioloc = isio_tell(fp);  /* ����дλ�ú���һ�ֽ� */
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
CC_INLINE int xpm_distill_comm(ISFILE* fp, char* comm, int commsize)
{
  char buff[4];
  long ioloc;
  int  wanlev, count;

  if (commsize < 5) {
    return -3;  /* ������������С��������Ҫ5���ֽ� */
  }

  if (isio_read((void*)buff, 2, 1, fp) == 0) {
    return -1;  /* ��������ļ����� */
  }

  buff[2] = 0;

  if (strcmp(buff, "/*")) {
    ioloc = isio_tell(fp);  /* ����дλ�ú���2�ֽ� */
    ioloc -= 2;

    if (isio_seek(fp, ioloc, SEEK_SET) == -1) {
      return -1;
    }

    return -2;     /* ��ǰλ�ò���ע����Ϣ */
  }

  strcpy(comm, buff);

  wanlev = 0;
  count = 2;

  while (1) {    /* ���ע����Ϣ */
    if (isio_read((void*)buff, 1, 1, fp) == 0) {
      return -1;
    }

    comm[count++] = buff[0];

    if (count == commsize) {
      return -4;  /* ע����Ϣ�������ѳ��������������ߴ� */
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
CC_INLINE int xpm_read_until_data(ISFILE* fp)
{
  char buff[4];
  long ioloc;
  int  wanlev;

  while (1) {
    if (isio_read((void*)buff, 1, 1, fp) == 0) {
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
      if (isio_read((void*)buff, 1, 1, fp) == 0) {
        return -1;
      }

      if (buff[0] == '*') {  /* ע����Ϣ��ʼ */
        wanlev = 0;

        while (1) {   /* ���ע����Ϣ */
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
        ioloc = isio_tell(fp);  /* ����дλ�ú���һ�ֽ� */
        ioloc --;

        if (isio_seek(fp, ioloc, SEEK_SET) == -1) {
          return -1;
        }
      }

      continue;
    }
    else {
      ioloc = isio_tell(fp);  /* ����дλ�ú���һ�ֽ� */
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
CC_INLINE int xpm_read_line(ISFILE* fp, char* buff)
{
  int i = 0;

  while (1) {
    if (isio_read((void*) & (buff[i]), 1, 1, fp) == 0) {
      return -1;  /* ��������ļ����� */
    }

    if (buff[i] == '\r') {
      continue;
    }
    else if (buff[i] == '\n') {   /* ȥ��/r/n���� */
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
CC_INLINE int xpm_parse_color_line(const char* p, int cpp, char* colname, unsigned long* rgb)
{
  static char mbuf[128], cbuf[128], gbuf[128];
  unsigned long mrgb, crgb, grgb;
  int  matt, catt, gatt;
  const char *coldat;
  char nametype;
  int  result, bmask = 0;
  int len = strlen(p);

  if (len <= cpp) {
    return -1;
  }

  memcpy((void*)colname, (const void*)p, cpp);  /* �������������� */

  p += cpp;       /* ������������� */

  matt = catt = gatt = 0;

  while (*p) {
    SKIPSPACE1(p);     /* �����հ׷� */

    if (*p == '\"') {
      break;
    }

    nametype = *p;
    SKIPNONSPACE(p);
    SKIPSPACE1(p);
    coldat = p;
    SKIPNONSPACE(p);

    switch (nametype) {
    case 'S':
    case 's':     /* �����������ƴ� */
      continue;

    case 'M':
    case 'm':
      matt = result = _color_to_rgb(coldat, len, &mrgb);

      if ((matt == 0) || (matt == 1)) {
        bmask |= XPM_MONO_MASK;
      }

      break;

    case 'C':
    case 'c':
      if (0) {
        if ((p = strchr(coldat, (int)'\"')) == 0) {
          assert(0);
          p = coldat;
          SKIPNONSPACE(p);
        }
      }

      catt = result = _color_to_rgb(coldat, len, &crgb);

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
      return -1;     /* Դ���ݸ�ʽ����ȷ */
    }

    if ((result != 1) && (result != 0) && (result != -1)) {
      assert(0);     /* ϵͳ���ȶ� :) */
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
    return -1;  /* Դͼ��������ȱ�� */
  }
}

/* ��ȡͼ���ļ��е�������ɫ���� */
CC_INLINE int xpm_read_color_line(LPXPMCOLOR pcol, int ncol, int cpp, ISFILE* fp)
{
  char buff[256];
  int i, result;
  char* p;

  for (i = 0; i < ncol; i++) {
    if (xpm_read_line(fp, buff) == -1) {
      return -1;  /* �ļ���д�� */
    }
    
    if ((p = strchr(buff, '\"')) == 0) {
      return -2;  /* Դ���ݸ�ʽ����ȷ */
    }
    
    p++;        /* ���ǰ������ */

    result = xpm_parse_color_line(p, cpp, pcol[i].pix, &(pcol[i].rgb));

    switch (result) {
    case -1:
      return -2;      /* �ļ����ݴ��� */

    case 0:
      pcol[i].attrib = 0;
      break;

    case 1:
      pcol[i].attrib = 1;
      break;

    case -2:
    default:
      assert(0);
      return -3;      /* ϵͳ���ȶ� */
    }
  }

  return i;
}

CC_INLINE int xpm_parse_pix_line(unsigned long* pixbuf, int width, int bitcount, LPXPMCOLOR pcol, int ncol, int cpp, const char* p) {
  int i, index;
  switch (bitcount) {
  case 1:
    for (i = 0; i < width; i++) {
      index = _search_pix(pcol, ncol, p, cpp);
      
      if (index == -1) {
        return -2;  /* �Ƿ��������� */
      }
      
      assert(index < 2);
      
      if (i % 8) {
        ((unsigned char*)pixbuf)[i / 8] |= (unsigned char)index << (7 - (i % 8));
      }
      else {
        ((unsigned char*)pixbuf)[i / 8] = (unsigned char)((index & 0x1) << 7);
      }
      
      p += cpp;
    }
    
    break;
    
  case 4:
    for (i = 0; i < width; i++) {
      index = _search_pix(pcol, ncol, p, cpp);
      
      if (index == -1) {
        return -2;  /* �Ƿ��������� */
      }
      
      assert(index < 16);
      
      if (i % 2) {
        ((unsigned char*)pixbuf)[i / 2] |= (unsigned char)index;
      }
      else {
        ((unsigned char*)pixbuf)[i / 2] = (unsigned char)((index & 0xf) << 4);
      }
      
      p += cpp;
    }
    
    break;
    
  case 8:
    for (i = 0; i < width; i++) {
      index = _search_pix(pcol, ncol, p, cpp);
      
      if (index == -1) {
        return -2;  /* �Ƿ��������� */
      }
      
      assert(index < 256);
      ((unsigned char*)pixbuf)[i] = (unsigned char)index;
      p += cpp;
    }
    
    break;
    
  case 32:
    for (i = 0; i < width; i++) {
      index = _search_pix(pcol, ncol, p, cpp);
      
      if (index == -1) {
        return -2;  /* �Ƿ��������� */
      }
      
      pixbuf[i] = pcol[index].rgb;
      p += cpp;
    }
    
    break;
    
  default:
    assert(0);
    return -3;    /* ϵͳ���ȶ� */
  }
  return 0;     /* �ɹ� */
}

/* ��ȡһ���������� */
CC_INLINE int xpm_read_pix_line(unsigned long* pixbuf, int width, int bitcount, LPXPMCOLOR pcol, int ncol, int cpp, ISFILE* fp)
{
  static char filebuf[256], buf[8];
  char* p;

  /* ȥ������Ŀո�ע���ַ� */
  if (xpm_read_until_data(fp) == -1) {
    return -1;  /* �ļ���д�� */
  }

  if (xpm_read_line(fp, filebuf) == -1) {
    return -1;
  }

  p = strchr(filebuf, '\"'); /* ��λ��ǰ�����Ŵ� */

  if (p == 0) {
    return -2;  /* �Ƿ������ظ�ʽ */
  }

  if (strlen(p) < 3) {
    return -3;  /* �������ż�һ�����ط�������г��� */
  }

  p++;      /* ���ǰ������ */

  return xpm_parse_pix_line(pixbuf, width, bitcount, pcol, ncol, cpp, p);
}

/* ��ȡͼ��λ���� */
CC_INLINE int xpm_load_chars(const char** xpm, int cn_req, img_t* im, int* pbpp, uchar* palette)
{
  int    w, h, cpp, ncol;
  int    i, result;
  LPXPMCOLOR  pcol = 0;
  int b_status = ER_SUCCESS;

  /* ��ȡͼ����ߡ�CPP����ɫֵ */
  if (sscanf(xpm[0], "%d %d %d %d", &w, &h, &ncol, &cpp) != 4) {
    b_status = ER_NONIMAGE;
  }

  /* ͼ�����ݺϷ��Լ�� */
  /* ע����CPPֵ����7ʱ��ģ����Ϊ���ǷǷ���������Ϊ����ʵ������
  û��������ͼ��CPPֵ��*/
  if ((w == 0) || (h == 0) || (cpp == 0) || (cpp > 7) || (ncol == 0)) {
    b_status = ER_NONIMAGE;
  }

  /* ����ͼ����ɫ�� */
  pcol = (LPXPMCOLOR)isirw_malloc(sizeof(XPMCOLOR) * ncol);

  if (!pcol) {
    b_status = ER_MEMORYERR;

  }
  
  xpm++;
  /* ��ȡ��ɫ��Ϣ */
  for (i = 0; i < ncol; i++) {
    result = xpm_parse_color_line(xpm[0], cpp, pcol[i].pix, &(pcol[i].rgb));
    switch (result) {
    case -1:
      return -2;      /* �ļ����ݴ��� */
      
    case 0:
      pcol[i].attrib = 0;
      break;
      
    case 1:
      pcol[i].attrib = 1;
      break;
      
    case -2:
    default:
      assert(0);
      return -3;      /* ϵͳ���ȶ� */
    }
    xpm++;
  }
  /* �����Ƿ���͸��ɫ */
  for (i = 0; i < ncol; i++) {
    if (pcol[i].attrib == 1) {
      break;
    }
  }

  cn_req = cn_req <= 0 ? 4 : cn_req;
  cn_req = 4;

  imsetsize(im, h, w, cn_req, 1);

  /* ���ж��� */
  for (i = 0; i < h; i++) {
    ulong* data = (ulong*)(im->tt.data + i * im->s);
    result = xpm_parse_pix_line(data, w, 32, pcol, ncol, cpp, xpm[0]);

    if (result == -1) {
      b_status = ER_FILERWERR;

    }
    else if ((result == -2) || (result == -3)) {
      b_status = ER_BADIMAGE;
    }
    ++xpm;
  }

  if (pcol) {
    isirw_free(pcol);
  }

  // b_status = ER_SYSERR;
  return (int)b_status;
}

/* ��ȡͼ��λ���� */
CC_INLINE int xpm_load(stream_t* pfile, int cn_req, img_t* im, int* pbpp, uchar* palette)
{
  char agb_name[256];
  int    w, h, cpp, ncol;
  int    i, result;
  char*   p;
  LPXPMCOLOR  pcol = 0;
  int b_status = ER_SUCCESS;
  char rwbuff[512];   /* ��д������ */

  /* ���ļ���λ */
  if (isio_seek(pfile, 0, SEEK_SET) == -1) {
    b_status = ER_FILERWERR;
  }

  /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
  /* ȥ���ļ��ײ�����Ŀո��ַ� */
  if (xpm_skip_spec(pfile) != 0) {
    b_status = ER_FILERWERR;

  }

  /* ��ȡע����Ϣ */
  if (xpm_distill_comm(pfile, rwbuff, 512) < 0) {
    b_status = ER_NONIMAGE;

  }

  /* ���XPM�ļ��ײ���־ */
  if (strcmp(rwbuff, XPM_MARK) != 0) {
    b_status = ER_NONIMAGE;

  }

  /* ȥ���ļ��ײ�����Ŀո�ע�͵��ַ� */
  if (xpm_read_until_data(pfile) == -1) {
    b_status = ER_FILERWERR;

  }

  /* ����һ������ */
  if (xpm_read_line(pfile, rwbuff) == -1) {
    b_status = ER_FILERWERR;

  }

  /* ��ȡͼ�����ݱ����� */
  if (rwbuff[0] == 's') {
    if (sscanf(rwbuff, "static char * %s[] = {", agb_name) != 1) {
      b_status = ER_NONIMAGE;
    }
  }
  else if (rwbuff[0] == 'c') {
    if (sscanf(rwbuff, "char * %s[] = {", agb_name) != 1) {
      b_status = ER_NONIMAGE;
    }
  }
  else {
    b_status = ER_NONIMAGE;

  }

  /* �������Я����[]���� */
  p = strrchr(agb_name, (int)'[');

  if (p) {
    *p = '\0';
  }

  /* ȥ������Ŀո�ע�͵��ַ� */
  if (xpm_read_until_data(pfile) == -1) {
    b_status = ER_FILERWERR;

  }

  /* ����һ������ */
  if (xpm_read_line(pfile, rwbuff) == -1) {
    b_status = ER_FILERWERR;

  }

  /* ��ȡͼ����ߡ�CPP����ɫֵ */
  if (sscanf(rwbuff, "\"%d %d %d %d", &w, &h, &ncol, &cpp) != 4) {
    b_status = ER_NONIMAGE;

  }

  /* ͼ�����ݺϷ��Լ�� */
  /* ע����CPPֵ����7ʱ��ģ����Ϊ���ǷǷ���������Ϊ����ʵ������
  û��������ͼ��CPPֵ��*/
  if ((w == 0) || (h == 0) || (cpp == 0) || (cpp > 7) || (ncol == 0)) {
    b_status = ER_NONIMAGE;

  }

  /* ȥ������Ŀո�ע�͵��ַ� */
  if (xpm_read_until_data(pfile) == -1) {
    b_status = ER_FILERWERR;

  }

  /* ����ͼ����ɫ�� */
  pcol = (LPXPMCOLOR)isirw_malloc(sizeof(XPMCOLOR) * ncol);

  if (!pcol) {
    b_status = ER_MEMORYERR;

  }

  /* ��ȡ��ɫ��Ϣ */
  if (xpm_read_color_line(pcol, ncol, cpp, pfile) < 0) {
    b_status = ER_NONIMAGE;

  }

  /* �����Ƿ���͸��ɫ */
  for (i = 0; i < ncol; i++) {
    if (pcol[i].attrib == 1) {
      break;
    }
  }

  /* ȥ������Ŀո�ע�͵��ַ�����λ���������ݴ� */
  if (xpm_read_until_data(pfile) == -1) {
    b_status = ER_FILERWERR;
  }

  cn_req = cn_req <= 0 ? 4 : cn_req;
  cn_req = 4;

  imsetsize(im, h, w, cn_req, 1);

  /* ���ж��� */
  for (i = 0; i < h; i++) {
    ulong* data = (ulong*)(im->tt.data + i * im->s);
    result = xpm_read_pix_line(data, w, 32, pcol, ncol, cpp, pfile);

    if (result == -1) {
      b_status = ER_FILERWERR;

    }
    else if ((result == -2) || (result == -3)) {
      b_status = ER_BADIMAGE;

    }
  }

  if (pcol) {
    isirw_free(pcol);
  }

  // b_status = ER_SYSERR;
  return (int)b_status;
}

int imhist(const img_t* im, int* cnt)
{
  int _cnt[256] = {0};
  int i, j, n = 0;
  cnt = cnt ? cnt : _cnt;

  for (i = 0; i < im->h; ++i) {
    uchar* p = im->tt.data + i * im->s;

    for (j = 0; j < im->w; ++j) {
      cnt[p[j]]++;
    }
  }

  for (i = 0; i < 256; ++i) {
    n += !!cnt[i];
  }

  return n;
}

/* ����ͼ�� */
CC_INLINE int xpm_save(stream_t* pfile, const img_t* im, const char* name, const uchar* palette0)
{
  char buffer[MAX_PATH_LEN];

  int i, j, k, ncol, cpp, ctloc0, ctloc1;
  unsigned char* p, tmpc;
  uint32 palette[256];
  uchar xmap[256] = {0};
  int b_status = ER_SUCCESS;
  int bitcount = (im->w * im->c) * 8 / im->s;
  int pal_count = 0;
  int bb = strlen(xpm_ct92);
  char _name[256];
  char* pdot = 0;

  if (palette0) {
    int cnt[256] = {0};
    const uint32* palette0_u = (const uint32*)palette0;
    pal_count = imhist(im, cnt);

    for (j = 0, i = 0; i < 256; ++i) {
      if (cnt[i]) {
        xmap[i] = j;
        palette[j++] = palette0_u[i];
      }
    }
  }

  /* д���ļ��ײ���־�� */
  sprintf(buffer, "%s\n", XPM_MARK);

  if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
    b_status = ER_FILERWERR;
  }

  /* д����������� */
  strncpy(_name, name, 256);

  if ((pdot = strchr(_name, '.'))!=NULL) {
    *pdot = 0;
  }

  snprintf(buffer, 256, "static char * %s_xpm[] = {\n", _name);

  if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
    b_status = ER_FILERWERR;

  }

  /* ֻ�����ɫ��ͼ����Ϊ��ɫ��Ŀ�����ͼ���ʺ���XPM��ʽ���� */
  assert(pal_count <= 256);

  ncol = pal_count;  /* ȡ����ɫ�� */
  cpp  = (ncol <= CTSIZE) ? 1 : 2; /* ȷ����������� */

  /* д����ߵȱ��������� */
  sprintf(buffer, "\"%d %d %d %d\",\n", im->w, im->h, ncol, cpp);

  if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
    b_status = ER_FILERWERR;
  }

  /* д����ɫ���� */
  for (i = 0; i < ncol; i++) {
    ctloc0 = i % CTSIZE;
    ctloc1 = i / CTSIZE;

    if (cpp == 1) {
      if (0 == palette[i]) {
        sprintf(buffer, "\"%c c None\",\n", xpm_ct92[ctloc0]);
      }
      else {
        sprintf(buffer, "\"%c c #%06X\",\n", xpm_ct92[ctloc0], palette[i] & 0x00ffffff);
      }

      if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
        b_status = ER_FILERWERR;

      }
    }
    else { /* cpp == 2 */
      if (0 == palette[i]) {
        sprintf(buffer, "\"%c%c c None\",\n", xpm_ct92[ctloc0], xpm_ct92[ctloc1]);
      }
      else {
        sprintf(buffer, "\"%c%c c #%06X\",\n", xpm_ct92[ctloc0], xpm_ct92[ctloc1], palette[i] & 0x00ffffff);
      }

      if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
        b_status = ER_FILERWERR;

      }
    }
  }

  /* д��ͼ������ */
  for (i = 0; i < im->h; i++) {
    p = (unsigned char*)(im->tt.data + i * im->s);

    /* д��ǰ������ */
    buffer[0] = '\"';
    buffer[1] = '\0';

    if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
      b_status = ER_FILERWERR;

    }

    /* д��ͼ�������� */
    switch (bitcount) {
    case 1:
      assert(cpp == 1);  /* ��ɫͼCPPֵ�ض�Ϊ1 */

      for (j = 0; j < im->w; j += 8) {
        tmpc = *p++;

        for (k = 0; ((k < 8) && ((j + k) < im->w)); k++) {
          if ((tmpc >> (7 - k)) & 1) {
            buffer[0] = xpm_ct92[1];
          }
          else {
            buffer[0] = xpm_ct92[0];
          }

          buffer[1] = 0;

          if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
            b_status = ER_FILERWERR;

          }
        }
      }

      break;

    case 4:
      assert(cpp == 1);

      for (j = 0; j < im->w; j += 2) {
        tmpc = *p++;

        for (k = 0; ((k < 2) && ((j + k) < im->w)); k++) {
          buffer[0] = xpm_ct92[((tmpc >> ((1 - k) * 4)) & 0xf)];
          buffer[1] = 0;

          if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
            b_status = ER_FILERWERR;

          }
        }
      }

      break;

    case 8:     /* 8λ��ͼ��CPPֵΪ2 */

      //assert(cpp == 2);
      if (2 == cpp) {
        for (j = 0; j < im->w; j++) {
          tmpc = xmap[*p++];
          buffer[0] = xpm_ct92[tmpc % CTSIZE];
          buffer[1] = xpm_ct92[tmpc / CTSIZE];
          buffer[2] = 0;

          if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
            b_status = ER_FILERWERR;
          }
        }
      }
      else {
        for (j = 0; j < im->w; j++) {
          tmpc = xmap[*p++];
          buffer[0] = xpm_ct92[tmpc];
          buffer[1] = 0;

          if (isio_write((const void*)buffer, 1, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
          }
        }
      }

      break;

    default:
      assert(0);  /* ϵͳ���ȶ� */
      b_status = ER_SYSERR;

      break;
    }

    /* ��β�����ж� */
    if ((i + 1) == (int)(im->h)) {
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

    }
  }

  return (int)b_status;
}
