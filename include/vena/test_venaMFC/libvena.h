
#ifndef _LIBVENA_H_
#define _LIBVENA_H_

#define VENA_DEBUG 0 // ����ģʽ
#define VENA_STDH  96 //��һ��ͼƬ��
#define VENA_STDW  64 //��һ��ͼƬ��
#define VENA_FEATLEN  (64*59) // ������С�ֽ�

//#undef GETARG
//#undef GETARGS
// buf, buflen ��Ҫ���ڴ����ٴ��� 10M
// h, w, img, imgstep, imgcn cn ����ͼ�� ֧�� 3(��ɫ) �� 1(�Ҷ�)
// n ������������
// featlib ������ ����ΪNULL
// featstep һ������ռ���ֽ��� ����>=VENA_FEATLEN
// imstd ��һ�� ͼƬ [VENA_STDH*VENA_STDW] ����ΪNULL
// feat ��ȡ��ǰͼƬ������ ����ΪNULL
// pid ���id
// pdis �������
// �������� 1��ʾ��⵽ 0��ʾû��⵽

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) int  __stdcall vena_recog_bf(unsigned char* buf, int buflen, int h, int w, unsigned char* img, int imgstep, int imgcn, int n, unsigned char* featlib, int featstep, unsigned char* imstd, unsigned char* feat, int* pid, int* pdis);

#ifdef __cplusplus
}
#endif

#endif // _LIBVENA_H_
