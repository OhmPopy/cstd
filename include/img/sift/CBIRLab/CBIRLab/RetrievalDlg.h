#pragma once
#include "stdafx.h"
#include <vector>

#include "MatchSIFT.h"
#include "MySIFT.h"
#include "RetrievalResult.h"
#include "CBIRLabDlg.h"

using namespace std;
// CRetrievalDlg �Ի���

class CRetrievalDlg : public CDialog
{
	DECLARE_DYNAMIC(CRetrievalDlg)

public:
	CRetrievalDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRetrievalDlg();

public:
	IplImage* m_imageShow;//ָ��ǰ��ʾ��ͼ��Ϊ����ʾ���������ű任��ͼ��
	IplImage* m_image;//ָ������ҵ�ͼ��
	IplImage* m_currentImage;//ָ��ǰ��ʾ��ͼ��

	//��ʾcomputeResults�ķ���ֵ
	int res;
	int m_time;
	int n_pos;
	CCBIRLabDlg* m_parentDlg;

	//CMySIFT imageSift;

	int picWidth;
	int picHeight;

public:
	void ShowImage( IplImage* img, UINT ID);
	void ResizeImage(IplImage* src, IplImage* dest);
	void computeResults();

// �Ի�������
	enum { IDD = IDD_DIALOG_RETRIEVAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ���յļ������
	CString m_editResult;
	CString m_folderPath;
	CString m_imagePath;
	CString m_editIndex;
	int showIndex;
	vector<CString> vcImageDataSet;
	vector<CRetrievalResult> vcRetrievalResultSet;
	// ��ʾ��ǰ��ʾ���ļ�����
	CString m_showFileName;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonPrior();
	afx_msg void OnClose();
};
