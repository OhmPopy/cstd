/********************************************************************
 tga.c
 ���ļ���;�� TGAͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ�δѹ����8��15��16��24��32λͼ�񣬼�
 ����RLEѹ����8��15��16��24��32λͼ��
 ���湦�ܣ�δѹ����8��15��16��24��32λͼ��
 ���ļ���д�ˣ� ���¶� xiaoyueer##263.net
 YZ
 ���ļ��汾�� 30401
 ����޸��ڣ� 2003-4-1
 ----------------------------------------------------------------
 ������ʷ��
 2003-4 ֧��ͼ���д�����ṩ�����ػ���������
 2002-9 ����ӿ�������2.2�������ڴ�������
 2001-12 �����°���루�ӿ���ͼ���ȡ�ٶȣ�������
 �˱���ͼ��Ĺ��ܣ�
 2001-3 Ϊ���ģ���ͼ���ٶȶ��޸Ĵ���
 2000-8 ��һ�����԰淢��
********************************************************************/


/**************TGA ͼ������ *************/
typedef enum {
  TGA_NULL    = 0,
    TGA_UNCPSPAL  = 1,
    TGA_UNCPSCOLOR  = 2,
    TGA_UNCPSGRAY = 3,
    TGA_RLEPAL    = 9,
    TGA_RLECOLOR  = 10,
    TGA_RLEGRAY   = 11
} TGATYPE;

/**********TGA ͼ�����ݴ洢����(��bDescriptor��Ӧλ��) ***********/
typedef enum {
  TGA_HORZMIRROR = 0x10,
    TGA_VERTMIRROR = 0x20
} TGAMIRROR;



/**************TGA �ļ�ͷ���� ***************/
typedef struct {
  uchar  bID_Length;    /* ������Ϣ���� */
  uchar  bPalType;    /* ��ɫ����Ϣ */
  uchar  bImageType;    /* ͼ������(0,1,2,3,9,10,11) */
  ushort wPalFirstNdx;  /* ��ɫ���һ������ֵ */
  ushort wPalLength;    /* ��ɫ��������(�Ե�ɫ�嵥ԪΪ��λ) */
  uchar  bPalBits;    /* һ����ɫ�嵥λλ��(15,16,24,32) */
  ushort wLeft;     /* ͼ���������(��������) */
  ushort wBottom;     /* ͼ��׶�����(��������) */
  ushort wWidth;      /* ͼ���� */
  ushort wDepth;      /* ͼ�󳤶� */
  uchar  bBits;     /* һ������λ�� */
  uchar  bDescriptor;   /* ������������  */
} TGAHEADER, *LPTGAHEADER;

/**************TGA �ļ�β���� ***************/
typedef struct {
  ulong  eao;       /* ��չ��ƫ�� */
  ulong  ddo;       /* ��������ƫ�� */
  uchar  info[16];    /* TRUEVISION-XFILE �̱��ַ��� */
  uchar  period;      /* �ַ�"." */
  uchar  zero;      /* 0 */
} TGAFOOTER, *LPTGAFOOTER;


//IRWP_INFO tga_irwp_info; /* �����Ϣ�� */
char* tga_save_idinfo = "ISeeExplorer";
/* ����ͼ��ʱע��ı�־����Ϣ */


/* �ڲ����ֺ��� */
int _decoding_line(uchar* pdest, uchar* psrc, int rleunit, int destlen);
int _tga_is_valid_img(INFOSTR* pinfo_str);

/* ��ȡͼ����Ϣ */
EXERESULT tga_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  ulong stream_length;
  TGAHEADER tgaHeader;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */
    do {
      /* ��ȡ������ */
      stream_length = isio_length(pfile);
      /* ��ȡ�ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&tgaHeader, sizeof(TGAHEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* �ж��ļ�ͷ��ͼ�����ͱ�־ */
      switch (tgaHeader.bImageType) {
      case TGA_RLEPAL:
      case TGA_RLECOLOR:
      case TGA_RLEGRAY:
        /* ��Ч��ͼ��������Ӧ����ͷ�ṹ�ߴ磨�����жϣ� */
        if (stream_length < sizeof(TGAHEADER)) {
          b_status = ER_NONIMAGE;
          break;
        }
        break;
      case TGA_UNCPSPAL:
      case TGA_UNCPSCOLOR:
      case TGA_UNCPSGRAY:
        /* δѹ��ͼ��ɼ����ͼ�����ݵĳߴ磬�����жϵ��ܸ���ȷһЩ */
        if (stream_length < (sizeof(TGAHEADER) + tgaHeader.bID_Length + ((tgaHeader.bBits + 7) >> 3)*tgaHeader.wDepth * tgaHeader.wWidth + tgaHeader.wPalLength * ((tgaHeader.bPalBits + 7) >> 3))) {
          b_status = ER_NONIMAGE;
          break;
        }
        break;
      case TGA_NULL: /* δ����ͼ�����ݵ��ļ�����Ϊ��Чͼ�� */
      default: /* δ֪�ļ����� */
        b_status = ER_NONIMAGE;
        break;
      }
      /* ��������λ���Ƿ�Ϸ� */
      if ((tgaHeader.bBits != 8) && (tgaHeader.bBits != 15) && \
          (tgaHeader.bBits != 16) && (tgaHeader.bBits != 24) && \
      (tgaHeader.bBits != 32)) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* ��дͼ����Ϣ�ṹ */
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_TGA; /* ͼ���ļ���ʽ����׺���� */
      switch (tgaHeader.bImageType) { /* ͼ���ѹ����ʽ */
      case TGA_UNCPSPAL:
      case TGA_UNCPSCOLOR:
      case TGA_UNCPSGRAY:
        pinfo_str->compression = ICS_RGB;
        break;
      case TGA_RLEPAL:
      case TGA_RLECOLOR:
      case TGA_RLEGRAY:
        switch (tgaHeader.bBits) {
        case 8:
          pinfo_str->compression = ICS_RLE8;
          break;
        case 15:
        case 16:
          pinfo_str->compression = ICS_RLE16;
          break;
        case 24:
          pinfo_str->compression = ICS_RLE24;
          break;
        case 32:
          pinfo_str->compression = ICS_RLE32;
          break;
        }
        break;
      default:
        assert(0); /* ϵͳ���� */
        b_status = ER_SYSERR;
        break;
      }
      /* ��дͼ����Ϣ */
      pinfo_str->width = (ulong)tgaHeader.wWidth;
      pinfo_str->height = (ulong)tgaHeader.wDepth;
      pinfo_str->order = (tgaHeader.bDescriptor & TGA_VERTMIRROR) ? 0 : 1;
      pinfo_str->bitcount = (ulong)(tgaHeader.bBits == 15) ? 16 : tgaHeader.bBits;
      IF_LEAVE(_get_mask(tgaHeader.bBits, (&pinfo_str->b_mask), (&pinfo_str->g_mask), (&pinfo_str->r_mask), (&pinfo_str->a_mask)));
      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    while (0);
  
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT tga_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  ulong stream_length;
  TGAHEADER tgaHeader;
  int i, k, j, linesize, decoded;
  int pallen = 0, firstindex = 0, newlinepos = 0, ile = 0;
  ushort palentry;
  uchar* pbuf = 0;
  uchar* pbits;
  uchar bpix;
  ushort wpix;
  ulong lpix;
  static uchar tmp_pal[1024]; /* ��ʱ��ɫ�建���� */
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */
  
    do {
      /* ��ȡ������ */
      stream_length = isio_length(pfile);
      /* ���ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&tgaHeader, sizeof(TGAHEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        /* �ж��ļ�ͷ��ͼ�����ͱ�־ */
        switch (tgaHeader.bImageType) {
        case TGA_RLEPAL:
        case TGA_RLECOLOR:
        case TGA_RLEGRAY:
          /* ��Ч��ͼ��������Ӧ����ͷ�ṹ�ߴ磨�����жϣ� */
          if (stream_length < sizeof(TGAHEADER)) {
            b_status = ER_NONIMAGE;
            break;
          }
          break;
        case TGA_UNCPSPAL:
        case TGA_UNCPSCOLOR:
        case TGA_UNCPSGRAY:
          /* δѹ��ͼ��ɼ����ͼ�����ݵĳߴ磬�����жϵ��ܸ���ȷһЩ */
          if (stream_length < (sizeof(TGAHEADER) + tgaHeader.bID_Length + ((tgaHeader.bBits + 7) >> 3)*tgaHeader.wDepth * tgaHeader.wWidth + tgaHeader.wPalLength * ((tgaHeader.bPalBits + 7) >> 3))) {
            b_status = ER_NONIMAGE;
            break;
          }
          break;
        case TGA_NULL: /* δ����ͼ�����ݵ��ļ�����Ϊ��Чͼ�� */
        default: /* δ֪�ļ����� */
          b_status = ER_NONIMAGE;
          break;
        }
        /* ��������λ���Ƿ�Ϸ� */
        if ((tgaHeader.bBits != 8) && (tgaHeader.bBits != 15) && \
            (tgaHeader.bBits != 16) && (tgaHeader.bBits != 24) && \
        (tgaHeader.bBits != 32)) {
          b_status = ER_BADIMAGE;
          break;
        }
        /* ��дͼ����Ϣ�ṹ */
        pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
        pinfo_str->imgformat = IMF_TGA; /* ͼ���ļ���ʽ����׺���� */
        switch (tgaHeader.bImageType) { /* ͼ���ѹ����ʽ */
        case TGA_UNCPSPAL:
        case TGA_UNCPSCOLOR:
        case TGA_UNCPSGRAY:
          pinfo_str->compression = ICS_RGB;
          break;
        case TGA_RLEPAL:
        case TGA_RLECOLOR:
        case TGA_RLEGRAY:
          switch (tgaHeader.bBits) {
          case 8:
            pinfo_str->compression = ICS_RLE8;
            break;
          case 15:
          case 16:
            pinfo_str->compression = ICS_RLE16;
            break;
          case 24:
            pinfo_str->compression = ICS_RLE24;
            break;
          case 32:
            pinfo_str->compression = ICS_RLE32;
            break;
          }
          break;
        default:
          assert(0); /* ϵͳ���� */
          b_status = ER_SYSERR;
          break;
        }
        /* ��дͼ����Ϣ */
        pinfo_str->width = (ulong)tgaHeader.wWidth;
        pinfo_str->height = (ulong)tgaHeader.wDepth;
        pinfo_str->order = (tgaHeader.bDescriptor & TGA_VERTMIRROR) ? 0 : 1;
        pinfo_str->bitcount = (ulong)(tgaHeader.bBits == 15) ? 16 : tgaHeader.bBits;
        /* ��д������Ϣ */
        IF_LEAVE(_get_mask(tgaHeader.bBits, (&pinfo_str->b_mask), (&pinfo_str->g_mask),
            (&pinfo_str->r_mask), (&pinfo_str->a_mask)));
        pinfo_str->data_state = 1;
      }
      /* ���õ�ɫ������ */
      if (pinfo_str->bitcount == 8) {
        pinfo_str->pal_count = 256;
        /* ���ɻҶ�ͼ��ɫ�� */
        if ((tgaHeader.bImageType == TGA_UNCPSGRAY) || (tgaHeader.bImageType == TGA_RLEGRAY)) {
          for (i = 0; i < (int)pinfo_str->pal_count; i++) {
            pinfo_str->palette[i] = (i << 16) | (i << 8) | (i);
          }
        }
        else { /* ������ת����ɫ�����ݴ��ļ� */
          if (isio_seek(pfile, sizeof(TGAHEADER) + tgaHeader.bID_Length, SEEK_SET) == -1) {
            b_status = ER_FILERWERR;
            break;
          }
          /* TGA��ɫ���������ͨ��ARGB��ʽ��������Ҫ������жϹ��� */
          pallen = tgaHeader.wPalLength * ((tgaHeader.bPalBits + 7) >> 3);
          firstindex = tgaHeader.wPalFirstNdx;
          if (isio_read((void*)tmp_pal, pallen, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
          /* ת����ɫ������ */
          switch (tgaHeader.bPalBits) {
          case 15:
          case 16: /* ת��2�ֽڵ�ɫ������ */
            for (i = 0; i < pallen; i += 2) {
              if (firstindex >= 256) {
                b_status = ER_BADIMAGE;
                break;
              }
              palentry = *((ushort*)(tmp_pal + i));
              ((PALITEM*)(&pinfo_str->palette[firstindex]))->blue = (palentry & 0x1f) << 3;
              ((PALITEM*)(&pinfo_str->palette[firstindex]))->green = ((palentry >> 5) & 0x1f) << 3;
              ((PALITEM*)(&pinfo_str->palette[firstindex]))->red = ((palentry >> 10) & 0x1f) << 3;
              pinfo_str->palette[firstindex++] &= 0xffffff; /* ����ֽ����� */
            }
            break;
          case 24: /* ת��3�ֽڵ�ɫ������ */
            for (i = 0; i < pallen; i += 3) {
              if (firstindex >= 256) {
                b_status = ER_BADIMAGE;
                break;
              }
              ((PALITEM*)(&pinfo_str->palette[firstindex]))->blue = tmp_pal[i];
              ((PALITEM*)(&pinfo_str->palette[firstindex]))->green = tmp_pal[i + 1];
              ((PALITEM*)(&pinfo_str->palette[firstindex]))->red = tmp_pal[i + 2];
              pinfo_str->palette[firstindex++] &= 0xffffff;
            }
            break;
          case 32: /* ת��4�ֽڵ�ɫ������ */
            for (i = 0; i < pallen; i += 4) {
              if (firstindex >= 256) {
                b_status = ER_BADIMAGE;
                break;
              }
              pinfo_str->palette[firstindex] = *(ulong*)(tmp_pal + i);
              pinfo_str->palette[firstindex++] &= 0xffffff;
            }
            break;
          default:
            assert(0); /* ϵͳ���� */
            b_status = ER_SYSERR;
            break;
          }
        }
      }
      else {
        pinfo_str->pal_count = 0; /* �޵�ɫ������ */
      }
      /* ȡ��ɨ���гߴ� */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      /* ����ͼ����� */
      pinfo_str->imgnumbers = 1; /* TGA�ļ���ֻ��һ��ͼ��*/
      /* Ӧ������ԭʼͼ����ͼ(Postage Stamp)��*/
      /* �����¶���δ����ʵ�� */
      pinfo_str->psubimg = (SUBIMGBLOCK*)0; /* ��ͼ�����ÿ� */
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
      /* ����ͼ��ɨ���л����� */
      pbuf = (uchar*)isirw_malloc(pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3) + pinfo_str->width); /* ��������RLEѹ����û��һ���ظ����� */
      if (!pbuf) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* ��ʼ�����׵�ַ���� */
      if (pinfo_str->order == 0) { /* ���� */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
        }
      }
      else { /* ���� */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
        }
      }
      /* ��ȡͼ������ */
      /* ����ͼ���ļ���λ��ȵĲ�ͬ�����ò�ͬ�Ķ�ȡ���� */
      switch (tgaHeader.bImageType) {
      case TGA_UNCPSPAL:
      case TGA_UNCPSGRAY:
      case TGA_UNCPSCOLOR:
        /* ��һ��ͼ���������ļ��е���ʼλ�� */
        newlinepos = sizeof(TGAHEADER) + tgaHeader.bID_Length + pallen;
        if (isio_seek(pfile, newlinepos, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* �ļ�ɨ���г��� */
        ile = pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3);
        pbits = pinfo_str->p_bit_data;
        /* ����Ϊ��λ��ȡͼ������ */
        if (tgaHeader.bDescriptor & TGA_HORZMIRROR) { /* ���ҷ��� */
          for (i = 0; i < (int)pinfo_str->height; i++) {
            if (isio_read((void*)pbuf, ile, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
            }
            for (k = 0; k < (int)pinfo_str->width; k++) {
              switch (pinfo_str->bitcount) {
              case 8:
                ((uchar*)pbits)[k] = pbuf[pinfo_str->width - 1 - k];
                break;
              case 15:
              case 16:
                ((ushort*)pbits)[k] = ((ushort*)pbuf)[pinfo_str->width - 1 - k];
                break;
              case 24:
                for (j = 0; j < 3; j++) {
                  ((uchar*)pbits)[k * 3 + j] = pbuf[(pinfo_str->width - 1 - k) * 3 + j];
                }
                break;
              case 32:
                ((ulong*)pbits)[k] = ((ulong*)pbuf)[pinfo_str->width - 1 - k];
                break;
              default:
                assert(0); /* ϵͳ���� */
                b_status = ER_SYSERR;
                break;
                break;
              }
            }
            pbits = (uchar*)(pbits + linesize);
          }
        }
        else {
          for (i = 0; i < (int)pinfo_str->height; i++) {
            if (isio_read((void*)pbits, ile, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
            }
            pbits = (uchar*)(pbits + linesize);
          }
        }
        break;
      case TGA_RLEGRAY:
      case TGA_RLEPAL:
      case TGA_RLECOLOR:
        /* ��һ��ͼ���������ļ��е���ʼλ�� */
        newlinepos = sizeof(TGAHEADER) + tgaHeader.bID_Length + pallen;
        /* �ļ�ɨ���г��ȣ������������㣩 */
        ile = pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3) + pinfo_str->width;
        pbits = pinfo_str->p_bit_data;
        /* ����Ϊ��λ��ȡͼ������ */
        if (tgaHeader.bDescriptor & TGA_HORZMIRROR) { /* ���ҷ��� */
          for (i = 0; i < (int)pinfo_str->height; i++) {
            if (isio_seek(pfile, newlinepos, SEEK_SET) == -1) {
              b_status = ER_FILERWERR;
              break;
            }
            if (isio_read((void*)pbuf, 1, ile, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
            }
            /* ��ѹһRLEͼ���� */
            decoded = _decoding_line(pbits, pbuf, (pinfo_str->bitcount + 7) >> 3, pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3));
            if (decoded == -1) {
              b_status = ER_BADIMAGE;
              break;
            }
            assert(decoded);
            /* ��һ��ͼ���������ļ��е���ʼλ�� */
            newlinepos += decoded;
            /* ���ҵߵ�ͼ������ */
            for (k = 0; k < (int)pinfo_str->width; k++) {
              switch (pinfo_str->bitcount) {
              case 8:
                bpix = ((uchar*)pbits)[k];
                ((uchar*)pbits)[k] = ((uchar*)pbits)[pinfo_str->width - 1 - k];
                ((uchar*)pbits)[pinfo_str->width - 1 - k] = bpix;
                break;
              case 15:
              case 16:
                wpix = ((ushort*)pbits)[k];
                ((ushort*)pbits)[k] = ((ushort*)pbits)[pinfo_str->width - 1 - k];
                ((ushort*)pbits)[pinfo_str->width - 1 - k] = wpix;
                break;
              case 24:
                for (j = 0; j < 3; j++) {
                  bpix = ((uchar*)pbits)[k * 3 + j];
                  ((uchar*)pbits)[k * 3 + j] = ((uchar*)pbits)[(pinfo_str->width - 1 - k) * 3 + j];
                  ((uchar*)pbits)[(pinfo_str->width - 1 - k) * 3 + j] = bpix;
                }
                break;
              case 32:
                lpix = ((ulong*)pbits)[k];
                ((ulong*)pbits)[k] = ((ulong*)pbits)[pinfo_str->width - 1 - k];
                ((ulong*)pbits)[pinfo_str->width - 1 - k] = lpix;
                break;
              default:
                assert(0); /* ϵͳ���� */
                b_status = ER_SYSERR;
                break;
                break;
              }
            }
            pbits = (uchar*)(pbits + linesize);
          }
        }
        else {
          for (i = 0; i < (int)pinfo_str->height; i++) {
            if (isio_seek(pfile, newlinepos, SEEK_SET) == -1) {
              b_status = ER_FILERWERR;
              break;
            }
            if (isio_read((void*)pbuf, 1, ile, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
            }
            /* ��ѹһRLEͼ���� */
            decoded = _decoding_line(pbits, pbuf, (pinfo_str->bitcount + 7) >> 3, pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3));
            if (decoded == -1) {
              b_status = ER_BADIMAGE;
              break;
            }
            assert(decoded);
            /* ��һ��ͼ���������ļ��е���ʼλ�� */
            newlinepos += decoded;
            pbits = (uchar*)(pbits + linesize);
          }
        }
        break;
      default:
        assert(0); /* ϵͳ���� */
        b_status = ER_SYSERR;
        break;
      }
      pinfo_str->data_state = 2;
    }
    while (0);
    
      if (pbuf) {
        isirw_free(pbuf);
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
EXERESULT tga_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  TGAHEADER tgaHeader;
  TGAFOOTER tgaFooter;
  int i, wrlinesize;
  uchar* pbits;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);
  
    do {
      /* �ж��Ƿ�����֧�ֵ�ͼ������ */
      if (_tga_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        break;
      }
      /* ������Ϣ���ȣ��˴�������һ��"ISeeExplorer"�ַ��� */
      tgaHeader.bID_Length = (uchar)strlen((const char*)tga_save_idinfo);
      /* ��ɫ����Ϣ���Ҷ�ͼ������Ҳ����������ɫ��ͼ�� */
      tgaHeader.bPalType = (uchar)(pinfo_str->bitcount == 8) ? 1 : 0;
      /* ͼ������(ֻ��1��2��������) */
      tgaHeader.bImageType = (uchar)(pinfo_str->bitcount == 8) ? TGA_UNCPSPAL : TGA_UNCPSCOLOR;
      /* ��ɫ���һ������ֵ */
      tgaHeader.wPalFirstNdx = 0;
      /* ��ɫ��������(�Ե�ɫ�嵥ԪΪ��λ) */
      tgaHeader.wPalLength = (ushort)(pinfo_str->bitcount == 8) ? 256 : 0;
      /* һ����ɫ�嵥λλ��(ISee��ɫ������32λ��) */
      tgaHeader.bPalBits = 32;
      /* ͼ�����¶����� */
      tgaHeader.wLeft = 0;
      tgaHeader.wBottom = 0;
      /* ͼ����߶� */
      tgaHeader.wWidth = (ushort)pinfo_str->width;
      tgaHeader.wDepth = (ushort)pinfo_str->height;
      /* ����λ�� */
      tgaHeader.bBits = (uchar)pinfo_str->bitcount;
      /* ����������??������Aͨ����һ��Ϊ����ͼ) */
      tgaHeader.bDescriptor = 0;
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* д���ļ�ͷ�ṹ */
      if (isio_write((const void*)&tgaHeader, sizeof(TGAHEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* д����Ϣ */
      if (isio_write((const void*)tga_save_idinfo, strlen(tga_save_idinfo), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* д���ɫ������ */
      if (tgaHeader.bPalType == 1) {
        if (isio_write((const void*)pinfo_str->palette, pinfo_str->pal_count * 4, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
      }
      /* TGAδѹ��ɨ���гߴ粢����4�ֽڶ���ģ�������Ҫ��ʵ�����ؼ����гߴ� */
      wrlinesize = pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3);
      /* д��ͼ��λ����
      ע�������ͼ�����ݱ�����ISeeλ����ʽ��������ܻ����ʧ��
      */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        /* һ�ɱ���Ϊ����ͼ */
        pbits = (uchar*)(pinfo_str->pp_line_addr[pinfo_str->height - 1 - i]);
        if (isio_write((const void*)pbits, wrlinesize, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
      }
      /* ��д�ļ�β����Ϣ���°�TGA��ʽ��*/
      tgaFooter.eao = 0;
      tgaFooter.ddo = 0;
      memcpy((void*)tgaFooter.info, (const void*)"TRUEVISION-XFILE", 16);
      tgaFooter.period = '.';
      tgaFooter.zero = 0;
      if (isio_write((const void*)&tgaFooter, sizeof(TGAFOOTER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
    }
    while (0);
    
  return b_status;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* �н�ѹ������psrcʵ�ʽ�ѹ�ֽ�������Դ�������󣬷���-1 */
int _decoding_line(
    uchar* pdest, /* Ŀ�껺���� */
    uchar* psrc, /* RLEԴ���� */
    int rleunit, /* rleunitΪ��ѹ��λ(in Bytes) */
    int destlen /* destlenΪ��ѹĿ�곤�� */
)
{
  int i, j, nCount;
  uchar* pdestEnd = pdest + destlen, *psrcOrg = psrc, *pdestOrg = pdest;
  uchar* ptmp;
  assert(psrc);
  assert(pdest);
  while (pdest < pdestEnd) {
    if (psrc[0] & 0x80) { /* �ظ����� */
      nCount = (psrc[0] & 0x7F) + 1;
      psrc++;
      for (i = 0; i < nCount; i++) {
        ptmp = psrc;
        for (j = 0; j < rleunit; j++) { /* ��ѹһ������(length equ rleunit) */
          *pdest++ = *ptmp++;
        }
      }
      psrc += rleunit;
    }
    else { /* ���ظ����� */
      nCount = (psrc[0] & 0x7F) + 1;
      psrc++;
      memcpy(pdest, psrc, nCount * rleunit); /* ���ظ�����ĳߴ���ܻ�Ƚϴ�����memcpy��Ч�ʿ��ܸ��� */
      pdest += nCount * rleunit;
      psrc += nCount * rleunit;
    }
  }
  if (pdest != pdestEnd) { /* ͼ�����ݷǷ������� -1 */
    return -1;
  }
  return psrc - psrcOrg; /* ��ѹ�ɹ������ر���ѹ�ֽ��� */
}
/* �жϴ����ͼ���Ƿ���Ա����� */
int _tga_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = (1UL << (8 - 1)) | (1UL << (16 - 1)) | (1UL << (24 - 1)) | (1UL << (32 - 1))};
  /* ## λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1; /* ��֧�ֵ�λ��ͼ�� */
  }
  assert(pinfo_str->imgnumbers);
  if (pinfo_str->imgnumbers != 1) {
    return -2; /* ͼ���������ȷ */
  }
  return 0;
}

