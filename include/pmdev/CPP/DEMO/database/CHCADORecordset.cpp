// CHCADORecordset.cpp: implementation of the CHCADORecordset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CHCADORecordset.h"
#include "CHCADOException.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHCADORecordset::CHCADORecordset()
{
	this->m_pRecordset = NULL;
}

CHCADORecordset::~CHCADORecordset()
{
	Close();
}

/*************************************************
  Function:       GetRecordCount
  Description:    ��ȡ��ǰ��¼���ļ�¼��Ŀ
  Calls:          
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          ��
  Output:         ��
  Return:         intֵ����¼��Ŀ��
  Others:         
*************************************************/

int CHCADORecordset::GetRecordCount() const
{
	ASSERT(m_pRecordset != NULL);
	long lcount = 0;
	try
	{
		m_pRecordset->get_RecordCount(&lcount);
		return (int)lcount;
	}
	catch(_com_error e)
	{
		throw new CHCADOException(e);
	} 
}


/*************************************************
  Function:       Close
  Description:    �رյ�ǰ��¼��
  Calls:          ADO����Close
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��ǰ��¼��
  Input:          ��
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::Close()
{
	try
	{
		if (m_pRecordset != NULL && m_pRecordset->State != adStateClosed)
		{
			SUCCEEDED(m_pRecordset->Close());
			m_pRecordset = NULL;
		}
	}
	catch (_com_error& e)
	{
		throw new CHCADOException(e);
	}
}


/*************************************************
  Function:       IsBOF
  Description:    �ж��Ƿ�λ�ڼ�¼�����ʼ
  Calls:          
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          ��
  Output:         ��
  Return:         BOOLֵ
  Others:         
*************************************************/

BOOL CHCADORecordset::IsBOF() const
{
	ASSERT(m_pRecordset != NULL);
	return m_pRecordset->BOF;
}

/*************************************************
  Function:       IsEOF
  Description:    �ж��Ƿ�λ�ڼ�¼�����ʼ
  Calls:          
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          ��
  Output:         ��
  Return:         BOOLֵ
  Others:         
*************************************************/

BOOL CHCADORecordset::IsEOF() const
{
	ASSERT(m_pRecordset != NULL);
	return m_pRecordset->adoEOF;
}

/*************************************************
  Function:       IsEmpty
  Description:    �жϼ�¼���Ƿ�Ϊ��
  Calls:          
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          ��
  Output:         ��
  Return:         BOOLֵ
  Others:         
*************************************************/

BOOL CHCADORecordset:: IsEmpty() const
{
	return IsEOF() && IsBOF();
}


/*************************************************
  Function:       MoveFirst
  Description:    �ƶ�����¼�����ʼ
  Calls:          ADO����MoveFirst
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          ��
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::MoveFirst()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			SUCCEEDED(m_pRecordset->MoveFirst());
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	} 
}


/*************************************************
  Function:       MovePrevious
  Description:    �ƶ�����¼�����ʼ
  Calls:          ADO����MovePrevious 
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          ��
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::MovePrevious()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			SUCCEEDED(m_pRecordset->MovePrevious());
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	} 
}


/*************************************************
  Function:       MoveNext
  Description:    �ƶ�����ǰ��¼����һ��λ��
  Calls:          ADO����MoveNext
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          ��
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::MoveNext()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			SUCCEEDED(m_pRecordset->MoveNext());
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}


/*************************************************
  Function:       MoveLast
  Description:    �ƶ�����¼�������
  Calls:          ADO����MoveLast
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          ��
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::MoveLast()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			SUCCEEDED(m_pRecordset->MoveLast());
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	} 
}


/*************************************************
  Function:       Update
  Description:    �Ե�ǰ��¼������������
  Calls:          ADO����Update
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��ǰ��¼��
  Input:          ��
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

BOOL CHCADORecordset::Update()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			HRESULT hr = m_pRecordset->Update();
			if (SUCCEEDED(hr))
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	} 
	return FALSE;
}


/*************************************************
  Function:       BatchUpdate
  Description:    �Ե�ǰ��¼������������
  Calls:          ADO����UpdateBatch
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��ǰ��¼��
  Input:          ��
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::BatchUpdate()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->UpdateBatch(adAffectAll);//Ĭ�Ϸ�ʽ
	}
	catch (_com_error e) 
	{
		throw new CHCADOException(e);
	}
}

CHCADORecordset::CHCADORecordset(_RecordsetPtr m_pRecordset)
{
	this->m_pRecordset = m_pRecordset;
}


/*************************************************
  Function:       SetShort(const CString &strIndex, SHORT nValue)
  Description:    ��ǰ��¼strIndex��������дSHORTֵ
  Calls:          ADO����PutCollect
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��ǰ��¼��
  Input:          const CString &strIndex:����
				  SHORT nValue:�����õ�ֵ
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::SetShort(const CString &strIndex, SHORT nValue)
{
	ASSERT( m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCollect(_variant_t(strIndex),_variant_t(nValue));
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}


/*************************************************
  Function:       SetLong(const CString &strIndex, LONG nValue)
  Description:    ��ǰ��¼strIndex��������дLONGֵ
  Calls:          ADO����PutCollect
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��ǰ��¼��
  Input:          const CString &strIndex:����
				  LONG nValue:�����õ�ֵ
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::SetLong(const CString &strIndex, LONG nValue)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCollect(_variant_t(strIndex),_variant_t(nValue));
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}


/*************************************************
  Function:       SetDouble(const CString &strIndex, DOUBLE dbValue)
  Description:    ��ǰ��¼strIndex��������дDoubleֵ
  Calls:          ADO����PutCollect
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��ǰ��¼��
  Input:          const CString &strIndex:����
				  DOUBLE dbValue:�����õ�ֵ
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::SetDouble(const CString &strIndex, DOUBLE dbValue)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCollect(_variant_t(strIndex),_variant_t(dbValue));
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}


/*************************************************
  Function:       SetFloat(const CString &strIndex, FLOAT fValue)
  Description:    ��ǰ��¼strIndex��������дFLOATֵ
  Calls:          ADO����PutCollect
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��ǰ��¼��
  Input:          const CString &strIndex:����
				  FLOAT fValue:�����õ�ֵ
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::SetFloat(const CString &strIndex, FLOAT fValue)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCollect(_variant_t(strIndex),_variant_t(fValue));
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}


/*************************************************
  Function:       SeCString(const CString &strIndex, const CString &strValue)
  Description:    ��ǰ��¼strIndex��������д�ַ���
  Calls:          ADO����PutCollect
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��ǰ��¼��
  Input:          const CString &strIndex:����
				  const CString &strValue:�����õ�ֵ
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::SetCString(const CString &strIndex, const CString &strValue)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCollect(_variant_t(strIndex),_variant_t(strValue));
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}


/*************************************************
  Function:       SetBool(const CString &strIndex, BOOL bValue)
  Description:    ��ǰ��¼strIndex��������дBOOLֵ
  Calls:          ADO����PutCollect
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��ǰ��¼��
  Input:          const CString &strIndex:����
				  BOOL bValue:�����õ�ֵ
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::SetBool(const CString &strIndex, BOOL bValue)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCollect(_variant_t(strIndex),_variant_t(bValue));
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}


/*************************************************
  Function:       NewRecord
  Description:    ��ǰ��¼������һ���հ׼�¼��������
				  �Ժ�ĳ��ʱ�����øü�¼������
  Calls:          ADO����Supports��AddNew
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��ǰ��¼��
  Input:          ��
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::NewRecord()
{
	ASSERT( m_pRecordset != NULL);
	try
	{
		if (m_pRecordset->Supports(adAddNew))
		{
			m_pRecordset->AddNew();//ֻ����һ���¼�¼��û�����ֵ��
		}
		else
		{
			AfxMessageBox(_T("���ܲ���"));
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}


/*************************************************
  Function:       GetShort(const CString &strIndex)
  Description:    �ӵ�ǰ��¼�õ�һ��BOOLֵ
  Calls:          ADO����GetCollect
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          const CString &strIndex:����Ҫȡ����
  Output:         ��
  Return:         Shortֵ
  Others:         
*************************************************/

short CHCADORecordset::GetShort(const CString &strIndex) const
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		short sdefault = 0;
		_variant_t var;
		var = m_pRecordset->GetCollect(_variant_t(strIndex));
		if (var.vt == VT_NULL)
		{
			return sdefault;
		}
		return short(var);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return 0;
}


/*************************************************
  Function:       GetLong(const CString &strIndex)
  Description:    �ӵ�ǰ��¼�õ�һ��BOOLֵ
  Calls:          ADO����GetCollect
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          const CString &strIndex:����Ҫȡ����
  Output:         ��
  Return:         Longֵ
  Others:         
*************************************************/

LONG CHCADORecordset::GetLong(const CString &strIndex) const
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		LONG ldefault = 0;
		_variant_t var;
		var = m_pRecordset->GetCollect(_variant_t(strIndex));
		if (var.vt == VT_NULL)
		{
			return ldefault;
		}
		return long(var);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return 0;
}


/*************************************************
  Function:       GetDouble(const CString &strIndex)
  Description:    �ӵ�ǰ��¼�õ�һ��BOOLֵ
  Calls:          ADO����GetCollect
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          const CString &strIndex:����Ҫȡ����
  Output:         ��
  Return:         Doubleֵ
  Others:         
*************************************************/

DOUBLE CHCADORecordset::GetDouble(const CString &strIndex) const
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		double ddefault = 0;
		_variant_t var;
		var = m_pRecordset->GetCollect(_variant_t(strIndex));
		if (var.vt == VT_NULL)
		{
			return ddefault;
		}
		return double(var);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return 0;
}


/*************************************************
  Function:       GetFloat(const CString &strIndex)
  Description:    �ӵ�ǰ��¼�õ�һ��BOOLֵ
  Calls:          ADO����GetCollect
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          const CString &strIndex:����Ҫȡ����
  Output:         ��
  Return:         Floatֵ
  Others:         
*************************************************/

FLOAT CHCADORecordset::GetFloat(const CString &strIndex) const
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		float fdefault = 0;
		_variant_t var;
		var = m_pRecordset->GetCollect(_variant_t(strIndex));
		if (var.vt == VT_NULL)
		{
			return fdefault;
		}
		return float(var);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return 0;
}


/*************************************************
  Function:       GeCString(const CString &strIndex)
  Description:    �ӵ�ǰ��¼�õ�һ��BOOLֵ
  Calls:          ADO����GetCollect
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          const CString &strIndex:����Ҫȡ����
  Output:         ��
  Return:         �ַ���
  Others:         
*************************************************/

CString CHCADORecordset::GetCString(const CString &strIndex) const
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		_variant_t var;
		CString strdefault;
		var = m_pRecordset->GetCollect(_variant_t(strIndex));
		if (var.vt == VT_NULL)
		{
			return strdefault;
		}
		return (TCHAR*)_bstr_t(var);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return _T("");
}


/*************************************************
  Function:       Delete
  Description:    ɾ����ǰ��¼
  Calls:          ADO����Delete��Update
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��ǰ��¼��
  Input:          ��
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::Delete()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->Delete(adAffectCurrent);//Ĭ�ϡ���ɾ����ǰ��¼��
		m_pRecordset->Update();
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}


/*************************************************
  Function:       GetBool(const CString &strIndex)
  Description:    �ӵ�ǰ��¼�õ�һ��BOOLֵ
  Calls:          ADO����GetCollect
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          const CString &strIndex:����Ҫȡ����
  Output:         ��
  Return:         BOOLֵ
  Others:         
*************************************************/

BOOL CHCADORecordset::GetBool(const CString &strIndex) const
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		BOOL bdefault = FALSE;
		_variant_t var;
		var = m_pRecordset->GetCollect(_variant_t(strIndex));
		if (var.vt == VT_NULL)
		{
			return bdefault;
		}
		return BOOL(var);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return false;
}

/*************************************************
  Function:       Get_variant_t(const CString &strIndex)
  Description:    �ӵ�ǰ��¼�õ�һ��_variant_tֵ
  Calls:          ADO����GetCollect
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��
  Input:          const CString &strIndex:����Ҫȡ����
  Output:         ��
  Return:         _variant_tֵ
  Others:         
*************************************************/

_variant_t CHCADORecordset::Get_variant_t(const CString& strIndex) const
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		_variant_t vdefault;
		_variant_t var;
		var = m_pRecordset->GetCollect(_variant_t(strIndex));
		if (var.vt == VT_NULL)
		{
			return vdefault;
		}
		return var;
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return false;
}

/*************************************************
  Function:       Set_variant_t(const CString& strIndex,_variant_t vr)
  Description:    ��ǰ��¼strIndex��������д_variant_tֵ
  Calls:          ADO����PutCollect
  Called By:      �������
  Table Accessed: ��ǰ��¼��
  Table Updated:  ��ǰ��¼��
  Input:          const CString &strIndex:����
				  _variant_t vr:�����õ�ֵ
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADORecordset::Set_variant_t(const CString& strIndex,_variant_t vr)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCollect(_variant_t(strIndex),vr);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}

LONG CHCADORecordset::GetChunk( const CString& strIndex, BYTE* buf /*= NULL*/, LONG len /*= 0*/) const
{
	ASSERT(m_pRecordset != NULL);
	LONG ret = 0;
	try
	{
		if ( buf == NULL || len == 0 )
		{
			ret = m_pRecordset->GetFields()->GetItem( _variant_t(strIndex) )->ActualSize;
		}
		else
		{
			_variant_t varBLOB;	
			varBLOB = m_pRecordset->GetFields()->GetItem( _variant_t(strIndex) )->GetChunk(len);		
			if(varBLOB.vt == (VT_ARRAY | VT_UI1)) ///�ж����������Ƿ���			
			{
				char *pBuf = NULL;
				SafeArrayAccessData(varBLOB.parray,(void **)&pBuf); 
				memcpy( buf, pBuf, len);
				//�õ�ָ�����ݵ�ָ�� 
			    /*****���������ǿ��Զ�pBuf�е����ݽ��д���*****/	
			    SafeArrayUnaccessData (varBLOB.parray);	
				ret = len;
			}
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return ret;

}

void CHCADORecordset::AppendChunk( const CString& strIndex, BYTE* buf, int len)
{
	ASSERT(m_pRecordset != NULL);
	if ( NULL == buf || len <= 0 ) return;
	try
	{
		VARIANT varBLOB;
		SAFEARRAY *psa;
		SAFEARRAYBOUND rgsabound[1];
		rgsabound[0].lLbound = 0;		
		rgsabound[0].cElements = len;	
		psa = SafeArrayCreate(VT_UI1, 1, rgsabound); ///����SAFEARRAY����		
		for (long i = 0; i < (long)len; i++)		
			SafeArrayPutElement (psa, &i, buf++); ///��pBufָ��Ķ��������ݱ��浽SAFEARRAY����psa�� 		
		varBLOB.vt = VT_ARRAY | VT_UI1;///��varBLOB����������ΪBYTE���͵�����	
		varBLOB.parray = psa; ///ΪvarBLOB������ֵ 
		m_pRecordset->GetFields()->GetItem( _variant_t(strIndex) )->AppendChunk(varBLOB); ///����BLOB���͵�����
		SafeArrayDestroy(psa);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}