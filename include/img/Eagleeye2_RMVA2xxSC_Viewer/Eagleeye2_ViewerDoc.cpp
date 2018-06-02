// Eagleeye2_ViewerDoc.cpp : implementation of the CEagleeye2_ViewerDoc class
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "Eagleeye2_ViewerDoc.h"
#include <winsock.h>
#include "SelectFileDlg.h"
#include "GeneralIODlg.h"
#include "process.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//socket ports
#define COMMANDPORT		35000				//�������������������̶߳˿�
#define TRGIMGPORT		45000				//����ģʽ��������������ݽ����������̶߳˿�
#define FCONIMGPORT		55000				//ȫ�ֱ�������ģʽ��������������ݽ����������̶߳˿�
#define HFRIMGPORT		65000				//������ģʽ��������������ݽ����������̶߳˿�
#define HFRBINIMGPORT	60000				//�����ʸ�������ģʽ��������������ݽ����������̶߳˿�
#define IMGINFLEN		sizeof(EagleEye2ImgInfStruct)//ץ����Ϣ����
//Recv image data thread
void	RecvTrgImagedata(LPVOID pParam);	//����ģʽ��������������ݽ����������̺߳�������
void	RecvConImagedata(LPVOID pParam);	//ȫ�ֱ�������ģʽ��������������ݽ����������̺߳�������
void	RecvHfconImagedata(LPVOID pParam);	//������ģʽ��������������ݽ����������̺߳�������
void	RecvHfBinconImagedata(LPVOID pParam);//������ģʽ��������������ݽ����������̺߳�������
//global variables
UINT			uiJpgDataLen;				//���ܵ���jpgͼ�����ݵĳ��� 
BUFFNODE		listJpgBuff[NUM_BUFF];		//װ��JPGͼ�����ݵĻ��λ�������
LONG			PrevBuff;					//���λ����ǰһ��ʹ������
LONG			CurrBuff;					//���λ���ĵ�ǰʹ������
volatile BYTE	TargetIPAdd1,TargetIPAdd2;	//
volatile BYTE	TargetIPAdd3,TargetIPAdd4;	//ά����ǰĿ�����IP��ַ��4������
char			cRcvBuf[4096];				//��Ŀ���������������Ľ��ܻ�����
char			cmdbuf[1024*5];				//��Ŀ���������������ķ��ͻ�����
volatile unsigned char	pImgBuf8[IMGLENGTH*3];
EagleEye2ImgInfStruct	EagleEye2_ImgInf =
{
	{'\0'},			//ui8LocusInf[101]
	{0},			//ui8TimeInf[7]
	0,				//ui8WayNum
	0,				//ui8TrgMode
	0.0				//fSpeed
};;
/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerDoc

IMPLEMENT_DYNCREATE(CEagleeye2_ViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CEagleeye2_ViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CEagleeye2_ViewerDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_CONFIG_TARGETIP, OnConfigTargetip)
	ON_COMMAND(ID_OMC_TRIGGER, OnOmcTrigger)
	ON_UPDATE_COMMAND_UI(ID_OMC_TRIGGER, OnUpdateOmcTrigger)
	ON_COMMAND(ID_TRGMODE_CONFIG, OnTrgmodeConfig)
	ON_UPDATE_COMMAND_UI(ID_TRGMODE_CONFIG, OnUpdateTrgmodeConfig)
	ON_COMMAND(ID_OPE_GRAB, OnOpeGrab)
	ON_UPDATE_COMMAND_UI(ID_OPE_GRAB, OnUpdateOpeGrab)
	ON_COMMAND(ID_OMC_MJPEG, OnOmcMjpeg)
	ON_UPDATE_COMMAND_UI(ID_OMC_MJPEG, OnUpdateOmcMjpeg)
	ON_COMMAND(ID_MJPEGMODE_CONFIG, OnMjpegmodeConfig)
	ON_UPDATE_COMMAND_UI(ID_MJPEGMODE_CONFIG, OnUpdateMjpegmodeConfig)
	ON_COMMAND(ID_OMC_HFR, OnOmcHfr)
	ON_UPDATE_COMMAND_UI(ID_OMC_HFR, OnUpdateOmcHfr)
	ON_COMMAND(ID_HFRMODE_CONFIG, OnHfrmodeConfig)
	ON_UPDATE_COMMAND_UI(ID_HFRMODE_CONFIG, OnUpdateHfrmodeConfig)
	ON_COMMAND(ID_CONFIG_NEWIP, OnConfigNewip)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_NEWIP, OnUpdateConfigNewip)
	ON_COMMAND(ID_CONFIG_NEWMAC, OnConfigNewmac)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_NEWMAC, OnUpdateConfigNewmac)
	ON_COMMAND(ID_CONFIG_AFEGAINCTRL, OnConfigAfegainctrl)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_AFEGAINCTRL, OnUpdateConfigAfegainctrl)
	ON_COMMAND(ID_CONFIG_WHITEBALANCE, OnConfigWhitebalance)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_WHITEBALANCE, OnUpdateConfigWhitebalance)
	ON_COMMAND(ID_CONFIG_GIOCONFIG, OnConfigGioconfig)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_GIOCONFIG, OnUpdateConfigGioconfig)
	ON_COMMAND(ID_CONFIG_RTCCONFIG, OnConfigRtcconfig)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_RTCCONFIG, OnUpdateConfigRtcconfig)
	ON_COMMAND(ID_CONFIG_WDTCONFIG, OnConfigWdtconfig)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_WDTCONFIG, OnUpdateConfigWdtconfig)
	ON_COMMAND(ID_OPE_AUTOWB, OnOpeAutowb)
	ON_UPDATE_COMMAND_UI(ID_OPE_AUTOWB, OnUpdateOpeAutowb)
	ON_COMMAND(ID_OPE_AUTOTRG_ENABLE, OnOpeAutotrgEnable)
	ON_UPDATE_COMMAND_UI(ID_OPE_AUTOTRG_ENABLE, OnUpdateOpeAutotrgEnable)
	ON_COMMAND(ID_OPE_AUTOTRG_DISABLE, OnOpeAutotrgDisable)
	ON_UPDATE_COMMAND_UI(ID_OPE_AUTOTRG_DISABLE, OnUpdateOpeAutotrgDisable)
	ON_COMMAND(ID_OPE_LUT_ENABLE, OnOpeLutEnable)
	ON_UPDATE_COMMAND_UI(ID_OPE_LUT_ENABLE, OnUpdateOpeLutEnable)
	ON_COMMAND(ID_OPE_LUT_DISABLE, OnOpeLutDisable)
	ON_UPDATE_COMMAND_UI(ID_OPE_LUT_DISABLE, OnUpdateOpeLutDisable)
	ON_COMMAND(ID_OPE_RELAY_OPEN, OnOpeRelayOpen)
	ON_UPDATE_COMMAND_UI(ID_OPE_RELAY_OPEN, OnUpdateOpeRelayOpen)
	ON_COMMAND(ID_OPE_RELAY_CLOSE, OnOpeRelayClose)
	ON_UPDATE_COMMAND_UI(ID_OPE_RELAY_CLOSE, OnUpdateOpeRelayClose)
	ON_COMMAND(ID_OPE_RESETSYS, OnOpeResetsys)
	ON_UPDATE_COMMAND_UI(ID_OPE_RESETSYS, OnUpdateOpeResetsys)
	ON_COMMAND(ID_OPE_REMOTEUPDATEDSPPRROM, OnOpeRemoteupdatedspprrom)
	ON_UPDATE_COMMAND_UI(ID_OPE_REMOTEUPDATEDSPPRROM, OnUpdateOpeRemoteupdatedspprrom)
	ON_COMMAND(ID_OPE_IMGFORMAT_RGB, OnOpeImgformatRgb)
	ON_UPDATE_COMMAND_UI(ID_OPE_IMGFORMAT_RGB, OnUpdateOpeImgformatRgb)
	ON_COMMAND(ID_OPE_IMGFORMAT_YUV, OnOpeImgformatYuv)
	ON_UPDATE_COMMAND_UI(ID_OPE_IMGFORMAT_YUV, OnUpdateOpeImgformatYuv)
	ON_COMMAND(ID_OPE_IMGFORMAT_BAYER, OnOpeImgformatBayer)
	ON_UPDATE_COMMAND_UI(ID_OPE_IMGFORMAT_BAYER, OnUpdateOpeImgformatBayer)
	ON_COMMAND(ID_OPE_TRGSHUTTERCTRL_MANUAL, OnOpeTrgshutterctrlManual)
	ON_UPDATE_COMMAND_UI(ID_OPE_TRGSHUTTERCTRL_MANUAL, OnUpdateOpeTrgshutterctrlManual)
	ON_COMMAND(ID_OPE_TRGSHUTTERCTRL_AUTO, OnOpeTrgshutterctrlAuto)
	ON_UPDATE_COMMAND_UI(ID_OPE_TRGSHUTTERCTRL_AUTO, OnUpdateOpeTrgshutterctrlAuto)
	ON_COMMAND(ID_OMC_HFRB, OnOmcHfrb)
	ON_UPDATE_COMMAND_UI(ID_OMC_HFRB, OnUpdateOmcHfrb)
	ON_COMMAND(ID_HFRBINMODE_CONFIG, OnHfrbinmodeConfig)
	ON_UPDATE_COMMAND_UI(ID_HFRBINMODE_CONFIG, OnUpdateHfrbinmodeConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerDoc construction/destruction

CEagleeye2_ViewerDoc::CEagleeye2_ViewerDoc()
{
	//һЩͨ�ÿ��Ʊ����ĳ�ʼ��
	m_bIsTrgMode	= FALSE;		//�Ƿ񴥷�ģʽ
	m_bIsFconMode	= FALSE;		//�Ƿ�ȫ�ֱ�������ģʽ
	m_bIsHFRMode	= FALSE;		//�Ƿ������ģʽ
	m_bIsHFRBinMode = FALSE;		//�Ƿ�����ʸ�������ģʽ
	m_bIsEnsureTgtIP = FALSE;		//�Ƿ�ȷ��Ŀ�������IP��ַ
	m_bRelayIsOpen = TRUE;			//����̵����Ƿ�Ͽ�
	m_bLUTEnable = TRUE;			//����Ƿ�ʹ��ͼ���LUTӳ��任
	m_bAutoTrgEnable = FALSE;		//�л�������ģʽ���Ƿ������Զ�����ץȡһ��ͼ��
	m_bFirstShow = TRUE ;			//��������Ƿ�Ϊ������ʾ������ά����ͼ��ˢ��
	m_bIsDataFromCamera = FALSE;	//��־��ǰ��ͼ��ʾ��ͼ���Ƿ�������������������Ҳ�ɴ�Ӳ���еľ�̬ͼ��
	m_bIsJpeg = FALSE;				//��־��ǰ��ͼ��ʾ��ͼ���Ƿ���JPG��ʽ
	m_bIsHighFR = FALSE;			//��־��ǰ��Ҫ��ʾ��ͼ���Ƿ����Ը�����ģʽ���Դ���������������ģʽ��������OnDraw�����н�����ʾ����
	m_bIsRGBArray = TRUE;			//����ɼ���ͼ���ʽ�Ƿ�ΪRGB��ʽ
	m_bIsYUVArray = FALSE;			//����ɼ���ͼ���ʽ�Ƿ�ΪYUV��ʽ
	m_bIsBAYERArray = FALSE;		//����ɼ���ͼ���ʽ�Ƿ�ΪBAYER��ʽ
	m_bTrgShutterIsManual = TRUE;	//����ģʽ�����ٶ��Ƿ����ֶ�ģʽ
	//��ʼ�����λ�����
	for (int nBuff=0; nBuff<NUM_BUFF; nBuff++) {
		listJpgBuff[nBuff].sizeBuff = 0;
		listJpgBuff[nBuff].bIsLocked = FALSE;
	}
	CurrBuff = 0; PrevBuff = -1;
}

CEagleeye2_ViewerDoc::~CEagleeye2_ViewerDoc()
{
}

BOOL CEagleeye2_ViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	POSITION firstViewPos;
	CView *pView;
	//�����ͼ��Ĵ��ڶ���
	firstViewPos=GetFirstViewPosition();
	pView=GetNextView(firstViewPos);
	m_hViewWnd=pView->GetSafeHwnd();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerDoc serialization
void CEagleeye2_ViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerDoc diagnostics
#ifdef _DEBUG
void CEagleeye2_ViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEagleeye2_ViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerDoc commands
void CEagleeye2_ViewerDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	int			i;
	CString		Filter;
	CString		strFileName;

	Filter.LoadString(IDS_PICFORMAT);
	
	CFileDialog dlgFile(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, Filter, NULL);
	if(dlgFile.DoModal() == IDCANCEL){
		return;
	}
	
	CWaitCursor		waitCursor;
	strFileName = dlgFile.GetPathName();
	i = strFileName.GetLength();
	if( i<4 || strFileName.GetAt(i-4)!='.'){
		strFileName += ".jpg";
	}
	if (!m_fPic.Save (strFileName))
	{
		::MessageBox (NULL, _T("�����ļ�ʧ��! �����Ƿ�ָ����չ��"), _T("����"), MB_OK | MB_ICONINFORMATION) ;
		return ;
	}

}

void CEagleeye2_ViewerDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CString		Filter;
	Filter.LoadString(IDS_PICFORMAT);

	CSelectFileDlg		dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, Filter, NULL);
	if(dlgFile.DoModal() == IDCANCEL){
		return;
	}

	DeleteContents();
	CWaitCursor		waitCursor;
	if (m_fPic.Load(dlgFile.GetPathName()) == IMAGE_UNKNOW){
		::MessageBox(NULL, _T("Failed to read file"), _T("Error"), MB_OK | MB_ICONINFORMATION);
		return;
	}
	this->SetPathName(dlgFile.GetPathName()); //pass the parameter to OnSaveDocument

	m_DDB.Attach(m_fPic.GetDDB_Handle()); //Get DDB
	m_fPic.SetUndoEnable(true); //set undo-flag
	m_bFirstShow = TRUE;
	m_bIsDataFromCamera = FALSE;
	this->UpdateAllViews (NULL);	
}

///////////////////////////////////////////////////////////////////////////////
//							Sub Threads
///////////////////////////////////////////////////////////////////////////////
void RecvTrgImagedata(LPVOID pParam)
{
	//����DSPͼ�����ݵ��߳�
	CEagleeye2_ViewerDoc* pDoc=(CEagleeye2_ViewerDoc*) pParam;
	WORD				wVersionRequested;
	WSADATA				wsaData;
	SOCKET				sockTrgClient;
	struct sockaddr_in	addr;
	int					i,cnt;
	BOOL				flag = TRUE;
	UINT				recvdatalen;
	BYTE				tmp1,tmp2,tmp3,tmp4;
	UINT				tmp;
	CString				strFileName;
	static CString		strFileNamebak="\0";
	static int			iIndex = 0;

	//��ȡĿ�������IP��ַ
	tmp1 = TargetIPAdd1; tmp2 = TargetIPAdd2;
	tmp3 = TargetIPAdd3; tmp4 = TargetIPAdd4;
	tmp =	tmp1;
	tmp |=	tmp2 << 8;
	tmp |=	tmp3 << 16;
	tmp |=	tmp4 << 24;
	wVersionRequested = MAKEWORD(1, 1);
	i = WSAStartup(wVersionRequested, &wsaData);
	if (i != 0){
		AfxMessageBox("�޷���ʼ��ȫ�ֱ�������ģʽ�׽ӿڶ���");
		return;
	}
	sockTrgClient = socket( AF_INET, SOCK_STREAM, 0 );
	if( sockTrgClient < 0){
		goto leave;
	}
	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr	= tmp;
	addr.sin_port		= htons(TRGIMGPORT);
	//����Ŀ�����
	if ( connect( sockTrgClient, (const struct sockaddr *)&addr, sizeof(addr)) < 0 ){
		goto leave;
	}
  //pDoc->OnOpeGrab();
	//��ʼ��������ϴ�ͼ������
	while(1){
		//����JPG��������
		i = 0;
		while( i < 4){
			cnt = recv( sockTrgClient, (char*)&uiJpgDataLen+i, 4-i, 0 );
			if( cnt <=0){
				goto leave;
			}
			i += cnt;
			if( i == 4){
				recvdatalen = uiJpgDataLen;
			}
			//���������ͻ�0xffff���ʾ��������ͨѶ
			if( recvdatalen==0x00 ){
				goto leave;
			}
		}
		//����ץ����Ϣ
		i = 0;
		while( i < IMGINFLEN ){
			cnt = recv( sockTrgClient, (char*)&EagleEye2_ImgInf+i, IMGINFLEN-i, 0 );
			if( cnt <= 0 ){
				goto leave;
			}
			i += cnt;
		}
		//����ͼ������
		i = 0;
		listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
		pDoc->m_bIsJpeg = TRUE;
		while( i < recvdatalen ){
			cnt = recv( sockTrgClient, listJpgBuff[CurrBuff].pBuff+i, recvdatalen-i, 0 );
			if( cnt <= 0 ){
				goto leave;
			}
			i += cnt;
		}
		CurrBuff++;
		if(CurrBuff==5){
			CurrBuff = 0;
		}
		PrevBuff++;
		if(PrevBuff==5){
			PrevBuff = 0;
		}
		pDoc->m_bIsDataFromCamera = TRUE;
		//֪ͨ��ͼ������ʾ
		PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);
		//����ץ��ͼ��
		CString	strFileName,strDirectory;
		char tmpStr[20];
		CString	strFolderName;
		CFileFind   f;
		//�ص��ʱ��
		if( strlen(EagleEye2_ImgInf.cLocusInf) >0 ){
			strFileName += EagleEye2_ImgInf.cLocusInf;
			if( EagleEye2_ImgInf.byTimeInf[0]>9 ){
				strFileName += "_20";
				strFolderName += "20";
			}
			else{
				strFileName += "_200";
				strFolderName += "200";
			}
		}
		else{
			if( EagleEye2_ImgInf.byTimeInf[0]>9 ){
				strFileName += "20";
				strFolderName += "20";
			}
			else{
				strFileName += "200";
				strFolderName += "200";
			}
		}
		strFileName += itoa(EagleEye2_ImgInf.byTimeInf[0],tmpStr,10);
		strFolderName += itoa(EagleEye2_ImgInf.byTimeInf[0],tmpStr,10);
		if( EagleEye2_ImgInf.byTimeInf[1] >9 ){
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[1],tmpStr,10);
			strFolderName+=itoa(EagleEye2_ImgInf.byTimeInf[1],tmpStr,10);
		}
		else{
			strFileName += "0";
			strFolderName += "0";
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[1],tmpStr,10);
			strFolderName+=itoa(EagleEye2_ImgInf.byTimeInf[1],tmpStr,10);
		}
		if( EagleEye2_ImgInf.byTimeInf[2] >9 ){
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[2],tmpStr,10);
			strFolderName+=itoa(EagleEye2_ImgInf.byTimeInf[2],tmpStr,10);
		}
		else{
			strFileName += "0";
			strFolderName += "0";
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[2],tmpStr,10);
			strFolderName+=itoa(EagleEye2_ImgInf.byTimeInf[2],tmpStr,10);
		}
		if( EagleEye2_ImgInf.byTimeInf[4] >9 ){
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[4],tmpStr,10);
			strFolderName+=itoa(EagleEye2_ImgInf.byTimeInf[4],tmpStr,10);
		}
		else{
			strFileName += "0";
			strFolderName += "0";
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[4],tmpStr,10);
			strFolderName+=itoa(EagleEye2_ImgInf.byTimeInf[4],tmpStr,10);
		}
		if( EagleEye2_ImgInf.byTimeInf[5] >9 ){
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[5],tmpStr,10);
		}
		else{
			strFileName += "0";
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[5],tmpStr,10);
		}
		if( EagleEye2_ImgInf.byTimeInf[6] >9 ){
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[6],tmpStr,10);
		}
		else{
			strFileName += "0";
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[6],tmpStr,10);
		}
		//������
		strFileName += "_";
		strFileName += itoa(EagleEye2_ImgInf.byWayNum,tmpStr,10);
		strFileName += "����";
		//������
		if( !strcmp(strFileName,strFileNamebak) ){
			//�����ļ���
			strFileNamebak = strFileName;
			//����������
			strFileName += "_";
			strFileName += itoa(++iIndex,tmpStr,10);
		}
		else{
			//�����ļ���
			strFileNamebak = strFileName;				
			iIndex = 0;
		}
		//��������
		switch( EagleEye2_ImgInf.byTrgMode ){
		case 0://����
			strFileName += "_����";
			break;
		case 1://����Ȧ
			strFileName += "_����Ȧ����";
			break;
		case 2://˫��Ȧ
			strFileName += "_˫��Ȧ��������Ϊ";
			if( EagleEye2_ImgInf.fSpeed - (int)EagleEye2_ImgInf.fSpeed < 0.0001 ){
				strFileName += itoa((int)EagleEye2_ImgInf.fSpeed,tmpStr,10);
			}
			else{
				strFileName += _gcvt( EagleEye2_ImgInf.fSpeed, 5, tmpStr );
				int tttt;
				tttt = strlen(strFileName);
				if( strFileName.GetAt(strlen(strFileName)-1)=='.' ){
					strFileName += "00";
				}
			}
			strFileName += "����";
			break;
		case 3://ר��Ӳ����
			strFileName += "_ר��Ӳ����";
			break;
		default:
			break;
		}
		//Creat Directory
		strDirectory.Format("D:\\Picture\\");
		if( !f.FindFile( strDirectory ) ){
			CreateDirectory(strDirectory,NULL);
		}
		strDirectory+=strFolderName;
		strDirectory+="\\";
		if( !f.FindFile( strDirectory ) ){
			CreateDirectory(strDirectory,NULL);
		}
		//end Creat Directory
		strDirectory += strFileName;
		strDirectory += ".jpg";
		pDoc->m_fPic1.LoadJpeg((BYTE*)listJpgBuff[PrevBuff].pBuff, listJpgBuff[PrevBuff].sizeBuff);
		pDoc->m_fPic1.Save (strDirectory);
	}

leave:
	//���������˳��߳�
	closesocket(sockTrgClient);
	WSACleanup();
}

void RecvConImagedata(LPVOID pParam)
{
	//����DSPͼ�����ݵ��߳�
	CEagleeye2_ViewerDoc* pDoc=(CEagleeye2_ViewerDoc*) pParam;
	WORD				wVersionRequested;
	WSADATA				wsaData;
	SOCKET				sockFconClient;
	struct sockaddr_in	addr;	
	int					i,cnt;
	BOOL				flag = TRUE;
	UINT				recvdatalen;
	BYTE				tmp1,tmp2,tmp3,tmp4;
	UINT				tmp;
	BYTE				replyBuf[2];

	//��ȡĿ�������IP��ַ
	tmp1 = TargetIPAdd1; tmp2 = TargetIPAdd2;
	tmp3 = TargetIPAdd3; tmp4 = TargetIPAdd4;
	tmp =	tmp1;
	tmp |=	tmp2 << 8;
	tmp |=	tmp3 << 16;
	tmp |=	tmp4 << 24;
	wVersionRequested = MAKEWORD(1, 1);
	i = WSAStartup(wVersionRequested, &wsaData);
	if (i != 0){
		AfxMessageBox("�޷���ʼ��ȫ�ֱ�������ģʽ�׽ӿڶ���");
		return;
	}
	sockFconClient = socket( AF_INET, SOCK_STREAM, 0 );
	if( sockFconClient < 0){
		goto leave;
	}
	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr	= tmp;
	addr.sin_port		= htons(FCONIMGPORT);
	//����Ŀ�����
	if ( connect( sockFconClient, (const struct sockaddr *)&addr, sizeof(addr)) < 0 ){
		goto leave;
	}
	//��ʼ��������ϴ�ͼ������
	while(1){
		i = 0;
		while( i < 4){
			cnt = recv( sockFconClient, (char*)&uiJpgDataLen+i, 4-i, 0 );
			if( cnt <=0){
				goto leave;
			}
			i += cnt;
			if( i == 4){
				recvdatalen = uiJpgDataLen;
			}
		}
		i = 0;
		listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
		pDoc->m_bIsJpeg = TRUE;
		while( i < recvdatalen ){
			cnt = recv( sockFconClient, listJpgBuff[CurrBuff].pBuff+i, recvdatalen-i, 0 );
			if( cnt <= 0 ){
				goto leave;
			}
			i += cnt;
		}
		CurrBuff++;
		if(CurrBuff==5){
			CurrBuff = 0;
		}
		PrevBuff++;
		if(PrevBuff==5){
			PrevBuff = 0;
		}
		pDoc->m_bIsDataFromCamera = TRUE;
		//��ȫ�ֱ���ģʽ�»ظ�����Ƿ������ǰ���ӣ����ظ�0xffff����������������������
		if( !pDoc->m_bIsFconMode ){
			replyBuf[0] = 0xff;
			replyBuf[1] = 0xff;
			if( send( sockFconClient, (char*)replyBuf, 2, 0 ) < 0 ){
				goto leave;
			}
			goto leave;
		}
		else{
			replyBuf[0] = 0x0;
			replyBuf[1] = 0x0;
			if( send( sockFconClient, (char*)replyBuf, 2, 0 ) < 0 ){
				goto leave;
			}
		}
		//֪ͨ��ͼ������ʾ
		PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);
	}

leave:
	//���������˳��߳�
	closesocket(sockFconClient);
	WSACleanup();
}

void RecvHfconImagedata(LPVOID pParam)
{
	//����DSPͼ�����ݵ��߳�
	CEagleeye2_ViewerDoc* pDoc=(CEagleeye2_ViewerDoc*) pParam;
	WORD				wVersionRequested;
	WSADATA				wsaData;
	SOCKET				sockHfrClient;
	struct sockaddr_in	addr;
	int					i,cnt;
	BOOL				flag = TRUE;
	UINT				recvdatalen;
	BYTE				tmp1,tmp2,tmp3,tmp4;
	UINT				tmp;
	BYTE				replyBuf[2];

	//��ȡĿ�������IP��ַ
	tmp1 = TargetIPAdd1; tmp2 = TargetIPAdd2;
	tmp3 = TargetIPAdd3; tmp4 = TargetIPAdd4;
	tmp =	tmp1;
	tmp |=	tmp2 << 8;
	tmp |=	tmp3 << 16;
	tmp |=	tmp4 << 24;
	wVersionRequested = MAKEWORD(1, 1);
	i = WSAStartup(wVersionRequested, &wsaData);
	if (i != 0){
		AfxMessageBox("�޷���ʼ��ȫ�ֱ�������ģʽ�׽ӿڶ���");
		return;
	}
	sockHfrClient = socket( AF_INET, SOCK_STREAM, 0 );
	if( sockHfrClient < 0){
		goto leave;
	}
	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr	= tmp;
	addr.sin_port		= htons(HFRIMGPORT);
	//����Ŀ�����
	if ( connect( sockHfrClient, (const struct sockaddr *)&addr, sizeof(addr)) < 0 ){
		goto leave;
	}
	//��ʼ��������ϴ�ͼ������
	while(1){
		i = 0;
		while( i < 4){
			cnt = recv( sockHfrClient, (char*)&uiJpgDataLen+i, 4-i, 0 );
			if( cnt <=0){
				goto leave;
			}
			i += cnt;
			if( i == 4){
				recvdatalen = uiJpgDataLen;
			}
		}
		i = 0;
		listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
		pDoc->m_bIsJpeg = TRUE;
		while( i < recvdatalen ){
			cnt = recv( sockHfrClient, listJpgBuff[CurrBuff].pBuff+i, recvdatalen-i, 0 );
			if( cnt <= 0 ){
				goto leave;
			}
			i += cnt;
		}
		CurrBuff++;
		if(CurrBuff==5){
			CurrBuff = 0;
		}
		PrevBuff++;
		if(PrevBuff==5){
			PrevBuff = 0;
		}
		pDoc->m_bIsDataFromCamera = TRUE;
		//�ڸ�����ģʽ�»ظ�����Ƿ������ǰ���ӣ����ظ�0xffff����������������������
		if( !pDoc->m_bIsHFRMode ){
			replyBuf[0] = 0xff;
			replyBuf[1] = 0xff;
			if( send( sockHfrClient, (char*)replyBuf, 2, 0 ) < 0 ){
				goto leave;
			}
			goto leave;
		}
		else{
			replyBuf[0] = 0x0;
			replyBuf[1] = 0x0;
			if( send( sockHfrClient, (char*)replyBuf, 2, 0 ) < 0 ){
				goto leave;
			}
		}
		//֪ͨ��ͼ������ʾ
		PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);
	}

leave:
	//���������˳��߳�
	closesocket(sockHfrClient);
	WSACleanup();
}

void RecvHfBinconImagedata(LPVOID pParam)
{
	//����DSPͼ�����ݵ��߳�
	CEagleeye2_ViewerDoc* pDoc=(CEagleeye2_ViewerDoc*) pParam;
	WORD				wVersionRequested;
	WSADATA				wsaData;
	SOCKET				sockHfrBinClient;
	struct sockaddr_in	addr;
	int					i,cnt;
	BOOL				flag = TRUE;
	UINT				recvdatalen;
	BYTE				tmp1,tmp2,tmp3,tmp4;
	UINT				tmp;
	BYTE				replyBuf[2];

	//��ȡĿ�������IP��ַ
	tmp1 = TargetIPAdd1; tmp2 = TargetIPAdd2;
	tmp3 = TargetIPAdd3; tmp4 = TargetIPAdd4;
	tmp =	tmp1;
	tmp |=	tmp2 << 8;
	tmp |=	tmp3 << 16;
	tmp |=	tmp4 << 24;
	wVersionRequested = MAKEWORD(1, 1);
	i = WSAStartup(wVersionRequested, &wsaData);
	if (i != 0){
		AfxMessageBox("�޷���ʼ��ȫ�ֱ�������ģʽ�׽ӿڶ���");
		return;
	}
	sockHfrBinClient = socket( AF_INET, SOCK_STREAM, 0 );
	if( sockHfrBinClient < 0){
		goto leave;
	}
	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr	= tmp;
	addr.sin_port		= htons(HFRBINIMGPORT);
	//����Ŀ�����
	if ( connect( sockHfrBinClient, (const struct sockaddr *)&addr, sizeof(addr)) < 0 ){
		goto leave;
	}
	//��ʼ��������ϴ�ͼ������
	while(1){
		i = 0;
		while( i < 4){
			cnt = recv( sockHfrBinClient, (char*)&uiJpgDataLen+i, 4-i, 0 );
			if( cnt <=0){
				goto leave;
			}
			i += cnt;
			if( i == 4){
				recvdatalen = uiJpgDataLen;
			}
		}
		i = 0;
		listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
		pDoc->m_bIsJpeg = TRUE;
		while( i < recvdatalen ){
			cnt = recv( sockHfrBinClient, listJpgBuff[CurrBuff].pBuff+i, recvdatalen-i, 0 );
			if( cnt <= 0 ){
				goto leave;
			}
			i += cnt;
		}
		CurrBuff++;
		if(CurrBuff==5){
			CurrBuff = 0;
		}
		PrevBuff++;
		if(PrevBuff==5){
			PrevBuff = 0;
		}
		pDoc->m_bIsDataFromCamera = TRUE;
		//�ڸ�����ģʽ�»ظ�����Ƿ������ǰ���ӣ����ظ�0xffff����������������������
		if( !pDoc->m_bIsHFRBinMode ){
			replyBuf[0] = 0xff;
			replyBuf[1] = 0xff;
			if( send( sockHfrBinClient, (char*)replyBuf, 2, 0 ) < 0 ){
				goto leave;
			}
			goto leave;
		}
		else{
			replyBuf[0] = 0x0;
			replyBuf[1] = 0x0;
			if( send( sockHfrBinClient, (char*)replyBuf, 2, 0 ) < 0 ){
				goto leave;
			}
		}
		//֪ͨ��ͼ������ʾ
		PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);
	}

leave:
	//���������˳��߳�
	closesocket(sockHfrBinClient);
	WSACleanup();
}
//////////////////////////////////////////////////////////////////////////
//��Ŀ�������������
BOOL CEagleeye2_ViewerDoc::SendCMDtoDSP( char* cmdbuf)
{
	char				*buf = cmdbuf;
	WORD				wVersionRequested;
	WSADATA				wsaData;
	SOCKET				sockCMDClient;
	struct sockaddr_in	addr;
	UINT				i,cnt,tmp;
	BYTE				tmp1,tmp2,tmp3,tmp4;
	u_long				bNonblock = 1;
	UINT				datalen,recvdatalen;

	//����ʹ��TCPЭ�����Ŀ���������ָ����׽ӿڶ���
	tmp1 = TargetIPAdd1; tmp2 = TargetIPAdd2;
	tmp3 = TargetIPAdd3; tmp4 = TargetIPAdd4;
	tmp =	tmp1;
	tmp |=	tmp2 << 8;
	tmp |=	tmp3 << 16;
	tmp |=	tmp4 << 24;
	wVersionRequested = MAKEWORD(1, 1);
	i = WSAStartup(wVersionRequested, &wsaData);
	if (i != 0){
		AfxMessageBox("�޷���ʼ�����������׽ӿڶ���");
		return FALSE;
	}
	sockCMDClient = socket( AF_INET, SOCK_STREAM, 0 );
	if( sockCMDClient < 0){
		goto leave;
	}
	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr	= tmp;
	addr.sin_port		= htons(COMMANDPORT);
	//����Ŀ�����
	if ( connect( sockCMDClient, (const struct sockaddr *)&addr, sizeof(addr)) < 0 ){
		goto leave;
	}
	//��������ID�Ͳ����ĳ���(�ֽ���)
	if( send( sockCMDClient, buf, 6, 0 ) < 0 ){
		goto leave;
	}
	//�����������
	if( send( sockCMDClient, buf+6, *(UINT*)(buf+2), 0 ) < 0 ){
		goto leave;
	}
	//�������Ӧ�𣬽��������Ӧ
	if( buf[1] ){
		i = 0;
		while( i < 4){
			cnt = recv( sockCMDClient, (char*)&datalen+i, 4-i, 0 );
			if( cnt <=0){
				goto	leave;
			}
			i += cnt;
			if( i == 4){
				recvdatalen = datalen;
			}
		}
		i = 0;
		while( i < recvdatalen){
			cnt = recv( sockCMDClient, cRcvBuf+i, recvdatalen-i, 0 );
			if( cnt <=0){
				goto	leave;
			}
			i += cnt;
		}
	}
	//�ر��׽ӿڶ��󲢷�����ֵ
	closesocket(sockCMDClient);
	WSACleanup();
	return TRUE;

//�������󣬹ر��׽ӿڶ��󲢷���
leave:
	closesocket(sockCMDClient);
	WSACleanup();
	m_bIsEnsureTgtIP = FALSE;
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//����Ŀ�������IP��ַ��ͬʱ��������ʹĿ���������ȫ�ֱ�������ģʽ�µ������ϴ�
void CEagleeye2_ViewerDoc::OnConfigTargetip() 
{
	CMainFrame	*pMain = NULL;
	UINT		tmp1 = 0, tmp2 = 0;

	if(m_TargetIPDlg.DoModal()==IDOK){
		TargetIPAdd1 = m_TargetIPDlg.m_byIPAdd1;
		TargetIPAdd2 = m_TargetIPDlg.m_byIPAdd2;
		TargetIPAdd3 = m_TargetIPDlg.m_byIPAdd3;
		TargetIPAdd4 = m_TargetIPDlg.m_byIPAdd4;
		//��λ����ģʽ�ı�־��ȷ��Ŀ��IP��־
		m_bIsTrgMode	= FALSE;
		m_bIsFconMode	= FALSE;
		m_bIsHFRMode	= FALSE;
		m_bIsHFRBinMode = FALSE;
		m_bIsEnsureTgtIP = FALSE;
		//��ⴥ��ģʽ�������Ƿ�û���˳����ӣ���û����Ͽ�
		cmdbuf[0] = 0x23;
		cmdbuf[1] = 0x00;//��Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = 0x00;
		if(!SendCMDtoDSP(cmdbuf)){
			AfxMessageBox("�������ʧ��!");
			return;
		}
		//��ѯĿ������Ĺ���ģʽ�������ӳɹ�������Ӧ�Ŀͻ����߳�
		cmdbuf[0] = 0x03;
		cmdbuf[1] = 0x01;//��Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = 0x00;
		if(SendCMDtoDSP(cmdbuf)){
			m_bIsEnsureTgtIP = TRUE;
			//��ȡ��ز������ý���
			//���ÿɱ��I/O�Ŀڷ�������ֵ�Ը��½���
			tmp1 = *(UINT*)(cRcvBuf+16);
			tmp2 = *(UINT*)(cRcvBuf+20);
			//port1����
			if( (tmp1&0x001) == 0 ){
				//����
				m_GeneralIODlg.m_bIsPort1IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio1 = 0;
			}
			else{
				//���
				m_GeneralIODlg.m_bIsPort1IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio1 = 1;
				if( (tmp2&0x001) == 0 ){
					m_GeneralIODlg.m_bPort1OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort1OUTValue = TRUE;
				}
			}
			//port2����
			if( (tmp1&0x002) == 0 ){
				//����
				m_GeneralIODlg.m_bIsPort2IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio2 = 0;
			}
			else{
				//���
				m_GeneralIODlg.m_bIsPort2IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio2 = 1;
				if( (tmp2&0x002) == 0 ){
					m_GeneralIODlg.m_bPort2OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort2OUTValue = TRUE;
				}
			}
			//port3����
			if( (tmp1&0x004) == 0 ){
				//����
				m_GeneralIODlg.m_bIsPort3IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio3 = 0;
			}
			else{
				//���
				m_GeneralIODlg.m_bIsPort3IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio3 = 1;
				if( (tmp2&0x004) == 0 ){
					m_GeneralIODlg.m_bPort3OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort3OUTValue = TRUE;
				}
			}
			//port4����
			if( (tmp1&0x008) == 0 ){
				//����
				m_GeneralIODlg.m_bIsPort4IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio4 = 0;
			}
			else{
				//���
				m_GeneralIODlg.m_bIsPort4IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio4 = 1;
				if( (tmp2&0x008) == 0 ){
					m_GeneralIODlg.m_bPort4OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort4OUTValue = TRUE;
				}
			}
			//port5����
			if( (tmp1&0x010) == 0 ){
				//����
				m_GeneralIODlg.m_bIsPort5IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio5 = 0;
			}
			else{
				//���
				m_GeneralIODlg.m_bIsPort5IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio5 = 1;
				if( (tmp2&0x010) == 0 ){
					m_GeneralIODlg.m_bPort5OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort5OUTValue = TRUE;
				}
			}
			//port6����
			if( (tmp1&0x020) == 0 ){
				//����
				m_GeneralIODlg.m_bIsPort6IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio6 = 0;
			}
			else{
				//���
				m_GeneralIODlg.m_bIsPort6IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio6 = 1;
				if( (tmp2&0x020) == 0 ){
					m_GeneralIODlg.m_bPort6OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort6OUTValue = TRUE;
				}
			}
			//port7����
			if( (tmp1&0x040) == 0 ){
				//����
				m_GeneralIODlg.m_bIsPort7IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio7 = 0;
			}
			else{
				//���
				m_GeneralIODlg.m_bIsPort7IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio7 = 1;
				if( (tmp2&0x040) == 0 ){
					m_GeneralIODlg.m_bPort7OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort7OUTValue = TRUE;
				}
			}
			//port8����
			if( (tmp1&0x080) == 0 ){
				//����
				m_GeneralIODlg.m_bIsPort8IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio8 = 0;
			}
			else{
				//���
				m_GeneralIODlg.m_bIsPort8IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio8 = 1;
				if( (tmp2&0x080) == 0 ){
					m_GeneralIODlg.m_bPort8OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort8OUTValue = TRUE;
				}
			}
			//port9����
			if( (tmp1&0x100) == 0 ){
				//����
				m_GeneralIODlg.m_bIsPort9IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio9 = 0;
			}
			else{
				//���
				m_GeneralIODlg.m_bIsPort9IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio9 = 1;
				if( (tmp2&0x100) == 0 ){
					m_GeneralIODlg.m_bPort9OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort9OUTValue = TRUE;
				}
			}
			//port10����
			if( (tmp1&0x200) == 0 ){
				//����
				m_GeneralIODlg.m_bIsPort10IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio10 = 0;
			}
			else{
				//���
				m_GeneralIODlg.m_bIsPort10IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio10 = 1;
				if( (tmp2&0x200) == 0 ){
					m_GeneralIODlg.m_bPort10OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort10OUTValue = TRUE;
				}
			}
			//����ͼ��ɼ���ʽ
			switch( cRcvBuf[15] ){
				case RGB_ARRAY:
					m_bIsRGBArray = TRUE;
					m_bIsYUVArray = FALSE;
					m_bIsBAYERArray = FALSE;
					break;
				case YUV_ARRAY:
					m_bIsRGBArray = FALSE;
					m_bIsYUVArray = TRUE;
					m_bIsBAYERArray = FALSE;
					break;
				case BAYER_ARRAY:
					m_bIsRGBArray = FALSE;
					m_bIsYUVArray = FALSE;
					m_bIsBAYERArray = TRUE;
					break;
				default:
					break;
			}
			//��������ģʽ
			switch( cRcvBuf[14] ) {
				case TrgMode:
					m_bIsTrgMode = TRUE;
					_beginthread(RecvTrgImagedata,0,(LPVOID)this);
					break;
				case FconMode:
					m_bIsFconMode = TRUE;
					_beginthread(RecvConImagedata,0,(LPVOID)this);
					break;
				case HfrMode:
					m_bIsHFRMode = TRUE;
					_beginthread(RecvHfconImagedata,0,(LPVOID)this);
					break;
				case HfrBinMode:
					m_bIsHFRBinMode = TRUE;
					_beginthread(RecvHfBinconImagedata,0,(LPVOID)this);
					break;
				default:
					break;
			}
			//���ð�ƽ�⻷��ѡ��Ľ�����ʾ
			switch( cRcvBuf[13] ) {
				case DAYLIGHT:
					m_WhiteBalanceDlg.m_uiWBType = 0;
					break;
				case FLUORESCENCE:
					m_WhiteBalanceDlg.m_uiWBType = 1;
					break;
				case WITHOUTWB:
					m_WhiteBalanceDlg.m_uiWBType = 2;
					break;
				default:
					break;
				}
			//���������ͬ�����״̬
			if( cRcvBuf[8] ){
				m_GeneralIODlg.m_bIsFlashSynEnable = TRUE;
			}
			else{
				m_GeneralIODlg.m_bIsFlashSynEnable = FALSE;
			}
			//�����Ƿ�����ģʽ�л�������ģʽ���Ƿ������Զ�����ץȡһ��ͼ��
			if( cRcvBuf[9] ){
				m_bAutoTrgEnable = TRUE;
			}
			else{
				m_bAutoTrgEnable = FALSE;
			}
			//�����Ƿ�ʹ��ͼ��LUTӳ��任
			if( cRcvBuf[10] ){
				m_bLUTEnable = TRUE;
			}
			else{
				m_bLUTEnable = FALSE;
			}
			//���ü̵����Ƿ�պ�
			if( cRcvBuf[11] ){
				m_bRelayIsOpen = FALSE;
			}
			else{
				m_bRelayIsOpen = TRUE;
			}
			//�����Ƿ�ʹ�ܿ��Ź������ÿ��Ź���ʱ��ʱ�������ʾֵ
			if( cRcvBuf[12] ){
				m_WDTDlg.m_bIsWDTEnable = TRUE;
			}
			else{
				m_WDTDlg.m_bIsWDTEnable = FALSE;
			}
			m_WDTDlg.m_iWDT = *(unsigned int*)(cRcvBuf+4);
			//���ô���������ź����������ʾֵ
			m_AfeConfigDlg.m_fAfeVGAGain = *(float*)(cRcvBuf);
			//////////////////////////////////////////////////////////////////////////
			//����������Ĵ��ڶ���,֪ͨ����������״̬��
			pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
			m_hMainFrameWnd = pMain->GetSafeHwnd();
			PostMessage(m_hMainFrameWnd, WM_SWICHNEWCAMERA, 0, 0);
		}
		else{
			m_bIsEnsureTgtIP = FALSE;
			AfxMessageBox("����Ŀ�����ʧ�ܣ���˶�Ŀ�������IP��ַ");
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//						�������ģʽ����ؿ���
//////////////////////////////////////////////////////////////////////////
//�л����������ģʽ
void CEagleeye2_ViewerDoc::OnOmcTrigger() 
{
	m_bIsTrgMode	= TRUE;
	m_bIsFconMode	= FALSE;
	m_bIsHFRMode	= FALSE;
	m_bIsHFRBinMode = FALSE;
	//���������л����������ģʽ
	cmdbuf[0] = 0x01;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x01;
	cmdbuf[6] = 0x00;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
	else{
		_beginthread( RecvTrgImagedata, 0, (LPVOID)this);
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOmcTrigger(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
	pCmdUI->SetCheck(m_bIsTrgMode);
}
//��������ڴ���ģʽ�µĹ���״̬
void CEagleeye2_ViewerDoc::OnTrgmodeConfig() 
{
	if(m_TriggerModeCfgDlg.DoModal()==IDOK){
		int cmdlen = 0;
		//����ģʽ�µ��������
		cmdbuf[0] = 0x04;
		cmdbuf[1] = 0x00;//����Ҫ����ش�����
		//ץ�ĵص�
		cmdlen += strlen(m_TriggerModeCfgDlg.m_strLocation);
		if( cmdlen>0 ){
			memcpy( cmdbuf+6, m_TriggerModeCfgDlg.m_strLocation, cmdlen);
		}
		memset( cmdbuf+6+cmdlen, '\0', 101-cmdlen );
		cmdlen = 101;
		//������
		cmdbuf[cmdlen+6] = m_TriggerModeCfgDlg.m_iWayIndex;
		cmdlen += 1;
		//������ʽ
		if( m_TriggerModeCfgDlg.m_bIsSingalCoin ){
			cmdbuf[cmdlen+6] = 1;
		}
		else{
			cmdbuf[cmdlen+6] = 0;
		}
		cmdlen += 1;
		//��Ϊ˫��Ȧ��������Ȧ���
		*(float*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_fInterval;
		cmdlen += 4;
		//�����������
		if( m_TriggerModeCfgDlg.m_bIsManualCtrl ){
			cmdbuf[cmdlen+6] = 0;
		}
		else{
			cmdbuf[cmdlen+6] = 1;
		}
		cmdlen += 1;
		//�ֶ�ģʽ�µĿ�������
		*(UINT*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_uiTrgShutterTime;
		cmdlen += 4;
		//�ֶ�ģʽ�µ���������
		*(float*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_fTrgPga;
		cmdlen += 4;
		//�Զ�ģʽ�µĿ�����������
		*(UINT*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_iShutLimit;
		cmdlen += 4;
		//�Զ�ģʽ�µ�������������
		*(float*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_fPGALimit;
		cmdlen += 4;
		//�Զ�ģʽ�µĵ�ǰ��������
		*(UINT*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_iCurShut;
		cmdlen += 4;
		//�Զ�ģʽ�µĵ�ǰ��������
		*(float*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_fCurPga;
		cmdlen += 4;
		//�Զ�ģʽ�µĲ��������ʼ��
		*(UINT*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_iSRow;
		cmdlen += 4;
		//�Զ�ģʽ�µĲ��������ʼ��
		*(UINT*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_iSCol;
		cmdlen += 4;
		//�Զ�ģʽ�µĲ��������ֹ��
		*(UINT*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_iERow;
		cmdlen += 4;
		//�Զ�ģʽ�µĲ��������ֹ��
		*(UINT*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_iECol;
		cmdlen += 4;
		//�Զ�ģʽ�µĲ����������ƽ������
		cmdbuf[cmdlen+6] = m_TriggerModeCfgDlg.m_byYMean;
		cmdlen += 1;
		//�Զ�ģʽ���Ƿ������Բ�ⴥ��
		if(m_TriggerModeCfgDlg.m_bEnableADLI){
			cmdbuf[cmdlen+6] = 1;
		}
		else{
			cmdbuf[cmdlen+6] = 0;
		}
		cmdlen += 1;
		//�Զ�ģʽ�����Բ�ⴥ������
		cmdbuf[cmdlen+6] = m_TriggerModeCfgDlg.m_iADLIP;
		cmdlen += 1;
		//�Ƿ񱣴����
		if(m_TriggerModeCfgDlg.m_bSaveParam){
			cmdbuf[cmdlen+6] = 1;
		}
		else{
			cmdbuf[cmdlen+6] = 0;
		}
		cmdlen += 1;
		//����������������ڴ���ģʽ�µ��ع�ʱ��
		cmdbuf[0] = 0x04;
		cmdbuf[1] = 0x00;//����Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = cmdlen;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("�������������ʧ��");
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateTrgmodeConfig(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsTrgMode&&m_bIsEnsureTgtIP);
}
//�������ץȡһ��ͼ��
void CEagleeye2_ViewerDoc::OnOpeGrab() 
{
	//������������ץȡһ��ͼ��
	cmdbuf[0] = 0x02;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x00;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeGrab(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsTrgMode&&m_bIsEnsureTgtIP);
}
//////////////////////////////////////////////////////////////////////////
//					���ȫ�ֱ�����ģʽ����ؿ���
//////////////////////////////////////////////////////////////////////////
//�л������ȫ�ֱ�������ģʽ
void CEagleeye2_ViewerDoc::OnOmcMjpeg() 
{
	BOOL	bStopTrgSvr = FALSE;

	if( m_bIsTrgMode ){
		bStopTrgSvr = TRUE;
	}
	m_bIsTrgMode	= FALSE;
	m_bIsFconMode	= TRUE;
	m_bIsHFRMode	= FALSE;
	m_bIsHFRBinMode = FALSE;
	//�Ƿ�ֹͣ����˴���ģʽ�ķ���
	if( bStopTrgSvr ){
		//����ֹͣ����ģʽ�·���ı�־
		cmdbuf[0] = 0x21;
		cmdbuf[1] = 0x00;//����Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = 0x00;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("�������������ʧ��");
			return;
		}
	}
	Sleep( 200 );
	//�л������ȫ�ֱ�������ģʽ
	cmdbuf[0] = 0x01;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 1;
	cmdbuf[6] = 0x01;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
	else{
		_beginthread(RecvConImagedata,0,(LPVOID)this);
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOmcMjpeg(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
	pCmdUI->SetCheck(m_bIsFconMode);
}
//���������ȫ�ֱ�����ģʽ�µĹ���״̬
void CEagleeye2_ViewerDoc::OnMjpegmodeConfig() 
{
	if(m_MJPEGModeCfgDlg.DoModal()==IDOK){
		if( m_MJPEGModeCfgDlg.m_bIsManual){
			//���������ȫ�ֱ�����ģʽ��Ϊ�ֶ�����ģʽ
			cmdbuf[0] = 0x0f;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = 0;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
			//���������ȫ�ֱ�����ģʽ���ֶ����з�ʽ�µ��ع�ʱ��
			cmdbuf[0] = 0x05;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x03;
			*(UINT*)(cmdbuf+6) = m_MJPEGModeCfgDlg.m_uiMJPEGShutterTime;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
		}
		else{
			//���������ȫ�ֱ�����ģʽ��Ϊ�Զ�����ģʽ
			cmdbuf[0] = 0x0f;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = 1;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
			//���������ȫ�ֱ�����ģʽ�µ��ź������Զ����ڵ���������
			cmdbuf[0] = 0x13;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x04;
			*(float*)(cmdbuf+6) = m_MJPEGModeCfgDlg.m_fPGAUpLimit;
			m_HFRModeCfgDlg.m_fPGAUpLimit = m_MJPEGModeCfgDlg.m_fPGAUpLimit;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
			//���������ȫ�ֱ�����ģʽ�µ��ع�ʱ���Զ����ڵ���������
			cmdbuf[0] = 0x10;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x03;
			*(UINT*)(cmdbuf+6) = m_MJPEGModeCfgDlg.m_uiAutoShutterUpLimit;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
			//���������ȫ�ֱ�����ģʽ�µ�ͼ��ƽ��������ֵ
			cmdbuf[0] = 0x14;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = m_MJPEGModeCfgDlg.m_uiBrightnessTH;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateMjpegmodeConfig(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsFconMode&&m_bIsEnsureTgtIP);
}
//////////////////////////////////////////////////////////////////////////
//					���������ģʽ����ؿ���
//////////////////////////////////////////////////////////////////////////
//�л������������ģʽ
void CEagleeye2_ViewerDoc::OnOmcHfr() 
{
	BOOL	bStopTrgSvr = FALSE;

	if( m_bIsTrgMode ){
		bStopTrgSvr = TRUE;
	}
	m_bIsTrgMode	= FALSE;
	m_bIsFconMode	= FALSE;
	m_bIsHFRMode	= TRUE;
	m_bIsHFRBinMode = FALSE;
	//�Ƿ�ֹͣ����˴���ģʽ�ķ���
	if( bStopTrgSvr ){
		//����ֹͣ����ģʽ�·���ı�־
		cmdbuf[0] = 0x21;
		cmdbuf[1] = 0x00;//����Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = 0x00;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("�������������ʧ��");
			return;
		}
	}
	//�л������������ģʽ
	cmdbuf[0] = 0x01;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 1;
	cmdbuf[6] = 0x02;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
	else{
		_beginthread(RecvHfconImagedata,0,(LPVOID)this);
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOmcHfr(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
	pCmdUI->SetCheck(m_bIsHFRMode);
}
//��������ڸ�����ģʽ�µĹ���״̬
void CEagleeye2_ViewerDoc::OnHfrmodeConfig() 
{
	if(m_HFRModeCfgDlg.DoModal()==IDOK){
		if( m_HFRModeCfgDlg.m_bIsManual){
			//��������ڸ�����ģʽ��Ϊ�ֶ�����ģʽ
			cmdbuf[0] = 0x11;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = 0;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
			//��������ڸ�����ģʽ���ֶ����з�ʽ�µ��ع�ʱ��
			cmdbuf[0] = 0x06;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x03;
			*(UINT*)(cmdbuf+6) = m_HFRModeCfgDlg.m_uiHFRShutterTime;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
		}
		else{
			//��������ڸ�����ģʽ��Ϊ�Զ�����ģʽ
			cmdbuf[0] = 0x11;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = 1;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
			//��������ڸ�����ģʽ�µ��ź������Զ����ڵ���������
			cmdbuf[0] = 0x13;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x04;
			*(float*)(cmdbuf+6) = m_HFRModeCfgDlg.m_fPGAUpLimit;
			m_MJPEGModeCfgDlg.m_fPGAUpLimit = m_HFRModeCfgDlg.m_fPGAUpLimit;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
			//��������ڸ�����ģʽ�µ��ع�ʱ���Զ����ڵ���������
			cmdbuf[0] = 0x12;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x03;
			*(UINT*)(cmdbuf+6) = m_HFRModeCfgDlg.m_uiAutoShutterUpLimit;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
			//��������ڸ�����ģʽ�µ�ͼ��ƽ��������ֵ
			cmdbuf[0] = 0x15;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = m_HFRModeCfgDlg.m_uiBrightnessTH;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateHfrmodeConfig(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsHFRMode&&m_bIsEnsureTgtIP);
}
//////////////////////////////////////////////////////////////////////////
//					��������ʸ�������ģʽ����ؿ���
//////////////////////////////////////////////////////////////////////////
//�л�����������ʸ�������ģʽ
void CEagleeye2_ViewerDoc::OnOmcHfrb() 
{
	BOOL	bStopTrgSvr = FALSE;

	if( m_bIsTrgMode ){
		bStopTrgSvr = TRUE;
	}
	m_bIsTrgMode	= FALSE;
	m_bIsFconMode	= FALSE;
	m_bIsHFRMode	= FALSE;
	m_bIsHFRBinMode = TRUE;
	//�Ƿ�ֹͣ����˴���ģʽ�ķ���
	if( bStopTrgSvr ){
		//����ֹͣ����ģʽ�·���ı�־
		cmdbuf[0] = 0x21;
		cmdbuf[1] = 0x00;//����Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = 0x00;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("�������������ʧ��");
			return;
		}
	}
	//�л������������ģʽ
	cmdbuf[0] = 0x01;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 1;
	cmdbuf[6] = 0x03;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
	else{
		_beginthread(RecvHfBinconImagedata,0,(LPVOID)this);
	}
}

void CEagleeye2_ViewerDoc::OnUpdateOmcHfrb(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
	pCmdUI->SetCheck(m_bIsHFRBinMode);
}

void CEagleeye2_ViewerDoc::OnHfrbinmodeConfig() 
{
	if(m_HFRBINModeCfgDlg.DoModal()==IDOK){
		if( m_HFRBINModeCfgDlg.m_bIsManual){
			//��������ڸ�����ģʽ��Ϊ�ֶ�����ģʽ
			cmdbuf[0] = 0x25;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = 0;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
			//��������ڸ�����ģʽ���ֶ����з�ʽ�µ��ع�ʱ��
			cmdbuf[0] = 0x24;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x03;
			*(UINT*)(cmdbuf+6) = m_HFRBINModeCfgDlg.m_uiHFRBINShutterTime;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
		}
		else{
			//��������ڸ�����ģʽ��Ϊ�Զ�����ģʽ
			cmdbuf[0] = 0x25;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = 1;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
			//��������ڸ�����ģʽ�µ��ź������Զ����ڵ���������
			cmdbuf[0] = 0x13;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x04;
			*(float*)(cmdbuf+6) = m_HFRBINModeCfgDlg.m_fPGAUpLimit;
			m_MJPEGModeCfgDlg.m_fPGAUpLimit = m_HFRBINModeCfgDlg.m_fPGAUpLimit;
			m_HFRModeCfgDlg.m_fPGAUpLimit = m_HFRBINModeCfgDlg.m_fPGAUpLimit;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
			//��������ڸ�����ģʽ�µ��ع�ʱ���Զ����ڵ���������
			cmdbuf[0] = 0x26;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x03;
			*(UINT*)(cmdbuf+6) = m_HFRBINModeCfgDlg.m_uiAutoShutterUpLimit;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
			//��������ڸ�����ģʽ�µ�ͼ��ƽ��������ֵ
			cmdbuf[0] = 0x27;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = m_HFRBINModeCfgDlg.m_uiBrightnessTH;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
		}
	}
}

void CEagleeye2_ViewerDoc::OnUpdateHfrbinmodeConfig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsHFRBinMode&&m_bIsEnsureTgtIP);
}

//////////////////////////////////////////////////////////////////////////
//					�������������
//////////////////////////////////////////////////////////////////////////
//��������Ŀ�������IP��ַ���������������������µ�IP��ַ����
void CEagleeye2_ViewerDoc::OnConfigNewip() 
{
	int					IPAddress;
	BYTE				tmp1,tmp2,tmp3,tmp4;
	CString				macadd;

	if(m_NewIPMACDlg.DoModal()==IDOK){
		tmp1 = m_NewIPMACDlg.m_byIPAdd1; tmp2 = m_NewIPMACDlg.m_byIPAdd2; tmp3 = m_NewIPMACDlg.m_byIPAdd3; tmp4 = m_NewIPMACDlg.m_byIPAdd4;
		IPAddress =	tmp1;
		IPAddress |=	tmp2 << 8;
		IPAddress |=	tmp3 << 16;
		IPAddress |=	tmp4 << 24;
		//����Ŀ���������IP��ַ
		cmdbuf[0] = 0x1a;
		cmdbuf[1] = 0x00;//����Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = 0x04;
		*(UINT*)(cmdbuf+6) = IPAddress;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("�������������ʧ��");
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateConfigNewip(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//��������Ŀ�������MAC��ַ���������������������µ�MAC��ַ����
void CEagleeye2_ViewerDoc::OnConfigNewmac() 
{
	CString				macadd;
	int					i,j;

	if(m_MACCfgDlg.DoModal()==IDOK){
		//Set new MAC Address
		macadd = m_MACCfgDlg.m_strMacAdd4 + m_MACCfgDlg.m_strMacAdd5 + m_MACCfgDlg.m_strMacAdd6;
		i = macadd.GetLength();
		if( i!=6 ){
			if( i==0 ){
				return;
			}
			AfxMessageBox("MAC address is invaliable!");
			return;
		}
		for(i=0; i<3; i++){
			unsigned char tmp[2];
			for(j=0;j<2;j++){
				if( (macadd[i*2+j]>=48&&macadd[i*2+j]<=57) || (macadd[i*2+j]>=65&&macadd[i*2+j]<=70) || (macadd[i*2+j]>=97&&macadd[i*2+j]<=102)){
					if(macadd[i*2+j]>=48&&macadd[i*2+j]<=57){
						tmp[j] = macadd[i*2+j] - 48;
					}
					if(macadd[i*2+j]>=65&&macadd[i*2+j]<=70){
						tmp[j] = macadd[i*2+j] - 55;
					}
					if(macadd[i*2+j]>=97&&macadd[i*2+j]<=102){
						tmp[j] = macadd[i*2+j] - 87;
					}
				}
				else{
					AfxMessageBox("MAC address is invaliable!");
					return;
				}
			}
			cmdbuf[i+9] = (tmp[0]*16 + tmp[1]);
		}
		//���������޸�Ŀ�������MAC��ַ
		cmdbuf[0] = 0x1b;
		cmdbuf[1] = 0x00;//����Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = 0x06;
		cmdbuf[6] = 0x08; cmdbuf[7] = 0x00; cmdbuf[8] = 0x28;	//MAC code for TI
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("�������������ʧ��");
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateConfigNewmac(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//CCD�źŵ��������(0 - 35.5dB �ɵ�)
void CEagleeye2_ViewerDoc::OnConfigAfegainctrl() 
{
	if(m_AfeConfigDlg.DoModal()==IDOK){
		//���ô���������źŵ�����
		cmdbuf[0] = 0x07;
		cmdbuf[1] = 0x00;//����Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = 0x04;
		*(float*)(cmdbuf+6) = m_AfeConfigDlg.m_fAfeVGAGain;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("�������������ʧ��");
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateConfigAfegainctrl(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//��ƽ�����ѡ��
void CEagleeye2_ViewerDoc::OnConfigWhitebalance() 
{
	if(m_WhiteBalanceDlg.DoModal()==IDOK){
		//����Ŀ�������ƽ��У���Ĺ�������
		cmdbuf[0] = 0x08;
		cmdbuf[1] = 0x00;//����Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = 0x01;
		*(UINT*)(cmdbuf+6) = *(UINT*)(&m_WhiteBalanceDlg.m_uiWBType);
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("�������������ʧ��");
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateConfigWhitebalance(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//�ɱ��IO�ں������ͬ�����ʹ�ܿ���
void CEagleeye2_ViewerDoc::OnConfigGioconfig() 
{
	if(m_GeneralIODlg.DoModal()==IDOK){
		//����Ŀ�����ʮ���ɱ��I/O�ķ���
		cmdbuf[0] = 0x0d;
		cmdbuf[1] = 0x00;//����Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = 0x02;
		*(UINT*)(cmdbuf+6) = *(UINT*)(&m_GeneralIODlg.m_pioIndicator)&0x3ff;
		m_GeneralIODlg.m_bIsPortDirAdj = FALSE;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("�������������ʧ��");
		}
		//����Ŀ�����ʮ���ɱ��I/O�������ƽ�������������Ч
		cmdbuf[0] = 0x0e;
		cmdbuf[1] = 0x00;//����Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = 0x02;
		*(UINT*)(cmdbuf+6) = (*(UINT*)(&m_GeneralIODlg.m_pioIndicator)&0xffc00)>>10;
		m_GeneralIODlg.m_bIsPortOutAdj = FALSE;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("�������������ʧ��");
		}
		//����Ŀ������������ͬ������Ƿ�ʹ��
		cmdbuf[0] = 0x0c;
		cmdbuf[1] = 0x00;//����Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = 0x01;
		if( m_GeneralIODlg.m_bIsFlashSynEnable){
			*(UINT*)(cmdbuf+6) = 1;//ʹ��
		}
		else{
			*(UINT*)(cmdbuf+6) = 0;//��ֹ
		}
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("�������������ʧ��");
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateConfigGioconfig(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//����ڲ���ʵʱʱ������
void CEagleeye2_ViewerDoc::OnConfigRtcconfig() 
{
	CString strRTC;

	if(m_RTCConfigDlg.DoModal()==IDOK){
		//����Ŀ�������RTCʱ��
		cmdbuf[0] = 0x18;
		cmdbuf[1] = 0x00;//����Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = 0x07;
		cmdbuf[6] = m_RTCConfigDlg.m_iYear%100;
		cmdbuf[7] = m_RTCConfigDlg.m_iMonth;
		cmdbuf[8] = m_RTCConfigDlg.m_iDay;
		cmdbuf[9] = m_RTCConfigDlg.m_iWeek;
		cmdbuf[10] = m_RTCConfigDlg.m_iHour;
		cmdbuf[11] = m_RTCConfigDlg.m_iMinute;
		cmdbuf[12] = m_RTCConfigDlg.m_iSecond;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("�������������ʧ��");
		}
		//��ȡĿ�������RTCʱ��
		cmdbuf[0] = 0x19;
		cmdbuf[1] = 0x01;//��Ҫ����ش�����
		*(UINT*)(cmdbuf+2) = 0;
		if( !SendCMDtoDSP( cmdbuf ) ){
			AfxMessageBox("�������������ʧ��");
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateConfigRtcconfig(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//����Ŀ��Ź�����
void CEagleeye2_ViewerDoc::OnConfigWdtconfig() 
{
	if(m_WDTDlg.DoModal()==IDOK){
		
		if( m_WDTDlg.m_bIsWDTEnable ){
			//ʹ��Ŀ������Ŀ��Ź�
			cmdbuf[0] = 0x16;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x01;
			cmdbuf[6] = 0x01;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
			//����Ŀ������Ŀ��Ź���ʱ��(1~256��)
			cmdbuf[0] = 0x17;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x01;
			cmdbuf[6] = m_WDTDlg.m_iWDT;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
		}
		else{
			//��ֹĿ������Ŀ��Ź�
			cmdbuf[0] = 0x16;
			cmdbuf[1] = 0x00;//����Ҫ����ش�����
			*(UINT*)(cmdbuf+2) = 0x01;
			cmdbuf[6] = 0x00;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("�������������ʧ��");
			}
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateConfigWdtconfig(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//�������ץȡһ����ǰͼ���������Ӧ��ƽ��У��
void CEagleeye2_ViewerDoc::OnOpeAutowb() 
{
	//�������ץȡһ����ǰͼ���������Ӧ��ƽ��У��
	cmdbuf[0] = 0x09;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x00;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeAutowb(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//�����������������ģʽ�л�������ģʽ���Ƿ��Զ�����ץȡһ��ͼ��
void CEagleeye2_ViewerDoc::OnOpeAutotrgEnable() 
{
	//ʹ���л����Զ���������
	m_bAutoTrgEnable = TRUE;
	cmdbuf[0] = 0x1f;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 0x01;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeAutotrgEnable(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bAutoTrgEnable);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
void CEagleeye2_ViewerDoc::OnOpeAutotrgDisable() 
{
	//��ֹ�л����Զ���������
	m_bAutoTrgEnable = FALSE;
	cmdbuf[0] = 0x1f;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 0x00;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeAutotrgDisable(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_bAutoTrgEnable);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//�Ƿ�ʹ��ͼ��LUT�任����
void CEagleeye2_ViewerDoc::OnOpeLutEnable() 
{
	//ʹ��LUT�任����
	m_bLUTEnable = TRUE;
	cmdbuf[0] = 0x0a;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 0x01;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeLutEnable(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bLUTEnable);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
void CEagleeye2_ViewerDoc::OnOpeLutDisable() 
{
	//��ֹLUT�任����
	m_bLUTEnable = FALSE;
	cmdbuf[0] = 0x0a;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x02;
	cmdbuf[6] = 0x00;
	cmdbuf[7] = 0x00;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeLutDisable(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_bLUTEnable);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//�̵�������
void CEagleeye2_ViewerDoc::OnOpeRelayOpen() 
{
	//�Ͽ��̵���
	m_bRelayIsOpen = TRUE;
	cmdbuf[0] = 0x1d;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 0;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeRelayOpen(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bRelayIsOpen);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
void CEagleeye2_ViewerDoc::OnOpeRelayClose() 
{
	//�պϼ̵���
	m_bRelayIsOpen = FALSE;
	cmdbuf[0] = 0x1d;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 1;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeRelayClose(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_bRelayIsOpen);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//�������ͼ��ɼ���ʽΪRGB����
void CEagleeye2_ViewerDoc::OnOpeImgformatRgb() 
{
	//�������ͼ��ɼ���ʽΪRGB����
	m_bIsRGBArray = TRUE;
	m_bIsYUVArray = FALSE;
	m_bIsBAYERArray = FALSE;
	cmdbuf[0] = 0x20;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 0;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeImgformatRgb(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bIsRGBArray);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//�������ͼ��ɼ���ʽΪYUV����
void CEagleeye2_ViewerDoc::OnOpeImgformatYuv() 
{
	//�������ͼ��ɼ���ʽΪYUV����
	m_bIsRGBArray = FALSE;
	m_bIsYUVArray = TRUE;
	m_bIsBAYERArray = FALSE;
	cmdbuf[0] = 0x20;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 1;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeImgformatYuv(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bIsYUVArray);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//�������ͼ��ɼ���ʽΪBAYER����
void CEagleeye2_ViewerDoc::OnOpeImgformatBayer() 
{
	//�������ͼ��ɼ���ʽΪYUV����
	m_bIsRGBArray = FALSE;
	m_bIsYUVArray = FALSE;
	m_bIsBAYERArray = TRUE;
	cmdbuf[0] = 0x20;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 2;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeImgformatBayer(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bIsBAYERArray);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//���ô���ģʽ�Ŀ����ٶȵ��ֶ�ģʽ
void CEagleeye2_ViewerDoc::OnOpeTrgshutterctrlManual() 
{
	m_bTrgShutterIsManual = TRUE;
	cmdbuf[0] = 0x22;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 0;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeTrgshutterctrlManual(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bTrgShutterIsManual);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//���ô���ģʽ�Ŀ����ٶȵ��Զ�ģʽ
void CEagleeye2_ViewerDoc::OnOpeTrgshutterctrlAuto() 
{
	m_bTrgShutterIsManual = FALSE;
	cmdbuf[0] = 0x22;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 1;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeTrgshutterctrlAuto(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_bTrgShutterIsManual);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}

//��λ���
void CEagleeye2_ViewerDoc::OnOpeResetsys() 
{
	//Զ������Ŀ�����
	cmdbuf[0] = 0x00;
	cmdbuf[1] = 0x00;//����Ҫ����ش�����
	*(UINT*)(cmdbuf+2) = 0x00;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("�������������ʧ��");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeResetsys(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//Զ�̸����û�����
void CEagleeye2_ViewerDoc::OnOpeRemoteupdatedspprrom() 
{
	char CmdLine[]="EagleeyeDSPRemoteUpdate";
	UINT RES = WinExec(CmdLine, SW_SHOW);
}
void CEagleeye2_ViewerDoc::OnUpdateOpeRemoteupdatedspprrom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
