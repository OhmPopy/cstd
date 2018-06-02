
#include "net.h"
#include "../thread.h"
#include "yuv2bmp.inl"
#include <assert.h>


//////////////////////////////////////////////////////////////////////


#define ID_LEN 24

//���֧�ֵĹ���ģ��
#define SUPPORT_NET 0x00000001 //֧��socket����
#define SUPPORT_2FRONT 0x00000002 //֧��2ǰ��
#define SUPPORT_GSOAP 0x00000004 //֧��gsoap��ʽ��web����
#define SUPPORT_ENROLL 0x00000008 //֧��ר�òɼ�ģʽ
#define SUPPORT_MAINPIC 0x00000010 //֧�ִ��������޸�
#define SUPPORT_FINGER 0x00000020 //֧��ָ��ʶ��

//˵��: M��ʾ����������0x01 - 0x80 ��S��ʾ���������0x81-0xff
#define M_USER 0x01 //�û�
#define M_VERIFY 0x02 //��֤��¼
#define M_ALARM 0x03 //������¼
#define M_OPTLOG 0x04 //��־��¼
#define M_CIMAGE 0x05 //���ע���ռ�¼
#define M_RANGE 0x06 //���伯����������
#define M_AUTH 0x07 //�ͻ��������֤
#define M_TIME 0x08 //ϵͳʱ��
#define M_HEART 0x09 //������
#define M_DEVINFO 0x0a //�豸������Ϣ
#define M_STATIS 0x0b //ͳ����Ϣ
#define M_DEVSET 0x0c //�豸��������
#define M_FORMAT 0x0d //�豸��ʽ��
#define M_DOOR_STATE 0x0e //��״̬���� ,��3��״̬��0����״̬ �� 1����״̬�� 2����״̬
#define M_REMOTE_OPEN 0x0f //Զ�̿�����,
#define M_TGROUP 0x10 //ʱ����
#define M_DSPRIGHT 0x11 //Ȩ��
#define M_UGROUP 0x12 //�û���
#define M_MCAST 0x13 //�ಥ��ַ
#define M_APPUPDATE 0x14 //�������
#define M_VIDEO_OUT 0x23 //UDP������Ƶ���� NET_ADDRESS
#define M_USER_EX 0x24 //�����û��ϴ�����
#define M_REBOOT 0x25 //�豸����
#define M_RANGE_EX 0x26 //��¼��������
#define M_SET_IP 0x27 //Զ���޸�IP��ַ
#define M_NOPEN 0x28 //N+1��������
#define M_IOCTRL 0x29 //IO����
#define M_NETPRO 0x30 //��������

//������
#define S_SET 0x81 //�ϴ�\����
#define S_DEL 0x82 //ɾ��
#define S_GET 0x83 //����\���
#define S_REALTIME_SEND 0x84 //�ɼ�ʵʱ����
#define S_REALTIME_LOAD 0x85 //ʵʱ����
#define S_REALTIME_CAP 0x86 //ʵʱ�û��ɼ�
#define S_REALTIME_CAP_INFO 0x88 //Զ�̲ɼ�ʵ�ǽ�����Ϣ
#define S_UPDATE_SEND 0x87 //����ʵʱ����
#define S_CLEAR_INOUT 0x88 //���ͳ�Ƽ���

//�ͻ�������ת������������
#define C_CNDEV 0xB0 //clientͨ����ת�������豸����
#define C_VERIFY 0xB1 //��ת��������֤client���
#define C_HEART 0xB2 //��ת��������client��������
//�ͻ�������ת������������
#define S_CN 0xc0 //��������
#define S_DN 0xc1 //�Ͽ�����

//����ִ�д���״̬
#define SUC_EXECUT 0x00 //�ɹ�
#define ERR_EXECUT 0x0A //ʧ��
#define ERR_BUSY 0x01 //�豸æ
#define ERR_LIMIT 0x02 //�Ѵ�����
#define ERR_NOFIND 0x03 //û���ҵ���Ӧ����
#define ERR_SAVE_FAIL 0x04 //���ݱ���ʧ��
#define ERR_SET_FAIL 0x05 //����ʧ��
#define ERR_VERIFY_FAIL 0x06 //��֤ʧ��
#define ERR_FROMAT  0x07 //��ʽ��ʧ��
#define ERR_PARAM   0x08 //��������
#define ERR_DISABLE 0x09 //Ҫ��ִ�еĹ���û��ʹ��
#define ERR_SUPPORT 0x10 //��֧�ֵ�����
#define ERR_INPUTDATA 0x11 //����˴�����������쳣

//���úͻ�ô���״̬
#define SET_ERR( state , err ) (state) = ( ((state) & 0xffffff00)+(err) )
#define GET_ERR( state ) ( (state) & 0xff )

#define SET_VMODE( cflag , vmode ) (cflag) = ( ((cflag)&0xff00ffff) + (((vmode)&0xff) << 16) )
#define GET_VMODE( cflag ) ( ((cflag)&0x00ff0000) >> 16 )

//GET/SET 0��3λ����
#define GET_TYPE_485( x ) ( (x)&0x0F )
#define SET_TYPE_485( x , v ) (x) = ( ((x)&0xF0) + (v) )
//GET/SET ���4��7λ����
#define GET_INDEX_485( x ) ( ( (x)&0xF0 ) >> 4 )
#define SET_INDEX_485( x , v ) ( (x) = ( ((x)&0x0F) + ((v)<<4)) )

#define CMD_STATE_REPLY 0x40000000 //��Ǹ�����Ϊ״̬�ظ�����
#define CMD_DOWN_ID 0x80000000 //��������ID����,���ȼ��ñ��
#define CMD_ID_ALL 0x00000100 //�����������ݵ����伯��
#define CMD_ID_TIME_RANGE 0x00000200 //�����ƶ�ʱ������伯��
#define CMD_ID_RANGE 0x00000400 //ȷ��ָ����������伯��

//��������
#define DL_IMAGE 0x00000100 //��Ҫ������Ƭ
#define DL_FEAT 0x00000200 //��Ҫ��������
#define DL_APPENDID 0x00000400 //ʵʱ����ָʾCID�Ǳ�Ż��ǿ���
#define LD_ENROLL 0x00000800 //��ҪԶ�̲ɼ�
#define USER_TEXT 0x00000400 //�û�������Ϣ
#define BATCH_END 0x00000800 //��������

//�û���֤
#define AUTH_VERIFY 0x00000100 //�û������֤

//��״̬: �����أ���ָ��Ϊ����״̬
#define DOOR_OPEN 0x00000100 //��
#define DOOR_CLOSE 0x00000200 //��

//ʱ��������
#define DL_NORMAL_TG 0x00000100 //��������ʱ����
#define DL_HOLIDAY_TG 0x00000200 //���ؽڼ���

//����ṹ�壬�����ϴ����ص���������,ͨ�������ݽṹ�������������
typedef struct {
  unsigned int m_MCmd; //������
  unsigned int m_SCmd; //������
  unsigned int m_CFlag; //�ض��������ò��������һ���ֽ�Ϊ����ִ��״̬��
  unsigned int m_Param1; //�û��Զ������1
  unsigned int m_Param2; //�û��Զ������2
} NCMD;

//����
typedef struct {
  int m_Year;
  char m_Month;
  char m_Day;
  char m_Reserve[2]; //�ṹ��4�ֽڶ���
} NETDATE;

//ʱ��
typedef struct {
  int m_Msec; //����
  char m_Hour;
  char m_Minute;
  char m_Second;
  char m_Reserve[1];
} NETTIME;

//���ں�ʱ��
typedef struct {
  NETDATE m_Date;
  NETTIME m_Time;
} DATETIME;

//ID��ʶ�ṹ��
typedef struct {
  char m_ID[ ID_LEN ];
} CID;

//���ָ��ʱ��ε�ID����
typedef struct {
  NETDATE m_Start; //��¼��ʼʱ��
  NETDATE m_End; //��¼����ʱ��
} TIMESE;

#define DL_OPENINTERVAL 0x00000001 //����ָ����¼��ˮ��֮���N����¼������ָ����¼����
typedef struct {
  //���ض�����,<=0 ��ʾ����m_SID֮����������м�¼,�˳�����m_SID������Ч
  int m_Count;
  //DL_OPENINTERVAL�򿪿���������,�������������
  int m_Flag;
  //����һ����ʼ��m_SID��ʾ��¼��ȷ����ˮ��,
  //��ˮ���ַ������:
  //SN(MAC��3�ֽ�) + TYPE(��¼���) + DATETIME(����) + ID(�Զ���) = (6BYTE + 1BYTE + 14BYTE + 2BYTE + \0)
  CID m_SID;
} RECORDRANGE;

//�ɱ�����ṹ�塣����Ԫ�ؾ���ʲô���ݣ���Ҫ����NCMD�趨������������
typedef struct {
  int m_Count; //����Ԫ�صĸ���
  char m_Block[4]; //�����ַ, 4��Ϊ�˽ṹ����ֽڶ���
} CARRAY;

#define WEB_LOADUSER 0x0001 //�����û�
#define WEB_WRITEUSER 0x0002 //д�û�
#define WEB_WRITEREC 0x0004 //д��¼
#define WEB_SYNCTIME 0x0008 //ͬ��ʱ��

//��������
typedef struct {
  char m_WebPro1;
  char m_WebPro2;
  char m_WebUrl1[127];
  char m_WebUrl2[127];

  char m_IPAdr[16];
  char m_MIPAdr[16];
  char m_NetMask[16];
  char m_GateWay[16];
  char m_Dns[16];
} NETPRO;

#define CHECK_TIME 0x01 //���ʱ��
#define CHECK_WEEK 0x02 //�������
#define CHECK_DAY 0x04 //�������
#define CHECK_MONTH 0x08 //����·�
#define CHECK_YEAR 0x10 //������

#define WEEK_1 0x01 //����һ��Ч
#define WEEK_2 0x02 //���ڶ���Ч
#define WEEK_3 0x04 //��������Ч
#define WEEK_4 0x08 //��������Ч
#define WEEK_5 0x10 //��������Ч
#define WEEK_6 0x20 //��������Ч
#define WEEK_7 0x40 //��������Ч

//����ʱ����Ч , ����ʱ���飬�ڼ���ʱ����
enum { ANY_TIME = -1 , TG_NORMAL = 1, TG_HOLIDY = 2 };

//ʱ����
typedef struct {
  int m_TGID; //ʱ����ID
  DATETIME m_Start; //ʱ���鿪ʼʱ��
  DATETIME m_End; //ʱ�������ʱ��
  char m_TGType; //ʱ��������, TG_NORMAL,TG_HOLIDY
  char m_CheckFlag; //ʱ����������m_CheckFlag = CHECK_TIME|CHECK_WEEK
  char m_WeekFlag; //�������ʱ�������Щ������Ч������m_WeekFlag = WEEK_1|WEEK_5
  char m_Reserve; //�����ֽ�
} DSPTIMEG;

//û��ָ���û�Ȩ��,�û�Ĭ��Ȩ��IDΪDEFAULT_NO_RIGHT����DEFAULT_SINGLE_OPEN,DSP�ϲɼ����û���
//��������Ա����ͳһ����ΪDSP_DEFAULT_SINGLE_OPEN
//Ĭ����Ȩ�ޣ�Ĭ�ϵ�һ����Ȩ�ޣ�����û�������û�����Ȩ��֧�����ʱ����
//DEFAULT_NO_RIGHT: DSP,����������û�,Ĭ��ֵ֮һ��Ȩ��
//DEFAULT_SINGLE_OPEN: DSP,����������û���Ĭ��ֵ֮����һ����Ȩ�ޣ���Ȩ��û��ʱ�������
enum { DEFAULT_NO_RIGHT = -2, DEFAULT_SINGLE_OPEN = -1, MAX_GROUPUSER = 60, TGROUP_NUM = 8 };


#define CHECK_HOLIDY 0x01 //Ȩ�޼��ڼ���
#define ACTION_LOCK 0x02 //�������
#define ACTION_OUTPUT 0x04 //�������
#define ACTION_N_1 0x08 //��Ȩ����ҪN+1
//Ȩ��
typedef struct {
  //Ȩ��ID
  int m_RightID;

  //ʱ����ID , (m_TimeGID[0]==ANY_TIME)δָ��ʱ���飬���ŷ�ʽ����ʱ�����ƣ�����ʱ�����֤�ɹ�ִ�п��ŷ�ʽ.
  int m_TimeGID[TGROUP_NUM];

  //BIT(0):1 ���ڼ���, �����ڼ���
  //BIT(1):1 ����, 1��� 0�����
  //BIT(2):1 ����, 1��� 0�����
  //BIT(3):1 N��1, ���� 0�ر�
  int m_Mode;
} DSPRIGHT;


//��ʾ�û��������κ���
#define INVALID_GROUPID -1

//�������
//�û����
//������Ͽ���: ͬһ���û�����������֤����������֤��ǿ���û�������֤��ͨ�û�
//������Ͽ���: �����Ժ���˳����֤��ֻҪ�����֤�ܽ���������֤��Ҫ����֤ͨ��.
#define G_ORDER 0x01


typedef struct {
  //��ID
  int m_GroupID;
  //BIT(0):1����0����
  char m_GroupFlag;
  //��ϳɹ����������ACTION_LOCK|ACTION_OUTPUT|ACTION_N_1
  char m_Param;
  //��ͨ�û�����Ч�û���
  char m_NormalValid;
  //ǿ���û�����Ч�û���
  char m_ForceValid;
  //��ͨ�û���ϣ����ȼ���
  CID m_NormalUsers[MAX_GROUPUSER];
  //ǿ���û���ϣ����ȼ���
  CID m_ForceUsers[MAX_GROUPUSER];
} USERGROUP;

typedef struct {
  int m_GroupID;
  int m_StrSize;
  char m_GroupFlag;
  char m_MinNum;
  char m_BLock[2];
} GROUPEX;

typedef struct {
  int m_UserType; //�ն��豸�ϵ��û�����(��ͨ���������� -- 0 , 1 , 2)
  CID m_UserID; //�û�ID
  DATETIME m_TypeUpdTime; //�û����͸���ʱ��
  DATETIME m_RegistTime; //����ע��ʱ�䣬�������ɼ�ʱ�䣬�ɼ��������²ɼ�Ҫ���´�ʱ��
  DATETIME m_LastUpdTime; //�����µ�ʱ�䣬ʶ��ʱ�����������»������²ɼ�Ҫ���´�ʱ��
} VUSER;

//�û���Ϣ
#define USER_CARD_INVALID 0x01 //����ʧ
#define USER_DLine_VALID 0x02 //ʧЧʱ����Ч
#define USER_BLACK 0x04 //������
#define USER_MIX1N 0x08 //1N������֤
#define USER_VIPCARD 0x10 //VIP�û�
#define USER_FINGER_PRINT 0x20 //ָ����֤
#define USER_SYNC_FINGEER 0x40 //ָ��ͬ��

typedef struct {
  int m_FeatBytes; //���������ֽ�����==0 ��ʾû����������
  int m_PicBytes; //ע��ͼ����ֽ�����==0 ��ʾû��ͼƬ
  int m_RightID; //�û�Ȩ��ID,���ֿ���ֵ(DEFAULT_NO_RIGHT,DEFAULT_SINGLE_OPEN,�û�ָ��)
  CID m_ImageSID; //�ϴ�ע���գ���Ӧ�ı��ID��ȫ��Ψһ
  CID m_UserID; //�û���ʶ
  CID m_Admin; //����ע�����Ա,��ʶ���û��������������ĸ�����Ա�ɼ�
  CID m_AppendID; //���Ż�����������1:1�ĸ������ȷ����Ϣ
  CID m_UserName; //�û���
  CID m_Department; //��������
  DATETIME m_DeadLine; //ʧЧʱ��
  DATETIME m_RegistTime; //����ע��ʱ�䣬�������ɼ�ʱ�䣬�ɼ��������²ɼ�Ҫ���´�ʱ��
  DATETIME m_LastUpdTime; //�����µ�ʱ�䣬ʶ��ʱ�����������»������²ɼ�Ҫ���´�ʱ��
  DATETIME m_TypeUpdTime; //�û����ͱ��ʱ��
  char m_UserFlag; //�û�״̬���
  char m_UserType; //�ն��豸�ϵ��û�����(��ͨ���������� -- 0 , 1 , 2)
  char m_Block[2]; //ע��ͼƬ,����JPG,������������m_UserIDΨһ�󶨵�.m_Block��ǰm_FeatBytes���ֽڱ�ʾ��������, ��m_PicBytes���ֽڱ�ʾͼƬ.
} USER;

//��¼����:
//0x01 - 0x20: �����¼�
//0x20 - 0x40: Ӳ�������¼�
//0x40 - 0x50: ��������
//0x50 - 0x60: �������
//0xf0 - 0xff: ��������¼�
//����: δ����
enum {
  VERIFY_SUCC = 0x01, //��֤�ɹ�
  VERIFY_FAIL = 0x02, //��֤ʧ��
  ADMIN_SUCC = 0x03, //����Ա��֤�ɹ�
  EMER_OPEN = 0x04, //��������
  RIGHT_OPEN = 0x05, //Ȩ�޿���
  GROUP_OPEN = 0x06, //��Ͽ���
  BUTTON_OPEN = 0x07, //��ť����

  ALARM_HD_MANGET_TIMEOUT = 0x20, //�Ŵų�ʱ
  ALARM_HD_MANGET_ILLOPEN = 0x21, //�ŴŷǷ�����
  ALARM_HD_OFFLINE = 0x22, //ǰ�˵��߱���
  ALARM_HD_BREAK = 0x30, //���𱨾�
  ALARM_HD_SHOCK = 0x31, //�𶯱���
  ALARM_HD_FPOWR = 0x36, //ǰ�˹����쳣����
  ALARM_HD_UPS_ON = 0x37, //UPS���õ�ؿ���
  ALARM_HD_UPS_OFF = 0x38, //UPS���õ�عر�

  ALARM_HD_ASSIST = 0x40, //�������봥��
  ALARM_HD_WL_DOOR_MAGNETIC = 0x41, //����
  ALARM_HD_WL_SMOKE = 0x42,
  ALARM_HD_WL_GAS = 0x43,
  ALARM_HD_WL_BUTTON1 = 0x44,
  ALARM_HD_WL_BUTTON2 = 0x45,
  ALARM_HD_WL_BUTTON3 = 0x46,
  ALARM_HD_WL_BUTTON4 = 0x47,
  ALARM_HD_WL_AREA1 = 0x48,
  ALARM_HD_WL_AREA2 = 0x49,
  ALARM_HD_WL_AREA3 = 0x4A,
  ALARM_HD_WL_AREA4 = 0x4B,
  ALARM_HD_WL_AREA5 = 0x4C,
  ALARM_HD_WL_AREA6 = 0x4D,
  ALARM_HD_WL_AREA7 = 0x4E,

  ALARM_AB_RULE = 0xE0,

  ALARM_SF_BACKLIST = 0xF0, //��������֤����
  ALARM_SF_ILLCARD = 0xF1, //��Ч��(��ʧ)
  ALARM_SF_ILLTIME = 0xF2, //�Ƿ�ʱ��ʶ��
  ALARM_SF_DEADLINE = 0xF3, //ʧЧʱ��
  ALARM_SF_DANGER_OPEN = 0xF4, //в�ȿ���
  ALARM_SF_SUPER_OPEN = 0xF5, //�������뿪��
  LINK_EVENT = 0xF6, //�����¼�
};

//��¼��Ϣ,ʶ���¼��������¼
typedef struct {
  int m_RecType; //��¼����
  int m_Score; //ʶ��÷� , �����¼�Ϊ�����¼���LINKID
  int m_PicBytes; //��¼ͼ����ֽ�����==0 ��ʾû�м�¼ͼƬ
  CID m_ChangID; //ע���ռ�¼��
  CID m_UserID; //�û�ID,���Ϊ�գ���ʾ�Ƿ���¼ , �����¼�Ϊ�¼�����Text
  CID m_SerialID; //ʶ���¼��ˮ��ID
  CID m_AppendID; //���Ż�����������1:1�ĸ������ȷ����Ϣ
  CID m_UserName; //�û���
  CID m_Department; //��������
  DATETIME m_RecTime; //��¼ʱ��
  char m_VerifyMode; //��֤ģʽ,��:VERIFY_CARD|VERIFY_FACE_1N,ˢ��+����
  char m_Source; //�¼�Դ�豸��485��ַ
  char m_Block[2]; //��¼ͼ��,����JPG , 2��Ϊ�˽ṹ����ֽڶ���
} RECORD;

//������־����
enum {
  DSPLOG_ADDUSER = 0x01, //�����û�
  DSPLOG_DELUSER = 0x02, //ɾ���û�
  DSPLOG_REREGIST = 0x03, //���²ɼ�
  DSPLOG_CAHNGETYPE = 0x04, //�ı��û�����
  DSPLOG_UDISK_ULUSER = 0x05, //U���ϴ��û�
  DSPLOG_UDISK_DLUSER = 0x06, //U�������û�
  DSPLOG_UDISK_DLRECORD = 0x07 , //U�����ؼ�¼
  DSPLOG_UDISK_UPDATEAPP = 0x08, //U�̸��³���
};

//�豸������־
typedef struct {
  int m_LogType; //��־����
  CID m_SerialID; //��ˮ��ID
  CID m_Admin; //����Ա
  CID m_BeOptUser; //������Ա
  DATETIME m_RecTime; //��¼ʱ��
} DSPLOG;

//���ע����
typedef struct {
  int m_PicBytes; //ע���մ�С
  CID m_UserID; //�û�ID
  CID m_SerialID; //��ˮ��ID
  CID m_AppendID; //���Ż�����������1:1�ĸ������ȷ����Ϣ
  CID m_UserName; //�û���
  CID m_Department; //��������
  CID m_Admin; //����ע�����Ա,��ʶ���û��������������ĸ�����Ա�ɼ�
  DATETIME m_RecTime; //��¼ʱ��
  char m_Block[4]; //ע�����ڴ�
} CHANGEIMAGE;

//DSP���ͳ����Ϣ
typedef struct {
  int m_TotalUsers; //��ǰ�豸�����û���
  int m_NoFeatUser; //û�вɼ������������û���
  int m_TotalDays; //ʶ���¼����������
  int m_TotalRecords; //�ܼ�¼��
  int m_TotalAlarm; //�ܱ�����¼��
  int m_TotalDspLog; //�ܲ�����־��Ŀ
  int m_TotalChangeImage; //�ܱ��ע����
} DEVSTATIS;

//��֤��Ϣ
typedef struct {
  CID m_UserName; //�û���
  CID m_Password; //����
} AUTH;

//�豸������Ϣ
typedef struct {
  int m_DevType; //�豸����
  int m_LimitUser; //����������
  int m_Limit1NUser; //1N�û�����
  CID m_SN; //�豸���
  CID m_Ver; //DSP����汾
  CID m_Space; //����������Ϣ
} DEVINFO;

//Զ�̲ɼ�������Ϣ
typedef struct {
  int m_Total; //�ɼ�����
  int m_Count; //��ǰ�ɼ���
  CID m_UserID; //�û��ǼǱ��
} RCAPINFO;

//2.0��֧�ֵĹ���
#define RECORD_SAVEFAIL 0x00000001 //�Ƿ񱣴�ʶ��ʧ�ܼ�¼
#define SUPER_PASSWORD 0x00000002 //�������뿪����Ч
#define HDBEEP_OPEN 0x00000004 //����/����(Ӳ����������������)
#define REALTIME_RECORD 0x00000010 //ʵʱ��̬��¼��ʾ
#define REALTIME_USERLOAD 0x00000020 //�����Ż��߿���ʶ��ʱ������޷��ڵ�ǰdsp�л����������,����������״̬�£����ӹ����ʵʱ������������
#define REALTIME_USERSEND 0x00000040 //�ɼ��û�ͬ��
#define DOORMANGET_OPEN 0x00000080 //�����Ŵż�ⱨ��
#define DOORFORCE_OPEN 0x00000100 //����в�ȿ��Ź���
#define REMOTE_CAP_SAVE 0x00000200 //Զ�̿��Ʋɼ����û��Ƿ񱣴浽DSP
#define GPRS_OPEN 0x00000400 //����GPRSģ��
#define UPDATE_USERSEND 0x00000800 //��������ʱ���Ƿ�ʵʱ�����û�����
#define REALTIME_USERLOAD_PRIOR 0x00001000 //������������£������������
#define AB_LOCK 0x00002000 //AB����
#define DOOR1_NOPEN 0x00004000 //��һN+1
#define DOOR2_NOPEN 0x00008000 //�Ŷ�N+1
#define ENABLE_COUNT 0x00010000 //���߼�¼��ͳ��
#define ENCRYP_DISPALY 0x00020000 //���߼�¼��ͳ��

//��֤ģʽ���ȼ���-��:UID<CARD<11<MIX<1N
#define VERIFY_USERID 0x01 //���
#define VERIFY_CARD 0x02 //ˢ��
#define VERIFY_FACE_11 0x04 //11��������
#define VERIFY_FACE_MIX 0x08 //�����������
#define VERIFY_FACE_1N 0x10 //1N��������
#define VERIFY_FINGER 0x20 //ָ����֤

#define OUTPUT_WG_YES 0x01 //���ΰ��Э��
#define OUTPUT_WG_34 0x02 //ָ�����ΰ��34����ָ�����ΰ��26
#define OUTPUT_WG_CTX 0x04 //ΰ�����ָ������
#define OUTPUT_WG_UID 0x08 //�����OUTPUT_WG_CTX������û�ID��Ĭ���������

#define WG_OLD 0
#define WG_26 1
#define WG_34 2
#define WG_35 3
#define WG_36 4
#define WG_37 5
#define WG_44 6
#define WG_45 7

//�豸������������
typedef struct {
  //�豸ʱ����,(m_TimeGID[0]==ANY_TIME)δָ��ʱ����,�豸����ʱ����Ч.�豸ʱ�������ȼ������û�Ȩ��ʱ����
  int m_DevCtx[ TGROUP_NUM ];
  //�豸Ĭ��Ȩ��: DEFAULT_NO_RIGHT , DEFAULT_SINGLE_OPEN
  int m_DefaultRight;
  //�μ�2.0�汾����ı��
  int m_BaseSet;
  //��������, ������һ��������¼�����һ�ץ�Ĳ�ɫȫ��ͼ.
  CID m_SuperPassword;
  //�Ŵ���ʱʱ�䣬��λ��
  char m_DoorMangetTime;
  //��������ʱ�䣬��λ��
  char m_LockTime;
  //�û������֤ģʽ,����:VERIFY_USERID|VERIFY_CARD|VERIFY_FACE_11
  char m_VerifyMode;
  //BIT(0) : 0�������չΤ���ź�,1���.
  //BIT(1) : 0���Τ��26,1���Τ��34.
  //BIT(2) : 0����BIT(4)��� , 1���m_OutputCtx��ָ������
  //BIT(4) : 0ֱ���������,1������
  //BIT(4-7): WG���Э��,0-wg26 1-wg34 2-wg35 3-wg36 4-wg37 5-wg44 6-wg45
  char m_OutputWG;
  //Τ��Э���������
  int m_OutputCtx;
} DEVSET;

//������½ṹ��
typedef struct {
  int m_Bytes; //�ļ���С
  CID m_FileName; //�ļ���
  char m_TotalFiles; //��Ҫ���µ��ļ�����
  char m_FileIndex; //��ǰ���µ��ڼ���
  char m_Block[2]; //�ļ�����
} APPUPDATE;

//IO�豸״̬
#define IO_MODE_NORMAL 0x00 //����״̬
#define IO_MODE_OPEN 0x01 //����״̬
#define IO_MODE_CLOSE 0x02 //����״̬
//�Ŵ�״̬
#define IO_STATE_CLOSE 0x00 //�ŴŹ�
#define IO_STATE_OPEN 0x01 //�Ŵſ�
//ִ�ж���
#define ACT_IO_OPEN 0x02 //ִ�д򿪶���
#define ACT_IO_OPENEX 0x04 //ִ�и�������
#define ACT_IO_CLOSE 0x10 //ִ�йرն���
#define ACT_MODE_SET 0x20 //����IO�豸����ģʽ
#define ACT_MODE_GET 0x40 //���IO�豸����ģʽ
#define ACT_STATE_GET 0x80 //���IO�豸��ǰ״̬

typedef struct {
  //IO�豸485��ַ
  char m_Source;
  //ִ�ж���
  char m_Action;
  //IO�豸��ǰģʽ
  char m_IOMode;
  //IO�豸��ǰ״̬״̬
  char m_IOState;
} IOCTRL;

//NOPEN:Զ���������˿���
typedef struct {
  //m_Users==1Ȩ��ID, m_Users>1���ID
  int m_CtxID;
  //��֤�û���Ŀ�� 0���뿪�ţ�1Ȩ�޿��ţ�>1��Ͽ���
  int m_Users;
  //������Ϣ
  IOCTRL m_XOpen;
  //��֤�û�����
  CID m_IDS[MAX_GROUPUSER * 2];
} NOPEN;

//����485��ַ����:
//1.485��ַ��һ���ֽڱ�ʾ��[4-7]��ʾ�豸������1-15 , [0-3]��ʾ֧��16�ֲ�ͬ�͵��豸
// �Ѿ�������豸���� 0--����ǰ�� , 1--������ ,
// ����: 0x11 ��ʾ1��������ʶ��ǰ��1�� INDEX[4-7] == 1 TYPE[0-3] == 1
// 0x10 ��ʾ1��������ʶ��ǰ��0��INDEX[4-7] == 1 TYPE[0-3] == 0
// 0x12 ��ʾ1����ˢ����0 INDEX[4-7] == 1 TYPE[0-3] == 2
// 0x13 ��ʾ1����ˢ����1 INDEX[4-7] == 1 TYPE[0-3] == 3
// 0x1F ��ʾ�ŵ㱾�� INDEX[4-7] == 1 TYPE[0-3] == F
// 0x14 ��ʾ��������1 INDEX[4-7] == 1 TYPE[0-3] == 4
// 0x25 ��ʾ�������2 INDEX[4-7] == 2 TYPE[0-3] == 5
//2. �����豸����Ŵ�1��ʼ����Э���֧��15���ţ�16�в�ͬ���͵��ŵ������豸��15�����������15���������
//3. [0-7] == 0x00 ��ʾ�Ƿ���ַ��[0-7] ���� 0x01��ʾ��˰屾��
//7_______3______0
//|_INDEX_|__type__|

#define DEV_TYPE_FACE1  0x0 //����ǰ��1
#define DEV_TYPE_FACE2  0x1 //����ǰ��2
#define DEV_TYPE_CARD1  0x2 //ˢ����1
#define DEV_TYPE_CARD2  0x3 //ˢ����2
#define DEV_TYPE_IN     0x4 //��������
#define DEV_TYPE_OUT    0x5 //�������
#define DEV_TYPE_BUTTON 0x6 //���Ű�ť
#define DEV_TYPE_DOOR   0xF //�ŵ㱾��

#define IMAGE_UYVY  0x01
#define IMAGE_YUYV  0x02
#define IMAGE_RGB24 0x03

typedef struct {
  INT32 m_Size; //ͼ�������ܴ�СBYTES
  INT32 m_W; //ͼ���
  INT32 m_H; //ͼ���
  INT32 m_Step; //�����ݲ���
  char m_Type; //ͼ������
  char m_Block[3]; //ͼ������
} DEVIMAGE;


#define ALIGN4( size )     ( ((size)+3) & ~3 )
#define USER_STEP( obj )   ALIGN4( sizeof(USER)+ (obj).m_FeatBytes+(obj).m_PicBytes )
#define RECORD_STEP( obj ) ALIGN4( sizeof(RECORD)+(obj).m_PicBytes-2 )
#define CGIMG_STEP( obj )  ALIGN4( sizeof(CHANGEIMAGE)+(obj).m_PicBytes-4 )
#define DSPLOG_STEP( obj ) ALIGN4( sizeof(DSPLOG) )



#ifndef BOOL
typedef int BOOL;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define DEV_REGION_ADDR ("224.0.1.100") //Ĭ�������ַ,�û��豸�Զ��Զ����� 
#define DEV_ID_LEN 24 //ID����
#define DEV_TIMEGROUP_NUMS 8 //ʱ�������
#define DEV_USER_COMBINS 6 //�û���ϸ���

#define DL_DEV_PIC 0x01 //��Ƭ
#define DL_DEV_USER_FEAT 0x02 //��������
#define DL_DEV_USER_TEXT 0x04 //�û�������Ϣ

#define DEV_CHECK_TIME 0x01 //���ʱ��
#define DEV_CHECK_WEEK 0x02 //�������
#define DEV_CHECK_DAY 0x04 //�������
#define DEV_CHECK_MONTH 0x08 //����·�
#define DEV_CHECK_YEAR 0x10 //������

#define DEV_WEEK_1 0x01 //����һ��Ч
#define DEV_WEEK_2 0x02 //���ڶ���Ч
#define DEV_WEEK_3 0x04 //��������Ч
#define DEV_WEEK_4 0x08 //��������Ч
#define DEV_WEEK_5 0x10 //��������Ч
#define DEV_WEEK_6 0x20 //��������Ч
#define DEV_WEEK_7 0x40 //��������Ч

#define DEV_RECORD_SAVEFAIL 0x00000001 //�Ƿ񱣴�ʶ��ʧ�ܼ�¼
#define DEV_SUPER_PASSWORD 0x00000002 //�������뿪����Ч
#define DEV_HDBEEP_OPEN 0x00000004 //����/����
#define DEV_REALTIME_RECORD 0x00000010 //ʵʱ��̬��¼����
#define DEV_REALTIME_USERLOAD 0x00000020 //�����Ż��߿���ʶ��ʱ������޷��ڵ�ǰdsp�л����������,����������״̬�£����ӹ����ʵʱ������������
#define DEV_REALTIME_USERSEND 0x00000040 //ͨ���豸�ɼ����û�ʵʱ���͵������ն�
#define DEV_DOORMANGET_OPEN 0x00000080 //�����Ŵż�ⱨ��
#define DEV_DOORFORCE_OPEN 0x00000100 //����в�ȿ��Ź���
#define DEV_REMOTE_CAP_SAVE 0x00000200 //Զ�̲ɼ����û��Ƿ񱣴浽DSP
#define DEV_GPRS_OPEN 0x00000400 //����GPRSģ��
#define DEV_UPDATE_USERSEND 0x00000800 //��������ʱ���Ƿ�ʵʱ�����û�����
#define DEV_AB_LOCK 0x00002000 //AB����
#define DEV_DOOR1_NOPEN 0x00004000 //��һN+1
#define DEV_DOOR2_NOPEN 0x00008000 //�Ŷ�N+1

//��֤ģʽ���ȼ���-��:UID->CARD->11->MIX->1N
#define DEV_VERIFY_USERID 0x01 //����
#define DEV_VERIFY_CARD 0x02 //ˢ��
#define DEV_VERIFY_FACE_11 0x04 //11��������
#define DEV_VERIFY_FACE_MIX 0x08 //�����������
#define DEV_VERIFY_FACE_1N 0x10 //1N��������
//�û�״̬��Ϣ
#define DEV_USER_CARD_INVALID 0x01 //����ʧ
#define DEV_USER_DLine_INVALID 0x02 //ʧЧʱ����Ч
#define DEV_USER_BLACK 0x04 //������
#define DEV_USER_MIX1N 0x08 //���ģʽ��1N�û�
//SDK�Զ���
#define DEV_USER_FEAT_BASE64 0x40 //��������base64����
#define DEV_USER_IMAGE_BASE64 0x80 //ע���վ���base64����

typedef enum {
  DEV_CONNECT_CUT = 0x00, //�Ͽ�����
  DEV_CONNECT_SUCCESS, //��������
  DEV_CONNECT_FAILUE , //����ʧ��
  DEV_CONNECT_NONE //δ��������
} DEV_CONNECT_STATUS;

typedef enum {
  DEV_OPER_UPLOAD = 0x01, //�ϴ�
  DEV_OPER_DOWNLOAD = 0x02, //����
  DEV_OPER_DELETE = 0x04 //ɾ��
} DEV_OPER_FLAG;

typedef enum {
  DEV_AUTH_OPER = 0x05, //�豸��֤����
  DEV_AUTH_SET_OPER, //�豸��֤�û����ò���
  DEV_REGION_OPER , //�豸�������
  DEV_FORMAT_OPER, //�豸��ʽ������
  DEV_UPDATE_OPER, //�豸������²���
  DEV_SYS_TIME_OPER , //�豸ϵͳʱ�����
  DEV_BASEINFO_OPER, //������Ϣ����
  DEV_STATISINFO_OPER, //ͳ����Ϣ����
  DEV_WORKATT_OPER, //�������Բ���
  DEV_USER_OPER , //�û�����
  DEV_USER_RT_OPER, //�û�ʵʱ���ز���
  DEV_USER_RANGE_OPER, //�û���������������������
  DEV_RECORD_OPER , //��¼����
  DEV_RECORD_RANGE_OPER, //��¼��������������������
  DEV_ALARM_OPER,
  DEV_ALARM_RANGE_OPER,
  DEV_LOG_OPER, //��־����
  DEV_LOG_RANGE_OPER, //��־��������������������
  DEV_CHANGEIMAGE_OPER, //���ע���ղ���
  DEV_CIMAGE_RANGE_OPER, //���ע������������������������
  DEV_TIME_GROUP_OPER, //ʱ�������
  DEV_RIGHT_OPER, //Ȩ�޲���
  DEV_USERGROUP_OPER, //�û������
  DEV_DOOR_STATE_OPER, //����״̬����
  DEV_REMOTE_OPEN_DOOR, //Զ�̿��Ų���
  DEV_VIDEO_TRANSFER, //��Ƶ����
  DEV_USER_EX_OPER, //�û���������
  DEV_RECORD_EX_OPER, //��¼��������
  DEV_LOG_EX_OPER, //��־��������
  DEV_CIMAGE_EX_OPER, //���ע������������
  DEV_REBOOT_OPER, //�豸����
  DEV_USER_REMOTE_CAP, //�û�Զ�̲ɼ�
  DEV_NOPEN_OPER, //N+1����
  DEV_IOCTRL_OPER, //IOCTRL������Ϣ
  CLI_AUTH_OPER, //client����ת��������֤����
  DEV_NETPRO_OPER, //�豸������������
} DEV_OPER_TYPE;

typedef enum {
  OPER_SUCCESS = 0x00, //�ɹ�
  OPER_ERR_BUSY = 0x01, //�豸æ
  OPER_ERR_LIMIT = 0x02, //�Ѵ�����
  OPER_ERR_NOFIND = 0x03, //û���ҵ���Ӧ����
  OPER_ERR_SAVEFAIL = 0x04, //���ݱ���ʧ��
  OPER_ERR_SETFAIL = 0x05, //����ʧ��
  OPER_ERR_FROMAT = 0x07, //��ʽ��ʧ��
  OPER_ERR_PARAM = 0x08, //��������
  OPER_ERR_DISABLE = 0x09, //Ҫ��ִ�еĹ���û��ʹ��
  OPER_ERR_EXECUT = 0x0A, //ʧ��
  OPER_ERR_SUPPORT = 0x10, //��֧�ֵ�����
  OPER_ERR_INPUTDATA = 0x11, //����˴�����������쳣
  OPER_BATCH_DL_COMPLETE = 0x1F //�����������
} DEV_OPER_RESULT; //�������


typedef enum {
  DEV_NORMAL = 0x01, //����ʱ��
  DEV_HOLIDY = 0x02, //�ڼ���
  DEV_ANY_TIME = 0x03 //����ʱ��
} DEV_TIMEGROUP_TYPE; //ʱ��������

typedef enum {
  DEV_USER_NOMAL = 0, //��ͨ
  DEV_USER_ADMIN = 1, //����Ա
  DEV_USER_SUPERADMIN = 2 //��������Ա
} DEV_USER_TYPE; //�û�����

typedef enum {
  DEV_DEFAULT_NO_RIGHT = -2, //��Ȩ��
  DEV_DEFAULT_SINGLE_OPEN = -1 //��һ����Ȩ��
} DEV_DEFAULT_RIGHT; //Ĭ��Ȩ��

//��¼����:
//0x01 - 0x20: �ճ���¼
//0x20 - 0xFF: ������¼
typedef enum {
  DEV_VERIFY_SUCC = 0x01, //��֤�ɹ�
  DEV_VERIFY_FAIL = 0x02, //��֤ʧ��
  DEV_ADMIN_SUCC = 0x03, //����Ա��֤�ɹ�
  DEV_EMER_OPEN = 0x04, //��������
  DEV_RIGHT_OPEN = 0x05, //Ȩ�޿���
  DEV_GROUP_OPEN = 0x06, //��Ͽ���
  DEV_BUTTON_OPEN = 0x07, //��ť����
  DEV_ALARM_HD_MANGET_TIMEOUT = 0x20, //�Ŵų�ʱ
  DEV_ALARM_HD_MANGET_ILLOPEN = 0x21, //�ŴŷǷ�����
  DEV_ALARM_HD_OFFLINE = 0x22, //ǰ�˵��߱���
  DEV_ALARM_HD_BREAK = 0x30, //���𱨾�
  DEV_ALARM_HD_SHOCK = 0x31, //�𶯱���
  DEV_ALARM_HD_FPOWR = 0x36, //ǰ�˹����쳣����
  DEV_ALARM_HD_UPS_ON = 0x37, //UPS���õ�ؿ���
  DEV_ALARM_HD_UPS_OFF = 0x38, //UPS���õ�عر�
  DEV_ALARM_HD_ASSIST = 0x40, //�������봥��
  DEV_ALARM_SF_BACKLIST = 0xF0, //��������֤����
  DEV_ALARM_SF_ILLCARD = 0xF1, //��Ч��(��ʧ)
  DEV_ALARM_SF_ILLTIME = 0xF2, //�Ƿ�ʱ��ʶ��
  DEV_ALARM_SF_DEADLINE = 0xF3, //ʧЧʱ��
  DEV_ALARM_SF_DANGER_OPEN = 0xF4, //в�ȿ���
  DEV_ALARM_SF_SUPER_OPEN = 0xF5 //�������뿪��
} DEV_REC_TYPE; //��¼����

typedef enum {
  DEV_DOOR_SOURCE = 0x01, //�������Դ
  DEV_SIGNAL_SOURCE, //�ź�������������Դ
  DEV_NO_NUM_DEV_SOURCE //�ޱ���豸��Դ
} DEV_REC_SOURCE_TYPE;

typedef enum {
  DEV_LOG_ADDUSER = 0x01, //�����û�
  DEV_LOG_DELUSER = 0x02, //ɾ���û�
  DEV_LOG_REREGIST = 0x03, //���²ɼ�
  DEV_LOG_CAHNGETYPE = 0x04, //�ı��û�����
  DEV_UDISK_ULUSER = 0x05, //U���ϴ��û�
  DEV_UDISK_DLUSER = 0x06, //U�������û�
  DEV_UDISK_DLRECORD = 0x07, //U�����ؼ�¼
  DEV_UDISK_UPDATEAPP = 0x08 //U�̸��³���
} DEV_LOG_TYPE;

typedef enum {
  DEV_DOOR_NOMAL = 0x00000000, //����״̬
  DEV_DOOR_OPEN = 0x00000100, //��״̬
  DEV_DOOR_CLOSE = 0x00000200 //��״̬
} DEV_DOOR_STATE;

typedef struct {
  char m_ID[ DEV_ID_LEN ];
} DEV_CID; //ID��ʶ�ṹ��

typedef struct {
  int m_Year;
  char m_Month;
  char m_Day;
} DEV_DATE; //����

typedef struct {
  int m_Msec; //����
  char m_Hour;
  char m_Minute;
  char m_Second;
} DEV_TIME; //ʱ��

typedef struct {
  DEV_DATE m_Date;
  DEV_TIME m_Time;
} DEV_DATETIME; //�豸���ں�ʱ��

typedef struct {
  int m_DevType; //�豸����
  int m_LimitUser; //��ʹ����������
  int m_Limit1NUser; //1N�û�����
  DEV_CID m_SN; //�豸���
  DEV_CID m_Ver; //DSP����汾
  DEV_CID m_Space; //����������Ϣ
} DEV_BASEINFO; //�豸������Ϣ

typedef struct {
  int m_TotalUsers; //��ǰ�豸�����û���
  int m_NoFeatUser; //û�вɼ������������û���
  int m_TotalDays; //ʶ���¼����������
  int m_TotalRecords; //�ܼ�¼��
  int m_TotalAlarm; //�ܱ�����¼��
  int m_TotalDspLog; //�ܲ�����־��Ŀ
  int m_TotalChangeImage; //�ܱ��ע����
} DEV_STATIS; //�豸ͳ����Ϣ

typedef struct {
  int m_TimeGID[DEV_TIMEGROUP_NUMS]; //ʱ����ID
  int m_BaseSet; //�μ��豸���ܱ�Ƕ���
  int m_DoorMangetTime; //�Ŵ���ʱʱ�䣬��λ��
  int m_LockTime; //��������ʱ�䣬��λ��
  int m_VerifyMode; //�û������֤ģʽ,����:DEV_VERIFY_USERID|DEV_VERIFY_CARD|DEV_VERIFY_FACE_11
  int m_nWGType; //Τ��Э������(0���Τ��26,1���Τ��34)
  int m_nWGOutType; //Τ���������(0���ID��1���WG����)
  int m_nWGOutIDType; //���ID����(0�������,1����û�ID)
  int m_nWGOutContent; //WG�������
  BOOL m_bWGOutPut; //�Ƿ�WG���
  DEV_CID m_szSuperPWD; //��������
  DEV_DEFAULT_RIGHT m_DefaultRight; //�豸Ĭ��Ȩ��
} DEV_WORKATT; // �豸������Ϣ

typedef struct {
  DEV_USER_TYPE m_UserType; //�ն��豸�ϵ��û�����(��ͨ���������� -- 0 , 1 , 2)
  DEV_CID m_UserID; //�û�ID
  DEV_DATETIME m_TypeTime; //�û����͸���ʱ��
  DEV_DATETIME m_RegistTime; //����ע��ʱ�䣬�������ɼ�ʱ�䣬�ɼ��������²ɼ�Ҫ���´�ʱ��
  DEV_DATETIME m_LastUpdTime; //�����µ�ʱ�䣬ʶ��ʱ�����������»������²ɼ�Ҫ���´�ʱ��
} DEV_VUSER;

typedef struct {
  int m_FeatLen; //������С
  int m_PicLen; //��Ƭ��С
  int m_RightID; //�û�Ȩ��ID
  DEV_CID m_ImageSID; //�ϴ�ע���գ���Ӧ�ı��ID��ȫ��Ψһ
  DEV_CID m_UserID; //�û�ΨһID,�ؼ���
  DEV_CID m_Admin; //����ע�����Ա
  DEV_CID m_AppendID; //���Ż�����������1:1�ĸ������ȷ����Ϣ
  DEV_CID m_UserName; //�û���
  DEV_CID m_Department; //��������
  DEV_DATETIME m_DeadLineTime; //ʧЧʱ��
  DEV_DATETIME m_RegistTime;  //����ע��ʱ��
  DEV_DATETIME m_LastUpdTime; //�����µ�ʱ��
  DEV_DATETIME m_TypeUpdTime; //�û����ͱ��ʱ��
  char m_UserFlag; //�û�״̬���
  DEV_USER_TYPE m_UserType; //�ն��豸�ϵ��û�����
  char* m_FeatData; //��������
  char* m_PicData; //��Ƭ����
  int m_bRTUser; //�Ƿ�Ϊʵʱ�û�(0 ��ʵʱ�û� 1 ʵʱ�ɼ��û� 2 ʵʱ���������û�)
} DEV_USER; //�û���Ϣ

typedef struct {
  int m_Count; //���ض�����
  DEV_VUSER* m_pArray; //ID����
} DEV_DL_USER_RANGE; //�û����ؼ���

//���ڼ�¼���¼���Դ485��ַ����:
//1.485��ַ��һ���ֽڱ�ʾ����4λ[4-7]��ʾ�豸������1-15 , ��4λ[0-3]��ʾ֧��16�ֲ�ͬ�͵��豸
// #define DEV_TYPE_FACE1 0x0 //����ǰ��1
// #define DEV_TYPE_FACE2 0x1 //����ǰ��2
// #define DEV_TYPE_CARD1 0x2 //ˢ����1
// #define DEV_TYPE_CARD2 0x3 //ˢ����2
// #define DEV_TYPE_IN 0x4 //��������
// #define DEV_TYPE_OUT 0x5 //�������
// #define DEV_TYPE_INWL 0x6 //��������
// #define DEV_TYPE_DOOR 0xF //�ŵ㱾��
// ����: 0x11 ��ʾ1��������ʶ��ǰ��1��INDEX[4-7] == 1 TYPE[0-3] == 1
// 0x10 ��ʾ1��������ʶ��ǰ��0��INDEX[4-7] == 1 TYPE[0-3] == 0
// 0x12 ��ʾ1����ˢ����0 INDEX[4-7] == 1 TYPE[0-3] == 2
// 0x13 ��ʾ1����ˢ����1 INDEX[4-7] == 1 TYPE[0-3] == 3
// 0x1F ��ʾ�ŵ㱾�� INDEX[4-7] == 1 TYPE[0-3] == F
// 0x14 ��ʾ��������1 INDEX[4-7] == 1 TYPE[0-3] == 4
// 0x25 ��ʾ�������2 INDEX[4-7] == 2 TYPE[0-3] == 5
//2. �����豸����Ŵ�1��ʼ����Э���֧��15���ţ�16�в�ͬ���͵��ŵ������豸��15�����������15���������
//3. ��������ĵ�ַ: [0-7] == 0x00 ��ʾ�Ƿ���ַ��[0-7] ���� 0x01��ʾ��˰屾��, [0-7] ���� 0x02��ʾGPRSģ��
//7_______3________0
//|_INDEX_|__type__|

typedef struct {
  int m_LoadFlag; //DL_DEV_PIC|DL_DEV_USER_FEAT=��Ҫ����ע����Ƭ����������
  int m_ReqSource; //������Դ,���485��ַ����
  BOOL m_bApeendID; //TRUE=����, FALSE=����
  DEV_CID m_ID; //����ID
} DEV_USER_REAL_LOAD; //ʵʱ�û�����

typedef struct {
  int m_RecType; //��¼����
  int m_Source; //��¼��Դ(�Ѿ���ʱ,����ʹ��)
  BOOL m_bRealTime; //�Ƿ�Ϊʵʱ��¼
  int m_Score; //ʶ��÷�
  int m_PicLen; //��Ƭ��С
  int m_VerifyMode; //��֤ģʽ,����:DEV_VERIFY_USERID|DEV_VERIFY_CARD|DEV_VERIFY_FACE_11
  DEV_CID m_ChangID; //�����¼��
  DEV_CID m_SerialID; //ʶ���¼��ˮ��ID
  DEV_CID m_UserID; //�û�ID,���Ϊ�գ���ʾ�Ƿ���¼
  DEV_CID m_AppendID; //���Ż�����������1:1�ĸ������ȷ����Ϣ
  DEV_CID m_UserName; //�û���
  DEV_CID m_Department; //��������
  DEV_DATETIME m_RecTime; //��¼ʱ��
  char m_ConcretSource; //��¼��Դ485��ַ�����485��ַ����
  char* m_PicData; //ԭʼJPGͼ������(δ��base64����)
} DEV_RECORD; //��¼��Ϣ

typedef struct {
  BOOL m_bRTLog; //�Ƿ�Ϊʵʱ������־
  DEV_LOG_TYPE m_LogType; //��־����
  DEV_CID m_SerialID; //��ˮ��ID
  DEV_CID m_Admin; //����Ա
  DEV_CID m_BeOptUser; //������Ա
  DEV_DATETIME m_RecTime; //��¼ʱ��
} DEV_LOG;

typedef struct { //���ע����
  int m_PicBytes; //ע���մ�С
  BOOL m_bRTChangeImage; //�Ƿ�Ϊʵʱ���ע����
  DEV_CID m_UserID; //�û�ID
  DEV_CID m_SerialID; //��ˮ��ID
  DEV_CID m_AppendID; //���Ż�����������1:1�ĸ������ȷ����Ϣ
  DEV_CID m_UserName; //�û���
  DEV_CID m_Department; //��������
  DEV_CID m_Admin; //����ע�����Ա,��ʶ���û��������������ĸ�����Ա�ɼ�
  DEV_DATETIME m_RecTime; //��¼ʱ��
  char* m_PicData; //JPGͼ������(δ��base64����)
} DEV_CHANGEIMAGE;

typedef struct {
  int m_Count; //���ض�����
  BOOL m_bOpenRange; //�Ƿ�Ϊ������(true=�ǣ� false=��)
  DEV_CID m_SID; //����һ����ʼ��m_SID��ʾ��¼��ȷ����ˮ�� SID��ɣ�SN + TYPE + DATE + ID = ( 6BYTE + 1BYTE + 8BYTE + 4BYTE + \0)
} DEV_DL_RECORD_RANGE; //��¼��������

typedef struct {
  int m_Count;
  DEV_DL_RECORD_RANGE* m_pRange;
} DEV_DL_RECORD_ARRAY; //��¼�������伯��

typedef struct {
  int m_TGID; //ʱ����ID
  DEV_DATETIME m_Start; //ʱ���鿪ʼʱ��
  DEV_DATETIME m_End; //ʱ�������ʱ��
  DEV_TIMEGROUP_TYPE m_TGType; //ʱ��������
  char m_CheckFlag; //ʱ����������m_CheckFlag = CHECK_TIME|CHECK_WEEK
  char m_WeekFlag; //�������ʱ�������Щ������Ч������m_WeekFlag = WEEK_1|WEEK_5
} DEV_TIMEGROUP; //ʱ����

typedef struct {
  int m_nCount;
  DEV_TIMEGROUP* m_pTGArray;
} DEV_TIMEGROUP_ARRAY; //ʱ��������

typedef struct {
  int m_RightID; //Ȩ��ID
  int m_TimeGID[DEV_TIMEGROUP_NUMS]; //ʱ����ID , (m_TimeGID[0]==ANY_TIME)δָ��ʱ���飬���ŷ�ʽ����ʱ�����ƣ�����ʱ�����֤�ɹ�ִ�п��ŷ�ʽ.
  BOOL m_bHolidyValid; //�ڼ����Ƿ���Ч
  BOOL m_bActionLock; //�������
  BOOL m_bActionOutPut; //�����������
} DEV_RIGHT; //Ȩ��

typedef struct {
  int m_nCount;
  DEV_RIGHT* m_pRtArray;
} DEV_RIGHT_ARRAY; //Ȩ������

typedef struct {
  int m_GroupID; //��ID
  int m_NormalValid; //��ͨ�û�����Ч�û���
  int m_ForceValid; //ǿ���û�����Ч�û���
  DEV_CID m_NormalUsers[DEV_USER_COMBINS]; //��ͨ�û���ϣ����ȼ���
  DEV_CID m_ForceUsers[DEV_USER_COMBINS]; //ǿ���û���ϣ����ȼ���
  BOOL m_bGroupOrder; //����Ƿ����� 1����0����
} DEV_USERGROUP;

typedef struct {
  int m_nCount;
  DEV_USERGROUP* m_pUGArray;
} DEV_USERGROUP_ARRAY; //�û�������

//������½ṹ��
typedef struct {
  int m_Bytes; //�ļ���С
  DEV_CID m_FileName; //�ļ���
  char m_TotalFiles; //��Ҫ���µ��ļ�����
  char m_FileIndex; //��ǰ���µ��ڼ���
  char* m_Buf; //�ļ����ݻ����׵�ַ
} DEV_APPUPDATE;

//�û���������
typedef struct {
  int m_nCount;
  DEV_USER* m_pUserArray;
} DEV_BATCH_USER;

//������¼
typedef struct {
  int m_nCount;
  DEV_RECORD* m_pRecordArray;
} DEV_BATCH_RECORD;

//������־
typedef struct {
  int m_nCount;
  DEV_LOG* m_pLogArray;
} DEV_BATCH_LOG;

//����ע����
typedef struct {
  int m_nCount;
  DEV_CHANGEIMAGE* m_pCImageArray;
} DEV_BATCH_CIMAGE;

//IO�豸״̬
#define DEV_IO_MODE_NORMAL 0x00 //����״̬
#define DEV_IO_MODE_OPEN 0x01 //����״̬
#define DEV_IO_MODE_CLOSE 0x02 //����״̬
//�Ŵ�״̬
#define DEV_IO_STATE_CLOSE 0x00 //�ŴŹ�
#define DEV_IO_STATE_OPEN 0x01 //�Ŵſ�
//ִ�ж���
#define DEV_ACT_IO_OPEN 0x02 //ִ�д򿪶���
#define DEV_ACT_IO_OPENEX 0x04 //ִ�и�������
#define DEV_ACT_IO_CLOSE 0x10 //ִ�йرն���
#define DEV_ACT_MODE_SET 0x20 //����IO�豸����ģʽ
#define DEV_ACT_MODE_GET 0x40 //���IO�豸����ģʽ
#define DEV_ACT_STATE_GET 0x80 //���IO�豸��ǰ״̬

typedef struct {
  //IO�豸485��ַ
  char m_Source;
  //ִ�ж���
  char m_Action;
  //IO�豸��ǰģʽ
  char m_IOMode;
  //IO�豸��ǰ״̬״̬
  char m_IOState;
} DEV_IOCTRL;

//NOPEN:Զ���������˿���
typedef struct {
  //m_Users==1Ȩ��ID, m_Users>1���ID
  int m_CtxID;
  //��֤�û���Ŀ�� 0���뿪�ţ�1Ȩ�޿��ţ�>1��Ͽ���
  int m_Users;
  //������Ϣ
  DEV_IOCTRL m_XOpen;
  //��֤�û�����
  DEV_CID m_IDS[DEV_USER_COMBINS * 2];
} DEV_NOPEN;

typedef struct {
  unsigned short Port; //�˿�
  char IP_Address[16]; //���ʮ����IP��ַ
} PEERADR;

#define DEV_WEB_LOADUSER 0x0001 //�����û�
#define DEV_WEB_WRITEUSER 0x0002 //д�û�
#define DEV_WEB_WRITEREC 0x0004 //д��¼
#define DEV_WEB_SYNCTIME 0x0008 //ͬ��ʱ��

//��������
typedef struct {
  char m_WebPro1;
  char m_WebPro2;
  char m_WebUrl1[127];
  char m_WebUrl2[127];

  char m_IPAdr[16];
  char m_MIPAdr[16];
  char m_NetMask[16];
  char m_GateWay[16];
  char m_Dns[16];
} DEV_NETPRO;

//�������λ
#define F_DL_ID CMD_DOWN_ID //����ID����
#define F_DL_ID_ALL CMD_ID_ALL //�����������ݵ�ID����
#define F_ID_TIME_RANGE CMD_ID_TIME_RANGE //�����ƶ�ʱ���ID����
#define F_ID_RANGE F_ID_TIME_RANGE<<1
#define F_AUTH_VERIFY AUTH_VERIFY //�û������֤
#define F_DL_PIC DL_IMAGE //��Ҫ������Ƭ
#define F_DL_FEAT DL_FEAT //��Ҫ������������
#define F_DL_APPENDID DL_APPENDID //Զ�̼���ʱ��m_ID��ʾ����
#define F_DL_NORMAL_TG DL_NORMAL_TG //��������ʱ����
#define F_DL_HOLIDAY_TG DL_HOLIDAY_TG //���ؽڼ���
#define F_DOOR_CLOSE DOOR_CLOSE //�������ţ�����Ϊ��������
#define F_DOOR_OPEN DOOR_OPEN //�������߳���״̬ȡ��
#define F_G_ORDER G_ORDER //��Ͽ�������
#define F_DEFAULT_NO_GROUP DEFAULT_NO_GROUP
#define TIMEGROUP_NUMS TGROUP_NUM //ʱ�������
#define USER_COMBINS MAX_GROUPUSER //�û���ϸ���

#define USER_JPEG_SIZE (64*1024)
#define REC_JPEG_SIZE (18*1024)
#define FEAT_SIZE (90*1024)
#define OFFSET_SIZE 20
//#define TRANS_SERVER


#ifndef TRANS_SERVER
#define CMD_S_CONNECT_CUT DEV_CONNECT_CUT //�Ͽ�����
#define CMD_S_CONNECT_SUCCESS DEV_CONNECT_SUCCESS //���ӳɹ�
#define CMD_S_CONNECT_FAILUE DEV_CONNECT_FAILUE //����ʧ��
#define CMD_S_CONNECT_NONE DEV_CONNECT_NONE //δ����
#define CMD_S_CONNECT_REQ DEV_CONNECT_NONE+1 //�������� 
#define CMD_S_CONNECT_REQ_CUT CMD_S_CONNECT_REQ+1 //����Ͽ�����
#define E_CONNECT_STATUS DEV_CONNECT_STATUS
#else
#define CMD_S_CONNECT_CUT NET_CONNECT_CUT //�Ͽ�����
#define CMD_S_CONNECT_SUCCESS NET_CONNECT_SUCCESS //���ӳɹ�
#define CMD_S_CONNECT_FAILUE NET_CONNECT_FAILUE //����ʧ��
#define CMD_S_CONNECT_NONE NET_CONNECT_NONE //δ����
#define CMD_S_CONNECT_REQ NET_CONNECT_NONE+1 //�������� 
#define CMD_S_CONNECT_REQ_CUT CMD_S_CONNECT_REQ+1 //����Ͽ�����
#define E_CONNECT_STATUS NET_CONNECT_STATUS
#endif

#ifndef TRANS_SERVER

//�ͻ��������֤
typedef struct {
  DEV_CID m_UserName; //�û���
  DEV_CID m_Password; //����
} DEV_AUTH;

typedef enum {
  DL_SINGLE = 0x01,
  DL_ALL,
  DL_SEGTIME,
  DL_RANGE
} DATA_DL_TYPE;

//���ָ��ʱ��ε�ID����
typedef struct {
  DEV_DATE m_Start; //��¼��ʼʱ��
  DEV_DATE m_End; //��¼����ʱ��
} DEV_TIMESEG;

#endif



typedef char int8;
typedef INT32 int32;
//typedef unsigned int uint32;
typedef unsigned short uint16;
#define MAX_SEND_COUNTS 5


#define M_AUTH 0x07 //�ͻ��������֤
#define M_HEART 0x09 //������
#define M_VIDEO_OUT 0x23 //UDP������Ƶ���� NET_ADDRESS

//����������
#define S_SET 0x81 //������Ƶ
#define S_GET 0x83 //�ر���Ƶ

#define F_REQ_FLAG 0x40000000 //����ռ�ô�λ
#define F_REPLY_FLAG 0x20000000 //����ռ�ô�λ

//�û���֤
#define AUTH_VERIFY 0x00000100 //�û������֤

#define PACK_SIZE (60*1024) //һ����Ϊ60��С

#define ALIGN(bytes, alignments) (((bytes)+((alignments)-1))&~((alignments)-1))


typedef struct pmdev_t pmdev_t;

typedef int (*frame_fun_t)(pmdev_t* vp, int h, int w, const unsigned char* img, int step, int cn);
typedef int (*event_fun_t)(pmdev_t* vp, const char* sDevIP, const char* sClientIP, int eType, int eFlag, void* pvContent, int nSerialNo, int nResult, void* pvClientData);

typedef struct pmdev_t {
  SOCKET m_sHost;//TCP�׽���,���豸�������ӣ����豸�˵�ʵʱ����
  SOCKET m_sVVIDEOSck;//UDP�׽��֣������豸���͹�����ʵʱ��Ƶ����

  int m_nIndex;//��������
  int m_Author;//��½�豸Ȩ����֤ͨ����־
  int m_VideoClose;//�ر���Ƶ��־
  int m_exitThread;
  thread_t m_hTCPThread[1];//����ʵʱ������Ӧ���ݵ��߳�
  thread_t m_hUDPThread[1];//����ʵʱ��Ƶ���ݵ��߳�
  frame_fun_t m_framefun;
  event_fun_t m_eventfun;
  void* userdata;
  //int m_nTCPPort;//TCP�˿� 13333
  int m_nUDPPort;//UDP�˿�,���ѡ��Ŀ��ö˿�
  char m_strDevIP[32];//�豸IP
  //char m_strClientIP[32];//����IP
  int cn;
  int flipud;
  int run;
} pmdev_t;

#define TCPPORT 13333
#define ERR_VERIFY_FAIL 0x06 //��֤ʧ��
#define SUC_EXECUT 0x00 //�ɹ�
#define GET_ERR( state ) ( (state) & 0xff )

//��ͷ�̶��̶�Ϊ20
#define OFFSET_SIZE 20

#ifdef WIN32
#define SOCK_LENTH int
#define T_VOID void
#define T_HANDLE HANDLE
#define SLEEP(Milliseconds) {Sleep(Milliseconds);}
#define LASTERROR WSAGetLastError()
#define MILLISEC 1
#define MEMRESERVER_BLOCKS 10 //�ڴ���С
#else
#define SOCKET int
#define SOCK_LENTH socklen_t
#define T_HANDLE pthread_t
#define T_VOID void*
#define INVALID_SOCKET 0
#define SOCKET_ERROR -1
#define closesocket close
#define ioctlsocket ioctl
#define SLEEP(Milliseconds) {struct timeval tval;tval.tv_sec= 0;tval.tv_usec=(Milliseconds); select( 0 , NULL , NULL , NULL , &tval ); }
#define LASTERROR errno
#define WSAECONNRESET ECONNRESET
#define WSAECONNABORTED ECONNABORTED
#define WSAEWOULDBLOCK EWOULDBLOCK
#define WSAEINTR EINTR
#define MILLISEC 1000
#define SD_BOTH SHUT_RDWR
#define MEMRESERVER_BLOCKS 2 //�ڴ���С
#define INFINITE 0xFFFFFFFF

#endif

#define TRACE printf
#ifndef ASSERT
#define ASSERT assert
#endif // ASSERT


#define OFF_SET(T, member) ((int)( &(((T*)0)->member)))

#include "Base64.inl"

void CopyDevUser2DspUser(USER* dspUser , const DEV_USER* devUser)
{
  ZeroMemory(dspUser , sizeof(USER));
  memcpy(dspUser, devUser, OFF_SET(USER, m_UserType));
  dspUser->m_UserType = devUser->m_UserType;

  if (devUser->m_FeatLen) {
    if ((devUser->m_UserFlag & DEV_USER_FEAT_BASE64) && devUser->m_FeatData[3]) {
      dspUser->m_FeatBytes = deBase64(devUser->m_FeatData , devUser->m_FeatLen , dspUser->m_Block);
    }
    else {
      memcpy(dspUser->m_Block, devUser->m_FeatData, devUser->m_FeatLen);
    }
  }

  if (devUser->m_PicLen) {
    if (devUser->m_UserFlag & DEV_USER_IMAGE_BASE64) {
      dspUser->m_PicBytes = deBase64(devUser->m_PicData , devUser->m_PicLen , dspUser->m_Block + dspUser->m_FeatBytes);
    }
    else {
      memcpy(dspUser->m_Block + dspUser->m_FeatBytes , devUser->m_PicData, devUser->m_PicLen);
    }
  }
}

static int dev_find_user(const DEV_BATCH_USER* users, const char* userid)
{
  int i;

  for (i = 0; i < users->m_nCount; ++i) {
    if (0 == strncmp(users->m_pUserArray[i].m_UserID.m_ID, userid, DEV_ID_LEN - 1)) {
      return i;
    }
  }

  return -1;
}
static int dev_find_record(const DEV_BATCH_RECORD* records, const char* serialid)
{
  int i;

  for (i = 0; i < records->m_nCount; ++i) {
    if (0 == strncmp(records->m_pRecordArray[i].m_SerialID.m_ID, serialid, DEV_ID_LEN - 1)) {
      return i;
    }
  }

  return -1;
}
static int dev_insert_user(DEV_BATCH_USER* users, const DEV_USER* newuser, int issort, int newsize)
{
  const char* userid = newuser->m_UserID.m_ID;
  int k = dev_find_user(users, userid);

  if (k < 0) {
    int i = users->m_nCount;

    if (issort) {
      for (i = 0; i < users->m_nCount; ++i) {
        int t = strncmp(users->m_pUserArray[i].m_UserID.m_ID, userid, DEV_ID_LEN - 1);

        if (t >= 0) {
          break;
        }
      }
    }

    users->m_nCount++;

    if (newsize) {
      REALLOC(DEV_USER, users->m_pUserArray, newsize);
    }

    MEMINST(users->m_pUserArray, users->m_nCount - 1, i, newuser, 1);
    users->m_pUserArray[i].m_FeatData = (char*)memdup(newuser->m_FeatData, newuser->m_FeatLen);
    users->m_pUserArray[i].m_PicData = (char*)memdup(newuser->m_PicData, newuser->m_PicLen);
  }

  return 0;
}


static int dev_insert_users(DEV_BATCH_USER* users, const DEV_USER* newuser, int n, int issort)
{
  int i;
  int newcount = users->m_nCount + n;

  for (i = 0; i < n; ++i) {
    dev_insert_user(users, newuser + i, issort, i ? 0 : newcount);
  }

  return 0;
}

static int dev_insert_record(DEV_BATCH_RECORD* records, const DEV_RECORD* newrecord, int issort, int newsize)
{
  const char* serialid = newrecord->m_SerialID.m_ID;
  int k = dev_find_record(records, serialid);

  if (k < 0) {
    int i = records->m_nCount;

    if (issort) {
      for (i = 0; i < records->m_nCount; ++i) {
        int t = strncmp(records->m_pRecordArray[i].m_SerialID.m_ID, serialid, DEV_ID_LEN - 1);

        if (t >= 0) {
          break;
        }
      }
    }

    records->m_nCount++;

    if (newsize) {
      REALLOC(DEV_RECORD, records->m_pRecordArray, newsize);
    }

    MEMINST(records->m_pRecordArray, records->m_nCount - 1, i, newrecord, 1);
    records->m_pRecordArray[i].m_PicData = (char*)memdup(newrecord->m_PicData, newrecord->m_PicLen);
  }

  return 0;
}


static int dev_insert_records(DEV_BATCH_RECORD* records, const DEV_RECORD* newrecord, int n, int issort)
{
  int i;
  int newcount = records->m_nCount + n;

  for (i = 0; i < n; ++i) {
    dev_insert_record(records, newrecord + i, issort, i ? 0 : newcount);
  }

  return 0;
}

//У��ͼ���
UINT16 sock_checksum(UINT16* buffer, UINT32 size)
{
  UINT32 cksum = 0;

  while (size > 1) {
    int z = *buffer;
    cksum += *buffer++;
    size -= sizeof(UINT16);
    //printf("cksum = %d z = %d\n", cksum, z);
  }

  if (size) {
    cksum += *(UINT8*)buffer;
  }

  cksum = (cksum >> 16) + (cksum & 0xffff);
  cksum += (cksum >> 16);
  return (UINT16)(~cksum);
}

int write_time_out(SOCKET sock, int time)
{
  fd_set fwds;
  struct timeval wtval;
  FD_ZERO(&fwds);
  FD_SET(sock, &fwds);
  wtval.tv_sec = 0;
  wtval.tv_usec = (time * 1000);
  return select(sock + 1, NULL, &fwds, NULL, &wtval) <= 0;
}


static int mylogprintf(const char* fmt, ...)
{
  FILE* pf;
  pf = fopen("log.txt", "a");

  if (pf) {
    va_list ARGLIST;
    va_start(ARGLIST, fmt);
    vfprintf(pf, fmt, ARGLIST);
    fflush(pf);
    va_end(ARGLIST);
    fclose(pf);
  }

  return 0;
}
int safe_send(SOCKET sock, const char* data, int len)
{
  int nTotalSends = len;
  int bOK = 0;
  int send_counts = 0;
#if 1
  {
    int i;
    mylogprintf("send %d:", len);

    for (i = 0; i < len; ++i) {
      //mylogprintf("%02x ", ((unsigned char*)data)[i]);
      mylogprintf("%d ", ((char*)data)[i]);
    }

    mylogprintf("\n");
  }
#endif

  while (send_counts++ < MAX_SEND_COUNTS) {
    int nHasSend, bTimeOut = write_time_out(sock , 300);

    if (bTimeOut) {
      continue;
    }

    nHasSend = send(sock, data, nTotalSends, 0);

    if (nHasSend <= 0) {
      int32 nError = LASTERROR;
      TRACE("3---nError = %d , len = %d nHasSend = %d\r\n" , nError , len , nHasSend);

      if (WSAEINTR == nError || WSAEWOULDBLOCK == nError) {
        continue;
      }
      else {
        break;
      }
    }

    data += nHasSend;
    nTotalSends -= nHasSend;

    if (nTotalSends <= 0) {
      bOK = 1;
      break;
    }
  }

  return bOK;
}

#ifndef MIN
#define MIN(a, b) ((a)<(b) ? (a) : (b))
#endif // MIN

#ifndef MAX
#define MAX(a, b) ((a)>(b) ? (a) : (b))
#endif // MIN

unsigned long _inet_addr(const char* ip)
{
  if (NULL == ip) {
    return 0;
  }
  else {
    unsigned int a = 0;
    unsigned int b = 0;
    unsigned int c = 0;
    unsigned int d = 0;
    unsigned int ipInt = 0;
    BYTE* pInt = (BYTE*)(&ipInt);
    sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
    pInt[0] = (BYTE)a;
    pInt[1] = (BYTE)b;
    pInt[2] = (BYTE)c;
    pInt[3] = (BYTE)d;
    return ipInt;
  }
}


int send_cmd_packet(pmdev_t* pv, SOCKET sock, UINT32 m_MCmd, UINT32 m_SCmd, UINT32 m_CFlag, const void* data0, int len)
{
#define OFFSET_SIZE 20
  char pBuffer[PACK_SIZE + OFFSET_SIZE] = { 0 };//���60K��Сһ����
  int nTotalSend = 0;
  int bOK = 1;
  int i = 0;
  int nSeq = ++pv->m_nIndex;
  const char* data = (const char*)data0;
  NCMD* pCmd = (NCMD*)(pBuffer + OFFSET_SIZE);

  if (!data0) {
    len = 0;
  }

  pCmd->m_MCmd = m_MCmd;
  pCmd->m_SCmd = m_SCmd;
  pCmd->m_CFlag = m_CFlag;
  pCmd->m_Param1 = -1;
  pCmd->m_Param2 = _inet_addr(pv->m_strDevIP);
  len += sizeof(NCMD);

  for (i = 0; i < len;) {
    int nSend, maxpacklen = 0;
    //��װ��������� �Ӱ�ͷ
    SOCK_DATA_HEADER* hdr = (SOCK_DATA_HEADER*)(pBuffer + i);
    hdr->msg_identify = nSeq; //��ʶ
    hdr->msg_offset = i * PACK_SIZE; //ƫ��
    hdr->msg_checksum = 0;  //�����
    hdr->msg_total = len;  //�����ܳ���
    hdr->msg_len = MIN(PACK_SIZE, (len - i)); //���ݳ���
    hdr->msg_len = ALIGN(hdr->msg_len, 4);
    hdr->msg_total = ALIGN(hdr->msg_total, 4);
    nSend = ALIGN(hdr->msg_len, 4) + OFFSET_SIZE;

    if (0 == i) {
      memcpy(pCmd + 1, data, hdr->msg_len - sizeof(NCMD));
    }
    else {
      memcpy(hdr + 1, data + i - sizeof(NCMD), hdr->msg_len);
    }

    hdr->msg_checksum = sock_checksum((uint16*)hdr, sizeof(SOCK_DATA_HEADER));

    //printf("send %d\n", nSend);
    if (!safe_send(sock, (const char*)hdr, nSend)) {
      bOK = 0;
      nTotalSend = SOCKET_ERROR;
      break;
    }

    nTotalSend += hdr->msg_len;
    i += hdr->msg_len;
  }

  if (SOCKET_ERROR == nTotalSend) {
    printf("send Heart CMD failed!\n");
    closesocket(pv->m_sHost); //�ر��׽���
    pv->m_sHost = 0;
    pv->m_eventfun(pv, pv->m_strDevIP, NULL, DEV_CONNECT_CUT, 0, 0, 0, 0, pv->userdata);
  }

  return nTotalSend;
}

//�����豸������������ͷ
int ResolveNCMD(const char* pBuffer, const int nLen , int* nRlt)
{
  NCMD* pCMD = (NCMD*)(pBuffer + OFFSET_SIZE);
  *nRlt = GET_ERR(pCMD->m_CFlag);
  return pCMD->m_MCmd;
}

typedef struct databuf_t {
  UINT32 msg_identify; //��ʶ
  UINT32 msg_len; //���ݳ���
  UINT32 msg_total; //�����ܳ���
  char* buf;
} databuf_t;

int myrecv(SOCKET s, void* buf, int buflen, int istcp, int time, int isudp, struct sockaddr *from, int * fromlen)
{
  int ret = 0, i = 0;
  char* cbuf = (char*)buf;

  for (i = 0; i < buflen;) {
    if (0 != time) {
      FD_SET fd = { 1, s };
      struct timeval timeout;
      timeout.tv_sec = time / 1000, timeout.tv_usec = 1000 * (time % 1000);

      if (select(0, &fd, NULL, NULL, &timeout) == 0) {
        return -2;
      }
    }

    if (isudp) {
      ret = recvfrom(s, cbuf + i, buflen - i, 0, (struct sockaddr*)from, fromlen);
    } else {
      ret = recv(s, cbuf + i, buflen - i, 0);
    }

    if (ret <= 0) {
      return ret;
    }

    i += ret;
  }

  return i;
}

databuf_t* recv_pack_data(SOCKET s, databuf_t* databuf, int bufnum, int istcp, int* err, int isudp, struct sockaddr *from, int * fromlen)
{
  databuf_t* it = NULL;
  int i, len = 0, checksum;
  //��ȡ��ͷ
  SOCK_DATA_HEADER Head[1] = {0};
  //�հ�ͷ
  len = myrecv(s, Head, sizeof(SOCK_DATA_HEADER), istcp, 0, isudp, from, fromlen);

#if 1
  {
    int i;

    for (i = 0; i < len; ++i) {
      //mylogprintf("%02x ", ((unsigned char*)data)[i]);
      printf("%d ", ((char*)(Head))[i]);
    }

    printf("\n");
  }
#endif

  //len = recvfrom(pv->m_sVVIDEOSck, pTemp, pack_size, 0, (struct sockaddr*)&addrClient, &len);
  if (len < 0) {
    *err = len;
    return NULL;
  }

#if 0
  {
    int i;
    unsigned char aa[20] = {117, 9, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 98, -10, 0, 0};
    checksum = sock_checksum((UINT16*)(aa), 20);
    mylogprintf("send %d:", len);

    for (i = 0; i < len; ++i) {
      //mylogprintf("%02x ", ((unsigned char*)data)[i]);
      mylogprintf("%d ", ((char*)Head)[i]);
    }

    mylogprintf("\n");
  }
#endif
  //printf("len = %d\n", len);
  //У��
  checksum = sock_checksum((UINT16*)(Head), sizeof(SOCK_DATA_HEADER));

  if (0 != checksum) {
    *err = -3;
    return NULL; //У���������
  }

  //���ұ��α�ʶ�����ݽ��ջ���
  for (i = 0; i < bufnum; ++i) {
    if (databuf[i].msg_identify == Head->msg_identify) {
      it = databuf + i;
      printf("it\n");
      break;
    }
  }

  if (NULL == it) { //û�ҵ�����Ϊ����Ƶ���ݰ���
    if (Head->msg_offset != 0) { //�����Ƿ񶪰�ͷ������Ϊ������ͷ�ķǷ���Ƶ���ݡ�
      return NULL;
    }

    it = databuf;

    for (i = 1; i < bufnum; ++i) {
      if (it->msg_identify > databuf[i].msg_identify) {
        it = databuf + i;
      }
    }

    //Ϊ���ձ�����Ƶ���ݿ���һ���컺�棬�����뵽MAPӳ�����
    it->buf = (char*)realloc(it->buf, Head->msg_total);
    ZeroMemory(it->buf, Head->msg_total);
    it->msg_identify = Head->msg_identify;
    it->msg_len = 0;
    it->msg_total = Head->msg_total;
  }

  assert((long)it->buf % 4 == 0);//��֤�����Ƿ�4�ֽڶ���

  //������
  len = myrecv(s, ((char*)it->buf) + Head->msg_offset, Head->msg_len, istcp, 100, isudp, from, fromlen);

  printf("len = %d %d\n", len, Head->msg_len);
  *err = len;

  if (len > 0) {
    it->msg_len += Head->msg_len;
  }
  else {
    *err = len;
  }

  if (it->msg_len == Head->msg_total) {
    //printf("msg_total = %d\n", it->msg_total);

    if (it->msg_len > 2 * PACK_SIZE) {
      int adsf = 0;
    }
  }

  return it->msg_len == Head->msg_total ? it : NULL;
}

//������Ƶ�����߳�
static DWORD WINAPI RecvVideoDataThread(pmdev_t* pv)
{
  int cn = pv->cn;
  int nLen = 0;
  int nCmd = 0;
  int nRlt = 0;
  unsigned char* pDataBuf = NULL;
  unsigned char* pVideoBuf = NULL;
  unsigned char* pRGBTMP;
  unsigned char* pRGBTRANS;

  SOCKADDR_IN addrClient = {0};
  int len = sizeof(SOCKADDR);
  char* pData = NULL;
  DEVIMAGE* pDevImage = NULL;

  enum { maxbufnum = 32, pack_size = sizeof(SOCK_DATA_HEADER) + PACK_SIZE};
  int h = 0, w = 0;
  uchar pTemp[pack_size];
  int bufsz = 0;
  int msg_identify = -1, msg_total = 0, msg_off=0;
  
  while (!pv->m_VideoClose) {
    //������Ƶ����
    nLen = recvfrom(pv->m_sVVIDEOSck, (char*)pTemp, pack_size, 0, (struct sockaddr*)&addrClient, &len);

    if (nLen < 0) {
      printf("recvfrom ���� %d\n", nLen);
      msg_off = 0;
      msg_identify = -1;
      continue;
      //pv->m_eventfun(pv, pv->m_strDevIP, NULL, DEV_CONNECT_CUT, 0, 0, 0, 0, pv->userdata);
    }

    //�ۻ���ǰ�����ѽ��յķְ��ܳ���
    //�ж�����Ƿ���ɣ�����򲥷�
    if (nLen>0) {
      SOCK_DATA_HEADER* pHead = NULL;
      pHead = (SOCK_DATA_HEADER*)(pTemp);
      if ( 0 != sock_checksum( (UINT16*)pHead, sizeof(SOCK_DATA_HEADER) ) ) {
        printf("У�����\n");
				continue;//У�����,����
      }
      printf("msg_identify=%d msg_offset=%d msg_len=%d bufsz=%d\n", pHead->msg_identify, pHead->msg_offset, pHead->msg_len, bufsz);

      //������������ݰ�
      if (msg_identify!=(int)pHead->msg_identify) {
        if (pHead->msg_offset == 0) {//��һ���ְ�
          NCMD* pCMD = (NCMD*)(pTemp+OFFSET_SIZE);
          nRlt       = GET_ERR(pCMD->m_CFlag);
          if ( pCMD->m_MCmd != M_VIDEO_OUT ) {
            continue; //�������Ƿ���
          }
          if (bufsz < (int)(pHead->msg_total+sizeof(SOCK_DATA_HEADER))) {
            bufsz = pHead->msg_total+sizeof(SOCK_DATA_HEADER);
            pDataBuf = (unsigned char*)realloc(pDataBuf, bufsz);
          }
          msg_identify = pHead->msg_identify;
        } else {
          printf("���ǵ�һ���ְ�\n");
          continue;
        }
      }
      memcpy(pDataBuf + pHead->msg_offset, pTemp+sizeof(SOCK_DATA_HEADER), pHead->msg_len);
      msg_total += pHead->msg_len;
      if (msg_total!=(int)pHead->msg_total) {
        continue;
      }
      msg_total = 0;

      pDevImage = (DEVIMAGE*)(pDataBuf + sizeof(NCMD));

      if (pDevImage->m_W != w || pDevImage->m_H != h) {
        pVideoBuf = (unsigned char*)realloc(pVideoBuf, pDevImage->m_H * pDevImage->m_W * cn * 2);
        pRGBTMP = pVideoBuf;
        pRGBTRANS = pVideoBuf + pDevImage->m_H * pDevImage->m_W * cn;
      }

#if 1

      if (IMAGE_UYVY == pDevImage->m_Type) {
        w = pDevImage->m_H;
        h = pDevImage->m_W;
        YUV422_C_RGB(pDevImage->m_H, pDevImage->m_W, (unsigned char*)pDevImage->m_Block, pDevImage->m_W * 2, pRGBTMP, pDevImage->m_W * cn, cn);
        RGB_EDDY(pRGBTMP, pDevImage->m_H, pDevImage->m_W, pRGBTRANS, cn, pv->flipud);
      }
      else if (IMAGE_YUYV == pDevImage->m_Type) {
        w = pDevImage->m_W;
        h = pDevImage->m_H;
        YUYV_C_RGB((unsigned char*)pDevImage->m_Block, pRGBTRANS, pDevImage->m_H, pDevImage->m_W, cn);
      }

#endif

      //��ʾ��Ƶ
      if (pv->m_framefun) {
        pv->m_framefun(pv, h, w, pRGBTRANS, w * cn, cn);
      }
      else {
        printf("%d %d %d\n", pv->m_nIndex, h, w);
      }
      msg_off = 0;
    }

    sleep(1);
  }
  
  if (pTemp) {
    free(pDataBuf);
    pDataBuf = NULL;
  }
  if (pVideoBuf) {
    free(pVideoBuf);
    pVideoBuf = 0;
    pRGBTMP = 0;
    pRGBTRANS = 0;
  }

  pv->m_VideoClose = 0;
  return 0;
}

#ifndef WIN32
int Sleep(int ms)
{
  return sleep(ms);
}
#endif
//�� �� �豸��֤��������֤
//�� �� cszDevAddr�豸��ַ
//cName �û���
//cPws ����
//nFlag 1=��֤ 2=����
//˵ ��
BOOL CPM_ULDevAuth(pmdev_t* pv, const char* sLoginName, const char* sPassword, int nFlag)
{
  DEV_AUTH pAuth[1] = {0};
  //����������
  strncpy(pAuth->m_UserName.m_ID, sLoginName, DEV_ID_LEN);
  strncpy(pAuth->m_Password.m_ID, sPassword, DEV_ID_LEN);
  return send_cmd_packet(pv, pv->m_sHost, M_AUTH, S_SET, ((nFlag == 2) ? 0 : F_AUTH_VERIFY), pAuth, sizeof(DEV_AUTH));
}
//�� �� �����豸���������ַ
//�� ��
//cszDevAddr �豸��ַ
//cszRgnAddr �豸�����ַ
//˵ �� ���豸�������Ӻ�ɽ��豸���õ�ĳһ�������Ժ󼰿ɶ����������������豸
BOOL CPM_ULDevRegionAddr(pmdev_t* pv, const char* cszGroupIP)
{
  CID cid[1] = {0};
  int slen = strlen(cszGroupIP);

  if (slen < ID_LEN) {
    memcpy(cid->m_ID, cszGroupIP, slen);
  }

  return send_cmd_packet(pv, pv->m_sHost, M_MCAST, S_SET, 0, cid, sizeof(CID));
}
BOOL devcmd_GetDevRgn(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_MCAST, S_GET, 0, 0, 0);
}
//�� �� �����豸ϵͳʱ��
//�� ��
//cszDevAddr �豸��ַ
//cSysTime ʱ����Ϣ
//˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
//eType = DEV_SYS_TIME_OPER
//eFlag = DEV_OPER_UPLOAD ��
//pvContent = NULL
BOOL CPM_ULDevTime(pmdev_t* pv, const DEV_DATETIME* cTime)
{
  return send_cmd_packet(pv, pv->m_sHost, M_TIME, S_SET, 0, cTime, sizeof(DATETIME));
}
//�� �� ��ȡ�豸ϵͳʱ��
//�� �� cszDevAddr �豸��ַ
//˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
//eType = DEV_SYS_TIME_OPER
//eFlag = DEV_OPER_DOWNLOAD��
//pvContent = DEV_DATETIME*
BOOL CPM_DLDevTime(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_TIME, S_GET, 0, 0, 0);
}
//�� �� ��ȡ�豸������Ϣ
//�� �� cszDevAddr �豸��ַ
//˵ ��
BOOL CPM_DLDevBaseInfo(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_DEVINFO, S_GET, 0, 0, 0);
}
//�� �� ��ȡ�豸ͳ����Ϣ
//�� �� cszDevAddr �豸��ַ
//˵ ��
BOOL CPM_DLDevStatisInfo(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_STATIS, S_GET, 0, 0, 0);
}
//�� �� ��ȡ��������������
//�� ��
//cszDevAddr ��������ַ
//nSeq �������к�
//˵ �� ��������DEV_WORKATTͨ���ص�����������
BOOL CPM_DLDevWorkAttInfo(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_DEVSET, S_GET, 0, 0, 0);
}
//�� �� �����豸����������Ϣ
//�� ��
//cszDevAddr �豸��ַ
//cWorkAtt �������Խṹ��
//nSeq �������к�
BOOL CPM_ULDevWorkAttInfo(pmdev_t* pv, const DEV_WORKATT* cAtt)
{
  DEVSET pSet[1] = {0};
  memcpy(pSet->m_DevCtx, cAtt->m_TimeGID, sizeof(cAtt->m_TimeGID));
  pSet->m_DefaultRight = cAtt->m_DefaultRight;
  pSet->m_BaseSet = cAtt->m_BaseSet;
  memcpy(pSet->m_SuperPassword.m_ID, cAtt->m_szSuperPWD.m_ID, sizeof(CID));
  pSet->m_DoorMangetTime = cAtt->m_DoorMangetTime;
  pSet->m_LockTime = cAtt->m_LockTime;
  pSet->m_VerifyMode = cAtt->m_VerifyMode;
  pSet->m_OutputWG = cAtt->m_bWGOutPut ? OUTPUT_WG_YES : 0x00;
  pSet->m_OutputWG |= cAtt->m_nWGType ? OUTPUT_WG_34 : 0x00;
  pSet->m_OutputWG |= cAtt->m_nWGOutType ? OUTPUT_WG_CTX : 0x00;
  pSet->m_OutputWG |= cAtt->m_nWGOutIDType ? OUTPUT_WG_UID : 0x00;
  pSet->m_OutputCtx = cAtt->m_nWGOutContent;
  return send_cmd_packet(pv, pv->m_sHost, M_DEVSET, S_SET, 0, pSet, sizeof(DEVSET));
}
//�� �� �ϴ�ָ���û�
//�� ��
//cszDevAddr �豸��ַ
//cUser �û���Ϣ�ṹ��
//nSeq �������к�
//˵ ��
//�û���������ʱ��DEV_USER�е� m_FeatLen = 0�� m_FeatData = NULL
//�û���ע����ʱ��DEV_USER�е� m_PicLen= 0�� m_PicData = NULL
//DevOperResultNotify�ص�������Ӧ���ͣ�eType = DEV_USER_OPER
//eFlag = DEV_OPER_UPLOAD��pvContent= NULL
BOOL CPM_ULUser(pmdev_t* pv, const DEV_USER* cUser)
{
  BOOL bAddFlag = TRUE;
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(USER) + FEAT_SIZE + USER_JPEG_SIZE, 4) >> 2] = {0};
  USER* pUser = (USER*)nData;
  ASSERT((cUser->m_FeatLen <= FEAT_SIZE) && (cUser->m_PicLen <= USER_JPEG_SIZE));

  if (!((cUser->m_FeatLen <= FEAT_SIZE) && (cUser->m_PicLen <= USER_JPEG_SIZE))) {
    return FALSE;
  }

  CopyDevUser2DspUser(pUser , cUser);
  return send_cmd_packet(pv, pv->m_sHost, M_USER, bAddFlag ? S_SET : S_REALTIME_CAP, 0, pUser, USER_STEP(*pUser));
}
//�� �� �ϴ�ʵʱ�����û�
//�� ��
//cszDevAddr �豸��ַ
//cUser �û���Ϣ�ṹ��
//cRtLoad ʵʱ������Ϣ
//nSeq �������к�
//˵ �� �豸������ͻ����ϴ�ĳ�û�ʱ��ͨ���˽ӿ��ϴ��������û���Ϣ��
//cRtLoadΪ�豸������ʱ����������Ϣ���ͻ��˽�ֹ�޸ġ��˺���ʵ��Զ���û����ع��ܡ�
BOOL CPM_ULRealTimeUser(pmdev_t* pv, const DEV_USER* cUser, const DEV_USER_REAL_LOAD* cLoad)
{
  int m_CFlag = 0;
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(USER) + FEAT_SIZE + USER_JPEG_SIZE, 4) >> 2] = {0};
  USER* pUser = (USER*)nData;
  ASSERT((cUser->m_FeatLen <= FEAT_SIZE) && (cUser->m_PicLen <= USER_JPEG_SIZE));

  if (!((cUser->m_FeatLen <= FEAT_SIZE) && (cUser->m_PicLen <= USER_JPEG_SIZE))) {
    return FALSE;
  }

  if (cLoad->m_bApeendID) {
    m_CFlag |= F_DL_APPENDID;
  }

  if (cLoad->m_LoadFlag & DL_DEV_PIC) {
    m_CFlag |= F_DL_PIC;
  }

  if (cLoad->m_LoadFlag & DL_DEV_USER_FEAT) {
    m_CFlag |= F_DL_FEAT;
  }

  //pCMD->m_Param1 = cLoad.m_ReqSource;
  CopyDevUser2DspUser(pUser , cUser);
  return send_cmd_packet(pv, pv->m_sHost, M_USER, S_REALTIME_LOAD, m_CFlag, pUser, USER_STEP(*pUser));
}
//�� �� Զ��ʵʱ�ɼ��û�����
//�� ��
//cszDevAddr �豸��ַ
//cUser �û���Ϣ
//˵ �� �����¼���û�����ʱ����ͨ���ú������û����Ϸ��͵��������������������ɼ����û���������
//Զ���û��ɼ����ܣ�ʹ��ֱ���ڹ���˾Ϳ��Բɼ��û��������Զ����Ƶ������²ɼ��Ŀ��ӻ��̶Ⱥ�ֱ���ڻ����ϲɼ�һ����
//���ʹ����(�û��ɼ�ʵʱͬ��)��Զ�̲ɼ����û�Ҳ��ʵʱ�ķ��ع����.
BOOL CPM_ULRealTimeUserCap(pmdev_t* pv, const DEV_USER* cUser)
{
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(USER), 4) >> 2] = {0};
  USER* pUser = (USER*)nData;
  ASSERT((cUser->m_FeatLen <= FEAT_SIZE) && (cUser->m_PicLen <= USER_JPEG_SIZE));

  if (!((cUser->m_FeatLen <= FEAT_SIZE) && (cUser->m_PicLen <= USER_JPEG_SIZE))) {
    return FALSE;
  }

  CopyDevUser2DspUser(pUser , cUser);
  return send_cmd_packet(pv, pv->m_sHost, M_USER, S_REALTIME_CAP, 0, pUser, USER_STEP(*pUser));
}
//�� �� ɾ��ָ���û�
//�� ��
//cszDevAddr �豸��ַ
//cID �û����
//nSeq �������к�
//˵ �� ɾ���ɹ����ͨ�������ص������������
//DevOperResultNotify�ص�������Ӧ���ͣ�
//eType = DEV_USER_OPER
//eFlag = DEV_OPER_DELETE��
//pvContent = NULL
BOOL CPM_DELUser(pmdev_t* pv, const DEV_CID* cID)
{
  return send_cmd_packet(pv, pv->m_sHost, M_USER, S_DEL, 0, cID, sizeof(CID));
}
//�� �� ����ָ���û�
//�� ��
//cszDevAddr �豸��ַ
//cID �û�ID
//nFlag DL_DEV_USER_PIC �������û���Ƭ
// DL_DEV_USER_FEAT ��Ҫ������������
// DL_DEV_USER_PIC|DL_DEV_USER_FEAT=����
// Ϊ0Ĭ������������Ϣ
//nSeq �������к�
//˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
//eType = DEV_USER_OPER
//eFlag = DEV_OPER_DOWNLOAD��
//pvContent = DEV_USER*
BOOL CPM_DLSingleUser(pmdev_t* pv, const DEV_CID* cID, int nFlag)
{
  int m_CFlag = 0;

  if (nFlag & DL_DEV_PIC) {
    m_CFlag |= F_DL_PIC;
  }

  if (nFlag & DL_DEV_USER_FEAT) {
    m_CFlag |= F_DL_FEAT;
  }

  return send_cmd_packet(pv, pv->m_sHost, M_USER, S_GET, 0, cID, sizeof(CID));
}
//�� �� ���������û�
//�� �� cszDevAddr �豸��ַ
//˵ ��
BOOL CPM_DLAllUser(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_USER, S_GET, F_DL_ID | F_DL_ID_ALL, 0, 0);
}
//�� �� ����ĳһʱ����ڵ�ע���û�
//�� ��
//cszDevAddr �豸��ַ
//cStart ��ʼ����
//cEnd ��ֹ����
//˵ ��
BOOL CPM_DLSegTimeUser(pmdev_t* pv, const DEV_DATE* cStart, const DEV_DATE* cEnd)
{
  DEV_TIMESEG seg[1] = {0};
  seg->m_Start = *cStart;
  seg->m_End = *cEnd;
  return send_cmd_packet(pv, pv->m_sHost, M_USER, S_GET, F_DL_ID | F_DL_ID_ALL, seg, sizeof(TIMESE));
}
//�� �� ����ʶ���¼����
//�� ��
//cszDevAddr �豸��ַ
//nSeq ��������
//˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
//eType = DEV_RECORD_OPER
//eFlag = DEV_OPER_DOWNLOAD
//pvContent = DEV_DL_RECORD_ARRAY*
//�ο�DEV_DL_RECORD_ARRAY�ṹ��һ��Ϊһ�����䣬�ú�������ʶ���¼���������䡣
BOOL CPM_DLAllIdentifyRecord(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_VERIFY, S_GET, F_DL_ID | F_DL_ID_ALL, 0, 0);
}
//�� �� ����ָ��ʱ���ʶ���¼
//�� �� cszDevAddr �豸��ַ
//cStart ��ʼ����
//cEnd ��ֹ����
//˵ ��
BOOL CPM_DLSegTimeIdentifyRecord(pmdev_t* pv, const DEV_DATE* cStart, const DEV_DATE* cEnd)
{
  DEV_TIMESEG seg[1] = {0};
  seg->m_Start = *cStart;
  seg->m_End = *cEnd;
  return send_cmd_packet(pv, pv->m_sHost, M_USER, S_GET, F_DL_ID | F_ID_TIME_RANGE, seg, sizeof(TIMESE));
}
//�� �� ��ȡ����ʶ���¼/������¼ָ��ID�ĺ�������
//�� ��
//cszDevAddr �豸��ַ
//cRange ID ����
//flag DL_DEV_RECORD_PIC=��Ҫ���ؼ�¼��Ƭ
//bKnowRange true=�������ݣ� false=���伯��
//nSeq ��������
//˵ �� ��bKnowRange Ϊtrue�ú������ؾ�������ݣ�Ϊfalseʱ��ѯָ���������豸�ϵľ����С�� DevOperResultNotify�ص�������Ӧ���ͣ�
//��������ʱ
//pvContent = DEV_DL_RECORD_ARRAY*
//cRange.m_SID = ָ��SN
//cRange.m_Count = -1;
//cRange.m_bOpenRange = TRUE;
//bKnowRange = FALSE��flag = 1����Ч������
//���ؾ����¼ʱ
//DevOperResultNotify�ص�������Ӧ���ͣ�
//pvContent = DEV_RECORD*
BOOL CPM_DLRangeRec(pmdev_t* pv, const DEV_DL_RECORD_RANGE* cRange, int nFlag, BOOL bKnowRange DEFAULT(FALSE))
{
  int m_CFlag = 0;

  if (bKnowRange) {
    nFlag |= 0x80000000;
  }

  m_CFlag = (nFlag & 0x80000000) ? 0 : F_DL_ID;

  if (nFlag & DL_DEV_PIC) {
    m_CFlag |= F_DL_PIC;
  }

  return send_cmd_packet(pv, pv->m_sHost, M_RANGE, S_GET, m_CFlag, cRange, sizeof(RECORDRANGE));
}
//�� �� ���ر�����¼����
//�� ��
//cszDevAddr �豸��ַ
//nSeq ��������
//˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
//eType = DEV_ALARM_OPER
//eFlag = DEV_OPER_DOWNLOAD��
//pvContent = DEV_DL_RECORD_ARRAY*
//�ο�DEV_DL_RECORD_ARRAY�ṹ��������¼ֻ��һ������
BOOL CPM_DLAllAlarmRecord(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_ALARM, S_GET, F_DL_ID | F_DL_ID_ALL, 0, 0);
}
//�� �� ����ָ��ʱ��α�����¼
//�� �� cszDevAddr �豸��ַ
//cStart ��ʼ����
//cEnd ��ֹ����
//˵ ��
BOOL CPM_DLSegTimeAlarmRecord(pmdev_t* pv, const DEV_DATE* cStart, const DEV_DATE* cEnd)
{
  DEV_TIMESEG seg[1] = {0};
  seg->m_Start = *cStart;
  seg->m_End = *cEnd;
  return send_cmd_packet(pv, pv->m_sHost, M_ALARM, S_GET, F_DL_ID | F_ID_TIME_RANGE, seg, sizeof(TIMESE));
}
//�� �� �������䱨����¼
//�� �� cszDevAddr �豸��ַ
//cRange ID����
//flag DL_DEV_RECORD_PIC=��Ҫ���ؼ�¼��Ƭ 0x00=��������Ƭ, 0x01������Ƭ
//bKnowRange ָ�������ؾ������ݻ�������ID����(true=�������ݣ� false=ID����)
//˵ �� �˽ӿ����ڴ�ĳһ����ȷ�ļ�¼ID��ʼ����nRang������
BOOL CPM_DLRangeAlarm(pmdev_t* pv, const DEV_DL_RECORD_RANGE* cRange, int nFlag, BOOL bKnowRange DEFAULT(FALSE))
{
  int m_CFlag = 0;

  if (bKnowRange) {
    nFlag |= 0x80000000;
  }

  m_CFlag = (nFlag & 0x80000000) ? 0 : F_DL_ID;

  if (nFlag & DL_DEV_PIC) {
    m_CFlag |= F_DL_PIC;
  }

  return send_cmd_packet(pv, pv->m_sHost, M_RANGE, S_GET, m_CFlag, cRange, sizeof(RECORDRANGE));
}
//�� �� ����������־
//�� �� cszDevAddr �豸��ַ
//˵ ��
BOOL CPM_DLLog(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_OPTLOG, S_GET, F_DL_ID | F_DL_ID_ALL, 0, 0);
}
//�� �� ����������־
//�� �� cszDevAddr �豸��ַ
//cRange ID����
//bKnowRange ָ�������ؾ������ݻ�������ID����(true=�������ݣ� false=ID����)
//˵ �� �˽ӿ����ڴ�ĳһ����ȷ�ļ�¼ID��ʼ����nRang������
BOOL CPM_DLRangeLog(pmdev_t* pv, const DEV_DL_RECORD_RANGE* cRange, int nFlag, BOOL bKnowRange DEFAULT(FALSE))
{
  int m_CFlag = 0;

  if (bKnowRange) {
    nFlag |= 0x80000000;
  }

  m_CFlag = (nFlag & 0x80000000) ? 0 : F_DL_ID;

  if (nFlag & DL_DEV_PIC) {
    m_CFlag |= F_DL_PIC;
  }

  return send_cmd_packet(pv, pv->m_sHost, M_RANGE, S_GET, m_CFlag, cRange, sizeof(RECORDRANGE));
}
//�� �� ��ȡ���ע���ռ�¼����
//�� ��
//cszDevAddr �豸��ַ
//nSeq ��������
//˵ ��
//DevOperResultNotify�ص�������Ӧ���ͣ�
//eType = DEV_CHANGEIMAGE_OPER
//eFlag = DEV_OPER_DOWNLOAD��
//pvContent = DEV_DL_RECORD_ARRAY*
//�ο�DEV_DL_RECORD_ARRAY�ṹ�����ע����ֻ��һ������
BOOL CPM_DLChangeImage(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_CIMAGE, S_GET, F_DL_ID | F_DL_ID_ALL, 0, 0);
}
//�� �� ����ָ������ı��ע���ռ�¼���߲�ѯ�������ݵľ����С
//�� ��
//cszDevAddr �豸��ַ
//cRange ����, �ο����䶨��
//flag DL_DEV_RECORD_PIC=��Ҫ���ؼ�¼��Ƭ
//bKnowRange true=�������ݣ� false=���伯��
//nSeq �������к�
//˵ �� ��bKnowRange Ϊtrue�ú������ؾ�������ݣ�Ϊfalseʱ��ѯָ���������豸�ϵľ����С��
//��������ʱ
//DevOperResultNotify�ص�������Ӧ���ͣ�
//pvContent = DEV_DL_RECORD_ARRAY*
//cRange.m_SID = ָ��SN
//cRange.m_Count = -1;
//cRange.m_bOpenRange = TRUE;
//bKnowRange = FALSE��flag = 1����Ч������
//���ؾ����¼ʱ
//DevOperResultNotify�ص�������Ӧ���ͣ�
//pvContent=DEV_RECORD*
BOOL CPM_DLRangeCImage(pmdev_t* pv, const DEV_DL_RECORD_RANGE* cRange, int nFlag, BOOL bKnowRange DEFAULT(FALSE))
{
  int m_CFlag = 0;

  if (bKnowRange) {
    nFlag |= 0x80000000;
  }

  m_CFlag = (nFlag & 0x80000000) ? 0 : F_DL_ID;

  if (nFlag & DL_DEV_PIC) {
    m_CFlag |= F_DL_PIC;
  }

  return send_cmd_packet(pv, pv->m_sHost, M_RANGE, S_GET, m_CFlag, cRange, sizeof(RECORDRANGE));
}
BOOL CPM_TimeGroup_impl(pmdev_t* pv, const DEV_TIMEGROUP_ARRAY* cArray, int m_SCmd, int nType)
{
#define TG_MAX_SIZE 256
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPTIMEG)*TG_MAX_SIZE, 4) >> 2] = {0};
  CARRAY* pArray = (CARRAY*)nData;
  DSPTIMEG* pTG = (DSPTIMEG*)(pArray->m_Block);
  int i, nLen = 0, m_CFlag = 0;
  ASSERT(cArray->m_nCount <= TG_MAX_SIZE);

  switch (nType) {
  case DEV_NORMAL:
    m_CFlag = F_DL_NORMAL_TG;
    break;

  case DEV_HOLIDY:
    m_CFlag = F_DL_HOLIDAY_TG;
    break;

  case DEV_ANY_TIME:
    m_CFlag = F_DL_NORMAL_TG | F_DL_HOLIDAY_TG;
    break;

  default:
    break;
  }

  pArray->m_Count = cArray->m_nCount;

  for (i = 0; i < pArray->m_Count; ++i) {
    pTG[i].m_TGID = (cArray->m_pTGArray)[i].m_TGID;
    memcpy(&(pTG[i].m_Start), &((cArray->m_pTGArray)[i].m_Start), sizeof(DATETIME));
    memcpy(&(pTG[i].m_End), &((cArray->m_pTGArray)[i].m_End), sizeof(DATETIME));
    pTG[i].m_TGType = (cArray->m_pTGArray)[i].m_TGType;
    pTG[i].m_CheckFlag = (cArray->m_pTGArray)[i].m_CheckFlag;
    pTG[i].m_WeekFlag = (cArray->m_pTGArray)[i].m_WeekFlag;
  }

  nLen = sizeof(CARRAY) + sizeof(DSPTIMEG) * (pArray->m_Count);
  return send_cmd_packet(pv, pv->m_sHost, M_TGROUP, m_SCmd, m_CFlag, pArray, nLen);
}
//�� �� ����ʱ����
//�� �� cszDevAddr �豸��ַ
//cTGArray ʱ����
//˵ ��
BOOL CPM_ULTimeGroup(pmdev_t* pv, const DEV_TIMEGROUP_ARRAY* cTGArray)
{
  return CPM_TimeGroup_impl(pv, cTGArray, S_SET, DEV_ANY_TIME);
}
//�� �� ��ȡʱ����
//�� �� dev_addr �豸��ַ
//cTGArray ʱ����
//eType ʱ��������
//˵ �� cTGArray.m_nCount=0ʱ��ʾ����eType���͵�����ʱ����
BOOL CPM_DLTimeGroup(pmdev_t* pv, const DEV_TIMEGROUP_ARRAY* cTGArray, DEV_TIMEGROUP_TYPE eType)
{
  return CPM_TimeGroup_impl(pv, cTGArray, S_GET, eType);
}
#define RT_MAX_SIZE 256
//�� �� ɾ��ʱ����
//�� �� cszDevAddr �豸��ַ
//cTGArray ʱ����
//eType ʱ��������
//˵ �� eType.m_nCount=0ʱ��ʾɾ��tg_type��������ʱ����
BOOL CPM_DELTimeGroup(pmdev_t* pv, const DEV_TIMEGROUP_ARRAY* cTGArray, DEV_TIMEGROUP_TYPE eType)
{
  return CPM_TimeGroup_impl(pv, cTGArray, S_DEL, eType);
}
BOOL CPM_Right_impl(pmdev_t* pv, const DEV_RIGHT_ARRAY* cArray, int m_SCmd)
{
  int i, nLen = 0;
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(DSPRIGHT)*RT_MAX_SIZE, 4) >> 2] = {0};
  CARRAY* pArray = (CARRAY*)(nData);
  DSPRIGHT* pRG = (DSPRIGHT*)(pArray->m_Block);
  ASSERT(cArray->m_nCount <= RT_MAX_SIZE);
  pArray->m_Count = cArray->m_nCount;

  for (i = 0; i < pArray->m_Count; ++i) {
    pRG[i].m_RightID = (cArray->m_pRtArray)[i].m_RightID;
    memcpy(pRG[i].m_TimeGID, (cArray->m_pRtArray)[i].m_TimeGID, sizeof(pRG[i].m_TimeGID));
    pRG[i].m_Mode = cArray->m_pRtArray[i].m_bHolidyValid ? CHECK_HOLIDY : 0;
    pRG[i].m_Mode |= cArray->m_pRtArray[i].m_bActionLock ? ACTION_LOCK : 0;
    pRG[i].m_Mode |= cArray->m_pRtArray[i].m_bActionOutPut ? ACTION_OUTPUT : 0;
  }

  nLen = sizeof(CARRAY) + sizeof(DSPRIGHT) * (pArray->m_Count);
  return send_cmd_packet(pv, pv->m_sHost, M_DSPRIGHT, m_SCmd, 0, pArray, nLen);
}
//�� �� ����Ȩ��
//�� �� cszDevAddr �豸��ַ
//cRTArray Ȩ��
//˵ ��
BOOL CPM_ULRight(pmdev_t* pv, const DEV_RIGHT_ARRAY* cRTArray)
{
  return CPM_Right_impl(pv, cRTArray, S_SET);
}
//�� �� ���ͻ�ȡȨ��
//�� �� cszDevAddr �豸��ַ
//cRTArray Ȩ��
//˵ �� ��rt_array.m_nCount=0ʱ��ʾ��������Ȩ��
BOOL CPM_DLRight(pmdev_t* pv, const DEV_RIGHT_ARRAY* cRTArray)
{
  return CPM_Right_impl(pv, cRTArray, S_GET);
}
//�� �� ɾ��Ȩ��
//�� �� cszDevAddr �豸��ַ
//cRTArray Ȩ��
//˵ �� ��rt_array.m_nCount=0ʱ��ʾɾ������Ȩ��
BOOL CPM_DELRight(pmdev_t* pv, const DEV_RIGHT_ARRAY* cRTArray)
{
  return CPM_Right_impl(pv, cRTArray, S_DEL);
}
#define UG_MAX_SIZE 256
BOOL CPM_UserGroup_impl(pmdev_t* pv, const DEV_USERGROUP_ARRAY* cArray, int m_SCmd)
{
  int i, nLen = 0;
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP)*UG_MAX_SIZE, 4) >> 2] = {0};
  CARRAY* pArray = (CARRAY*)(nData);
  USERGROUP* pUG = (USERGROUP*)(pArray->m_Block);
  DEV_USERGROUP* pTmp = cArray->m_pUGArray;
  ASSERT(cArray->m_nCount <= UG_MAX_SIZE);
  pArray->m_Count = cArray->m_nCount;

  for (i = 0; i < pArray->m_Count; ++i) {
    pUG->m_GroupID = pTmp->m_GroupID;
    pUG->m_GroupFlag = pTmp->m_bGroupOrder ;
    pUG->m_NormalValid = pTmp->m_NormalValid;
    pUG->m_ForceValid = pTmp->m_ForceValid;
    memcpy(pUG->m_NormalUsers, pTmp->m_NormalUsers, sizeof(pUG->m_NormalUsers));
    memcpy(pUG->m_ForceUsers, pTmp->m_ForceUsers, sizeof(pUG->m_ForceUsers));
    pUG++;
    pTmp++;
  }

  nLen = sizeof(NCMD) + sizeof(CARRAY) + sizeof(USERGROUP) * (pArray->m_Count);
  return send_cmd_packet(pv, pv->m_sHost, M_UGROUP, m_SCmd, 0, pArray, nLen);
}
//�� �� �����û���
//�� �� cszDevAddr �豸��ַ
//cUGrray �û���
//˵ ��
BOOL CPM_ULUserGroup(pmdev_t* pv, const DEV_USERGROUP_ARRAY* cUGrray)
{
  return CPM_UserGroup_impl(pv, cUGrray, S_SET);
}
//�� �� ���ͻ�ȡ�û���
//�� �� cszDevAddr �豸��ַ
//ug_array �û���
//˵ �� ��rt_array.m_nCount=0ʱ��ʾ���������û���
BOOL CPM_DLUserGroup(pmdev_t* pv, const DEV_USERGROUP_ARRAY* cUGrray)
{
  return CPM_UserGroup_impl(pv, cUGrray, S_GET);
}
//�� �� ɾ���û���
//�� �� cszDevAddr �豸��ַ
//ug_array �û���
//˵ �� ��rt_array.m_nCount=0ʱ��ʾɾ�������û���
BOOL CPM_DELUserGroup(pmdev_t* pv, const DEV_USERGROUP_ARRAY* cUGrray)
{
  return CPM_UserGroup_impl(pv, cUGrray, S_DEL);
}
//�� �� ���ÿ�����״̬
//�� ��
//cszDevAddr �豸��ַ
//bEOpen true=�������ţ� false=��������
//˵ ��
BOOL CPM_ULOpenDoorState(pmdev_t* pv, DEV_DOOR_STATE eState)
{
  return send_cmd_packet(pv, pv->m_sHost, M_DOOR_STATE, S_SET, eState, 0, 0);
}
//�� �� ��ȡ������״̬
//�� �� cszDevAddr �豸��ַ
//bEOpen true=�������ţ� false=��������
//˵ ��
BOOL CPM_DLOpenDoorState(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_DOOR_STATE, S_GET, 0, 0, 0);
}
//�� �� Զ�̿���
//�� �� cszDevAddr �豸��ַ
//bEOpen true=���ţ� false=����
//pDoorArray �ŵ�����
//size �����С
//˵ ��
BOOL CPM_ULRemoteOpenDoor(pmdev_t* pv, BOOL bOpen)
{
  return send_cmd_packet(pv, pv->m_sHost, M_REMOTE_OPEN, S_SET, bOpen ? F_DOOR_OPEN : F_DOOR_CLOSE, 0, 0);
}
//�� �� IO�豸����
//�� ��
//cszDevAddr �豸��ַ
//DEV_IOCTRL ������Ϣ
//nSeq ����ִ�����к�
//˵ �� DEV_IOCTRL:m_Sourceָ��Ҫ�������Ķ˿ڣ�DEV_IOCTRL:m_ActionҪִ�еĶ���
//����1��m_Source = 0x1f m_Action = DEV_ACT_IO_OPEN
//����1��m_Source = 0x15 m_Action = DEV_ACT_IO_OPEN
//0x1f��015������ô�����ģ���ο�485��ַ����
BOOL CPM_IOCtrl(pmdev_t* pv, const DEV_IOCTRL* ioctrl)
{
  return send_cmd_packet(pv, pv->m_sHost, M_IOCTRL, S_SET, 0, ioctrl, sizeof(DEV_IOCTRL));
}
//�� �� ��ʽ���豸
//�� �� cszDevAddr �豸��ַ
//˵ ��
BOOL CPM_ULFormat(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_FORMAT, S_SET, 0, 0, 0);
}
//�� �� �豸�������
//�� �� cszDevAddr �豸��ַ
//cAppData ������½ṹ��
//˵ ��
BOOL CPM_ULUpdate(pmdev_t* pv, const APPUPDATE* cAppData)
{
  int nLen = sizeof(APPUPDATE) + cAppData->m_Bytes;
  return send_cmd_packet(pv, pv->m_sHost, M_APPUPDATE, S_SET, 0, cAppData, nLen);
}
//�� �� �����ϴ��û�
//�� ��
//cszDevAddr �豸��ַ
//cUsers �����û�����
//nFlag DL_DEV_PIC //��Ƭ
// DL_DEV_USER_FEAT //��������
// DL_DEV_USER_TEXT //�û�������Ϣ
//˵ ��: �����ϴ��û���������û�������Ϣ���������
//nFlag = DL_DEV_USER_TEXT �ϴ�������Ϣ
//nFlag = DL_DEV_USER_TEXT|DL_DEV_PIC ����+ע����
//nFlag = DL_DEV_USER_TEXT|DL_DEV_USER_FEAT ����+������
//nFlag = DL_DEV_USER_TEXT|DL_DEV_PIC|DL_DEV_USER_FEAT����+ע����+������
//DevOperResultNotify�ص�������Ӧ���ͣ�
//eType = DEV_USER_EX_OPER
//eFlag = DEV_OPER_UPLOAD ��
//pvContent = DEV_BATCH_USER*��
//��OPER_SUCCESS ����£������ϴ����û������뷵�ص��û�����һ�¡������豸����ʧ�ܡ������ϴ��û����趨��5������(������������������ٶ��й�)��
BOOL CPM_ULUserEx(pmdev_t* pv, const DEV_BATCH_USER* cUsers, int nFlag)
{
  int ret = 0, nLen = 0;
  char* pbBuf = 0;

  if (nFlag & DL_DEV_USER_TEXT) {
    int binLen = 0 , i = 0;

    for (; i < cUsers->m_nCount; ++i) {
      binLen += ALIGN4(sizeof(USER) + cUsers->m_pUserArray[i].m_PicLen + cUsers->m_pUserArray[i].m_FeatLen);
    }

    pbBuf = (char*)malloc(ALIGN4(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + binLen));
    nLen = sizeof(NCMD) + sizeof(CARRAY);

    if (pbBuf) {
      CARRAY* pArray = (CARRAY*)(pbBuf);
      USER* pData = (USER*)(pArray->m_Block);
      DEV_USER* pCurUser = cUsers->m_pUserArray;
      pArray->m_Count = cUsers->m_nCount;

      for (i = 0; i < cUsers->m_nCount; ++i) {
        CopyDevUser2DspUser(pData , pCurUser + i);
        nLen += USER_STEP(*pData);
        pData = (USER*)((char*)pData + USER_STEP(*pData));
      }

      ret = send_cmd_packet(pv, pv->m_sHost, M_USER_EX, S_SET, USER_TEXT, pbBuf, nLen);
      free(pbBuf);
    }
  }
  else {
    ASSERT(FALSE);
  }

  return ret;
}
//�� �� ���������û�
//�� ��
//cszDevAddr �豸��ַ
//pUserIDArray �û�ID�����׵�ַ
//nIDCount ����Ԫ�ظ���
//nFlag ͬCPM_ULUserEx
//˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
//eType = DEV_USER_EX_OPER
//eFlag = DEV_OPER_DOWNLOAD ��
//pvContent= DEV_BATCH_USER*��
//OPER_SUCCESS ����£�����������ص��û�����Ҫ�����ص��û�����һ�£�����ͨ��
//CPM_DLAllUser(pmdev_t* pv) ��ȷ���û��Ƿ���Ĵ������豸�ϡ� ���������û����趨��5������(������������������ٶ��й�)��
BOOL CPM_DLUserEx(pmdev_t* pv, const DEV_CID* pUserIDArray, int nIDCount, int nFlag)
{
  enum {max_n = 5};
  char pbBuf[sizeof(CARRAY) + sizeof(CID) * max_n] = {0};
  int ret = 0, nLen = sizeof(CARRAY) + sizeof(CID) * max_n;
  int i, j, m_CFlag = 0;
  CARRAY* pArray = (CARRAY*)(pbBuf);
  CID* pID = (CID*)(pArray->m_Block);

  if (nFlag & DL_DEV_USER_TEXT) {
    m_CFlag |= USER_TEXT;
  }

  if (nFlag & DL_DEV_USER_FEAT) {
    m_CFlag |= DL_FEAT;
  }

  if (nFlag & DL_DEV_PIC) {
    m_CFlag |= DL_IMAGE;
  }

  for (j = 0; j < nIDCount;) {
    pArray->m_Count = MIN(max_n, nIDCount - j);

    for (i = 0; i < pArray->m_Count; ++i) {
      strncpy(pID[i].m_ID , pUserIDArray[j + i].m_ID , sizeof(CID) - 1);
    }

    ret = send_cmd_packet(pv, pv->m_sHost, M_USER_EX, S_GET, m_CFlag, pbBuf, nLen);
    j += pArray->m_Count;
  }

  return ret;
}
//�� �� ����ɾ���û�
//�� ��
//cszDevAddr �豸��ַ
//pUserIDArray ��ɾ���û��ı������
//nIDCount ����Ԫ�ظ���
//˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
//eType = DEV_USER_EX_OPER
//eFlag = DEV_OPER_DELETE��
//pvContent = DEV_BATCH_USER*��
//��OPER_SUCCESS ����£�����ɾ�����û������뷵�ص��û�������һ�£�����ͨ��CPM_DLAllUser(pmdev_t* pv)
//��ȷ���û��Ƿ���Ĵ������豸�ϡ���������ɾ���û����趨��10������Ϊ�á�
BOOL CPM_DELUserEx(pmdev_t* pv, const DEV_CID* pUserIDArray, int nIDCount)
{
  char* pbBuf = (char*)malloc(ALIGN4(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(CID) * nIDCount));
  int i, ret = 0, nLen = sizeof(CARRAY) + sizeof(CID) * nIDCount;
  int m_CFlag = 0;

  if (pbBuf) {
    CARRAY* pArray = (CARRAY*)(pbBuf);
    CID* pID = (CID*)(pArray->m_Block);
    const DEV_CID* pIDTmp = pUserIDArray;
    pArray->m_Count = nIDCount;

    for (i = 0; i < nIDCount; ++i) {
      ZeroMemory(pID , sizeof(CID));
      strncpy(pID->m_ID , pIDTmp->m_ID , sizeof(CID) - 1);
      ++pID;
      ++pIDTmp;
    }

    ret = send_cmd_packet(pv, pv->m_sHost, M_USER_EX, S_DEL, m_CFlag, pbBuf, nLen);
    free(pbBuf);
  }

  return ret;
}
//�� �� �������ؼ�¼
//�� �� cszDevAddr �豸��ַ
//cRange ID����
//˵ �� ������֤��¼��������¼��������־�����ע����
BOOL CPM_DLRecEx(pmdev_t* pv, const DEV_DL_RECORD_RANGE* cRange)
{
  return send_cmd_packet(pv, pv->m_sHost, M_RANGE_EX, S_GET, F_DL_PIC, cRange, sizeof(RECORDRANGE));
}
//�� �� �����豸
//�� �� cszDevAddr �豸��ַ
//˵ ��
BOOL CPM_RebootDev(pmdev_t* pv)
{
  return send_cmd_packet(pv, pv->m_sHost, M_REBOOT, S_SET, 0, 0, 0);
}
//�� �� ���/���� �豸������Ϣ
//�� �� cszDevAddr �豸��ַ
// pNetPro NULL��ʾ��ã���Ϊ�ձ�ʾ����
//˵ ��
BOOL CPM_NetPro(pmdev_t* pv, DEV_NETPRO* pNetPro)
{
  return send_cmd_packet(pv, pv->m_sHost, M_NETPRO, pNetPro ? S_SET : S_GET, 0, pNetPro, pNetPro ? sizeof(DEV_NETPRO) : 0);
}


void DspRec2UspRec(RECORD* dsp, DEV_RECORD* usp, BOOL realTime)
{
  usp->m_RecType = (DEV_REC_TYPE)(dsp->m_RecType);
  usp->m_bRealTime = realTime;
  usp->m_Score = dsp->m_Score;
  usp->m_PicLen = dsp->m_PicBytes;
  usp->m_VerifyMode = dsp->m_VerifyMode;
  usp->m_ConcretSource = dsp->m_Source;
  usp->m_Source = DEV_DOOR_SOURCE;
  usp->m_PicData = dsp->m_Block;
  memcpy(usp->m_SerialID.m_ID, dsp->m_SerialID.m_ID, sizeof(CID));
  memcpy(usp->m_UserID.m_ID, dsp->m_UserID.m_ID, sizeof(CID));
  memcpy(usp->m_ChangID.m_ID, dsp->m_ChangID.m_ID, sizeof(CID));
  memcpy(usp->m_AppendID.m_ID, dsp->m_AppendID.m_ID, sizeof(CID));
  memcpy(usp->m_UserName.m_ID, dsp->m_UserName.m_ID, sizeof(CID));
  memcpy(usp->m_Department.m_ID, dsp->m_Department.m_ID, sizeof(CID));
  memcpy(&(usp->m_RecTime), &(dsp->m_RecTime), sizeof(DATETIME));
}


int send_msg(pmdev_t* pv, const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  int i, MCmd = pCMD->m_MCmd;

  if (pCMD->m_CFlag & F_DL_ID) {
    switch (MCmd) {
    case M_USER: {
      DEV_DL_USER_RANGE Range = {0};

      if (0 == GET_ERR(pCMD->m_CFlag)) {
        CARRAY* pArray = (CARRAY*)(pCMD + 1);
        Range.m_Count = pArray->m_Count;
        Range.m_pArray = (DEV_VUSER*)(pArray->m_Block);
      }

      pv->m_eventfun(pv, cszIP, NULL, DEV_USER_RANGE_OPER, DEV_OPER_DOWNLOAD, &Range, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    case M_VERIFY:
    case M_ALARM:
    case M_OPTLOG:
    case M_CIMAGE:
    case M_RANGE: {
      DEV_OPER_TYPE type = DEV_RECORD_RANGE_OPER;
      DEV_DL_RECORD_ARRAY temp[1] = {0};

      switch (pCMD->m_MCmd) {
      case M_VERIFY:
        type = DEV_RECORD_RANGE_OPER;
        break;

      case M_ALARM:
        type = DEV_ALARM_RANGE_OPER;
        break;

      case M_OPTLOG:
        type = DEV_LOG_RANGE_OPER;
        break;

      case M_CIMAGE:
        type = DEV_CIMAGE_RANGE_OPER;
        break;

      default:
        break;
      }

      if (0 == GET_ERR(pCMD->m_CFlag)) {
        CARRAY* pArray = (CARRAY*)(pCMD + 1);
        temp->m_Count = pArray->m_Count;
        temp->m_pRange = (DEV_DL_RECORD_RANGE*)(pArray->m_Block);
      }

      pv->m_eventfun(pv, cszIP, NULL, type, DEV_OPER_DOWNLOAD, temp, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    default:
      ASSERT(0);
      break;
    }
  }
  else {
    switch (MCmd) {
    case M_AUTH:
      pv->m_Author = (pCMD->m_CFlag & F_AUTH_VERIFY);
      pv->m_eventfun(pv, cszIP, NULL, (pCMD->m_CFlag & F_AUTH_VERIFY) ? DEV_AUTH_OPER : DEV_AUTH_SET_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_DEVINFO:
      pv->m_eventfun(pv, cszIP, NULL, DEV_BASEINFO_OPER, DEV_OPER_DOWNLOAD, (DEV_BASEINFO*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_REBOOT:
      pv->m_eventfun(pv, cszIP, NULL, DEV_REBOOT_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_NETPRO:
      pv->m_eventfun(pv, cszIP, NULL, DEV_NETPRO_OPER, DEV_OPER_DOWNLOAD, (DEV_NETPRO*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_CIMAGE: {
      DEV_CHANGEIMAGE Image[1] = {0};
      ASSERT(pCMD);

      if (SUC_EXECUT == GET_ERR(pCMD->m_CFlag)) {
        CHANGEIMAGE* pRecord = (CHANGEIMAGE*)(pCMD + 1);
        Image->m_PicBytes = pRecord->m_PicBytes;
        Image->m_bRTChangeImage = (pCMD->m_SCmd == S_REALTIME_SEND) ? TRUE : FALSE;
        memcpy(Image->m_UserID.m_ID, pRecord->m_UserID.m_ID, sizeof(CID));
        memcpy(Image->m_SerialID.m_ID, pRecord->m_SerialID.m_ID, sizeof(CID));
        memcpy(Image->m_AppendID.m_ID, pRecord->m_AppendID.m_ID, sizeof(CID));
        memcpy(Image->m_UserName.m_ID, pRecord->m_UserName.m_ID, sizeof(CID));
        memcpy(Image->m_Department.m_ID, pRecord->m_Department.m_ID, sizeof(CID));
        memcpy(Image->m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
        memcpy(&(Image->m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
        Image->m_PicData = pRecord->m_Block;
      }
      else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
        memcpy(&(Image->m_SerialID), pCMD + 1, sizeof(CID));
      }

      pv->m_eventfun(pv, cszIP, NULL, DEV_CHANGEIMAGE_OPER, DEV_OPER_DOWNLOAD, Image, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    case M_OPTLOG: {
      DEV_LOG Log;
      ASSERT(pCMD);
      memset(&Log, 0x0, sizeof(DEV_LOG));

      if (0 == GET_ERR(pCMD->m_CFlag)) {
        DSPLOG* pRecord = (DSPLOG*)(pCMD + 1);
        Log.m_LogType = (DEV_LOG_TYPE)(pRecord->m_LogType);
        Log.m_bRTLog = (pCMD->m_SCmd == S_REALTIME_SEND) ? TRUE : FALSE;
        memcpy(Log.m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
        memcpy(Log.m_BeOptUser.m_ID, pRecord->m_BeOptUser.m_ID, sizeof(CID));
        memcpy(Log.m_SerialID.m_ID, pRecord->m_SerialID.m_ID, sizeof(CID));
        memcpy(&(Log.m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
      }
      else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
        memcpy(Log.m_SerialID.m_ID, pCMD + 1, sizeof(CID));
      }

      pv->m_eventfun(pv, cszIP, NULL, DEV_LOG_OPER, DEV_OPER_DOWNLOAD, &Log, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    case M_DOOR_STATE: {
      if (pCMD->m_SCmd == S_SET) {
        pv->m_eventfun(pv, cszIP, NULL, DEV_DOOR_STATE_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      else if (pCMD->m_SCmd == S_GET) {
        DEV_DOOR_STATE nStatus = DEV_DOOR_NOMAL;

        if (pCMD->m_CFlag & DEV_DOOR_OPEN) {
          nStatus = DEV_DOOR_OPEN;
        }
        else if (pCMD->m_CFlag & DEV_DOOR_CLOSE) {
          nStatus = DEV_DOOR_CLOSE;
        }
        else {
          nStatus = DEV_DOOR_NOMAL;
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_DOOR_STATE_OPER, DEV_OPER_DOWNLOAD, &nStatus, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
    }
    break;

    case M_REMOTE_OPEN:
      pv->m_eventfun(pv, cszIP, NULL, DEV_REMOTE_OPEN_DOOR, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_IOCTRL:
      pv->m_eventfun(pv, cszIP, NULL, DEV_IOCTRL_OPER, DEV_OPER_DOWNLOAD, (DEV_IOCTRL*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_NOPEN:
      pv->m_eventfun(pv, cszIP, NULL, DEV_NOPEN_OPER, DEV_OPER_DOWNLOAD, (DEV_NOPEN*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_APPUPDATE:
      pv->m_eventfun(pv, cszIP, NULL, DEV_UPDATE_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      break;

    case M_VERIFY: {
      DEV_RECORD Tmp[1] = {0};
      ASSERT(pCMD);

      if (0 == GET_ERR(pCMD->m_CFlag)) {
        DspRec2UspRec((RECORD*)(pCMD + 1), Tmp, pCMD->m_SCmd == S_REALTIME_SEND);
      }
      else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
        memcpy(Tmp->m_SerialID.m_ID, pCMD + 1, sizeof(CID));//??
      }

      pv->m_eventfun(pv, cszIP, NULL, DEV_RECORD_OPER, DEV_OPER_DOWNLOAD, Tmp, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    case M_ALARM: {
      DEV_RECORD Tmp[1];
      ASSERT(pCMD);
      memset(Tmp, 0x0, sizeof(DEV_RECORD));

      if (0 == GET_ERR(pCMD->m_CFlag)) {
        DspRec2UspRec((RECORD*)(pCMD + 1), Tmp, pCMD->m_SCmd == S_REALTIME_SEND);
      }
      else {
        memcpy(Tmp->m_SerialID.m_ID, pCMD + 1, sizeof(CID));
      }

      pv->m_eventfun(pv, cszIP, NULL, DEV_ALARM_OPER, DEV_OPER_DOWNLOAD, Tmp, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
    }
    break;

    case M_RANGE_EX:

    {
      ASSERT(pCMD);

#define BATCH_MAX 200

      if (SUC_EXECUT == GET_ERR(pCMD->m_CFlag)) {
        CARRAY* pArray = (CARRAY*)(pCMD + 1);
        int i;

        switch (pCMD->m_SCmd) {
        case M_VERIFY:
        case M_ALARM:
          if (1) {
            RECORD* pData = (RECORD*)(pArray->m_Block);
            DEV_BATCH_RECORD records = {0};
            DEV_RECORD record[BATCH_MAX];
            records.m_nCount = pArray->m_Count;
            records.m_pRecordArray = record;

            for (i = 0; i < pArray->m_Count; ++i) {
              DspRec2UspRec(pData, record + i, FALSE);
              pData = (RECORD*)((char*)pData + RECORD_STEP(*pData));
            }

            if (pCMD->m_CFlag & BATCH_END) {
              pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
            }

            pv->m_eventfun(pv, cszIP, NULL, DEV_RECORD_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
          }

          break;

        case M_OPTLOG: {
          DSPLOG* pRecord = (DSPLOG*)(pArray->m_Block);
          DEV_BATCH_LOG records = {0};
          DEV_LOG record[BATCH_MAX];
          records.m_nCount = pArray->m_Count;
          records.m_pLogArray = record;

          for (i = 0; i < pArray->m_Count; ++i) {
            DEV_LOG* Log = record + i;
            Log->m_LogType = (DEV_LOG_TYPE)(pRecord->m_LogType);
            Log->m_bRTLog = FALSE;
            memcpy(Log->m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
            memcpy(Log->m_BeOptUser.m_ID, pRecord->m_BeOptUser.m_ID, sizeof(CID));
            memcpy(Log->m_SerialID.m_ID, pRecord->m_SerialID.m_ID, sizeof(CID));
            memcpy(&(Log->m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
            pRecord = (DSPLOG*)((char*)pRecord + DSPLOG_STEP(*pRecord));
          }

          if (pCMD->m_CFlag & BATCH_END) {
            pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
          }

          pv->m_eventfun(pv, cszIP, NULL, DEV_LOG_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        }
        break;

        case M_CIMAGE: {
          CHANGEIMAGE* pRecord = (CHANGEIMAGE*)(pArray->m_Block);
          DEV_BATCH_CIMAGE records = {0};
          DEV_CHANGEIMAGE record[BATCH_MAX];
          records.m_nCount = pArray->m_Count;
          records.m_pCImageArray = record;

          for (i = 0; i < pArray->m_Count; ++i) {
            DEV_CHANGEIMAGE* Image = record + i;
            Image->m_PicBytes = pRecord->m_PicBytes;
            Image->m_PicData = pRecord->m_Block;
            Image->m_bRTChangeImage = (pCMD->m_SCmd == S_REALTIME_SEND) ? TRUE : FALSE;
            memcpy(&(Image->m_UserID), &(pRecord->m_UserID), sizeof(CID));
            memcpy(&(Image->m_SerialID), &(pRecord->m_SerialID), sizeof(CID));
            memcpy(Image->m_AppendID.m_ID, pRecord->m_AppendID.m_ID, sizeof(CID));
            memcpy(Image->m_UserName.m_ID, pRecord->m_UserName.m_ID, sizeof(CID));
            memcpy(Image->m_Department.m_ID, pRecord->m_Department.m_ID, sizeof(CID));
            memcpy(Image->m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
            memcpy(&(Image->m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
            pRecord = (CHANGEIMAGE*)((char*)pRecord + CGIMG_STEP(*pRecord));
          }

          if (pCMD->m_CFlag & BATCH_END) {
            pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
          }

          pv->m_eventfun(pv, cszIP, NULL, DEV_CIMAGE_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        }
        break;

        default:
          break;
        }
      }
      else {
        switch (pCMD->m_SCmd) {
        case M_VERIFY:
        case M_ALARM:
          pv->m_eventfun(pv, cszIP, NULL, DEV_RECORD_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
          break;

        case M_OPTLOG:
          pv->m_eventfun(pv, cszIP, NULL, DEV_LOG_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
          break;

        case M_CIMAGE:
          pv->m_eventfun(pv, cszIP, NULL, DEV_CIMAGE_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
          break;

        default:
          break;
        }
      }
    }
    break;

    case M_MCAST: {
      ASSERT(pCMD);

      switch (pCMD->m_SCmd) {
      case S_SET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_REGION_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_GET:
        //GetRgn(cszIP, pCMD, bServer, pvClientData);
        break;

      default:
        break;
      }

    }
    break;

    case M_DSPRIGHT: {
      ASSERT(pCMD);

      switch (pCMD->m_SCmd) {
      case S_SET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_RIGHT_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_GET: {
        int i;
#define TG_MAX_SIZE 256
        DEV_RIGHT_ARRAY r_array = {0};
        DEV_RIGHT RG[TG_MAX_SIZE] = {0};

        if (0 == GET_ERR(pCMD->m_CFlag)) {
          CARRAY* pArray = (CARRAY*)(pCMD + 1);
          DSPRIGHT* pRG = (DSPRIGHT*)(pArray->m_Block);
          r_array.m_nCount = pArray->m_Count > TG_MAX_SIZE ? TG_MAX_SIZE : pArray->m_Count;
          r_array.m_pRtArray = RG;
          ASSERT(pArray->m_Count <= TG_MAX_SIZE);

          for (i = 0; i < r_array.m_nCount; ++i) {
            RG[i].m_RightID = pRG[i].m_RightID;
            memcpy(RG[i].m_TimeGID, pRG[i].m_TimeGID, sizeof(pRG[i].m_TimeGID));
            RG[i].m_bHolidyValid = (BOOL)(pRG[i].m_Mode & CHECK_HOLIDY);
            RG[i].m_bActionLock = (BOOL)(pRG[i].m_Mode & ACTION_LOCK);
            RG[i].m_bActionOutPut = (BOOL)(pRG[i].m_Mode & ACTION_OUTPUT);
          }
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_RIGHT_OPER, DEV_OPER_DOWNLOAD, &r_array, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      break;

      case S_DEL:
        pv->m_eventfun(pv, cszIP, NULL, DEV_RIGHT_OPER, DEV_OPER_DELETE, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;
      }
    }

    break;

    case M_DEVSET: {
      ASSERT(pCMD);

      switch (pCMD->m_SCmd) {
      case S_SET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_WORKATT_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_GET: {
        DEV_WORKATT att[1] = {0};

        if (0 == GET_ERR(pCMD->m_CFlag)) {
          DEVSET* pSet = (DEVSET*)(pCMD + 1);
          memcpy(att->m_TimeGID, pSet->m_DevCtx, sizeof(pSet->m_DevCtx));
          att->m_BaseSet = pSet->m_BaseSet;
          att->m_DoorMangetTime = pSet->m_DoorMangetTime;
          att->m_LockTime = pSet->m_LockTime;
          att->m_VerifyMode = pSet->m_VerifyMode;
          att->m_bWGOutPut = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_YES);
          att->m_nWGType = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_34);
          att->m_nWGOutType = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_CTX);
          att->m_nWGOutIDType = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_UID);
          att->m_nWGOutContent = pSet->m_OutputCtx;
          att->m_DefaultRight = (DEV_DEFAULT_RIGHT)(pSet->m_DefaultRight);
          memcpy(att->m_szSuperPWD.m_ID, pSet->m_SuperPassword.m_ID, sizeof(DEV_CID));
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_WORKATT_OPER, DEV_OPER_DOWNLOAD, att, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      break;

      default:
        break;
      }
    }
    break;

    case M_STATIS:
      pv->m_eventfun(pv, cszIP, NULL, DEV_STATISINFO_OPER, DEV_OPER_DOWNLOAD, (DEV_STATIS*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag) , pvClientData);
      break;

    case M_TIME: {
      ASSERT(pCMD);

      switch (pCMD->m_SCmd) {
      case S_SET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_SYS_TIME_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_GET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_SYS_TIME_OPER, DEV_OPER_DOWNLOAD, (DEV_DATETIME*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      default:
        break;
      }
    }
    break;

    case M_TGROUP: {
      ASSERT(pCMD);

      switch (pCMD->m_SCmd) {
      case S_SET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_GET: {
#define TG_MAX_SIZE 256

        if (OPER_SUCCESS == GET_ERR(pCMD->m_CFlag)) {
          CARRAY* pArray = (CARRAY*)(pCMD + 1);
          DSPTIMEG* pTG = (DSPTIMEG*)(pArray->m_Block);
          DEV_TIMEGROUP_ARRAY t_array = {0};
          DEV_TIMEGROUP TG[TG_MAX_SIZE] = {0};
          ASSERT(pArray->m_Count <= TG_MAX_SIZE);
          t_array.m_nCount = pArray->m_Count > TG_MAX_SIZE ? TG_MAX_SIZE : pArray->m_Count;
          t_array.m_pTGArray = TG;

          for (i = 0; i < t_array.m_nCount; ++i) {
            TG[i].m_TGID = pTG[i].m_TGID;
            memcpy(&(TG[i].m_Start), &(pTG[i].m_Start), sizeof(DATETIME));
            memcpy(&(TG[i].m_End), &(pTG[i].m_End), sizeof(DATETIME));
            TG[i].m_TGType = (DEV_TIMEGROUP_TYPE)(pTG[i].m_TGType);
            TG[i].m_CheckFlag = pTG[i].m_CheckFlag;
            TG[i].m_WeekFlag = pTG[i].m_WeekFlag;
          }

          pv->m_eventfun(pv, cszIP, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DOWNLOAD, &t_array, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        }
        else {
          pv->m_eventfun(pv, cszIP, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        }

#undef TG_MAX_SIZE
      }
      break;

      case S_DEL:
        pv->m_eventfun(pv, cszIP, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DELETE, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;
      }
    }
    break;

    case M_USER: {
      ASSERT(pCMD);

      switch (pCMD->m_SCmd) {
      case S_SET: {
        DEV_USER user = {0};
        memcpy(user.m_UserID.m_ID, pCMD + 1, sizeof(CID));
        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_OPER, DEV_OPER_UPLOAD, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      break;

      case S_DEL: {
        DEV_USER user = {0};
        memcpy(user.m_UserID.m_ID, pCMD + 1, sizeof(CID));
        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_OPER, DEV_OPER_DELETE, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      break;

      case S_REALTIME_LOAD: {
        DEV_USER_REAL_LOAD mode = {0};

        if (pCMD->m_CFlag & F_DL_PIC) {
          mode.m_LoadFlag |= DL_DEV_PIC;
        }

        if (pCMD->m_CFlag & F_DL_FEAT) {
          mode.m_LoadFlag |= DL_DEV_USER_FEAT;
        }

        mode.m_ReqSource = pCMD->m_Param1;
        mode.m_bApeendID = (pCMD->m_CFlag & F_DL_APPENDID) ? TRUE : FALSE;
        memcpy(mode.m_ID.m_ID, pCMD + 1, sizeof(CID));
        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_RT_OPER, DEV_OPER_UPLOAD, &mode, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      break;

      case S_GET:
      case S_REALTIME_SEND:
      case S_UPDATE_SEND: {
        DEV_USER user = {0};

        if (0 == GET_ERR(pCMD->m_CFlag)) {
          USER* pUser = (USER*)(pCMD + 1);
          memcpy(&user, pUser, OFF_SET(USER, m_UserType));
          user.m_UserType = (DEV_USER_TYPE)pUser->m_UserType;
          user.m_FeatData = pUser->m_Block; //��������
          user.m_PicData = pUser->m_Block + pUser->m_FeatBytes; //��Ƭ����

          switch (pCMD->m_SCmd) {
          case S_REALTIME_SEND:
            user.m_bRTUser = 1;
            break;

          case S_UPDATE_SEND:
            user.m_bRTUser = 2;
            break;
          }
        }
        else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
          memcpy(user.m_UserID.m_ID, pCMD + 1, sizeof(CID));
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_OPER, DEV_OPER_DOWNLOAD, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      break;

      case S_REALTIME_CAP: {
        DEV_USER user = {0};

        if (0 == pCMD->m_Param1) {
          if (0 == GET_ERR(pCMD->m_CFlag)) {
            USER* pUser = (USER*)(pCMD + 1);
            memcpy(&user, pUser, OFF_SET(USER, m_UserType));
            user.m_UserType = (DEV_USER_TYPE)pUser->m_UserType;
            user.m_FeatData = pUser->m_Block; //��������
            user.m_PicData = pUser->m_Block + pUser->m_FeatBytes; //��Ƭ����
            user.m_bRTUser = 1;
          }
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_REMOTE_CAP, DEV_OPER_UPLOAD, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }
      break;

      default:
        break;
      }
    }
    break;

    case M_USER_EX: {
      switch (pCMD->m_SCmd) {
      case S_SET: {
        DEV_BATCH_USER users = {0};

        if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
          CARRAY* pArray = (CARRAY*)(pCMD + 1);
          users.m_nCount = pArray->m_Count;

          if (users.m_nCount) {
            CID* pID;
            DEV_USER* pUser;
            users.m_pUserArray = (DEV_USER*)malloc(ALIGN4(sizeof(DEV_USER) * users.m_nCount));
            pID = (CID*)(pArray->m_Block);
            pUser = users.m_pUserArray;

            for (i = 0; i < pArray->m_Count; ++i) {
              ZeroMemory(pUser , sizeof(DEV_USER));
              strncpy(pUser->m_UserID.m_ID , pID->m_ID , sizeof(CID) - 1);
              ++pUser;
              ++pID;
            }
          }
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_EX_OPER, DEV_OPER_UPLOAD, &users, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);

        if (users.m_pUserArray) {
          free((char*)users.m_pUserArray);
        }
      }
      break;

      case S_DEL: {
        DEV_BATCH_USER users;

        if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
          CARRAY* pArray = (CARRAY*)(pCMD + 1);
          users.m_nCount = pArray->m_Count;

          if (users.m_nCount) {
            CID* pID;
            DEV_USER* pUser;

            users.m_pUserArray = (DEV_USER*)malloc(ALIGN4(sizeof(DEV_USER) * users.m_nCount));
            pID = (CID*)(pArray->m_Block);
            pUser = users.m_pUserArray;

            for (i = 0; i < pArray->m_Count; ++i) {
              ZeroMemory(pUser, sizeof(DEV_USER));
              strncpy(pUser->m_UserID.m_ID , pID->m_ID , sizeof(CID) - 1);
              ++pUser;
              ++pID;
            }
          }
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_EX_OPER, DEV_OPER_DELETE, &users, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        free((char*)users.m_pUserArray);
      }
      break;

      case S_GET: {
        DEV_BATCH_USER users = {0};
        CARRAY* pArray = (CARRAY*)(pCMD + 1);

        if (OPER_SUCCESS == GET_ERR(pCMD->m_CFlag)) {
          users.m_nCount = pArray->m_Count;
          users.m_pUserArray = (DEV_USER*)malloc(ALIGN4(sizeof(DEV_USER) * pArray->m_Count));

          if (users.m_pUserArray) {
            if (pCMD->m_CFlag & USER_TEXT) {
              USER* pData = (USER*)(pArray->m_Block);
              DEV_USER* pCurUser = users.m_pUserArray;

              for (i = 0; i < pArray->m_Count; ++i) {
                memcpy(pCurUser, pData, OFF_SET(USER , m_UserType));
                pCurUser->m_UserType = (DEV_USER_TYPE)pData->m_UserType;
                pCurUser->m_FeatData = pData->m_Block;  //��������
                pCurUser->m_PicData = pData->m_Block + pData->m_FeatBytes; //��Ƭ����
                pData = (USER*)((char*)pData + USER_STEP(*pData));
                ++pCurUser;
              }
            }
            else {
              ASSERT(FALSE);
            }
          }

          if (pCMD->m_CFlag & BATCH_END) {
            pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
          }
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_USER_EX_OPER, DEV_OPER_DOWNLOAD, &users, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);

        if (users.m_pUserArray) {
          free((char*)users.m_pUserArray);
        }
      }
      break;
      }
    }
    break;

    case M_UGROUP: {
      ASSERT(pCMD);

      switch (pCMD->m_SCmd) {
      case S_SET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_USERGROUP_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_DEL:
        pv->m_eventfun(pv, cszIP, NULL, DEV_RIGHT_OPER, DEV_OPER_DELETE, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_GET: {
#define TG_MAX_SIZE 256
        DEV_USERGROUP_ARRAY u_array = {0};
        DEV_USERGROUP UG[TG_MAX_SIZE] = {0};

        if (OPER_SUCCESS == GET_ERR(pCMD->m_CFlag)) {
          CARRAY* pArray = (CARRAY*)(pCMD + 1);
          USERGROUP* pUG = (USERGROUP*)(pArray->m_Block);
          ASSERT(pArray->m_Count <= TG_MAX_SIZE);
          u_array.m_nCount = pArray->m_Count > TG_MAX_SIZE ? TG_MAX_SIZE : pArray->m_Count;
          u_array.m_pUGArray = UG;

          for (i = 0; i < u_array.m_nCount; ++i) {
            UG[i].m_GroupID = pUG[i].m_GroupID;
            UG[i].m_bGroupOrder = pUG[i].m_GroupFlag ;
            UG[i].m_NormalValid = pUG[i].m_NormalValid;
            UG[i].m_ForceValid = pUG[i].m_ForceValid;
            memcpy(UG[i].m_NormalUsers, pUG[i].m_NormalUsers, sizeof(pUG[i].m_NormalUsers));
            memcpy(UG[i].m_ForceUsers, pUG[i].m_ForceUsers, sizeof(pUG[i].m_ForceUsers));
          }
        }

        pv->m_eventfun(pv, cszIP, NULL, DEV_USERGROUP_OPER, DEV_OPER_DOWNLOAD, &u_array, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
      }

#undef TG_MAX_SIZE
      break;

      }
    }
    break;

    case M_VIDEO_OUT: {
      switch (pCMD->m_SCmd) {
      case S_SET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_VIDEO_TRANSFER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      case S_GET:
        pv->m_eventfun(pv, cszIP, NULL, DEV_VIDEO_TRANSFER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
        break;

      default:
        break;
      }
    }
    break;

    default:
      printf("m_MCmd = %d ???\n", pCMD->m_MCmd);
      break;
    }
  }

  return 0;
}


//����ʵʱ���������߳�
static DWORD WINAPI RecvRealControlDataThread(pmdev_t* pv)
{
  enum {pack_size = sizeof(SOCK_DATA_HEADER) + PACK_SIZE};
  int i, nLen = 0;
  int nCmd = 0, err = 0;

  enum { maxbufnum = 32 };
  databuf_t databuf[maxbufnum] = {0};

  while (!pv->m_exitThread) {
    //������������ݰ�
    databuf_t* it = recv_pack_data(pv->m_sHost, databuf, maxbufnum, 1, &err, 0, 0, 0);

    //printf("recv len %d\n", nLen);
    if (err < 0) {
      pv->m_eventfun(pv, pv->m_strDevIP, NULL, DEV_CONNECT_CUT, 0, 0, 0, err, pv->userdata);
      break;
    }

    if (it && it->buf) {
      NCMD* pCMD = (NCMD*)(it->buf);

      //����
      if (M_HEART == pCMD->m_MCmd) {
        if (SUC_EXECUT == GET_ERR(pCMD->m_CFlag)) {
          //���ͷ��
          send_cmd_packet(pv, pv->m_sHost, M_HEART, S_SET, 0, 0, 0);
        }
      }
      else {
        send_msg(pv, pv->m_strDevIP, pCMD, 0, pv->userdata);
      }
    }

    sleep(50);
  }

  for (i = 0; i < maxbufnum; ++i) {
    if (databuf[i].buf) {
      free(databuf[i].buf);
      databuf[i].buf = NULL;
    }
  }

  pv->m_exitThread = FALSE;
  return FALSE;
}

int _Connect(SOCKET s, const struct sockaddr FAR* name, int namelen, int ms)
{
  //set the socket in non-blocking mode
  int nResult;

  if (ms > 0) {
    struct timeval timeout = {ms / 1000, ms % 1000};
    unsigned long argp = 1;
    FD_SET fd = { 1, s };
    ioctlsocket(s, FIONBIO, &argp);
    nResult = connect(s, name, namelen);
    select(0, NULL, &fd, NULL, &timeout);

    //set the socket in blocking mode again.
    argp = 0;
    ioctlsocket(s, FIONBIO, &argp);
  }
  else {
    nResult = connect(s, name, namelen);
  }

  return nResult;
}

//��ʼ��
int CPM_Connect(pmdev_t* pv, const char* strDevIP, const char* sLoginName, const char* sPassword, event_fun_t eventfun, void* userdata)
{
  int nServAddlen, retVal;
  //���豸��������
  SOCKADDR_IN servAddr; //��������ַ

  netinit();

  //��ʼ��YVUתRGB�ı���ʾ��Ƶ�����ȳ�ʼ��

  pv->m_nIndex = 0;//��������
  pv->m_Author = FALSE; // ��½�豸Ȩ����֤ͨ����־
  pv->m_VideoClose = FALSE;// �ر���Ƶ��־
  pv->m_exitThread = FALSE;

  pv->userdata = userdata;
  pv->m_eventfun  = eventfun;
  strcpy(pv->m_strDevIP, strDevIP);

  //�����׽���
  pv->m_sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (INVALID_SOCKET == pv->m_sHost) {
    printf("socket failed!\n");
    return 0;
  }

  //���÷�������ַ
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr(pv->m_strDevIP);
  servAddr.sin_port = htons((short)TCPPORT);
  nServAddlen = sizeof(servAddr);

  //���ӷ�����
  retVal = connect(pv->m_sHost, (SOCKADDR*)&servAddr, sizeof(servAddr));
  //retVal = _Connect(pv->m_sHost, (SOCKADDR*)&servAddr, sizeof(servAddr), 500);

  if (SOCKET_ERROR == retVal) {
    closesocket(pv->m_sHost); //�ر��׽���
    pv->m_eventfun(pv, pv->m_strDevIP, NULL, DEV_CONNECT_FAILUE, 0, NULL, 0, OPER_ERR_EXECUT, pv->userdata);
    return 0;
  }

  pv->m_eventfun(pv, pv->m_strDevIP, NULL, DEV_CONNECT_SUCCESS, 0, NULL, 0, OPER_SUCCESS, pv->userdata);

  CPM_ULDevAuth(pv, sLoginName, sPassword, 1);
  //��������ʵʱ���������߳�
  thread_create(pv->m_hTCPThread, (thread_cb)RecvRealControlDataThread, (void*)pv);
  return retVal;
}

const char* getmyip()
{
  struct hostent* pHost;
  char szHost[256];
  static char strClientIP[20];

  //ȡ�ñ�����������
  if (gethostname(szHost, 256) == 0) {
    printf("host: %s\n", szHost);

    if ((pHost = gethostbyname(szHost)) != NULL) { //��Щ���ǻ��IP�ĺ���
      char* ip = inet_ntoa(*(struct in_addr*)*pHost->h_addr_list);
      printf("IP: %s\n", ip);
      strcpy(strClientIP, ip);
    }
  }

  return strClientIP;
}

/***********************************************************************************/
/*�� �� ������Ƶ
/*�� �� cszDevAddr �豸��ַ
hwnd ��Ƶ��ʾ���ھ��
/*˵ ��
/***********************************************************************************/
//typedef int (*frame_callback_t)(const char* cszIP, const unsigned char* cpbImage, int w, int h, int step, int nImageType, void* user);
int CPM_StartVideo(pmdev_t* pv, frame_fun_t framefun1, int cn, int flipud)
{
  const char* strClientIP = getmyip();

  if (!pv->m_Author) { //ͨ��Ȩ����֤
    return FALSE;
  }

  pv->cn = cn;
  pv->flipud = flipud;

  pv->m_framefun  = framefun1;

  {
    //����Ƶ������������Ƶ���ݽ����߳�
    //����������Ƶ���ݵ�SOKECT,�Ѿ�����������Ƶ���ݰ����̡߳�
    int nLen;
    //����������Ƶ���ݵ�SOKECT
    SOCKADDR_IN addrSrv;
    struct sockaddr_in addr = {0};
    pv->m_sVVIDEOSck = socket(AF_INET, SOCK_DGRAM, 0);
    //inet_ntoa(m_address.sin_addr);
    addrSrv.sin_addr.s_addr = inet_addr(strClientIP);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(0);//�����ȡһ�����õ�UDP�˿�
    bind(pv->m_sVVIDEOSck, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

    nLen = sizeof(struct sockaddr_in);

    if (SOCKET_ERROR == getsockname(pv->m_sVVIDEOSck, (struct sockaddr*)&addr, &nLen)) {
      return 0;
    }

    pv->m_nUDPPort = ntohs(addr.sin_port); //����Ѿ��󶨵Ķ˿ں�
    //����������Ƶ�����߳�
    thread_create(pv->m_hUDPThread, (thread_cb)RecvVideoDataThread, (void*)pv);
  }

  {
    int retVal;
    SOCK_ADDRESS Addr[1] = {0};
    //���豸�������Ƶ������
    //����������
    strcpy(Addr->IP_Address, strClientIP);
    Addr->Port = pv->m_nUDPPort;
    retVal = send_cmd_packet(pv, pv->m_sHost, M_VIDEO_OUT, S_SET, 0, Addr, sizeof(SOCK_ADDRESS));
  }

  return TRUE;
}

//ֹͣ��Ƶ
int CPM_StopVideo(pmdev_t* pv)
{
  int retVal;
  const char* strClientIP = getmyip();
  SOCK_ADDRESS Addr[1] = {0};
  //�ر���Ƶ

  //����������
  strcpy(Addr->IP_Address, strClientIP);
  Addr->Port = pv->m_nUDPPort;

  retVal = send_cmd_packet(pv, pv->m_sHost, M_VIDEO_OUT, S_GET, F_REQ_FLAG, Addr, sizeof(SOCK_ADDRESS));

  if (SOCKET_ERROR == retVal) {
    printf("send StopVideo failed!\n");
    closesocket(pv->m_sHost); //�ر��׽���
    return FALSE;
  }

  //�ر���ƵUDP�׽���
  if (pv->m_sVVIDEOSck) {
    closesocket(pv->m_sVVIDEOSck);
    pv->m_sVVIDEOSck = 0;
  }

  //�ر���Ƶ���ݴ����߳�
  if (pv->m_hUDPThread) {
    pv->m_VideoClose = TRUE;

    thread_join(pv->m_hUDPThread, 5000);
    thread_detach(pv->m_hUDPThread);
  }

  return TRUE;
}

//�Ͽ�����
void CPM_DisConnect(pmdev_t* pv)
{
  //�رպ��豸������
  if (pv->m_sHost) {
    closesocket(pv->m_sHost); //�ر��׽���
    pv->m_sHost = 0;
  }

  //�ر�ʵʱ���������߳�
  if (pv->m_hTCPThread) {
    pv->m_exitThread = TRUE;

    thread_join(pv->m_hTCPThread, 5000);
    thread_detach(pv->m_hTCPThread);
  }

  pv->m_Author = FALSE;
}

#if 0
static DWORD WINAPI start_sdk_thd(pmdev_t* pv)
{
  pv->run = 1;

  for (; pv->run;) {
    sleep(1000);
  }

  CPM_StopVideo(pv);//�ر���Ƶ
  CPM_DisConnect(pv);//�ر�����
  netexit();//�ͷ��׽�����Դ
  return 0;
}

int start_sdk(pmdev_t* pv, thread_t* hthread, const char* strDevIP, const char* strClientIP, frame_fun_t framefun1, void* userdata, int cn, int flipud)
{
  CPM_Init(pv, strDevIP, strClientIP, framefun1, userdata, cn, flipud);
  CPM_Connect(pv);
  CPM_StartVideo(pv);
  thread_create(hthread, (thread_cb)start_sdk_thd, (void*)pv);
  return 0;
}
#endif