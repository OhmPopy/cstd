
#include <string.h>
#include <stdlib.h>
#include <math.h> // for sqrt
#include "_MILTrack.h"
#include "macro.h"

//�������ܣ�����M����ѡ�����������Լ���������ֵ�ľ�ֵ�ͱ�׼��
//YX_neg��YX_pos�洢������������������(�������Ͻǵ�)
//pic_w��pic_hΪ֡�Ŀ�͸�
//MFeaturesΪ���ѡ���M������������λ�á����͡��߶�
//IΪ��ǰ����֡
//MIU1_last_f,DELTA1_last_f,MIU2_last_f,DELTA2_last_f�洢���Ӹ�˹�ֲ�������ֵ�ľ�ֵ�ͱ�׼��
//vΪѧϰ��(��ֵ�ͱ�׼��ĸ�����)

int icvIntegralImage();

void candidateWeakClf( int tempnum_neg, int tempnum_pos, const MILPOINT* YX_all, int sample_w, int sample_h,
                       MILHAAR* MFeatures, int M, unsigned char* I, int pic_h, int pic_w, int step,
                       real v, int frame, real* _h_candidateWeakClf ) {
  int i, j;
  int totalnum = tempnum_neg + tempnum_pos; //��������
  real* _AllFeatures = _h_candidateWeakClf; //�洢������������������ֵ
  int* intg_pic = MALLOCSET( int, ( pic_h + 1 ) * ( pic_w + 1 ), 0 );
  int intg_step = pic_w + 1;
  int* intg_im;
  if ( frame == 0 ) {
    v = 0; //��һ֡ʱ�ĸ�����Ϊ0
  }
#define AllFeatures(y, x)   _AllFeatures[y*M+x]
#define h_candidateWeakClf(_y, _x)   _h_candidateWeakClf[_y*M+_x]
  // YX_all = [YX_neg;YX_pos];
  icvIntegralImage( I, step, intg_pic, intg_step, pic_h, pic_w ); //�����ͼ
  //FPRINT2D("intg_pic.txt", "%10d, ", pic_h, pic_w, intg_pic, intg_step, 1);
  //I = double(imadjust(I));//���ӶԱȶȣ�
  imadjust( pic_w * pic_h, I, I );
  for ( i = 0; i < totalnum; ++i ) { //����ÿһ������
    int X1 = YX_all[ i ].x;
    int Y1 = YX_all[ i ].y;
    //int X3 = X1 + sample_w - 1;
    //int Y3 = Y1 + sample_h - 1;
    // patch_I = I(Y1:Y3,X1:X3);
#if 1
    
    intg_im = intg_pic + Y1 * intg_step + X1;
    for ( j = 0; j < M; ++j ) { //����ÿһ������
      MILPOINT FeatureStPos = MFeatures[ j ].StPos;
      MILPOINT FeatureScale = MFeatures[ j ].Scale;
      int FeatureType = MFeatures[ j ].Type;
      AllFeatures( i, j ) = GetOneFeatureValue_2( FeatureType, FeatureStPos, FeatureScale, intg_im, intg_step ); //��������ֵ
    }
#else
    intg_step = ( sample_w + 1 );
    intg_im = MALLOC( int, intg_step * ( sample_h + 1 ) );
    icvIntegralImage( I + Y1 * pic_w + X1, pic_w, intg_im, intg_step, sample_h, sample_w ); //�����ͼ
    for ( j = 0; j < M; ++j ) { //����ÿһ������
      MILPOINT FeatureStPos = MFeatures[ j ].StPos;
      MILPOINT FeatureScale = MFeatures[ j ].Scale;
      int FeatureType = MFeatures[ j ].Type;
      //PRINT2D(10, 20, intg_im, intg_step, 1, "%5d, ");
      AllFeatures( i, j ) = GetOneFeatureValue_2( FeatureType, FeatureStPos, FeatureScale, intg_im, intg_step ); //��������ֵ
    }
    SAFEFREE( intg_im );
#endif
    
  }
  DO_FPRINT2D( UNEIXST_FILE( ".\\out\\AllFeatures_%d.txt" ), "%g, ", ( totalnum * M ), 1, _AllFeatures, 1, 1 );
  
  //���¼��м���������ֵ�ͱ�׼��(ÿ�������������������������и������ľ�ֵ�ͱ�׼,Ҫ������������)
  //MIU2[i]   = v*MIU2_last_f   + (1-v)*sum(AllFeatures(1:tempnum_neg,:),1)/tempnum_neg;//���ڸ�����
  //temp = repmat(MIU2,tempnum_neg,1);
  //DELTA2 = v*DELTA2_last_f + (1-v)*sqrt(sum((AllFeatures(1:tempnum_neg,:)-temp).^2,1)/tempnum_neg);
  //MIU1   = v*MIU1_last_f   + (1-v)*sum(AllFeatures(1+tempnum_neg:end,:),1)/tempnum_pos;//����������
  //temp = repmat(MIU1,tempnum_pos,1);
  //DELTA1 = v*DELTA1_last_f + (1-v)*sqrt(sum((AllFeatures(1+tempnum_neg:end,:)-temp).^2,1)/tempnum_pos);
  //h_candidateWeakClf = zeros(totalnum,M);//�洢���������ĺ�ѡ��������
  
  for ( j = 0; j < M; ++j ) {
    real miu1 = 0., miu2 = 0., delta1 = 0., delta2 = 0.;
    for ( i = 0; i < tempnum_neg; ++i ) {
      miu2 += AllFeatures( i, j );
    }
    for ( i = tempnum_neg; i < tempnum_neg + tempnum_pos; ++i ) {
      miu1 += AllFeatures( i, j );
    }
    miu2 /= tempnum_neg;
    miu1 /= tempnum_pos;
    miu2 = MFeatures[ j ].MIU2 = v * MFeatures[ j ].MIU2 + ( 1. - v ) * miu2;
    miu1 = MFeatures[ j ].MIU1 = v * MFeatures[ j ].MIU1 + ( 1. - v ) * miu1;
    for ( i = 0; i < tempnum_neg; ++i ) {
      real t = AllFeatures( i, j ) - miu2;
      delta2 += t * t;
    }
    for ( i = tempnum_neg; i < tempnum_neg + tempnum_pos; ++i ) {
      real t = AllFeatures( i, j ) - miu1;
      delta1 += t * t;
    }
    delta2 /= tempnum_neg;
    delta1 /= tempnum_pos;
    delta2 = sqrt( delta2 );
    delta1 = sqrt( delta1 );
    delta2 = MFeatures[ j ].DELTA2 = v * MFeatures[ j ].DELTA2 + ( 1. - v ) * ( delta2 );
    delta1 = MFeatures[ j ].DELTA1 = v * MFeatures[ j ].DELTA1 + ( 1. - v ) * ( delta1 );
    {
      //const real inv_sq_pi = 1/sqrt(2*PI);
      real inv_delta1 = INV_SQ_PI / ( delta1 + ESP );
      real inv_delta2 = INV_SQ_PI / ( delta2 + ESP );
      real sq2_inv_delta1 = 1 / ( 2 * delta1 * delta1 + ESP );
      real sq2_inv_delta2 = 1 / ( 2 * delta2 * delta2 + ESP );
      for ( i = 0; i < totalnum; ++i ) {
        real tempFeature = AllFeatures( i, j );
        real P_F_1 = inv_delta1 * exp( -( tempFeature - miu1 ) * ( tempFeature - miu1 ) * sq2_inv_delta1 );
        real P_F_0 = inv_delta2 * exp( -( tempFeature - miu2 ) * ( tempFeature - miu2 ) * sq2_inv_delta2 );
        real tmp = log( ( P_F_1 + ESP ) / ( P_F_0 + ESP ) );
        h_candidateWeakClf( i, j ) = tmp;
      }
    }
  }
  
  SAFEFREE( intg_pic );
  
#undef AllFeatures
#undef h_candidateWeakClf
  
}
