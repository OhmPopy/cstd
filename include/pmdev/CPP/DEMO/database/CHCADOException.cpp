// CHCADOException.cpp: implementation of the CHCADOException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CHCADOException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHCADOException::CHCADOException():m_Error(0)
{
	
}

CHCADOException::~CHCADOException()
{

}


/*************************************************
  Function:       Delete
  Description:    ɾ���쳣������
  Calls:          ������delete
  Called By:      �������
  Table Accessed: ��
  Table Updated:  ��
  Input:          ��
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADOException::Delete()
{
	delete this;
}


/*************************************************
  Function:       Description
  Description:    ��ȡ�Ը��쳣��������Ϣ
  Calls:          _com_error����Description
  Called By:      �������
  Table Accessed: ��
  Table Updated:  ��
  Input:          ��
  Output:         ��
  Return:         ������Ϣ�ַ���
  Others:         
*************************************************/

CString CHCADOException::Description()
{
	_bstr_t tmp = this->m_Error.Description();
	TCHAR* strtmp = tmp;
	return strtmp;
}


/*************************************************
  Function:       ReportError
  Description:    ���û������쳣��
  Calls:          ȫ�ֺ���AfxMessageBox
  Called By:      �������
  Table Accessed: ��
  Table Updated:  ��
  Input:          ��
  Output:         ��
  Return:         ��
  Others:         
*************************************************/

void CHCADOException::ReportError()
{
	AfxMessageBox(this->Description());	
}

CHCADOException::CHCADOException(HRESULT hr):m_Error(hr)
{
	
}

CHCADOException::CHCADOException(_com_error& er):m_Error(er)
{

}
