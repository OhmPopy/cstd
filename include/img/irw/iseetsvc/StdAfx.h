// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__46E3C581_CBE0_4226_96B1_27A4D6E8735D__INCLUDED_)
#define AFX_STDAFX_H__46E3C581_CBE0_4226_96B1_27A4D6E8735D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <shlwapi.h>
#include <afxmt.h>
#include <afxtempl.h>
#include <vfw.h>

#include "iseemem.h"
#include "iseeio.h"
#include "iseeimn.h"
#include "iseeirw.h"

#include "btnst.h"


/////////////////////////////////////////////////////////////////////////////////
// ISeeTsVC ������Ϣ����

#define WM_DANI_QUIT			(WM_APP+327)			// ������߳��˳�
#define WM_DANI_DIABLE			(WM_APP+326)			// ������ǰ����
#define WM_DANI_NEWCOMM			(WM_APP+325)			// ��ͼ������

#define WM_DANI_NEXT			(WM_APP+321)			// ��ʾ��һ����ͼ��
#define WM_DANI_INIT			(WM_APP+320)			// �����߳���ɳ�ʼ��

#define WM_DISP_FAILD			(WM_APP+306)			// ��д���̽�����ʧ��
#define WM_DISP_OK				(WM_APP+305)			// ��д���̽������ɹ�
#define WM_DISP_OVER			(WM_APP+304)			// ����һ����֡ͼ��Ķ�ȡ
#define WM_DISP_PROCESS			(WM_APP+303)			// ����
#define WM_DISP_BUILDSI			(WM_APP+302)			// ���ݰ��Ѿ�������ͼ��
#define WM_DISP_BUILDMI			(WM_APP+301)			// ���ݰ��Ѿ�������ͼ��
#define WM_DISP_INTOPLUG		(WM_APP+300)			// ���ƽ�����

#define WM_TVN_DEFERFILL		(WM_APP+255)			// �ӳ������Ϣ
#define WM_TVN_SELCHANGED		(WM_APP+254)			// ѡ���޸���Ϣ
#define WM_TVN_PROCCREATE		(WM_APP+253)			// �ӳ��̴߳�����Ϣ
#define WM_TVN_UPDATE			(WM_APP+252)			// ����������Ϣˢ���б�����

#define WM_TVN_DEFER			(WM_APP+250)			// �ӳ���Ϣ
#define WM_TVN_QUEUECLR			(WM_APP+249)			// �����Ϣ
#define WM_TVN_QUIT				(WM_APP+248)			// �˳���Ϣ

#define WM_LVN_FILLBEGIN		(WM_APP+245)			// ��俪ʼ��Ϣ
#define WM_LVN_FILLEND			(WM_APP+244)			// ��������Ϣ
#define WM_LVN_SUBITEMDATA		(WM_APP+243)			// �������������Ϣ
#define WM_LVN_PROCCREATE		(WM_APP+242)			// �����̴߳�����Ϣ
#define WM_LVN_QUIT				(WM_APP+241)			// �˳���Ϣ
#define WM_LVN_NEWCOMM			(WM_APP+240)			// ��������Ϣ

#define WM_LVN_AUTOSEL			(WM_APP+235)			// �Զ�����ͼ��Ĵ�����Ϣ
#define WM_LVN_AUTONEXT			(WM_APP+234)			// �Զ�������һ��ͼƬ

#define WM_RV_CREATED			(WM_APP+230)			// ���д��ڴ������֪ͨ




/////////////////////////////////////////////////////////////////////////////////
// ISeeTsVC ע����������

#define	ITSVCMAIN_RKEYSEC			("Information")		// ��������

#define	ITSVCMAIN_RKEY_WINPOS		("WinPosition")		// ����ر�ʱ�Ĵ���λ��
#define	ITSVCMAIN_RKEY_LEFTWINPOS	("TreeViewSize")	// ���ߴ�
#define	ITSVCMAIN_RKEY_RIGHTWINPOS	("ListViewSize")
#define	ITSVCMAIN_RKEY_DISPWINPOS	("DispViewSize")

#define	ITSVCDISP_RKEY_AUTORESIZE	("AutoResize")		// �Զ�����״̬
#define	ITSVCDISP_RKEY_RSROT		("Ratio")			// ���ű���
#define	ITSVCDISP_RKEY_INITPOS		("InitPosition")	// ���뷽ʽ
#define	ITSVCDISP_RKEY_SWITCH		("SwitchType")		// �л���ʽ

#define	ITSVCDOC_RKEY_LASTPATH		("LastPath")		// ��������·��
#define	ITSVCRIGHT_RKEY_LISTYPE		("ListStyle")		// �б��ӵķ��
#define	ITSVCLIST_RKEY_AUTOPLAY		("AutoPlay")		// �Զ����ű�־



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__46E3C581_CBE0_4226_96B1_27A4D6E8735D__INCLUDED_)
