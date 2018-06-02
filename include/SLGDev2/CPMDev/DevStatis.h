// DevStatisCommand.h: interface for the DevStatisCommand class.
//
#if !defined(AFX_DEVSTATISCOMMAND_H__BDF9604C_FC86_43CB_81D6_10F2000C0AE6__INCLUDED_)
#define AFX_DEVSTATISCOMMAND_H__BDF9604C_FC86_43CB_81D6_10F2000C0AE6__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DevCmd.h"
class DevStatis : public DevData
{
public:
  DevStatis(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData) ;
  DevStatis(const CSock* s, int nSeq, char* pbBuf, unsigned int& nLen); //��ȡ
private:
  void ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData);
};
#endif // !defined(AFX_DEVSTATISCOMMAND_H__BDF9604C_FC86_43CB_81D6_10F2000C0AE6__INCLUDED_)

