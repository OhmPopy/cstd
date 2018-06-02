
#ifndef _CASCADE_H_
#define _CASCADE_H_

#include <stdio.h>
#include "xtypes.h"
#include "macro.h"
//#include "faceDetect.h"
//#include "debug.h"

#define ESP (0.00001f)

#include "sys\console.h"
#define STATIC static

#define _PRINT_PRO          // ��ӡ����
#define LUT_BIN (32)
#define X_ESP   (0.00001f)
#define DELIMT    " "
#define _D(_F)   "%"#_F DELIMT
#define _DF()    FLOATFMT DELIMT
#if 0
//typedef float real;
//typedef double real;
#define _F()   "%f" DELIMT
#else
//typedef double real;
#define _F()   "%lf" DELIMT
#endif
typedef unsigned int size_t;


#define INIFILENAME ".\\adaboost.ini"
#define ADABOOSTSECT  "adaboost"


enum {
  FT_NUL = 0,
#define FEATTYPEDEF(a, b)  FT_##a,
#include "feattypedef.txt"
#undef FEATTYPEDEF
  FEATNUM,
};

enum {
  WT_NUL = 0,
#define WEAKTYPEDEF(a, b)  WT_##a,
#include "weaktypedef.txt"
#undef WEAKTYPEDEF
  BOOSTNUM,
};

typedef double(*_FEAT_VAL_FUN_T)(double* samp, int al, void* feat, int i, double iv);
typedef double(*_WEAK_VAL_FUN_T)(double featval, void*, int i);
typedef int (*_FPRINT_FUN_T)(FILE* pf, const void* ptr);
typedef int (*_FSCANF_FUN_T)(FILE* pf, void* ptr);
typedef int (*_SETOFFSET_FUN_T)(void* eoh, int al, int ai, double cs);
typedef int (*_INTEGRAL_FUN_T)(int h, int w, const unsigned char* img, int al, double* mag, int ml, int mi);

typedef struct {
  const char* name;
  size_t elemsize;
  size_t INTEGRAL_CN;
  _FEAT_VAL_FUN_T val;
  _FPRINT_FUN_T print;
  _FSCANF_FUN_T scanf;
  _SETOFFSET_FUN_T setoffset;
  _INTEGRAL_FUN_T integral;
}
feat_fun_t;

typedef struct {
  const char* name;
  size_t elemsize;
  _WEAK_VAL_FUN_T val;
  _FPRINT_FUN_T print;
  _FSCANF_FUN_T scanf;
}
weak_fun_t;

#ifndef XRECT_DEFINED
#define XRECT_DEFINED
// ���ο�
typedef struct XRECT {
  int x, y, w, h;      /* һ��Ŀ������п��λ��ƽ��ֵ */
  double score;       /* һ��Ŀ������п�ĵ÷�ƽ��ֵ */
  int count;             /* һ��Ŀ������п�ĸ���       */
  int id;
}
XRECT;
#endif // XRECT_DEFINED

typedef struct _IRECTW {
  int x, y, w, h, d;
  int p0, p1, p2, p3;
  double dd;
}
IRECTW;

typedef struct _HAARFEAT {
  IRECTW ft0;
  IRECTW ft1;
}
HAARFEAT;

typedef struct _HAAR3FEAT {
  IRECTW ft0;
  IRECTW ft1;
  IRECTW ft2;
}
HAAR3FEAT;

// HOG (Histograms of Oriented Gradients)
// [x, y, w, h]  - ���ο� ���Ͻ�x����, ���Ͻ�y����, ���ο��, ���ο��
// bi            - [0-8] ��ʾ9�ֽǶ� [0~40][40~80]..[320~360]
typedef struct _EOHFEAT {
  int x, y, w, h, bi;
  int p0, p1, p2, p3;
  //double iarea; // ����ĵ���
  //int type;
}
EOHFEAT, EOHEXFEAT;

typedef int HIMMFEAT;
typedef int CHIFEAT;
typedef int BASEFEAT;
typedef int BABSFEAT;

typedef struct _EOHHAARFEAT {
  int type;
  union {
    IRECTW ft0;
    IRECTW ft1;
    struct {
      int x, y, w, h, bi;
      int p0, p1, p2, p3;
    };
  };
}
EOHHAARFEAT;

#define EOH_CN                         (10)
#define HAAR_CN                        ( 2)
#define EOHHAAR_CN                     (EOH_CN+HAAR_CN)
#define EOHEX_CN                       EOH_CN

#define RECT_VAL(_A, p0, p1, p2, p3)   (*(_A + (p0)) - *(_A + (p1)) - *(_A + (p2)) + *(_A + (p3)))
#define RECT_VAL2(_A, _a, _b)          (*(_A) - *(_A+(_a)) - *(_A+(_b)) + *(_A+(_a)+(_b)))
// �� haar ����
#define HAAR_RECT_VAL(A, rect)         (RECT_VAL(A, (rect).p0, (rect).p1, (rect).p2, (rect).p3) * (rect).dd)
#define HAAR_FEAT_VAL(A, haar, IV)     (IV*(HAAR_RECT_VAL(A, (haar).ft0) + HAAR_RECT_VAL(A, (haar).ft1)))
#define HAAR3_FEAT_VAL(A, haar, IV)    (IV*(HAAR_RECT_VAL(A, (haar).ft0) + HAAR_RECT_VAL(A, (haar).ft1) + ((haar).ft2.w ? HAAR_RECT_VAL(A, (haar).ft2) : 0)))
// �� eoh ����
#define EOH_RECT_VAL(_A, rect)         (RECT_VAL(_A, (rect).p0, (rect).p1, (rect).p2, (rect).p3))
#define EOH_FEAT_VAL(A, rect, IV)      ((EOH_RECT_VAL((A+(rect).bi+1), rect))/(EOH_RECT_VAL(A, rect)+0.00001f))
// �� chi ����

// �� eohex ����
const static int eohex_idx0[ 27 ] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const static int eohex_idx1[ 27 ] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8,
};
const static int eohex_idx2[ 27 ] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 1, 2, 3, 4, 5, 6, 7, 8, 8, 9, 1, 2, 3, 4, 5, 6, 7,
};

#define EOHEX_FEAT_VAL(A, rect, IV)  ( ( (EOH_RECT_VAL((A+eohex_idx0[(rect).bi]), rect)) + \
    ((rect).bi<9?0.f:(EOH_RECT_VAL((A+eohex_idx1[(rect).bi]), rect)) ) +  \
    ((rect).bi<18?0.f:(EOH_RECT_VAL((A+eohex_idx2[(rect).bi]), rect)) ) )/ \
    (EOH_RECT_VAL(A, rect)+0.00001f))

//#define EOHEX_FEAT_VAL EOH_FEAT_VAL

// �� ed ����
#define ED_FEAT_VAL(A, rect, IV)       ((EOH_RECT_VAL(A, rect)*(rect).iarea)
#define EOHED_FEAT_VAL(A, rect, IV)    (((rect).type==FT_EOH) ? EOH_FEAT_VAL(A, rect, IV) : ED_FEAT_VAL(A, rect, IV))
// �� eohhaar ����
#define EOHHAAR_FEAT_VAL(A, rect, IV)  ( (FT_EOH==(rect).type) ? (EOH_FEAT_VAL(A, rect, IV)) : (HAAR_FEAT_VAL((A+EOH_CN), rect, IV)) )

#define EOH_SETOFFSET(eoh, al, ai, cs)                                        \
  do {                                                                          \
    int a = XFLOOR((eoh).y * cs) * al + XFLOOR((eoh).x * cs) * ai;              \
    int w = XFLOOR((eoh).w * cs), h = XFLOOR((eoh).h * cs);                 \
    int b = w * ai, c = h * al;                                               \
    (eoh).p0 = a;                                                               \
    (eoh).p1 = a+b;                                                             \
    (eoh).p2 = a+c;                                                             \
    (eoh).p3 = a+b+c;                                                           \
  } while(0)

#define EOHEX_SETOFFSET EOH_SETOFFSET

// ����ƫ����
#define RECT_SETOFFSET(RECT, al, ai, cs, OUT, area)  {                        \
    int w = XFLOOR((RECT).w* cs);                                             \
    int h = XFLOOR((RECT).h* cs);                                             \
    int a = XFLOOR((RECT).y * cs) * al + XFLOOR((RECT).x * cs) * ai;            \
    int b = w * ai, c = h * al;                                               \
    area = w * h;                                                             \
    (OUT).p0 = a;                                                               \
    (OUT).p1 = a+b;                                                             \
    (OUT).p2 = a+c;                                                             \
    (OUT).p3 = a+b+c;                                                           \
  }

#define HAAR_SETOFFSET(RECT, al, ai, cs)                                      \
  do {                                                                          \
    int ar0, ar1;                                                               \
    RECT_SETOFFSET((RECT).ft0, al, ai, cs, (RECT).ft0, ar0);                \
    RECT_SETOFFSET((RECT).ft1, al, ai, cs, (RECT).ft1, ar1);                \
    (RECT).ft1.dd = ((RECT).ft1.d*cs*cs);                                   \
    if (ar0) {                                                                  \
      (RECT).ft0.dd = -((RECT).ft1.dd * ar1/ar0);                           \
    }                                                                           \
  } while(0)

#define HAAR3_SETOFFSET(RECT, al, ai, cs)                                     \
  do {                                                                          \
    int ar0, ar1, ar2;                                                          \
    RECT_SETOFFSET((RECT).ft0, al, ai, cs, (RECT).ft0, ar0);                \
    RECT_SETOFFSET((RECT).ft1, al, ai, cs, (RECT).ft1, ar1);                \
    (RECT).ft1.dd = ((RECT).ft1.d*cs*cs);                                   \
    if ((RECT).ft2.w) {                                                      \
      RECT_SETOFFSET((RECT).ft2, al, ai, cs, (RECT).ft2, ar2);              \
      (RECT).ft2.dd = ((RECT).ft2.d*cs*cs);                                 \
      if (ar0) {                                                                \
        (RECT).ft0.dd = -(((RECT).ft1.dd * ar1 + (RECT).ft2.dd * ar2)/ar0);\
      }                                                                         \
    } else {                                                                    \
      if (ar0) {                                                                \
        (RECT).ft0.dd = -((RECT).ft1.dd * ar1/ar0);                         \
      }                                                                         \
    }                                                                           \
  } while(0)

#define EOHHAAR_SETOFFSET(RECT, al, ai, cs)                                   \
  do {                                                                          \
    if (FT_EOH==(RECT).type) {                                                  \
      EOH_SETOFFSET(RECT, al, ai, cs);                                          \
    } else if (FT_HAAR==(RECT).type) {                                          \
      HAAR_SETOFFSET(RECT, al, ai, cs);                                         \
    }                                                                           \
  } while(0)

// ����ͼ��ƫ����
#define FEAT_SETOFFSET(_PTR, _LEN, _AL, _AI, _CS, _FEAT)                      \
  do {                                                                          \
    int _I;                                                                     \
    for (_I=0; _I<_LEN; ++_I) {                                                 \
      _FEAT##_SETOFFSET(*((_PTR)+_I), _AL, _AI, _CS);                           \
    }                                                                           \
  } while(0)

typedef struct _LUTWEAK {
  double minval;
  double binval; // = bin/(maxval-minval);
  double val[ LUT_BIN ];
}
LUTWEAK;

typedef struct _GENWEAK {
  double thd;
  double val[ 2 ];
}
GENWEAK;


enum {TREEWEAK_LV = 1};
typedef struct _TREEWEAK {
  double thd[ (1<<TREEWEAK_LV) - 1 ];
  double val[ 1<<TREEWEAK_LV ];
}
TREEWEAK;

typedef struct _TANWEAK {
  double thd;
  double gm;
}
TANWEAK;

typedef struct _carrect {
  unsigned char x, y, w, h, id;// ��Ӧblock ��λ��
} carrect;

typedef struct _carnode {
  double constrain;   // ��ֵ
  double coeffs[2];   // [0]С����ֵ�ĵ÷�, [1]������ֵ�ĵ÷�
  unsigned char chr[2];         // [0]���ӵ����� 0Ϊ��, [1]�Һ��ӵ�����
  unsigned char x, y, w, h, id;// ��Ӧblock ��λ��
  //int dim;            // ��Ӧ���������ĵڼ�ά
} carnode;

#define CARTMAXSPLIT   5
typedef struct CARTFFEAT {
  carnode nodes[CARTMAXSPLIT];
} CARTFFEAT;

// ǿ�������ṹ
#ifndef STAGE_DEFED
#define STAGE_DEFED
typedef struct _STAGE {
  int len;  // ��������������Ŀ
  double thd; // ǿ��������ֵ
}
STAGE;
#endif // STAGE_DEFED

typedef struct _RANKWEAK {
  int id;
  double alpha;
}
RANKWEAK;

#define TAN_VAL(x, weak)   (double)(atan((weak)->gm*((x)-(weak)->thd))*(2./PI))

#define GUS_BIN_POS  100
#define GUS_BIN_NEG  100
#define GUS_BIN  (GUS_BIN_POS+GUS_BIN_NEG)

typedef struct GUSWEAK {
  double val[GUS_BIN];
} GUSWEAK;

typedef struct _CASCADE {
  int feat_type;    // ��������
  int weak_type;    // boost ����
  int stagelen;     // ǿ��������
  int weaklen;      // ����������
  int maxstagelen;  // ���ǿ��������
  int maxweaklen;   // �������������
  int w, h;       // ��С����
  int is_nesting;   // ʹ��nesting�ṹ
  int* id;
  STAGE* stage;
  union {
    void* weakptr;
    GENWEAK* gen;
    LUTWEAK* lut;
    TANWEAK* tan;
    RANKWEAK* rank;
    GUSWEAK* gus;
    TREEWEAK* tree;
  };
  union {
    void* featptr;
    int* chi;
    int* base;
    EOHFEAT* eoh;
    HAARFEAT* haar;
    HAAR3FEAT* haar3;
    EOHHAARFEAT* eohhaar;
    CARTFFEAT* cart;
  };
  //int (*setoffset)(const CASCADE* ca, int step, int cn, double cs);
  //int (*pass)(const CASCADE* ca, const double* Samp, double* score);
}
CASCADE;

#define del_boost(hh)  (del_boost_(hh), (hh)=0)
//#include "adaboost_config.h"
#define GetFeat_elemsize(hh)    _feat_fun[ (hh)->feat_type ].elemsize
//#define GetWeak_elemsize(hh)    _weak_fun[ (hh)->weak_type ].elemsize
#define GetFeatNum(hh)   (hh)->feat_num
#define GetSampNum(hh)   (hh)->samp_num
#define GetPosNum(hh)    (hh)->pos_num
#define GetNegNum(hh)    ((hh)->samp_num - (hh)->pos_num)
#define getfeat(hh, i)   (hh)->feat_vt->f_get_feat(hh, i)
#define findsamp(hh, t)  (hh)->feat_vt->f_find_samp(hh, t)
#define savefeat(hh, t)  (hh)->feat_vt->f_save_feat(hh, t)
#define updateweight(hh, t)  (hh)->weak_vt->f_update_weight(hh, t)
#define geterror(hh, t)       (hh)->weak_vt->f_get_error(hh, t)
#define uninit_feat(hh)       (hh)->feat_vt->f_uninit_feat(hh)
#define init_feat(hh)       (hh)->feat_vt->f_init_feat(hh)
#define samp_data(hh, buf, len)       (hh)->feat_vt->f_samp_data(hh, &buf, &len)
//#define init_feat(hh)       (hh)->feat_vt->f_init_feat(hh)
typedef struct _BOOST BOOST;
typedef struct _WORKINFO {
  double Ha, Fa, Error, Threshold, Time;
  int id, beg, end;
  double val[ 4 ];
  CARTFFEAT cart;
}
WORKINFO;
typedef int (*f_init_feat_t)(BOOST* hh);
typedef struct featvtable {
  int (*f_find_samp)(BOOST* hh, int type);
  double* (*f_get_feat)(BOOST* hh, int i);       //ȡ���������ĵ�i������
  int (*f_save_feat)(BOOST* hh, int id);
  int (*f_uninit_feat)(BOOST* hh);
  int (*f_samp_data)(BOOST* hh, char** buf, int* size);
  int (*f_init_feat)(BOOST* hh);
  const char* name; //������
  const char* disc; //����
}
featvtable;

typedef struct weakvtable {
  int (*f_get_error)(BOOST* hh, WORKINFO* pwi);
  int (*f_update_weight)(BOOST* hh, WORKINFO* pwi);
}
weakvtable;

// adaboostѵ��
typedef struct _BOOST {
  featvtable* feat_vt;
  weakvtable* weak_vt;
  double* vi;           // [samp_num] �����÷�
  double* wi;           // [samp_num] ����Ȩ��
  double* fi;           // [samp_num] ��������Ȩ��
  CASCADE* ca;          // ������
  void* h_feat;

  int feat_num;         // ��������Ŀ
  double ha;            // ������ͨ����
  double fa;            // ��������ʶ��
  double Hr;            // ��������Ȩ�ر���
  double min_neg_rate;  // ��С����������
  int pos_num;          // ��������Ŀ��С�����������������������ڵ��ڵ��Ǹ���������
  int samp_num;         // ��������Ŀ
  unsigned char* flag;  // [feat_num] ��������������û���ù�=0�������Ѿ��ù�=1
  int flag_unreselect;  // ����ѡ����
  int weak_type;        // boost����
  int feat_type;        // �������ͣ�������ʱ�õ�����[ST_CHI, ...]
}
BOOST;

// ����
CASCADE* new_cascade(int maxstagelen, int maxweaklen, int type, int weak_type);

// ����
void del_cascade(CASCADE* ptr);

// ��ʽ����ӡ
void cascade_printf(const CASCADE* ptr, FILE* pf);

// ��ʽ������
//pf = fopen("cas.txt", "rb");
//cascade_scanf(pf, 1000, 10000);//���������
CASCADE* cascade_scanf(FILE* pf, int maxstagelen, int maxweaklen);

int EOH_grads_integral(int h, int w, const unsigned char* img, int al, double* mag, int ml, int mi);

int detect(CASCADE* ca, int height, int width, const unsigned char* data, int step,
    double ssmin, double ssmax, double ss, int stepxy, XRECT* B, int B_len);

int detect1(CASCADE* ca, int height, int width, const unsigned char* data, int step,
    double ssmin, double ssmax, double ss, int stepxy, XRECT* B, int B_len);

int read_rect_from_file(const char* fname, EOHFEAT* out, int cn);
int get_haar_feat(int h, int w, int mincx, int mincy, int a, int b, HAARFEAT* out);

void cascade_pass(CASCADE* _CA, double* _SAMP, int al, double _IV, int* _ISPASS, double* _PSCORE);

void cascade2hpp(const CASCADE* ptr, const char* name);

CASCADE* read_cascade_xml(const char* xml);




#define DO_PRINT_PRO(_PRO, _ALL, _TIME) { if ((_ALL) < 200 || !((_PRO) % ((_ALL) / MIN(_ALL, 10000))) || ((_ALL) - (_PRO)) < 3) \
    { printf("%6d|%9g %9g| %9g  %9g  %9d [%4.1f%%]\r", pwi->id, pwi->Ha, pwi->Fa, pwi->Error, pwi->Threshold, _TIME, ((_PRO + 1) * 100.f / (_ALL)) ); } }

#define SETVTABLE(HH, _NAME) if (FT_##_NAME!=(hh)->feat_type) {return 0;} else { \
    static featvtable vtable = {_NAME##_find_samp, _NAME##_feat, _NAME##_save_feat, _NAME##_uninit, _NAME##_samp_data}; \
    (hh)->feat_vt = &vtable; }


#endif // _CASCADE_H_
