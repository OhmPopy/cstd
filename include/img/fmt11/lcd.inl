/********************************************************************

  lcd.c

  ----------------------------------------------------------------
    ������֤ �� GPL
  ��Ȩ���� (C) 2003 VCHelp coPathway ISee workgroup.
  ----------------------------------------------------------------
  ��һ����������������������������������������GNU ͨ�ù�����
  ��֤�������޸ĺ����·�����һ���򡣻��������֤�ĵڶ��棬���ߣ���
  �����ѡ�����κθ��µİ汾��

    ������һ�����Ŀ����ϣ�������ã���û���κε���������û���ʺ��ض�
  Ŀ�ص������ĵ���������ϸ����������GNUͨ�ù������֤��

    ��Ӧ���Ѿ��ͳ���һ���յ�һ��GNUͨ�ù������֤�ĸ�������Ŀ¼
  GPL.txt�ļ����������û�У�д�Ÿ���
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA
  ----------------------------------------------------------------
  �������ʹ�ñ����ʱ��ʲô������飬�������µ�ַ������ȡ����ϵ��

      http://isee.126.com
      http://cosoft.org.cn/projects/iseeexplorer

  ���ŵ���

      isee##vip.163.com
  ----------------------------------------------------------------
  ���ļ���;��  ISeeͼ���������LCDͼ���дģ��ʵ���ļ�

          ��ȡ���ܣ���ȡ�Զ���ṹ -����(8λһ���ֽ�)�ж��뷽ʽ�洢��
                Һ����������

          ���湦�ܣ��Զ���ṹ -����(8λһ���ֽ�)�ж��뷽ʽ�洢��Һ��
                �������ݣ���.h�ļ��ж��壩

  ���ļ���д�ˣ�  zhaojinpo (zhaojinpo##sina.com)

  ���ļ��汾��  30902
  ����޸��ڣ�  2003-9-2

  ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
      ��ַ�ռ������
    ----------------------------------------------------------------
  ������ʷ��

    2003-9    ��һ�η���

********************************************************************/


#ifndef WIN32
#if defined(_WIN32)||defined(_WINDOWS)
#define WIN32
#endif
#endif /* WIN32 */

/*###################################################################

  ��ֲע�ͣ����´���ʹ����WIN32ϵͳ��SEH(�ṹ���쳣����)�����߳�ͬ��
      ���󡰹ؼ��Ρ�������ֲʱӦתΪLinux����Ӧ��䡣

  #################################################################*/


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN       /* ����windows.h�ļ��ı���ʱ�� */
#include <windows.h>
#endif /* WIN32 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lcd.h"


IRWP_INFO     lcd_irwp_info;      /* �����Ϣ�� */

#ifdef WIN32
CRITICAL_SECTION  lcd_get_info_critical;  /* lcd_get_image_info�����Ĺؼ��� */
CRITICAL_SECTION  lcd_load_img_critical;  /* lcd_load_image�����Ĺؼ��� */
CRITICAL_SECTION  lcd_save_img_critical;  /* lcd_save_image�����Ĺؼ��� */
#else
/* Linux��Ӧ����� */
#endif


/* �ڲ����ֺ��� */
void CALLAGREEMENT _init_irwp_info(LPIRWP_INFO lpirwp_info);
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */);
int CALLAGREEMENT _lcd_is_valid_img(LPINFOSTR pinfo_str);
unsigned  char CALLAGREEMENT _get_byte_bit(unsigned char my_byte, unsigned char bit);


#ifdef WIN32
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
    /* ��ʼ�������Ϣ�� */
    _init_irwp_info(&lcd_irwp_info);

    /* ��ʼ�����ʹؼ��� */
    InitializeCriticalSection(&lcd_get_info_critical);
    InitializeCriticalSection(&lcd_load_img_critical);
    InitializeCriticalSection(&lcd_save_img_critical);

    break;

  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;

  case DLL_PROCESS_DETACH:
    /* ���ٷ��ʹؼ��� */
    DeleteCriticalSection(&lcd_get_info_critical);
    DeleteCriticalSection(&lcd_load_img_critical);
    DeleteCriticalSection(&lcd_save_img_critical);
    break;
  }

  return TRUE;
}
#endif  /* WIN32 */



#ifdef WIN32

LCD_API LPIRWP_INFO CALLAGREEMENT is_irw_plugin()
{
  return (LPIRWP_INFO)&lcd_irwp_info;
}

#else

LCD_API LPIRWP_INFO CALLAGREEMENT lcd_get_plugin_info()
{
  _init_irwp_info(&lcd_irwp_info);

  return (LPIRWP_INFO)&lcd_irwp_info;
}

LCD_API void CALLAGREEMENT lcd_init_plugin()
{
  /* ��ʼ�����߳�ͬ������ */
}

LCD_API void CALLAGREEMENT lcd_detach_plugin()
{
  /* ���ٶ��߳�ͬ������ */
}

#endif /* WIN32 */


/* ��ʼ�������Ϣ�ṹ */
void CALLAGREEMENT _init_irwp_info(LPIRWP_INFO lpirwp_info)
{
  assert(lpirwp_info);

  /* ��ʼ���ṹ���� */
  memset((void*)lpirwp_info, 0, sizeof(IRWP_INFO));

  /* �汾�š���ʮ����ֵ��ʮλΪ���汾�ţ���λΪ���汾��*/
  lpirwp_info->irwp_version = MODULE_BUILDID;
  /* ������� */
  strcpy((char*)(lpirwp_info->irwp_name), MODULE_NAME);
  /* ��ģ�麯��ǰ׺ */
  strcpy((char*)(lpirwp_info->irwp_func_prefix), MODULE_FUNC_PREFIX);
  /* ��д�����ʶ */
  lpirwp_info->plug_id = IMF_LCD;


  /* ����ķ������͡�0�����԰�����1���������� */
#ifdef _DEBUG
  lpirwp_info->irwp_build_set = 0;
#else
  lpirwp_info->irwp_build_set = 1;
#endif


  /* ���ܱ�ʶ ��##���ֶ��������粻֧�ֱ��湦���뽫IRWP_WRITE_SUPP��ʶȥ���� */
  lpirwp_info->irwp_function = IRWP_READ_SUPP | IRWP_WRITE_SUPP;

  /* ����ģ��֧�ֵı���λ�� */
  /* ################################################################# */
  /* ��λ����һ������32λ��ʽ�޷���ʾ���˴�����LCD��ȷ��λ����д��*/
  lpirwp_info->irwp_save.bitcount = 1UL;
  /* ################################################################# */

  lpirwp_info->irwp_save.img_num = 1; /* 1��ֻ�ܱ���һ��ͼ�� */
  /* ���������趨���������޸Ĵ�ֵ��##���ֶ������� */
  lpirwp_info->irwp_save.count = 0;

  /* ����������������������Ϣ����Ч��ĸ�������##���ֶ�������*/
  lpirwp_info->irwp_author_count = 1;


  /* ��������Ϣ��##���ֶ������� */
  /* ---------------------------------[0] �� ��һ�� -------------- */
  strcpy((char*)(lpirwp_info->irwp_author[0].ai_name),
      (const char*)"zhaojinpo");
  strcpy((char*)(lpirwp_info->irwp_author[0].ai_email),
      (const char*)"zhaojinpo##sina.com");
  strcpy((char*)(lpirwp_info->irwp_author[0].ai_message),
      (const char*)"Һ��������ȡ��ʽ");
  /* ---------------------------------[1] �� �ڶ��� -------------- */
  /* ������������Ϣ�ɼ��ڴ˴���
  strcpy((char*)(lpirwp_info->irwp_author[1].ai_name),
        (const char *)"");
  strcpy((char*)(lpirwp_info->irwp_author[1].ai_email),
        (const char *)"@");
  strcpy((char*)(lpirwp_info->irwp_author[1].ai_message),
        (const char *)":)");
  */
  /* ------------------------------------------------------------- */


  /* ���������Ϣ����չ����Ϣ��*/
  strcpy((char*)(lpirwp_info->irwp_desc_info.idi_currency_name),
      (const char*)MODULE_FILE_POSTFIX);

  lpirwp_info->irwp_desc_info.idi_rev = 0;

  /* ����������##���ֶ���������LCD��ʽ�б�����ο�BMP����б��������ã� */
  lpirwp_info->irwp_desc_info.idi_synonym_count = 0;

  /* ���ó�ʼ����ϱ�־ */
  lpirwp_info->init_tag = 1;

  return;
}



/* ��ȡͼ����Ϣ */
LCD_API int CALLAGREEMENT lcd_get_image_info(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;
  LCD_FILE_HEADER bfh;
  int       style;        /* ���0��WINDOWS��1��OS/2 */
  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */

  __try {
    __try {
      /* ����ؼ��� */
      EnterCriticalSection(&lcd_get_info_critical);

      /* ��ָ���� */
      if ((pfile = isio_open((const char*)psct, "rb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ��ȡ�ļ�ͷ�ṹ */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (isio_read((void*)&bfh, sizeof(LCD_FILE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (bfh.bfType == LCD_HEADER_MARKER) {
        style = 0;
      }
      else {
        b_status = ER_NONIMAGE;
        __leave;
      }

      /* ������Ч�Լ��� */
      if (style == 0) {
        if ((bfh.width == 0) || (bfh.hight == 0)) {
          b_status = ER_NONIMAGE;
          __leave;
        }
      }

      pinfo_str->imgtype    = IMT_RESSTATIC;  /* ͼ���ļ����� */
      pinfo_str->imgformat  = IMF_LCD;      /* ͼ���ļ���ʽ����׺���� */
      pinfo_str->compression  = ICS_RGB;

      pinfo_str->width  = bfh.width;
      pinfo_str->height = bfh.hight;
      pinfo_str->order  = 0;
      pinfo_str->bitcount = 1;

      pinfo_str->b_mask = 0x0;
      pinfo_str->g_mask = 0x0;
      pinfo_str->r_mask = 0x0;
      pinfo_str->a_mask = 0x0;


      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    __finally {
      if (pfile) {
        isio_close(pfile);
      }

      LeaveCriticalSection(&lcd_get_info_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    pinfo_str->data_state = 0;
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* ��ȡͼ��λ���� */
LCD_API int CALLAGREEMENT lcd_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;
  LCD_FILE_HEADER bfh;
  int       style;          /* ���0��WINDOWS */
  unsigned long LCD_data_len;     /* LCDλ���ݳߴ� */
  int       i, linesize, line = 0; /* ��ǰ�� */
  unsigned int  k;
  unsigned char tmp , j;
  unsigned char* pbit = (unsigned char*)0;
  unsigned char* p_pre_bit = (unsigned char*)0;
  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);    /* ���ݰ��в��ܴ���ͼ��λ���� */

  __try {
    __try {
      EnterCriticalSection(&lcd_load_img_critical);

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

      if (isio_read((void*)&bfh, sizeof(LCD_FILE_HEADER), 1, pfile) == 0) {
        b_status = (pinfo_str->data_state == 1) ? ER_FILERWERR : ER_NONIMAGE;
        __leave;
      }

      /* �ж��Ƿ�����Ч��λͼ�ļ� */
      if (bfh.bfType == LCD_HEADER_MARKER) {
        style = 0;
      }
      else {
        b_status = ER_NONIMAGE;
        __leave;
      }

      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        /* ������Ч�Լ��� */
        if (style == 0) {
          if ((bfh.width == 0) || (bfh.hight == 0)) {
            b_status = ER_NONIMAGE;
            __leave;
          }
        }

        pinfo_str->imgtype    = IMT_RESSTATIC;  /* ͼ���ļ����� */
        pinfo_str->imgformat  = IMF_LCD;      /* ͼ���ļ���ʽ����׺���� */
        pinfo_str->compression  = ICS_RGB;

        pinfo_str->width  = bfh.width;
        pinfo_str->height = bfh.hight;
        pinfo_str->order  = 0;
        pinfo_str->bitcount = 1;

        pinfo_str->b_mask = 0x0;
        pinfo_str->g_mask = 0x0;
        pinfo_str->r_mask = 0x0;
        pinfo_str->a_mask = 0x0;

        pinfo_str->data_state = 1;
      }

      /* ���õ�ɫ������ */
      if (pinfo_str->bitcount <= 8) {
        pinfo_str->pal_count = 1UL << pinfo_str->bitcount;

        if (style == 0) {
          pinfo_str->palette[0] = 0xff00;   /* ��ɫ������ֽ����� */
          pinfo_str->palette[1] = 0xffffff;
        }
      }
      else {
        pinfo_str->pal_count = 0;
      }

      /* ȡ��λ���ݳ��� */
      LCD_data_len = isio_length(pfile) - 1;
      /* ȡ��ɨ���гߴ� */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);

      /* ����ͼ����� */
      pinfo_str->imgnumbers = 1;
      pinfo_str->psubimg    = (LPSUBIMGBLOCK)0;


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

      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + (i * linesize));
      }

      /* ��ȡλ���ݵ��ڴ� */
      if (isio_seek(pfile, LCD_data_offset, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      pbit = pinfo_str->p_bit_data;

      /* ÿ�ζ�ȡ8������ ����8�����ݻ�����(�����б�������+8�ֽڱ���) */
      p_pre_bit = (unsigned char*)isirw_malloc(pinfo_str->width + 8);

      if (p_pre_bit == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      for (i = 0; i < (int)(pinfo_str->height);) {
        /* ��һ�����ݵ������� */
        if (isio_read(p_pre_bit, pinfo_str->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        j = 0;

        /* ���������ʾ���� */
        while (i < (int)(pinfo_str->height) && (j < 8)) {
          /* ���г�ȡ��������ƴ��Ϊ������ */
          for (k = 0; k < (int)(pinfo_str->width); k = k + 8) {
            tmp = (_get_byte_bit(*(p_pre_bit + k), j) << 7) | (_get_byte_bit(*(p_pre_bit + 1 + k), j) << 6) | (_get_byte_bit(*(p_pre_bit + 2 + k), j) << 5) | (_get_byte_bit(*(p_pre_bit + k + 3), j) << 4) | (_get_byte_bit(*(p_pre_bit + k + 4), j) << 3) | (_get_byte_bit(*(p_pre_bit + k + 5), j) << 2) | (_get_byte_bit(*(p_pre_bit + k + 6), j) << 1) | (_get_byte_bit(*(p_pre_bit + k + 7), j) << 0);
            *(unsigned char*)(pbit + (k >> 3)) = tmp;
          }

          i++;
          j++;

          /* �ƶ����µ�λ��4�ֽڶ��� */
          pbit = (unsigned char*)(pbit + linesize);
        }
      }

      /* ����ڴ� */
      isirw_free(p_pre_bit);

      p_pre_bit = 0;

      /* һ���н��ȵ��� */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_LOAD)) {
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
      if (p_pre_bit) {
        isirw_free(p_pre_bit);
      }

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

      LeaveCriticalSection(&lcd_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* ����ͼ�� */
LCD_API int CALLAGREEMENT lcd_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;
  LCD_FILE_HEADER bfh;
  unsigned char* p_pre_bit = (unsigned char*)0;
  int       i, k, linesize;

  typedef struct _tag_byte_bit {
    unsigned    bit0: 1;
    unsigned      bit1: 1;
    unsigned    bit2: 1;
    unsigned    bit3: 1;
    unsigned    bit4: 1;
    unsigned    bit5: 1;
    unsigned    bit6: 1;
    unsigned    bit7: 1;
  } BIT_DATA;

  union {
    unsigned char   uc_data;
    BIT_DATA    bit_data;
  } mylcd_bit;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* �������ͼ��λ���� */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&lcd_save_img_critical);

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
      if (_lcd_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* �������ͬ��������������������ȷ����ʾ */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ����ļ�ͷ */
      bfh.bfType = LCD_HEADER_MARKER;
      bfh.width  = (unsigned short)pinfo_str->width;
      bfh.hight  = (unsigned short)pinfo_str->height;
      bfh.Size   = pinfo_str->width * ((pinfo_str->height + 7) / 8);
      bfh.bwidth = (unsigned char)pinfo_str->width;
      bfh.bhight = (unsigned char)pinfo_str->height;
      bfh.bSize  = ((((unsigned short)bfh.Size) << 4) & 0x0f) | ((((unsigned short)bfh.Size) >> 4) & 0xf0);

      /* д���ļ�ͷ�ṹ */
      if (isio_write((const void*)&bfh, sizeof(LCD_FILE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* ȡ��ɨ���гߴ� */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      /* ������Ϊ8�ı���--�����ڴ� */
      p_pre_bit = (unsigned char*)isirw_malloc(8L * linesize * ((pinfo_str->height + 7) / 8) + 4);

      if (p_pre_bit == 0) {
        b_status = ER_MEMORYERR;
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

      for (i = 0; i < (int)(pinfo_str->height); i++) {
        memmove((void*)(p_pre_bit + linesize * i), (const void*)(unsigned char*)(pinfo_str->pp_line_addr[i]), linesize);
      }

      /* ��-8���ƶ� */
      for (i = 0; i < (int)(pinfo_str->height); i = i + 8) {
        /* ���ƶ� */
        for (k = 0; k < (int)(pinfo_str->width); k++) {
          mylcd_bit.uc_data = 0;
          /* ���г�ȡ��������ƴ��Ϊ������ */
          mylcd_bit.bit_data.bit0 = _get_byte_bit(*(p_pre_bit + linesize * i + k / 8) , (unsigned char)((7 - (k % 8))));
          mylcd_bit.bit_data.bit1 = _get_byte_bit(*(p_pre_bit + linesize * (i + 1) + k / 8) , (unsigned char)((7 - (k % 8))));
          mylcd_bit.bit_data.bit2 = _get_byte_bit(*(p_pre_bit + linesize * (i + 2) + k / 8) , (unsigned char)((7 - (k % 8))));
          mylcd_bit.bit_data.bit3 = _get_byte_bit(*(p_pre_bit + linesize * (i + 3) + k / 8) , (unsigned char)((7 - (k % 8))));
          mylcd_bit.bit_data.bit4 = _get_byte_bit(*(p_pre_bit + linesize * (i + 4) + k / 8) , (unsigned char)((7 - (k % 8))));
          mylcd_bit.bit_data.bit5 = _get_byte_bit(*(p_pre_bit + linesize * (i + 5) + k / 8) , (unsigned char)((7 - (k % 8))));
          mylcd_bit.bit_data.bit6 = _get_byte_bit(*(p_pre_bit + linesize * (i + 6) + k / 8) , (unsigned char)((7 - (k % 8))));
          mylcd_bit.bit_data.bit7 = _get_byte_bit(*(p_pre_bit + linesize * (i + 7) + k / 8) , (unsigned char)((7 - (k % 8))));

          if (isio_write((const void*)&mylcd_bit.uc_data, 1, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
          }
        }
      }

      /* ����ڴ� */
      isirw_free(p_pre_bit);

      p_pre_bit = 0;

      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_SAVE)) {
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
      if (p_pre_bit) {
        isirw_free(p_pre_bit);
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

      LeaveCriticalSection(&lcd_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}





/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

/* ����ɨ���гߴ�(���ֽڶ���) */
int CALLAGREEMENT _calcu_scanline_size(int w/* ��� */, int bit/* λ�� */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* �жϴ����ͼ���Ƿ���Ա����棨�粻֧�ֱ��湦�ܣ��ɽ�������ȥ���� */
int CALLAGREEMENT _lcd_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* λ����һ������32λ��ʽ�޷���ʾ����LONGΪ32λ��*/
  if (!(lcd_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* ��֧�ֵ�λ��ͼ�� */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (lcd_irwp_info.irwp_save.img_num) {
    if (lcd_irwp_info.irwp_save.img_num == 1) {
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* ͼ���������ȷ */
      }
    }
  }

  return 0;
}


unsigned  char CALLAGREEMENT _get_byte_bit(unsigned char my_byte, unsigned char bit)
{
  unsigned char result = 0;

  typedef struct _tag_byte_bit {
    unsigned    bit0: 1;
    unsigned      bit1: 1;
    unsigned    bit2: 1;
    unsigned    bit3: 1;
    unsigned    bit4: 1;
    unsigned    bit5: 1;
    unsigned    bit6: 1;
    unsigned    bit7: 1;
  } BIT_DATA;

  union {
    unsigned char   uc_data;
    BIT_DATA    bit_data;
  } my_bit;

  my_bit.uc_data = my_byte;

  switch (bit) {
  case 0x0:
    result = my_bit.bit_data.bit0;
    break;

  case 0x1:
    result = my_bit.bit_data.bit1;
    break;

  case 0x2:
    result = my_bit.bit_data.bit2;
    break;

  case 0x3:
    result = my_bit.bit_data.bit3;
    break;

  case 0x4:
    result = my_bit.bit_data.bit4;
    break;

  case 0x5:
    result = my_bit.bit_data.bit5;
    break;

  case 0x6:
    result = my_bit.bit_data.bit6;
    break;

  case 0x7:
    result = my_bit.bit_data.bit7;
    break;
  }

  return result;  /* λ���� */
}