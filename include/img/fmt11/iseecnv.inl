/********************************************************************

  iseecnv.c

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
  ���ļ���;��  ͼ���д���棭ͼ��ת��ģ��ʵ���ļ�
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


#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "iseemem.h"
#include "iseeirw.h"
#include "iseecnv.h"


/* ��׼��ʽ���Ͻṹ���� */
typedef struct _tag_std_fmt {
  int       bits;   /* ÿ���ص�λ��� */
  unsigned long bmask;    /* R��G��B��A λ���� */
  unsigned long gmask;
  unsigned long rmask;
  unsigned long amask;
} IRW_STD_FMT, *LPIRW_STD_FMT;


/* ��׼��ʽ���� */
#define IRWSTD_FMT_NUM    7

/* ��׼��ʽ���ݣ�ע������ĸ�ʽ˳������Ķ����� */
IRW_STD_FMT irw_stdfmt[IRWSTD_FMT_NUM] = {
  { 1, 0, 0, 0, 0 },                /* 1 λ��ɫ��ͼ */
  { 4, 0, 0, 0, 0 },                /* 4 λ��ɫ��ͼ */
  { 8, 0, 0, 0, 0 },                /* 8 λ��ɫ��ͼ */
  { 16, 0x1f, 0x3e0, 0x7c00, 0x0 },       /* 16 λ(555)��ʽλͼ */
  { 16, 0x1f, 0x7e0, 0xf800, 0x0 },       /* 16 λ(565)��ʽλͼ */
  { 24, 0xff, 0xff00, 0xff0000, 0x0 },      /* 24 λλͼ */
  { 32, 0xff, 0xff00, 0xff0000, 0xff000000 },   /* 32 λλͼ(��Alpha����) */
};


/* ��׼2ɫ�Ҷȵ�ɫ�� */
PALITEM irw_std_pal2[2] = {
  {0x0, 0x0, 0x0, 0x0},
  {0xff, 0xff, 0xff, 0x0}
};

/* ��׼16ɫ�Ҷȵ�ɫ�� */
PALITEM irw_std_pal16[16] = {
  {0x0, 0x0, 0x0, 0x0},
  {0x11, 0x11, 0x11, 0x0},
  {0x22, 0x22, 0x22, 0x0},
  {0x33, 0x33, 0x33, 0x0},
  {0x44, 0x44, 0x44, 0x0},
  {0x55, 0x55, 0x55, 0x0},
  {0x66, 0x66, 0x66, 0x0},
  {0x77, 0x77, 0x77, 0x0},
  {0x88, 0x88, 0x88, 0x0},
  {0x99, 0x99, 0x99, 0x0},
  {0xaa, 0xaa, 0xaa, 0x0},
  {0xbb, 0xbb, 0xbb, 0x0},
  {0xcc, 0xcc, 0xcc, 0x0},
  {0xdd, 0xdd, 0xdd, 0x0},
  {0xee, 0xee, 0xee, 0x0},
  {0xff, 0xff, 0xff, 0x0}
};

/* ��׼256ɫ�Ҷȵ�ɫ�� */
PALITEM irw_std_pal256[256] = {
  {0x0, 0x0, 0x0, 0x0}, {0x1, 0x1, 0x1, 0x0}, {0x2, 0x2, 0x2, 0x0},
  {0x3, 0x3, 0x3, 0x0}, {0x4, 0x4, 0x4, 0x0}, {0x5, 0x5, 0x5, 0x0},
  {0x6, 0x6, 0x6, 0x0}, {0x7, 0x7, 0x7, 0x0}, {0x8, 0x8, 0x8, 0x0},
  {0x9, 0x9, 0x9, 0x0}, {0xa, 0xa, 0xa, 0x0}, {0xb, 0xb, 0xb, 0x0},
  {0xc, 0xc, 0xc, 0x0}, {0xd, 0xd, 0xd, 0x0}, {0xe, 0xe, 0xe, 0x0},
  {0xf, 0xf, 0xf, 0x0},     {0x10, 0x10, 0x10, 0x0},  {0x11, 0x11, 0x11, 0x0},
  {0x12, 0x12, 0x12, 0x0},  {0x13, 0x13, 0x13, 0x0},  {0x14, 0x14, 0x14, 0x0},
  {0x15, 0x15, 0x15, 0x0},  {0x16, 0x16, 0x16, 0x0},  {0x17, 0x17, 0x17, 0x0},
  {0x18, 0x18, 0x18, 0x0},  {0x19, 0x19, 0x19, 0x0},  {0x1a, 0x1a, 0x1a, 0x0},
  {0x1b, 0x1b, 0x1b, 0x0},  {0x1c, 0x1c, 0x1c, 0x0},  {0x1d, 0x1d, 0x1d, 0x0},
  {0x1e, 0x1e, 0x1e, 0x0},  {0x1f, 0x1f, 0x1f, 0x0},  {0x20, 0x20, 0x20, 0x0},
  {0x21, 0x21, 0x21, 0x0},  {0x22, 0x22, 0x22, 0x0},  {0x23, 0x23, 0x23, 0x0},
  {0x24, 0x24, 0x24, 0x0},  {0x25, 0x25, 0x25, 0x0},  {0x26, 0x26, 0x26, 0x0},
  {0x27, 0x27, 0x27, 0x0},  {0x28, 0x28, 0x28, 0x0},  {0x29, 0x29, 0x29, 0x0},
  {0x2a, 0x2a, 0x2a, 0x0},  {0x2b, 0x2b, 0x2b, 0x0},  {0x2c, 0x2c, 0x2c, 0x0},
  {0x2d, 0x2d, 0x2d, 0x0},  {0x2e, 0x2e, 0x2e, 0x0},  {0x2f, 0x2f, 0x2f, 0x0},
  {0x30, 0x30, 0x30, 0x0},  {0x31, 0x31, 0x31, 0x0},  {0x32, 0x32, 0x32, 0x0},
  {0x33, 0x33, 0x33, 0x0},  {0x34, 0x34, 0x34, 0x0},  {0x35, 0x35, 0x35, 0x0},
  {0x36, 0x36, 0x36, 0x0},  {0x37, 0x37, 0x37, 0x0},  {0x38, 0x38, 0x38, 0x0},
  {0x39, 0x39, 0x39, 0x0},  {0x3a, 0x3a, 0x3a, 0x0},  {0x3b, 0x3b, 0x3b, 0x0},
  {0x3c, 0x3c, 0x3c, 0x0},  {0x3d, 0x3d, 0x3d, 0x0},  {0x3e, 0x3e, 0x3e, 0x0},
  {0x3f, 0x3f, 0x3f, 0x0},  {0x40, 0x40, 0x40, 0x0},  {0x41, 0x41, 0x41, 0x0},
  {0x42, 0x42, 0x42, 0x0},  {0x43, 0x43, 0x43, 0x0},  {0x44, 0x44, 0x44, 0x0},
  {0x45, 0x45, 0x45, 0x0},  {0x46, 0x46, 0x46, 0x0},  {0x47, 0x47, 0x47, 0x0},
  {0x48, 0x48, 0x48, 0x0},  {0x49, 0x49, 0x49, 0x0},  {0x4a, 0x4a, 0x4a, 0x0},
  {0x4b, 0x4b, 0x4b, 0x0},  {0x4c, 0x4c, 0x4c, 0x0},  {0x4d, 0x4d, 0x4d, 0x0},
  {0x4e, 0x4e, 0x4e, 0x0},  {0x4f, 0x4f, 0x4f, 0x0},  {0x50, 0x50, 0x50, 0x0},
  {0x51, 0x51, 0x51, 0x0},  {0x52, 0x52, 0x52, 0x0},  {0x53, 0x53, 0x53, 0x0},
  {0x54, 0x54, 0x54, 0x0},  {0x55, 0x55, 0x55, 0x0},  {0x56, 0x56, 0x56, 0x0},
  {0x57, 0x57, 0x57, 0x0},  {0x58, 0x58, 0x58, 0x0},  {0x59, 0x59, 0x59, 0x0},
  {0x5a, 0x5a, 0x5a, 0x0},  {0x5b, 0x5b, 0x5b, 0x0},  {0x5c, 0x5c, 0x5c, 0x0},
  {0x5d, 0x5d, 0x5d, 0x0},  {0x5e, 0x5e, 0x5e, 0x0},  {0x5f, 0x5f, 0x5f, 0x0},
  {0x60, 0x60, 0x60, 0x0},  {0x61, 0x61, 0x61, 0x0},  {0x62, 0x62, 0x62, 0x0},
  {0x63, 0x63, 0x63, 0x0},  {0x64, 0x64, 0x64, 0x0},  {0x65, 0x65, 0x65, 0x0},
  {0x66, 0x66, 0x66, 0x0},  {0x67, 0x67, 0x67, 0x0},  {0x68, 0x68, 0x68, 0x0},
  {0x69, 0x69, 0x69, 0x0},  {0x6a, 0x6a, 0x6a, 0x0},  {0x6b, 0x6b, 0x6b, 0x0},
  {0x6c, 0x6c, 0x6c, 0x0},  {0x6d, 0x6d, 0x6d, 0x0},  {0x6e, 0x6e, 0x6e, 0x0},
  {0x6f, 0x6f, 0x6f, 0x0},  {0x70, 0x70, 0x70, 0x0},  {0x71, 0x71, 0x71, 0x0},
  {0x72, 0x72, 0x72, 0x0},  {0x73, 0x73, 0x73, 0x0},  {0x74, 0x74, 0x74, 0x0},
  {0x75, 0x75, 0x75, 0x0},  {0x76, 0x76, 0x76, 0x0},  {0x77, 0x77, 0x77, 0x0},
  {0x78, 0x78, 0x78, 0x0},  {0x79, 0x79, 0x79, 0x0},  {0x7a, 0x7a, 0x7a, 0x0},
  {0x7b, 0x7b, 0x7b, 0x0},  {0x7c, 0x7c, 0x7c, 0x0},  {0x7d, 0x7d, 0x7d, 0x0},
  {0x7e, 0x7e, 0x7e, 0x0},  {0x7f, 0x7f, 0x7f, 0x0},  {0x80, 0x80, 0x80, 0x0},
  {0x81, 0x81, 0x81, 0x0},  {0x82, 0x82, 0x82, 0x0},  {0x83, 0x83, 0x83, 0x0},
  {0x84, 0x84, 0x84, 0x0},  {0x85, 0x85, 0x85, 0x0},  {0x86, 0x86, 0x86, 0x0},
  {0x87, 0x87, 0x87, 0x0},  {0x88, 0x88, 0x88, 0x0},  {0x89, 0x89, 0x89, 0x0},
  {0x8a, 0x8a, 0x8a, 0x0},  {0x8b, 0x8b, 0x8b, 0x0},  {0x8c, 0x8c, 0x8c, 0x0},
  {0x8d, 0x8d, 0x8d, 0x0},  {0x8e, 0x8e, 0x8e, 0x0},  {0x8f, 0x8f, 0x8f, 0x0},
  {0x90, 0x90, 0x90, 0x0},  {0x91, 0x91, 0x91, 0x0},  {0x92, 0x92, 0x92, 0x0},
  {0x93, 0x93, 0x93, 0x0},  {0x94, 0x94, 0x94, 0x0},  {0x95, 0x95, 0x95, 0x0},
  {0x96, 0x96, 0x96, 0x0},  {0x97, 0x97, 0x97, 0x0},  {0x98, 0x98, 0x98, 0x0},
  {0x99, 0x99, 0x99, 0x0},  {0x9a, 0x9a, 0x9a, 0x0},  {0x9b, 0x9b, 0x9b, 0x0},
  {0x9c, 0x9c, 0x9c, 0x0},  {0x9d, 0x9d, 0x9d, 0x0},  {0x9e, 0x9e, 0x9e, 0x0},
  {0x9f, 0x9f, 0x9f, 0x0},  {0xa0, 0xa0, 0xa0, 0x0},  {0xa1, 0xa1, 0xa1, 0x0},
  {0xa2, 0xa2, 0xa2, 0x0},  {0xa3, 0xa3, 0xa3, 0x0},  {0xa4, 0xa4, 0xa4, 0x0},
  {0xa5, 0xa5, 0xa5, 0x0},  {0xa6, 0xa6, 0xa6, 0x0},  {0xa7, 0xa7, 0xa7, 0x0},
  {0xa8, 0xa8, 0xa8, 0x0},  {0xa9, 0xa9, 0xa9, 0x0},  {0xaa, 0xaa, 0xaa, 0x0},
  {0xab, 0xab, 0xab, 0x0},  {0xac, 0xac, 0xac, 0x0},  {0xad, 0xad, 0xad, 0x0},
  {0xae, 0xae, 0xae, 0x0},  {0xaf, 0xaf, 0xaf, 0x0},  {0xb0, 0xb0, 0xb0, 0x0},
  {0xb1, 0xb1, 0xb1, 0x0},  {0xb2, 0xb2, 0xb2, 0x0},  {0xb3, 0xb3, 0xb3, 0x0},
  {0xb4, 0xb4, 0xb4, 0x0},  {0xb5, 0xb5, 0xb5, 0x0},  {0xb6, 0xb6, 0xb6, 0x0},
  {0xb7, 0xb7, 0xb7, 0x0},  {0xb8, 0xb8, 0xb8, 0x0},  {0xb9, 0xb9, 0xb9, 0x0},
  {0xba, 0xba, 0xba, 0x0},  {0xbb, 0xbb, 0xbb, 0x0},  {0xbc, 0xbc, 0xbc, 0x0},
  {0xbd, 0xbd, 0xbd, 0x0},  {0xbe, 0xbe, 0xbe, 0x0},  {0xbf, 0xbf, 0xbf, 0x0},
  {0xc0, 0xc0, 0xc0, 0x0},  {0xc1, 0xc1, 0xc1, 0x0},  {0xc2, 0xc2, 0xc2, 0x0},
  {0xc3, 0xc3, 0xc3, 0x0},  {0xc4, 0xc4, 0xc4, 0x0},  {0xc5, 0xc5, 0xc5, 0x0},
  {0xc6, 0xc6, 0xc6, 0x0},  {0xc7, 0xc7, 0xc7, 0x0},  {0xc8, 0xc8, 0xc8, 0x0},
  {0xc9, 0xc9, 0xc9, 0x0},  {0xca, 0xca, 0xca, 0x0},  {0xcb, 0xcb, 0xcb, 0x0},
  {0xcc, 0xcc, 0xcc, 0x0},  {0xcd, 0xcd, 0xcd, 0x0},  {0xce, 0xce, 0xce, 0x0},
  {0xcf, 0xcf, 0xcf, 0x0},  {0xd0, 0xd0, 0xd0, 0x0},  {0xd1, 0xd1, 0xd1, 0x0},
  {0xd2, 0xd2, 0xd2, 0x0},  {0xd3, 0xd3, 0xd3, 0x0},  {0xd4, 0xd4, 0xd4, 0x0},
  {0xd5, 0xd5, 0xd5, 0x0},  {0xd6, 0xd6, 0xd6, 0x0},  {0xd7, 0xd7, 0xd7, 0x0},
  {0xd8, 0xd8, 0xd8, 0x0},  {0xd9, 0xd9, 0xd9, 0x0},  {0xda, 0xda, 0xda, 0x0},
  {0xdb, 0xdb, 0xdb, 0x0},  {0xdc, 0xdc, 0xdc, 0x0},  {0xdd, 0xdd, 0xdd, 0x0},
  {0xde, 0xde, 0xde, 0x0},  {0xdf, 0xdf, 0xdf, 0x0},  {0xe0, 0xe0, 0xe0, 0x0},
  {0xe1, 0xe1, 0xe1, 0x0},  {0xe2, 0xe2, 0xe2, 0x0},  {0xe3, 0xe3, 0xe3, 0x0},
  {0xe4, 0xe4, 0xe4, 0x0},  {0xe5, 0xe5, 0xe5, 0x0},  {0xe6, 0xe6, 0xe6, 0x0},
  {0xe7, 0xe7, 0xe7, 0x0},  {0xe8, 0xe8, 0xe8, 0x0},  {0xe9, 0xe9, 0xe9, 0x0},
  {0xea, 0xea, 0xea, 0x0},  {0xeb, 0xeb, 0xeb, 0x0},  {0xec, 0xec, 0xec, 0x0},
  {0xed, 0xed, 0xed, 0x0},  {0xee, 0xee, 0xee, 0x0},  {0xef, 0xef, 0xef, 0x0},
  {0xf0, 0xf0, 0xf0, 0x0},  {0xf1, 0xf1, 0xf1, 0x0},  {0xf2, 0xf2, 0xf2, 0x0},
  {0xf3, 0xf3, 0xf3, 0x0},  {0xf4, 0xf4, 0xf4, 0x0},  {0xf5, 0xf5, 0xf5, 0x0},
  {0xf6, 0xf6, 0xf6, 0x0},  {0xf7, 0xf7, 0xf7, 0x0},  {0xf8, 0xf8, 0xf8, 0x0},
  {0xf9, 0xf9, 0xf9, 0x0},  {0xfa, 0xfa, 0xfa, 0x0},  {0xfb, 0xfb, 0xfb, 0x0},
  {0xfc, 0xfc, 0xfc, 0x0},  {0xfd, 0xfd, 0xfd, 0x0},  {0xfe, 0xfe, 0xfe, 0x0},
  {0xff, 0xff, 0xff, 0x0}
};




/* ת������ԭ�Ͷ��� */
#define DECCNVFUN(name)   name(int dstd, unsigned char *soubits, LPPALITEM soupal, int soupatch, unsigned char *desbits, LPPALITEM despal, int despatch, int linenum, int width)

/* λ��ת���������� */
static int DECCNVFUN(_1tox);
static int DECCNVFUN(_4tox);
static int DECCNVFUN(_8tox);

static int _paltox(int, int, unsigned char*, LPPALITEM, int, unsigned char*, LPPALITEM, \
    int, int, int, unsigned long, unsigned long, unsigned long, unsigned long);
static int _rgbtox(int, int, unsigned char*, LPPALITEM, int, unsigned char*, LPPALITEM, \
    int, int, int, unsigned long, unsigned long, unsigned long, unsigned long);

static unsigned long _get_next_pix(unsigned char*, int, int, int);
static int _get_mask_offset(unsigned long mask);



/* *********************************************************************************** */
/* ��ģ���и�������ת��ԭ��

  λͼ��
    1��תΪ��ɫ��ͼ���ȼ���Ҷ�ֵ��Ȼ��ȷ�����ڵ�ɫ���е�λ�ã�
    2��ͬλ���ת�������������ͬ��ֱ�������������������ɫ��ϳɣ�
    3��ת��Ϊ��λ���λͼ��������λ���ݣ�ȥ����λ���ݣ�
    4��תΪ��λ���λͼ�����λ���䣩

  ��ɫ��ͼ��
    1��תΪ��λ��ɫ��ͼ����ֵ��ƽ����ȡ��ɫ�������Ŀ���ɫ�壩
    2��ͬλ���ת����ֱ����
    3��תΪ��λ��ȵ�ɫ��ͼ��׼ֱ�������ؼ���ɫ�����ݾ�ֱ����
    4��תΪλͼ����չ��ɫ�����ݣ��ϳɣ�
*/
/* *********************************************************************************** */



/* �ж�ָ����λ��ȼ���������Ƿ���ISee��׼��ʽ֮һ */
int iscnv_is_std(int bit,
    unsigned long rmask,
    unsigned long gmask,
    unsigned long bmask,
    unsigned long amask)
{
  int i;

  for (i = 0; i < IRWSTD_FMT_NUM; i++) {
    if ((irw_stdfmt[i].bits == bit) && (irw_stdfmt[i].rmask == rmask) && \
        (irw_stdfmt[i].gmask == gmask) && (irw_stdfmt[i].bmask == bmask) && \
        (irw_stdfmt[i].amask == amask)) {
      break;
    }
  }

  /* ����-1�����ָ����ͼ���Ǳ�׼��ʽ�����򷵻ر�׼��ʽID */
  return (i == IRWSTD_FMT_NUM) ? -1 : i;
}


/* ƥ����ӽ���һ����׼��ʽ������������ID */
int iscnv_match(int bit)
{
  int i, result = -1;

  for (i = 0; i < IRWSTD_FMT_NUM; i++) {
    if (irw_stdfmt[i].bits >= bit) {
      result = i;
      break;
    }
  }

  assert(result != -1);               /* ���λ��ܳ���32λ */

  return result;
}


/* ��ȡָ��ID��׼��ʽ���������� */
void iscnv_get_mask(int id,
    unsigned long* rmask,
    unsigned long* gmask,
    unsigned long* bmask,
    unsigned long* amask)
{
  assert(rmask && gmask && bmask && amask);
  assert(id < IRWSTD_FMT_NUM);

  *rmask = irw_stdfmt[id].rmask;
  *gmask = irw_stdfmt[id].gmask;
  *bmask = irw_stdfmt[id].bmask;
  *amask = irw_stdfmt[id].amask;
}


/* ��׼��ʽIDתλ���ֵ */
int iscnv_idtobit(int id)
{
  assert((id >= 0) && (id < IRWSTD_FMT_NUM));

  return irw_stdfmt[id].bits;
}


/* ��ȡָ��ID����ظ�ʽ��Ϣ */
int iscnv_get_id_info(int id,
    unsigned long* bit,
    unsigned long* rmask,
    unsigned long* gmask,
    unsigned long* bmask,
    unsigned long* amask,
    int* pal_size,
    LPPALITEM* pppal)
{
  if ((id < 0) || (id >= IRWSTD_FMT_NUM)) {
    return -1;                    /* �Ƿ���IDֵ */
  }

  if (bit) {                    /* ��дλ���ֵ */
    *bit = irw_stdfmt[id].bits;
  }

  if (rmask) {                    /* ��д�������� */
    *rmask = irw_stdfmt[id].rmask;
  }

  if (gmask) {
    *gmask = irw_stdfmt[id].gmask;
  }

  if (bmask) {
    *bmask = irw_stdfmt[id].bmask;
  }

  if (amask) {
    *amask = irw_stdfmt[id].amask;
  }

  if (pal_size) {                 /* ��д��ɫ��ߴ� */
    switch (id) {
    case  0:
      *pal_size = 2;
      break;

    case  1:
      *pal_size = 16;
      break;

    case  2:
      *pal_size = 256;
      break;

    default:
      *pal_size = 0;
      break;
    }
  }

  if (pppal) {                    /* ��д��׼��ɫ���ַ */
    switch (id) {
    case  0:
      *pppal = (LPPALITEM)irw_std_pal2;
      break;

    case  1:
      *pppal = (LPPALITEM)irw_std_pal16;
      break;

    case  2:
      *pppal = (LPPALITEM)irw_std_pal256;
      break;

    default:
      *pppal = (LPPALITEM)0;
      break;
    }
  }

  return 0;                     /* �ɹ����� */
}


/* ��ȡ��׼�Ҷȵ�ɫ���ַ */
LPPALITEM iscnv_get_pal(int bit)
{
  switch (bit) {
  case  1:
    return irw_std_pal2;
    break;

  case  4:
    return irw_std_pal16;
    break;

  case  8:
    return irw_std_pal256;
    break;

  default:
    return 0;
  }
}


/* ת��ģ��ӿ� */
int iscnv_cnvfun(int sbit,                /* Դͼ��λ��� */
    int dstd,                /* Ŀ���ʽ��׼ID�� */
    unsigned char* soubits,        /* Դͼ���������ݵ�ַ */
    LPPALITEM soupal,            /* Դͼ���ɫ���ַ */
    int soupatch,              /* Դͼ��ɨ���гߴ� */
    unsigned char* desbits,        /* Ŀ��ͼ���������ݵ�ַ */
    LPPALITEM despal,            /* Ŀ��ͼ���ɫ���ַ */
    int despatch,              /* Ŀ��ͼ��ɨ���гߴ� */
    int linenum,               /* Ҫת����ͼ������ */
    int width,               /* һ��ͼ������ؿ�� */
    unsigned long bmask,         /* Դͼ���������� */
    unsigned long gmask,
    unsigned long rmask,
    unsigned long amask)
{
  int result;

  assert((dstd >= 0) && (dstd < IRWSTD_FMT_NUM));

  if (sbit > 8) {                 /* λͼת�� */
    result = _rgbtox(sbit, dstd, soubits, soupal, soupatch, desbits, despal, \
        despatch, linenum, width, bmask, gmask, rmask, amask);
  }
  else {                      /* ��ɫ��ͼת�� */
    switch (sbit) {
    case  1:
      result = _1tox(dstd, soubits, soupal, soupatch, desbits, despal, \
          despatch, linenum, width);
      break;

    case  4:
      result = _4tox(dstd, soubits, soupal, soupatch, desbits, despal, \
          despatch, linenum, width);
      break;

    case  8:
      result = _8tox(dstd, soubits, soupal, soupatch, desbits, despal, \
          despatch, linenum, width);
      break;

    default:
      result = _paltox(sbit, dstd, soubits, soupal, soupatch, desbits, despal, \
          despatch, linenum, width, bmask, gmask, rmask, amask);
      break;
    }
  }

  return result;
}


/* 1λ��ɫ��ͼת������ */
static int DECCNVFUN(_1tox)
{
  int       i, j, k;
  unsigned long lz, lo;
  unsigned short  sz, so;
  unsigned char c;

  switch (dstd) {
  case  0:  /* 1λ��ɫ��ͼ */
    while (linenum--) {
      /* ͬλ��ɫ��ͼֱ�� */
      memcpy((void*)desbits, (const void*)soubits, (size_t)((width + 7) / 8));

      /* ��λ����һ�� */
      desbits += despatch;
      soubits += soupatch;
    }

    break;

  case  1:  /* 4λ��ɫ��ͼ */
    while (linenum--) {
      i = 0;
      j = -1;
      k = 0;

      while (i < width) {           /* ת��һ�е��������� */
        if (j == -1) {
          c = soubits[i / 8];       /* ��ȡ8�����أ���һ�ֽڣ�*/
          j = 7;
          k = 0;
        }

        if (k == 0) {             /* ��4λ */
          desbits[i / 2] = ((c >> j) & 0x1) << 4;
          k = 1;
        }
        else {                /* ��4λ */
          desbits[i / 2] |= ((c >> j) & 0x1);
          k = 0;
        }

        j--;
        i++;                  /* ����λ�ü�һ */
      }

      desbits += despatch;            /* ��ʼ��һ�� */
      soubits += soupatch;
    }

    break;

  case  2:  /* 8λ��ɫ��ͼ */
    while (linenum--) {
      i = 0;
      j = -1;

      while (i < width) {           /* ת��һ�е��������� */
        if (j == -1) {
          c = soubits[i / 8];       /* ��ȡ8�����أ���һ�ֽڣ�*/
          j = 7;
        }

        desbits[i++] = ((c >> j--) & 0x1);
      }

      desbits += despatch;            /* ��ʼ��һ�� */
      soubits += soupatch;
    }

    break;

  case  3:  /* 16λ(555)λͼ */
    /* �ϳ������ɫ�����ݣ��Լӿ�����ٶ� */
    sz = ((((unsigned short)soupal[0].blue) >> 3) & 0x1f) | \
        ((((unsigned short)soupal[0].green) << 2) & 0x3e0) | \
        ((((unsigned short)soupal[0].red) << 7) & 0x7c00);

    so = ((((unsigned short)soupal[1].blue) >> 3) & 0x1f) | \
        ((((unsigned short)soupal[1].green) << 2) & 0x3e0) | \
        ((((unsigned short)soupal[1].red) << 7) & 0x7c00);

    while (linenum--) {
      i = 0;
      j = -1;

      while (i < width) {           /* ת��һ�е��������� */
        if (j == -1) {
          c = soubits[i / 8];       /* ��ȡ8�����أ���һ�ֽڣ�*/
          j = 7;
        }

        ((unsigned short*)desbits)[i++] = ((c >> j--) & 0x1) ? so : sz;
      }

      desbits += despatch;            /* ��ʼ��һ�� */
      soubits += soupatch;
    }

    break;

  case  4:  /* 16λ(565)λͼ */
    sz = ((((unsigned short)soupal[0].blue) >> 3) & 0x1f) | \
        ((((unsigned short)soupal[0].green) << 3) & 0x7e0) | \
        ((((unsigned short)soupal[0].red) << 8) & 0xf800);

    so = ((((unsigned short)soupal[1].blue) >> 3) & 0x1f) | \
        ((((unsigned short)soupal[1].green) << 3) & 0x7e0) | \
        ((((unsigned short)soupal[1].red) << 8) & 0xf800);

    while (linenum--) {
      i = 0;
      j = -1;

      while (i < width) {           /* ת��һ�е��������� */
        if (j == -1) {
          c = soubits[i / 8];       /* ��ȡ8�����أ���һ�ֽڣ�*/
          j = 7;
        }

        ((unsigned short*)desbits)[i++] = ((c >> j--) & 0x1) ? so : sz;
      }

      desbits += despatch;            /* ��ʼ��һ�� */
      soubits += soupatch;
    }

    break;

  case  5:  /* 24λ(888)λͼ */
    while (linenum--) {
      i = 0;
      j = -1;

      while (i < width) {           /* ת��һ�е��������� */
        if (j == -1) {
          c = soubits[i / 8];       /* ��ȡ8�����أ���һ�ֽڣ�*/
          j = 7;
        }

        k = (int)(unsigned int)((c >> j) & 0x1); /* ��ȡ����ֵ */

        if ((i + 1) == width) {
          desbits[i * 3]   = soupal[k].blue;
          desbits[i * 3 + 1] = soupal[k].green;
          desbits[i * 3 + 2] = soupal[k].red;
        }
        else {
          *((unsigned long*)(((unsigned char*)desbits) + i * 3)) = \
              (((unsigned long*)soupal)[k]) & 0xffffff;
        }

        i++;
        j--;
      }

      desbits += despatch;            /* ��ʼ��һ�� */
      soubits += soupatch;
    }

    break;

  case  6:  /* 32λ(888)λͼ */
    lz = (((unsigned long*)soupal)[0]) & 0xffffff;
    lo = (((unsigned long*)soupal)[1]) & 0xffffff;

    while (linenum--) {
      i = 0;
      j = -1;

      while (i < width) {           /* ת��һ�е��������� */
        if (j == -1) {
          c = soubits[i / 8];       /* ��ȡ8�����أ���һ�ֽڣ�*/
          j = 7;
        }

        ((unsigned long*)desbits)[i++] = ((c >> j--) & 0x1) ? lo : lz;
      }

      desbits += despatch;            /* ��ʼ��һ�� */
      soubits += soupatch;
    }

    break;

  default:  /* �Ƿ���ʽ */
    assert(0);
    return -1;
    break;
  }

  return 0;
}


/* 4λ��ɫ��ͼת������ */
static int DECCNVFUN(_4tox)
{
  int       i, j, k;
  unsigned long lz, lo;
  unsigned char c;

  switch (dstd) {
  case  0:  /* 1λ��ɫ��ͼ */
    while (linenum--) {
      i = 0;
      j = -1;
      k = -1;

      while (i < width) {           /* ת��һ�е��������� */
        if (k == -1) {
          c = soubits[i / 2];       /* ��ȡ2�����أ���һ�ֽڣ�*/
          k = 0;
        }

        if (j == -1) {
          j = 7;
          desbits[i / 8] = 0;
        }

        if (k == 0) {             /* ��4λ */
          desbits[i / 8] |= (((c >> 4) & 0xf) / 8) << j;
          k = 1;
        }
        else {                /* ��4λ */
          desbits[i / 8] |= ((c & 0xf) / 8) << j; /* ��8�ĺ��壺0��7��0��8��15��1 */
          k = -1;
        }

        j--;
        i++;                  /* ����λ�ü�һ */
      }

      desbits += despatch;            /* ��ʼ��һ�� */
      soubits += soupatch;
    }

    break;

  case  1:  /* 4λ��ɫ��ͼ */
    while (linenum--) {
      /* ͬλ��ɫ��ͼֱ�� */
      memcpy((void*)desbits, (const void*)soubits, (size_t)((width + 1) / 2));

      /* ��λ����һ�� */
      desbits += despatch;
      soubits += soupatch;
    }

    break;

  case  2:  /* 8λ��ɫ��ͼ */
    while (linenum--) {
      i = 0;
      k = -1;

      while (i < width) {           /* ת��һ�е��������� */
        if (k == -1) {
          c = soubits[i / 2];       /* ��ȡ2�����أ���һ�ֽڣ�*/
          k = 0;
        }

        if (k == 0) {             /* ��4λ */
          desbits[i++] = ((c >> 4) & 0xf);
          k = 1;
        }
        else {                /* ��4λ */
          desbits[i++] = (c & 0xf);
          k = -1;
        }
      }

      desbits += despatch;            /* ��ʼ��һ�� */
      soubits += soupatch;
    }

    break;

  case  3:  /* 16λ(555)λͼ */
    while (linenum--) {
      i = 0;
      k = -1;

      while (i < width) {           /* ת��һ�е��������� */
        if (k == -1) {
          c = soubits[i / 2];       /* ��ȡ2�����أ���һ�ֽڣ�*/
          k = 0;
        }

        if (k == 0) {
          j = (int)(unsigned int)((c >> 4) & 0xf);
          k = 1;
        }
        else {
          j = (int)(unsigned int)(c & 0xf);
          k = -1;
        }

        ((unsigned short*)desbits)[i++] = \
            ((((unsigned short)soupal[j].blue) >> 3) & 0x1f) | \
            ((((unsigned short)soupal[j].green) << 2) & 0x3e0) | \
            ((((unsigned short)soupal[j].red) << 7) & 0x7c00);
      }

      desbits += despatch;            /* ��ʼ��һ�� */
      soubits += soupatch;
    }

    break;

  case  4:  /* 16λ(565)λͼ */
    while (linenum--) {
      i = 0;
      k = -1;

      while (i < width) {           /* ת��һ�е��������� */
        if (k == -1) {
          c = soubits[i / 2];       /* ��ȡ2�����أ���һ�ֽڣ�*/
          k = 0;
        }

        if (k == 0) {
          j = (int)(unsigned int)((c >> 4) & 0xf);
          k = 1;
        }
        else {
          j = (int)(unsigned int)(c & 0xf);
          k = -1;
        }

        ((unsigned short*)desbits)[i++] = \
            ((((unsigned short)soupal[j].blue) >> 3) & 0x1f) | \
            ((((unsigned short)soupal[j].green) << 3) & 0x7e0) | \
            ((((unsigned short)soupal[j].red) << 8) & 0xf800);
      }

      desbits += despatch;            /* ��ʼ��һ�� */
      soubits += soupatch;
    }

    break;

  case  5:  /* 24λλͼ */
    while (linenum--) {
      i = 0;
      k = -1;

      while (i < width) {           /* ת��һ�е��������� */
        if (k == -1) {
          c = soubits[i / 2];       /* ��ȡ2�����أ���һ�ֽڣ�*/
          k = 0;
        }

        if (k == 0) {
          j = (int)(unsigned int)((c >> 4) & 0xf);
          k = 1;
        }
        else {
          j = (int)(unsigned int)(c & 0xf);
          k = -1;
        }

        if ((i + 1) == width) {       /* ���һ������ */
          desbits[i * 3]   = soupal[j].blue;
          desbits[i * 3 + 1] = soupal[j].green;
          desbits[i * 3 + 2] = soupal[j].red;
        }
        else {
          *((unsigned long*)(((unsigned char*)desbits) + i * 3)) = \
              (((unsigned long*)soupal)[j]) & 0xffffff;
        }

        i++;
      }

      desbits += despatch;            /* ��ʼ��һ�� */
      soubits += soupatch;
    }

    break;

  case  6:  /* 32λλͼ */
    lz = (((unsigned long*)soupal)[0]) & 0xffffff;
    lo = (((unsigned long*)soupal)[1]) & 0xffffff;

    while (linenum--) {
      i = 0;
      k = -1;

      while (i < width) {           /* ת��һ�е��������� */
        if (k == -1) {
          c = soubits[i / 2];       /* ��ȡ2�����أ���һ�ֽڣ�*/
          k = 0;
        }

        if (k == 0) {
          j = (int)(unsigned int)((c >> 4) & 0xf);
          k = 1;
        }
        else {
          j = (int)(unsigned int)(c & 0xf);
          k = -1;
        }

        ((unsigned long*)desbits)[i++] = ((unsigned long*)soupal)[j] & 0xffffff;
      }

      desbits += despatch;            /* ��ʼ��һ�� */
      soubits += soupatch;
    }

    break;

  default:  /* �Ƿ���ʽ */
    assert(0);
    return -1;
    break;
  }

  return 0;
}


/* 8λ��ɫ��ͼת������ */
static int DECCNVFUN(_8tox)
{
  int       i, j, k;
  unsigned short  sz;
  unsigned char c;

  switch (dstd) {
  case  0:  /* 1λ��ɫ��ͼ */
    while (linenum--) {
      i = 0;
      j = -1;

      while (i < width) {
        c = soubits[i];             /* ���һ������ */

        if (j == -1) {
          desbits[i / 8] = 0;       /* ��ʼ��Ŀ���ֽ�λ0 */
          j = 7;
        }

        desbits[i / 8] |= (c / 128) << j; /* 0��127���0�128��255���1�� */

        i++;
        j--;
      }

      soubits += soupatch;            /* ת����һ�д��� */
      desbits += despatch;
    }

    break;

  case  1:  /* 4λ��ɫ��ͼ */
    while (linenum--) {
      i = 0;
      k = -1;

      while (i < width) {
        c = soubits[i];             /* ���һ������ */

        if (k == -1) {
          desbits[i / 2] = 0;       /* ��ʼ��Ŀ������ */
          k = 0;
        }

        if (k == 0) {             /* ��4λ */
          desbits[i / 2] |= ((c / 16) & 0xf) << 4; /* תΪ16ɫ��ɫ�� */
          k = 1;
        }
        else {                /* ��4λ */
          desbits[i / 2] |= ((c / 16) & 0xf);
          k = -1;
        }

        i++;
      }

      soubits += soupatch;
      desbits += despatch;
    }

    break;

  case  2:  /* 8λ��ɫ��ͼ */
    while (linenum--) {
      /* ͬλ��ɫ��ͼֱ�� */
      memcpy((void*)desbits, (const void*)soubits, (size_t)width);

      /* ��λ����һ�� */
      desbits += despatch;
      soubits += soupatch;
    }

    break;

  case  3:  /* 16λ(555)λͼ */
    while (linenum--) {
      i = 0;

      while (i < width) {
        c = soubits[i];             /* ��ȡһ������ */

        sz = ((((unsigned short)soupal[(unsigned int)c].blue) >> 3) & 0x1f) | \
            ((((unsigned short)soupal[(unsigned int)c].green) << 2) & 0x3e0) | \
            ((((unsigned short)soupal[(unsigned int)c].red) << 7) & 0x7c00);

        ((unsigned short*)desbits)[i++] = sz;
      }

      soubits += soupatch;
      desbits += despatch;
    }

    break;

  case  4:  /* 16λ(565)λͼ */
    while (linenum--) {
      i = 0;

      while (i < width) {
        c = soubits[i];             /* ��ȡһ������ */

        sz = ((((unsigned short)soupal[(unsigned int)c].blue) >> 3) & 0x1f) | \
            ((((unsigned short)soupal[(unsigned int)c].green) << 3) & 0x7e0) | \
            ((((unsigned short)soupal[(unsigned int)c].red) << 8) & 0xf800);

        ((unsigned short*)desbits)[i++] = sz;
      }

      soubits += soupatch;
      desbits += despatch;
    }

    break;

  case  5:  /* 24λλͼ */
    while (linenum--) {
      i = 0;

      while (i < width) {
        c = soubits[i];

        if ((i + 1) == width) {       /* ���һ������ */
          desbits[i * 3 + 0] = soupal[(unsigned int)c].blue;
          desbits[i * 3 + 1] = soupal[(unsigned int)c].green;
          desbits[i * 3 + 2] = soupal[(unsigned int)c].red;
        }
        else {                /* �����һ�����أ����ÿ����㷨 */
          *(unsigned long*)(desbits + i * 3) = ((unsigned long*)soupal)[(unsigned int)c] & 0xffffff;
        }

        i++;
      }

      soubits += soupatch;            /* ת����һ�� */
      desbits += despatch;
    }

    break;

  case  6:  /* 32λλͼ */
    while (linenum--) {
      i = 0;

      while (i < width) {
        c = soubits[i];

        ((unsigned long*)desbits)[i++] = (((unsigned long*)soupal)[(unsigned int)c]) & 0xffffff;
      }

      soubits += soupatch;
      desbits += despatch;
    }

    break;

  default:  /* �Ƿ���ʽ */
    assert(0);
    return -1;
    break;
  }

  return 0;
}


/* 2��3��5��6��7λ��ɫ��ͼת������ */
static int _paltox(
    int sbit,
    int dstd,
    unsigned char* soubits,
    LPPALITEM soupal,
    int soupatch,
    unsigned char* desbits,
    LPPALITEM despal,
    int despatch,
    int linenum,
    int width,
    unsigned long bmask,
    unsigned long gmask,
    unsigned long rmask,
    unsigned long amask)
{
  unsigned long code, descode;
  int       i, j, k;
  int       bits;               /* һ�������ص�λ���ۼ� */
  unsigned long sr, sb, sg, sa;           /* Դ���ط��� */
  unsigned long dmr, dmg, dmb, dma;         /* Ŀ��ͼ������ */
  int       dmor, dmog, dmob, dmoa;       /* Ŀ��ͼ������λ�� */


  switch (dstd) {
  case  0:  /* 1λ��ɫ��ͼ */
    while (linenum--) {
      j = -1;
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        if (j == -1) {
          desbits[i / 8] = 0;       /* ��ʼ��Ŀ���ֽ� */
          j = 7;
        }

        /* תΪ1λ��ɫ������ */
        descode = (code / (((unsigned long)pow((int)2, (int)sbit)) / 2)) & 0x1;

        desbits[i / 8] |= ((unsigned char)descode) << j;

        j--;
      }

      soubits += soupatch;            /* ��ʼ��һ�� */
      desbits += despatch;
    }

    break;

  case  1:  /* 4λ��ɫ��ͼ */
    while (linenum--) {
      k = -1;
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        if (k == -1) {
          desbits[i / 2] = 0;       /* ��ʼ��Ŀ���ֽ� */
          k = 0;
        }

        if (sbit == 2) {            /* 2λ */
          if (k == 0) {           /* ��4λ */
            desbits[i / 2] |= ((unsigned char)code & 0x3) << 4;
            k = 1;
          }
          else {              /* ��4λ */
            desbits[i / 2] |= (unsigned char)code & 0x3;
            k = -1;
          }
        }
        else if (sbit == 3) {
          if (k == 0) {           /* ��4λ */
            desbits[i / 2] |= ((unsigned char)code & 0x7) << 4;
            k = 1;
          }
          else {              /* ��4λ */
            desbits[i / 2] |= (unsigned char)code & 0x7;
            k = -1;
          }
        }
        else {                /* 5��6��7λ */
          descode = (code / (((unsigned long)pow((int)2, (int)sbit)) / 16)) & 0xf;

          if (k == 0) {           /* ��4λ */
            desbits[i / 2] |= ((unsigned char)descode & 0xf) << 4;
            k = 1;
          }
          else {              /* ��4λ */
            desbits[i / 2] |= (unsigned char)descode & 0xf;
            k = -1;
          }
        }
      }

      soubits += soupatch;            /* ��ʼ��һ�� */
      desbits += despatch;
    }

    break;

  case  2:  /* 8λ��ɫ��ͼ */
    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        desbits[i] = (unsigned char)code & 0xff;
      }

      soubits += soupatch;            /* ��ʼ��һ�� */
      desbits += despatch;
    }

    break;

  case  3:  /* 16λ(555)λͼ */
    dmr = 0x7c00;                 /* ��Ϊ��׼16λ555��ʽ���� */
    dmg = 0x3e0;
    dmb = 0x1f;
    dma = 0;

    dmor = _get_mask_offset(dmr);         /* ���������0λ��ƫ���� */
    dmog = _get_mask_offset(dmg);
    dmob = _get_mask_offset(dmb);
    dmoa = _get_mask_offset(dma);

    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* �ӵ�ɫ�����з������ɫ����������С��555���� */
        sr = ((unsigned long)((unsigned char)(soupal[(int)code].red))) >> 3;
        sg = ((unsigned long)((unsigned char)(soupal[(int)code].green))) >> 3;
        sb = ((unsigned long)((unsigned char)(soupal[(int)code].blue))) >> 3;
        sa = 0;

        sr <<= dmor;
        sg <<= dmog;
        sb <<= dmob;
        sa <<= dmoa;

        /* �ϳ����� */
        descode = sr | sg | sb | sa;

        ((unsigned short*)desbits)[i] = (unsigned short)descode;
      }

      soubits += soupatch;            /* ��ʼ��һ�� */
      desbits += despatch;
    }

    break;

  case  4:  /* 16λ(565)λͼ */
    dmr = 0xf800;                 /* ��Ϊ��׼16λ565��ʽ���� */
    dmg = 0x7e0;
    dmb = 0x1f;
    dma = 0;

    dmor = _get_mask_offset(dmr);         /* ���������0λ��ƫ���� */
    dmog = _get_mask_offset(dmg);
    dmob = _get_mask_offset(dmb);
    dmoa = _get_mask_offset(dma);

    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* �ӵ�ɫ�����з������ɫ����������С��565���� */
        sr = ((unsigned long)((unsigned char)(soupal[(int)code].red))) >> 3;
        sg = ((unsigned long)((unsigned char)(soupal[(int)code].green))) >> 2;
        sb = ((unsigned long)((unsigned char)(soupal[(int)code].blue))) >> 3;
        sa = 0;

        sr <<= dmor;
        sg <<= dmog;
        sb <<= dmob;
        sa <<= dmoa;

        /* �ϳ����� */
        descode = sr | sg | sb | sa;

        ((unsigned short*)desbits)[i] = (unsigned short)descode;
      }

      soubits += soupatch;            /* ��ʼ��һ�� */
      desbits += despatch;
    }

    break;

  case  5:  /* 24λλͼ */
    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* �ӵ�ɫ�����з������ɫ���� */
        sr = (unsigned long)((unsigned char)(soupal[(int)code].red));
        sg = (unsigned long)((unsigned char)(soupal[(int)code].green));
        sb = (unsigned long)((unsigned char)(soupal[(int)code].blue));
        sa = 0;

        /* �ϳ����� */
        descode = (sa << 24) | (sr << 16) | (sg << 8) | sb;

        if ((i + 1) == width) {
          desbits[i * 3 + 0] = (unsigned char)sb;
          desbits[i * 3 + 1] = (unsigned char)sg;
          desbits[i * 3 + 2] = (unsigned char)sr;
        }
        else {
          *(unsigned long*)((unsigned char*)desbits + i * 3) = descode;
        }
      }

      soubits += soupatch;            /* ��ʼ��һ�� */
      desbits += despatch;
    }

    break;

  case  6:  /* 32λλͼ */
    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        ((unsigned long*)desbits)[i] = ((unsigned long*)soupal)[(int)code] & 0xffffff;
      }

      soubits += soupatch;            /* ��ʼ��һ�� */
      desbits += despatch;
    }

    break;

  default:  /* �Ƿ���ʽ */
    assert(0);
    return -1;
    break;
  }

  return 0;
}


/* 9~32λλͼת������ */
static int _rgbtox(
    int sbit,
    int dstd,
    unsigned char* soubits,
    LPPALITEM soupal,
    int soupatch,
    unsigned char* desbits,
    LPPALITEM despal,
    int despatch,
    int linenum,
    int width,
    unsigned long bmask,
    unsigned long gmask,
    unsigned long rmask,
    unsigned long amask)
{
  unsigned long code, descode;
  int       i, j, k;
  int       bits;               /* һ�������ص�λ���ۼ� */
  unsigned long rcou, offr, offg, offb, offa;
  unsigned long sr, sb, sg, sa;           /* Դ���ط��� */
  unsigned long smr, smg, smb, sma;         /* Դͼ������ */
  unsigned long dmr, dmg, dmb, dma;         /* Ŀ��ͼ������ */
  int       smor, smog, smob, smoa;       /* Դͼ������λ�� */
  int       dmor, dmog, dmob, dmoa;       /* Ŀ��ͼ������λ�� */


  smr = rmask;                    /* ����Դ���� */
  smg = gmask;
  smb = bmask;
  sma = amask;

  smor = _get_mask_offset(smr);           /* ���������0λ��ƫ���� */
  smog = _get_mask_offset(smg);
  smob = _get_mask_offset(smb);
  smoa = _get_mask_offset(sma);

  smr >>= smor;                   /* ��������뵽0λ */
  smg >>= smog;
  smb >>= smob;
  sma >>= smoa;

  switch (dstd) {
  case  0:  /* 1λ��ɫ��ͼ */
    while (linenum--) {
      j = -1;
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        if (j == -1) {
          desbits[i / 8] = 0;       /* ��ʼ��Ŀ���ֽ� */
          j = 7;
        }

        /* �������ɫ���� */
        sr = (code >> smor)&smr;        /* R */
        sg = (code >> smog)&smg;        /* G */
        sb = (code >> smob)&smb;        /* B */

        descode = (sr * 3 + sg * 6 + sb) / 10; /* ת��Ϊ�Ҷ����� */
        descode = (descode / 128) & 0x1;  /* תΪ2ɫ��ɫ�������� */

        desbits[i / 8] |= ((unsigned char)descode) << j;

        j--;
      }

      soubits += soupatch;            /* ��ʼ��һ�� */
      desbits += despatch;
    }

    break;

  case  1:  /* 4λ��ɫ��ͼ */
    while (linenum--) {
      k = -1;
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        if (k == -1) {
          desbits[i / 2] = 0;       /* ��ʼ��Ŀ���ֽ� */
          k = 0;
        }

        /* �������ɫ���� */
        sr = (code >> smor)&smr;        /* R */
        sg = (code >> smog)&smg;        /* G */
        sb = (code >> smob)&smb;        /* B */

        descode = (sr * 3 + sg * 6 + sb) / 10; /* ת��Ϊ�Ҷ����� */
        descode = (descode / 16) & 0xf;   /* תΪ16ɫ��ɫ�������� */

        if (k == 0) {             /* ��4λ */
          desbits[i / 2] |= ((unsigned char)descode) << 4;
          k = 1;
        }
        else {                /* ��4λ */
          desbits[i / 2] |= ((unsigned char)descode) & 0xf;
          k = -1;
        }
      }

      soubits += soupatch;            /* ��ʼ��һ�� */
      desbits += despatch;
    }

    break;

  case  2:  /* 8λ��ɫ��ͼ */
    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* �������ɫ���� */
        sr = (code >> smor)&smr;        /* R */
        sg = (code >> smog)&smg;        /* G */
        sb = (code >> smob)&smb;        /* B */

        descode = ((sr * 3 + sg * 6 + sb) / 10) & 0xff; /* ת��Ϊ�Ҷ����� */

        desbits[i] = (unsigned char)descode;
      }

      soubits += soupatch;            /* ��ʼ��һ�� */
      desbits += despatch;
    }

    break;

  case  3:  /* 16λ(555)λͼ */
    dmr = 0x7c00;                 /* ��Ϊ��׼16λ555��ʽ���� */
    dmg = 0x3e0;
    dmb = 0x1f;
    dma = 0;

    dmor = _get_mask_offset(dmr);         /* ���������0λ��ƫ���� */
    dmog = _get_mask_offset(dmg);
    dmob = _get_mask_offset(dmb);
    dmoa = _get_mask_offset(dma);

    dmr >>= dmor;                 /* ��������뵽0λ */
    dmg >>= dmog;
    dmb >>= dmob;
    dma >>= dmoa;

    offr = offg = offb = offa = 0;

    if (smr > dmr) {
      rcou = smr;

      while (rcou > dmr) {
        offr++;
        rcou >>= 1;
      }
    }
    else if (smr < dmr) {
      rcou = smr;

      while (rcou < dmr) {
        offr++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smg > dmg) {
      rcou = smg;

      while (rcou > dmg) {
        offg++;
        rcou >>= 1;
      }
    }
    else if (smg < dmg) {
      rcou = smg;

      while (rcou < dmg) {
        offg++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smb > dmb) {
      rcou = smb;

      while (rcou > dmb) {
        offb++;
        rcou >>= 1;
      }
    }
    else if (smb < dmb) {
      rcou = smb;

      while (rcou < dmb) {
        offb++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (sma > dma) {
      rcou = sma;

      while (rcou > dma) {
        offa++;
        rcou >>= 1;
      }
    }
    else if (sma < dma) {
      rcou = sma;

      while (rcou < dma) {
        offa++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* �������ɫ���� */
        sr = (code >> smor)&smr;        /* R */
        sg = (code >> smog)&smg;        /* G */
        sb = (code >> smob)&smb;        /* B */
        sa = (code >> smoa)&sma;        /* A */

        /* ���Դ������Χ����Ŀ�������Χ������������λ���Ƴ���λ */
        if (smr > dmr) {
          sr >>= offr;
        }
        else if (smr < dmr) {
          sr <<= offr;
        }

        if (smg > dmg) {
          sg >>= offg;
        }
        else if (smg < dmg) {
          sg <<= offg;
        }

        if (smb > dmb) {
          sb >>= offb;
        }
        else if (smb < dmb) {
          sb <<= offb;
        }

        if (sma > dma) {
          sa >>= offa;
        }
        else if (sma < dma) {
          sa <<= offa;
        }

        sr <<= dmor;
        sg <<= dmog;
        sb <<= dmob;
        sa <<= dmoa;

        /* �ϳ����� */
        descode = sr | sg | sb | sa;

        ((unsigned short*)desbits)[i] = (unsigned short)descode;
      }

      soubits += soupatch;            /* ��ʼ��һ�� */
      desbits += despatch;
    }

    break;

  case  4:  /* 16λ(565)λͼ */
    dmr = 0xf800;                 /* ��Ϊ��׼16λ565��ʽ���� */
    dmg = 0x7e0;
    dmb = 0x1f;
    dma = 0;

    dmor = _get_mask_offset(dmr);         /* ���������0λ��ƫ���� */
    dmog = _get_mask_offset(dmg);
    dmob = _get_mask_offset(dmb);
    dmoa = _get_mask_offset(dma);

    dmr >>= dmor;                 /* ��������뵽0λ */
    dmg >>= dmog;
    dmb >>= dmob;
    dma >>= dmoa;

    offr = offg = offb = offa = 0;

    if (smr > dmr) {                /* ������������Ҫ��չ����ѹ�� */
      rcou = smr;

      while (rcou > dmr) {
        offr++;
        rcou >>= 1;
      }
    }
    else if (smr < dmr) {
      rcou = smr;

      while (rcou < dmr) {
        offr++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smg > dmg) {
      rcou = smg;

      while (rcou > dmg) {
        offg++;
        rcou >>= 1;
      }
    }
    else if (smg < dmg) {
      rcou = smg;

      while (rcou < dmg) {
        offg++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smb > dmb) {
      rcou = smb;

      while (rcou > dmb) {
        offb++;
        rcou >>= 1;
      }
    }
    else if (smb < dmb) {
      rcou = smb;

      while (rcou < dmb) {
        offb++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (sma > dma) {
      rcou = sma;

      while (rcou > dma) {
        offa++;
        rcou >>= 1;
      }
    }
    else if (sma < dma) {
      rcou = sma;

      while (rcou < dma) {
        offa++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* �������ɫ���� */
        sr = (code >> smor)&smr;        /* R */
        sg = (code >> smog)&smg;        /* G */
        sb = (code >> smob)&smb;        /* B */
        sa = (code >> smoa)&sma;        /* A */

        /* ���Դ������Χ����Ŀ�������Χ������������λ���Ƴ���λ */
        /* ��������չ�������������� */
        if (smr > dmr) {
          sr >>= offr;
        }
        else if (smr < dmr) {
          sr <<= offr;
        }

        if (smg > dmg) {
          sg >>= offg;
        }
        else if (smg < dmg) {
          sg <<= offg;
        }

        if (smb > dmb) {
          sb >>= offb;
        }
        else if (smb < dmb) {
          sb <<= offb;
        }

        if (sma > dma) {
          sa >>= offa;
        }
        else if (sma < dma) {
          sa <<= offa;
        }

        sr <<= dmor;
        sg <<= dmog;
        sb <<= dmob;
        sa <<= dmoa;

        /* �ϳ����� */
        descode = sr | sg | sb | sa;

        ((unsigned short*)desbits)[i] = (unsigned short)descode;
      }

      soubits += soupatch;            /* ��ʼ��һ�� */
      desbits += despatch;
    }

    break;

  case  5:  /* 24λλͼ */
    dmr = 0xff0000;                 /* ��Ϊ��׼24λ��ʽ���� */
    dmg = 0xff00;
    dmb = 0xff;
    dma = 0;

    dmor = _get_mask_offset(dmr);         /* ���������0λ��ƫ���� */
    dmog = _get_mask_offset(dmg);
    dmob = _get_mask_offset(dmb);
    dmoa = _get_mask_offset(dma);

    dmr >>= dmor;                 /* ��������뵽0λ */
    dmg >>= dmog;
    dmb >>= dmob;
    dma >>= dmoa;

    offr = offg = offb = offa = 0;

    if (smr < dmr) {
      rcou = smr;

      while (rcou < dmr) {
        offr++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smg < dmg) {
      rcou = smg;

      while (rcou < dmg) {
        offg++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smb < dmb) {
      rcou = smb;

      while (rcou < dmb) {
        offb++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (sma < dma) {
      rcou = sma;

      while (rcou < dma) {
        offa++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* �������ɫ���� */
        sr = (code >> smor) & 0xff & smr; /* R */
        sg = (code >> smog) & 0xff & smg; /* G */
        sb = (code >> smob) & 0xff & smb; /* B */
        sa = (code >> smoa) & 0xff & sma; /* A */

        /* ���Դ������ΧС��Ŀ�������Χ���������չ�������ͼ������ */
        /* ��24λͼ����ɫ������8λ��������λ��ȣ������Բ����ٽ���  */
        /* ��С�ж� */
        if (smr < dmr) {
          sr <<= offr;
        }

        if (smg < dmg) {
          sg <<= offg;
        }

        if (smb < dmb) {
          sb <<= offb;
        }

        if (sma < dma) {
          sa <<= offa;
        }

        /* �ϳ����� */
        descode = (sa << 24) | (sr << 16) | (sg << 8) | sb;

        if ((i + 1) == width) {       /* �������أ������㷨��*/
          desbits[i * 3 + 0] = (unsigned char)sb;
          desbits[i * 3 + 1] = (unsigned char)sg;
          desbits[i * 3 + 2] = (unsigned char)sr;
        }
        else {
          *(unsigned long*)((unsigned char*)desbits + i * 3) = descode;
        }
      }

      soubits += soupatch;            /* ��ʼ��һ�� */
      desbits += despatch;
    }

    break;

  case  6:  /* 32λλͼ */
    dmr = 0xff0000;                 /* ��Ϊ��׼24λ��ʽ���� */
    dmg = 0xff00;
    dmb = 0xff;
    dma = 0xff000000;

    dmor = _get_mask_offset(dmr);         /* ���������0λ��ƫ���� */
    dmog = _get_mask_offset(dmg);
    dmob = _get_mask_offset(dmb);
    dmoa = _get_mask_offset(dma);

    dmr >>= dmor;                 /* ��������뵽0λ */
    dmg >>= dmog;
    dmb >>= dmob;
    dma >>= dmoa;

    offr = offg = offb = offa = 0;

    if (smr < dmr) {
      rcou = smr;

      while (rcou < dmr) {
        offr++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smg < dmg) {
      rcou = smg;

      while (rcou < dmg) {
        offg++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smb < dmb) {
      rcou = smb;

      while (rcou < dmb) {
        offb++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (sma < dma) {
      rcou = sma;

      while (rcou < dma) {
        offa++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* �������ɫ���� */
        sr = (code >> smor) & 0xff & smr; /* R */
        sg = (code >> smog) & 0xff & smg; /* G */
        sb = (code >> smob) & 0xff & smb; /* B */
        sa = (code >> smoa) & 0xff & sma; /* A */

        if (smr < dmr) {
          sr <<= offr;
        }

        if (smg < dmg) {
          sg <<= offg;
        }

        if (smb < dmb) {
          sb <<= offb;
        }

        if (sma < dma) {
          sa <<= offa;
        }

        /* �ϳ����� */
        descode = (sa << 24) | (sr << 16) | (sg << 8) | sb;

        ((unsigned long*)desbits)[i] = descode;
      }

      soubits += soupatch;            /* ��ʼ��һ�� */
      desbits += despatch;
    }

    break;

  default:  /* �Ƿ���ʽ */
    assert(0);
    return -1;
    break;
  }

  return 0;
}


/* ��ȡ��һ���������� */
static unsigned long _get_next_pix(
    unsigned char* p, /* ����ɨ���е����׵�ַ */
    int offset_byte,  /* ƫ���ֽ� */
    int offset_bit,   /* ƫ��λ�� */
    int bit_len)    /* ������ռλ�� */
{
  union {
    unsigned char b[8];
    unsigned long ul;
    __int64     up;
  } code;

  code.ul   = *(unsigned long*)(unsigned char*)(p + offset_byte);
  code.b[4] = *(unsigned char*)(p + offset_byte + 4);

  code.up >>= offset_bit;
  code.ul  &= (unsigned long)(((__int64)1 << bit_len) - 1);

  return code.ul;
}


/* ��ȡ����λ�ã����������Ƶ�0λʱ������ƶ������� */
/* ע������������������λ�ģ��������������󣬷���˺��������壡 */
static int _get_mask_offset(unsigned long mask)
{
  register result = 0;

  __asm {
    mov eax, mask;
    get_mask_offset_loop:
    shr eax, 1;
    jc  get_mask_offset_endloop;
    inc result;
    cmp result, 32;
    je  get_mask_offset_enderr;
    jmp get_mask_offset_loop;
    get_mask_offset_enderr:
    mov result, 0;
    get_mask_offset_endloop:
  }

  return (int)result;
}


