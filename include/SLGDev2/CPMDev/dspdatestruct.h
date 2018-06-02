
//1. ����MAC��ַ��MAC��6���ֽ� ǰ3���ֽڱ�ʾ������ݣ���3���ֽ��ɳ����Զ���
//   BYTE[0-2] = 11-03-31 , ������ݴ��� , ʮ�����Ʊ�ʾ
//   BYTE[3]   = ���(00-99),ʮ�����Ʊ�ʾ
//   BYTE[4-5] = ����(short ��)��֧���������� 0xFFFF = 65535̨
//
//2. SID���м�������16�����ַ���ʾ������: 11F0A4-2-20110604221310-FF������Ϊ
//    SN       = 11F0A4 , ��ʾ11���F0A4(61604)̨
//    TYPE     = 2 , M_VERIFY
//    DATETIME = 2011-06-04 22:13:10
//    ID       = FF , ��ʾ��ֵ255

#define ID_LEN 24
//���֧�ֵĹ���ģ��
#define SUPPORT_NET          0x00000001      //֧��socket����
#define SUPPORT_2FRONT       0x00000002      //֧��2ǰ��
#define SUPPORT_GSOAP        0x00000004      //֧��gsoap��ʽ��web����
#define SUPPORT_ENROLL       0x00000008      //֧��ר�òɼ�ģʽ
#define SUPPORT_MAINPIC      0x00000010      //֧�ִ��������޸�
#define SUPPORT_FINGER       0x00000020      //֧��ָ��ʶ��
#define SUPPORT_EXTHID       0x00010000      //֧������HID
#define WEB_LOADUSER         0x01   //�����û�
#define WEB_WRITEUSER        0x02   //д�û�
#define WEB_WRITEREC         0x04   //д��¼
#define WEB_SYNCTIME         0x08   //ͬ��ʱ��
#define WEB_JUDGE            0x10   //ʶ��ɹ����ɷ����Ȩ����֤
#define WEB_RECORD_TXT       0x20   //��¼�Զ��ϴ�ʱ��ֻ�ϴ�������Ϣ
#define WEB_ENABLE           0x80   //
//˵��: M��ʾ����������0x01 - 0x80 ��S��ʾ���������0x81-0xff
#define M_USER               0x01   //�û�
#define M_VERIFY             0x02   //��֤��¼
#define M_ALARM              0x03   //������¼
#define M_OPTLOG             0x04   //��־��¼
#define M_CIMAGE             0x05   //���ע���ռ�¼
#define M_RANGE              0x06   //���伯����������
#define M_AUTH               0x07   //�ͻ��������֤
#define M_TIME               0x08   //ϵͳʱ��
#define M_HEART              0x09   //������
#define M_DEVINFO            0x0a   //�豸������Ϣ
#define M_STATIS             0x0b   //ͳ����Ϣ
#define M_DEVSET             0x0c   //�豸��������
#define M_FORMAT             0x0d   //�豸��ʽ��
#define M_DOOR_STATE         0x0e   //��״̬���� ,��3��״̬��0����״̬ �� 1����״̬�� 2����״̬
#define M_REMOTE_OPEN        0x0f   //Զ�̿�����,
#define M_TGROUP             0x10   //ʱ����
#define M_DSPRIGHT           0x11   //Ȩ��
#define M_UGROUP             0x12   //�û���
#define M_MCAST              0x13   //�ಥ��ַ
#define M_APPUPDATE          0x14   //�������
#define M_IMGCHANGE          0x15   //��������ͼƬ����
#define M_VIDEO_OUT          0x23   //UDP������Ƶ���� NET_ADDRESS
#define M_USER_EX            0x24   //�����û��ϴ�����
#define M_REBOOT             0x25   //�豸����
#define M_RANGE_EX           0x26   //��¼��������
#define M_SET_IP             0x27   //Զ���޸�IP��ַ
#define M_NOPEN              0x28   //N+1��������
#define M_IOCTRL             0x29   //IO����
#define M_NETPRO             0x30   //��������
#define M_RESETSCREEN        0x32   //��ĻУ��
//������
#define S_SET                0x81   //�ϴ�\����
#define S_DEL                0x82   //ɾ��
#define S_GET                0x83   //����\���
#define S_REALTIME_SEND      0x84   //�ɼ�ʵʱ����
#define S_REALTIME_LOAD      0x85   //ʵʱ����
#define S_REALTIME_CAP       0x86   //ʵʱ�û��ɼ�
#define S_REALTIME_CAP_INFO  0x88   //Զ�̲ɼ�ʵ�ǽ�����Ϣ
#define S_UPDATE_SEND        0x87   //����ʵʱ����
#define S_CLEAR_INOUT        0x88   //���ͳ�Ƽ���
//�ͻ�������ת������������
#define C_CNDEV              0xB0          //clientͨ����ת�������豸����
#define C_VERIFY             0xB1          //��ת��������֤client���
#define C_HEART              0xB2          //��ת��������client��������
//�ͻ�������ת������������
#define S_CN                 0xc0          //��������
#define S_DN                 0xc1          //�Ͽ�����
//����ִ�д���״̬
#define SUC_EXECUT           0x00   //�ɹ�
#define ERR_EXECUT           0x0A   //ʧ��
#define ERR_BUSY             0x01   //�豸æ
#define ERR_LIMIT            0x02   //�Ѵ�����
#define ERR_NOFIND           0x03   //û���ҵ���Ӧ����
#define ERR_SAVE_FAIL        0x04   //���ݱ���ʧ��
#define ERR_SET_FAIL         0x05   //����ʧ��
#define ERR_VERIFY_FAIL      0x06   //��֤ʧ��
#define ERR_FROMAT           0x07   //��ʽ��ʧ��
#define ERR_PARAM            0x08   //��������
#define ERR_DISABLE          0x09   //Ҫ��ִ�еĹ���û��ʹ��
#define ERR_SUPPORT          0x10   //��֧�ֵ�����
#define ERR_INPUTDATA        0x11   //����˴�����������쳣
//���úͻ�ô���״̬
#define SET_ERR( state , err ) (state) = ( ((state) & 0xffffff00)+(err) )
#define GET_ERR( state ) ( (state) & 0xff )
#define SET_VMODE( cflag , vmode ) (cflag) = ( ((cflag)&0xff00ffff) + (((vmode)&0xff) << 16) )
#define GET_VMODE( cflag ) ( ((cflag)&0x00ff0000) >> 16 )
//GET/SET 0��3λ����
#define GET_TYPE_485( x ) ( (x)&0x0F )
#define SET_TYPE_485( x , v )  (x) = ( ((x)&0xF0) + (v) )
//GET/SET ���4��7λ����
#define GET_INDEX_485( x ) ( ( (x)&0xF0 ) >> 4 )
#define SET_INDEX_485( x , v ) ( (x) = ( ((x)&0x0F) + ((v)<<4)) )
#define H_BYTE( word ) ( ((word) >> 8) & 0xFF )
#define L_BYTE( word ) ( (word) & 0xFF )
#define W_BYTE( L, H ) ( ((H) << 8)|(L) )
#define CMD_STATE_REPLY       0x40000000   //��Ǹ�����Ϊ״̬�ظ�����
#define CMD_DOWN_ID           0x80000000   //��������ID����,���ȼ��ñ��
#define CMD_ID_ALL            0x00000100   //�����������ݵ����伯��
#define CMD_ID_TIME_RANGE     0x00000200   //�����ƶ�ʱ������伯��
#define CMD_ID_RANGE          0x00000400   //ȷ��ָ����������伯��
//��������
#define DL_IMAGE              0x00000100   //��Ҫ������Ƭ
#define DL_FEAT               0x00000200   //��Ҫ��������
#define DL_APPENDID           0x00000400   //ʵʱ����ָʾCID�Ǳ�Ż��ǿ���
#define LD_ENROLL             0x00000800   //��ҪԶ�̲ɼ�
#define USER_TEXT             0x00000400   //�û�������Ϣ
#define BATCH_END             0x00000800   //��������
//�û���֤
#define AUTH_VERIFY           0x00000100   //�û������֤
//��״̬: �����أ���ָ��Ϊ����״̬
#define DOOR_OPEN             0x00000100   //��
#define DOOR_CLOSE            0x00000200   //��
//ʱ��������
#define DL_NORMAL_TG          0x00000100   //��������ʱ����
#define DL_HOLIDAY_TG         0x00000200   //���ؽڼ���
// INET6_ADDRSTRLEN �ַ�������
//����ṹ�壬�����ϴ����ص���������,ͨ�������ݽṹ�������������
typedef struct {
  unsigned int m_MCmd;           //������
  unsigned int m_SCmd;           //������
  unsigned int m_CFlag;          //�ض��������ò��������һ���ֽ�Ϊ����ִ��״̬��
  unsigned int m_Param1;         //�û��Զ������1
  addr_inx m_Param2;         //�û��Զ������2
} NCMD;
//����
typedef struct {
  int   m_Year;
  char  m_Month;
  char  m_Day;
  char  m_Reserve[2];            //�ṹ��4�ֽڶ���
} NETDATE;
//ʱ��
typedef struct {
  int   m_Msec;                  //����
  char  m_Hour;
  char  m_Minute;
  char  m_Second;
  char  m_Reserve[1];
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
  NETDATE m_Start;              //��¼��ʼʱ��
  NETDATE m_End;                //��¼����ʱ��
} TIMESE;
#define DL_OPENINTERVAL 0x00000001    //����ָ����¼��ˮ��֮���N����¼������ָ����¼����
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
  int  m_Count;          //����Ԫ�صĸ���
  char m_Block[4];         //�����ַ, 4��Ϊ�˽ṹ����ֽڶ���
} CARRAY;
//��������
typedef struct {
  char m_WebPro1;
  char m_WebPro2;
  char m_WebUrl1[127];
  char m_WebUrl2[127];
  char m_IPAdr[IP_LEN];
  char m_MIPAdr[IP_LEN];
  char m_NetMask[IP_LEN];
  char m_GateWay[IP_LEN];
  char m_Dns[IP_LEN];
  char m_AuthUser[32];
  char m_AuthPSW[32];
} NETPRO;
#define CHECK_TIME   0x01     //���ʱ��
#define CHECK_WEEK   0x02     //�������
#define CHECK_DAY    0x04     //�������
#define CHECK_MONTH  0x08     //����·�
#define CHECK_YEAR   0x10     //������
#define WEEK_1       0x01     //����һ��Ч
#define WEEK_2       0x02     //���ڶ���Ч
#define WEEK_3       0x04     //��������Ч
#define WEEK_4       0x08     //��������Ч
#define WEEK_5       0x10     //��������Ч
#define WEEK_6       0x20     //��������Ч
#define WEEK_7       0x40     //��������Ч
//����ʱ����Ч , ����ʱ���飬�ڼ���ʱ����
enum { ANY_TIME = -1 , TG_NORMAL = 1, TG_HOLIDY = 2 };
//ʱ����
typedef struct {
  int      m_TGID;      //ʱ����ID
  DATETIME m_Start;     //ʱ���鿪ʼʱ��
  DATETIME m_End;       //ʱ�������ʱ��
  char     m_TGType;    //ʱ��������, TG_NORMAL,TG_HOLIDY
  char     m_CheckFlag; //ʱ����������m_CheckFlag = CHECK_TIME|CHECK_WEEK
  char     m_WeekFlag;  //�������ʱ�������Щ������Ч������m_WeekFlag = WEEK_1|WEEK_5
  char     m_Reserve;   //�����ֽ�
} DSPTIMEG;
//û��ָ���û�Ȩ��,�û�Ĭ��Ȩ��IDΪDEFAULT_NO_RIGHT����DEFAULT_SINGLE_OPEN,DSP�ϲɼ����û���
//��������Ա����ͳһ����ΪDSP_DEFAULT_SINGLE_OPEN
//Ĭ����Ȩ�ޣ�Ĭ�ϵ�һ����Ȩ�ޣ�����û�������û�����Ȩ��֧�����ʱ����
//DEFAULT_NO_RIGHT: DSP,����������û�,Ĭ��ֵ֮һ��Ȩ��
//DEFAULT_SINGLE_OPEN: DSP,����������û���Ĭ��ֵ֮����һ����Ȩ�ޣ���Ȩ��û��ʱ�������
enum { DEFAULT_NO_RIGHT = -2, DEFAULT_SINGLE_OPEN = -1, MAX_GROUPUSER = 60, TGROUP_NUM = 8 };
#define CHECK_HOLIDY      0x01     //Ȩ�޼��ڼ���
#define ACTION_LOCK       0x02     //�������
#define ACTION_OUTPUT     0x04     //�������
#define ACTION_N_1        0x08     //��Ȩ����ҪN+1
//Ȩ��
typedef struct {
  //Ȩ��ID
  int m_RightID;
  //ʱ����ID , (m_TimeGID[0]==ANY_TIME)δָ��ʱ���飬���ŷ�ʽ����ʱ�����ƣ�����ʱ�����֤�ɹ�ִ�п��ŷ�ʽ.
  int m_TimeGID[TGROUP_NUM];
  //BIT(0):1 ���ڼ���, �����ڼ���
  //BIT(1):1 ����, 1��� 0�����
  //BIT(2):1 ����, 1��� 0�����
  //BIT(3):1 N��1, ����  0�ر�
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
  CID  m_NormalUsers[MAX_GROUPUSER];
  //ǿ���û���ϣ����ȼ���
  CID  m_ForceUsers[MAX_GROUPUSER];
} USERGROUP;
typedef struct {
  int  m_GroupID;
  int  m_StrSize;
  char m_GroupFlag;
  char m_MinNum;
  char m_BLock[2];
} GROUPEX;
typedef struct {
  int      m_UserType;       //�ն��豸�ϵ��û�����(��ͨ���������� -- 0 , 1 , 2)
  CID      m_UserID;         //�û�ID
  DATETIME m_TypeUpdTime;    //�û����͸���ʱ��
  DATETIME m_RegistTime;     //����ע��ʱ�䣬�������ɼ�ʱ�䣬�ɼ��������²ɼ�Ҫ���´�ʱ��
  DATETIME m_LastUpdTime;    //�����µ�ʱ�䣬ʶ��ʱ�����������»������²ɼ�Ҫ���´�ʱ��
} VUSER;
//�û���Ϣ
#define USER_CARD_INVALID  0x01    //����ʧ
#define USER_DLine_VALID   0x02    //ʧЧʱ����Ч
#define USER_BLACK         0x04    //������
#define USER_MIX1N         0x08    //1N������֤
#define USER_VIPCARD       0x10    //VIP�û�
#define USER_FINGER_PRINT  0x20    //ָ����֤
#define USER_SYNC_FINGEER  0x40    //ָ��ͬ��
#define USER_MAX_MID       0x40    //���ó����е���ֵ
#define USER_MAX_LOW       0x80    //���ó����ϸ���ֵ
typedef struct {
  int  m_FeatBytes;          //���������ֽ�����==0 ��ʾû����������
  int  m_PicBytes;           //ע��ͼ����ֽ�����==0 ��ʾû��ͼƬ
  int  m_RightID;            //�û�Ȩ��ID,���ֿ���ֵ(DEFAULT_NO_RIGHT,DEFAULT_SINGLE_OPEN,�û�ָ��)
  CID  m_ImageSID;           //�ϴ�ע���գ���Ӧ�ı��ID��ȫ��Ψһ
  CID  m_UserID;             //�û���ʶ
  CID  m_Admin;              //����ע�����Ա,��ʶ���û��������������ĸ�����Ա�ɼ�
  CID  m_AppendID;           //���Ż�����������1:1�ĸ������ȷ����Ϣ
  CID  m_UserName;           //�û���
  CID  m_Department;         //��������
  DATETIME m_DeadLine;       //ʧЧʱ��
  DATETIME m_RegistTime;     //����ע��ʱ�䣬�������ɼ�ʱ�䣬�ɼ��������²ɼ�Ҫ���´�ʱ��
  DATETIME m_LastUpdTime;    //�����µ�ʱ�䣬ʶ��ʱ�����������»������²ɼ�Ҫ���´�ʱ��
  DATETIME m_TypeUpdTime;    //�û����ͱ��ʱ��
  char m_UserFlag;           //�û�״̬���
  char m_UserType;           //�ն��豸�ϵ��û�����(��ͨ���������� -- 0 , 1 , 2)
  char m_Block[2];           //ע��ͼƬ,����JPG,������������m_UserIDΨһ�󶨵�.m_Block��ǰm_FeatBytes���ֽڱ�ʾ��������, ��m_PicBytes���ֽڱ�ʾͼƬ.
} USER;
//��¼����:
//0x01 - 0x20: �����¼�
//0x20 - 0x40: Ӳ�������¼�
//0x40 - 0x50: ��������
//0x50 - 0x60: �������
//0xf0 - 0xff: ��������¼�
//����: δ����
enum {
  VERIFY_SUCC = 0x01,               //��֤�ɹ�
  VERIFY_FAIL = 0x02,               //��֤ʧ��
  ADMIN_SUCC  = 0x03,               //����Ա��֤�ɹ�
  EMER_OPEN   = 0x04,               //��������
  RIGHT_OPEN  = 0x05,               //Ȩ�޿���
  GROUP_OPEN  = 0x06,               //��Ͽ���
  BUTTON_OPEN = 0x07,               //��ť����
  ALARM_HD_MANGET_TIMEOUT = 0x20,   //�Ŵų�ʱ
  ALARM_HD_MANGET_ILLOPEN = 0x21,   //�ŴŷǷ�����
  ALARM_HD_OFFLINE  = 0x22,         //ǰ�˵��߱���
  ALARM_HD_BREAK    = 0x30,         //���𱨾�
  ALARM_HD_SHOCK    = 0x31,         //�𶯱���
  ALARM_HD_FPOWR    = 0x36,         //ǰ�˹����쳣����
  ALARM_HD_UPS_ON   = 0x37,         //UPS���õ�ؿ���
  ALARM_HD_UPS_OFF  = 0x38,         //UPS���õ�عر�
  ALARM_HD_ASSIST   = 0x40,         //�������봥��
  ALARM_HD_WL_DOOR_MAGNETIC = 0x41, //����
  ALARM_HD_WL_SMOKE    = 0x42,
  ALARM_HD_WL_GAS      = 0x43,
  ALARM_HD_WL_BUTTON1  = 0x44,
  ALARM_HD_WL_BUTTON2  = 0x45,
  ALARM_HD_WL_BUTTON3  = 0x46,
  ALARM_HD_WL_BUTTON4  = 0x47,
  ALARM_HD_WL_AREA1    = 0x48,
  ALARM_HD_WL_AREA2    = 0x49,
  ALARM_HD_WL_AREA3    = 0x4A,
  ALARM_HD_WL_AREA4    = 0x4B,
  ALARM_HD_WL_AREA5    = 0x4C,
  ALARM_HD_WL_AREA6    = 0x4D,
  ALARM_HD_WL_AREA7    = 0x4E,
  ALARM_LINK_OUT1           = 0x50,   //�������1
  ALARM_LINK_OUT2           = 0x51,   //�������2
  ALARM_LINK_GPRS           = 0x52,   //����GPRS
  ALARM_LINK_2OPEN          = 0x53,   //˫��
  ALARM_LINK_2CLOSE         = 0x54,   //˫��
  ALARM_LINK_2NORMAL        = 0x55,   //����
  ALARM_AB_RULE        = 0xE0,
  ALARM_SF_ILLREMOTE        = 0xE1,   //Զ�̼�����Ч
  ALARM_SF_ILLGROUP         = 0xE2,   //���ʧ�ܣ�����
  ALARM_SF_ILLCARD          = 0xE3,   //��Ч��
  ALARM_SF_ERRSTATE         = 0xE4,   //������״̬
  ALARM_SF_INTERLOCK        = 0xE5,   //AB����
  ALARM_SF_BACKLIST         = 0xF0,   //��������֤����
  ALARM_SF_LOSSCARD         = 0xF1,   //��ʧ��
  ALARM_SF_ILLTIME          = 0xF2,   //�Ƿ�ʱ��ʶ��
  ALARM_SF_DEADLINE         = 0xF3,   //ʧЧʱ��
  ALARM_SF_DANGER_OPEN      = 0xF4,   //в�ȿ���
  ALARM_SF_SUPER_OPEN       = 0xF5,   //�������뿪��
  ALARM_SF_CAP_SHAKE        = 0xF6,   //�ɼ�����
};
//��¼��Ϣ,ʶ���¼��������¼
typedef struct {
  int  m_RecType;                   //��¼����
  int  m_Score;                     //ʶ��÷� , �����¼�Ϊ�����¼���LINKID
  int  m_PicBytes;                  //��¼ͼ����ֽ�����==0 ��ʾû�м�¼ͼƬ
  CID  m_ChangID;                   //ע���ռ�¼��
  CID  m_UserID;                    //�û�ID,���Ϊ�գ���ʾ�Ƿ���¼ , �����¼�Ϊ�¼�����Text
  CID  m_SerialID;                  //ʶ���¼��ˮ��ID
  CID  m_AppendID;                  //���Ż�����������1:1�ĸ������ȷ����Ϣ
  CID  m_UserName;                  //�û���
  CID  m_Department;                //��������
  DATETIME m_RecTime;               //��¼ʱ��
  char m_VerifyMode;                //��֤ģʽ,��:VERIFY_CARD|VERIFY_FACE_1N,ˢ��+����
  char m_Source;                    //�¼�Դ�豸��485��ַ
  char m_Block[2];                  //��¼ͼ��,����JPG , 2��Ϊ�˽ṹ����ֽڶ���
} RECORD;
//������־����
enum {
  DSPLOG_ADDUSER = 0x01,            //�����û�
  DSPLOG_DELUSER = 0x02,            //ɾ���û�
  DSPLOG_REREGIST = 0x03,           //���²ɼ�
  DSPLOG_CAHNGETYPE = 0x04,         //�ı��û�����
  DSPLOG_UDISK_ULUSER = 0x05,       //U���ϴ��û�
  DSPLOG_UDISK_DLUSER = 0x06,       //U�������û�
  DSPLOG_UDISK_DLRECORD = 0x07 ,    //U�����ؼ�¼
  DSPLOG_UDISK_UPDATEAPP = 0x08,    //U�̸��³���
};
//�豸������־
typedef struct {
  int m_LogType;      //��־����
  CID m_SerialID;     //��ˮ��ID
  CID m_Admin;        //����Ա
  CID m_BeOptUser;    //������Ա
  DATETIME m_RecTime; //��¼ʱ��
} DSPLOG;
//���ע����
typedef struct {
  int  m_PicBytes;               //ע���մ�С
  CID  m_UserID;                 //�û�ID
  CID m_SerialID;                //��ˮ��ID
  CID  m_AppendID;               //���Ż�����������1:1�ĸ������ȷ����Ϣ
  CID  m_UserName;               //�û���
  CID  m_Department;             //��������
  CID  m_Admin;                  //����ע�����Ա,��ʶ���û��������������ĸ�����Ա�ɼ�
  DATETIME m_RecTime;            //��¼ʱ��
  char m_Block[4];               //ע�����ڴ�
} CHANGEIMAGE;
//DSP���ͳ����Ϣ
typedef struct {
  int  m_TotalUsers;             //��ǰ�豸�����û���
  int  m_NoFeatUser;             //û�вɼ������������û���
  int  m_TotalDays;              //ʶ���¼����������
  int  m_TotalRecords;           //�ܼ�¼��
  int  m_TotalAlarm;             //�ܱ�����¼��
  int  m_TotalDspLog;            //�ܲ�����־��Ŀ
  int  m_TotalChangeImage;       //�ܱ��ע����
} DEVSTATIS;
//��֤��Ϣ
typedef struct {
  CID m_UserName;                //�û���
  CID m_Password;                //����
} AUTH;
//�豸������Ϣ
typedef struct {
  int m_DevType;                 //�豸����
  int m_LimitUser;               //����������
  int m_Limit1NUser;             //1N�û�����
  CID m_SN;                      //�豸���
  CID m_Ver;                     //DSP����汾
  CID m_Space;                   //����������Ϣ
} DEVINFO;
//Զ�̲ɼ�������Ϣ
typedef struct {
  int  m_Total;               //�ɼ�����
  int  m_Count;               //��ǰ�ɼ���
  CID  m_UserID;                 //�û��ǼǱ��
} RCAPINFO;
//2.0��֧�ֵĹ���
#define RECORD_SAVEFAIL         0x00000001    //�Ƿ񱣴�ʶ��ʧ�ܼ�¼
#define SUPER_PASSWORD          0x00000002    //�������뿪����Ч
//#define HDBEEP_OPEN             0x00000004    //����/����(Ӳ����������������)
#define ARMED_ENABLE_ONCE       0x00000004    //��������ʱ������һ��
#define ENROLL_VERIFY           0x00000008    //�ɼ�ͬʱʶ��
#define REALTIME_RECORD         0x00000010    //ʵʱ��̬��¼��ʾ
#define REALTIME_USERLOAD       0x00000020    //�����Ż��߿���ʶ��ʱ������޷��ڵ�ǰdsp�л����������,����������״̬�£����ӹ����ʵʱ������������
#define REALTIME_USERSEND       0x00000040    //�ɼ��û�ͬ��
#define DOORMANGET_OPEN         0x00000080    //�����Ŵż�ⱨ��
#define DOORFORCE_OPEN          0x00000100    //����в�ȿ��Ź���
#define REMOTE_CAP_SAVE         0x00000200    //Զ�̿��Ʋɼ����û��Ƿ񱣴浽DSP
#define GPRS_OPEN               0x00000400    //����GPRSģ��
#define UPDATE_USERSEND         0x00000800    //��������ʱ���Ƿ�ʵʱ�����û�����
#define REALTIME_USERLOAD_PRIOR 0x00001000    //������������£������������
#define AB_LOCK                 0x00002000    //AB����
#define DOOR1_NOPEN             0x00004000    //��һN+1
#define DOOR2_NOPEN             0x00008000    //�Ŷ�N+1
#define ENABLE_COUNT            0x00010000    //���߼�¼��ͳ��
#define ENCRYP_DISPALY          0x00020000    //���߼�¼��ͳ��
#define ENROLL_MUTIFACE         0x00040000    //�Ǽ�ʱ����������ж�
#define REMOTE_LOAD_SAVE        0x00080000    //
#define G3_OUTPUT               0x00100000    //
#define RECORD_SAVE_BUTTON      0x00200000    //������Ű�ť��¼
#define ARMED_ENABLE_AWAYS      0x00400000    //��������ʱ����������
#define OPENSSL_ENABLE          0x00800000    //����Openssl����
#define SOAP_HEAD_WITH_IP       0x01000000    //soap��֤ͷ�����豸IP��ַ
#define ENABLE_IPV4             0x02000000
#define ENABLE_IPV6             0x04000000
#define WAKE_INPUT_ENABLED      0x20000000    //���������������ܼ�����֤
#define IPV4_DHCP_ENABLED       0x40000000    //����IPv4��DHCP����
#define IPV6_DHCP_ENABLED       0x80000000    //����IPv6��DHCP����
//��֤ģʽ���ȼ���-��:UID<CARD<11<MIX<1N
#define VERIFY_USERID   0x01    //���
#define VERIFY_CARD     0x02    //ˢ��
#define VERIFY_FACE_11  0x04    //11��������
#define VERIFY_FACE_MIX 0x08    //�����������
#define VERIFY_FACE_1N  0x10    //1N��������
#define VERIFY_FINGER   0x20    //ָ����֤
#define VERIFY_PLUSALL  0x40    //1:1���ʱ������֤ #define VERIFY_WEB      0x40
#define VERIFY_REMOTE   0x80    //Զ�̼��ط�ʽ
#define OUTPUT_WG_YES   0x01    //���ΰ��Э��
#define OUTPUT_WG_34    0x02    //ָ�����ΰ��34����ָ�����ΰ��26
#define OUTPUT_WG_CTX   0x04    //ΰ�����ָ������
#define OUTPUT_WG_UID   0x08    //�����OUTPUT_WG_CTX������û�ID��Ĭ���������
#define WG_OLD 0
#define WG_26  1
#define WG_34  2
#define WG_35  3
#define WG_36  4
#define WG_37  5
#define WG_44  6
#define WG_45  7
//�豸������������
typedef struct {
  //�豸ʱ����,(m_TimeGID[0]==ANY_TIME)δָ��ʱ����,�豸����ʱ����Ч.�豸ʱ�������ȼ������û�Ȩ��ʱ����
  int  m_DevCtx[ TGROUP_NUM ];
  //�豸Ĭ��Ȩ��: DEFAULT_NO_RIGHT , DEFAULT_SINGLE_OPEN
  int  m_DefaultRight;
  //�μ�2.0�汾����ı��
  int m_BaseSet;
  //��������, ������һ��������¼�����һ�ץ�Ĳ�ɫȫ��ͼ.
  CID  m_SuperPassword;
  //�Ŵ���ʱʱ�䣬��λ��
  char m_DoorMangetTime;
  //��������ʱ�䣬��λ��
  char m_LockTime;
  //�û������֤ģʽ,����:VERIFY_USERID|VERIFY_CARD|VERIFY_FACE_11
  char m_VerifyMode;
  //BIT(0)  : 0�������չΤ���ź�,1���.
  //BIT(1)  : 0���Τ��26,1���Τ��34.
  //BIT(2)  : 0����BIT(4)��� , 1���m_OutputCtx��ָ������
  //BIT(4)  : 0ֱ���������,1������
  //BIT(4-7): WG���Э��,0-wg26 1-wg34 2-wg35 3-wg36 4-wg37 5-wg44 6-wg45
  char m_OutputWG;
  //Τ��Э���������
  int m_OutputCtx;
} DEVSET;
//������½ṹ��
//#define F_APP_PACKAEG  0
//#define F_IMG_STANDBY  1
//#define F_PSD_CERTIFI  2
typedef struct {
  int  m_Bytes;          //�ļ���С
  int  m_Type;           //file type
  CID  m_CtxInfo;
  CID  m_FileName;       //�ļ���
  char m_Block[4];       //�ļ�����
} APPUPDATE;
//IO�豸״̬
#define IO_MODE_NORMAL   0x00   //����״̬
#define IO_MODE_OPEN     0x01   //����״̬
#define IO_MODE_CLOSE    0x02   //����״̬
//�Ŵ�״̬
#define IO_STATE_CLOSE   0x00   //�ŴŹ�
#define IO_STATE_OPEN    0x01   //�Ŵſ�
//ִ�ж���
#define ACT_IO_OPEN      0x02   //ִ�д򿪶���
#define ACT_IO_OPENEX    0x04   //ִ�и�������
#define ACT_IO_CLOSE     0x10   //ִ�йرն���
#define ACT_MODE_SET     0x20   //����IO�豸����ģʽ
#define ACT_MODE_GET     0x40   //���IO�豸����ģʽ
#define ACT_STATE_GET    0x80   //���IO�豸��ǰ״̬
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
  int    m_CtxID;
  //��֤�û���Ŀ�� 0���뿪�ţ�1Ȩ�޿��ţ�>1��Ͽ���
  int    m_Users;
  //������Ϣ
  IOCTRL m_XOpen;
  //��֤�û�����
  CID    m_IDS[MAX_GROUPUSER * 2];
} NOPEN;
//����485��ַ����:
//1.485��ַ��һ���ֽڱ�ʾ��[4-7]��ʾ�豸������1-15 , [0-3]��ʾ֧��16�ֲ�ͬ�͵��豸
//    �Ѿ�������豸���� 0--����ǰ�� , 1--������ ,
//    ����: 0x11 ��ʾ1��������ʶ��ǰ��1�� INDEX[4-7] == 1  TYPE[0-3] == 1
//          0x10 ��ʾ1��������ʶ��ǰ��0��INDEX[4-7] == 1  TYPE[0-3] == 0
//          0x12 ��ʾ1����ˢ����0       INDEX[4-7] == 1  TYPE[0-3] == 2
//          0x13 ��ʾ1����ˢ����1       INDEX[4-7] == 1  TYPE[0-3] == 3
//          0x1F ��ʾ�ŵ㱾��           INDEX[4-7] == 1  TYPE[0-3] == F
//          0x14 ��ʾ��������1          INDEX[4-7] == 1  TYPE[0-3] == 4
//          0x25 ��ʾ�������2          INDEX[4-7] == 2  TYPE[0-3] == 5
//2. �����豸����Ŵ�1��ʼ����Э���֧��15���ţ�16�в�ͬ���͵��ŵ������豸��15�����������15���������
//3. [0-7] == 0x00 ��ʾ�Ƿ���ַ��[0-7] ���� 0x01��ʾ��˰屾��
//7_______3______0
//|_INDEX_|__type__|
#define DEV_TYPE_FACE1     0x0      //����ǰ��1
#define DEV_TYPE_FACE2     0x1      //����ǰ��2
#define DEV_TYPE_CARD1     0x2      //ˢ����1
#define DEV_TYPE_CARD2     0x3      //ˢ����2
#define DEV_TYPE_IN        0x4      //��������
#define DEV_TYPE_OUT       0x5      //�������
#define DEV_TYPE_BUTTON    0x6      //���Ű�ť
#define DEV_TYPE_DOOR      0xF      //�ŵ㱾��
#define IMAGE_UYVY         0x01
#define IMAGE_YUYV         0x02
#define IMAGE_RGB24        0x03
typedef struct {
  //ͼ�������ܴ�СBYTES
  int     m_Size;
  //ͼ���
  int     m_W;
  //ͼ���
  int     m_H;
  //�����ݲ���
  int     m_Step;
  //ͼ������
  char    m_Type;
  //ͼ������
  char m_Block[3];
} DEVIMAGE;
#define ALIGN4( size )     ( ((size)+3) & ~3 )
#define USER_STEP( obj )   ALIGN4( sizeof(USER)+ (obj).m_FeatBytes+(obj).m_PicBytes )
#define RECORD_STEP( obj ) ALIGN4( sizeof(RECORD)+(obj).m_PicBytes-2 )
#define CGIMG_STEP( obj )  ALIGN4( sizeof(CHANGEIMAGE)+(obj).m_PicBytes-4 )
#define DSPLOG_STEP( obj ) ALIGN4( sizeof(DSPLOG) )

