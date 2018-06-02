
#ifndef __MILTRACK_H_
#define __MILTRACK_H_

#include "findfile.h"

#define ESP  0.000000001
#define INV_SQ_PI  0.39894228040143270  // 1/sqrt(2*PI)

#define _USE_LBP_FEATURE
#define _DEBUG_DQ

#if 0
#define DO_FPRINT2D(_F, FMT, _H, _W, _A, _L, _I) FPRINT2D(_F, FMT, _H, _W, _A, _L, _I)
#else
#define DO_FPRINT2D(_F, FMT, _H, _W, _A, _L, _I) {}
#endif

#define FOREACH_DO(_N, _I, _DO)  for (_I=0; _I<(_N); ++_I) {_DO;}

typedef double real;


typedef struct MILPOINT {
  int y;
  int x;
}
MILPOINT;

typedef struct MILHAAR {
  MILPOINT StPos;
  MILPOINT Scale;
  int Kind;
  int Type;
  //��˹�ֲ���ֵ�ͱ�׼��
  real MIU1;  // ������ƽ��ֵ
  real DELTA1; // ����������
  real MIU2;  // ������ƽ��ֵ
  real DELTA2; // ����������
}
MILHAAR;

// �������ܣ����ھ���m*n�е�ÿ��Ԫ�أ����������ΧΪ[1,a]������
int unidrnd( int a, int m, int n );

//�������ܣ��������������ѡ��M���������(����λ�á��߶Ⱥ�����)
int randomMFeatures( int sample_w, int sample_h, int M, MILHAAR* MFeatures );

//�������ܣ���һ����Χ�����ѡ���������͸��������������ظ������������������Ͻǵ�����
//(X,Y)Ϊ��ǰλ�þ��ο����Ͻǵ�����
//sample_w��sample_hΪ(����)������͸�
//pic_w��pic_hΪ֡�Ŀ�͸�
//[beita1��beita2]��gamaΪ���������������ķ�Χ
//tempnum_neg��tempnum_posΪ����������������
int randomSelectSample( int X, int Y, int sample_w, int sample_h, int pic_w, int pic_h,
                        int beita1, int beita2, int gama, int tempnum_neg, int tempnum_pos,
                        int frame, MILPOINT*YX_all );
                        
//�������ܣ�������һ����Χ�����ѡ��ĺ�ѡλ��(�������Ͻǵ�����)
//(X,Y)Ϊ��ǰλ�þ��ο����Ͻǵ�����
//sample_w��sample_hΪ(����)������͸�
//pic_w��pic_hΪ֡�Ŀ�͸�
//sΪ��ѡ�����ķ�Χ
//num_candidate_result Ϊ��ѡ������
//num_fix_cr Ҫ��ԭλ�ø����̶���������
int randomSelectSample_for_result( int X, int Y, int sample_w, int sample_h, int pic_w, int pic_h,
                                   int s, int num_candidate_result, int num_fix_cr, MILPOINT* YX_candidate );
                                   
                                   
//�������ܣ�����ÿ�������ڻ���ͼ�ж�Ӧ������ֵ
int GetOneFeatureValue_2( int FeatureType, MILPOINT FeatureStPos, MILPOINT FeatureScale,
                          int* _InterData, int InterStep );
                          
void my_imadjust( int _N, const unsigned char* f, unsigned char* p );
void histeq( int _N, const unsigned char* f, unsigned char* p );

#ifdef _DEBUG_DQ
#define imadjust(_N, f, p) {}//my_imadjust(_N, f, p)
#else
#define imadjust(_N, f, p) {}
#endif

//�������ܣ��������λ��(�������Ͻǵ�����)
//YX_candidateΪ��ѡ(����)λ�õ����Ͻǵ�����
//sample_w��sample_hΪ(����)������͸�
//SelectKFeaturesΪѡ�������K������(����λ�á��߶ȡ�����)
//SelectMIU1,SelectDELTA1,SelectMIU2,SelectDELTA2Ϊѡ�������K��������Ӧ��������ֵ�ͱ�׼��
MILPOINT compute_reult( int num_candidate, const MILPOINT* YX_candidate, int sample_w, int sample_h,
                        int K, const MILHAAR* SelectKFeatures, const int* L_ind,
                        unsigned char* I, int pic_h, int pic_w, int step );
                        
void candidateWeakClf( int tempnum_neg, int tempnum_pos, const MILPOINT* YX_all, int sample_w, int sample_h,
                       MILHAAR* MFeatures, int M, unsigned char* I, int pic_h, int pic_w, int step,
                       real v, int frame, real* _h_candidateWeakClf );
                       
int icvIntegralImage( const unsigned char* src, int srcstep, int* sum, int sumstep, int size_height, int size_width );

/*��������
Ӱ����پ��ȵ���Ҫ����ΪM��K (��Ϊ��Ҫ)������������
������������ѧϰ��v ͨ����ѧϰ�ʵ��ڵ��ӽ�1����0.99��
�ɴ������߸�����������������ʹ������ֵ�ͱ�׼��ĸ����ٶȱ�����
���ܲ����ڱ����ںܶ�ʱ���ڱ仯�ϴ�����)
*/

typedef struct MILTRACK {
  int M; // = 1000; //��ѡ������(��ѡ��������)
  int K; // = 100; //ѡ��������(ѡ����������)
  int num_neg; // = 65;//��������
  int num_pos; // = 45;//��������
  //int num_all; // = 110; //��������
  int num_candidate_result; // = 500; //����������,Ҫ>=num_fix_cr
  //int num_replace_clk = 5;//�����滻�ķ�����(����)��
  int beita1; // = 50;//��������������õ��ľ���1
  int beita2; // = 80;//��������������õ��ľ���2
  int gama; //  = 5; //��������������õ��ľ���
  int s; // = 30;//��ѡ��������,Ҫ>=gama,����ñ�֤s > (num_fix_cr/10)/2
  int num_fix_cr; // = 300;//Ҫ��ԭλ�ø����̶���������,ĿǰȡֵΪ100��200��300��������randomSelectSample_for_result.m
  real v; // = 0.95;//ѧϰ��,0.85~1,vԽС������ֵ�ͱ�׼��͸��µ�Խ�죬v = 1�򲻸���������ֵ�ͱ�׼����ڱ��治����߱仯�ܻ����Ķ���vӦ��ȡ���ӽ�1
  
  int X1, Y1, sample_w, sample_h;
  MILHAAR* MFeatures; // [M]
  real* L; // [M]
  int* L_ind; // [M]
  
  MILPOINT* YX_all; // = NULL; // [(num_neg+num_pos)]
  MILPOINT* YX_candidate; // = NULL; // [num_candidate_result]
  real* _h_candidateWeakClf; // [M*(num_neg+num_pos)]
}
MILTRACK;

MILTRACK* NewMilTrack(
  int M,  // = 1000, //��ѡ������(��ѡ��������)
  int K,  // = 100, //ѡ��������(ѡ����������)
  int num_neg,  // = 65,//��������
  int num_pos,  // = 45,//��������
  //int num_all, // = 110, //��������
  int num_candidate_result, // = 500, //����������,Ҫ>=num_fix_cr
  //int num_replace_clk = 5,//�����滻�ķ�����(����)��
  int beita1, // = 50,//��������������õ��ľ���1
  int beita2, // = 80,//��������������õ��ľ���2
  int gama, //  = 5, //��������������õ��ľ���
  int s, // = 30,//��ѡ��������,Ҫ>=gama,����ñ�֤s > (num_fix_cr/10)/2
  int num_fix_cr, // = 300,//Ҫ��ԭλ�ø����̶���������,ĿǰȡֵΪ100��200��300��������randomSelectSample_for_result.m
  real v // = 0.95,//ѧϰ��,0.85~1,vԽС������ֵ�ͱ�׼��͸��µ�Խ�죬v = 1�򲻸���������ֵ�ͱ�׼����ڱ��治����߱仯�ܻ����Ķ���vӦ��ȡ���ӽ�1
);

#endif // __MILTRACK_H_
