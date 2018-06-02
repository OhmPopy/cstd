
#ifndef _MILTRACK_H_
#define _MILTRACK_H_

#include "debug.h"

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct MILTRACK MILTRACK;
  
  MILTRACK* CPM_NewMilTrack(
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
    double v // = 0.95,//ѧϰ��,0.85~1,vԽС������ֵ�ͱ�׼��͸��µ�Խ�죬v = 1�򲻸���������ֵ�ͱ�׼����ڱ��治����߱仯�ܻ����Ķ���vӦ��ȡ���ӽ�1
  );
  
  int CPM_DelMilTrack( MILTRACK * pmt );
  
  /*
  �������ܣ�ѡ��һ������Ŀ��
  ����˵����
  pic_h     - ͼƬ��
  pic_w     - ͼƬ��
  I         - ͼƬ���Ͻ�ָ��
  step      - ͼƬÿ��������
  Y1        - ����Ŀ����ο����Ͻ� y����
  X1        - ����Ŀ����ο����Ͻ� x����
  sample_h  - ����Ŀ����ο��
  sample_w  - ����Ŀ����ο��
  ���أ�0��ʾʧ�ܣ�1��ʾ�ɹ�
  */
  int CPM_MilTrackSelectObject( MILTRACK * pmt,
                                int pic_h, int pic_w, unsigned char * I, int step,
                                int Y1, int X1, int sample_h, int sample_w );
                                
  /*
  �������ܣ����ƴ�����Ŀ����ͼƬ�е����Ͻ�����(X1, Y1)
  ����˵����
  pic_h     - ͼƬ��
  pic_w     - ͼƬ��
  I         - ͼƬ���Ͻ�ָ��
  step      - ͼƬÿ��������
  Y1        - ����Ŀ����ο����Ͻ� y����ָ��
  X1        - ����Ŀ����ο����Ͻ� x����ָ��
  ���أ�0��ʾʧ�ܣ�1��ʾ�ɹ�
  */
  int CPM_MilTrack( MILTRACK * pmt, int pic_h, int pic_w, unsigned char * I, int step,
                    int * Y1, int * X1 );
                    
#ifdef __cplusplus
}
#endif

#endif // _MILTRACK_H_
