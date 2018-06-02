
#include <math.h>
#include <assert.h>
#include "macro.h"
#include "_MILTrack.h"


#ifdef _USE_LBP_FEATURE
#define BLOCKH 3
#define BLOCKW 3
int randomMFeatures( int sample_w, int sample_h, int M, MILHAAR* MFeatures ) {
  int m;
  MILPOINT FeatureStPos;
  
  for ( m = 0; m < M; ++m ) {
    FeatureStPos.y = unidrnd( sample_h - 9, 1, 1 ) + 4; //����1�����������Χ��6:sample_h+5
    FeatureStPos.x = unidrnd( sample_w - 9, 1, 1 ) + 4;
    MFeatures[ m ].StPos = FeatureStPos;
    MFeatures[ m ].Kind = ( FeatureStPos.y * BLOCKH / ( sample_h ) ) * BLOCKW + FeatureStPos.x * BLOCKW / ( sample_w );
    assert( MFeatures[ m ].Kind < ( BLOCKH * BLOCKW ) );
  }
  return M;
}

#else 
//�������ܣ��������������ѡ��M���������(����λ�á��߶Ⱥ�����)
int randomMFeatures( int sample_w, int sample_h, int M, MILHAAR* MFeatures ) {
  int temp, m;
  int min_scale;
  MILPOINT FeatureStPos;
  MILPOINT FeatureScale;
  //Ҫ���ݲ�ͬ��������С������С�������߶ȣ�Ҳ����˵������Խ�С�߶ȵ�����
  temp = MAX( sample_h, sample_w );
  if ( temp >= 100 ) {
    min_scale = 10; //�����߶ȵ���С��λ
  } else if ( temp >= 70 ) {
    min_scale = 7;
  } else if ( temp >= 50 ) {
    min_scale = 5;
  } else {
    min_scale = 2;
  }
  temp = MIN( sample_h, sample_w );
  if ( temp <= 30 ) {
    min_scale = 2;
  } else if ( temp <= 50 ) {
    min_scale = 5;
  }

  for ( m = 0; m < M; ++m ) {
    int temp1 = 0;
    int temp2 = 0;
    int FeatureType = unidrnd( 5, 1, 1 );
    while ( temp1 < 1 ) {
      FeatureStPos.y = unidrnd( sample_h - min_scale - 1, 1, 1 ); //�������������1:sample_h-min_scale-1�е�һ��
      temp1 = sample_h - FeatureStPos.y - min_scale;
    }
    while ( temp2 < 1 ) {
      FeatureStPos.x = unidrnd( sample_w - min_scale - 1, 1, 1 );
      temp2 = sample_w - FeatureStPos.x - min_scale;
    }
    if ( FeatureType == 0 ) {
      FeatureScale.y = unidrnd( sample_h - FeatureStPos.y - min_scale, 1, 1 ) + min_scale; //�������������min_scale+1:sample_h-FeatureStPos.y�е�һ��
      FeatureScale.x = ( int ) floor( ( 1. / 2 ) * ( unidrnd( sample_w - FeatureStPos.x - min_scale, 1, 1 ) + min_scale ) );
    } else if ( FeatureType == 1 ) {
      FeatureScale.y = ( int ) floor( ( 1. / 2 ) * ( unidrnd( sample_h - FeatureStPos.y - min_scale, 1, 1 ) + min_scale ) ); //�������������min_scale+1:sample_h-FeatureStPos.y�е�һ��
      FeatureScale.x = unidrnd( sample_w - FeatureStPos.x - min_scale, 1, 1 ) + min_scale;
    } else if ( FeatureType == 2 ) {
      FeatureScale.y = unidrnd( sample_h - FeatureStPos.y - min_scale, 1, 1 ) + min_scale;
      FeatureScale.x = ( int ) floor( ( 1. / 3 ) * ( unidrnd( sample_w - FeatureStPos.x - min_scale, 1, 1 ) + min_scale ) );
    } else if ( FeatureType == 3 ) {
      FeatureScale.y = ( int ) floor( ( 1. / 2 ) * ( unidrnd( sample_h - FeatureStPos.y - min_scale, 1, 1 ) + min_scale ) );
      FeatureScale.x = ( int ) floor( ( 1. / 2 ) * ( unidrnd( sample_w - FeatureStPos.x - min_scale, 1, 1 ) + min_scale ) );
    } else if ( FeatureType == 4 ) {
      FeatureScale.y = ( int ) floor( ( 1. / 3 ) * ( unidrnd( sample_h - FeatureStPos.y - min_scale, 1, 1 ) + min_scale ) );
      FeatureScale.x = unidrnd( sample_w - FeatureStPos.x - min_scale, 1, 1 ) + min_scale;
    }
    //printf("%3d %3d %3d %3d %1d\n", FeatureStPos.x, FeatureStPos.y, FeatureScale.x, FeatureScale.y, FeatureType);
    MFeatures[ m ].StPos = FeatureStPos;
    MFeatures[ m ].Scale = FeatureScale;
    MFeatures[ m ].Type = FeatureType;
    MFeatures[ m ].Kind = 0;
  }
  return M;
}

#endif
