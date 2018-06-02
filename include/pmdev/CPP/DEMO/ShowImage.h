#if !defined(AFX_SHOWIMAGE_H__486EEA2D_3D06_47E0_8B74_BAE6CDB295F7__INCLUDED_)
#define AFX_SHOWIMAGE_H__486EEA2D_3D06_47E0_8B74_BAE6CDB295F7__INCLUDED_
#include "Picture.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowImage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShowImage window



class CShowImage : public CStatic
{
// Construction
public:
	CShowImage();
	
// Attributes
public:

	//�����ڴ泤��
	BOOL AllocMemory( int nMemoryLen );
	
	//�ͷ��ڴ�
	void FreeMemory();

	//ͨ��·������ͼƬ
	void SetPicPath( CString path );

	//����ͼƬ·��
	CString getpicPath();
	
	//��ȡ�Ƿ����ͼƬ
	void GetLoadState( BOOL& bState ){ bState = bLoad; };

	//�����Ƿ����ͼƬ��ʶ
	void SetLoadState( BOOL bState ){ bLoad = bState; };

	//����ˢ����ʾ
	void ResetPic();
	
public:
	CString			m_path;								//����ͼƬ��·��			
	long			m_controlID;						//����ؼ�ID
	int				m_nReadLen;							//��ȡ���ļ���С
	int				m_nMemoryLen;						//�ڴ泤��
	int				iPos;								//ͼƬ���Ǹ������е�;
	CPicture		m_Pic;								//ͼƬ��ʾ
	HGLOBAL			m_hGlobal;							//��̬�ڴ�

	//CriticalSection	m_Lock;								//��ס���ݸ���

protected:
	BOOL bLoad;											//�Ƿ������ͼƬ

//	STATICDBL pDrawInfo;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowImage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShowImage();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShowImage)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
protected:
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWIMAGE_H__486EEA2D_3D06_47E0_8B74_BAE6CDB295F7__INCLUDED_)
