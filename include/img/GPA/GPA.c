

#include "chalg.h"
#include "macro.h"
#include "chreg.h"

#define REAL float
#define REG_SAVE()  chreg_regsave("GPAdll.dll")
#ifdef _DEMO
#define REG_CHECK() { }
#else
#define REG_CHECK() if (!chreg_check("GPAdll.dll")) { return 0; }
#endif

/*
%������Ĺ������������������(��������)���õ��ĺ����ǻҶ�ͶӰ�㷨(GPA)
%�����ڵ�ǰ����֡����ڵ�ǰ�ο�֡���ƽ��20������(������)���ڵ������Ҳ����˵�������е�ǰ����֡���ƽ��20������(������)���ڵ����
%��������ͷ�볡������̫����ʵ�������̫���Ļ������ڵ�ǰ�����ƶ����죬Ҳ����ǰ����֡��ͬһ�ƶ������ƶ��÷��Ƚϴ����Ի�Ӱ������˶�ʸ��
%�ο����ף�
%(1)һ�ֻ��ڻҶ�ͶӰ�㷨�ĵ������񷽷������� ������ ������
%(2)��ȡ��̬ͼ��λ��ʸ���ĻҶ�ͶӰ�㷨��Ӧ��,�Ժ�ӱ���ܾ���,�й���ѧԺ������ѧ���ܻ�е�������о��������ֳ���
%(3)������ڵ�������ĻҶ�ͶӰ�㷨���ȵķ���,�� ƽ����ǰ�������ѣ��� ��(�й���ѧԺ������ѧ���ܻ�е�������о��������ֳ���130022)
*/

static REAL sum(int h, int w, const unsigned char* A, int al, int ai, REAL* B, int flag) {
  int i, j;
  REAL s=0;
  assert(2==flag || 1==flag);
  if (2==flag) {
    for (i=0; i<h; ++i) {
      REAL b=0;
      const unsigned char* A2 = A + i * al;
      for (j=0; j<w; ++j, A2+=ai) {
        b += *A2;
      }
      B[i] = b;
      s += b;
    }
  } else if (1==flag) {
    for (j=0; j<w; ++j) {
      B[j] = 0;
    }
    for (i=0; i<h; ++i) {
      const unsigned char* A2 = A + i * al;
      for (j=0; j<w; ++j, A2+=ai) {
        B[j] += *A2;
      }
    }
    for (j=0; j<w; ++j) {
      s += B[j];
    }
  }
  return s;
}

// ���ڱ�Ե���������˲�
static int edge_cos(REAL* x, int len, int off) {
  int i;
  for (i=0; i<len; ++i) {
    if ((i<off)||(i>len-off)) {
      x[i] = (REAL)(x[i]*((1+cos(PI*(off-1-i)/off))/2));//%���߸�Ϊ��refprojrow[i]=refprojrow[i]*(1+cos(pi*(offset_V-1-i)/offset_V/2));�ĸ�Ч�����þ����ĸ����ĵĻ��������Ƶĵط�����
    }
  }
  return 0;
}

  //% �ο�֡����ͶӰ����ͶӰ�Լ������˲� %
static REAL* proj(int h, int w, const unsigned char* A, int al, int ai, int off, int flag) {
  REAL promean, prosum;
  REAL* pro;
  int i, len=0;
  assert(2==flag || 1==flag);
  len = (2==flag) ? h : w;
  pro = MALLOC(REAL, len);//%�洢�ο�֡����ͶӰ
  FILL(len, pro, 0.f);
  //refprojrow     = sum(referenceframe,2);
  //refprojrowsum  = sum(refprojrow);
  //%**********ͶӰ*********%
  prosum = sum(h, w, A, al, ai, pro, flag);
  promean = prosum;
  promean /= len;
  //refprojrow     = refprojrow - refprojrowmean;
  for (i=0; i<len; ++i) {
    pro[i] -= promean;
  }
  //%**********���ڱ�Ե���������˲�*********%
  edge_cos(pro, len, off);
  return pro;
}

static int projcmp(int len, REAL* refproj, REAL* curproj, int off) {
  REAL* correlation_row;
  REAL* refproj2;
  int i, j, dy, correlation_len, refproj2_len, min_index;
  correlation_len = 2*off+1;
  refproj2_len = len+2*off+1;
  //%**********�������������˶�ʸ��*********%
  correlation_row = MALLOC(REAL, correlation_len);
  refproj2 = MALLOC(REAL, refproj2_len);
  //refprojrow = [refproj;extend_row];
  FILL(refproj2_len, refproj2, 0.f);
  FILL(correlation_len, correlation_row, 0.f);
  COPY(len, refproj, refproj2);
  for (j=0; j<correlation_len; ++j) {
    for (i=0; i<len-2*off; ++i) {
      REAL x = refproj2[j+i-1]-curproj[off+i];
      correlation_row[j]=correlation_row[j]+(x*x);
    }
  }
  //[temp,min_index]=min(correlation_row);
  MIN_ELEMENT(correlation_len, correlation_row, min_index);
  SAFEFREE(correlation_row);
  SAFEFREE(refproj2);
  dy=(off+1)-min_index;//%��ֱ������(����),����ֱ�����˶�ʸ��
  return dy;
}

static int off_copy(int h, int w, const unsigned char* A, int al, unsigned char* B, int bl, int cn, int x, int y) {
  int i, j, k;
  int h0, w0;
  int h1, w1;
  w0 = MAX(0, x), w1 = MIN(w, w+x);
  h0 = MAX(0, y), h1 = MIN(h, h+y);
  for (i=h0; i<h1; ++i) {
    const unsigned char* A0 = A+i*al+w0*cn;
    unsigned char* B0 = B+(i-y)*bl+(w0-x)*cn;
    for (j=w0; j<w1; ++j, A0+=cn, B0+=cn) {
      for (k=0; k<cn; ++k) {
        B0[k] = A0[k];
      }
    }
  }
  return 0;
}

typedef struct GPA {
  int offset_V;// = 20;//%���±�Եȥ������ע�⣺offset_V �� offset_HҪ����ʵ�ʿ��ܵ���󶶶����ʵ�ȡֵ��������̫��̫��Ļ���ؼ���ʱ�ƶ�λ�õĺ�ѡλ�þ��٣����Ի�Ч�������ή�ͣ�ȡ20��30�Ƚ�����
  int offset_H;// = 30;//%���ұ�Եȥ����
  int height,width;
  REAL* refprojrow, * refprojcol;
  unsigned char* data;
} GPA;

int GPA_reinit(GPA* g) {
  unsigned char* gry;
  int h = g->height, w = g->width;
  SAFEFREE(g->refprojrow);
  SAFEFREE(g->refprojcol);
  g->offset_V = 20;//%���±�Եȥ������ע�⣺offset_V �� offset_HҪ����ʵ�ʿ��ܵ���󶶶����ʵ�ȡֵ��������̫��̫��Ļ���ؼ���ʱ�ƶ�λ�õĺ�ѡλ�þ��٣����Ի�Ч�������ή�ͣ�ȡ20��30�Ƚ�����
  g->offset_H = 30;//%���ұ�Եȥ����
  gry = MALLOC(unsigned char, h*w);
  //colorcvt( h, w, g->data, w*3, 3, gry, w, 1, T_BGR, T_GRAY );
  IMTRANS(h, w, g->data, w*3, 3, gry, w, 1, BGR2GRAY);
  g->refprojrow = proj(h, w, gry, w, 1, g->offset_V, 2);
  g->refprojcol = proj(h, w, gry, w, 1, g->offset_H, 1);
  SAFEFREE(gry);
  return 0;
}

GPA* GPA_new(int height, int width, const unsigned char* reference_frame_bgr, int step) {
  GPA* g;
  //REG_SAVE();
  REG_CHECK();
  g = MALLOC(GPA, 1);
  //int offset_V, offset_H;
  memset(g, 0, sizeof(GPA));
  g->offset_V = (int)(height*0.1);//%���±�Եȥ������ע�⣺offset_V �� offset_HҪ����ʵ�ʿ��ܵ���󶶶����ʵ�ȡֵ��������̫��̫��Ļ���ؼ���ʱ�ƶ�λ�õĺ�ѡλ�þ��٣����Ի�Ч�������ή�ͣ�ȡ20��30�Ƚ�����
  g->offset_H = (int)(width*0.1);//%���ұ�Եȥ����
  g->data = MALLOC(unsigned char, height*width*3);
  off_copy(height, width, reference_frame_bgr, step, g->data, width*3, 3, 0, 0);
  g->height = height, g->width = width;
  GPA_reinit(g);
  return g;
}

int GPA_update(GPA* g, const unsigned char* current_frame_bgr, int step, unsigned char* out, int out_step) {
  int dx, dy;
  REAL* curprojrow, *curprojcol;
  unsigned char* gry;
  int h = g->height, w = g->width;
  gry = MALLOC(unsigned char, h*w);
  //colorcvt( h, w, current_frame_bgr, step, 3, gry, w, 1, T_BGR, T_GRAY );
  IMTRANS(h, w, current_frame_bgr, step, 3, gry, w, 1, BGR2GRAY);
  curprojrow = proj(h, w, gry, w, 1, g->offset_V, 2);
  curprojcol = proj(h, w, gry, w, 1, g->offset_H, 1);
  SAFEFREE(gry);
  dy = projcmp(h, g->refprojrow, curprojrow, g->offset_V);
  dx = projcmp(w, g->refprojcol, curprojcol, g->offset_H);
  //printf("%2d, %2d\n", dx, dy);
  off_copy(h, w, current_frame_bgr, step, g->data, w*3, 3, dx, dy);
  if (out) {
    off_copy(h, w, g->data, w*3, out, out_step, 3, 0, 0);
  }
  SAFEFREE(gry);
  SAFEFREE(curprojrow);
  SAFEFREE(curprojcol);
  //GPA_reinit(g);
  return 1;
}

void GPA_free(GPA* g) {
  SAFEFREE(g->data);
  SAFEFREE(g->refprojrow);
  SAFEFREE(g->refprojcol);
  SAFEFREE(g);
}

/*��������������㷨
���㷨��Ҫ���Ӿ������ƶ���(������)���õ����ǻҶ�ͶӰ����㷨(GPA)
���õ��ǻҶ�ͼƬ���д����������£�
1) �Կ�ʼ�����ĵ�һ֡��Ϊ��ʼ�ο�֡
2) ���ڲο�֡�����һ����ˮƽͶӰ�ʹ�ֱͶӰ
3) ���ڹ�һ�����ˮƽ�ʹ�ֱͶӰ���������˲�
4) ��õڶ�֡/��ǰ֡/��һ֡��ͬ�����ղ���1)��3)���д���
5) ���òο�֡�͵�ǰ֡��ˮƽͶӰ�ʹ�ֱͶӰ���л���������������˶�
ʸ��(������ƫ����dy ��dx),�����㷨�ϼ򵥣���matlab ����
6) �˶�������������ǰ֡��ο�֡����ƽ�ƣ�ƽ����Ϊdy ��dx
��dy<0,��ǰ֡����ڲο�֡�����˶���|dy|������ǰ֡��Ҫ����ƽ��
|dy|�����ز�����ο�֡���룬����ԭͼ��ߴ�Ĳ��ֱ���ȥ���������
���ֲ�0
��dy>0,��ǰ֡����ڲο�֡�����˶���|dy|
��dx<0,��ǰ֡����ڲο�֡�����˶���|dx|
��dx>0,��ǰ֡����ڲο�֡�����˶���|dx|
�������ϵ�ƽ�Ʊ�õ���ǰ֡���ȶ�֡
7) �ѵ�ǰ�ȶ�֡��Ϊ��һ֡�Ĳο�֡�ظ�����2)��6)
ע�⣺�����ȶ�֡ʱҪ���ȶ�֡������������4 ����Ե�Ľضϣ�Ҳ����˵��
����ȶ�֡����Ҫ��ԭʼ���еĳߴ�ҪС
*/
