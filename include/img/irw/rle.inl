/********************************************************************
 rle.c
 ���ļ���;�� Utah RLEͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ��ɶ�ȡ8λ��24λ��32λRLEͼ�����
 ��������ɫ��ͨ����һ��ALPHAͨ����
 ���湦�ܣ��ɽ�8λ��24λͼ�󱣴�ΪRLEͼ�󣨲�
 ѹ����ʽ��
 ���ļ���д�ˣ�
 YZ yzfree##yeah.net
 ���ļ��汾�� 30401
 ����޸��ڣ� 2003-4-1
 ----------------------------------------------------------------
 ������ʷ��
 2003-4 ֧��ͼ���д�����ṩ�����ػ���������
 2002-9 ����ӿ�������2.2�������ڴ�������
 2002-5 ��һ�������棨�°棩
********************************************************************/

/* �ļ��б��־ */
#define RLE_MAGIC           ((ushort)0xcc52)

/* ���������б�λ */
#define     LONGOPCODE      0x40

/* RLE�������붨�� */
#define     RLE_SKIPLINE_OP   1
#define     RLE_SETCOLOR_OP   2
#define     RLE_SKIPPIXELS_OP 3
#define     RLE_BYTEDATA_OP   5
#define     RLE_RUNDATA_OP    6
#define     RLE_EOF_OP      7

#define     RLE_CLEARFIRST_F  0x1   /* ������õĻ�����ѹǰ�������������ʹ�ñ���ɫ�� */
#define     RLE_NO_BACKGROUND_F 0x2   /* ������õĻ���������δ�ṩ����ɫ��Ϣ */
#define     RLE_ALPHA_F     0x4   /* ������õĻ����������д���ALPHAͨ����ͨ����-1�� */
#define     RLE_COMMENT_F   0x8   /* ������õĻ����������д���ע����Ϣ */

#define   RLE_OPCODE(s)   (s[0] & ~(uchar)LONGOPCODE)
#define   RLE_LONGP(s)    (s[0] & (uchar)LONGOPCODE)
#define   RLE_DATUM(s)    (s[1] & (uchar)0xff)


/* RLEͷ�ṹ���� */
typedef struct {
  ushort  magic;        /* RLE�б��־��0xcc52�� */
  ushort  xpos;       /* ͼ���x��yԭ�����꣨���½ǣ���һ���ޣ�*/
  ushort  ypos;
  ushort  width;        /* ͼ��Ŀ�Ⱥ͸߶ȣ����ص�λ��*/
  ushort  height;
  uchar flags;        /* ��־λ */
  uchar channels;     /* ��ɫͨ������������ALPHAͨ�� */
  uchar chan_bits;      /* ÿ����ɫͨ����λ��ȣ���ǰ��Чֵֻ��8��*/
  uchar pal_chan_count;   /* ��ɫ�����ͨ���� */
  uchar pal_chan_bits;    /* ��ɫ��ͨ����λ��� */
} RLE_HEADER, *LPRLE_HEADER;

/* �ڲ����ֺ��� */
int _rle_is_valid_img(INFOSTR* pinfo_str);
static int _rle_pack(uchar* psou, int len, uchar* pdec);

/* ��ȡͼ����Ϣ */
EXERESULT rle_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  RLE_HEADER rle_header;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */

    do {
      /* ��ȡ�ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&rle_header, sizeof(RLE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* �ж�ָ����ͼ�����Ƿ���RLE��ʽ��ֻ֧��8��24��32λ1��3��4ͨ��ͼ�� */
      if ((rle_header.magic != RLE_MAGIC) || \
          ((rle_header.channels != 1) && (rle_header.channels != 3)) || \
      (rle_header.chan_bits != 8)) {
        b_status = ER_NONIMAGE;
        break;
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_RLE; /* ͼ���ļ���ʽ����׺���� */
      /* ��дͼ����Ϣ */
      pinfo_str->width = rle_header.width;
      pinfo_str->height = rle_header.height;
      pinfo_str->order = 1; /* RLEͼ���ǵ���ͼ����һ���ޣ�*/
      if (rle_header.flags & RLE_ALPHA_F) {
        /* �˴�δʹ��rle_header�е�chan_bits����Ϊ���ֵ����8 */
        if (rle_header.channels == 1) {
          b_status = ER_NONIMAGE;
          break; /* ��֧�ִ�ALPHAͨ����8λ����ͼ */
        }
        else {
          assert(rle_header.channels == 3);
          pinfo_str->bitcount = (rle_header.channels + 1) * 8;
        }
        assert(pinfo_str->bitcount == 32);
      }
      else {
        pinfo_str->bitcount = rle_header.channels * 8;
      }
      pinfo_str->compression = ICS_RLE8;
      /* ����ͼ���������� */
      switch (pinfo_str->bitcount) {
      case 8:
        pinfo_str->r_mask = 0;
        pinfo_str->g_mask = 0;
        pinfo_str->b_mask = 0;
        pinfo_str->a_mask = 0;
        break;
      case 24:
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->b_mask = 0xff;
        pinfo_str->a_mask = 0;
        break;
      case 32:
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->b_mask = 0xff;
        pinfo_str->a_mask = 0xff000000;
        break;
      }
      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    while (0);
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT rle_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  int i, j, k, linesize, end_mark;
  RLE_HEADER rle_header;
  char bg[4], filler, bgmark;
  ushort* pal_buf = 0, comm_len;
  uchar* pcomm = 0, *p, oper[2], *pbuf = 0;
  unsigned int xinc;
  ushort skiplines, skipcol, nc, cor;
  uchar currchann;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */
  do {
      /* ���ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&rle_header, sizeof(RLE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* �����ͼ��δ��?ù���ȡ��Ϣ����������дͼ�����?*/
      if (pinfo_str->data_state == 0) {
        if ((rle_header.magic != RLE_MAGIC) || \
            ((rle_header.channels != 1) && (rle_header.channels != 3)) || \
        (rle_header.chan_bits != 8)) {
          b_status = ER_NONIMAGE;
          break;
        }
        pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
        pinfo_str->imgformat = IMF_RLE; /* ͼ���ļ���ʽ����׺���� */
        /* ��дͼ����Ϣ */
        pinfo_str->width = rle_header.width;
        pinfo_str->height = rle_header.height;
        pinfo_str->order = 1;
        if (rle_header.flags & RLE_ALPHA_F) {
          if (rle_header.channels == 1) {
            b_status = ER_NONIMAGE;
            break;
          }
          else {
            assert(rle_header.channels == 3);
            pinfo_str->bitcount = (rle_header.channels + 1) * 8;
          }
          assert(pinfo_str->bitcount == 32);
        }
        else {
          pinfo_str->bitcount = rle_header.channels * 8;
        }
        pinfo_str->compression = ICS_RLE8;
        /* ����ͼ���������� */
        switch (pinfo_str->bitcount) {
        case 8:
          pinfo_str->r_mask = 0;
          pinfo_str->g_mask = 0;
          pinfo_str->b_mask = 0;
          pinfo_str->a_mask = 0;
          break;
        case 24:
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->b_mask = 0xff;
          pinfo_str->a_mask = 0;
          break;
        case 32:
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->b_mask = 0xff;
          pinfo_str->a_mask = 0xff000000;
          break;
        }
        pinfo_str->data_state = 1;
      }
      /* ��ȡ����ɫ��Ϣ */
      if (rle_header.flags & RLE_NO_BACKGROUND_F) {
        /* �������ֽ� */
        if (isio_read((void*)&filler, 1, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        bgmark = 0;
      }
      else {
        assert((rle_header.channels == 3) || (rle_header.channels == 1));
        memset((void*)bg, 0, 4);
        /* ��ȡ����ɫ */
        if (isio_read((void*)bg, rle_header.channels, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ȷ���Ƿ���Ҫ������䱳��ɫ */
        if (rle_header.flags & RLE_CLEARFIRST_F) {
          bgmark = 1;
        }
        else {
          bgmark = 0;
        }
      }
      /* ���õ�ɫ������ */
      if (pinfo_str->bitcount <= 8) {
        assert(rle_header.channels == 1);
        assert(pinfo_str->bitcount == 8); /* ��ǰֻ֧��8λ����ͼ */
        if ((rle_header.pal_chan_count != 3) || (rle_header.pal_chan_bits != 8)) {
          b_status = ER_BADIMAGE;
          break;
        }
        /* �����ɫ����?ݻ����?*/
        if ((pal_buf = (ushort*)isirw_malloc(256 * 3 * 2)) == 0) { /* 256�3ͨ����2�ֽ� */
          b_status = ER_MEMORYERR;
          break;
        }
        /* ��ȡ��ɫ������ */
        if (isio_read((void*)pal_buf, (256 * 3 * 2), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ת�������ݰ� */
        for (i = 0; i < 256; i++) {
          pinfo_str->palette[i] = (((ulong)(pal_buf[i] >> 8)) << 16) | \
              (((ulong)(pal_buf[256 + i] >> 8)) << 8) | \
              ((ulong)(pal_buf[512 + i] >> 8));
        }
        pinfo_str->pal_count = 256;
      }
      else {
        if (rle_header.pal_chan_count != 0) {
          b_status = ER_BADIMAGE;
          break; /* ������ͼ��Ӧ���ڵ�ɫ������ */
        }
        pinfo_str->pal_count = 0;
      }
      /* ��ȡע����Ϣ */
      if (rle_header.flags & RLE_COMMENT_F) {
        /* ��ȡע����Ϣ����ֵ��2�ֽڣ�*/
        if (isio_read((void*)&comm_len, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* ż������ */
        comm_len = (comm_len + 1) & (ushort)0xfffe;
        if (comm_len) {
          /* ����ע?���Ϣ�����?*/
          if ((pcomm = (uchar*)isirw_malloc(comm_len)) == 0) {
            b_status = ER_MEMORYERR;
            break;
          }
          /* ��ע����Ϣ */
          if (isio_read((void*)pcomm, comm_len, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
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
      /* ��д���׵�ַ���飨����ͼ��*/
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
      }
      /* �ص�֪ͨ */
       
      /* ��䱳��ɫ */
      if (bgmark) {
        k = (int)pinfo_str->bitcount / 8;
        for (i = 0; i < (int)pinfo_str->height; i++) {
          for (j = 0; j < (int)pinfo_str->width; j++) {
            memcpy((void*)((uchar*)pinfo_str->pp_line_addr[i] + j * k), (const void*)bg, k);
          }
        }
      }
      /* ����ɨ���л����� */
      if ((pbuf = (uchar*)isirw_malloc(pinfo_str->width * 3 + 4)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      currchann = 0;
      nc = 0;
      k = pinfo_str->bitcount / 8;
      end_mark = 0;
      /* ���벢��ѹ�������������ݰ� */
      for (i = 0; i < (int)pinfo_str->height; i += skiplines) {
        if (end_mark) {
          b_status = ER_BADIMAGE;
          break; /* Դͼ��ȱ������ */
        }
        skiplines = 0;
        xinc = 0;
        p = pinfo_str->p_bit_data + i * linesize;
        while (!skiplines) {
          /* ���Դ�����Ƿ�Ϸ� */
          if (xinc > pinfo_str->width) {
            b_status = ER_BADIMAGE;
            break;
          }
          /* ��ȡ�����������������͵��ֽڲ�������*/
          if (isio_read((void*)oper, 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
          /* �ֽⲢִ������ */
          switch (RLE_OPCODE(oper)) {
          case RLE_SKIPLINE_OP: /* 1 �� ����ָ���� */
            if (RLE_LONGP(oper)) {
              if (isio_read((void*)&skiplines, 2, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                break;
                break;
              }
            }
            else {
              skiplines = (ushort)RLE_DATUM(oper);
            }
            break;
          case RLE_SETCOLOR_OP: /* 2 �� ���õ�ǰ?��?*/
            currchann = RLE_DATUM(oper);
            switch (k) {
            case 1:
              if (currchann != 0) { /* ֻ����0��ͨ������ */
                b_status = ER_BADIMAGE;
                break;
                break;
              }
              break;
            case 3:
              if (currchann == 0) { /* ����ǰͨ��ֵתΪ����д��λ�� */
                currchann = 2;
              }
              else if (currchann == 1) {
                currchann = 1;
              }
              else if (currchann == 2) {
                currchann = 0;
              }
              else { /* ������������ͨ���� */
                b_status = ER_BADIMAGE;
                break;
                break;
              }
              break;
            case 4:
              if (currchann == 0) {
                currchann = 2;
              }
              else if (currchann == 1) {
                currchann = 1;
              }
              else if (currchann == 2) {
                currchann = 0;
              }
              else if (currchann == (uchar)255) { /* ALPHAͨ��תΪ����д��λ�� */
                currchann = 3;
              }
              else {
                b_status = ER_BADIMAGE;
                break;
                break;
              }
              break;
            default:
              assert(0);
              b_status = ER_SYSERR;
              break;
              break;
            }
            assert(currchann <= 3);
            xinc = 0; /* ����������0 */
            break;
          case RLE_SKIPPIXELS_OP: /* 3 �� ����ָ������������ */
            if (RLE_LONGP(oper)) {
              if (isio_read((void*)&skipcol, 2, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                break;
                break;
              }
            }
            else {
              skipcol = (ushort)RLE_DATUM(oper);
            }
            /* ��ֹ����д��� */
            if ((xinc + (unsigned int)skipcol) > pinfo_str->width) {
              b_status = ER_BADIMAGE;
              break;
              break;
            }
            xinc += (unsigned int)skipcol;
            break;
          case RLE_BYTEDATA_OP: /* 5 �� δѹ������ */
            if (RLE_LONGP(oper)) {
              if (isio_read((void*)&nc, 2, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                break;
                break;
              }
            }
            else {
              nc = RLE_DATUM(oper);
            }
            nc++;
            /* ����δѹ�����ݣ�������ܣ�Ҳ��������ֽڣ� */
            if (isio_read((void*)pbuf, ((nc + 1) / 2) * 2, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
              break;
            }
            /* ��ֹ����д��� */
            if ((xinc + (unsigned int)nc) > pinfo_str->width) {
              b_status = ER_BADIMAGE;
              break;
              break;
            }
            /* д���������ݰ� */
            for (j = 0; j < (int)(unsigned int)nc; j++) {
              p[k * (xinc + j) + (int)(unsigned int)currchann] = pbuf[j];
            }
            xinc += (unsigned int)nc;
            break;
          case RLE_RUNDATA_OP: /* 6 �� ѹ������ */
            if (RLE_LONGP(oper)) {
              if (isio_read((void*)&nc, 2, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                break;
                break;
              }
            }
            else {
              nc = RLE_DATUM(oper);
            }
            nc++;
            /* ��ѹ���ֽڣ���һ�ֽڵ�����룩*/
            if (isio_read((void*)&cor, 2, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
              break;
            }
            /* ��ֹ����д��� */
            if ((xinc + (unsigned int)nc) > pinfo_str->width) {
              b_status = ER_BADIMAGE;
              break;
              break;
            }
            /* д���������ݰ� */
            for (j = 0; j < (int)(unsigned int)nc; j++) {
              p[k * (xinc + j) + (int)(unsigned int)currchann] = (uchar)cor;
            }
            xinc += (unsigned int)nc;
            break;
          case RLE_EOF_OP: /* 7 �� ���ս�� */
            skiplines = 1; /* ��ֵ��ʹwhileѭ����ֹ */
            end_mark = 1;
            break;
          default:
            b_status = ER_BADIMAGE;
            break; /* δ����Ĳ����� */
          }
        }
        /* ���ȵ��� */
         
      }
      /* ��ͼ����Ƚ��� */
       
      pinfo_str->data_state = 2;
    }
    while (0);
    
      if (pbuf) {
        isirw_free(pbuf);
      }
      if (pcomm) {
        isirw_free(pcomm);
      }
      if (pal_buf) {
        isirw_free(pal_buf);
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
EXERESULT rle_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  RLE_HEADER rle_header;
  uchar filler = 0;
  ushort commlen, skipline, chann;
  int i, j, l, k, packlen;
  uchar* psou = 0, *pdec = 0, *p;
  static char* ptag = "manufacturer=ISee"; /* ע�ʹ���17�ֽ� */
  static ushort pal[3][256]; /* ��ɫ������ */
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);
  do {
      /* �ж��Ƿ�����֧�ֵ�ͼ������ */
      if (_rle_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        break;
      }
      /* ��дͷ�ṹ */
      rle_header.magic = (ushort)RLE_MAGIC;
      rle_header.xpos = 0;
      rle_header.ypos = 0;
      rle_header.width = (ushort)pinfo_str->width;
      rle_header.height = (ushort)pinfo_str->height;
      rle_header.channels = (uchar)pinfo_str->bitcount / 8;
      assert((rle_header.channels == 1) || (rle_header.channels == 3)); /* ֻ����8λ��24λͼ�� */
      rle_header.chan_bits = 8;
      rle_header.flags = RLE_NO_BACKGROUND_F | RLE_COMMENT_F;
      if (pinfo_str->bitcount == 8) {
        rle_header.pal_chan_count = 3;
        rle_header.pal_chan_bits = 8;
      }
      else {
        rle_header.pal_chan_count = 0;
        rle_header.pal_chan_bits = 8;
      }
      /* д��ͷ�ṹ */
      if (isio_write((const void*)&rle_header, sizeof(RLE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
       
      /* д������ֽ� */
      if (isio_write((const void*)&filler, 1, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* д���ɫ������ */
      if (rle_header.pal_chan_count) {
        assert(pinfo_str->bitcount == 8);
        /* �ֽ�ԭ��ɫ������ */
        for (i = 0; i < 3; i++) {
          for (j = 0; j < 256; j++) {
            pal[i][j] = (ushort)(((pinfo_str->palette[j] >> ((2 - i) * 8)) & 0xff) << 8);
          }
        }
        /* д��ͨ�������ĵ�ɫ������ */
        for (i = 0; i < 3; i++) {
          if (isio_write((const void*)pal[i], 256 * 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
      }
      commlen = strlen(ptag) + 1; /* ����ע�ʹ����ȣ�������β0����*/
      /* д��ע����Ϣ */
      if (isio_write((const void*)&commlen, 2, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_write((const void*)ptag, commlen, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ȷ��ż�ֽڶ��� */
      if (commlen & 0x1) {
        /* д������ֽ� */
        if (isio_write((const void*)&filler, 1, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
      }
      /* ����ѹ�������� */
      if ((psou = (uchar*)isirw_malloc(pinfo_str->width + 4)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      if ((pdec = (uchar*)isirw_malloc(pinfo_str->width * 3 + 4)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      k = pinfo_str->bitcount / 8;
      /* ����д������ */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        /* �Ե���ͼ��ʽ����ͼ�� */
        p = (uchar*)pinfo_str->pp_line_addr[pinfo_str->height - i - 1];
        for (j = 0; j < k; j++) {
          chann = ((j & 0xff) << 8) | (RLE_SETCOLOR_OP & 0xff);
          /* д��ͨ��ֵ */
          if (isio_write((const void*)&chann, 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
          /* ��ȡͨ������ */
          for (l = 0; l < (int)pinfo_str->width; l++) {
            psou[l] = p[k * l + (k - 1 - j)];
          }
          packlen = _rle_pack(psou, (int)pinfo_str->width, pdec);
          /* д��ͨ������ */
          if (isio_write((const void*)pdec, packlen, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
        if ((i + 1) == (int)pinfo_str->height) { /* ���һ���Ѿ�?��?*/
          skipline = RLE_EOF_OP;
          /* д��EOF��־ */
          if (isio_write((const void*)&skipline, 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
        else {
          skipline = (0x1 << 8) | (RLE_SKIPLINE_OP & 0xff);
          /* д��ͨ��ֵ */
          if (isio_write((const void*)&skipline, 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
         
      }
       
    }
    while (0);
    
      if (psou) {
        isirw_free(psou);
      }
      if (pdec) {
        isirw_free(pdec);
      }
  return b_status;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* �жϴ����ͼ���Ƿ���Ա����棨�粻֧�ֱ��湦�ܣ��ɽ�������ȥ���� */
int _rle_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = (1UL << (8 - 1)) | (1UL << (24 - 1))};
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
#define RLE_UNKNOW 0
#define RLE_REPEAT 1
#define RLE_NONREPEAT 2
/* ѹ��һ������ */
/* �ú���ʵ���ϲ�δѹ�����ݣ���Ϊ����ѹ������ԭʼ�������õĻ��ͻ������Ч�� */
static int _rle_pack(uchar* psou, int len, uchar* pdec)
{
  assert(psou && len && pdec);
  *pdec++ = RLE_BYTEDATA_OP | LONGOPCODE;
  *pdec++ = 0;
  *(ushort*)pdec = (ushort)(len - 1);
  pdec += 2;
  memcpy((void*)pdec, (const void*)psou, len);
  return (len + 2 + 2 + 1) & 0xfffffffe; /* ѹ��������ݳ��ȣ�ż�ֽڶ��� */
}

