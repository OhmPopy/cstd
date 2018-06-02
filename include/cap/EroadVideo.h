
// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the VIDEO_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// VIDEO_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifndef VIDEO_MODE
#define VIDEO_MODE
#ifdef __cplusplus
#define VIDEO_API extern "C" __declspec(dllexport)
#else
#define VIDEO_API __declspec(dllexport)
#endif

typedef BOOL (WINAPI* PFCALLBACK)(DWORD Width, DWORD Height, BYTE* buffer, LPVOID param) ;
//��ʾ��Ƶ�Ļص�������Width��Height�ֱ�Ϊ��Ƶ�Ŀ�Ⱥ͸߶ȣ�bufferΪRGB��ʽ����Ƶ����

BOOL WINAPI CBFunc(DWORD Width, DWORD Height, BYTE* buffer, LPVOID param);
//��Ƶ��ʾ�Ļص�����

VIDEO_API BOOL SetVideoModel(BYTE VideoModel);//0:640*480,1:320*240
//������Ƶ��ʾģʽ��VideoModel=0Ϊ640*480�ķֱ�����ʾ��VideoModel=1Ϊ320*240�ķֱ�����ʾ������Ϊ640*480�ֱ�����ʾ

VIDEO_API BOOL StartVideo(PFCALLBACK Func, LPVOID param);
//������Ƶ��FuncΪ����Ļص�������paramΪ�ص���������

VIDEO_API void ShowVideo(HWND hWnd, DWORD Width, DWORD Height);
//��ʾ��Ƶ��hwndΪ���ھ��������ΪNULL��Width��Height�ֱ�Ϊ��Ƶ�Ŀ�͸�

VIDEO_API BOOL SetSensorAEWorAEB(BYTE light);
//���ô�������Ƶ���ȣ���Χ��0-255��Ĭ�ϴ�Ϊ63

VIDEO_API BOOL SetSensorAGC(BYTE Contrast);
//���ô�������Ƶ�Աȶȣ���Χ��0-255��Ĭ�ϴ��Ǵ��������Զ����ã��ֶ�Ĭ��Ϊ38

VIDEO_API BOOL SetSensorAEC(int Saturation);
//������Ƶ�ع�ȣ���Χ��0-65535��Ĭ�ϴ�Ϊ�������Զ����ã��ֶ�Ĭ��Ϊ140

VIDEO_API BOOL SetManuallight();
//�ֶ��������ȵĿ��أ����ڴ�������ԭ������û���Զ�����һ�Ҫ�ֶ��������ȱ��뽫�ԱȶȺ��ع������Ϊ�Զ�

VIDEO_API BOOL SetManualcontrast(BOOL state);
//�ֶ��Զ����öԱȶȵĿ��أ�stateΪTRUE���ֶ���FALSE���Զ�

VIDEO_API BOOL SetManualsaturation(BOOL state);
//�ֶ��Զ������ع�ȵĿ��أ�stateΪTRUE���ֶ���FALSE���Զ�

VIDEO_API BOOL SetFrameRate(int rate);
//����֡�ʣ�rate��60��30��15����ֵ������������Ϊ60

VIDEO_API BOOL GetBMPPhoto(TCHAR* filename);
//ɨ��ͼ��filenameΪ�����ļ�·����ɨ��ͼ�񱣴��ļ���

VIDEO_API BOOL GetPhoto();
//���Ӳ�����հ�ť�Ƿ��£��˺�����Ҫ��һ����ʱ��������ʵʱ���
//���Ӳ�����հ�ť���·���TRUE�����򷵻�FALSE������Բ�ͬ�ķ���ֵ����Ӧ�Ĵ���

VIDEO_API int GetSaturation();
//����ֶ����õ��ع�ֵ���������Զ�����ʱ��Ч

VIDEO_API int GetContrast();
//����ֶ����õĶԱȶȣ��������Զ�����ʱ��Ч

VIDEO_API int GetLight();
//����ֶ����õ�����

VIDEO_API void ExitVideo();
//�ر���Ƶ�豸���˳�����������

VIDEO_API void SethandLight(int light);
//������λ��Ƶ���ȣ����������ֶ��Զ���ֻ���ֶ�

VIDEO_API void SethandContrast(int contrast);
//������λ��Ƶ�Աȶȣ����������ֶ��Զ���ֻ���ֶ�

VIDEO_API void SetThresholdValue(int value);
//������λ�Աȶȷ�ֵ

VIDEO_API int GethandLight();
//�����λ��Ƶ����

VIDEO_API int GethandContrast();
//�����λ��Ƶ�Աȶ�

VIDEO_API int GetThresholdValue();
//�����λ�Աȶȷ�ֵ

VIDEO_API BOOL CheckSensor();
//����豸�Ƿ����Ӿ���

VIDEO_API BOOL SendMsg(BYTE msg);
//���豸������Ϣ
#endif