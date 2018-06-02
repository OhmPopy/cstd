#ifndef _FEATURE_H_
#define _FEATURE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _USRDLL
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC
#endif

typedef unsigned char uchar;

// �������Ͷ���
typedef struct tag_complex_se {
	double re, im;
} complex_se;

/* ��������PP+LBP59BIN����
 * ah:      �ú�����������100 x 100�Ҷ�ͼ, ����Ϊ100.
 * aw:      �ú�����������100 x 100�Ҷ�ͼ, ����Ϊ100.
 * A :      ͼ������ָ��.
 * al:      �ú�����������100 x 100�Ҷ�ͼ, ����Ϊ100.
 * ai:      �ú�����������100 x 100�Ҷ�ͼ, ����Ϊ1.
 * type:    ��������, ��ǰ��ֵ��ָ������ֵ.
 * featptr: ����ָ��, ��ָ��Ϊ��ʱ������������Ҫ��float�������������С, �����������������.
*/
DECLSPEC int feature_gen(int ah, int aw, const uchar* A, int al, int ai, int type, float* featptr);

/* ��������PP+LGBP8BIN����
 * ah:      �ú�����������80 x 80�Ҷ�ͼ, ����Ϊ80.
 * aw:      �ú�����������80 x 80�Ҷ�ͼ, ����Ϊ80.
 * A:       ͼ������ָ��.
 * al:      �ú�����������80 x 80�Ҷ�ͼ, ����Ϊ80
 * ai:      �ú�����������80 x 80�Ҷ�ͼ, ����Ϊ1.
 * type:    ��������, ��ǰ��ֵ��ָ������ֵ.
 * featptr: ����ָ��, ��ָ��Ϊ��ʱ������������Ҫ��float�������������С, �����������������.
*/
DECLSPEC int feature_gen2(int ah, int aw, const unsigned char* A, int al, int ai, int type, float* featptr);

/*
 * n: ���������, �˴���������nΪ40�����
 * a: ����a��ָ��, a�ĳ���Ϊ40 * 191
 * b: ����b��ָ��, b�ĳ���Ϊ40 * 191
*/
DECLSPEC float cos_dist(int n, const float *a, const float *b);

#ifdef __cplusplus
}
#endif

#endif