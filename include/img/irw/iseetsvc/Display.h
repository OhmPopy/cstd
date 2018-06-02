/********************************************************************

	Display.h

	----------------------------------------------------------------
    ������֤ �� GPL
	��Ȩ���� (C) 2003 VCHelp coPathway ISee workgroup.
	----------------------------------------------------------------
	��һ����������������������������������������GNU ͨ�ù�����
	��֤�������޸ĺ����·�����һ���򡣻��������֤�ĵڶ��棬���ߣ���
	�����ѡ�����κθ��µİ汾��

    ������һ�����Ŀ����ϣ�������ã���û���κε���������û���ʺ��ض�
	Ŀ�ص������ĵ���������ϸ����������GNUͨ�ù������֤��

    ��Ӧ���Ѿ��ͳ���һ���յ�һ��GNUͨ�ù������֤�ĸ�������Ŀ¼
	GPL.txt�ļ����������û�У�д�Ÿ���
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA
	----------------------------------------------------------------
	�������ʹ�ñ����ʱ��ʲô������飬�������µ�ַ������ȡ����ϵ��

			http://isee.126.com
			http://cosoft.org.cn/projects/iseeexplorer
			
	���ŵ���

			isee##vip.163.com
	----------------------------------------------------------------
	���ļ���;��	ͼ���д���棭C���԰棭VC���������������ͼ��
					�ඨ���ļ���

	���ļ���д�ˣ�	
					YZ				yzfree##sina.com

	���ļ��汾��	30112
	����޸��ڣ�	2003-1-12

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������

	������ʷ��

		2003-1		��һ�����԰淢��

********************************************************************/

#if !defined(AFX_DISPLAY_H__C43754E9_BE42_4A57_9F44_7D51DAC16819__INCLUDED_)
#define AFX_DISPLAY_H__C43754E9_BE42_4A57_9F44_7D51DAC16819__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Display.h : header file
//

// ��ʾ������������ͻ����߽�ļ��
enum {ISDISP_LEFT = 10, ISDISP_RIGHT = 10, ISDISP_TOP = 10, ISDISP_BOTTOM = 10};

enum {ISDISP_HEADER_H = 20};							// �����߶�
enum {ISDISP_HEADER_W = 60};							// �������
enum {ISCOL_HEADER_BG = 0xe7b29c};						// ����鱳��ɫ

enum {ISDISP_HISPEC   = 10};							// ����������ʾ���ļ��
enum {ISDISP_FRMSPEC  = 16};							// ������ͼ�����С���

enum {ISDISP_MIN_WIDTH= 160, ISDISP_MIN_HEIGHT = 160};	// ʹ��������С����Сͼ��ߴ�

enum {ISDISP_MAXWIDTH = 128000};						// ����ͼ��������
enum {ISDISP_LINEBUFFSIZE = ISDISP_MAXWIDTH*4};			// ɨ���л������ߴ�
enum {ISDISP_MAXBUFFSIZE = 10*IRWE_MB};					// ��ʾ�������ߴ�

enum {ISDISP_SWH_FAST, ISDISP_SWH_STYLE};				// �����л���ʽ

enum {ISDISP_MINBTN_WIDTH = 60};						// ��ť������֮�����С���
enum {ISDISP_BTN_W = 17, ISDISP_BTN_H = 17};
enum {ISDISP_BTN_SPEC = 4};

enum {ISDISP_TREE_MIN = 0, ISDISP_LISTSS = 100};		// �ۿ�ģʽ�����ӡ��б��ӿ��
enum {ISDISP_NOL = 0, ISDISP_VIEWMODL = 1};				// ����ģʽ�����ģʽ����

enum {ISDISP_MAXANI_TIME = 10000, ISDISP_MINANI_TIME = 10};	// �Զ����ŵ��ӳ�ʱ������


#define ISDBTNID_DISPTYPE	200							// ��ťID
#define ISDBTNID_RS			201

#define ISDWHEEL_DELTA		40							// �����ƶ�ֵ�ķŴ���

#define	MAX_PALETTE			256

typedef struct _tagBITMAPINFO_EX
{
	wBITMAPINFOHEADER	info;
	wRGBQUAD				pal[MAX_PALETTE];
} BITMAPINFO_EX, *PBITMAPINFO_EX;


#define DPTRS_MIN			((float)0.1)				// ��С���ű���
#define DPTRS_MAX			((float)8.0)				// ������ű���
#define DPTRS_STEP			((float)0.4)				// ���ű�������ֵ
#define DPTRS_REAL			((float)1.0)				// ԭʼ�ߴ�����ű���
#define DPTRS_DEFAULT		DPTRS_REAL					// ȱʡ���ű�����ԭʼ�ߴ磩




/////////////////////////////////////////////////////////////////////////////
// CDisplay view

class CDisplay : public CView
{
public:
	enum {DPT_LEFTTOP, DPT_RIGHTTOP, DPT_LEFTBOTTOM, DPT_RIGHTBOTTOM, DPT_CENTER};

protected:
	CDisplay();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDisplay)

// Attributes
public:
	HDRAWDIB	m_hDrawDib;								// DrawDib���
	CString		m_sImgInfo;								// ͼ�������Ϣ���ַ���

	int			m_iTalAutResize;						// ȫ���Զ����ű�־
	float		m_fTalRatio;							// ȫ�����ű���
	int			m_iAutoResize;							// ��ǰ�Զ����ű�־
	float 		m_fRatio;								// ��ǰ���ű���
	int			m_iPassMark;							// �Ŵ����ñ�־

	int			m_iInitPos;								// ͼ��ĳ�ʼ����λ��(���ϡ����¡����ϡ����¡�����)
	int			m_iSwitch;								// ͼ���л��ķ�ʽ(���١����)

	CRect		m_rcBmpRc;								// ͼ��ԭʼ�ߴ�

	CRect		m_rcPict;								// ����ͼ������ųߴ�
	CRect		m_rcPTRc;								// ͼ��Ŀͻ�����
	CRect		m_rcFrme;								// ��ΧͼƬ�����ο�ĳߴ�
	CRect		m_rcFTRc;								// ���ο�Ŀͻ�����

	CRect		m_rcClient;								// �ͻ�������
	CRect		m_rcHeader;								// ��Ϣ������
	CRect		m_rcImage;								// ͼ��������
	
	int			m_iCurrFr;								// ��ǰ��ʾͼ���֡��(0-������0-��)
	int			m_iInfo;								// ǰһ֡ͼ��Ĵ��÷�ʽ����
	
	CPoint		m_cpPrePt;								// �϶�״̬��ǰһ���������
	BOOL		m_bMoveing;								// �϶���־

	CBitmap		m_cbBackupBmp;							// ����λͼ

	int			m_icbTrueState;							// ��ť�ɼ���־
	int			m_icbResize;
	CButtonST	m_cbTrue;								// �Զ���������ʵ�ߴ���л���ť
	CButtonST	m_cbResize;								// �ֶ����Ű�ť

	HCURSOR		m_hcHSd;								// ��׼��꼰���ι��
	HCURSOR		m_hcHFr;
	HCURSOR		m_hcHOd;
	HCURSOR		m_hcHPs;								// �Ŵ󾵹��

	char		*m_pLineBuf;							// ͼ��ת������
	char		*m_pDispBuf;							// ��ʾ������
	ulong	m_lBuffSize;						// ��ʾ��������ǰ�ߴ�

	CTimeCounter	*m_cCurrPackCnt;					// ��ͼ���ʱ��

	CWinThread		*m_cAniThread;						// ������ʱ�߳�
	BOOL			m_bDiableMark;						// ���������ǩ

	int				m_iViewModl;						// ���ģʽ��־
	WINDOWPLACEMENT	m_wp;
	int				m_iOTW, m_iORW, m_iODW, m_iOLW;
	
	
// Operations
public:
	void	DrawHeader(CDC *pDC, int ord);
	void	DrawImage(CDC *pDC);

	void	SetDispLocInfo(void);
	void	SetFrmeLocInfo(BOOL init=FALSE);
	void	SetPictLocInfo(BOOL init=FALSE);

	BOOL	IsDrop(void);

	void	ClearDispPosInfo(void);

	static UINT AniProc(LPVOID parm);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplay)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDisplay();

	BOOL	IsCurrPack(LPSPEC_DATAPACK psdp);

	void	_RSS(CPoint point);
	void	_RSL(CPoint point);

	void	_UpdateImageRgn(CRect& rect);
	void	_UpdateHeadRgn(BOOL rgn);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDisplay)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnDispIntoPlug(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispBuildMi(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispBuildSi(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispProcess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispOver(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispOK(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispFaild(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispAniNext(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnBtn1();
	afx_msg void OnBtn2();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAY_H__C43754E9_BE42_4A57_9F44_7D51DAC16819__INCLUDED_)
