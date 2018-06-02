/*************************************************
  Description:    CHCADOException��װADO�е�_com_error�쳣
  Others:         
  Function List:  
    1. Delete:ɾ�����׳����쳣�����Լ���
	2. Description:��ȡ�쳣��������Ϣ
	3. ReportError:���û���������Ծ���Ի������ʽ
*************************************************/


// CHCADOException.h: interface for the CHCADOException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UEADOEXCEPTION_H__08C141D4_7CFB_4323_B4EC_9A433E5C8F96__INCLUDED_)
#define AFX_UEADOEXCEPTION_H__08C141D4_7CFB_4323_B4EC_9A433E5C8F96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CHCException.h"


class CHCADOException : public CHCException  
{
public:

	CHCADOException();
	virtual ~CHCADOException();
public:
	CHCADOException( _com_error& er );
	CHCADOException( HRESULT hr );
	void ReportError();
	CString Description();
	void Delete();


private:
	_com_error m_Error;
};

#endif // !defined(AFX_UEADOEXCEPTION_H__08C141D4_7CFB_4323_B4EC_9A433E5C8F96__INCLUDED_)
