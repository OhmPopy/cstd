/********************************************************************

  lcd.h

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
  ���ļ���;��  ISeeͼ���������LCDͼ���дģ�鶨���ļ�

          ��ȡ���ܣ���ȡ�Զ���ṹ -����(8λһ���ֽ�)�ж��뷽ʽ�洢��
                Һ����������

          ���湦�ܣ��Զ���ṹ -����(8λһ���ֽ�)�ж��뷽ʽ�洢��Һ��
                ��������

  ���ļ���д�ˣ�zhaojinpo (zhaojinpo##sina.com)

  ���ļ��汾��30902
  ����޸��ڣ�2003-9-2

  ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
  ��ַ�ռ������
  ----------------------------------------------------------------
  ������ʷ��

    2003-9    ��һ�η���

********************************************************************/



#ifndef __LCD_MODULE_INC__
#define __LCD_MODULE_INC__

#ifndef __ISEEIO_INC__
#include "iseeio.h"                   /* ���ݴ洢�豸�ӿڶ����ļ� */
#endif


/* ����ģ��汾�����ơ�����ǰ׺��Ĭ�ϵ��ļ�����׺ */
#define MODULE_BUILDID          22
#define MODULE_NAME           ("LCD���")
#define MODULE_FUNC_PREFIX        ("lcd_")
#define MODULE_FILE_POSTFIX       ("lcd")

/* �ļ��б��־ */


#ifdef WIN32  /* Windows */

# ifdef  LCD_EXPORTS
# define LCD_API __declspec(dllexport)
# else
# define LCD_API __declspec(dllimport)
# endif
# define CALLAGREEMENT  __cdecl

#else     /* Linux */

# define LCD_API
# define CALLAGREEMENT

#endif  /*WIN32*/



/* �ļ��ṹ���� */

/* MAGIC���� */
#define LCD_HEADER_MARKER 0x0064636c          //"LCD "

/* ���ݿ�λ�� */
#define LCD_data_offset   0x010

/* ����ͼ�ļ�ͷ�ṹ ��Windows��*/
typedef struct _tag_lcd_file_header {
  unsigned long bfType;
  unsigned long Size;
  unsigned short  width;
  unsigned short  hight;
  unsigned short  bSize;                //Һ�����ڲ���˫���ֽڴ洢
  unsigned char bwidth;               //Һ�����ڲ��õ��ֽڴ洢
  unsigned char bhight;               //Һ�����ڲ��õ��ֽڴ洢
} LCD_FILE_HEADER;

/* ���ݴ洢��ʽ�����ݴ洢1�ֽڴ���һ����8���㡣

   --------lcd_file_header-------------
   ----unsigned long  bfType;   lcd�ļ���ʾ
   ----unsigned long  Size;   Һ��ͼ�δ洢���ݴ�С������ͷ�ṹ���ȣ�
   ----unsigned short width;    ͼ�ο��
   ----unsigned short hight;    ͼ�θ߶�
   ----unsigned short bSize;      �ҵ�Һ�����ڲ���˫���ֽڴ洢
   ----unsigned char  bwidth;     �ҵ�Һ�����ڲ��õ��ֽڴ洢
   ----unsigned char  bhight;     �ҵ�Һ�����ڲ��õ��ֽڴ洢
   ---------lcd�д洢����-------------

   ----unsigned char  data;       Һ�����ֽڰ��д洢����

     �ֽڴ洢��1��----����11������ȡ��

     �������е�1�� ��|0|1| 2 3 4 5 6 7 8 9 10
     |1|
     |2|
     |3|
     |4|
     |5|
     |6|
     �������е�8�� ��|7|

*/


#ifndef __ISEE_IRW_INC__
#include "iseeirw.h"                  /* ���������ݽӿڶ����ļ� */
#endif

#if (defined(__cplusplus) || defined(c_plusplus))
extern  "C" {
#endif

#ifdef WIN32
  extern LCD_API LPIRWP_INFO  CALLAGREEMENT is_irw_plugin(void);
#else /* Linux */
  extern LCD_API LPIRWP_INFO  CALLAGREEMENT lcd_get_plugin_info(void);
  extern LCD_API int      CALLAGREEMENT lcd_init_plugin(void);
  extern LCD_API int      CALLAGREEMENT lcd_detach_plugin(void);
#endif  /* WIN32 */

  extern LCD_API int  CALLAGREEMENT lcd_get_image_info(PISADDR psct, LPINFOSTR pinfo_str);
  extern LCD_API int  CALLAGREEMENT lcd_load_image(PISADDR psct, LPINFOSTR pinfo_str);
  extern LCD_API int  CALLAGREEMENT lcd_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave);

#if (defined(__cplusplus) || defined(c_plusplus))
}
#endif


#endif /* __LCD_MODULE_INC__ */
