/***************************************************************
SmartCombinExport   date: 06/03/2009
-------------------------------------------------------------
�汾: 1.1.0.1
���ߣ�����
˵����ģ�����UNICODE���룬����DSHOWʵ��
���£�
-------------------------------------------------------------
�������� Copyright (C) 2009 - All Rights Reserved
****************************************************************/

#ifndef _VIDEO_CTRL_
#define _VIDEO_CTRL_

#ifdef  VIDEOCTRL_EXPORTS
#ifdef __cplusplus
#define VIDEOSPE_API extern "C" __declspec(dllexport)
#else
#define VIDEOSPE_API __declspec(dllexport)
#endif
#else
#ifdef __cplusplus
#define VIDEOSPE_API extern "C" __declspec(dllimport)
#else
#define VIDEOSPE_API __declspec(dllimport)
#endif
#endif
#define VIDEO_CALL __stdcall
#define AUTO_STATE 0xffff

/****************************************************�������ͺͽṹ*****************************************************/

/*
�������ܣ�����Ƶ���ص����ⲿ
����������
[
pImage:     ���ص�ַ
width:      ͼ���
height:     ͼ���
step:       ͼ�񲽳�
channel:    ����ͨ��
cardIndex:  ��ǰ����ͷ��ID��
userData:   ���ûص�����ʱ�û��ṩ������
]
�������أ���
ʹ��˵����ϵͳ�����κδ���֮ǰ��ͨ���ûص�������ԭʼ��Ƶ���ݻص����ⲿӦ�ó���
*/
typedef void (VIDEO_CALL* OutImageStreamCallBack)
(
    BYTE* pImage , int width , int height , int step , int channel ,
    int cardIndex , long userData
);

typedef struct _Camera_Pro {
  HWND   m_ShowWnd;                    /*��Ƶ��ʾ����*/
  HWND   m_EventWnd;                   /*�¼���Ӧ����*/
  WCHAR  m_DisplayName[1024];          /*�ɼ�������ʾ���� , ָ�������ȼ���*/
  WCHAR  m_FriendlyName[1024];         /*�ɼ������Ѻ����� �� ָ�������ȼ���*/
  BOOL   m_BeUse;                      /*�豸�Ƿ�ʹ��*/
  long   m_AreadyLive;                 /*�豸�Ĵ��״�� -1��ȷ��״̬ �� 0 �豸����� , 1�豸���*/
  long   m_CameraID;                   /*�ɼ�����ţ� ָ�������ȼ���*/
  long   m_PID;                        /*ָ���豸��PID*/
  long   m_VID;                        /*ָ���豸��VID*/
  long   m_CaptureW;                   /*�ɼ��� */
  long   m_CaptureH;                   /*�ɼ���*/
  long   m_FramRate;                   /*֡��*/
  BOOL   m_Eddy;                       /*�Ƿ���ת*/
  BOOL   m_Repeat;                     /*�Ƿ���ż���غ�*/
  OutImageStreamCallBack m_CallBackFn; /*��Ƶ���ص�����*/
  long   m_UserData;                   /*�û�����*/
  long   m_AEWorAEBValue;                   /*��������Ƶ����*/
} Camera_Pro;

typedef enum _VideoProperty {
  /*ͼ������*/
  Brightness  = 0,                    /*����*/
  Contrast  = 1,                /*�Աȶ�*/
  Hue         = 2,                /*ɫ��*/
  Saturation  = 3,                /*���Ͷ�*/
  Sharpness = 4,                /*���*/
  Gamma     = 5,
  ColorEnable = 6,                /*��ɫ\�ڰ�*/
  WhiteBalance          = 7,      /*��ƽ��*/
  BacklightCompensation = 8,      /*���ⲹ��*/
  Gain                = 9,      /*����*/
  /*����ͷ����*/
  Camera_Pan      = 10,
  Camera_Tilt     = 11,
  Camera_Roll     = 12,
  Camera_Zoom     = 13,
  Camera_Exposure = 14,           /*�ع�*/
  Camera_Iris     = 15,           /*��Ȧ*/
  Camera_Focus    = 16        /*�۽�*/
} VideoProperty;


typedef enum _VIDEO_STATE {
  ERR_STATE = 0,
  RUN_STATE,
  PAUSE_STATE,
  STOP_STATE
} VIDEO_STATE;

typedef void* HANDLE;                   /*��Ƶ����������*/

typedef struct _MYRECT {
  RECT     m_Rect;                    /*���ο�*/
  COLORREF m_Color;                   /*��ɫ*/
} MYRECT;

typedef struct _CIRCLE {
  POINT  m_Center;                    /*���ĵ�����*/
  long   m_radius;                    /*�뾶*/
  long   m_flag;                      /*������Ч*/
  COLORREF m_Color;                   /*��ɫ*/
} CIRCLE;


//һ�½ṹ����ʱ����δ��
typedef struct _STRING {
  POINT        m_Start;               /*��ʼ����*/
  long         m_FontSize;            /*�����С*/
  COLORREF     m_Color;               /*��ɫ*/
  WCHAR        m_Str[64];             /*�ַ���UNICODE*/
} CHZZ_STRING;

typedef struct _CROSS {
  POINT        m_Center;              /*���ĵ�*/
  long         m_radius;              /*�뾶*/
  COLORREF     m_Color;               /*��ɫ*/
} CROSS;

typedef enum CAMTYPE { NORMAL , PRIVATE } CAMTYPE;

/**********************************************************���ܺ���****************************************************/

/*
�������ܣ����ϵͳ��Ƶ�豸��Ŀ�������豸�б�
������������
�������أ�ϵͳ����ͷ��Ŀ
*/
VIDEOSPE_API int  VIDEO_CALL  CHC_EnumCameraDeviceInSystem(CAMTYPE type);

/*
�������ܣ�ˢ��ϵͳ�豸�б�
������������
�������أ���
ʹ��˵��������ϵͳ��ǰʵ����Ƶ�豸���ˢ���豸�б�
*/
VIDEOSPE_API void  VIDEO_CALL CHC_UpdateCameraDeviceList(CAMTYPE type);

/*
�������ܣ��ͷ�����ö�ٵ��豸
������������
�������أ���
ʹ��˵�����ͷ��豸�б�����Ƶ��������豸�б�, ����Ƶ�豸����ʹ��ʱ�����豸���ᱻ����������Ϊ��Ч
*/
VIDEOSPE_API void  VIDEO_CALL CHC_ClearDeviceByEnum(void);

/*
�������ܣ��õ��豸����
����������
[
index: �б��±꣬��Χ[ 0 ,  ��Ƶ�豸��Ŀ)
pro  : �����豸�������Ϣ���ṹ�����ɿͻ����롣
]
�������أ��ɹ� TRUE �� ʧ�� FLASE
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_GetCameraDevicePro(int index , Camera_Pro* pro);

/*
�������ܣ�ͨ������������ִ��豸�б���ɾ���豸��
����������
[
deviceName: �豸�����֣��������Ѻ����ֻ����豸ȫ��
]
�������أ��ɹ� TRUE �� ʧ�� FLASE
ʹ��˵���������Ƶ����ʹ�ã����������Ч������Ƶ�豸ֹͣʹ��ʱ�Ž�����豸�б���ɾ����
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_ReleaseDeviceByName(BSTR deviceName);

/*
�������ܣ�������Ƶ���ƶ���
������������
�������أ���ƵԴ������ �� ʧ�ܷ���NULL
ʹ��˵�������ص���ƵԴ���������Ϊʶ��ģ���ͼ��Դ����
*/
VIDEOSPE_API HANDLE VIDEO_CALL CHC_CreateVideoCtrlObjEx(CAMTYPE type);

/*
�������ܣ�������Ƶ���ƶ���
������������
�������أ���ƵԴ������ �� ʧ�ܷ���NULL
ʹ��˵�������ص���ƵԴ���������Ϊʶ��ģ���ͼ��Դ����
*/
VIDEOSPE_API HANDLE VIDEO_CALL CHC_CreateVideoCtrlObj(void);

/*
�������ܣ��ͷ�����ƵԴ����
����������
[
pVideoCtrl: CHC_CreateVideoCtrlObj���ص���ƵԴ����
]
�������أ���
ʹ��˵�����ͷ�֮ǰȷ����ƵԴ�����Ѿ�������Ҫ���������������Ĺ�ϵ�Ѿ������
*/
VIDEOSPE_API void VIDEO_CALL  CHC_FreeVideoCtrlObj(HANDLE pVideoCtrl);

/*
�������ܣ�������Ƶ��ؿ�������
����������
[
pVideoCtrl����ƵԴ����
pro�� ���Խṹ��
]
�������أ���
ʹ��˵����������������Ƶ֮ǰ���ò�����Ч������ͷö�ٹ���
*/
VIDEOSPE_API void VIDEO_CALL  CHC_SetCtrlPro(HANDLE pVideoCtrl , Camera_Pro* pro);

/*
�������ܣ��õ���Ƶ��ؿ�������
����������pro ���Խṹ��
����������
[
pVideoCtrl����ƵԴ����
pro�� ���Խṹ��
]
�������أ���
*/
VIDEOSPE_API void VIDEO_CALL  CHC_GetCtrlPro(HANDLE pVideoCtrl ,  Camera_Pro* pro);

/*
�������ܣ�������Ƶ��
����������
[
pVideoCtrl����ƵԴ����
]
�������أ��ɹ�TRUE��ʧ��FALSE
ʹ��˵����������Ƶ֮ǰ���������ú�Camera_Pro��ز���
*/
VIDEOSPE_API BOOL VIDEO_CALL CHC_StartVideoStream(HANDLE pVideoCtrl);

/*
�������ܣ�ֹͣ��Ƶ��
����������
[
pVideoCtrl����ƵԴ����
]
�������أ��ɹ�TRUE��ʧ��FALSE
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_StopVideoStream(HANDLE pVideoCtrl);

/*
�������ܣ���ͣ��Ƶ��
����������
[
pVideoCtrl����ƵԴ����
]
�������أ��ɹ�TRUE��ʧ��FALSE
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_PauseVideoStream(HANDLE pVideoCtrl);

/*
�������ܣ��ĵ���Ƶ��ǰ�Ĺ���״̬
����������
[
pVideoCtrl����ƵԴ����
]
�������أ��ɹ�TRUE��ʧ��FALSE
*/
VIDEOSPE_API VIDEO_STATE VIDEO_CALL CHC_VideoState(HANDLE pVideoCtrl);

/*
�������ܣ�������Ƶ��ʾ�Ĵ���
����������
[
pVideoCtrl����ƵԴ����
videoWnd��  ��Ƶ��ʾ����
msgWnd��    �¼�֪ͨ����
]
�������أ��ɹ�TRUE �� ʧ��FALSE
ʹ��˵������������ʾ���ں��¼�֪ͨ����ΪNULL����ʾ����ʾ��Ƶ��ע��,����Ƶ��ʾ�Ĵ���Ҫ����ʱ��
����ڴ�������ǰ����ʾ��������Ϊ�գ�������Ƶ��һ���Ѿ����ٵĴ�������ʾ����������Ƶ�쳣��ֹ��
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_SetVideoWnd(HANDLE pVideoCtrl , HWND videoWnd , HWND msgWnd);

/*
�������ܣ���ʾ���ԶԻ���
����������
[
pVideoCtrl����ƵԴ����
]
�������أ��ɹ� TURE �� ʧ�� FALSE
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_DisplayProPage(HANDLE pVideoCtrl);

/*
�������ܣ��õ�ָ�����Եķ�Χ��Ĭ��ֵ
����������
[
pVideoCtrl��    ��ƵԴ����
Property��      ��������
pMin��          ��Сֵ
pMax��          ���ֵ
pSteppingDelta������ֵ
pDefault��      Ĭ��ֵ
]
�������أ��ɹ� TRUE ��ʧ�� FALSE
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_GetVideoProRange(HANDLE pVideoCtrl , VideoProperty Property , long* pMin , long* pMax , long* pSteppingDelta , long* pDefault);

/*
�������ܣ�����ָ�����Ե�ֵ
����������
[
pVideoCtrl����ƵԴ����
Property:   ��������
lValue:     ����ֵ
]
�������أ��ɹ� TRUE ��ʧ�� FALSE
ʹ��˵�����������ʧ�ܣ�����ֵά��ԭֵ
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_SetVideoPro(HANDLE pVideoCtrl , VideoProperty Property , long lValue);

/*
�������ܣ��õ�ָ�����Ե�ֵ
����������
[
pVideoCtrl����ƵԴ����
Property��  ��������
lValue ��   ��ǰֵ
]
�������أ��ɹ� TRUE ��ʧ�� FALSE
ʹ��˵��������ʧ�ܣ���ʾ��Ƶ��֧�ָ�����
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_GetVideoPro(HANDLE pVideoCtrl , VideoProperty Property , long* lValue);

/*
�������ܣ���Ƶ�л����ο�
����������
[
pVideoCtrl����ƵԴ����
pRect��     pRect ���ο�������ʼ��ַ
total��     ���ο������С
]
�������أ��ɹ� TRUE  , ʧ�� FALSE
ʹ��˵�������þ��ο������Ƶ��Ƶ���滭�����첽�ģ�Ҫ�����Ƶ����ʾ�ľ��ο���Ҫ����������Ϊ
�գ��������С����Ϊ0��
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_DrawRect(HANDLE pVideoCtrl , MYRECT* pRect , int total);

/*
�������ܣ���Ƶ�л�����Բ
����������
[
pVideoCtrl����ƵԴ����
pCircle     pCircle Բ��������ʼ��ַ
total��     ���ο������С
]
�������أ��ɹ� TRUE  , ʧ�� FALSE
ʹ��˵��������Բ������Ƶ��Ƶ���滭Բ���첽�ģ�Ҫ�����Ƶ����ʾ��Բ��Ҫ����������Ϊ
�գ��������С����Ϊ0��
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_DrawCircle(HANDLE pVideoCtrl , CIRCLE* pCircle , int total);

/*
�������ܣ���Ƶ�е����ַ�
����������
[
pVideoCtrl����ƵԴ����
pString��   STRING�������ʾ��ַ����ʾ�������ַ�
total��     �����С
]
�������أ��ɹ� TRUE  , ʧ�� FALSE
ʹ��˵����
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_DrawString(HANDLE pVideoCtrl , CHZZ_STRING* pString , int total);

/*
�������ܣ�����Ƶ������ȡһ��ͼƬ.
����������
[
pVideoCtrl����ƵԴ����
pImage:     ͼ��CIPImageָ��
tp��        ͼ����������˳��TRUE��ʾ���ؾ���ԭ�������Ͻǣ�FALSE��ʾ���½ǡ�
]
�������أ��ɹ� TRUE  , ʧ�� FALSE
ʹ��˵������pImage�ṹ��������һ������Ϊ0ʱ���ú������زɼ�ͼ���С��������䴫��Ľṹ�壬
���Կ�������������ʣ����ȵõ�ͼ��Ĵ�С�������ڴ棬Ȼ���ٵ��øú����õ�ͼ��
*/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_CaptureImage(HANDLE pVideoCtrl , CIPImage* pImage , BOOL tp);

VIDEOSPE_API BOOL VIDEO_CALL  CHC_SetAGBValue(long lValue);

/******��Ƶ����****/
VIDEOSPE_API void VIDEO_CALL  CHC_SetVideoVChange(BOOL bValue, BOOL isOwnVideo /*= FALSE*/);//

VIDEOSPE_API BOOL VIDEO_CALL  CHC_SetSenseAuto(long lValue , BOOL isOwnVideo /*= FALSE*/);//

/******�򿪹رպ����****/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_OpenLight(BOOL openorclose, BOOL isOwnVideo /*= FALSE*/);//,

/******������ͷ��д����***/
VIDEOSPE_API BOOL VIDEO_CALL  CHC_VideoWrite(int addressWrite, BYTE byteV[100]); //,BOOL isOwnVideo = FALSE

//������Ƶ�Ĵ�����AGC\AEC�Զ�,����ΪAEW/AEBֵ

#endif