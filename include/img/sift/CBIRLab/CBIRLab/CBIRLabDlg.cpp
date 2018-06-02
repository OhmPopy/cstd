
// CBIRLabDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CBIRLab.h"
#include "CBIRLabDlg.h"
#include "RetrievalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCBIRLabDlg �Ի���




CCBIRLabDlg::CCBIRLabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCBIRLabDlg::IDD, pParent)
	, m_editFileName(_T(""))
	, m_editFormat(_T(""))
	, m_editSize(_T(""))
	, m_editByte(_T(""))
	, m_editFolderPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_image = NULL;
}

void CCBIRLabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_editFileName);
	DDX_Text(pDX, IDC_EDIT_FORMAT, m_editFormat);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_editSize);
	DDX_Text(pDX, IDC_EDIT_BYTE, m_editByte);
	DDX_Text(pDX, IDC_EDIT_DATASET_PATH, m_editFolderPath);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressCtrl);
}

BEGIN_MESSAGE_MAP(CCBIRLabDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_MENU_QUIT, CCBIRLabDlg::OnMenuQuit)
	ON_COMMAND(ID_MENU_LOAD, CCBIRLabDlg::OnMenuLoad)
	ON_COMMAND(ID_MENU_ABOUT, CCBIRLabDlg::OnMenuAbout)
	ON_COMMAND(ID_MENU_FOLDER, CCBIRLabDlg::OnMenuFolder)
	ON_COMMAND(ID_MENU_RETRIEVAL, CCBIRLabDlg::OnMenuRetrieval)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, CCBIRLabDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_RETRIEVAL, CCBIRLabDlg::OnBnClickedButtonRetrieval)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, CCBIRLabDlg::OnBnClickedButtonFolder)
END_MESSAGE_MAP()


// CCBIRLabDlg ��Ϣ�������

BOOL CCBIRLabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//��Ӳ˵���
	m_myMenu.LoadMenu(IDR_MENU1);
	SetMenu(&m_myMenu);

	//�õ�ͼƬ�ռ�ĳߴ�
	// �����ʾ�ؼ��� DC
	CDC* pDC = GetDlgItem( IDC_PICTURE_CTL ) ->GetDC();
	// ��ȡ HDC(�豸���) �����л�ͼ����		
	HDC hDC = pDC ->GetSafeHdc();				
	CRect rect;
	GetDlgItem(IDC_PICTURE_CTL) ->GetClientRect( &rect );
	// ���ͼƬ�ؼ��Ŀ�͸�
	picWidth = rect.right - rect.left;			
	picHeight = rect.bottom - rect.top;

	ReleaseDC( pDC );

	//��ʼ��m_image
	CvSize ImgSize;
	ImgSize.height = picHeight;
	ImgSize.width = picWidth;
	m_imageShow = cvCreateImage( ImgSize, IMG_DEPTH_8U, IMAGE_CHANNELS );
	cvSet(m_imageShow, cvScalarAll(255));

	//��ʼ��m_progressCtrl
	m_progressCtrl.SetRange(0, 100);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCBIRLabDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCBIRLabDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// �ػ�Ի���
		CDialog::OnPaint();
		// ����windows���ڣ�������ⲽ���ã�ͼƬ��ʾ�����������
		CDialog::UpdateWindow();
		// �ػ�ͼƬ����
		ShowImage( m_imageShow, IDC_PICTURE_CTL);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCBIRLabDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCBIRLabDlg::OnMenuQuit()
{
	// TODO: �ڴ���������������
	OnCancel();
}

void CCBIRLabDlg::OnMenuLoad()
{
	// TODO: �ڴ���������������
	CFileDialog dlg(TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,
		_T("image files (*.bmp; *.jpg; *png; *.jpeg) |*.bmp;*.jpg;*.png;*.jpeg|All Files (*.*)|*.*||"),NULL);

	dlg.m_ofn.lpstrTitle= _T("����ͼ��");

	if (dlg.DoModal() == IDOK)
	{
		imagePath = dlg.GetPathName();
		// ��ȡͼƬ
		m_image = cvLoadImage( imagePath, 1 );
		
		//cvShowImage("CC", m_image);
		
		// �ж��Ƿ�ɹ���ȡͼƬ	
		if( !m_image )			
			return;

		m_editFileName = imagePath;
		m_editFormat = PathFindExtensionA(imagePath);
		m_editFormat.Delete(0);
		m_editFormat.MakeUpper();
		m_editSize.Format("%d*%d", m_image->width, m_image->height);

		//�ļ���С
		CFile m_file;
		m_file.Open(imagePath, CFile::modeRead);
		m_editByte.Format("%dKB", m_file.GetLength()/1024);
		m_file.Close();


		// ����һ����ʾ��ͼƬ��������
		if( m_imageShow )
				cvSet(m_imageShow, cvScalarAll(255));

		// �Զ����ͼƬ�������ŵ�picture�ؼ��ĺ���λ�ã��ٸ��Ƶ� TheImage ��
		ResizeImage( m_image, m_imageShow );	

		ShowImage( m_imageShow, IDC_PICTURE_CTL);

		UpdateData(FALSE);
	}
}

void CCBIRLabDlg::ShowImage( IplImage* img, UINT ID)
{
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
void CCBIRLabDlg::ResizeImage(IplImage* src, IplImage* dest)
{
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


void CCBIRLabDlg::OnMenuAbout()
{
	// TODO: �ڴ���������������
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CCBIRLabDlg::OnMenuFolder()
{
	// TODO: �ڴ���������������
	BROWSEINFO bi;
	char szPathName[MAX_PATH];
	char szTitle[] = "ѡ��·��";
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = GetSafeHwnd();
	bi.pszDisplayName = szPathName;
	bi.lpszTitle = szTitle;
	bi.ulFlags = 0x0040 ;
	CString str;

	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if(idl == NULL)
	{
		folderPath= "";
		return;
	}
	SHGetPathFromIDList(idl, str.GetBuffer(MAX_PATH * 2));
	str.ReleaseBuffer();
	if(str != "" && str.GetAt(str.GetLength() - 1) != '\\')
		str += "\\";
	folderPath = str;
	m_editFolderPath = folderPath;
	UpdateData(FALSE);
}

void CCBIRLabDlg::OnMenuRetrieval()
{
	// TODO: �ڴ���������������
	if (!m_image)
	{
		AfxMessageBox("�����������ͼ��");
		return;
	}
	if (folderPath.IsEmpty())
	{
		AfxMessageBox("���趨�������ݼ�Ŀ¼��");
		return;
	}
	CRetrievalDlg m_retrievalDlg;
	m_retrievalDlg.m_parentDlg = this;
	m_retrievalDlg.m_image = m_image;
	m_retrievalDlg.m_imagePath = imagePath;
	m_retrievalDlg.m_folderPath = folderPath;
	m_retrievalDlg.m_time = clock();
	m_retrievalDlg.computeResults();
	m_retrievalDlg.m_time = clock() - m_retrievalDlg.m_time;
	m_retrievalDlg.DoModal();
}

void CCBIRLabDlg::OnBnClickedButtonLoad()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnMenuLoad();
}

void CCBIRLabDlg::OnBnClickedButtonRetrieval()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnMenuRetrieval();
}

void CCBIRLabDlg::OnBnClickedButtonFolder()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnMenuFolder();
}
