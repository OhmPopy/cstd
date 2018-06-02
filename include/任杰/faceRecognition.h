
/***************************************************************
faceRecognition   date: 06/03/2009
------------------------------------------------------------- 
�汾: 1.0.0.1
���ߣ���ΰ
˵��������ͼ���һ����Ԥ����������ȡ���ȶ�
-------------------------------------------------------------
�������� Copyright (C) 2009 - All Rights Reserved
****************************************************************/
#ifndef _FACERECOGNITION_H_
#define _FACERECOGNITION_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _USE_STDCALL_
#define CHC_STDCALL __stdcall
#else
#define CHC_STDCALL __cdecl
#endif


#ifndef REAL_TYPE
  // ��������
  typedef int INT;
  typedef float real;
  typedef unsigned char uchar;
#define USEFLOT
#ifdef USEFLOT
#define REAL_TYPE real
#else
#define REAL_TYPE int
#endif // USEFLOT
#endif

  // point�ṹ
#ifndef _DEFINED_IPOINT_
#define _DEFINED_IPOINT_

  typedef struct tagIPOINT {
    int x;
    int y;
  }
  IPOINT, *PIPOINT;
#endif

#ifndef _DEFINED_FPOINT_
#define _DEFINED_FPOINT_

  typedef struct tagFPOINT {
    float x;
    float y;
  }
  FPOINT, *PFPOINT;
#endif


  /*
   �������ܣ�������һ��
   ����˵����
   [ah, aw, A, al, ai] - ����ͼƬ
   [bh, bw, B, bl, bi] - ���ͼƬ
   leye, reye - ����������λ��
   lx - ����x�������bw һ��ȡ 0.25
   ly - ����y�������bh һ��ȡ 0.25
   rx - ����x�������bw һ��ȡ 0.75
   ry - ����y�������bh һ��ȡ 0.25
   �������أ�������
   ʹ��˵����
  */
  int CHC_STDCALL CHC_FaceStd(int ah, int aw, const uchar* A, int al, int ai,
                              int bh, int bw, uchar* B, int bl, int bi,
                              IPOINT leye, IPOINT reye,
                              real lx, real ly, real rx, real ry);

  /* �˲����Ժ��������ã� */
  int CHC_STDCALL imfilter(int ah, int aw, const uchar* A, int al, int ai,
                           uchar* B, int bl, int bi, int x, int y);

  /*
   �������ܣ�������ĻҶ�ͼ��ppԤ����
   ����˵����
   [ah, aw, A, al, ai] - ����ͼƬ����һ��ͼ��
   [ah, aw, B, bl, bi] - ���ͼƬ
   �������أ�������
   ʹ��˵����
  */
  int CHC_STDCALL CHC_PpLight(int h, int w, const uchar* A, int al, int ai, uchar* B, int bl, int bi);

  /*
   �������ܣ�������ĻҶ�ͼ����ȡ����
   ����˵����
   ���룺
   [ah, aw, A, al, ai] - һ����һ��ͼ���pp������ͼ��
   type                - ��������
   �����
   featptr             - ����������飬ָ�������ⲿ���䣬ָ��ΪNULLʱ��������real�ĸ���
   ���� - ���������С
   ע�⣺ֻ��PPBOOSTLBPXBIN����Ҫ100��100��ͼ��������������Ҫ
   �������أ����ֻ��ͼ��ߺͿ�������������ά��
   ʹ��˵����
  */
  int CHC_STDCALL CHC_GetFeature(int ah, int aw, const uchar* A, int al, int ai, int type, real* featptr);

  // ����汾��������ȡ����
  int CHC_STDCALL lbp_ex_ft_16s(int srch, int srcw, const uchar* src, int srcstep, int cn, short* featptr, int isall, int fillborder);

#define CHI_DIST(a, b, out)  { out = (a) + (b); (out) = (out) ? (((a)-(b))*((a)-(b))/(out)) : 0; }
#define ABS_DIST(a, b, out)  { (out) = ((a)>(b)) ? ((a)-(b)) : ((b)-(a)); }

  /*
   �������ܣ���������
   ����˵����
   n    - ���������С
   a, b - ָ������
   type - ��������
   �������أ�
   ���ظ������ֵ(ֵԽС��ʾa��bԽ����)
   ����ֵ��:D=[0,+inf]
   ӳ�䵽[0,1]�Ĺ�ʽ: S=1/(1+D)
   ʹ��˵����
  */
  real CHC_STDCALL CHC_Distance(int n, const real* a, const real* b, int type);

  int CHC_STDCALL CHC_ChiSquare16s(int n, const short* a, const short* b);

  /*
   �������ܣ�����ͼ��A��ͼ��A�����ҶԳ�ͼ��ľ���
   ����˵����
   type    - ��������(�Ƽ�ֵΪ�� LBP59RECTBIN)
   �������أ�ͼ��A��ͼ��A�����ҶԳ�ͼ��ľ���
   ʹ��˵�������type�Ƿ����Ĭ��ֵ�� LBP59RECTBIN
  */
  real CHC_STDCALL CHC_SymmetryDist(int h, int w, const uchar* A, int al, int type);

#define RECOGNITION_0 (0)  /* ���ó���Ϊ�ɼ���ļ��������������������11�������������0��Ҫ������ͼƬΪ80*80����������Ϊ64000ά�����ó���Ϊ�ɼ����                                    ���������ע������1-10���˵�ϵͳ����һ������СֵΪ0�����ֵΪ50000����ֵΪ0.52������0.52��Ϊ�ܾ�ͨ����С��0.52��Ϊͨ��*/
#define RECOGNITION_1 (1)  /* ���ó���Ϊ������Ƶ���ϵͳ�Ŀɼ���������ȶ�ϵͳ�ɼ��⣬������������11�����������ʽ2��Ҫ������ͼƬΪ80*80������                                               ����Ϊ64000ά������һ������СֵΪ0�����ֵΪ288550����ֵΪ0.52��С��0.52��Ϊ�ܾ�ͨ��������0.52��Ϊͨ��*/
#define RECOGNITION_2 (2)  /* ���ó���Ϊ������ļ��������ϵͳ��ע������Ϊ1-10���ˡ�������������11�����������ʽ2��Ҫ������ͼƬΪ80*80����������Ϊ64000ά��                                                     �ȶ�ϵͳ����һ������СֵΪ0�����ֵΪ169000 ����ֵΪ0.52��С��0.52��Ϊ�ܾ�ͨ��������0.52��Ϊͨ��*/
#define RECOGNITION_3 (3)  /* ���ó���ΪPC����,ע������Ϊ��1-600���ˣ��Ľ�����ļ�����������ڣ��Ž�ϵͳ�����õ���������Ϊ6�����������ʽΪ0��Ҫ����                                    ���ͼƬΪ100*100����������Ϊ2243ά����һ������СֵΪ0�����ֵΪ8000����ֵΪ0.51��ʶ��÷ִ���0.51��ܾ�ͨ����С��0.51��                                    Ϊͨ����*/
#define RECOGNITION_4 (4)  /* ���ó���Ϊ����ͼ���������жϣ��ж�ͼ���Ƿ�Ϊ����ϸ�������������������9��Ҫ������ͼƬΪ100*100���÷ִ���100000������ͼ                                     ���������ϸ� */
#define RECOGNITION_5 (5)  /* ���ó���ΪPC����,ע������Ϊ��1-600���ˣ��Ľ�����ļ�����������ڣ��Ž�ϵͳ�����õ���������Ϊ12�����������ʽΪ0��Ҫ��                                   ���ͼƬΪ100*100����������Ϊ2243ά����һ������СֵΪ0�����ֵΪ12154����ֵΪ0.52��ʶ��÷ִ���0.52��ܾ�ͨ����С��0.52                                       Ϊͨ�������ξ�س���������㷨��һ������  */

  /* ʶ���㷨 */
  typedef struct RECOALGO {
    int feat_type;    /* �������� */
    int feat_dims;    /* ����ά�� */
    int std_height;   /* ��һ��ͼƬ�ߣ�0��ʾ����ֵ */
    int std_width;    /* ��һ��ͼƬ�ߣ�0��ʾ����ֵ */
    int std_cn;       /* ��һ��ͼƬͨ���� */
    int is_dist;      /* 1��ʾ�Ǿ��룬0��ʾ���ƶ� */
    real dist_min;    /* ������Сֵ�����ڰѾ����һ����0~1֮�� */
    real dist_max;    /* �������ֵ����ʽ��(����ֵ-dist_min)/(dist_max-dist_min) */
    real dist_th;     /* ������ֵ */
    int dist_type;    /* �������� */
  }
  RECOALGO;

  /*
   �������ܣ�ȡ����ʶ���㷨����ID��������Ϣ
   ����˵����
   ���룺
   type                - ��������
   �����ʶ���㷨�ṹ��ָ�룬NULL��ʾû�������������Ϣ
  */
  const struct RECOALGO* CHC_STDCALL CHC_GetRecoAlgoInfo(int type);

#ifdef __cplusplus
}
#endif

#endif // _FACERECOGNITION_H_
