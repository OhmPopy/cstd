#ifndef __COMMON__H__
#define __COMMON__H__
#include <map>
using namespace std;
//������
#define CMD_M_ID          0xA1
#define CMD_M_AUTH          M_AUTH
#define CMD_M_HEART         M_HEART
#define CMD_M_DEVINFO       M_DEVINFO
#define CMD_M_STATIS        M_STATIS
#define CMD_M_DEVSET        M_DEVSET
#define CMD_M_TIME          M_TIME
#define CMD_M_FORMAT        M_FORMAT
#define CMD_M_EOPEN         M_DOOR_STATE
#define CMD_M_REMOTE_OPEN   M_REMOTE_OPEN
#define CMD_M_NOPEN         M_NOPEN
#define CMD_M_USER          M_USER
#define CMD_M_VERIFY        M_VERIFY
#define CMD_M_ALARM         M_ALARM
#define CMD_M_OPTLOG        M_OPTLOG
#define CMD_M_CIMAGE        M_CIMAGE
#define CMD_M_RANGE         M_RANGE
#define CMD_M_TGROUP        M_TGROUP
#define CMD_M_DSPRIGHT      M_DSPRIGHT
#define CMD_M_UGROUP        M_UGROUP
#define CMD_M_MCAST       M_MCAST
#define CMD_M_APPUPDATE     M_APPUPDATE
#define CMD_M_VIDEO_OUT     M_VIDEO_OUT
#define CMD_M_USER_EX     M_USER_EX
#define CMD_M_RECORD_EX     M_RANGE_EX
#define CMD_M_REBOOT      M_REBOOT
#define CMD_M_IOCTRL        M_IOCTRL
#define CMD_M_NETPRO        M_NETPRO
#define CMD_M_RESETSCREEN   M_RESETSCREEN
//������
#define CMD_S_SET           S_SET                   //�ϴ�\����
#define CMD_S_DEL           S_DEL                   //ɾ��
#define CMD_S_GET           S_GET                   //����\���
#define CMD_S_REALTIME_SEND     S_REALTIME_SEND         //�ɼ�ʵʱ����
#define CMD_S_REALTIME_LOAD     S_REALTIME_LOAD         //ʵʱ����
#define CMD_S_REALTIME_CAP      S_REALTIME_CAP          //ʵʱ�û��ɼ�
#define CMD_S_REALTIME_CAP_INFO S_REALTIME_CAP_INFO     //Զ�̲ɼ�ʵ�ǽ�����Ϣ
#define CMD_S_UPDATE_SEND     S_UPDATE_SEND           //����ʵʱ����
#define CMD_S_CLEAR_INOUT       S_CLEAR_INOUT           //���ͳ�Ƽ���
//״̬���λ
#define F_ERR_BUSY          ERR_BUSY
#define F_ERR_LIMIT         ERR_LIMIT
#define F_ERR_NOFIND        ERR_NOFIND
#define F_ERR_SAVE_FAIL     ERR_SAVE_FAIL
#define F_ERR_SET_FAIL      ERR_SET_FAIL
#define F_ERR_VERIFY_FAIL   ERR_VERIFY_FAIL
//�������λ
#define F_DL_ID       CMD_DOWN_ID     //����ID����
#define F_DL_ID_ALL     CMD_ID_ALL      //�����������ݵ�ID����
#define F_ID_TIME_RANGE     CMD_ID_TIME_RANGE   //�����ƶ�ʱ���ID����
#define F_ID_RANGE      F_ID_TIME_RANGE<<1
#define F_AUTH_VERIFY   AUTH_VERIFY     //�û������֤
#define F_DL_PIC      DL_IMAGE      //��Ҫ������Ƭ
#define F_DL_FEAT     DL_FEAT       //��Ҫ������������
#define F_DL_APPENDID   DL_APPENDID     //Զ�̼���ʱ��m_ID��ʾ����
#define F_DL_NORMAL_TG    DL_NORMAL_TG    //��������ʱ����
#define F_DL_HOLIDAY_TG   DL_HOLIDAY_TG   //���ؽڼ���
#define F_DOOR_CLOSE    DOOR_CLOSE        //�������ţ�����Ϊ��������
#define F_DOOR_OPEN     DOOR_OPEN       //�������߳���״̬ȡ��
#define F_G_ORDER       G_ORDER             //��Ͽ�������
#define F_DEFAULT_NO_GROUP  DEFAULT_NO_GROUP
#define TIMEGROUP_NUMS      TGROUP_NUM        //ʱ�������
#define USER_COMBINS        MAX_GROUPUSER     //�û���ϸ���
#define USER_JPEG_SIZE      (64*1024)
#define REC_JPEG_SIZE     (18*1024)
#define FEAT_SIZE       (90*1024)
#define OFFSET_SIZE          20
//#define TRANS_SERVER
#ifndef TRANS_SERVER
#define CMD_S_CONNECT_CUT       DEV_CONNECT_CUT     //�Ͽ�����
#define CMD_S_CONNECT_SUCCESS   DEV_CONNECT_SUCCESS   //���ӳɹ�
#define CMD_S_CONNECT_FAILUE    DEV_CONNECT_FAILUE    //����ʧ��
#define CMD_S_CONNECT_NONE      DEV_CONNECT_NONE    //δ����
#define CMD_S_CONNECT_REQ   DEV_CONNECT_NONE+1    //��������  
#define CMD_S_CONNECT_REQ_CUT CMD_S_CONNECT_REQ+1   //����Ͽ�����
#define E_CONNECT_STATUS    DEV_CONNECT_STATUS
#else
#define CMD_S_CONNECT_CUT       NET_CONNECT_CUT     //�Ͽ�����
#define CMD_S_CONNECT_SUCCESS   NET_CONNECT_SUCCESS   //���ӳɹ�
#define CMD_S_CONNECT_FAILUE    NET_CONNECT_FAILUE    //����ʧ��
#define CMD_S_CONNECT_NONE      NET_CONNECT_NONE    //δ����
#define CMD_S_CONNECT_REQ   NET_CONNECT_NONE+1    //��������  
#define CMD_S_CONNECT_REQ_CUT CMD_S_CONNECT_REQ+1   //����Ͽ�����
#define E_CONNECT_STATUS    NET_CONNECT_STATUS
#endif
#ifndef TRANS_SERVER
typedef struct {
  DEV_CID m_UserName;        //�û���
  DEV_CID m_Password;        //����
} DEV_AUTH;
typedef enum {
  DL_SINGLE = 0x01,
  DL_ALL,
  DL_SEGTIME,
  DL_RANGE
} DATA_DL_TYPE;
//���ָ��ʱ��ε�ID����
typedef struct {
  DEV_DATE m_Start;  //��¼��ʼʱ��
  DEV_DATE m_End;    //��¼����ʱ��
} DEV_TIMESEG;
typedef struct {
  DATA_DL_TYPE  m_DLType;
  int       m_DLFlag;
  char          m_DLContent[4];
} DEV_DATA_DL;
#endif
#endif

