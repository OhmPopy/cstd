// CHCADOConnection.cpp: implementation of the CHCADOConnection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CHCADOConnection.h"
#include <io.h> 
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHCADOConnection::CHCADOConnection():m_pConnection(*m_pConn),m_pConn(new _ConnectionPtr)

{
	//m_pConnection = *m_pConn;
	//m_pConn = new _ConnectionPtr;
}

CHCADOConnection::~CHCADOConnection()
{
	this->Close();
}

void CHCADOConnection::Open(const CString &strCource)
{
	
	HRESULT hr = 0;
	try
	{
		//û�ж�д�����������ļ����ж�д����
		if ( _taccess( strCource, 6 ) != 0 ) //06	Read and write permission.
		{
			_tchmod( strCource, _S_IREAD | _S_IWRITE );
		}

		CoInitialize(NULL); 
		hr = this->m_pConnection.CreateInstance("ADODB.Connection"/*__uuidof(Connection)*/);
		if (SUCCEEDED(hr))
		{
			CString str = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");
			str += strCource;
			hr = m_pConnection->Open(_bstr_t( str ),"","",-1);
		//	if ()
			/*******************************************
			HRESULT Connection15::Open ( _bstr_t ConnectionString, _bstr_t UserID, _bstr_t Password, long Options )
			ConnectionStringΪ�����ִ�,UserID���û���, Password�ǵ�½����,Options������ѡ��,����ָ��Connection��������ݵĸ������Ȩ,
			Options���������¼�������:
			adModeUnknown:ȱʡ����ǰ�����Ȩδ����
			adModeRead:ֻ��
			adModeWrite:ֻд
			adModeReadWrite:���Զ�д
			adModeShareDenyRead:��ֹ����Connection�����Զ�Ȩ�޴�����
			adModeShareDenyWrite:��ֹ����Connection������дȨ�޴�����
			adModeShareExclusive:��ֹ����Connection�����Զ�дȨ�޴�����
			adModeShareDenyNone:��ֹ����Connection�������κ�Ȩ�޴�����
			
			  ���Ǹ���һЩ���õ����ӷ�ʽ����Ҳο�:
			  (1)ͨ��JET���ݿ������ACCESS2000���ݿ������
			  
				m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\\test.mdb","","",adModeUnknown);
				
			**********************************************/
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}

void CHCADOConnection::Open(const CString &strCource, enum CursorLocationEnum eCursorLocation )
{
	
	HRESULT hr = 0;
	try
	{
		//û�ж�д�����������ļ����ж�д����
		if ( _taccess( strCource, 6 ) != 0 ) //06	Read and write permission.
		{
			_tchmod( strCource, _S_IREAD | _S_IWRITE );
		}

		CoInitialize(NULL); 

		hr = this->m_pConnection.CreateInstance("ADODB.Connection"/*__uuidof(Connection)*/);
		if(adStateConnecting!=( m_pConnection->GetState()& adStateConnecting))
		{
			m_pConnection->CursorLocation = eCursorLocation ;
		}
		if (SUCCEEDED(hr))
		{
			CString str = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");
			str += strCource;
			hr = m_pConnection->Open(_bstr_t( str ),"","",-1);
			/*******************************************
			HRESULT Connection15::Open ( _bstr_t ConnectionString, _bstr_t UserID, _bstr_t Password, long Options )
			ConnectionStringΪ�����ִ�,UserID���û���, Password�ǵ�½����,Options������ѡ��,����ָ��Connection��������ݵĸ������Ȩ,
			Options���������¼�������:
			adModeUnknown:ȱʡ����ǰ�����Ȩδ����
			adModeRead:ֻ��
			adModeWrite:ֻд
			adModeReadWrite:���Զ�д
			adModeShareDenyRead:��ֹ����Connection�����Զ�Ȩ�޴�����
			adModeShareDenyWrite:��ֹ����Connection������дȨ�޴�����
			adModeShareExclusive:��ֹ����Connection�����Զ�дȨ�޴�����
			adModeShareDenyNone:��ֹ����Connection�������κ�Ȩ�޴�����
			
			  ���Ǹ���һЩ���õ����ӷ�ʽ����Ҳο�:
			  (1)ͨ��JET���ݿ������ACCESS2000���ݿ������
			  
				m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\\test.mdb","","",adModeUnknown);
				
			**********************************************/
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
//		AfxMessageBox( e.Description() ) ;
	}
}

/*************************************************
Function:       Close
Description:    �ر���ADO���ݿ������
Calls:          ADO����Close
Called By:      �������
Table Accessed: ��
Table Updated:  ��
Input:          ��
Output:         ��
Return:         ��
Others:         
*************************************************/

void CHCADOConnection::Close()
{
	try
	{
		if( m_pConnection != NULL && m_pConnection->State != adStateClosed )
		{
			m_pConnection->Close();
			CoUninitialize();
		}
		m_pConnection = NULL; 
	//	delete m_pConn;
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}


BOOL CHCADOConnection::IsOpen()
{
	if( m_pConnection == NULL || m_pConnection->State == adStateClosed )
	{
		return FALSE;
	}
	return TRUE;
}


/*************************************************
Function:       ExecuteNoSelectSQL
Description:    ִ�в���Ҫ���ؼ�¼���Ĳ���
Calls:          ADO����Execute
Called By:      �������
Table Accessed: ��ǰ�򿪱�
Table Updated:  ��ǰ�򿪱�
Input:          ��
Output:         ��
Return:         ��
Others:         
*************************************************/

BOOL CHCADOConnection::ExecuteNoSelectSQL(const CString &strSQL)
{
	try
	{
		_variant_t RecordsAffected;

#if 0
		//afxDump<<strSQL.c_str()<<_T("\n");
#endif

		m_pConnection->Execute(_bstr_t(strSQL),NULL,adCmdText);
		ULONG iInfluence = (long)RecordsAffected;	
		
		return TRUE;	
	}
	catch (_com_error e)
	{		
		throw new CHCADOException(e);
		return FALSE;			
	}	
}


/*************************************************
  Function:       ExecuteSelectSQL
  Description:    ִ����Ҫ���ؼ�¼���Ĳ���
  Calls:          ADO����CreateInstance��Open
  Called By:      �������
  Table Accessed: ��ǰ�򿪱�
  Table Updated:  ��ǰ�򿪱�
  Input:          ��
  Output:         ��
  Return:         
  Others:         
*************************************************/

CHCRecordsetPtr CHCADOConnection::ExecuteSelectSQL(const CString &strSQL)
{
	try
	{	
		_RecordsetPtr m_pRecordset;
		//DEBUG_INFO1(_T("%s"), strSQL.c_str());
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(_bstr_t(strSQL)
							,(IDispatch*)m_pConnection
							,adOpenDynamic
							,adLockOptimistic
							,adCmdText);
		return CHCRecordsetPtr(new CHCADORecordset(m_pRecordset));
	}
	catch (_com_error e)
	{	
		throw new CHCADOException(e);
	}

	return CHCRecordsetPtr( NULL );//��ʱ���� 
}

_RecordsetPtr CHCADOConnection::ExecuteSelectSQL(const CString &strSQL,short nMode)
{
	try
	{	
		_RecordsetPtr m_pRecordset;
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(_bstr_t(strSQL)
			,(IDispatch*)m_pConnection
			,adOpenDynamic
			,adLockOptimistic
			,adCmdText);
		
		return m_pRecordset;
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return NULL;//��ʱ���� 
}


/*************************************************
  Function:       GetADOSmartpConnection
  Description:    ����ADO����ָ�롣
  Calls:          
  Called By:      �������
  Table Accessed: ��ǰ�򿪱�
  Table Updated:  ��ǰ�򿪱�
  Input:          ��
  Output:         ��
  Return:         _ConnectionPtr*
  Others:         
*************************************************/

_ConnectionPtr* CHCADOConnection::GetADOSmartpConnection()
{
	return m_pConn;
}


/*************************************************
  Function:       BeginTrans
  Description:    ����������
  Calls:          
  Called By:      �������
  Input:          ��
  Output:         ��
  Return:         LONG
  Others:         
*************************************************/

LONG CHCADOConnection::BeginTrans()
{
	LONG lResult = 0;
	try
	{
		m_pConnection->BeginTrans();
	}
	catch ( _com_error e ) 
	{
		throw new CHCADOException( e );
	}
	return lResult;
}


/*************************************************
  Function:       BeginTrans
  Description:    ȷ�ϵ�ǰ����
  Calls:          
  Called By:      �������
  Input:          ��
  Output:         ��
  Return:         LONG
  Others:         
*************************************************/

LONG CHCADOConnection::CommitTrans()
{
	LONG lResult = 0;
	try
	{
		m_pConnection->CommitTrans();
	}
	catch ( _com_error e ) 
	{
		throw new CHCADOException( e );
	}
	return lResult;
}



/*************************************************
  Function:       BeginTrans
  Description:    ������ǰ����
  Calls:          
  Called By:      �������
  Input:          ��
  Output:         ��
  Return:         LONG
  Others:         
*************************************************/

LONG CHCADOConnection::RollbackTrans()
{
	LONG lResult = 0;
	try
	{
		m_pConnection->RollbackTrans();
	}
	catch ( _com_error e ) 
	{
		throw new CHCADOException( e );
	}
	return lResult;
}