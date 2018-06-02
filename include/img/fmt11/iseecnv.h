/********************************************************************

  iseecnv.h

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
  ���ļ���;��  ͼ��ת��ģ�鶨���ļ�
  ���ļ���д�ˣ�
          YZ        yzfree##sina.com

  ���ļ��汾��  20822
  ����޸��ڣ�  2002-8-22

  ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
  ��ַ�ռ������
  ----------------------------------------------------------------
  ������ʷ��

    2002-8    ����λͼת��ɫ��ͼ������λͼת��λͼʱ��BUG��
    2002-8    ��ת�����沢��ͼ���д���棬����ȫ����д��
    2002-4    ������ʽת����������ɫ�����ص���BUG����������
          С���Ա tearshark��
    2001-12   С���Ա menghui ���ָ�ʽת��������ȱ�ٶԶ�ҳ
          ͼ���֧�֡�����BUG��δ������
    2001-7    ������һ�����԰�

********************************************************************/


#ifndef __ISEECNV_INC__
#define __ISEECNV_INC__


#ifndef WIN32
#if defined(_WIN32) || defined(_WINDOWS)
#define WIN32
#endif
#endif /* WIN32 */


#ifdef WIN32  /* Windows OS */

# define CALLAGREEMENT  __cdecl

#else     /* Linux OS */

# define CALLAGREEMENT

#endif  /*WIN32*/


#if (defined(__cplusplus) || defined(c_plusplus))
extern  "C" {
#endif

  int CALLAGREEMENT  iscnv_is_std(int, unsigned long, unsigned long, unsigned long, unsigned long);
  int CALLAGREEMENT  iscnv_match(int);
  int CALLAGREEMENT  iscnv_idtobit(int);
  LPPALITEM CALLAGREEMENT iscnv_get_pal(int);

  void CALLAGREEMENT iscnv_get_mask(int, unsigned long*, unsigned long*, unsigned long*, unsigned long*);
  int CALLAGREEMENT  iscnv_cnvfun(int, int, unsigned char*, LPPALITEM, int, unsigned char*, LPPALITEM, \
      int, int, int, unsigned long, unsigned long, unsigned long, unsigned long);

  int CALLAGREEMENT  iscnv_get_id_info(int, unsigned long*, unsigned long*, unsigned long*, unsigned long*, \
      unsigned long*, int*, LPPALITEM*);

#if (defined(__cplusplus) || defined(c_plusplus))
}
#endif


#endif /* __ISEECNV_INC__ */
