
// CBIRLab.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCBIRLabApp:
// �йش����ʵ�֣������ CBIRLab.cpp
//

class CCBIRLabApp : public CWinAppEx
{
public:
	CCBIRLabApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCBIRLabApp theApp;