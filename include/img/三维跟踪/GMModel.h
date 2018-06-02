#ifndef _GMMODEL_H_
#define _GMMODEL_H_
#include <stdlib.h>
#include "sort.h"
struct BlobRect {
  int l,t,r,b; //��������
  int n;
};
// ��˹������ģ
struct GMM {
public:
  double afa;               // ���ڵ�������ģ�͸����ٶȵĲ���
  double thrd;              // ǰ���ж�ʱ��Ȩ����ֵ
  double vir_v;             // ��ʼ������ֵ
  double vir_w;             // ��ʼ��Ȩ��ֵ
  double shadowp[ 4 ];      // �ڶ�����Ӱ����������ʹ�õĲ���
  int row;                  // �������ظ߶�
  int col;                  // �������ؿ��
  int kind;                 // ����ģ�͸���
  unsigned int min_pts;     // �����ſ��С�����ſ��Ƿ�ȥ����ʹ�õ���ֵ
  bool rlabel;              // ȥ��С�ſ����������Ŀ���
  bool rshadowx;            // ��һ����Ӱ������������
  bool rshadowy;            // �ڶ�����Ӱ������������
  bool rshape;              // ����������������
  int *s;
  double *w;
  double *m;
  double *v;
  double *b;
  //  blob �ſ����֮��Ľ��
  // ͼ��ߴ������õļ��߿�ͬ�ߴ�
  // ������Ϊ�Ѿ�������ͨ������޷�������ͼ��, 0ֵ���ر�ʾ����, ����0��ʾǰ��
  unsigned int *blob;       // �ſ����֮��Ľ��
  unsigned char *label;
  unsigned char* bk;
public:
  // ��ǰ����ͼ��
  const double* Background( void ) const {
    return b;
  }
  // ��ͨ������
  const unsigned int* BlobRst( void ) const {
    return blob;
  }
  // �����߶�
  int SceneH( void ) const {
    return row;
  }
  // �������
  int SceneW( void ) const {
    return col;
  }
public:
  int Init( double Afa = 0.005,
       double Thd = 0.4,
       double VirV = 5.0,
       double VirW = 0.05,
       double shadowp0 = 0.95,
       double shadowp1 = 0.3,
       double shadowp2 = 0.3,
       double shadowp3 = 0.1,
       int R = 120,
       int C = 160,
       int Num = 3,
       int MinPts = 20,
       bool IsRectLabel = true,
       bool IsRectShadowX = true,
       bool IsRectShadowY = false,
       bool IsRectShape = true
     ) {
    afa =( Afa ),
      thrd =( Thd ),
      vir_v=( VirV ),
      vir_w=( VirW ),
      row =( R ),
      col =( C ),
      kind=( Num ),
      min_pts=( MinPts ),
      s=( new int[ row * col ] ),
      w=( new double[ row * col * kind ] ),
      m=( new double[ row * col * kind * 3 ] ),
      v=( new double[ row * col * kind ] ),
      b=( new double[ row * col * 3 ] ),
      blob=( new unsigned int[ col * row ] ),
      label=( new unsigned char[ row * col ] ),
      rlabel =( IsRectLabel ),
      rshadowx=( IsRectShadowX ),
      rshadowy=( IsRectShadowY ),
      rshape =( IsRectShape );
    *( shadowp + 0 ) = shadowp0;
    *( shadowp + 1 ) = shadowp1;
    *( shadowp + 2 ) = shadowp2;
    *( shadowp + 3 ) = shadowp3;
    bk = new uchar[row * col * 3];
    for ( int i = 0; i < row * col * kind; ++i ) {
      w[i] = 0.0;
      v[i] = 0.0;
    }
    for ( i = 0; i < row * col * kind * 3; ++i ) {
      m[i] = 0.0;
    }
    for ( i = 0; i < row * col * 3; ++i ) {
      b[i] = 0.0;
    }
    for ( i = 0; i < row * col; ++i ) {
      blob[i] = 0;
    }
    SetS();
    return 0;
  }
  ~GMM() {
    UnInit();
  }
  int UnInit() {
    delete [] label;
    label = NULL;
    delete [] blob;
    blob = NULL;
    delete [] b;
    b = NULL;
    delete [] v;
    w = NULL;
    delete [] m;
    m = NULL;
    delete [] w;
    v = NULL;
    delete [] s;
    s = NULL;
    delete [] bk;
    bk = NULL;
    return 0;
  }
  // ��ʼ������㵱ǰ���е��������ÿһ�����½�������ģ�Ͷ�������ô˺���
  void SetS( void ) {
    for ( int i = 0; i < row * col; ++i ) {
      *( s + i ) = 1;
    }
  }
  BlobRect br[100];
  int GetRect() {
    int i, j, n=0;
    memset(br, 0, sizeof(BlobRect)*100);
    for (i=0; i<row; ++i) {
      for (j=0; j<col; ++j) {
        int k=blob[i*col+j];
        if (k<100) {
          if (0==br[k].n) {
            br[k].l=br[k].r=j;
            br[k].t=br[k].b=i;
          } else {
            br[k].l=MIN(j, br[k].l);
            br[k].r=MAX(j, br[k].r);
            br[k].t=MIN(i, br[k].t);
            br[k].b=MAX(i, br[k].b);
          }
          br[k].n++;
          n=MAX(n, k);
        }
      }
    }
    ++n;
#define BlobRectCmp(x, y)  (x).n>(y).n
    QSORT(br, br+n, BlobRectCmp, BlobRect);
    return n;
  }
  unsigned char* GetBk() {
    int i, n=row*col*3;
    for (i=0; i<n; ++i) {
      bk[i]=(unsigned char)(b[i]);
    }
    return bk;
  }
  // ������ģ��������������ģ��ʱÿ֡ͼƬ����һ�θú���
  void SetupBG( const unsigned char* src, int step, int num );
  // ��������ģȨ��, ��ֵ, �������������ı��ĵ�
  void Export2File( const char* wf, const char* mf, const char* vf ) const;
  // ǰ�����ͺ��ڴ���, ���ؼ��������
  int Detect( const unsigned char* src, int step, const unsigned char *pmask = NULL );
  // ���������ܱ����ĺ�����ÿ�μ��ǰ�������
  void GetIb( void );
  // ǰ�����
  void DetectForeGround( const unsigned char* pt, int step, const unsigned char *pmask = NULL );
  // �ն������
  void RectifyLabel( void );
  // ����ͻ��������Ӱ�������
  void Rectify( const unsigned char *pt, int step );
  // ��ʴ, ��������
  int RectifyShape( void );
};
#endif
