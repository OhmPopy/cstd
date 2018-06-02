/********************************************************************

	irwcpp.h

	
	���ļ���;��	ͼ���д����C++���԰�ӿڶ����ļ�

	���ļ���д�ˣ�	
					YZ			yzfree##sina.com

	���ļ��汾��	30831
	����޸��ڣ�	2003-08-31

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
	    ��ַ�ռ������
  	----------------------------------------------------------------
	������ʷ��

		2003-8		����Clear()�ӿں���
		2003-8		�������ز�������=������
		2003-8		GetInfo()��������CHAVIMG��Ϣ����
		2003-7		��һ�����԰淢��

********************************************************************/

#ifndef __SEEIMAGE_INC__
#define __SEEIMAGE_INC__

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <vfw.h>
#include <stdarg.h>
#include <assert.h>

#include "iseemem.h"
#include "iseeio.h"
#include "iseeimn.h"
#include "iseeirw.h"

#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CIrwcppApp

class CIrwcppApp : public CWinApp
{
public:
	CIrwcppApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIrwcppApp)
	public:
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIrwcppApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// ������������������

#define DEFAULTEXT						("bmp")

#define ISD_COMBOBOXLIST_HEIGHT			100
#define ISD_CTLCNT						(ISD_ITEM_NUM+1)

// �¿ؼ�ID����
#define IDC_SAVEASCNT_STC				2012
#define IDC_SAVEASCNT_CON				(IDC_SAVEASCNT_STC+ISD_ITEM_NUM+1)

// �б��湦�ܵĲ����Ϣ�ṹ
typedef struct _tagSavePluginInfo
{
	int				index;								// �������
	int				piid;								// ���ID
	char			str[64];							// ��ʾ��
	char			ext[32];							// ȱʡ��չ����
	IRW_SAVE_DESC	cont;								// ԭʼ�ı��湦������
} SAVEPLUGININFO, *LPSAVEPLUGININFO;


/////////////////////////////////////////////////////////////////////////////
// ����������
typedef enum
{
	CR_FILE		= ISEEIO_CONFER_LOC_FILE,				// �����ļ�����
	CR_FILEFAST	= ISEEIO_CONFER_LOC_FILE_SEQUENTIAL,	// �����ļ����ͣ����ٰ棩
	CR_MEMORY	= ISEEIO_CONFER_LOC_MEMORY,				// �����ڴ�����
	CR_SEEIMAGE	= ISEEIO_CONFER_MAX+1,					// ��һ��CSeeImage����
	CR_NIHILITY,										// ���ޣ���������
	CR_CLIP,											// ������
	CR_WINDOW,											// ����
	CR_RESOURCE_ID,										// ��Դ����ID��ʽ������
	CR_RESOURCE_NAME,									// ��Դ������Դ���ƴ���ʽ������
	CR_HBITMAP,											// HBITMAP���
	CR_HDIB												// HDIB���
} CSITYPER, *LPCSITYPER;

// д��������
typedef enum
{
	CW_FILE		= ISEEIO_CONFER_LOC_FILE,				// �����ļ�����
	CW_FILEFAST	= ISEEIO_CONFER_LOC_FILE_SEQUENTIAL,	// �����ļ����ͣ����ٰ棩
	CW_MEMORY	= ISEEIO_CONFER_LOC_MEMORY,				// �����ڴ�����
	CW_CLIP		= ISEEIO_CONFER_MAX+1,					// ������
	CW_HBITMAP,											// HBITMAP���
	CW_HDIB												// HDIB���
} CSITYPEW, *LPCSITYPEW;

// ���ƶ���
typedef enum
{
	CD_NON	= 0x1,										// ԭ�ߴ����
	CD_STL	= 0x2,										// ����
	CD_CEN	= 0x100,									// ����
	CD_TAN	= 0x200,									// ͸������
} CSIDRAW, *LPCSIDRAW;

// ת����ʶ
typedef enum
{
	COVER    = IRWE_CNVMARK_AUTO,						/* �Զ�תΪ��ƥ��ı�׼��ʽ��Ҳ�Ǳ䳤�����Ľ�β��־�� */
	CNVAUTO  = COVER,
	CNV1     = IRWE_CNVMARK_1,							/* ǿ��תΪ 1 λ��ɫ��ͼ */           
	CNV4     = IRWE_CNVMARK_4,							/* ǿ��תΪ 4 λ��ɫ��ͼ */           
	CNV8     = IRWE_CNVMARK_8,							/* ǿ��תΪ 8 λ��ɫ��ͼ */           
	CNV16555 = IRWE_CNVMARK_16_555,						/* ǿ��תΪ 16 λ(555)��ʽλͼ */     
	CNV16565 = IRWE_CNVMARK_16_565,						/* ǿ��תΪ 16 λ(565)��ʽλͼ */     
	CNV24    = IRWE_CNVMARK_24,							/* ǿ��תΪ 24 λλͼ */              
	CNV32    = IRWE_CNVMARK_32							/* ǿ��תΪ 32 λλͼ(��Alpha����) */ 
} CNVTYPE, *LPCNVTYPE;

// ����λ�������
typedef enum
{
	CSAVEAUTO	= IRWE_CNVMARK_AUTO,
	CSAVE1		= IRWE_CNVMARK_1,
	CSAVE4		= IRWE_CNVMARK_4,
	CSAVE8		= IRWE_CNVMARK_8,
	CSAVE16555	= IRWE_CNVMARK_16_555,
	CSAVE16565	= IRWE_CNVMARK_16_565,
	CSAVE24		= IRWE_CNVMARK_24,
	CSAVE32		= IRWE_CNVMARK_32
} CSAVETYPE, *LPCSAVETYPE;

// ��״̬����
typedef enum
{
	CSISTATUS_NULL,										// ����
	CSISTATUS_IRW,										// ��д״̬
	CSISTATUS_IP										// ͼ����״̬
} CSISTATUS, *LPCSISTATUS;

// ��ڲ����������
typedef enum
{
	CSIPARMCHK_LOAD,									// ������
	CSIPARMCHK_DRAW,									// ���ƺ���
	CSIPARMCHK_SAVE										// ���溯��
} CSIPARMCHK, *LPCSIPARMCHK;

// Ԥ����HDIB���ͣ�ʵ���Ͼ���ָ��û���ļ�ͷ�ṹ��BMP���ݣ�
typedef char	*HDIB;

// CSeeImage�����ǩ
#define CSEEIMAGE_MAGIC		0x4d495343					// "CSIM"

// �ض����Զ���
#ifdef _DEBUG
#define Assert(expr, str)	{ if (!(expr)){ assert(!(str)); } }
#else
#define Assert(expr, str)	((void*)0)
#endif

// IPģʽλ���
#define IP_BITCNT			32

// ��ɫ�����������
#define	MAX_PALETTE_CNT		256

// DIB��Ϣ�ṹ
typedef struct _tagDIBINFO
{
	wBITMAPINFOHEADER	bmi;
	wRGBQUAD				pal[MAX_PALETTE_CNT];
} DIBINFO, *LPDIBINFO;

// �߼���ɫ��ṹ
typedef struct _tagLOGPAL
{
	LOGPALETTE			pal;
	PALETTEENTRY		entry[MAX_PALETTE_CNT];
} LOGPAL, *LPLOGPAL;

// �ڲ�ͼ���д������������־
const bool CSIRW_BUFFER = false;

// �׻ӷ���������Դ�����е��ļ���
#define CSINIHFILENAME		"..bmp"


// ��ȡ��Ϣ�������õ��ı�־
typedef enum
{
	CWIDTH		= 0x1,									// ��ȡͼ����
	CHEIGHT		= 0x2,									// ��ȡͼ��߶�
	CBITCNT		= 0x4,									// λ���
	CORDER		= 0x8,									// ͼ��ķ��÷�ʽ

	CCURIMGNUM	= 0x10,									// ��ǰͼ�����ţ�0����
	CPALCNT		= 0x20,									// ��ȡ��ɫ����ĸ���
	CSTATUS		= 0x40,									// ��ȡ���״̬����д��IP״̬��
	CIMGCNT		= 0x80,									// ��ȡͼ��ҳ��

	CENGSTATUS	= 0x100,								// ����״̬���Ƿ�����
	CHAVIMG		= 0x200,								// �����Ƿ����ͼ��

	CDIRECT		= 0x10000,								// ֱ�����ӻ�ȡ�����ķָ�ֵ

	CPALETTE	= 0x10000,								// ��ȡ��ɫ������
	CPACKPTR	= 0x20000,								// ��ȡ����רҵ���ĵ�ַ
	CBITS		= 0x40000,								// ��ȡ���ػ������׵�ַ
	COPENSTR	= 0x80000								// ��ȡ��д�����OPEN��

} IMGINFOTYPE, *LPIMGINFOTYPE;

// ͼ����Ϣ�ṹ
typedef struct _tagImageInfoStr
{
	ulong	flag;

	ulong	width;								/* ͼ���� */
	ulong	height;								/* ͼ��߶� */
	ulong	bitcount;							/* ÿ��������ռ��λ����λ��ȣ� */
	ulong	order;								/* ���÷�ʽ���������*/
	
	unsigned int	pal_count;							/* ��ɫ����Ч��ĸ��� */
	ulong	palette[MAX_PALETTE_COUNT];			/* ��ɫ������ */
	char			*pbits;								/* ���������׵�ַ */

	int				engstatus;							/* ����״̬(true-�ѿ�����flase-δ����) */
	CSISTATUS		status;								/* ��״̬ */
	int				havimg;								/* �����Ƿ����ͼ��(1-���ڡ�2-������) */
	int				imgcnt;								/* ͼ��ҳ�� */
	int				currimgnum;							/* ��ǰͼ��0����ţ�*/

	LPSPEC_DATAPACK	pack;								/* רҵ����ַ */

	char			*openstr;							/* ����open�� */

	int				rev[12];							/* ���� */
} IMAGEINFOSTR, *LPIMAGEINFOSTR;


/////////////////////////////////////////////////////////////////////////////
// CFileDialogEx - �����Ϊ���Ի�����

class CFileDialogEx : public CFileDialog
{
	DECLARE_DYNAMIC(CFileDialogEx)
		
public:
	CFileDialogEx(LPSPEC_DATAPACK);
	
	static EXERESULT	DispLastErrMess(int err);
	static IRWE_CNVMARK GetStdIDFromBitCnt(int bitcnt);


protected:
	LPSPEC_DATAPACK	m_pPack;							// �������ͼ�����ַ
	
	int				m_iRunMark;							// ��ʼ���ɹ���ǣ�0���ɹ���
	int				m_ctrlcnt;							// ��ǰ��ѡ����������б����
	SAVESTR			m_save;								// ��������ṹ
	CString			m_ext;								// ��ǰȱʡ��չ��
	
	CStatic			*m_pStatic[ISD_CTLCNT];				// ��ǩ������
	CComboBox		*m_pComboBox[ISD_CTLCNT];			// �б�������
	
	int				m_saveinfocnt;						// ���б��湦�ܵĲ������
	SAVEPLUGININFO	m_saveinfo[ISEEIRW_MAX_PLUGIN_CNT];	// �б��湦�ܲ������Ϣ�ṹ����
	
	
	int  _GetPluginIndex(LPCTSTR sou);
	int  _GetPluginIndexFromFilterIndex(int idx);
	int  _SetFileType(int index);
	
	BOOL _OK(void);
	
protected:
	//{{AFX_MSG(CFileDialogEx)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////////////
// CSeeImage�ඨ��

#ifdef	_AFXDLL
#define CLASSEXTTYPE	
#else
#define CLASSEXTTYPE	__declspec(dllimport)
#endif

class CLASSEXTTYPE CSeeImage
{
public:
	CSeeImage();
	~CSeeImage();


/////////////////////////////////////////////////////////////////////////////////////
// ϵͳ�ӿ�
public:
	static int		OpenSeeImage(char *path);
	static int		CloseSeeImage(void);
	static void *	ImAlloc(size_t size);
	static void *	ImFree(void *p);

	CSeeImage& operator=(CSeeImage&sou);
	void			Clear(void);
	
// ��д�����ƽӿ�
public:

	int				Load(CSITYPER type, ...);
	int				GetInfo(int flag, LPIMAGEINFOSTR pinfo = NULL);
	int				Draw(CSIDRAW type, ...);
	int				Save(CSITYPEW type, ...);

// ͼ����ӿ�
public:
	int				OpenIPMode();
	char*			IP_GetScanLineAddr(int y);
	ulong	IP_GetPixel(int x, int y);
	ulong	IP_SetPixel(int x, int y, ulong newpix);
	int				CloseIPMode();



/////////////////////////////////////////////////////////////////////////////////////	
// �ڲ�����
protected:
	int				_typesafe_check(CSIPARMCHK pct, int type, va_list ap);
	void			_cnvtype_check(int cnk);

	char*			_LoadDIBFromPE(char *fname, int id);
	char*			_LoadDIBFromResource(char *lpResourceName);
	int				_AllocSpecPack(LPSPEC_DATAPACK *ppPack, wBITMAPINFO* pbmi, int cnvmark, int sn, char *pbits, int bitmark);
	int				_SaveSpecPack(ISEEIO_CONFER_TYPE type, char *name, ulong par1, ulong par2, CSAVETYPE cnvmark, SAVESTR* par);
	SUBIMGBLOCK*	_CalSubImageAddr(LPINFOSTR pinfo, int num);
	void			_UpdateDispInfo(void);

	static char*	_NormalizePath(char *path, char *buff);



/////////////////////////////////////////////////////////////////////////////////////
// �ڲ�����
#ifdef	_AFXDLL
	friend	class CIrwcppApp;
#endif

#ifdef _DEBUG
#define TYPESAFE_CHECK(pct) _typesafe_check(pct, type, reinterpret_cast<va_list>(&type)+_INTSIZEOF(type))
#define CNVTYPE_CHECK(cnv)	_cnvtype_check(cnv)
#else
#define TYPESAFE_CHECK(pct)	((void*)0)
#define CNVTYPE_CHECK(cnv)	((void*)0)
#endif



/////////////////////////////////////////////////////////////////////////////////////
// �ڲ�����
private:
	ulong		m_uMagic;						// ���ǩ-CSEEIMAGE_MAGIC

	LPSPEC_DATAPACK		m_pPack;						// רҵ���ݰ�ָ��
	DIBINFO				m_cDispInfo;					// ��ʾ����Ϣ
	CSISTATUS			m_eStatus;						// ��״̬���
	bool				m_bIsChange;					// �Ƿ�򿪹�ͼ��IPģʽ

	HDRAWDIB			m_hDrawDib;						// DrawDib����
	
	static bool			m_bMark;						// �����ʼ���ɹ����
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif  // __SEEIMAGE_INC__
