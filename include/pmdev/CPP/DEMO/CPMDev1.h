/*************************************************************************************/
/*��    ��:CPMDev.dll
/*��    ��:����˸�dsp�˵�ҵ���߼�ͨ�Žӿڣ�Ϊ������ṩ���������ϴ����ع��ܣ�ͬʱ����
dsp���û�����
/*˵    ��:��ʹ�ñ�������У���������������
1. �ϴ��û���Ƭʱ����Ƭ�ڴ��С������60KB����
2. �û������ϴ����ع����У��벻Ҫ��ջ�Ϸ������150kb���ڴ�
/*************************************************************************************/

#ifndef __CPMDEV_H__
#define __CPMDEV_H__

#ifdef CPMDEV_EXPORTS
#define CPMDEV_API __declspec(dllexport)
#else
#define CPMDEV_API __declspec(dllimport)
#endif
#define CPMDEV_CALL __stdcall

#ifdef __cplusplus
extern "C"
{
#endif
	
#ifndef BOOL 
	typedef int BOOL;
#endif
	
#ifndef TRUE
#define TRUE 1
#endif
	
#ifndef FALSE 
#define FALSE 0
#endif
	
#define DEV_REGION_ADDR ("224.0.1.100") //Ĭ�������ַ
	
#define DEV_ID_LEN 24
	
#define DEV_TIMEGROUP_NUMS 8    //ʱ�������
#define DEV_USER_COMBINS   6	//�û���ϸ���
	
#define DL_DEV_PIC        0x00000001   //��Ƭ
#define DL_DEV_USER_FEAT  0x00000002   //��������
#define	DL_DEV_USER_TEXT  0x00000004   //�û�������Ϣ
	
#define DEV_CHECK_TIME  0x01      //���ʱ��
#define DEV_CHECK_WEEK  0x02      //�������
#define DEV_CHECK_DAY   0x04      //�������
#define DEV_CHECK_MONTH 0x08      //����·�
#define DEV_CHECK_YEAR  0x10      //������
	
#define DEV_WEEK_1      0x01      //����һ��Ч
#define DEV_WEEK_2      0x02      //���ڶ���Ч
#define DEV_WEEK_3      0x04      //��������Ч
#define DEV_WEEK_4      0x08      //��������Ч
#define DEV_WEEK_5      0x10      //��������Ч
#define DEV_WEEK_6      0x20      //��������Ч
#define DEV_WEEK_7      0x40      //��������Ч
	
	//2.0��֧�ֵĹ���
#define DEV_RECORD_SAVEFAIL			0x00000001    //�Ƿ񱣴�ʶ��ʧ�ܼ�¼
#define DEV_SUPER_PASSWORD			0x00000002     //�������뿪����Ч
#define DEV_HDBEEP_OPEN             0x00000004    //Ӳ����������������
#define DEV_EMERGENCY_LOCK			0x00000008    //���������������˹���,��ʾʶ��ɹ��������Ȩ�޲�������ִ��.
#define DEV_REALTIME_RECORD			0x00000010    //ʵʱ��̬��¼��ʾ
#define DEV_REALTIME_USERLOAD		0x00000020    //�����Ż��߿���ʶ��ʱ������޷��ڵ�ǰdsp�л����������,����������״̬�£����ӹ����ʵʱ������������
#define DEV_REALTIME_USERSEND		0x00000040    //�ɼ��û�ͬ��
#define DEV_DOORMANGET_OPEN			0x00000080    //�����Ŵż�ⱨ��
#define DEV_DOORFORCE_OPEN			0x00000100    //����в�ȿ��Ź���
	
    //3.0��֧�ֵĹ���
#define DEV_REMOTE_CAP_SAVE         0x00000200    //Զ�̿��Ʋɼ����û��Ƿ񱣴浽DSP
#define DEV_GPRS_OPEN               0x00000400    //����GPRSģ��
#define DEV_UPDATE_USERSEND			0x00000800    //��������ʱ���Ƿ�ʵʱ�����û�����
	
	//��֤ģʽ���ȼ���-��:UID->CARD->11->MIX->1N
#define DEV_VERIFY_USERID   0x01    //����
#define DEV_VERIFY_CARD     0x02    //ˢ��
#define DEV_VERIFY_FACE_11  0x04    //11��������
#define DEV_VERIFY_FACE_MIX 0x08    //�����������
#define DEV_VERIFY_FACE_1N  0x10    //1N��������
	
	//�û�״̬��Ϣ
#define DEV_USER_CARD_INVALID	0x01   //����ʧ
#define DEV_USER_DLine_INVALID  0x02   //ʧЧʱ����Ч
#define DEV_USER_BLACK          0x04   //������
#define DEV_USER_MIX1N          0x08   //���ģʽ��1N�û�
	
	typedef enum
	{
		DEV_CONNECT_CUT = 0x00, //�Ͽ�����
			DEV_CONNECT_SUCCESS,	//��������
			DEV_CONNECT_FAILUE ,    //����ʧ��
			DEV_CONNECT_NONE        //δ��������
	}DEV_CONNECT_STATUS;
	
	typedef enum{
		DEV_OPER_UPLOAD = 0x01,  //�ϴ�
			DEV_OPER_DOWNLOAD = 0x02,//����
			DEV_OPER_DELETE  = 0x04  //ɾ��
	}DEV_OPER_FLAG;
	
	typedef enum{
		DEV_AUTH_OPER = 0x01,	//�豸��֤����
			DEV_AUTH_SET_OPER,		//�豸��֤�û����ò���
			DEV_REGION_OPER ,       //�豸�������
			DEV_FORMAT_OPER,		//�豸��ʽ������
			DEV_UPDATE_OPER,		//�豸������²���
			DEV_SYS_TIME_OPER ,		//�豸ϵͳʱ�����
			DEV_BASEINFO_OPER,		//������Ϣ����
			DEV_STATISINFO_OPER,	//ͳ����Ϣ����
			DEV_WORKATT_OPER,       //�������Բ���
			
			DEV_USER_OPER ,			//�û�����
			DEV_USER_RT_OPER,		//�û�ʵʱ���ز���
			DEV_USER_RANGE_OPER,    //�û���������������������
			DEV_RECORD_OPER  ,		//��¼����
			DEV_RECORD_RANGE_OPER, //��¼��������������������
			DEV_ALARM_OPER, 
			DEV_ALARM_RANGE_OPER,
			DEV_LOG_OPER,			//��־����
			DEV_LOG_RANGE_OPER,    //��־��������������������
			DEV_CHANGEIMAGE_OPER,	//���ע���ղ���
			DEV_CIMAGE_RANGE_OPER,//���ע������������������������
			
			DEV_TIME_GROUP_OPER,	//ʱ�������
			DEV_RIGHT_OPER,			//Ȩ�޲���
			DEV_USERGROUP_OPER,		//�û������
			
			DEV_DOOR_STATE_OPER,    //����״̬����	
			DEV_REMOTE_OPEN_DOOR,	//Զ�̿��Ų���	
			
			DEV_VIDEO_TRANSFER,		//��Ƶ����
			DEV_USER_EX_OPER,		//�û���������
			DEV_RECORD_EX_OPER,     //��¼��������
			DEV_LOG_EX_OPER,		//��־��������
			DEV_CIMAGE_EX_OPER,		//���ע������������
			
			DEV_REBOOT_OPER,		//�豸����
			DEV_USER_REMOTE_CAP		//�û�Զ�̲ɼ�
			
	}DEV_OPER_TYPE;
	
	typedef enum
	{
		OPER_SUCCESS		=0x00,    //�ɹ�
			OPER_ERR_BUSY       =0x01,    //�豸æ
			OPER_ERR_LIMIT      =0x02,    //�Ѵ�����
			OPER_ERR_NOFIND     =0x03,    //û���ҵ���Ӧ����
			OPER_ERR_SAVEFAIL   =0x04,    //���ݱ���ʧ��
			OPER_ERR_SETFAIL    =0x05,     //����ʧ��
			OPER_ERR_FROMAT		=0x07,            //��ʽ��ʧ��
			OPER_ERR_PARAM		=0x08,            //��������
			OPER_ERR_DISABLE	=0x09,            //Ҫ��ִ�еĹ���û��ʹ��
			OPER_ERR_EXECUT     =0x0A,       //ʧ��
			OPER_ERR_SUPPORT    =0x10,      //��֧�ֵ�����
			OPER_ERR_INPUTDATA  =0x11,       //����˴�����������쳣
			OPER_BATCH_DL_COMPLETE = 0x1F	 //�����������  
	}DEV_OPER_RESULT;//�������
	
	
    typedef enum{     
		DEV_NORMAL   = 0x01,		//����ʱ�� 			
			DEV_HOLIDY   = 0x02,		//�ڼ��� 
			DEV_ANY_TIME = 0x03	        //����ʱ��	
	}DEV_TIMEGROUP_TYPE; //ʱ��������
	
	typedef enum{
		DEV_USER_NOMAL = 0,			//��ͨ
			DEV_USER_ADMIN = 1,			//����Ա
			DEV_USER_SUPERADMIN = 2		//��������Ա
	}DEV_USER_TYPE; //�û�����
	
	
	typedef enum
	{
		DEV_DEFAULT_NO_RIGHT    = -2,		//��Ȩ��
			DEV_DEFAULT_SINGLE_OPEN = -1  //��һ����Ȩ��
	}DEV_DEFAULT_RIGHT;//Ĭ��Ȩ��
	
	
	//��¼����:
    //0x01 - 0x20: �����¼�
    //0x20 - 0x40: Ӳ�������¼�
    //0x40 - 0x50: ��������
    //0x50 - 0x60: �������
    //0xf0 - 0xff: ��������¼�
    //����: δ����
	typedef enum{
		DEV_VERIFY_SUCC = 0x01,          //��֤�ɹ�
			DEV_VERIFY_FAIL    = 0x02,          //��֤ʧ��
			DEV_ADMIN_SUCC  = 0x03,           //����Ա��֤�ɹ�
			DEV_EMER_OPEN   = 0x04,            //��������
			DEV_RIGHT_OPEN   = 0x05,           //Ȩ�޿���
			DEV_GROUP_OPEN = 0x06,           //��Ͽ���
			DEV_BUTTON_OPEN=0x07,           //��ť����
			
			DEV_ALARM_HD_MANGET_TIMEOUT = 0x20,     //�Ŵų�ʱ
			DEV_ALARM_HD_MANGET_ILLOPEN = 0x21,      //�ŴŷǷ�����
			DEV_ALARM_HD_OFFLINE  = 0x22,                         //ǰ�˵��߱���
			
			DEV_ALARM_HD_BREAK    = 0x30,       //���𱨾�
			DEV_ALARM_HD_SHOCK    =  0x31,      //�𶯱���
			DEV_ALARM_HD_FPOWR    = 0x36,      //ǰ�˹����쳣����
			DEV_ALARM_HD_UPS_ON  = 0x37,      //UPS���õ�ؿ���
			DEV_ALARM_HD_UPS_OFF= 0x38,      //UPS���õ�عر�
			
			DEV_ALARM_HD_ASSIST = 0x40,      //�������봥��
			
			DEV_ALARM_SF_BACKLIST = 0xF0,    //��������֤����
			DEV_ALARM_SF_ILLCARD = 0xF1,     //��Ч��(��ʧ)
			DEV_ALARM_SF_ILLTIME = 0xF2,     //�Ƿ�ʱ��ʶ��
			DEV_ALARM_SF_DEADLINE = 0xF3,    //ʧЧʱ��
			DEV_ALARM_SF_DANGER_OPEN = 0xF4, //в�ȿ���
			DEV_ALARM_SF_SUPER_OPEN = 0xF5  //�������뿪��
	}DEV_REC_TYPE; //��¼����
	
	
	typedef enum
	{
		DEV_DOOR_SOURCE = 0x01,		//�������Դ
			DEV_SIGNAL_SOURCE,			//�ź�������������Դ
			DEV_NO_NUM_DEV_SOURCE		//�ޱ���豸��Դ
	}DEV_REC_SOURCE_TYPE;
	
	typedef enum{ 
		DEV_LOG_ADDUSER = 0x01,			//�����û�
			DEV_LOG_DELUSER = 0x02,			//ɾ���û�
			DEV_LOG_REREGIST = 0x03,		//���²ɼ�
			DEV_LOG_CAHNGETYPE = 0x04,		//�ı��û�����
			DEV_UDISK_ULUSER = 0x05,		//U���ϴ��û�
			DEV_UDISK_DLUSER = 0x06,		//U�������û�
			DEV_UDISK_DLRECORD = 0x07 ,		//U�����ؼ�¼
			DEV_UDISK_UPDATEAPP = 0x08		//U�̸��³���
	}DEV_LOG_TYPE;
	
	enum
	{ 
		DEV_SOFTWAVE_MAKE = 0x01, 
			DEV_CTRL_PANEL	  = 0x02 
	};
	
	typedef enum 
	{
		DEV_DOOR_NOMAL = 0x00000000,   //����״̬
			DEV_DOOR_OPEN = 0x00000100,	  //��״̬
			DEV_DOOR_CLOSE = 0x00000200	  //��״̬
	}DEV_DOOR_STATE;
		  
	typedef struct
	{
		char m_ID[ DEV_ID_LEN ];
    }DEV_CID; //ID��ʶ�ṹ��
	
	typedef struct
    {
        int   m_Year;
        char  m_Month;
        char  m_Day;
    }DEV_DATE; //����
	
	typedef struct
    {
        int   m_Msec;            //����
        char  m_Hour;
        char  m_Minute;
        char  m_Second;
    }DEV_TIME; //ʱ��
	
	typedef struct
    {
        DEV_DATE m_Date;
        DEV_TIME m_Time;
    }DEV_DATETIME; //���ں�ʱ��
	
	typedef struct
    {
        int  m_DevType;        //�豸����
        int  m_LimitUser;      //��ʹ����������
		int  m_Limit1NUser;    //1N�û�����
		DEV_CID m_SN;          //�豸���
		DEV_CID m_Ver;         //DSP����汾
        DEV_CID m_Space;       //����������Ϣ
    }DEV_BASEINFO;//�豸������Ϣ
	
	typedef struct
    {
		int  m_TotalUsers;                   //��ǰ�豸�����û���
        int  m_NoFeatUser;                //û�вɼ������������û���
        int  m_TotalDays;                     //ʶ���¼����������
        int  m_TotalRecords;               //�ܼ�¼��
        int  m_TotalAlarm;                    //�ܱ�����¼��
        int  m_TotalDspLog;                //�ܲ�����־��Ŀ
        int  m_TotalChangeImage;     //�ܱ��ע����
    }DEV_STATIS; //DEV���ͳ����Ϣ
	
	typedef struct
	{
		int      m_TimeGID[DEV_TIMEGROUP_NUMS];          //ʱ����ID	
		int		 m_BaseSet;  			//�μ�2.0�汾����ı��
        int		 m_DoorMangetTime;		//�Ŵ���ʱʱ�䣬��λ��		
        int		 m_LockTime;			//��������ʱ�䣬��λ��
        int		 m_VerifyMode;			//�û������֤ģʽ,����:DEV_VERIFY_USERID|DEV_VERIFY_CARD|DEV_VERIFY_FACE_11
		int		 m_nWGType;				//Τ��Э������(0���Τ��26,1���Τ��34)
		int      m_nWGOutType;			//Τ���������(0���ID��1���WG����)
		int		 m_nWGOutIDType;		//���ID����(0�������,1����û�ID)
		int		 m_nWGOutContent;		//WG�������	
		BOOL	 m_bWGOutPut;			//�Ƿ�WG���	 
		DEV_CID				m_szSuperPWD;	    //��������
		DEV_DEFAULT_RIGHT	m_DefaultRight;		//�豸Ĭ��Ȩ��:	
	}DEV_WORKATT;
	
	typedef struct
    {
        DEV_USER_TYPE	m_UserType;         //�ն��豸�ϵ��û�����(��ͨ���������� -- 0 , 1 , 2)
        DEV_CID			m_UserID;           //�û�ID
		DEV_DATETIME	m_TypeTime;          //�û����͸���ʱ��
        DEV_DATETIME	m_RegistTime;       //����ע��ʱ�䣬�������ɼ�ʱ�䣬�ɼ��������²ɼ�Ҫ���´�ʱ��
        DEV_DATETIME	m_LastUpdTime;      //�����µ�ʱ�䣬ʶ��ʱ�����������»������²ɼ�Ҫ���´�ʱ��
    }DEV_VUSER;
	
    typedef struct
    {		
		int      m_FeatLen;			  //������С
		int      m_PicLen;			  //��Ƭ��С
		int		 m_RightID;           //�û�Ȩ��ID	
		DEV_CID  m_ImageSID;		  //�ϴ�ע���գ���Ӧ�ı��ID��ȫ��Ψһ
        DEV_CID  m_UserID;            //�û�ΨһID,�ؼ���	
		DEV_CID  m_Admin;             //����ע�����Ա
		DEV_CID  m_AppendID;          //���Ż�����������1:1�ĸ������ȷ����Ϣ
		DEV_CID  m_UserName;          //�û���
        DEV_CID  m_Department;        //��������
		DEV_DATETIME m_DeadLineTime;   //ʧЧʱ��
		DEV_DATETIME m_RegistTime;    //����ע��ʱ��
        DEV_DATETIME m_LastUpdTime; //�����µ�ʱ��
		DEV_DATETIME m_TypeUpdTime;  //�û����ͱ��ʱ��
		char m_UserFlag;                       //�û�״̬���
		DEV_USER_TYPE m_UserType;     //�ն��豸�ϵ��û�����
		char*   m_FeatData;		//��������
		char*   m_PicData;		//��Ƭ����
		int     m_bRTUser;	//�Ƿ�Ϊʵʱ�û�(0 ��ʵʱ�û� 1 ʵʱ�ɼ��û� 2ʵʱ���������û�)
    }DEV_USER; //�û���Ϣ
	
	typedef struct {
		int		m_LoadFlag;  //DL_DEV_PIC|DL_DEV_USER_FEAT=��Ҫ����ע����Ƭ����������
		int		m_ReqSource; //������Դ
		BOOL	m_bApeendID; //TRUE=����, FALSE=����
		DEV_CID m_ID;		 //����ID
	}DEV_USER_REAL_LOAD; //ʵʱ�û�����
	
	typedef struct 
	{
		int m_Count;         //���ض�����
		DEV_VUSER* m_pArray; //ID����
	}DEV_DL_USER_RANGE;//�û����ؼ���
	
	typedef struct
    {
		DEV_REC_TYPE  m_RecType;  //��¼����
		DEV_REC_SOURCE_TYPE m_Source;//��¼��Դ
		BOOL m_bRealTime;         //�Ƿ�Ϊʵʱ��¼
		int  m_Score;             //ʶ��÷�
		int  m_PicLen;            //��Ƭ��С
		int  m_VerifyMode;        //��֤ģʽ,����:DEV_VERIFY_USERID|DEV_VERIFY_CARD|DEV_VERIFY_FACE_11
		DEV_CID		m_ChangID;	  //�����¼��	
        DEV_CID		m_SerialID;  //ʶ���¼��ˮ��ID
        DEV_CID		m_UserID;    //�û�ID,���Ϊ�գ���ʾ�Ƿ���¼ 
		
		DEV_CID		m_AppendID;          //���Ż�����������1:1�ĸ������ȷ����Ϣ
		DEV_CID		m_UserName;          //�û���				  
		DEV_CID		m_Department;         //��������
		
        DEV_DATETIME m_RecTime;   //��¼ʱ��
		//m_ConcretSourceֵ����m_Source����
        //DEV_DOOR_SOURCE
        //��4λ��ʾ�ŵ���ţ��ŵ��1��ʼ���,��4λ��ʾ������ص��豸���(1:ˢ��1, 2:ˢ��2, 3:����1��4:����2)
        //DEV_SIGNAL_OUTPUT_SOURCE
        //��ʾ������ӱ��
        //DEV_NO_NUM_DEV_SOURCE
        //SOFTWAVE_MAKE ���� CTRL_PANEL
		char m_ConcretSource;	  //
		char* m_PicData;
    }DEV_RECORD; //��¼��Ϣ
	
	typedef struct
	{
		BOOL  m_bRTLog;				//�Ƿ�Ϊʵʱ������־
		DEV_LOG_TYPE m_LogType;      //��־����
		DEV_CID m_SerialID;               //��ˮ��ID
		DEV_CID m_Admin;        //����Ա
		DEV_CID m_BeOptUser;    //������Ա
		DEV_DATETIME m_RecTime; //��¼ʱ��
	}DEV_LOG;
	
    typedef struct
    {
        int		 m_PicBytes;   //ע���մ�С
		BOOL	 m_bRTChangeImage;	//�Ƿ�Ϊʵʱ���ע����
        DEV_CID  m_UserID;     //�û�ID
        DEV_CID  m_SerialID;   //��ˮ��ID
		
		DEV_CID  m_AppendID;          //���Ż�����������1:1�ĸ������ȷ����Ϣ
        DEV_CID  m_UserName;          //�û���
        DEV_CID  m_Department;         //��������
        DEV_CID  m_Admin;             //����ע�����Ա,��ʶ���û��������������ĸ�����Ա�ɼ�
		
		
		DEV_DATETIME m_RecTime; //��¼ʱ��
        char*    m_PicData;	   //ע����
    }DEV_CHANGEIMAGE;//���ע����
	
	typedef struct 
	{
		int m_Count;      //���ض�����
		BOOL m_bOpenRange;	  //�Ƿ�Ϊ������(true=�ǣ� false=��)
		DEV_CID m_SID;    //����һ����ʼ��m_SID��ʾ��¼��ȷ����ˮ��
						  //SID��ɣ�SN + TYPE + DATE + ID = ( 6BYTE + 1BYTE + 8BYTE + 4BYTE + \0)
	}DEV_DL_RECORD_RANGE;//��¼��������
	
	
	typedef struct 
	{
		DEV_DL_RECORD_RANGE* m_pRange;
		int m_Count;	
	}DEV_DL_RECORD_ARRAY;//��¼�������伯��
	
	typedef struct  
	{
		int      m_TGID;      //ʱ����ID
		DEV_DATETIME m_Start; //ʱ���鿪ʼʱ��
		DEV_DATETIME m_End;   //ʱ�������ʱ��
		DEV_TIMEGROUP_TYPE     m_TGType;    //ʱ��������
		char     m_CheckFlag; //ʱ����������m_CheckFlag = CHECK_TIME|CHECK_WEEK
		char     m_WeekFlag;  //�������ʱ�������Щ������Ч������m_WeekFlag = WEEK_1|WEEK_5
	}DEV_TIMEGROUP; //ʱ����
	
	typedef struct
	{
		DEV_TIMEGROUP* m_pTGArray;
		int			   m_nCount;
	}DEV_TIMEGROUP_ARRAY;//ʱ��������
	
	typedef struct
	{
		int m_RightID;        //Ȩ��ID
		int m_TimeGID[DEV_TIMEGROUP_NUMS];     //ʱ����ID , (m_TimeGID[0]==ANY_TIME)δָ��ʱ���飬���ŷ�ʽ����ʱ�����ƣ�����ʱ�����֤�ɹ�ִ�п��ŷ�ʽ.    
		BOOL m_bHolidyValid;	//�ڼ����Ƿ���Ч
		BOOL m_bActionLock;		//�������
		BOOL m_bActionOutPut;	//�����������
	}DEV_RIGHT; //Ȩ��
	
	typedef struct
	{
		DEV_RIGHT*	m_pRtArray;
		int			m_nCount;
	}DEV_RIGHT_ARRAY; //Ȩ������
	
	
	typedef struct
    {        
        int  m_GroupID;							//��ID
		int  m_NormalValid;						//��ͨ�û�����Ч�û���
		int  m_ForceValid;						//ǿ���û�����Ч�û���
		DEV_CID  m_NormalUsers[DEV_USER_COMBINS];	//��ͨ�û���ϣ����ȼ���
		DEV_CID  m_ForceUsers[DEV_USER_COMBINS];	//ǿ���û���ϣ����ȼ���
        BOOL m_bGroupOrder; //����Ƿ����� 1����0���� 
    }DEV_USERGROUP;
	
	
	typedef struct
	{
		DEV_USERGROUP* m_pUGArray;
		int			   m_nCount;
	}DEV_USERGROUP_ARRAY;//�û�������
	
	//������½ṹ��
	typedef struct
	{
		int   m_Bytes;            //�ļ���С
		DEV_CID  m_FileName;      //�ļ���
		char  m_TotalFiles;       //��Ҫ���µ��ļ�����
		char  m_FileIndex;        //��ǰ���µ��ڼ���
		char* m_Buf;			  //�ļ����ݻ����׵�ַ
	}DEV_APPUPDATE;
	
	//�û���������
	typedef struct
	{
		int       m_nCount;
		DEV_USER* m_pUserArray;
	}DEV_BATCH_USER;
	
	//������¼
	typedef struct
	{
		int			m_nCount;
		DEV_RECORD* m_pRecordArray;
	}DEV_BATCH_RECORD;
	
	//������־
	typedef struct
	{
		int       m_nCount;
		DEV_LOG*  m_pLogArray;
	}DEV_BATCH_LOG;
	
	//����ע����
	typedef struct
	{
		int				  m_nCount;
		DEV_CHANGEIMAGE*  m_pCImageArray;
	}DEV_BATCH_CIMAGE;
	
	/***********************************************************************************/
	/*��    �� �豸��������
	/*��    �� cszDevAddr �豸��ַ
	eCNStatus  ����״̬
	pvContext  Ӧ��������
	/*˵    ��
	/***********************************************************************************/
	typedef void (CPMDEV_CALL *DevConnectStatus)( const char* cszDevAddr , DEV_CONNECT_STATUS eCNStatus , void* pvContext ); 
	
	/***********************************************************************************/
	/*��    �� �豸��������ص�
	/*��    �� cszDevAddr  �豸��ַ
	eType       ��������
	eFlag       ������ʶ
	pvContent   �����������
	nSeq   ������ˮ��
	eResult     �������
	pvContext   Ӧ�ò�������
	/*˵    �� �ϲ�Ӧ���յ�����Ϣ�󼴿�֪��ǰһ�β����Ƿ�ɹ�
	eType��eFlag��eResult����pvContent����
	��eResult=OPER_SUCCESS && eFlag=DEV_OPER_DOWNLOADʱ��pvContent != NULL
	DEV_AUTH_OPER       pvContent=NULL
	DEV_AUTH_SET_OPER   pvContent=NULL
	DEV_REGION_OPER     pvContent= NULL
	DEV_FORMAT_OPER     pvContent=NULL		 
	DEV_UPDATE_OPER	   pvContent=NULL
	DEV_SYS_TIME_OPER   pvContent=DEV_DATETIME* or = NULL
	DEV_BASEINFO_OPER   pvContent=DEV_BASEINFO* or = NULL
	DEV_STATISINFO_OPER pvContent=DEV_STATIS* or = NULL
	DEV_WORKATT_OPER    pvContent=DEV_WORKATT* or = NULL
	
			   DEV_USER_OPER            pvContent=DEV_USER* or = NULL   
			   DEV_USER_RT_OPER         pvContent=DEV_USER_REAL_LOAD*
			   DEV_USER_NUMS_GET_OPER   pvContent=DEV_DL_USER_RANGE* or = NULL   
			   DEV_RECORD_OPER          pvContent=DEV_RECORD* or = NULL   
			   DEV_RECORD_NUMS_GET_OPER pvContent=DEV_DL_RECORD_RANGE* or = NULL   
			   DEV_LOG_OPER             pvContent=DEV_LOG* or = NULL   
			   DEV_LOG_NUMS_GET_OPER    pvContent=DEV_DL_RECORD_RANGE* or = NULL   
			   DEV_CHANGEIMAGE_OPER		pvContent=DEV_CHANGEIMAGE* or = NULL   
			   DEV_CHANGEIMAGE_NUMS_GET_OPER pvContent=DEV_DL_RECORD_RANGE* or = NULL   
			   
				 DEV_TIME_GROUP_OPER		pvContent=DEV_TIMEGROUP_ARRAY* or = NULL   
				 DEV_RIGHT_OPER			pvContent=DEV_RIGHT_ARRAY* or = NULL   
				 DEV_USERGROUP_OPER		pvContent=DEV_USERGROUP_ARRAY* or = NULL   
				 
				   
					 DEV_DOOR_STATE_OPER	    pvContent=DEV_DOOR_STATE* or = NULL   		
	/***********************************************************************************/
	typedef void (CPMDEV_CALL *DevOperResultNotify)( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult, void* pvContext );
	
	/***********************************************************************************/
	/*��    �� ��ʼ���ڷ���ʼ��ϵͳ
	/*��    �� bFlag(true=��ʼ��  false=����ʼ��)
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_InitSys(BOOL bFlag) ;  
	
	/***********************************************************************************/
	/*��    �� ע���豸����״̬���ѻص�
	/*��    �� pfnCNNotify �ص�����ָ��
	pvContext Ӧ��������
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API void CPMDEV_CALL CPM_RegDevConnectStatusCB(DevConnectStatus pfnCNNotify, void* pvContext);
	
	/***********************************************************************************/
	/*��    �� ע���豸�������֪ͨ
	/*��    �� pfnOperNotify   �ص�����ָ��
	pvContext  Ӧ��������
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API void CPMDEV_CALL CPM_RegOperResultNotifyCB(DevOperResultNotify pfnOperNotify, void* pvContext); 
	
	/***********************************************************************************/
	/*��    �� �����豸�������
	/*��    �� bFlag(true=����  false=ֹͣ)
	cszRgnAddr �豸���������ַ
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_StartDevSerch(BOOL bFlag, const char* cszRgnAddr = DEV_REGION_ADDR); 
	
	CPMDEV_API BOOL CPMDEV_CALL CPM_CNServer( const char* cszServer );
	
	CPMDEV_API void CPMDEV_CALL CPM_DCNServer( const char* cszServer );
	
	/***********************************************************************************/
	/*��    �� ���豸��������
	/*��    �� cszDevAddr �豸��ַ
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_CNDev( const char* cszDevAddr );  
	
	/***********************************************************************************/
	/*��    �� �Ͽ��豸����
	/*��    �� cszDevAddr �豸��ַ
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API void CPMDEV_CALL CPM_DCNDev( const char* cszDevAddr ); 
	
	/***********************************************************************************/
	/*��    �� �Ͽ������豸����
	/*��    ��  
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API void CPMDEV_CALL CPM_DCNAllDev(); 
	
	/***********************************************************************************/
	/*��    �� �豸��֤��������֤
	/*��    �� cszDevAddr  �豸��ַ
	cName	   �û���
	cPws		   ����
	nFlag	   1=��֤ 2=����
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULDevAuth( const char* cszDevAddr, const DEV_CID& cName, const DEV_CID& cPws, int nFlag, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� �����豸���������ַ
	/*��    �� cszDevAddr        �豸��ַ
	cszRgnAddr        �豸�����ַ
	/*˵    �� ���豸�������Ӻ�ɽ��豸���õ�ĳһ�������Ժ󼰿ɶ����������������豸
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULDevRegionAddr(const char* cszDevAddr, const char* cszRgnAddr = DEV_REGION_ADDR, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� �����豸ϵͳʱ��
	/*��    �� cszDevAddr �豸��ַ
	cSysTime ʱ����Ϣ
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULDevTime(const char* cszDevAddr, const DEV_DATETIME& cSysTime, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� ��ȡ�豸ϵͳʱ��
	/*��    �� cszDevAddr �豸��ַ
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLDevTime(const char* cszDevAddr, int nSeq = -1);  
	
	
	/***********************************************************************************/
	/*��    �� ��ȡ�豸������Ϣ
	/*��    �� cszDevAddr �豸��ַ
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLDevBaseInfo(const char* cszDevAddr, int nSeq =-1 );  
	
	/***********************************************************************************/
	/*��    �� ��ȡ�豸ͳ����Ϣ
	/*��    �� cszDevAddr �豸��ַ
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLDevStatisInfo(const char* cszDevAddr,int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� ��ȡ�豸����������Ϣ
	/*��    �� cszDevAddr �豸��ַ
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLDevWorkAttInfo(const char* cszDevAddr, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� �����豸����������Ϣ
	/*��    �� cszDevAddr �豸��ַ
	cWorkAtt   �������Խṹ��
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULDevWorkAttInfo(const char* cszDevAddr, const DEV_WORKATT& cWorkAtt, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� �ϴ��û�
	/*��    �� cszDevAddr �豸��ַ
	cUser      �û���Ϣ�ṹ��
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULUser(const char* cszDevAddr, const DEV_USER& cUser, int nSeq = -1) ;
	
	/***********************************************************************************/
	/*��    �� �ϴ�ʵʱ�û�
	/*��    �� cszDevAddr �豸��ַ
	cUser      �û���Ϣ�ṹ��
	cRtLoad    ʵʱ������Ϣ
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULRealTimeUser(const char* cszDevAddr, const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cRtLoad) ;
	
	/***********************************************************************************/
	/*��    �� ʵʱ�ɼ��û�����
	/*��    �� cszDevAddr �豸��ַ
	cUser      �û���Ϣ�ṹ��
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULRealTimeUserCap(const char* cszDevAddr, const DEV_USER& cUser, int nSeq = -1) ;
	
	/***********************************************************************************/
	/*��    �� ɾ��ĳ�û�
	/*��    �� cszDevAddr �豸��ַ
	cID        �û�ID
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DELUser(const char* cszDevAddr, const DEV_CID& cID, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� ����ĳһ�û�
	/*��    �� cszDevAddr �豸��ַ
	cID        �û�ID
	nFlag    DL_DEV_USER_PIC=��Ҫ����ע����Ƭ��DL_DEV_USER_FEAT=��Ҫ������������
	DL_DEV_USER_PIC|DL_DEV_USER_FEAT=��Ҫ����ע����Ƭ����������
	0x00=��������������Ƭ
	/*˵    �� 	
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLSingleUser(const char* cszDevAddr, const DEV_CID& cID, int nFlag, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� ���������û�
	/*��    �� cszDevAddr �豸��ַ
	
	  /*˵    �� 	
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLAllUser(const char* cszDevAddr, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� ����ĳһʱ����ڵ�ע���û�
	/*��    �� cszDevAddr �豸��ַ
	cStart    ��ʼ����
	cEnd      ��ֹ����
	/*˵    �� 	
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLSegTimeUser(const char* cszDevAddr, const DEV_DATE& cStart, const DEV_DATE& cEnd , int nSeq = -1); 
	
	
	/***********************************************************************************/
	/*��    �� ��������ʶ���¼
	/*��    �� cszDevAddr �豸��ַ
	/*˵    �� 	
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLAllIdentifyRecord(const char* cszDevAddr, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� ����ָ��ʱ���ʶ���¼
	/*��    �� cszDevAddr �豸��ַ
	cStart    ��ʼ����
	cEnd      ��ֹ����
	/*˵    �� 	
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLSegTimeIdentifyRecord(const char* cszDevAddr, const DEV_DATE& cStart, const DEV_DATE& cEnd, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� ��������ʶ���¼
	/*��    �� cszDevAddr �豸��ַ
	cRange ID���� 
	flag   DL_DEV_RECORD_PIC=��Ҫ���ؼ�¼��Ƭ 0x00=��������Ƭ, 0x01������Ƭ
	bKnowRange ָ�������ؾ������ݻ�������ID����(true=�������ݣ� false=ID����)
	/*˵    �� �˽ӿ����ڴ�ĳһ����ȷ�ļ�¼ID��ʼ����nRang������
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLRangeRec(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange ,int nFlag, BOOL bKnowRange = FALSE, int nSeq = -1);
	
	/***********************************************************************************/
	/*��    �� �������б�����¼
	/*��    �� cszDevAddr �豸��ַ
	/*˵    �� 	
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLAllAlarmRecord(const char* cszDevAddr, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� ����ָ��ʱ��α�����¼
	/*��    �� cszDevAddr �豸��ַ
	cStart    ��ʼ����
	cEnd      ��ֹ����
	/*˵    �� 	
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLSegTimeAlarmRecord(const char* cszDevAddr, const DEV_DATE& cStart, const DEV_DATE& cEnd, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� �������䱨����¼
	/*��    �� cszDevAddr �豸��ַ
	cRange ID���� 
	flag   DL_DEV_RECORD_PIC=��Ҫ���ؼ�¼��Ƭ 0x00=��������Ƭ, 0x01������Ƭ
	bKnowRange ָ�������ؾ������ݻ�������ID����(true=�������ݣ� false=ID����)
	/*˵    �� �˽ӿ����ڴ�ĳһ����ȷ�ļ�¼ID��ʼ����nRang������
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLRangeAlarm(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange ,int nFlag, BOOL bKnowRange = FALSE, int nSeq = -1);
	
	/***********************************************************************************/
	/*��    �� ����������־
	/*��    �� cszDevAddr �豸��ַ
	/*˵    ��
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLLog( const char* cszDevAddr, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� ����������־
	/*��    �� cszDevAddr �豸��ַ
	cRange ID���� 
	bKnowRange ָ�������ؾ������ݻ�������ID����(true=�������ݣ� false=ID����)
	/*˵    �� �˽ӿ����ڴ�ĳһ����ȷ�ļ�¼ID��ʼ����nRang������
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLRangeLog(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange , BOOL bKnowRange = FALSE, int nSeq = -1);
	
	/***********************************************************************************/
	/*��    �� �������б��ע����
	/*��    �� cszDevAddr �豸��ַ
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLChangeImage( const char* cszDevAddr, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� ����������ע����
	/*��    �� cszDevAddr �豸��ַ
	cRange ID���� 
	flag   DL_DEV_RECORD_PIC=��Ҫ���ؼ�¼��Ƭ 0x00=��������Ƭ, 0x01������Ƭ
	bKnowRange ָ�������ؾ������ݻ�������ID����(true=�������ݣ� false=ID����)
	/*˵    �� �˽ӿ����ڴ�ĳһ����ȷ�ļ�¼ID��ʼ����nRang������
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLRangeCImage(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange , int nFlag, BOOL bKnowRange = FALSE, int nSeq = -1);
	
	/***********************************************************************************/
	/*��    �� ����ʱ����
	/*��    �� cszDevAddr �豸��ַ
	cTGArray   ʱ���� 
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULTimeGroup( const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& cTGArray, int nSeq = -1);  
	
	/***********************************************************************************/
	/*��    �� ��ȡʱ����
	/*��    �� dev_addr �豸��ַ
	cTGArray ʱ���� 
	eType	ʱ��������
	/*˵    �� cTGArray.m_nCount=0ʱ��ʾ����eType���͵�����ʱ����
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLTimeGroup( const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& cTGArray, DEV_TIMEGROUP_TYPE eType, int nSeq = -1);  
	
	/***********************************************************************************/
	/*��    �� ɾ��ʱ����
	/*��    �� cszDevAddr �豸��ַ
	cTGArray ʱ���� 
	eType	ʱ��������
	/*˵    �� eType.m_nCount=0ʱ��ʾɾ��tg_type��������ʱ����
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DELTimeGroup( const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& cTGArray, DEV_TIMEGROUP_TYPE eType, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� ����Ȩ��
	/*��    �� cszDevAddr �豸��ַ
	cRTArray Ȩ�� 
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULRight( const char* cszDevAddr, const DEV_RIGHT_ARRAY& cRTArray, int nSeq = -1);
	
	/***********************************************************************************/
	/*��    �� ���ͻ�ȡȨ��
	/*��    �� cszDevAddr �豸��ַ
	cRTArray Ȩ�� 
	/*˵    �� ��rt_array.m_nCount=0ʱ��ʾ��������Ȩ��
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLRight( const char* cszDevAddr, const DEV_RIGHT_ARRAY& cRTArray, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� ɾ��Ȩ��
	/*��    �� cszDevAddr �豸��ַ
	cRTArray Ȩ�� 
	/*˵    �� ��rt_array.m_nCount=0ʱ��ʾɾ������Ȩ��
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DELRight( const char* cszDevAddr, const DEV_RIGHT_ARRAY& cRTArray, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� �����û���
	/*��    �� cszDevAddr �豸��ַ
	cUGrray �û��� 
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULUserGroup( const char* cszDevAddr, const DEV_USERGROUP_ARRAY& cUGrray, int nSeq = -1);
	
	/***********************************************************************************/
	/*��    �� ���ͻ�ȡ�û���
	/*��    �� cszDevAddr �豸��ַ
	ug_array �û��� 
	/*˵    �� ��rt_array.m_nCount=0ʱ��ʾ���������û���
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLUserGroup( const char* cszDevAddr, const DEV_USERGROUP_ARRAY& cUGrray, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� ɾ���û���
	/*��    �� cszDevAddr �豸��ַ
	ug_array �û��� 
	/*˵    �� ��rt_array.m_nCount=0ʱ��ʾɾ�������û��� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DELUserGroup( const char* cszDevAddr, const DEV_USERGROUP_ARRAY& cUGrray, int nSeq = -1); 
	
	/***********************************************************************************/
	/*��    �� ���ÿ�����״̬
	/*��    �� cszDevAddr �豸��ַ
	bEOpen true=�������ţ� false=��������
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULOpenDoorState( const char* cszDevAddr, DEV_DOOR_STATE eState, int nSeq = -1);  
	
	/***********************************************************************************/
	/*��    �� ��ȡ������״̬
	/*��    �� cszDevAddr �豸��ַ
	bEOpen true=�������ţ� false=��������
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLOpenDoorState( const char* cszDevAddr, int nSeq = -1);  
	
	/***********************************************************************************/
	/*��    �� Զ�̿���
	/*��    �� cszDevAddr �豸��ַ
	bEOpen true=���ţ� false=����
	pDoorArray �ŵ�����
	size       �����С
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULRemoteOpenDoor( const char* cszDevAddr, BOOL bOpen, int nSeq = -1);
	
	/***********************************************************************************/
	/*��    �� ��ʽ���豸
	/*��    �� cszDevAddr �豸��ַ
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULFormat( const char* cszDevAddr, int nSeq = -1);  
	
	
	/***********************************************************************************/
	/*��    �� �豸�������
	/*��    �� cszDevAddr �豸��ַ
	cAppData ������½ṹ��		    
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULUpdate( const char* cszDevAddr, const DEV_APPUPDATE& cAppData, int nSeq = -1);  
	
	/***********************************************************************************/
	/*��    �� ������Ƶ
	/*��    �� cszDevAddr �豸��ַ
	hwnd   ��Ƶ��ʾ���ھ��
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_StartVideo( const char* cszDevAddr, LONG hwnd, int nSeq = -1 );
	
	/***********************************************************************************/
	/*��    �� ֹͣ��Ƶ
	/*��    �� cszDevAddr �豸��ַ	    
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_StopVideo( const char* cszDevAddr, int nSeq = -1 );
	
	
	/***********************************************************************************/
	/*��    �� �����ϴ��û�
	/*��    �� cszDevAddr �豸��ַ	   
	cUsers �����û�����
	nFlag  DL_DEV_PIC		  //��Ƭ
	DL_DEV_USER_FEAT    //��������
	DL_DEV_USER_TEXT    //�û�������Ϣ
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_ULUserEx( const char* cszDevAddr, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq = -1 );
	
	/***********************************************************************************/
	/*��    �� �����û�
	/*��    �� cszDevAddr �豸��ַ	
	pUserIDArray �û�ID�����׵�ַ
	nIDCount    ����Ԫ�ظ���
	nFlag       ͬCPM_ULUserEx
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLUserEx(  const char* cszDevAddr, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq = -1 );
	
	/***********************************************************************************/
	/*��    �� ɾ���û�
	/*��    �� cszDevAddr �豸��ַ	   
	pUserIDArray �û�ID�����׵�ַ
	nIDCount    ����Ԫ�ظ���
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DELUserEx( const char* cszDevAddr, DEV_CID* pUserIDArray, int nIDCount, int nSeq = -1 );
	
	
	/***********************************************************************************/
	/*��    �� �������ؼ�¼
	/*��    �� cszDevAddr �豸��ַ	
	cRange     ID����
	/*˵    �� ������֤��¼��������¼��������־�����ע����
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_DLRecEx(  const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange, int nSeq = -1 );
	
	
	/***********************************************************************************/
	/*��    �� �����豸
	/*��    �� cszDevAddr �豸��ַ	   
	/*˵    �� 
	/***********************************************************************************/
	CPMDEV_API BOOL CPMDEV_CALL CPM_RebootDev( const char* cszDevAddr, int nSeq = -1 );

#ifdef __cplusplus
}
#endif

#endif

