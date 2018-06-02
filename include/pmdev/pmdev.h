#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define ASSERT assert
enum {
  TRUE = 1,
  FALSE = 0,
  TCPPORT = 13333, // �������˿�
  //����
  M_USER = 0x01, // �û�
  M_VERIFY = 0x02, // ��֤��¼
  M_ALARM = 0x03, // ������¼
  M_OPTLOG = 0x04, // ��־��¼
  M_CIMAGE = 0x05, // ���ע���ռ�¼
  M_RANGE = 0x06, // ���伯����������
  M_AUTH = 0x07, // �ͻ��������֤
  M_TIME = 0x08, // ϵͳʱ��
  M_HEART = 0x09, // ������
  M_DEVINFO = 0x0a, // �豸������Ϣ
  M_STATIS = 0x0b, // ͳ����Ϣ
  M_DEVSET = 0x0c, // �豸��������
  M_FORMAT = 0x0d, // �豸��ʽ��
  M_DOOR_STATE = 0x0e, // ��״̬���� ,��3��״̬��0����״̬ �� 1����״̬��
  // 2����״̬
  M_REMOTE_OPEN = 0x0f, // Զ�̿�����,
  M_TGROUP = 0x10, // ʱ����
  M_DSPRIGHT = 0x11, // Ȩ��
  M_UGROUP = 0x12, // �û���
  M_MCAST = 0x13, // �ಥ��ַ
  M_APPUPDATE = 0x14, // �������
  M_VIDEO_OUT = 0x23, // UDP������Ƶ���� NET_ADDRESS
  M_USER_EX = 0x24, // �����û��ϴ�����
  M_REBOOT = 0x25, // �豸����
  M_RANGE_EX = 0x26, // ��¼��������
  M_SET_IP = 0x27, // Զ���޸�IP��ַ
  M_NOPEN = 0x28, // N+1��������
  M_IOCTRL = 0x29, // IO����
  M_NETPRO = 0x30, // ��������
  S_SET = 0x81, // �ϴ�\����
  S_DEL = 0x82, // ɾ��
  S_GET = 0x83, // ����\���
  S_REALTIME_SEND = 0x84, // �ɼ�ʵʱ����
  S_REALTIME_LOAD = 0x85, // ʵʱ����
  S_REALTIME_CAP = 0x86, // ʵʱ�û��ɼ�
  S_REALTIME_CAP_INFO = 0x88, // Զ�̲ɼ�ʵ�ǽ�����Ϣ
  S_UPDATE_SEND = 0x87, // ����ʵʱ����
  S_CLEAR_INOUT = 0x88, // ���ͳ�Ƽ���
  // �ͻ�������ת������������
  C_CNDEV = 0xB0, // clientͨ����ת�������豸����
  C_VERIFY = 0xB1, // ��ת��������֤client���
  C_HEART = 0xB2, // ��ת��������client��������
  // �ͻ�������ת������������
  S_CN = 0xc0, // ��������
  S_DN = 0xc1, // �Ͽ�����
  // ����ִ�д���״̬
  SUC_EXECUT = 0x00, // �ɹ�
  ERR_EXECUT = 0x0A, // ʧ��
  ERR_BUSY = 0x01, // �豸æ
  ERR_LIMIT = 0x02, // �Ѵ�����
  ERR_NOFIND = 0x03, // û���ҵ���Ӧ����
  ERR_SAVE_FAIL = 0x04, // ���ݱ���ʧ��
  ERR_SET_FAIL = 0x05, // ����ʧ��
  ERR_VERIFY_FAIL = 0x06, // ��֤ʧ��
  ERR_FROMAT = 0x07, // ��ʽ��ʧ��
  ERR_PARAM = 0x08, // ��������
  ERR_DISABLE = 0x09, // Ҫ��ִ�еĹ���û��ʹ��
  ERR_SUPPORT = 0x10, // ��֧�ֵ�����
  ERR_INPUTDATA = 0x11, // ����˴�����������쳣
  CMD_STATE_REPLY = 0x40000000, // ��Ǹ�����Ϊ״̬�ظ�����
  CMD_DOWN_ID = 0x80000000, // ��������ID����,���ȼ��ñ��
  CMD_ID_ALL = 0x00000100, // �����������ݵ����伯��
  CMD_ID_TIME_RANGE = 0x00000200, // �����ƶ�ʱ������伯��
  CMD_ID_RANGE = 0x00000400, // ȷ��ָ����������伯��
  // ��������
  DL_IMAGE = 0x00000100, // ��Ҫ������Ƭ
  DL_FEAT = 0x00000200, // ��Ҫ��������
  DL_APPENDID = 0x00000400, // ʵʱ����ָʾCID�Ǳ�Ż��ǿ���
  LD_ENROLL = 0x00000800, // ��ҪԶ�̲ɼ�
  USER_TEXT = 0x00000400, // �û�������Ϣ
  BATCH_END = 0x00000800, // ��������
  // �û���֤
  AUTH_VERIFY = 0x00000100, // �û������֤
  // ��״̬: �����أ���ָ��Ϊ����״̬
  DOOR_OPEN = 0x00000100, // ��
  DOOR_CLOSE = 0x00000200, // ��
  // ʱ��������
  DL_NORMAL_TG = 0x00000100, // ��������ʱ����
  DL_HOLIDAY_TG = 0x00000200, // ���ؽڼ���
  // �������
  // �û����
  // ������Ͽ���: ͬһ���û�����������֤����������֤��ǿ���û�������֤��ͨ�û�
  // ������Ͽ���: �����Ժ���˳����֤��ֻҪ�����֤�ܽ���������֤��Ҫ����֤ͨ��.
  G_ORDER = 0x01,
  CMD_M_ID = 0xA1,
  CMD_M_AUTH = M_AUTH,
  CMD_M_HEART = M_HEART,
  CMD_M_DEVINFO = M_DEVINFO,
  CMD_M_STATIS = M_STATIS,
  CMD_M_DEVSET = M_DEVSET,
  CMD_M_TIME = M_TIME,
  CMD_M_FORMAT = M_FORMAT,
  CMD_M_EOPEN = M_DOOR_STATE,
  CMD_M_REMOTE_OPEN = M_REMOTE_OPEN,
  CMD_M_NOPEN = M_NOPEN,
  CMD_M_USER = M_USER,
  CMD_M_VERIFY = M_VERIFY,
  CMD_M_ALARM = M_ALARM,
  CMD_M_OPTLOG = M_OPTLOG,
  CMD_M_CIMAGE = M_CIMAGE,
  CMD_M_RANGE = M_RANGE,
  CMD_M_TGROUP = M_TGROUP,
  CMD_M_DSPRIGHT = M_DSPRIGHT,
  CMD_M_UGROUP = M_UGROUP,
  CMD_M_MCAST = M_MCAST,
  CMD_M_APPUPDATE = M_APPUPDATE,
  CMD_M_VIDEO_OUT = M_VIDEO_OUT,
  CMD_M_USER_EX = M_USER_EX,
  CMD_M_RECORD_EX = M_RANGE_EX,
  CMD_M_REBOOT = M_REBOOT,
  CMD_M_IOCTRL = M_IOCTRL,
  CMD_M_NETPRO = M_NETPRO,
  CMD_S_SET = S_SET, // �ϴ�\����
  CMD_S_DEL = S_DEL, // ɾ��
  CMD_S_GET = S_GET, // ����\���
  CMD_S_REALTIME_SEND = S_REALTIME_SEND, // �ɼ�ʵʱ����
  CMD_S_REALTIME_LOAD = S_REALTIME_LOAD, // ʵʱ����
  CMD_S_REALTIME_CAP = S_REALTIME_CAP, // ʵʱ�û��ɼ�
  CMD_S_REALTIME_CAP_INFO = S_REALTIME_CAP_INFO, // Զ�̲ɼ�ʵ�ǽ�����Ϣ
  CMD_S_UPDATE_SEND = S_UPDATE_SEND, // ����ʵʱ����
  CMD_S_CLEAR_INOUT = S_CLEAR_INOUT, // ���ͳ�Ƽ���
  // ״̬���λ
  F_ERR_BUSY = ERR_BUSY,
  F_ERR_LIMIT = ERR_LIMIT,
  F_ERR_NOFIND = ERR_NOFIND,
  F_ERR_SAVE_FAIL = ERR_SAVE_FAIL,
  F_ERR_SET_FAIL = ERR_SET_FAIL,
  F_ERR_VERIFY_FAIL = ERR_VERIFY_FAIL,
  // �������λ
  F_DL_ID = CMD_DOWN_ID, // ����ID����
  F_DL_ID_ALL = CMD_ID_ALL, // �����������ݵ�ID����
  F_ID_TIME_RANGE = CMD_ID_TIME_RANGE, // �����ƶ�ʱ���ID����
  F_ID_RANGE = F_ID_TIME_RANGE << 1,
  F_AUTH_VERIFY = AUTH_VERIFY, // �û������֤
  F_DL_PIC = DL_IMAGE, // ��Ҫ������Ƭ
  F_DL_FEAT = DL_FEAT, // ��Ҫ������������
  F_DL_APPENDID = DL_APPENDID, // Զ�̼���ʱ��m_ID��ʾ����
  F_DL_NORMAL_TG = DL_NORMAL_TG, // ��������ʱ����
  F_DL_HOLIDAY_TG = DL_HOLIDAY_TG, // ���ؽڼ���
  F_DOOR_CLOSE = DOOR_CLOSE, // �������ţ�����Ϊ��������
  F_DOOR_OPEN = DOOR_OPEN, // �������߳���״̬ȡ��
  F_G_ORDER = G_ORDER, // ��Ͽ�������
  //  F_DEFAULT_NO_GROUP = DEFAULT_NO_GROUP,
  // û��ָ���û�Ȩ��,�û�Ĭ��Ȩ��IDΪDEFAULT_NO_RIGHT����DEFAULT_SINGLE_OPEN,DSP�ϲɼ����û���
  // ��������Ա����ͳһ����ΪDSP_DEFAULT_SINGLE_OPEN
  // Ĭ����Ȩ�ޣ�Ĭ�ϵ�һ����Ȩ�ޣ�����û�������û�����Ȩ��֧�����ʱ����
  // DEFAULT_NO_RIGHT: DSP,����������û�,Ĭ��ֵ֮һ��Ȩ��
  // DEFAULT_SINGLE_OPEN: DSP,����������û���Ĭ��ֵ֮����һ����Ȩ�ޣ���Ȩ��û��ʱ�������
  DEFAULT_NO_RIGHT = -2,
  DEFAULT_SINGLE_OPEN = -1,
  MAX_GROUPUSER = 60,
  TGROUP_NUM = 8,
  TIMEGROUP_NUMS = TGROUP_NUM, // ʱ�������
  USER_COMBINS = MAX_GROUPUSER, // �û���ϸ���
  DL_OPENINTERVAL = 0x00000001, // ����ָ����¼��ˮ��֮���N����¼������ָ����¼����
  WEB_LOADUSER = 0x0001, // �����û�
  WEB_WRITEUSER = 0x0002, // д�û�
  WEB_WRITEREC = 0x0004, // д��¼
  WEB_SYNCTIME = 0x0008, // ͬ��ʱ��
  CHECK_TIME = 0x01, // ���ʱ��
  CHECK_WEEK = 0x02, // �������
  CHECK_DAY = 0x04, // �������
  CHECK_MONTH = 0x08, // ����·�
  CHECK_YEAR = 0x10, // ������
  // ����
  WEEK_1 = 0x01, // ����һ��Ч
  WEEK_2 = 0x02, // ���ڶ���Ч
  WEEK_3 = 0x04, // ��������Ч
  WEEK_4 = 0x08, // ��������Ч
  WEEK_5 = 0x10, // ��������Ч
  WEEK_6 = 0x20, // ��������Ч
  WEEK_7 = 0x40, // ��������Ч
  // ����ʱ����Ч , ����ʱ���飬�ڼ���ʱ����
  ANY_TIME = -1,
  TG_NORMAL = 1,
  TG_HOLIDY = 2,
  // ʱ����
  CHECK_HOLIDY = 0x01, // Ȩ�޼��ڼ���
  ACTION_LOCK = 0x02, // �������
  ACTION_OUTPUT = 0x04, // �������
  ACTION_N_1 = 0x08, // ��Ȩ����ҪN+1
  // ��ʾ�û��������κ���
  INVALID_GROUPID = -1,
  // �û���Ϣ
  USER_CARD_INVALID = 0x01, // ����ʧ
  USER_DLine_VALID = 0x02, // ʧЧʱ����Ч
  USER_BLACK = 0x04, // ������
  USER_MIX1N = 0x08, // 1N������֤
  USER_VIPCARD = 0x10, // VIP�û�
  USER_FINGER_PRINT = 0x20, // ָ����֤
  USER_SYNC_FINGEER = 0x40, // ָ��ͬ��
  // ��¼����:
  // 0x01 - 0x20: �����¼�
  // 0x20 - 0x40: Ӳ�������¼�
  // 0x40 - 0x50: ��������
  // 0x50 - 0x60: �������
  // 0xf0 - 0xff: ��������¼�
  // ����: δ����
  VERIFY_SUCC = 0x01, // ��֤�ɹ�
  VERIFY_FAIL = 0x02, // ��֤ʧ��
  ADMIN_SUCC = 0x03, // ����Ա��֤�ɹ�
  EMER_OPEN = 0x04, // ��������
  RIGHT_OPEN = 0x05, // Ȩ�޿���
  GROUP_OPEN = 0x06, // ��Ͽ���
  BUTTON_OPEN = 0x07, // ��ť����
  ALARM_HD_MANGET_TIMEOUT = 0x20, // �Ŵų�ʱ
  ALARM_HD_MANGET_ILLOPEN = 0x21, // �ŴŷǷ�����
  ALARM_HD_OFFLINE = 0x22, // ǰ�˵��߱���
  ALARM_HD_BREAK = 0x30, // ���𱨾�
  ALARM_HD_SHOCK = 0x31, // �𶯱���
  ALARM_HD_FPOWR = 0x36, // ǰ�˹����쳣����
  ALARM_HD_UPS_ON = 0x37, // UPS���õ�ؿ���
  ALARM_HD_UPS_OFF = 0x38, // UPS���õ�عر�
  ALARM_HD_ASSIST = 0x40, // �������봥��
  ALARM_HD_WL_DOOR_MAGNETIC = 0x41, // ����
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
  ALARM_SF_BACKLIST = 0xF0, // ��������֤����
  ALARM_SF_ILLCARD = 0xF1, // ��Ч��(��ʧ)
  ALARM_SF_ILLTIME = 0xF2, // �Ƿ�ʱ��ʶ��
  ALARM_SF_DEADLINE = 0xF3, // ʧЧʱ��
  ALARM_SF_DANGER_OPEN = 0xF4, // в�ȿ���
  ALARM_SF_SUPER_OPEN = 0xF5, // �������뿪��
  LINK_EVENT = 0xF6, // �����¼�
  // ������־����
  DSPLOG_ADDUSER = 0x01, // �����û�
  DSPLOG_DELUSER = 0x02, // ɾ���û�
  DSPLOG_REREGIST = 0x03, // ���²ɼ�
  DSPLOG_CAHNGETYPE = 0x04, // �ı��û�����
  DSPLOG_UDISK_ULUSER = 0x05, // U���ϴ��û�
  DSPLOG_UDISK_DLUSER = 0x06, // U�������û�
  DSPLOG_UDISK_DLRECORD = 0x07, // U�����ؼ�¼
  DSPLOG_UDISK_UPDATEAPP = 0x08, // U�̸��³���
  // 2.0��֧�ֵĹ���
  RECORD_SAVEFAIL = 0x00000001, // �Ƿ񱣴�ʶ��ʧ�ܼ�¼
  SUPER_PASSWORD = 0x00000002, // �������뿪����Ч
  HDBEEP_OPEN = 0x00000004, // ����/����(Ӳ����������������)
  REALTIME_RECORD = 0x00000010, // ʵʱ��̬��¼��ʾ
  REALTIME_USERLOAD = 0x00000020, // �����Ż��߿���ʶ��ʱ������޷��ڵ�ǰdsp�л����������,����������״̬�£����ӹ����ʵʱ������������
  REALTIME_USERSEND = 0x00000040, // �ɼ��û�ͬ��
  DOORMANGET_OPEN = 0x00000080, // �����Ŵż�ⱨ��
  DOORFORCE_OPEN = 0x00000100, // ����в�ȿ��Ź���
  REMOTE_CAP_SAVE = 0x00000200, // Զ�̿��Ʋɼ����û��Ƿ񱣴浽DSP
  GPRS_OPEN = 0x00000400, // ����GPRSģ��
  UPDATE_USERSEND = 0x00000800, // ��������ʱ���Ƿ�ʵʱ�����û�����
  REALTIME_USERLOAD_PRIOR = 0x00001000, // ������������£������������
  AB_LOCK = 0x00002000, // AB����
  DOOR1_NOPEN = 0x00004000, // ��һN+1
  DOOR2_NOPEN = 0x00008000, // �Ŷ�N+1
  ENABLE_COUNT = 0x00010000, // ���߼�¼��ͳ��
  ENCRYP_DISPALY = 0x00020000, // ���߼�¼��ͳ��
  // ��֤ģʽ���ȼ���-��:UID<CARD<11<MIX<1N
  VERIFY_USERID = 0x01, // ���
  VERIFY_CARD = 0x02, // ˢ��
  VERIFY_FACE_11 = 0x04, // 11��������
  VERIFY_FACE_MIX = 0x08, // �����������
  VERIFY_FACE_1N = 0x10, // 1N��������
  VERIFY_FINGER = 0x20, // ָ����֤
  OUTPUT_WG_YES = 0x01, // ���ΰ��Э��
  OUTPUT_WG_34 = 0x02, // ָ�����ΰ��34����ָ�����ΰ��26
  OUTPUT_WG_CTX = 0x04, // ΰ�����ָ������
  OUTPUT_WG_UID = 0x08, // �����OUTPUT_WG_CTX������û�ID��Ĭ���������
  WG_OLD = 0,
  WG_26 = 1,
  WG_34 = 2,
  WG_35 = 3,
  WG_36 = 4,
  WG_37 = 5,
  WG_44 = 6,
  WG_45 = 7,
  // IO�豸״̬
  IO_MODE_NORMAL = 0x00, // ����״̬
  IO_MODE_OPEN = 0x01, // ����״̬
  IO_MODE_CLOSE = 0x02, // ����״̬
  // �Ŵ�״̬
  IO_STATE_CLOSE = 0x00, // �ŴŹ�
  IO_STATE_OPEN = 0x01, // �Ŵſ�
  // ִ�ж���
  ACT_IO_OPEN = 0x02, // ִ�д򿪶���
  ACT_IO_OPENEX = 0x04, // ִ�и�������
  ACT_IO_CLOSE = 0x10, // ִ�йرն���
  ACT_MODE_SET = 0x20, // ����IO�豸����ģʽ
  ACT_MODE_GET = 0x40, // ���IO�豸����ģʽ
  ACT_STATE_GET = 0x80, // ���IO�豸��ǰ״̬
  // ����485��ַ����:
  // 1.485��ַ��һ���ֽڱ�ʾ��[4-7]��ʾ�豸������1-15 , [0-3]��ʾ֧��16�ֲ�ͬ�͵��豸
  // �Ѿ�������豸���� 0--����ǰ�� , 1--������ ,
  // ����: 0x11 ��ʾ1��������ʶ��ǰ��1�� INDEX[4-7] == 1 TYPE[0-3] == 1
  // 0x10 ��ʾ1��������ʶ��ǰ��0��INDEX[4-7] == 1 TYPE[0-3] == 0
  // 0x12 ��ʾ1����ˢ����0 INDEX[4-7] == 1 TYPE[0-3] == 2
  // 0x13 ��ʾ1����ˢ����1 INDEX[4-7] == 1 TYPE[0-3] == 3
  // 0x1F ��ʾ�ŵ㱾�� INDEX[4-7] == 1 TYPE[0-3] == F
  // 0x14 ��ʾ��������1 INDEX[4-7] == 1 TYPE[0-3] == 4
  // 0x25 ��ʾ�������2 INDEX[4-7] == 2 TYPE[0-3] == 5
  // 2. �����豸����Ŵ�1��ʼ����Э���֧��15���ţ�16�в�ͬ���͵��ŵ������豸��15�����������15���������
  // 3. [0-7] == 0x00 ��ʾ�Ƿ���ַ��[0-7] ���� 0x01��ʾ��˰屾��
  // 7_______3______0
  // |_INDEX_|__type__|
  DEV_TYPE_FACE1 = 0x0, // ����ǰ��1
  DEV_TYPE_FACE2 = 0x1, // ����ǰ��2
  DEV_TYPE_CARD1 = 0x2, // ˢ����1
  DEV_TYPE_CARD2 = 0x3, // ˢ����2
  DEV_TYPE_IN = 0x4, // ��������
  DEV_TYPE_OUT = 0x5, // �������
  DEV_TYPE_BUTTON = 0x6, // ���Ű�ť
  DEV_TYPE_DOOR = 0xF, // �ŵ㱾��
  // ͼ���ʽ
  IMAGE_UYVY = 0x01,
  IMAGE_YUYV = 0x02,
  IMAGE_RGB24 = 0x03,
  //�ṹ����
  OFFSET_SIZE = 20, // ��ͷ�ֽ�
  NCMD_size = 20, // ����ͷ�ֽ�
  PACK_SIZE = (60 * 1024), // ���
  ID_LEN = 24, // id��󳤶� ���� �û��� ���� 
  URL_LEN = 127, // url����
  IP_LEN = 64, // IP�ַ�������
  NETPRO_size = 128 + 128 + 16 * 5,
  NETDATE_size = 8,
  NETTIME_size = 8,
  DATETIME_size = NETDATE_size + NETTIME_size,
  TIMESE_size = NETDATE_size * 2,
  DSPTIMEG_size = 4 + DATETIME_size * 2 + 4,
  DSPRIGHT_size = (TGROUP_NUM + 2) * 4,
  USERGROUP_size = 8 + 2 * ID_LEN * MAX_GROUPUSER,
  RECORDRANGE_size = ID_LEN + 8,
  IOCTRL_size = 4,
};
#define MIN(a, b)   ((a)<(b) ? (a):(b))
#define MAX(a, b)   ((a)>(b) ? (a):(b))
typedef unsigned char byte;

// �ɱ�����ṹ�塣����Ԫ�ؾ���ʲô���ݣ���Ҫ����NCMD�趨������������
struct CARRAY {
  int m_Count; // ����Ԫ�صĸ���
  byte* m_Block; // �����ַ, 4��Ϊ�˽ṹ����ֽڶ���
};
// ����
struct NETDATE {
  int m_Year;
  byte m_Month;
  byte m_Day;
};
int NETDATE_tostring(const NETDATE* date, char* str, int len)
{
  _snprintf(str, len, "%d/%d/%d", date->m_Year, date->m_Month, date->m_Day);
  return 0;
}
// ʱ��
struct NETTIME {
  int m_Msec; // ����
  byte m_Hour;
  byte m_Minute;
  byte m_Second;
};
int NETTIME_tostring(const NETTIME* date, char* str, int len)
{
  _snprintf(str, len, "%d:%d-%d-%d", date->m_Hour, date->m_Minute, date->m_Second, date->m_Msec);
  return 0;
}
struct DATETIME {
  NETDATE m_Date;
  NETTIME m_Time;
};
int DATETIME_tostring(const DATETIME* dt, char* str, int len)
{
  char buf1[32] = {0};
  char buf2[32] = {0};
  NETDATE_tostring(&dt->m_Date, buf1, 32);
  NETTIME_tostring(&dt->m_Time, buf2, 32);
  _snprintf(str, len, "����:%s ʱ��:%s", buf1, buf2);
  return 0;
}
// ��������
struct NETPRO {
  byte m_WebPro1;
  byte m_WebPro2;
  char m_WebUrl1[URL_LEN];// = new byte[127];
  char m_WebUrl2[URL_LEN];// = new byte[127];
  char m_IPAdr[IP_LEN];// = new byte[16];
  char m_MIPAdr[IP_LEN];// = new byte[16];
  char m_NetMask[IP_LEN];// = new byte[16];
  char m_GateWay[IP_LEN];// = new byte[16];
  char m_Dns[IP_LEN];// = new byte[16];
};
// ���ָ��ʱ��ε�ID����
struct TIMESE {
  NETDATE m_Start; // ��¼��ʼʱ��
  NETDATE m_End; // ��¼����ʱ��
};
void TIMESE_set(TIMESE* s, NETDATE Start, NETDATE End)
{
  s->m_Start = Start;
  s->m_End = End;
}
// ʱ����
struct DSPTIMEG {
  int m_TGID; // ʱ����ID
  DATETIME m_Start; // ʱ���鿪ʼʱ��
  DATETIME m_End; // ʱ�������ʱ��
  byte m_TGType; // ʱ��������, TG_NORMAL,TG_HOLIDY
  byte m_CheckFlag; // ʱ����������m_CheckFlag = CHECK_TIME|CHECK_WEEK
  byte m_WeekFlag; // �������ʱ�������Щ������Ч������m_WeekFlag = WEEK_1|WEEK_5
  byte m_Reserve; // �����ֽ�
};
// Ȩ��
struct DSPRIGHT {
  // Ȩ��ID
  int m_RightID;
  // ʱ����ID , (m_TimeGID[0]==ANY_TIME)δָ��ʱ���飬���ŷ�ʽ����ʱ�����ƣ�����ʱ�����֤�ɹ�ִ�п��ŷ�ʽ.
  int m_TimeGID[TGROUP_NUM];
  // BIT(0):1 ���ڼ���, �����ڼ���
  // BIT(1):1 ����, 1��� 0�����
  // BIT(2):1 ����, 1��� 0�����
  // BIT(3):1 N��1, ���� 0�ر�
  int m_Mode;
};
struct CID {
  char m_ID[ID_LEN];
};
struct USERGROUP {
  // ��ID
  int m_GroupID;
  // BIT(0):1����0����
  byte m_GroupFlag;
  // ��ϳɹ����������ACTION_LOCK|ACTION_OUTPUT|ACTION_N_1
  byte m_Param;
  // ��ͨ�û�����Ч�û���
  byte m_NormalValid;
  // ǿ���û�����Ч�û���
  byte m_ForceValid;
  // ��ͨ�û���ϣ����ȼ���
  CID m_NormalUsers[MAX_GROUPUSER];// [MAX_GROUPUSER];
  // ǿ���û���ϣ����ȼ���
  CID m_ForceUsers[MAX_GROUPUSER]; // [MAX_GROUPUSER];
};
struct GROUPEX {
  int m_GroupID;
  int m_StrSize;
  byte m_GroupFlag;
  byte m_MinNum;
  byte* m_BLock;//
};
struct VUSER {
  int m_UserType; // �ն��豸�ϵ��û�����(��ͨ���������� -- 0 , 1 , 2)
  CID m_UserID; // �û�ID
  DATETIME m_TypeUpdTime; // �û����͸���ʱ��
  DATETIME m_RegistTime; // ����ע��ʱ�䣬�������ɼ�ʱ�䣬�ɼ��������²ɼ�Ҫ���´�ʱ��
  DATETIME m_LastUpdTime; // �����µ�ʱ�䣬ʶ��ʱ�����������»������²ɼ�Ҫ���´�ʱ��
};
int VUSER_tostring(VUSER* s, char* str, int len)
{
  const char* typestr[] = { "��ͨ", "����", "����" };
  char buf1[32] = {0};
  char buf2[32] = {0};
  char buf3[32] = {0};
  DATETIME_tostring(&s->m_TypeUpdTime, buf1, 32);
  DATETIME_tostring(&s->m_RegistTime, buf2, 32);
  DATETIME_tostring(&s->m_LastUpdTime, buf3, 32);
  _snprintf(str, len, "�ն��豸�ϵ��û�����: (%d, %s)\n�û�ID: %s\n�û����͸���ʱ��%s\n����ע��ʱ��%s\n�����µ�ʱ��%s"
      , s->m_UserType, typestr[s->m_UserType], s->m_UserID.m_ID, buf1, buf2, buf3);
  return 0;
}
struct USER {
  int m_FeatBytes; //���������ֽ�����==0 ��ʾû����������
  int m_PicBytes; //ע��ͼ����ֽ�����==0 ��ʾû��ͼƬ
  int m_RightID; // �û�Ȩ��ID,���ֿ���ֵ(DEFAULT_NO_RIGHT,DEFAULT_SINGLE_OPEN,�û�ָ��)
  CID m_ImageSID; // �ϴ�ע���գ���Ӧ�ı��ID��ȫ��Ψһ
  CID m_UserID; // �û���ʶ
  CID m_Admin; // ����ע�����Ա,��ʶ���û��������������ĸ�����Ա�ɼ�
  CID m_AppendID; // ���Ż�����������1:1�ĸ������ȷ����Ϣ
  CID m_UserName; // �û���
  CID m_Department; // ��������
  DATETIME m_DeadLine; // ʧЧʱ��
  DATETIME m_RegistTime; // ����ע��ʱ�䣬�������ɼ�ʱ�䣬�ɼ��������²ɼ�Ҫ���´�ʱ��
  DATETIME m_LastUpdTime; // �����µ�ʱ�䣬ʶ��ʱ�����������»������²ɼ�Ҫ���´�ʱ��
  DATETIME m_TypeUpdTime; // �û����ͱ��ʱ��
  byte m_UserFlag; // �û�״̬���
  byte m_UserType; // �ն��豸�ϵ��û�����(��ͨ���������� -- 0 , 1 , 2)
  byte* m_Feat; // ����������==0 ��ʾû����������
  byte* m_Pic; // ע��ͼ�� ==NULL ��ʾû��ͼƬ
};
static int ALIGN(int bytes, int alignments)
{
  return (((bytes) + ((alignments) - 1)) & ~((alignments) - 1));
}
// ע��ͼƬ,����JPG,������������m_UserIDΨһ�󶨵�.m_Block��ǰm_FeatBytes���ֽڱ�ʾ��������,
// ��m_PicBytes���ֽڱ�ʾͼƬ.
int USER_size(USER* s)
{
  return ALIGN(6 * ID_LEN + 4 * DATETIME_size + 2 * 4 + s->m_FeatBytes + s->m_PicBytes + 16, 4);
}
struct RECORDRANGE {
  // ���ض�����,<=0 ��ʾ����m_SID֮����������м�¼,�˳�����m_SID������Ч
  int m_Count;
  // DL_OPENINTERVAL�򿪿���������,�������������
  int m_Flag;
  // ����һ����ʼ��m_SID��ʾ��¼��ȷ����ˮ��,
  // ��ˮ���ַ������:
  // SN(MAC��3�ֽ�) + TYPE(��¼���) + DATETIME(����) + ID(�Զ���) = (6BYTE + 1BYTE +
  // 14BYTE + 2BYTE + \0)
  CID m_SID;
};
// ��¼��Ϣ,ʶ���¼��������¼
struct RECORD {
  int m_RecType; // ��¼����
  int m_Score; // ʶ��÷� , �����¼�Ϊ�����¼���LINKID
  int m_PicBytes; // ��¼ͼ����ֽ�����==0 ��ʾû�м�¼ͼƬ
  CID m_ChangID; // ע���ռ�¼��
  CID m_UserID; // �û�ID,���Ϊ�գ���ʾ�Ƿ���¼ , �����¼�Ϊ�¼�����Text
  CID m_SerialID; // ʶ���¼��ˮ��ID
  CID m_AppendID; // ���Ż�����������1:1�ĸ������ȷ����Ϣ
  CID m_UserName; // �û���
  CID m_Department; // ��������
  DATETIME m_RecTime; // ��¼ʱ��
  byte m_VerifyMode; // ��֤ģʽ,��:VERIFY_CARD|VERIFY_FACE_1N,ˢ��+����
  byte m_Source; // �¼�Դ�豸��485��ַ
  byte* m_Pic; // ��¼ͼ��,����JPG , 2��Ϊ�˽ṹ����ֽڶ���
};
int RECORD_size(RECORD* s)
{
  return ALIGN(4 * 4 + ID_LEN * 6 + s->m_PicBytes, 4);
}
// CGIMG_STEP( obj ) ALIGN4(
// sizeof(CHANGEIMAGE)+(this).m_PicBytes-4 )
// DSPLOG_STEP( obj ) ALIGN4( sizeof(DSPLOG) )
// �豸������־
struct DSPLOG {
  int m_LogType; // ��־����
  CID m_SerialID; // ��ˮ��ID
  CID m_Admin; // ����Ա
  CID m_BeOptUser; // ������Ա
  DATETIME m_RecTime; // ��¼ʱ��
};
// ���ע����
struct CHANGEIMAGE {
  int m_PicBytes; //ע���մ�С
  CID m_UserID; // �û�ID
  CID m_SerialID; // ��ˮ��ID
  CID m_AppendID; // ���Ż�����������1:1�ĸ������ȷ����Ϣ
  CID m_UserName; // �û���
  CID m_Department; // ��������
  CID m_Admin; // ����ע�����Ա,��ʶ���û��������������ĸ�����Ա�ɼ�
  DATETIME m_RecTime; // ��¼ʱ��
  byte* m_Pic; // ע�����ڴ�
};
int CHANGEIMAGE_size(CHANGEIMAGE* s)
{
  return ALIGN(6 * ID_LEN + 8 + s->m_PicBytes, 4);
}
// DSP���ͳ����Ϣ
struct STATIS {
  int m_TotalUsers; // ��ǰ�豸�����û���
  int m_NoFeatUser; // û�вɼ������������û���
  int m_TotalDays; // ʶ���¼����������
  int m_TotalRecords; // �ܼ�¼��
  int m_TotalAlarm; // �ܱ�����¼��
  int m_TotalDspLog; // �ܲ�����־��Ŀ
  int m_TotalChangeImage; // �ܱ��ע����
};
int STATIS_tostring(STATIS* s, char* str, int len)
{
  _snprintf(str, len, "��ǰ�豸�����û���:%d\nû�вɼ������������û���:%s\nʶ���¼����������:%s\n�ܼ�¼��:%s\n�ܱ�����¼��:%s\n�ܲ�����־��Ŀ:%s\n�ܱ��ע����",
      s->m_TotalUsers, s->m_NoFeatUser, s->m_TotalDays, s->m_TotalRecords, s->m_TotalAlarm, s->m_TotalDspLog, s->m_TotalChangeImage);
  return 0;
}
// ��֤��Ϣ
struct AUTH {
  CID m_UserName; // �û���
  CID m_Password; // ����
};
// �豸������Ϣ
struct BASEINFO {
  int m_DevType; // �豸����
  int m_LimitUser; // ����������
  int m_Limit1NUser; // 1N�û�����
  CID m_SN; // �豸���
  CID m_Ver; // DSP����汾
  CID m_Space; // ����������Ϣ
};
int BASEINFO_tostring(BASEINFO* s, char* str, int len)
{
  _snprintf(str, len, "�豸����:%d\n��ʹ����������:%d\n1N�û�����%d\n�豸���:%s\nDSP����汾:%s\n����������Ϣ%s",
      s->m_DevType, s->m_LimitUser, s->m_Limit1NUser, s->m_SN.m_ID, s->m_Ver.m_ID, s->m_Space.m_ID);
  return 0;
}
// Զ�̲ɼ�������Ϣ
struct RCAPINFO {
  int m_Total; // �ɼ�����
  int m_Count; // ��ǰ�ɼ���
  CID m_UserID; // �û��ǼǱ��
};
// �豸������������
struct WORKATT {
  // �豸ʱ����,(m_TimeGID[0]==ANY_TIME)δָ��ʱ����,�豸����ʱ����Ч.�豸ʱ�������ȼ������û�Ȩ��ʱ����
  int m_DevCtx[TGROUP_NUM]; // [ TGROUP_NUM ];
  // �豸Ĭ��Ȩ��: DEFAULT_NO_RIGHT , DEFAULT_SINGLE_OPEN
  int m_DefaultRight;
  // �μ�2.0�汾����ı��
  int m_BaseSet;
  // ��������, ������һ��������¼�����һ�ץ�Ĳ�ɫȫ��ͼ.
  CID m_SuperPassword;
  // �Ŵ���ʱʱ�䣬��λ��
  byte m_DoorMangetTime;
  // ��������ʱ�䣬��λ��
  byte m_LockTime;
  // �û������֤ģʽ,����:VERIFY_USERID|VERIFY_CARD|VERIFY_FACE_11
  byte m_VerifyMode;
  // BIT(0) : 0�������չΤ���ź�,1���.
  // BIT(1) : 0���Τ��26,1���Τ��34.
  // BIT(2) : 0����BIT(4)��� , 1���m_OutputCtx��ָ������
  // BIT(4) : 0ֱ���������,1������
  // BIT(4-7): WG���Э��,0-wg26 1-wg34 2-wg35 3-wg36 4-wg37 5-wg44 6-wg45
  byte m_OutputWG;
  // Τ��Э���������
  int m_OutputCtx;
};
// ������½ṹ��
struct APPUPDATE {
  int m_Bytes; //�ļ���С
  CID m_FileName; // �ļ���
  byte m_TotalFiles; // ��Ҫ���µ��ļ�����
  byte m_FileIndex; // ��ǰ���µ��ڼ���
  byte* m_File; // �ļ�����
};
int APPUPDATE_size(APPUPDATE* s)
{
  return ID_LEN + 6 + s->m_Bytes;
}
struct IOCTRL {
  // IO�豸485��ַ
  byte m_Source;
  // ִ�ж���
  byte m_Action;
  // IO�豸��ǰģʽ
  byte m_IOMode;
  // IO�豸��ǰ״̬״̬
  byte m_IOState;
};
// NOPEN:Զ���������˿���
struct NOPEN {
  // m_Users==1Ȩ��ID, m_Users>1���ID
  int m_CtxID;
  // ��֤�û���Ŀ�� 0���뿪�ţ�1Ȩ�޿��ţ�>1��Ͽ���
  int m_Users;
  // ������Ϣ
  IOCTRL m_XOpen;
  // ��֤�û�����
  CID m_IDS[MAX_GROUPUSER * 2];
};
struct IMAGE {
  int m_Size; // ͼ�������ܴ�СBYTES
  int m_W; // ͼ���
  int m_H; // ͼ���
  int m_Step; // �����ݲ���
  byte m_Type; // ͼ������
  byte* m_Block; // ͼ������
};
struct set_stream {
  int off;
  byte* bytes;
  set_stream(byte* bytes1, int off1) {
    bytes = bytes1;
    off = off1;
  }
  int set_STRING(const char* data, int maxlen) {
    int len = strlen(data);
    len = MIN(len, maxlen);
    memcpy(bytes + off, data, len);
    if (len<maxlen) {
      memset(bytes + off+len, 0, maxlen-len);
    }
    off += maxlen;
    return off;
  }
  int set_BYTE(byte data) {
    bytes[off + 0] = (byte)(data);
    off += 1;
    return off;
  }
  int set_BYTES(byte* data, int n) {
    int i = 0;

    for (; i < n; ++i) {
      bytes[off + i] = data[i];
    }

    off += n;
    return off;
  }
  int set_SHORT(short data) {
    bytes[off + 0] = (byte)(data & 0xff);
    bytes[off + 1] = (byte)((data & 0xff00) >> 8);
    off += 2;
    return off;
  }
  int set_INT(int data) {
    bytes[off + 0] = (byte)(data & 0xff);
    bytes[off + 1] = (byte)((data & 0xff00) >> 8);
    bytes[off + 2] = (byte)((data & 0xff0000) >> 16);
    bytes[off + 3] = (byte)((data & 0xff000000) >> 24);
    off += 4;
    return off;
  }
  int set_INTS(const int* arr, int n) {
    for (int i = 0; i < n; ++i) {
      set_INT(arr[i]);
    }

    return off;
  }
  int set_CID(const CID* str) {
    return set_STRING(str->m_ID, ID_LEN);
  }
  int set_NETDATE(NETDATE* date) {
    if (date != NULL) {
      set_INT(date->m_Year);
      set_BYTE(date->m_Month);
      set_BYTE(date->m_Day);
    }

    off += 8;
    return off;
  }
  int set_NETTIME(NETTIME* time) {
    if (time != NULL) {
      set_INT(time->m_Msec);
      set_BYTE(time->m_Hour);
      set_BYTE(time->m_Minute);
      set_BYTE(time->m_Second);
    }

    off += 8;
    return off;
  }
  int set_DATETIME(DATETIME* datetime) {
    if (datetime != NULL) {
      set_NETDATE(&datetime->m_Date);
      set_NETTIME(&datetime->m_Time);
    }
    else {
      set_NETDATE(NULL);
      set_NETTIME(NULL);
    }

    return off;
  }
  int offset(int add) {
    int i = off;
    off += add;

    for (; i < off; ++i) {
      bytes[i] = 0;
    }

    return off;
  }
  int set_CIDS(const CID* strs, int n) {
    for (int i = 0; i < n; ++i) {
      set_CID(strs + i);
    }

    return off;
  }
  int set_WORKATT(WORKATT* workatt) {
    set_INTS(workatt->m_DevCtx, TGROUP_NUM);
    set_INT(workatt->m_DefaultRight);
    set_INT(workatt->m_BaseSet);
    set_CID(&workatt->m_SuperPassword);
    set_BYTE((byte) workatt->m_DoorMangetTime);
    set_BYTE((byte) workatt->m_LockTime);
    set_BYTE((byte) workatt->m_VerifyMode);
    set_BYTE((byte) workatt->m_OutputCtx);
    set_INT(workatt->m_OutputCtx);
    return off;
  }
  int set_USER(USER* obj) {
    int off1 = off;
    set_INT(obj->m_FeatBytes);
    set_INT(obj->m_PicBytes);
    set_INT(obj->m_RightID);
    set_CID(&obj->m_ImageSID);
    set_CID(&obj->m_UserID);
    set_CID(&obj->m_Admin);
    set_CID(&obj->m_AppendID);
    set_CID(&obj->m_UserName);
    set_CID(&obj->m_Department);
    set_DATETIME(&obj->m_DeadLine);
    set_DATETIME(&obj->m_RegistTime);
    set_DATETIME(&obj->m_LastUpdTime);
    set_DATETIME(&obj->m_TypeUpdTime);
    set_BYTE(obj->m_UserFlag);
    set_BYTE(obj->m_UserType);

    if (obj->m_Feat != NULL) {
      set_BYTES(obj->m_Feat, obj->m_FeatBytes);
    }

    if (obj->m_Pic != NULL) {
      set_BYTES(obj->m_Pic, obj->m_PicBytes);
    }

    off = off1 + USER_size(obj);
    return off;
  }
  int set_RECORDRANGE(RECORDRANGE* obj) {
    int off1 = off;
    set_INT(obj->m_Count);
    set_INT(obj->m_Flag);
    set_CID(&obj->m_SID);
    off = off1 + RECORDRANGE_size;
    return off;
  }
  int set_DSPTIMEG(DSPTIMEG* obj) {
    int off1 = off;
    set_INT(obj->m_TGID);
    set_DATETIME(&obj->m_Start);
    set_DATETIME(&obj->m_End);
    set_BYTE(obj->m_TGType);
    set_BYTE(obj->m_CheckFlag);
    set_BYTE(obj->m_WeekFlag);
    set_BYTE(obj->m_Reserve);
    off = off1 + DSPTIMEG_size;
    return off;
  }
  int set_DSPRIGHT(DSPRIGHT* obj) {
    int off1 = off;
    set_INT(obj->m_RightID);
    set_INTS(obj->m_TimeGID, TGROUP_NUM);
    set_INT(obj->m_Mode);
    off = off1 + DSPTIMEG_size;
    return off;
  }
  int set_USERGROUP(USERGROUP* obj) {
    int off1 = off;
    set_INT(obj->m_GroupID);
    set_BYTE(obj->m_GroupFlag);
    set_BYTE(obj->m_Param);
    set_BYTE(obj->m_NormalValid);
    set_BYTE(obj->m_ForceValid);
    set_CIDS(obj->m_NormalUsers, MAX_GROUPUSER);
    set_CIDS(obj->m_ForceUsers, MAX_GROUPUSER);
    off = off1 + USERGROUP_size;
    return off;
  }
  int set_IOCTRL(IOCTRL* obj) {
    int off1 = off;
    set_BYTE(obj->m_Source);
    set_BYTE(obj->m_Action);
    set_BYTE(obj->m_IOMode);
    set_BYTE(obj->m_IOState);
    off = off1 + USERGROUP_size;
    return off;
  }
  int set_APPUPDATE(APPUPDATE* obj) {
    int off1 = off;
    set_INT(obj->m_Bytes);
    set_CID(&obj->m_FileName);
    set_BYTE(obj->m_TotalFiles);
    set_BYTE(obj->m_FileIndex);

    if (obj->m_File != NULL) {
      set_BYTES(obj->m_File, obj->m_Bytes);
    }

    off = off1 + APPUPDATE_size(obj);
    return off;
  }
  int set_NETPRO(NETPRO* obj) {
    int off1 = off;
    set_BYTE(obj->m_WebPro1);
    set_BYTE(obj->m_WebPro2);
    set_STRING(obj->m_WebUrl1, URL_LEN);
    set_STRING(obj->m_WebUrl2, URL_LEN);
    set_STRING(obj->m_IPAdr, IP_LEN);
    set_STRING(obj->m_MIPAdr, IP_LEN);
    set_STRING(obj->m_NetMask, IP_LEN);
    set_STRING(obj->m_GateWay, IP_LEN);
    set_STRING(obj->m_Dns, IP_LEN);
    off = off1 + NETPRO_size;
    return off;
  }
};
int myrealloc(void* pp1, int n)
{
  void** pp2 = (void**)(pp1);
  *pp2 = realloc(*pp2, n);
  return n;
}
struct get_stream {
  int off;
  byte* bytes;
  get_stream(byte* bytes1, int off1) {
    bytes = bytes1;
    off = off1;
  }
  int get_BYTE(byte* by) {
    *by = (byte)(0xff & bytes[off]);
    off += 1;
    return off;
  }
  int get_INT(int* ret) {
    *ret = (0xff & bytes[off + 0])
        | (0xff00 & (bytes[off + 1] << 8))
        | (0xff0000 & (bytes[off + 2] << 16))
        | (0xff000000 & (bytes[off + 3] << 24));
    off += 4;
    return off;
  }
  int get_BYTES(byte* by, int len) {
    if (len > 0) {
      memcpy(by, bytes + off, len);
      off += len;
    }

    return off;
  }
  int get_STRING(char* s, int len) {
    if (len > 0) {
      memcpy(s, bytes + off, len);
      off += len;
    }

    return off;
  }
  int get_CID(CID* obj) {
    return get_STRING(obj->m_ID, ID_LEN);
  }
  int get_NETDATE(NETDATE* obj) {
    get_INT(&obj->m_Year);
    get_BYTE(&obj->m_Month);
    get_BYTE(&obj->m_Day);
    off += 2;
    return off;
  }
  int get_NETTIME(NETTIME* obj) {
    get_INT(&obj->m_Msec);
    get_BYTE(&obj->m_Hour);
    get_BYTE(&obj->m_Minute);
    get_BYTE(&obj->m_Second);
    off += 1;
    return off;
  }
  int get_DATETIME(DATETIME* obj) {
    get_NETDATE(&obj->m_Date);
    get_NETTIME(&obj->m_Time);
    return off;
  }
  int get_VUSER(VUSER* obj) {
    get_INT(&obj->m_UserType);
    get_CID(&obj->m_UserID);
    get_DATETIME(&obj->m_TypeUpdTime);
    get_DATETIME(&obj->m_RegistTime);
    get_DATETIME(&obj->m_LastUpdTime);
    return off;
  }
  int ALIGN4(int size) {
    return (((size) + 3) & ~3);
  }
  int get_USER(USER* obj) {
    get_INT(&obj->m_FeatBytes);
    get_INT(&obj->m_PicBytes);
    get_INT(&obj->m_RightID);
    get_CID(&obj->m_ImageSID);
    get_CID(&obj->m_UserID);
    get_CID(&obj->m_Admin);
    get_CID(&obj->m_AppendID);
    get_CID(&obj->m_UserName);
    get_CID(&obj->m_Department);
    get_DATETIME(&obj->m_DeadLine);
    get_DATETIME(&obj->m_RegistTime);
    get_DATETIME(&obj->m_LastUpdTime);
    get_DATETIME(&obj->m_TypeUpdTime);
    get_BYTE(&obj->m_UserFlag);
    get_BYTE(&obj->m_UserType);

    if (obj->m_FeatBytes > 0) {
      myrealloc(&obj->m_Feat, obj->m_FeatBytes);
      get_BYTES(obj->m_Feat, obj->m_FeatBytes);
    }

    if (obj->m_PicBytes > 0) {
      myrealloc(&obj->m_Pic, obj->m_PicBytes);
      get_BYTES(obj->m_Pic, obj->m_PicBytes);
    }

    off = ALIGN4(off);
    return off;
  }
  int get_RECORDRANGE(RECORDRANGE* obj) {
    get_INT(&obj->m_Count);
    get_INT(&obj->m_Flag);
    get_CID(&obj->m_SID);
    return off;
  }
  int get_RECORD(RECORD* obj) {
    get_INT(&obj->m_RecType);
    get_INT(&obj->m_Score);
    get_INT(&obj->m_PicBytes);
    get_CID(&obj->m_ChangID);
    get_CID(&obj->m_UserID);
    get_CID(&obj->m_SerialID);
    get_CID(&obj->m_AppendID);
    get_CID(&obj->m_UserName);
    get_CID(&obj->m_Department);
    get_DATETIME(&obj->m_RecTime);
    get_BYTE(&obj->m_VerifyMode);
    get_BYTE(&obj->m_Source);

    if (obj->m_PicBytes > 0) {
      myrealloc(&obj->m_Pic, obj->m_PicBytes);
      get_BYTES(obj->m_Pic, obj->m_PicBytes);
    }

    off = ALIGN4(off);
    return off;
  }
  int get_NETPRO(NETPRO* obj) {
    get_BYTE(&obj->m_WebPro1);
    get_BYTE(&obj->m_WebPro2);
    get_STRING(obj->m_WebUrl1, URL_LEN);
    get_STRING(obj->m_WebUrl2, URL_LEN);
    get_STRING(obj->m_IPAdr, IP_LEN);
    get_STRING(obj->m_MIPAdr, IP_LEN);
    get_STRING(obj->m_NetMask, IP_LEN);
    get_STRING(obj->m_GateWay, IP_LEN);
    get_STRING(obj->m_Dns, IP_LEN);
    return off;
  }
};
