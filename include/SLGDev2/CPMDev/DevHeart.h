///////////////////////////////////////////////////////////////////////////////////////
//��    ��:������������
//��    ��:����˸�dsp�豸�˽������Ӻ�dsp�豸�˻᲻��Ϸ�������������������յ�������
//       ��Ϣ��֪ͨ�ⲿ���ⲿ���ܵ�����Ϣ�����������������
///////////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_DEVHEARTCOMMAND_H__8EDBF5F4_97D4_4F20_8CC6_A986F8B75AF2__INCLUDED_)
#define AFX_DEVHEARTCOMMAND_H__8EDBF5F4_97D4_4F20_8CC6_A986F8B75AF2__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DevCmd.h"
class DevHeart : public DevData
{
public:
  DevHeart(const CSock* addr, NCMD* pCMD, BOOL bServer, void* pvClientData);
private:
  void ProcessNCMD(const CSock* addr, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
#endif // !defined(AFX_DEVHEARTCOMMAND_H__8EDBF5F4_97D4_4F20_8CC6_A986F8B75AF2__INCLUDED_)

