// SGIͼ���дģ��ʵ���ļ�
// ��ȡ���ܣ�8��24��32λSGIͼ�󣨰���ѹ����δѹ����
// ���湦�ܣ�24λ��ѹ����ʽ

/* �ļ��б��־ */
#define SGI_MAGIC           0x1da

#define SGI_REV0_SIZE   4
#define SGI_REV1_SIZE   404
#define SGI_NAME_SIZE   80

/* �ļ�ͷ�ṹ���� */
typedef struct _tag_sgi_header {
  unsigned short  magic;          /* �ļ���ʶ */
  unsigned char storage;        /* �洢��ʽ */
  unsigned char bpc;          /* ÿ��ͨ�����ص��ֽ��� */
  unsigned short  dim;          /* ά�� */
  unsigned short  width;          /* ��� */
  unsigned short  hight;          /* �߶� */
  unsigned short  channel;        /* ͨ���� */
  long      pixmin;         /* ��С����ֵ */
  long      pixmax;         /* �������ֵ */
  char      rev0[SGI_REV0_SIZE];  /* ���� */
  char      imgname[SGI_NAME_SIZE]; /* ͼ�������� */
  long      palid;          /* ��ɫ��ID */
  char      rev1[SGI_REV1_SIZE];  /* ���� */
} SGIHEADER, *LPSGIHEADER;

/* SGI��ͷ�ṹ */
static SGIHEADER  sgi_header;



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* ����ɨ���гߴ�(���ֽڶ���) */
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* �жϴ����ͼ���Ƿ���Ա����棨�粻֧�ֱ��湦�ܣ��ɽ�������ȥ���� */
int CALLAGREEMENT _sgi_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(sgi_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* ��֧�ֵ�λ��ͼ�� */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (sgi_irwp_info.irwp_save.img_num) {
    if (sgi_irwp_info.irwp_save.img_num == 1) {
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* ͼ���������ȷ */
      }
    }
  }

  return 0;
}


/* ��δѹ��ͼ�����ݵ����ݰ��� */
static enum EXERESULT CALLAGREEMENT _read_uncom(ISFILE* pfile, LPINFOSTR pinfo_str, LPSGIHEADER psgi)
{
  int       i, linesize;
  unsigned char* p;
  long      fr, fg, fb, fa;
  unsigned char* pr = 0, *pg = 0, *pb = 0, *pa = 0;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(pfile && pinfo_str && psgi);

  __try {
    if (isio_seek(pfile, sizeof(SGIHEADER), SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      __leave;
    }

    assert(pinfo_str->pp_line_addr);

    linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
    p = pinfo_str->p_bit_data;

    switch (pinfo_str->bitcount) {
    case  8:
      assert((psgi->channel == 1) && (psgi->bpc == 1));

      /* ���ж�ȡ�������� */
      for (i = 0; i < (int)psgi->hight; i++) {
        if (isio_read((void*)p, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
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

        p += linesize;
      }

      break;

    case  24:
      assert((psgi->channel == 3) && (psgi->bpc == 1));

      /* ����R��G��B��ʱ������ */
      pr = (unsigned char*)isirw_malloc(psgi->width);
      pg = (unsigned char*)isirw_malloc(psgi->width);
      pb = (unsigned char*)isirw_malloc(psgi->width);

      if (!(pr && pg && pb)) {
        b_status = ER_MEMORYERR;
        __leave;
        break;
      }

      /* ����R��G��B�������������е�ƫ�� */
      fr = sizeof(SGIHEADER);
      fg = fr + psgi->width * psgi->hight;
      fb = fg + psgi->width * psgi->hight;

      /* ���ж�ȡ�������� */
      for (i = 0; i < (int)psgi->hight; i++) {
        /* ��ɫ���� */
        if (isio_seek(pfile, fr, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)pr, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ��ɫ���� */
        if (isio_seek(pfile, fg, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)pg, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ��ɫ���� */
        if (isio_seek(pfile, fb, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)pb, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        _to_rgb(pr, pg, pb, p, psgi->width);

        p += linesize;

        fr += (long)psgi->width;
        fg += (long)psgi->width;
        fb += (long)psgi->width;

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

      break;

    case  32:
      assert((psgi->channel == 4) && (psgi->bpc == 1));

      /* ����R��G��B��ʱ������ */
      pr = (unsigned char*)isirw_malloc(psgi->width);
      pg = (unsigned char*)isirw_malloc(psgi->width);
      pb = (unsigned char*)isirw_malloc(psgi->width);
      pa = (unsigned char*)isirw_malloc(psgi->width);

      if (!(pr && pg && pb && pa)) {
        b_status = ER_MEMORYERR;
        __leave;
        break;
      }

      /* ����R��G��B�������������е�ƫ�� */
      fr = sizeof(SGIHEADER);
      fg = fr + psgi->width * psgi->hight;
      fb = fg + psgi->width * psgi->hight;
      fa = fb + psgi->width * psgi->hight;

      /* ���ж�ȡ�������� */
      for (i = 0; i < (int)psgi->hight; i++) {
        /* ��ɫ���� */
        if (isio_seek(pfile, fr, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)pr, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ��ɫ���� */
        if (isio_seek(pfile, fg, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)pg, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ��ɫ���� */
        if (isio_seek(pfile, fb, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)pb, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ALPHAͨ������ */
        if (isio_seek(pfile, fa, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)pa, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        _to_rgba(pr, pg, pb, pa, p, psgi->width);

        p += linesize;

        fr += (long)psgi->width;
        fg += (long)psgi->width;
        fb += (long)psgi->width;
        fa += (long)psgi->width;

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

      break;

    default:
      assert(0);
      b_status = ER_SYSERR;
      __leave;
      break;
    }

  }
  __finally {
    if (pr) {
      isirw_free(pr);
    }

    if (pg) {
      isirw_free(pg);
    }

    if (pb) {
      isirw_free(pb);
    }

    if (pa) {
      isirw_free(pa);
    }
  }

  return b_status;
}


/* ��RLE8ѹ��ͼ�����ݵ����ݰ��� */
static enum EXERESULT CALLAGREEMENT _read_rle8(ISFILE* pfile, LPINFOSTR pinfo_str, LPSGIHEADER psgi)
{
  int       i, linesize;
  unsigned char* p;
  unsigned char* pr = 0, *pg = 0, *pb = 0, *pa = 0, *ptmp = 0;
  unsigned long* pofftab = 0, *plentab = 0;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(pfile && pinfo_str && psgi);

  __try {
    if (isio_seek(pfile, sizeof(SGIHEADER), SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      __leave;
    }

    /* ����RLE��ѹ����ʱ������ */
    if ((ptmp = (unsigned char*)isirw_malloc(psgi->width * 2 + 4)) == 0) {
      b_status = ER_MEMORYERR;
      __leave;
    }

    /* �������ڴ洢RLE��ƫ�Ƽ��г��ȵĻ����� */
    pofftab = (unsigned long*)isirw_malloc(psgi->hight * psgi->channel * sizeof(long));
    plentab = (unsigned long*)isirw_malloc(psgi->hight * psgi->channel * sizeof(long));

    if (!(pofftab && plentab)) {
      b_status = ER_MEMORYERR;
      __leave;
    }

    /* ����RLE��ƫ�Ʊ� */
    if (_read_long_tab(pfile, pofftab, psgi->hight * psgi->channel) == -1) {
      b_status = ER_FILERWERR;
      __leave;
    }

    /* ����RLE�г��ȱ� */
    if (_read_long_tab(pfile, plentab, psgi->hight * psgi->channel) == -1) {
      b_status = ER_FILERWERR;
      __leave;
    }

    linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
    p = pinfo_str->p_bit_data;

    switch (pinfo_str->bitcount) {
    case  8:
      assert((psgi->channel == 1) && (psgi->bpc == 1));

      /* ���ж�ȡ�������� */
      for (i = 0; i < (int)psgi->hight; i++) {
        if (isio_seek(pfile, pofftab[i], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ��ѹ��RLE�� */
        if (_uncompression_rle8(ptmp, (int)plentab[i], p, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        p += linesize;

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

      break;

    case  24:
      assert((psgi->channel == 3) && (psgi->bpc == 1));

      /* ����R��G��B��ʱ������ */
      pr = (unsigned char*)isirw_malloc(psgi->width);
      pg = (unsigned char*)isirw_malloc(psgi->width);
      pb = (unsigned char*)isirw_malloc(psgi->width);

      if (!(pr && pg && pb)) {
        b_status = ER_MEMORYERR;
        __leave;
        break;
      }

      /* ���ж�ȡ�������� */
      for (i = 0; i < (int)psgi->hight; i++) {
        /* ��ɫ���� */
        if (isio_seek(pfile, pofftab[i], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_uncompression_rle8(ptmp, (int)plentab[i], pr, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* ��ɫ���� */
        if (isio_seek(pfile, pofftab[i + (int)(psgi->hight)], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i + (int)(psgi->hight)], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_uncompression_rle8(ptmp, (int)plentab[i + (int)(psgi->hight)], pg, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* ��ɫ���� */
        if (isio_seek(pfile, pofftab[i + (int)(psgi->hight) * 2], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i + (int)(psgi->hight) * 2], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_uncompression_rle8(ptmp, (int)plentab[i + (int)(psgi->hight) * 2], pb, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        _to_rgb(pr, pg, pb, p, psgi->width);

        p += linesize;

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

      break;

    case  32:
      assert((psgi->channel == 4) && (psgi->bpc == 1));

      /* ����R��G��B��ʱ������ */
      pr = (unsigned char*)isirw_malloc(psgi->width);
      pg = (unsigned char*)isirw_malloc(psgi->width);
      pb = (unsigned char*)isirw_malloc(psgi->width);
      pa = (unsigned char*)isirw_malloc(psgi->width);

      if (!(pr && pg && pb && pa)) {
        b_status = ER_MEMORYERR;
        __leave;
        break;
      }

      /* ���ж�ȡ�������� */
      for (i = 0; i < (int)psgi->hight; i++) {
        /* ��ɫ���� */
        if (isio_seek(pfile, pofftab[i], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_uncompression_rle8(ptmp, (int)plentab[i], pr, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* ��ɫ���� */
        if (isio_seek(pfile, pofftab[i + (int)(psgi->hight)], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i + (int)(psgi->hight)], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_uncompression_rle8(ptmp, (int)plentab[i + (int)(psgi->hight)], pg, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* ��ɫ���� */
        if (isio_seek(pfile, pofftab[i + (int)(psgi->hight) * 2], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i + (int)(psgi->hight) * 2], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_uncompression_rle8(ptmp, (int)plentab[i + (int)(psgi->hight) * 2], pb, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* ALPHAͨ������ */
        if (isio_seek(pfile, pofftab[i + (int)(psgi->hight) * 3], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i + (int)(psgi->hight) * 3], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_uncompression_rle8(ptmp, (int)plentab[i + (int)(psgi->hight) * 3], pa, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        _to_rgba(pr, pg, pb, pa, p, psgi->width);

        p += linesize;

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

      break;

    default:
      assert(0);
      b_status = ER_SYSERR;
      __leave;
      break;
    }

  }
  __finally {
    if (pr) {
      isirw_free(pr);
    }

    if (pg) {
      isirw_free(pg);
    }

    if (pb) {
      isirw_free(pb);
    }

    if (pa) {
      isirw_free(pa);
    }

    if (ptmp) {
      isirw_free(ptmp);
    }

    if (pofftab) {
      isirw_free(pofftab);
    }

    if (plentab) {
      isirw_free(plentab);
    }
  }

  return b_status;
}


/* ���R��G��B������RGB���� */
static void _to_rgb(unsigned char* pr, unsigned char* pg, unsigned char* pb, unsigned char* p, int width)
{
  int i;

  assert(pr && pg && pb && p && width);

  for (i = 0; i < width; i++) {
    *p++ = *pb++;
    *p++ = *pg++;
    *p++ = *pr++;
  }
}


/* ���R��G��B��A������RGBA���� */
static void _to_rgba(unsigned char* pr, unsigned char* pg, unsigned char* pb, unsigned char* pa, unsigned char* p, int width)
{
  int i;

  assert(pr && pg && pb && pa && p && width);

  for (i = 0; i < width; i++) {
    *p++ = *pb++;
    *p++ = *pg++;
    *p++ = *pr++;
    *p++ = *pa++;
  }
}


/* ��ȡlong�ͱ�ƫ�Ʊ���г��ȱ� */
static int _read_long_tab(ISFILE* pfile, unsigned long* pl, int count)
{
  int i;

  assert(pfile && pl);

  if (isio_read((void*)pl, count * sizeof(long), 1, pfile) == 0) {
    return -1;    /* �������� */
  }

  /* ת������ */
  for (i = 0; i < count; i++) {
    pl[i] = EXCHANGE_DWORD((pl[i]));
  }

  return 0;
}


/* ��ѹһ��RLE���� */
static int _uncompression_rle8(unsigned char* prle, int rlesize, unsigned char* p, int bufsize)
{
  int       bufcount = 0;
  unsigned char pix, count;

  while (1) {
    /* ����ֵ */
    pix = *prle++;
    rlesize--;

    /* ��ȡ����ֵ���������ֵΪ0����˵��RLE�н����� */
    if ((count = (pix & 0x7f)) == 0) {
      if ((bufcount == bufsize) && (rlesize == 0)) {
        return 0;
      }
      else {
        return -1;
      }
    }

    bufcount += (int)count;

    if (bufcount > bufsize) {
      return -1;    /* Դ�������� */
    }

    if (pix & 0x80) {
      if (rlesize < (int)count) {
        return -1;  /* ���� */
      }
      else {
        rlesize -= (int)count;
      }

      /* ����δѹ������ */
      while (count--) {
        *p++ = *prle++;
      }
    }
    else {
      if (rlesize == (int)0) {
        return -1;  /* ���� */
      }
      else {
        rlesize--;
      }

      pix = *prle++;

      /* �����ظ����� */
      while (count--) {
        *p++ = pix;
      }
    }

    if (bufcount == bufsize) {
      if (rlesize == 0) {
        return 0;  /* �ɹ� */
      }
      else if ((rlesize = 1) && (((*prle) & 0x7f) == 0)) {
        return 0;  /* �ɹ� */
      }
      else {
        return -1;
      }
    }
  }

  return -1;        /* ���� */
}


/* ��ȡͼ��λ���� */
SGI_API int CALLAGREEMENT sgi_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       i, linesize;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&sgi_load_img_critical);

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

      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ��ȡ��ͷ�ṹ */
      if (isio_read((void*)&sgi_header, sizeof(SGIHEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ת������ */
      sgi_header.magic  = EXCHANGE_WORD((sgi_header.magic));
      sgi_header.dim    = EXCHANGE_WORD((sgi_header.dim));
      sgi_header.width  = EXCHANGE_WORD((sgi_header.width));
      sgi_header.hight  = EXCHANGE_WORD((sgi_header.hight));
      sgi_header.channel = EXCHANGE_WORD((sgi_header.channel));

      sgi_header.pixmin = EXCHANGE_DWORD((sgi_header.pixmin));
      sgi_header.pixmax = EXCHANGE_DWORD((sgi_header.pixmax));
      sgi_header.palid  = EXCHANGE_DWORD((sgi_header.palid));

      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        /* �ж��Ƿ�����Ч����֧�֣���SGIͼ���� */
        if ((sgi_header.magic != SGI_MAGIC) ||  \
            (sgi_header.width == 0)   ||    \
            (sgi_header.hight == 0)   ||    \
            (sgi_header.channel == 0) ||    \
            (sgi_header.channel == 2) ||    \
            (sgi_header.channel > 4)  ||    \
            (sgi_header.palid != 0)   ||    \
            (sgi_header.dim == 0)   ||    \
            (sgi_header.dim > 3)    ||    \
            (sgi_header.bpc != 1)) {
          b_status = ER_NONIMAGE;
          __leave;
        }

        /* ��дͼ����Ϣ */
        pinfo_str->imgtype    = IMT_RESSTATIC;  /* ͼ�������� */
        pinfo_str->imgformat  = IMF_SGI;      /* ͼ������ʽ����׺���� */

        pinfo_str->compression  = (sgi_header.storage == 0) ? ICS_RGB : ICS_RLE8;

        pinfo_str->width    = (unsigned long)sgi_header.width;
        pinfo_str->height   = (unsigned long)sgi_header.hight;
        pinfo_str->order    = 1;        /* SGI ͼ��Ϊ����ͼ */
        pinfo_str->bitcount   = (unsigned long)(sgi_header.bpc * sgi_header.channel * 8);

        /* ������������ */
        switch (pinfo_str->bitcount) {
        case  8:
          pinfo_str->r_mask = 0x0;
          pinfo_str->g_mask = 0x0;
          pinfo_str->b_mask = 0x0;
          pinfo_str->a_mask = 0x0;
          break;

        case  24:
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->b_mask = 0xff;
          pinfo_str->a_mask = 0x0;
          break;

        case  32:
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->b_mask = 0xff;
          pinfo_str->a_mask = 0xff000000;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          __leave;
          break;
        }

        pinfo_str->data_state = 1;
      }


      /* �ϳɻҶȵ�ɫ�� */
      if (pinfo_str->bitcount == 8) {
        pinfo_str->pal_count = 256;

        for (i = 0; i < 256; i++) {
          pinfo_str->palette[i] = (unsigned long)((i << 16) | (i << 8) | (i << 0));
        }
      }
      else {
        pinfo_str->pal_count = 0;
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

      /* ��ʼ�����׵�ַ���飨���� */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
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

      /* ��ȡͼ�����ݰ� */
      if (sgi_header.storage == 0) {
        b_status = _read_uncom(pfile, pinfo_str, &sgi_header);
      }
      else {
        b_status = _read_rle8(pfile, pinfo_str, &sgi_header);
      }

      if (b_status != ER_SUCCESS) {
        __leave;
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

      LeaveCriticalSection(&sgi_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* ����ͼ�� */
SGI_API int CALLAGREEMENT sgi_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;

  unsigned char* pbuf = 0;
  int       i, j;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&sgi_save_img_critical);

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
      if (_sgi_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* �������ͬ��������������������ȷ����ʾ */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* Դ���ݱ����Ǳ�׼��24λͼ�� */
      assert((pinfo_str->r_mask == 0xff0000) && (pinfo_str->g_mask == 0xff00) && (pinfo_str->b_mask == 0xff));

      /* ��д��ͷ�ṹ��ת��ΪMOTO���� */
      sgi_header.magic  = EXCHANGE_WORD((SGI_MAGIC));
      sgi_header.dim    = EXCHANGE_WORD((3));
      sgi_header.width  = EXCHANGE_WORD(((unsigned short)pinfo_str->width));
      sgi_header.hight  = EXCHANGE_WORD(((unsigned short)pinfo_str->height));
      sgi_header.channel = EXCHANGE_WORD((3));

      sgi_header.pixmin = EXCHANGE_DWORD((0));
      sgi_header.pixmax = EXCHANGE_DWORD((0xff));
      sgi_header.palid  = EXCHANGE_DWORD((0));

      sgi_header.storage = 0;
      sgi_header.bpc    = 1;

      memset((void*)sgi_header.rev0, 0, SGI_REV0_SIZE);
      memset((void*)sgi_header.rev1, 0, SGI_REV1_SIZE);
      memcpy((void*)sgi_header.imgname, (const void*)"iseeexplorer image.", 20);

      /* д���ļ�ͷ�ṹ */
      if (isio_write((const void*)&sgi_header, sizeof(SGIHEADER), 1, pfile) == 0) {
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

      /* ������ɫ���������� */
      if ((pbuf = (unsigned char*)isirw_malloc(pinfo_str->width + 4)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* д��R�������� */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        for (j = 0; j < (int)pinfo_str->width; j++) {
          pbuf[j] = ((unsigned char*)(pinfo_str->pp_line_addr[((int)pinfo_str->height) - i - 1]))[j * 3 + 2];
        }

        if (isio_write((const void*)pbuf, pinfo_str->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }
      }

      /* д��G�������� */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        for (j = 0; j < (int)pinfo_str->width; j++) {
          pbuf[j] = ((unsigned char*)(pinfo_str->pp_line_addr[((int)pinfo_str->height) - i - 1]))[j * 3 + 1];
        }

        if (isio_write((const void*)pbuf, pinfo_str->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }
      }

      /* д��B�������� */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        for (j = 0; j < (int)pinfo_str->width; j++) {
          pbuf[j] = ((unsigned char*)(pinfo_str->pp_line_addr[((int)pinfo_str->height) - i - 1]))[j * 3 + 0];
        }

        if (isio_write((const void*)pbuf, pinfo_str->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
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
      if (pbuf) {
        isirw_free(pbuf);
      }

      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }
      else {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }

      LeaveCriticalSection(&sgi_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


