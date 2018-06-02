
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


#define __LEAVE if (ER_SUCCESS != b_status) { break; }
#define IF_LEAVE(X) if (ER_SUCCESS != (b_status=(X))) { break; }
/* ͼ���дģ��ִ�н�� */
typedef enum EXERESULT {
  ER_SUCCESS,       /* ִ�гɹ� */
  ER_USERBREAK,     /* �������û��ж� */
  ER_BADIMAGE,      /* ָ�����ļ���һ�������ͼ���ļ� */
  ER_NONIMAGE,      /* ָ�����ļ�����һ��ͼ���ļ� */
  ER_MEMORYERR,     /* �ڴ治�㣬���²���ʧ�� */
  ER_FILERWERR,     /* �ļ��ڶ�д�����з�������ϵͳ��Ӳ����ԭ�������ļ���д�쳣�� */
  ER_SYSERR,        /* ����ϵͳ���ȶ����޷�ִ������ */
  ER_NOTSUPPORT,    /* ��֧�ֵ����� */
  ER_NSIMGFOR,      /* ���湦�ܲ�֧�ֵ�ͼ���ʽ */
  ER_WAITOUTTIME,   /* ��д������ʱ�������˳����� */
  /*ER_XXXX,*/      /* �µĴ�����Ϣ�ڴ˲��� */
  ER_MAX = 64       /* �߽�ֵ */
} EXERESULT;


/* ͼ������ */
typedef enum {
  IMT_NULL,         /* ��Чͼ������ */
  IMT_RESSTATIC,    /* ��դ����̬ͼ�� */
  IMT_RESDYN,       /* ��դ����̬ͼ�� */
  IMT_VECTORSTATIC, /* ʸ������̬ͼ�� */
  IMT_VECTORDYN,    /* ʸ������̬ͼ�� */
  IMT_CNV,          /* ת��������ͼ�� */
  /*IMT_XXXXX,*/    /* �ڴ˲����µ�ͼ������ */
  IMT_MAX = 64      /* �߽�ֵ */
} IMGTYPE;


/* ͼ���ʽ */
typedef enum {
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
  IMF_IFF,
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
  IMF_ICN,
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
} IMGFORMAT;



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

#define IDI_CURRENCY_NAME_SIZE  16
#define IDI_SYNONYM_NAME_COUNT  4

#define ISPD_ITEM_NUM     16
#define ISPD_STRING_SIZE    32

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

#define MAX_PALETTE_COUNT   256         /* ����ɫ�������  */

#ifdef _DEBUG
#define INFOSTR_DBG_MARK    0x65655349      /* ͼ��ṹ��ʶ��"ISee" */
#endif  /*_DEBUG*/


/* �֡�˫�ֶ���꣨DIBSCANLINE_WIDTHBYTES�����Ŀ�Ķ��������� */
#define DIBSCANLINE_WORDALIGN(bits)   ((((bits)+15)>>4)<<1)
#define DIBSCANLINE_DWORDALIGN(bits)    ((((bits)+31)>>5)<<2)
#define DIBSCANLINE_WIDTHBYTES(bits)    ((((bits)+31)>>5)<<2)

/* ����ɨ���гߴ�(���ֽڶ���) */
#define _calcu_scanline_size(w/* ��� */, bit/* λ�� */)  DIBSCANLINE_WIDTHBYTES(w * bit)

/* 68K������X86���򻥻��� */
#define EXCHANGE_DWORD(dword) (((dword&0xff)<<24)|((dword&0xff00)<<8)|((dword&0xff0000)>>8)|((dword&0xff000000)>>24))
#define EXCHANGE_WORD(word)   (((word&0xff)<<8)|((word&0xff00)>>8))


/* 1��2��4λ�����ֽ���λ�öԵ��� */
#define CVT_BITS4(c)   { c = (uchar)((((uchar)c)>>4) | (((uchar)c)<<4)); }

#define CVT_BITS2(c)  {\
    c = (uchar)( ((((uchar)c)>>6)&0x3) | ((((uchar)c)>>2)&0xc) | \
        ((((uchar)c)<<2)&0x30)| ((((uchar)c)<<6)&0xc0)  );\
  }

#define CVT_BITS1(c)   {\
    c = (uchar)( ((((uchar)c)>>7)&0x1) | ((((uchar)c)>>5)&0x2) | \
        ((((uchar)c)>>3)&0x4) | ((((uchar)c)>>1)&0x8) | \
        ((((uchar)c)<<1)&0x10)| ((((uchar)c)<<3)&0x20)| \
        ((((uchar)c)<<5)&0x40)| ((((uchar)c)<<7)&0x80) );\
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
    ushort k1 = 255 - (ushort)(uchar)k; \
    r = (uchar)((c > k1) ? (c-k1) : 0); \
    g = (uchar)((m > k1) ? (m-k1) : 0); \
    b = (uchar)((y > k1) ? (y-k1) : 0); \
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



/* YUV(YCbCr)תRGB�� */
#define YUV2RGB(y, cb, cr, pr, pg, pb)    \
  {\
    double  t;  \
    t = floor(irw_yuv_cr1 * y + irw_yuv_cr2 * (cb - 156) + irw_yuv_cr3 * (cr - 137) + 0.5);\
    *pr = (uchar)((t < 0) ? 0 : (t > 255) ? 255 : t);\
    t = floor(irw_yuv_cg1 * y + irw_yuv_cg2 * (cb - 156) + irw_yuv_cg3 * (cr - 137) + 0.5);\
    *pg = (uchar)((t < 0) ? 0 : (t > 255) ? 255 : t);\
    t = floor(irw_yuv_cb1 * y + irw_yuv_cb2 * (cb - 156) + irw_yuv_cb3 * (cr - 137) + 0.5);\
    *pb = (uchar)((t < 0) ? 0 : (t > 255) ? 255 : t);\
  }

/* ��ɫ����ṹ */
typedef struct _tag_palette_item {
  uchar blue;
  uchar green;
  uchar red;
  uchar reserved;
} PALITEM;

/////////////////////////////////////////////////////////////////////////
/* Windows ƽ̨���ݽṹ��ֲ */
typedef const char* LPCTSTR;
typedef uchar BYTE;
typedef ushort WORD;
typedef ulong DWORD;
typedef long LONG;
#pragma pack(1)
typedef struct {
  DWORD biSize;
  LONG biWidth;
  LONG biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG biXPelsPerMeter;
  LONG biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} wBITMAPINFOHEADER;
typedef struct {
  BYTE rgbBlue;
  BYTE rgbGreen;
  BYTE rgbRed;
  BYTE rgbReserved;
} wRGBQUAD;
typedef struct {
  wBITMAPINFOHEADER bmiHeader;
  wRGBQUAD bmiColors[1];
} wBITMAPINFO;
#pragma pack()

/*********************************************************************************/

typedef struct _SUBIMGBLOCK SUBIMGBLOCK;
/* ����ͼ�����ͼ���ṹ����Ҫ���ڶ�����ͼ���ļ���*/
struct _SUBIMGBLOCK {
  int   left, top;        /* ��ͼ������ʾλ�ã����Ͻǣ�*/
  int   width, height;      /* ��ͼ����ͼ���ȼ��߶� */
  int   bitcount;       /* ��ͼ���λ�� */
  int stride;
  int   number;         /* ��ͼ�������к� */
  int   order;          /* ͼ�������˳�� ��0������1������*/
  int   dowith;         /* ���÷�����ֻ��Զ�̬ͼ��*/
  int   userinput;        /* �û����루ֻ��Զ�̬ͼ��*/
  int   time;         /* ��ʾ���ӳ�ʱ�䣨ֻ��Զ�̬ͼ�� */
  ulong r_mask;     /* ����ɫ����������ֵ */
  ulong g_mask;     /* ע��1~8λ��ͼ������������ */
  ulong b_mask;
  ulong a_mask;
  
  long      colorkey;   /* ͸��ɫ����ֵ */
  
  uchar** pp_line_addr; /* ��׼ͼ��λ�����д�0�е�n-1�е�ÿһ�е��׵�ַ */
  uchar* p_bit_data;  /* ��׼ͼ��λ���ݻ������׵�ַ */
  int    pal_count;    /* ��ɫ����Ч��ĸ��� */
  COLOR palette[MAX_PALETTE_COUNT];   /* ��ɫ������ */
  
  //INFOSTR*   parents;    /* ��һ��ͼ�����ݵĽṹ��ַ���������ݰ��ṹ�ĵ�ַ��*/
  //SUBIMGBLOCK* prev;     /* ǰһ����ͼ�����ݵĽṹ��ַ */
  //SUBIMGBLOCK* next;     /* ��һ����ͼ�����ݵĽṹ��ַ��NULL��ʾ�������һ��ͼ��*/
};

/* ͼ���д���ݰ��ṹ�����Ľṹ�� */
typedef struct {
  //char* name;
  ulong width;      /* ͼ���� */
  ulong height;     /* ͼ��߶� */
  ulong bitcount;   /* ÿ��������ռ��λ����λ��ȣ� */
  ulong order;      /* ͼ�������˳�� ��0������1������*/
  int stride;
  
  ulong r_mask;     /* ����ɫ����������ֵ */
  ulong g_mask;     /* ע��1~8λ��ͼ������������ */
  ulong b_mask;
  ulong a_mask;

  int     para_value[ISD_ITEM_NUM]; /* ���������Ӧֵ����IRW_SAVE_DESC�ṹ�� */
  IMGTYPE  imgtype;    /* ͼ���ļ����� */
  IMGFORMAT  imgformat;    /* ͼ���ļ���ʽ�������ʶ�� */
  IMGCOMPRESS compression; /* ͼ���ļ���ѹ����ʽ */
  PixelFormat pixfmt;
  int lockmode;
  int numlocks;
  int lockx, locky;

  uchar** pp_line_addr; /* ͼ��λ�����д�0�е�n-1��ÿһ�е��׵�ַ */
  uchar* p_bit_data;  /* ͼ��λ���ݻ������׵�ַ */
  unsigned int  pal_count;    /* ��ɫ����Ч��ĸ��� */
  COLOR palette[MAX_PALETTE_COUNT];   /* ��ɫ������ */

  ulong play_number;  /* ����˳���ߴ磨��Ԫ��Ϊ��λ�� */
  ulong* play_order;  /* ����˳��� */
  ulong time;     // ��֡ͣ��ʱ��
  long colorkey;   // ��ͼ��͸��ɫ��-1��ʾ��͸��ɫ��

  int current_frame;
  int imgnumbers;   // ���ļ���ͼ��ĸ���
  SUBIMGBLOCK* psubimg;    // ��ͼ����������ַ
//     ���ݰ���ǰ������״̬��
//       0 �� �յ����ݰ���û���κα�����������Ч��
//       1 �� ����ͼ���������Ϣ
//       2 �� ����ͼ������
  ushort  data_state;
} INFOSTR;

typedef INFOSTR Bitmap;
typedef INFOSTR Image;

#endif  //__ISEE_IRW_INC__
