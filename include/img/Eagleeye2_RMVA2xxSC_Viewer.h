
#ifndef _EAGLEEYE2_RMVA2XXSC_VIEWER_H_
#define _EAGLEEYE2_RMVA2XXSC_VIEWER_H_

#define WM_RECVTRGIMAGEOK  WM_USER + 5  //message variable for notice view after recv data
#define WM_SWICHNEWCAMERA  WM_USER + 6  //message variable for notice Mainframe after switch new camera

#define IMGWIDTH  1616        //horizontal resolution of image
#define  IMGHEIGHT  1232        //vertical resolution of image
#define IMGLENGTH  IMGWIDTH*IMGHEIGHT  //resolution of image

#define NUM_BUFF  5          //buffer number for motion jpeg 
#define  MAX_BUFF  IMGWIDTH*IMGHEIGHT*2//Size of each buffer loading MJpeg

//////////////////////////////////////////////////////////////////////////
//socket ports
#define COMMANDPORT    35000        //�������������������̶߳˿�
#define TRGIMGPORT     45000        //����ģʽ��������������ݽ����������̶߳˿�
#define FCONIMGPORT    55000        //ȫ�ֱ�������ģʽ��������������ݽ����������̶߳˿�
#define HFRIMGPORT     65000        //������ģʽ��������������ݽ����������̶߳˿�
#define HFRBINIMGPORT  60000        //�����ʸ�������ģʽ��������������ݽ����������̶߳˿�
#define IMGINFLEN    sizeof(EagleEye2ImgInfStruct)//ץ����Ϣ����

//struct of buffer node for loading MJPEG
typedef struct tag_BuffNode {
  int          sizeBuff;
  char        pBuff[MAX_BUFF];
  BOOL        bIsLocked;
} BUFFNODE;
//ץ����Ϣ
typedef struct ImgInfStruct {
  char  cLocusInf[101];      //ץ�ĵص���Ϣ����50������
  BYTE  byTimeInf[7];      //ץ��ʱ�䣬��-��-��-ʱ-��-��
  BYTE  byWayNum;        //������
  BYTE  byTrgMode;        //������ʽ��0->����;1->����Ȧ;2->˫��Ȧ(����)
  float  fSpeed;          //����(��λ:����/Сʱ)
} EagleEye2ImgInfStruct;

//�������������ģʽ,ͼ��ɼ���ʽ�Ͱ�ƽ����ջ�����ö�ٱ�������
enum EagleEye2_SensorRunMode { TrgMode, FconMode, HfrMode, HfrBinMode };
enum EagleEye2_ImgAcquisition_Format { RGB_ARRAY, YUV_ARRAY, BAYER_ARRAY };
enum EagleEye2_WhiteBalance_environment { DAYLIGHT, FLUORESCENCE, WITHOUTWB };

//struct for PIO
typedef struct PIO_indicator {
  unsigned pio1 : 1;
  unsigned pio2 : 1;
  unsigned pio3 : 1;
  unsigned pio4 : 1;
  unsigned pio5 : 1;
  unsigned pio6 : 1;
  unsigned pio7 : 1;
  unsigned pio8 : 1;
  unsigned pio9 : 1;
  unsigned pio10 : 1;
  unsigned pio1outvalue : 1;
  unsigned pio2outvalue : 1;
  unsigned pio3outvalue : 1;
  unsigned pio4outvalue : 1;
  unsigned pio5outvalue : 1;
  unsigned pio6outvalue : 1;
  unsigned pio7outvalue : 1;
  unsigned pio8outvalue: 1;
  unsigned pio9outvalue : 1;
  unsigned pio10outvalue : 1;
  unsigned reserved : 12;
} PIO_indicator;

typedef struct RTCConfig {
  UINT  m_iDay;
  UINT  m_iHour;
  UINT  m_iMinute;
  UINT  m_iMonth;
  UINT  m_iSecond;
  UINT  m_iWeek;
  UINT  m_iYear;
} RTCConfig;

//////////////////////////////////////////////////////////////////////////
typedef struct Eagleeye2_RMVA2xxSC_Viewer {
  img_t*          m_fPic;        //FCImage�����
  img_t*          m_fPic1;      //FCImage�����
  BOOL            m_bFirstShow;    //��ͼ�Ƿ��ǵ�һ����ʾ
  volatile HWND    m_hViewWnd;      //��ͼ�Ĵ��ھ������������ͼ�෢����Ϣ
  volatile HWND    m_hMainFrameWnd;  //����ܵĴ��ھ����������������෢����Ϣ
  BOOL            m_bIsDataFromCamera;//��ǰͼ���Ƿ����������
  BOOL            m_bIsJpeg;      //��ʾ��ǰͼ���Ƿ���JPG��ʽ
  BOOL            m_bIsHighFR;    //��־��ǰ��Ҫ��ʾ��ͼ���Ƿ����Ը�����ģʽ���Դ���������������ģʽ��������OnDraw�����н�����ʾ����

  //һЩͨ�ÿ��Ʊ���
  BOOL          m_bIsTrgMode;        //�Ƿ��Ǵ���ģʽ
  BOOL          m_bIsFconMode;       //�Ƿ���ȫ�ֱ�������ģʽ
  BOOL          m_bIsHFRMode;        //�Ƿ��Ǹ�����ģʽ
  BOOL          m_bIsHFRBinMode;     //�Ƿ��Ǹ����ʸ�������ģʽ
  BOOL          m_bIsEnsureTgtIP;    //�Ƿ�ȷ��Ŀ�������IP��ַ
  BOOL          m_bRelayIsOpen;      //�̵����Ƿ�Ͽ�
  BOOL          m_bLUTEnable;        //����Ƿ�ʹ��ͼ���LUTӳ��任
  BOOL          m_bAutoTrgEnable;    //�л�������ģʽ���Ƿ������Զ�����ץȡһ��ͼ��
  BOOL          m_bIsRGBArray;       //����ɼ���ͼ���ʽ�Ƿ�ΪRGB��ʽ
  BOOL          m_bIsYUVArray;       //����ɼ���ͼ���ʽ�Ƿ�ΪYUV��ʽ
  BOOL          m_bIsBAYERArray;     //����ɼ���ͼ���ʽ�Ƿ�ΪBAYER��ʽ
  BOOL          m_bTrgShutterIsManual;  //����ģʽ�����ٶ��Ƿ����ֶ�ģʽ

  BYTE  m_byIPAdd1, m_byIPAdd2, m_byIPAdd3, m_byIPAdd4;


  BOOL  m_bPort10OUTValue;
  BOOL  m_bPort1OUTValue;
  BOOL  m_bPort2OUTValue;
  BOOL  m_bPort3OUTValue;
  BOOL  m_bPort4OUTValue;
  BOOL  m_bPort5OUTValue;
  BOOL  m_bPort6OUTValue;
  BOOL  m_bPort7OUTValue;
  BOOL  m_bPort8OUTValue;
  BOOL  m_bPort9OUTValue;

  BOOL  m_bIsPort1IN;
  BOOL  m_bIsPort2IN;
  BOOL  m_bIsPort3IN;
  BOOL  m_bIsPort4IN;
  BOOL  m_bIsPort5IN;
  BOOL  m_bIsPort6IN;
  BOOL  m_bIsPort7IN;
  BOOL  m_bIsPort8IN;
  BOOL  m_bIsPort9IN;
  BOOL  m_bIsPort10IN;
  BOOL  m_bIsFlashSynEnable;
  struct PIO_indicator  m_pioIndicator;

  BOOL  m_bIsPortDirAdj;
  BOOL  m_bIsPortOutAdj;
  BOOL  m_bIsFlashSynAdj;

  UINT  m_uiWBType; // Daylight | Fluorescence | Without WhiteBalance
  UINT  m_iWDT; // WD Countor:
  BOOL  m_bIsWDTEnable; // Enable Watch Dog

  float m_fAfeVGAGain; // CCD Signal PGA Config (0.f, 36.f)


  //ץ����Ϣ
  char  m_strLocation[256]; // ץ�ĵص�
  float m_fInterval; // ��Ȧ���:
  int   m_iWayIndex; // �� �� ��:
  BOOL  m_bIsSingalCoin; // ����Ȧ����
  BOOL  m_bIsDoubleCoin; // ˫��Ȧ����(����)

  // �����������
  BOOL  m_bIsShutPgaManualCtrl; // ���š������ֶ�����
  UINT  m_uiTrgShutterTime; // ����(uS):
  float m_fTrgPga; // ����(dB):

  BOOL  m_bIsShutPgaAutoCtrl; // ���š������Զ�����
  int   m_iShutLimit; // ��������(uS):
  float m_fPGALimit; // ��������(dB):
  int   m_iCurShut; // ��ǰ����(uS):
  float m_fCurPga; // ��ǰ����(dB):
  // ���������:
  int   m_iSRow; // ��ʼ��:
  int   m_iSCol; // ��ʼ��:
  int   m_iERow; // ��ֹ��:
  int   m_iECol; // ��ֹ��:
  BYTE  m_byYMean; // �����������ƽ������:
  BOOL  m_bEnableADLI; // ʹ�ܷǴ����Բ��
  int   m_iADLIP; // �Բ�ⴥ������(��):

  BOOL  m_bIsManualCtrl;
  BOOL  m_bSaveParam;

} Eagleeye2_RMVA2xxSC_Viewer;

#endif // _EAGLEEYE2_RMVA2XXSC_VIEWER_H_
