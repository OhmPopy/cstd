/*************************************************************************************/
/*��    ��:�豸����������
/*��    ��:����˸�dsp�豸�˽������ӻ�Ͽ�����ʱ�����ʹ�����֪ͨ���ⲿ
           �ⲿ���ܵ�������ɻ�ȡ����״̬
/*************************************************************************************/

#if !defined(AFX_DEVREGION_H__DFDB31A7_48EF_4101_A0C8_63306CFFAB35__INCLUDED_)
#define AFX_DEVREGION_H__DFDB31A7_48EF_4101_A0C8_63306CFFAB35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevCmd.h"

class DevRgn : public DevData
{
public:
  DevRgn(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  DevRgn(const char* cszIP, const char* cszGroupIP, int nSeq, char* pbBuf, unsigned int& nLen);//����
  DevRgn(const char* cszIP, int nSeq, char* pbBuf, unsigned int& nLen);//��ȡ
private:
  void ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void SetRgn(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
  void GetRgn(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData);
};



#endif // !defined(AFX_DEVCONNECT_H__DFDB31A7_48EF_4101_A0C8_63306CFFAB35__INCLUDED_)
