/********************************************************************

	ShellPidl.h

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
	���ļ���;��	ͼ���д���棭C���԰棭VC���������������shell pidl
					�ඨ���ļ���

	���ļ���д�ˣ�	
					Selom Ofori		none
					YZ				yzfree##sina.com

	���ļ��汾��	30315
	����޸��ڣ�	2003-3-15

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������
	----------------------------------------------------------------
	����ע�ͣ�

	���ļ���ԭʼ����Ϊ Selom Ofori��Դ��û��������ơ�����YZ��д������
	ISee��Ŀ�����֤��ΪGPL���� Selom Ofori û��������ϵ��ַ������δ��
	�������������

	������ʷ��

		2003-3		�޸��б������˽�����ݽṹ������ISee���ݰ�������
		2003-1		��һ�����԰淢��

********************************************************************/

#if !defined(AFX_SHELLPIDL_H__INCLUDED_)
#define AFX_SHELLPIDL_H__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include <shlobj.h>

// �б������ݽṹ�е�����״̬
enum {SPTS_EMPTY = 0, SPTS_FATT = 1, SPTS_IMGINF = 2};


// �б���˽�����ݽṹ
typedef struct tagLVID
{
	int							type;				// �����ͣ�������

	WIN32_FILE_ATTRIBUTE_DATA	wfad;				// ����ļ�����

	DWORD						width;				// ͼ��Ŀ��ߡ�λ�����
	DWORD						height;
	DWORD						bitcount;

	LPSHELLFOLDER				lpsfParent;			// ���ļ���IShellFolder�ӿ�
	LPITEMIDLIST				lpi;				// �ļ������PIDL

	UINT						state;				// �������״̬(enum SPTS_XXX)

	char						rev[1016];			// ���������ܽ���������ͼ����
} LVITEMDATA, *LPLVITEMDATA;



// ����˽�����ݽṹ
typedef struct tagID
{
	int				type;							// �����ͣ�����������δ����չ��
	LPSHELLFOLDER	lpsfParent;						// ���ļ���IShellFolder�ӿ�
	LPITEMIDLIST	lpi;							// ����ڸ��ļ��еġ���ԡ�PIDL
	LPITEMIDLIST	lpifq;							// ����ڸ��ļ��еġ����ԡ�PIDL
} TVITEMDATA, *LPTVITEMDATA;


class CShellPidl
{
public:

public:

    LPITEMIDLIST	ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
    LPITEMIDLIST	GetFullyQualPidl(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi);
    LPITEMIDLIST	CopyITEMID(LPMALLOC lpMalloc, LPITEMIDLIST lpi);
    BOOL			GetName(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpi, DWORD dwFlags, LPSTR lpFriendlyName);
    LPITEMIDLIST	CreatePidl(UINT cbSize);
    UINT			GetSize(LPCITEMIDLIST pidl);
    LPITEMIDLIST	Next(LPCITEMIDLIST pidl);

	BOOL	DoTheMenuThing(HWND hwnd, LPSHELLFOLDER lpsfParent, LPITEMIDLIST  lpi, LPPOINT lppt);
	int		GetItemIcon(LPITEMIDLIST lpi, UINT uFlags);
};

#endif // !defined(AFX_SHELLTREE_H__INCLUDED_)
