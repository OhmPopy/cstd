/////////////////////////////////////////////////////////////
// ��־��
// ���� 08:54 2016/5/31 ���ڶ� ������������Ƶ���벻����
// ���� 11:00 2016/5/5 ������    ����ʵʱʶ���¼�Ļ�ȡbug
// 9:30 2015/9/12  �����û����ؽӿںͼ�¼���ؽӿڣ���������������Ϣ��ȡ�ӿڡ�dev.cmd_GetNetPro();
/////////////////////////////////////////////////////////////
// ʹ��˵��:
// ��SDK ����������ʶ��3����
// ���������õ�TCP������ʹ�ü����ļ�ĩβ�� test1 ������
// ��Ƶ����õ�UDP������豸���ֻ���Ҫ���ó�ͬһ�����Ρ�
// ������Ƶ���� dev.cmd_StartVideo("192.168.1.3");
/////////////////////////////////////////////////////////////
package org.coprometro.CHFaceLib;

//import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.*;
import java.net.*;
import java.nio.charset.Charset;

import javax.imageio.ImageIO;

public class pmdev extends Thread {
	public static final int TCPPORT = 13333; // �������˿�
	public static final int OFFSET_SIZE = 20; // ��ͷ�ֽ�
	public static final int size_NCMD = 20; // ����ͷ�ֽ�
	public static final int PACK_SIZE = (60 * 1024); // ���
	public static final int ID_LEN = 24; // id����

	public static final int M_USER = 0x01; // �û�
	public static final int M_VERIFY = 0x02; // ��֤��¼
	public static final int M_ALARM = 0x03; // ������¼
	public static final int M_OPTLOG = 0x04; // ��־��¼
	public static final int M_CIMAGE = 0x05; // ���ע���ռ�¼
	public static final int M_RANGE = 0x06; // ���伯����������
	public static final int M_AUTH = 0x07; // �ͻ��������֤
	public static final int M_TIME = 0x08; // ϵͳʱ��
	public static final int M_HEART = 0x09; // ������
	public static final int M_DEVINFO = 0x0a; // �豸������Ϣ
	public static final int M_STATIS = 0x0b; // ͳ����Ϣ
	public static final int M_DEVSET = 0x0c; // �豸��������
	public static final int M_FORMAT = 0x0d; // �豸��ʽ��
	public static final int M_DOOR_STATE = 0x0e; // ��״̬���� ,��3��״̬��0����״̬ �� 1����״̬��
													// 2����״̬
	public static final int M_REMOTE_OPEN = 0x0f; // Զ�̿�����,
	public static final int M_TGROUP = 0x10; // ʱ����
	public static final int M_DSPRIGHT = 0x11; // Ȩ��
	public static final int M_UGROUP = 0x12; // �û���
	public static final int M_MCAST = 0x13; // �ಥ��ַ
	public static final int M_APPUPDATE = 0x14; // �������
	public static final int M_VIDEO_OUT = 0x23; // UDP������Ƶ���� NET_ADDRESS
	public static final int M_USER_EX = 0x24; // �����û��ϴ�����
	public static final int M_REBOOT = 0x25; // �豸����
	public static final int M_RANGE_EX = 0x26; // ��¼��������
	public static final int M_SET_IP = 0x27; // Զ���޸�IP��ַ
	public static final int M_NOPEN = 0x28; // N+1��������
	public static final int M_IOCTRL = 0x29; // IO����
	public static final int M_NETPRO = 0x30; // ��������

	public static final int S_SET = 0x81; // �ϴ�\����
	public static final int S_DEL = 0x82; // ɾ��
	public static final int S_GET = 0x83; // ����\���
	public static final int S_REALTIME_SEND = 0x84; // �ɼ�ʵʱ����
	public static final int S_REALTIME_LOAD = 0x85; // ʵʱ����
	public static final int S_REALTIME_CAP = 0x86; // ʵʱ�û��ɼ�
	public static final int S_REALTIME_CAP_INFO = 0x88; // Զ�̲ɼ�ʵ�ǽ�����Ϣ
	public static final int S_UPDATE_SEND = 0x87; // ����ʵʱ����
	public static final int S_CLEAR_INOUT = 0x88; // ���ͳ�Ƽ���

	// �ͻ�������ת������������
	public static final int C_CNDEV = 0xB0; // clientͨ����ת�������豸����
	public static final int C_VERIFY = 0xB1; // ��ת��������֤client���
	public static final int C_HEART = 0xB2; // ��ת��������client��������
	// �ͻ�������ת������������
	public static final int S_CN = 0xc0; // ��������
	public static final int S_DN = 0xc1; // �Ͽ�����

	// ����ִ�д���״̬
	public static final int SUC_EXECUT = 0x00; // �ɹ�
	public static final int ERR_EXECUT = 0x0A; // ʧ��
	public static final int ERR_BUSY = 0x01; // �豸æ
	public static final int ERR_LIMIT = 0x02; // �Ѵ�����
	public static final int ERR_NOFIND = 0x03; // û���ҵ���Ӧ����
	public static final int ERR_SAVE_FAIL = 0x04; // ���ݱ���ʧ��
	public static final int ERR_SET_FAIL = 0x05; // ����ʧ��
	public static final int ERR_VERIFY_FAIL = 0x06; // ��֤ʧ��
	public static final int ERR_FROMAT = 0x07; // ��ʽ��ʧ��
	public static final int ERR_PARAM = 0x08; // ��������
	public static final int ERR_DISABLE = 0x09; // Ҫ��ִ�еĹ���û��ʹ��
	public static final int ERR_SUPPORT = 0x10; // ��֧�ֵ�����
	public static final int ERR_INPUTDATA = 0x11; // ����˴�����������쳣

	public static final int CMD_STATE_REPLY = 0x40000000; // ��Ǹ�����Ϊ״̬�ظ�����
	public static final int CMD_DOWN_ID = 0x80000000; // ��������ID����,���ȼ��ñ��
	public static final int CMD_ID_ALL = 0x00000100; // �����������ݵ����伯��
	public static final int CMD_ID_TIME_RANGE = 0x00000200; // �����ƶ�ʱ������伯��
	public static final int CMD_ID_RANGE = 0x00000400; // ȷ��ָ����������伯��

	// ��������
	public static final int DL_IMAGE = 0x00000100; // ��Ҫ������Ƭ
	public static final int DL_FEAT = 0x00000200; // ��Ҫ��������
	public static final int DL_APPENDID = 0x00000400; // ʵʱ����ָʾCID�Ǳ�Ż��ǿ���
	public static final int LD_ENROLL = 0x00000800; // ��ҪԶ�̲ɼ�
	public static final int USER_TEXT = 0x00000400; // �û�������Ϣ
	public static final int BATCH_END = 0x00000800; // ��������

	// �û���֤
	public static final int AUTH_VERIFY = 0x00000100; // �û������֤

	// ��״̬: �����أ���ָ��Ϊ����״̬
	public static final int DOOR_OPEN = 0x00000100; // ��
	public static final int DOOR_CLOSE = 0x00000200; // ��

	// ʱ��������
	public static final int DL_NORMAL_TG = 0x00000100; // ��������ʱ����
	public static final int DL_HOLIDAY_TG = 0x00000200; // ���ؽڼ���

	// �������
	// �û����
	// ������Ͽ���: ͬһ���û�����������֤����������֤��ǿ���û�������֤��ͨ�û�
	// ������Ͽ���: �����Ժ���˳����֤��ֻҪ�����֤�ܽ���������֤��Ҫ����֤ͨ��.
	public static final int G_ORDER = 0x01;

	public static final int CMD_M_ID = 0xA1;
	public static final int CMD_M_AUTH = M_AUTH;
	public static final int CMD_M_HEART = M_HEART;
	public static final int CMD_M_DEVINFO = M_DEVINFO;
	public static final int CMD_M_STATIS = M_STATIS;
	public static final int CMD_M_DEVSET = M_DEVSET;
	public static final int CMD_M_TIME = M_TIME;
	public static final int CMD_M_FORMAT = M_FORMAT;
	public static final int CMD_M_EOPEN = M_DOOR_STATE;
	public static final int CMD_M_REMOTE_OPEN = M_REMOTE_OPEN;
	public static final int CMD_M_NOPEN = M_NOPEN;
	public static final int CMD_M_USER = M_USER;
	public static final int CMD_M_VERIFY = M_VERIFY;
	public static final int CMD_M_ALARM = M_ALARM;
	public static final int CMD_M_OPTLOG = M_OPTLOG;
	public static final int CMD_M_CIMAGE = M_CIMAGE;
	public static final int CMD_M_RANGE = M_RANGE;
	public static final int CMD_M_TGROUP = M_TGROUP;
	public static final int CMD_M_DSPRIGHT = M_DSPRIGHT;
	public static final int CMD_M_UGROUP = M_UGROUP;
	public static final int CMD_M_MCAST = M_MCAST;
	public static final int CMD_M_APPUPDATE = M_APPUPDATE;
	public static final int CMD_M_VIDEO_OUT = M_VIDEO_OUT;
	public static final int CMD_M_USER_EX = M_USER_EX;
	public static final int CMD_M_RECORD_EX = M_RANGE_EX;
	public static final int CMD_M_REBOOT = M_REBOOT;
	public static final int CMD_M_IOCTRL = M_IOCTRL;
	public static final int CMD_M_NETPRO = M_NETPRO;

	public static final int CMD_S_SET = S_SET; // �ϴ�\����
	public static final int CMD_S_DEL = S_DEL; // ɾ��
	public static final int CMD_S_GET = S_GET; // ����\���
	public static final int CMD_S_REALTIME_SEND = S_REALTIME_SEND; // �ɼ�ʵʱ����
	public static final int CMD_S_REALTIME_LOAD = S_REALTIME_LOAD; // ʵʱ����
	public static final int CMD_S_REALTIME_CAP = S_REALTIME_CAP; // ʵʱ�û��ɼ�
	public static final int CMD_S_REALTIME_CAP_INFO = S_REALTIME_CAP_INFO; // Զ�̲ɼ�ʵ�ǽ�����Ϣ
	public static final int CMD_S_UPDATE_SEND = S_UPDATE_SEND; // ����ʵʱ����
	public static final int CMD_S_CLEAR_INOUT = S_CLEAR_INOUT; // ���ͳ�Ƽ���

	// ״̬���λ
	public static final int F_ERR_BUSY = ERR_BUSY;
	public static final int F_ERR_LIMIT = ERR_LIMIT;
	public static final int F_ERR_NOFIND = ERR_NOFIND;
	public static final int F_ERR_SAVE_FAIL = ERR_SAVE_FAIL;
	public static final int F_ERR_SET_FAIL = ERR_SET_FAIL;
	public static final int F_ERR_VERIFY_FAIL = ERR_VERIFY_FAIL;

	// �������λ
	public static final int F_DL_ID = CMD_DOWN_ID; // ����ID����
	public static final int F_DL_ID_ALL = CMD_ID_ALL; // �����������ݵ�ID����
	public static final int F_ID_TIME_RANGE = CMD_ID_TIME_RANGE; // �����ƶ�ʱ���ID����
	public static final int F_ID_RANGE = F_ID_TIME_RANGE << 1;
	public static final int F_AUTH_VERIFY = AUTH_VERIFY; // �û������֤
	public static final int F_DL_PIC = DL_IMAGE; // ��Ҫ������Ƭ
	public static final int F_DL_FEAT = DL_FEAT; // ��Ҫ������������
	public static final int F_DL_APPENDID = DL_APPENDID; // Զ�̼���ʱ��m_ID��ʾ����
	public static final int F_DL_NORMAL_TG = DL_NORMAL_TG; // ��������ʱ����
	public static final int F_DL_HOLIDAY_TG = DL_HOLIDAY_TG; // ���ؽڼ���
	public static final int F_DOOR_CLOSE = DOOR_CLOSE; // �������ţ�����Ϊ��������
	public static final int F_DOOR_OPEN = DOOR_OPEN; // �������߳���״̬ȡ��
	public static final int F_G_ORDER = G_ORDER; // ��Ͽ�������
	// public static final int F_DEFAULT_NO_GROUP = DEFAULT_NO_GROUP;

	// û��ָ���û�Ȩ��,�û�Ĭ��Ȩ��IDΪDEFAULT_NO_RIGHT����DEFAULT_SINGLE_OPEN,DSP�ϲɼ����û���
	// ��������Ա����ͳһ����ΪDSP_DEFAULT_SINGLE_OPEN
	// Ĭ����Ȩ�ޣ�Ĭ�ϵ�һ����Ȩ�ޣ�����û�������û�����Ȩ��֧�����ʱ����
	// DEFAULT_NO_RIGHT: DSP,����������û�,Ĭ��ֵ֮һ��Ȩ��
	// DEFAULT_SINGLE_OPEN: DSP,����������û���Ĭ��ֵ֮����һ����Ȩ�ޣ���Ȩ��û��ʱ�������
	public static final int DEFAULT_NO_RIGHT = -2;
	public static final int DEFAULT_SINGLE_OPEN = -1;
	public static final int MAX_GROUPUSER = 60;
	public static final int TGROUP_NUM = 8;

	public static final int TIMEGROUP_NUMS = TGROUP_NUM; // ʱ�������
	public static final int USER_COMBINS = MAX_GROUPUSER; // �û���ϸ���

	// ����
	public class NETDATE {
		public int m_Year;
		public byte m_Month;
		public byte m_Day;
		public static final int size = 8;

		public String tostring() {
			return "" + m_Year + "/" + m_Month + "/" + m_Day;
		}
	}

	// ʱ��
	public class NETTIME {
		public int m_Msec; // ����
		public byte m_Hour;
		public byte m_Minute;
		public byte m_Second;
		public static final int size = 8;

		String tostring() {
			return "" + m_Hour + ":" + m_Minute + "-" + m_Second + "-" + m_Msec;
		}
	}

	public class DATETIME {
		public NETDATE m_Date;
		public NETTIME m_Time;
		public static final int size = NETDATE.size + NETTIME.size;

		String tostring() {
			return "����:" + m_Date.tostring() + " ʱ��: " + m_Date.tostring();
		}
	}

	// ���ָ��ʱ��ε�ID����
	public class TIMESE {
		public NETDATE m_Start; // ��¼��ʼʱ��
		public NETDATE m_End; // ��¼����ʱ��
		public static final int size = NETDATE.size * 2;

		TIMESE(NETDATE Start, NETDATE End) {
			m_Start = Start;
			m_End = End;
		}
	};

	public static final int DL_OPENINTERVAL = 0x00000001; // ����ָ����¼��ˮ��֮���N����¼������ָ����¼����

	// �ɱ�����ṹ�塣����Ԫ�ؾ���ʲô���ݣ���Ҫ����NCMD�趨������������
	public class CARRAY {
		public int m_Count; // ����Ԫ�صĸ���
		public byte[] m_Block; // �����ַ, 4��Ϊ�˽ṹ����ֽڶ���
	};

	public static final int WEB_LOADUSER = 0x0001; // �����û�
	public static final int WEB_WRITEUSER = 0x0002; // д�û�
	public static final int WEB_WRITEREC = 0x0004; // д��¼
	public static final int WEB_SYNCTIME = 0x0008; // ͬ��ʱ��

	// ��������
	public class NETPRO {
		public byte m_WebPro1;
		public byte m_WebPro2;
		public String m_WebUrl1;// = new byte[127];
		public String m_WebUrl2;// = new byte[127];
		public String m_IPAdr;// = new byte[16];
		public String m_MIPAdr;// = new byte[16];
		public String m_NetMask;// = new byte[16];
		public String m_GateWay;// = new byte[16];
		public String m_Dns;// = new byte[16];
		public static final int size = 128 + 128 + 16 * 5;
	}

	public static final int CHECK_TIME = 0x01; // ���ʱ��
	public static final int CHECK_WEEK = 0x02; // �������
	public static final int CHECK_DAY = 0x04; // �������
	public static final int CHECK_MONTH = 0x08; // ����·�
	public static final int CHECK_YEAR = 0x10; // ������

	public static final int WEEK_1 = 0x01; // ����һ��Ч
	public static final int WEEK_2 = 0x02; // ���ڶ���Ч
	public static final int WEEK_3 = 0x04; // ��������Ч
	public static final int WEEK_4 = 0x08; // ��������Ч
	public static final int WEEK_5 = 0x10; // ��������Ч
	public static final int WEEK_6 = 0x20; // ��������Ч
	public static final int WEEK_7 = 0x40; // ��������Ч

	// ����ʱ����Ч , ����ʱ���飬�ڼ���ʱ����
	public static final int ANY_TIME = -1;
	public static final int TG_NORMAL = 1;
	public static final int TG_HOLIDY = 2;

	// ʱ����
	public class DSPTIMEG {
		public int m_TGID; // ʱ����ID
		public DATETIME m_Start; // ʱ���鿪ʼʱ��
		public DATETIME m_End; // ʱ�������ʱ��
		public byte m_TGType; // ʱ��������, TG_NORMAL,TG_HOLIDY
		public byte m_CheckFlag; // ʱ����������m_CheckFlag = CHECK_TIME|CHECK_WEEK
		public byte m_WeekFlag; // �������ʱ�������Щ������Ч������m_WeekFlag = WEEK_1|WEEK_5
		public byte m_Reserve; // �����ֽ�
		public static final int size = 4 + DATETIME.size + 4;
	}

	public static final int CHECK_HOLIDY = 0x01; // Ȩ�޼��ڼ���
	public static final int ACTION_LOCK = 0x02; // �������
	public static final int ACTION_OUTPUT = 0x04; // �������
	public static final int ACTION_N_1 = 0x08; // ��Ȩ����ҪN+1

	// Ȩ��
	public class DSPRIGHT {
		// Ȩ��ID
		public int m_RightID;

		// ʱ����ID , (m_TimeGID[0]==ANY_TIME)δָ��ʱ���飬���ŷ�ʽ����ʱ�����ƣ�����ʱ�����֤�ɹ�ִ�п��ŷ�ʽ.
		public int[] m_TimeGID = new int[TGROUP_NUM];

		// BIT(0):1 ���ڼ���, �����ڼ���
		// BIT(1):1 ����, 1��� 0�����
		// BIT(2):1 ����, 1��� 0�����
		// BIT(3):1 N��1, ���� 0�ر�
		public int m_Mode;

		public static final int size = (TGROUP_NUM + 2) * 4;
	};

	// ��ʾ�û��������κ���
	public static final int INVALID_GROUPID = -1;

	public class USERGROUP {
		// ��ID
		public int m_GroupID;
		// BIT(0):1����0����
		public byte m_GroupFlag;
		// ��ϳɹ����������ACTION_LOCK|ACTION_OUTPUT|ACTION_N_1
		public byte m_Param;
		// ��ͨ�û�����Ч�û���
		public byte m_NormalValid;
		// ǿ���û�����Ч�û���
		public byte m_ForceValid;
		// ��ͨ�û���ϣ����ȼ���
		public String[] m_NormalUsers;// [MAX_GROUPUSER];
		// ǿ���û���ϣ����ȼ���
		public String[] m_ForceUsers; // [MAX_GROUPUSER];
		public static final int size = 8 + ID_LEN * 2 * MAX_GROUPUSER;
	};

	public class GROUPEX {
		public int m_GroupID;
		public int m_StrSize;
		public byte m_GroupFlag;
		public byte m_MinNum;
		public byte[] m_BLock;//
	};

	public class VUSER {
		public int m_UserType; // �ն��豸�ϵ��û�����(��ͨ���������� -- 0 , 1 , 2)
		public String m_UserID; // �û�ID
		public DATETIME m_TypeUpdTime; // �û����͸���ʱ��
		public DATETIME m_RegistTime; // ����ע��ʱ�䣬�������ɼ�ʱ�䣬�ɼ��������²ɼ�Ҫ���´�ʱ��
		public DATETIME m_LastUpdTime; // �����µ�ʱ�䣬ʶ��ʱ�����������»������²ɼ�Ҫ���´�ʱ��

		public String tostring() {
			String[] typestr = { "��ͨ", "����", "����" };
			String ss = "";
			ss += "�ն��豸�ϵ��û�����: " + m_UserType + "(" + typestr[m_UserType] + ")";
			ss += "\n�û�ID: " + m_UserID;
			ss += "\n�û����͸���ʱ��" + m_TypeUpdTime.tostring();
			ss += "\n����ע��ʱ��" + m_UserID;
			ss += "\n�����µ�ʱ��" + m_UserID;
			return ss;
		}
	};

	// �û���Ϣ
	public static final int USER_CARD_INVALID = 0x01; // ����ʧ
	public static final int USER_DLine_VALID = 0x02; // ʧЧʱ����Ч
	public static final int USER_BLACK = 0x04; // ������
	public static final int USER_MIX1N = 0x08; // 1N������֤
	public static final int USER_VIPCARD = 0x10; // VIP�û�
	public static final int USER_FINGER_PRINT = 0x20; // ָ����֤
	public static final int USER_SYNC_FINGEER = 0x40; // ָ��ͬ��

	public class USER {
		public int m_RightID; // �û�Ȩ��ID,���ֿ���ֵ(DEFAULT_NO_RIGHT,DEFAULT_SINGLE_OPEN,�û�ָ��)
		public String m_ImageSID; // �ϴ�ע���գ���Ӧ�ı��ID��ȫ��Ψһ
		public String m_UserID; // �û���ʶ
		public String m_Admin; // ����ע�����Ա,��ʶ���û��������������ĸ�����Ա�ɼ�
		public String m_AppendID; // ���Ż�����������1:1�ĸ������ȷ����Ϣ
		public String m_UserName; // �û���
		public String m_Department; // ��������
		public DATETIME m_DeadLine; // ʧЧʱ��
		public DATETIME m_RegistTime; // ����ע��ʱ�䣬�������ɼ�ʱ�䣬�ɼ��������²ɼ�Ҫ���´�ʱ��
		public DATETIME m_LastUpdTime; // �����µ�ʱ�䣬ʶ��ʱ�����������»������²ɼ�Ҫ���´�ʱ��
		public DATETIME m_TypeUpdTime; // �û����ͱ��ʱ��
		public byte m_UserFlag; // �û�״̬���
		public byte m_UserType; // �ն��豸�ϵ��û�����(��ͨ���������� -- 0 , 1 , 2)
		public byte[] m_Feat; // ����������==0 ��ʾû����������
		public byte[] m_Pic; // ע��ͼ�� ==null ��ʾû��ͼƬ
		int FeatLen() {
			int m_Feat_length = 0;
			if (m_Feat != null) {
				m_Feat_length = m_Feat.length;
			}
			return m_Feat_length;
		}
		int PicLen() {
			int m_Pic_length = 0;
			if (m_Pic != null) {
				m_Pic_length = m_Pic.length;
			}
			return m_Pic_length;
		}
		// ע��ͼƬ,����JPG,������������m_UserIDΨһ�󶨵�.m_Block��ǰm_FeatBytes���ֽڱ�ʾ��������,
		// ��m_PicBytes���ֽڱ�ʾͼƬ.
		public int size() {
			return ALIGN(6 * ID_LEN + 4 * size_DATETIME + 2 * 4 + FeatLen()
					+ PicLen()+16, 4);
		}
	}

	// ��¼����:
	// 0x01 - 0x20: �����¼�
	// 0x20 - 0x40: Ӳ�������¼�
	// 0x40 - 0x50: ��������
	// 0x50 - 0x60: �������
	// 0xf0 - 0xff: ��������¼�
	// ����: δ����

	public static final int VERIFY_SUCC = 0x01; // ��֤�ɹ�
	public static final int VERIFY_FAIL = 0x02; // ��֤ʧ��
	public static final int ADMIN_SUCC = 0x03; // ����Ա��֤�ɹ�
	public static final int EMER_OPEN = 0x04; // ��������
	public static final int RIGHT_OPEN = 0x05; // Ȩ�޿���
	public static final int GROUP_OPEN = 0x06; // ��Ͽ���
	public static final int BUTTON_OPEN = 0x07; // ��ť����

	public static final int ALARM_HD_MANGET_TIMEOUT = 0x20; // �Ŵų�ʱ
	public static final int ALARM_HD_MANGET_ILLOPEN = 0x21; // �ŴŷǷ�����
	public static final int ALARM_HD_OFFLINE = 0x22; // ǰ�˵��߱���
	public static final int ALARM_HD_BREAK = 0x30; // ���𱨾�
	public static final int ALARM_HD_SHOCK = 0x31; // �𶯱���
	public static final int ALARM_HD_FPOWR = 0x36; // ǰ�˹����쳣����
	public static final int ALARM_HD_UPS_ON = 0x37; // UPS���õ�ؿ���
	public static final int ALARM_HD_UPS_OFF = 0x38; // UPS���õ�عر�

	public static final int ALARM_HD_ASSIST = 0x40; // �������봥��
	public static final int ALARM_HD_WL_DOOR_MAGNETIC = 0x41; // ����
	public static final int ALARM_HD_WL_SMOKE = 0x42;
	public static final int ALARM_HD_WL_GAS = 0x43;
	public static final int ALARM_HD_WL_BUTTON1 = 0x44;
	public static final int ALARM_HD_WL_BUTTON2 = 0x45;
	public static final int ALARM_HD_WL_BUTTON3 = 0x46;
	public static final int ALARM_HD_WL_BUTTON4 = 0x47;
	public static final int ALARM_HD_WL_AREA1 = 0x48;
	public static final int ALARM_HD_WL_AREA2 = 0x49;
	public static final int ALARM_HD_WL_AREA3 = 0x4A;
	public static final int ALARM_HD_WL_AREA4 = 0x4B;
	public static final int ALARM_HD_WL_AREA5 = 0x4C;
	public static final int ALARM_HD_WL_AREA6 = 0x4D;
	public static final int ALARM_HD_WL_AREA7 = 0x4E;

	public static final int ALARM_AB_RULE = 0xE0;

	public static final int ALARM_SF_BACKLIST = 0xF0; // ��������֤����
	public static final int ALARM_SF_ILLCARD = 0xF1; // ��Ч��(��ʧ)
	public static final int ALARM_SF_ILLTIME = 0xF2; // �Ƿ�ʱ��ʶ��
	public static final int ALARM_SF_DEADLINE = 0xF3; // ʧЧʱ��
	public static final int ALARM_SF_DANGER_OPEN = 0xF4; // в�ȿ���
	public static final int ALARM_SF_SUPER_OPEN = 0xF5; // �������뿪��
	public static final int LINK_EVENT = 0xF6; // �����¼�

	public class RECORDRANGE {
		// ���ض�����,<=0 ��ʾ����m_SID֮����������м�¼,�˳�����m_SID������Ч
		public int m_Count;
		// DL_OPENINTERVAL�򿪿���������,�������������
		public int m_Flag;
		// ����һ����ʼ��m_SID��ʾ��¼��ȷ����ˮ��,
		// ��ˮ���ַ������:
		// SN(MAC��3�ֽ�) + TYPE(��¼���) + DATETIME(����) + ID(�Զ���) = (6BYTE + 1BYTE +
		// 14BYTE + 2BYTE + \0)
		public String m_SID;
		public static final int size = ID_LEN + 8;
	};

	// ��¼��Ϣ,ʶ���¼��������¼
	public class RECORD {
		public int m_RecType; // ��¼����
		public int m_Score; // ʶ��÷� , �����¼�Ϊ�����¼���LINKID
		public int m_PicBytes; // ��¼ͼ����ֽ�����==0 ��ʾû�м�¼ͼƬ
		public String m_ChangID; // ע���ռ�¼��
		public String m_UserID; // �û�ID,���Ϊ�գ���ʾ�Ƿ���¼ , �����¼�Ϊ�¼�����Text
		public String m_SerialID; // ʶ���¼��ˮ��ID
		public String m_AppendID; // ���Ż�����������1:1�ĸ������ȷ����Ϣ
		public String m_UserName; // �û���
		public String m_Department; // ��������
		public DATETIME m_RecTime; // ��¼ʱ��
		public byte m_VerifyMode; // ��֤ģʽ,��:VERIFY_CARD|VERIFY_FACE_1N,ˢ��+����
		public byte m_Source; // �¼�Դ�豸��485��ַ
		public byte[] m_Pic; // ��¼ͼ��,����JPG , 2��Ϊ�˽ṹ����ֽڶ���

		public int size() {
			return ALIGN(4 * 4 + ID_LEN * 6 + m_Pic.length, 4);
		}
	}

	// public static final int CGIMG_STEP( obj ) ALIGN4(
	// sizeof(CHANGEIMAGE)+(this).m_PicBytes-4 )
	// public static final int DSPLOG_STEP( obj ) ALIGN4( sizeof(DSPLOG) )

	// ������־����

	public static final int DSPLOG_ADDUSER = 0x01; // �����û�
	public static final int DSPLOG_DELUSER = 0x02; // ɾ���û�
	public static final int DSPLOG_REREGIST = 0x03; // ���²ɼ�
	public static final int DSPLOG_CAHNGETYPE = 0x04; // �ı��û�����
	public static final int DSPLOG_UDISK_ULUSER = 0x05; // U���ϴ��û�
	public static final int DSPLOG_UDISK_DLUSER = 0x06; // U�������û�
	public static final int DSPLOG_UDISK_DLRECORD = 0x07; // U�����ؼ�¼
	public static final int DSPLOG_UDISK_UPDATEAPP = 0x08; // U�̸��³���

	// �豸������־
	public class DSPLOG {
		public int m_LogType; // ��־����
		public String m_SerialID; // ��ˮ��ID
		public String m_Admin; // ����Ա
		public String m_BeOptUser; // ������Ա
		public DATETIME m_RecTime; // ��¼ʱ��
	}

	// ���ע����
	public class CHANGEIMAGE {
		public String m_UserID; // �û�ID
		public String m_SerialID; // ��ˮ��ID
		public String m_AppendID; // ���Ż�����������1:1�ĸ������ȷ����Ϣ
		public String m_UserName; // �û���
		public String m_Department; // ��������
		public String m_Admin; // ����ע�����Ա,��ʶ���û��������������ĸ�����Ա�ɼ�
		public DATETIME m_RecTime; // ��¼ʱ��
		public byte[] m_Pic; // ע�����ڴ�

		public int size() {
			return ALIGN(6 * ID_LEN + 8 + m_Pic.length, 4);
		}
	};

	// DSP���ͳ����Ϣ
	public class STATIS {
		public int m_TotalUsers; // ��ǰ�豸�����û���
		public int m_NoFeatUser; // û�вɼ������������û���
		public int m_TotalDays; // ʶ���¼����������
		public int m_TotalRecords; // �ܼ�¼��
		public int m_TotalAlarm; // �ܱ�����¼��
		public int m_TotalDspLog; // �ܲ�����־��Ŀ
		public int m_TotalChangeImage; // �ܱ��ע����

		public String tostring() {
			String ss = "";
			ss += ("��ǰ�豸�����û���: " + m_TotalUsers);
			ss += ("\nû�вɼ������������û���: " + m_NoFeatUser);
			ss += ("\nʶ���¼����������: " + m_TotalDays);
			ss += ("\n�ܼ�¼��: " + m_TotalRecords);
			ss += ("\n�ܱ�����¼��: " + m_TotalAlarm);
			ss += ("\n�ܲ�����־��Ŀ: " + m_TotalDspLog);
			ss += ("\n�ܱ��ע����: " + m_TotalChangeImage);
			return ss;
		}
	};

	// ��֤��Ϣ
	public class AUTH {
		public String m_UserName; // �û���
		public String m_Password; // ����
	};

	// �豸������Ϣ
	public class BASEINFO {
		public int m_DevType; // �豸����
		public int m_LimitUser; // ����������
		public int m_Limit1NUser; // 1N�û�����
		public String m_SN; // �豸���
		public String m_Ver; // DSP����汾
		public String m_Space; // ����������Ϣ

		public String tostring() {
			String ss = "";
			ss += ("�豸���� : " + m_DevType);
			ss += ("\n��ʹ���������� : " + m_LimitUser);
			ss += ("\n1N�û����� : " + m_Limit1NUser);
			ss += ("\n�豸��� : " + m_SN);
			ss += ("\nDSP����汾 : " + m_Ver);
			ss += ("\n����������Ϣ : " + m_Space);
			return ss;
		}
	}

	// Զ�̲ɼ�������Ϣ
	public class RCAPINFO {
		public int m_Total; // �ɼ�����
		public int m_Count; // ��ǰ�ɼ���
		public String m_UserID; // �û��ǼǱ��
	};

	// 2.0��֧�ֵĹ���
	public static final int RECORD_SAVEFAIL = 0x00000001; // �Ƿ񱣴�ʶ��ʧ�ܼ�¼
	public static final int SUPER_PASSWORD = 0x00000002; // �������뿪����Ч
	public static final int HDBEEP_OPEN = 0x00000004; // ����/����(Ӳ����������������)
	public static final int REALTIME_RECORD = 0x00000010; // ʵʱ��̬��¼��ʾ
	public static final int REALTIME_USERLOAD = 0x00000020; // �����Ż��߿���ʶ��ʱ������޷��ڵ�ǰdsp�л����������,����������״̬�£����ӹ����ʵʱ������������
	public static final int REALTIME_USERSEND = 0x00000040; // �ɼ��û�ͬ��
	public static final int DOORMANGET_OPEN = 0x00000080; // �����Ŵż�ⱨ��
	public static final int DOORFORCE_OPEN = 0x00000100; // ����в�ȿ��Ź���
	public static final int REMOTE_CAP_SAVE = 0x00000200; // Զ�̿��Ʋɼ����û��Ƿ񱣴浽DSP
	public static final int GPRS_OPEN = 0x00000400; // ����GPRSģ��
	public static final int UPDATE_USERSEND = 0x00000800; // ��������ʱ���Ƿ�ʵʱ�����û�����
	public static final int REALTIME_USERLOAD_PRIOR = 0x00001000; // ������������£������������
	public static final int AB_LOCK = 0x00002000; // AB����
	public static final int DOOR1_NOPEN = 0x00004000; // ��һN+1
	public static final int DOOR2_NOPEN = 0x00008000; // �Ŷ�N+1
	public static final int ENABLE_COUNT = 0x00010000; // ���߼�¼��ͳ��
	public static final int ENCRYP_DISPALY = 0x00020000; // ���߼�¼��ͳ��

	// ��֤ģʽ���ȼ���-��:UID<CARD<11<MIX<1N
	public static final int VERIFY_USERID = 0x01; // ���
	public static final int VERIFY_CARD = 0x02; // ˢ��
	public static final int VERIFY_FACE_11 = 0x04; // 11��������
	public static final int VERIFY_FACE_MIX = 0x08; // �����������
	public static final int VERIFY_FACE_1N = 0x10; // 1N��������
	public static final int VERIFY_FINGER = 0x20; // ָ����֤

	public static final int OUTPUT_WG_YES = 0x01; // ���ΰ��Э��
	public static final int OUTPUT_WG_34 = 0x02; // ָ�����ΰ��34����ָ�����ΰ��26
	public static final int OUTPUT_WG_CTX = 0x04; // ΰ�����ָ������
	public static final int OUTPUT_WG_UID = 0x08; // �����OUTPUT_WG_CTX������û�ID��Ĭ���������

	public static final int WG_OLD = 0;
	public static final int WG_26 = 1;
	public static final int WG_34 = 2;
	public static final int WG_35 = 3;
	public static final int WG_36 = 4;
	public static final int WG_37 = 5;
	public static final int WG_44 = 6;
	public static final int WG_45 = 7;

	// �豸������������
	public class WORKATT {
		// �豸ʱ����,(m_TimeGID[0]==ANY_TIME)δָ��ʱ����,�豸����ʱ����Ч.�豸ʱ�������ȼ������û�Ȩ��ʱ����
		public int[] m_DevCtx; // [ TGROUP_NUM ];
		// �豸Ĭ��Ȩ��: DEFAULT_NO_RIGHT , DEFAULT_SINGLE_OPEN
		public int m_DefaultRight;
		// �μ�2.0�汾����ı��
		public int m_BaseSet;
		// ��������, ������һ��������¼�����һ�ץ�Ĳ�ɫȫ��ͼ.
		public String m_SuperPassword;
		// �Ŵ���ʱʱ�䣬��λ��
		public byte m_DoorMangetTime;
		// ��������ʱ�䣬��λ��
		public byte m_LockTime;
		// �û������֤ģʽ,����:VERIFY_USERID|VERIFY_CARD|VERIFY_FACE_11
		public byte m_VerifyMode;
		// BIT(0) : 0�������չΤ���ź�,1���.
		// BIT(1) : 0���Τ��26,1���Τ��34.
		// BIT(2) : 0����BIT(4)��� , 1���m_OutputCtx��ָ������
		// BIT(4) : 0ֱ���������,1������
		// BIT(4-7): WG���Э��,0-wg26 1-wg34 2-wg35 3-wg36 4-wg37 5-wg44 6-wg45
		public byte m_OutputWG;
		// Τ��Э���������
		public int m_OutputCtx;
	};

	// ������½ṹ��
	public class APPUPDATE {
		public String m_FileName; // �ļ���
		public byte m_TotalFiles; // ��Ҫ���µ��ļ�����
		public byte m_FileIndex; // ��ǰ���µ��ڼ���
		public byte[] m_File; // �ļ�����

		public int size() {
			return ID_LEN + 6 + m_File.length;
		}
	};

	// IO�豸״̬
	public static final int IO_MODE_NORMAL = 0x00; // ����״̬
	public static final int IO_MODE_OPEN = 0x01; // ����״̬
	public static final int IO_MODE_CLOSE = 0x02; // ����״̬
	// �Ŵ�״̬
	public static final int IO_STATE_CLOSE = 0x00; // �ŴŹ�
	public static final int IO_STATE_OPEN = 0x01; // �Ŵſ�
	// ִ�ж���
	public static final int ACT_IO_OPEN = 0x02; // ִ�д򿪶���
	public static final int ACT_IO_OPENEX = 0x04; // ִ�и�������
	public static final int ACT_IO_CLOSE = 0x10; // ִ�йرն���
	public static final int ACT_MODE_SET = 0x20; // ����IO�豸����ģʽ
	public static final int ACT_MODE_GET = 0x40; // ���IO�豸����ģʽ
	public static final int ACT_STATE_GET = 0x80; // ���IO�豸��ǰ״̬

	public class IOCTRL {
		// IO�豸485��ַ
		public byte m_Source;
		// ִ�ж���
		public byte m_Action;
		// IO�豸��ǰģʽ
		public byte m_IOMode;
		// IO�豸��ǰ״̬״̬
		public byte m_IOState;
		public static final int size = 4;
	};

	// NOPEN:Զ���������˿���
	public class NOPEN {
		// m_Users==1Ȩ��ID, m_Users>1���ID
		public int m_CtxID;
		// ��֤�û���Ŀ�� 0���뿪�ţ�1Ȩ�޿��ţ�>1��Ͽ���
		public int m_Users;
		// ������Ϣ
		public IOCTRL m_XOpen;
		// ��֤�û�����
		public String[] m_IDS;// [MAX_GROUPUSER * 2];
	};

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

	public static final int DEV_TYPE_FACE1 = 0x0; // ����ǰ��1
	public static final int DEV_TYPE_FACE2 = 0x1; // ����ǰ��2
	public static final int DEV_TYPE_CARD1 = 0x2; // ˢ����1
	public static final int DEV_TYPE_CARD2 = 0x3; // ˢ����2
	public static final int DEV_TYPE_IN = 0x4; // ��������
	public static final int DEV_TYPE_OUT = 0x5; // �������
	public static final int DEV_TYPE_BUTTON = 0x6; // ���Ű�ť
	public static final int DEV_TYPE_DOOR = 0xF; // �ŵ㱾��

	public static final int IMAGE_UYVY = 0x01;
	public static final int IMAGE_YUYV = 0x02;
	public static final int IMAGE_RGB24 = 0x03;

	public class IMAGE {
		public int m_Size; // ͼ�������ܴ�СBYTES
		public int m_W; // ͼ���
		public int m_H; // ͼ���
		public int m_Step; // �����ݲ���
		public byte m_Type; // ͼ������
		public byte[] m_Block; // ͼ������
	};

	Socket sc;
	DataInputStream netInputStream;
	DataOutputStream netOutputStream;
	// int isexit;
	int m_nIndex;

	public static int bytecopy(byte[] src, int srcoff, byte[] dst, int dstoff,
			int maxlen) {
		maxlen = Math.min(maxlen, src.length - srcoff);
		maxlen = Math.min(maxlen, dst.length - dstoff);
		System.arraycopy(src, srcoff, dst, dstoff, maxlen);
		return maxlen;
	}

	public static int bytefill(byte[] src, int srcoff, int maxlen, byte x) {
		maxlen = Math.min(maxlen, src.length - srcoff);
		for (int i = 0; i < maxlen; ++i) {
			src[srcoff + i] = x;
		}
		return maxlen;
	}

	public static byte[] str2bytes(String data, String charsetName) {
		Charset charset = Charset.forName(charsetName);
		if (data==null) {
			return new byte[0];
		} else {
			return data.getBytes(charset);
		}
	}

	public class set_stream {
		int off;
		byte[] bytes;

		set_stream(byte[] bytes1, int off1) {
			bytes = bytes1;
			off = off1;
		}

		public int set_STRING(String data, int maxlen) {
			byte[] s = str2bytes(data, "GBK");
			int len = bytecopy(s, 0, bytes, off, maxlen);
			if (maxlen > len) {
				bytefill(bytes, off + len, maxlen - len, (byte) 0);
			}
			off += maxlen;
			return off;
		}

		public int set_BYTE(byte data) {
			bytes[off + 0] = (byte) (data);
			off += 1;
			return off;
		}

		public int set_BYTES(byte[] data, int n) {
			int i = 0, m = Math.min(data.length, n);
			for (; i < m; ++i) {
				bytes[off + i] = data[i];
			}
			for (; i < n; ++i) {
				bytes[i] = 0;
			}
			off += n;
			return off;
		}

		public int set_SHORT(short data) {
			bytes[off + 0] = (byte) (data & 0xff);
			bytes[off + 1] = (byte) ((data & 0xff00) >> 8);
			off += 2;
			return off;
		}

		public int set_INT(int data) {
			bytes[off + 0] = (byte) (data & 0xff);
			bytes[off + 1] = (byte) ((data & 0xff00) >> 8);
			bytes[off + 2] = (byte) ((data & 0xff0000) >> 16);
			bytes[off + 3] = (byte) ((data & 0xff000000) >> 24);
			off += 4;
			return off;
		}

		public int set_INTS(int[] arr, int n) {
			int m = Math.min(arr.length, n);
			for (int i = 0; i < m; ++i) {
				set_INT(arr[i]);
			}
			for (; m < n; ++m) {
				set_INT((int) 0);
			}
			return off;
		}

		public int set_CID(String str) {
			return set_STRING(str, ID_LEN);
		}

		public int set_NETDATE(NETDATE date) {
			if (date!=null) {
			set_INT(date.m_Year);
			set_BYTE(date.m_Month);
			set_BYTE(date.m_Day);
			}
			off += 8;
			return off;
		}

		public int set_NETTIME(NETTIME time) {
			if (time!=null) {
			set_INT(time.m_Msec);
			set_BYTE(time.m_Hour);
			set_BYTE(time.m_Minute);
			set_BYTE(time.m_Second);
			}
			off += 8;
			return off;
		}

		public int set_DATETIME(DATETIME datetime) {
			if (datetime!=null) {
			set_NETDATE(datetime.m_Date);
			set_NETTIME(datetime.m_Time);
			} else {
				set_NETDATE(null);
				set_NETTIME(null);
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

		public int set_CIDS(String[] strs, int n) {
			int m = Math.min(strs.length, n);
			for (int i = 0; i < m; ++i) {
				set_CID(strs[i]);
			}
			if (m < n) {
				offset((n - m) * ID_LEN);
			}
			return off;
		}

		public int set_WORKATT(WORKATT workatt) {
			set_INTS(workatt.m_DevCtx, TGROUP_NUM);
			set_INT(workatt.m_DefaultRight);
			set_INT(workatt.m_BaseSet);
			set_CID(workatt.m_SuperPassword);
			set_BYTE((byte) workatt.m_DoorMangetTime);
			set_BYTE((byte) workatt.m_LockTime);
			set_BYTE((byte) workatt.m_VerifyMode);
			set_BYTE((byte) workatt.m_OutputCtx);
			set_INT(workatt.m_OutputCtx);
			return off;
		}

		public int set_USER(USER obj) {
			int off1 = off;
			set_INT(obj.FeatLen());
			set_INT(obj.PicLen());
			set_INT(obj.m_RightID);
			set_CID(obj.m_ImageSID);
			set_CID(obj.m_UserID);
			set_CID(obj.m_Admin);
			set_CID(obj.m_AppendID);
			set_CID(obj.m_UserName);
			set_CID(obj.m_Department);
			set_DATETIME(obj.m_DeadLine);
			set_DATETIME(obj.m_RegistTime);
			set_DATETIME(obj.m_LastUpdTime);
			set_DATETIME(obj.m_TypeUpdTime);
			set_BYTE(obj.m_UserFlag);
			set_BYTE(obj.m_UserType);
			if (obj.m_Feat != null) {
				set_BYTES(obj.m_Feat, obj.m_Feat.length);
			}
			if (obj.m_Pic != null) {
				set_BYTES(obj.m_Pic, obj.m_Pic.length);
			}
			off = off1 + obj.size();
			return off;
		}

		int set_RECORDRANGE(RECORDRANGE obj) {
			int off1 = off;
			set_INT(obj.m_Count);
			set_INT(obj.m_Flag);
			set_CID(obj.m_SID);
			off = off1 + RECORDRANGE.size;
			return off;
		}

		int set_DSPTIMEG(DSPTIMEG obj) {
			int off1 = off;
			set_INT(obj.m_TGID);
			set_DATETIME(obj.m_Start);
			set_DATETIME(obj.m_End);
			set_BYTE(obj.m_TGType);
			set_BYTE(obj.m_CheckFlag);
			set_BYTE(obj.m_WeekFlag);
			set_BYTE(obj.m_Reserve);
			off = off1 + DSPTIMEG.size;
			return off;
		}

		int set_DSPRIGHT(DSPRIGHT obj) {
			int off1 = off;
			set_INT(obj.m_RightID);
			set_INTS(obj.m_TimeGID, TGROUP_NUM);
			set_INT(obj.m_Mode);
			off = off1 + DSPTIMEG.size;
			return off;
		}

		int set_USERGROUP(USERGROUP obj) {
			int off1 = off;
			set_INT(obj.m_GroupID);
			set_BYTE(obj.m_GroupFlag);
			set_BYTE(obj.m_Param);
			set_BYTE(obj.m_NormalValid);
			set_BYTE(obj.m_ForceValid);
			set_CIDS(obj.m_NormalUsers, MAX_GROUPUSER);
			set_CIDS(obj.m_ForceUsers, MAX_GROUPUSER);
			off = off1 + USERGROUP.size;
			return off;
		}

		int set_IOCTRL(IOCTRL obj) {
			int off1 = off;
			set_BYTE(obj.m_Source);
			set_BYTE(obj.m_Action);
			set_BYTE(obj.m_IOMode);
			set_BYTE(obj.m_IOState);
			off = off1 + USERGROUP.size;
			return off;
		}

		int set_APPUPDATE(APPUPDATE obj) {
			int off1 = off;
			set_INT(obj.m_File.length);
			set_CID(obj.m_FileName);
			set_BYTE(obj.m_TotalFiles);
			set_BYTE(obj.m_FileIndex);
			if (obj.m_File != null) {
				set_BYTES(obj.m_File, obj.m_File.length);
			}
			off = off1 + obj.size();
			return off;
		}

		int set_NETPRO(NETPRO obj) {
			int off1 = off;
			set_BYTE(obj.m_WebPro1);
			set_BYTE(obj.m_WebPro2);
			set_STRING(obj.m_WebUrl1, 127);
			set_STRING(obj.m_WebUrl2, 127);
			set_STRING(obj.m_IPAdr, 16);
			set_STRING(obj.m_MIPAdr, 16);
			set_STRING(obj.m_NetMask, 16);
			set_STRING(obj.m_GateWay, 16);
			set_STRING(obj.m_Dns, 16);
			off = off1 + NETPRO.size;
			return off;
		}

	}

	public class get_stream {
		public int off;
		byte[] bytes;

		get_stream(byte[] bytes1, int off1) {
			bytes = bytes1;
			off = off1;
		}

		public byte get_BYTE() {
			byte by = (byte) (0xff & bytes[off]);
			off += 1;
			return by;
		}

		public int get_INT() {
			int ret = (0xff & bytes[off + 0])
					| (0xff00 & (bytes[off + 1] << 8))
					| (0xff0000 & (bytes[off + 2] << 16))
					| (0xff000000 & (bytes[off + 3] << 24));
			off += 4;
			return ret;
		}

		public byte[] get_BYTES(int len) {
			if (len > 0) {
				byte[] by = new byte[len];
				bytecopy(bytes, off, by, 0, len);
				off += len;
				return by;
			}
			return null;
		}

		public String get_STRING(int len) {
			String s = "";
			try {
				s = new String(bytes, off, len, "GBK");
			} catch (Exception e) {
				// li.msg_DEBUGMSG(this, "get_STRING ����");
				e.printStackTrace();
			}
			off += len;
			return s;
		}

		public String get_CID() {
			return get_STRING(ID_LEN);
		}

		public NETDATE get_NETDATE() {
			NETDATE obj = new NETDATE();
			obj.m_Year = get_INT();
			obj.m_Month = get_BYTE();
			obj.m_Day = get_BYTE();
			off += 2;
			return obj;
		}

		public NETTIME get_NETTIME() {
			NETTIME obj = new NETTIME();
			obj.m_Msec = get_INT();
			obj.m_Hour = get_BYTE();
			obj.m_Minute = get_BYTE();
			obj.m_Second = get_BYTE();
			off += 1;
			return obj;
		}

		public DATETIME get_DATETIME() {
			DATETIME obj = new DATETIME();
			obj.m_Date = get_NETDATE();
			obj.m_Time = get_NETTIME();
			return obj;
		}

		public VUSER get_VUSER() {
			VUSER obj = new VUSER();
			obj.m_UserType = get_INT();
			obj.m_UserID = get_CID();
			obj.m_TypeUpdTime = get_DATETIME();
			obj.m_RegistTime = get_DATETIME();
			obj.m_LastUpdTime = get_DATETIME();
			return obj;
		}

		int ALIGN4(int size) {
			return (((size) + 3) & ~3);
		}

		public USER get_USER() {
			USER obj = new USER();
			int m_FeatLen = get_INT();
			int m_PicLen = get_INT();
			obj.m_RightID = get_INT();
			obj.m_ImageSID = get_CID();
			obj.m_UserID = get_CID();
			obj.m_Admin = get_CID();
			obj.m_AppendID = get_CID();
			obj.m_UserName = get_CID();
			obj.m_Department = get_CID();

			obj.m_DeadLine = get_DATETIME();
			obj.m_RegistTime = get_DATETIME();
			obj.m_LastUpdTime = get_DATETIME();
			obj.m_TypeUpdTime = get_DATETIME();
			obj.m_UserFlag = get_BYTE();
			obj.m_UserType = get_BYTE();

			if (m_FeatLen > 0) {
				obj.m_Feat = get_BYTES(m_FeatLen);
			}
			if (m_PicLen > 0) {
				obj.m_Pic = get_BYTES(m_PicLen);
			}
			off = ALIGN4(off);
			return obj;
		}

		public RECORDRANGE get_RECORDRANGE() {
			RECORDRANGE obj = new RECORDRANGE();
			obj.m_Count = get_INT();
			obj.m_Flag = get_INT();
			obj.m_SID = get_CID();
			return obj;
		}

		public RECORD get_RECORD() {
			RECORD obj = new RECORD();
			obj.m_RecType = get_INT();
			obj.m_Score = get_INT();
			obj.m_PicBytes = get_INT();
			obj.m_ChangID = get_CID();
			obj.m_UserID = get_CID();
			obj.m_SerialID = get_CID();
			obj.m_AppendID = get_CID();
			obj.m_UserName = get_CID();
			obj.m_Department = get_CID();
			obj.m_RecTime = get_DATETIME();
			obj.m_VerifyMode = get_BYTE();
			obj.m_Source = get_BYTE();

			if (obj.m_PicBytes > 0) {
				obj.m_Pic = get_BYTES(obj.m_PicBytes);
			}
			off = ALIGN4(off);
			return obj;
		}

		public NETPRO get_NETPRO() {
			NETPRO obj = new NETPRO();
			obj.m_WebPro1 = get_BYTE();
			obj.m_WebPro2 = get_BYTE();
			obj.m_WebUrl1 = get_STRING(127);
			obj.m_WebUrl2 = get_STRING(127);
			obj.m_IPAdr = get_STRING(16);
			obj.m_MIPAdr = get_STRING(16);
			obj.m_NetMask = get_STRING(16);
			obj.m_GateWay = get_STRING(16);
			obj.m_Dns = get_STRING(16);
			return obj;
		}

	}

	public int close() {
		try {
			if (null != netInputStream) {
				netInputStream.close();
			}
			if (null != netOutputStream) {
				netOutputStream.close();
			}
			if (null != sc) {
				sc.close();
			}
		} catch (Exception e) {
			System.out.println("close ����");
			e.printStackTrace();
		}
		return 0;
	}

	public static int _inet_addr(String ip) {
		int num = 0;
		if (ip == null) {
			return num;
		}

		try {
			ip = ip.replaceAll("[^0-9\\.]", ""); // ȥ���ַ���ǰ�Ŀ��ַ�
			String[] ips = ip.split("\\.");
			if (ips.length == 4) {
				num = Integer.parseInt(ips[3], 10) * 256 * 256 * 256
						+ Integer.parseInt(ips[2], 10) * 256 * 256
						+ Integer.parseInt(ips[1], 10) * 256
						+ Integer.parseInt(ips[0], 10);
				// num = num >>> 0;
			}
		} catch (NullPointerException ex) {
			System.out.println(ip);
		}

		return num;
	}

	public static int ALIGN(int bytes, int alignments) {
		return (((bytes) + ((alignments) - 1)) & ~((alignments) - 1));
	}

	// У��ͼ���
	public static int sock_checksum(byte buffer[], int off, int size) {
		int cksum = 0;
		while ((size - 1) > off) {
			int low = buffer[off];
			int high = buffer[off + 1];
			int z = (int) (((high & 0xff) << 8) | (low & 0xff));
			cksum += z;
			// li.msg_DEBUGMSG(this, "cksum = " + cksum + " z = " + z);
			off += 2;
		}

		if (size > off) {
			cksum += buffer[off];
		}

		cksum = (cksum >> 16) + (cksum & 0xffff);
		cksum += (cksum >> 16);
		return (~cksum) & 0xffff;
	}

	int send_cmd_packet(int m_MCmd, int m_SCmd, int m_CFlag, byte[] data,
			int len) {
		byte[] pBuffer = new byte[PACK_SIZE + OFFSET_SIZE];// ���60K��Сһ����
		int nTotalSend = 0;
		// int bOK = 1;
		int i = 0;
		int nSeq = ++m_nIndex;
		int m_Param1 = -1;
		int m_Param2;
		int cmdlen = len + size_NCMD;
		set_stream set = new set_stream(pBuffer, OFFSET_SIZE);

		set.set_INT((int) m_MCmd);
		set.set_INT((int) m_SCmd);
		set.set_INT((int) m_CFlag);
		set.set_INT((int) m_Param1);
		m_Param2 = _inet_addr(get_ip());
		set.set_INT((int) m_Param2);

		for (i = 0; i < cmdlen;) {
			int nSend;
			// ��װ��������� �Ӱ�ͷ
			int msg_identify = nSeq; // ��ʶ
			int msg_offset = i * PACK_SIZE; // ƫ��
			int msg_total = cmdlen; // �����ܳ���
			int msg_checksum = 0; // �����
			int msg_len = Math.min(PACK_SIZE, (cmdlen - i)); // ���ݳ���

			if (len > 0 && data != null) {
				if (0 == i) {
					bytecopy(data, 0, pBuffer, OFFSET_SIZE + size_NCMD, msg_len
							- size_NCMD);
				} else {
					bytecopy(data, i, pBuffer, size_NCMD, msg_len);
				}
			}

			nSend = ALIGN(msg_len, 4) + OFFSET_SIZE;

			set = new set_stream(pBuffer, 0);
			set.set_INT((int) msg_identify);
			set.set_INT((int) msg_offset);
			set.set_INT((int) msg_len);
			set.set_INT((int) msg_total);
			msg_checksum = sock_checksum(pBuffer, 0, OFFSET_SIZE);
			set.set_INT((int) msg_checksum);

			// println("send" + nSend);

			try {
				netOutputStream.write(pBuffer, 0, nSend);
			} catch (Exception e) {
				e.printStackTrace();
				li.msg_CONNECT_CUT(this);
			}

			nTotalSend += msg_len;
			i += msg_len;
		}
		// li.msg_DEBUGMSG(this, "���� �ɹ�:" + nTotalSend + "bytes");
		return nTotalSend;
	}

	byte[] msg_buf;
	int msg_identify = -1; // ��ʶ
	int msg_len; // ���ݳ���

	int safe_read(byte[] bytes, int off, int len) throws Exception {
		int i = 0;
		for (; i < len;) {
			int readed_len = netInputStream.read(bytes, off + i, len - i);
			if (readed_len <= 0) {
				return -1;
			}
			i += readed_len;
		}
		return i;
	}

	String bytes2string(byte[] bytes, int off, int len) {
		String ss = "";
		for (int j = 0; j < len; ++j) {
			ss += " " + bytes[j];
		}
		return ss;
	}

	int recv_pack_data() throws Exception {
		byte[] pbuf = new byte[OFFSET_SIZE + 40];
		int readed_len = 0;
		get_stream get = new get_stream(pbuf, 0);

		try {
			// ��ȡ��ͷ
			readed_len = safe_read(pbuf, 0, OFFSET_SIZE);
			// li.msg_DEBUGMSG(this, "read " + readed_len + "bytes");
		} catch (SocketTimeoutException e) {
			// li.msg_DEBUGMSG(this, "read ��ͷ ��ʱ");
			return 0;
		} catch (Exception e) {
			li.msg_DEBUGMSG(this, "read ��ͷ ����");
			e.printStackTrace();
			// close();
			return -1;
		}

		// li.msg_VIDEO_OUT(this, bytes2string(pbuf, 0, 20));

		if (readed_len != 20) {
			li.msg_DEBUGMSG(this, "read ��ͷ ���� ���Ȳ��� len=" + readed_len);
			return 0;
		}

		int checksum = sock_checksum(pbuf, 0, OFFSET_SIZE);
		// У��
		if (0 != checksum) {
			li.msg_DEBUGMSG(this, "У���������");
			return -3; // У���������
		}

		// ���ұ��α�ʶ�����ݽ��ջ���
		int identify = get.get_INT();
		int offset = get.get_INT();
		int len = get.get_INT();
		int total = get.get_INT();

		if (identify != msg_identify || null == msg_buf) { // û�ҵ�����Ϊ����Ƶ���ݰ���
			if (offset != 0) { // �����Ƿ񶪰�ͷ������Ϊ������ͷ�ķǷ���Ƶ���ݡ�
				li.msg_DEBUGMSG(this, "������ͷ������");
				return -1;
			}

			// Ϊ���ձ�����Ƶ���ݿ���һ���컺�棬�����뵽MAPӳ�����
			msg_buf = new byte[total];
			msg_identify = identify;
			msg_len = 0;
		}

		// ������
		try {
			readed_len = safe_read(msg_buf, offset, len);
			// li.msg_DEBUGMSG(this, "len " + readed_len + " " + len);
			// li.msg_DEBUGMSG(this, "read " + readed_len + "bytes");
		} catch (Exception e) {
			li.msg_DEBUGMSG(this, "safe_read ����" + readed_len + " " + len);
			return -1;
		}

		msg_len += len;

		int ret = 0;
		if (msg_len == msg_buf.length) {
			ret = msg_len;
			msg_identify = -1;
			// li.msg_DEBUGMSG(this, "recv_pack_data " + ret + "bytes");
		}
		return ret;
	}

	public int GET_ERR(int state) {
		int errcode = ((state) & 0xff);
		switch (errcode) {
		case SUC_EXECUT:
			li.msg_DEBUGMSG(this, "����ɹ�");
			break;
		case ERR_EXECUT:
			li.msg_DEBUGMSG(this, "����ʧ��");
			break;
		case ERR_BUSY:
			li.msg_DEBUGMSG(this, "����: �豸æ");
			break;
		case ERR_LIMIT:
			li.msg_DEBUGMSG(this, "����: �Ѵ�����");
			break;
		case ERR_NOFIND:
			li.msg_DEBUGMSG(this, "����: û���ҵ���Ӧ����");
			break;
		case ERR_SAVE_FAIL:
			li.msg_DEBUGMSG(this, "����: ���ݱ���ʧ��");
			break;
		case ERR_SET_FAIL:
			li.msg_DEBUGMSG(this, "����: ����ʧ��");
			break;
		case ERR_VERIFY_FAIL:
			li.msg_DEBUGMSG(this, "����: ��֤ʧ��");
			break;
		case ERR_FROMAT:
			li.msg_DEBUGMSG(this, "����: ��ʽ��ʧ��");
			break;
		case ERR_PARAM:
			li.msg_DEBUGMSG(this, "����: ��������");
			break;
		case ERR_DISABLE:
			li.msg_DEBUGMSG(this, "����: Ҫ��ִ�еĹ���û��ʹ��");
			break;
		case ERR_SUPPORT:
			li.msg_DEBUGMSG(this, "����: ��֧�ֵ�����");
			break;
		case ERR_INPUTDATA:
			li.msg_DEBUGMSG(this, "����: ����˴�����������쳣");
			break;
		}
		return errcode;
	}

	int m_Author;

	int msg_proc(int m_MCmd, int m_SCmd, int m_CFlag, int m_Param1, int m_Param2) {
		int MCmd = m_MCmd;
		int off = size_NCMD;
		// int eResult = GET_ERR(m_CFlag);
		get_stream get = new get_stream(msg_buf, off);
		li.msg_DEBUGMSG(this, "�յ���Ϣ m_MCmd=" + m_MCmd + "����: " + msg_buf.length);
		switch (MCmd) {
		case CMD_M_AUTH:
			m_Author = (m_CFlag & F_AUTH_VERIFY);
			// if ( (pCMD.m_CFlag & F_AUTH_VERIFY) ? DEV_AUTH_OPER :
			// DEV_AUTH_SET_OPER, DEV_OPER_UPLOAD, NULL, pCMD.m_Param1,
			// GET_ERR(pCMD.m_CFlag), pvClientData);
			li.msg_AUTH(this, m_CFlag);
			break;
		case CMD_M_DEVINFO:
			BASEINFO baseinfo = new BASEINFO();
			baseinfo.m_DevType = get.get_INT();
			baseinfo.m_LimitUser = get.get_INT();
			baseinfo.m_Limit1NUser = get.get_INT();
			baseinfo.m_SN = get.get_CID();
			baseinfo.m_Ver = get.get_CID();
			baseinfo.m_Space = get.get_CID();
			li.msg_DEVINFO(this, baseinfo);
			break;
		case CMD_M_STATIS:
			STATIS statis = new STATIS();
			statis.m_TotalUsers = get.get_INT();
			statis.m_NoFeatUser = get.get_INT();
			statis.m_TotalDays = get.get_INT();
			statis.m_TotalRecords = get.get_INT();
			statis.m_TotalAlarm = get.get_INT();
			statis.m_TotalDspLog = get.get_INT();
			statis.m_TotalChangeImage = get.get_INT();
			li.msg_STATIS(this, statis);
			break;
		case CMD_M_NETPRO:
			NETPRO netpro = get.get_NETPRO();
			li.msg_NETPRO(this, netpro);
			break;

		case CMD_M_USER:
			if ((m_CFlag & F_DL_ID) != 0) {
				if (0 == GET_ERR(m_CFlag)) {
					int m_Count = get.get_INT();
					VUSER[] vuser = new VUSER[m_Count];
					for (int i = 0; i < m_Count; ++i) {
						vuser[i] = get.get_VUSER();
					}
					li.msg_VUSER(this, vuser);
				}
			}
			break;
		case CMD_M_USER_EX:
			if (0 == GET_ERR(m_CFlag)) {
				switch (m_SCmd) {
				case CMD_S_GET:
					int m_Count = get.get_INT();
					li.msg_DEBUGMSG(this, "CMD_M_USER_EX m_Count: " + m_Count
							+ " msg_buf.len: " + msg_buf.length);
					if (m_Count > 0) {
						USER[] user = new USER[m_Count];
						for (int i = 0; i < m_Count; ++i) {
							user[i] = get.get_USER();
						}
						li.msg_USER(this, user);
					}
					break;
				default:
					li.msg_DEBUGMSG(this, "�ݲ�֧�ֵ�������  m_SCmd=" + m_SCmd);
					break;
				}
			}
			// dev_insert_users(s->user, pUserBatch->m_pUserArray,
			// pUserBatch->m_nCount, 1);
			// listbox_set_item_count(s->lb_reg, s->user->m_nCount);
			break;

		case CMD_M_VERIFY:
		case CMD_M_ALARM:
			if (0 == GET_ERR(m_CFlag)) {
				RECORD[] record = new RECORD[1];
				record[0] = get.get_RECORD();
				li.msg_RECORD(this, m_MCmd, record);
			}
			break;
		case CMD_M_OPTLOG:
		case CMD_M_CIMAGE:
		case CMD_M_RANGE:
			if (0 == GET_ERR(m_CFlag)) {
				int m_Count = get.get_INT();
				li.msg_DEBUGMSG(this, "" + m_Count);
				RECORDRANGE[] range = new RECORDRANGE[m_Count];
				for (int i = 0; i < m_Count; ++i) {
					range[i] = get.get_RECORDRANGE();
				}
				li.msg_RECORDRANGE(this, m_MCmd, range);
			}
			break;

		case CMD_M_RECORD_EX:
			if (0 == GET_ERR(m_CFlag)) {
				switch (m_SCmd) {
				case CMD_M_VERIFY:
				case CMD_M_ALARM:
					int m_Count = get.get_INT();
					RECORD[] record = new RECORD[m_Count];
					for (int i = 0; i < m_Count; ++i) {
						record[i] = get.get_RECORD();
					}
					li.msg_RECORDEX(this, m_MCmd, record);
					break;
				}
			}
			break;
		default:
			break;
		}
		return 0;
	}

	public void run() {
		msg_len = 0;
		msg_identify = -1;
		while (!sc.isClosed()) {
			try {
				int ret = recv_pack_data();
				if (ret > 0) {
					get_stream get = new get_stream(msg_buf, 0);
					int m_MCmd = get.get_INT();
					int m_SCmd = get.get_INT();
					int m_CFlag = get.get_INT();
					int m_Param1 = get.get_INT();
					int m_Param2 = get.get_INT();
					if (CMD_M_HEART == m_MCmd) {
						if (SUC_EXECUT == GET_ERR(m_CFlag)) {
							// ����������
							send_cmd_packet(CMD_M_HEART, CMD_S_SET, 0, null, 0);
						}
					} else {
						msg_proc(m_MCmd, m_SCmd, m_CFlag, m_Param1, m_Param2);
					}
				} else if (ret < 0) {
					close();
				}
			} catch (Exception e) {
				close();
				li.msg_DEBUGMSG(this, "recv_pack_data ����, �����Ͽ�");
				return;
			}
		}
	}

	// �� �� �豸��֤��������֤
	// �� �� cszDevAddr�豸��ַ
	// cName �û���
	// cPws ����
	// nFlag 1=��֤ 2=����
	// ˵ ��
	public int cmd_SetDevAuth(String sLoginName, String sPassword, int nFlag) {
		byte pAuth[] = new byte[2 * ID_LEN];
		set_stream set = new set_stream(pAuth, 0);
		set.set_CID(sLoginName);
		set.set_CID(sPassword);
		// ����������
		return send_cmd_packet(CMD_M_AUTH, CMD_S_SET, ((nFlag == 2) ? 0
				: F_AUTH_VERIFY), pAuth, pAuth.length);
	}

	// �� �� �����豸���������ַ
	// �� ��
	// cszDevAddr �豸��ַ
	// cszRgnAddr �豸�����ַ
	// ˵ �� ���豸�������Ӻ�ɽ��豸���õ�ĳһ�������Ժ󼰿ɶ����������������豸
	public int cmd_SetDevRegionAddr(String cszGroupIP) {
		byte cid[] = new byte[ID_LEN];
		set_stream set = new set_stream(cid, 0);
		set.set_CID(cszGroupIP);
		return send_cmd_packet(CMD_M_MCAST, CMD_S_SET, 0, cid, cid.length);
	}

	public int cmd_GetDevRgn() {
		return send_cmd_packet(CMD_M_MCAST, CMD_S_GET, 0, null, 0);
	}

	public static int TRUE = 1;
	public static int FALSE = 0;
	public static int size_DATETIME = 16;

	// �� �� �����豸ϵͳʱ��
	// �� ��
	// cszDevAddr �豸��ַ
	// cSysTime ʱ����Ϣ
	// ˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
	// eType = DEV_SYS_TIME_OPER
	// eFlag = DEV_OPER_UPLOAD ��
	// pvContent = NULL
	public int cmd_SetDevTime(DATETIME cTime) {
		byte[] dt = new byte[size_DATETIME];
		set_stream set = new set_stream(dt, 0);
		set.set_DATETIME(cTime);
		return send_cmd_packet(CMD_M_TIME, CMD_S_SET, 0, dt, size_DATETIME);
	}

	// �� �� ��ȡ�豸ϵͳʱ��
	// �� �� cszDevAddr �豸��ַ
	// ˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
	// eType = DEV_SYS_TIME_OPER
	// eFlag = DEV_OPER_DOWNLOAD��
	// pvContent = DEV_DATETIME*
	public int cmd_GetDevTime() {
		return send_cmd_packet(CMD_M_TIME, CMD_S_GET, 0, null, 0);
	}

	// �� �� ��ȡ�豸������Ϣ
	// �� �� cszDevAddr �豸��ַ
	// ˵ ��
	public int cmd_GetDevBaseInfo() {
		return send_cmd_packet(CMD_M_DEVINFO, CMD_S_GET, 0, null, 0);
	}

	// �� �� ��ȡ�豸ͳ����Ϣ
	// �� �� cszDevAddr �豸��ַ
	// ˵ ��
	public int cmd_GetDevStatisInfo() {
		return send_cmd_packet(CMD_M_STATIS, CMD_S_GET, 0, null, 0);
	}

	// �� �� ��ȡ��������������
	// �� ��
	// cszDevAddr ��������ַ
	// nSeq �������к�
	// ˵ �� ��������DEV_WORKATTͨ���ص�����������
	public int cmd_GetDevWorkAttInfo() {
		return send_cmd_packet(CMD_M_DEVSET, CMD_S_GET, 0, null, 0);
	}

	// �� �� �����豸����������Ϣ
	// �� ��
	// cszDevAddr �豸��ַ
	// cWorkAtt �������Խṹ��
	// nSeq �������к�

	public int cmd_SetDevWorkAttInfo(WORKATT workatt) {
		int pSet_len = 4 * (TGROUP_NUM + 3) + 4 + ID_LEN + 20;
		byte[] pSet = new byte[pSet_len];
		set_stream set = new set_stream(pSet, 0);
		set.set_WORKATT(workatt);
		return send_cmd_packet(CMD_M_DEVSET, CMD_S_SET, 0, pSet, pSet_len);
	}

	// public static int USER_JPEG_SIZE = (64 * 1024);
	// public static int REC_JPEG_SIZE = (18 * 1024);
	// public static int FEAT_SIZE = (90 * 1024);

	// �� �� �ϴ�ָ���û�
	// �� ��
	// cszDevAddr �豸��ַ
	// cUser �û���Ϣ�ṹ��
	// nSeq �������к�
	// ˵ ��
	// �û���������ʱ��DEV_USER�е� m_FeatLen = 0�� m_FeatData = NULL
	// �û���ע����ʱ��DEV_USER�е� m_PicLen= 0�� m_PicData = NULL
	// DevOperResultNotify�ص�������Ӧ���ͣ�eType = DEV_USER_OPER
	// eFlag = DEV_OPER_UPLOAD��pvContent= NULL
	public int cmd_SetUser(USER cUser) {
		byte[] pUser = new byte[cUser.size()];
		set_stream set = new set_stream(pUser, 0);
		int len = set.off;
		return send_cmd_packet(CMD_M_USER, CMD_S_SET, 0, pUser, len);
	}

	// �� �� �ϴ�ʵʱ�����û�
	// �� ��
	// cszDevAddr �豸��ַ
	// cUser �û���Ϣ�ṹ��
	// m_CFlag ʵʱ������Ϣ [F_DL_APPENDID|F_DL_PIC|F_DL_FEAT]
	// nSeq �������к�
	// ˵ �� �豸������ͻ����ϴ�ĳ�û�ʱ��ͨ���˽ӿ��ϴ��������û���Ϣ��
	// cRtLoadΪ�豸������ʱ����������Ϣ���ͻ��˽�ֹ�޸ġ��˺���ʵ��Զ���û����ع��ܡ�
	public int cmd_SetRealTimeUser(USER cUser, int m_CFlag) {
		byte[] pUser = new byte[cUser.size()];
		set_stream set = new set_stream(pUser, 0);
		int len = set.off;
		return send_cmd_packet(CMD_M_USER, CMD_S_REALTIME_LOAD, m_CFlag, pUser,
				len);
	}

	// �� �� Զ��ʵʱ�ɼ��û�����
	// �� ��
	// cszDevAddr �豸��ַ
	// cUser �û���Ϣ
	// ˵ �� �����¼���û�����ʱ����ͨ���ú������û����Ϸ��͵��������������������ɼ����û���������
	// Զ���û��ɼ����ܣ�ʹ��ֱ���ڹ���˾Ϳ��Բɼ��û��������Զ����Ƶ������²ɼ��Ŀ��ӻ��̶Ⱥ�ֱ���ڻ����ϲɼ�һ����
	// ���ʹ����(�û��ɼ�ʵʱͬ��)��Զ�̲ɼ����û�Ҳ��ʵʱ�ķ��ع����.
	public int cmd_SetRealTimeUserCap(String cUserID) {
		USER cUser = new USER();
		cUser.m_UserID = cUserID;
		byte[] pUser = new byte[cUser.size()];
		set_stream set = new set_stream(pUser, 0);
		set.set_USER(cUser);
		int len = set.off;
		li.msg_DEBUGMSG(this, "send_cmd_packet len=" + len + " cUserID = " + cUserID + "cUser.size()=" + cUser.size());
		return send_cmd_packet(CMD_M_USER, CMD_S_REALTIME_CAP, 0, pUser, len);
	}

	// �� �� ɾ��ָ���û�
	// �� ��
	// cszDevAddr �豸��ַ
	// cID �û����
	// nSeq �������к�
	// ˵ �� ɾ���ɹ����ͨ�������ص������������
	// DevOperResultNotify�ص�������Ӧ���ͣ�
	// eType = DEV_USER_OPER
	// eFlag = DEV_OPER_DELETE��
	// pvContent = NULL
	public int cmd_DelUser(String cID) {
		byte[] cid = new byte[ID_LEN];
		set_stream set = new set_stream(cid, 0);
		set.set_CID(cID);
		return send_cmd_packet(CMD_M_USER, CMD_S_DEL, 0, cid, ID_LEN);
	}

	// �� �� ����ָ���û�
	// �� ��
	// cszDevAddr �豸��ַ
	// cID �û�ID
	// nFlag F_DL_PIC �������û���Ƭ
	// F_DL_FEAT ��Ҫ������������
	// F_DL_PIC|F_DL_FEAT=����
	// Ϊ0Ĭ������������Ϣ
	// nSeq �������к�
	// ˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
	// eType = DEV_USER_OPER
	// eFlag = DEV_OPER_DOWNLOAD��
	// pvContent = USER*
	public int cmd_GetSingleUser(String cID, int m_CFlag) {
		byte[] cid = new byte[ID_LEN];
		set_stream set = new set_stream(cid, 0);
		set.set_CID(cID);
		return send_cmd_packet(CMD_M_USER, CMD_S_GET, 0, cid, ID_LEN);
	}

	// �� �� ���������û�
	// �� �� cszDevAddr �豸��ַ
	// ˵ ��
	public int cmd_GetAllUser() {
		return send_cmd_packet(CMD_M_USER, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
				null, 0);
	}

	// �� �� ����ĳһʱ����ڵ�ע���û�
	// �� ��
	// cszDevAddr �豸��ַ
	// cStart ��ʼ����
	// cEnd ��ֹ����
	// ˵ ��
	public int cmd_GetSegTimeUser(NETDATE cStart, NETDATE cEnd) {
		byte[] seg = new byte[16];
		set_stream set = new set_stream(seg, 0);
		set.set_NETDATE(cStart);
		set.set_NETDATE(cEnd);
		return send_cmd_packet(CMD_M_USER, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
				seg, 16);
	}

	// �� �� ����ʶ���¼����
	// �� ��
	// cszDevAddr �豸��ַ
	// nSeq ��������
	// ˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
	// eType = DEV_RECORD_OPER
	// eFlag = DEV_OPER_DOWNLOAD
	// pvContent = DEV_DL_RECORD_ARRAY*
	// �ο�DEV_DL_RECORD_ARRAY�ṹ��һ��Ϊһ�����䣬�ú�������ʶ���¼���������䡣
	public int cmd_GetAllIdentifyRecord() {
		return send_cmd_packet(CMD_M_VERIFY, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
				null, 0);
	}

	// �� �� ����ָ��ʱ���ʶ���¼
	// �� �� cszDevAddr �豸��ַ
	// cStart ��ʼ����
	// cEnd ��ֹ����
	// ˵ ��
	public int cmd_GetSegTimeIdentifyRecord(NETDATE cStart, NETDATE cEnd) {
		byte[] seg = new byte[NETDATE.size * 2];
		set_stream set = new set_stream(seg, 0);
		set.set_NETDATE(cStart);
		set.set_NETDATE(cEnd);
		return send_cmd_packet(CMD_M_USER, CMD_S_GET,
				F_DL_ID | F_ID_TIME_RANGE, seg, NETDATE.size * 2);
	}

	// �� �� ��ȡ����ʶ���¼/������¼ָ��ID�ĺ�������
	// �� ��
	// cszDevAddr �豸��ַ
	// cRange ID ����
	// m_CFlag F_DL_PIC=��Ҫ���ؼ�¼��Ƭ
	// bKnowRange true=�������ݣ� false=���伯��
	// nSeq ��������
	// ˵ �� ��bKnowRange Ϊtrue�ú������ؾ�������ݣ�Ϊfalseʱ��ѯָ���������豸�ϵľ����С��
	// DevOperResultNotify�ص�������Ӧ���ͣ�
	// ��������ʱ
	// pvContent = DEV_DL_RECORD_ARRAY*
	// cRange.m_SID = ָ��SN
	// cRange.m_Count = -1;
	// cRange.m_bOpenRange = TRUE;
	// bKnowRange = FALSE��flag = 1����Ч������
	// ���ؾ����¼ʱ
	// DevOperResultNotify�ص�������Ӧ���ͣ�
	// pvContent = DEV_RECORD*
	public int cmd_GetRangeRec(RECORDRANGE cRange, int m_CFlag) {
		byte[] bytes = new byte[RECORDRANGE.size];
		set_stream set = new set_stream(bytes, 0);
		set.set_RECORDRANGE(cRange);
		return send_cmd_packet(CMD_M_RANGE, CMD_S_GET, m_CFlag, bytes,
				RECORDRANGE.size);
	}

	// �� �� ���ر�����¼����
	// �� ��
	// cszDevAddr �豸��ַ
	// nSeq ��������
	// ˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
	// eType = DEV_ALARM_OPER
	// eFlag = DEV_OPER_DOWNLOAD��
	// pvContent = DEV_DL_RECORD_ARRAY*
	// �ο�DEV_DL_RECORD_ARRAY�ṹ��������¼ֻ��һ������
	public int cmd_GetAllAlarmRecord() {
		return send_cmd_packet(CMD_M_ALARM, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
				null, 0);
	}

	// �� �� ����ָ��ʱ��α�����¼
	// �� �� cszDevAddr �豸��ַ
	// cStart ��ʼ����
	// cEnd ��ֹ����
	// ˵ ��
	public int cmd_GetSegTimeAlarmRecord(NETDATE cStart, NETDATE cEnd) {
		byte[] seg = new byte[NETDATE.size * 2];
		set_stream set = new set_stream(seg, 0);
		set.set_NETDATE(cStart);
		set.set_NETDATE(cEnd);
		return send_cmd_packet(CMD_M_ALARM, CMD_S_GET, F_DL_ID
				| F_ID_TIME_RANGE, seg, NETDATE.size * 2);
	}

	// �� �� �������䱨����¼
	// �� �� cszDevAddr �豸��ַ
	// cRange ID����
	// flag DL_DEV_RECORD_PIC=��Ҫ���ؼ�¼��Ƭ 0x00=��������Ƭ, 0x01������Ƭ
	// bKnowRange ָ�������ؾ������ݻ�������ID����(true=�������ݣ� false=ID����)
	// ˵ �� �˽ӿ����ڴ�ĳһ����ȷ�ļ�¼ID��ʼ����nRang������
	public int cmd_GetRangeAlarm(RECORDRANGE cRange, int m_CFlag) {
		byte[] bytes = new byte[RECORDRANGE.size];
		set_stream set = new set_stream(bytes, 0);
		set.set_RECORDRANGE(cRange);
		return send_cmd_packet(CMD_M_RANGE, CMD_S_GET, m_CFlag, bytes,
				RECORDRANGE.size);
	}

	// �� �� ����������־
	// �� �� cszDevAddr �豸��ַ
	// ˵ ��
	public int cmd_GetLog() {
		return send_cmd_packet(CMD_M_OPTLOG, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
				null, 0);
	}

	// �� �� ����������־
	// �� �� cszDevAddr �豸��ַ
	// cRange ID����
	// bKnowRange ָ�������ؾ������ݻ�������ID����(true=�������ݣ� false=ID����)
	// ˵ �� �˽ӿ����ڴ�ĳһ����ȷ�ļ�¼ID��ʼ����nRang������
	public int cmd_GetRangeLog(RECORDRANGE cRange, int m_CFlag) {
		byte[] bytes = new byte[RECORDRANGE.size];
		set_stream set = new set_stream(bytes, 0);
		set.set_RECORDRANGE(cRange);
		return send_cmd_packet(CMD_M_RANGE, CMD_S_GET, m_CFlag, bytes,
				RECORDRANGE.size);
	}

	// �� �� ��ȡ���ע���ռ�¼����
	// �� ��
	// cszDevAddr �豸��ַ
	// nSeq ��������
	// ˵ ��
	// DevOperResultNotify�ص�������Ӧ���ͣ�
	// eType = DEV_CHANGEIMAGE_OPER
	// eFlag = DEV_OPER_DOWNLOAD��
	// pvContent = DEV_DL_RECORD_ARRAY*
	// �ο�DEV_DL_RECORD_ARRAY�ṹ�����ע����ֻ��һ������
	public int cmd_GetChangeImage() {
		return send_cmd_packet(CMD_M_CIMAGE, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
				null, 0);
	}

	// �� �� ����ָ������ı��ע���ռ�¼���߲�ѯ�������ݵľ����С
	// �� ��
	// cszDevAddr �豸��ַ
	// cRange ����, �ο����䶨��
	// flag DL_DEV_RECORD_PIC=��Ҫ���ؼ�¼��Ƭ
	// bKnowRange true=�������ݣ� false=���伯��
	// nSeq �������к�
	// ˵ �� ��bKnowRange Ϊtrue�ú������ؾ�������ݣ�Ϊfalseʱ��ѯָ���������豸�ϵľ����С��
	// ��������ʱ
	// DevOperResultNotify�ص�������Ӧ���ͣ�
	// pvContent = DEV_DL_RECORD_ARRAY*
	// cRange.m_SID = ָ��SN
	// cRange.m_Count = -1;
	// cRange.m_bOpenRange = TRUE;
	// bKnowRange = FALSE��flag = 1����Ч������
	// ���ؾ����¼ʱ
	// DevOperResultNotify�ص�������Ӧ���ͣ�
	// pvContent=DEV_RECORD*
	public int cmd_GetRangeCImage(RECORDRANGE cRange, int m_CFlag) {
		byte[] bytes = new byte[RECORDRANGE.size];
		set_stream set = new set_stream(bytes, 0);
		set.set_RECORDRANGE(cRange);
		return send_cmd_packet(CMD_M_RANGE, CMD_S_GET, m_CFlag, bytes,
				RECORDRANGE.size);
	}

	public int cmd_TimeGroup_impl(DSPTIMEG[] cArray, int m_SCmd, int m_CFlag) {
		byte[] nData = new byte[cArray.length * DSPTIMEG.size + 4];
		set_stream set = new set_stream(nData, 0);
		set.set_INT(cArray.length);

		for (int i = 0; i < cArray.length; ++i) {
			set.set_DSPTIMEG(cArray[i]);
		}

		return send_cmd_packet(CMD_M_TGROUP, m_SCmd, m_CFlag, nData,
				nData.length);
	}

	// �� �� ����ʱ����
	// �� �� cszDevAddr �豸��ַ
	// cTGArray ʱ����
	// ˵ ��
	public int cmd_SetTimeGroup(DSPTIMEG[] cTGArray) {
		return cmd_TimeGroup_impl(cTGArray, CMD_S_SET, F_DL_NORMAL_TG
				| F_DL_HOLIDAY_TG);
	}

	// �� �� ��ȡʱ����
	// �� �� dev_addr �豸��ַ
	// cTGArray ʱ����
	// m_CFlag ʱ��������
	// ˵ �� cTGArray.m_nCount=0ʱ��ʾ����eType���͵�����ʱ����
	public int cmd_GetTimeGroup(DSPTIMEG[] cTGArray, int m_CFlag) {
		return cmd_TimeGroup_impl(cTGArray, CMD_S_GET, m_CFlag);
	}

	// �� �� ɾ��ʱ����
	// �� �� cszDevAddr �豸��ַ
	// cTGArray ʱ����
	// eType ʱ��������
	// ˵ �� eType.m_nCount=0ʱ��ʾɾ��tg_type��������ʱ����
	public int cmd_DelTimeGroup(DSPTIMEG[] cTGArray, int m_CFlag) {
		return cmd_TimeGroup_impl(cTGArray, CMD_S_DEL, m_CFlag);
	}

	public int cmd_Right_impl(DSPRIGHT[] cArray, int m_SCmd) {
		byte[] nData = new byte[cArray.length * DSPRIGHT.size + 4];
		set_stream set = new set_stream(nData, 0);
		set.set_INT(cArray.length);

		for (int i = 0; i < cArray.length; ++i) {
			set.set_DSPRIGHT(cArray[i]);
		}

		return send_cmd_packet(CMD_M_DSPRIGHT, m_SCmd, 0, nData, nData.length);
	}

	// �� �� ����Ȩ��
	// �� �� cszDevAddr �豸��ַ
	// cRTArray Ȩ��
	// ˵ ��
	public int cmd_SetRight(DSPRIGHT[] cRTArray) {
		return cmd_Right_impl(cRTArray, CMD_S_SET);
	}

	// �� �� ���ͻ�ȡȨ��
	// �� �� cszDevAddr �豸��ַ
	// cRTArray Ȩ��
	// ˵ �� ��rt_array.m_nCount=0ʱ��ʾ��������Ȩ��
	public int cmd_GetRight(DSPRIGHT[] cRTArray) {
		return cmd_Right_impl(cRTArray, CMD_S_GET);
	}

	// �� �� ɾ��Ȩ��
	// �� �� cszDevAddr �豸��ַ
	// cRTArray Ȩ��
	// ˵ �� ��rt_array.m_nCount=0ʱ��ʾɾ������Ȩ��
	public int cmd_DelRight(DSPRIGHT[] cRTArray) {
		return cmd_Right_impl(cRTArray, CMD_S_DEL);
	}

	public int cmd_UserGroup_impl(USERGROUP[] cArray, int m_SCmd) {
		byte[] nData = new byte[cArray.length * DSPRIGHT.size + 4];
		set_stream set = new set_stream(nData, 0);
		set.set_INT(cArray.length);

		for (int i = 0; i < cArray.length; ++i) {
			set.set_USERGROUP(cArray[i]);
		}

		return send_cmd_packet(CMD_M_UGROUP, m_SCmd, 0, nData, nData.length);
	}

	// �� �� �����û���
	// �� �� cszDevAddr �豸��ַ
	// cUGrray �û���
	// ˵ ��
	public int cmd_SetUserGroup(USERGROUP[] cUGrray) {
		return cmd_UserGroup_impl(cUGrray, CMD_S_SET);
	}

	// �� �� ���ͻ�ȡ�û���
	// �� �� cszDevAddr �豸��ַ
	// ug_array �û���
	// ˵ �� ��rt_array.m_nCount=0ʱ��ʾ���������û���
	public int cmd_GetUserGroup(USERGROUP[] cUGrray) {
		return cmd_UserGroup_impl(cUGrray, CMD_S_GET);
	}

	// �� �� ɾ���û���
	// �� �� cszDevAddr �豸��ַ
	// ug_array �û���
	// ˵ �� ��rt_array.m_nCount=0ʱ��ʾɾ�������û���
	public int cmd_DelUserGroup(USERGROUP[] cUGrray) {
		return cmd_UserGroup_impl(cUGrray, CMD_S_DEL);
	}

	// �� �� ���ÿ�����״̬
	// �� ��
	// cszDevAddr �豸��ַ
	// bEOpen true=�������ţ� false=��������
	// ˵ ��
	public int cmd_SetOpenDoorState(int eState) {
		return send_cmd_packet(CMD_M_EOPEN, CMD_S_SET, eState, null, 0);
	}

	// �� �� ��ȡ������״̬
	// �� �� cszDevAddr �豸��ַ
	// bEOpen true=�������ţ� false=��������
	// ˵ ��
	public int cmd_GetOpenDoorState() {
		return send_cmd_packet(CMD_M_EOPEN, CMD_S_GET, 0, null, 0);
	}

	// �� �� Զ�̿���
	// �� �� cszDevAddr �豸��ַ
	// bEOpen true=���ţ� false=����
	// pDoorArray �ŵ�����
	// size �����С
	// ˵ ��
	public int cmd_SetRemoteOpenDoor(int bOpen) {
		int nFlag = 0;
		if (0 != bOpen) {
			nFlag = F_DOOR_OPEN;
		} else {
			nFlag = F_DOOR_CLOSE;
		}
		return send_cmd_packet(CMD_M_REMOTE_OPEN, CMD_S_SET, nFlag, null, 0);
	}

	// �� �� IO�豸����
	// �� ��
	// cszDevAddr �豸��ַ
	// DEV_IOCTRL ������Ϣ
	// nSeq ����ִ�����к�
	// ˵ �� DEV_IOCTRL:m_Sourceָ��Ҫ�������Ķ˿ڣ�DEV_IOCTRL:m_ActionҪִ�еĶ���
	// ����1��m_Source = 0x1f m_Action = DEV_ACT_IO_OPEN
	// ����1��m_Source = 0x15 m_Action = DEV_ACT_IO_OPEN
	// 0x1f��015������ô�����ģ���ο�485��ַ����
	public int cmd_IOCtrl(IOCTRL ioctrl) {
		byte[] nData = new byte[IOCTRL.size];
		set_stream set = new set_stream(nData, 0);
		set.set_IOCTRL(ioctrl);
		return send_cmd_packet(CMD_M_IOCTRL, CMD_S_SET, 0, nData, IOCTRL.size);
	}

	// �� �� ��ʽ���豸
	// �� �� cszDevAddr �豸��ַ
	// ˵ ��
	public int cmd_SetFormat() {
		return send_cmd_packet(CMD_M_FORMAT, CMD_S_SET, 0, null, 0);
	}

	// �� �� �豸�������
	// �� �� cszDevAddr �豸��ַ
	// cAppData ������½ṹ��
	// ˵ ��
	public int cmd_SetUpdate(APPUPDATE cAppData) {
		byte[] nData = new byte[cAppData.size()];
		set_stream set = new set_stream(nData, 0);
		set.set_APPUPDATE(cAppData);
		return send_cmd_packet(CMD_M_APPUPDATE, CMD_S_SET, 0, nData,
				nData.length);
	}

	// �� �� �����ϴ��û�
	// �� ��
	// cszDevAddr �豸��ַ
	// cUsers �����û�����
	// nFlag F_DL_PIC //��Ƭ
	// DL_DEV_USER_FEAT //��������
	// DL_DEV_USER_TEXT //�û�������Ϣ
	// ˵ ��: �����ϴ��û���������û�������Ϣ���������
	// nFlag = DL_DEV_USER_TEXT �ϴ�������Ϣ
	// nFlag = DL_DEV_USER_TEXT|F_DL_PIC ����+ע����
	// nFlag = DL_DEV_USER_TEXT|DL_DEV_USER_FEAT ����+������
	// nFlag = DL_DEV_USER_TEXT|F_DL_PIC|DL_DEV_USER_FEAT����+ע����+������
	// DevOperResultNotify�ص�������Ӧ���ͣ�
	// eType = DEV_USER_EX_OPER
	// eFlag = DEV_OPER_UPLOAD ��
	// pvContent = DEV_BATCH_USER*��
	// ��OPER_SUCCESS
	// ����£������ϴ����û������뷵�ص��û�����һ�¡������豸����ʧ�ܡ������ϴ��û����趨��5������(������������������ٶ��й�)��
	public int cmd_SetUserEx(USER[] cUsers, int nFlag) {
		int sz = 0;
		for (int i = 0; i < cUsers.length; ++i) {
			sz += cUsers[i].size();
		}
		byte[] nData = new byte[sz + 4];
		set_stream set = new set_stream(nData, 0);
		set.set_INT(cUsers.length);

		for (int i = 0; i < cUsers.length; ++i) {
			set.set_USER(cUsers[i]);
		}

		return send_cmd_packet(CMD_M_USER_EX, CMD_S_SET, USER_TEXT, nData,
				nData.length);
	}

	public int cmd_UserEx_impl(String[] pUserIDArray, int m_SCmd, int m_CFlag,
			int max_n) {
		int ret = 0;

		for (int j = 0; j < pUserIDArray.length;) {
			int m_Count = Math.min(max_n, pUserIDArray.length - j);
			byte[] nData = new byte[ID_LEN * m_Count + 8];
			set_stream set = new set_stream(nData, 0);
			set.set_INT(m_Count);
			for (int i = 0; i < m_Count; ++i) {
				set.set_CID(pUserIDArray[j + i]);
			}

			ret = send_cmd_packet(CMD_M_USER_EX, m_SCmd, m_CFlag, nData,
					nData.length);
			j += m_Count;
		}

		return ret;
	}

	// �� �� ���������û�
	// �� ��
	// cszDevAddr �豸��ַ
	// pUserIDArray �û�ID�����׵�ַ
	// nIDCount ����Ԫ�ظ���
	// nFlag ͬcmd_SetUserEx
	// ˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
	// eType = USER_TEXT DL_FEAT DL_IMAGE
	// eFlag = DEV_OPER_DOWNLOAD ��
	// pvContent= DEV_BATCH_USER*��
	// OPER_SUCCESS ����£�����������ص��û�����Ҫ�����ص��û�����һ�£�����ͨ��
	// cmd_GetAllUser() ��ȷ���û��Ƿ���Ĵ������豸�ϡ� ���������û����趨��5������(������������������ٶ��й�)��
	public int cmd_GetUserEx(String[] pUserIDArray, int m_CFlag) {
		return cmd_UserEx_impl(pUserIDArray, CMD_S_GET, m_CFlag, 5);
	}

	// �� �� ����ɾ���û�
	// �� ��
	// cszDevAddr �豸��ַ
	// pUserIDArray ��ɾ���û��ı������
	// nIDCount ����Ԫ�ظ���
	// ˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
	// eType = DEV_USER_EX_OPER
	// eFlag = DEV_OPER_DELETE��
	// pvContent = DEV_BATCH_USER*��
	// ��OPER_SUCCESS ����£�����ɾ�����û������뷵�ص��û�������һ�£�����ͨ��cmd_GetAllUser()
	// ��ȷ���û��Ƿ���Ĵ������豸�ϡ���������ɾ���û����趨��10������Ϊ�á�
	public int cmd_DelUserEx(String[] pUserIDArray) {
		return cmd_UserEx_impl(pUserIDArray, CMD_S_DEL, 0, 1000);
	}

	// �� �� �������ؼ�¼
	// �� �� cszDevAddr �豸��ַ
	// cRange ID����
	// ˵ �� ������֤��¼��������¼��������־�����ע����
	public int cmd_GetRecEx(RECORDRANGE cRange) {
		byte[] nData = new byte[RECORDRANGE.size];
		set_stream set = new set_stream(nData, 0);
		set.set_RECORDRANGE(cRange);
		return send_cmd_packet(CMD_M_RECORD_EX, CMD_S_GET, F_DL_PIC, nData,
				nData.length);
	}

	// �� �� �����豸
	// �� �� cszDevAddr �豸��ַ
	// ˵ ��
	public int cmd_RebootDev() {
		return send_cmd_packet(CMD_M_REBOOT, CMD_S_SET, 0, null, 0);
	}

	// �� �� ���/���� �豸������Ϣ
	// �� �� cszDevAddr �豸��ַ
	// pNetPro NULL��ʾ��ã���Ϊ�ձ�ʾ����
	// ˵ ��
	public int cmd_SetNetPro(NETPRO pNetPro) {
		byte[] nData = new byte[NETPRO.size];
		set_stream set = new set_stream(nData, 0);
		set.set_NETPRO(pNetPro);
		return send_cmd_packet(CMD_M_NETPRO, CMD_S_SET, 0, nData, nData.length);
	}

	public int cmd_GetNetPro() {
		return send_cmd_packet(CMD_M_NETPRO, CMD_S_GET, 0, null, 0);
	}

	String devip;

	public String get_ip() {
		return devip;
	}

	public interface listener {
		public int msg_DEBUGMSG(pmdev dev, String s);

		public int msg_CONNECT_SUCCESS(pmdev dev);

		public int msg_CONNECT_CUT(pmdev dev);

		public int msg_AUTH(pmdev dev, int m_CFlag);

		public int msg_DEVINFO(pmdev dev, pmdev.BASEINFO baseinfo);

		public int msg_STATIS(pmdev dev, pmdev.STATIS statis);

		public int msg_NETPRO(pmdev dev, pmdev.NETPRO netpro);

		public int msg_RECORDRANGE(pmdev dev, int type,
				pmdev.RECORDRANGE[] range);

		public int msg_RECORD(pmdev dev, int type, pmdev.RECORD[] range);
		
		public int msg_RECORDEX(pmdev dev, int type, pmdev.RECORD[] range);

		public int msg_VUSER(pmdev dev, pmdev.VUSER[] vuser);

		public int msg_USER(pmdev dev, pmdev.USER[] vuser);

		public int msg_VIDEO_OUT(pmdev dev, int[] data, int rgb_offset, int w,
				int h, int step);

	}

	listener li;

	public int connect(String ip, String user, String pass, listener listener) {
		try {
			devip = ip;
			li = listener;
			InetSocketAddress address = null;
			address = new InetSocketAddress(ip, TCPPORT);
			sc = new Socket();
			sc.connect(address, 1000);
			sc.setSoTimeout(1000);
			netInputStream = new DataInputStream(sc.getInputStream());
			netOutputStream = new DataOutputStream(sc.getOutputStream());
			li.msg_CONNECT_SUCCESS(this);
			cmd_SetDevAuth(user, pass, 1);
			msg_identify = -1;
			start();
			// 1.send file length
		} catch (SocketTimeoutException e) {
			li.msg_DEBUGMSG(this, "connetc ��ʱ :" + ip + " �쳣");
		} catch (UnknownHostException e) {
			li.msg_DEBUGMSG(this, "-.δ֪��������:" + ip + " �쳣");
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
			li.msg_DEBUGMSG(this, "-hostName=" + ip + " portNum=" + TCPPORT
					+ "---.IO�쳣����" + e.getMessage());
		}
		return 0;
	}

	public class video_th extends Thread {
		DatagramSocket ds = null;
		int m_nIndex;
		int msg_len;
		int msg_identify;
		int port;
		public volatile boolean exit = false;
		pmdev dev;

		video_th() {
		}

		int startUDP() {
			int port = 0;
			exit = true;
			try {
				ds = new DatagramSocket(0);
				ds.close();
				ds = new DatagramSocket(0);
				port = ds.getLocalPort();
				ds.setSoTimeout(1000);
			} catch (Exception e) {
				e.printStackTrace();
				throw new RuntimeException(e);
			} finally {
				start();
			}
			return port;
		}

		int clip(int x) {
			if (x < 0)
				return 0;
			if (x > 255)
				return 255;
			return x;
		}

		public static final int IMAGE_UYVY = 0x01;
		public static final int IMAGE_YUYV = 0x02;
		public static final int IMAGE_RGB24 = 0x03;

		public int[] YUYV_RGB_int(byte[] src, int off, int step, int width,
				int height) {
			int r, g, b;
			int R, G, B;
			int Y00, Y01, U, V;
			int numOfPixel = width * height;
			int[] rgb = new int[numOfPixel];
			for (int i = 0; i < height; i++) {
				int startY = off + i * step;
				int index = i * width;
				for (int j = 0; j < width; j += 2) {
					Y00 = src[startY + 0];
					U = src[startY + 1];
					Y01 = src[startY + 2];
					V = src[startY + 3];

					startY += 4;
					r = (int) (1.4075 * ((V & 0xff) - 128));
					g = (int) (0.3455 * ((U & 0xff) - 128) + 0.7169 * ((V & 0xff) - 128));
					b = (int) (1.779 * ((U & 0xff) - 128));
					R = ((Y00 & 0xff) + r);
					G = ((Y00 & 0xff) - g);
					B = ((Y00 & 0xff) + b);
					R = clip(R);
					G = clip(G);
					B = clip(B);
					rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
					index += 1;

					R = (int) ((Y01 & 0xff) + r);
					G = (int) ((Y01 & 0xff) - g);
					B = (int) ((Y01 & 0xff) + b);
					R = clip(R);
					G = clip(G);
					B = clip(B);
					rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
					index += 1;

				}
			}
			return rgb;
		}

		public int[] UYVY_RGB_int(byte[] src, int off, int step, int width,
				int height) {
			int r, g, b;
			int R, G, B;
			int Y00, Y01, U, V;
			int numOfPixel = width * height;
			int[] rgb = new int[numOfPixel];
			for (int i = 0; i < height; i++) {
				int startY = off + i * step;
				int index = i * width;
				for (int j = 0; j < width; j += 2) {
					U = src[startY + 0];
					Y00 = src[startY + 1];
					V = src[startY + 2];
					Y01 = src[startY + 3];

					startY += 4;
					r = (int) (1.4075 * ((V & 0xff) - 128));
					g = (int) (0.3455 * ((U & 0xff) - 128) + 0.7169 * ((V & 0xff) - 128));
					b = (int) (1.779 * ((U & 0xff) - 128));
					R = ((Y00 & 0xff) + r);
					G = ((Y00 & 0xff) - g);
					B = ((Y00 & 0xff) + b);
					R = clip(R);
					G = clip(G);
					B = clip(B);
					rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
					index += 1;

					R = (int) ((Y01 & 0xff) + r);
					G = (int) ((Y01 & 0xff) - g);
					B = (int) ((Y01 & 0xff) + b);
					R = clip(R);
					G = clip(G);
					B = clip(B);
					rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
					index += 1;

				}
			}
			return rgb;
		}

		public int[] RGB24_RGB_int(byte[] src, int off, int step, int width,
				int height) {
			int R, G, B;
			int numOfPixel = width * height;
			int[] rgb = new int[numOfPixel];
			for (int i = 0; i < height; i++) {
				int startY = off + i * step;
				int index = i * width;
				for (int j = 0; j < width; j += 1) {
					R = src[startY + 0];
					G = src[startY + 1];
					B = src[startY + 2];
					startY += 3;
					rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
					index += 1;
				}
			}
			return rgb;
		}

		public int close() {
			try {
				exit = true;
				// sleep(1000); // ���߳��ӳ�1��
				// interrupt();
				join();
			} catch (Exception e) {
				e.printStackTrace();
			}
			return 0;
		};

		public void run() {
			msg_len = 0;
			msg_identify = -1;
			exit = false;
			dev.li.msg_DEBUGMSG(dev, "video Thread run");
			int PACK_SIZEEX = PACK_SIZE + 20;
			byte[] buf = new byte[PACK_SIZEEX];
			byte[] data = null;
			int pre_identify = -1, len = 0;
			while (!ds.isClosed() && !exit) {
				try {
					DatagramPacket dp = null;
					dp = new DatagramPacket(buf, PACK_SIZEEX); // ���ڽ��ճ���Ϊ1024�����ݰ����������ݰ������ݲ��ִ���buf��
					ds.receive(dp); // �Ӵ��׽��ֽ������ݱ���
					// li.msg_DEBUGMSG(this, "ip" + dp.getAddress().toString() +
					// " len=" + dp.getLength());
					int recelen = dp.getLength();

					if (recelen < 0) {
						continue;
					}
					if (sock_checksum(buf, 0, 20) != 0) {
						dev.li.msg_DEBUGMSG(dev, "У�����");
						continue;
					}
					get_stream get = new get_stream(buf, 0);
					int msg_identify = get.get_INT(); // ��ʶ
					int msg_offset = get.get_INT(); // ƫ��
					int msg_len = get.get_INT();// ���Ѿ����͹������������ݳ��ȣ���������ͷ�ĳ��ȣ������������ͷ����
					int msg_total = get.get_INT();// �ܵ��������ݳ��ȣ���������ͷ���Ⱥ�����Ĳ�������
					// int msg_checksum = ;
					get.get_INT();// �����
					// dev.li.msg_DEBUGMSG(dev, "msg_identify=" + msg_identify +
					// " msg_offset=" + msg_offset + " msg_len=" + msg_len +
					// " msg_total=" + msg_total);
					if (pre_identify != msg_identify && 0 == msg_offset) {
						if (null == data || data.length < msg_total) {
							data = new byte[msg_total];
						}
						len = 0;
					}
					bytecopy(buf, 20, data, msg_offset, msg_len);
					len += msg_len;
					if (len >= msg_total) {
						get = new get_stream(data, 20);
						int m_Size = get.get_INT(); // ͼ�������ܴ�СBYTES
						int m_W = get.get_INT(); // ͼ���
						int m_H = get.get_INT(); // ͼ���
						int m_Step = get.get_INT(); // �����ݲ���
						int m_Type = get.get_BYTE(); // �������� IMAGE_UYVY
						int off = 4 * 4 + 1;
						//dev.li.msg_DEBUGMSG(dev, "m_Size=" + m_Size + " m_W="
						//		+ m_W + " m_H=" + m_H + " m_Step=" + m_Step);

						int[] rgb = null;
						if (IMAGE_YUYV==m_Type) {
						rgb = YUYV_RGB_int(data, 20 + off, m_Step, m_W,
								m_H);
						} else if (IMAGE_UYVY==m_Type) {
						rgb = UYVY_RGB_int(data, 20 + off, m_Step, m_W,
								m_H);
						} else if (IMAGE_RGB24==m_Type) {
							rgb = RGB24_RGB_int(data, 20 + off, m_Step, m_W,
									m_H);
						} else {
							rgb = null;
						}
						// BufferedImage image = new BufferedImage(WIDTH,
						// HEIGHT, BufferedImage.TYPE_3BYTE_BGR);
						// Image aa;
						// Bitmap bitmap = Bitmap.createBitmap(data, off,
						// m_Step, m_W, m_H,
						// Bitmap.Config.ARGB_8888);
						dev.li.msg_VIDEO_OUT(dev, rgb, 0, m_W, m_H, m_W);
					}
				} catch (SocketTimeoutException e) {
					ds.close();
					// e.printStackTrace();
					dev.li.msg_DEBUGMSG(dev, "udp ��ʱ :" + devip + " �쳣");
				} catch (Exception e) {
					ds.close();
					// e.printStackTrace();
					dev.li.msg_DEBUGMSG(dev,
							"udp receive DatagramPacket ����, �����Ͽ�");
					return;
				}
			}
			ds.close();
			ds = null;
			dev.li.msg_DEBUGMSG(dev, "�߳��˳�!");
		}
	};

	int ntohs(int x) {
		return (short) (((x & 0xff) << 8) | ((x >> 8) & 0xff));
	}

	video_th vth;
	int m_port = 0;

	public int cmd_StartVideo(String myIP) {
		int ret = 0;
		if (null == vth) {
			vth = new video_th();
			vth.dev = this;
			m_port = vth.startUDP();
			byte[] nData = new byte[18];
			set_stream set = new set_stream(nData, 0);
			// int port1 = ntohs(port);
			set.set_SHORT((short) m_port);
			set.set_STRING(myIP, 16);
			li.msg_DEBUGMSG(this, "cmd_StartVideo ip = " + myIP + " port="
					+ m_port);
			ret = send_cmd_packet(CMD_M_VIDEO_OUT, CMD_S_SET, 0, nData,
					nData.length);
		} else {
			li.msg_DEBUGMSG(this, "��Ƶ�Ѿ��򿪣����ȹر�!");
		}
		return ret;
	}

	// ֹͣ��Ƶ
	public int cmd_StopVideo() {
		int retVal = 0;
		byte[] nData = new byte[18];
		set_stream set = new set_stream(nData, 0);
		// �ر���Ƶ

		// ����������
		set.set_SHORT((short) m_port);
		set.set_STRING(devip, 16);
		retVal = send_cmd_packet(M_VIDEO_OUT, S_GET, 0, nData, nData.length);
		if (vth != null) {
			vth.close();
			vth = null;
		} else {
			li.msg_DEBUGMSG(this, "��Ƶ�ѹر�!�����ٹر�!");
		}
		return retVal;
	}

	public static int test1() {
		pmdev client = new pmdev();
		//155
		client.connect("192.168.1.46", "admin", "201031",
				new pmdev.listener() {
					public int println(String s) {
						System.out.println(s);
						return 0;
					}

					public int msg_DEBUGMSG(pmdev dev, String s) {
						println("DEBUG: " + s);
						return 0;
					}

					public int msg_CONNECT_SUCCESS(pmdev dev) {
						println("������:" + dev.get_ip() + "�ɹ�");
						return 0;
					}

					public int msg_CONNECT_CUT(pmdev dev) {
						println("������:" + dev.get_ip() + "�Ͽ�����");
						return 0;
					}

					public int msg_AUTH(pmdev dev, int m_CFlag) {
						println("msg_AUTH");
						if ((m_CFlag & F_AUTH_VERIFY) != 0) {
							println("�û������֤ �ɹ�!");
						} else {
							println("�豸��֤�û����ò��� �ɹ�!");
						}
						// dev.cmd_GetAllUser();
						// dev.cmd_GetNetPro(); // ��ȡ����������Ϣ
						// dev.cmd_GetAllIdentifyRecord();
						dev.cmd_StopVideo();
						dev.cmd_StartVideo("192.168.1.3");
						try {
							sleep(1000);
						} catch (InterruptedException e) {
							e.printStackTrace();
						} // �ӳ�1��
						//dev.cmd_StopVideo();
						//dev.cmd_StartVideo("192.168.1.3");
						dev.cmd_GetDevBaseInfo();
						//println("cmd_SetRealTimeUserCap!");
						//dev.cmd_SetRealTimeUserCap("0000");
						// dev.cmd_GetDevStatisInfo();
						return 0;
					}

					public int msg_NETPRO(pmdev dev, pmdev.NETPRO netpro) {
						println("msg_NETPRO");
						println("�豸%s:��ȡ������Ϣ�ɹ�");
						println("<< �豸������Ϣ ");
						println("   m_WebPro1: " + netpro.m_WebPro1);
						println("   m_WebPro2: " + netpro.m_WebPro2);
						println("   m_WebUrl1: " + netpro.m_WebUrl1);
						println("   m_WebUrl2: " + netpro.m_WebUrl2);
						println("   m_IPAdr: " + netpro.m_IPAdr);
						println("   m_MIPAdr: " + netpro.m_MIPAdr);
						println("   m_NetMask: " + netpro.m_NetMask);
						println("   m_GateWay: " + netpro.m_GateWay);
						println("   m_Dns: " + netpro.m_Dns);
						println(">>");
						return 0;
					}

					public int msg_DEVINFO(pmdev dev, pmdev.BASEINFO baseinfo) {
						println("msg_DEVINFO");
						println("�豸%s:��ȡ������Ϣ�ɹ�");
						println("<< �豸������Ϣ ");
						println("   �豸������Ϣ " + baseinfo.tostring());
						println(">>");
						return 0;
					}

					public int msg_STATIS(pmdev dev, pmdev.STATIS statis) {
						println("msg_STATIS");
						println("�豸%s:��ȡͳ����Ϣ�ɹ�: " + dev.get_ip());
						println("   �豸ͳ����Ϣ " + statis.tostring());
						return 0;
					}

					public int msg_RECORDRANGE(pmdev dev, int type,
							pmdev.RECORDRANGE[] range) {
						println("msg_RECORDRANGE");
						println("��¼����: " + type);
						println("��¼������Ŀ: " + range.length);
						for (int i = 0; i < range.length; ++i) {
							println("m_Count: " + range[i].m_Count
									+ " m_Flag: " + range[i].m_Flag
									+ " m_SID: " + range[i].m_SID);
						}
						if (range.length > 0) {
							dev.cmd_GetRecEx(range[0]);
						}
						return 0;
					}

					public int msg_RECORD(pmdev dev, int type,
							pmdev.RECORD[] range) {
						println("msg_RECORD");
						for (int i = 0; i < range.length; ++i) {
							println(i + " ID: " + range[i].m_UserID + "ʱ��"
									+ range[i].m_RecTime.tostring());
						}
						return 0;
					}

					public int msg_RECORDEX(pmdev dev, int type,
							pmdev.RECORD[] range) {
						println("msg_RECORDEX");
						for (int i = 0; i < range.length; ++i) {
							println(i + " ID: " + range[i].m_UserID + "ʱ��"
									+ range[i].m_RecTime.tostring());
						}
						return 0;
					}

					public int msg_VUSER(pmdev dev, pmdev.VUSER[] vuser) {
						println("msg_VUSER");
						println("��ȡ�û��б�ɹ�!");
						for (int i = 0; i < vuser.length; ++i) {
							// println("�û�: " + vuser[i].m_UserID);
						}
						if (vuser.length > 2) {
							// ���������˵�����
							String[] strlist = new String[2];
							strlist[0] = vuser[0].m_UserID;
							strlist[1] = vuser[1].m_UserID;
							dev.cmd_GetUserEx(strlist, USER_TEXT | DL_FEAT
									| DL_IMAGE);
						}
						println("�û�����: " + vuser.length);
						return 0;
					}

					public int msg_USER(pmdev dev, pmdev.USER[] user) {
						println("msg_USER");
						println("�û�����: " + user.length);
						for (int i = 0; i < user.length; ++i) {
							println("�û�: " + user[i].m_UserID + "�������� "
									+ user[i].m_Feat.length);
						}
						return 0;
					}
					
					public int msg_VIDEO_OUT(pmdev dev, int[] data,
							int yuvdataoff, int w, int h, int step) {
						println("��Ƶ: w:" + w + " h:" + h + " datalen:" + data.length);

						++aa;
						if (aa<10) {
							println("asdfadsf");
							FileOutputStream fos = null;
							try {
								fos = new FileOutputStream("C:\\aaa.dat");
							} catch (FileNotFoundException e) {
								e.printStackTrace();
							}
							// ��FileOutputStream ��write����д���ֽ�����
							try {
								for (int i=0; i<data.length; ++i) {
									fos.write(intToByteArray(data[i]));
								}
							} catch (IOException e) {
								e.printStackTrace();
							}
							System.out.println("д��ɹ�");
							// Ϊ�˽�ʡIO���Ŀ�������Ҫ�ر�
							try {
								fos.close();
							} catch (IOException e) {
								e.printStackTrace();
							}
						}
						return 0;
					}
				});
		try {
			client.join();
		} catch (Exception e) {
			e.printStackTrace();
		}
		client.close();
		System.out.println("�˳�");
		return 0;
	}

	static int aa=0;
	public static byte[] intToByteArray(final int integer) {
		int byteNum = (40 - Integer.numberOfLeadingZeros(integer < 0 ? ~integer
				: integer)) / 8;
		byte[] byteArray = null;
		if (integer < 0) {
			byteArray = new byte[] { -1, -1, -1, -1 };
		} else {
			byteArray = new byte[4];
		}

		for (int n = 0; n < byteNum; n++)
			byteArray[3 - n] = (byte) (integer >>> (n * 8));

		return (byteArray);
	}
	public static int test2() {
		String imgPath = "E://58.jpg";
		BufferedImage image;

		try {
			image = ImageIO.read(new FileInputStream(imgPath));
			System.out.println("getWidth " + image.getWidth());
			System.out.println("getHeight " + image.getHeight());
		} catch (Exception e) {
			e.printStackTrace();
		}
		System.out.println("test2 �˳�");
		return 0;
	}

	public static void main(String[] args) throws Exception {
		test1();
		return;
	}
}
