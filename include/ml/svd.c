
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include "matmul.h"
#include "macro.h"
#include "matrix.h"



/////////////////////////////////////////////////////////////////////////////////////////

#define icvGivens_64f( n, x, y, c, s )                                        \
{                                                                             \
  int _i;                                                                     \
  real* _x = (x);                                                             \
  real* _y = (y);                                                             \
                                                                              \
  for( _i = 0; _i < n; _i++ ) {                                               \
    real t0 = _x[_i];                                                         \
    real t1 = _y[_i];                                                         \
    _x[_i] = t0*c + t1*s;                                                     \
    _y[_i] = -t0*s + t1*c;                                                    \
  }                                                                           \
}


/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
static void
icvMatrAXPY_64f( int m, int n, const real* x, int dx,
                 const real* a, real* y, int dy ) {
  int i, j;
  
  for ( i = 0; i < m; i++, x += dx, y += dy ) {
    real s = a[ i ];
    
    for ( j = 0; j <= n - 4; j += 4 ) {
      real t0 = y[ j ] + s * x[ j ];
      real t1 = y[ j + 1 ] + s * x[ j + 1 ];
      y[ j ] = t0;
      y[ j + 1 ] = t1;
      t0 = y[ j + 2 ] + s * x[ j + 2 ];
      t1 = y[ j + 3 ] + s * x[ j + 3 ];
      y[ j + 2 ] = t0;
      y[ j + 3 ] = t1;
    }
    
    for ( ; j < n; j++ )
      y[ j ] += s * x[ j ];
  }
}


/* y[1:m,-1] = h*y[1:m,0:n]*x[0:1,0:n]'*x[-1]  (this is used for U&V reconstruction)
   y[1:m,0:n] += h*y[1:m,0:n]*x[0:1,0:n]'*x[0:1,0:n] */
static void
icvMatrAXPY3_64f( int m, int n, const real* x, int l, real* y, real h ) {
  int i, j;
  
  for ( i = 1; i < m; i++ ) {
    real s = 0;
    
    y += l;
    
    for ( j = 0; j <= n - 4; j += 4 )
      s += x[ j ] * y[ j ] + x[ j + 1 ] * y[ j + 1 ] + x[ j + 2 ] * y[ j + 2 ] + x[ j + 3 ] * y[ j + 3 ];
      
    for ( ; j < n; j++ )
      s += x[ j ] * y[ j ];
      
    s *= h;
    y[ -1 ] = s * x[ -1 ];
    
    for ( j = 0; j <= n - 4; j += 4 ) {
      real t0 = y[ j ] + s * x[ j ];
      real t1 = y[ j + 1 ] + s * x[ j + 1 ];
      y[ j ] = t0;
      y[ j + 1 ] = t1;
      t0 = y[ j + 2 ] + s * x[ j + 2 ];
      t1 = y[ j + 3 ] + s * x[ j + 3 ];
      y[ j + 2 ] = t0;
      y[ j + 3 ] = t1;
    }
    
    for ( ; j < n; j++ )
      y[ j ] += s * x[ j ];
  }
}


#define icvGivens_32f( n, x, y, c, s )                                        \
{                                                                             \
  int _i;                                                                     \
  real* _x = (x);                                                             \
  real* _y = (y);                                                             \
                                                                              \
  for( _i = 0; _i < n; _i++ ) {                                               \
    real t0 = _x[_i];                                                         \
    real t1 = _y[_i];                                                         \
    _x[_i] = (real)(t0*c + t1*s);                                             \
    _y[_i] = (real)(-t0*s + t1*c);                                            \
  }                                                                           \
}

static void
icvMatrAXPY_32f( int m, int n, const real* x, int dx,
                 const real* a, real* y, int dy ) {
  int i, j;
  
  for ( i = 0; i < m; i++, x += dx, y += dy ) {
    real s = a[ i ];
    
    for ( j = 0; j <= n - 4; j += 4 ) {
      real t0 = y[ j ] + s * x[ j ];
      real t1 = y[ j + 1 ] + s * x[ j + 1 ];
      y[ j ] = ( real ) t0;
      y[ j + 1 ] = ( real ) t1;
      t0 = y[ j + 2 ] + s * x[ j + 2 ];
      t1 = y[ j + 3 ] + s * x[ j + 3 ];
      y[ j + 2 ] = ( real ) t0;
      y[ j + 3 ] = ( real ) t1;
    }
    
    for ( ; j < n; j++ )
      y[ j ] = ( real ) ( y[ j ] + s * x[ j ] );
  }
}


static void
icvMatrAXPY3_32f( int m, int n, const real* x, int l, real* y, real h ) {
  int i, j;
  
  for ( i = 1; i < m; i++ ) {
    real s = 0;
    y += l;
    
    for ( j = 0; j <= n - 4; j += 4 )
      s += x[ j ] * y[ j ] + x[ j + 1 ] * y[ j + 1 ] + x[ j + 2 ] * y[ j + 2 ] + x[ j + 3 ] * y[ j + 3 ];
      
    for ( ; j < n; j++ )
      s += x[ j ] * y[ j ];
      
    s *= h;
    y[ -1 ] = ( real ) ( s * x[ -1 ] );
    
    for ( j = 0; j <= n - 4; j += 4 ) {
      real t0 = y[ j ] + s * x[ j ];
      real t1 = y[ j + 1 ] + s * x[ j + 1 ];
      y[ j ] = ( real ) t0;
      y[ j + 1 ] = ( real ) t1;
      t0 = y[ j + 2 ] + s * x[ j + 2 ];
      t1 = y[ j + 3 ] + s * x[ j + 3 ];
      y[ j + 2 ] = ( real ) t0;
      y[ j + 3 ] = ( real ) t1;
    }
    
    for ( ; j < n; j++ )
      y[ j ] = ( real ) ( y[ j ] + s * x[ j ] );
  }
}

/* accurate hypotenuse calculation */
real
pythag( real a, real b ) {
  a = FABS( a );
  b = FABS( b );
  if ( a > b ) {
    b /= a;
    a *= SQRT( 1. + b * b );
  } else if ( b != 0 ) {
    a /= b;
    a = b * SQRT( 1. + a * a );
  }
  
  return a;
}

/****************************************************************************************/
/****************************************************************************************/

#define MAX_ITERS  30

void
icvSVD( real* a, int lda, int m, int n,
        real* w, real* uT, int lduT, int nu,
        real* vT, int ldvT, real* buffer ) {
  real * e;
  real* temp;
  real *w1, *e1;
  real *hv;
  real ku0 = 0, kv0 = 0;
  real anorm = 0;
  const real *a1;
  real *u0 = uT, *v0 = vT;
  real scale, h;
  int i, j, k, l;
  int nm, m1, n1;
  int nv = n;
  int iters = 0;
  real* hv0 = ( real* ) cvStackAlloc( ( m + 2 ) * sizeof( hv0[ 0 ] ) ) + 1;
  
  e = buffer;
  
  w1 = w;
  e1 = e + 1;
  nm = n;
  
  temp = buffer + nm;
  
  memset( w, 0, nm * sizeof( w[ 0 ] ) );
  memset( e, 0, nm * sizeof( e[ 0 ] ) );
  
  m1 = m;
  n1 = n;
  
  /* transform a to bi-diagonal form */
  for ( ;; ) {
    int update_u;
    int update_v;
    
    if ( m1 == 0 )
      break;
      
    scale = h = 0;
    
    update_u = uT && m1 > m - nu;
    hv = update_u ? uT : hv0;
    
    for ( j = 0, a1 = a; j < m1; j++, a1 += lda ) {
      real t = a1[ 0 ];
      scale += FABS( hv[ j ] = ( real ) t );
    }
    
    if ( scale != 0 ) {
      real f = 1.f / scale, g, s = 0;
      
      for ( j = 0; j < m1; j++ ) {
        real t = ( hv[ j ] = ( real ) ( hv[ j ] * f ) );
        s += t * t;
      }
      
      g = SQRT( s );
      f = hv[ 0 ];
      if ( f >= 0 )
        g = -g;
      hv[ 0 ] = ( real ) ( f - g );
      h = 1.f / ( f * g - s );
      
      memset( temp, 0, n1 * sizeof( temp[ 0 ] ) );
      
      /* calc temp[0:n-i] = a[i:m,i:n]'*hv[0:m-i] */
      icvMatrAXPY_32f( m1, n1 - 1, a + 1, lda, hv, temp + 1, 0 );
      
      for ( k = 1; k < n1; k++ )
        temp[ k ] = ( real ) ( temp[ k ] * h );
        
      /* modify a: a[i:m,i:n] = a[i:m,i:n] + hv[0:m-i]*temp[0:n-i]' */
      icvMatrAXPY_32f( m1, n1 - 1, temp + 1, 0, hv, a + 1, lda );
      *w1 = ( real ) ( g * scale );
    }
    w1++;
    
    /* store -2/(hv'*hv) */
    if ( update_u ) {
      if ( m1 == m )
        ku0 = h;
      else
        hv[ -1 ] = ( real ) h;
    }
    
    a++;
    n1--;
    if ( vT )
      vT += ldvT + 1;
      
    if ( n1 == 0 )
      break;
      
    scale = h = 0;
    update_v = vT && n1 > n - nv;
    hv = update_v ? vT : hv0;
    
    for ( j = 0; j < n1; j++ ) {
      real t = a[ j ];
      scale += FABS( hv[ j ] = ( real ) t );
    }
    
    if ( scale != 0 ) {
      real f = 1.f / scale, g, s = 0;
      
      for ( j = 0; j < n1; j++ ) {
        real t = ( hv[ j ] = ( real ) ( hv[ j ] * f ) );
        s += t * t;
      }
      
      g = SQRT( s );
      f = hv[ 0 ];
      if ( f >= 0 )
        g = -g;
      hv[ 0 ] = ( real ) ( f - g );
      h = 1.f / ( f * g - s );
      hv[ -1 ] = 0.f;
      
      /* update a[i:m:i+1:n] = a[i:m,i+1:n] + (a[i:m,i+1:n]*hv[0:m-i])*... */
      icvMatrAXPY3_32f( m1, n1, hv, lda, a, h );
      
      *e1 = ( real ) ( g * scale );
    }
    e1++;
    
    /* store -2/(hv'*hv) */
    if ( update_v ) {
      if ( n1 == n )
        kv0 = h;
      else
        hv[ -1 ] = ( real ) h;
    }
    
    a += lda;
    m1--;
    if ( uT )
      uT += lduT + 1;
  }
  
  m1 -= m1 != 0;
  n1 -= n1 != 0;
  
  /* accumulate left transformations */
  if ( uT ) {
    m1 = m - m1;
    uT = u0 + m1 * lduT;
    for ( i = m1; i < nu; i++, uT += lduT ) {
      memset( uT + m1, 0, ( m - m1 ) * sizeof( uT[ 0 ] ) );
      uT[ i ] = 1.;
    }
    
    for ( i = m1 - 1; i >= 0; i-- ) {
      real s;
      int lh = nu - i;
      
      l = m - i;
      
      hv = u0 + ( lduT + 1 ) * i;
      h = i == 0 ? ku0 : hv[ -1 ];
      
      assert( h <= 0 );
      
      if ( h != 0 ) {
        uT = hv;
        icvMatrAXPY3_32f( lh, l - 1, hv + 1, lduT, uT + 1, h );
        
        s = hv[ 0 ] * h;
        for ( k = 0; k < l; k++ )
          hv[ k ] = ( real ) ( hv[ k ] * s );
        hv[ 0 ] += 1;
      } else {
        for ( j = 1; j < l; j++ )
          hv[ j ] = 0;
        for ( j = 1; j < lh; j++ )
          hv[ j * lduT ] = 0;
        hv[ 0 ] = 1;
      }
    }
    uT = u0;
  }
  
  /* accumulate right transformations */
  if ( vT ) {
    n1 = n - n1;
    vT = v0 + n1 * ldvT;
    for ( i = n1; i < nv; i++, vT += ldvT ) {
      memset( vT + n1, 0, ( n - n1 ) * sizeof( vT[ 0 ] ) );
      vT[ i ] = 1.;
    }
    
    for ( i = n1 - 1; i >= 0; i-- ) {
      real s;
      int lh = nv - i;
      
      l = n - i;
      hv = v0 + ( ldvT + 1 ) * i;
      h = i == 0 ? kv0 : hv[ -1 ];
      
      assert( h <= 0 );
      
      if ( h != 0 ) {
        vT = hv;
        icvMatrAXPY3_32f( lh, l - 1, hv + 1, ldvT, vT + 1, h );
        
        s = hv[ 0 ] * h;
        for ( k = 0; k < l; k++ )
          hv[ k ] = ( real ) ( hv[ k ] * s );
        hv[ 0 ] += 1;
      } else {
        for ( j = 1; j < l; j++ )
          hv[ j ] = 0;
        for ( j = 1; j < lh; j++ )
          hv[ j * ldvT ] = 0;
        hv[ 0 ] = 1;
      }
    }
    vT = v0;
  }
  
  for ( i = 0; i < nm; i++ ) {
    real tnorm = FABS( w[ i ] );
    tnorm += FABS( e[ i ] );
    
    if ( anorm < tnorm )
      anorm = tnorm;
  }
  
  anorm *= FLT_EPSILON;
  
  /* diagonalization of the bidiagonal form */
  for ( k = nm - 1; k >= 0; k-- ) {
    real z = 0;
    iters = 0;
    
    for ( ;; )                /* do iterations */
    {
      real c, s, f, g, x, y;
      int flag = 0;
      
      /* test for splitting */
      for ( l = k; l >= 0; l-- ) {
        if ( FABS( e[ l ] ) <= anorm ) {
          flag = 1;
          break;
        }
        assert( l > 0 );
        if ( FABS( w[ l - 1 ] ) <= anorm )
          break;
      }
      
      if ( !flag ) {
        c = 0;
        s = 1;
        
        for ( i = l; i <= k; i++ ) {
          f = s * e[ i ];
          e[ i ] = ( real ) ( e[ i ] * c );
          
          if ( anorm + FABS( f ) == anorm )
            break;
            
          g = w[ i ];
          h = pythag( f, g );
          w[ i ] = ( real ) h;
          c = g / h;
          s = -f / h;
          
          if ( uT )
            icvGivens_32f( m, uT + lduT * ( l - 1 ), uT + lduT * i, c, s );
        }
      }
      
      z = w[ k ];
      if ( l == k || iters++ == MAX_ITERS )
        break;
        
      /* shift from bottom 2x2 minor */
      x = w[ l ];
      y = w[ k - 1 ];
      g = e[ k - 1 ];
      h = e[ k ];
      f = 0.5f * ( ( ( g + z ) / h ) * ( ( g - z ) / y ) + y / h - h / y );
      g = pythag( f, 1 );
      if ( f < 0 )
        g = -g;
      f = x - ( z / x ) * z + ( h / x ) * ( y / ( f + g ) - h );
      /* next QR transformation */
      c = s = 1;
      
      for ( i = l + 1; i <= k; i++ ) {
        g = e[ i ];
        y = w[ i ];
        h = s * g;
        g *= c;
        z = pythag( f, h );
        e[ i - 1 ] = ( real ) z;
        c = f / z;
        s = h / z;
        f = x * c + g * s;
        g = -x * s + g * c;
        h = y * s;
        y *= c;
        
        if ( vT )
          icvGivens_32f( n, vT + ldvT * ( i - 1 ), vT + ldvT * i, c, s );
          
        z = pythag( f, h );
        w[ i - 1 ] = ( real ) z;
        
        /* rotation can be arbitrary if z == 0 */
        if ( z != 0 ) {
          c = f / z;
          s = h / z;
        }
        f = c * g + s * y;
        x = -s * g + c * y;
        
        if ( uT )
          icvGivens_32f( m, uT + lduT * ( i - 1 ), uT + lduT * i, c, s );
      }
      
      e[ l ] = 0;
      e[ k ] = ( real ) f;
      w[ k ] = ( real ) x;
    }                       /* end of iteration loop */
    
    if ( iters > MAX_ITERS )
      break;
      
    if ( z < 0 ) {
      w[ k ] = ( real ) ( -z );
      if ( vT ) {
        for ( j = 0; j < n; j++ )
          vT[ j + k * ldvT ] = -vT[ j + k * ldvT ];
      }
    }
  }                           /* end of diagonalization loop */
  
  /* sort singular values and corresponding vectors */
  for ( i = 0; i < nm; i++ ) {
    k = i;
    for ( j = i + 1; j < nm; j++ )
      if ( w[ k ] < w[ j ] )
        k = j;
        
    if ( k != i ) {
      real t;
      CV_SWAP( w[ i ], w[ k ], t );
      
      if ( vT )
        for ( j = 0; j < n; j++ )
          CV_SWAP( vT[ j + ldvT * k ], vT[ j + ldvT * i ], t );
          
      if ( uT )
        for ( j = 0; j < m; j++ )
          CV_SWAP( uT[ j + lduT * k ], uT[ j + lduT * i ], t );
    }
  }
}

static void
icvSVBkSb( int m, int n, const real* w,
           const real* uT, int lduT,
           const real* vT, int ldvT,
           const real* b, int ldb, int nb,
           real* x, int ldx, real* buffer ) {
  real threshold = 0.f;
  int i, j, nm = MIN( m, n );
  
  if ( !b )
    nb = m;
    
  for ( i = 0; i < n; i++ )
    memset( x + i * ldx, 0, nb * sizeof( x[ 0 ] ) );
    
  for ( i = 0; i < nm; i++ )
    threshold += w[ i ];
  threshold *= 2 * FLT_EPSILON;
  
  /* vT * inv(w) * uT * b */
  for ( i = 0; i < nm; i++, uT += lduT, vT += ldvT ) {
    real wi = w[ i ];
    
    if ( wi > threshold ) {
      wi = 1.f / wi;
      
      if ( nb == 1 ) {
        real s = 0;
        if ( b ) {
          if ( ldb == 1 ) {
            for ( j = 0; j <= m - 4; j += 4 )
              s += uT[ j ] * b[ j ] + uT[ j + 1 ] * b[ j + 1 ] + uT[ j + 2 ] * b[ j + 2 ] + uT[ j + 3 ] * b[ j + 3 ];
            for ( ; j < m; j++ )
              s += uT[ j ] * b[ j ];
          } else {
            for ( j = 0; j < m; j++ )
              s += uT[ j ] * b[ j * ldb ];
          }
        } else
          s = uT[ 0 ];
        s *= wi;
        
        if ( ldx == 1 ) {
          for ( j = 0; j <= n - 4; j += 4 ) {
            real t0 = x[ j ] + s * vT[ j ];
            real t1 = x[ j + 1 ] + s * vT[ j + 1 ];
            x[ j ] = ( real ) t0;
            x[ j + 1 ] = ( real ) t1;
            t0 = x[ j + 2 ] + s * vT[ j + 2 ];
            t1 = x[ j + 3 ] + s * vT[ j + 3 ];
            x[ j + 2 ] = ( real ) t0;
            x[ j + 3 ] = ( real ) t1;
          }
          
          for ( ; j < n; j++ )
            x[ j ] = ( real ) ( x[ j ] + s * vT[ j ] );
        } else {
          for ( j = 0; j < n; j++ )
            x[ j * ldx ] = ( real ) ( x[ j * ldx ] + s * vT[ j ] );
        }
      } else {
        if ( b ) {
          memset( buffer, 0, nb * sizeof( buffer[ 0 ] ) );
          icvMatrAXPY_32f( m, nb, b, ldb, uT, buffer, 0 );
          for ( j = 0; j < nb; j++ )
            buffer[ j ] = ( real ) ( buffer[ j ] * wi );
        } else {
          for ( j = 0; j < nb; j++ )
            buffer[ j ] = ( real ) ( uT[ j ] * wi );
        }
        icvMatrAXPY_32f( n, nb, buffer, 0, vT, x, ldx );
      }
    }
  }
}

/*
��ʵ����������������ֵ�ֽ�
 
A M��N ���������
W �������ֵ���� (M��N ���� N��N) ���� ���� (N��1).
U ��ѡ������������ (M��M or M��N). ��� CV_SVD_U_T ��ָ���� Ӧ�ý���������˵�������е���Ŀ��
V ��ѡ�Ҳ���������(N��N)
flags ������־�������� 0 ���������ֵ�����:
CV_SVD_MODIFY_A ͨ�����������޸ľ��� src1 �����������ٶȻ�ȽϿ졣
CV_SVD_U_T ��ζ�Ż᷵��ת�þ��� U ��ָ�������־���ӿ촦���ٶȡ�
CV_SVD_V_T ��ζ�Ż᷵��ת�þ��� V ��ָ�������־���ӿ촦���ٶȡ�
���� cvSVD ������ A �ֽ��һ���Խ��߾����������������ĳ˻���
 
���� W ��һ������ֵ�ĶԽ��߾��������Ա����������ֵ��һά������U �� V Ҳ��һ����
���е�����ֵ���ǷǸ��Ĳ�������洢����U �� V Ҳ��Ӧ�Ĵ洢����
 
SVD �㷨����ֵ�������Ѿ����ȶ������ĵ���Ӧ�ð�����
 
�� A ��һ�����󡢶Գ����������ʱ��ȷ���������ֵ���⣬����, �� A ʱһ��Э�������ʱ��
����������� W ����һ������ֵ�ĵ����������� U=V�Ǿ������������
(��ˣ�����Ҫ������������ʱ U �� V ֻ��Ҫ��������һ���Ϳ�����) ��
��ȷ����ⲡ̬����ϵͳ��
��������ϵͳ����С������⡣��һ�������������ⶼ������ָ�� CV_SVD �� cvSolve ������
��ȷ�������Ĳ�ͬ����������(��������ֵ����Ŀ), ������(�������ֵ����С����ֵ�ı���),
����ʽֵ(����ʽ�ľ���ֵ��������ֵ�ĳ˻�).������������Щֵ����Ҫ�������� U �� V ��
��ȷ�������Ĳ�ͬ����������(��������ֵ����Ŀ), ������(�������ֵ����С����ֵ�ı���),
����ʽֵ(����ʽ�ľ���ֵ��������ֵ�ĳ˻�).������������Щֵ����Ҫ�������� U �� V ��
*/
void
cvSVD( int arows, int acols, real* aarr, int astep,
       real* warr, int wstep, int ucols, real* uarr, int ustep,
       real* varr, int vstep, int flags ) {
  real * buffer = 0;
  int local_alloc = 0;
  real* a = aarr;
  real* w = warr;
  real* u = uarr;
  real* v = varr;
  
  real* tw = 0;
  int a_buf_offset = 0, u_buf_offset = 0, buf_size;
  int temp_u = 0,  /* temporary storage for U is needed */
               t_svd; /* special case: arows < acols */
  int m, n;
  
  if ( arows >= acols ) {
    m = arows;
    n = acols;
    t_svd = 0;
  } else {
    real* t;
    CV_SWAP( u, v, t );
    
    flags = ( flags & CV_SVD_U_T ? CV_SVD_V_T : 0 ) |
            ( flags & CV_SVD_V_T ? CV_SVD_U_T : 0 );
    m = acols;
    n = arows;
    t_svd = 1;
  }
  
  if ( wstep == 1 )
    tw = w;
    
  buf_size = n * 2 + m;
  
  if ( !( flags & CV_SVD_MODIFY_A ) ) {
    a_buf_offset = buf_size;
    buf_size += arows * acols;
  }
  
  if ( temp_u ) {
    u_buf_offset = buf_size;
    buf_size += m * m;
  }
  
  if ( buf_size <= CV_MAX_LOCAL_SIZE ) {
    buffer = ( real* ) cvStackAlloc( buf_size * sizeof( real ) );
    local_alloc = 1;
  } else {
    buffer = ( real* ) cvAlloc( buf_size * sizeof( real ) );
  }
  
  if ( !( flags & CV_SVD_MODIFY_A ) ) {
    real * t = ( real* ) ( buffer + a_buf_offset );
    if ( !t_svd ) {
      SKIP_COPY( arows, acols, a, astep, 1, t, acols, 1 );
      astep = acols;
    } else {
      TRANSPOSE( arows, acols, a, astep, t, arows );
      astep = arows;
    }
    a = t;
  }
  
  if ( !tw )
    tw = buffer + ( n + m );
    
  icvSVD( a, astep, m, n, tw, u, ustep, ucols, v, vstep, buffer );
  
  if ( tw != w ) {
    int i;
    //cvSetZero( w );
    for ( i = 0; i < n; i++ )
      ( ( w + i * wstep ) ) [ i ] = ( tw ) [ i ];
  }
  
  if ( u && uarr ) {
    if ( !( flags & CV_SVD_U_T ) ) {
      SAFETRANSPOSE( m, m, u, ustep, uarr, ustep, real );
    } else if ( temp_u )
      SKIP_COPY( m, m, u, ustep, 1, uarr, ustep, 1 );
  }
  
  if ( v && varr ) {
    if ( !( flags & CV_SVD_V_T ) )
      SAFETRANSPOSE( n, n, v, vstep, varr, vstep, real );
  }
  
  if ( buffer && !local_alloc )
    cvFree( &buffer );
}

/*
����ֵ�ش��㷨��back substitution��
W ����ֵ�����������
U ���������� (������ת�õ�)
V ���������� (������ת�õ�)
B ԭʼ���� A ��α��ĳ˷���������ǿ�ѡ������
�������ʡ����ٶ�����һ���ʵ���С�ĵ�λ����
(��� x ���� A ��α����ؽ�).��
X Ŀ�����: ����ֵ�ش��㷨�Ľ��
flags ������־, �͸ո����۵� cvSVD �ı�־һ����
���� cvSVBkSb Ϊ���ֽ�ľ��� A �;��� B ����ش���
��back substitution�� (�μ� cvSVD ˵��) :
X=V*W-1*UT*B
����
W-1(i,i)=1/W(i,i) ��� W(i,i) > epsilon?sumiW(i,i),
����:0.
epsilon ��һ�������ھ����������͵ĵĺ�С������
�ú����� cvSVD ����������ִ�� cvInvert �� cvSolve,
����Щ���� (svd & bksb)��ԭ���ǳ���������low-level��
�������Ա���߼����� (inv & solve) �������ڲ��������ʱ����
*/
void
cvSVBkSb( const real* warr, int wstep, const real* uarr, int ustep,
          const real* varr, int vstep, int bcols, const real* barr, int bstep,
          int xrows, int xcols, real* xarr, int xstep, int flags ) {
  real * w = ( real* ) warr;
  const real *u = uarr;
  const real *v = varr;
  const real *b = barr;
  real *x = xarr;
  real* buffer = 0;
  int local_alloc = 0;
  real* tw = 0;
  int temp_u = 0, temp_v = 0;
  int u_buf_offset = 0, v_buf_offset = 0, w_buf_offset = 0, t_buf_offset = 0;
  int buf_size = 0;
  int m, n, nm;
  
  if ( !( flags & CV_SVD_U_T ) ) {
    temp_u = 1;
    u_buf_offset = buf_size;
    buf_size += xcols * xcols;
  }
  
  if ( !( flags & CV_SVD_V_T ) ) {
    temp_v = 1;
    v_buf_offset = buf_size;
    buf_size += xrows * xrows;
  }
  
  m = xrows;
  n = xcols;
  nm = MIN( n, m );
  
  if ( wstep == 1 ) {
    tw = w;
  } else {
    w_buf_offset = buf_size;
    buf_size += nm;
  }
  
  t_buf_offset = buf_size;
  buf_size += ( MAX( m, n ) + bcols );
  
  if ( buf_size <= CV_MAX_LOCAL_SIZE ) {
    buffer = ( real* ) cvStackAlloc( buf_size * sizeof( real ) );
    local_alloc = 1;
  } else
    buffer = ( real* ) cvAlloc( buf_size * sizeof( real ) );
    
  if ( temp_u ) {
    real * t = buffer + u_buf_offset;
    TRANSPOSE( xcols, xcols, u, ustep, t, xcols );
    u = t;
  }
  
  if ( temp_v ) {
    real * t = buffer + v_buf_offset;
    TRANSPOSE( xrows, xrows, u, ustep, t, xrows );
    u = t;
  }
  
  if ( !tw ) {
    int i;
    tw = buffer + w_buf_offset;
    for ( i = 0; i < nm; i++ )
      tw[ i ] = w[ i * ( wstep ) ];
  }
  
  icvSVBkSb( m, n, tw, u, ustep, v, vstep, b, bstep, bcols, x, xstep, ( buffer + t_buf_offset ) );
  
  if ( buffer && !local_alloc )
    cvFree( &buffer );
}

/* End of file. */
