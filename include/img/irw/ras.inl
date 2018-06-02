/********************************************************************
 ras.c
 ���ļ���;�� Sun RASͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ��ɶ�ȡ1��8��24��32λѹ����δѹ��RASͼ��
 ���湦�ܣ��ṩ24λ��ѹ����ʽ�ı��湦��
 ���ļ���д�ˣ�
 netmap beahuang##hotmail.com
 YZ yzfree##yeah.net
 ���ļ��汾�� 30628
 ����޸��ڣ� 2003-6-28
 ----------------------------------------------------------------
 ע�����ļ��Ĳ��ֺ����ο���FreeImageͼ����Դ���룬�ڴ���
 Floris van den Berg (flvdberg@wxs.nl)
 Herv Drolon (drolon@iut.univ-lehavre.fr)
 ��ʾ��л������������˽�FreeImageͼ������������
 �����Ĺٷ���վ��
 http://www.6ixsoft.com/
 ----------------------------------------------------------------
 ������ʷ��
 2003-6 ȥ����32λͼ���ȡʱ��������루��β����йصĴ��룩(EhomSoft zhouwei##ehom.net)
 2003-4 ֧��ͼ���д�����ṩ�����ػ���������
 2002-9 ����ӿ�������2.2�������ڴ�������
 2002-6 ��һ���汾�������°棩
********************************************************************/

/* �ļ��б��־ */
#define RAS_MAGIC           0x59a66a95

/* �ļ��ṹ���� */
#define RT_OLD          0     /* δѹ����ʽ */
#define RT_STANDARD       1     /* δѹ����ʽ */
#define RT_BYTE_ENCODED     2     /* RLEѹ����ʽ */
#define RT_FORMAT_RGB     3     /* RGB��ʽ */
#define RT_FORMAT_TIFF      4
#define RT_FORMAT_IFF     5

#define RT_EXPERIMENTAL     0xFFFF

#define RMT_NONE        0     /* �޵�ɫ������ */
#define RMT_EQUAL_RGB     1
#define RMT_RAW         2

#define RESC          0x80    /* RLEѹ����ʶ�� */
#define RAS_ROUNDROW      16      /* ɨ���ж���λ�� */


/* RASͼ��ͷ�ṹ */
typedef struct _SUNHeaderinfo {
  ulong ras_magic;        /* �б��־ */
  ulong ras_width;        /* ��� */
  ulong ras_height;       /* �߶� */
  ulong ras_depth;        /* λ��� */
  ulong ras_length;       /* ͼ�����ݳ��ȣ�������ͷ�ṹ�����ܵĵ�ɫ�壩*/
  ulong ras_type;       /* ��ʽ���� */
  ulong ras_maptype;      /* ��ɫ�����ݴ�ŷ�ʽ */
  ulong ras_maplength;      /* ��ɫ�峤�ȣ��ֽڼƣ�*/
} SUNHeaderinfo, *LPSUNHeaderinfo;

/* �ڲ����ֺ��� */
int _ras_is_valid_img(INFOSTR* pinfo_str);
enum EXERESULT _read_data(ISFILE*, uchar*, long, int, int);

/* ��ȡͼ����Ϣ */
EXERESULT ras_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  SUNHeaderinfo header;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */

    do {
      /* ��ȡͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&header, sizeof(SUNHeaderinfo), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ת��X86���� */
      header.ras_magic = EXCHANGE_DWORD((header.ras_magic));
      header.ras_width = EXCHANGE_DWORD((header.ras_width));
      header.ras_height = EXCHANGE_DWORD((header.ras_height));
      header.ras_depth = EXCHANGE_DWORD((header.ras_depth));
      header.ras_length = EXCHANGE_DWORD((header.ras_length));
      header.ras_type = EXCHANGE_DWORD((header.ras_type));
      header.ras_maptype = EXCHANGE_DWORD((header.ras_maptype));
      header.ras_maplength = EXCHANGE_DWORD((header.ras_maplength));
      /* �ж��Ƿ���RASͼ�� */
      if (header.ras_magic != RAS_MAGIC) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* ͼ�����������Ƿ�Ϸ���RT_FORMAT_TIFF��RT_FORMAT_IFF��
      RT_EXPERIMENTAL��ʽ������֧�֣�*/
      if ((header.ras_type != RT_OLD) &&
          (header.ras_type != RT_STANDARD) &&
          (header.ras_type != RT_BYTE_ENCODED) &&
      (header.ras_type != RT_FORMAT_RGB)) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* �жϳߴ�ֵ�Ƿ�Ϸ� */
      if ((header.ras_width == 0) || (header.ras_height == 0) || (header.ras_depth == 0)) {
        b_status = ER_BADIMAGE;
        break;
      }
      switch (header.ras_depth) {
      case 1:
      case 8:
      case 24:
      case 32:
        break;
      default:
        b_status = ER_BADIMAGE;
        break; /* �������֧?��������ָ�ʽ֮���ͼ� */
 }
 pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
        pinfo_str->imgformat = IMF_RAS; /* ͼ���ļ���ʽ����׺���� */
        switch (header.ras_type) { /* ͼ���ѹ����ʽ */
        case RT_OLD:
        case RT_STANDARD:
        case RT_FORMAT_RGB:
          pinfo_str->compression = ICS_RGB;
          break;
        case RT_BYTE_ENCODED:
          pinfo_str->compression = ICS_RLE8;
          break;
        default:
          assert(0);
          b_status = ER_SYSERR;
          break;
        }
        /* ��дͼ����Ϣ */
        pinfo_str->width = header.ras_width;
        pinfo_str->height = header.ras_height;
        pinfo_str->order = 0;
        pinfo_str->bitcount = header.ras_depth;
        /* ��д�������� */
        switch (pinfo_str->bitcount) {
        case 1:
        case 8:
          SETMASK_8(pinfo_str);
          break;
        case 24:
          SETMASK_24(pinfo_str);
          break;
        case 32:
          SETMASK_32(pinfo_str);
          break;
        default:
          assert(0);
          b_status = ER_SYSERR;
          break;
        }
        /* �趨���ݰ�״̬ */
        pinfo_str->data_state = 1;
      }
      while (0);


    return b_status;
}
  /* ��ȡͼ��λ���� */
EXERESULT ras_load_image(ISFILE * pfile, INFOSTR* pinfo_str) {
    int linesize, linelength, fill, i, j, numcolors, isrle, isrgb, fillchar;
    uchar c, *r = 0, *g, *b, *colormap = 0, *buf = 0, *bp, *bits;
    SUNHeaderinfo header;
    PALITEM* pal;
    enum EXERESULT b_status = ER_SUCCESS;
    assert(pfile && pinfo_str);
    assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */
    
      do {
        /* ���ļ�ͷ�ṹ */
        if (isio_seek(pfile, 0, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        if (isio_read((void*)&header, sizeof(SUNHeaderinfo), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ת��X86���� */
        header.ras_magic = EXCHANGE_DWORD((header.ras_magic));
        header.ras_width = EXCHANGE_DWORD((header.ras_width));
        header.ras_height = EXCHANGE_DWORD((header.ras_height));
        header.ras_depth = EXCHANGE_DWORD((header.ras_depth));
        header.ras_length = EXCHANGE_DWORD((header.ras_length));
        header.ras_type = EXCHANGE_DWORD((header.ras_type));
        header.ras_maptype = EXCHANGE_DWORD((header.ras_maptype));
        header.ras_maplength = EXCHANGE_DWORD((header.ras_maplength));
        /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
        if (pinfo_str->data_state == 0) {
          if (header.ras_magic != RAS_MAGIC) {
            b_status = ER_NONIMAGE;
            break;
          }
          if ((header.ras_type != RT_OLD) &&
              (header.ras_type != RT_STANDARD) &&
              (header.ras_type != RT_BYTE_ENCODED) &&
          (header.ras_type != RT_FORMAT_RGB)) {
            b_status = ER_BADIMAGE;
            break;
          }
          if ((header.ras_width == 0) || (header.ras_height == 0) || (header.ras_depth == 0)) {
            b_status = ER_BADIMAGE;
            break;
          }
          switch (header.ras_depth) {
          case 1:
          case 8:
          case 24:
          case 32:
            break;
          default:
            b_status = ER_BADIMAGE;
            break; /* �������֧���������ָ�ʽ֮���ͼ��*/
          }
          pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
          pinfo_str->imgformat = IMF_RAS; /* ͼ���ļ���ʽ����׺���� */
          switch (header.ras_type) { /* ͼ���ѹ����ʽ */
          case RT_OLD:
          case RT_STANDARD:
          case RT_FORMAT_RGB:
            pinfo_str->compression = ICS_RGB;
            break;
          case RT_BYTE_ENCODED:
            pinfo_str->compression = ICS_RLE8;
            break;
          default:
            assert(0);
            b_status = ER_SYSERR;
            break;
          }
          pinfo_str->width = header.ras_width;
          pinfo_str->height = header.ras_height;
          pinfo_str->order = 0;
          pinfo_str->bitcount = header.ras_depth;
          switch (pinfo_str->bitcount) {
          case 1:
          case 8:
            SETMASK_8(pinfo_str);
            break;
          case 24:
            SETMASK_24(pinfo_str);
            break;
          case 32:
            SETMASK_32(pinfo_str);
            break;
          default:
            assert(0);
            b_status = ER_SYSERR;
            break;
          }
          pinfo_str->data_state = 1;
        }
        isrle = 0;
        isrgb = 0;
        /* �趨�������ݴ�ŷ�ʽ���Ƿ�ѹ������RGB�ŷ�˳��*/
        switch (header.ras_type) {
        case RT_OLD:
        case RT_STANDARD: /* ��ѹ����BGR���� */
          break;
        case RT_BYTE_ENCODED: /* ѹ�� */
          isrle = 1;
          break;
        case RT_FORMAT_RGB: /* ��ѹ����RGB���� */
          isrgb = 1;
          break;
        case RT_FORMAT_TIFF: /* ��֧�ֵĴ洢��ʽ */
        case RT_FORMAT_IFF:
        default:
          b_status = ER_BADIMAGE;
          break;
        }
        /* ���õ�ɫ������ */
        switch (pinfo_str->bitcount) {
        case 1:
        case 8:
          pinfo_str->pal_count = numcolors = 1 << header.ras_depth;
          switch (header.ras_maptype) {
          case RMT_NONE: /* �˹��ϳɻҶȵ�ɫ�� */
            pal = (PALITEM*)pinfo_str->palette;
            for (i = 0; i < numcolors; i++) {
              c = (uchar)((255 * i) / (numcolors - 1));
              pal[i].red = c;
              pal[i].green = c;
              pal[i].blue = c;
              pal[i].reserved = 0;
            }
            break;
          case RMT_EQUAL_RGB: /* ��ȡ��ɫ������ */
            pal = (PALITEM*)pinfo_str->palette;
            /* �ж�ͼ���ɫ�峤�����ֵ�Ƿ�Ϸ� */
            if (header.ras_maplength > (3 * numcolors * sizeof(uchar))) {
              b_status = ER_BADIMAGE;
              break;
              break;
            }
            /* �������ܷ����ɫ�建���� */
            if ((r = (uchar*)isirw_malloc(3 * numcolors * sizeof(uchar))) == 0) {
              b_status = ER_MEMORYERR;
              break;
              break;
            }
            /* ?���ʵ�ʵ�ɫ������?*/
            numcolors = header.ras_maplength / 3;
            g = r + numcolors;
            b = g + numcolors;
            /* �����ɫ������ */
            if (isio_read((void*)r, header.ras_maplength, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
              break;
            }
            for (i = 0; i < numcolors; i++) {
              pal[i].red = r[i];
              pal[i].green = g[i];
              pal[i].blue = b[i];
              pal[i].reserved = 0;
            }
            break;
          case RMT_RAW: /* �������ɫ������ */
            /* δ�ܸ������ֵ�ɫ��ĸ�ʽ����ʱ���� */
            if ((colormap = (uchar*)isirw_malloc(header.ras_maplength * sizeof(uchar))) == 0) {
              b_status = ER_MEMORYERR;
              break;
              break;
            }
            if (isio_read((void*)colormap, header.ras_maplength, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
              break;
            }
            break;
          default: /* �Ƿ��ĵ�ɫ���ǩֵ */
            b_status = ER_BADIMAGE;
            break;
          }
          break;
        case 24:
        case 32:
          /* ���ͼ���д��ڵ�ɫ�����ݣ���˵����һ����������ͼ�� */
          if (header.ras_maptype != RMT_NONE) {
            b_status = ER_BADIMAGE;
            break;
          }
          pinfo_str->pal_count = 0;
          break;
        default:
          assert(0);
          b_status = ER_SYSERR;
          break;
        }
        /* ȡ��ɨ���гߴ� */
        linesize = DIBSCANLINE_WIDTHBYTES(pinfo_str->width * pinfo_str->bitcount);
        /* ����ͼ����� */
        pinfo_str->imgnumbers = 1;
        pinfo_str->psubimg = (SUBIMGBLOCK*)0;
        assert(pinfo_str->p_bit_data == (uchar*)0);
        /* ����Ŀ��ͼ���ڴ�飨+4 �� β����??�ֽڻ������� */
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
        /* ��ʼ�����׵�ַ���飨RASͼ��Ϊ���� */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
        }
        /* �ص�֪ͨ */
         
        if (header.ras_depth == 1) {
          linelength = (int)((header.ras_width / 8) + (header.ras_width % 8 ? 1 : 0));
        }
        else {
          linelength = (int)header.ras_width;
        }
        /* RASɨ����Ϊ˫�ֽڶ��� */
        fill = (linelength % 2) ? 1 : 0;
        /* ��ʼ����ȡ���� */
        _read_data(pfile, 0, 0, 0, 1);
        /* ���ж��� */
        switch (pinfo_str->bitcount) {
        case 1:
        case 8:
          for (i = 0; i < (int)pinfo_str->height; i++) {
            bits = pinfo_str->p_bit_data + i * linesize;
            /* ����һ�е����� */
            if ((b_status = _read_data(pfile, bits, linelength, isrle, 0)) != ER_SUCCESS) {
              break;
              break;
            }
            /* ����еĻ�����ȡ��β����ֽ� */
            if (fill) {
              if ((b_status = _read_data(pfile, (uchar*)&fillchar, fill, isrle, 0)) != ER_SUCCESS) {
                break;
                break;
              }
            }
            /* ���ȵ��� */
             
          }
          break;
        case 24:
          /* ���仺���� */
          if ((buf = (uchar*)isirw_malloc(pinfo_str->width * 3)) == 0) {
            b_status = ER_MEMORYERR;
            break;
          }
          for (i = 0; i < (int)pinfo_str->height; i++) {
            bits = pinfo_str->p_bit_data + i * linesize;
            if ((b_status = _read_data(pfile, buf, pinfo_str->width * 3, isrle, 0)) != ER_SUCCESS) {
              break;
              break;
            }
            bp = buf;
            for (j = 0; j < (int)pinfo_str->width; j++) {
              bits[2] = *bp++; /* red */
              bits[1] = *bp++; /* green */
              bits[0] = *bp++; /* blue */
              bits += 3;
            }
#if 0
            /* ������δ�����ʵ��ͼ���ʽ��ͻ��isrgbΪ��ʱ������ΪRGB���У�*/
            /* ��ʱ������δ��� */
            if (isrgb) { /* RGB���� */
              for (j = 0; j < (int)pinfo_str->width; j++) {
                bits[2] = *bp++; /* red */
                bits[1] = *bp++; /* green */
                bits[0] = *bp++; /* blue */
                bits += 3;
              }
            }
            else { /* BGR���� */
              for (j = 0; j < (int)pinfo_str->width; j++) {
                bits[0] = *bp++; /* blue */
                bits[1] = *bp++; /* green */
                bits[2] = *bp++; /* red */
                bits += 3;
              }
            }
#endif
            if (fill) {
              if ((b_status = _read_data(pfile, (uchar*)&fillchar, fill, isrle, 0)) != ER_SUCCESS) {
                break;
                break;
              }
            }
            /* ���ȵ��� */
             
          }
          break;
        case 32:
          /* ���仺���� */
          if ((buf = (uchar*)isirw_malloc(pinfo_str->width * 4)) == 0) {
            b_status = ER_MEMORYERR;
            break;
          }
          for (i = 0; i < (int)pinfo_str->height; i++) {
            bits = pinfo_str->p_bit_data + i * linesize;
            if ((b_status = _read_data(pfile, buf, pinfo_str->width * 4, isrle, 0)) != ER_SUCCESS) {
              break;
              break;
            }
            bp = buf;
            for (j = 0; j < (int)pinfo_str->width; j++) {
              bits[3] = *bp++; /* empty */
              bits[2] = *bp++; /* red */
              bits[1] = *bp++; /* green */
              bits[0] = *bp++; /* blue */
              bits += 4;
            }
#if 0
            /* ע��ԭ����� */
            if (isrgb) { /* RGB���� */
              for (j = 0; j < (int)pinfo_str->width; j++) {
                bits[3] = *bp++; /* empty */
                bits[2] = *bp++; /* red */
                bits[1] = *bp++; /* green */
                bits[0] = *bp++; /* blue */
                bits += 4;
              }
            }
            else { /* BGR���� */
              for (j = 0; j < (int)pinfo_str->width; j++) {
                bits[3] = *bp++; /* empty */
                bits[0] = *bp++; /* blue */
                bits[1] = *bp++; /* green */
                bits[2] = *bp++; /* red */
                bits += 4;
              }
            }
#endif
            /* ���ȵ��� */
             
          }
          break;
        default:
          assert(0);
          b_status = ER_SYSERR;
          break;
        }
        /* ��ͼ����Ƚ��� */
         
        pinfo_str->data_state = 2;
      }
      while (0);
      
        if (buf) {
          isirw_free(buf);
        }
        if (colormap) {
          isirw_free(colormap);
        }
        if (r) {
          isirw_free(r);
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
EXERESULT ras_save_image(ISFILE * pfile, INFOSTR * pinfo_str) {
    int i, j, wlen;
    uchar* bits, *pbuf = 0;
    SUNHeaderinfo header;
    enum EXERESULT b_status = ER_SUCCESS;
    assert(pfile && pinfo_str);
    assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
    assert(pinfo_str->p_bit_data);

      do {
        /* �ж��Ƿ�����֧�ֵ�ͼ������ */
        if (_ras_is_valid_img(pinfo_str) != 0) {
          b_status = ER_NSIMGFOR;
          break;
        }
        /* ��дͷ�ṹ */
        header.ras_magic = RAS_MAGIC;
        header.ras_width = (int)pinfo_str->width;
        header.ras_height = (int)pinfo_str->height;
        header.ras_depth = (int)pinfo_str->bitcount;
        header.ras_length = (header.ras_width * 3 + ((header.ras_width * 3) % 2)) * header.ras_height;
        header.ras_type = RT_STANDARD;
        header.ras_maptype = RMT_NONE;
        header.ras_maplength = 0;
        /* ת��68K���� */
        header.ras_magic = EXCHANGE_DWORD((header.ras_magic));
        header.ras_width = EXCHANGE_DWORD((header.ras_width));
        header.ras_height = EXCHANGE_DWORD((header.ras_height));
        header.ras_depth = EXCHANGE_DWORD((header.ras_depth));
        header.ras_length = EXCHANGE_DWORD((header.ras_length));
        header.ras_type = EXCHANGE_DWORD((header.ras_type));
        header.ras_maptype = EXCHANGE_DWORD((header.ras_maptype));
        header.ras_maplength = EXCHANGE_DWORD((header.ras_maplength));
        if (isio_seek(pfile, 0, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* д��ͷ�ṹ��Ŀ���� */
        if (isio_write((const void*)&header, sizeof(SUNHeaderinfo), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
         
        wlen = pinfo_str->width * 3 + ((pinfo_str->width * 3) % 2);
        /* ����д��������ɨ���У�*/
        if ((pbuf = (uchar*)isirw_malloc(wlen + 4)) == 0) {
          b_status = ER_MEMORYERR;
          break;
        }
        /* д��ͼ�����ݣ������ͼ�����ݱ�����ISeeλ����ʽ��������ܻ����ʧ�棩*/
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          bits = (uchar*)pinfo_str->pp_line_addr[i];
          /* �ߵ�R��B������ע���ĵ�������ʵ��ͼ���ʽ����������ACDSee��ʾЧ��Ϊ׼ */
          for (j = 0; j < (int)pinfo_str->width; j++) {
            pbuf[j * 3 + 0] = bits[j * 3 + 2];
            pbuf[j * 3 + 1] = bits[j * 3 + 1];
            pbuf[j * 3 + 2] = bits[j * 3 + 0];
          }
          /* д��Ŀ���� */
          if (isio_write((const void*)pbuf, wlen, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
           
        }
         
      }
      while (0);
      
        if (pbuf) {
          isirw_free(pbuf);
        }
    return b_status;
  }
  /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
  /* �ڲ��������� */

  /* �жϴ����ͼ���Ƿ���Ա����棨�粻֧�ֱ��湦�ܣ��ɽ�������ȥ���� */
  int _ras_is_valid_img(INFOSTR * pinfo_str) {
    enum {bitcount = (1UL << (24 - 1))};
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
  /* ��ȡָ����Ŀ������������������*/
  static enum EXERESULT _read_data(ISFILE * pfile,
      uchar * buf,
      long length,
      int rle,
  int init) {
    static uchar repchar, remaining;
    enum EXERESULT b_status = ER_SUCCESS;
    /* ��ʼ������ֵ */
    if (init) {
      repchar = remaining = 0; /* ��̬�������ظ�����ʱ��Ҫ��ʱ�ĳ�ʼ�� */
      return b_status;
    }
      do {
        /* �����RLEѹ�����ݣ����Ƚ��� */
        if (rle) {
          while (length--) {
            if (remaining) {
              remaining--;
              *(buf++) = repchar;
            }
            else {
              /* ��ȡ��ʶ�ֽ� */
              if (isio_read((void*)&repchar, 1, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                break;
              }
              /* �ж��Ƿ�����ѹ�����ܵ��ֶ� */
              if (repchar == RESC) {
                if (isio_read((void*)&remaining, 1, 1, pfile) == 0) {
                  b_status = ER_FILERWERR;
                  break;
                }
                /* ѹ������Ϊ1�ֽڵ�0x80 */
                if (remaining == 0) {
                  *(buf++) = RESC;
                }
                else { /* �ظ����� */
                  /* ��ȡ�ظ��ֽ� */
                  if (isio_read((void*)&repchar, 1, 1, pfile) == 0) {
                    b_status = ER_FILERWERR;
                    break;
                  }
                  *(buf++) = repchar;
                }
              }
              else { /* δѹ�����ֽ����� */
                *(buf++) = repchar;
              }
            }
          }
        }
        else { /* δѹ��ͼ�� */
          if (isio_read((void*)buf, length, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
      }
      while (0);
    return b_status;
  }

