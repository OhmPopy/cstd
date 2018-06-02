///////////////////////////////////////////////////////////////////////////////////////
//��    ��:�豸������Ϣ��ȡ������
//��    ��:���ͻ��ȡ�豸������Ϣ
///////////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_DEVINFOCOMMAND_H__77B73CF1_7AC4_4720_9C86_BC5A752DD67A__INCLUDED_)
#define AFX_DEVINFOCOMMAND_H__77B73CF1_7AC4_4720_9C86_BC5A752DD67A__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DevCmd.h"
class DevInfo : public DevData
{
public:
  DevInfo(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevInfo(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen);    //��ȡ
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
class DevReboot : public DevData
{
public:
  DevReboot(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevReboot(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen);    //��ȡ
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
class CResetScreen : public DevData
{
public:
  CResetScreen(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  CResetScreen(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen);    //��ȡ
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
class CNetPro : public DevData
{
public:
  // ���������ݣ����촦�����
  CNetPro(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
  // ��������ʱ�����촦�����
  CNetPro(const CSock* s, DEV_NETPRO* pNetPro, int nSeq  , char* pbBuf , unsigned int& nLen);
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
#endif // !defined(AFX_DEVINFOCOMMAND_H__77B73CF1_7AC4_4720_9C86_BC5A752DD67A__INCLUDED_)

