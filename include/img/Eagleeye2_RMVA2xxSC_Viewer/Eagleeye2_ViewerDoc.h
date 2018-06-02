// Eagleeye2_ViewerDoc.h : interface of the CEagleeye2_ViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EAGLEEYE2_VIEWERDOC_H__919511EB_33A7_4E3A_BB57_64C701E5FF97__INCLUDED_)
#define AFX_EAGLEEYE2_VIEWERDOC_H__919511EB_33A7_4E3A_BB57_64C701E5FF97__INCLUDED_

#include ".\INCLUDE\PIC\Image.h"	// Added by ClassView
#include "TargetIPAddressDlg.h"
#include "NewIPMACDlg.h"
#include "AfeConfigDlg.h"
#include "WhiteBalanceDlg.h"
#include "GeneralIODlg.h"
#include "TriggerModeCfgDlg.h"
#include "MJPEGModeCfgDlg.h"
#include "HFRModeCfgDlg.h"
#include "RTCConfigDlg.h"
#include "WDTDlg.h"
#include "MACCfgDlg.h"
#include "HFRBINModeCfgDlg.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_RECVTRGIMAGEOK	WM_USER + 5	//message variable for notice view after recv data
#define WM_SWICHNEWCAMERA	WM_USER + 6	//message variable for notice Mainframe after switch new camera

#define IMGWIDTH	1616				//horizontal resolution of image
#define	IMGHEIGHT	1232				//vertical resolution of image
#define IMGLENGTH	IMGWIDTH*IMGHEIGHT	//resolution of image

#define NUM_BUFF	5					//buffer number for motion jpeg 
#define	MAX_BUFF	IMGWIDTH*IMGHEIGHT*2//Size of each buffer loading MJpeg

//struct of buffer node for loading MJPEG
typedef struct tag_BuffNode {
	int					sizeBuff;
	char				pBuff[MAX_BUFF];
	BOOL				bIsLocked;
} BUFFNODE;
//ץ����Ϣ
typedef struct ImgInfStruct{
	char	cLocusInf[101];		//ץ�ĵص���Ϣ����50������
	BYTE	byTimeInf[7];			//ץ��ʱ�䣬��-��-��-ʱ-��-��
	BYTE	byWayNum;				  //������
	BYTE	byTrgMode;				//������ʽ��0->����;1->����Ȧ;2->˫��Ȧ(����)
	float	fSpeed;					  //����(��λ:����/Сʱ)
}EagleEye2ImgInfStruct;
//�������������ģʽ,ͼ��ɼ���ʽ�Ͱ�ƽ����ջ�����ö�ٱ�������
enum EagleEye2_SensorRunMode{ TrgMode, FconMode, HfrMode, HfrBinMode };
enum EagleEye2_ImgAcquisition_Format{ RGB_ARRAY, YUV_ARRAY, BAYER_ARRAY };
enum EagleEye2_WhiteBalance_environment { DAYLIGHT, FLUORESCENCE, WITHOUTWB };

//////////////////////////////////////////////////////////////////////////
//class 
class CGeneralIODlg;

//////////////////////////////////////////////////////////////////////////
class CEagleeye2_ViewerDoc : public CDocument
{
protected: // create from serialization only
	CEagleeye2_ViewerDoc();
	DECLARE_DYNCREATE(CEagleeye2_ViewerDoc)

// Attributes
public:
	FCImage					m_fPic;				//FCImage�����
	FCImage					m_fPic1;			//FCImage�����
	FCBitmap				m_DDB;				//FCBitmap�����
	FCDib					m_Dib;				//FCDib�����
	BOOL					m_bFirstShow;		//��ͼ�Ƿ��ǵ�һ����ʾ
	volatile HWND			m_hViewWnd;			//��ͼ�Ĵ��ھ������������ͼ�෢����Ϣ
	volatile HWND			m_hMainFrameWnd;	//����ܵĴ��ھ����������������෢����Ϣ
	BOOL					m_bIsDataFromCamera;//��ǰͼ���Ƿ����������
	BOOL					m_bIsJpeg;			//��ʾ��ǰͼ���Ƿ���JPG��ʽ
	BOOL					m_bIsHighFR;		//��־��ǰ��Ҫ��ʾ��ͼ���Ƿ����Ը�����ģʽ���Դ���������������ģʽ��������OnDraw�����н�����ʾ����
	//�����Ӵ��ڵĶ���
	CTargetIPAddressDlg		m_TargetIPDlg;
	CNewIPMACDlg			m_NewIPMACDlg;
	CAfeConfigDlg			m_AfeConfigDlg;
	CWhiteBalanceDlg		m_WhiteBalanceDlg;
	CGeneralIODlg			m_GeneralIODlg;
	CTriggerModeCfgDlg		m_TriggerModeCfgDlg;
	CMJPEGModeCfgDlg		m_MJPEGModeCfgDlg;
	CHFRModeCfgDlg			m_HFRModeCfgDlg;
	CHFRBINModeCfgDlg		m_HFRBINModeCfgDlg;
	CRTCConfigDlg			m_RTCConfigDlg;
	CWDTDlg					m_WDTDlg;
	CMACCfgDlg				m_MACCfgDlg;
	//һЩͨ�ÿ��Ʊ���
	BOOL					m_bIsTrgMode;			//�Ƿ��Ǵ���ģʽ
	BOOL					m_bIsFconMode;			//�Ƿ���ȫ�ֱ�������ģʽ
	BOOL					m_bIsHFRMode;			//�Ƿ��Ǹ�����ģʽ
	BOOL					m_bIsHFRBinMode;		//�Ƿ��Ǹ����ʸ�������ģʽ
	BOOL					m_bIsEnsureTgtIP;		//�Ƿ�ȷ��Ŀ�������IP��ַ
	BOOL					m_bRelayIsOpen;			//�̵����Ƿ�Ͽ�
	BOOL					m_bLUTEnable;			//����Ƿ�ʹ��ͼ���LUTӳ��任
	BOOL					m_bAutoTrgEnable;		//�л�������ģʽ���Ƿ������Զ�����ץȡһ��ͼ��
	BOOL					m_bIsRGBArray;			//����ɼ���ͼ���ʽ�Ƿ�ΪRGB��ʽ
	BOOL					m_bIsYUVArray;			//����ɼ���ͼ���ʽ�Ƿ�ΪYUV��ʽ
	BOOL					m_bIsBAYERArray;		//����ɼ���ͼ���ʽ�Ƿ�ΪBAYER��ʽ
	BOOL					m_bTrgShutterIsManual;	//����ģʽ�����ٶ��Ƿ����ֶ�ģʽ

// Operations
public:
	BOOL	SendCMDtoDSP( char* cmdbuf );

private:
	CGeneralIODlg	*m_pGeneralIODlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEagleeye2_ViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEagleeye2_ViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
public:
	//{{AFX_MSG(CEagleeye2_ViewerDoc)
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileOpen();
	afx_msg void OnConfigTargetip();
	afx_msg void OnOmcTrigger();
	afx_msg void OnUpdateOmcTrigger(CCmdUI* pCmdUI);
	afx_msg void OnTrgmodeConfig();
	afx_msg void OnUpdateTrgmodeConfig(CCmdUI* pCmdUI);
	afx_msg void OnOpeGrab();
	afx_msg void OnUpdateOpeGrab(CCmdUI* pCmdUI);
	afx_msg void OnOmcMjpeg();
	afx_msg void OnUpdateOmcMjpeg(CCmdUI* pCmdUI);
	afx_msg void OnMjpegmodeConfig();
	afx_msg void OnUpdateMjpegmodeConfig(CCmdUI* pCmdUI);
	afx_msg void OnOmcHfr();
	afx_msg void OnUpdateOmcHfr(CCmdUI* pCmdUI);
	afx_msg void OnHfrmodeConfig();
	afx_msg void OnUpdateHfrmodeConfig(CCmdUI* pCmdUI);
	afx_msg void OnConfigNewip();
	afx_msg void OnUpdateConfigNewip(CCmdUI* pCmdUI);
	afx_msg void OnConfigNewmac();
	afx_msg void OnUpdateConfigNewmac(CCmdUI* pCmdUI);
	afx_msg void OnConfigAfegainctrl();
	afx_msg void OnUpdateConfigAfegainctrl(CCmdUI* pCmdUI);
	afx_msg void OnConfigWhitebalance();
	afx_msg void OnUpdateConfigWhitebalance(CCmdUI* pCmdUI);
	afx_msg void OnConfigGioconfig();
	afx_msg void OnUpdateConfigGioconfig(CCmdUI* pCmdUI);
	afx_msg void OnConfigRtcconfig();
	afx_msg void OnUpdateConfigRtcconfig(CCmdUI* pCmdUI);
	afx_msg void OnConfigWdtconfig();
	afx_msg void OnUpdateConfigWdtconfig(CCmdUI* pCmdUI);
	afx_msg void OnOpeAutowb();
	afx_msg void OnUpdateOpeAutowb(CCmdUI* pCmdUI);
	afx_msg void OnOpeAutotrgEnable();
	afx_msg void OnUpdateOpeAutotrgEnable(CCmdUI* pCmdUI);
	afx_msg void OnOpeAutotrgDisable();
	afx_msg void OnUpdateOpeAutotrgDisable(CCmdUI* pCmdUI);
	afx_msg void OnOpeLutEnable();
	afx_msg void OnUpdateOpeLutEnable(CCmdUI* pCmdUI);
	afx_msg void OnOpeLutDisable();
	afx_msg void OnUpdateOpeLutDisable(CCmdUI* pCmdUI);
	afx_msg void OnOpeRelayOpen();
	afx_msg void OnUpdateOpeRelayOpen(CCmdUI* pCmdUI);
	afx_msg void OnOpeRelayClose();
	afx_msg void OnUpdateOpeRelayClose(CCmdUI* pCmdUI);
	afx_msg void OnOpeResetsys();
	afx_msg void OnUpdateOpeResetsys(CCmdUI* pCmdUI);
	afx_msg void OnOpeRemoteupdatedspprrom();
	afx_msg void OnUpdateOpeRemoteupdatedspprrom(CCmdUI* pCmdUI);
	afx_msg void OnOpeImgformatRgb();
	afx_msg void OnUpdateOpeImgformatRgb(CCmdUI* pCmdUI);
	afx_msg void OnOpeImgformatYuv();
	afx_msg void OnUpdateOpeImgformatYuv(CCmdUI* pCmdUI);
	afx_msg void OnOpeImgformatBayer();
	afx_msg void OnUpdateOpeImgformatBayer(CCmdUI* pCmdUI);
	afx_msg void OnOpeTrgshutterctrlManual();
	afx_msg void OnUpdateOpeTrgshutterctrlManual(CCmdUI* pCmdUI);
	afx_msg void OnOpeTrgshutterctrlAuto();
	afx_msg void OnUpdateOpeTrgshutterctrlAuto(CCmdUI* pCmdUI);
	afx_msg void OnOmcHfrb();
	afx_msg void OnUpdateOmcHfrb(CCmdUI* pCmdUI);
	afx_msg void OnHfrbinmodeConfig();
	afx_msg void OnUpdateHfrbinmodeConfig(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EAGLEEYE2_VIEWERDOC_H__919511EB_33A7_4E3A_BB57_64C701E5FF97__INCLUDED_)
