/********************************************************************

  iseeimn.c

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
  ���ļ���;��  ͼ���д���棭�������ʵ���ļ�
  ���ļ���д�ˣ�
          YZ        yzfree##sina.com

  ���ļ��汾��  30517
  ����޸��ڣ�  2003-5-17

  ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
    ��ַ�ռ������
  ----------------------------------------------------------------
  ������ʷ��

    2003-5    ismf_update()�������ӷ�����Ϣ�ṹ
          �����������ȼ��ļ��㷽��
    2003-2    ������������ͷ�ʽ�д��ڵ�BUG���Է����ж������
    2002-8    ��һ�����԰淢��

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


/*************************************************************************/
static LPIRWMF_HEADER ismf_info = 0;          /* �����Ϣ�� */
static CRITICAL_SECTION ismf_cs;            /* �����Ϣ����ʹؼ��� */

static char     _ismf_openstr[ISIRW_OPENSTR_SIZE];  /* ����Windowsƽ̨OPEN�Ի������չ�����˴� */
/* ����ʽ��μ�MSDN���й�CFileDialog���캯����5���������ĵ�˵�� */
static char     _ismf_shellstr[ISIRW_OPENSTR_SIZE]; /* ͨ����չ�����˴�*/
/* ����ʽΪ��"*.bmp,*.gif,*.xxx,....." */

static unsigned long  _ismf_idle_counter = 0;     /* ���д�������ͼ��� */



/*************************************************************************/
LPIRWMF_INFOBLOCK _ismf_alloc_IRWMF_INFOBLOCK(void);
void        _ismf_free_IRWMF_INFOBLOCK(LPIRWMF_INFOBLOCK pt);
LPIRWMF_HEADER    _ismf_alloc_IRWMF_HEADER(void);
void        _ismf_free_IRWMF_HEADER(LPIRWMF_HEADER pt);

int         _create_mini(char* path_str, LPIRWMF_HEADER ispt);
int         _load_mini(FILE* fp, LPIRWMF_HEADER ispt);
void        _verify_mini(LPIRWMF_HEADER phead, char* path, LPUPDATAINFO rinfo);

LPIRWMF_INFOBLOCK _remove_node(LPIRWMF_HEADER phead, int type, LPIRWMF_INFOBLOCK pnode);

int         _search_plugin_info_block(LPIRWMF_INFOBLOCK* ppblock, const char* path);
LPIRWMF_INFOBLOCK _ismf_search_at_filename(const char* ss);
int         _isirw_update_filtrate_str(void);
char* CALLAGREEMENT _ismf_get_openstr(void);
char* CALLAGREEMENT _ismf_get_shellstr(void);

void        _ismf_idle(void);



/*************************************************************************/


/* ��ʼ�������Ϣ���ڴ棩*/
int CALLAGREEMENT ismf_init(char* path,/*���INI�ļ�����·��*/ char* fname/*���ȴ����ͼ���ļ�*/)
{
  FILE*    fp = 0;
  char    pfull[MAX_PATH], path2[MAX_PATH], path3[MAX_PATH];
  int     result = 0, i = 0, actcnt = IRWMF_ACTIVE_INIT_CNT, j;

  LPIRWMF_INFOBLOCK ptmp;
  LPIRWMF_INFOBLOCK phig;


  assert(path);
  assert(ismf_info == 0);               /* �����������ظ����� */


  __try {
    __try {
      /* ��ʼ����Ϣ����ʹؼ��� */
      InitializeCriticalSection(&ismf_cs);
      EnterCriticalSection(&ismf_cs);

      /* ��չ��˴������� */
      memset((void*)_ismf_openstr, 0, ISIRW_OPENSTR_SIZE);
      memset((void*)_ismf_shellstr, 0, ISIRW_OPENSTR_SIZE);

      /* �����µĵı�ͷ�ṹ */
      if ((ismf_info = _ismf_alloc_IRWMF_HEADER()) == 0) {
        result = -1;
        __leave;         /* �ڴ治�� */
      }

      /* �ϳ���Ϣ�ļ���ȫ·�� */
      strcpy((char*)pfull, (const char*)path);

      /* ȷ��·�������һ���ַ���"\" */
      if (pfull[strlen((const char*)pfull) - 1] != '\\') {
        strcat((char*)pfull, (const char*)"\\");
      }

      /* �ϳ�INI�ļ�ȫ·�� */
      strcpy((char*)path2, (const char*)pfull);
      strcat((char*)pfull, (const char*)IRWMF_FILENAME);

      /* ��̽�Ŵ�INI�ļ� */
      if ((fp = fopen((const char*)pfull, "rb")) == 0) {
        /* û���ҵ�INI�ļ������Ŵ����� */
        switch (_create_mini(path2, ismf_info)) {
        case  0:                /* �����ɹ� */
          break;

        case  1:                /* �޿��ò�� */
          result = -2;
          __leave;
          break;

        case  2:                /* �ļ���д���� */
          result = -3;
          __leave;
          break;

        case  3:                /* �ڴ治�� */
          result = -1;
          __leave;
          break;

        default:                /* ϵͳ�쳣 */
          result = -5;
          __leave;
          break;
        }

        assert(ismf_info->plugin_num > 0);    /* ����Ӧ����һ����� */

        ismf_info->build = IRWMF_HEADER_BUD_CREATE;   /* �����ķ�ʽ */
      }
      else {
        /* �ҵ���INI�ļ������Ŷ������е����� */
        switch (_load_mini(fp, ismf_info)) {
        case  0:                /* ����ɹ� */
          break;

        case  1:                /* �ļ���д���� */
          result = -3;
          __leave;
          break;

        case  2:                /* INI�ļ����ݷǷ� */
          result = -4;
          __leave;
          break;

        case  3:                /* �ڴ治�� */
          result = -1;
          __leave;
          break;

        default:                /* ϵͳ�쳣 */
          result = -5;
          __leave;
          break;
        }

        assert(ismf_info->plugin_num > 0);

        /* У������INI���� */
        _verify_mini(ismf_info, path2, 0);

        /* ����������Ƿ�Ϊ0 */
        if (ismf_info->plugin_num == 0) {
          result = -2;
          __leave;       /* û�в������ */
        }

        /* ���ñ����ķ�ʽ */
        ismf_info->build = IRWMF_HEADER_BUD_LOAD;
      }

      /* ���������ָ����ĳ��ͼ���ļ��������ȼ����Ӧ�Ĳ�� */
      if (fname != 0) {
        /* �����ļ���������Ӧ��� */
        if (_search_plugin_info_block(&ptmp, (const char*)fname) > 0) {
          assert(ptmp && ptmp->use && (ptmp->act == 0));

          /* �ϳɲ��ȫ·�� */
          strcpy((char*)path3, (const char*)path2);
          strcat((char*)path3, (const char*)ptmp->path);

          /* ������ */
          if ((ptmp->handle = LoadLibrary((LPCTSTR)path3)) == 0) {
            result = -3;
            __leave;     /* �ļ���д���� */
          }

          /* ȡ�ýӿں�����ַ */
          if (((ptmp->fn_get_img_info = (IRWP_API_GET_IMG_INFO)isirw_get_api_addr(ptmp->handle, &(ptmp->ifo), get_image_info)) == 0) || \
              ((ptmp->fn_load_img = (IRWP_API_LOAD_IMG)isirw_get_api_addr(ptmp->handle, &(ptmp->ifo), load_image)) == 0) || \
              ((ptmp->fn_save_img = (IRWP_API_SAVE_IMG)isirw_get_api_addr(ptmp->handle, &(ptmp->ifo), save_image)) == 0)) {
            /* ����ļ��⵽�ƻ��������ò�� */
            FreeLibrary(ptmp->handle);
            ptmp->handle = 0;
            result = -4;
            __leave;
          }

          ptmp->act = 1;            /* ���ü����־ */

          assert(actcnt > 0);
          actcnt--;
        }
      }

      /* ���ݲ�����ȼ������� */
      for (i = 0; i < actcnt; i++) {
        phig = 0;

        /* ����¼���������ȼ���ߵĲ�� */
        for (j = 0, ptmp = ismf_info->puh; j < ismf_info->uc; j++, ptmp = ptmp->next) {
          assert(ptmp);

          /* ����ò���Ѿ�����������������һ����� */
          if (ptmp->act > 0) {
            continue;
          }

          /* ð�������ҵ����ȼ���ߵĲ�� */
          if ((phig == 0) || (ptmp->pr > phig->pr)) {
            phig = ptmp;
          }
        }

        /* ����Ѿ�û�пɼ������ˣ����˳�ѭ�� */
        if (phig == 0) {
          break;
        }

        ptmp = phig;

        assert(ptmp && ptmp->use && (ptmp->act == 0));

        /* �ϳɲ��ȫ·�� */
        strcpy((char*)path3, (const char*)path2);
        strcat((char*)path3, (const char*)ptmp->path);

        /* ������ */
        if ((ptmp->handle = LoadLibrary((LPCTSTR)path3)) == 0) {
          result = -3;
          __leave;         /* �ļ���д���� */
        }

        /* ȡ�ýӿں�����ַ */
        if (((ptmp->fn_get_img_info = (IRWP_API_GET_IMG_INFO)isirw_get_api_addr(ptmp->handle, &(ptmp->ifo), get_image_info)) == 0) || \
            ((ptmp->fn_load_img = (IRWP_API_LOAD_IMG)isirw_get_api_addr(ptmp->handle, &(ptmp->ifo), load_image)) == 0) || \
            ((ptmp->fn_save_img = (IRWP_API_SAVE_IMG)isirw_get_api_addr(ptmp->handle, &(ptmp->ifo), save_image)) == 0)) {
          /* ����ļ��⵽�ƻ��������ò�� */
          FreeLibrary(ptmp->handle);
          ptmp->handle = 0;
          result = -4;
          __leave;
        }

        ptmp->act = 1;                /* ���ü����־ */
      }

      _isirw_update_filtrate_str();         /* ��д��չ�����˴� */

      ismf_info->mark = 1;              /* ��ͷ��Ϣ��Ч��� */
    }
    __finally {
      if ((result != 0) || (AbnormalTermination())) {
        if (ismf_info) {
          /* ����в�����ڼ���״̬�����ͷŸò�� */
          for (i = 0, ptmp = ismf_info->puh; i < (int)ismf_info->uc; i++, ptmp = ptmp->next) {
            assert(ptmp);
            assert(ptmp->act <= 1);     /* �ò�����ڻ������ܱ���Ϣ��ʹ�� */

            /* �������ѱ�������ͷ��� */
            if (ptmp->act == 1) {
              FreeLibrary(ptmp->handle);

              ptmp->handle          = 0;
              ptmp->fn_get_img_info = 0;
              ptmp->fn_load_img     = 0;
              ptmp->fn_save_img     = 0;
              ptmp->act = 0;
            }
          }

          _ismf_free_IRWMF_HEADER(ismf_info);
          ismf_info = 0;
        }

        LeaveCriticalSection(&ismf_cs);
        DeleteCriticalSection(&ismf_cs);
      }
      else {
        LeaveCriticalSection(&ismf_cs);
      }

      if (fp) {
        fclose(fp);
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = -5;                  /* ϵͳ�쳣 */
  }

  return result;
}


/* ��ȡ�������ϵͳ������״̬ */
int CALLAGREEMENT ismf_get_status()
{
  if ((ismf_info) && (ismf_info->mark)) {
    return 1;                   /* ϵͳ������ */
  }
  else {
    return 0;                   /* ϵͳ��δ���� */
  }
}


/* ���������Ϣ������Ҳ����ͣ������*/
LPIRWMF_HEADER CALLAGREEMENT ismf_lock()
{
  int         i;
  LPIRWMF_INFOBLOCK pii;

  assert((ismf_info) && (ismf_info->mark == 1));

  /* ����������־ */
  EnterCriticalSection(&ismf_cs);
  assert(ismf_info->locked == 0);           /* ��Ϣ���ܱ�Ƕ������ */
  ismf_info->locked++;
  LeaveCriticalSection(&ismf_cs);

  /* �ȴ����еĲ������������ */
  for (i = 0, pii = ismf_info->puh; i < (int)ismf_info->uc; i++, pii = pii->next) {
    if ((pii->act == 0) || (pii->act == 1)) {
      continue;
    }
    else {
      while (pii->act > 1) {
        Sleep(ISIRW_THREAD_IDLE);
      }
    }
  }

  return ismf_info;
}


/* ˢ�²����Ϣ����ʱ�����Ϣ����봦������״̬�£�*/
int CALLAGREEMENT ismf_update(LPIRWMF_HEADER pt, int type, LPUPDATAINFO rinfo)
{
  int         i, result = 0;
  char        path[MAX_PATH], path2[MAX_PATH];
  LPIRWMF_INFOBLOCK pa;
  LPIRWMF_INFOBLOCK pifb = 0;
  LPIRWP_INFO     pii = 0;

  WIN32_FIND_DATA   file;
  HANDLE        hFind = 0;
  HMODULE       hDll  = 0;

  WIN32_FILE_ATTRIBUTE_DATA wfa;

  /* ��ʱ��Ϣ�������Ч�����ұ��봦������״̬ */
  assert(ismf_info && (ismf_info->mark == 1) && ismf_info->locked);
  assert((ismf_info == pt) && (ismf_info->plugin_num > 0));
  assert((type == IRWE_UPDTYPE_EMPLOY) || (type == IRWE_UPDTYPE_DISMISSAL));

  __try {
    __try {
      /* ��ȡ�������·�����ϳ������� */
      strcpy((char*)path, (const char*)ismf_info->infpath);
      *(char*)(strrchr((const char*)path, '\\') + 1) = '\0';
      strcpy((char*)path2, (const char*)path);
      strcat((char*)path2, (const char*)"*.dll");

      /* У���Ѵ��ڲ����Ϣ��ȥ�����п����Ѿ������ڵĲ������Ϣ�飩 */
      _verify_mini(ismf_info, path, rinfo);

      /* �����Ƿ����µĻ������Ĳ�� */
      hFind = FindFirstFile((LPCTSTR)path2, &file);

      if (INVALID_HANDLE_VALUE == hFind) {
        assert(ismf_info->plugin_num == 0);
        result = 1;
        __leave;              /* û���ҵ�ͼ���д��� */
      }

      do {
        /* �ϳ�ȫ·�� */
        strcpy((char*)path2, (const char*)path);
        strcat((char*)path2, (const char*)(file.cFileName));

        /* ���ļ��Ƿ�����Ϣ���У����ھͱ�ʾ����һ�����ϡ������*/
        if ((pa = _ismf_search_at_filename((const char*)file.cFileName)) == 0) {
          /* ������ */
          /* �����DLL�ļ� */
          if ((hDll = LoadLibrary((LPCTSTR)path2)) == 0) {
            result = 2;
            __leave;          /* ������� */
          }

          /* �жϴ�DLL�Ƿ���ͼ���д��� */
          if ((pii = isirw_get_info(isirw_test_plugin(hDll))) == 0) {
            FreeLibrary(hDll);
            hDll = 0;
            continue;               /* ��DLL���ǲ�������� */
          }

          /* ������Ϣ�黺���� */
          if ((pifb = _ismf_alloc_IRWMF_INFOBLOCK()) == 0) {
            result = 3;
            __leave;          /* �ڴ治�� */
          }

          /* ��¼�����������Ϣ��ṹ��ַ */
          if (rinfo) {
            ((IRWMF_INFOBLOCK**)rinfo->newpp)[rinfo->newcnt++] = pifb;
          }

          /* ��д��Ϣ�� */
          pifb->pr    = IRWMF_INFOBLOCK_DEFPIR; /* ����������ȼ� */
          pifb->use   = (type == IRWE_UPDTYPE_EMPLOY) ? IRWMF_INFOBLOCK_USEING : IRWMF_INFOBLOCK_NOTUSE;
          pifb->pilen   = file.nFileSizeLow;    /* ����ļ����� */
          /* ����ļ�����ʱ�� */
          pifb->ctimeh  = file.ftCreationTime.dwHighDateTime;
          pifb->ctimel  = file.ftCreationTime.dwLowDateTime;
          /* ����ļ������޸�ʱ�� */
          pifb->mtimeh  = file.ftLastWriteTime.dwHighDateTime;
          pifb->mtimel  = file.ftLastWriteTime.dwLowDateTime;

          /* ����ļ��� */
          strcpy((char*)pifb->path, (const char*)file.cFileName);
          /* �����Ϣ�� */
          memcpy((void*) & (pifb->ifo), (const void*)pii, sizeof(IRWP_INFO));

          pifb->act   = 0;            /* �����ǣ�ȱʡΪδ���*/
          pifb->handle  = 0;            /* ������ */

          pifb->fn_get_img_info = 0;        /* get_image_info�ӿ� */
          pifb->fn_load_img   = 0;        /* load_image�ӿ� */
          pifb->fn_save_img   = 0;        /* save_image�ӿ� */

          pifb->prev    = 0;            /* ǰһ����Ϣ��ĵ�ַ */
          pifb->next    = 0;            /* ��һ����Ϣ��ĵ�ַ */
          pifb->pe    = pt;           /* ��ͷ��ַ */

          if (pifb->use == IRWMF_INFOBLOCK_USEING) {
            /* �����ͷ�ṹ�е�¼���� */
            if (pt->put == 0) {           /* ��һ�� */
              pt->puh = pt->put = pifb;
            }
            else {                  /* ���� */
              pifb->prev = pt->put;
              pt->put->next = pifb;
              pt->put = pifb;
            }

            pt->uc++;
          }
          else {
            assert(pifb->use == IRWMF_INFOBLOCK_NOTUSE);

            /* �����ͷ�ṹ�е�δ¼���� */
            if (pt->pnt == 0) {           /* ��һ�� */
              pt->pnh = pt->pnt = pifb;
            }
            else {                  /* ���� */
              pifb->prev = pt->pnt;
              pt->pnt->next = pifb;
              pt->pnt = pifb;
            }

            pt->nc++;
          }

          /* ����ܸ����ۼ� */
          pt->plugin_num++;

          /* �ͷŲ��DLL */
          FreeLibrary(hDll);

          hDll = 0;
          pifb = 0;
        }
        else {                    /* ���� */
          /* ��ȡ����ļ�����Ϣ */
          if (GetFileAttributesEx((LPCSTR)path2, GetFileExInfoStandard, (LPVOID)&wfa) == 0) {
            /* ��Ӧ���ļ���Ӧ���Ҳ�����ǰ������У�麯��У����ˣ� */
            result = 4;
            __leave;          /* ϵͳ���� */
          }
          else {
            i = 0;

            /* �������ļ����Ķ�������������ˢ����Ϣ */
            if (pa->pilen != wfa.nFileSizeLow) {
              i++;
              pa->pilen = wfa.nFileSizeLow;
            }

            if ((pa->ctimel != wfa.ftCreationTime.dwLowDateTime) || \
                (pa->ctimeh != wfa.ftCreationTime.dwHighDateTime)) {
              i++;
              pa->ctimel = wfa.ftCreationTime.dwLowDateTime;
              pa->ctimeh = wfa.ftCreationTime.dwHighDateTime;
            }

            if ((pa->mtimel != wfa.ftLastWriteTime.dwLowDateTime) || \
                (pa->mtimeh != wfa.ftLastWriteTime.dwHighDateTime)) {
              i++;
              pa->mtimel = wfa.ftLastWriteTime.dwLowDateTime;
              pa->mtimeh = wfa.ftLastWriteTime.dwHighDateTime;
            }

            if (i) {
              assert(pa->act == 0);       /* ����Ĳ���ļ��ǲ�Ӧ�ñ�����ϵͳ�ı�� */

              /* �жϴ�DLL�Ƿ���ͼ���д��� */
              if (((hDll = LoadLibrary((LPCTSTR)path2)) == 0) || \
                  ((pii = isirw_get_info(isirw_test_plugin(hDll))) == 0)) {
                /* ����ļ��⵽�ƻ��������ò�� */
                if (hDll) {
                  FreeLibrary(hDll);
                  hDll = 0;
                }

                /* ���ڵ�ժ�� */
                _remove_node(pt, (pa->use == IRWMF_INFOBLOCK_USEING) ? 0 : 1, pa);

                assert(pa->tag == IRWMF_INFOBLOCK_TAG);

                /* ��¼��ɾ����������� */
                if (rinfo) {
                  strcpy((char*)(rinfo->delpp[rinfo->delcnt++]), (const char*)(pa->ifo.irwp_name));
                }

                /* �ͷ���Ϣ�� */
                _ismf_free_IRWMF_INFOBLOCK(pa);
              }
              else {
                /* ȡ���µĲ����Ϣ */
                memcpy((void*) & (pa->ifo), (const void*)pii, sizeof(IRWP_INFO));

                /* ��¼�����������Ϣ���ַ */
                if (rinfo) {
                  ((IRWMF_INFOBLOCK**)rinfo->updpp)[rinfo->updcnt++] = pa;
                }

                FreeLibrary(hDll);
                hDll = 0;
              }
            }
          }
        }
      }
      while (FindNextFile(hFind, &file));

      _isirw_update_filtrate_str();           /* ˢ����չ�����˴� */

      if (ismf_info->plugin_num == 0) {
        result = 1;
        __leave;              /* û���ҵ�ͼ���д��� */
      }
    }
    __finally {
      if (pifb) {
        _ismf_free_IRWMF_INFOBLOCK(pifb);
      }

      if (hDll) {
        FreeLibrary(hDll);
      }

      if (hFind) {
        FindClose(hFind);
      }

      if (ismf_info->plugin_num == 0) {
        _ismf_free_IRWMF_HEADER(ismf_info);
        ismf_info = 0;

        DeleteCriticalSection(&ismf_cs);
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = 4;                       /* ϵͳ�쳣 */
  }

  return result;
}


/* ����Ϣ���в����Ƿ���ָ�����ļ������ļ�����*/
LPIRWMF_INFOBLOCK _ismf_search_at_filename(const char* ss)
{
  int         ui, ni;
  LPIRWMF_INFOBLOCK pb;

  /* �˺���ֻ�ܱ�ismf_update()�������ã���Ϣ�������Ч�����ұ��봦������״̬ */
  assert(ismf_info && (ismf_info->mark == 1) && ismf_info->locked && ss);


  /* ��¼���������� */
  for (ui = 0, pb = ismf_info->puh; ui < (int)ismf_info->uc; ui++, pb = pb->next) {
    assert(pb);

    /* �Ƚ��ļ����Ƿ���ͬ */
    if (_stricmp((const char*)ss, (const char*)pb->path) == 0) {
      return pb;
    }
  }

  /* ��δ���������� */
  for (ni = 0, pb = ismf_info->pnh; ni < (int)ismf_info->nc; ni++, pb = pb->next) {
    assert(pb);

    if (_stricmp((const char*)ss, (const char*)pb->path) == 0) {
      return pb;
    }
  }

  /* δ�ҵ�����0 */
  return 0;
}


/* ��ָ����δ�ò��ת��¼���������������*/
int CALLAGREEMENT ismf_employ(int id /*���ID����*/)
{
  int         ni;
  LPIRWMF_INFOBLOCK pb;

  /* ��Ϣ�������Ч�����ұ��봦������״̬ */
  assert(ismf_info && (ismf_info->mark == 1) && ismf_info->locked);

  /* ��δ����������id��ʶ�Ĳ�� */
  for (ni = 0, pb = ismf_info->pnh; ni < (int)ismf_info->nc; ni++, pb = pb->next) {
    assert(pb);

    if ((int)pb->ifo.plug_id == id) {
      break;
    }
  }

  /* ���û���ҵ�id��ʶ�Ĳ������˵�������ߵĳ�����ڴ��� */
  assert(pb);

  /* ��δ�����н������Ϣ��ժ�� */
  _remove_node(ismf_info, 1, pb);

  /* ������Ϣ������ */
  pb->use     = IRWMF_INFOBLOCK_USEING;     /* ����¼�ñ�� */

  pb->act     = 0;                /* ������ */
  pb->handle    = 0;                /* ������ */
  pb->fn_get_img_info = 0;              /* get_image_info�ӿ� */
  pb->fn_load_img   = 0;              /* load_image�ӿ� */
  pb->fn_save_img   = 0;              /* save_image�ӿ� */

  pb->prev      = 0;              /* ǰһ����Ϣ��ĵ�ַ */
  pb->next      = 0;              /* ��һ����Ϣ��ĵ�ַ */
  pb->pe        = ismf_info;          /* ��ͷ��ַ */

  /* ����¼����β�� */
  if (ismf_info->put == 0) {            /* ��һ�� */
    ismf_info->puh = ismf_info->put = pb;
  }
  else {                      /* ���� */
    pb->prev = ismf_info->put;
    ismf_info->put->next = pb;
    ismf_info->put = pb;
  }

  /* ¼�ò�������ۼ� */
  ismf_info->uc++;
  ismf_info->plugin_num++;

  /* ˢ����չ�����˴� */
  _isirw_update_filtrate_str();

  /* �ɹ�����0 */
  return 0;
}


/* ��ָ����¼�ò��ת��δ������ */
int CALLAGREEMENT ismf_dismissal(int id /*���ID����*/)
{
  int         ui;
  LPIRWMF_INFOBLOCK pb;

  /* ��Ϣ�������Ч�����ұ��봦������״̬ */
  assert(ismf_info && (ismf_info->mark == 1) && ismf_info->locked);

  /* ��¼����������id��ʶ�Ĳ����Ϣ�� */
  for (ui = 0, pb = ismf_info->puh; ui < (int)ismf_info->uc; ui++, pb = pb->next) {
    assert(pb);

    if ((int)pb->ifo.plug_id == id) {
      break;
    }
  }

  assert(pb);

  /* ��¼�����н������Ϣ��ժ�� */
  _remove_node(ismf_info, 0, pb);

  /* �������ѱ���������ߣ��ͷţ��� */
  if (pb->act != 0) {
    assert(pb->act == 1);             /* act�����ܴ���1����Ϊ��Ϣ���ִ�������״̬ */
    FreeLibrary(pb->handle);
  }

  /* ������Ϣ������ */
  pb->use     = IRWMF_INFOBLOCK_NOTUSE;     /* ����δ�ñ�� */

  pb->act     = 0;                /* ������ */
  pb->handle    = 0;                /* ������ */
  pb->fn_get_img_info = 0;              /* get_image_info�ӿ� */
  pb->fn_load_img   = 0;              /* load_image�ӿ� */
  pb->fn_save_img   = 0;              /* save_image�ӿ� */

  pb->prev      = 0;              /* ǰһ����Ϣ��ĵ�ַ */
  pb->next      = 0;              /* ��һ����Ϣ��ĵ�ַ */
  pb->pe        = ismf_info;          /* ��ͷ��ַ */

  /* ����δ����β�� */
  if (ismf_info->pnt == 0) {            /* ��һ�� */
    ismf_info->pnh = ismf_info->pnt = pb;
  }
  else {                      /* ���� */
    pb->prev = ismf_info->pnt;
    ismf_info->pnt->next = pb;
    ismf_info->pnt = pb;
  }

  /* δ�ò�������ۼ� */
  ismf_info->nc++;
  ismf_info->plugin_num++;

  /* ˢ����չ�����˴� */
  _isirw_update_filtrate_str();

  /* �ɹ�����0 */
  return 0;
}


/* �����Ϣ����� */
int CALLAGREEMENT ismf_unlock(LPIRWMF_HEADER pt)
{
  assert((ismf_info) && (ismf_info->mark == 1) && ismf_info->locked);
  assert(pt == ismf_info);              /* ����Ĳ���������ismf_lock()�����Ĳ��� */

  /* ����������־ */
  EnterCriticalSection(&ismf_cs);
  assert(ismf_info->locked == 1);           /* ����ǰ��Ϣ��������ѱ������� */
  ismf_info->locked--;
  LeaveCriticalSection(&ismf_cs);

  return 0;
}


/* ��ȡ�������Ŀ¼(pathָ��Ļ������ߴ������ڵ���MAX_PATH�Ķ���ĳߴ�) */
void CALLAGREEMENT ismf_get_plugin_path(char* path)
{
  assert((ismf_info) && (ismf_info->mark == 1));

  /* ��ȡ�������·�� */
  strcpy((char*)path, (const char*)ismf_info->infpath);
  *(char*)(strrchr((const char*)path, '\\')) = '\0';
}


/* ����ָ����ͼ���ļ������ض�Ӧ�Ĳ��������˾��������������ȡ�ӿڵ�ַ���������� */
unsigned long CALLAGREEMENT ismf_get_plugin_heandle(const char* path)
{
  LPIRWMF_INFOBLOCK pib;
  char        path2[MAX_PATH];
  unsigned long   result = 0;

  assert(path);
  assert((ismf_info) && (ismf_info->mark == 1) && (ismf_info->plugin_num > 0));

  __try {
    /* ����ؼ��� */
    EnterCriticalSection(&ismf_cs);

    if (ismf_info->locked > 0) {
      __leave;                  /* �����Ϣ������ */
    }

    /* �����ܴ�����ļ��Ĳ�� */
    if (_search_plugin_info_block(&pib, (const char*)path) <= 0) {
      __leave;                  /* ָ����ͼ���ļ����ܱ��κ�һ��������� */
    }

    assert(pib && (pib->use == IRWMF_INFOBLOCK_USEING));

    /* �жϸò���Ƿ��ѱ����� */
    switch (pib->act) {
    case  0:                    /* δ���� */
      /* �ϳɲ��ȫ·�� */
      strcpy((char*)path2, (const char*)ismf_info->infpath);
      *(char*)(strrchr((const char*)path2, '\\') + 1) = '\0';
      strcat((char*)path2, (const char*)pib->path);

      /* ������ */
      if ((pib->handle = LoadLibrary((LPCTSTR)path2)) == 0) {
        __leave;                /* �ļ���д���� */
      }

      /* ȡ�ýӿں�����ַ */
      if (((pib->fn_get_img_info = (IRWP_API_GET_IMG_INFO)isirw_get_api_addr(pib->handle, &(pib->ifo), get_image_info)) == 0) || \
          ((pib->fn_load_img = (IRWP_API_LOAD_IMG)isirw_get_api_addr(pib->handle, &(pib->ifo), load_image)) == 0) || \
          ((pib->fn_save_img = (IRWP_API_SAVE_IMG)isirw_get_api_addr(pib->handle, &(pib->ifo), save_image)) == 0)) {
        /* ����ļ��⵽�ƻ��������ò�� */
        FreeLibrary(pib->handle);
        pib->handle = 0;
        __leave;                /* ����⵽�ƻ� */
      }

      pib->act = 1;               /* ���ü����־ */

      /* �˴�û��break;��䣬Ŀ�����ÿ���ת������ļ������ۼӲ��� */
    default:                    /* �Ѽ��� */
      assert(pib->act >= 1);
      /* ʹ�ü�����һ��ʹ�ü�������act��Ա�����ĸ�16λ��������Ҫ�������㣩*/
      pib->act = (((pib->act >> 16) + 1) << 16) | (pib->act & 0xffff);

      /* ����ʹ�ü������ۼ� */
      pib->pcr++;

      break;
    }

    /* ���д�������ͼ������ۼ� */
    _ismf_idle_counter++;

    /* ��������˷���������������Ϣ�÷��Ϳ��д������� */
    if (_ismf_idle_counter > IRWMF_IDLE_MAX_COUNT) {
      _ismf_idle_counter = 0;

      /* ������δ����Ŀ�������ʱ�������ٷ��Ϳ��������ֹ���������*/
      if (!get_idlesse_count()) {
        add_idlesse((void*)_ismf_idle);
      }
    }

    result = (unsigned long)pib;
  }
  __finally {
    /* �뿪�ؼ��� */
    LeaveCriticalSection(&ismf_cs);
  }

  return result;                      /* �ɹ��ҵ���Ӧ�Ĳ���������Ϣ���ַ��*/
}


/* �ͷ�ismf_get_plugin_heandle()�������صĲ���������������������ɶ�ʹ�ã�*/
int CALLAGREEMENT ismf_free_plugin_heandle(unsigned long hand)
{
  LPIRWMF_INFOBLOCK pib = (LPIRWMF_INFOBLOCK)hand;
  int         result = 0;

  assert(ismf_info);
  assert((pib) && (pib->tag == IRWMF_INFOBLOCK_TAG)); /* ��ڲ�����������Ч�Ĳ����� */

  __try {
    /* ����ؼ��� */
    EnterCriticalSection(&ismf_cs);

    if (ismf_info->locked > 0) {
      result = -3;
      __leave;             /* �����Ϣ������ */
    }

    assert(pib->act > 1);               /* �����Ǽ����ʹ�õĲ�� */

    /* ʹ�ü�����һ������������ʹ������ߣ�*/
    pib->act = (((pib->act >> 16) - 1) << 16) | (pib->act & 0xffff);
  }
  __finally {
    LeaveCriticalSection(&ismf_cs);
  }

  return result;
}


/* ���ݸ����Ĳ�������ȡ������get_info�����ӿڵ�ַ */
IRWP_API_GET_IMG_INFO CALLAGREEMENT ismf_get_info_addr(unsigned long hand)
{
  LPIRWMF_INFOBLOCK   pib = (LPIRWMF_INFOBLOCK)hand;
  IRWP_API_GET_IMG_INFO result = 0;

  assert(ismf_info);
  /* ��ڲ�����������Ч�Ĳ����������ұ����Ǳ���������õ� */
  assert(pib && (pib->tag == IRWMF_INFOBLOCK_TAG) && (pib->act > 1));

  __try {
    /* ����ؼ��� */
    EnterCriticalSection(&ismf_cs);

    if (ismf_info->locked > 0) {
      result = 0;
      __leave;              /* �����Ϣ������ */
    }

    assert(pib->fn_get_img_info);

    result = pib->fn_get_img_info;
  }
  __finally {
    LeaveCriticalSection(&ismf_cs);
  }

  return result;
}


/* ���ݸ����Ĳ�������ȡ������load�����ӿڵ�ַ */
IRWP_API_LOAD_IMG CALLAGREEMENT ismf_get_load_addr(unsigned long hand)
{
  LPIRWMF_INFOBLOCK   pib = (LPIRWMF_INFOBLOCK)hand;
  IRWP_API_LOAD_IMG   result = 0;

  assert(ismf_info);
  /* ��ڲ�����������Ч�Ĳ����������ұ����Ǳ���������õ� */
  assert(pib && (pib->tag == IRWMF_INFOBLOCK_TAG) && (pib->act > 1));

  __try {
    /* ����ؼ��� */
    EnterCriticalSection(&ismf_cs);

    if (ismf_info->locked > 0) {
      result = 0;
      __leave;              /* �����Ϣ������ */
    }

    assert(pib->fn_load_img);

    result = pib->fn_load_img;
  }
  __finally {
    LeaveCriticalSection(&ismf_cs);
  }

  return result;
}


/* ���ݸ����Ĳ�������ȡ������save�����ӿڵ�ַ */
IRWP_API_SAVE_IMG CALLAGREEMENT ismf_get_save_addr(unsigned long hand)
{
  LPIRWMF_INFOBLOCK   pib = (LPIRWMF_INFOBLOCK)hand;
  IRWP_API_SAVE_IMG   result = 0;

  assert(ismf_info);
  /* ��ڲ�����������Ч�Ĳ����������ұ����Ǳ���������õ� */
  assert(pib && (pib->tag == IRWMF_INFOBLOCK_TAG) && (pib->act > 1));

  __try {
    /* ����ؼ��� */
    EnterCriticalSection(&ismf_cs);

    if (ismf_info->locked > 0) {
      result = 0;
      __leave;              /* �����Ϣ������ */
    }

    result = pib->fn_save_img;
  }
  __finally {
    LeaveCriticalSection(&ismf_cs);
  }

  return result;
}


/* �رղ����Ϣ�� */
int CALLAGREEMENT ismf_close()
{
  FILE*        fp = 0;
  LPIRWMF_INFOBLOCK pii;
  int         result = 0, i;

  __try {
    __try {
      /* ���û���ڴ�INI��ֱ�ӷ��� */
      if (ismf_info == 0) {
        result = 1;
        __leave;
      }

      /* �ڹر�ϵͳǰ�������Ϣ���ܴ�������״̬ */
      assert(ismf_info->locked == 0);
      assert(ismf_info->mark == 1);

      /* ����������־���رչ��̽�������״̬�½��� */
      EnterCriticalSection(&ismf_cs);
      ismf_info->locked++;

      /* ����в�����ڼ���״̬����ʹ�ò������ */
      for (i = 0, pii = ismf_info->puh; i < (int)ismf_info->uc; i++, pii = pii->next) {
        assert(pii);

        /* �ȴ���ֱ���ò��������Ϣ��ʹ�� */
        while (pii->act > 1) {
          Sleep(ISIRW_THREAD_IDLE);
        }

        /* �������ѱ���������ߣ��ͷţ��� */
        if (pii->act == 1) {
          FreeLibrary(pii->handle);

          pii->handle          = 0;
          pii->fn_get_img_info = 0;
          pii->fn_load_img     = 0;
          pii->fn_save_img     = 0;

          pii->act = 0;
        }
      }

      /* ���ļ�INI */
      if ((fp = fopen((const char*)ismf_info->infpath, "wb")) == 0) {
        result = -1;
        __leave;         /* ���ļ��� */
      }

      /* д���ͷ */
      if (fwrite((const void*)ismf_info, sizeof(IRWMF_HEADER), 1, fp) == 0) {
        result = -2;
        __leave;         /* �ļ�д���� */
      }

      /* д��¼���� */
      for (i = 0, pii = ismf_info->puh; i < ismf_info->uc; i++, pii = pii->next) {
        assert(pii);

        /* ˢ�µ������ȼ� */
        pii->pr += (pii->pcr > 0) ? 1 : 0;

        /* д��¼���� */
        if (fwrite((const void*)pii, sizeof(IRWMF_INFOBLOCK), 1, fp) == 0) {
          result = -2;
          __leave;       /* �ļ�д���� */
        }
      }

      assert(pii == 0);

      /* д��δ���� */
      for (i = 0, pii = ismf_info->pnh; i < ismf_info->nc; i++, pii = pii->next) {
        assert(pii);

        /* д��δ���� */
        if (fwrite((const void*)pii, sizeof(IRWMF_INFOBLOCK), 1, fp) == 0) {
          result = -2;
          __leave;       /* �ļ�д���� */
        }
      }
    }
    __finally {
      if (fp) {
        fclose(fp);
      }

      /* �ͷ���Ϣ�� */
      _ismf_free_IRWMF_HEADER(ismf_info);
      ismf_info = 0;

      /* ɾ����Ϣ����ʹؼ��� */
      LeaveCriticalSection(&ismf_cs);
      DeleteCriticalSection(&ismf_cs);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = -3;                  /* ϵͳ�쳣 */
  }

  return result;
}


/***********************************************************************/

/* ���䲢��ʼ��һ��������� */
LPIRWMF_INFOBLOCK _ismf_alloc_IRWMF_INFOBLOCK()
{
  LPIRWMF_INFOBLOCK pt;

  if ((pt = (LPIRWMF_INFOBLOCK)im_malloc(sizeof(IRWMF_INFOBLOCK))) == 0) {
    return 0;
  }

  memset((void*)pt, 0, sizeof(IRWMF_INFOBLOCK));

  pt->tag = IRWMF_INFOBLOCK_TAG;

  return pt;
}


/* �ͷ�һ��������� */
void _ismf_free_IRWMF_INFOBLOCK(LPIRWMF_INFOBLOCK pt)
{
  assert(pt && (pt->tag == IRWMF_INFOBLOCK_TAG));

  im_free(pt);
}


/* ��������Ϣ�� */
LPIRWMF_HEADER _ismf_alloc_IRWMF_HEADER()
{
  LPIRWMF_HEADER  pt;

  if ((pt = (LPIRWMF_HEADER)im_malloc(sizeof(IRWMF_HEADER))) == 0) {
    return 0;
  }

  memset((void*)pt, 0, sizeof(IRWMF_HEADER));

  pt->tag = IRWMF_HEADER_TAG;

  return pt;
}


/* �ͷŲ����Ϣ�� */
void _ismf_free_IRWMF_HEADER(LPIRWMF_HEADER pt)
{
  int         i;
  LPIRWMF_INFOBLOCK p;

  assert(pt && (pt->tag == IRWMF_HEADER_TAG));

  /* �ͷ�¼���� */
  for (i = 0; i < pt->uc; i++) {
    p = pt->puh;
    pt->puh = pt->puh->next;

    assert(p);

    p->tag = IRWMF_INFOBLOCK_TAG;         /* ʹ���ִ���Ŀ�Ҳ�ܱ������ͷ� */

    _ismf_free_IRWMF_INFOBLOCK(p);
  }

  assert(pt->puh == 0);

  /* �ͷ�δ¼���� */
  for (i = 0; i < (int)pt->nc; i++) {
    p = pt->pnh;
    pt->pnh = pt->pnh->next;

    assert(p);

    p->tag = IRWMF_INFOBLOCK_TAG;

    _ismf_free_IRWMF_INFOBLOCK(p);
  }

  assert(pt->pnh == 0);

  /* �ͷű�ͷ */
  im_free(pt);
}


/***********************************************************************/


/* �����µ��ڴ�INIӳ�� */
int _create_mini(char* path_str/* ���·�� */, LPIRWMF_HEADER pt/* ��ͷ�ṹ */)
{
  LPIRWP_INFO     pii = 0;
  LPIRWMF_INFOBLOCK pifb = 0;
  char        path[MAX_PATH];
  char        path2[MAX_PATH];
  int         result = 0, i;

  WIN32_FIND_DATA   file;
  HANDLE        hFind = 0;
  HMODULE       hDll  = 0;


  /* �����߱������·�� */
  assert(strlen(path_str) > 1);
  assert(pt);

  /* ���������� */
  strcpy((char*)path, (const char*)path_str);
  strcat((char*)path, (const char*)"*.dll");

  __try {
    /* ������� */
    hFind = FindFirstFile((LPCTSTR)path, &file);

    if (INVALID_HANDLE_VALUE == hFind) {
      result = 1;
      __leave;            /* û���ҵ�ͼ���д��� */
    }

    do {
      /* �ϳɲ��ȫ·�� */
      strcpy((char*)path2, (const char*)path_str);
      strcat((char*)path2, (const char*)(file.cFileName));

      /* ������ */
      if ((hDll = LoadLibrary((LPCTSTR)path2)) == 0) {
        result = 2;
        __leave;          /* ���������� */
      }

      /* �жϴ�DLL�Ƿ���ͼ���д��� */
      if ((pii = isirw_get_info(isirw_test_plugin(hDll))) == 0) {
        FreeLibrary(hDll);
        hDll = 0;
        continue;               /* ��DLL���ǲ�������� */
      }

      /* ������ʱ����Ϣ�黺���� */
      if ((pifb = _ismf_alloc_IRWMF_INFOBLOCK()) == 0) {
        result = 3;
        __leave;          /* �ڴ治�� */
      }

      /* ��д��Ϣ�� */
      pifb->pr    = IRWMF_INFOBLOCK_DEFPIR; /* ����������ȼ� */
      pifb->use   = IRWMF_INFOBLOCK_USEING; /* ¼�ñ�ǣ�ȱʡΪ��¼�ã�*/
      pifb->pilen   = file.nFileSizeLow;    /* ����ļ����� */
      /* ����ļ�����ʱ�� */
      pifb->ctimeh  = file.ftCreationTime.dwHighDateTime;
      pifb->ctimel  = file.ftCreationTime.dwLowDateTime;
      /* ����ļ������޸�ʱ�� */
      pifb->mtimeh  = file.ftLastWriteTime.dwHighDateTime;
      pifb->mtimel  = file.ftLastWriteTime.dwLowDateTime;

      /* ����ļ��� */
      strcpy((char*)pifb->path, (const char*)file.cFileName);
      /* �����Ϣ�� */
      memcpy((void*) & (pifb->ifo), (const void*)pii, sizeof(IRWP_INFO));

      pifb->act   = 0;            /* �����ǣ�ȱʡΪδ���*/
      pifb->handle  = 0;            /* ������ */

      pifb->fn_get_img_info = 0;        /* get_image_info�ӿ� */
      pifb->fn_load_img   = 0;        /* load_image�ӿ� */
      pifb->fn_save_img   = 0;        /* save_image�ӿ� */

      pifb->prev    = 0;            /* ǰһ����Ϣ��ĵ�ַ */
      pifb->next    = 0;            /* ��һ����Ϣ��ĵ�ַ */
      pifb->pe    = pt;           /* ��ͷ��ַ */

      /* �����ͷ�ṹ�е�¼���� */
      if (pt->put == 0) {           /* ��һ�� */
        pt->puh = pt->put = pifb;
      }
      else {                  /* ���� */
        pifb->prev = pt->put;
        pt->put->next = pifb;
        pt->put = pifb;
      }

      pt->uc++;

      /* �ͷŲ��DLL */
      FreeLibrary(hDll);

      hDll = 0;
      pifb = 0;

    }
    while (FindNextFile(hFind, &file));

    FindClose(hFind);

    hFind = 0;

    if (pt->uc == 0) {
      result = 1;
      __leave;            /* ��ͼ���д��� */
    }

    pt->plugin_num = pt->uc;            /* ����ܸ��� */

    pt->pnh     = 0;              /* δ����ͷ��ַ */
    pt->pnt     = 0;              /* δ����β��ַ */
    pt->nc      = 0;              /* δ��������ĸ��� */

    strcpy((char*)pt->infpath, (const char*)path_str); /* ��Ϣ�ļ�ȫ·�� */
    strcat((char*)pt->infpath, (const char*)IRWMF_FILENAME);

    pt->locked    = 0;              /* ��ʼ��������־ */
    pt->build   = IRWMF_HEADER_BUD_NULL;    /* ����Ϊ��Ч���� */
    pt->mark    = 0;              /* ��ͷ��Ϣ��Ч�����ʱ��Ϊ0 */
  }
  __finally {
    if (pifb) {
      assert(pifb->tag == IRWMF_INFOBLOCK_TAG);
      _ismf_free_IRWMF_INFOBLOCK(pifb);
    }

    if (hDll) {
      FreeLibrary(hDll);
    }

    if (hFind) {
      FindClose(hFind);
    }

    if (AbnormalTermination() || (result != 0)) {
      /* �ͷ�¼���� */
      for (i = 0; i < pt->uc; i++) {
        pifb = pt->puh;
        pt->puh = pt->puh->next;

        assert(pifb);
        assert(pifb->tag == IRWMF_INFOBLOCK_TAG);

        _ismf_free_IRWMF_INFOBLOCK(pifb);
      }

      assert(pt->puh == 0);

      pt->put = pt->puh = 0;
      pt->uc = 0;
      pt->plugin_num = 0;
    }
  }

  return result;
}


/* ������Ϣ�����γ��ڴ�INIӳ�� */
int _load_mini(FILE* fp/* ���ini�ļ���� */, LPIRWMF_HEADER pt)
{
  LPIRWMF_INFOBLOCK pifb = 0;
  int         result = 0, i, uc, nc, pln;

  assert(fp && pt);

  __try {
    if (fseek(fp, 0, SEEK_SET)) {
      result = 1;
      __leave;          /* �ļ��� */
    }

    /* ����ͷ�ṹ */
    if (fread((void*)pt, sizeof(IRWMF_HEADER), 1, fp) == 0) {
      result = 1;
      __leave;
    }

    /* ��ʼ�����ֲ��� */
    pt->puh = pt->put = 0;
    pt->pnh = pt->pnt = 0;

    pt->locked = 0;               /* ��ʼ��������־ */
    pt->build  = IRWMF_HEADER_BUD_NULL;     /* ����Ϊ��Ч���� */
    pt->mark   = 0;               /* ��ͷ��Ϣ��Ч�����ʱ��Ϊ0 */

    /* ��ȡ���������Ϣ */
    uc  = pt->uc;
    nc  = pt->nc;
    pln = pt->plugin_num;

    pt->uc = 0;
    pt->nc = 0;
    pt->plugin_num = 0;

    /* ������Ϣ�ĺϷ��� */
    if ((pt->tag != IRWMF_HEADER_TAG) || (pln <= 0)) {
      result = 2;
      __leave;          /* �ļ������⵽�ƻ� */
    }

    /* ��ȡ¼���� */
    for (i = 0; i < uc; i++) {
      if ((pifb = _ismf_alloc_IRWMF_INFOBLOCK()) == 0) {
        result = 3;
        __leave;        /* �ڴ治��*/
      }

      /* ����һ����Ϣ�� */
      if (fread((void*)pifb, sizeof(IRWMF_INFOBLOCK), 1, fp) == 0) {
        result = 1;
        __leave;        /* ���ļ��� */
      }

      /* �����Ϣ�����ݵĺϷ��� */
      if (pifb->tag != IRWMF_INFOBLOCK_TAG) {
        result = 2;
        __leave;
      }

      pifb->pcr     = 0;
      pifb->use     = IRWMF_INFOBLOCK_USEING;
      pifb->act     = 0;        /* ������ */
      pifb->handle    = 0;        /* ������ */

      pifb->fn_get_img_info = 0;        /* get_image_info�ӿ� */
      pifb->fn_load_img = 0;        /* load_image�ӿ� */
      pifb->fn_save_img = 0;        /* save_image�ӿ� */

      pifb->prev = pifb->next = 0;      /* ��ʼ�������ַ */

      /* ����¼���� */
      if (pt->put == 0) {         /* ��һ����Ϣ�� */
        pt->puh = pt->put = pifb;
      }
      else {                /* ������Ϣ�� */
        pifb->prev = pt->put;
        pt->put->next = pifb;
        pt->put = pifb;
      }

      pifb->pe = pt;              /* ��ͷ��ַ */
      pt->uc++;
      pt->plugin_num++;           /* ��������ۼ� */

      pifb = 0;
    }

    /* ��ȡδ���� */
    for (i = 0; i < nc; i++) {
      if ((pifb = _ismf_alloc_IRWMF_INFOBLOCK()) == 0) {
        result = 3;
        __leave;        /* �ڴ治��*/
      }

      /* ����һ����Ϣ�� */
      if (fread((void*)pifb, sizeof(IRWMF_INFOBLOCK), 1, fp) == 0) {
        result = 1;
        __leave;        /* ���ļ��� */
      }

      /* �����Ϣ�����ݵĺϷ��� */
      if (pifb->tag != IRWMF_INFOBLOCK_TAG) {
        result = 2;
        __leave;
      }

      pifb->pcr     = 0;
      pifb->use     = IRWMF_INFOBLOCK_NOTUSE;
      pifb->act     = 0;        /* ������ */
      pifb->handle    = 0;        /* ������ */

      pifb->fn_get_img_info = 0;        /* get_image_info�ӿ� */
      pifb->fn_load_img = 0;        /* load_image�ӿ� */
      pifb->fn_save_img = 0;        /* save_image�ӿ� */

      pifb->prev = pifb->next = 0;      /* ��ʼ�������ַ */

      /* ����δ���� */
      if (pt->pnt == 0) {         /* ��һ����Ϣ�� */
        pt->pnh = pt->pnt = pifb;
      }
      else {                /* ������Ϣ�� */
        pifb->prev = pt->pnt;
        pt->pnt->next = pifb;
        pt->pnt = pifb;
      }

      pifb->pe = pt;              /* ��ͷ��ַ */
      pt->nc++;
      pt->plugin_num++;           /* ��������ۼ� */

      pifb = 0;
    }
  }
  __finally {
    if (pifb) {
      pifb->tag = IRWMF_INFOBLOCK_TAG;    /* ʹ���ִ���Ŀ�Ҳ�ܱ������ͷ� */
      _ismf_free_IRWMF_INFOBLOCK(pifb);
    }

    if ((result != 0) || AbnormalTermination()) {
      /* �ͷ�¼���� */
      for (i = 0; i < pt->uc; i++) {
        pifb = pt->puh;
        pt->puh = pt->puh->next;

        assert(pifb);
        assert(pifb->tag == IRWMF_INFOBLOCK_TAG);

        _ismf_free_IRWMF_INFOBLOCK(pifb);
      }

      assert(pt->puh == 0);

      /* �ͷ�δ¼���� */
      for (i = 0; i < (int)pt->nc; i++) {
        pifb = pt->pnh;
        pt->pnh = pt->pnh->next;

        assert(pifb);
        assert(pifb->tag == IRWMF_INFOBLOCK_TAG);

        _ismf_free_IRWMF_INFOBLOCK(pifb);
      }

      assert(pt->pnh == 0);

      pt->put = pt->puh = 0;
      pt->uc  = 0;
      pt->pnt = pt->pnh = 0;
      pt->nc  = 0;

      pt->plugin_num = 0;
    }
  }

  return result;
}


/* У���ڴ�ini����ʵ�ԣ����б�Ҫ������ڴ�ini */
void _verify_mini(LPIRWMF_HEADER phead, char* path, LPUPDATAINFO rinfo)
{
  LPIRWMF_INFOBLOCK pifb;
  LPIRWMF_INFOBLOCK ptmp;
  LPIRWP_INFO     pii = 0;

  HMODULE       hDll  = 0;
  char        path2[MAX_PATH];
  int         i;

  WIN32_FILE_ATTRIBUTE_DATA wfa;


  assert(phead && path);

  __try {
    /* ���¼����ͷ���ַ */
    pifb = phead->puh;

    while (pifb) {
      /* �ϳɴ�У�����ļ���ȫ·�� */
      strcpy((char*)path2, (const char*)path);
      strcat((char*)path2, (const char*)pifb->path);

      /* ��ȡ����ļ�����Ϣ */
      if (GetFileAttributesEx((LPCSTR)path2, GetFileExInfoStandard, (LPVOID)&wfa) == 0) {
        if (pifb->act) {
          /* ����ò���Ǽ���ģ�����ʹ������ */
          FreeLibrary(pifb->handle);
          pifb->handle = 0;
          pifb->act = 0;
          pifb->fn_get_img_info = 0;
          pifb->fn_load_img     = 0;
          pifb->fn_save_img     = 0;
        }

        /* ��Ӧ�Ĳ���ļ�û���ҵ������ܱ��û�˽��ɾ�� */
        ptmp = pifb;
        pifb = pifb->next;

        /* ���ڵ��¼������ժ�� */
        _remove_node(phead, 0, ptmp);

        assert(ptmp->tag == IRWMF_INFOBLOCK_TAG);

        /* ��¼��ɾ����������� */
        if (rinfo) {
          strcpy((char*)(rinfo->delpp[rinfo->delcnt++]), (const char*)(ptmp->ifo.irwp_name));
        }

        /* �ͷ���Ϣ�� */
        _ismf_free_IRWMF_INFOBLOCK(ptmp);

        continue;
      }
      else {
        i = 0;

        /* �������ļ����Ķ�������������ˢ����Ϣ */
        if (pifb->pilen != wfa.nFileSizeLow) {
          i++;
          pifb->pilen = wfa.nFileSizeLow;
        }

        if ((pifb->ctimel != wfa.ftCreationTime.dwLowDateTime) || \
            (pifb->ctimeh != wfa.ftCreationTime.dwHighDateTime)) {
          i++;
          pifb->ctimel = wfa.ftCreationTime.dwLowDateTime;
          pifb->ctimeh = wfa.ftCreationTime.dwHighDateTime;
        }

        if ((pifb->mtimel != wfa.ftLastWriteTime.dwLowDateTime) || \
            (pifb->mtimeh != wfa.ftLastWriteTime.dwHighDateTime)) {
          i++;
          pifb->mtimel = wfa.ftLastWriteTime.dwLowDateTime;
          pifb->mtimeh = wfa.ftLastWriteTime.dwHighDateTime;
        }

        if (i) {
          /* �������ϵͳ�Ĳ���ļ��ǲ����ܱ��޸ĵ� */
          assert(pifb->act == 0);

          /* �жϴ�DLL�Ƿ���ͼ���д��� */
          if (((hDll = LoadLibrary((LPCTSTR)path2)) == 0) || \
              ((pii = isirw_get_info(isirw_test_plugin(hDll))) == 0)) {
            /* ����ļ��⵽�ƻ��������ò�� */
            if (hDll) {
              FreeLibrary(hDll);
              hDll = 0;
            }

            ptmp = pifb;
            pifb = pifb->next;

            /* ���ڵ��¼������ժ�� */
            _remove_node(phead, 0, ptmp);

            assert(ptmp->tag == IRWMF_INFOBLOCK_TAG);

            /* ��¼��ɾ����������� */
            if (rinfo) {
              strcpy((char*)(rinfo->delpp[rinfo->delcnt++]), (const char*)(ptmp->ifo.irwp_name));
            }

            /* �ͷ���Ϣ�� */
            _ismf_free_IRWMF_INFOBLOCK(ptmp);

            continue;
          }
          else {
            /* ȡ���µĲ����Ϣ */
            memcpy((void*) & (pifb->ifo), (const void*)pii, sizeof(IRWP_INFO));

            /* ��¼�����������Ϣ���ַ */
            if (rinfo) {
              ((IRWMF_INFOBLOCK**)rinfo->updpp)[rinfo->updcnt++] = pifb;
            }

            FreeLibrary(hDll);
            hDll = 0;
          }
        }
      }

      pifb = pifb->next;
    }


    /* ���δ����ͷ���ַ */
    pifb = phead->pnh;

    while (pifb) {
      /* �ϳɴ�У�����ļ���ȫ·�� */
      strcpy((char*)path2, (const char*)path);
      strcat((char*)path2, (const char*)pifb->path);

      /* ��ȡ����ļ�����Ϣ */
      if (GetFileAttributesEx((LPCSTR)path2, GetFileExInfoStandard, (LPVOID)&wfa) == 0) {
        /* δ�����еĶ�Ӧ����ļ��ǲ����ܱ������ */
        assert(pifb->act == 0);

        /* ��Ӧ�Ĳ���ļ�û���ҵ������ܱ��û�˽��ɾ�� */
        ptmp = pifb;
        pifb = pifb->next;

        /* ���ڵ��δ������ժ�� */
        _remove_node(phead, 1, ptmp);

        assert(ptmp->tag == IRWMF_INFOBLOCK_TAG);

        /* ��¼��ɾ����������� */
        if (rinfo) {
          strcpy((char*)(rinfo->delpp[rinfo->delcnt++]), (const char*)(ptmp->ifo.irwp_name));
        }

        /* �ͷ���Ϣ�� */
        _ismf_free_IRWMF_INFOBLOCK(ptmp);

        continue;
      }
      else {
        i = 0;

        /* �������ļ����Ķ�������������ˢ����Ϣ */
        if (pifb->pilen != wfa.nFileSizeLow) {
          i++;
          pifb->pilen = wfa.nFileSizeLow;
        }

        if ((pifb->ctimel != wfa.ftCreationTime.dwLowDateTime) || \
            (pifb->ctimeh != wfa.ftCreationTime.dwHighDateTime)) {
          i++;
          pifb->ctimel = wfa.ftCreationTime.dwLowDateTime;
          pifb->ctimeh = wfa.ftCreationTime.dwHighDateTime;
        }

        if ((pifb->mtimel != wfa.ftLastWriteTime.dwLowDateTime) || \
            (pifb->mtimeh != wfa.ftLastWriteTime.dwHighDateTime)) {
          i++;
          pifb->mtimel = wfa.ftLastWriteTime.dwLowDateTime;
          pifb->mtimeh = wfa.ftLastWriteTime.dwHighDateTime;
        }

        if (i) {
          /* δ�����еĶ�Ӧ����ļ��ǲ����ܱ������ */
          assert(pifb->act == 0);

          /* �жϴ�DLL�Ƿ���ͼ���д��� */
          if (((hDll = LoadLibrary((LPCTSTR)path2)) == 0) || \
              ((pii = isirw_get_info(isirw_test_plugin(hDll))) == 0)) {
            /* ����ļ��⵽�ƻ��������ò�� */
            if (hDll) {
              FreeLibrary(hDll);
              hDll = 0;
            }

            ptmp = pifb;
            pifb = pifb->next;

            /* ���ڵ��δ������ժ�� */
            _remove_node(phead, 1, ptmp);

            assert(ptmp->tag == IRWMF_INFOBLOCK_TAG);

            /* ��¼��ɾ����������� */
            if (rinfo) {
              strcpy((char*)(rinfo->delpp[rinfo->delcnt++]), (const char*)(ptmp->ifo.irwp_name));
            }

            /* �ͷ���Ϣ�� */
            _ismf_free_IRWMF_INFOBLOCK(ptmp);

            continue;
          }
          else {
            /* ȡ���µĲ����Ϣ */
            memcpy((void*) & (pifb->ifo), (const void*)pii, sizeof(IRWP_INFO));

            /* ��¼�����������Ϣ���ַ */
            if (rinfo) {
              ((IRWMF_INFOBLOCK**)rinfo->updpp)[rinfo->updcnt++] = pifb;
            }

            FreeLibrary(hDll);
            hDll = 0;
          }
        }
      }

      pifb = pifb->next;
    }
  }
  __finally {
    if (hDll) {
      FreeLibrary(hDll);
    }
  }
}


/* ժ��¼������δ�����еĽڵ㣨�����������ͷŸýڵ㣩 */
LPIRWMF_INFOBLOCK _remove_node(LPIRWMF_HEADER phead, int type, LPIRWMF_INFOBLOCK pnode)
{
  assert(phead && pnode);

  if (type == 0) {                  /* ¼�ýڵ� */
    if (phead->puh == pnode) {
      phead->puh = pnode->next;
    }

    if (phead->put == pnode) {
      phead->put = pnode->prev;
    }

    if (pnode->prev) {
      pnode->prev->next = pnode->next;
    }

    if (pnode->next) {
      pnode->next->prev = pnode->prev;
    }

    phead->uc --;                 /* �ڵ������1 */
    phead->plugin_num --;
  }
  else {                      /* δ�ýڵ� */
    if (phead->pnh == pnode) {
      phead->pnh = pnode->next;
    }

    if (phead->pnt == pnode) {
      phead->pnt = pnode->prev;
    }

    if (pnode->prev) {
      pnode->prev->next = pnode->next;
    }

    if (pnode->next) {
      pnode->next->prev = pnode->prev;
    }

    phead->nc --;                 /* �ڵ������1 */
    phead->plugin_num --;
  }

  return pnode;
}


/* �����ṩ���ļ�·������������Ӧ�������Ϣ���ַ */
int _search_plugin_info_block(
    LPIRWMF_INFOBLOCK* ppblock/* out */,
    const char* path/* in */)
{
  LPIRWMF_INFOBLOCK ptmp;
  int         i, l, j;
  char        ext[_MAX_EXT];
  const char*      p = path;

  assert(ppblock && path);
  assert(ismf_info);

  /* ���ñ�����ʱ����Ϣ���ܴ�������״̬ */
  if (ismf_info->locked > 0) {
    assert(0);
    *ppblock = 0;
    return 0;
  }

  l = strlen((const char*)path);

  /* ������ļ���չ���Ļ���lֵ�ؽ����ڵ���3(x.x)������ֱ�ӷ��� */
  if (l < 3) {
    *ppblock = 0;
    return 0;
  }

  ext[0] = '\0';

  /* ��ȡ�ļ���չ�� */
  for (i = l - 1; i >= 0; i--) {
    if (*(p + i) == '.') {
      strcpy((char*)ext, (const char*)(p + i + 1)); /* ������չ����������dot��*/
      break;
    }
  }

  l = strlen((const char*)ext);

  /* ���û���ҵ���չ������չ����������ֱ�ӷ��� */
  if ((l == 0) || (l >= IDI_CURRENCY_NAME_SIZE)) {
    *ppblock = 0;
    return 0;
  }


  /* ������¼���� */
  for (i = 0, ptmp = ismf_info->puh; i < ismf_info->uc; i++, ptmp = ptmp->next) {
    assert(ptmp);

    /* �Ƚϲ����ͨ����չ�� */
    if (_stricmp((const char*)ext, (const char*)ptmp->ifo.irwp_desc_info.idi_currency_name) == 0) {
      *ppblock = ptmp;
      return 1;                 /* ¼�������ҵ���ز�� */
    }

    /* �Ƚϲ����������չ�� */
    for (j = 0; j < (int)ptmp->ifo.irwp_desc_info.idi_synonym_count; j++) {
      if (_stricmp((const char*)ext, (const char*)ptmp->ifo.irwp_desc_info.idi_synonym[j]) == 0) {
        *ppblock = ptmp;
        return 1;
      }
    }
  }

  /* ������δ���� */
  for (i = 0, ptmp = ismf_info->pnh; i < ismf_info->nc; i++, ptmp = ptmp->next) {
    assert(ptmp);

    /* �Ƚϲ����ͨ����չ�� */
    if (_stricmp((const char*)ext, (const char*)ptmp->ifo.irwp_desc_info.idi_currency_name) == 0) {
      *ppblock = ptmp;
      return -1;                  /* δ�������ҵ���ز�� */
    }

    /* �Ƚϲ����������չ�� */
    for (j = 0; j < (int)ptmp->ifo.irwp_desc_info.idi_synonym_count; j++) {
      if (_stricmp((const char*)ext, (const char*)ptmp->ifo.irwp_desc_info.idi_synonym[j]) == 0) {
        *ppblock = ptmp;
        return -1;
      }
    }
  }

  *ppblock = 0;

  return 0;                     /* δ���ҵ���Ӧ��� */
}


/* ˢ��OPEN�Ի�����չ�����˴���ͨ����չ�����˴� */
int _isirw_update_filtrate_str()
{
  static char     strbuf[ISIRW_OPENSTR_SIZE];
  char        buff[IDI_CURRENCY_NAME_SIZE];
  LPIRWMF_INFOBLOCK ptmp;
  int         i, j, count = 0;

  assert(ismf_info);

  /* ��չ��˴������� */
  memset((void*)_ismf_openstr, 0, ISIRW_OPENSTR_SIZE);
  memset((void*)_ismf_shellstr, 0, ISIRW_OPENSTR_SIZE);
  memset((void*)strbuf, 0, ISIRW_OPENSTR_SIZE);

  /* ֻ����¼�ò������չ���� */
  if (ismf_info->uc) {
    strcpy((char*)_ismf_openstr, (const char*)"����ͼ���ļ� (");

    /* ֻ��ȡ¼�����и��������չ����Ϣ */
    for (i = 0, ptmp = ismf_info->puh; i < ismf_info->uc; i++, ptmp = ptmp->next) {
      assert(ptmp);

      strcpy((char*)buff, (const char*)ptmp->ifo.irwp_desc_info.idi_currency_name);
      _strlwr((char*)buff);         /* ת��ΪСд */

      /* ���ͨ����չ���� */
      strcat((char*)_ismf_shellstr, (const char*)"*.");
      strcat((char*)_ismf_shellstr, (const char*)buff);
      strcat((char*)_ismf_shellstr, (const char*)",");

      /* ���OPEN�Ի�����չ�� */
      strcat((char*)strbuf, (const char*)"*.");
      strcat((char*)strbuf, (const char*)buff);
      strcat((char*)strbuf, (const char*)";");

      /* �ۼ���չ������ */
      count++;

      /* ����������չ�� */
      for (j = 0; j < (int)ptmp->ifo.irwp_desc_info.idi_synonym_count; j++) {
        strcpy((char*)buff, (const char*)ptmp->ifo.irwp_desc_info.idi_synonym[j]);
        _strlwr((char*)buff);

        strcat((char*)_ismf_shellstr, (const char*)"*.");
        strcat((char*)_ismf_shellstr, (const char*)buff);
        strcat((char*)_ismf_shellstr, (const char*)",");

        strcat((char*)strbuf, (const char*)"*.");
        strcat((char*)strbuf, (const char*)buff);
        strcat((char*)strbuf, (const char*)";");

        count++;
      }
    }

    /* ȥ����������ĩβ��';'��','����ͨ�ô����˺ϳ���ϣ�*/
    strbuf[strlen((const char*)strbuf) - 1] = '\0';
    _ismf_shellstr[strlen((const char*)_ismf_shellstr) - 1] = '\0';

    /* ������дOPEN�� */
    strcat((char*)_ismf_openstr, (const char*)strbuf);
    strcat((char*)_ismf_openstr, (const char*)")|");
    strcat((char*)_ismf_openstr, (const char*)strbuf);
    strcat((char*)_ismf_openstr, (const char*)"|");

    /* ��д���������չ����Ϣ */
    for (i = 0, ptmp = ismf_info->puh; i < ismf_info->uc; i++, ptmp = ptmp->next) {
      strcpy((char*)buff, (const char*)ptmp->ifo.irwp_desc_info.idi_currency_name);
      _strlwr((char*)buff);           /* ת��ΪСд */

      strcpy((char*)strbuf, (const char*)"*.");
      strcat((char*)strbuf, (const char*)buff);
      strcat((char*)strbuf, (const char*)";");

      strcat((char*)_ismf_openstr, (const char*)buff);  /* ��ʽ��"jpeg�ļ� (*.jpeg;*.jpg)|*.jpeg;*.jpg|" */
      strcat((char*)_ismf_openstr, (const char*)"�ļ� (*.");
      strcat((char*)_ismf_openstr, (const char*)buff);
      strcat((char*)_ismf_openstr, (const char*)";");

      /* �ϲ�������չ�� */
      for (j = 0; j < (int)ptmp->ifo.irwp_desc_info.idi_synonym_count; j++) {
        strcpy((char*)buff, (const char*)ptmp->ifo.irwp_desc_info.idi_synonym[j]);
        _strlwr((char*)buff);

        strcat((char*)strbuf, (const char*)"*.");
        strcat((char*)strbuf, (const char*)buff);
        strcat((char*)strbuf, (const char*)";");

        strcat((char*)_ismf_openstr, (const char*)"*.");
        strcat((char*)_ismf_openstr, (const char*)buff);
        strcat((char*)_ismf_openstr, (const char*)";");
      }

      /* ȥ��ĩβ�ġ�;���� */
      strbuf[strlen((const char*)strbuf) - 1] = '\0';
      _ismf_openstr[strlen((const char*)_ismf_openstr) - 1] = '\0';

      strcat((char*)_ismf_openstr, (const char*)")|");
      strcat((char*)_ismf_openstr, (const char*)strbuf);
      strcat((char*)_ismf_openstr, (const char*)"|");
    }

    /* ��д�����С��ļ�����չ�� */
    strcat((char*)_ismf_openstr, (const char*)"�����ļ� (*.*)|*.*||");

    /* ��β˫NULL */
    _ismf_openstr[strlen(_ismf_openstr) + 1] = '\0';
  }

  return count;                   /* ������չ���ܸ��� */
}


/* ��ȡOPEN�Ի�����չ�����˴� */
char* CALLAGREEMENT _ismf_get_openstr()
{
  return (char*)_ismf_openstr;
}


/* ��ȡͨ�ù��˴� */
char* CALLAGREEMENT _ismf_get_shellstr()
{
  return (char*)_ismf_shellstr;
}


/* ���д����� */
void _ismf_idle()
{
  static int      mark = 1;
  int         i, j;
  LPIRWMF_INFOBLOCK pstr;
  LPIRWMF_INFOBLOCK ptmp;
  LPIRWMF_INFOBLOCK phig;

  assert(ismf_info && (ismf_info->mark == 1));

  /* ����״̬�򲻽��н�һ���Ĵ��� */
  if (ismf_info->locked) {
    return;
  }

  pstr = ptmp = phig = 0;

  /* Ϊ���ٿ��к�����ִ��ʱ�䣬�ֱ�Ķ�¼������δ������������ */
  if (mark % 2) {
    /* ͨ��ð�����򷨶Բ����Ϣ��¼������������ */
    for (i = 0, pstr = ismf_info->puh; i < ismf_info->uc; i++) {
      for (j = i + 1, ptmp = pstr->next; j < ismf_info->uc; j++, ptmp = ptmp->next) {
        /* ���ݱ���ʹ�ü������� */
        if ((phig == 0) || (ptmp->pcr > phig->pcr)) {
          phig = ptmp;
        }
      }

      /* ������� */
      if (phig == 0) {
        break;
      }

      /* ������ʹ�ý϶�Ĳ����Ϣ��ǰ�� */
      if (phig->pcr > pstr->pcr) {
        _remove_node(ismf_info, 0, phig);

        phig->prev = pstr->prev;
        phig->next = pstr;
        pstr->prev = phig;

        if (phig->prev) {
          phig->prev->next = phig;
        }

        if (ismf_info->puh == pstr) {
          ismf_info->puh = phig;
        }

        ismf_info->uc++;
        ismf_info->plugin_num++;
      }
      else {
        pstr = pstr->next;
      }

      phig = 0;
    }
  }
  else {
    /* ͨ��ð�����򷨶Բ����Ϣ��δ������������ */
    for (i = 0, pstr = ismf_info->pnh; i < ismf_info->nc; i++) {
      for (j = i + 1, ptmp = pstr->next; j < ismf_info->nc; j++, ptmp = ptmp->next) {
        /* ���ݵ������ȼ����� */
        if ((phig == 0) || (ptmp->pr > phig->pr)) {
          phig = ptmp;
        }
      }

      /* ������� */
      if (phig == 0) {
        break;
      }

      /* ���������ȼ��ϸߵĲ����Ϣ��ǰ�� */
      if (phig->pr > pstr->pr) {
        _remove_node(ismf_info, 1, phig);

        phig->prev = pstr->prev;
        phig->next = pstr;
        pstr->prev = phig;

        if (phig->prev) {
          phig->prev->next = phig;
        }

        if (ismf_info->pnh == pstr) {
          ismf_info->pnh = phig;
        }

        ismf_info->nc++;
        ismf_info->plugin_num++;
      }
      else {
        pstr = pstr->next;
      }

      phig = 0;
    }
  }

  mark++;

  return;
}

