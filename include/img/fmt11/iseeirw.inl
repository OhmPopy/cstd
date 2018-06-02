/********************************************************************

  iseeirw.c

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
  ���ļ���;��  ͼ���д���棭����ӿ�ʵ���ļ�
  ���ļ���д�ˣ�
          YZ        yzfree##sina.com

  ���ļ��汾��  30723
  ����޸��ڣ�  2003-7-23

  ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
  ��ַ�ռ������
  ----------------------------------------------------------------
  ������ʷ��

    2003-7    ����isirw_add_idle()��isirw_get_idlesse_count()�������ú����ӿ�
    2003-7    ����isirw_cnv�д��ڵ�����BUG��ԭ�����Զ�ת����֧�������⣩
    2003-5    �޸�isirw_update()����ԭ�ͣ����ӷ�����Ϣ�ṹ
          �޸�isirw_get_eng_info()����ԭ�ͣ����ӻ�ȡ���·������
    2003-4    �Ľ�isirw_s_read_img_c()�ӿں������̰߳�ȫ���ܡ�
          ����isirw_cnv()�ӿں�����������ת����Ϊ���ơ�
    2003-4    ����isirw_orgpix_malloc()��isirw_cnvpix_malloc()���ýӿڡ�
    2003-2    ����isirw_s_wait()�ӿں������Ľ����߳����ԡ�
    2002-12   �������׶�д�ӿ��е�����BUG����л�������ﳬ��
    2002-10   �°�ͼ���д�����д��ϡ�
    2002-8    ͼ���д���漰ͼ��ת�����沢�뱾ģ�顣
          ����ӿ�������2.2��
          �����ڴ�������
    2002-3    ���Ӳ���˳���������ص�INFOSTR���估�ͷź���
    2002-1    ������̬ͼ���ļ����ܰ������ͼ��Ĵ���ע��
    2001-6    ��һ�����԰淢��

********************************************************************/

#ifndef WIN32
#if defined(_WIN32) || defined(_WINDOWS)
#define WIN32
#endif
#endif /* WIN32 */

#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#ifdef WIN32
#include <pmalloc.h>
#else
#include <mem.h>
#endif

#include "iseemem.h"
#include "iseeirw.h"
#include "iseecnv.h"
#include "iseeimn.h"
#include "queue.h"
#include "iseeeng.h"



/**********************************************************************************/
/* �ڲ������� */

#ifdef _DEBUG
static unsigned long sct_count = 0;           /* �ṹʵ����������ݰ��ĸ�����*/
static CRITICAL_SECTION  sct_access;          /* ������������ɱ�ʶ */
#endif

static int      _isirw_mark = 0;          /* ����ϵͳ���ñ�־ */
static int      _isirw_lock = 0;          /* ����������־ */
static int      _isirw_init_result = -10;     /* �����ʼ���Ľ������isirw_init()�����ķ���ֵ��*/

static int      _s_last_err = ISIRW_LASTERR_SUCCESS;/* רҵ�ͽӿ����һ�β����Ĵ�����Ϣ */
static int      _f_last_err = ISIRW_LASTERR_SUCCESS;/* �����ͽӿ����һ�β����Ĵ�����Ϣ */

static unsigned int _f_img_order = 1;         /* ���������ݰ�ͼ�������˳��ȱʡΪ���� */
/* ����Ҫ���ֶ��޸�����ֵ���Ըı�������Ϊ */
/* 0������1������ */

static unsigned int   _isirw_buff_mark = 0;     /* ���û�����������ǣ�IRWE_BUFTYPE_ORG|IRWE_BUFTYPE_CNV��*/
static unsigned char*  _isirw_porg     = 0;     /* ԭʼ���ػ�������ַ */
static unsigned char*  _isirw_pcnv     = 0;     /* ת�����ػ�������ַ */

#ifdef WIN32
#pragma pack(push)
#pragma pack(4)
#endif

static int      _isirw_org_used = 0;        /* ԭʼ���ػ�����ʹ�ñ�־����0��ʹ�ã�*/
static int      _isirw_cnv_used = 0;        /* ת�����ػ�����ʹ�ñ�־ */

#ifdef WIN32
#pragma pack(pop)
#endif

static char* IRWP_API_NAME_STRING[4] = { "", "get_image_info", "load_image", "save_image"};
/* ��д����ӿں���ͨ���� */




/**********************************************************************************/
/* �ڲ��������� */

static void CALLAGREEMENT _free_subimg(LPINFOSTR pinfo);
static int  CALLAGREEMENT _defalt_process(LPSPEC_DATAPACK, PFUN_COMM, int, int, int, int);
ISEEIRW_API IRWPFUN CALLAGREEMENT isirw_set_pfun(LPINFOSTR pinfo, IRWPFUN new_pfun);

ISEEIRW_API LPSUBIMGBLOCK CALLAGREEMENT isirw_alloc_SUBIMGBLOCK();
ISEEIRW_API void      CALLAGREEMENT isirw_free_SUBIMGBLOCK(LPSUBIMGBLOCK p_node);

int     CALLAGREEMENT _isirw_get_last_sert_image_info(LPINFOSTR pinfo, LPSUBIMGBLOCK* ppsi);
int     CALLAGREEMENT _isirw_get_sert_image_info(LPINFOSTR pinfo, int sid, LPSUBIMGBLOCK* ppsi);

LPINFOSTR         CALLAGREEMENT _isirw_build_mi(LPSPEC_DATAPACK sdp, int fmt);
LPSUBIMGBLOCK       CALLAGREEMENT _isirw_build_si(LPSUBIMGBLOCK psub_str, int fmt);






/**********************************************************************************/
/* ����ӿں��� */
/**********************************************************************************/

/* ��ʼ��ͼ���д���� */
ISEEIRW_API int CALLAGREEMENT isirw_init(char* path, char* fname)
{
  int     result = 0;

  assert(_isirw_mark == 0);             /* ֻ�ܳ�ʼ��һ�� */

  __try {
    __try {
      /* �����������ģ�� */
      if ((result = ismf_init(path, fname)) != 0) {
        __leave;
      }

      /* ������Ϣ��ģ�� */
      if ((result = open_pump()) != 0) {
        __leave;
      }

      /* �����������ñ�־ */
      _isirw_mark = 1;
    }
    __finally {
      if ((result != 0) || (AbnormalTermination())) {
        if (ismf_get_status()) {
          ismf_close();
        }

        if (get_pump_status()) {
          close_pump();
        }
      }

      /* ��ʼ�����ػ��������� */
      _isirw_buff_mark = 0;           /* ���û������������ */
      _isirw_porg      = 0;           /* ԭʼ���ػ�������ַ */
      _isirw_pcnv      = 0;           /* ת�����ػ�������ַ */
      _isirw_org_used  = 0;           /* ԭʼ���ػ�����ʹ�ñ�־����0��ʹ�ã�*/
      _isirw_cnv_used  = 0;           /* ת�����ػ�����ʹ�ñ�־ */
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = -5;
  }

  _isirw_init_result = result;

  /*  0 :�ɹ� */
  /* -1 :�ڴ治�� */
  /* -2 :�޿��ò�� */
  /* -3 :�ļ���д���� */
  /* -4 :INI�ļ����ݷǷ� */
  /* -5 :ϵͳ�쳣 */
  /* -6 :��Ϣ���̴߳���ʧ�� */
  return result;
}


/* �ر�ͼ���д���棬���ͷ��й���Դ */
ISEEIRW_API void CALLAGREEMENT isirw_close()
{
  assert(_isirw_mark == 1);
  assert(get_pump_status());
  assert(ismf_get_status());

  __try {
    /* �ر���Ϣ�ü��������ģ�� */
    close_pump();
    ismf_close();
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    ;
  }

  /* ��������߿��������ػ���������ر��� */
  isirw_dibuffer(IRWE_BUFTYPE_ORG | IRWE_BUFTYPE_CNV);

  _isirw_mark = 0;

  return;
}


/* ����ͼ���д���� */
ISEEIRW_API LPIRWMF_HEADER CALLAGREEMENT isirw_lock()
{
  LPIRWMF_HEADER result = 0;

  assert(_isirw_mark == 1);

  __try {
    /* ������Ϣ�� */
    lock_pump();

    /* ���������Ϣ�������ر��ַ */
    result = ismf_lock();

    _isirw_lock = 1;
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = 0;
  }

  /* �ɹ�������ϵͳ�����ز����Ϣ���ַ */
  /* ʧ�ܣ�����0 */
  return result;
}


/* ˢ�²����Ϣ�� */
ISEEIRW_API int CALLAGREEMENT isirw_update(LPIRWMF_HEADER pt, int type, LPUPDATAINFO rinfo)
{
  int result = 0;

  /* ˢ���ڼ䣬ϵͳ���봦������״̬ */
  assert(pt);
  assert(_isirw_mark == 1);
  assert(_isirw_lock == 1);
  assert((type == IRWE_UPDTYPE_EMPLOY) || (type == IRWE_UPDTYPE_DISMISSAL));

  __try {
    if (rinfo) {
      /* ���÷�����Ϣ�ṹ */
      memset((void*)rinfo, 0, sizeof(UPDATAINFO));
    }

    /* ˢ�²����Ϣ�� */
    result = ismf_update(pt, type, rinfo);
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = 4;
  }

  /* 0: ˢ�³ɹ� */
  /* 1: ��������û���κ�ͼ���д��������潫�޷�����������*/
  /* 2: ��д���� */
  /* 3: �ڴ治�� */
  /* 4: ϵͳ�쳣 */
  return result;
}


/* ¼��ָ����� */
ISEEIRW_API int CALLAGREEMENT isirw_employ(int id /*��¼�ò����ID����*/)
{
  int result = 0;

  /* ˢ���ڼ䣬ϵͳ���봦������״̬ */
  assert(_isirw_mark == 1);
  assert(_isirw_lock == 1);

  __try {
    /* ��ָ����δ�ò��ת��¼���������������*/
    ismf_employ(id);
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = 1;
  }

  /* 0: �ɹ� */
  /* 1: ϵͳ�쳣 */
  return result;
}


/* ����ָ�������ȡ��¼�ã�*/
ISEEIRW_API int CALLAGREEMENT isirw_dismissal(int id /*������͡������ID����*/)
{
  int result = 0;

  /* ˢ���ڼ䣬ϵͳ���봦������״̬ */
  assert(_isirw_mark == 1);
  assert(_isirw_lock == 1);

  __try {
    /* ��ָ����¼�ò��ת��δ����������ò���ѱ��������ʹ�����ߣ�*/
    ismf_dismissal(id);
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = 1;
  }

  /* 0: �ɹ� */
  /* 1: ϵͳ�쳣 */
  return result;
}


/* ����ͼ���д���� */
ISEEIRW_API int CALLAGREEMENT isirw_unlock(LPIRWMF_HEADER pt)
{
  int result = 0;

  assert(_isirw_mark == 1);
  assert(pt && (pt->tag == IRWMF_HEADER_TAG));

  __try {
    /* �����Ϣ����� */
    ismf_unlock(pt);

    /* ��Ϣ�ý��� */
    unlock_pump();

    _isirw_lock = 0;
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = 1;
  }

  /* 0: �����ɹ� */
  /* 1: ϵͳ�쳣 */
  return result;
}


/* �����������ػ����� */
ISEEIRW_API int CALLAGREEMENT isirw_enbuffer(int type)
{
  int iRet = 0;

  assert(_isirw_mark == 1);

  /* ����ԭʼ���ػ����� */
  if (type & IRWE_BUFTYPE_ORG) {
    if (!(_isirw_buff_mark & IRWE_BUFTYPE_ORG)) {
      if ((_isirw_porg = (unsigned char*)im_malloc(IRWE_MAX_BUFFSIZE)) != 0) {
        _isirw_buff_mark |= IRWE_BUFTYPE_ORG;
      }
      else {
        iRet = -1;                /* �ڴ治�� */
      }
    }
  }

  /* ����ת�����ػ����� */
  if (type & IRWE_BUFTYPE_CNV) {
    if (!(_isirw_buff_mark & IRWE_BUFTYPE_CNV)) {
      if ((_isirw_pcnv = (unsigned char*)im_malloc(IRWE_MAX_BUFFSIZE)) != 0) {
        _isirw_buff_mark |= IRWE_BUFTYPE_CNV;
      }
      else {
        iRet = -1;                /* �ڴ治�� */
      }
    }
  }

  return iRet;
}


/* �ر��������ػ����� */
ISEEIRW_API int CALLAGREEMENT isirw_dibuffer(int type)
{
  int iRet = 0;

  assert(_isirw_mark == 1);

  /* �ر�ԭʼ���ػ����� */
  if (type & IRWE_BUFTYPE_ORG) {
    if (_isirw_buff_mark & IRWE_BUFTYPE_ORG) {
      assert(_isirw_porg);
      assert(_isirw_org_used == 0);       /* �ͷŻ�����ʱ���û��������봦�ڿ���״̬ */

      im_free(_isirw_porg);
      _isirw_porg = 0;
      _isirw_buff_mark &= ~IRWE_BUFTYPE_ORG;
    }
  }

  /* �ر�ת�����ػ����� */
  if (type & IRWE_BUFTYPE_CNV) {
    if (_isirw_buff_mark & IRWE_BUFTYPE_CNV) {
      assert(_isirw_pcnv);
      assert(_isirw_cnv_used == 0);

      im_free(_isirw_pcnv);
      _isirw_pcnv = 0;
      _isirw_buff_mark &= ~IRWE_BUFTYPE_CNV;
    }
  }

  return iRet;
}


/* ��ȡ���������һЩ��Ϣ����������汾������ӿڰ汾������״̬��*/
ISEEIRW_API void CALLAGREEMENT isirw_get_eng_info(
    int* peng_ver,    /* ���ص�ǰ����İ汾�� */
    int* pplug_ver,   /* ���ص�ǰ������ʹ�õĶ�д����ӿڰ汾�� */
    int* pdebug,    /* ����ϵͳ�Ƿ��ǵ��԰汾��0������״̬��1������״̬��*/
    char* path)     /* ���ز�����ڵ�·�� */
{
  assert(_isirw_mark == 1);

  if (peng_ver) {
    *peng_ver = ISEEIRW_ENG_VER;          /* ����汾 */
  }

  if (pplug_ver) {
    *pplug_ver = ISEEIRW_ENG_PLUGIN_VER;      /* ����ӿڰ汾 */
  }

  if (pdebug) {
#ifdef _DEBUG
    *pdebug = 0;                  /* ����״̬ */
#else
    *pdebug = 1;                  /* ����״̬ */
#endif /* _DEBUG */
  }

  if (path) {
    ismf_get_plugin_path(path);           /* ��ȡ�������·����β������'\'���ţ�*/
  }
}


/* ��ȡ����æµ�̶ȵ���Ϣ */
ISEEIRW_API int CALLAGREEMENT isirw_is_busy()
{
  unsigned long cnt;
  int       result;

  assert(_isirw_mark == 1);

  /* ��ȡ��Ϣ���л��ж��ٸ������������ */
  cnt = get_comm_count();

  if (cnt == 0) {
    result = 0;                   /* �� */
  }
  else if (cnt < 10) {
    result = 1;                   /* æ */
  }
  else {
    result = 2;                   /* ��æ */
  }

  if (_isirw_lock == 1) {
    result = 3;                   /* ϵͳ������ */
  }

  return result;
}


/* ���������ģ�����ýӿڣ��������ṩ���ļ�·������������Ӧ�������Ϣ���ַ */
int CALLAGREEMENT _search_plugin_info_block(LPIRWMF_INFOBLOCK* ppblock, const char* path);

/* �ж�ָ����ͼ���Ƿ��ܱ����洦������չ���жϣ�*/
ISEEIRW_API int CALLAGREEMENT isirw_is_supimg(const char* path)
{
  LPIRWMF_INFOBLOCK piib;             /* ռλ�� */

  assert(_isirw_mark == 1);

  /* 1 - ���Ա����洦�� */
  /* 0 - ���ܱ����洦�� */
  return (_search_plugin_info_block(&piib, path) == 1) ? 1 : 0;
}


/* �������ģ���л�ȡ���˴������ýӿ� */
char* CALLAGREEMENT _ismf_get_openstr(void);
char* CALLAGREEMENT _ismf_get_shellstr(void);

/* ��ȡ����Windowsƽ̨Open�Ի�����ļ���չ�����˴� */
/* ����ʽ��μ�MSDN���й�OPENFILENAME�ṹ���ĵ������е�lpstrFilter��Ա��*/
ISEEIRW_API char* CALLAGREEMENT isirw_get_openstr()
{
  assert(_isirw_mark == 1);

  return _ismf_get_openstr();
}


/* ��ȡͨ���ļ���չ�����˴� */
/* ����ʽΪ��"*.bmp,*.gif,*.xxx,....." */
ISEEIRW_API char* CALLAGREEMENT isirw_get_shellstr()
{
  assert(_isirw_mark == 1);

  return _ismf_get_shellstr();
}


/* �ж������Ƿ����ɹ� */
ISEEIRW_API int CALLAGREEMENT isirw_is_ok()
{
  /*  0 :�ɹ����� */
  /* -1 :�ڴ治�� */
  /* -2 :�޿��ò�� */
  /* -3 :�ļ���д���� */
  /* -4 :INI�ļ����ݷǷ� */
  /* -5 :ϵͳ�쳣 */
  /* -6 :��Ϣ���̴߳���ʧ�� */
  /* -10:���滹δ����ʼ�� */
  return _isirw_init_result;
}




/**********************************************************************************/
/* רҵͼ���д�ӿ� */
/**********************************************************************************/

/* ���ͼ��Ŀ��ߡ�λ��ȵ���Ϣ */
ISEEIRW_API LPSPEC_DATAPACK CALLAGREEMENT isirw_s_get_img_info(ISEEIO_CONFER_TYPE type, const char* name, unsigned long par1, unsigned long par2, unsigned long sn)
{
  LPSPEC_DATAPACK   psdp = 0;

  assert(_isirw_mark != 0);             /* ����ϵͳ�����Ѿ����� */
  assert(name);

  /* ��ʼ������������ */
  _s_last_err = ISIRW_LASTERR_SUCCESS;

  __try {
    __try {
      /* ���벢��ʼ��һ��רҵ���ݰ� */
      if ((psdp = isirw_alloc_SPEC_DATAPACK(type, name, par1, par2, 0)) == 0) {
        _s_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }

      /* ��д������� */
      psdp->command     = IRWE_SDPCOMM_GET_INFO;
      psdp->data_state |= SDP_DATA_STATE_PATH;
      psdp->exec_state  = IRWE_SDPSTATE_IDLE;
      psdp->last_err    = ER_SUCCESS;
      psdp->s_break     = 0;
      psdp->sn          = sn;
      psdp->callback    = 0;

      /* ������Ϣ�� */
      if (add_pump_comm(psdp) == -1) {
        _s_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }
    }
    __finally {
      if ((_s_last_err != ISIRW_LASTERR_SUCCESS) || (AbnormalTermination())) {
        if (psdp) {
          isirw_free_SPEC_DATAPACK(psdp);
          psdp = 0;
        }
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    _s_last_err = ISIRW_LASTERR_OSERR;
  }

  return psdp;
}


/* ���ͼ����������ݣ����б�Ҫ�ɽ���λ���ת����*/
ISEEIRW_API LPSPEC_DATAPACK CALLAGREEMENT isirw_s_read_img(LPSPEC_DATAPACK psdp, IRWE_CNVMARK cnv, void* fn)
{
  assert(_isirw_mark != 0);             /* ����ϵͳ�����Ѿ����� */
  assert(psdp);

  /* ��ʼ������������ */
  _s_last_err = ISIRW_LASTERR_SUCCESS;

  __try {
    __try {
      /* ��д������� */
      psdp->command     = IRWE_SDPCOMM_LOAD;
      psdp->exec_state  = IRWE_SDPSTATE_IDLE;
      psdp->last_err    = ER_SUCCESS;
      psdp->cnvmark     = cnv;
      psdp->s_break     = 0;
      psdp->callback    = (IRWPFUN)fn;

      /* ������Ϣ�� */
      if (add_pump_comm(psdp) == -1) {
        _s_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }
    }
    __finally {
      if ((_s_last_err != ISIRW_LASTERR_SUCCESS) || (AbnormalTermination())) {
        psdp->last_err   = ER_MEMORYERR;
        psdp->exec_state = IRWE_SDPSTATE_IDLE;
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    _s_last_err = ISIRW_LASTERR_OSERR;
  }

  return psdp;
}


/* ֱ�ӻ��ͼ����������ݣ����б�Ҫ�ɽ���λ���ת����������Ҫ���ȵ���isirw_s_get_img_info������*/
ISEEIRW_API int CALLAGREEMENT isirw_s_read_img_c(ISEEIO_CONFER_TYPE type, const char* name, unsigned long par1, unsigned long par2, unsigned long sn, IRWE_CNVMARK cnv, void* fn, LPSPEC_DATAPACK* pppack)
{
  LPSPEC_DATAPACK   psdp = 0;

  assert(_isirw_mark != 0);             /* ����ϵͳ�����Ѿ����� */
  assert(name);

  /* ��ʼ������������ */
  _s_last_err = ISIRW_LASTERR_SUCCESS;

  __try {
    __try {
      *pppack = 0;

      /* ���벢��ʼ��һ��רҵ���ݰ� */
      if ((psdp = isirw_alloc_SPEC_DATAPACK(type, name, par1, par2, cnv)) == 0) {
        _s_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }

      /* ��д������� */
      psdp->command     = IRWE_SDPCOMM_LOAD;
      psdp->data_state |= SDP_DATA_STATE_PATH;
      psdp->exec_state  = IRWE_SDPSTATE_IDLE;
      psdp->last_err    = ER_SUCCESS;
      psdp->s_break     = 0;
      psdp->sn          = sn;
      psdp->callback    = (IRWPFUN)fn;

      /* �Ȼ���רҵ����ַ���Ա��������߳�ͨ���ص������������������
         ��������ʱ���������ܻ�δ���ü����أ���*/
      *pppack = psdp;

      /* ������Ϣ�� */
      if (add_pump_comm(psdp) == -1) {
        _s_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }
    }
    __finally {
      if ((_s_last_err != ISIRW_LASTERR_SUCCESS) || (AbnormalTermination())) {
        if (psdp) {
          isirw_free_SPEC_DATAPACK(psdp);
          psdp = 0;
        }

        *pppack = 0;
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    _s_last_err = ISIRW_LASTERR_OSERR;
  }

  return _s_last_err;
}


/* ��Դרҵ���ݰ��е�ͼ�����ݱ��浽ָ���Ľ����� */
ISEEIRW_API int CALLAGREEMENT isirw_s_write_img(ISEEIO_CONFER_TYPE type, const char* name, unsigned long par1, unsigned long par2, LPSPEC_DATAPACK psdp, LPSAVESTR savedec, void* fn)
{
  int   result = 0;

  assert(_isirw_mark != 0);             /* ����ϵͳ�����Ѿ����� */
  assert(psdp);

  /* ��ʼ������������ */
  _s_last_err = ISIRW_LASTERR_SUCCESS;

  __try {
    __try {
      /* ��д������� */
      psdp->command     = IRWE_SDPCOMM_SAVE;
      psdp->exec_state  = IRWE_SDPSTATE_IDLE;
      psdp->last_err    = ER_SUCCESS;
      psdp->s_break     = 0;
      psdp->callback    = (IRWPFUN)fn;

      /* ���Ʊ������ */
      memcpy((void*)psdp->savestr->para_value, (const void*)savedec->para_value, sizeof(int)*ISD_ITEM_NUM);

      /* ��дĿ��ͼ��·�� */
      isio_open_param(psdp->despath, type, name, par1, par2);

      /* ������Ϣ�� */
      if (add_pump_comm(psdp) == -1) {
        _s_last_err = ISIRW_LASTERR_MEM;
        result = 1;               /* �ڴ治�� */
        __leave;
      }
    }
    __finally {
      if ((_s_last_err != ISIRW_LASTERR_SUCCESS) || (AbnormalTermination())) {
        psdp->last_err   = ER_MEMORYERR;
        psdp->exec_state = IRWE_SDPSTATE_IDLE;
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = 2;                   /* ����ϵͳ���ȶ� */
    _s_last_err = ISIRW_LASTERR_OSERR;
  }

  return result;
}


/* ����ָ�����ε�����������Ѿ�ִ�е������޷�������*/
ISEEIRW_API int CALLAGREEMENT isirw_dist_sn(unsigned long sn)
{
  int   count = 0;

  assert(_isirw_mark == 1);

  /* �������л�û��ִ�е�sn���ε����� */
  while (dist_pump_comm_sn(sn)) {
    count++;                    /* ��������������� */
  }

  return count;
}


/* ��ȡרҵ�Ͷ�д�ӿڵ����һ�β����Ĵ������ */
ISEEIRW_API int CALLAGREEMENT isirw_s_get_last_err()
{
  return _s_last_err;
}


/* ��ȡISee��׼��ʽ����ظ�ʽ��Ϣ */
ISEEIRW_API int CALLAGREEMENT isirw_s_get_id_info(int id,
    unsigned long* bit,
    unsigned long* rmask,
    unsigned long* gmask,
    unsigned long* bmask,
    unsigned long* amask,
    int* pal_size,
    LPPALITEM* pppal)
{
  return iscnv_get_id_info(id, bit, rmask, gmask, bmask, amask, pal_size, pppal);
}


/* �ȴ�רҵ�����ݰ�ִ����� */
ISEEIRW_API enum EXERESULT CALLAGREEMENT isirw_s_wait(LPSPEC_DATAPACK psdp, int defer)
{
  clock_t ctime = clock();
  clock_t mspf  = CLOCKS_PER_SEC / 1000;      /* �ֲڵļ�ʱ������ÿ������������*/

  assert(_isirw_mark != 0);             /* ����ϵͳ�����Ѿ����� */
  assert(psdp);

  /* �����жϲ��� */
  if (defer == -1) {
    psdp->s_break = 1;                /* �����жϲ������ */

    /* �ȴ�����ִ����ϣ���������������������жϽ����� */
    while (psdp->exec_state != IRWE_SDPSTATE_IDLE) {
      Sleep(0);                 /* �ͷ�ʣ���ʱ��Ƭ */
    }

    psdp->s_break = 0;                /* �ָ��жϲ������ */

    return psdp->last_err;
  }

  /* �¼��ȴ���ʽ����δ��ɣ��ݱ����˽ӿڣ� */
  if (defer == -2) {
    ;
  }

  /* ��ָ��ʱ���ڵȴ����ݰ�ִ����� */
  while (psdp->exec_state != IRWE_SDPSTATE_IDLE) {
    if ((defer) && (((clock() - ctime) / mspf) >= defer)) {
      break;                    /* �����ʱ���ٵȴ� */
    }
    else {
      Sleep(0);                 /* �ͷ�ʣ���ʱ��Ƭ */
    }
  }

  if (psdp->exec_state == IRWE_SDPSTATE_IDLE) {   /* ���ز������ */
    return psdp->last_err;
  }
  else {                      /* �ӳ�ʱ����� */
    return ER_WAITOUTTIME;
  }
}


// ��ȡרҵ�����ݰ���ָ����ͼ��Ľڵ��ַ��LPSUBIMGBLOCK��
ISEEIRW_API LPSUBIMGBLOCK CALLAGREEMENT isirw_s_get_subimg(LPSPEC_DATAPACK psdp, int subid, int type)
{
  LPSUBIMGBLOCK psub;
  int       i;

  assert(_isirw_mark != 0);             /* ����ϵͳ�����Ѿ����� */
  assert(psdp && psdp->org_img);
  assert(subid < (int)psdp->org_img->imgnumbers);   /* ��ͼ������ֵ��ȻҪС��ͼ�����ֵ */
  assert(subid > 0);                  /* �������������ڻ�ȡ��ͼ��Ľṹ��ַ */

  // ȷ��Ҫ���������ݰ���ԭʼ����ת������
  psub = (type == 0) ? psdp->org_img->psubimg : psdp->cnv_img->psubimg;

  assert(psub);

  // ������ͼ������
  for (i = 1; i < (int)psdp->org_img->imgnumbers; i++, psub = psub->next) {
    if (i == subid) {
      assert(psub->number == i);          /* У��������ͼ�����кŵ������Ƿ���ȷ */
      return psub;
    }
  }

  return 0;
}



/**********************************************************************************/
/* ����ͼ���д�ӿ� */
/**********************************************************************************/

/* ��ȡָ����ͼ���ļ� */
ISEEIRW_API LPFACI_DATAPACK CALLAGREEMENT isirw_f_read_img(const char* filename, IRWE_CNVMARK cnv)
{
  LPFACI_DATAPACK   pfdp = 0;           /* �������ݰ� */
  LPSPEC_DATAPACK   psdp = 0;           /* רҵ���ݰ� */

  int         linesize, i;
  unsigned char*   p = 0;

  assert(_isirw_mark != 0);             /* ����ϵͳ�����Ѿ����� */
  assert(filename);
  assert(cnv != IRWE_CNVMARK_NOT);          /* ���������ݰ��е�ͼ�����ݱ�����ISee��׼��ʽ */

  /* ��ʼ������������ */
  _f_last_err = ISIRW_LASTERR_SUCCESS;

  __try {
    __try {
      /* ����һ�����������ݰ� */
      if ((pfdp = isirw_alloc_FACI_DATAPACK()) == 0) {
        _f_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }

      /* ���벢��ʼ��һ��רҵ���ݰ� */
      if ((psdp = isirw_alloc_SPEC_DATAPACK(ISEEIO_CONFER_LOC_FILE, filename, 0, 0, cnv)) == 0) {
        _f_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }

      /* ��д������� */
      psdp->command     = IRWE_SDPCOMM_LOAD;
      psdp->data_state |= SDP_DATA_STATE_PATH;
      psdp->exec_state  = IRWE_SDPSTATE_IDLE;
      psdp->last_err    = ER_SUCCESS;
      psdp->s_break     = 0;
      psdp->sn          = 0;            /* ���װ����κ�Ϊ0 */
      psdp->callback    = (IRWPFUN)0;

      /* ������Ϣ�� */
      if (add_pump_comm(psdp) == -1) {
        _f_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }

      /* �ȴ���ȡ����ִ����� */
      while (psdp->exec_state != IRWE_SDPSTATE_IDLE) {
        Sleep(0);
      }

      /* �����ȡ�ɹ������ݴ�רҵ��ת����װ� */
      if (psdp->last_err == ER_SUCCESS) {
        assert(psdp->org_img->p_bit_data);

        /* ���������λ���ת������ȡת������������� */
        if (psdp->cnv_img) {
          linesize = DIBSCANLINE_DWORDALIGN(psdp->cnv_img->bitcount * psdp->cnv_img->width);

          pfdp->width    = psdp->cnv_img->width;
          pfdp->height   = psdp->cnv_img->height;
          pfdp->bitcount = psdp->cnv_img->bitcount;
          memcpy((void*)pfdp->palette, (const void*)psdp->cnv_img->palette, sizeof(unsigned long)*MAX_PALETTE_COUNT);

          /* ��������ֱ�Ӵ�ת������ժȡ */
          pfdp->pbits    = psdp->cnv_img->p_bit_data;
          psdp->cnv_img->p_bit_data = 0;

          /* ��д��׼��ʽID�� */
          pfdp->std_id = iscnv_is_std(pfdp->bitcount, psdp->cnv_img->r_mask, psdp->cnv_img->g_mask, psdp->cnv_img->b_mask, psdp->cnv_img->a_mask);
          assert(pfdp->std_id != -1);

          /* ��������ͼ������˳������װ��Ĺ���˳��ͬ����ߵ�֮ */
          if (_f_img_order != psdp->cnv_img->order) {
            /* ����ߵ��û����� */
            if ((p = (unsigned char*)im_malloc(linesize)) == 0) {
              _f_last_err = ISIRW_LASTERR_MEM;
              __leave;
            }

            /* �ߵ�ͼ������˳�� */
            for (i = 0; i < (int)psdp->cnv_img->height / 2; i++) {
              memcpy((void*)p, (const void*)psdp->cnv_img->pp_line_addr[i], linesize);
              memcpy((void*)psdp->cnv_img->pp_line_addr[i], (void*)psdp->cnv_img->pp_line_addr[(int)psdp->cnv_img->height - i - 1], linesize);
              memcpy((void*)psdp->cnv_img->pp_line_addr[(int)psdp->cnv_img->height - i - 1], (const void*)p, linesize);
            }
          }
        }
        else {
          linesize = DIBSCANLINE_DWORDALIGN(psdp->org_img->bitcount * psdp->org_img->width);

          pfdp->width    = psdp->org_img->width;
          pfdp->height   = psdp->org_img->height;
          pfdp->bitcount = psdp->org_img->bitcount;
          memcpy((void*)pfdp->palette, (const void*)psdp->org_img->palette, sizeof(unsigned long)*MAX_PALETTE_COUNT);

          /* ��������ֱ�Ӵ�ԭʼ����ժȡ */
          pfdp->pbits    = psdp->org_img->p_bit_data;
          psdp->org_img->p_bit_data = 0;

          pfdp->std_id = iscnv_is_std(pfdp->bitcount, psdp->org_img->r_mask, psdp->org_img->g_mask, psdp->org_img->b_mask, psdp->org_img->a_mask);
          assert(pfdp->std_id != -1);

          if (_f_img_order != psdp->org_img->order) {
            if ((p = (unsigned char*)im_malloc(linesize)) == 0) {
              _f_last_err = ISIRW_LASTERR_MEM;
              __leave;
            }

            for (i = 0; i < (int)psdp->org_img->height / 2; i++) {
              memcpy((void*)p, (const void*)psdp->org_img->pp_line_addr[i], linesize);
              memcpy((void*)psdp->org_img->pp_line_addr[i], (void*)psdp->org_img->pp_line_addr[(int)psdp->org_img->height - i - 1], linesize);
              memcpy((void*)psdp->org_img->pp_line_addr[(int)psdp->org_img->height - i - 1], (const void*)p, linesize);
            }
          }
        }
      }
      else {
        switch (psdp->last_err) {
        case  ER_BADIMAGE:    /* ָ�����ļ���һ�������ͼ���ļ� */
        case  ER_NONIMAGE:    /* ָ�����ļ�����һ��ͼ���ļ� */
          _f_last_err = ISIRW_LASTERR_IMGERR;
          break;

        case  ER_MEMORYERR:   /* �ڴ治�㣬���²���ʧ�� */
          _f_last_err = ISIRW_LASTERR_MEM;
          break;

        case  ER_FILERWERR:   /* �ļ��ڶ�д�����з�������ϵͳ��Ӳ����ԭ�������ļ���д�쳣�� */
          _f_last_err = ISIRW_LASTERR_RW;
          break;

        case  ER_SYSERR:      /* ����ϵͳ���ȶ����޷�ִ������ */
          _f_last_err = ISIRW_LASTERR_OSERR;
          break;

        default:
          assert(0);
          _f_last_err = ISIRW_LASTERR_OSERR;
          break;
        }
      }
    }
    __finally {
      if ((_f_last_err != ISIRW_LASTERR_SUCCESS) || (AbnormalTermination())) {
        if (pfdp) {
          isirw_free_FACI_DATAPACK(pfdp);
          pfdp = 0;
        }
      }

      /* רҵ���ں����˳�ǰ�����ã��ͷ�֮ */
      if (psdp) {
        isirw_free_SPEC_DATAPACK(psdp);
        psdp = 0;
      }

      if (p) {
        im_free(p);
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    _f_last_err = ISIRW_LASTERR_OSERR;
  }

  return pfdp;
}


/* ��ͼ�󱣴浽ָ�����ļ����� */
ISEEIRW_API int CALLAGREEMENT isirw_f_write_img(const char* new_filename, LPFACI_DATAPACK psou, LPSAVESTR savedec)
{
  LPSPEC_DATAPACK   psdp = 0;           /* רҵ���ݰ� */

  int         result = 0, i, linesize;

  assert(_isirw_mark != 0);             /* ����ϵͳ�����Ѿ����� */
  assert(new_filename && psou);

  /* ��ʼ������������ */
  _f_last_err = ISIRW_LASTERR_SUCCESS;

  __try {
    __try {
      /* ���벢��ʼ��һ��רҵ���ݰ����˰�Ϊ��ʱ�Եģ�*/
      if ((psdp = isirw_alloc_SPEC_DATAPACK(ISEEIO_CONFER_LOC_FILE, new_filename, 0, 0, 0)) == 0) {
        _f_last_err = ISIRW_LASTERR_MEM;
        result = 4;               /* �ڴ治�� */
        __leave;
      }

      /* ��д������� */
      psdp->command     = IRWE_SDPCOMM_SAVE;
      psdp->exec_state  = IRWE_SDPSTATE_IDLE;
      psdp->data_state  = SDP_DATA_STATE_PATH | SDP_DATA_STATE_ORG_INFO | SDP_DATA_STATE_ORG_PIX;
      psdp->last_err    = ER_SUCCESS;
      psdp->s_break     = 0;
      psdp->callback    = (IRWPFUN)0;

      /* ���Ʊ������ */
      memcpy((void*)psdp->savestr->para_value, (const void*)savedec->para_value, sizeof(int)*ISD_ITEM_NUM);

      /* ��дĿ��ͼ��·�� */
      isio_open_param(psdp->despath, ISEEIO_CONFER_LOC_FILE, new_filename, 0, 0);

      psdp->org_img->imgtype = IMT_RESSTATIC;   /* ͼ���ļ����� */
      psdp->org_img->imgformat = IMF_BMP;     /* ͼ���ļ���ʽ����׺���� */
      psdp->org_img->compression = ICS_RGB;   /* ͼ���ѹ����ʽ */

      psdp->org_img->width = psou->width;     /* ͼ���� */
      psdp->org_img->height = psou->height;   /* ͼ��߶� */
      psdp->org_img->order = _f_img_order;    /* ͼ�������˳�� ��0������1������*/
      psdp->org_img->bitcount = psou->bitcount; /* ÿ��������ռ��λ������Ӧ����ɫ���� */
      psdp->org_img->p_bit_data = psou->pbits;  /* ���ü��װ��е��������� */

      /* ȡ����ɫ����������ֵ */
      iscnv_get_mask(psou->std_id, &(psdp->org_img->r_mask), &(psdp->org_img->g_mask), &(psdp->org_img->b_mask), &(psdp->org_img->a_mask));

      linesize = DIBSCANLINE_DWORDALIGN(psdp->org_img->bitcount * psdp->org_img->width);

      /* �������׵�ַ���� */
      if ((psdp->org_img->pp_line_addr = (void**)isirw_malloc(sizeof(void*)*psou->height)) == 0) {
        _f_last_err = ISIRW_LASTERR_MEM;
        result = 4;               /* �ڴ治�� */
        __leave;
      }

      /* ��ʼ�����׵�ַ���� */
      if (psdp->org_img->order == 0) {      /* ���� */
        for (i = 0; i < (int)(psdp->org_img->height); i++) {
          psdp->org_img->pp_line_addr[i] = (void*)(psdp->org_img->p_bit_data + (i * linesize));
        }
      }
      else {                  /* ���� */
        for (i = 0; i < (int)(psdp->org_img->height); i++) {
          psdp->org_img->pp_line_addr[i] = (void*)(psdp->org_img->p_bit_data + ((psdp->org_img->height - i - 1) * linesize));
        }
      }

      /* ��д��ɫ��ߴ� */
      switch (psou->std_id) {
      case  0:
        psdp->org_img->pal_count = 2;
        break;

      case  1:
        psdp->org_img->pal_count = 16;
        break;

      case  2:
        psdp->org_img->pal_count = 256;
        break;

      default:
        psdp->org_img->pal_count = 0;
        break;
      }

      /* ���Ƶ�ɫ������ */
      memcpy((void*)(psdp->org_img->palette), (const void*)psou->palette, MAX_PALETTE_COUNT * sizeof(unsigned long));

      psdp->org_img->psubimg = (LPSUBIMGBLOCK)0;  /* ��ͼ����������ַ */
      psdp->org_img->imgnumbers = 1;        /* ���ļ���ͼ��ĸ��� */
      psdp->org_img->data_state = 2;        /* ���ݰ���ǰ������״̬ */

      psdp->org_img->play_number = 0UL;     /* ����˳���ߴ磨��Ԫ��Ϊ��λ�� */
      psdp->org_img->play_order = 0UL;      /* ����˳��� */
      psdp->org_img->time = 0;          /* ��֡ͣ��ʱ�� */
      psdp->org_img->colorkey = -1;       /* ��ͼ��͸��ɫ */

      psdp->org_img->pater   = psdp;
      psdp->org_img->irwpfun = _defalt_process; /* ʹ��ȱʡ�ص���������֮����Ϊ�գ� */

      psdp->org_img->omnipotence = -1;      /* ��Ϊת����ʱʹ�õı�׼��ʽID */

      /* ������Ϣ�� */
      if (add_pump_comm(psdp) == -1) {
        _f_last_err = ISIRW_LASTERR_MEM;
        result = 4;               /* �ڴ治�� */
        __leave;
      }

      /* �ȴ���������ִ����� */
      while (psdp->exec_state != IRWE_SDPSTATE_IDLE) {
        Sleep(0);
      }

      result = psdp->last_err;
    }
    __finally {
      if (psdp) {
        psdp->org_img->p_bit_data = 0;      /* ��ͼ���������Ǽ��װ��ģ����Բ�Ӧ���ͷ� */
        isirw_free_SPEC_DATAPACK(psdp);
        psdp = 0;
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    _f_last_err = ISIRW_LASTERR_OSERR;        /* ԭ��û�б��д��룬������ ���ﳬ �����˴�BUG�����޲� */
    result = 6;
  }

  /* 0 - �ɹ����� */
  /* 1 - �������û��ж� */
  /* 2 - ָ�����ļ���һ�������ͼ���ļ� */
  /* 3 - ָ�����ļ�����һ��ͼ���ļ� */
  /* 4 - �ڴ治�㣬���²���ʧ�� */
  /* 5 - �ļ��ڶ�д�����з�������ϵͳ��Ӳ����ԭ�������ļ���д�쳣�� */
  /* 6 - ����ϵͳ���ȶ����޷�ִ������ */
  /* 7 - ��֧�ֵ����� */
  /* 8 - ���湦�ܲ�֧�ֵ�ͼ���ʽ */
  return result;
}


/* ��ȡ���׽ӿ����һ�β����Ľ����Ϣ */
ISEEIRW_API int CALLAGREEMENT isirw_f_get_last_err()
{
  return _f_last_err;
}




/**********************************************************************************/
/* ת���ӿ� */
/**********************************************************************************/

/* ת��ԭʼ����ͼ��ָ����ʽ */
ISEEIRW_API int CALLAGREEMENT isirw_cnv(LPSPEC_DATAPACK psdp, IRWE_CNVMARK newspec, int ctrl)
{
  int       i, result = 0;
  LPSUBIMGBLOCK psib;

  assert(newspec != IRWE_CNVMARK_NOT);
  assert(_isirw_mark != 0);             /* ����ϵͳ�����Ѿ����� */
  assert(psdp);

  /* ���ݿ���������Ԥ���� */
  switch (ctrl) {
  case  -1:                     /* ɾ��ת���� */
    if (psdp->cnv_img) {
      assert(psdp->data_state & SDP_DATA_STATE_CNV);
      isirw_free_INFOSTR(psdp->cnv_img);      /* �ͷ�ת���� */
      psdp->cnv_img = 0;
    }

    return 0;

  default:
    assert(0);

  case  0:                      /* ��Ȼת��ԭ�� */

    /* �ж�רҵ�����Ƿ����ת����(cnv_img) */
    if (psdp->cnv_img) {
      assert(psdp->data_state & SDP_DATA_STATE_CNV);

      /* ��ת�����е�ͼ���Ƿ���ISee��׼��ʽ�� */
      if (newspec == IRWE_CNVMARK_AUTO) {
        return 1;
      }

      /* �ж�ת������ָ���ĸ�ʽ�Ƿ���ͬ */
      if (psdp->cnv_img->bitcount == (unsigned long)iscnv_idtobit((int)newspec)) {
        return 1;               /* �Ѵ���newspec��ʽ��ת���� */
      }
      else {
        isirw_free_INFOSTR(psdp->cnv_img);    /* ����ͬ���ͷ�ת���� */
        psdp->cnv_img = 0;
      }
    }
    else {
      /* �ж�Դͼ���Ƿ����ISee��׼ͼ���ʽ */
      i = iscnv_is_std(psdp->org_img->bitcount,
          psdp->org_img->r_mask,
          psdp->org_img->g_mask,
          psdp->org_img->b_mask,
          psdp->org_img->a_mask);

      /* ���ԭͼ�����ISee��׼��ʽ������ת����ǩΪ�Զ������ٽ��н�һ���Ĵ��� */
      if ((i != -1) && (newspec == IRWE_CNVMARK_AUTO)) {
        return 1;
      }
    }

    break;

  case  1:                      /* ǿ��ת��ԭ�� */
    if (psdp->cnv_img) {
      assert(psdp->data_state & SDP_DATA_STATE_CNV);
      isirw_free_INFOSTR(psdp->cnv_img);      /* �ͷ�ת���� */
      psdp->cnv_img = 0;
    }

    break;
  }


  /* �����µ�ת����־ */
  psdp->cnvmark = newspec;
  psdp->s_break = 0;

  __try {
    /* ���ò���ص���������ת�� */
    if ((result = _defalt_process(psdp, PFC_INTO_PLUG, 0, 0, 0, 0)) != 0) {
      __leave;
    }

    /* ת����ͼ�� */
    if ((result = _defalt_process(psdp, PFC_BUILD_MAST_IMAGE, 0, 0, 0, 0)) != 0) {
      __leave;
    }

    if ((result = _defalt_process(psdp, PFC_PROCESS, 0, 0, (int)psdp->org_img->height, 0)) != 0) {
      __leave;
    }

    if ((result = _defalt_process(psdp, PFC_OVER_SIR, 0, 0, 0, 0)) != 0) {
      __leave;
    }

    /* ת����ͼ�� */
    if (psdp->org_img->imgnumbers > 1) {
      psib = psdp->org_img->psubimg;

      for (i = 0; i < (int)(psdp->org_img->imgnumbers - 1); i++) {
        if ((result = _defalt_process(psdp, PFC_BUILD_SERT_IMAGE, i + 1, 0, 0, 0)) != 0) {
          __leave;
        }

        if ((result = _defalt_process(psdp, PFC_PROCESS, i + 1, 0, (int)psib->height, 0)) != 0) {
          __leave;
        }

        if ((result = _defalt_process(psdp, PFC_OVER_SIR, i + 1, 0, 0, 0)) != 0) {
          __leave;
        }

        psib = psib->next;
      }
    }

    /* ����ת�� */
    if ((result = _defalt_process(psdp, PFC_OK, 0, 0, 0, 0)) != 0) {
      __leave;
    }
  }
  __finally {
    switch (result) {
    case  0:
      break;

    case  2:
      result = -1;                /* ϵͳ�쳣 */
      break;

    case  3:
      result = -2;                /* �ڴ治�� */
      break;

    default:
      result = -1;
      break;
    }

    if ((result < 0) && (psdp->cnv_img)) {    /* ���ת��ʧ�ܣ����ͷ�ת���� */
      isirw_free_INFOSTR(psdp->cnv_img);
      psdp->cnv_img = 0;
      psdp->data_state &= ~SDP_DATA_STATE_CNV;
      psdp->cnvmark = IRWE_CNVMARK_AUTO;
    }
    else {
      psdp->data_state |= SDP_DATA_STATE_CNV;
    }
  }

  return result;
}





/**********************************************************************************/
/* �ڴ�ӿ� */
/**********************************************************************************/

/* �ڴ����ӿ� */
ISEEIRW_API void* CALLAGREEMENT isirw_malloc(size_t size)
{
  assert(size > 0);

  return im_malloc(size);
}


/* �ڴ��ط���ӿ� */
ISEEIRW_API void* CALLAGREEMENT isirw_realloc(void* p, size_t size)
{
  /* ������������ػ�������������ж� */
  if (p == _isirw_porg) {
    assert(_isirw_org_used);

    if (size < IRWE_MAX_BUFFSIZE) {       /* ����³ߴ�δ�����������ߴ磬���Ծ�ʹ�û������ڴ� */
      return (void*)_isirw_porg;
    }
    else {
      p = 0;
      InterlockedDecrement(&_isirw_org_used); /* ����������ڴ棨���£�*/
    }
  }
  else if (p == _isirw_pcnv) {
    assert(_isirw_cnv_used);

    if (size < IRWE_MAX_BUFFSIZE) {
      return (void*)_isirw_pcnv;
    }
    else {
      p = 0;
      InterlockedDecrement(&_isirw_cnv_used);
    }
  }

  return im_realloc(p, size);
}


/* �ڴ��ͷŽӿ� */
ISEEIRW_API void CALLAGREEMENT isirw_free(void* memblock)
{
  assert(memblock != 0);

  /* ������������ػ��������򲻽����ͷŲ��� */
  if (memblock == _isirw_porg) {
    assert(_isirw_org_used);
    InterlockedDecrement(&_isirw_org_used);
  }
  else if (memblock == _isirw_pcnv) {
    assert(_isirw_cnv_used);
    InterlockedDecrement(&_isirw_cnv_used);
  }
  else {
    im_free(memblock);                /* ͨ���ڴ���ֱ���ͷ� */
  }
}



/*
  ���ܣ�����һ���µ����ݰ�

  ��ڣ�pater �� ������רҵ���ݰ�

  ���أ�  0   �� ����ִ��ʧ�ܣ��ڴ治�㵼�µ�
    ��0   �� ����ִ�гɹ�������ֵ�������ݰ����׵�ַ
          ��ע���ɱ�������������ݰ�������isirw_free_INFOSTR()�ͷ�
*/
ISEEIRW_API LPINFOSTR CALLAGREEMENT isirw_alloc_INFOSTR(LPSPEC_DATAPACK pater)
{
  LPINFOSTR ptmp;

  /* ����ṹ�ռ� */
  if ((ptmp = (LPINFOSTR)isirw_malloc(sizeof(INFOSTR))) == (LPINFOSTR)0) {
    return (LPINFOSTR)0;
  }

#ifdef _DEBUG
  ptmp->sct_mark = INFOSTR_DBG_MARK;  /* �ṹ�ı�ʶ�����ڵ��� */

  if (sct_count == 0) {
    InitializeCriticalSection(&(sct_access));
  }

  EnterCriticalSection(&(sct_access));
  sct_count++;            /* �ṹʵ����� */
  LeaveCriticalSection(&(sct_access));
#endif

  ptmp->imgtype     = IMT_NULL;   /* ͼ���ļ����� */
  ptmp->imgformat   = IMF_NULL;   /* ͼ���ļ���ʽ����׺���� */
  ptmp->compression = ICS_RGB;    /* ͼ���ѹ����ʽ */

  ptmp->width    = 0;         /* ͼ���� */
  ptmp->height   = 0;         /* ͼ��߶� */
  ptmp->order    = 0;         /* ͼ�������˳�� ��0������1������*/
  ptmp->bitcount = 0;         /* ÿ��������ռ��λ������Ӧ����ɫ���� */

  ptmp->r_mask = 0;         /* ����ɫ����������ֵ */
  ptmp->g_mask = 0;
  ptmp->b_mask = 0;
  ptmp->a_mask = 0;

  ptmp->pp_line_addr = (void**)0;
  ptmp->p_bit_data   = (unsigned char*)0;
  ptmp->pal_count    = 0;
  memset((void*)(ptmp->palette), 0, MAX_PALETTE_COUNT * sizeof(unsigned long));

  ptmp->psubimg = (LPSUBIMGBLOCK)0; /* ��ͼ����������ַ */
  ptmp->imgnumbers = 0;       /* ���ļ���ͼ��ĸ��� */

  ptmp->data_state = 0;       /* ���ݰ���ǰ������״̬��
                      0 �� �յ����ݰ���û���κα�����������Ч��
                      1 �� ����ͼ���������Ϣ
                      2 �� ����ͼ������ */
  InitializeCriticalSection(&(ptmp->access));

  ptmp->play_number = 0UL;      /* ����˳���ߴ磨��Ԫ��Ϊ��λ�� */
  ptmp->play_order = 0UL;       /* ����˳��� */
  ptmp->time = 0;           /* ��֡ͣ��ʱ�� */
  ptmp->colorkey = -1;        /* ��ͼ��͸��ɫ */

  ptmp->pater   = pater;
  ptmp->irwpfun = _defalt_process;  /* ʹ��ȱʡ�ص���������֮����Ϊ�գ� */

  ptmp->omnipotence = -1;       /* ��Ϊת����ʱʹ�õı�׼��ʽID */

  return ptmp;
}



/*
  ���ܣ��ͷ�һ�����ݰ�

  ��ڣ�
    pinfo �� ���ݰ��׵�ַ

  ���أ�  0   �� �㶨����ֵ�����������ݰ�ָ������������
*/
ISEEIRW_API LPINFOSTR CALLAGREEMENT isirw_free_INFOSTR(LPINFOSTR pinfo)
{
  assert(pinfo);
  assert(pinfo->sct_mark == INFOSTR_DBG_MARK);

  assert(sct_count);

  EnterCriticalSection(&(pinfo->access));

#ifdef _DEBUG
  EnterCriticalSection(&(sct_access));
  sct_count--;            /* �ṹʵ����� */
  LeaveCriticalSection(&(sct_access));

  if (sct_count == 0) {
    DeleteCriticalSection(&(sct_access));
  }

#endif

  if (pinfo->pp_line_addr) {
    isirw_free(pinfo->pp_line_addr);
    pinfo->pp_line_addr = (void**)0;
  }

  if (pinfo->p_bit_data) {
    isirw_free(pinfo->p_bit_data);
    pinfo->p_bit_data = (unsigned char*)0;
  }

  if (pinfo->psubimg) {
    assert(pinfo->imgnumbers > 1);
    _free_subimg(pinfo);
  }

  if (pinfo->play_order) {
    isirw_free(pinfo->play_order);
    pinfo->play_order = (unsigned long*)0;
  }

  LeaveCriticalSection(&(pinfo->access));
  DeleteCriticalSection(&(pinfo->access));

  isirw_free(pinfo);

  return (LPINFOSTR)0;
}



/* �ڲ������� �ͷ����ݰ��е���ͼ����������еĻ��� */
void CALLAGREEMENT _free_subimg(LPINFOSTR pinfo)
{
  int i;
  LPSUBIMGBLOCK ptmp, pfree = pinfo->psubimg;

  /* �ͷ�������ͼ��ڵ� */
  for (i = 0; i < (int)(pinfo->imgnumbers - 1); i++) {
    ptmp = pfree->next;

    assert(pfree);

    isirw_free_SUBIMGBLOCK(pfree);

    pfree = ptmp;
  }

  pinfo->imgnumbers -= i;
  assert(pinfo->imgnumbers == 1);   /* ��Ϊֻʣ����֡ͼ���� */
  pinfo->psubimg = (LPSUBIMGBLOCK)0;
}



/*
  ���ܣ�����һ����ͼ��ڵ㣬����ʼ���ڵ�������

  ��ڣ�

  ���أ�  0   �� ����ִ��ʧ�ܣ��ڴ治�㵼�µ�
    ��0   �� ����ִ�гɹ�������ֵ����ͼ��ڵ���׵�ַ
          ��ע���ɱ������������ͼ��ڵ������isirw_free_SUBIMGBLOCK()�ͷ�
*/
ISEEIRW_API LPSUBIMGBLOCK CALLAGREEMENT isirw_alloc_SUBIMGBLOCK()
{
  LPSUBIMGBLOCK p_subimg;

  /* ������ͼ����Ϣ�ڵ�(SUBIMGBLOCK) */
  if ((p_subimg = (LPSUBIMGBLOCK)isirw_malloc(sizeof(SUBIMGBLOCK))) == 0) {
    return 0;  /* �ڴ治�� */
  }

  /* ��ʼ��Ϊ 0 */
  memset((void*)p_subimg, 0, sizeof(SUBIMGBLOCK));

  p_subimg->number   = 1;     /* ��ͼ����ʼ���Ϊ 1 */
  p_subimg->colorkey = -1;    /* ͸��ɫֵ��Ϊ�� */

  return p_subimg;
}



/*
  ���ܣ��ͷ�һ����ͼ��ڵ㣬�������е�λ�����������׵�ַ����

  ��ڣ�p_node  �� ���ͷŵ���ͼ��ڵ�

  ���أ�
*/
ISEEIRW_API void CALLAGREEMENT isirw_free_SUBIMGBLOCK(LPSUBIMGBLOCK p_node)
{
  assert(p_node);
  assert(p_node->number > 0);

  if (p_node->pp_line_addr) {
    isirw_free(p_node->pp_line_addr);
  }

  if (p_node->p_bit_data) {
    isirw_free(p_node->p_bit_data);
  }

  isirw_free(p_node);
}



/*
  ���ܣ�����һ���յ�רҵ���ݰ�������ʼ�����еĸ���Ա����

  ��ڣ�

  ���أ�  0   �� ����ִ��ʧ�ܣ��ڴ治�㵼�µ�
    ��0   �� ����ִ�гɹ�
*/
ISEEIRW_API LPSPEC_DATAPACK CALLAGREEMENT isirw_alloc_SPEC_DATAPACK(
    ISEEIO_CONFER_TYPE type,
    const char* name,
    unsigned long param1,
    unsigned long param2,
    IRWE_CNVMARK cm)
{
  LPSPEC_DATAPACK p_sdp;

  /* ����רҵ���ݰ����� */
  if ((p_sdp = (LPSPEC_DATAPACK)isirw_malloc(sizeof(SPEC_DATAPACK))) == 0) {
    return 0;         /* �ڴ治�� */
  }

  /* ��ʼ��Ϊ 0 */
  memset((void*)p_sdp, 0, sizeof(SPEC_DATAPACK));

  p_sdp->dbg_mark    = DATAPACK_MARK_SPECIALTY;
  p_sdp->exec_state  = IRWE_SDPSTATE_IDLE;
  p_sdp->data_state  = 0;
  p_sdp->last_err    = ER_SUCCESS;
  p_sdp->s_total     = 0;
  p_sdp->s_current   = 0;
  p_sdp->s_break     = 0;
  p_sdp->s_state     = -1;
  p_sdp->cnvmark     = cm;
  p_sdp->command     = IRWE_SDPCOMM_NULL;

  /* �������ݰ� */
  if ((p_sdp->org_img = (LPINFOSTR)isirw_alloc_INFOSTR(p_sdp)) == 0) {
    isirw_free(p_sdp);
    return 0;
  }

  /* ����Դͼ��·���� */
  if ((p_sdp->path = (PISADDR)isirw_malloc(sizeof(ISADDR))) == 0) {
    isirw_free_INFOSTR(p_sdp->org_img);
    isirw_free(p_sdp);
    return 0;
  }

  /* ���ó�ʼ·�� */
  isio_open_param(p_sdp->path, type, name, param1, param2);

  /* ����Ŀ��ͼ��·���� */
  if ((p_sdp->despath = (PISADDR)isirw_malloc(sizeof(ISADDR))) == 0) {
    isirw_free(p_sdp->path);
    isirw_free_INFOSTR(p_sdp->org_img);
    isirw_free(p_sdp);
    return 0;
  }

  /* ���뱣����������ṹ */
  if ((p_sdp->savestr = (LPSAVESTR)isirw_malloc(sizeof(SAVESTR))) == 0) {
    isirw_free(p_sdp->despath);
    isirw_free(p_sdp->path);
    isirw_free_INFOSTR(p_sdp->org_img);
    isirw_free(p_sdp);
    return 0;
  }

  return p_sdp;
}



/*
  ���ܣ��ͷ�һ��רҵ���ݰ����������а�����ԭʼ���ݰ���ת�����ݰ���·������

  ��ڣ�p_sdp �� ���ͷŵ�רҵ���ݰ�

  ���أ�
*/
ISEEIRW_API void CALLAGREEMENT isirw_free_SPEC_DATAPACK(LPSPEC_DATAPACK p_sdp)
{
  assert(p_sdp);
  assert(p_sdp->exec_state == IRWE_SDPSTATE_IDLE);

  if (p_sdp->org_img) {
    isirw_free_INFOSTR(p_sdp->org_img);
  }

  if (p_sdp->cnv_img) {
    isirw_free_INFOSTR(p_sdp->cnv_img);
  }

  if (p_sdp->savestr) {
    isirw_free(p_sdp->savestr);
  }

  if (p_sdp->despath) {
    isirw_free(p_sdp->despath);
  }

  if (p_sdp->path) {
    isirw_free(p_sdp->path);
  }

  isirw_free(p_sdp);
}


/*
  ���ܣ�����һ�����������ݰ�

  ��ڣ���
  ���أ�  0   �� ����ִ��ʧ�ܣ��ڴ治�㵼�µ�
    ��0   �� ����ִ�гɹ�������һ���յļ��������ݰ���
*/
ISEEIRW_API LPFACI_DATAPACK CALLAGREEMENT isirw_alloc_FACI_DATAPACK()
{
  LPFACI_DATAPACK   pfdp;

  /* ����רҵ���ݰ����� */
  if ((pfdp = (LPFACI_DATAPACK)isirw_malloc(sizeof(FACI_DATAPACK))) == 0) {
    return 0;         /* �ڴ治�� */
  }

  /* ��ʼ��Ϊ 0 */
  memset((void*)pfdp, 0, sizeof(FACI_DATAPACK));

  pfdp->dbg_mark = DATAPACK_MARK_FACILITY;
  pfdp->std_id   = -1;
  pfdp->pbits    = 0;

  return pfdp;
}


/*
  ���ܣ��ͷ�һ�����������ݰ����������а�����ԭʼͼ���������ݣ�

  ��ڣ�pfdp  �� ���ͷŵļ��������ݰ�

  ���أ�
*/
ISEEIRW_API void CALLAGREEMENT isirw_free_FACI_DATAPACK(LPFACI_DATAPACK pfdp)
{
  assert(pfdp);
  assert(pfdp->dbg_mark == DATAPACK_MARK_FACILITY);

  /* �����ͼ���������ݣ����ͷ��� */
  if (pfdp->pbits) {
    isirw_free(pfdp->pbits);
  }

  isirw_free(pfdp);
}





/**********************************************************************************/
/*�����ýӿں��� */
/**********************************************************************************/

#ifdef WIN32

/*
  ���ܣ�����ָ��DLL�Ƿ���ISeeͼ���д���

  ��ڣ�
    hmode �� DLL���

  ���أ�  0   �� ָ����DLL����ISee��ͼ���д���
    ��0   �� ��

*/
ISEEIRW_API unsigned long CALLAGREEMENT isirw_test_plugin(HMODULE hmod)
{
  return (unsigned long)GetProcAddress(hmod, IRWP_TAG);
}



/*
  ���ܣ����ISeeͼ���д������ڲ���Ϣ

  ��ڣ�
    pfun_add�� �������isirw_test_plugin()�������ص�ֵ

  ���أ�  0   �� ����ִ��ʧ�ܣ������ò���������޷�����
    ��0   �� ����ִ�гɹ�������ֵ��ָ��IRWP_INFO�ṹ�ĵ�ַ��
           �ýṹ��ʵ���ڲ���ڲ���
*/
ISEEIRW_API LPIRWP_INFO CALLAGREEMENT isirw_get_info(unsigned long pfun_add)
{
  return (pfun_add == 0UL) ? (LPIRWP_INFO)0 : (*((IRWP_ISX)pfun_add))();
}



/*
  ���ܣ����ISeeͼ���д����ӿڵ�ַ

  ��ڣ�
    hmod  �� ���ģ����
    pinfo �� ��isirw_get_info()�������صĲ����Ϣ�ṹ��ַ
    name  �� Ҫ��ȡ�Ľӿ����ƣ���IRWP_API_NAME�Ķ��壩

  ���أ�  0   �� ����ִ��ʧ�ܣ������ò���������޷�����
    ��0   �� ����ִ�гɹ�������ֵ��ָ���ӿڵĺ�����ַ
*/
ISEEIRW_API void* CALLAGREEMENT isirw_get_api_addr(HMODULE hmod, LPIRWP_INFO pinfo, enum IRWP_API_NAME name)
{
  char buf[64];

  assert(hmod);
  assert(pinfo);

  strcpy((char*)buf, (const char*)(pinfo->irwp_func_prefix));
  strcat((char*)buf, (const char*)IRWP_API_NAME_STRING[name]);

  return (void*)GetProcAddress(hmod, (char*)buf);
}

#endif /* WIN32 */





/*
  ���ܣ�Ϊָ�����ݰ������µĻص�����

  ��ڣ�pinfo �����趨���ݰ�
    new_pfun���µĻص�����

  ���أ��ϵĻص�����
*/
ISEEIRW_API IRWPFUN CALLAGREEMENT isirw_set_pfun(LPINFOSTR pinfo, IRWPFUN new_pfun)
{
  IRWPFUN old;

  assert(pinfo);

  if (new_pfun == 0) {
    old = _defalt_process;
    new_pfun = _defalt_process;   /* �ص���������Ϊ�� */
  }
  else {
    old = pinfo->irwpfun;
  }

  pinfo->irwpfun = new_pfun;

  return old;
}


/*
  ���ܣ�����ԭʼ�����ڴ�顣���ԭʼ���ػ��������ã��򷵻�ԭʼ���ػ������׵�ַ������
    ����һ���µ��ڴ�顣

  ��ڣ�size  ��������ڴ��ߴ磨�ֽڵ�λ��

  ���أ��������׵�ַ�������ڴ���׵�ַ
*/
ISEEIRW_API void* CALLAGREEMENT isirw_orgpix_malloc(size_t size)
{
  /* ���������������ڴ���󣬻�ԭʼ���ػ�������û�б����䣬���߻��������ڱ�ʹ�ã��������µ��ڴ�� */
  if ((size > IRWE_MAX_BUFFSIZE) || (!(_isirw_buff_mark & IRWE_BUFTYPE_ORG)) || (_isirw_org_used)) {
    return im_malloc(size);
  }

  /* �ۼ�ʹ�ü��� */
  InterlockedIncrement(&_isirw_org_used);

  assert(_isirw_porg);

  /* ����ԭʼ���ػ������׵�ַ */
  return (void*)_isirw_porg;
}


/*
  ���ܣ�����ת�������ڴ�顣���ת�����ػ��������ã��򷵻�ת�����ػ������׵�ַ������
    ����һ���µ��ڴ�顣

  ��ڣ�size  ��������ڴ��ߴ磨�ֽڵ�λ��

  ���أ��������׵�ַ�������ڴ���׵�ַ
*/
ISEEIRW_API void* CALLAGREEMENT isirw_cnvpix_malloc(size_t size)
{
  /* ���������������ڴ���󣬻�ת�����ػ�������û�б����䣬���߻��������ڱ�ʹ�ã��������µ��ڴ�� */
  if ((size > IRWE_MAX_BUFFSIZE) || (!(_isirw_buff_mark & IRWE_BUFTYPE_CNV)) || (_isirw_cnv_used)) {
    return im_malloc(size);
  }

  /* �ۼ�ʹ�ü��� */
  InterlockedIncrement(&_isirw_cnv_used);

  assert(_isirw_pcnv);

  /* ����ԭʼ���ػ������׵�ַ */
  return (void*)_isirw_pcnv;
}


/* �����������д������� */
ISEEIRW_API int CALLAGREEMENT isirw_add_idle(void* p/*���д�������ַ*/)
{
  return add_idlesse(p);                /* ������������-1���ɹ�����0 */
}


/* ��ȡ������Ŀ���������� */
ISEEIRW_API int CALLAGREEMENT isirw_get_idlesse_count()
{
  return get_idlesse_count();
}


/*
  ���ܣ�ȱʡ�Ľ��Ȼص�����

  ��ڣ�sdp     ��רҵ���ݰ���ַ
    type    ���������ͣ���PFUN_COMM�Ķ���ע�ͣ�
    img_num   ��ͼ����ţ�0Ϊ��ͼ������Ϊ��ͼ����ţ�
    star_line ����ʼ�кţ��Ի���������ַΪ��׼��
    line_num  ����������
    funid   �������߱�ʶ��0����������1��д������

  ���أ�0��������дͼ��
    1���û��ж�
    2��ϵͳ�쳣
    3���ڴ治��
*/
static int CALLAGREEMENT _defalt_process(
    LPSPEC_DATAPACK sdp,
    PFUN_COMM type,
    int img_num,
    int star_line,
    int line_num,
    int funid)
{
  int       result = 0;
  int       i, j, sls, dls, fmt;
  LPINFOSTR   pinfo_str;
  LPSUBIMGBLOCK psub_str;
  LPSUBIMGBLOCK psub_tmp;
  LPSUBIMGBLOCK psub_a;
  LPSUBIMGBLOCK psub_b;

  assert(sdp);

  pinfo_str = sdp->org_img;
  psub_str  = sdp->org_img->psubimg;

  switch (type) {
  case  PFC_NULL:           /* ������ */
    break;

  case  PFC_INTO_PLUG:          /* ���ƽ����� */
    if (funid == IRWE_CALLBACK_FUNID_LOAD) {
      sdp->s_total   = 1;
      sdp->s_current = 0;

      if (sdp->s_break != 0) {
        result = 1;
        break;
      }
    }
    else if (funid == IRWE_CALLBACK_FUNID_SAVE) {
      sdp->s_total   = 1;
      sdp->s_current = 0;

      if (sdp->s_break != 0) {
        result = 1;
        break;
      }
    }
    else {
      assert(0);            /* ��ǰֻ���������ֵ����ߺ������� */
    }

    if (sdp->callback) {
      (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
    }

    break;

  case  PFC_BUILD_MAST_IMAGE:     /* �µ���ͼ�� */
    if (funid == IRWE_CALLBACK_FUNID_LOAD) {
      switch (sdp->cnvmark) {
      case  IRWE_CNVMARK_NOT:
        fmt = -1;
        break;

      case  IRWE_CNVMARK_AUTO:
        /* �ж�Դͼ���Ƿ���ISee��׼��ʽ֮һ */
        i = iscnv_is_std(sdp->org_img->bitcount, sdp->org_img->r_mask, sdp->org_img->g_mask, sdp->org_img->b_mask, sdp->org_img->a_mask);

        if (i == -1) {        /* �Ǳ�׼��ʽ */
          /* �����ƥ��ı�׼��ʽID�� */
          fmt = iscnv_match(sdp->org_img->bitcount);
        }
        else {            /* ��׼��ʽ */
          fmt = -1;
        }

        break;

      case  IRWE_CNVMARK_1:
      case  IRWE_CNVMARK_4:
      case  IRWE_CNVMARK_8:
      case  IRWE_CNVMARK_16_555:
      case  IRWE_CNVMARK_16_565:
      case  IRWE_CNVMARK_24:
      case  IRWE_CNVMARK_32:
        fmt = sdp->cnvmark;
        break;

      default:
        assert(0);            /* cnvmark��ֵ�����ܳ������Ϸ�Χ */
        return 2;
        break;
      }

      /* �����û��ж����� */
      if (sdp->s_break != 0) {
        result = 1;
        break;
      }

      /* ���Դͼ���Ǳ�׼��ʽ��������ת���� */
      if (fmt >= 0) {
        if ((pinfo_str = _isirw_build_mi(sdp, fmt)) == 0) {
          result = 3;
          break;      /* �ڴ治�� */
        }

        assert(sdp->cnv_img == 0);

        sdp->cnv_img = pinfo_str;
      }

      /* ���ý��ȼ���״̬���� */
      sdp->s_total   = sdp->org_img->height;
      sdp->s_current = 0;
      sdp->s_state   = 0;
    }
    else if (funid == IRWE_CALLBACK_FUNID_SAVE) {
      sdp->s_total   = sdp->org_img->height;
      sdp->s_current = 0;
      sdp->s_state   = 0;
    }
    else {
      assert(0);
    }

    if (sdp->callback) {
      (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
    }

    break;

  case  PFC_BUILD_SERT_IMAGE:     /* �µ���ͼ�� */
    if (funid == IRWE_CALLBACK_FUNID_LOAD) {
      /* �����û��ж����� */
      if (sdp->s_break != 0) {
        result = 1;
        break;
      }

      if (sdp->cnv_img) {
        assert(sdp->cnvmark != IRWE_CNVMARK_NOT);

        /* ��ȡ��ͼ���׼��ʽID */
        fmt = iscnv_is_std(sdp->cnv_img->bitcount, sdp->cnv_img->r_mask, sdp->cnv_img->g_mask, sdp->cnv_img->b_mask, sdp->cnv_img->a_mask);

        assert(fmt != -1);

        /* ��ȡ����ԭʼ��ͼ������ݰ���ַ */
        _isirw_get_sert_image_info(sdp->org_img, img_num, (LPSUBIMGBLOCK*)&psub_str);

        /* �����µ���ͼ��ת���� */
        if ((psub_tmp = _isirw_build_si(psub_str, fmt)) == 0) {
          result = 3;
          break;
        }

        psub_tmp->parents = sdp->cnv_img;

        /* ��ȡת�������һ����ͼ������кż����ݰ���ַ */
        j = _isirw_get_last_sert_image_info(sdp->cnv_img, (LPSUBIMGBLOCK*)&psub_str);

        /* ������ת������ͼ������β�� */
        if (j != 0) {
          psub_tmp->prev = psub_str;
          psub_str->next = psub_tmp;
        }
        else {
          psub_tmp->prev = 0;
          assert(sdp->cnv_img->psubimg == 0);
          sdp->cnv_img->psubimg = psub_tmp;
        }

        psub_tmp->next    = 0;

        psub_tmp->parents->imgnumbers++;

        /* �ۼӽ��� */
        sdp->s_total     += psub_tmp->height;
        sdp->s_state      = psub_tmp->number;
      }
      else {
        if (sdp->cnvmark == IRWE_CNVMARK_NOT) {
          if (sdp->callback) {
            (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
          }

          break;
        }

        assert(sdp->cnvmark == IRWE_CNVMARK_AUTO);
        assert(sdp->org_img);

        /* ��ñ�����ͼ�����ݰ���ַ */
        _isirw_get_sert_image_info(sdp->org_img, img_num, (LPSUBIMGBLOCK*)&psub_str);

        assert(psub_str);

        /* �������ͼ��ĸ�ʽ��ͬ��������ת����������һ����ͼ�� */
        if ((sdp->org_img->bitcount == (unsigned long)psub_str->bitcount) && \
            (sdp->org_img->r_mask == psub_str->r_mask) && \
            (sdp->org_img->g_mask == psub_str->g_mask) && \
            (sdp->org_img->b_mask == psub_str->b_mask) && \
            (sdp->org_img->a_mask == psub_str->a_mask)) {
          if (sdp->callback) {
            (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
          }

          break;
        }

        /* �����ͼ���Ǳ�׼��ʽ֮һ���������һ����ͼ�� */
        if (iscnv_is_std(psub_str->bitcount, psub_str->r_mask, psub_str->g_mask, psub_str->b_mask, psub_str->a_mask) != -1) {
          if (sdp->callback) {
            (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
          }

          break;
        }

        /* ȡ��ԭʼ��ͼ��λ��ֵ */
        j = (int)sdp->org_img->bitcount;

        /* ָ���һ����ͼ�����ݰ� */
        psub_str = sdp->org_img->psubimg;

        /* ѡ�����λ��ֵ */
        while (psub_str) {
          if (psub_str->bitcount > j) {
            j = psub_str->bitcount;
          }

          psub_str = psub_str->next;
        }

        assert((j > 0) && (j <= 32));

        /* �����ƥ��ı�׼��ʽID�� */
        fmt = iscnv_match(j);

        /* �Ƚ�����������ͼ��ת�� */
        if ((pinfo_str = _isirw_build_mi(sdp, fmt)) == 0) {
          result = 3;
          break;      /* �ڴ治�� */
        }

        sdp->cnv_img = pinfo_str;

        /* ����Դ��Ŀ��ͼ��ɨ���п�� */
        sls = DIBSCANLINE_DWORDALIGN(sdp->org_img->bitcount * sdp->org_img->width);
        dls = DIBSCANLINE_DWORDALIGN(sdp->cnv_img->bitcount * sdp->cnv_img->width);

        /* һ����ת�����е���ͼ���� */
        result = iscnv_cnvfun(sdp->org_img->bitcount, sdp->cnv_img->omnipotence,
            sdp->org_img->p_bit_data, (LPPALITEM)sdp->org_img->palette, sls,
            sdp->cnv_img->p_bit_data, (LPPALITEM)sdp->cnv_img->palette, dls,
            sdp->org_img->height, sdp->org_img->width, sdp->org_img->b_mask, sdp->org_img->g_mask,
            sdp->org_img->r_mask, sdp->org_img->a_mask);

        if (result == -1) {
          result = 2;
          break;
        }

        psub_str = sdp->org_img->psubimg;

        assert(psub_str);

        /* ת����ͼ�� */
        while (psub_str) {
          /* �����µ���ͼ��ת���� */
          if ((psub_tmp = _isirw_build_si(psub_str, fmt)) == 0) {
            result = 3;
            break;
          }

          psub_tmp->parents = sdp->cnv_img;

          /* ��ȡת�������һ����ͼ������кż����ݰ���ַ */
          j = _isirw_get_last_sert_image_info(sdp->cnv_img, (LPSUBIMGBLOCK*)&psub_a);

          /* ����ת������ͼ������β�� */
          if (j != 0) {
            psub_tmp->prev = psub_a;
            psub_a->next   = psub_tmp;
          }
          else {
            psub_tmp->prev = 0;
            assert(sdp->cnv_img->psubimg == 0);
            sdp->cnv_img->psubimg = psub_tmp;
          }

          psub_tmp->next    = 0;

          /* ת������ͼ�������һ */
          psub_tmp->parents->imgnumbers++;

          /* �趨���ȼ�ͼ��� */
          sdp->s_total     += psub_tmp->height;
          sdp->s_state      = psub_tmp->number;

          /* ���ε���ָ������ͼ����ǰ����ͼ�󶼽�����һ����ת�� */
          if (psub_str->number < img_num) {
            /* ����Դͼ���Ŀ��ͼ��ɨ���гߴ� */
            sls = DIBSCANLINE_DWORDALIGN(psub_str->bitcount * psub_str->width);
            dls = DIBSCANLINE_DWORDALIGN(psub_tmp->bitcount * psub_tmp->width);

            /* һ����ת����ͼ�� */
            result = iscnv_cnvfun(psub_str->bitcount, sdp->cnv_img->omnipotence,
                psub_str->p_bit_data, (LPPALITEM)psub_str->palette, sls,
                psub_tmp->p_bit_data, (LPPALITEM)psub_tmp->palette, dls,
                psub_str->height, psub_str->width, psub_str->b_mask, psub_str->g_mask,
                psub_str->r_mask, psub_str->a_mask);

            if (result == -1) {
              result = 2;
              break;
            }

            /* �ۼӽ��� */
            sdp->s_current += psub_tmp->height;
          }
          else {
            break;
          }

          /* ������һ����ͼ�� */
          psub_str = psub_str->next;
        }
      }
    }
    else if (funid == IRWE_CALLBACK_FUNID_SAVE) {
      /* ��ȡԭʼ��������ͼ������ݰ���ַ */
      _isirw_get_sert_image_info(sdp->org_img, img_num, (LPSUBIMGBLOCK*)&psub_a);

      /* �ۼӽ��� */
      sdp->s_total   += psub_a->height;
      sdp->s_state   = 0;
    }
    else {
      assert(0);
    }

    if (sdp->callback) {
      (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
    }

    break;

  case  PFC_PROCESS:          /* ���� */
    if (funid == IRWE_CALLBACK_FUNID_LOAD) {
      if (sdp->cnv_img) {
        if (sdp->s_state == 0) {  /* ��ͼ�� */
          sls = DIBSCANLINE_DWORDALIGN(sdp->org_img->bitcount * sdp->org_img->width);
          dls = DIBSCANLINE_DWORDALIGN(sdp->cnv_img->bitcount * sdp->cnv_img->width);

          result = iscnv_cnvfun(sdp->org_img->bitcount, sdp->cnv_img->omnipotence,
              sdp->org_img->p_bit_data + star_line * sls, (LPPALITEM)sdp->org_img->palette, sls,
              sdp->cnv_img->p_bit_data + star_line * dls, (LPPALITEM)sdp->cnv_img->palette, dls,
              line_num, sdp->org_img->width, sdp->org_img->b_mask, sdp->org_img->g_mask,
              sdp->org_img->r_mask, sdp->org_img->a_mask);

          result = (result == -1) ? 2 : 0;
        }
        else {          /* ��ͼ�� */
          /* ��ȡת�������һ����ͼ������ݰ���ַ��ԭʼ��������ͼ�����ݰ���ַ */
          _isirw_get_sert_image_info(sdp->org_img, img_num, (LPSUBIMGBLOCK*)&psub_a);
          _isirw_get_last_sert_image_info(sdp->cnv_img, (LPSUBIMGBLOCK*)&psub_b);

          assert(psub_a && psub_b);
          assert(psub_a->number == psub_b->number);

          sls = DIBSCANLINE_DWORDALIGN(psub_a->bitcount * psub_a->width);
          dls = DIBSCANLINE_DWORDALIGN(psub_b->bitcount * psub_b->width);

          result = iscnv_cnvfun(psub_a->bitcount, sdp->cnv_img->omnipotence,
              psub_a->p_bit_data + star_line * sls, (LPPALITEM)psub_a->palette, sls,
              psub_b->p_bit_data + star_line * dls, (LPPALITEM)psub_b->palette, dls,
              line_num, psub_a->width, psub_a->b_mask, psub_a->g_mask,
              psub_a->r_mask, psub_a->a_mask);

          result = (result == -1) ? 2 : 0;
        }
      }

      if (((star_line % 10) == 1) && (sdp->callback)) {
        (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
      }

      /* �ۼӽ��� */
      sdp->s_current += line_num;

      /* �����û��ж����� */
      if (sdp->s_break != 0) {
        result = 1;
        break;
      }
    }
    else if (funid == IRWE_CALLBACK_FUNID_SAVE) {
      if (((star_line % 10) == 1) && (sdp->callback)) {
        (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
      }

      /* �ۼӽ��� */
      sdp->s_current += line_num;

      /* �����û��ж����� */
      if (sdp->s_break != 0) {
        result = 1;
        break;
      }
    }
    else {
      assert(0);            /* ��ǰֻ���������ֵ����ߺ������� */
    }

    break;

  case  PFC_OVER_SIR:         /* ����һ��ͼ�� */
    if (funid == IRWE_CALLBACK_FUNID_LOAD) {
      /* �����û��ж����� */
      if (sdp->s_break != 0) {
        result = 1;
        break;
      }

      if (sdp->cnv_img) {
        if (sdp->s_state == 0) {      /* ��ͼ�� */
          sdp->cnv_img->time     = sdp->org_img->time;
          sdp->cnv_img->colorkey = sdp->org_img->colorkey;
          sdp->cnv_img->data_state = 2;
        }
        else {              /* ��ͼ�� */
          ;
        }
      }
    }
    else if (funid == IRWE_CALLBACK_FUNID_SAVE) {
      ;
    }
    else {
      assert(0);
    }

    if (sdp->callback) {
      (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
    }

    break;

  case  PFC_OK:             /* ���Ƽ����뿪�����ִ�гɹ���״̬�� */
    if (funid == IRWE_CALLBACK_FUNID_LOAD) {
      if (sdp->cnv_img) {
        sdp->cnv_img->play_number = sdp->org_img->play_number;

        /* ������ͼ�󲥷ű� */
        if (sdp->cnv_img->play_number) {
          sdp->cnv_img->play_order  = (unsigned long*)isirw_malloc(sdp->cnv_img->play_number * sizeof(long));

          if (sdp->cnv_img->play_order == 0) {
            result = 3;
            break;
          }

          memcpy((void*)sdp->cnv_img->play_order, (const void*)sdp->org_img->play_order, (size_t)sdp->cnv_img->play_number * sizeof(long));
        }

        sdp->cnv_img->imgnumbers  = sdp->org_img->imgnumbers;
      }

      /* ��ǰ���ȵ����ܽ��� */
      sdp->s_current = sdp->s_total;
    }
    else if (funid == IRWE_CALLBACK_FUNID_SAVE) {
      sdp->s_current = sdp->s_total;
    }
    else {
      assert(0);
    }

    if (sdp->callback) {
      (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
    }

    break;

  case  PFC_BREAK:            /* ���Ƽ����뿪�����ִ��ʧ�ܵ�״̬�� */
    if (funid == IRWE_CALLBACK_FUNID_LOAD) {
      if (sdp->cnv_img) {
        isirw_free_INFOSTR(sdp->cnv_img);
        sdp->cnv_img = 0;
      }
    }
    else if (funid == IRWE_CALLBACK_FUNID_SAVE) {
      ;
    }
    else {
      assert(0);
    }

    if (sdp->callback) {
      (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
    }

    break;

  default:
    assert(0);
    result = 2;
    break;   /* ϵͳ�����жϲ���Ķ�д���� */
  }

  return result;
  /* 0��������д��1���û��жϣ�2��ϵͳ�쳣��3���ڴ治�� */
}


/* ����һ���µ�ת������ͼ�����ݰ� */
LPINFOSTR CALLAGREEMENT _isirw_build_mi(LPSPEC_DATAPACK sdp, int fmt/* Ŀ���ʽID */)
{
  LPINFOSTR pinfo_str;
  int     linesize, i, j;

  if ((pinfo_str = isirw_alloc_INFOSTR(sdp)) == 0) {
    return 0;                   /* �ڴ治�� */
  }

  pinfo_str->imgtype    = IMT_CNV;
  pinfo_str->imgformat  = sdp->org_img->imgformat;
  pinfo_str->compression  = sdp->org_img->compression;

  pinfo_str->width    = sdp->org_img->width;
  pinfo_str->height   = sdp->org_img->height;
  pinfo_str->bitcount   = iscnv_idtobit(fmt);
  pinfo_str->order    = sdp->org_img->order;

  /* �����׼��ʽ���������� */
  iscnv_get_mask(fmt, &(pinfo_str->r_mask), &(pinfo_str->g_mask), &(pinfo_str->b_mask), &(pinfo_str->a_mask));

  pinfo_str->psubimg    = 0;
  pinfo_str->imgnumbers = 1;

  pinfo_str->omnipotence  = fmt;

  /* ���Ƶ�ɫ������ */
  if (pinfo_str->bitcount <= 8) {
    switch (pinfo_str->bitcount) {
    case  1:
      if (sdp->org_img->bitcount <= 8) {
        ((unsigned long*)(pinfo_str->palette))[0] = ((unsigned long*)sdp->org_img->palette)[0];
        ((unsigned long*)(pinfo_str->palette))[1] = ((unsigned long*)sdp->org_img->palette)[(unsigned int)pow((int)2, (int)sdp->org_img->bitcount) / 2];
      }
      else {
        memcpy((void*)pinfo_str->palette, (const void*)iscnv_get_pal(1), 2 * sizeof(unsigned long));
      }

      break;

    case  4:
      if (sdp->org_img->bitcount <= 4) {  /* 1~4 bit */
        memcpy((void*)pinfo_str->palette, (const void*)sdp->org_img->palette, MAX_PALETTE_COUNT * sizeof(unsigned long));
      }
      else if (sdp->org_img->bitcount <= 8) { /* 5~8 bit */
        j = (unsigned int)pow((int)2, (int)sdp->org_img->bitcount) / 16;

        for (i = 0; i < 16; i++) {
          ((unsigned long*)(pinfo_str->palette))[i] = ((unsigned long*)sdp->org_img->palette)[i * j];
        }
      }
      else {
        memcpy((void*)pinfo_str->palette, (const void*)iscnv_get_pal(4), 16 * sizeof(unsigned long));
      }

      break;

    case  8:
      if (sdp->org_img->bitcount <= 8) {
        memcpy((void*)pinfo_str->palette, (const void*)sdp->org_img->palette, MAX_PALETTE_COUNT * sizeof(unsigned long));
      }
      else {
        memcpy((void*)pinfo_str->palette, (const void*)iscnv_get_pal(8), 256 * sizeof(unsigned long));
      }

      break;

    default:
      assert(0);
      isirw_free_INFOSTR(pinfo_str);
      return 0;
    }

    pinfo_str->pal_count = (unsigned int)pow((int)2, (int)pinfo_str->bitcount);
  }
  else {
    pinfo_str->pal_count = 0;
  }

  linesize = DIBSCANLINE_DWORDALIGN(pinfo_str->bitcount * pinfo_str->width);

  /* ����ת���� */
  pinfo_str->p_bit_data = (unsigned char*)isirw_cnvpix_malloc(linesize * pinfo_str->height + 4);

  if (!pinfo_str->p_bit_data) {
    isirw_free_INFOSTR(pinfo_str);
    return 0;
  }

  /* �����ַ�� */
  pinfo_str->pp_line_addr = (void**)isirw_malloc(pinfo_str->height * sizeof(void*));

  if (!pinfo_str->pp_line_addr) {
    isirw_free_INFOSTR(pinfo_str);
    return 0;
  }

  /* ��ʼ�����׵�ַ */
  if (pinfo_str->order == 0) {  /* ����ͼ */
    for (i = 0; i < (int)pinfo_str->height; i++) {
      pinfo_str->pp_line_addr[i] = (void*)(unsigned char*)(pinfo_str->p_bit_data + i * linesize);
    }
  }
  else {            /* ����ͼ */
    for (i = 0; i < (int)pinfo_str->height; i++) {
      pinfo_str->pp_line_addr[i] = (void*)(unsigned char*)(pinfo_str->p_bit_data + ((pinfo_str->height - 1 - i) * linesize));
    }
  }

  pinfo_str->data_state = 2;

  return pinfo_str;
}


/* ����һ���µ�ת������ͼ�����ݰ� */
LPSUBIMGBLOCK CALLAGREEMENT _isirw_build_si(LPSUBIMGBLOCK psub_str, int fmt/* Ŀ���ʽID */)
{
  LPSUBIMGBLOCK psub_tmp;
  int       linesize, i, k;

  /* �����µ���ͼ��ڵ� */
  if ((psub_tmp = isirw_alloc_SUBIMGBLOCK()) == 0) {
    return 0;
  }

  /* ������ͼ��ڵ����� */
  psub_tmp->left  = psub_str->left;
  psub_tmp->top   = psub_str->top;
  psub_tmp->width = psub_str->width;
  psub_tmp->height = psub_str->height;

  assert((psub_tmp->width != 0) && (psub_tmp->height != 0));

  psub_tmp->bitcount = iscnv_idtobit(fmt);

  /* ɨ���гߴ� */
  linesize = DIBSCANLINE_DWORDALIGN(psub_tmp->width * psub_tmp->bitcount);

  /* ����Ŀ��ͼ�񻺳�����+4 �� ���ӵ�4�ֽڻ������� */
  if ((psub_tmp->p_bit_data = (unsigned char*)isirw_malloc(linesize * psub_tmp->height + 4)) == 0) {
    isirw_free_SUBIMGBLOCK(psub_tmp);
    return 0;
  }

  /* �����е�ַ���� */
  if ((psub_tmp->pp_line_addr = (void**)isirw_malloc(sizeof(void*)*psub_tmp->height)) == 0) {
    isirw_free_SUBIMGBLOCK(psub_tmp);
    return 0;
  }

  psub_tmp->order = psub_str->order;

  /* ��ʼ�����׵�ַ */
  if (psub_tmp->order == 0) { /* ����ͼ */
    for (i = 0; i < (int)psub_tmp->height; i++) {
      psub_tmp->pp_line_addr[i] = (void*)(unsigned char*)(psub_tmp->p_bit_data + i * linesize);
    }
  }
  else {            /* ����ͼ */
    for (i = 0; i < (int)psub_tmp->height; i++) {
      psub_tmp->pp_line_addr[i] = (void*)(unsigned char*)(psub_tmp->p_bit_data + ((psub_tmp->height - 1 - i) * linesize));
    }
  }

  /* ������������ */
  iscnv_get_mask(fmt, &(psub_tmp->r_mask), &(psub_tmp->g_mask), &(psub_tmp->b_mask), &(psub_tmp->a_mask));

  /* ���÷�ʽ */
  psub_tmp->dowith    = psub_str->dowith;
  /* ����ʾ��һ����ͼ��ǰ�Ƿ���Ҫ�ȴ�ĳһ�¼� */
  psub_tmp->userinput = psub_str->userinput;
  /* �ӳ�ʱ�䣨1/1000 s��*/
  psub_tmp->time    = psub_str->time;
  /* ����͸��ɫ */
  psub_tmp->colorkey  = psub_str->colorkey;


  /* ���Ƶ�ɫ������ */
  if (psub_tmp->bitcount <= 8) {
    switch (psub_tmp->bitcount) {
    case  1:
      if (psub_str->bitcount <= 8) {
        ((unsigned long*)(psub_tmp->palette))[0] = ((unsigned long*)psub_str->palette)[0];
        ((unsigned long*)(psub_tmp->palette))[1] = ((unsigned long*)psub_str->palette)[(unsigned int)pow((int)2, (int)psub_str->bitcount) / 2];

        if (psub_tmp->colorkey != -1) {
          if (psub_tmp->colorkey == (long)(((unsigned long*)(psub_tmp->palette))[0])) {
            psub_tmp->colorkey = (long)(((unsigned long*)(psub_tmp->palette))[0]);
          }
          else {
            psub_tmp->colorkey = (long)(((unsigned long*)(psub_tmp->palette))[1]);
          }
        }
      }
      else {
        memcpy((void*)psub_tmp->palette, (const void*)iscnv_get_pal(1), 2 * sizeof(unsigned long));

        if (psub_tmp->colorkey != -1) {
          /* תΪ�Ҷ�ֵ */
          k = ((((psub_tmp->colorkey >> 16) & 0xff) * 3 + ((psub_tmp->colorkey >> 8) & 0xff) * 6 + (psub_tmp->colorkey & 0xff)) / 10) & 0xff;
          k = (k << 16) | (k << 8) | k;

          psub_tmp->colorkey = ((unsigned long)k >= ((unsigned long*)(psub_tmp->palette))[1]) ? ((unsigned long*)(psub_tmp->palette))[1] : ((unsigned long*)(psub_tmp->palette))[0];
        }
      }

      break;

    case  4:
      if (psub_str->bitcount <= 4) {  /* 1~4 bit */
        memcpy((void*)psub_tmp->palette, (const void*)psub_str->palette, MAX_PALETTE_COUNT * sizeof(unsigned long));
      }
      else if (psub_str->bitcount <= 8) { /* 5~8 bit */
        k = (unsigned int)pow((int)2, (int)psub_str->bitcount) / 16;

        for (i = 0; i < 16; i++) {
          ((unsigned long*)(psub_tmp->palette))[i] = ((unsigned long*)psub_str->palette)[i * k];
        }

        if (psub_tmp->colorkey != -1) {
          /* ����͸��ɫ�Ƿ񻹴����ڵ�ɫ���� */
          for (i = 0; i < 16; i++) {
            if (((unsigned long*)(psub_tmp->palette))[i] == (unsigned long)psub_tmp->colorkey) {
              break;
            }
          }

          /* ���͸��ɫ��ɫ�����ѱ���������ȡ��͸��ɫ */
          if (i == 16) {
            psub_tmp->colorkey = -1;
          }
        }
      }
      else {
        memcpy((void*)psub_tmp->palette, (const void*)iscnv_get_pal(4), 16 * sizeof(unsigned long));

        if (psub_tmp->colorkey != -1) {
          /* תΪ�Ҷ�ֵ */
          k = ((((psub_tmp->colorkey >> 16) & 0xff) * 3 + ((psub_tmp->colorkey >> 8) & 0xff) * 6 + (psub_tmp->colorkey & 0xff)) / 10) & 0xff;
          k = (k << 16) | (k << 8) | k;

          psub_tmp->colorkey = (long)k;
        }
      }

      break;

    case  8:
      if (psub_str->bitcount <= 8) {
        memcpy((void*)psub_tmp->palette, (const void*)psub_str->palette, MAX_PALETTE_COUNT * sizeof(unsigned long));
      }
      else {
        memcpy((void*)psub_tmp->palette, (const void*)iscnv_get_pal(8), 256 * sizeof(unsigned long));

        if (psub_tmp->colorkey != -1) {
          /* תΪ�Ҷ�ֵ */
          k = ((((psub_tmp->colorkey >> 16) & 0xff) * 3 + ((psub_tmp->colorkey >> 8) & 0xff) * 6 + (psub_tmp->colorkey & 0xff)) / 10) & 0xff;
          k = (k << 16) | (k << 8) | k;

          psub_tmp->colorkey = (long)k;
        }
      }

      break;

    default:
      assert(0);
      return 0;
    }

    psub_tmp->pal_count = (unsigned int)pow((int)2, (int)psub_tmp->bitcount);
  }
  else {
    psub_tmp->pal_count = 0;
  }

  /* ��ͼ�������� */
  psub_tmp->number  = psub_str->number;

  return psub_tmp;
}


/* ��ȡ���һ����ͼ�����Ϣ����ż���ͼ�����ݰ���ַ��*/
int CALLAGREEMENT _isirw_get_last_sert_image_info(LPINFOSTR pinfo, LPSUBIMGBLOCK* ppsi)
{
  LPSUBIMGBLOCK ptmp;

  if (pinfo->psubimg == 0) {
    *ppsi  = 0;
    return 0;             /* û����ͼ�� */
  }

  ptmp = pinfo->psubimg;

  while (ptmp->next) {          /* �������һ����ͼ�����ݰ� */
    ptmp = ptmp->next;
  }

  *ppsi  = ptmp;

  return (int)ptmp->number;
}


/* ��ȡָ����ŵ���ͼ����Ϣ����ż���ͼ�����ݰ���ַ��*/
int CALLAGREEMENT _isirw_get_sert_image_info(LPINFOSTR pinfo, int sid, LPSUBIMGBLOCK* ppsi)
{
  LPSUBIMGBLOCK ptmp;

  if (pinfo->psubimg == 0) {
    *ppsi  = 0;
    return 0;             /* û����ͼ�� */
  }

  ptmp = pinfo->psubimg;

  while (ptmp) {            /* ����ָ����ͼ�����ݰ� */
    if (ptmp->number == sid) {
      break;
    }
    else {
      ptmp = ptmp->next;
    }
  }

  *ppsi  = ptmp;

  return (ptmp == 0) ? 0 : sid;
}


