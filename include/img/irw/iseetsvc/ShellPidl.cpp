/********************************************************************

	ShellPidl.cpp

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
					��ʵ���ļ���

	���ļ���д�ˣ�	
					Selom Ofori		none
					YZ				yzfree##sina.com

	���ļ��汾��	30119
	����޸��ڣ�	2003-1-19

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������
	----------------------------------------------------------------
	����ע�ͣ�

	���ļ���ԭʼ����Ϊ Selom Ofori��Դ��û��������ơ�����YZ��д������
	ISee��Ŀ�����֤��ΪGPL���� Selom Ofori û��������ϵ��ַ������δ��
	�������������

	������ʷ��

		2003-1		��һ�����԰淢��

********************************************************************/


#include "stdafx.h"

#include "ShellPidl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/****************************************************************************
*
*    ����: Next(LPCITEMIDLIST pidl)
*
*    ��;: ��ȡPIDL����һ��Ԫ�ر�ʶ��ַ
*
****************************************************************************/
LPITEMIDLIST CShellPidl::Next(LPCITEMIDLIST pidl)
{
   LPSTR lpMem = (LPSTR)pidl;

   lpMem += pidl->mkid.cb;

   return (LPITEMIDLIST)lpMem;
}


/****************************************************************************
*
*    ����: GetSize(LPCITEMIDLIST pidl)
*
*    ��;:  ��ȡ����PIDL�ĳߴ磨�ֽڵ�λ��
*
****************************************************************************/
UINT CShellPidl::GetSize(LPCITEMIDLIST pidl)
{
    UINT cbTotal = 0;

    if (pidl)
    {
        while (pidl->mkid.cb)
        {
            cbTotal += pidl->mkid.cb;
            pidl = Next(pidl);
        }

        cbTotal += sizeof(pidl->mkid.cb);				// ���Ͻ�β��˫'\0'��
    }

    return cbTotal;
}


/****************************************************************************
*
*    ����: CreatePidl(UINT cbSize)
*
*    ��;: ����һ���յġ�ָ���ߴ��PIDL��
*
****************************************************************************/
LPITEMIDLIST CShellPidl::CreatePidl(UINT cbSize)
{
    LPMALLOC		lpMalloc;
    LPITEMIDLIST	pidl;

    if (FAILED(SHGetMalloc(&lpMalloc)))					// ��ȡShell�ķ���ӿ�
	{
       return 0;
	}
	
    pidl = (LPITEMIDLIST)lpMalloc->Alloc(cbSize);		// ����PIDL�ڴ��

    if (pidl)
	{
        memset((void*)pidl, 0, cbSize);					// ��ʼ���ڴ������Ϊ0
	}
	
	lpMalloc->Release();

    return pidl;
}


/****************************************************************************
*
*    ����: ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
*
*    ��;: ��������ָ����PIDL��������PIDL����ʽ����
*
****************************************************************************/
LPITEMIDLIST CShellPidl::ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
    LPITEMIDLIST	pidlNew;
    UINT			cb1;
    UINT			cb2;

    if (pidl1)
	{
		cb1 = GetSize(pidl1) - sizeof(pidl1->mkid.cb);	// ��һ��PIDL��β��16λ0ֵ���ڼ��㷶Χ��
	}
    else
	{
		cb1 = 0;
	}

	ASSERT(pidl2);

	cb2 = GetSize(pidl2);

    if ((pidlNew=CreatePidl(cb1+cb2)))
    {
        if (pidl1)
		{
			memcpy((void*)pidlNew, pidl1, cb1);
		}
        memcpy(((LPSTR)pidlNew)+cb1, pidl2, cb2);		// ��������PIDL������
    }

    return pidlNew;
}


/****************************************************************************
*
*    ����: CopyITEMID(LPMALLOC lpMalloc, LPITEMIDLIST lpi)
*
*    ��;: ����lpi�е�һ��Ԫ�ر�ʶ���ṹ�����γ�һ���µ�PIDL����
*
****************************************************************************/
LPITEMIDLIST CShellPidl::CopyITEMID(LPMALLOC lpMalloc, LPITEMIDLIST lpi)
{
   LPITEMIDLIST lpiTemp;

   if (lpiTemp=(LPITEMIDLIST)lpMalloc->Alloc(lpi->mkid.cb+sizeof(lpi->mkid.cb)))
   {
	   memcpy((void*)lpiTemp, (const void *)lpi, lpi->mkid.cb+sizeof(lpi->mkid.cb));
   }

   return lpiTemp;
}


/****************************************************************************
*
*    ����: GetName(LPSHELLFOLDER lpsf,LPITEMIDLIST  lpi,DWORD dwFlags, LPSTR lpFriendlyName)
*
*    ��;: ��ȡָ��Ŀ¼���ļ�����ʾ��
*
****************************************************************************/
BOOL CShellPidl::GetName(LPSHELLFOLDER lpsf,
							LPITEMIDLIST  lpi,
							DWORD         dwFlags,
							LPSTR         lpFriendlyName)
{
	BOOL   bSuccess = TRUE;
	STRRET str;

	if (NOERROR == lpsf->GetDisplayNameOf(lpi, dwFlags, &str))
	{
		switch (str.uType)
		{
		case STRRET_WSTR:
			WideCharToMultiByte(CP_ACP,                 // ����ANSI����ҳ
								0,
								str.pOleStr,            // ԭ���ַ�����ַ
								-1,
								lpFriendlyName,         // Ŀ�Ļ�����
								MAX_PATH,
								NULL, NULL);
			break;
		case STRRET_OFFSET:
			lstrcpy(lpFriendlyName, (LPSTR)lpi+str.uOffset);
			break;
		case STRRET_CSTR:
			lstrcpy(lpFriendlyName, (LPSTR)str.cStr);
			break;
		default:
			bSuccess = FALSE;
			break;
		}
	}
	else
	{
		bSuccess = FALSE;
	}

	return bSuccess;
}


/****************************************************************************
*
*    ����: GetFullyQualPidl(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi)
*
*    ��;: ��ȡָ���ļ��еľ���PIDL�������浽���ļ��У�
*
****************************************************************************/
LPITEMIDLIST CShellPidl::GetFullyQualPidl(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi)
{
	char			szBuff[MAX_PATH];
	OLECHAR			szOleChar[MAX_PATH];
	LPSHELLFOLDER	lpsfDeskTop;
	LPITEMIDLIST	lpifq;
	ULONG			ulEaten = 0, ulAttribs = 0;
	HRESULT			hr;

	if (GetName(lpsf, lpi, SHGDN_FORPARSING, szBuff) == FALSE)
	{
		return NULL;
	}

	if (FAILED(SHGetDesktopFolder(&lpsfDeskTop)))		// ��ȡ���ļ��нӿڣ����棩
	{
		return NULL;
	}

	MultiByteToWideChar(CP_ACP,
						MB_PRECOMPOSED,
						szBuff,
						-1,
						(USHORT *)szOleChar,
						sizeof(szOleChar));

	hr = lpsfDeskTop->ParseDisplayName(NULL,			// ת��·����PIDL
										NULL,
										szOleChar,
										&ulEaten,
										&lpifq,
										&ulAttribs);

	lpsfDeskTop->Release();

	if (FAILED(hr))
	{
		return NULL;
	}

	return lpifq;
}


/****************************************************************************
*
*  ����: DoTheMenuThing(HWND hwnd, 
*                           LPSHELLFOLDER lpsfParent,
*                           LPITEMIDLIST  lpi,
*                           LPPOINT lppt)
*
*  ��;: ��ʾһ��ShellԪ�ص������Ĳ˵�����ͨ�����Ҽ��˵���
*
*
*  ����:
*    hwnd       - �����ھ��
*    lpsfParent - Ԫ�صĸ��ļ��нӿ�
*    lpi        - Ԫ�ص�PIDL
*    lppt       - �����˵������Ͻ�λ��
*
*  ����:
*    TRUE - �ɹ�, FALSE - ʧ��
*
****************************************************************************/
BOOL CShellPidl::DoTheMenuThing(HWND hwnd, LPSHELLFOLDER lpsfParent, LPITEMIDLIST lpi, LPPOINT lppt)
{
    LPCONTEXTMENU		lpcm;
    HRESULT				hr;
    char				szTemp[64];
    CMINVOKECOMMANDINFO cmi;
    DWORD               dwAttribs=0;
    int                 idCmd;
    HMENU               hMenu;
    BOOL                bSuccess=TRUE;

	// ��ȡԪ�ص������Ĳ˵��ӿ�
    hr = lpsfParent->GetUIObjectOf(hwnd,
									1,					// ֻ��һ��Ԫ��
									(const struct _ITEMIDLIST **)&lpi,
									IID_IContextMenu,
									0,
									(LPVOID *)&lpcm);

    if (SUCCEEDED(hr))  
    {
       hMenu = CreatePopupMenu();

       if (hMenu)
       {
          hr = lpcm->QueryContextMenu(hMenu, 0, 1, 0x7fff, CMF_EXPLORE);

          if (SUCCEEDED(hr))
          {
             idCmd = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON, 
									lppt->x, lppt->y, 0, hwnd, NULL);

             if (idCmd)
             {
                cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
                cmi.fMask  = 0;
                cmi.hwnd   = hwnd;
                cmi.lpVerb = MAKEINTRESOURCE(idCmd-1);
                cmi.lpParameters = NULL;
      	        cmi.lpDirectory  = NULL;
                cmi.nShow        = SW_SHOWNORMAL;
                cmi.dwHotKey     = 0;
                cmi.hIcon        = NULL;

                hr = lpcm->InvokeCommand(&cmi);

                if (!SUCCEEDED(hr))  
                {
                   wsprintf(szTemp, "��������ʧ�ܣ�������[%lx]", hr);
                   AfxMessageBox(szTemp);
                }
             }
          }
          else
		  {
			  bSuccess = FALSE;
		  }

          DestroyMenu(hMenu);
       }
       else
	   {
		   bSuccess = FALSE;
	   }

       lpcm->Release();
    } 
    else
    {
       wsprintf(szTemp, "GetUIObjectOf() ��������ʧ�ܣ�������[%lx]", hr);
       AfxMessageBox(szTemp );
       bSuccess = FALSE;
    }

    return bSuccess;
}


/****************************************************************************
*
*    ����: GetItemIcon(LPITEMIDLIST lpi, UINT uFlags)
*
*    ��;: ��ȡ��PIDLָ����Ԫ����ϵͳͼ����е�ͼ�������š�
*
*    ����:
*		lpi    - Ԫ�صľ���PIDL
*		uFlags - ����ȡ��ͼ�����ͣ���MSDN���й�SHGetFileInfo()������˵����
*
*	����:
*		Ԫ�ص�ͼ������
****************************************************************************/
int CShellPidl::GetItemIcon(LPITEMIDLIST lpi, UINT uFlags)
{
   SHFILEINFO	sfi;

   SHGetFileInfo((LPCSTR)lpi, 0, &sfi, sizeof(SHFILEINFO), uFlags);

   return sfi.iIcon;
}
