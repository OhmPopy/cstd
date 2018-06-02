/********************************************************************
 lzi.c
 ���ļ���;�� LZIͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ�
 ���湦�ܣ�
 ���ļ���д�ˣ�
 ���ļ��汾��
 ����޸��ڣ�
 ----------------------------------------------------------------
 ������ʷ��
********************************************************************/




/* �ļ��ṹ���� */
#define CURLZIVER 1     /* ��ǰ�ļ���ʽ�汾 */

#define BUFSIZE   4096    /* size of ring buffer */
#define UPLIMIT   18      /* upper limit for match_length */
#define THRESHOLD 2     /* encode string into position and length if match_length is greater than this */
#define NIL     BUFSIZE   /* index for root of binary search trees */

#define ERREXCEPT -1L     /* memory r/w except */

#define LZIMAGIC  0x46495A4C  /* LZI �ļ���ǩ */


/* �ļ�ͷ�ṹ */
typedef struct _tagLZIHEAD {
  ulong magic;    /* LZIMAGIC */
  ulong ver;    /* ��ʽ�汾 */
  
  ulong width;
  ulong height;
  ulong bitcnt;
  ulong imgsize;
  
  ulong order;    /* 0������ͼ��1������ͼ */
  ulong rev0;
} LZIFILEHEAD, *LPLZIFILEHEAD;


uchar text_buf[BUFSIZE + UPLIMIT - 1]; /* ring buffer of size BUFSIZE, with extra UPLIMIT-1 bytes to facilitate string comparison */
int match_position, match_length; /* of longest match. These are set by the InsertNode() procedure. */
int lson[BUFSIZE + 1], rson[BUFSIZE + 257], dad[BUFSIZE + 1]; /* left & right children & parents -- These constitute binary search trees. */

/* �ڲ����ֺ��� */
int _lzi_is_valid_img(INFOSTR* pinfo_str);
long LzssEncode(uchar* soudat, ulong sousize, uchar* desbuf);
long LzssDecode(uchar* lzssdat, ulong lzssize, uchar* extbuf);

/* ��ȡͼ����Ϣ */
EXERESULT lzi_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LZIFILEHEAD head;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */

    do {
      /* ��ȡ�ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&head, sizeof(LZIFILEHEAD), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* �ж��Ƿ�����Ч��ͼ���ļ� */
      if ((head.magic != LZIMAGIC) || ((head.bitcnt != 24) && (head.bitcnt != 32))) {
        b_status = ER_NONIMAGE;
        break;
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_APM; /* ͼ���ļ���ʽ����׺���� */
      pinfo_str->compression = ICS_RGB;
      pinfo_str->width = head.width;
      pinfo_str->height = head.height;
      pinfo_str->order = head.order;
      pinfo_str->bitcount = head.bitcnt;
      pinfo_str->b_mask = 0xff;
      pinfo_str->g_mask = 0xff00;
      pinfo_str->r_mask = 0xff0000;
      pinfo_str->a_mask = 0x0;
      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    while (0);
    
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT lzi_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LZIFILEHEAD head;
  ulong lzssdatsize;
  uchar* plzss = 0;
  int linesize, i;
  long result;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */

    do {
      /* ���ļ���λ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&head, sizeof(LZIFILEHEAD), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* �ж��Ƿ�����Ч��ͼ���ļ� */
      if ((head.magic != LZIMAGIC) || ((head.bitcnt != 24) && (head.bitcnt != 32))) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
        pinfo_str->imgformat = IMF_APM; /* ͼ��?ļ���ʽ����׺���?*/
        pinfo_str->compression = ICS_RGB;
        pinfo_str->width = head.width;
        pinfo_str->height = head.height;
        pinfo_str->order = head.order;
        pinfo_str->bitcount = head.bitcnt;
        pinfo_str->b_mask = 0xff;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->a_mask = 0x0;
        /* �趨���ݰ�״̬ */
        pinfo_str->data_state = 1;
      }
      pinfo_str->pal_count = 0;
      /* ȡ��λ���ݳ��� */
      lzssdatsize = isio_length(pfile) - sizeof(LZIFILEHEAD);
      /* ȡ��ɨ���гߴ� */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      /* ����ͼ���??*/
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
      /* ����LZSѹ�����ݻ����� */
      plzss = (uchar*) isirw_malloc(lzssdatsize);
      if (!plzss) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* һ���Զ���ѹ������ */
      if (isio_read((void*)plzss, lzssdatsize, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ���� */
      result = LzssDecode(plzss, lzssdatsize, pinfo_str->p_bit_data);
      if ((ulong)result != head.imgsize) {
        b_status = ER_BADIMAGE;
        break;
      }
      pinfo_str->data_state = 2;
    }
    while (0);
    
      if (plzss) {
        isirw_free(plzss);
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
EXERESULT lzi_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LZIFILEHEAD head;
  uchar* plzss = 0;
  int linesize, imgsize;
  long lzssdatsize;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);

    do {
      /* �ж��Ƿ�����֧�ֵ�ͼ������ */
      if (_lzi_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        break;
      }
      head.magic = LZIMAGIC;
      head.ver = CURLZIVER;
      head.width = pinfo_str->width;
      head.height = pinfo_str->height;
      head.bitcnt = pinfo_str->bitcount;
      /* ȡ��ɨ���гߴ缰ͼ��ߴ� */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      imgsize = linesize * pinfo_str->height;
      /* ����ѹ�������� */
      plzss = (uchar*) isirw_malloc(imgsize);
      if (!plzss) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* ѹ��ͼ������ */
      lzssdatsize = LzssEncode(pinfo_str->p_bit_data, imgsize, plzss);
      if ((lzssdatsize == ERREXCEPT) || (lzssdatsize == 0)) {
        b_status = ER_BADIMAGE;
        break;
      }
      head.imgsize = imgsize;
      head.rev0 = 0;
      head.order = pinfo_str->order;
      /* д���ļ�ͷ�ṹ */
      if (isio_write((const void*)&head, sizeof(LZIFILEHEAD), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* д��ѹ������ */
      if (isio_write((const void*)plzss, lzssdatsize, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
    }
    while (0);
    
      if (plzss) {
        isirw_free(plzss);
      }
  return b_status;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */
/* �жϴ����ͼ���Ƿ���Ա����棨�粻֧�ֱ��湦�ܣ��ɽ�������ȥ���� */
int _lzi_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = (1UL << (24 - 1)) | (1UL << (32 - 1))};
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
/* initialize trees */
void InitTree(void)
{
  int i;
  /* For i = 0 to BUFSIZE - 1, rson[i] and lson[i] will be the right and
  left children of node i. These nodes need not be initialized.
  Also, dad[i] is the parent of node i. These are initialized to
  NIL (= BUFSIZE), which stands for 'not used.'
  For i = 0 to 255, rson[BUFSIZE + i + 1] is the root of the tree
  for strings that begin with character i. These are initialized
  to NIL. Note there are 256 trees.
  */
  for (i = BUFSIZE + 1; i <= BUFSIZE + 256; i++) {
    rson[i] = NIL;
  }
  for (i = 0; i < BUFSIZE; i++) {
    dad[i] = NIL;
  }
}
/* Inserts string of length UPLIMIT, text_buf[r..r+UPLIMIT-1], into one of the
 trees (text_buf[r]'th tree) and returns the longest-match position
 and length via the global variables match_position and match_length.
 If match_length = UPLIMIT, then removes the old node in favor of the new
 one, because the old one will be deleted sooner.
 Note r plays double role, as tree node and position in buffer.
*/
void InsertNode(int r)
{
  int i, p, cmp;
  uchar* key;
  cmp = 1;
  key = &text_buf[r];
  p = BUFSIZE + 1 + key[0];
  rson[r] = lson[r] = NIL;
  match_length = 0;
  for (;;) {
    if (cmp >= 0) {
      if (rson[p] != NIL) {
        p = rson[p];
      }
      else {
        rson[p] = r;
        dad[r] = p;
        return;
      }
    }
    else {
      if (lson[p] != NIL) {
        p = lson[p];
      }
      else {
        lson[p] = r;
        dad[r] = p;
        return;
      }
    }
    for (i = 1; i < UPLIMIT; i++) {
      if ((cmp = key[i] - text_buf[p + i]) != 0) {
        break;
      }
    }
    if (i > match_length) {
      match_position = p;
      if ((match_length = i) >= UPLIMIT) {
        break;
      }
    }
  }
  dad[r] = dad[p];
  lson[r] = lson[p];
  rson[r] = rson[p];
  dad[lson[p]] = r;
  dad[rson[p]] = r;
  if (rson[dad[p]] == p) {
    rson[dad[p]] = r;
  }
  else {
    lson[dad[p]] = r;
  }
  dad[p] = NIL; /* remove p */
}
/* deletes node p from tree */
void DeleteNode(int p)
{
  int q;
  if (dad[p] == NIL) {
    return; /* not in tree */
  }
  if (rson[p] == NIL) {
    q = lson[p];
  }
  else if (lson[p] == NIL) {
    q = rson[p];
  }
  else {
    q = lson[p];
    if (rson[q] != NIL) {
      do {
        q = rson[q];
      }
      while (rson[q] != NIL);
      rson[dad[q]] = lson[q];
      dad[lson[q]] = dad[q];
      lson[q] = lson[p];
      dad[lson[p]] = q;
    }
    rson[q] = rson[p];
    dad[rson[p]] = q;
  }
  dad[q] = dad[p];
  if (rson[dad[p]] == p) {
    rson[dad[p]] = q;
  }
  else {
    lson[dad[p]] = q;
  }
  dad[p] = NIL;
}
/* Encode data */
long LzssEncode(uchar* soudat, ulong sousize, uchar* desbuf)
{
  int i, c, len, r, s, last_match_length, code_buf_ptr;
  long result = 0L;
  ulong ulspos = 0UL, uldpos = 0UL;
  uchar* psou = soudat, *pdes = desbuf;
  uchar code_buf[17], mask;
  /* initialize trees */
  InitTree();
  /*
  code_buf[1..16] saves eight units of code, and
  code_buf[0] works as eight flags, "1" representing that the unit
  is an unencoded letter (1 byte), "0" a position-and-length pair
  (2 bytes). Thus, eight units require at most 16 bytes of code.
  */
  code_buf[0] = 0;
  code_buf_ptr = mask = 1;
  s = 0;
  r = BUFSIZE - UPLIMIT;
  for (i = s; i < r; i++) {
    text_buf[i] = ' '; /* Clear the buffer with any character that will appear often. */
  }
  if (sousize < UPLIMIT) {
    len = sousize;
  }
  else {
    len = UPLIMIT;
  }
  /* Read UPLIMIT bytes into the last UPLIMIT bytes of the buffer */
  memcpy((void*)(uchar*)&text_buf[r], (const void*)psou, len);
  psou += len;
  ulspos += len;
  if (len == 0) {
    return 0; /* text of size zero */
  }
  /*
  Insert the UPLIMIT strings, each of which begins with one or more 'space' characters. Note
  the order in which these strings are inserted. This way, degenerate trees will be less likely to occur.
  */
  for (i = 1; i <= UPLIMIT; i++) {
    InsertNode(r - i);
  }
  /* Finally, insert the whole string just read. The global variables match_length and match_position are set. */
  InsertNode(r);

    do {
      do {
        if (match_length > len) {
          match_length = len; /* match_length may be spuriously long near the end of text. */
        }
        if (match_length <= THRESHOLD) {
          match_length = 1; /* Not long enough match. Send one byte. */
          code_buf[0] |= mask; /* 'send one byte' flag */
          code_buf[code_buf_ptr++] = text_buf[r]; /* Send uncoded. */
        }
        else {
          code_buf[code_buf_ptr++] = (uchar) match_position;
          /* Send position and length pair. Note match_length > THRESHOLD. */
          code_buf[code_buf_ptr++] = (uchar)(((match_position >> 4) & 0xf0) | (match_length - (THRESHOLD + 1)));
        }
        if ((mask <<= 1) == 0) { /* Shift mask left one bit. */
          for (i = 0; i < code_buf_ptr; i++) { /* Send at most 8 units of code together */
            *pdes++ = code_buf[i];
          }
          uldpos += code_buf_ptr;
          code_buf[0] = 0;
          code_buf_ptr = mask = 1;
        }
        last_match_length = match_length;
        for (i = 0; i < last_match_length && ulspos < sousize; i++) {
          DeleteNode(s); /* Delete old strings and */
          c = *psou++;
          ulspos++;
          text_buf[s] = c; /* read new bytes */
          /* If the position is near the end of buffer, extend the buffer to make string comparison easier. */
          if (s < UPLIMIT - 1) {
            text_buf[s + BUFSIZE] = c;
          }
          /* Since this is a ring buffer, increment the position modulo BUFSIZE. */
          s = (s + 1) & (BUFSIZE - 1);
          r = (r + 1) & (BUFSIZE - 1);
          InsertNode(r); /* Register the string in text_buf[r..r+UPLIMIT-1] */
        }
        /* After the end of text, */
        while (i++ < last_match_length) {
          DeleteNode(s); /* no need to read, but */
          s = (s + 1) & (BUFSIZE - 1);
          r = (r + 1) & (BUFSIZE - 1);
          if (--len) {
            InsertNode(r); /* buffer may not be empty. */
          }
        }
      }
      while (len > 0); /* until length of string to be processed is zero */
      if (code_buf_ptr > 1) {
        for (i = 0; i < code_buf_ptr; i++) {
          *pdes++ = code_buf[i];
        }
        uldpos += code_buf_ptr;
      }
      result = uldpos;
    }
    while (0);
  return result;
}
/* Just the reverse of Encode(). */
long LzssDecode(uchar* lzssdat, ulong lzssize, uchar* extbuf)
{
  int i, j, k, r, c;
  long result = 0L;
  unsigned int flags;
  ulong ullzsspos = 0UL, ulextpos = 0UL;
  uchar* plzss = lzssdat, *pext = extbuf;
  for (i = 0; i < BUFSIZE - UPLIMIT; i++) {
    text_buf[i] = ' ';
  }
  r = BUFSIZE - UPLIMIT;
  flags = 0;
    do {
      for (;;) {
        if (((flags >>= 1) & 256) == 0) {
          if (ullzsspos >= lzssize) {
            break;
          }
          else {
            c = (int) * plzss++;
            ullzsspos++;
          }
          flags = c | 0xff00; /* uses higher byte cleverly to count eight */
        }
        if (flags & 1) {
          if (ullzsspos >= lzssize) {
            break;
          }
          else {
            c = (int) * plzss++;
            ullzsspos++;
          }
          *pext++ = (uchar)c;
          ulextpos++;
          text_buf[r++] = c;
          r &= (BUFSIZE - 1);
        }
        else {
          if (ullzsspos >= lzssize) {
            break;
          }
          else {
            i = (int) * plzss++;
            ullzsspos++;
          }
          if (ullzsspos >= lzssize) {
            break;
          }
          else {
            j = (int) * plzss++;
            ullzsspos++;
          }
          i |= ((j & 0xf0) << 4);
          j = (j & 0x0f) + THRESHOLD;
          for (k = 0; k <= j; k++) {
            c = text_buf[(i + k) & (BUFSIZE - 1)];
            *pext++ = (uchar)c;
            ulextpos++;
            text_buf[r++] = c;
            r &= (BUFSIZE - 1);
          }
        }
      }
      result = ulextpos;
    }
    while (0);
  return result;
}

