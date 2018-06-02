// RetrievalDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CBIRLab.h"
#include "RetrievalDlg.h"
#include "MatchSIFT.h"
#include "MySIFT.h"
#include "RetrievalResult.h"

// CRetrievalDlg �Ի���

IMPLEMENT_DYNAMIC(CRetrievalDlg, CDialog)

CRetrievalDlg::CRetrievalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRetrievalDlg::IDD, pParent)
	, m_editResult(_T(""))
	, m_showFileName(_T(""))
	, m_editIndex(_T(""))
{
	showIndex = 0;
	m_currentImage = NULL;
	m_imageShow = NULL;
	res = 0;
	n_pos = 0;
}

CRetrievalDlg::~CRetrievalDlg()
{
}

void CRetrievalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RESULT, m_editResult);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_showFileName);
	DDX_Text(pDX, IDC_EDIT_INDEX, m_editIndex);
}


BEGIN_MESSAGE_MAP(CRetrievalDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_NEXT, CRetrievalDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PRIOR, CRetrievalDlg::OnBnClickedButtonPrior)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CRetrievalDlg ��Ϣ�������

BOOL CRetrievalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString str;
	
	//�õ�ͼƬ�ռ�ĳߴ�
	// �����ʾ�ؼ��� DC
	CDC* pDC = GetDlgItem( IDC_PICTURE_RES_CTL ) ->GetDC();
	// ��ȡ HDC(�豸���) �����л�ͼ����		
	HDC hDC = pDC ->GetSafeHdc();				
	CRect rect;
	GetDlgItem(IDC_PICTURE_RES_CTL) ->GetClientRect( &rect );
	// ���ͼƬ�ؼ��Ŀ�͸�
	picWidth = rect.right - rect.left;			
	picHeight = rect.bottom - rect.top;

	ReleaseDC( pDC );

	//��ʼ��m_imageShow
	CvSize ImgSize;
	ImgSize.height = picHeight;
	ImgSize.width = picWidth;
	m_imageShow = cvCreateImage( ImgSize, IMG_DEPTH_8U, IMAGE_CHANNELS );
	cvSet(m_imageShow, cvScalarAll(255));

	//��ʼ��������ͼ���SIFT����
	//imageSift.initialize(m_imagePath);

	if (res == 0)
	{
		str.Format("�ļ�����û�и�ʽ��ȷ��ͼƬ.\r\n");
		m_editResult += str;
	}
	else if (res == -1)
	{
		str.Format("��ƥ����.\r\n");
		m_editResult += str;
	}
	else if(m_currentImage)
	{
		ResizeImage( m_currentImage, m_imageShow );
		ShowImage( m_imageShow, IDC_PICTURE_RES_CTL);

		m_showFileName = (vcRetrievalResultSet[showIndex]).imagePath;
		m_editIndex.Format("��ǰ��%d/%d��", showIndex+1, vcRetrievalResultSet.size());
	}

	str.Format("\r\n���μ�������ʱ�䣺%d��.\r\n", m_time/1000);
	m_editResult += str;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CRetrievalDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	// �ػ�Ի���
	CDialog::OnPaint();
	// ����windows���ڣ�������ⲽ���ã�ͼƬ��ʾ�����������
	CDialog::UpdateWindow();
	// �ػ�ͼƬ����
	ShowImage( m_imageShow, IDC_PICTURE_RES_CTL);
}

void CRetrievalDlg::ShowImage( IplImage* img, UINT ID)
{
	if (img == NULL)
	{
		return;
	}
	// �����ʾ�ؼ��� DC
	CDC* pDC = GetDlgItem( ID ) ->GetDC();
	// ��ȡ HDC(�豸���) �����л�ͼ����		
	HDC hDC = pDC ->GetSafeHdc();				

	CRect rect;
	GetDlgItem(ID) ->GetClientRect( &rect );
	// ���ͼƬ�ؼ��Ŀ�͸�
	int rw = rect.right - rect.left;			
	int rh = rect.bottom - rect.top;
	// ��ȡͼƬ�Ŀ�͸�
	int iw = img->width;	
	int ih = img->height;
	// ʹͼƬ����ʾλ�������ڿؼ�������
	int tx = (int)(rw - iw)/2;	
	int ty = (int)(rh - ih)/2;
	SetRect( rect, tx, ty, tx+iw, ty+ih );
	// ����ͼƬ
	CvvImage cimg;
	cimg.CopyOf( img );
	// ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������	
	cimg.DrawToHDC( hDC, &rect );	

	ReleaseDC( pDC );
}

//��ͼƬsrc���ŵ�dest�Ĵ�С��������ţ�С���򲻱�
void CRetrievalDlg::ResizeImage(IplImage* src, IplImage* dest)
{
	if (!src || !dest)
	{
		return;
	}
	// ��ȡͼƬ�Ŀ�͸�
	int w = src->width;
	int h = src->height;
	float scale1 = (float)w/(float)picWidth;
	float scale2 = (float)h/(float)picHeight;

	// �ҳ���͸������ű����Ľϴ�ֵ��
	// ���㽫ͼƬ���ŵ�TheImage��������ı�������
	float scale = (scale1 > scale2) ? scale1: scale2;

	// ���ź�ͼƬ�Ŀ�͸�
	int nw, nh;
	if(scale > 1.0f)
	{
		nw = (int)( w/scale );
		nh = (int)( h/scale );
	}
	else
	{
		nw = w;
		nh = h;
	}

	// Ϊ�˽����ź��ͼƬ���� TheImage �����в�λ�������ͼƬ�� TheImage ���Ͻǵ���������ֵ
	int tlx = (nw > picWidth)? 0: (int)(picWidth-nw)/2;
	int tly = (picHeight > nh)? (int)(picHeight-nh)/2: 0;

	// ���� TheImage �� ROI ������������ͼƬ img
	cvSetImageROI( dest, cvRect( tlx, tly, nw, nh) );

	// ��ͼƬ img �������ţ������뵽 TheImage ��
	cvResize( src, dest );

	// ���� TheImage �� ROI ׼��������һ��ͼƬ
	cvResetImageROI( dest );
}

void CRetrievalDlg::computeResults()
{
	CFileFind tempFind;
	BOOL bFound; //�ж��Ƿ�ɹ��ҵ��ļ�
	bFound = tempFind.FindFile(m_folderPath + "*.*");   //�޸�" "�����ݸ��޶������ļ�����
	int index = 0;
	CString strTmp, str;

	//���������ļ�
	while(bFound)
	{
		//��һ��ִ��FindNextFile��ѡ�񵽵�һ���ļ����Ժ�ִ��Ϊѡ����һ���ļ�
		bFound = tempFind.FindNextFile();

		//�ҵ��Ĳ����ļ��У�Ҳ���Ƿ����ϲ��Ŀ¼
		if(!tempFind.IsDots() && !tempFind.IsDirectory())
		{
			//�����ļ�����������׺��
			strTmp = tempFind.GetFilePath();
			//�����ļ���ӵ�������
			CString strExtention = PathFindExtensionA(strTmp);
			strExtention.MakeLower();
			if (strExtention.Compare(".png") == 0 || strExtention.Compare(".jpg") == 0
				|| strExtention.Compare(".jpeg") == 0 || strExtention.Compare(".bmp") == 0 
				|| strExtention.Compare(".gif") == 0)
			{
				vcImageDataSet.push_back(strTmp);
			}
		}
	}
	tempFind.Close();

	if (vcImageDataSet.size() == 0)
	{
		//�ļ���Ϊ��
		res = 0;
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//����SIFT������ƥ����
	CMatchSIFT matchImage;
	index = 1;
	for(vector<CString>::iterator it = vcImageDataSet.begin(); it != vcImageDataSet.end(); ++it, ++index)
	{
		CMySIFT tmpSift1, tmpSift2;
		if (!tmpSift1.initialize(m_imagePath) || !tmpSift2.initialize(*it, true))
		{
			AfxMessageBox("����ͼ��"+(*it)+"����");
			continue;
		}
		CRetrievalResult tmpResult;
		matchImage.initialize(&tmpSift1, &tmpSift2);
		tmpResult.initialize(*it);
		if (matchImage.matchTwoImage(tmpResult))
		{
			//���һ���������������ͼ���·�����ͽ��������
			vcRetrievalResultSet.push_back(tmpResult);
		}
		n_pos = (int)((index*100)/vcImageDataSet.size());
		m_parentDlg->m_progressCtrl.SetPos(n_pos);
		m_parentDlg->UpdateData(FALSE);
	}

	m_editResult.Format("��%d��ͼƬ�й���������%d���������\r\n\r\n", 
		vcImageDataSet.size(), vcRetrievalResultSet.size());

	if (vcRetrievalResultSet.size() == 0)
	{
		//��ƥ����
		res = -1;
		return;
	}

	index = 1;
  {for (vector<CRetrievalResult>::iterator it = vcRetrievalResultSet.begin(); 
		it != vcRetrievalResultSet.end(); ++it, ++index)
	{
		if ((*it).vpRegions.size() >= 0)
		{
			str.Format("%d,  ͼƬ%s,  ��������:  (%d, %d), (%d, %d).\r\n",
				index, PathFindFileName((*it).imagePath), (((*it).vpRegions[0]).rectTopLeft).x, (((*it).vpRegions[0]).rectTopLeft).y, 
				(((*it).vpRegions[0]).rectBottomRight).x, (((*it).vpRegions[0]).rectBottomRight).y);
			m_editResult += str;
		}
  }}

	// �����ݽ���ж�ȡһ���Ϸ���ͼ�������ļ�
	while(showIndex < vcRetrievalResultSet.size())
	{
		m_currentImage = cvLoadImage(vcRetrievalResultSet[showIndex].imagePath, 1);
		if(m_currentImage)
		{
			//����ǰ����ʾͼ������м�����������ʶ����
			for (int i = 0; i < vcRetrievalResultSet[showIndex].regionCount; i++)
			{
				cvRectangle(m_currentImage, ((vcRetrievalResultSet[showIndex]).vpRegions[i]).rectTopLeft, 
					((vcRetrievalResultSet[showIndex]).vpRegions[i]).rectBottomRight, CC_RGB(255,0,255), 2, 8, 0);

				/*��ע���ı�������
				cvLine(m_currentImage, ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[0], ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[1], cvScalarAll(0), 2, 8, 0);
				cvLine(m_currentImage, ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[1], ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[2], cvScalarAll(0), 2, 8, 0);
				cvLine(m_currentImage, ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[2], ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[3], cvScalarAll(0), 1, 8, 0);
				cvLine(m_currentImage, ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[3], ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[0], cvScalarAll(0), 1, 8, 0);
				*/
			}
			break;
		}
		else
			showIndex++;
	}

	res = 1;
	return;
}
void CRetrievalDlg::OnBnClickedButtonNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	showIndex++;
	if (showIndex >= vcRetrievalResultSet.size())
	{
		AfxMessageBox("�Ѿ��������һ��ͼ��");
		showIndex--;
		return;
	}
	m_currentImage = cvLoadImage(vcRetrievalResultSet[showIndex].imagePath, 1);
	if(m_currentImage)
	{
		//����ǰ����ʾͼ������м�����������ʶ����
		for (int i = 0; i < vcRetrievalResultSet[showIndex].regionCount; i++)
		{
			cvRectangle(m_currentImage, ((vcRetrievalResultSet[showIndex]).vpRegions[i]).rectTopLeft, 
				((vcRetrievalResultSet[showIndex]).vpRegions[i]).rectBottomRight, CC_RGB(255,0,255), 2, 8, 0);
		}
		// ����һ����ʾ��ͼƬ��������
		if( m_imageShow )
			cvSet(m_imageShow, cvScalarAll(255));
		ResizeImage( m_currentImage, m_imageShow );
		ShowImage( m_imageShow, IDC_PICTURE_RES_CTL);
		m_showFileName = vcRetrievalResultSet[showIndex].imagePath;
		m_editIndex.Format("��ǰ��%d/%d��", showIndex+1, vcRetrievalResultSet.size());
	}

	UpdateData(FALSE);
}

void CRetrievalDlg::OnBnClickedButtonPrior()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	showIndex--;
	if (showIndex < 0)
	{
		AfxMessageBox("�Ѿ������һ��һ��ͼ��");
		showIndex++;
		return;
	}
	m_currentImage = cvLoadImage(vcRetrievalResultSet[showIndex].imagePath, 1);
	if(m_currentImage)
	{
		//����ǰ����ʾͼ������м�����������ʶ����
		for (int i = 0; i < vcRetrievalResultSet[showIndex].regionCount; i++)
		{
			cvRectangle(m_currentImage, ((vcRetrievalResultSet[showIndex]).vpRegions[i]).rectTopLeft, 
				((vcRetrievalResultSet[showIndex]).vpRegions[i]).rectBottomRight, CC_RGB(255,0,255), 2, 8, 0);
		}
		// ����һ����ʾ��ͼƬ��������
		if( m_imageShow )
			cvSet(m_imageShow, cvScalarAll(255));
		ResizeImage( m_currentImage, m_imageShow );
		ShowImage( m_imageShow, IDC_PICTURE_RES_CTL);
		m_showFileName = vcRetrievalResultSet[showIndex].imagePath;
		m_editIndex.Format("��ǰ��%d/%d��", showIndex+1, vcRetrievalResultSet.size());
	}

	UpdateData(FALSE);
}

void CRetrievalDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	m_parentDlg->m_progressCtrl.SetPos(0);
	CDialog::OnClose();
}
