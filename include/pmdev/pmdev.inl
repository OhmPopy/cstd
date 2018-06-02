/////////////////////////////////////////////////////////////
// ��־��
// ���� 08:54 2016/5/31 ���ڶ� ������������Ƶ���벻����
// ���� 11:00 2016/5/5 ������ ����ʵʱʶ���¼�Ļ�ȡbug
// 9:30 2015/9/12 �����û����ؽӿںͼ�¼���ؽӿڣ���������������Ϣ��ȡ�ӿڡ�dev.cmd_GetNetPro();
/////////////////////////////////////////////////////////////
// ʹ��˵��:
// ��SDK ����������ʶ��3����
// ���������õ�TCP������ʹ�ü����ļ�ĩβ�� test1 ������
// ��Ƶ����õ�UDP������豸���ֻ���Ҫ���ó�ͬһ�����Ρ�
// ������Ƶ���� dev.cmd_StartVideo("192.168.1.3");
/////////////////////////////////////////////////////////////
#include "pmdev.h"
// У��ͼ���
static int sock_checksum(const byte* buffer, int off, int size)
{
  int cksum = 0;
  while ((size - 1) > off) {
    int low = buffer[off];
    int high = buffer[off + 1];
    int z = (int)(((high & 0xff) << 8) | (low & 0xff));
    cksum += z;
    // li->msg_DEBUGMSG(this, "cksum = " + cksum + " z = " + z);
    off += 2;
  }
  if (size > off) {
    cksum += buffer[off];
  }
  cksum = (cksum >> 16) + (cksum & 0xffff);
  cksum += (cksum >> 16);
  return (~cksum) & 0xffff;
}
struct DataInputStream {
  virtual int read(byte* by, int len) = 0;
  virtual int close() = 0;
  virtual int isClosed() = 0;
};
struct DataOutputStream {
  virtual int write(byte* by, int len) = 0;
  virtual int close() = 0;
  virtual int isClosed() = 0;
};
struct pmdev;
struct listener {
  virtual int msg_DEBUGMSG(pmdev* dev, const char* s) = 0;
  virtual int msg_CONNECT_SUCCESS(pmdev* dev) = 0;
  virtual int msg_CONNECT_CUT(pmdev* dev) = 0;
  virtual int msg_AUTH(pmdev* dev, int m_CFlag) = 0;
  virtual int msg_DEVINFO(pmdev* dev, BASEINFO* baseinfo) = 0;
  virtual int msg_STATIS(pmdev* dev, STATIS* statis) = 0;
  virtual int msg_NETPRO(pmdev* dev, NETPRO* netpro);
  virtual int msg_RECORDRANGE(pmdev* dev, int type, RECORDRANGE* range, int n) = 0;
  virtual int msg_RECORD(pmdev* dev, int type, RECORD* range, int n) = 0;
  virtual int msg_RECORDEX(pmdev* dev, int type, RECORD* range, int n) = 0;
  virtual int msg_VUSER(pmdev* dev, VUSER* vuser, int n) = 0;
  virtual int msg_USER(pmdev* dev, USER* vuser, int n) = 0;
  virtual int msg_VIDEO_OUT(pmdev* dev, int* data, int rgb_offset, int w, int h, int step) = 0;
};
struct pmdev {
  int m_nIndex;
  listener* li;
  DataInputStream* netInputStream;
  DataOutputStream* netOutputStream;
  byte* msg_buf;
  int msg_buf_len;
  int msg_identify; // ��ʶ
  int msg_len; // ���ݳ���
  pmdev() {
    msg_identify = -1;
  }
  int send_cmd_packet(int m_MCmd, int m_SCmd, int m_CFlag, byte* data, int len) {
    byte pBuffer[PACK_SIZE + OFFSET_SIZE];// ���60K��Сһ����
    int nTotalSend = 0;
    // int bOK = 1;
    int i = 0;
    int nSeq = ++m_nIndex;
    int m_Param1 = -1;
    int m_Param2 = 0;
    int cmdlen = len + NCMD_size;
    set_stream set(pBuffer, OFFSET_SIZE);
    set.set_INT((int) m_MCmd);
    set.set_INT((int) m_SCmd);
    set.set_INT((int) m_CFlag);
    set.set_INT((int) m_Param1);
    set.set_INT((int) m_Param2);
    for (i = 0; i < cmdlen;) {
      int nSend;
      // ��װ��������� �Ӱ�ͷ
      int msg_identify = nSeq; // ��ʶ
      int msg_offset = i * PACK_SIZE; // ƫ��
      int msg_total = cmdlen; // �����ܳ���
      int msg_checksum = 0; // �����
      int msg_len = MIN(PACK_SIZE, (cmdlen - i)); // ���ݳ���
      if (len > 0 && data != NULL) {
        if (0 == i) {
          memcpy(pBuffer + OFFSET_SIZE + NCMD_size, data, msg_len - NCMD_size);
        }
        else {
          memcpy(pBuffer + NCMD_size, data + i, msg_len);
        }
      }
      nSend = ALIGN(msg_len, 4) + OFFSET_SIZE;
      set = set_stream(pBuffer, 0);
      set.set_INT((int) msg_identify);
      set.set_INT((int) msg_offset);
      set.set_INT((int) msg_len);
      set.set_INT((int) msg_total);
      msg_checksum = sock_checksum(pBuffer, 0, OFFSET_SIZE);
      set.set_INT((int) msg_checksum);
      // println("send" + nSend);
      int ret = netOutputStream->write(pBuffer, nSend);
      if (ret < 0) {
        li->msg_CONNECT_CUT(this);
        return ret;
      }
      nTotalSend += msg_len;
      i += msg_len;
    }
    // li->msg_DEBUGMSG(this, "���� �ɹ�:" + nTotalSend + "bytes");
    return nTotalSend;
  }
  // int isexit;
  int close() {
    int ret = 0;
    if (NULL != netInputStream) {
      ret = netInputStream->close();
    }
    if (ret >= 0 && NULL != netOutputStream) {
      ret = netOutputStream->close();
    }
    if (ret < 0) {
      printf("close ����");
    }
    return ret;
  }
  int safe_read(byte* bytes, int off, int len) {
    int i = 0;
    for (; i < len;) {
      int ret = netInputStream->read(bytes + off + i, len - i);
      if (ret <= 0) {
        return -1;
      }
      i += ret;
    }
    return i;
  }
  int recv_pack_data() {
    char buf[256];
    byte pbuf[OFFSET_SIZE + 40];
    int ret = 0;
    get_stream get(pbuf, 0);
    // ��ȡ��ͷ
    ret = safe_read(pbuf, 0, OFFSET_SIZE);
    // li->msg_DEBUGMSG(this, "read " + ret + "bytes");
    if (ret == -1) {
      li->msg_DEBUGMSG(this, "read ��ͷ ��ʱ");
      return ret;
    }
    if (ret < 0) {
      li->msg_DEBUGMSG(this, "read ��ͷ ����");
      // close();
      return ret;
    }
    // li->msg_VIDEO_OUT(this, bytes2string(pbuf, 0, 20));
    if (ret != 20) {
      li->msg_DEBUGMSG(this, "read ��ͷ ���� ���Ȳ��� len=" + ret);
      return 0;
    }
    int checksum = sock_checksum(pbuf, 0, OFFSET_SIZE);
    // У��
    if (0 != checksum) {
      li->msg_DEBUGMSG(this, "У���������");
      return -3; // У���������
    }
    // ���ұ��α�ʶ�����ݽ��ջ���
    int identify = 0, offset = 0, len = 0, total = 0;
    get.get_INT(&identify);
    get.get_INT(&offset);
    get.get_INT(&len);
    get.get_INT(&total);
    if (identify != msg_identify || NULL == msg_buf) { // û�ҵ�����Ϊ����Ƶ���ݰ���
      if (offset != 0) { // �����Ƿ񶪰�ͷ������Ϊ������ͷ�ķǷ���Ƶ���ݡ�
        li->msg_DEBUGMSG(this, "������ͷ������");
        return -1;
      }
      // Ϊ���ձ�����Ƶ���ݿ���һ���컺�棬�����뵽MAPӳ�����
      myrealloc(&msg_buf, total);
      msg_buf_len = total;
      msg_identify = identify;
      msg_len = 0;
    }
    if (offset + len > total) {
      _snprintf(buf, 256, "safe_read ����%d %d", ret, len);
      li->msg_DEBUGMSG(this, buf);
      return 0;
    }
    // ������
    ret = safe_read(msg_buf, offset, len);
    // li->msg_DEBUGMSG(this, "len " + ret + " " + len);
    // li->msg_DEBUGMSG(this, "read " + ret + "bytes");
    if (ret < 0) {
      _snprintf(buf, 256, "safe_read ����%d %d", ret, len);
      li->msg_DEBUGMSG(this, buf);
      return -1;
    }
    msg_len += len;
    if (msg_len == msg_buf_len) {
      ret = msg_len;
      msg_identify = -1;
      // li->msg_DEBUGMSG(this, "recv_pack_data " + ret + "bytes");
    }
    return ret;
  }
  int GET_ERR(int state) {
    int errcode = ((state) & 0xff);
    switch (errcode) {
    case SUC_EXECUT:
      li->msg_DEBUGMSG(this, "����ɹ�");
      break;
    case ERR_EXECUT:
      li->msg_DEBUGMSG(this, "����ʧ��");
      break;
    case ERR_BUSY:
      li->msg_DEBUGMSG(this, "����: �豸æ");
      break;
    case ERR_LIMIT:
      li->msg_DEBUGMSG(this, "����: �Ѵ�����");
      break;
    case ERR_NOFIND:
      li->msg_DEBUGMSG(this, "����: û���ҵ���Ӧ����");
      break;
    case ERR_SAVE_FAIL:
      li->msg_DEBUGMSG(this, "����: ���ݱ���ʧ��");
      break;
    case ERR_SET_FAIL:
      li->msg_DEBUGMSG(this, "����: ����ʧ��");
      break;
    case ERR_VERIFY_FAIL:
      li->msg_DEBUGMSG(this, "����: ��֤ʧ��");
      break;
    case ERR_FROMAT:
      li->msg_DEBUGMSG(this, "����: ��ʽ��ʧ��");
      break;
    case ERR_PARAM:
      li->msg_DEBUGMSG(this, "����: ��������");
      break;
    case ERR_DISABLE:
      li->msg_DEBUGMSG(this, "����: Ҫ��ִ�еĹ���û��ʹ��");
      break;
    case ERR_SUPPORT:
      li->msg_DEBUGMSG(this, "����: ��֧�ֵ�����");
      break;
    case ERR_INPUTDATA:
      li->msg_DEBUGMSG(this, "����: ����˴�����������쳣");
      break;
    }
    return errcode;
  }
  int m_Author;
  int msg_proc(int m_MCmd, int m_SCmd, int m_CFlag, int m_Param1, int m_Param2) {
    int i, MCmd = m_MCmd;
    int off = NCMD_size;
    char buf[256];
    // int eResult = GET_ERR(m_CFlag);
    get_stream get = get_stream(msg_buf, off);
    _snprintf(buf, 256, "�յ���Ϣ m_MCmd=%d ����: %d", m_MCmd, msg_buf_len);
    li->msg_DEBUGMSG(this, buf);
    switch (MCmd) {
    case CMD_M_AUTH:
      m_Author = (m_CFlag & F_AUTH_VERIFY);
      // if ( (pCMD.m_CFlag & F_AUTH_VERIFY) ? DEV_AUTH_OPER :
      // DEV_AUTH_SET_OPER, DEV_OPER_UPLOAD, NULL, pCMD.m_Param1,
      // GET_ERR(pCMD.m_CFlag), pvClientData);
      li->msg_AUTH(this, m_CFlag);
      break;
    case CMD_M_DEVINFO: {
      BASEINFO baseinfo[1] = {0};
      get.get_INT(&baseinfo->m_DevType);
      get.get_INT(&baseinfo->m_LimitUser);
      get.get_INT(&baseinfo->m_Limit1NUser);
      get.get_CID(&baseinfo->m_SN);
      get.get_CID(&baseinfo->m_Ver);
      get.get_CID(&baseinfo->m_Space);
      li->msg_DEVINFO(this, baseinfo);
    }
    break;
    case CMD_M_STATIS: {
      STATIS statis[1] = {0};
      get.get_INT(&statis->m_TotalUsers);
      get.get_INT(&statis->m_NoFeatUser);
      get.get_INT(&statis->m_TotalDays);
      get.get_INT(&statis->m_TotalRecords);
      get.get_INT(&statis->m_TotalAlarm);
      get.get_INT(&statis->m_TotalDspLog);
      get.get_INT(&statis->m_TotalChangeImage);
      li->msg_STATIS(this, statis);
    }
    break;
    case CMD_M_NETPRO: {
      NETPRO netpro[1] = {0};
      get.get_NETPRO(netpro);
      li->msg_NETPRO(this, netpro);
    }
    break;
    case CMD_M_USER:
      if ((m_CFlag & F_DL_ID) != 0) {
        if (0 == GET_ERR(m_CFlag)) {
          VUSER* vuser = NULL;
          int m_Count = 0;
          get.get_INT(&m_Count);
          myrealloc(&vuser, m_Count*sizeof(VUSER));
          memset(vuser, 0, m_Count*sizeof(VUSER));
          for (i = 0; i < m_Count; ++i) {
            get.get_VUSER(vuser+i);
          }
          li->msg_VUSER(this, vuser, m_Count);
          free(vuser);
        }
      }
      break;
    case CMD_M_USER_EX:
      if (0 == GET_ERR(m_CFlag)) {
        switch (m_SCmd) {
        case CMD_S_GET: {
            int m_Count = 0;
            get.get_INT(&m_Count);
            _snprintf(buf, 256, "CMD_M_USER_EX m_Count: %d msg_buf.len: %d", m_Count, msg_buf_len);
              li->msg_DEBUGMSG(this, buf);
            if (m_Count > 0) {
              USER* user = NULL;
              myrealloc(&user, m_Count*sizeof(USER));
              memset(user, 0, m_Count*sizeof(USER));
              for (i = 0; i < m_Count; ++i) {
                get.get_USER(user+i);
              }
              li->msg_USER(this, user, m_Count);
              for (i = 0; i < m_Count; ++i) {
                free(user[i].m_Feat);
                free(user[i].m_Pic);
              }
              free(user);
            }
          }
          break;
        default:
          _snprintf(buf, 256, "�ݲ�֧�ֵ������� m_SCmd=%d", m_SCmd );
          li->msg_DEBUGMSG(this, buf);
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
        RECORD record[1] = {0};
        get.get_RECORD(record);
        li->msg_RECORD(this, m_MCmd, record, 1);
      }
      break;
    case CMD_M_OPTLOG:
    case CMD_M_CIMAGE:
    case CMD_M_RANGE:
      if (0 == GET_ERR(m_CFlag)) {
        int m_Count = 0;
        get.get_INT(&m_Count);
        _snprintf(buf, 256, "%d", m_Count);
        li->msg_DEBUGMSG(this, buf);
        RECORDRANGE* range = NULL;
        myrealloc(&range, m_Count*sizeof(RECORDRANGE));
        memset(range, 0, m_Count*sizeof(RECORDRANGE));
        for (i = 0; i < m_Count; ++i) {
          get.get_RECORDRANGE(range+i);
        }
        li->msg_RECORDRANGE(this, m_MCmd, range, m_Count);
        for (i = 0; i < m_Count; ++i) {
          get.get_RECORDRANGE(range+i);
        }
        free(range);
      }
      break;
    case CMD_M_RECORD_EX:
      if (0 == GET_ERR(m_CFlag)) {
        switch (m_SCmd) {
        case CMD_M_VERIFY:
        case CMD_M_ALARM:
          int m_Count = 0;
          get.get_INT(&m_Count);
          RECORD* record = NULL;
          myrealloc(&record, m_Count*sizeof(RECORD));
          memset(record, 0, m_Count*sizeof(RECORD));
          for (i = 0; i < m_Count; ++i) {
            get.get_RECORD(record+i);
          }
          li->msg_RECORDEX(this, m_MCmd, record, m_Count);
          for (i = 0; i < m_Count; ++i) {
            free(record[i].m_Pic);
          }
          free(record);
          break;
        }
      }
      break;
    default:
      break;
    }
    return 0;
  }
  void run() {
    msg_len = 0;
    msg_identify = -1;
    while (!netInputStream->isClosed()) {
      int ret = recv_pack_data();
      if (ret > 0) {
        get_stream get(msg_buf, 0);
        int m_MCmd, m_SCmd, m_CFlag, m_Param1, m_Param2;
        get.get_INT(&m_MCmd);
        get.get_INT(&m_SCmd);
        get.get_INT(&m_CFlag);
        get.get_INT(&m_Param1);
        get.get_INT(&m_Param2);
        if (CMD_M_HEART == m_MCmd) {
          if (SUC_EXECUT == GET_ERR(m_CFlag)) {
            // ����������
            send_cmd_packet(CMD_M_HEART, CMD_S_SET, 0, NULL, 0);
          }
        }
        else {
          msg_proc(m_MCmd, m_SCmd, m_CFlag, m_Param1, m_Param2);
        }
      } else if (ret < 0) {
        close();
        li->msg_DEBUGMSG(this, "recv_pack_data ����, �����Ͽ�");
      }
    }
  }
  // �� �� �豸��֤��������֤
  // �� �� cszDevAddr�豸��ַ
  // cName �û���
  // cPws ����
  // nFlag 1=��֤ 2=����
  // ˵ ��
  int cmd_SetDevAuth(const char* sLoginName, const char* sPassword, int nFlag) {
    byte pAuth[2 * ID_LEN];
    set_stream set(pAuth, 0);
    ASSERT(1==nFlag || 2==nFlag);
    set.set_STRING(sLoginName, ID_LEN);
    set.set_STRING(sPassword, ID_LEN);
    // ����������
    return send_cmd_packet(CMD_M_AUTH, CMD_S_SET, ((nFlag == 2) ? 0 : F_AUTH_VERIFY), pAuth, 2 * ID_LEN);
  }
  // �� �� �����豸���������ַ
  // �� ��
  // cszDevAddr �豸��ַ
  // cszRgnAddr �豸�����ַ
  // ˵ �� ���豸�������Ӻ�ɽ��豸���õ�ĳһ�������Ժ󼰿ɶ����������������豸
  int cmd_SetDevRegionAddr(const char* cszGroupIP) {
    byte cid[IP_LEN];
    set_stream set(cid, 0);
    set.set_STRING(cszGroupIP, IP_LEN);
    return send_cmd_packet(CMD_M_MCAST, CMD_S_SET, 0, cid, IP_LEN);
  }
  int cmd_GetDevRgn() {
    return send_cmd_packet(CMD_M_MCAST, CMD_S_GET, 0, NULL, 0);
  }
  // �� �� �����豸ϵͳʱ��
  // �� ��
  // cszDevAddr �豸��ַ
  // cSysTime ʱ����Ϣ
  // ˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
  // eType = DEV_SYS_TIME_OPER
  // eFlag = DEV_OPER_UPLOAD ��
  // pvContent = NULL
  int cmd_SetDevTime(DATETIME cTime) {
    byte* dt = new byte[DATETIME_size];
    set_stream set(dt, 0);
    set.set_DATETIME(cTime);
    return send_cmd_packet(CMD_M_TIME, CMD_S_SET, 0, dt, DATETIME_size);
  }
  // �� �� ��ȡ�豸ϵͳʱ��
  // �� �� cszDevAddr �豸��ַ
  // ˵ �� DevOperResultNotify�ص�������Ӧ���ͣ�
  // eType = DEV_SYS_TIME_OPER
  // eFlag = DEV_OPER_DOWNLOAD��
  // pvContent = DEV_DATETIME*
  int cmd_GetDevTime() {
    return send_cmd_packet(CMD_M_TIME, CMD_S_GET, 0, NULL, 0);
  }
  // �� �� ��ȡ�豸������Ϣ
  // �� �� cszDevAddr �豸��ַ
  // ˵ ��
  int cmd_GetDevBaseInfo() {
    return send_cmd_packet(CMD_M_DEVINFO, CMD_S_GET, 0, NULL, 0);
  }
  // �� �� ��ȡ�豸ͳ����Ϣ
  // �� �� cszDevAddr �豸��ַ
  // ˵ ��
  int cmd_GetDevStatisInfo() {
    return send_cmd_packet(CMD_M_STATIS, CMD_S_GET, 0, NULL, 0);
  }
  // �� �� ��ȡ��������������
  // �� ��
  // cszDevAddr ��������ַ
  // nSeq �������к�
  // ˵ �� ��������DEV_WORKATTͨ���ص�����������
  int cmd_GetDevWorkAttInfo() {
    return send_cmd_packet(CMD_M_DEVSET, CMD_S_GET, 0, NULL, 0);
  }
  // �� �� �����豸����������Ϣ
  // �� ��
  // cszDevAddr �豸��ַ
  // cWorkAtt �������Խṹ��
  // nSeq �������к�
  int cmd_SetDevWorkAttInfo(WORKATT workatt) {
    int pSet_len = 4 * (TGROUP_NUM + 3) + 4 + ID_LEN + 20;
    byte* pSet = new byte[pSet_len];
    set_stream set(pSet, 0);
    set.set_WORKATT(workatt);
    return send_cmd_packet(CMD_M_DEVSET, CMD_S_SET, 0, pSet, pSet_len);
  }
  // static int USER_JPEG_SIZE = (64 * 1024);
  // static int REC_JPEG_SIZE = (18 * 1024);
  // static int FEAT_SIZE = (90 * 1024);
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
  int cmd_SetUser(USER cUser) {
    byte* pUser = new byte[cUser.size()];
    set_stream set(pUser, 0);
    int len = set.off;
    return send_cmd_packet(CMD_M_USER, CMD_S_SET, 0, pUser, len);
  }
  // �� �� �ϴ�ʵʱ�����û�
  // �� ��
  // cszDevAddr �豸��ַ
  // cUser �û���Ϣ�ṹ��
  // m_CFlag ʵʱ������Ϣ [F_DL_APPENDID|F_DL_PIC|F_DL_FEAT]
  // nSeq �������к�
  // ˵ �� �豸������ͻ����ϴ�ĳ�û�ʱ��ͨ����?ӿ��ϴ��������û���Ϣ�?
  // cRtLoadΪ�豸������ʱ����������Ϣ���ͻ��˽�ֹ�޸ġ��˺���ʵ��Զ���û����ع��ܡ�
  int cmd_SetRealTimeUser(USER cUser, int m_CFlag) {
    byte* pUser = new byte[cUser.size()];
    set_stream set(pUser, 0);
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
  int cmd_SetRealTimeUserCap(const char* cUserID) {
    USER cUser = new USER();
    cUser.m_UserID = cUserID;
    byte* pUser = new byte[cUser.size()];
    set_stream set(pUser, 0);
    set.set_USER(cUser);
    int len = set.off;
    li->msg_DEBUGMSG(this, "send_cmd_packet len=" + len + " cUserID = " + cUserID + "cUser.size()=" + cUser.size());
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
  int cmd_DelUser(const char* cID) {
    byte* cid = new byte[ID_LEN];
    set_stream set(cid, 0);
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
  int cmd_GetSingleUser(const char* cID, int m_CFlag) {
    byte* cid = new byte[ID_LEN];
    set_stream set(cid, 0);
    set.set_CID(cID);
    return send_cmd_packet(CMD_M_USER, CMD_S_GET, 0, cid, ID_LEN);
  }
  // �� �� ���������û�
  // �� �� cszDevAddr �豸��ַ
  // ˵ ��
  int cmd_GetAllUser() {
    return send_cmd_packet(CMD_M_USER, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
        NULL, 0);
  }
  // �� �� ����ĳһʱ����ڵ�ע?��û?
  // �� ��
  // cszDevAddr �豸��ַ
  // cStart ��ʼ����
  // cEnd ��ֹ����
  // ˵ ��
  int cmd_GetSegTimeUser(NETDATE cStart, NETDATE cEnd) {
    byte* seg = new byte[16];
    set_stream set(seg, 0);
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
  int cmd_GetAllIdentifyRecord() {
    return send_cmd_packet(CMD_M_VERIFY, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
        NULL, 0);
  }
  // �� �� ����ָ��ʱ���ʶ���¼
  // �� �� cszDevAddr �豸��ַ
  // cStart ��ʼ����
  // cEnd ��ֹ����
  // ˵ ��
  int cmd_GetSegTimeIdentifyRecord(NETDATE cStart, NETDATE cEnd) {
    byte* seg = new byte[NETDATE.size * 2];
    set_stream set(seg, 0);
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
  int cmd_GetRangeRec(RECORDRANGE cRange, int m_CFlag) {
    byte* bytes = new byte[RECORDRANGE.size];
    set_stream set(bytes, 0);
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
  int cmd_GetAllAlarmRecord() {
    return send_cmd_packet(CMD_M_ALARM, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
        NULL, 0);
  }
  // �� �� ����ָ��ʱ��α�����¼
  // �� �� cszDevAddr �豸��ַ
  // cStart ��ʼ����
  // cEnd ��ֹ����
  // ˵ ��
  int cmd_GetSegTimeAlarmRecord(NETDATE cStart, NETDATE cEnd) {
    byte* seg = new byte[NETDATE.size * 2];
    set_stream set(seg, 0);
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
  int cmd_GetRangeAlarm(RECORDRANGE cRange, int m_CFlag) {
    byte* bytes = new byte[RECORDRANGE.size];
    set_stream set(bytes, 0);
    set.set_RECORDRANGE(cRange);
    return send_cmd_packet(CMD_M_RANGE, CMD_S_GET, m_CFlag, bytes,
        RECORDRANGE.size);
  }
  // �� �� ����������־
  // �� �� cszDevAddr �豸��ַ
  // ˵ ��
  int cmd_GetLog() {
    return send_cmd_packet(CMD_M_OPTLOG, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
        NULL, 0);
  }
  // �� �� ����������־
  // �� �� cszDevAddr �豸��ַ
  // cRange ID����
  // bKnowRange ָ�������ؾ������ݻ�������ID����(true=�������ݣ� false=ID����)
  // ˵ �� �˽ӿ����ڴ�ĳһ����ȷ�ļ�¼ID��ʼ����nRang������
  int cmd_GetRangeLog(RECORDRANGE cRange, int m_CFlag) {
    byte* bytes = new byte[RECORDRANGE.size];
    set_stream set(bytes, 0);
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
  int cmd_GetChangeImage() {
    return send_cmd_packet(CMD_M_CIMAGE, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
        NULL, 0);
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
  int cmd_GetRangeCImage(RECORDRANGE cRange, int m_CFlag) {
    byte bytes[RECORDRANGE_size];
    set_stream set(bytes, 0);
    set.set_RECORDRANGE(cRange);
    return send_cmd_packet(CMD_M_RANGE, CMD_S_GET, m_CFlag, bytes, RECORDRANGE_size);
  }
  int cmd_TimeGroup_impl(DSPTIMEG* cArray, int cArray_len, int m_SCmd, int m_CFlag) {
    byte* nData = NULL;
    int nData_len=cArray_len * DSPTIMEG_size + 4;
    myrealloc(&nData, nData_len);
    int ret;
    set_stream set(nData, 0);
    set.set_INT(cArray_len);
    for (int i = 0; i < cArray_len; ++i) {
      set.set_DSPTIMEG(cArray+i);
    }
    ret = send_cmd_packet(CMD_M_TGROUP, m_SCmd, m_CFlag, nData, nData_len);
    free(nData);
    return ret;
  }
  // �� �� ����ʱ����
  // �� �� cszDevAddr �豸��ַ
  // cTGArray ʱ����
  // ˵ ��
  int cmd_SetTimeGroup(DSPTIMEG* cTGArray, int cTGArray_len) {
    return cmd_TimeGroup_impl(cTGArray, cTGArray_len, CMD_S_SET, F_DL_NORMAL_TG | F_DL_HOLIDAY_TG);
  }
  // �� �� ��ȡʱ����
  // �� �� dev_addr �豸��ַ
  // cTGArray ʱ����
  // m_CFlag ʱ��������
  // ˵ �� cTGArray.m_nCount=0ʱ��ʾ����eType���͵�����ʱ����
  int cmd_GetTimeGroup(DSPTIMEG* cTGArray, int cTGArray_len, int m_CFlag) {
    return cmd_TimeGroup_impl(cTGArray, cTGArray_len, CMD_S_GET, m_CFlag);
  }
  // �� �� ɾ��ʱ����
  // �� �� cszDevAddr �豸��ַ
  // cTGArray ʱ����
  // eType ʱ��������
  // ˵ �� eType.m_nCount=0ʱ��ʾɾ��tg_type��������ʱ����
  int cmd_DelTimeGroup(DSPTIMEG* cTGArray, int cTGArray_len, int m_CFlag) {
    return cmd_TimeGroup_impl(cTGArray, cTGArray_len, CMD_S_DEL, m_CFlag);
  }
  int cmd_Right_impl(DSPRIGHT* cArray, int cArray_len, int m_SCmd) {
    byte* nData = NULL;
    int nData_len = cArray_len * DSPRIGHT_size + 4;
    myrealloc(&nData, cArray_len);
    set_stream set(nData, 0);
    set.set_INT(cArray_len);
    for (int i = 0; i < cArray_len; ++i) {
      set.set_DSPRIGHT(cArray+i);
    }
    return send_cmd_packet(CMD_M_DSPRIGHT, m_SCmd, 0, nData, nData_len);
  }
  // �� �� ����Ȩ��
  // �� �� cszDevAddr �豸��ַ
  // cRTArray Ȩ��
  // ˵ ��
  int cmd_SetRight(DSPRIGHT* cRTArray, int cArray_len) {
    return cmd_Right_impl(cRTArray, CMD_S_SET);
  }
  // �� �� ���ͻ�ȡȨ��
  // �� �� cszDevAddr �豸��ַ
  // cRTArray Ȩ��
  // ˵ �� ��rt_array.m_nCount=0ʱ��ʾ��������Ȩ��
  int cmd_GetRight(DSPRIGHT* cRTArray, int cArray_len) {
    return cmd_Right_impl(cRTArray, CMD_S_GET);
  }
  // �� �� ɾ��Ȩ��
  // �� �� cszDevAddr �豸��ַ
  // cRTArray Ȩ??
  // ˵ �� ��rt_array.m_nCount=0ʱ��ʾɾ������Ȩ��
  int cmd_DelRight(DSPRIGHT* cRTArray, int cArray_len) {
    return cmd_Right_impl(cRTArray, CMD_S_DEL);
  }
  int cmd_UserGroup_impl(USERGROUP* cArray, int m_SCmd) {
    byte* nData = new byte[cArray_len * DSPRIGHT.size + 4];
    set_stream set(nData, 0);
    set.set_INT(cArray_len);
    for (int i = 0; i < cArray_len; ++i) {
      set.set_USERGROUP(cArray[i]);
    }
    return send_cmd_packet(CMD_M_UGROUP, m_SCmd, 0, nData, nData_len);
  }
  // �� �� ������?��?
  // �� �� cszDevAddr �豸��ַ
  // cUGrray �û���
  // ˵ ��
  int cmd_SetUserGroup(USERGROUP* cUGrray) {
    return cmd_UserGroup_impl(cUGrray, CMD_S_SET);
  }
  // �� �� ���ͻ�ȡ�û���
  // �� �� cszDevAddr �豸��ַ
  // ug_array �û���
  // ˵ �� ��rt_array.m_nCount=0ʱ��ʾ���������û���
  int cmd_GetUserGroup(USERGROUP* cUGrray) {
    return cmd_UserGroup_impl(cUGrray, CMD_S_GET);
  }
  // �� �� ɾ���û���
  // �� �� cszDevAddr �豸��ַ
  // ug_array �û���
  // ˵ �� ��rt_array.m_nCount=0ʱ��ʾɾ�������û���
  int cmd_DelUserGroup(USERGROUP* cUGrray) {
    return cmd_UserGroup_impl(cUGrray, CMD_S_DEL);
  }
  // �� �� ���ÿ�����״̬
  // �� ��
  // cszDevAddr �豸��ַ
  // bEOpen true=�������ţ� false=��������
  // ˵ ��
  int cmd_SetOpenDoorState(int eState) {
    return send_cmd_packet(CMD_M_EOPEN, CMD_S_SET, eState, NULL, 0);
  }
  // �� �� ��ȡ������״̬
  // �� �� cszDevAddr �豸��ַ
  // bEOpen true=�������ţ� false=��������
  // ˵ ��
  int cmd_GetOpenDoorState() {
    return send_cmd_packet(CMD_M_EOPEN, CMD_S_GET, 0, NULL, 0);
  }
  // �� �� Զ�̿���
  // �� �� cszDevAddr �豸��ַ
  // bEOpen true=���ţ� false=����
  // pDoorArray �ŵ�����
  // size �����С
  // ˵ ��
  int cmd_SetRemoteOpenDoor(int bOpen) {
    int nFlag = 0;
    if (0 != bOpen) {
      nFlag = F_DOOR_OPEN;
    }
    else {
      nFlag = F_DOOR_CLOSE;
    }
    return send_cmd_packet(CMD_M_REMOTE_OPEN, CMD_S_SET, nFlag, NULL, 0);
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
  int cmd_IOCtrl(IOCTRL ioctrl) {
    byte* nData = new byte[IOCTRL.size];
    set_stream set(nData, 0);
    set.set_IOCTRL(ioctrl);
    return send_cmd_packet(CMD_M_IOCTRL, CMD_S_SET, 0, nData, IOCTRL.size);
  }
  // �� �� ��ʽ���豸
  // �� �� cszDevAddr �豸��ַ
  // ˵ ��
  int cmd_SetFormat() {
    return send_cmd_packet(CMD_M_FORMAT, CMD_S_SET, 0, NULL, 0);
  }
  // �� �� �豸�������
  // �� �� cszDevAddr �豸��ַ
  // cAppData ������½ṹ��
  // ˵ ��
  int cmd_SetUpdate(APPUPDATE cAppData) {
    byte* nData = new byte[cAppData.size()];
    set_stream set(nData, 0);
    set.set_APPUPDATE(cAppData);
    return send_cmd_packet(CMD_M_APPUPDATE, CMD_S_SET, 0, nData,
        nData_len);
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
  int cmd_SetUserEx(USER* cUsers, int nFlag) {
    int sz = 0;
    for (int i = 0; i < cUsers_len; ++i) {
      sz += cUsers[i].size();
    }
    byte* nData = new byte[sz + 4];
    set_stream set(nData, 0);
    set.set_INT(cUsers_len);
    for (int i = 0; i < cUsers_len; ++i) {
      set.set_USER(cUsers[i]);
    }
    return send_cmd_packet(CMD_M_USER_EX, CMD_S_SET, USER_TEXT, nData,
        nData_len);
  }
  int cmd_UserEx_impl(const char** pUserIDArray, int m_SCmd, int m_CFlag,
      int max_n) {
    int ret = 0;
    for (int j = 0; j < pUserIDArray_len;) {
      int m_Count = MIN(max_n, pUserIDArray_len - j);
      byte* nData = new byte[ID_LEN * m_Count + 8];
      set_stream set(nData, 0);
      set.set_INT(m_Count);
      for (int i = 0; i < m_Count; ++i) {
        set.set_CID(pUserIDArray[j + i]);
      }
      ret = send_cmd_packet(CMD_M_USER_EX, m_SCmd, m_CFlag, nData,
          nData_len);
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
  int cmd_GetUserEx(const char** pUserIDArray, int m_CFlag) {
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
  int cmd_DelUserEx(const char** pUserIDArray) {
    return cmd_UserEx_impl(pUserIDArray, CMD_S_DEL, 0, 1000);
  }
  // �� �� �������ؼ�¼
  // �� �� cszDevAddr �豸��ַ
  // cRange ID����
  // ˵ �� ������֤��¼��������¼��������־�����ע����
  int cmd_GetRecEx(RECORDRANGE cRange) {
    byte* nData = new byte[RECORDRANGE.size];
    set_stream set(nData, 0);
    set.set_RECORDRANGE(cRange);
    return send_cmd_packet(CMD_M_RECORD_EX, CMD_S_GET, F_DL_PIC, nData,
        nData_len);
  }
  // �� �� �����豸
  // �� �� cszDevAddr �豸��ַ
  // ˵ ��
  int cmd_RebootDev() {
    return send_cmd_packet(CMD_M_REBOOT, CMD_S_SET, 0, NULL, 0);
  }
  // �� �� ���/���� �豸������Ϣ
  // �� �� cszDevAddr �豸��ַ
  // pNetPro NULL��ʾ��ã���Ϊ�ձ�ʾ����
  // ˵ ��
  int cmd_SetNetPro(NETPRO pNetPro) {
    byte* nData = new byte[NETPRO.size];
    set_stream set(nData, 0);
    set.set_NETPRO(pNetPro);
    return send_cmd_packet(CMD_M_NETPRO, CMD_S_SET, 0, nData, nData_len);
  }
  int cmd_GetNetPro() {
    return send_cmd_packet(CMD_M_NETPRO, CMD_S_GET, 0, NULL, 0);
  }
  int connect(const char* ip, const char* user, const char* pass, listener* listener) {
    try {
      devip = ip;
      li = listener;
      InetSocketAddress address = NULL;
      address = new InetSocketAddress(ip, TCPPORT);
      sc = new Socket();
      sc.connect(address, 1000);
      sc.setSoTimeout(1000);
      netInputStream = new DataInputStream(sc.getInputStream());
      netOutputStream = new DataOutputStream(sc.getOutputStream());
      li->msg_CONNECT_SUCCESS(this);
      cmd_SetDevAuth(user, pass, 1);
      msg_identify = -1;
      start();
      // 1.send file length
    }
    catch (SocketTimeoutException e) {
      li->msg_DEBUGMSG(this, "connetc ��ʱ :" + ip + " �쳣");
    }
    catch (UnknownHostException e) {
      li->msg_DEBUGMSG(this, "-.δ֪��������:" + ip + " �쳣");
    }
    catch (IOException e) {
      
    }
    catch (Exception e) {
      
      li->msg_DEBUGMSG(this, "-hostName=" + ip + " portNum=" + TCPPORT
          + "---.IO�쳣����" + e.getMessage());
    }
    return 0;
  }
  struct video_th {
    DatagramSocket ds = NULL;
    int m_nIndex;
    int msg_len;
    int msg_identify;
    int port;
    volatile boolean exit = false;
    pmdev* dev;
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
      }
      catch (Exception e) {
        
        throw new RuntimeException(e);
      }
      finally {
        start();
      }
      return port;
    }
    int clip(int x) {
      if (x < 0) {
        return 0;
      }
      if (x > 255) {
        return 255;
      }
      return x;
    }
    int* YUYV_RGB_int(byte* src, int off, int step, int width,
        int height) {
      int r, g, b;
      int R, G, B;
      int Y00, Y01, U, V;
      int numOfPixel = width * height;
      int* rgb = new int[numOfPixel];
      for (int i = 0; i < height; i++) {
        int startY = off + i * step;
        int index = i * width;
        for (int j = 0; j < width; j += 2) {
          Y00 = src[startY + 0];
          U = src[startY + 1];
          Y01 = src[startY + 2];
          V = src[startY + 3];
          startY += 4;
          r = (int)(1.4075 * ((V & 0xff) - 128));
          g = (int)(0.3455 * ((U & 0xff) - 128) + 0.7169 * ((V & 0xff) - 128));
          b = (int)(1.779 * ((U & 0xff) - 128));
          R = ((Y00 & 0xff) + r);
          G = ((Y00 & 0xff) - g);
          B = ((Y00 & 0xff) + b);
          R = clip(R);
          G = clip(G);
          B = clip(B);
          rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
          index += 1;
          R = (int)((Y01 & 0xff) + r);
          G = (int)((Y01 & 0xff) - g);
          B = (int)((Y01 & 0xff) + b);
          R = clip(R);
          G = clip(G);
          B = clip(B);
          rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
          index += 1;
        }
      }
      return rgb;
    }
    int* UYVY_RGB_int(byte* src, int off, int step, int width,
        int height) {
      int r, g, b;
      int R, G, B;
      int Y00, Y01, U, V;
      int numOfPixel = width * height;
      int* rgb = new int[numOfPixel];
      for (int i = 0; i < height; i++) {
        int startY = off + i * step;
        int index = i * width;
        for (int j = 0; j < width; j += 2) {
          U = src[startY + 0];
          Y00 = src[startY + 1];
          V = src[startY + 2];
          Y01 = src[startY + 3];
          startY += 4;
          r = (int)(1.4075 * ((V & 0xff) - 128));
          g = (int)(0.3455 * ((U & 0xff) - 128) + 0.7169 * ((V & 0xff) - 128));
          b = (int)(1.779 * ((U & 0xff) - 128));
          R = ((Y00 & 0xff) + r);
          G = ((Y00 & 0xff) - g);
          B = ((Y00 & 0xff) + b);
          R = clip(R);
          G = clip(G);
          B = clip(B);
          rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
          index += 1;
          R = (int)((Y01 & 0xff) + r);
          G = (int)((Y01 & 0xff) - g);
          B = (int)((Y01 & 0xff) + b);
          R = clip(R);
          G = clip(G);
          B = clip(B);
          rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
          index += 1;
        }
      }
      return rgb;
    }
    int* RGB24_RGB_int(byte* src, int off, int step, int width,
        int height) {
      int R, G, B;
      int numOfPixel = width * height;
      int* rgb = new int[numOfPixel];
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
    int close() {
      try {
        exit = true;
        // sleep(1000); // ���߳��ӳ�1��
        // interrupt();
        join();
      }
      catch (Exception e) {
        
      }
      return 0;
    };
    void run() {
      msg_len = 0;
      msg_identify = -1;
      exit = false;
      dev.li->msg_DEBUGMSG(dev, "video Thread run");
      int PACK_SIZEEX = PACK_SIZE + 20;
      byte* buf = new byte[PACK_SIZEEX];
      byte* data = NULL;
      int pre_identify = -1, len = 0;
      while (!ds.isClosed() && !exit) {
        try {
          DatagramPacket dp = NULL;
          dp = new DatagramPacket(buf, PACK_SIZEEX); // ���ڽ��ճ���Ϊ1024�����ݰ����������ݰ������ݲ��ִ���buf��
          ds.receive(dp); // �Ӵ��׽��ֽ������ݱ���
          // li->msg_DEBUGMSG(this, "ip" + dp.getAddress().toconst char*() +
          // " len=" + dp.getLength());
          int recelen = dp.getLength();
          if (recelen < 0) {
            continue;
          }
          if (sock_checksum(buf, 0, 20) != 0) {
            dev.li->msg_DEBUGMSG(dev, "У�����");
            continue;
          }
          get_stream get = get_stream(buf, 0);
          int msg_identify = get.get_INT(); // ��ʶ
          int msg_offset = get.get_INT(); // ƫ��
          int msg_len = get.get_INT();// ���Ѿ����͹������������ݳ��ȣ���������ͷ�ĳ��ȣ������������ͷ����
          int msg_total = get.get_INT();// �ܵ��������ݳ��ȣ���������ͷ���Ⱥ�����Ĳ�������
          // int msg_checksum = ;
          get.get_INT();// �����
          // dev.li->msg_DEBUGMSG(dev, "msg_identify=" + msg_identify +
          // " msg_offset=" + msg_offset + " msg_len=" + msg_len +
          // " msg_total=" + msg_total);
          if (pre_identify != msg_identify && 0 == msg_offset) {
            if (NULL == data || data_len < msg_total) {
              data = new byte[msg_total];
            }
            len = 0;
          }
          bytecopy(buf, 20, data, msg_offset, msg_len);
          len += msg_len;
          if (len >= msg_total) {
            get = get_stream(data, 20);
            int m_Size = get.get_INT(); // ͼ�������ܴ�СBYTES
            int m_W = get.get_INT(); // ͼ���
            int m_H = get.get_INT(); // ͼ���
            int m_Step = get.get_INT(); // �����ݲ���
            int m_Type = get.get_BYTE(); // �������� IMAGE_UYVY
            int off = 4 * 4 + 1;
            //dev.li->msg_DEBUGMSG(dev, "m_Size=" + m_Size + " m_W="
            // + m_W + " m_H=" + m_H + " m_Step=" + m_Step);
            int* rgb = NULL;
            if (IMAGE_YUYV == m_Type) {
              rgb = YUYV_RGB_int(data, 20 + off, m_Step, m_W, m_H);
            }
            else if (IMAGE_UYVY == m_Type) {
              rgb = UYVY_RGB_int(data, 20 + off, m_Step, m_W, m_H);
            }
            else if (IMAGE_RGB24 == m_Type) {
              rgb = RGB24_RGB_int(data, 20 + off, m_Step, m_W, m_H);
            }
            else {
              rgb = NULL;
            }
            // BufferedImage image = new BufferedImage(WIDTH,
            // HEIGHT, BufferedImage.TYPE_3BYTE_BGR);
            // Image aa;
            // Bitmap bitmap = Bitmap.createBitmap(data, off,
            // m_Step, m_W, m_H,
            // Bitmap.Config.ARGB_8888);
            dev.li->msg_VIDEO_OUT(dev, rgb, 0, m_W, m_H, m_W);
          }
        }
        catch (SocketTimeoutException e) {
          ds.close();
          dev.li->msg_DEBUGMSG(dev, "udp ��ʱ :" + devip + " �쳣");
        }
        catch (Exception e) {
          ds.close();
          dev.li->msg_DEBUGMSG(dev, "udp receive DatagramPacket ����, �����Ͽ�");
          return;
        }
      }
      ds.close();
      ds = NULL;
      dev.li->msg_DEBUGMSG(dev, "�߳��˳�!");
    }
  };
  int ntohs(int x) {
    return (short)(((x & 0xff) << 8) | ((x >> 8) & 0xff));
  }
  video_th vth;
  int m_port = 0;
  int cmd_StartVideo(const char* myIP) {
    int ret = 0;
    if (NULL == vth) {
      vth = new video_th();
      vth.dev = this;
      m_port = vth.startUDP();
      byte* nData = new byte[18];
      set_stream set(nData, 0);
      // int port1 = ntohs(port);
      set.set_SHORT((short) m_port);
      set.set_STRING(myIP, 16);
      li->msg_DEBUGMSG(this, "cmd_StartVideo ip = " + myIP + " port="
          + m_port);
      ret = send_cmd_packet(CMD_M_VIDEO_OUT, CMD_S_SET, 0, nData,
          nData_len);
    }
    else {
      li->msg_DEBUGMSG(this, "��Ƶ�Ѿ��򿪣����ȹر�!");
    }
    return ret;
  }
  // ֹͣ��Ƶ
  int cmd_StopVideo() {
    int retVal = 0;
    byte* nData = new byte[18];
    set_stream set(nData, 0);
    // �ر���Ƶ
    // ����������
    set.set_SHORT((short) m_port);
    set.set_STRING(devip, 16);
    retVal = send_cmd_packet(M_VIDEO_OUT, S_GET, 0, nData, nData_len);
    if (vth != NULL) {
      vth.close();
      vth = NULL;
    }
    else {
      li->msg_DEBUGMSG(this, "��Ƶ�ѹر�!�����ٹر�!");
    }
    return retVal;
  }
  static int test1() {
    pmdev client = new pmdev();
    //155
    client.connect("192.168.1.46", "admin", "201031",
    new pmdev.listener() {
      int println(const char* s) {
        System.out.println(s);
        return 0;
      }
      int msg_DEBUGMSG(pmdev * dev, const char* s) {
        println("DEBUG: " + s);
        return 0;
      }
      int msg_CONNECT_SUCCESS(pmdev * dev) {
        println("������:" + dev.get_ip() + "�ɹ�");
        return 0;
      }
      int msg_CONNECT_CUT(pmdev * dev) {
        println("������:" + dev.get_ip() + "�Ͽ�����");
        return 0;
      }
      int msg_AUTH(pmdev * dev, int m_CFlag) {
        println("msg_AUTH");
        if ((m_CFlag & F_AUTH_VERIFY) != 0) {
          println("�û������֤ �ɹ�!");
        }
        else {
          println("�豸��֤�û����ò��� �ɹ�!");
        }
        // dev.cmd_GetAllUser();
        // dev.cmd_GetNetPro(); // ��ȡ����������Ϣ
        // dev.cmd_GetAllIdentifyRecord();
        dev.cmd_StopVideo();
        dev.cmd_StartVideo("192.168.1.3");
        try {
          sleep(1000);
        }
        catch (InterruptedException e) {
          
        } // �ӳ�1��
        //dev.cmd_StopVideo();
        //dev.cmd_StartVideo("192.168.1.3");
        dev.cmd_GetDevBaseInfo();
        //println("cmd_SetRealTimeUserCap!");
        //dev.cmd_SetRealTimeUserCap("0000");
        // dev.cmd_GetDevStatisInfo();
        return 0;
      }
      int msg_NETPRO(pmdev * dev, pmdev.NETPRO netpro) {
        println("msg_NETPRO");
        println("�豸%s:��ȡ������Ϣ�ɹ�");
        println("<< �豸������Ϣ ");
        println(" m_WebPro1: " + netpro->m_WebPro1);
        println(" m_WebPro2: " + netpro->m_WebPro2);
        println(" m_WebUrl1: " + netpro->m_WebUrl1);
        println(" m_WebUrl2: " + netpro->m_WebUrl2);
        println(" m_IPAdr: " + netpro->m_IPAdr);
        println(" m_MIPAdr: " + netpro->m_MIPAdr);
        println(" m_NetMask: " + netpro->m_NetMask);
        println(" m_GateWay: " + netpro->m_GateWay);
        println(" m_Dns: " + netpro->m_Dns);
        println(">>");
        return 0;
      }
      int msg_DEVINFO(pmdev * dev, pmdev.BASEINFO baseinfo) {
        println("msg_DEVINFO");
        println("�豸%s:��ȡ������Ϣ�ɹ�");
        println("<< �豸������Ϣ ");
        println(" �豸������Ϣ " + baseinfo->tostring());
        println(">>");
        return 0;
      }
      int msg_STATIS(pmdev * dev, pmdev.STATIS statis) {
        println("msg_STATIS");
        println("�豸%s:��ȡͳ����Ϣ�ɹ�: " + dev.get_ip());
        println(" �豸ͳ����Ϣ " + statis->tostring());
        return 0;
      }
      int msg_RECORDRANGE(pmdev * dev, int type,
      pmdev.RECORDRANGE* range) {
        println("msg_RECORDRANGE");
        println("��¼����: " + type);
        println("��¼������Ŀ: " + range_len);
        for (int i = 0; i < range_len; ++i) {
          println("m_Count: " + range[i].m_Count
              + " m_Flag: " + range[i].m_Flag
              + " m_SID: " + range[i].m_SID);
        }
        if (range_len > 0) {
          dev.cmd_GetRecEx(range[0]);
        }
        return 0;
      }
      int msg_RECORD(pmdev * dev, int type,
      pmdev.RECORD* range) {
        println("msg_RECORD");
        for (int i = 0; i < range_len; ++i) {
          println(i + " ID: " + range[i].m_UserID + "ʱ��"
              + range[i].m_RecTime.tostring());
        }
        return 0;
      }
      int msg_RECORDEX(pmdev * dev, int type,
      pmdev.RECORD* range) {
        println("msg_RECORDEX");
        for (int i = 0; i < range_len; ++i) {
          println(i + " ID: " + range[i].m_UserID + "ʱ��"
              + range[i].m_RecTime.tostring());
        }
        return 0;
      }
      int msg_VUSER(pmdev * dev, pmdev.VUSER* vuser) {
        println("msg_VUSER");
        println("��ȡ�û��б�ɹ�!");
        for (int i = 0; i < vuser_len; ++i) {
          // println("�û�: " + vuser[i].m_UserID);
        }
        if (vuser_len > 2) {
          // ���������˵�����
          const char** strlist = new const char*[2];
          strlist[0] = vuser[0].m_UserID;
          strlist[1] = vuser[1].m_UserID;
          dev.cmd_GetUserEx(strlist, USER_TEXT | DL_FEAT
              | DL_IMAGE);
        }
        println("�û�����: " + vuser_len);
        return 0;
      }
      int msg_USER(pmdev * dev, pmdev.USER* user) {
        println("msg_USER");
        println("�û�����: " + user_len);
        for (int i = 0; i < user_len; ++i) {
          println("�û�: " + user[i].m_UserID + "�������� "
              + user[i].m_Feat_len);
        }
        return 0;
      }
      int msg_VIDEO_OUT(pmdev * dev, int* data,
      int yuvdataoff, int w, int h, int step) {
        println("��Ƶ: w:" + w + " h:" + h + " datalen:" + data_len);
        ++aa;
        if (aa < 10) {
          println("asdfadsf");
          FileOutputStream fos = NULL;
          try {
            fos = new FileOutputStream("C:\\aaa.dat");
          }
          catch (FileNotFoundException e) {
            
          }
          // ��FileOutputStream ��write����д���ֽ�����
          try {
            for (int i = 0; i < data_len; ++i) {
              fos.write(intToByteArray(data[i]));
            }
          }
          catch (IOException e) {
            
          }
          System.out.println("д��ɹ�");
          // Ϊ�˽�ʡIO���Ŀ�������Ҫ�ر�
          try {
            fos.close();
          }
          catch (IOException e) {
            
          }
        }
        return 0;
      }
    });
    try {
      client.join();
    }
    catch (Exception e) {
      
    }
    client.close();
    System.out.println("�˳�");
    return 0;
  }
  static int aa = 0;
  static byte* intToByteArray(final int integer) {
    int byteNum = (40 - Integer.numberOfLeadingZeros(integer < 0 ? ~integer
        : integer)) / 8;
    byte* byteArray = NULL;
    if (integer < 0) {
      byteArray = new byte* { -1, -1, -1, -1 };
    }
    else {
      byteArray = new byte[4];
    }
    for (int n = 0; n < byteNum; n++) {
      byteArray[3 - n] = (byte)(integer >>> (n * 8));
    }
    return (byteArray);
  }
  static int test2() {
    const char* imgPath = "E://58.jpg";
    BufferedImage image;
    try {
      image = ImageIO.read(new FileInputStream(imgPath));
      System.out.println("getWidth " + image.getWidth());
      System.out.println("getHeight " + image.getHeight());
    }
    catch (Exception e) {
      
    }
    System.out.println("test2 �˳�");
    return 0;
  }
  static void main(const char** args) throws Exception {
    test1();
    return;
  }

};
#if 0

#endif

