
#include <math.h>
#include "_MILTrack.h"

// �ھ�������(X,Y)�뾶Ϊ[beita1��beita2]Բ����Χ�����ѡ������(ָ�����������ε����Ͻǵ�)
int randomSelectSample1( int X, int Y, int sample_w, int sample_h,
                         int pic_w, int pic_h, int beita1, int beita2,
                         int tempnum_neg, MILPOINT* YX_neg ) {
                         
  int domainX1 = X - beita2;
  int domainY1 = Y - beita2;
  int domainX3 = X + beita2;
  int domainY3 = Y + beita2;
  int num, times = 10000;
  
  if ( domainX1 < 1 ) {
    domainX1 = 1;
  }
  if ( domainY1 < 1 ) {
    domainY1 = 1;
  }
  if ( domainX3 >= ( pic_w - sample_w - 1 ) ) {
    domainX3 = ( pic_w - sample_w );
  }
  if ( domainY3 >= ( pic_h - sample_h - 1 ) ) {
    domainY3 = ( pic_h - sample_h );
  }
  
  /* �жϻ��ε���Ӿ������ */
  if ( domainX1 >= domainX3 || domainY1 >= domainY3 ) {
    return 0;
  }
  
  for ( num = 0; num < tempnum_neg; --times ) {
    int y = unidrnd( domainY3 - domainY1, 1, 1 ) + domainY1;
    int x = unidrnd( domainX3 - domainX1, 1, 1 ) + domainX1;
    int dis = ( Y - y ) * ( Y - y ) + ( X - x ) * ( X - x );
    int tempY = y + sample_h;
    int tempX = x + sample_w;
    if ( dis < ( beita2 * beita2 ) && dis >= ( beita1 * beita1 ) && tempY < pic_h && tempX < pic_w ) { //Ҫ��ֹ����ͼ��Χ
      YX_neg[ num ].y = y;
      YX_neg[ num ].x = x;
      ++num;
    }
    if ( times < 0 ) {
      return 0;
    }
  }
  return num;
}

//�������ܣ���һ����Χ�����ѡ���������͸��������������ظ������������������Ͻǵ�����
//(X,Y)Ϊ��ǰλ�þ��ο����Ͻǵ�����
//sample_w��sample_hΪ(����)������͸�
//pic_w��pic_hΪ֡�Ŀ�͸�
//[beita1��beita2]��gamaΪ���������������ķ�Χ
//tempnum_neg��tempnum_posΪ����������������
int randomSelectSample( int X, int Y, int sample_w, int sample_h, int pic_w, int pic_h,
                        int beita1, int beita2, int gama, int tempnum_neg, int tempnum_pos,
                        int frame, MILPOINT* YX_all ) {
  MILPOINT * YX_neg = YX_all;
  MILPOINT*YX_pos = YX_all + tempnum_neg;
  //�ھ�������(X,Y)�뾶Ϊ[beita1��beita2]Բ����Χ�����ѡ������(ָ�����������ε����Ͻǵ�)
  int ret;
  ret = randomSelectSample1( X, Y, sample_w, sample_h, pic_w, pic_h, beita1, beita2, tempnum_neg, YX_neg );
  if ( 0 == ret )
    return 0;
  //�ھ�������(X,Y)�뾶Ϊ[0��gama]Բ����Χ�����ѡ��ѡ��������
  ret = randomSelectSample1( X, Y, sample_w, sample_h, pic_w, pic_h, 0, gama, tempnum_pos, YX_pos );
  return ret;
}

//�������ܣ�������һ����Χ�����ѡ��ĺ�ѡλ��(�������Ͻǵ�����)
//(X,Y)Ϊ��ǰλ�þ��ο����Ͻǵ�����
//sample_w��sample_hΪ(����)������͸�
//pic_w��pic_hΪ֡�Ŀ�͸�
//sΪ��ѡ�����ķ�Χ
//num_candidate_result Ϊ��ѡ������
//num_fix_cr Ҫ��ԭλ�ø����̶���������
int randomSelectSample_for_result( int X, int Y, int sample_w, int sample_h, int pic_w, int pic_h,
                                   int s, int num_candidate_result, int num_fix_cr, MILPOINT* YX_candidate ) {
  int y, x;
  int num = 0;
  int r = ( int ) ( sqrt( num_fix_cr ) * 1. / 2 );
  if ( r >= s ) {
    return 0;
  }
  for ( y = ( Y - r ); y < ( Y + r ) && num < num_fix_cr; ++y ) {
    for ( x = ( X - r ); x < ( X + r ) && num < num_fix_cr; ++x ) {
      int tempY = ( y + sample_h );
      int tempX = ( x + sample_w );
      int dis = ( ( Y - y ) * ( Y - y ) + ( X - x ) * ( X - x ) );
      if ( y > 0 && x > 0 && tempY < pic_h && tempX < pic_w // && dis<(r*r)
         ) {
        YX_candidate[ num ].y = y;
        YX_candidate[ num ].x = x;
        ++num;
      }
    }
  }
  if ( num < num_candidate_result ) {
    return randomSelectSample1( X, Y, sample_w, sample_h, pic_w, pic_h, r, s,
                                num_candidate_result - num, YX_candidate + num );
  }
  return 0;
}
