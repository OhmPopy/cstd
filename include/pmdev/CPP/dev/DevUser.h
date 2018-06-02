// DevUserCommand.h: interface for the DevUserCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVUSERCOMMAND_H__CAB9310C_8A5D_470C_A427_F3A2125DAA5D__INCLUDED_)
#define AFX_DEVUSERCOMMAND_H__CAB9310C_8A5D_470C_A427_F3A2125DAA5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevUser : public DevData
{
public:
  DevUser(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData) ;
  DevUser(const char* cszIP, const CID& cID, int nFlag, int nSeq, char* pbBuf, unsigned int& nLen);  //�û���ȡ����
  DevUser(const char* cszIP, const CID& cID, int nSeq, char* pbBuf, unsigned int& nLen);  //�û�ɾ������
  DevUser(const char* cszIP, const DEV_USER& cUser, BOOL bAddFlag, int nSeq, char* pbBuf, unsigned int& nLen);  //�û���ӻ�ɼ�����
  DevUser(const char* cszIP, const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cLoad, char* pbBuf, unsigned int& nLen);  //�û�ʵʱ��Ӳ���\
  //  DevUser( const char* cszIP, const DEV_USER& cUser, int nSeq, char* pbBuf, unsigned int& nLen ); //ʵʱ�û��ɼ�
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserAdd(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserDel(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserDown(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserRtLoad(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserRemoteCap(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};

class DevUserEx : public DevData
{
public:
  DevUserEx(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData) ;
  DevUserEx(const char* cszIP, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq, char*& pbBuf, unsigned int& nLen); //�û������ϴ�����
  DevUserEx(const char* cszIP, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq, char*& pbBuf, unsigned int& nLen); //�û��������ز���
  DevUserEx(const char* cszIP, DEV_CID* pUserIDArray, int nIDCount, int nSeq, char*& pbBuf, unsigned int& nLen); //�û�����ɾ������
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserAdd(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserDel(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void UserDown(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);

  void UserAdd(const char* cszIP, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq, char*& pbBuf, unsigned int& nLen);
  void UserDel(const char* cszIP, DEV_CID* pUserIDArray, int nIDCount, int nSeq, char*& pbBuf, unsigned int& nLen);
  void UserDown(const char* cszIP, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq, char*& pbBuf, unsigned int& nLen);

};

#endif // !defined(AFX_DEVUSERCOMMAND_H__CAB9310C_8A5D_470C_A427_F3A2125DAA5D__INCLUDED_)
