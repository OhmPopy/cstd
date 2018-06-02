/*************************************************
  Description:    CHCADOConnection����һ�����ض�ADO���ݿ������				  
  Others:         
  Function List:  
    1. Close:�ر�����
	2. ExecuteNoSelectSQL:ִ�в���Ҫ���ؼ�¼���Ĳ���
	3. ExecuteSelectSQL:ִ����Ҫ���ؼ�¼���Ĳ���
	4. Open:��һ�����ݿ�����
	5.GetADOSmartpConnection:����ADO���ӵ�ָ�롣
*************************************************/

// CHCADOConnection.h: interface for the CHCADOConnection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UEADOCONNECTION_H__2DF4E582_1B67_42F3_805B_EE7152265DFB__INCLUDED_)
#define AFX_UEADOCONNECTION_H__2DF4E582_1B67_42F3_805B_EE7152265DFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CHCADO.h"
#include "CHCConnection.h"
#include "CHCADOException.h"
#include "CHCADORecordset.h"

class CHCADOConnection : public CHCConnection  
{
public:
	CHCADOConnection();
	virtual ~CHCADOConnection();

public:
	_RecordsetPtr ExecuteSelectSQL(const CString& strSQL,short nMode);
	CHCRecordsetPtr ExecuteSelectSQL(const CString& strSQL);
	BOOL ExecuteNoSelectSQL(const CString& strSQL);
	void Close();
	BOOL IsOpen();
	void Open(const CString& strCource);
	void Open(const CString& strCource, enum CursorLocationEnum eCursorLocation );
	LONG BeginTrans();
	LONG CommitTrans();
	LONG RollbackTrans();
	_ConnectionPtr* GetADOSmartpConnection();
private:
	_ConnectionPtr* m_pConn;
	_ConnectionPtr& m_pConnection;
    CString m_StrCurDBType;
	
	//	ProcMultiTypeDB m_ProMultiTypeDB;
};


#endif // !defined(AFX_UEADOCONNECTION_H__2DF4E582_1B67_42F3_805B_EE7152265DFB__INCLUDED_)
