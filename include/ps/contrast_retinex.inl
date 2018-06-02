// Retinex�������������Ĥ(Retina)�ʹ���Ƥ��(Cortex) ��������Ϲ��ɵġ�Retinex������Ҫ������������������ݣ��������ɫ��������Գ����� �в��Ͷ̲����ߵķ������������ģ��������ɷ����ǿ�ȵľ���ֵ�����ģ������ɫ�ʲ��ܹ��� �Ǿ����Ե�Ӱ��,����һ���� ��
// ����Retinex���ۣ����۸�֪���������ȡ���ڻ������������������������ķ��䣬����ѧ���ʽΪ��
// I(x,y)=L(x,y)*R(x,y)��������������������������������������������������������(2-1)
// ����ʽ�У� I(x,y)�����۲����������յ���ͼ���źţ�L(x,y)���������������� ��R(x,y)��ʾЯ��ͼ��ϸ����Ϣ��Ŀ������ķ������ ��
// ��(2-1)ʽ����ȡ����������׿����������ʵõ�����ı�����ò�����й�ϵʽ ��
// Log[R(x,y)] = Log[I(x,y)]-Log[L(x,y)];��������������������������������  ��������(2-2)
// ����������۵Ľ���һ���򵥵�ע�Ͱɡ�������������õ�ͼ�����ϣ�����������������Ѿ���õ�һ��ͼ������I(x,y�����������Ӧ��R(x,y)����R(x,y)��Ϊ����ǿ���ͼ�����ڵĹؼ�����εõ�L(X,Y)��Retinex���۵������ָ�����L(x,y������ͨ����ͼ������I(x,y�����и�˹ģ�����õ����ܶ������ж��г����Ǹ�����/Χ�ƺ����Լ���Ҫ��һ����Kֵ����ĺܶ����ֶ���������ʲô�ˣ���ʵ����һ��ģ�����ѡ���ʵ�����õĽǶ���˵��Ҳ�����þ�ֵģ���������˹ģ����
// �������㷨��ϸ·�ͺܼ��ˣ����岽�����£�
// 1������: ԭʼͼ������I(x,y),�߶ȣ�Ҳ������ν��ģ���İ뾶��
// 2������(1) ����ԭʼͼ��ָ���߶Ƚ���ģ�����ͼ�� L(x,y);
// (2) ����2-2ʽ�ļ��㷽������� Log[R(x,y)]��ֵ��
// (3) �� Log[R(x,y)]����Ϊ0��255��Χ������ֵ����Ϊ���յ������
// ���Կ��ó����㷨�ܼ򵥣�����ĵĶ����������ڸ�˹ģ����ʵ�֡����ڸ�˹ģ���������кܶ�����Ż������²ο�������Ĳο�������ܺ����к����ṩ�ġ�
// ע�⵽һ�㣬�ƺ���������ʱ��û��˭�Ὣ Log[R(x,y)]����Exp�����������ֱ�ӵõ�R(x,y)������Ϊʲô�����޷�������ȷ�Ĵ𰸡�
// �����ķ�ʽ��ʵ�кܶ��֣�����������ںܴ�ĳ̶��϶Դ����Ч�����ž����Ե�Ӱ�졣������û��������һƪ���Ķ���һ�齲�ĺ������Ҳ��֪�����ǵ���Щ��������ȡ�õģ�һ����򵥵ķ�ʽ���Ǽ����Log[R(x,y)]�����ֵMax����СֵMin,Ȼ���ÿһ��ֵValue������������������ʽΪ��
// R(x,y) = ( Value - Min ) / (Max - Min) * (255-0)                           (2-3)

typedef unsigned char guchar;
typedef int gint;
typedef int gboolean;
typedef float gfloat;
typedef double gdouble;

#define MAX_RETINEX_SCALES    8
#define MIN_GAUSSIAN_SCALE   16
#define MAX_GAUSSIAN_SCALE  250
#define SCALE_WIDTH         150
#define ENTRY_WIDTH           4


typedef struct {
  gint     scale;
  gint     nscales;
  gint     scales_mode;
  gdouble   cvar;
} RetinexParams;

/*
  Definit comment sont repartis les
  differents filtres en fonction de
  l'echelle (~= ecart type de la gaussienne)
 */
#define RETINEX_UNIFORM 0
#define RETINEX_LOW     1
#define RETINEX_HIGH    2

static gdouble RetinexScales[MAX_RETINEX_SCALES];

typedef struct {
  gint    N;
  gdouble sigma;
  gdouble B;
  gdouble b[4];
} gauss3_coefs;

/*
 * Private variables.
 */
static RetinexParams rvals = {
  240,             /* ���߶� Scale Biggest scale value */
  3,               /* �߶����� Scales Number of scales*/
  RETINEX_UNIFORM, /* �Աȶ� Echelles reparties uniformement  Retinex distribution through scales*/
  1.2f              /* A voir Variance value*/
};


// N_("Enhance contrast using the Retinex method"),
// "The Retinex Image Enhancement Algorithm is an "
// "automatic image enhancement method that enhances "
// "a digital image in terms of dynamic range "
// "compression, color independence from the spectral "
// "distribution of the scene illuminant, and "
// "color/lightness rendition.",
// "Fabien Pelisson",
// "Fabien Pelisson",
// "2003",
// N_("Retine_x..."),
// "RGB*",
// GIMP_PLUGIN,
// G_N_ELEMENTS (args), 0,
// args, NULL);




/*
 * calculate scale values for desired distribution.
 */
static void retinex_scales_distribution(gdouble* scales, gint nscales, gint mode, gint s)
{
  if (nscales == 1) {
    /* For one filter we choose the median scale */
    scales[0] = (gdouble) s / 2;
  }
  else if (nscales == 2) {
    /* For two filters whe choose the median and maximum scale */
    scales[0] = (gdouble) s / 2;
    scales[1] = (gdouble) s;
  }
  else {
    gdouble size_step = (gdouble) s / (gdouble) nscales;
    gint   i;

    switch (mode) {
    case RETINEX_UNIFORM:
      for (i = 0; i < nscales; ++i) {
        scales[i] = 2.f + (gdouble) i * size_step;
      }

      break;

    case RETINEX_LOW:
      size_step = (gdouble) log(s - 2.0) / (gdouble) nscales;

      for (i = 0; i < nscales; ++i) {
        scales[i] = 2.f + pow(10, (i * size_step) / log(10));
      }

      break;

    case RETINEX_HIGH:
      size_step = (gdouble) log(s - 2.0) / (gdouble) nscales;

      for (i = 0; i < nscales; ++i) {
        scales[i] = s - pow(10, (i * size_step) / log(10));
      }

      break;

    default:
      break;
    }
  }
}

/*
 * Calculate the coefficients for the recursive filter algorithm
 * Fast Computation of gaussian blurring.
 */
static void compute_coefs3(gauss3_coefs* c, gdouble sigma)
{
  /*
   * Papers:  "Recursive Implementation of the gaussian filter.",
   *          Ian T. Young , Lucas J. Van Vliet, Signal Processing 44, Elsevier 1995.
   * formula: 11b       computation of q
   *          8c        computation of b0..b1
   *          10        alpha is normalization constant B
   */
  gdouble q, q2, q3;

  q = 0;

  if (sigma >= 2.5) {
    q = 0.98711 * sigma - 0.96330;
  }
  else if ((sigma >= 0.5) && (sigma < 2.5)) {
    q = 3.97156 - 4.14554 * (gdouble) sqrt((double) 1 - 0.26891 * sigma);
  }
  else {
    q = 0.1147705018520355224609375;
  }

  q2 = q * q;
  q3 = q * q2;
  c->b[0] = (1.57825 + (2.44413 * q) + (1.4281 * q2) + (0.422205 * q3));
  c->b[1] = ((2.44413 * q) + (2.85619 * q2) + (1.26661 * q3));
  c->b[2] = (-((1.4281 * q2) + (1.26661 * q3)));
  c->b[3] = ((0.422205 * q3));
  c->B = 1.0 - ((c->b[1] + c->b[2] + c->b[3]) / c->b[0]);
  c->sigma = sigma;
  c->N = 3;

  /*
    g_printerr ("q %f\n", q);
    g_printerr ("q2 %f\n", q2);
    g_printerr ("q3 %f\n", q3);
    g_printerr ("c->b[0] %f\n", c->b[0]);
    g_printerr ("c->b[1] %f\n", c->b[1]);
    g_printerr ("c->b[2] %f\n", c->b[2]);
    g_printerr ("c->b[3] %f\n", c->b[3]);
    g_printerr ("c->B %f\n", c->B);
    g_printerr ("c->sigma %f\n", c->sigma);
    g_printerr ("c->N %d\n", c->N);
  */
}

static int gausssmooth(gdouble* in, gdouble* out, gint size, gint rowstride, gauss3_coefs* c)
{
  /*
   * Papers:  "Recursive Implementation of the gaussian filter.",
   *          Ian T. Young , Lucas J. Van Vliet, Signal Processing 44, Elsevier 1995.
   * formula: 9a        forward filter
   *          9b        backward filter
   *          fig7      algorithm
   */
  gint i, n, bufsize;
  gdouble* w1, *w2;

#define g_try_malloc malloc
#define g_free free
  /* forward pass */
  bufsize = size + 3;
  size -= 1;
  w1 = (gdouble*) g_try_malloc(bufsize * sizeof(gdouble));
  w2 = (gdouble*) g_try_malloc(bufsize * sizeof(gdouble));
  w1[0] = in[0];
  w1[1] = in[0];
  w1[2] = in[0];

  for (i = 0 , n = 3; i <= size ; i++, n++) {
    w1[n] = (gdouble)(c->B * in[i * rowstride] +
        ((c->b[1] * w1[n - 1] +
            c->b[2] * w1[n - 2] +
            c->b[3] * w1[n - 3]) / c->b[0]));
  }

  /* backward pass */
  w2[size + 1] = w1[size + 3];
  w2[size + 2] = w1[size + 3];
  w2[size + 3] = w1[size + 3];

  for (i = size, n = i; i >= 0; i--, n--) {
    w2[n] = out[i * rowstride] = (gdouble)(c->B * w1[n] +
        ((c->b[1] * w2[n + 1] +
            c->b[2] * w2[n + 2] +
            c->b[3] * w2[n + 3]) / c->b[0]));
  }

  g_free(w1);
  g_free(w2);
  return 0;
}

// Calculate the average and variance in one go.
static void compute_mean_var(gdouble* src, gdouble* mean, gdouble* var, gint size, gint bytes)
{
  gdouble vsquared;
  gint i, j;
  gdouble* psrc;

  vsquared = 0;
  *mean = 0;

  for (i = 0; i < size; i += bytes) {
    psrc = src + i;

    for (j = 0 ; j < 3 ; j++) {
      *mean += psrc[j];
      vsquared += psrc[j] * psrc[j];
    }
  }

  *mean /= (gdouble) size; /* mean */
  vsquared /= (gdouble) size; /* mean (x^2) */
  *var = (vsquared - (*mean * *mean));
  *var = sqrt(*var); /* var */
}

// MSRCR = MultiScale Retinex with Color Restoration
//  * This function is the heart of the algo.
//  * (a)  Filterings at several scales and sumarize the results.
//  * (b)  Calculation of the final values.
int MSRCR(gint width, gint height, const guchar* src, guchar* b, int bl, gint bytes)
{
  gint          scale, row, col;
  gint          i, j;
  gint          size;
  gint          pos;
  gint          channel;
  const guchar*       psrc = NULL;            /* backup pointer for src buffer */
  gdouble*       dst  = NULL;            /* float buffer for algorithm */
  gdouble*       pdst = NULL;            /* backup pointer for float buffer */
  gdouble*       in, *out;
  gint          channelsize;            /* Float memory cache for one channel */
  gdouble        weight;
  gauss3_coefs  coef;
  gdouble        mean, var;
  gdouble        mini, range, maxi;
  gdouble        alpha;
  gdouble        gain;
  gdouble        offset;

#define g_warning printf
  /* Allocate all the memory needed for algorithm*/
  size = width * height * bytes;
  dst = g_try_malloc(size * sizeof(gdouble));

  if (dst == NULL) {
    g_warning("Failed to allocate memory");
    return 0;
  }

  memset(dst, 0, size * sizeof(gdouble));

  channelsize  = (width * height);
  in  = (gdouble*) g_try_malloc(channelsize * sizeof(gdouble));

  if (in == NULL) {
    g_free(dst);
    g_warning("Failed to allocate memory");
    return 0; /* do some clever stuff */
  }

  out  = (gdouble*) g_try_malloc(channelsize * sizeof(gdouble));

  if (out == NULL) {
    g_free(in);
    g_free(dst);
    g_warning("Failed to allocate memory");
    return 0; /* do some clever stuff */
  }



  // Calculate the scales of filtering according to the
  // number of filter and their distribution.
  retinex_scales_distribution(RetinexScales, rvals.nscales, rvals.scales_mode, rvals.scale);


  // Filtering according to the various scales.
  // Summerize the results of the various filters according to a
  // specific weight(here equivalent for all).
  weight = 1. / (gdouble) rvals.nscales;


  // The recursive filtering algorithm needs different coefficients according
  // to the selected scale (~ = standard deviation of Gaussian).
  pos = 0;

  for (channel = 0; channel < 3; channel++) {
    for (i = 0, pos = channel; i < channelsize ; i++, pos += bytes) {
      /* 0-255 => 1-256 */
      in[i] = (gdouble)(src[pos] + 1.0);
    }

    for (scale = 0; scale < rvals.nscales; scale++) {
      compute_coefs3(&coef, RetinexScales[scale]);

      //Filtering (smoothing) Gaussian recursive.
      //Filter rows first
      for (row = 0 ; row < height; row++) {
        pos =  row * width;
        gausssmooth(in + pos, out + pos, width, 1, &coef);
      }

      memcpy(in,  out, channelsize * sizeof(gdouble));
      memset(out, 0  , channelsize * sizeof(gdouble));

      //Filtering (smoothing) Gaussian recursive.
      //Second columns
      for (col = 0; col < width; col++) {
        pos = col;
        gausssmooth(in + pos, out + pos, height, width, &coef);
      }

      // Summarize the filtered values.
      //  In fact one calculates a ratio between the original values and the filtered values.
      for (i = 0, pos = channel; i < channelsize; i++, pos += bytes) {
        dst[pos] += weight * (log(src[pos] + 1.) - log(out[i]));
      }
    }
  }

  g_free(in);
  g_free(out);

  // Final calculation with original value and cumulated filter values.
  // The parameters gain, alpha and offset are constants.

  // Ci(x,y)=log[a Ii(x,y)]-log[ Ei=1-s Ii(x,y)]

  alpha  = 128.;
  gain   = 1.;
  offset = 0.;

  for (i = 0; i < size; i += bytes) {
    gdouble logl;

    psrc = src + i;
    pdst = dst + i;

    logl = log((gdouble)psrc[0] + (gdouble)psrc[1] + (gdouble)psrc[2] + 3.);

    pdst[0] = gain * ((log(alpha * (psrc[0] + 1.)) - logl) * pdst[0]) + offset;
    pdst[1] = gain * ((log(alpha * (psrc[1] + 1.)) - logl) * pdst[1]) + offset;
    pdst[2] = gain * ((log(alpha * (psrc[2] + 1.)) - logl) * pdst[2]) + offset;
  }

  // if (!preview_mode)
  // gimp_progress_update ((2.0 + (rvals.nscales * 3)) / ((rvals.nscales * 3) + 3));

  //Adapt the dynamics of the colors according to the statistics of the first and second order.
  // The use of the variance makes it possible to control the degree of saturation of the colors.
  pdst = dst;

  compute_mean_var(pdst, &mean, &var, size, bytes);
  mini = mean - rvals.cvar * var;
  maxi = mean + rvals.cvar * var;
  range = maxi - mini;

  if (!range) {
    range = 1.0;
  }

  for (i = 0; i < size; i += bytes) {
    guchar* b1 = b + i;
    psrc = src + i;
    pdst = dst + i;

    for (j = 0 ; j < 3 ; j++) {
      gdouble c = 255 * (pdst[j] - mini) / range;

      b1[j] = (guchar) CLAMP(c, 0, 255);
    }
  }

  g_free(dst);
  return 0;
}

