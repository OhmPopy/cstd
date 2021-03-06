
#ifndef _CC_MATRIX_H_
#define _CC_MATRIX_H_

#define icvCopyVector(src, dst, len) memcpy((dst), (src), (len)*sizeof((dst)[0]))
#define icvSetZero(dst, len) memset((dst), 0, (len)*sizeof((dst)[0]))

#define icvCopyVector_32f(src, len, dst) memcpy((dst),(src),(len)*sizeof(float))
#define icvSetZero_32f(dst, cols, rows) memset((dst),0,(rows)*(cols)*sizeof(float))
#define icvCopyVector_64d(src, len, dst) memcpy((dst),(src),(len)*sizeof(double))
#define icvSetZero_64d(dst, cols, rows) memset((dst),0,(rows)*(cols)*sizeof(double))
#define icvCopyMatrix_32f(src, w, h, dst) memcpy((dst),(src),(w)*(h)*sizeof(float))
#define icvCopyMatrix_64d(src, w, h, dst) memcpy((dst),(src),(w)*(h)*sizeof(double))

#define icvCreateVector_32f(len)  (float*)cAlloc((len)*sizeof(float))
#define icvCreateVector_64d(len)  (double*)cAlloc((len)*sizeof(double))
#define icvCreateMatrix_32f(w, h)  (float*)cAlloc((w)*(h)*sizeof(float))
#define icvCreateMatrix_64d(w, h)  (double*)cAlloc((w)*(h)*sizeof(double))

#define icvDeleteVector(vec)  cFree(&(vec))
#define icvDeleteMatrix icvDeleteVector

#define icvAddMatrix_32f(src1, src2, dst, w, h) \
    icvAddVector_32f((src1), (src2), (dst), (w)*(h))

#define icvSubMatrix_32f(src1, src2, dst, w, h) \
    icvSubVector_32f((src1), (src2), (dst), (w)*(h))

#define icvNormVector_32f(src, len)  \
    sqrt(icvDotProduct_32f(src, src, len))

#define icvNormVector_64d(src, len)  \
    sqrt(icvDotProduct_64d(src, src, len))


#define icvDeleteMatrix icvDeleteVector

#define icvCheckVector_64f(ptr, len)
#define icvCheckVector_32f(ptr, len)

CC_INLINE double icvSum_32f(const float* src, int len)
{
    double s = 0;
    for(int i = 0; i < len; i++) s += src[i];

    icvCheckVector_64f(&s, 1);

    return s;
}

CC_INLINE double icvDotProduct_32f(const float* src1, const float* src2, int len)
{
    double s = 0;
    for(int i = 0; i < len; i++) s += src1[i]*src2[i];

    icvCheckVector_64f(&s, 1);

    return s;
}


CC_INLINE double icvDotProduct_64f(const double* src1, const double* src2, int len)
{
    double s = 0;
    for(int i = 0; i < len; i++) s += src1[i]*src2[i];

    icvCheckVector_64f(&s, 1);

    return s;
}


CC_INLINE void icvMulVectors_32f(const float* src1, const float* src2,
                                  float* dst, int len)
{
    int i;
    for(i = 0; i < len; i++)
        dst[i] = src1[i] * src2[i];

    icvCheckVector_32f(dst, len);
}

CC_INLINE void icvMulVectors_64d(const double* src1, const double* src2,
                                  double* dst, int len)
{
    int i;
    for(i = 0; i < len; i++)
        dst[i] = src1[i] * src2[i];

    icvCheckVector_64f(dst, len);
}


CC_INLINE void icvAddVector_32f(const float* src1, const float* src2,
                                  float* dst, int len)
{
    int i;
    for(i = 0; i < len; i++)
        dst[i] = src1[i] + src2[i];

    icvCheckVector_32f(dst, len);
}

CC_INLINE void icvAddVector_64d(const double* src1, const double* src2,
                                  double* dst, int len)
{
    int i;
    for(i = 0; i < len; i++)
        dst[i] = src1[i] + src2[i];

    icvCheckVector_64f(dst, len);
}


CC_INLINE void icvSubVector_32f(const float* src1, const float* src2,
                                  float* dst, int len)
{
    int i;
    for(i = 0; i < len; i++)
        dst[i] = src1[i] - src2[i];

    icvCheckVector_32f(dst, len);
}

CC_INLINE void icvSubVector_64d(const double* src1, const double* src2,
                                  double* dst, int len)
{
    int i;
    for(i = 0; i < len; i++)
        dst[i] = src1[i] - src2[i];

    icvCheckVector_64f(dst, len);
}


#define icvAddMatrix_64d(src1, src2, dst, w, h) \
    icvAddVector_64d((src1), (src2), (dst), (w)*(h))

#define icvSubMatrix_64d(src1, src2, dst, w, h) \
    icvSubVector_64d((src1), (src2), (dst), (w)*(h))


CC_INLINE void icvSetIdentity_32f(float* dst, int w, int h)
{
    int i, len = MIN(w, h);
    icvSetZero_32f(dst, w, h);
    for(i = 0; len--; i += w+1)
        dst[i] = 1.f;
}


CC_INLINE void icvSetIdentity_64d(double* dst, int w, int h)
{
    int i, len = MIN(w, h);
    icvSetZero_64d(dst, w, h);
    for(i = 0; len--; i += w+1)
        dst[i] = 1.;
}


CC_INLINE void icvTrace_32f(const float* src, int w, int h, float* trace)
{
    int i, len = MIN(w, h);
    double sum = 0;
    for(i = 0; len--; i += w+1)
        sum += src[i];
    *trace = (float)sum;

    icvCheckVector_64f(&sum, 1);
}


CC_INLINE void icvTrace_64d(const double* src, int w, int h, double* trace)
{
    int i, len = MIN(w, h);
    double sum = 0;
    for(i = 0; len--; i += w+1)
        sum += src[i];
    *trace = sum;

    icvCheckVector_64f(&sum, 1);
}


CC_INLINE void icvScaleVector_32f(const float* src, float* dst,
                                   int len, double scale)
{
    int i;
    for(i = 0; i < len; i++)
        dst[i] = (float)(src[i]*scale);

    icvCheckVector_32f(dst, len);
}


CC_INLINE void icvScaleVector_64d(const double* src, double* dst,
                                   int len, double scale)
{
    int i;
    for(i = 0; i < len; i++)
        dst[i] = src[i]*scale;

    icvCheckVector_64f(dst, len);
}


CC_INLINE void icvTransposeMatrix_32f(const float* src, int w, int h, float* dst)
{
    int i, j;

    for(i = 0; i < w; i++)
        for(j = 0; j < h; j++)
            *dst++ = src[j*w + i];
        
    icvCheckVector_32f(dst, w*h);
}

CC_INLINE void icvTransposeMatrix_64d(const double* src, int w, int h, double* dst)
{
    int i, j;

    for(i = 0; i < w; i++)
        for(j = 0; j < h; j++)
            *dst++ = src[j*w + i];

    icvCheckVector_64f(dst, w*h);
}

CC_INLINE void icvDetMatrix3x3_64d(const double* mat, double* det)
{
    #define m(y,x) mat[(y)*3 + (x)]
    
    *det = m(0,0)*(m(1,1)*m(2,2) - m(1,2)*m(2,1)) -
           m(0,1)*(m(1,0)*m(2,2) - m(1,2)*m(2,0)) +
           m(0,2)*(m(1,0)*m(2,1) - m(1,1)*m(2,0));

    #undef m

    icvCheckVector_64f(det, 1);
}


CC_INLINE void icvMulMatrix_32f(const float* src1, int w1, int h1,
                                 const float* src2, int w2, int h2,
                                 float* dst)
{
    int i, j, k;

    if(w1 != h2)
    {
        assert(0);
        return;
    }

    for(i = 0; i < h1; i++, src1 += w1, dst += w2)
        for(j = 0; j < w2; j++)
        {
            double s = 0;
            for(k = 0; k < w1; k++)
                s += src1[k]*src2[j + k*w2];
            dst[j] = (float)s;
        }

    icvCheckVector_32f(dst, h1*w2);
}


CC_INLINE void icvMulMatrix_64d(const double* src1, int w1, int h1,
                                 const double* src2, int w2, int h2,
                                 double* dst)
{
    int i, j, k;

    if(w1 != h2)
    {
        assert(0);
        return;
    }

    for(i = 0; i < h1; i++, src1 += w1, dst += w2)
        for(j = 0; j < w2; j++)
        {
            double s = 0;
            for(k = 0; k < w1; k++)
                s += src1[k]*src2[j + k*w2];
            dst[j] = s;
        }

    icvCheckVector_64f(dst, h1*w2);
}


#define icvTransformVector_32f(matr, src, dst, w, h) \
    icvMulMatrix_32f(matr, w, h, src, 1, w, dst)

#define icvTransformVector_64d(matr, src, dst, w, h) \
    icvMulMatrix_64d(matr, w, h, src, 1, w, dst)


#define icvScaleMatrix_32f(src, dst, w, h, scale) \
    icvScaleVector_32f((src), (dst), (w)*(h), (scale))

#define icvScaleMatrix_64d(src, dst, w, h, scale) \
    icvScaleVector_64d((src), (dst), (w)*(h), (scale))

#define icvDotProduct_64d icvDotProduct_64f


CC_INLINE void icvInvertMatrix_64d(double* A, int n, double* invA)
{
    img_t Am = cvMat(n, n, CC_64F, A);
    img_t invAm = cvMat(n, n, CC_64F, invA);

    cvInvert(&Am, &invAm, CC_SVD);
}

CC_INLINE void icvMulTransMatrixR_64d(double* src, int width, int height, double* dst)
{
    img_t srcMat = cvMat(height, width, CC_64F, src);
    img_t dstMat = cvMat(width, width, CC_64F, dst);

    cvMulTransposed(&srcMat, &dstMat, 1);
}

CC_INLINE void icvMulTransMatrixL_64d(double* src, int width, int height, double* dst)
{
    img_t srcMat = cvMat(height, width, CC_64F, src);
    img_t dstMat = cvMat(height, height, CC_64F, dst);

    cvMulTransposed(&srcMat, &dstMat, 0);
}

CC_INLINE void icvMulTransMatrixR_32f(float* src, int width, int height, float* dst)
{
    img_t srcMat = cvMat(height, width, CC_32F, src);
    img_t dstMat = cvMat(width, width, CC_32F, dst);

    cvMulTransposed(&srcMat, &dstMat, 1);
}

CC_INLINE void icvMulTransMatrixL_32f(float* src, int width, int height, float* dst)
{
    img_t srcMat = cvMat(height, width, CC_32F, src);
    img_t dstMat = cvMat(height, height, CC_32F, dst);

    cvMulTransposed(&srcMat, &dstMat, 0);
}

CC_INLINE void icvCvt_32f_64d(const float* src, double* dst, int len)
{
    int i;
    for(i = 0; i < len; i++)
        dst[i] = src[i];
}

CC_INLINE void icvCvt_64d_32f(const double* src, float* dst, int len)
{
    int i;
    for(i = 0; i < len; i++)
        dst[i] = (float)src[i];
}

#endif/*_CC_MATRIX_H_*/


