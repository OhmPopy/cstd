
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <float.h>



#ifndef MALLOC
#define MALLOC(TYPE, size)(TYPE*)pmalloc(sizeof(TYPE) *(size_t)(size))
#endif

#ifndef MAX
#define MAX(a, b)  ((a) > (b) ? (a) : (b))
#endif

/********************************************************************************
                        shen-castan���ӵĹ��ܺ���
********************************************************************************/

#define ISEF1D(_x, _y, _A, _width, _cn, b, b1, b2)                             \
  do {                                                                           \
    int _I;                                                                      \
    *(_A) = b1 * *(_x);                                                          \
    /* ������س�Ա���� */                                                       \
    for (_I=1; _I<(_width); ++_I)                                                \
      *(_A+_I) = b1 * *(_x+_I*(_cn)) + b * *(_A+_I-1);                           \
    /* ����ISEF�˲���ͼ��ı�Ե��� */                                           \
    *(_y+(_width-1)*(_cn)) = *(_A+_width-1);                                     \
    for (_I=0; _I<(_width)-1; ++_I)                                              \
      *(_y+_I*(_cn)) = *(_A+_I);                                                 \
    *(_A+_width-1) = b2 * *(_x+(_width-1)*(_cn));                                \
    /* �������س�Ա���� */                                                     \
    for (_I=(_width)-2; _I>=0; --_I)                                             \
      *(_A+_I) = b2 * *(_x+_I*(_cn)) + b * *(_A+_I+1);                           \
    for (_I=0; _I<(_width)-1; ++_I)                                              \
      *(_y+_I*(_cn)) += *(_A+_I+1);                                              \
  } while(0)

/******************************************************************************
  ���ܣ���ISEF�˲�����ͼ������˲�
  ���룺x, y  - �洢ͼ��Ļ�������
        nrows - ͼ�������
        ncols - ͼ�������
******************************************************************************/
void compute_ISEF(const float* x, float* y, int nrows, int ncols, int step, float b)
{
  int row, col;
  float b1 = (1.0f - b) / (1.0f + b);
  float b2 = b * b1;
  float* A = MALLOC(float, nrows > ncols ? nrows : ncols);
  float* z = MALLOC(float, nrows * ncols);

  /* �����ڴ�ֱ�����˲� */
  for (col = 0; col < ncols; col += 1) {
    ISEF1D((x + col), (z + col), A, nrows, step, b, b1, b2);
  }

  /* ��ˮƽ�����˲� */
  for (row = 0; row < nrows * ncols; row += ncols) {
    ISEF1D((z + row), (y + row), A, ncols, 1, b, b1, b2);
  }

  /* �ͷ��ڴ� */
  pfree(A);
  pfree(z);
}

/******************************************************************************
  ���ܣ��������ص���ݶ�ֵ
  ���룺bli��orig-�洢ͼ��Ļ�����
******************************************************************************/
float compute_adaptive_gradient(const unsigned char* bli, const float* orig, int half_size, int step, int step2)
{
  register int i, j;
  float sum_on, sum_off;
  float avg_on, avg_off;
  int num_on, num_off;

  sum_on = sum_off = 0.0f;
  num_on = num_off = 0;

  for (i = (-half_size); i <= (half_size); i++) {
    const unsigned char* _bli = bli + i * step2;
    const float* _orig = orig + i * step;

    for (j = (-half_size); j <= (half_size); j++) {
      if (_bli[ j ]) {
        sum_on += _orig[ j ];
        num_on++;
      }
      else {
        sum_off += _orig[ j ];
        num_off++;
      }
    }
  }

  avg_off = (num_off) ? sum_off / num_off : 0;
  avg_on = (num_on) ? sum_on / num_on : 0;
  return (avg_off - avg_on);
}

/******************************************************************************
  ���ܣ�ȷ���ݶȵĸߡ�������ֵ
  ���룺nr-ͼ�������
        nc-ͼ�������
  �����high_thresh - �ߡ�������ֵ
******************************************************************************/
double estimate_thresh(float* lap, int sz, float ratio)
{
  float vmax = -FLT_MAX, vmin = FLT_MAX, scale, x;
  int j, k, hist[ 256 ], count;
  memset(hist, 0, 256 * sizeof(int));

  for (j = 0; j < sz; j++) {
    x = lap[ j ];

    if (vmin > x) {
      vmin = x;
    }

    if (vmax < x) {
      vmax = x;
    }
  }

  scale = 256.0f / ((vmax - vmin) + 1.f);

  for (j = 0; j < sz; j++) {
    x = lap[ j ];
    k = (int)((x - vmin) * scale);
    ++(hist[ k ]);
  }

  k = 255;
  j = (int)(ratio * sz);
  count = hist[ 255 ];

  while (count < j) {
    k--;

    if (k < 0) {
      break;
    }

    count += hist[ k ];
  }

  return (double) k / scale + vmin;
}

/******************************************************************************
  ���ܣ�������ɢ�ı�Ե��
  ���룺i,j-��Ӧ�������
        level-
******************************************************************************/
int mark_connected(unsigned char* edges, int step)
{
  int notChainEnd;

  /* ��ǰ�㱻���ʹ��򷵻� */
  if (*edges) {
    return 0;
  }

  *edges = 2; //((lap[i][j]) > param->low_thresh) ? 1 : 255;

  notChainEnd = 0;

  notChainEnd |= mark_connected(edges + 1, step);
  notChainEnd |= mark_connected(edges - 1, step);
  notChainEnd |= mark_connected(edges + step + 1, step);
  notChainEnd |= mark_connected(edges + step , step);
  notChainEnd |= mark_connected(edges + step - 1, step);
  notChainEnd |= mark_connected(edges - step - 1, step);
  notChainEnd |= mark_connected(edges - step , step);
  notChainEnd |= mark_connected(edges - step + 1, step);

  /* ��Եϸ�� */
  //int thinFactor = param->thinFactor;
  //if (notChainEnd && thinFactor && (level > 0) && (level%thinFactor)  ) {
  //  edges[i][j] = 255;
  //}

  return 1;
}

#define OUTLINE 1

/******************************************************************************
  ���ܣ���shen-castan������ȡͼ��ı�Ե
  ���룺im  - ָ��洢��ԭͼ��Ľṹ��ָ��
        res - �洢��Ե��ȡ�������Ľṹ���ָ��
        b   - ƽ������ 0 < b < 1�����Ʊ�Ե������Խ���ԵԽ��
******************************************************************************/
int shen_castan(int h, int w, const unsigned char* src, int al, unsigned char* edges, int bl, float b)
{
  int i, j;
  float* orig, *smoothed;
  unsigned char* bli;
  int nrows = h, ncols = w;
  int src_step = ncols;
  int dst_step = src_step;
  int beg = OUTLINE * ncols + OUTLINE;
  float ratio = 0.99f;
  const int window_size = 3, do_hysteresis = 0;
  double high_thresh, low_thresh; /* ��Ե����ĸߵ����� */

  int bufsize = nrows * ncols;
  /* ������ͼ��ת��Ϊ������ */
  orig = MALLOC(float, 2 * bufsize);
  bli = MALLOC(unsigned char, bufsize);

  if (!orig || !bli) {
    return 0;
  }

  memset(bli, 0, sizeof(unsigned char) * bufsize);
  smoothed = orig + bufsize;

  for (i = 0; i < nrows; i++) {
    const unsigned char* _src = src + i * src_step;
    float* _orig = orig + i * ncols;

    for (j = 0; j < ncols; j++) {
      _orig[ j ] = (float)(_src[ j ]);
    }
  }

  /* ��ISEF�˲�����ͼ������˲� */
  //smoothed = f2d(nrows, ncols);
  compute_ISEF(orig, smoothed, nrows, ncols, ncols, b);

  /* ������Ƶ�������˹�˲�ͼ�� */
  /* smoothed>orig������ֵ��Ϊ1������ĸ�Ϊ0 */
  for (i = 0; i < bufsize; ++i) {
    bli[ i ] = (smoothed[ i ] > orig[ i ]);
  }

  /* ͨ��������˹�˲�ͼ����ݶ����޽��б�Ե��⣬��ȡ�㽻��� */
  /* ���ͼ���Ե */
  memset(edges, 1, ncols);
  memset(edges + dst_step * (nrows - 1), 1, ncols);

  for (i = 1; i < nrows - 1; i++) {
    int pos = ncols * i;
    unsigned char* _bli = bli + pos;
    unsigned char* _edges = edges + dst_step * i;
    float* _orig = orig + pos;
    float* _smoothed = smoothed + pos;
    _edges[ 0 ] = _edges[ ncols - 1 ] = 1;

    for (j = 0; j < ncols; j++) {
      /* ���������������˹�˲�ͼ����㽻����������һ������ */
      int is_candidate_edge;
#define FDFDF(_A, _B, _k, _N)  (0 == *(_A + _k) ? (*(_B+(_k)) > *(_B-(_k))) : (_N) )

      /* �����㽻��� */
#define IS_CANDIDATE_EDGE(_A, _B, _step) (1 == *(_A) ?   \
    FDFDF(_A, _B, _step, FDFDF(_A, _B, 1, FDFDF(_A, _B, -_step, FDFDF(_A, _B, -1, 0)))) : 0)

      is_candidate_edge = IS_CANDIDATE_EDGE(_bli + j, _smoothed + j, ncols);
#undef FDFDF
#undef IS_CANDIDATE_EDGE

      if (is_candidate_edge) {
        /* �����ݶ������о� */
        _orig[ j ] = compute_adaptive_gradient(_bli + j, _smoothed + j, window_size / 2, ncols, ncols);
        _edges[ j ] = 0;
      }
      else {
        _orig[ j ] = 0.0;
        _edges[ j ] = 1;
      }
    }
  }

  high_thresh = estimate_thresh(orig, nrows * ncols, ratio);

  if (do_hysteresis) {
    low_thresh = (high_thresh) / 2;
  }
  else {
    low_thresh = high_thresh;
  }

  /* ͨ���ݶ����ޣ�ѡȡ��Ե�㣬ȥ��α��Ե�� */
  for (i = 0; i < nrows; i++) {
    unsigned char* _edges = edges + dst_step * i;
    float* _orig = orig + ncols * i;

    for (j = 0; j < ncols; j++) {
      if ((_orig[ j ]) > high_thresh) {
        mark_connected(_edges + j, ncols);
      }
    }
  }

  for (i = 0; i < nrows; i++) {
    unsigned char* _edges = edges + dst_step * i;

    for (j = 0; j < ncols; j++) {
      _edges[ j ] = (unsigned char) - (_edges[ j ] >> 1);
    }
  }

  pfree(bli);
  pfree(orig);
  return 1;
}
