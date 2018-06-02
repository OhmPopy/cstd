
#ifndef __ISEE_IRW_INC__
#define __ISEE_IRW_INC__



/* ISeeͼ���д���浱ǰ�汾 */
#define ISEEIRW_ENG_VER     20

/* ��ǰ��ISeeͼ���д����ʹ�õĲ���ӿڰ汾 */
#define ISEEIRW_ENG_PLUGIN_VER  22

/* ISeeͼ���д���֧�ֵ����λ��ֵ */
#define ISEEIRW_MAX_BITCOUNT  32

/* ISeeͼ���д����֧�ֵ��������Ŀ */
#define ISEEIRW_MAX_PLUGIN_CNT  128

/* ������˴����������ߴ磨ÿ������Ĺ�����ռ��24���ֽڣ�*/
#define ISIRW_OPENSTR_SIZE    (ISEEIRW_MAX_PLUGIN_CNT*24)

/* �߳��л��ȴ��Ŀ���ʱ�䳤�ȣ�ms��*/
#define ISIRW_THREAD_IDLE   10


/* ͼ���дģ��ִ�н�� */
enum EXERESULT {
  ER_SUCCESS,     /* ִ�гɹ� */
  ER_USERBREAK,   /* �������û��ж� */
  ER_BADIMAGE,    /* ָ�����ļ���һ�������ͼ���ļ� */
  ER_NONIMAGE,    /* ָ�����ļ�����һ��ͼ���ļ� */
  ER_MEMORYERR,   /* �ڴ治�㣬���²���ʧ�� */
  ER_FILERWERR,   /* �ļ��ڶ�д�����з�������ϵͳ��Ӳ����ԭ�������ļ���д�쳣�� */
  ER_SYSERR,      /* ����ϵͳ���ȶ����޷�ִ������ */
  ER_NOTSUPPORT,    /* ��֧�ֵ����� */
  ER_NSIMGFOR,    /* ���湦�ܲ�֧�ֵ�ͼ���ʽ */
  ER_WAITOUTTIME,   /* ��д������ʱ�������˳����� */
  /*ER_XXXX,*/    /* �µĴ�����Ϣ�ڴ˲��� */
  ER_MAX = 64     /* �߽�ֵ */
};


/* ͼ������ */
enum IMGTYPE {
  IMT_NULL,     /* ��Чͼ������ */
  IMT_RESSTATIC,    /* ��դ����̬ͼ�� */
  IMT_RESDYN,     /* ��դ����̬ͼ�� */
  IMT_VECTORSTATIC, /* ʸ������̬ͼ�� */
  IMT_VECTORDYN,    /* ʸ������̬ͼ�� */
  IMT_CNV,      /* ת��������ͼ�� */
  /*IMT_XXXXX,*/    /* �ڴ˲����µ�ͼ������ */
  IMT_MAX = 64    /* �߽�ֵ */
};


/* ͼ���ʽ */
enum IMGFORMAT {
  IMF_NULL,     /* δ֪����Ч��ͼ�� */
  IMF_BMP,      /* BMP��ʽ��ͼ�� */
  IMF_CUR,      /* .... */
  IMF_ICO,
  IMF_PCX,
  IMF_DCX,
  IMF_GIF,
  IMF_PNG,
  IMF_TGA,
  IMF_WMF,
  IMF_EMF,
  IMF_TIFF,
  IMF_JPEG,     /* ... */
  IMF_PSD,      /* PSD��ʽ��ͼ�� */
  IMF_XBM,
  IMF_XPM,
  IMF_PCD,
  IMF_RAW,
  IMF_RAS,
  IMF_PPM,
  IMF_PGM,
  IMF_PBM,
  IMF_IFF,      /* ... */
  IMF_JP2,      /* JPEG2000��ʽͼ�� */
  IMF_LBM,
  IMF_BW,
  IMF_PIX,
  IMF_RLE,
  IMF_XWD,
  IMF_DXF,
  IMF_WBMP,
  IMF_ANI,
  IMF_ART,
  IMF_FPX,
  IMF_ICN,      /* ... */
  IMF_KDC,
  IMF_LDF,
  IMF_LWF,
  IMF_MAG,
  IMF_PIC,
  IMF_PCT,
  IMF_PDF,
  IMF_PS,
  IMF_PSP,
  IMF_RSB,
  IMF_SGI,
  IMF_RLA,
  IMF_RLB,
  IMF_RLC,
  IMF_JBIG,
  IMF_APM,
  IMF_MNG,
  IMF_CGM,
  IMF_PAL,
  IMF_YUV,
  IMF_DJVU,
  IMF_DICOM,
  IMF_LCD,
  /*IMF_XXX*/     /* ������ʽ�ڴ˴����� */
  IMF_MAX = 511   /* �߽�ֵ */
};



/* �������ݵĴ����ʽ */
enum IMGCOMPRESS {
  ICS_RGB,        /* δ��ѹ����RGB�洢��ʽ */
  ICS_RLE4,       /* RLE4�洢��ʽ */
  ICS_RLE8,       /* RLE8�洢��ʽ */
  /* @@@@@@@@@@Added By xiaoyueer 2000.8.20 */
  ICS_RLE16,        /* RLE16�洢��ʽ */
  ICS_RLE24,        /* RLE24�洢��ʽ */
  ICS_RLE32,        /* RLE32�洢��ʽ */
  /* @@@@@@@@@End 2000.8.20 */
  ICS_BITFIELDS,      /* ��λ�洢��ʽ */
  ICS_PCXRLE,       /* PCX RLE�洢��ʽ */
  ICS_GIFLZW,       /* GIF LZW�洢��ʽ */
  ICS_GDIRECORD,      /* WMF��Ԫ�ļ��洢��ʽ */
  /* @@@@@@@@@@Added By orbit 2000.8.31 */
  ICS_JPEGGRAYSCALE,    /* �Ҷ�ͼ��,JPEG-256���Ҷ� */
  ICS_JPEGRGB,      /* red/green/blue */
  ICS_JPEGYCbCr,      /* Y/Cb/Cr (also known as YUV) */
  ICS_JPEGCMYK,     /* C/M/Y/K */
  ICS_JPEGYCCK,     /* Y/Cb/Cr/K */
  /* @@@@@@@@@End 2000.8.31 */
  /* @@@@@@@@@@LibTiff: Sam Leffler 2001.1.6 */
  ICS_TIFF_NONE,      /* TIFF non-compress ABGR mode */
  ICS_TIFF_CCITTRLE,    /* CCITT modified Huffman RLE */
  ICS_TIFF_CCITTAX3,    /* CCITT Group 3 fax encoding */
  ICS_TIFF_CCITTFAX4,   /* CCITT Group 4 fax encoding */
  ICS_TIFF_LZW,     /* Lempel-Ziv  & Welch */
  ICS_TIFF_OJPEG,     /* 6.0 JPEG */
  ICS_TIFF_JPEG,      /* JPEG DCT compression */
  ICS_TIFF_NEXT,      /* NeXT 2-bit RLE */
  ICS_TIFF_CCITTRLEW,   /* 1 w/ word alignment */
  ICS_TIFF_PACKBITS,    /* Macintosh RLE */
  ICS_TIFF_THUNDERSCAN, /* ThunderScan RLE */
  /* @@@@@@@@@@Added By <dkelly@etsinc.com> */
  ICS_TIFF_IT8CTPAD,    /* IT8 CT w/padding */
  ICS_TIFF_IT8LW,     /* IT8 Linework RLE */
  ICS_TIFF_IT8MP,     /* IT8 Monochrome picture */
  ICS_TIFF_IT8BL,     /* IT8 Binary line art */
  ICS_TIFF_PIXARFILM,   /* Pixar companded 10bit LZW */
  ICS_TIFF_PIXARLOG,    /* Pixar companded 11bit ZIP */
  ICS_TIFF_DEFLATE,   /* Deflate compression */
  ICS_TIFF_ADOBE_DEFLATE, /* Deflate compression, as recognized by Adobe */
  /* @@@@@@@@@@Added By <dev@oceana.com> */
  ICS_TIFF_DCS,     /* Kodak DCS encoding */
  ICS_TIFF_JBIG,      /* ISO JBIG */
  ICS_TIFF_SGILOG,    /* SGI Log Luminance RLE */
  ICS_TIFF_SGILOG24,    /* SGI Log 24-bit packed */
  /* @@@@@@@@@End 2001.1.6 */
  /* @@@@@@@@@@Added By YZ 2001.8.3 */
  ICS_PNG_GRAYSCALE,    /* PNG�Ҷ�ͼ */
  ICS_PNG_PALETTE,    /* PNG����ͼ */
  ICS_PNG_RGB,      /* PNG���ͼ */
  /* @@@@@@@@@End 2001.8.3 */
  ICS_XBM_TEXT,     /* �ı���ʽ */
  ICS_XPM_TEXT,
  /* @@@@@@@@@@Added By YZ 2002.2.3 */
  ICS_PNM_TEXT,
  ICS_PNM_BIN,
  /* @@@@@@@@@End 2002.2.3 */
  /* @@@@@@@@@@Added By YZ 2002.5.30 */
  ICS_JBIG,
  /* @@@@@@@@@End 2002.5.30 */
  /*ICS_XXXX,*/     /* �����µĴ洢��ʽ���� */
  ICS_UNKONW = 1024,    /* δ֪��ѹ����ʽ */
  ICS_MAX = 2048      /* �߽� */
};




#define AI_NAME_SIZE    32
#define AI_EMAIL_SIZE   64
#define AI_MESSAGE_SIZE   160

/* ��������Ϣ�ṹ */
struct _tag_author_info {
  char ai_name[AI_NAME_SIZE];           /* ���������� */
  char ai_email[AI_EMAIL_SIZE];         /* ������email��ַ */
  char ai_message[AI_MESSAGE_SIZE];       /* ���������ԡ����������������159��*/
  /* Ӣ���ַ�����79�����֣�*/
};




#define IDI_CURRENCY_NAME_SIZE  16
#define IDI_SYNONYM_NAME_COUNT  4

/* ����ܴ����ͼ���ļ�������Ϣ�ṹ������չ����Ϣ�� */
struct _tag_irwp_desc_info {
  char idi_currency_name[IDI_CURRENCY_NAME_SIZE]; /* ͨ����չ�������� */
  unsigned long idi_rev;              /* ���� */
  unsigned long idi_synonym_count;        /* ������չ����Ч����� */
  char idi_synonym[IDI_SYNONYM_NAME_COUNT][IDI_CURRENCY_NAME_SIZE];
  /* ������չ�� */
};



#define ISPD_ITEM_NUM     16
#define ISPD_STRING_SIZE    32

/* ���湦��ѡ����� */
struct _tag_irw_save_para_desc {
  int     count;                /* ��Ч��������� */
  char    desc[ISPD_STRING_SIZE];       /* �ṹ������ */

  int     value[ISPD_ITEM_NUM];       /* ֵ�б� */
  char    value_desc[ISPD_ITEM_NUM][ISPD_STRING_SIZE];/* ��Ӧ��ֵ������ */
};  /* 612 byte */


#define ISD_ITEM_NUM      4

#define IRWP_NAME_SIZE      32          /* ������Ƶ���󳤶� */
#define IRWP_AUTHOR_SIZE    16          /* ��һ���������������������� */
#define IRWP_FUNC_PREFIX_SIZE 16          /* ����ǰ׺�����ߴ� */

#define IRWP_READ_SUPP      0x1         /* ��֧�� */
#define IRWP_WRITE_SUPP     0x2         /* д֧�� */
#define IRWP_REREAD_SUPP    0x4         /* �ض�֧�֣���ǰδʵ�֣�*/


/* ����ˢ�º�����ˢ��ģʽ */
#define IRWE_UPDTYPE_EMPLOY   0         /* ¼�������²�� */
#define IRWE_UPDTYPE_DISMISSAL  1         /* ��¼���²�� */

/* ����ˢ�º���������Ϣ�ṹ */
struct _tag_irw_update_info {
  int   newcnt;                 /* ��������ĸ��� */
  void*  newpp[ISEEIRW_MAX_PLUGIN_CNT];     /* ��������Ĳ����Ϣ��ṹ��ַ��������ÿһ��Ԫ�ض���һ�������Ϣ��ṹ�ĵ�ַ������ת��ΪLPIRWMF_INFOBLOCK���ͣ�*/
  int   updcnt;                 /* ˢ�£�������������ĸ��� */
  void*  updpp[ISEEIRW_MAX_PLUGIN_CNT];     /* ˢ�²���Ĳ����Ϣ��ṹ��ַ */
  int   delcnt;                 /* ��ע������ĸ��� */
  char  delpp[ISEEIRW_MAX_PLUGIN_CNT][IRWP_NAME_SIZE];  /* ��ע������Ĳ�����ƴ� */
};


#define MAX_PALETTE_COUNT   256         /* ����ɫ�������  */

#ifdef _DEBUG
#define INFOSTR_DBG_MARK    0x65655349      /* ͼ��ṹ��ʶ��"ISee" */
#endif  /*_DEBUG*/



/* ����ͼ��ʱ�����趨�����Ľṹ */
struct _tag_savestr {
  int     para_value[ISD_ITEM_NUM]; /* ���������Ӧֵ����IRW_SAVE_DESC�ṹ�� */
};



/* �֡�˫�ֶ���꣨DIBSCANLINE_WIDTHBYTES�����Ŀ�Ķ��������� */
#define DIBSCANLINE_WORDALIGN(bits)   ((((bits)+15)>>4)<<1)
#define DIBSCANLINE_DWORDALIGN(bits)    ((((bits)+31)>>5)<<2)
#define DIBSCANLINE_WIDTHBYTES(bits)    ((((bits)+31)>>5)<<2)


/* 68K������X86���򻥻��� */
#define EXCHANGE_DWORD(dword) (((dword&0xff)<<24)|((dword&0xff00)<<8)|((dword&0xff0000)>>8)|((dword&0xff000000)>>24))
#define EXCHANGE_WORD(word)   (((word&0xff)<<8)|((word&0xff00)>>8))


/* 1��2��4λ�����ֽ���λ�öԵ��� */
#define CVT_BITS4(c) \
  {\
    c = (unsigned char)((((unsigned char)c)>>4) | (((unsigned char)c)<<4));\
  }

#define CVT_BITS2(c) \
  {\
    c = (unsigned char)( ((((unsigned char)c)>>6)&0x3) | ((((unsigned char)c)>>2)&0xc) | \
        ((((unsigned char)c)<<2)&0x30)| ((((unsigned char)c)<<6)&0xc0)  );\
  }

#define CVT_BITS1(c) \
  {\
    c = (unsigned char)( ((((unsigned char)c)>>7)&0x1) | ((((unsigned char)c)>>5)&0x2) | \
        ((((unsigned char)c)>>3)&0x4) | ((((unsigned char)c)>>1)&0x8) | \
        ((((unsigned char)c)<<1)&0x10)| ((((unsigned char)c)<<3)&0x20)| \
        ((((unsigned char)c)<<5)&0x40)| ((((unsigned char)c)<<7)&0x80) );\
  }


/* �������ú꣨p -> LPINFOSTR�� */
#define SETMASK_32(p)   \
  {\
    p->b_mask = 0xff;   \
    p->g_mask = 0xff00;   \
    p->r_mask = 0xff0000; \
    p->a_mask = 0xff000000; \
  }
#define SETMASK_24(p)   \
  {\
    p->b_mask = 0xff;   \
    p->g_mask = 0xff00;   \
    p->r_mask = 0xff0000; \
    p->a_mask = 0x0;    \
  }
#define SETMASK_8(p)    \
  {\
    p->b_mask = 0x0;    \
    p->g_mask = 0x0;    \
    p->r_mask = 0x0;    \
    p->a_mask = 0x0;    \
  }


/* CMYKɫתRGB�꣨���ֲڵ�ת����*/
#define CVT_CMYK_TO_RGB(c, m, y, k, r, g, b)  \
  {\
    unsigned short k1 = 255 - (unsigned short)(unsigned char)k; \
    r = (unsigned char)((c > k1) ? (c-k1) : 0); \
    g = (unsigned char)((m > k1) ? (m-k1) : 0); \
    b = (unsigned char)((y > k1) ? (y-k1) : 0); \
  }

/* YUVתRGB����ĳ������� */
#define irw_yuv_cr1   ((double)((double) 0.0054980  * 256.0))
#define irw_yuv_cr2   ((double)((double) 0.0000001  * 256.0))
#define irw_yuv_cr3   ((double)((double) 0.0051681  * 256.0))
#define irw_yuv_cg1   ((double)((double) 0.0054980  * 256.0))
#define irw_yuv_cg2   ((double)((double)-0.0015446  * 256.0))
#define irw_yuv_cg3   ((double)((double)-0.0026325  * 256.0))
#define irw_yuv_cb1   ((double)((double) 0.0054980  * 256.0))
#define irw_yuv_cb2   ((double)((double) 0.0079533  * 256.0))
#define irw_yuv_cb3   ((double)((double) 0.0000001  * 256.0))

#include <math.h>

/* YUV(YCbCr)תRGB�� */
#define YUV2RGB(y, cb, cr, pr, pg, pb)    \
  {\
    double  t;  \
    t = floor(irw_yuv_cr1 * y + irw_yuv_cr2 * (cb - 156) + irw_yuv_cr3 * (cr - 137) + 0.5);\
    *pr = (unsigned char)((t < 0) ? 0 : (t > 255) ? 255 : t);\
    t = floor(irw_yuv_cg1 * y + irw_yuv_cg2 * (cb - 156) + irw_yuv_cg3 * (cr - 137) + 0.5);\
    *pg = (unsigned char)((t < 0) ? 0 : (t > 255) ? 255 : t);\
    t = floor(irw_yuv_cb1 * y + irw_yuv_cb2 * (cb - 156) + irw_yuv_cb3 * (cr - 137) + 0.5);\
    *pb = (unsigned char)((t < 0) ? 0 : (t > 255) ? 255 : t);\
  }




/* ��ɫ����ṹ */
typedef struct _tag_palette_item {
  unsigned char blue;
  unsigned char green;
  unsigned char red;
  unsigned char reserved;
} PALITEM, *LPPALITEM;



/* Windows ƽ̨���ݽṹ��ֲ */
#ifndef WIN32

typedef struct _tag_rgb_quad {
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbRed;
  unsigned char rgbReserved;
} RGBQUAD, LPRGBQUAD;

typedef struct _tag_bitmap_info_header {
  unsigned long biSize;
  long      biWidth;
  long      biHeight;
  unsigned short  biPlanes;
  unsigned short  biBitCount;
  unsigned long biCompression;
  unsigned long biSizeImage;
  long      biXPelsPerMeter;
  long      biYPelsPerMeter;
  unsigned long biClrUsed;
  unsigned long biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER;

typedef struct _tag_bitmap_info {
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD          bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO;

#endif /* WIN32 */


#define DATAPACK_MARK_FACILITY      0x44504D46    /* �������ݰ���ǩ��DPMF��*/
#define DATAPACK_MARK_SPECIALTY     0x44504D53    /* רҵ���ݰ���ǩ��DPMS��*/


/*********************************************************************************/

#if 0
/* ���壨ҳ��ͼ�����ͼ���ṹ */
struct _tag_subimage_block {
  int   number;         /* ��ͼ�������к� */

  int   left, top;        /* ��ͼ������ʾλ�ã����Ͻǣ�*/
  int   width, height;      /* ��ͼ����ͼ���ȼ��߶� */
  int   bitcount;       /* ��ͼ���λ�� */
  int   order;          /* ͼ�������˳�� ��0������1������*/

  int   dowith;         /* ���÷�����ֻ��Զ�̬ͼ��*/
  int   userinput;        /* �û����루ֻ��Զ�̬ͼ��*/
  int   time;         /* ��ʾ���ӳ�ʱ�䣨ֻ��Զ�̬ͼ�� */

  unsigned long r_mask;     /* ����ɫ����������ֵ */
  unsigned long g_mask;     /* ע��1~8λ��ͼ������������ */
  unsigned long b_mask;
  unsigned long a_mask;

  long      colorkey;   /* ͸��ɫ����ֵ */

  void**      pp_line_addr; /* ��׼ͼ��λ�����д�0�е�n-1�е�ÿһ�е��׵�ַ */
  unsigned char* p_bit_data;  /* ��׼ͼ��λ���ݻ������׵�ַ */
  int       pal_count;    /* ��ɫ����Ч��ĸ��� */
  unsigned long palette[MAX_PALETTE_COUNT];   /* ��ɫ������ */

  INFOSTR*    parents;    /* ��һ��ͼ�����ݵĽṹ��ַ���������ݰ��ṹ�ĵ�ַ��*/
  LPSUBIMGBLOCK prev;     /* ǰһ����ͼ�����ݵĽṹ��ַ */
  LPSUBIMGBLOCK next;     /* ��һ����ͼ�����ݵĽṹ��ַ��NULL��ʾ�������һ��ͼ��*/
};
#endif


/* �����ص������ */
typedef enum _tag_pfun_command {
  PFC_NULL,           /* ������ */
  PFC_INTO_PLUG,          /* ��ʼִ������ */
  PFC_BUILD_MAST_IMAGE,     /* ������ͼ�� */
  PFC_BUILD_SERT_IMAGE,     /* �����µ���ͼ�� */
  PFC_PROCESS,          /* ���� */
  PFC_OVER_SIR,         /* ����һ��ͼ��Ĳ�������ͼ�����ͼ��*/
  PFC_OK,             /* ����ɹ�ִ�� */
  PFC_BREAK,            /* ����ж� */
} PFUN_COMM, *LPPFUN_COMM;

/* ͼ���д���ݰ��ṹ�����Ľṹ�� */
typedef struct INFOSTR {
#ifdef _DEBUG
  unsigned long sct_mark;   /* �ṹ�ı�ʶ�����ڵ��ԣ�����INFOSTR_DBG_MARK */
#endif

  int imgtype;    /* ͼ���ļ����� */
  int imgformat;    /* ͼ���ļ���ʽ�������ʶ�� */
  int compression;  /* ͼ���ļ���ѹ����ʽ */

  unsigned long width;      /* ͼ���� */
  unsigned long height;     /* ͼ��߶� */
  unsigned long bitcount;   /* ÿ��������ռ��λ����λ��ȣ� */
  unsigned long order;      /* ͼ�������˳�� ��0������1������*/

  unsigned long r_mask;     /* ����ɫ����������ֵ */
  unsigned long g_mask;     /* ע��1~8λ��ͼ������������ */
  unsigned long b_mask;
  unsigned long a_mask;

  unsigned int  pal_count;    /* ��ɫ����Ч��ĸ��� */
  unsigned long palette[MAX_PALETTE_COUNT];   /* ��ɫ������ */

  //unsigned long play_number;  /* ����˳���ߴ磨��Ԫ��Ϊ��λ�� */
  //unsigned long *play_order;  /* ����˳��� */
  unsigned long time;     /* ��֡ͣ��ʱ�� */
  long      colorkey;   /* ��ͼ��͸��ɫ��-1��ʾ��͸��ɫ�� */

  unsigned long imgnumbers;   /* ���ļ���ͼ��ĸ��� */
  //SUBIMGBLOCK psubimg;    /* ��ͼ����������ַ */

  unsigned short  data_state;   /* ���ݰ���ǰ������״̬��
                      0 �� �յ����ݰ���û���κα�����������Ч��
                      1 �� ����ͼ���������Ϣ
                      2 �� ����ͼ������
                  */

  int         omnipotence;/* ����;��������ת�����д�ű�׼��ʽID�� */

  long        rev[28];  /* ���� */
} INFOSTR;


/* רҵ���ݰ�ִ��״̬ */
typedef enum {
  IRWE_SDPSTATE_WAIT = 0,
  IRWE_SDPSTATE_EXEC = 1,
  IRWE_SDPSTATE_IDLE = 2
} SDP_EXEC_STATE, *LPSDPSTATE;


/* רҵ���ݰ�����״̬ */
typedef unsigned long       SDP_DATA_STATE;

#define SDP_DATA_STATE_PATH     0x1
#define SDP_DATA_STATE_ORG_INFO   0x2
#define SDP_DATA_STATE_ORG_PIX    0x4
#define SDP_DATA_STATE_CNV      0x8


/* רҵ���ݰ���ִ����������� */
typedef enum {
  IRWE_SDPCOMM_NULL,
  IRWE_SDPCOMM_GET_INFO,
  IRWE_SDPCOMM_LOAD,
  IRWE_SDPCOMM_SAVE
} SDP_COMM_TYPE, *LPSDP_COMM_TYPE;


/* רҵ���ݰ�ת������ֵ���� */
typedef enum _tag_cnvmark {
  IRWE_CNVMARK_NOT    = -2,     /* �������κ�ת�� */
  IRWE_CNVMARK_AUTO   = -1,     /* �Զ�תΪ��ƥ��ı�׼��ʽ */
  IRWE_CNVMARK_1      = 0,      /* ǿ��תΪ 1 λ��ɫ��ͼ */
  IRWE_CNVMARK_4      = 1,      /* ǿ��תΪ 4 λ��ɫ��ͼ */
  IRWE_CNVMARK_8      = 2,      /* ǿ��תΪ 8 λ��ɫ��ͼ */
  IRWE_CNVMARK_16_555 = 3,      /* ǿ��תΪ 16 λ(555)��ʽλͼ */
  IRWE_CNVMARK_16_565 = 4,      /* ǿ��תΪ 16 λ(565)��ʽλͼ */
  IRWE_CNVMARK_24     = 5,      /* ǿ��תΪ 24 λλͼ */
  IRWE_CNVMARK_32     = 6       /* ǿ��תΪ 32 λλͼ(��Alpha����) */
} IRWE_CNVMARK, *LPIRWE_CNVMARK;


/* �ص���������ID */
#define IRWE_CALLBACK_FUNID_LOAD  0 /* ��ȡ���� */
#define IRWE_CALLBACK_FUNID_SAVE  1 /* ���溯�� */


/* ���ó��ȵ�λ */
#define IRWE_KB             1024
#define IRWE_MB             (IRWE_KB*IRWE_KB)

/* �����ڲ���������󳤶� */
#define IRWE_MAX_BUFFSIZE       (10*IRWE_MB)  /* ȱʡΪ10MB */

/* �����ڲ����忪ʼ���� */
#define IRWE_BUFTYPE_ORG        1       /* ԭʼ���ػ����� */
#define IRWE_BUFTYPE_CNV        2       /* ת�����ػ����� */


/* IRWP API ���Ͷ��� */
//enum IRWP_API_NAME {  get_image_info = 1,  load_image,  save_image};


/* ����Ľӿڲ��������һ�β����Ĵ���ֵ���� */
#define ISIRW_LASTERR_SUCCESS     0
#define ISIRW_LASTERR_SYSCLOSE      1       /* ����ϵͳδ���� */
#define ISIRW_LASTERR_MEM       2       /* �ڴ治�� */
#define ISIRW_LASTERR_RW        3       /* ���ʶ�д���� */
#define ISIRW_LASTERR_USER        4       /* �������û��ж� */
#define ISIRW_LASTERR_IMGERR      5       /* ͼ�����(������ͼ��) */
#define ISIRW_LASTERR_OSERR       10        /* ����ϵͳ���� */


#if 0



/* ��������Ϣ�ṹ */
struct _tag_author_info;
typedef struct _tag_author_info  AUTHOR_INFO;
typedef struct _tag_author_info*  LPAUTHOR_INFO;


/* ����ܴ����ͼ���ļ�������Ϣ�ṹ */
struct _tag_irwp_desc_info;
typedef struct _tag_irwp_desc_info  IRWP_DESC_INFO;
typedef struct _tag_irwp_desc_info*  LPIRWP_DESC_INFO;


/* ���������Ϣ�ṹ */
struct _tag_irwp_info;
typedef struct _tag_irwp_info IRWP_INFO;
typedef struct _tag_irwp_info* LPIRWP_INFO;


/* ���湦�ܲ����趨���� */
struct _tag_irw_save_para_desc;
typedef struct _tag_irw_save_para_desc  IRW_SAVE_PARA_DESC;
typedef struct _tag_irw_save_para_desc*  LPIRW_SAVE_PARA_DESC;


/* ���湦������ */
struct _tag_irw_save_desc;
typedef struct _tag_irw_save_desc IRW_SAVE_DESC;
typedef struct _tag_irw_save_desc* LPIRW_SAVE_DESC;


/* ����ͼ�����ͼ���ṹ����Ҫ���ڶ�����ͼ���ļ���*/
struct _tag_subimage_block;
typedef struct _tag_subimage_block  SUBIMGBLOCK;
typedef struct _tag_subimage_block*  LPSUBIMGBLOCK;

/* ����ͼ��ʱ�����趨�����Ľṹ */
struct _tag_savestr;
typedef struct _tag_savestr   SAVESTR;
typedef struct _tag_savestr*   LPSAVESTR;


/* ����ˢ�º���������Ϣ�ṹ */
struct _tag_irw_update_info;
typedef struct _tag_irw_update_info UPDATAINFO;
typedef struct _tag_irw_update_info* LPUPDATAINFO;

#endif


#endif  //__ISEE_IRW_INC__
