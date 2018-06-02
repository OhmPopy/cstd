// DevManager.h: interface for the DevManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVMANAGER_H__EF52C8CA_C60E_484E_B595_6B9FFF3A316A__INCLUDED_)
#define AFX_DEVMANAGER_H__EF52C8CA_C60E_484E_B595_6B9FFF3A316A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Net/Net.h"
#include "udisk/UDisk.h"
#include "dev/common.h"
#include "video/video.h"
#include "thread.hpp"

class DspDevManager : public CThread
{
  friend class DevData;
  friend class CNet;
  friend class CUDisk;
public:
  DspDevManager();
  virtual ~DspDevManager();
public:
  void RegDevOPNotify(DevOperResultNotify pfnOpNotify, void* pvContext) { //���ݲ�������ص�
    //    AoutLock lock( &m_syn );
    m_pfnOpNotify = pfnOpNotify;
    m_pOpNotifyContext = pvContext;
  }
  void RegNetCNNotify(DevConnectStatus pfnCN, void* pCNContext) { //ע���豸����״̬���ѻص�
    //    AoutLock lock( &m_syn );
    m_pfnCN = pfnCN;
    m_pCNContext = pCNContext;
  }
public:
  BOOL InitSys(BOOL bFlag);
  BOOL DevSerch(BOOL bFlag, const char* cszGroupIP = DEV_REGION_ADDR);
  //�豸����ģʽ
  BOOL SetMode(BOOL srvMode, QuerySrvCB pfn,  void* pvContext);
  //����/�Ͽ���ת������
  BOOL CnSrv(BOOL bFlag, const char* ip, const char* name, const char* psw);
  //���ӶϿ��豸
  BOOL CNDev(const char* cszIP);
  void DCNDev(const char* cszIP);
  //��֤
  BOOL Auth(const char* cszIP, const DEV_CID& cName, const DEV_CID& cPws, int nFlag, int nSeq = -1);
  //����
  BOOL SetDevRgn(const char* cszIP, const char* cszGroupIP = DEV_REGION_ADDR, int nSeq = -1);
  BOOL GetDevRgn(const char* cszIP, int nSeq = -1);
  //ʱ��
  BOOL SetDevTime(const char* cszIP, const DEV_DATETIME& cTime, int nSeq = -1);
  BOOL GetDevTime(const char* cszIP, int nSeq = -1);
  //Ӳ��������Ϣ
  BOOL GetDevInfo(const char* cszIP, int nSeq = -1);
  //ͳ����Ϣ
  BOOL GetDevStatis(const char* cszIP, int nSeq = -1);
  //��������
  BOOL SetDevWorkAtt(const char* cszIP, const DEV_WORKATT& cAtt, int nSeq = -1);
  BOOL GetDevWorkAtt(const char* cszIP, int nSeq = -1);
  //��ʽ���豸
  BOOL FormatDev(const char* cszIP, int nSeq = -1);
  //�����豸����
  BOOL UpdateApp(const char* cszIP, const DEV_APPUPDATE& cData, int nSeq = -1);
  //�û�
  BOOL AddDevUser(const char* cszIP, const DEV_USER& cUser, int nSeq = -1);
  BOOL AddDevRealTimeUser(const char* cszIP, const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cLoad);
  BOOL RealTimeUserCap(const char* cszDevAddr, const DEV_USER& cUser, int nSeq) ;
  BOOL GetDevUser(const char* cszIP, const DEV_DATA_DL& cDlData, int nSeq = -1);
  BOOL DelDevUser(const char* cszIP, const DEV_CID& cID, int nSeq = -1);
  //��¼
  BOOL GetDevRecord(const char* cszIP, int nClass, const DEV_DATA_DL& cDlData, int nSeq = -1);
  //��������
  BOOL SetOpenDoorState(const char* cszIP, DEV_DOOR_STATE eState, int nSeq = -1);
  BOOL GetOpenDoorState(const char* cszIP, int nSeq = -1);
  BOOL RemoteOpenDoor(const char* cszDevAddr, BOOL bOpen, int nSeq = -1);
  BOOL IOCtrl(const char* cszDevAddr, const DEV_IOCTRL& ioctrl, int nSeq = -1);
  //ʱ����
  BOOL SetDevTG(const char* cszIP, const DEV_TIMEGROUP_ARRAY& cArray, int nSeq = -1);
  BOOL GetDevTG(const char* cszIP, const DEV_TIMEGROUP_ARRAY& cArray, DEV_TIMEGROUP_TYPE eType, int nSeq = -1);
  BOOL DelDevTG(const char* cszIP, const DEV_TIMEGROUP_ARRAY& cArray, DEV_TIMEGROUP_TYPE eType, int nSeq = -1);
  //Ȩ��
  BOOL SetDevRT(const char* cszIP, const DEV_RIGHT_ARRAY& cArray, int nSeq = -1);
  BOOL GetDevRT(const char* cszIP, const DEV_RIGHT_ARRAY& cArray, int nSeq = -1);
  BOOL DelDevRT(const char* cszIP, const DEV_RIGHT_ARRAY& cArray, int nSeq = -1);
  //�û���
  BOOL SetDevUserGroup(const char* cszIP, const DEV_USERGROUP_ARRAY& cArray, int nSeq);
  BOOL GetDevUserGroup(const char* cszIP, const DEV_USERGROUP_ARRAY& cArray, int nSeq);
  BOOL DelDevUserGroup(const char* cszIP, const DEV_USERGROUP_ARRAY& cArray, int nSeq);
  //��Ƶ����
  BOOL StartVideo(const char* cszIP, void* hwnd, frame_callback_t frame_rgb, int nSeq);
  BOOL StopVideo(const char* cszIP, int nSeq);
  //�û���չ����
  BOOL ULUserEx(const char* cszDevAddr, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq);
  BOOL DLUserEx(const char* cszDevAddr, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq);
  BOOL DELUserEx(const char* cszDevAddr, DEV_CID* pUserIDArray, int nIDCount, int nSeq);
  //��¼��չ����
  BOOL DLRecEx(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange, int nSeq /*= -1*/);
  //�豸����
  BOOL RebootDev(const char* cszDevAddr, int nSeq /*= -1*/);
  //��ú�������������
  BOOL DevNetPro(const char* cszDevAddr, DEV_NETPRO* pNetPro , int nSeq);

protected:
  virtual void process();

public:
  friend void DevCMDNotify(const char* cszDevAddr,
      const char* cszClientAddr,
      DEV_OPER_TYPE eType,
      DEV_OPER_FLAG eFlag,
      void* pvContent,
      int nSeq,
      DEV_OPER_RESULT rt,
      void* pvClientData
                          );
  friend BOOL InternalSend(const char* cszIP, const char* pbBuf, unsigned int uLen, void* pvClientData);
  friend void CNNotify(const char* cszIP, E_CONNECT_STATUS eFlag, BOOL bServer, void* pvClientData);

private:
  void OnCNNotify(const char* cszIP, E_CONNECT_STATUS eFlag, BOOL bServer = TRUE);
  void OnRecvNotify(const char* cszIP, const char* cpbBuf, unsigned int uLen, BOOL bServer = TRUE);
  void OnRecvFromNotify(const SOCK_ADDRESS& Addr, const char* cpbBuf, unsigned int uLen);
  void OnDevCMDNotify(const char* cszDevAddr,
      const char* cszClientAddr,
      DEV_OPER_TYPE eType,
      DEV_OPER_FLAG eFlag,
      void* pvContent,
      int nSeq,
      DEV_OPER_RESULT eResult
                     );
private:
  BOOL Send(const char* cszIP, const char* pbBuf, unsigned int* puLen);
  BOOL DevCN(const char* cszIP, BOOL bFlag);
private:
  DevConnectStatus    m_pfnCN;
  void*               m_pCNContext;
  DevOperResultNotify m_pfnOpNotify;
  void*               m_pOpNotifyContext;
private:
  CNet   m_Net;
  CUDisk m_UDisk;
  CVideo m_Video;
private:
  //  CriticalSection m_syn;
};

#endif // !defined(AFX_DEVMANAGER_H__EF52C8CA_C60E_484E_B595_6B9FFF3A316A__INCLUDED_)
