/********************************************************************

  iseeeng.c

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
  ���ļ���;��  ͼ���д���棭��Ϣ��ģ��ʵ���ļ�
  ���ļ���д�ˣ�
          YZ        yzfree##sina.com
          ����      qing_se##163.com
          ����      mssoft##sina.com

  ���ļ��汾��  30719
  ����޸��ڣ�  2003-7-19

  ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
  ��ַ�ռ������
  ----------------------------------------------------------------
  ������ʷ��

    2003-7    ����һ��BUG���ڶ����������������ת����Ϊ��ԭ���Ĵ���û����Ӧ��
          ����ת�����ݱ�־��
    2003-6    ��Ϣ�ø����¼�������ʽ���У���Ч����CPU���ɣ����ͣ�
    2003-5    �޸ı��湦�ܵ�����Դָ�����ȱ���ת�����ͼ�����ݣ�
    2003-2    �޲�����ӿ��д��ڸ���Դͼ���BUG����������
    2003-2    �޸���Ϣ�ù�����ʽ���޲�lock_pump()�����е�BUG
    2002-11   �Ľ���Ϣ�öԿ�������Ĵ���ʽ����Ϊ�ӳٴ���
    2002-10   ��һ�����԰淢��

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
#include <process.h>
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
#include "iseeeng.h"
#include "queue.h"


/**********************************************************************************/
/* �ڲ������� */
static int        _isee_pump_mark    = 0;     /* ��Ϣ��ϵͳ���ñ�־��0��δ���á�1������ */
static unsigned long  _isee_pump_handle  = 0xffffffff;/* ��Ϣ�þ�� */
static int        _isee_pump_locked  = 0;     /* ��Ϣ��������־��0��δ����1������ */
static int        _isee_pump_run_mark = 1;    /* ��Ϣ�����б�־��0���رա�1������ */
static unsigned int   _isee_pump_lccb    = 0;     /* ��Ϣ��������֤��־ */
static HANDLE     _isee_pump_semap   = 0;     /* ��Ϣ�źŵ� */


/**********************************************************************************/
/* �ڲ����� */
static void __cdecl _isirw_pump(void* par);       /* ��Ϣ�� */
void _isirw_pump_comm_process(void* pcomm);
void _isirw_pump_idle_process(void* pidle);
void _ipcp_get_info(LPSPEC_DATAPACK sdp);
void _ipcp_load(LPSPEC_DATAPACK sdp);
void _ipcp_save(LPSPEC_DATAPACK sdp);





/**********************************************************************************/
/* �ӿں��� */

/* ����Ϣ��ϵͳ */
int CALLAGREEMENT open_pump()
{
  assert(_isee_pump_mark == 0);           /* �����������ظ����� */

  /* ��ʼ������ */
  init_queue();

  /* ��������źŵ� */
  if ((_isee_pump_semap = CreateSemaphore(0, 0, ISEEENG_QUEUE_COMM_SIZE, 0)) == 0) {
    destroy_queue();
    return -6;
  }

  /* ������Ϣ�ã��̣߳�*/
  if ((_isee_pump_handle = _beginthread(_isirw_pump, 0, 0)) == 0xffffffff) {
    CloseHandle(_isee_pump_semap);
    _isee_pump_semap = 0;

    destroy_queue();
    return -6;                    /* �̴߳���ʧ�� */
  }

  /* ��ʼ������־ */
  _isee_pump_locked = 0;              /* ��Ϣ��������־ */
  _isee_pump_run_mark = 1;              /* ��Ϣ�����б�־ */
  _isee_pump_mark   = 1;              /* ��Ϣ��ϵͳ���ñ�־ */
  _isee_pump_lccb     = 0;

  return 0;
}


/* �ر���Ϣ��ϵͳ */
int CALLAGREEMENT close_pump()
{
  assert(_isee_pump_mark != 0);
  assert(_isee_pump_locked == 0);

  /* ��������Ϣ�ã���ֹͣ��־ */
  lock_pump();

  assert(_isee_pump_locked == 1);

  _isee_pump_run_mark = 0;

  ReleaseSemaphore(_isee_pump_semap, 1, 0);     /* �����źŵƣ���ʹ�����ܸ����ֹͣ */

  /* �ȴ���Ϣ��ֹͣ */
  while (_isee_pump_run_mark == 0) {
    Sleep(0);
  }

  assert(get_comm_count() == 0);            /* ��ʱ��������в�Ӧ���������� */

  /* ���ٶ��� */
  destroy_queue();

  /* ���ٶ����źŵ� */
  CloseHandle(_isee_pump_semap);

  _isee_pump_mark     = 0;
  _isee_pump_run_mark = 1;
  _isee_pump_locked   = 0;
  _isee_pump_lccb     = 0;
  _isee_pump_semap    = 0;

  return 0;
}


/* ��ȡ��Ϣ��ϵͳ��״̬���Ƿ�����*/
int CALLAGREEMENT get_pump_status()
{
  /* 0-δ���á�1-���� */
  return _isee_pump_mark;
}


/* ������Ϣ�� */
void CALLAGREEMENT lock_pump()
{
  unsigned int  ispl;

  assert(_isee_pump_mark == 1);
  assert(_isee_pump_locked == 0);   /* ������Ƕ������ */

  ispl = _isee_pump_lccb;

  _isee_pump_locked = 1;        /* ��������־ */

  while (ispl == _isee_pump_lccb) { /* �ȴ���ֱ�����������֤Ϊֹ */
    Sleep(0);
  }
}


/* ������Ϣ�� */
void CALLAGREEMENT unlock_pump()
{
  assert(_isee_pump_mark == 1);
  assert(_isee_pump_locked == 1);   /* ������Ƕ������ */

  _isee_pump_locked = 0;
}


/* �������� */
int CALLAGREEMENT add_pump_comm(LPSPEC_DATAPACK p)
{
  int   result;

  assert(_isee_pump_mark == 1);
  assert(p->exec_state == IRWE_SDPSTATE_IDLE);

  p->exec_state = IRWE_SDPSTATE_WAIT;

  /* ��������ȷ����������ȼ� */
  if (p->command == IRWE_SDPCOMM_GET_INFO) {
    result = add_comm((void*)p, 0);         /* ����������е�β�� */
  }
  else {
    result = add_comm((void*)p, 1);         /* ����������е�ͷ�������ȴ���*/
  }

  /* ��������δ��������У������ִ��״̬��Ϊ���� */
  if (result == -1) {
    p->exec_state = IRWE_SDPSTATE_IDLE;
  }

  /* �����źŵƸ��� */
  ReleaseSemaphore(_isee_pump_semap, 1, 0);

  return result;                    /* 0 :����ɹ���-1 :�ڴ治�㣬ʧ�� */
}


/* ȡ��������ݲ���p�� */
int CALLAGREEMENT dist_pump_comm(LPSPEC_DATAPACK p)
{
  void*  pt;

  assert((_isee_pump_mark == 1) && (p));

  /* Ԥ�ȼ��ٶ����źŵƸ��� */
  if (WaitForSingleObject(_isee_pump_semap, 0) != WAIT_OBJECT_0) {
    return -1;                    /* ����������������� */
  }

  /* ��������ȡָ���� */
  if ((pt = distill_comm((void*)p)) != 0) {
    assert(pt == p);
    assert(p->exec_state == IRWE_SDPSTATE_WAIT);  /* �մӶ�����ȡ��ʱӦ���е�״̬ */
    p->exec_state = IRWE_SDPSTATE_IDLE;       /* �ð���ִ��״̬Ϊ���� */
  }
  else {
    /* ���û����ȡ��ָ�������ָ��źŵƸ��� */
    ReleaseSemaphore(_isee_pump_semap, 1, 0);
  }

  return (pt == 0) ? -1 : 0;  /* 0 :�ҵ���ȡ����-1 :δ�ҵ�ָ�������� */
}


/* ȡ��ָ�����κţ�sn���ĵ�һ������ */
LPSPEC_DATAPACK CALLAGREEMENT dist_pump_comm_sn(unsigned long sn)
{
  void*  pt;

  assert(_isee_pump_mark == 1);

  /* Ԥ�ȼ��ٶ����źŵƸ��� */
  if (WaitForSingleObject(_isee_pump_semap, 0) != WAIT_OBJECT_0) {
    return 0;                   /* 0 :����������������� */
  }

  /* ��ȡָ�����εĵ�һ���� */
  if ((pt = distill_comm_sn(sn)) != 0) {
    assert(((LPSPEC_DATAPACK)pt)->exec_state == IRWE_SDPSTATE_WAIT);
    ((LPSPEC_DATAPACK)pt)->exec_state = IRWE_SDPSTATE_IDLE;
  }
  else {
    /* ���û��ָ�����ε������ָ��źŵƸ��� */
    ReleaseSemaphore(_isee_pump_semap, 1, 0);
  }

  return (LPSPEC_DATAPACK)pt;             /* 0 :�������������sn���ε������0 :��ȡ�������� */
}


/* ��ȡ�������������� */
int CALLAGREEMENT get_pump_comm_count()
{
  assert(_isee_pump_mark == 1);

  return (int)get_comm_count();
}


/* �ӿ��д������� */
int CALLAGREEMENT add_pump_idle(void* p/*���д�������ַ*/)
{
  assert(_isee_pump_mark == 1);

  return add_idlesse(p);                /* 0 :����ɹ���-1 :��������ʧ�� */
}





/**********************************************************************************/
/* ISeeͼ���д������Ϣ�� */

static void __cdecl _isirw_pump(void* par)
{
  void*  pcomm = 0;

  /* �ȴ�open_pump()������ʼ�������еı�־ */
  while (_isee_pump_mark == 0) {
    Sleep(0);
  }

  /* ��ȡ���������� */
  while (_isee_pump_run_mark) {
    /* ����ñ��������򲻽����κ�����Ĵ����ͷ�ʣ���ʱ��Ƭ */
    if (_isee_pump_locked) {
      _isee_pump_lccb++;              /* ����������֤ */
      Sleep(0);
      continue;
    }

    /* �ж��Ƿ��д���������� */
    switch ((unsigned int)WaitForSingleObject(_isee_pump_semap, ISEEENG_IDLE_STIME * 1000)) {
    case  WAIT_OBJECT_0:

      /* ��������еĶ�д���� */
      if ((pcomm = distill_comm(0)) != 0) {
        _isirw_pump_comm_process(pcomm);
      }

      break;

    case  WAIT_TIMEOUT:

      /* ��ʱ������п���ָ��� */
      if ((pcomm = distill_idlesse()) != 0) {
        _isirw_pump_idle_process(pcomm);
      }

      break;

    default:
      break;
    }
  }

  /* ��close_pump()��������ֹͣ��Ϣ */
  _isee_pump_run_mark = 1;

  /* �˳��̣߳����ͷ���Դ */
  _endthread();
}


/* ����� */
void _isirw_pump_comm_process(void* pcomm)
{
  LPSPEC_DATAPACK sdp = (LPSPEC_DATAPACK)pcomm;

  assert((sdp) && (sdp->dbg_mark == DATAPACK_MARK_SPECIALTY));
  assert(sdp->exec_state == IRWE_SDPSTATE_WAIT);

  /* ��־ִ�п�ʼ */
  sdp->exec_state = IRWE_SDPSTATE_EXEC;

  /* �������������Ӧ�Ľ��ͺ��� */
  switch (sdp->command) {
  case  IRWE_SDPCOMM_GET_INFO:            /* ��ͼ����Ϣ */
    _ipcp_get_info(sdp);
    break;

  case  IRWE_SDPCOMM_LOAD:              /* ��ͼ�� */
    _ipcp_load(sdp);
    break;

  case  IRWE_SDPCOMM_SAVE:              /* ����ͼ�� */
    _ipcp_save(sdp);
    break;

  default:                      /* �Ƿ����� */
    assert(0);
    break;
  }

  /* ��־ִ�н��� */
  sdp->exec_state = IRWE_SDPSTATE_IDLE;
}


/* ��ȡͼ����Ϣ������ͺ��� */
void _ipcp_get_info(LPSPEC_DATAPACK sdp)
{
  unsigned long     handle;
  IRWP_API_GET_IMG_INFO pfn_get_info;

  /* ������û�н�����ϢҪ���� */
  sdp->s_total   = 0;   /* �ܽ���ֵ���۽�ʽ��*/
  sdp->s_current = 0;   /* ��ǰ����ֵ */
  sdp->s_state   = 0;   /* ����״̬��ͼ��ţ� */

  /* ������ݰ����Ѿ�������ͼ����Ϣ����ֱ�ӷ��� */
  if (sdp->data_state & SDP_DATA_STATE_ORG_INFO) {
    sdp->last_err = ER_SUCCESS;
    sdp->s_break   = 0;   /* ��ֹ������־����0��ֹ�� */
    return;
  }

  /* ����û��Ƿ�Ҫ�ж�����Ĳ��� */
  if (sdp->s_break) {
    sdp->last_err = ER_USERBREAK;
    return;
  }

  /* ȡ�ô������ľ�� */
  if ((handle = ismf_get_plugin_heandle((const char*)sdp->path->name)) == 0) {
    sdp->last_err = ER_NONIMAGE;
    return;
  }

  /* ȡ��get_info�ӿڵ�ַ */
  pfn_get_info = ismf_get_info_addr(handle);
  assert(pfn_get_info);

  /* ��ȡͼ����Ϣ */
  sdp->last_err = (*pfn_get_info)(sdp->path, sdp->org_img);

  /* �ͷŴ�������� */
  ismf_free_plugin_heandle(handle);

  /* �����ð��Ѻ���ԭʼͼ����Ϣ */
  if (sdp->last_err == ER_SUCCESS) {
    sdp->data_state |= SDP_DATA_STATE_ORG_INFO;
  }

  return;
}


/* ��ͼ��������ͺ��� */
void _ipcp_load(LPSPEC_DATAPACK sdp)
{
  unsigned long   handle;
  IRWP_API_LOAD_IMG pfn_load;

  /* ��ʼ��������Ϣ */
  sdp->s_total   = 1;   /* �ܽ���ֵ���۽�ʽ��*/
  sdp->s_current = 0;   /* ��ǰ����ֵ */
  sdp->s_state   = 0;   /* ����״̬��ͼ��ţ� */

  /* ������ݰ����Ѿ�������ͼ����ֱ�ӷ��� */
  if (sdp->data_state & SDP_DATA_STATE_ORG_PIX) {
    sdp->last_err = ER_SUCCESS;
    sdp->s_break   = 0;   /* ��ֹ������־����0��ֹ�� */
    return;
  }

  /* ����û��Ƿ�Ҫ�ж�����Ĳ��� */
  if (sdp->s_break) {
    sdp->last_err = ER_USERBREAK;
    return;
  }

  /* ȡ�ô������ľ�� */
  if ((handle = ismf_get_plugin_heandle((const char*)sdp->path->name)) == 0) {
    sdp->last_err = ER_NONIMAGE;
    return;
  }

  /* ȡ��load�ӿڵ�ַ */
  pfn_load = ismf_get_load_addr(handle);
  assert(pfn_load);

  /* ��ȡͼ���������� */
  sdp->last_err = (*pfn_load)(sdp->path, sdp->org_img);

  /* �ͷŴ�������� */
  ismf_free_plugin_heandle(handle);

  /* �����ð��Ѻ���ԭʼͼ�� */
  if (sdp->last_err == ER_SUCCESS) {
    sdp->data_state |= SDP_DATA_STATE_ORG_INFO;
    sdp->data_state |= SDP_DATA_STATE_ORG_PIX;

    /* ���������ת����������ת�����ݱ�־ */
    if (sdp->cnv_img) {
      sdp->data_state |= SDP_DATA_STATE_CNV;
    }
  }

  return;
}


/* ����ͼ��������ͺ��� */
void _ipcp_save(LPSPEC_DATAPACK sdp)
{
  unsigned long   handle;
  IRWP_API_SAVE_IMG pfn_save;

  /* ��ʼ��������Ϣ */
  sdp->s_total   = 1;   /* �ܽ���ֵ���۽�ʽ��*/
  sdp->s_current = 0;   /* ��ǰ����ֵ */
  sdp->s_state   = 0;   /* ����״̬��ͼ��ţ� */

  /* ������ݰ���û��ͼ����ֱ�ӷ��� */
  if (!(sdp->data_state & SDP_DATA_STATE_ORG_PIX)) {
    assert(0);
    sdp->last_err = ER_SYSERR;
    return;
  }

  /* ����û��Ƿ�Ҫ�ж�����Ĳ��� */
  if (sdp->s_break) {
    sdp->last_err = ER_USERBREAK;
    return;
  }

  /* ȡ�ô������ľ�� */
  if ((handle = ismf_get_plugin_heandle((const char*)sdp->despath->name)) == 0) {
    sdp->last_err = ER_NONIMAGE;
    return;
  }

  /* ȡ��save�ӿڵ�ַ */
  pfn_save = ismf_get_save_addr(handle);
  assert(pfn_save);

  /* ������е�ͼ�� */
  if (sdp->cnv_img) {
    sdp->last_err = (*pfn_save)(sdp->despath, sdp->cnv_img, sdp->savestr);  /* �������ת��ͼ����洢ת��ͼ�� */
  }
  else {
    sdp->last_err = (*pfn_save)(sdp->despath, sdp->org_img, sdp->savestr);
  }

  /* �ͷŴ�������� */
  ismf_free_plugin_heandle(handle);

  return;
}



/* ���д��� */
void _isirw_pump_idle_process(void* pidle)
{
  _ISIRW_PUMP_IDLE_FN fnp = (_ISIRW_PUMP_IDLE_FN)pidle;

  (*fnp)();

  return;
}





