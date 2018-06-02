/*************************************************
  Description:    CHCRecordset��һ��������࣬����һ��
				  �����ݿ��ļ��ж����ļ�¼��
				  ������������ʲô����ODBC,ADO,DAO�����ԣ�
  Others:         
  Function List:  
    1. BatchUpdate:�Լ�¼���������¡�
	2. Close:�رռ�¼��
	3. Delete:ɾ����ǰ��¼
	4. GetBool:�ӵ�ǰ��¼�ж�ȡһ��BOOLֵ
	5. GetDouble:�ӵ�ǰ��¼�ж�ȡһ��Doubleֵ
	6. GetFloat:�ӵ�ǰ��¼�ж�ȡһ��Floatֵ
	7. GetLong:�ӵ�ǰ��¼�ж�ȡһ��Longֵ
	8. GetRecordCount:��ȡ��ǰ��¼���ļ�¼��
	9. GetShort:�ӵ�ǰ��¼�ж�ȡһ��Shortֵ
	10.GeCString:�ӵ�ǰ��¼�ж�ȡһ���ַ���ֵ
	11.Get_variant_t�ӵ�ǰ��¼�ж�ȡһ������ֵ
	12.IsBOF:�ж��Ƿ�λ�ڼ�¼�����ʼ
	13.IsEOF:�ж��Ƿ�λ�ڼ�¼������ĩβ
	14.IsEmpty:�жϼ�¼���Ƿ�Ϊ��
	15.MoveFirst:�ƶ�����¼�����ʼλ��
	16.MoveLast:�ƶ�����¼�������λ��
	17.MoveNext:�ƶ�����¼���е�ǰ��¼����һ��λ��
	18.MovePrevious:�ƶ�����¼���е�ǰ��¼��ǰһ��λ��
	19.NewRecord:���¼��������һ������Ϊ�յ��¼�¼
	20.SetBool:���¼����ǰλ��дһ��BOOLֵ
	21.SetDouble:���¼����ǰλ��дһ��Doubleֵ
	22.SetFloat:���¼����ǰλ��дһ��Floatֵ
	23.SetLong:���¼����ǰλ��дһ��Longֵ
	24.SetShort:���¼����ǰλ��дһ��shortֵ
	25.SeCString:���¼����ǰλ��дһ���ַ���
	26.Set_variant_t:���¼����ǰλ��дһ������ֵ
	27.Update:���µ�ǰ��¼
*************************************************/

// CHCRecordset.h: interface for the CHCRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UERECORDSET_H__09C0D072_B717_4F35_9298_704C992A6CC2__INCLUDED_)
#define AFX_UERECORDSET_H__09C0D072_B717_4F35_9298_704C992A6CC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CHCAutoPtr.h"
class CHCRecordset  
{
public:

	CHCRecordset();
	virtual ~CHCRecordset();
public:
	//ɾ����ǰλ�ü�¼

	virtual void Delete() = 0;

	virtual void Close() = 0;
	//�Ƿ��ڼ�¼�����ʼλ��

	virtual BOOL IsBOF() const = 0;
	// ��¼���Ƿ�Ϊ��
	virtual BOOL IsEmpty() const = 0;

	//�Ƿ��ڼ�¼����ĩβ
	virtual BOOL IsEOF() const = 0;
	//�õ��ü�¼���м�¼�ĸ���

	virtual INT GetRecordCount() const= 0;
	virtual void MoveNext() = 0;
	virtual void MovePrevious() = 0;
	virtual void MoveLast() = 0;
	virtual void MoveFirst() = 0;
	virtual void SetShort(const CString& strIndex, SHORT nValue) = 0;
	virtual void SetLong(const CString& strIndex, LONG nValue) = 0;
	virtual void SetDouble(const CString& strIndex, DOUBLE dbValue) = 0;
	virtual void SetFloat(const CString& strIndex, FLOAT fValue) = 0;
	virtual void SetCString(const CString& strIndex, const CString& strValue) = 0;
	virtual void Set_variant_t(const CString& strIndex,_variant_t vr) = 0;
	virtual void SetBool(const CString& strIndex, BOOL bValue) = 0;
	virtual void AppendChunk( const CString& strIndex, BYTE* buf, int len) = 0;
	//ADO����UpdateBatch������Ӧ�˹��ܡ�

	virtual void BatchUpdate() = 0;
	virtual BOOL Update() = 0;
	
	virtual void NewRecord() = 0;
	virtual BOOL GetBool(const CString& strIndex) const= 0;
	virtual short GetShort(const CString& strIndex) const= 0;
	virtual LONG GetLong(const CString& strIndex) const= 0;
	virtual DOUBLE GetDouble(const CString& strIndex) const= 0;
	virtual FLOAT GetFloat(const CString& strIndex) const= 0;
	virtual CString GetCString(const CString& strIndex) const= 0;
	virtual _variant_t Get_variant_t(const CString& strIndex) const = 0;
	virtual LONG GetChunk( const CString& strIndex, BYTE* buf = NULL, LONG len = 0) const = 0;

};

typedef CHCAutoPtr<CHCRecordset> CHCRecordsetPtr;
#endif // !defined(AFX_UERECORDSET_H__09C0D072_B717_4F35_9298_704C992A6CC2__INCLUDED_)
