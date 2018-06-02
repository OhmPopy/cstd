
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

static int randinit = 0;

#define RAND(a)  rand()%a

// �������ܣ����ھ���m*n�е�ÿ��Ԫ�أ����������ΧΪ[0,a-1]������
int unidrnd( int a, int m, int n ) {
  assert( a > 0 );
  if ( 0 == randinit ) {
#if 0
    srand( time( 0 ) );
#else
    
    srand( 0 );
#endif
    
    randinit = 1;
  }
  return RAND( a );
}

int gaussrnd( int a, int m, int n ) {
  assert( a > 0 );
  if ( 0 == randinit ) {
    srand( time( 0 ) );
    randinit = 1;
  }
  return abs( rand() + rand() + rand() + rand() + rand() + rand() + rand() + rand() + rand() + rand() + rand() + rand() - 6 * RAND_MAX ) * a / RAND_MAX;
}
