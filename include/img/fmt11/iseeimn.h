/********************************************************************

  iseeimn.h

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
  ���ļ���;��  ͼ���д���棭����������ļ�
  ���ļ���д�ˣ�
          YZ        yzfree##sina.com

  ���ļ��汾��  30206
  ����޸��ڣ�  2003-2-6

  ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
    ��ַ�ռ������
  ----------------------------------------------------------------
  ������ʷ��

    2003-2    �������д�������ķ���բֵ
    2002-8    ��һ�����԰淢��

********************************************************************/

#ifndef __ISEE_IMN_INC__
#define __ISEE_IMN_INC__

#ifdef WIN32  /* Windows */
# define CALLAGREEMENT  __cdecl
#else     /* Linux */
# define CALLAGREEMENT
#endif  /* WIN32 */

#ifndef __ISEE_IRW_INC__
#include "iseeirw.h"                  /* ��Ҫ���е�ĳЩ�ṹ���� */
#endif


struct  _tag_irwmf_infoblock;
typedef struct _tag_irwmf_infoblock   IRWMF_INFOBLOCK;
typedef struct _tag_irwmf_infoblock*   LPIRWMF_INFOBLOCK;

struct  _tag_irwmf_header;
typedef struct _tag_irwmf_header    IRWMF_HEADER;
typedef struct _tag_irwmf_header*    LPIRWMF_HEADER;


#define IRWMF_INFOBLOCK_TAG       0xFAFBFBFA    /* IRWMF_INFOBLOCK�ṹ��ǩ */
#define IRWMF_INFOBLOCK_DEFPIR      30        /* ���ȱʡ���ȼ� */
#define IRWMF_INFOBLOCK_USEING      1       /* ¼�ñ�� */
#define IRWMF_INFOBLOCK_NOTUSE      0       /* δ¼�ñ�� */


/* ���������Ϣ�ļ���ÿ�������Ӧ����Ϣ��ṹ���� */
struct _tag_irwmf_infoblock {
  unsigned long   tag;              /* ʶ���ǩ(IRWMF_INFOBLOCK_TAG) */

  unsigned long   pr;               /* ����������ȼ� */
  unsigned long   pcr;              /* ����ʹ�ü��� */
  int         use;              /* ¼�ñ�ǣ�1����¼�á�0��δ¼�ã�*/
  unsigned long   pilen;              /* ����ļ����� */
  unsigned long   ctimeh;             /* ����ļ�����ʱ�䣨��32λ��*/
  unsigned long   ctimel;             /* ����ļ�����ʱ�䣨��32λ��*/
  unsigned long   mtimeh;             /* ����ļ������޸�ʱ�� */
  unsigned long   mtimel;             /* ����ļ������޸�ʱ�� */
  char        path[_MAX_FNAME];       /* ����ļ��� */
  IRWP_INFO     ifo;              /* �����Ϣ�� */

  int           act;            /* ������ 0-δ���1-���*/
  /* ��16λΪ��Ϣ��ʹ�ü����� */
  HMODULE         handle;           /* ������ */

  IRWP_API_GET_IMG_INFO fn_get_img_info;      /* get_image_info�ӿ� */
  IRWP_API_LOAD_IMG   fn_load_img;        /* load_image�ӿ� */
  IRWP_API_SAVE_IMG   fn_save_img;        /* save_image�ӿ� */

  LPIRWMF_INFOBLOCK   prev;           /* ǰһ����Ϣ��ĵ�ַ */
  LPIRWMF_INFOBLOCK   next;           /* ��һ����Ϣ��ĵ�ַ */
  LPIRWMF_HEADER      pe;             /* ��ͷ��ַ */

  char          rev[28];          /* ���������ڽ�����չ */
};


#define IRWMF_HEADER_TAG        0xC3C2C2C3
#define IRWMF_HEADER_FILTSIZE     3200
#define IRWMF_HEADER_BUD_NULL     0       /* ��Ч��ͷ���� */
#define IRWMF_HEADER_BUD_LOAD     1       /* �����ͱ�ͷ�ṹ */
#define IRWMF_HEADER_BUD_CREATE     2       /* �����ͱ�ͷ�ṹ */


/* ���������Ϣ�ļ���ͷ�ṹ���� */
struct _tag_irwmf_header {
  unsigned long   tag;              /* ��ͷ��ǩ(IRWMF_HEADER_TAG) */

  int         mark;             /* ��ͷ��Ϣ��Ч��ǣ�0����Ч��1����Ч��*/
  int         plugin_num;           /* ����ܸ��� */

  LPIRWMF_INFOBLOCK puh;              /* ¼����ͷ��ַ */
  LPIRWMF_INFOBLOCK put;              /* ¼����β��ַ */
  int         uc;               /* ¼��������ĸ��� */

  LPIRWMF_INFOBLOCK pnh;              /* δ����ͷ��ַ */
  LPIRWMF_INFOBLOCK pnt;              /* δ����β��ַ */
  int         nc;               /* δ��������ĸ��� */

  int         build;              /* �����ķ�ʽ */
  int         locked;             /* ��������־��0��δ��������0��������*/

  char        infpath[MAX_PATH];        /* ����Ϣ�ļ�ȫ·�� */

  char        rev[64];            /* ���� */
};


#define IRWMF_FILENAME        ("iseeirw.ini")   /* �����Ϣ�����ļ������� */
#define IRWMF_ACTIVE_INIT_CNT   2         /* ��ʼ����������Ŀ */
#define IRWMF_IDLE_MAX_COUNT    80          /* ���д��������բֵ */


#if (defined(__cplusplus) || defined(c_plusplus))
extern  "C" {
#endif

  int       CALLAGREEMENT ismf_init(char* path, char* fname);
  int       CALLAGREEMENT ismf_close(void);
  int       CALLAGREEMENT ismf_get_status(void);

  LPIRWMF_HEADER  CALLAGREEMENT ismf_lock(void);
  int       CALLAGREEMENT ismf_update(LPIRWMF_HEADER pt, int type, LPUPDATAINFO rinfo);
  int       CALLAGREEMENT ismf_employ(int id);
  int       CALLAGREEMENT ismf_dismissal(int id);
  int       CALLAGREEMENT ismf_unlock(LPIRWMF_HEADER pt);

  void      CALLAGREEMENT ismf_get_plugin_path(char* path);

  unsigned long CALLAGREEMENT ismf_get_plugin_heandle(const char* path);
  int       CALLAGREEMENT ismf_free_plugin_heandle(unsigned long hand);

  IRWP_API_GET_IMG_INFO CALLAGREEMENT ismf_get_info_addr(unsigned long hand);
  IRWP_API_LOAD_IMG   CALLAGREEMENT ismf_get_load_addr(unsigned long hand);
  IRWP_API_SAVE_IMG   CALLAGREEMENT ismf_get_save_addr(unsigned long hand);



#if (defined(__cplusplus) || defined(c_plusplus))
}
#endif

#endif /* __ISEE_IMN_INC__ */
