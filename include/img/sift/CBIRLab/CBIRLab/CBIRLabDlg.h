
// CBIRLabDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CCBIRLabDlg �Ի���
class CCBIRLabDlg : public CDialog
{
// ����
public:
	CCBIRLabDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CBIRLAB_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	CMenu m_myMenu;
	CProgressCtrl m_progressCtrl;

	IplImage* m_imageShow;
	IplImage* m_image;

	int picWidth;
	int picHeight;

	CString imagePath;
	CString folderPath;

public:
	void ShowImage( IplImage* img, UINT ID);
	void ResizeImage(IplImage* src, IplImage* dest);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuQuit();
	afx_msg void OnMenuLoad();
	// ��ʾ�ļ���Ϣ�ı༭��
	CString m_editFileName;
	CString m_editFormat;
	CString m_editSize;
	CString m_editByte;
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuFolder();
	CString m_editFolderPath;
	afx_msg void OnMenuRetrieval();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonRetrieval();
	afx_msg void OnBnClickedButtonFolder();
};
