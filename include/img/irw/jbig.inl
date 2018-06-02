/********************************************************************
 jbig.c
 ���ô����д�˼���Ȩ��Ϣ��
 ----------------------------------------------------------------
 ������� JBIG�������(jbig kit)
 ����ļ��� libjbig.h��libjbig.c��libjbigtab.c��libjbig.txt
 ʹ����ɣ� GPL
 ----------------------------------------------------------------
 ������ʷ��
 2003-4 ֧��ͼ���д�����ṩ�����ػ���������
 2002-8 ����ӿ�������2.2�������ڴ�������
 2002-5 ��һ�������棨�°棩
********************************************************************/

#include "libjbig.inl"
#include "libjbigtab.inl"
#define BUFFER_SIZE (32*1024) /* �����ݻ������ߴ磨32KB��*/

/* ��ȡͼ����Ϣ */
EXERESULT jbig_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  JBG_DEC_STATE jds;
  uchar* buf = 0, *p;
  int len, result;
  size_t cnt;
  ulong xmax = 4294967295UL;
  ulong ymax = 4294967295UL;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */

    do {
      jbg_dec_init(&jds);
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ��������ݻ����� */
      if ((buf = (uchar*)isirw_malloc(BUFFER_SIZE)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* �趨ͼ������޳� */
      jbg_dec_maxsize(&jds, xmax, ymax);
      result = JBG_EAGAIN;
      do {
        /* �ֶζ�ȡѹ������ */
        if ((len = isio_read((void*)buf, 1, BUFFER_SIZE, pfile)) == 0) {
          if ((result == JBG_EOK_INTR) || (result == JBG_EOK)) {
            break;
          }
          else {
            b_status = ER_FILERWERR;
            break;
          }
        }
        cnt = 0;
        p = buf;
        while (len > 0 && (result == JBG_EAGAIN || result == JBG_EOK)) {
          /* �����ݵ�������� */
          result = jbg_dec_in(&jds, p, len, &cnt);
          p += cnt;
          len -= cnt;
        }
      }
      while (result == JBG_EAGAIN || result == JBG_EOK);
      /* �ж�ָ��ͼ���Ƿ���JBIGͼ�� */
      switch (result) {
      case JBG_EOK:
      case JBG_EOK_INTR:
        break;
      case JBG_ENOMEM:
        b_status = ER_MEMORYERR;
        break;
        break;
      case JBG_EAGAIN:
      case JBG_EABORT:
      case JBG_EMARKER:
      case JBG_ENOCONT:
      case JBG_EINVAL:
      case JBG_EIMPL:
        b_status = ER_NONIMAGE;
        break;
        break;
      case JBG_ESYSERR:
        b_status = ER_SYSERR;
        break;
        break;
      }
      /* ֻ֧�ֵ�ɫͼ�� */
      if (jbg_dec_getplanes(&jds) != 1) {
        b_status = ER_NONIMAGE;
        break;
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_JBIG; /* ͼ���ļ���ʽ����׺���� */
      pinfo_str->compression = ICS_JBIG; /* ѹ����ʽ */
      pinfo_str->width = (ulong)jbg_dec_getwidth(&jds);
      pinfo_str->height = (ulong)jbg_dec_getheight(&jds);
      pinfo_str->order = 0; /* ��Ϊ����ͼ */
      pinfo_str->bitcount = (ulong)jbg_dec_getplanes(&jds);
      SETMASK_8(pinfo_str); /* ��������Ϊȫ0 */
      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    while (0);
    
      if (buf) {
        isirw_free(buf);
      }
      jbg_dec_free(&jds);
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT jbig_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  JBG_DEC_STATE jds;
  uchar* buf = 0, *p, *bits;
  int len, srclinesize, result, i, linesize, step;
  size_t cnt;
  ulong xmax = 4294967295UL;
  ulong ymax = 4294967295UL;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */

    do {
      jbg_dec_init(&jds);
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* ��������ݻ����� */
      if ((buf = (uchar*)isirw_malloc(BUFFER_SIZE)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* �趨ͼ������޳� */
      jbg_dec_maxsize(&jds, xmax, ymax);
      result = JBG_EAGAIN;
      do {
        /* �ֶζ�ȡѹ������ */
        if ((len = isio_read((void*)buf, 1, BUFFER_SIZE, pfile)) == 0) {
          if ((result == JBG_EOK_INTR) || (result == JBG_EOK)) {
            break;
          }
          else {
            b_status = ER_FILERWERR;
            break;
          }
        }
        cnt = 0;
        p = buf;
        while (len > 0 && (result == JBG_EAGAIN || result == JBG_EOK)) {
          /* �����ݵ�������� */
          result = jbg_dec_in(&jds, p, len, &cnt);
          p += cnt;
          len -= cnt;
        }
      }
      while (result == JBG_EAGAIN || result == JBG_EOK);
      /* �ж�ָ��ͼ���Ƿ���JBIGͼ�� */
      switch (result) {
      case JBG_EOK:
      case JBG_EOK_INTR:
        break;
      case JBG_ENOMEM:
        b_status = ER_MEMORYERR;
        break;
        break;
      case JBG_EAGAIN:
      case JBG_EABORT:
      case JBG_EMARKER:
      case JBG_ENOCONT:
      case JBG_EINVAL:
      case JBG_EIMPL:
        b_status = ER_NONIMAGE;
        break;
        break;
      case JBG_ESYSERR:
        b_status = ER_SYSERR;
        break;
        break;
      }
      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        if (jbg_dec_getplanes(&jds) != 1) {
          b_status = ER_NONIMAGE;
          break;
        }
        pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
        pinfo_str->imgformat = IMF_JBIG; /* ͼ���ļ���ʽ����׺���� */
        pinfo_str->compression = ICS_JBIG; /* ѹ����ʽ */
        pinfo_str->width = (ulong)jbg_dec_getwidth(&jds);
        pinfo_str->height = (ulong)jbg_dec_getheight(&jds);
        pinfo_str->order = 0; /* ��Ϊ����ͼ */
        pinfo_str->bitcount = (ulong)jbg_dec_getplanes(&jds);
        SETMASK_8(pinfo_str); /* ��������Ϊȫ0 */
        pinfo_str->data_state = 1;
      }
      /* ���õ�ɫ������ */
      assert(pinfo_str->bitcount == 1);
      pinfo_str->pal_count = 1UL << pinfo_str->bitcount;
      switch (pinfo_str->bitcount) {
      case 1:
        pinfo_str->palette[0] = 0xffffff;
        pinfo_str->palette[1] = 0x0;
        break;
      case 4:
      case 8:
        assert(0); /* ��Ӧ��ִ�е��˴�������Ĵ���ֻ��Ϊ������չ֮�� */
        step = 256 / (int)pinfo_str->pal_count;
        for (i = 0; i < (int)pinfo_str->pal_count; i++) {
          pinfo_str->palette[i] = step * (pinfo_str->pal_count - i) - 1;
        }
        break;
      default:
        assert(0);
        b_status = ER_SYSERR;
        break;
        break;
      }
      /* ȡ��ɨ���гߴ� */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      /* JBIG��ѹ���ɨ���г��ȣ��ֽڱ߽磩 */
      srclinesize = (int)((pinfo_str->bitcount * pinfo_str->width + 7) / 8);
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
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
      }
      /* ��ȡ��ѹ���ͼ�����ݵ�ַ */
      bits = jbg_dec_getimage(&jds, 0);
      /* ���ж��� */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        p = (uchar*)pinfo_str->pp_line_addr[i];
        memcpy((void*)p, (const void*)bits, srclinesize);
        bits += srclinesize;
      }
      pinfo_str->data_state = 2;
    }
    while (0);
      jbg_dec_free(&jds);
      if (buf) {
        isirw_free(buf);
      }
      if ((b_status != ER_SUCCESS) || (1)) {
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
EXERESULT jbig_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  /* ################ ���������д����ͼ��Ĵ��� ################ */
  /* �ݲ�֧��JBIG�ı��湦�ܡ�����ͼƬһ�㶼����ɨ���ǡ�����������ģ�����ֻ֧�ֶ����ܼ��ɡ���YZ */
  /* ############################################################ */
  return ER_NOTSUPPORT;
}
