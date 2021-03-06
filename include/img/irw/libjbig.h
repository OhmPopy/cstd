/*
 *  Header file for the portable pfree JBIG compression library
 *
 *  Markus Kuhn -- mkuhn@acm.org
 *
 *  $Id: jbig.h,v 1.11 2002-04-09 11:14:37+01 mgk25 Exp $
 */

/*
 * 注：YZ于2002-5修改了其中的部分代码，所以本文件并非是Markus Kuhn写的原始
 *     jbig kit，原始代码可从Markus Kuhn的网站中获得，地址为：
 *
 *                 http://www.cl.cam.ac.uk/~mgk25/jbigkit/
 *
 *     （凡是修改的地方都有"YZ"字样，用文本搜索可以找到这些修改的代码）
 *
 *     在此也感谢Markus Kuhn和他基于GPL的JBIG代码！
 */

#ifndef JBG_H
#define JBG_H



/* >>>>>>>>>>>>>>>>>>> modifyed by YZ */
/* OLD
*/
/* NEW */
#ifdef  WIN321
#define RAISE_EXCEPTION() RaiseException(3<<30|2<<28|1<<16|1, EXCEPTION_NONCONTINUABLE, 0, 0)
#else
#define RAISE_EXCEPTION() ((void*)0)    /* 此处应写入Linux的异常提交函数 */
#endif /* WIN32 */
/* <<<<<<<<<<<<<<<<<<< modify end */


/*
 * JBIG-KIT version number
 */

#define JBG_VERSION    "1.4"

/*
 * Buffer block for SDEs which are temporarily stored by encoder
 */

#define JBG_BUFSIZE 4000

struct jbg_buf {
  uchar d[JBG_BUFSIZE];              /* one block of a buffer list */
  int len;                             /* length of the data in this block */
  struct jbg_buf* next;                           /* pointer to next block */
  struct jbg_buf* previous;                   /* pointer to previous block *
                 * (unused in freelist)      */
  struct jbg_buf* last;     /* only used in list head: final block of list */
  struct jbg_buf** free_list;   /* pointer to pointer to head of pfree list */
};

/*
 * Maximum number of allowed ATMOVEs per stripe
 */

#define JBG_ATMOVES_MAX  64

/*
 * Option and order flags
 */

#define JBG_HITOLO     0x08
#define JBG_SEQ        0x04
#define JBG_ILEAVE     0x02
#define JBG_SMID       0x01

#define JBG_LRLTWO     0x40
#define JBG_VLENGTH    0x20
#define JBG_TPDON      0x10
#define JBG_TPBON      0x08
#define JBG_DPON       0x04
#define JBG_DPPRIV     0x02
#define JBG_DPLAST     0x01

#define JBG_DELAY_AT   0x100  /* delay ATMOVE until the first line of the next
* stripe. Option available for compatibility
* with conformance test example in clause 7.2.*/


/*
 * Possible error code return values
 */

#define JBG_EOK        0
#define JBG_EOK_INTR   1
#define JBG_EAGAIN     2
#define JBG_ENOMEM     3
#define JBG_EABORT     4
#define JBG_EMARKER    5
#define JBG_ENOCONT    6
#define JBG_EINVAL     7
#define JBG_EIMPL      8
/* >>>>>>>>>>>>>>>>>>> modifyed by YZ */
/* OLD
*/
/* NEW */
#define JBG_ESYSERR    9
/* <<<<<<<<<<<<<<<<<<< modify end */


/*
 * Language code for error message strings (based on ISO 639 2-letter
 * standard language name abbreviations).
 */

#define JBG_EN         0        /* English */
#define JBG_DE_8859_1  1        /* German in ISO Latin 1 character set */
#define JBG_DE_UTF_8   2        /* German in Unicode UTF-8 encoding */

/*
 * Status description of an arithmetic encoder
 */

struct jbg_arenc_state {
  uchar st[4096];    /* probability status for contexts, MSB = MPS */
    ulong c;                /* C register, base of coding intervall, *
                                   * layout as in Table 23                 */
    ulong a;      /* A register, normalized size of coding intervall */
    long sc;        /* counter for buffered 0xff values which might overflow */
    int ct;  /* bit shift counter, determines when next byte will be written */
    int buffer;                /* buffer for most recent output byte != 0xff */
    void (*byte_out)(int, void*);  /* function which receives all PSCD bytes */
    void* file;                              /* parameter passed to byte_out */
  };


/*
 * Status description of an arithmetic decoder
 */
enum gbg_result {
    JBG_OK,                        /* symbol has been successfully decoded */
    JBG_READY,             /* no more bytes of this PSCD required, marker  *
                * encountered, probably more symbols available */
    JBG_MORE,          /* more PSCD data bytes required to decode a symbol */
    JBG_MARKER   /* more PSCD data bytes required, ignored final 0xff byte */
  };


struct jbg_ardec_state {
  uchar st[4096];    /* probability status for contexts, MSB = MPS */
  ulong c;                /* C register, base of coding intervall, *
                                   * layout as in Table 25                 */
  ulong a;      /* A register, normalized size of coding intervall */
  int ct;     /* bit shift counter, determines when next byte will be read */
  uchar* pscd_ptr;               /* pointer to next PSCD data byte */
  uchar* pscd_end;                   /* pointer to byte after PSCD */
  enum gbg_result result;                              /* result of previous decode call */
  int startup;                            /* controls initial fill of s->c */
};

#ifdef TEST_CODEC
void arith_encode_init(struct jbg_arenc_state* s, int reuse_st);
void arith_encode_flush(struct jbg_arenc_state* s);
void arith_encode(struct jbg_arenc_state* s, int cx, int pix);
void arith_decode_init(struct jbg_ardec_state* s, int reuse_st);
int arith_decode(struct jbg_ardec_state* s, int cx);
#endif


/*
 * Status of a JBIG encoder
 */

struct jbg_enc_state {
  int d;                            /* resolution layer of the input image */
  ulong xd, yd;    /* size of the input image (resolution layer d) */
  int planes;                         /* number of different bitmap planes */
  int dl;                       /* lowest resolution layer in the next BIE */
  int dh;                      /* highest resolution layer in the next BIE */
  ulong l0;                /* number of lines per stripe at lowest *
                                    * resolution layer 0                   */
  ulong stripes;    /* number of stripes required  (determ. by l0) */
  uchar** lhp[2];    /* pointers to lower/higher resolution images */
  int* highres;                 /* index [plane] of highres image in lhp[] */
  int order;                                    /* SDE ordering parameters */
  int options;                                      /* encoding parameters */
  unsigned mx, my;                           /* maximum ATMOVE window size */
  int* tx;       /* array [plane] with x-offset of adaptive template pixel */
  char* dppriv;         /* optional private deterministic prediction table */
  char* res_tab;           /* table for the resolution reduction algorithm */
  struct jbg_buf**** sde;      /* array [stripe][layer][plane] pointers to *
        * buffers for stored SDEs                  */
  struct jbg_arenc_state* s;  /* array [planes] for arithm. encoder status */
  struct jbg_buf* free_list; /* list of currently unused SDE block buffers */
  void (*data_out)(uchar* start, size_t len, void* file);
  /* data write callback */
  void* file;                            /* parameter passed to data_out() */
  char* tp;    /* buffer for temp. values used by diff. typical prediction */
};


/*
 * Status of a JBIG decoder
 */

struct jbg_dec_state {
  /* data from BIH */
  int d;                             /* resolution layer of the full image */
  int dl;                            /* first resolution layer in this BIE */
  ulong xd, yd;     /* size of the full image (resolution layer d) */
  int planes;                         /* number of different bitmap planes */
  ulong l0;                /* number of lines per stripe at lowest *
            * resolution layer 0                   */
  ulong stripes;    /* number of stripes required  (determ. by l0) */
  int order;                                    /* SDE ordering parameters */
  int options;                                      /* encoding parameters */
  int mx, my;                                /* maximum ATMOVE window size */
  char* dppriv;         /* optional private deterministic prediction table */

  /* loop variables */
  ulong ii[3];  /* current stripe, layer, plane (outer loop first) */

  /*
   * Pointers to array [planes] of lower/higher resolution images.
   * lhp[d & 1] contains image of layer d.
   */
  uchar** lhp[2];

  /* status information */
  int** tx, ** ty;  /* array [plane][layer-dl] with x,y-offset of AT pixel */
  struct jbg_ardec_state** s;    /* array [plane][layer-dl] for arithmetic *
          * decoder status */
  int** reset;     /* array [plane][layer-dl] remembers if previous stripe *
        * in that plane/resolution ended with SDRST.           */
  ulong bie_len;                    /* number of bytes read so far */
  uchar buffer[20]; /* used to store BIH or marker segments fragm. */
  int buf_len;                                /* number of bytes in buffer */
  ulong comment_skip;      /* remaining bytes of a COMMENT segment */
  ulong x;              /* x position of next pixel in current SDE */
  ulong i; /* line in current SDE (first line of each stripe is 0) */
  int at_moves;                /* number of AT moves in the current stripe */
  ulong at_line[JBG_ATMOVES_MAX];           /* lines at which an   *
                       * AT move will happen */
  int at_tx[JBG_ATMOVES_MAX], at_ty[JBG_ATMOVES_MAX]; /* ATMOVE offsets in *
                   * current stripe    */
  ulong line_h1, line_h2, line_h3;     /* variables of decode_pscd */
  ulong line_l1, line_l2, line_l3;
  int pseudo;         /* flag for TPBON/TPDON:  next pixel is pseudo pixel */
  int** lntp;        /* flag [plane][layer-dl] for TP: line is not typical */

  ulong xmax, ymax;         /* if possible abort before image gets *
             * larger than this size */
  int dmax;                                      /* abort after this layer */
};

/* >>>>>>>>>>>>>>>>>>> modifyed by YZ */
/* OLD
*/
/* NEW */
typedef struct jbg_enc_state  JBG_ENC_STATE;
typedef struct jbg_enc_state*  LPJBG_ENC_STATE;
typedef struct jbg_dec_state  JBG_DEC_STATE;
typedef struct jbg_dec_state*  LPJBG_DEC_STATE;
/* <<<<<<<<<<<<<<<<<<< modify end */


/* some macros (too trivial for a function) */

#define jbg_dec_getplanes(s)     ((s)->planes)



/* function prototypes */

void jbg_enc_init(struct jbg_enc_state* s, ulong x, ulong y,
    int planes, uchar** p,
    void (*data_out)(uchar* start, size_t len,
        void* file),
    void* file);
int jbg_enc_lrlmax(struct jbg_enc_state* s, ulong mwidth,
    ulong mheight);
void jbg_enc_layers(struct jbg_enc_state* s, int d);
int  jbg_enc_lrange(struct jbg_enc_state* s, int dl, int dh);
void jbg_enc_options(struct jbg_enc_state* s, int order, int options,
    long l0, int mx, int my);
void jbg_enc_out(struct jbg_enc_state* s);
void jbg_enc_free(struct jbg_enc_state* s);


void jbg_dec_init(struct jbg_dec_state* s);
void jbg_dec_maxsize(struct jbg_dec_state* s, ulong xmax,
    ulong ymax);
int  jbg_dec_in(struct jbg_dec_state* s, uchar* data, size_t len,
    size_t* cnt);
long jbg_dec_getwidth(const struct jbg_dec_state* s);
long jbg_dec_getheight(const struct jbg_dec_state* s);
uchar* jbg_dec_getimage(const struct jbg_dec_state* s, int plane);
long jbg_dec_getsize(const struct jbg_dec_state* s);
void jbg_dec_merge_planes(const struct jbg_dec_state* s, int use_graycode,
    void (*data_out)(uchar* start, size_t len,
        void* file), void* file);
long jbg_dec_getsize_merged(const struct jbg_dec_state* s);
void jbg_dec_free(struct jbg_dec_state* s);


const char* jbg_strerror(int errnum, int language);
void jbg_int2dppriv(uchar* dptable, const char* internal);
void jbg_dppriv2int(char* internal, const uchar* dptable);
ulong jbg_ceil_half(ulong x, int n);
void jbg_split_planes(ulong x, ulong y, int has_planes,
    int encode_planes,
    const uchar* src, uchar** dest,
    int use_graycode);

#endif /* JBG_H */
