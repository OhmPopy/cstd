/********************************************************************

	irwcpp.cpp

	
	���ļ���;��	ͼ���д����C++���԰�ӿ�ʵ���ļ�

	���ļ���д�ˣ�	
					YZ			yzfree##sina.com

	���ļ��汾��	30903
	����޸��ڣ�	2003-9-3

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
	    ��ַ�ռ������
  	----------------------------------------------------------------
	������ʷ��

		2003-9		�������溯���е�һ��BUG��ȱ�ٶԱ����ļ�����Ч���жϣ�
		2003-8		����Clear()�ӿں���
		2003-8		����Load()�ӿں�����ԴID������ö�ٹ��ܵ�һ��BUG
		2003-8		�������ز�������=������
		2003-8		GetInfo()��������CHAVIMG��Ϣ���ͣ�������һ��BUG
		2003-7		��һ�����԰淢��

********************************************************************/

#include "stdafx.h"

#include <dlgs.h>
#include "irwcpp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment (lib, "vfw32.lib")
#ifdef _DEBUG
#pragma comment (lib, "iseemem.lib")
#endif
#pragma comment (lib, "iseeio.lib")
#pragma comment (lib, "iseeirw.lib")

bool CSeeImage::m_bMark = false;


/////////////////////////////////////////////////////////////////////////////
// CIrwcppApp

BEGIN_MESSAGE_MAP(CIrwcppApp, CWinApp)
	//{{AFX_MSG_MAP(CIrwcppApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIrwcppApp construction

CIrwcppApp::CIrwcppApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIrwcppApp object

CIrwcppApp theApp;


/////////////////////////////////////////////////////////////////////////////
// C++�ӿ�ʵ��

// ����ӿڳ�ʼ��
int CSeeImage::OpenSeeImage(char *path)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	Assert(!m_bMark, "OpenSeeImage()���������治���ظ���ʼ����");

	char	buff[MAX_PATH];
	int		result;

	// ���滯�������ṩ��·������ת��Ϊ����·����
	_NormalizePath(path, (char*)buff);

	// ��ʼ��ISeeͼ���д����
	if ((result=isirw_init((char*)buff, 0)) != 0)
	{
		/*
			-1 :�ڴ治��
			-2 :�޿��ò��
			-3 :�ļ���д����
			-4 :INI�ļ����ݷǷ�
			-5 :ϵͳ�쳣
			-6 :��Ϣ���̴߳���ʧ��
		*/	
		return result;
	}

	// �����趨����������ڲ����ػ�����
	if (CSIRW_BUFFER)
	{
		isirw_enbuffer(IRWE_BUFTYPE_ORG|IRWE_BUFTYPE_CNV);
	}

	// �������濪����־
	m_bMark = true;

	// ��������
	LPIRWMF_HEADER	phead = isirw_lock();

	if (phead == NULL)
	{
		CloseSeeImage();
		return -5;										// ϵͳ�쳣
	}

	UPDATAINFO	udp;
	
	// ˢ�²����Ϣ��
	switch (result=isirw_update(phead, IRWE_UPDTYPE_EMPLOY, &udp))
	{
	case	0:			// 0: ˢ�³ɹ�
		break;
	case	1:			// 1: �޿��ò��
		result = -2;
		break;
	case	2:			// 2: ��д����
		result = -3;
		break;
	case	3:			// 3: �ڴ治��
		result = -1;
		break;
	case	4:			// 4: ϵͳ�쳣
	default:
		result = -5;
		break;
	}
	
	// �������
	isirw_unlock(phead);

	if (result != 0)
	{
		CloseSeeImage();
	}

	return result;
}


// �ر�����
int CSeeImage::CloseSeeImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!m_bMark)
	{
		return -1;										// ���沢δ�򿪹�
	}
	
	// �������濪����־
	m_bMark = false;

	// �ر���������ػ�����
	if (CSIRW_BUFFER)
	{
		isirw_dibuffer(IRWE_BUFTYPE_ORG|IRWE_BUFTYPE_CNV);
	}
	
	// �ر�ISeeͼ���д����
	isirw_close();

	return 0;
}



CSeeImage::CSeeImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_eStatus   = CSISTATUS_NULL;
	m_pPack     = NULL;
	m_bIsChange	= false;
	m_hDrawDib	= DrawDibOpen();

	memset((void*)&m_cDispInfo, 0, sizeof(DIBINFO));

	m_uMagic	= CSEEIMAGE_MAGIC;
}


CSeeImage::~CSeeImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	Assert(m_uMagic == CSEEIMAGE_MAGIC, "CSeeImage�������������ڴ��⵽�ƻ���");

	if (m_hDrawDib)
	{
		DrawDibClose(m_hDrawDib);
	}
	
	if (m_pPack != NULL)
	{
		isirw_free_SPEC_DATAPACK(m_pPack);
	}
}


// ��ָ����λ�ö�ȡͼ��
int CSeeImage::Load(CSITYPER type, ...)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	LPSPEC_DATAPACK		psOldPack;
	enum EXERESULT		eer;
	va_list				argp;
	int					cnvmark, i;

	// ��������Ѿ�����
	Assert(m_bMark, "Load()���������滹δ������");

	if (!m_bMark)
	{
		return 0;
	}

	// ��ڲ������ͼ��
	TYPESAFE_CHECK(CSIPARMCHK_LOAD);

	va_start(argp, type);
	
	switch (type)
	{
	case	CR_FILE:									// �����ļ�����
	case	CR_FILEFAST:								// �����ļ����ͣ����ٰ棩
		{
			char	*fname;
			char	buff[MAX_PATH];

			fname   = va_arg(argp, char*);
			cnvmark	= va_arg(argp, int);

			va_end(argp);

			Assert(cnvmark != IRWE_CNVMARK_NOT, "Load()������FILE���ͣ��ڶ�����������Ϊ-2��");

			// ����ԭ�е�רҵ��
			psOldPack = m_pPack;

			// ����Ƿ���Ҫ��Open�Ի���
			if (fname == NULL)
			{
				CFileDialog		fdl(TRUE, NULL, NULL, 0, (LPCTSTR)isirw_get_openstr(), ::AfxGetMainWnd());

				if (fdl.DoModal() == IDOK)
				{
					CString	str = fdl.GetPathName();
					lstrcpy((LPSTR)buff, (LPCSTR)str);
					fname = (char*)buff;
				}
				else
				{
					return ER_NONIMAGE;
				}
			}

			// �����淢�Ͷ�ȡ����
			if (isirw_s_read_img_c((ISEEIO_CONFER_TYPE)type, fname, 0, 0, (int)type, (IRWE_CNVMARK)cnvmark, NULL, &m_pPack) == ISIRW_LASTERR_SUCCESS)
			{
				assert(m_pPack != NULL);

				// �ȴ����������
				eer = isirw_s_wait(m_pPack, 0);

				if (eer != ER_SUCCESS)					// ��ȡ����
				{
					assert(m_pPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(m_pPack);
				}
				else									// ��ȡ�ɹ�
				{
					;
				}
			}
			else
			{
				eer = ER_MEMORYERR;						// �ڴ治�㣨ϵͳ���ȶ��Ŀ����Խ�С��
			}

			if (eer == ER_SUCCESS)
			{
				// ������ھ�רҵ���������ͷ���
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}

				// ����Ϊ��д״̬
				m_eStatus = CSISTATUS_IRW;
				// �赱ǰͼ��Ϊ��ͼ��
				m_pPack->lab1 = 0;
				// ������ʾ����Ϣ
				_UpdateDispInfo();
			}
			else
			{
				// �����ȡʧ�ܣ���ָ��ɵ�����
				m_pPack = psOldPack;
			}

			return eer;
		}
		break;
	case	CR_MEMORY:									// �����ڴ�����
		{
			char			*mname;
			char			*addr;
			ulong	len;
			
			mname   = va_arg(argp, char*);
			addr	= va_arg(argp, char*);
			len		= va_arg(argp, ulong);
			cnvmark	= va_arg(argp, int);
			
			va_end(argp);
			
			// ����ԭ�е�רҵ��
			psOldPack = m_pPack;
			
			// �����淢�Ͷ�ȡ����
			if (isirw_s_read_img_c(ISEEIO_CONFER_LOC_MEMORY, mname, (ulong)addr, len, (int)type, (IRWE_CNVMARK)cnvmark, NULL, &m_pPack) == ISIRW_LASTERR_SUCCESS)
			{
				assert(m_pPack != NULL);
				
				// �ȴ����������
				eer = isirw_s_wait(m_pPack, 0);
				
				if (eer != ER_SUCCESS)					// ��ȡ����
				{
					assert(m_pPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(m_pPack);
				}
				else									// ��ȡ�ɹ�
				{
					;
				}
			}
			else
			{
				eer = ER_MEMORYERR;						// �ڴ治�㣨ϵͳ���ȶ��Ŀ����Խ�С��
			}
			
			if (eer == ER_SUCCESS)
			{
				// ������ھ�רҵ���������ͷ���
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// ����Ϊ��д״̬
				m_eStatus = CSISTATUS_IRW;
				// �赱ǰͼ��Ϊ��ͼ��
				m_pPack->lab1 = 0;
				// ������ʾ����Ϣ
				_UpdateDispInfo();
			}
			else
			{
				// �����ȡʧ�ܣ���ָ��ɵ�����
				m_pPack = psOldPack;
			}
			
			return eer;
		}
		break;
	case	CR_SEEIMAGE:								// ��һ��CSeeImage����
		{
			CSeeImage	*pSou;
			
			pSou    = va_arg(argp, CSeeImage*);
			cnvmark	= va_arg(argp, int);
			
			va_end(argp);

			assert(pSou&&(pSou->GetInfo(CSTATUS)!=(int)CSISTATUS_NULL));

			// ����ԭ�е�רҵ��
			psOldPack = m_pPack;

			DIBINFO			bmi;
			IMAGEINFOSTR	iis;
			
			// ȡ��Դͼ��Ŀ��ߵ�����
			iis.flag = CWIDTH|CHEIGHT|CBITCNT|CORDER|CPALCNT|CPALETTE|CBITS;
			pSou->GetInfo(0, &iis);

			// ��дԴͼ����Ϣ�ṹ
			bmi.bmi.biSize     = sizeof(wBITMAPINFOHEADER);
			bmi.bmi.biWidth	   = iis.width;
			bmi.bmi.biHeight   = (iis.order == 0) ? -(long)(iis.height) : iis.height;
			bmi.bmi.biBitCount = (ushort)iis.bitcount;
			bmi.bmi.biPlanes   = 1;
			
			bmi.bmi.biCompression   = BI_RGB;
			bmi.bmi.biSizeImage     = 0;
			bmi.bmi.biXPelsPerMeter = 0;
			bmi.bmi.biYPelsPerMeter = 0;
			bmi.bmi.biClrImportant  = 0;
			bmi.bmi.biClrUsed       = 0;
			
			memcpy((void*)bmi.pal, (const void *)iis.palette, sizeof(wRGBQUAD)*iis.pal_count);
			
			// ���벢��дרҵ��
			eer = (enum EXERESULT)_AllocSpecPack(&m_pPack, (wBITMAPINFO*)&bmi, cnvmark, (int)type, iis.pbits, 1);
			
			if (eer == ER_SUCCESS)
			{
				// ������ھ�רҵ���������ͷ���
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// ����Ϊ��д״̬
				m_eStatus = CSISTATUS_IRW;
				// �赱ǰͼ��Ϊ��ͼ��
				m_pPack->lab1 = 0;
				// ������ʾ����Ϣ
				_UpdateDispInfo();
			}
			else
			{
				// �����ȡʧ�ܣ���ָ��ɵ�����
				m_pPack = psOldPack;
			}
			
			return eer;
		}
		break;
	case	CR_NIHILITY:								// ���ޣ���������
		{
			int		width;
			int		height;
			int		bitcount;
			
			width   = va_arg(argp, int);
			height	= va_arg(argp, int);
			bitcount= va_arg(argp, int);
			cnvmark	= va_arg(argp, int);
			
			va_end(argp);
			
			// ����ԭ�е�רҵ��
			psOldPack = m_pPack;

			DIBINFO	bmi;

			// ��дԴͼ����Ϣ�ṹ
			bmi.bmi.biSize     = sizeof(wBITMAPINFOHEADER);
			bmi.bmi.biWidth	   = width;
			bmi.bmi.biHeight   = height;
			bmi.bmi.biBitCount = bitcount;
			bmi.bmi.biPlanes   = 1;

			bmi.bmi.biCompression   = BI_RGB;
			bmi.bmi.biSizeImage     = 0;
			bmi.bmi.biXPelsPerMeter = 0;
			bmi.bmi.biYPelsPerMeter = 0;
			bmi.bmi.biClrImportant  = 0;
			bmi.bmi.biClrUsed       = 0;

			// �ϳɻҶȵ�ɫ��
			switch (bitcount)
			{
			case	1:
				*(ulong *)&(bmi.pal[0]) = 0;
				*(ulong *)&(bmi.pal[1]) = 0xffffff;
				break;
			case	4:
				for (i=0; i<16; i++)
				{
					*(ulong *)&(bmi.pal[i]) = ((i*17)<<16)|((i*17)<<8)|(i*17);
				}
				break;
			case	8:
				for (i=0; i<256; i++)
				{
					*(ulong *)&(bmi.pal[i]) = (i<<16)|(i<<8)|(i);
				}
				break;
			case	16:
			case	24:
			case	32:
				break;
			default:
				assert(0);
				return ER_NOTSUPPORT;
			}
			
			// ���벢��дרҵ��
			eer = (enum EXERESULT)_AllocSpecPack(&m_pPack, (wBITMAPINFO*)&bmi, cnvmark, (int)type, NULL, 1);

			if (eer == ER_SUCCESS)
			{
				// ������ھ�רҵ���������ͷ���
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// ����Ϊ��д״̬
				m_eStatus = CSISTATUS_IRW;
				// �赱ǰͼ��Ϊ��ͼ��
				m_pPack->lab1 = 0;
				// ������ʾ����Ϣ
				_UpdateDispInfo();
			}
			else
			{
				// �����ȡʧ�ܣ���ָ��ɵ�����
				m_pPack = psOldPack;
			}
			
			return eer;
		}
		break;
	case	CR_CLIP:									// ������
		{
			cnvmark	= va_arg(argp, int);
			
			va_end(argp);
			
			// ����ԭ�е�רҵ��
			psOldPack = m_pPack;

			// �жϼ��������Ƿ����ͼ��
			if (IsClipboardFormatAvailable(CF_DIB) == FALSE)
			{
				return ER_NONIMAGE;
			}

			// �򿪼�����
			if (::OpenClipboard(NULL))
			{
				// ��ȡ�������е�DIB����
				HANDLE h = ::GetClipboardData(CF_DIB);
				
				if (h == 0)
				{
					CloseClipboard();
					return ER_BADIMAGE;
				}

				// ������������Դ����ȡ��ַ
				LPSTR lpDIB = (LPSTR)::GlobalLock((HGLOBAL)h);
				wBITMAPINFOHEADER*	pbmi;
				char				*pbits;

				if (!lpDIB)
				{
					CloseClipboard();					// ����ļ��������ݾ������Ҫ�ͷţ�ϵͳ���Զ��ͷ�
					return ER_BADIMAGE;
				}
				
				// ���DIB��Ϣ�����Ч��
				if (*((DWORD*)lpDIB) != sizeof(wBITMAPINFOHEADER))
				{
					::GlobalUnlock((HGLOBAL)h);
					CloseClipboard();
					return ER_BADIMAGE;
				}
				
				pbmi  = (wBITMAPINFOHEADER*)lpDIB;
				pbits = (char*)(pbmi+1);

				// ���벢��дרҵ��
				eer = (enum EXERESULT)_AllocSpecPack(&m_pPack, (wBITMAPINFO*)pbmi, cnvmark, (int)type, pbits, 0);

				::GlobalUnlock((HGLOBAL)h);
				::CloseClipboard();
				
				if (eer == ER_SUCCESS)
				{
					// ������ھ�רҵ���������ͷ���
					if (psOldPack != NULL)
					{
						assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
						isirw_free_SPEC_DATAPACK(psOldPack);
					}
					
					// ����Ϊ��д״̬
					m_eStatus = CSISTATUS_IRW;
					// �赱ǰͼ��Ϊ��ͼ��
					m_pPack->lab1 = 0;
					// ������ʾ����Ϣ
					_UpdateDispInfo();
				}
				else
				{
					// �����ȡʧ�ܣ���ָ��ɵ�����
					m_pPack = psOldPack;
				}
				
				return eer;
			}
		}
		break;
	case	CR_WINDOW:									// ����
		{
			HWND	hWnd;

			hWnd    = va_arg(argp, HWND);
			cnvmark	= va_arg(argp, int);
			
			va_end(argp);
			
			// ����ԭ�е�רҵ��
			psOldPack = m_pPack;
			
			// �����µ�רҵ���ݰ�
			if ((m_pPack=isirw_alloc_SPEC_DATAPACK(ISEEIO_CONFER_LOC_FILE, CSINIHFILENAME, 0, 0, (IRWE_CNVMARK)cnvmark)) != NULL)
			{
				assert(m_pPack->org_img != NULL);

				CWnd		*pWnd = CWnd::FromHandle(hWnd);
				CBitmap		bitmap;
				CWindowDC	dc(pWnd);
				CDC			memDC;
				CRect		rect;
				
				memDC.CreateCompatibleDC(&dc);
				
				pWnd->GetWindowRect(rect);

				//�ô��ڴ�С����һ������λͼ
				bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());
				
				CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
				memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc, 0, 0, SRCCOPY);
				
				// ������ɫ��
				CPalette	pal;
				LOGPAL		logp;

				if (dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
				{
					logp.pal.palVersion    = 0x300;
					logp.pal.palNumEntries = GetSystemPaletteEntries(dc, 0, MAX_PALETTE_CNT, logp.pal.palPalEntry);
					pal.CreatePalette(&logp.pal);
				}
				
				memDC.SelectObject(pOldBitmap);
				memDC.DeleteDC();

				BITMAP		btm;
				
				// ���λͼ��Ϣ��λ��ȣ�
				bitmap.GetBitmap(&btm);
				
				int				width    = btm.bmWidth;
				int				height   = abs(btm.bmHeight);
				int				bitcount = btm.bmBitsPixel;
				LPINFOSTR		pinfo    = m_pPack->org_img;
				ulong	linesize = DIBSCANLINE_DWORDALIGN(width*bitcount);
				ulong	palcnt;
				
				// �������׵�ַ����
				if ((pinfo->pp_line_addr=(void**)isirw_malloc(sizeof(char*)*height)) == NULL)
				{
					isirw_free_SPEC_DATAPACK(m_pPack);
					m_pPack = psOldPack;
					return ER_MEMORYERR;
				}
				
				// �������ػ�����
				if ((pinfo->p_bit_data=(uchar *)isirw_malloc((int)linesize*height)) == NULL)
				{
					isirw_free_SPEC_DATAPACK(m_pPack);
					m_pPack = psOldPack;
					return ER_MEMORYERR;
				}

				DIBINFO			bih;

				bih.bmi.biSize			= sizeof(wBITMAPINFOHEADER);
				bih.bmi.biWidth			= btm.bmWidth;
				bih.bmi.biHeight		= btm.bmHeight;
				bih.bmi.biPlanes		= 1;
				bih.bmi.biBitCount		= btm.bmBitsPixel;
				bih.bmi.biSizeImage		= 0;
				bih.bmi.biXPelsPerMeter	= 0;
				bih.bmi.biYPelsPerMeter	= 0;
				bih.bmi.biClrImportant	= 0;
				bih.bmi.biClrUsed		= 0;
				bih.bmi.biCompression	= BI_RGB;
				
				::GetDIBits(dc, bitmap, 0, height, pinfo->p_bit_data, (BITMAPINFO*)&bih, DIB_RGB_COLORS);

				switch (bitcount)
				{
				case	1:
					SETMASK_8(pinfo);
					palcnt = 2;
					pal.GetPaletteEntries(0, palcnt, (LPPALETTEENTRY)pinfo->palette);

					for (i=0; i<(int)palcnt; i++)
					{
						pinfo->palette[i] = ((pinfo->palette[i]&0xff)<<16)|((pinfo->palette[i]&0xff0000)>>16)|((pinfo->palette[i]&0xff00));
					}
					break;
				case	4:
					SETMASK_8(pinfo);
					palcnt = 16;
					pal.GetPaletteEntries(0, palcnt, (LPPALETTEENTRY)pinfo->palette);

					for (i=0; i<(int)palcnt; i++)
					{
						pinfo->palette[i] = ((pinfo->palette[i]&0xff)<<16)|((pinfo->palette[i]&0xff0000)>>16)|((pinfo->palette[i]&0xff00));
					}
					break;
				case	8:
					SETMASK_8(pinfo);
					palcnt = 256;
					pal.GetPaletteEntries(0, palcnt, (LPPALETTEENTRY)pinfo->palette);

					for (i=0; i<(int)palcnt; i++)
					{
						pinfo->palette[i] = ((pinfo->palette[i]&0xff)<<16)|((pinfo->palette[i]&0xff0000)>>16)|((pinfo->palette[i]&0xff00));
					}
					break;
				case	16:
					if (bih.bmi.biCompression == BI_RGB)
					{
						pinfo->r_mask = 0x7c00;
						pinfo->g_mask = 0x3e0;
						pinfo->b_mask = 0x1f;
						pinfo->a_mask = 0x0;
					}
					else
					{
						pinfo->r_mask = 0xf800;
						pinfo->g_mask = 0x7e0;
						pinfo->b_mask = 0x1f;
						pinfo->a_mask = 0x0;
					}
					palcnt = 0;
					break;
				case	24:
					SETMASK_24(pinfo);
					palcnt = 0;
					break;
				case	32:
					SETMASK_32(pinfo);
					palcnt = 0;
					break;
				default:
					assert(0);
					isirw_free_SPEC_DATAPACK(m_pPack);
					m_pPack = psOldPack;
					return ER_NOTSUPPORT;
					break;
				}
				
				pinfo->imgtype     = IMT_RESSTATIC;		/* ͼ���ļ����� */
				pinfo->imgformat   = IMF_BMP;			/* ͼ���ļ���ʽ�������ʶ�� */
				pinfo->compression = ICS_RGB;			/* ͼ���ļ���ѹ����ʽ */

				pinfo->width    = (ulong)width;
				pinfo->height   = (ulong)height;
				pinfo->bitcount = (ulong)bitcount;
				pinfo->order    = (bih.bmi.biHeight > 0) ? 1 : 0;
				
				// ��д���׵�ַ����
				if (pinfo->order == 0)		/* ���� */
				{
					for (i=0;i<(int)(pinfo->height);i++)
					{
						pinfo->pp_line_addr[i] = (void *)(pinfo->p_bit_data+(i*linesize));
					}
				}
				else							/* ���� */
				{
					for (i=0;i<(int)(pinfo->height);i++)
					{
						pinfo->pp_line_addr[i] = (void *)(pinfo->p_bit_data+((pinfo->height-i-1)*linesize));
					}
				}
				
				pinfo->pal_count = palcnt;				/* ��ɫ����Ч��ĸ��� */
				
				pinfo->play_number = 0;					/* ����˳���ߴ磨��Ԫ��Ϊ��λ�� */
				pinfo->play_order  = NULL;				/* ����˳��� */
				pinfo->time        = 0;					/* ��֡ͣ��ʱ�� */
				pinfo->colorkey    = -1;				/* ��ͼ��͸��ɫ��-1��ʾ��͸��ɫ�� */
				
				pinfo->imgnumbers  = 1;					/* ���ļ���ͼ��ĸ��� */
				pinfo->psubimg     = NULL;				/* ��ͼ����������ַ */
				pinfo->data_state  = 2;					/* ���ݰ���ǰ������״̬ */
				
				m_pPack->command   = IRWE_SDPCOMM_LOAD;	/* ģ������� */
				m_pPack->data_state= SDP_DATA_STATE_ORG_PIX|SDP_DATA_STATE_ORG_INFO|SDP_DATA_STATE_PATH;
				m_pPack->exec_state= IRWE_SDPSTATE_IDLE;
				m_pPack->sn        = (int)type;
				
				if (cnvmark != IRWE_CNVMARK_NOT)
				{
					if (isirw_cnv(m_pPack, (IRWE_CNVMARK)cnvmark, 0) < 0)
					{
						isirw_free_SPEC_DATAPACK(m_pPack);
						m_pPack = psOldPack;
						return ER_MEMORYERR;
					}
				}
				
				eer = ER_SUCCESS;
			}
			else
			{
				eer = ER_MEMORYERR;
			}
			
			if (eer == ER_SUCCESS)
			{
				// ������ھ�רҵ���������ͷ���
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// ����Ϊ��д״̬
				m_eStatus = CSISTATUS_IRW;
				// �赱ǰͼ��Ϊ��ͼ��
				m_pPack->lab1 = 0;
				// ������ʾ����Ϣ
				_UpdateDispInfo();
			}
			else
			{
				// �����ȡʧ�ܣ���ָ��ɵ�����
				m_pPack = psOldPack;
			}
			
			return eer;
		}
		break;
	case	CR_RESOURCE_ID:								// ��Դ����ID��ʽ������
		{
			char	*filename;							// ��Դ�ļ�ȫ·����EXE��DLL��
			int		id;
			char	*pdib;
			wBITMAPINFO*	pmi;

			filename = va_arg(argp, char*);
			id       = va_arg(argp, int);
			cnvmark	 = va_arg(argp, int);
			
			va_end(argp);
			
			// ����ԭ�е�רҵ��
			psOldPack = m_pPack;

			// �ж��Ƿ��Ƕ�ȡ��ǰ�����е���Դ
			if (filename == NULL)
			{
				assert(id >= 0);						// ����ǰ�����е���Դʱû��ö������
				pdib = _LoadDIBFromResource((char*)MAKEINTRESOURCE(id));
			}
			else
			{
				pdib = _LoadDIBFromPE(filename, id);	// ��ָ����PE��ʽ�ļ��ж�ȡ��Դ
			}

			if (pdib == NULL)
			{
				return ER_NONIMAGE;						// û��ͼ������ݷǷ�
			}

			pmi = (wBITMAPINFO*)pdib;

			if ((pmi->bmiHeader.biSize != sizeof(wBITMAPINFOHEADER))||
				(pmi->bmiHeader.biWidth == 0)||(pmi->bmiHeader.biWidth > 32767)||
				(pmi->bmiHeader.biHeight == 0))
			{
				if (filename != NULL)
				{
					delete []pdib;
				}
				return ER_NONIMAGE;
			}

			if (pmi->bmiHeader.biCompression == BI_RGB)
			{
				// ���벢��дרҵ��
				eer = (enum EXERESULT)_AllocSpecPack(&m_pPack, (wBITMAPINFO*)pdib, cnvmark, (int)type, (char*)(pdib+sizeof(wBITMAPINFOHEADER)), 0);
			}
			else
			{
				int		imgsize = 0;
				int		bmpoff  = 0;

				// ����ͼ���ļ�����ʵ�ߴ�
				imgsize = sizeof(BITMAPFILEHEADER);
				imgsize += sizeof(wBITMAPINFOHEADER);

				// �ۼӵ�ɫ��ߴ�
				if (pmi->bmiHeader.biBitCount <= 8)
				{
					if (pmi->bmiHeader.biClrUsed == 0)
					{
						switch (pmi->bmiHeader.biBitCount)
						{
						case	1:
							imgsize += 2*sizeof(wRGBQUAD);
							break;
						case	2:
							imgsize += 4*sizeof(wRGBQUAD);
							break;
						case	4:
							imgsize += 16*sizeof(wRGBQUAD);
							break;
						case	8:
							imgsize += (unsigned int)256*sizeof(wRGBQUAD);
							break;
						default:
							if (filename != NULL)
							{
								delete []pdib;
							}
							return ER_NONIMAGE;
						}
					}
					else
					{
						imgsize += pmi->bmiHeader.biClrUsed*sizeof(wRGBQUAD);
					}
				}
				else
				{
					if (pmi->bmiHeader.biCompression == BI_BITFIELDS)
					{
						imgsize += 3*sizeof(ulong);
					}
				}

				bmpoff = imgsize;

				// �ۼ�λͼ�������ߴ�
				if (pmi->bmiHeader.biSizeImage == 0)
				{
					int lns = DIBSCANLINE_DWORDALIGN(pmi->bmiHeader.biWidth*pmi->bmiHeader.biBitCount);

					imgsize += lns*abs(pmi->bmiHeader.biHeight);
				}
				else
				{
					imgsize += pmi->bmiHeader.biSizeImage;
				}

				// �����ڴ��ļ������
				char	*pbmp = (char*)isirw_malloc(imgsize);

				if (pbmp == NULL)
				{
					if (filename != NULL)
					{
						delete []pdib;
					}
					return ER_MEMORYERR;				// �ڴ治��
				}

				LPBITMAPFILEHEADER	pfi = (LPBITMAPFILEHEADER)pbmp;

				// �����ڴ�BMP�ļ�
				pfi->bfType = ((ushort)('M'<<8)|'B');
				pfi->bfSize = imgsize;
				pfi->bfOffBits   = bmpoff;
				pfi->bfReserved1 = 0;
				pfi->bfReserved2 = 0;

				memcpy((void*)(pfi+1), (const void *)pmi, imgsize-sizeof(BITMAPFILEHEADER));

				// �����淢�Ͷ�ȡ����
				if (isirw_s_read_img_c(ISEEIO_CONFER_LOC_MEMORY, "a.bmp", (ulong)pbmp, imgsize, (int)type, (IRWE_CNVMARK)cnvmark, NULL, &m_pPack) == ISIRW_LASTERR_SUCCESS)
				{
					assert(m_pPack != NULL);
					
					// �ȴ����������
					eer = isirw_s_wait(m_pPack, 0);
					
					if (eer != ER_SUCCESS)					// ��ȡ����
					{
						assert(m_pPack->exec_state == IRWE_SDPSTATE_IDLE);
						isirw_free_SPEC_DATAPACK(m_pPack);
					}
				}
				else
				{
					eer = ER_MEMORYERR;						// �ڴ治�㣨ϵͳ���ȶ��Ŀ����Խ�С��
				}

				isirw_free(pbmp);
			}

			if (filename != NULL)						// �����ͷŴ�PE�ļ��ж���Ļ�����
			{
				delete []pdib;
			}
			
			if (eer == ER_SUCCESS)
			{
				// ������ھ�רҵ���������ͷ���
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// ����Ϊ��д״̬
				m_eStatus = CSISTATUS_IRW;
				// �赱ǰͼ��Ϊ��ͼ��
				m_pPack->lab1 = 0;
				// ������ʾ����Ϣ
				_UpdateDispInfo();
			}
			else
			{
				// �����ȡʧ�ܣ���ָ��ɵ�����
				m_pPack = psOldPack;
			}

			return eer;
		}
	case	CR_RESOURCE_NAME:							// ��Դ������Դ���ƴ���ʽ������
		{
			char	*resname;							// ��Դ��
			char	*pdib;

			resname  = va_arg(argp, char*);
			cnvmark	 = va_arg(argp, int);
			
			va_end(argp);
			
			// ����ԭ�е�רҵ��
			psOldPack = m_pPack;

			// ��ȡ��ǰ�����е���Դ
			pdib = _LoadDIBFromResource(resname);

			if (pdib == NULL)
			{
				return ER_NONIMAGE;						// û��ͼ������ݷǷ�
			}

			// ���벢��дרҵ��
			eer = (enum EXERESULT)_AllocSpecPack(&m_pPack, (wBITMAPINFO*)pdib, cnvmark, (int)type, (char*)(pdib+sizeof(wBITMAPINFOHEADER)), 0);
			
			if (eer == ER_SUCCESS)
			{
				// ������ھ�רҵ���������ͷ���
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// ����Ϊ��д״̬
				m_eStatus = CSISTATUS_IRW;
				// �赱ǰͼ��Ϊ��ͼ��
				m_pPack->lab1 = 0;
				// ������ʾ����Ϣ
				_UpdateDispInfo();
			}
			else
			{
				// �����ȡʧ�ܣ���ָ��ɵ�����
				m_pPack = psOldPack;
			}

			return eer;
		}
	case	CR_HBITMAP:									// HBITMAP���
		{
			HBITMAP	hBmp;								// Դλͼ���
			
			hBmp     = va_arg(argp, HBITMAP);
			cnvmark	 = va_arg(argp, int);
			
			va_end(argp);
			
			// ����ԭ�е�רҵ��
			psOldPack = m_pPack;
			
			HWND	hWnd = ::GetDesktopWindow();
			HDC		hDC;
			BITMAP	bmp;
			DIBINFO	binfo;

			assert(hWnd != NULL);
			
			// ��ȡ����λͼ�Ŀ�ȡ��߶ȼ���ɫ��ʽ��Ϣ
			if (::GetObject(hBmp, sizeof(BITMAP), (LPVOID)&bmp) != sizeof(BITMAP))
			{
				return ER_BADIMAGE;
			}
			
			// ����DIB��Ϣ������
			binfo.bmi.biSize   = sizeof(wBITMAPINFOHEADER);
			binfo.bmi.biWidth  = bmp.bmWidth;
			binfo.bmi.biHeight = bmp.bmHeight;
			binfo.bmi.biPlanes = 1;
			// ��16ɫϵͳ�У�ƽ������4����λ����1��������DIB���ǲ������
			binfo.bmi.biBitCount    = max(bmp.bmPlanes, bmp.bmBitsPixel);
			binfo.bmi.biCompression = BI_RGB;
			
			// �����豸�ֱ���
			hDC = ::GetDC(hWnd);
			binfo.bmi.biXPelsPerMeter = (GetDeviceCaps(hDC,HORZRES)*1000)/GetDeviceCaps(hDC,HORZSIZE);
			binfo.bmi.biYPelsPerMeter = (GetDeviceCaps(hDC,VERTRES)*1000)/GetDeviceCaps(hDC,VERTSIZE);
			::ReleaseDC(hWnd, hDC);
			
			binfo.bmi.biClrUsed      = 0;
			binfo.bmi.biClrImportant = 0;
			binfo.bmi.biSizeImage    = 0;
			
			// �����µ�רҵ���ݰ�
			if ((m_pPack=isirw_alloc_SPEC_DATAPACK(ISEEIO_CONFER_LOC_FILE, CSINIHFILENAME, 0, 0, (IRWE_CNVMARK)cnvmark)) != NULL)
			{
				assert(m_pPack->org_img != NULL);

				int				width    = binfo.bmi.biWidth;
				int				height   = abs(binfo.bmi.biHeight);
				int				bitcount = binfo.bmi.biBitCount;
				LPINFOSTR		pinfo    = m_pPack->org_img;
				char			*pbits   = 0;
				ulong	linesize = DIBSCANLINE_DWORDALIGN(width*bitcount);
				ulong	palcnt;

				// �������׵�ַ����
				if ((pinfo->pp_line_addr=(void**)isirw_malloc(sizeof(char*)*height)) == NULL)
				{
					isirw_free_SPEC_DATAPACK(m_pPack);
					m_pPack = psOldPack;
					return ER_MEMORYERR;
				}
				
				// �������ػ�����
				if ((pinfo->p_bit_data=(uchar *)isirw_malloc((int)linesize*height)) == NULL)
				{
					isirw_free_SPEC_DATAPACK(m_pPack);
					m_pPack = psOldPack;
					return ER_MEMORYERR;
				}

				// ��ȡ�������ݼ���ɫ������
				hDC = ::GetDC(hWnd);
				if (!::GetDIBits(hDC, hBmp, 0, height, (LPVOID)pinfo->p_bit_data, (wBITMAPINFO*)&binfo, DIB_RGB_COLORS))
				{
					::ReleaseDC(hWnd, hDC);
					isirw_free_SPEC_DATAPACK(m_pPack);
					m_pPack = psOldPack;
					return ER_BADIMAGE;
				}
				::ReleaseDC(hWnd, hDC);

				switch (bitcount)
				{
				case	1:
					SETMASK_8(pinfo);
					palcnt = 2;
					for (i=0; i<(int)palcnt; i++)
					{
						pinfo->palette[i] = *(ulong *)&(binfo.pal[i]);
					}
					break;
				case	4:
					SETMASK_8(pinfo);
					palcnt = 16;
					for (i=0; i<(int)palcnt; i++)
					{
						pinfo->palette[i] = *(ulong *)&(binfo.pal[i]);
					}
					break;
				case	8:
					SETMASK_8(pinfo);
					palcnt = 256;
					for (i=0; i<(int)palcnt; i++)
					{
						pinfo->palette[i] = *(ulong *)&(binfo.pal[i]);
					}
					break;
				case	16:
					if (binfo.bmi.biCompression == BI_RGB)
					{
						pinfo->r_mask = 0x7c00;
						pinfo->g_mask = 0x3e0;
						pinfo->b_mask = 0x1f;
						pinfo->a_mask = 0x0;
					}
					else
					{
						pinfo->r_mask = 0xf800;
						pinfo->g_mask = 0x7e0;
						pinfo->b_mask = 0x1f;
						pinfo->a_mask = 0x0;
					}
					palcnt = 0;
					break;
				case	24:
					SETMASK_24(pinfo);
					palcnt = 0;
					break;
				case	32:
					SETMASK_32(pinfo);
					palcnt = 0;
					break;
				default:
					assert(0);
					isirw_free_SPEC_DATAPACK(m_pPack);
					m_pPack = psOldPack;
					return ER_NOTSUPPORT;
					break;
				}
				
				pinfo->imgtype     = IMT_RESSTATIC;		/* ͼ���ļ����� */
				pinfo->imgformat   = IMF_BMP;			/* ͼ���ļ���ʽ�������ʶ�� */
				pinfo->compression = ICS_RGB;			/* ͼ���ļ���ѹ����ʽ */

				pinfo->width    = (ulong)width;
				pinfo->height   = (ulong)height;
				pinfo->bitcount = (ulong)bitcount;
				pinfo->order    = (binfo.bmi.biHeight > 0) ? 1 : 0;
				
				// ��д���׵�ַ����
				if (pinfo->order == 0)		/* ���� */
				{
					for (i=0;i<(int)(pinfo->height);i++)
					{
						pinfo->pp_line_addr[i] = (void *)(pinfo->p_bit_data+(i*linesize));
					}
				}
				else							/* ���� */
				{
					for (i=0;i<(int)(pinfo->height);i++)
					{
						pinfo->pp_line_addr[i] = (void *)(pinfo->p_bit_data+((pinfo->height-i-1)*linesize));
					}
				}
				
				pinfo->pal_count = palcnt;				/* ��ɫ����Ч��ĸ��� */
				
				pinfo->play_number = 0;					/* ����˳���ߴ磨��Ԫ��Ϊ��λ�� */
				pinfo->play_order  = NULL;				/* ����˳��� */
				pinfo->time        = 0;					/* ��֡ͣ��ʱ�� */
				pinfo->colorkey    = -1;				/* ��ͼ��͸��ɫ��-1��ʾ��͸��ɫ�� */
				
				pinfo->imgnumbers  = 1;					/* ���ļ���ͼ��ĸ��� */
				pinfo->psubimg     = NULL;				/* ��ͼ����������ַ */
				pinfo->data_state  = 2;					/* ���ݰ���ǰ������״̬ */
				
				m_pPack->command   = IRWE_SDPCOMM_LOAD;	/* ģ������� */
				m_pPack->data_state= SDP_DATA_STATE_ORG_PIX|SDP_DATA_STATE_ORG_INFO|SDP_DATA_STATE_PATH;
				m_pPack->exec_state= IRWE_SDPSTATE_IDLE;
				m_pPack->sn        = (int)type;
				
				if (cnvmark != IRWE_CNVMARK_NOT)
				{
					if (isirw_cnv(m_pPack, (IRWE_CNVMARK)cnvmark, 0) < 0)
					{
						isirw_free_SPEC_DATAPACK(m_pPack);
						m_pPack = psOldPack;
						return ER_MEMORYERR;
					}
				}
				
				eer = ER_SUCCESS;
			}
			else
			{
				eer = ER_MEMORYERR;
			}

			if (eer == ER_SUCCESS)
			{
				// ������ھ�רҵ���������ͷ���
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// ����Ϊ��д״̬
				m_eStatus = CSISTATUS_IRW;
				// �赱ǰͼ��Ϊ��ͼ��
				m_pPack->lab1 = 0;
				// ������ʾ����Ϣ
				_UpdateDispInfo();
			}
			else
			{
				// �����ȡʧ�ܣ���ָ��ɵ�����
				m_pPack = psOldPack;
			}

			return eer;
		}
		break;											
	case	CR_HDIB:									// HDIB���
		{
			HDIB			hDib;
			wBITMAPINFO*	pbmi;
			char			*pbits = 0;
			
			hDib     = va_arg(argp, HDIB);
			cnvmark	 = va_arg(argp, int);
			
			va_end(argp);
			
			// ����ԭ�е�רҵ��
			psOldPack = m_pPack;
			
			pbmi  = (wBITMAPINFO*)(char*)hDib;
			pbits = (char*)(((char*)pbmi)+sizeof(wBITMAPINFOHEADER));

			// �ж�Դ�����Ƿ���Ч
			if (pbmi->bmiHeader.biSize != sizeof(wBITMAPINFOHEADER))
			{
				return	ER_BADIMAGE;
			}

			// ���벢��дרҵ��
			eer = (enum EXERESULT)_AllocSpecPack(&m_pPack, (wBITMAPINFO*)pbmi, cnvmark, (int)type, pbits, 0);

			if (eer == ER_SUCCESS)
			{
				// ������ھ�רҵ���������ͷ���
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// ����Ϊ��д״̬
				m_eStatus = CSISTATUS_IRW;
				// �赱ǰͼ��Ϊ��ͼ��
				m_pPack->lab1 = 0;
				// ������ʾ����Ϣ
				_UpdateDispInfo();
			}
			else
			{
				// �����ȡʧ�ܣ���ָ��ɵ�����
				m_pPack = psOldPack;
			}

			return eer;
		}
		break;
	default:
		va_end(argp);
		assert(0);
		break;
	}

	return 0;
}


// ��ȡ�Ѷ���ͼ��ĸ�����Ϣ
int CSeeImage::GetInfo(int flag, LPIMAGEINFOSTR pimginfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// ��ȡ���״̬����д��IP״̬��
	// ��Ϊ�ù��ܿ��ܻ���û��ͼ�����ݵ�����µ��ã�Ϊ�˱��ⴥ�����ԣ������ȶ��������ж�
	if (pimginfo == NULL)
	{
		if (flag == CSTATUS)
		{
			return (int)m_eStatus;						
		}
		else if (flag == CENGSTATUS)
		{
			return m_bMark;
		}
		else if (flag == CHAVIMG)
		{
			return (m_eStatus == CSISTATUS_NULL) ? 0 : 1;
		}
	}
	else
	{
		if (pimginfo->flag == CSTATUS)
		{
			pimginfo->status = m_eStatus;
			return 0;
		}
		else if (flag == COPENSTR)
		{
			pimginfo->openstr = isirw_get_openstr();
			return 0;
		}
		else if (flag == CENGSTATUS)
		{
			pimginfo->engstatus = m_bMark;
			return 0;
		}
		else if (flag == CHAVIMG)
		{
			pimginfo->havimg = (m_eStatus == CSISTATUS_NULL) ? 0 : 1;
			return 0;
		}
	}

	// ��ʱ��������Ѿ�����
	Assert(m_bMark, "GetInfo()���������滹δ������");

	if (!m_bMark)
	{
		return 0;
	}
	
	Assert(m_eStatus != CSISTATUS_NULL, "GetInfo()���������б������ͼ�����ݡ�");
	Assert(m_pPack&&m_pPack->org_img, "GetInfo()���������б������ͼ�����ݡ�");

	LPINFOSTR	pinfo = (m_pPack->cnv_img) ? m_pPack->cnv_img : m_pPack->org_img;

	// �ж��Ƿ��ǻ�ȡ��������
	if (pimginfo == NULL)
	{
		int	result = 0;

		if (flag == CCURIMGNUM)							// ��ǰͼ�����ţ�0����
		{
			return (int)m_pPack->lab1;
		}
		else if (flag == CIMGCNT)						// ��ȡͼ���ҳ��
		{
			return (int)m_pPack->org_img->imgnumbers;
		}
		else
		{
			// ����ǻ�ȡ��ͼ����Ϣ
			if (m_pPack->lab1 == 0)
			{
				switch (flag)
				{
				case	CWIDTH:							// ��ȡͼ����
					result = (int)pinfo->width;
					break;
				case	CHEIGHT:						// ��ȡͼ��߶�
					result = (int)pinfo->height;
					break;
				case	CBITCNT:						// λ���
					result = (int)pinfo->bitcount;
					break;
				case	CORDER:							// ���÷�ʽ���������
					result = (int)pinfo->order;
					break;
				case	CPALCNT:						// ��ȡ��ɫ����ĸ���
					result = (int)pinfo->pal_count;
					break;
				default:
					Assert(0, "GetInfo()��������ݷ�ʽ��һ��ֻ�ܻ�ȡһ��������");
					return 0;
					break;
				}
			}
			else
			{
				// ��ȡ��ǰͼ��ṹ�ĵ�ַ
				SUBIMGBLOCK* psub = _CalSubImageAddr(pinfo, m_pPack->lab1);

				assert(psub);

				switch (flag)
				{
				case	CWIDTH:							// ��ȡͼ����
					result = (int)psub->width;
					break;
				case	CHEIGHT:						// ��ȡͼ��߶�
					result = (int)psub->height;
					break;
				case	CBITCNT:						// λ���
					result = (int)psub->bitcount;
					break;
				case	CORDER:							// ���÷�ʽ���������
					result = (int)psub->order;
					break;
				case	CPALCNT:						// ��ȡ��ɫ����ĸ���
					result = (int)psub->pal_count;
					break;
				default:
					Assert(0, "GetInfo()��������ݷ�ʽ��һ��ֻ�ܻ�ȡһ��������");
					return 0;
					break;
				}
			}

			return result;
		}
	}
	else
	{
		flag = pimginfo->flag;

		// ��ʼ���ṹ�и�������ֵ��flag��Ա�������⣩
		memset((void*)(((char*)pimginfo)+sizeof(ulong)), 0, sizeof(IMAGEINFOSTR)-sizeof(ulong));

		if (flag & CCURIMGNUM)							// ��ǰͼ�����ţ�0����
		{
			pimginfo->currimgnum = (int)m_pPack->lab1;
		}

		if (flag & CSTATUS)								// ��ȡ���״̬����д��IP״̬��
		{
			pimginfo->status = m_eStatus;
		}

		if (flag & CIMGCNT)								// ��ȡͼ���ҳ��
		{
			pimginfo->imgcnt = (int)m_pPack->org_img->imgnumbers;
		}

		if (flag & CPACKPTR)							// ��ȡרҵ����ַ
		{
			pimginfo->pack = m_pPack;
		}

		if (flag & COPENSTR)							// ��ȡ����open��
		{
			pimginfo->openstr = isirw_get_openstr();
		}

		if (flag == CENGSTATUS)							// ��ȡ����״̬
		{
			pimginfo->engstatus = m_bMark;
		}
		
		// ����ǻ�ȡ��ͼ����Ϣ
		if (m_pPack->lab1 == 0)
		{
			if (flag & CWIDTH)							// ��ȡͼ����
			{
				pimginfo->width = pinfo->width;
			}

			if (flag & CHEIGHT)							// ��ȡͼ��߶�
			{
				pimginfo->height = pinfo->height;
			}

			if (flag & CBITCNT)							// λ���
			{
				pimginfo->bitcount = pinfo->bitcount;
			}

			if (flag & CORDER)							// ���÷�ʽ
			{
				pimginfo->order = pinfo->order;
			}

			if (flag & CPALCNT)							// ��ȡ��ɫ����ĸ���
			{
				pimginfo->pal_count = pinfo->pal_count;
			}

			if (flag & CPALETTE)						// ��ȡ��ɫ������
			{
				memcpy((void*)pimginfo->palette, (const void *)pinfo->palette, sizeof(ulong)*pinfo->pal_count);
			}

			if (flag & CBITS)							// ��ȡ���ػ������׵�ַ
			{
				pimginfo->pbits = (char*)pinfo->p_bit_data;
			}
		}
		else
		{
			// ��ȡ��ǰͼ��ṹ�ĵ�ַ
			SUBIMGBLOCK* psub = _CalSubImageAddr(pinfo, m_pPack->lab1);
			
			assert(psub);

			if (flag & CWIDTH)							// ��ȡͼ����
			{
				pimginfo->width = psub->width;
			}
			
			if (flag & CHEIGHT)							// ��ȡͼ��߶�
			{
				pimginfo->height = psub->height;
			}
			
			if (flag & CBITCNT)							// λ���
			{
				pimginfo->bitcount = psub->bitcount;
			}
			
			if (flag & CORDER)							// ���÷�ʽ
			{
				pimginfo->order = psub->order;
			}

			if (flag & CPALCNT)							// ��ȡ��ɫ����ĸ���
			{
				pimginfo->pal_count = psub->pal_count;
			}
			
			if (flag & CPALETTE)						// ��ȡ��ɫ������
			{
				memcpy((void*)pimginfo->palette, (const void *)psub->palette, sizeof(ulong)*psub->pal_count);
			}

			if (flag & CBITS)							// ��ȡ���ػ������׵�ַ
			{
				pimginfo->pbits = (char*)psub->p_bit_data;
			}
		}
	}

	return 0;
}


// �������е�ͼ��ָ��λ��
int CSeeImage::Save(CSITYPEW type, ...)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// ��������Ѿ�����
	Assert(m_bMark, "Save()���������滹δ������");

	if (!m_bMark)
	{
		return 0;
	}
	
	// ���б����Ѿ���ͼ������
	Assert((m_eStatus!=CSISTATUS_NULL)&&m_pPack&&m_pPack->org_img, "Save���������б������ͼ�����ݡ�");

	va_list			argp;
	LPINFOSTR		pOldInfo = NULL;
	int				result   = (int)ER_SUCCESS;
	
	va_start(argp, type);

	// ��ڲ������ͼ��
	TYPESAFE_CHECK(CSIPARMCHK_SAVE);

	// �����򿪹�IPģʽ���򱣴�IP��������ͼ���û����ܶ������й��޸ġ�
	if (m_bIsChange)
	{
		assert(m_pPack->cnv_img&&(m_pPack->cnv_img->bitcount==IP_BITCNT));
		
		// ��ת������IP������������ԭʼͼ���
		pOldInfo = m_pPack->org_img;
		m_pPack->org_img = m_pPack->cnv_img;
		m_pPack->cnv_img = NULL;
		
		m_pPack->data_state &= ~SDP_DATA_STATE_CNV;
	}
	
	switch (type)
	{
	case	CW_FILE:									// �����ļ�����
	case	CW_FILEFAST:								// �����ļ����ͣ����ٰ棩
		{
			char			*fname;
			CSAVETYPE		bitcount;
			int				par1;
			int				par2;
			
			fname    = va_arg(argp, char*);
			bitcount = va_arg(argp, CSAVETYPE);
			par1     = va_arg(argp, int);
			par2     = va_arg(argp, int);
			
			va_end(argp);

			// ���û��ָ��Ŀ���ļ�������ʹ�öԻ���
			if (fname == NULL)
			{
				CFileDialogEx	dsa(m_pPack);

				// ʹ�ö��Ƶġ����Ϊ���Ի��򱣴�ͼ��
				result = dsa.DoModal();

				if (result != IDOK)
				{
					result = ER_USERBREAK;
				}
				else
				{
					result = ER_SUCCESS;
				}
			}
			else
			{
				SAVESTR			savestr;

				// ���ñ������
				savestr.para_value[0] = par1;
				savestr.para_value[1] = par2;
				
				// ����ͼ��
				result = _SaveSpecPack((ISEEIO_CONFER_TYPE)type, fname, 0, 0, bitcount, &savestr);
			}
		}
		break;
	case	CW_MEMORY:									// �����ڴ�����
		{
			char			*pname;
			char			**ppaddr;
			int				*pimgsize;
			CSAVETYPE		bitcount;
			int				par1;
			int				par2;
			
			pname    = va_arg(argp, char*);
			ppaddr   = va_arg(argp, char**);
			pimgsize = va_arg(argp, int*);
			bitcount = va_arg(argp, CSAVETYPE);
			par1     = va_arg(argp, int);
			par2     = va_arg(argp, int);
			
			va_end(argp);
			
			SAVESTR			savestr;
			
			// ���ñ������
			savestr.para_value[0] = par1;
			savestr.para_value[1] = par2;
			
			// ����ͼ��
			result = _SaveSpecPack((ISEEIO_CONFER_TYPE)type, pname, (ulong)0, (ulong)0, bitcount, &savestr);

			if (result == ER_SUCCESS)
			{
				// ��д�ڴ����׵�ַ��ͼ��ߴ磨���ڴ�������ImFree()�ӿں����ͷţ�
				*ppaddr   = (char*)m_pPack->despath->param1;
				*pimgsize = (int)m_pPack->despath->param2;
				assert((*ppaddr)&&(*pimgsize));
			}
			else
			{
				*ppaddr   = NULL;
				*pimgsize = 0;
			}
		}
		break;
	case	CW_CLIP:									// ������
		{
			LPRECT	prc;

			prc = va_arg(argp, LPRECT);

			va_end(argp);

			// ת��������
			LPINFOSTR	pinfo = (m_pPack->cnv_img != NULL) ? m_pPack->cnv_img : m_pPack->org_img;
			CRect		rc;

			if (prc != NULL)
			{
				// ����1��4λ���ͼ�󣬲�֧�־ֲ�ճ��
				assert(pinfo->bitcount >= 8);
				rc.SetRect(prc->left, prc->top, prc->right, prc->bottom);
			}
			else
			{
				rc.SetRect(0, 0, pinfo->width, pinfo->height);
			}

			rc.NormalizeRect();

			// ���α���Ϸ�
			assert(pinfo);
			Assert((rc.left<(int)pinfo->width)&&(rc.top<(int)pinfo->height), "Save������CLIP���ͣ�����ľ����������Ϸ���");
			Assert((rc.Width()>0)&&(rc.Height()>0), "Save������CLIP���ͣ�����ľ����������Ϸ���");

			// ���þ���
			rc.right  = min(rc.right, (int)pinfo->width);
			rc.bottom = min(rc.bottom, (int)pinfo->height);

			int			linesize = DIBSCANLINE_DWORDALIGN(rc.Width()*pinfo->bitcount);
			int			imgsize  = linesize*rc.Height();
			int			headsize = sizeof(wBITMAPINFOHEADER);
			int			colssize = 0;
			int			bpp      = 0;

			// ȷ����ɫ��ߴ�
			switch (pinfo->bitcount)
			{
			case	1:
			case	4:
			case	8:
				colssize = (1<<pinfo->bitcount)*sizeof(wRGBQUAD);
				bpp      = 1;
				break;
			case	16:
				colssize = 0;
				bpp      = 2;
				break;
			case	24:
				colssize = 0;
				bpp      = 3;
				break;
			case	32:
				colssize = 0;
				bpp      = 4;
				break;
			default:
				assert(0);
				break;
			}

			HGLOBAL			hData = ::GlobalAlloc(GHND, headsize+colssize+imgsize);
			if (hData == NULL)
			{
				result = ER_MEMORYERR;
				break;
			}

			char			*pda = (char*)::GlobalLock(hData);
			if (pda == NULL)
			{
				::GlobalFree(hData);
				result = ER_MEMORYERR;
				break;
			}

			wBITMAPINFO*	pbmi = (wBITMAPINFO*)pda;
			char			*pbit= pda+headsize+colssize;
			char			*pcpy;
			int				y;

			// ����DIB��Ϣͷ�ṹ
			pbmi->bmiHeader.biSize          = sizeof(wBITMAPINFOHEADER);
			pbmi->bmiHeader.biWidth         = rc.Width();
			pbmi->bmiHeader.biHeight        = rc.Height();				// �̶�����Ϊ����ͼ
			pbmi->bmiHeader.biPlanes        = 1;
			pbmi->bmiHeader.biBitCount      = (WORD)pinfo->bitcount;
			pbmi->bmiHeader.biCompression   = BI_RGB;
			pbmi->bmiHeader.biSizeImage     = 0;
			pbmi->bmiHeader.biXPelsPerMeter = 0;
			pbmi->bmiHeader.biYPelsPerMeter = 0;
			pbmi->bmiHeader.biClrUsed       = 0;
			pbmi->bmiHeader.biClrImportant  = 0;
			
			// ������ɫ������
			if (pinfo->pal_count > 0)
			{
				memcpy((void*)pbmi->bmiColors, (const void *)pinfo->palette, sizeof(wRGBQUAD)*pinfo->pal_count);
			}
			
			// ������������
			for (y=rc.top; y<rc.bottom; y++)
			{
				pcpy = (char*)pinfo->pp_line_addr[y] + rc.left*bpp;
				memcpy((void*)(pbit+(rc.bottom-1-y)*linesize), (const void *)pcpy, rc.Width()*bpp);
			}

			// �ڴ�������׼�����������壩
			::GlobalUnlock(hData);

			// �򿪼�����
			if (::OpenClipboard(NULL))
			{
				// �����������
				if (::EmptyClipboard())
				{
					// ��ͼ����������
					if (::SetClipboardData(CF_DIB, hData) == NULL)
					{
						result = ER_SYSERR;
					}
					else
					{
						result = ER_SUCCESS;			// ���ͳɹ�
					}
				}
				else
				{
					result = ER_SYSERR;
				}

				::CloseClipboard();
			}
			else
			{
				result = ER_SYSERR;
			}

			// �������ʧ�ܣ����ͷ���ǰ������ڴ�
			if (result != ER_SUCCESS)
			{
				::GlobalFree(hData);
			}
		}
		break;
	case	CW_HBITMAP:									// HBITMAP���
		{
			HBITMAP		*phb;

			phb = va_arg(argp, HBITMAP *);
			
			va_end(argp);
			
			HWND	hWnd = ::GetDesktopWindow();
			HDC		hDC  = ::GetDC(hWnd);
			wBITMAPINFOHEADER	bih;
			DIBINFO				binfo;
			HBITMAP				bmp;

			if ((hWnd == NULL)||(hDC == NULL))
			{
				result = ER_SYSERR;
				*phb   = NULL;
				break;
			}

			LPINFOSTR	pinfo = (m_pPack->cnv_img != NULL) ? m_pPack->cnv_img : m_pPack->org_img;

			// ����Ŀ��λͼ��Ϣͷ�ṹ
			bih.biSize          = sizeof(wBITMAPINFOHEADER);
			bih.biWidth         = pinfo->width;
			bih.biHeight        = (pinfo->order == 0) ? -(long)pinfo->height : pinfo->height;
			bih.biPlanes        = 1;
			bih.biBitCount      = (WORD)pinfo->bitcount;
			bih.biCompression   = BI_RGB;
			bih.biSizeImage     = 0;
			bih.biXPelsPerMeter = 0;
			bih.biYPelsPerMeter = 0;
			bih.biClrUsed       = 0;
			bih.biClrImportant  = 0;
			
			memcpy((void*)&binfo.bmi, (const void *)&bih, sizeof(wBITMAPINFOHEADER));

			// ������ɫ������
			if (pinfo->pal_count > 0)
			{
				memcpy((void*)binfo.pal, (const void *)pinfo->palette, sizeof(wRGBQUAD)*pinfo->pal_count);
			}
			
			// ����DDBλͼ
			bmp = ::CreateDIBitmap(hDC, &bih, CBM_INIT, (const void *)pinfo->p_bit_data, (CONST BITMAPINFO *)&binfo, DIB_RGB_COLORS);

			::ReleaseDC(hWnd, hDC);

			if (bmp != NULL)
			{
				*phb = bmp;
				result = ER_SUCCESS;
			}
			else
			{
				*phb = NULL;
				result = ER_SYSERR;
			}
		}
		break;
	case	CW_HDIB:									// HDIB���
		{
			HDIB	*phdib;

			phdib = va_arg(argp, HDIB *);

			va_end(argp);

			// ת��������
			LPINFOSTR	pinfo = (m_pPack->cnv_img != NULL) ? m_pPack->cnv_img : m_pPack->org_img;

			assert(pinfo);

			int			linesize = DIBSCANLINE_DWORDALIGN(pinfo->width*pinfo->bitcount);
			int			imgsize  = linesize*pinfo->height;
			int			headsize = sizeof(wBITMAPINFOHEADER);
			int			colssize = 0;

			// ȷ����ɫ��ߴ�
			if ((pinfo->bitcount==1)||(pinfo->bitcount==4)||(pinfo->bitcount==8))
			{
				colssize = (1<<pinfo->bitcount)*sizeof(wRGBQUAD);
			}
			else
			{
				colssize = 0;
			}

			char	*pimg = (char*)ImAlloc(headsize+colssize+imgsize);

			if (pimg == NULL)
			{
				result = ER_MEMORYERR;
				*phdib = NULL;
				break;
			}

			wBITMAPINFO*	pbmi = (wBITMAPINFO*)pimg;
			char			*pbit= pimg+headsize+colssize;
			char			*pcpy;
			int				y;

			// ����DIB��Ϣͷ�ṹ
			pbmi->bmiHeader.biSize          = sizeof(wBITMAPINFOHEADER);
			pbmi->bmiHeader.biWidth         = pinfo->width;
			pbmi->bmiHeader.biHeight        = pinfo->height;	// �̶�����Ϊ����ͼ
			pbmi->bmiHeader.biPlanes        = 1;
			pbmi->bmiHeader.biBitCount      = (WORD)pinfo->bitcount;
			pbmi->bmiHeader.biCompression   = BI_RGB;
			pbmi->bmiHeader.biSizeImage     = 0;
			pbmi->bmiHeader.biXPelsPerMeter = 0;
			pbmi->bmiHeader.biYPelsPerMeter = 0;
			pbmi->bmiHeader.biClrUsed       = 0;
			pbmi->bmiHeader.biClrImportant  = 0;
			
			// ������ɫ������
			if (pinfo->pal_count > 0)
			{
				memcpy((void*)pbmi->bmiColors, (const void *)pinfo->palette, sizeof(wRGBQUAD)*pinfo->pal_count);
			}
			
			// ������������
			for (y=0; y<(int)pinfo->height; y++)
			{
				pcpy = (char*)pinfo->pp_line_addr[y];
				memcpy((void*)(pbit+(pinfo->height-1-y)*linesize), (const void *)pcpy, linesize);
			}

			// ����DIB���ݿ�
			*phdib = pimg;

			result = ER_SUCCESS;
		}
		break;
	default:
		assert(0);
		break;
	}

	if (m_bIsChange)
	{
		m_pPack->cnv_img = m_pPack->org_img;
		m_pPack->org_img = pOldInfo;
		
		m_pPack->data_state |= SDP_DATA_STATE_CNV;
	}
	
	return result;
}


// �����е�ͼ����ָ���ķ�ʽ�����������ָ���Ĵ�����
int CSeeImage::Draw(CSIDRAW type, ...)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// ��������Ѿ�����
	Assert(m_bMark, "Draw()���������滹δ������");

	if (!m_bMark)
	{
		return 0;
	}
	
	// �������û��ͼ����ֱ�ӷ���
	if (m_eStatus == CSISTATUS_NULL)
	{
		return 0;
	}

	assert(m_pPack&&m_pPack->org_img);
	
	va_list			argp;
	LPINFOSTR		pinfo  = (m_pPack->cnv_img != NULL) ? m_pPack->cnv_img : m_pPack->org_img;
	
	va_start(argp, type);
	
	// ��ڲ������ͼ��
	
	switch (type)
	{
	case	CD_NON:										// ��������
		{
			HDC	hdc;
			int	x, y;

			hdc = va_arg(argp, HDC);
			x   = va_arg(argp, int);
			y   = va_arg(argp, int);
			
			va_end(argp);

			if (m_hDrawDib&&(pinfo->order==1))
			{
				DrawDibDraw(m_hDrawDib, hdc, 
					x, y, pinfo->width, pinfo->height,
					(wBITMAPINFOHEADER*)&m_cDispInfo, 
					(LPVOID)pinfo->p_bit_data, 
					0, 0, pinfo->width, pinfo->height, 
					DDF_HALFTONE);
			}
			else
			{	
				StretchDIBits(hdc, 
					x, y, m_cDispInfo.bmi.biWidth, abs(m_cDispInfo.bmi.biHeight), 
					0, 0, m_cDispInfo.bmi.biWidth, abs(m_cDispInfo.bmi.biHeight), 
					pinfo->p_bit_data, (wBITMAPINFO*)&m_cDispInfo, DIB_RGB_COLORS, SRCCOPY);
			}
		}
		break;
	case	CD_STL:										// �������
		{
			HDC	hdc;
			int	x, y, w, h;
			
			hdc = va_arg(argp, HDC);
			x   = va_arg(argp, int);
			y   = va_arg(argp, int);
			w   = va_arg(argp, int);
			h   = va_arg(argp, int);
			
			va_end(argp);
			
			if (m_hDrawDib&&(pinfo->order==1))
			{
				DrawDibDraw(m_hDrawDib, hdc, 
					x, y, w, h,
					(wBITMAPINFOHEADER*)&m_cDispInfo, 
					(LPVOID)pinfo->p_bit_data, 
					0, 0, pinfo->width, pinfo->height, 
					DDF_HALFTONE);
			}
			else
			{	
				StretchDIBits(hdc, 
				x, y, w, h, 
				0, 0, m_cDispInfo.bmi.biWidth, abs(m_cDispInfo.bmi.biHeight), 
				pinfo->p_bit_data, (wBITMAPINFO*)&m_cDispInfo, DIB_RGB_COLORS, SRCCOPY);
			}
		}
		break;
	case	CD_CEN:										// ���С���������
		{
			HDC	hdc;
			int	x, y, w, h;
			
			hdc = va_arg(argp, HDC);
			x   = va_arg(argp, int);
			y   = va_arg(argp, int);
			w   = va_arg(argp, int);
			h   = va_arg(argp, int);
			
			va_end(argp);
			
			x = x+w/2-m_cDispInfo.bmi.biWidth/2;
			y = y+h/2-abs(m_cDispInfo.bmi.biHeight)/2;
			
			StretchDIBits(hdc, 
				x, y, m_cDispInfo.bmi.biWidth, abs(m_cDispInfo.bmi.biHeight), 
				0, 0, m_cDispInfo.bmi.biWidth, abs(m_cDispInfo.bmi.biHeight), 
				pinfo->p_bit_data, (wBITMAPINFO*)&m_cDispInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		break;
	case	CD_TAN:										// ͸������������
		break;
	default:
		break;
	}

	return 0;
}


// �ض��帳ֵ������
CSeeImage& CSeeImage::operator=(CSeeImage&sou)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// ��������Ѿ�����
	Assert(m_bMark, "OpenIPMode()���������滹δ������");
	
	if (!m_bMark)
	{
		return *this;
	}

	// ��������Ƿ���ͼ��
	if (m_eStatus != CSISTATUS_NULL)
	{
		if (m_eStatus == CSISTATUS_IP)
		{
			CloseIPMode();
		}

		assert(m_pPack != NULL);
		
		// ��������Ƿ�ԭ��ͼ��
		isirw_free_SPEC_DATAPACK(m_pPack);

		m_eStatus   = CSISTATUS_NULL;
		m_pPack     = NULL;
		m_bIsChange	= false;
		
		memset((void*)&m_cDispInfo, 0, sizeof(DIBINFO));
	}

	assert(m_eStatus == CSISTATUS_NULL);

	// ��������
	m_pPack     = sou.m_pPack;							// רҵ���ݰ�ָ��
	m_eStatus   = sou.m_eStatus;						// ��״̬���
	m_bIsChange = sou.m_bIsChange;						// �Ƿ�򿪹�ͼ��IPģʽ

	// ��ʾ����Ϣ
	memcpy((void*)&m_cDispInfo, (const void *)&sou.m_cDispInfo, sizeof(DIBINFO));
	
	// Դ���ÿ�
	sou.m_pPack     = NULL;
	sou.m_eStatus   = CSISTATUS_NULL;
	sou.m_bIsChange = false;
	memset((void*)&sou.m_cDispInfo, 0, sizeof(DIBINFO));

	return *this;
}


// ������е�ͼ������еĻ���
void CSeeImage::Clear()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// ��������Ѿ�����
	Assert(m_bMark, "OpenIPMode()���������滹δ������");
	
	if (!m_bMark)
	{
		return;
	}

	Assert(m_uMagic == CSEEIMAGE_MAGIC, "Clear���������ڴ��⵽�ƻ���");
	
	if (m_pPack != NULL)
	{
		if (m_eStatus == CSISTATUS_IP)
		{
			CloseIPMode();
		}

		// �ͷ�רҵ���ݰ�
		isirw_free_SPEC_DATAPACK(m_pPack);

		m_pPack = NULL;
	}

	// ���ÿ�
	m_eStatus   = CSISTATUS_NULL;
	m_bIsChange = false;
	memset((void*)&m_cDispInfo, 0, sizeof(DIBINFO));

	return;
}


int CSeeImage::OpenIPMode()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// ��������Ѿ�����
	Assert(m_bMark, "OpenIPMode()���������滹δ������");

	if (!m_bMark)
	{
		return 0;
	}
	
	// ��������Ƿ���ͼ��
	if (m_eStatus == CSISTATUS_NULL)
	{
		assert(m_pPack == NULL);

		return 1;										// ���л�û��ͼ������
	}

	assert((m_pPack != NULL)&&(m_pPack->org_img != NULL));

	// ����Ƿ��Ѵ���IP(Image Process)ģʽ
	if (m_eStatus == CSISTATUS_IP)
	{
		assert(m_pPack&&m_pPack->cnv_img&&(m_pPack->cnv_img->bitcount==IP_BITCNT));
		assert(m_bIsChange == true);

		return 2;										// �Ѵ���IPģʽ
	}

	m_eStatus = CSISTATUS_IP;

	LPINFOSTR		poldcnv = NULL;
	IRWE_CNVMARK	oldcnvmark;

	if (m_pPack->cnv_img != NULL)
	{
		// �������32λ��ת��������ֱ��ʹ����
		if (m_pPack->cnv_img->bitcount == IP_BITCNT)
		{
			m_bIsChange = true;
			return 0;
		}
		else
		{
			// ����Ƿ�32λת���������ȱ�����
			poldcnv          = m_pPack->cnv_img;
			m_pPack->cnv_img = NULL;
			oldcnvmark       = m_pPack->cnvmark;
		}
	}

	// ����IP��������32λת������
	switch (isirw_cnv(m_pPack, IRWE_CNVMARK_32, 1))
	{
	case	-1:											// ����ϵͳ�쳣���޷�ת��IPģʽ
		if (poldcnv != NULL)
		{
			m_pPack->cnv_img = poldcnv;
			m_pPack->cnvmark = oldcnvmark;
		}
		m_eStatus = CSISTATUS_IRW;
		return 3;
	case	-2:											// �ڴ治��		
		if (poldcnv != NULL)
		{
			m_pPack->cnv_img = poldcnv;
			m_pPack->cnvmark = oldcnvmark;
		}
		m_eStatus = CSISTATUS_IRW;
		return 4;
	case	0:											// �ɹ�
		if (poldcnv != NULL)
		{
			isirw_free_INFOSTR(poldcnv);
		}

		_UpdateDispInfo();
		break;
	default:
		assert(0);										// �������ٳ��ֱ�ķ���ֵ
		break;
	}

	m_bIsChange = true;

	return 0;
}


// ���ָ�������е���ʼ��ַ
char* CSeeImage::IP_GetScanLineAddr(int y)
{
	// ��������Ѿ����������ұ����Ѵ���IPģʽ
	Assert(m_bMark, "IP_GetScanLineAddr()���������滹δ������");

	if (!m_bMark)
	{
		return 0;
	}
	
	Assert(m_eStatus == CSISTATUS_IP, "IP_GetScanLineAddr()�������ಢδ����IPģʽ��");
	assert(m_pPack&&m_pPack->cnv_img&&(m_pPack->cnv_img->bitcount==IP_BITCNT));

	// y������ֵ���ܳ���ͼ��ĸ߶�
	Assert((y>=0)&&(y<(int)m_pPack->cnv_img->height), "IP_GetScanLineAddr()�����������Ŀ�Ȼ�߶�ֵ�Ƿ���");

	return (char*)(m_pPack->cnv_img->pp_line_addr[y]);
}


// ��ȡָ��λ�õ�����ֵ
ulong CSeeImage::IP_GetPixel(int x, int y)
{
	// ��������Ѿ����������ұ����Ѵ���IPģʽ
	Assert(m_bMark, "IP_GetPixel()���������滹δ������");

	if (!m_bMark)
	{
		return 0;
	}
	
	Assert(m_eStatus == CSISTATUS_IP, "IP_GetPixel()�������ಢδ����IPģʽ��");
	assert(m_pPack&&m_pPack->cnv_img&&(m_pPack->cnv_img->bitcount==IP_BITCNT));
	
	// x��y�Ĳ�������Ϸ�
	Assert((y>=0)&&(y<(int)m_pPack->cnv_img->height), "IP_GetPixel()������ָ����yֵ�Ƿ���");
	Assert((x>=0)&&(x<(int)m_pPack->cnv_img->width), "IP_GetPixel()������ָ����xֵ�Ƿ���");

	return ((ulong *)(m_pPack->cnv_img->pp_line_addr[y]))[x];
}


// ����ָ��λ�õ�����ֵ
ulong CSeeImage::IP_SetPixel(int x, int y, ulong newpix)
{
	// ��������Ѿ����������ұ����Ѵ���IPģʽ
	Assert(m_bMark, "IP_SetPixel()���������滹δ������");

	if (!m_bMark)
	{
		return 0;
	}
	
	Assert(m_eStatus == CSISTATUS_IP, "IP_SetPixel()�������ಢδ����IPģʽ��");
	assert(m_pPack&&m_pPack->cnv_img&&(m_pPack->cnv_img->bitcount==IP_BITCNT));
	
	// x��y�Ĳ�������Ϸ�
	Assert((y>=0)&&(y<(int)m_pPack->cnv_img->height), "IP_SetPixel()������ָ����yֵ�Ƿ���");
	Assert((x>=0)&&(x<(int)m_pPack->cnv_img->width), "IP_SetPixel()������ָ����xֵ�Ƿ���");
	
	ulong	*pl = (ulong *)(m_pPack->cnv_img->pp_line_addr[y]);
	ulong	old = pl[x];

	pl[x] = newpix;

	// ���ؾɵ���������
	return old;
}


// �ر�IPģʽ��ת������дģʽ��
int CSeeImage::CloseIPMode()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// ��������Ѿ�����
	Assert(m_bMark, "CloseIPMode()���������滹δ������");
	
	if (!m_bMark)
	{
		return 0;
	}

	// ����Ƿ���IPģʽ
	if (m_eStatus == CSISTATUS_IP)
	{
		assert(m_pPack&&m_pPack->cnv_img&&(m_pPack->cnv_img->bitcount==IP_BITCNT));

		// ֻ�Ǽ򵥵����ñ�ʶΪ��д״̬
		m_eStatus = CSISTATUS_IRW;
		
		return 0;
	}
	
	// �ಢδ����IPģʽ
	return 1;
}


// ISee�ڴ����ϵͳ�ڴ�����ӿ�
void *CSeeImage::ImAlloc(size_t size)
{
	Assert(size > 0, "ImAlloc�������������ڴ�ĳߴ粻��С�ڵ���0��");
	return isirw_malloc(size);
}


// ISee�ڴ����ϵͳ�ڴ��ͷŽӿ�
void *CSeeImage::ImFree(void *p)
{
	if (p == NULL)
	{
		return p;
	}

	isirw_free(p);

	return NULL;
}





////////////////////////////////////////////////////////////////////////////////////
// ���ýӿں���

// �������͵İ�ȫ��⣨ֻ�����ڵ��԰棩
//
// ע����Ȼ�ɱ䳤���������ṩ��ǿ������ȣ���Ҳɥʧ��C++���ԵĲ������ͼ�飬����
//     ����˺��������ֲ��ⷽ��Ĳ��㡣�ú���ֻ�ڵ��԰�״̬����Ч����������յķ���
//     ������κ������ϵ���ʧ��
//
int CSeeImage::_typesafe_check(CSIPARMCHK pct, int type, va_list ap)
{
	va_list			argp = ap;
	int				cnvmark;
	
	assert(argp);

	switch (pct)
	{
	case	CSIPARMCHK_LOAD:							// ������
		switch (type)
		{
		case	CR_FILE:								// �����ļ�����
		case	CR_FILEFAST:							// �����ļ����ͣ����ٰ棩
			{
				char	*fname;
				
				fname   = va_arg(argp, char*);
				cnvmark	= va_arg(argp, int);

				if (fname != NULL)
				{
					Assert(lstrlen((LPCSTR)fname) < MAX_PATH, "Load()������FILE���ͣ���һ��������������Ч���ļ�·����");
					Assert(fname[1] == ':', "Load()������FILE���ͣ���һ�������������ļ�ȫ·����");
				}

				// ����β�����ĺϷ���
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_MEMORY:								// �����ڴ�����
			{
				char			*mname;
				char			*addr;
				ulong	len;
				LPFGUARDSTRUCT	pfdt;
				
				mname   = va_arg(argp, char*);
				addr	= va_arg(argp, char*);
				len		= va_arg(argp, ulong);
				cnvmark	= va_arg(argp, int);

				// �ڴ�ͼ�������һ����Ч������
				Assert((mname != NULL)&&(lstrlen((LPCSTR)mname)<MAX_PATH), "Load()������MEMORY���ͣ���һ����������ָ��һ����Ч���ڴ�ͼ������");
				// ��������ڴ�ͼ����׵�ַ
				Assert(addr != NULL, "Load()������MEMORY���ͣ��ڶ�����������������ͼ�����ݵ��ڴ���׵�ַ��");

				pfdt = (LPFGUARDSTRUCT)(addr-sizeof(FGUARDSTRUCT));
				// �ڴ��������CSeeImage::ImAlloc()���������
				Assert(pfdt->magic == IM_GUARD_MAGIC, "Load()������MEMORY���ͣ��ڶ�������ָ�����ڴ���������CSeeImage::ImAlloc()��������ġ�");

				// ͼ�󳤶Ȳ���Ϊ0
				Assert(len > 0, "Load()������MEMORY���ͣ�������������ֵ����Ϊ0��");
				// ����β�����ĺϷ���
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_SEEIMAGE:							// ��һ��CSeeImage����
			{
				CSeeImage	*pSou;
				
				pSou    = va_arg(argp, CSeeImage*);
				cnvmark	= va_arg(argp, int);

				// �������һ����Ч��ԴCSeeImage����
				Assert((pSou != NULL)&&(pSou->m_uMagic == CSEEIMAGE_MAGIC), "Load()������SEEIMAGE���ͣ���һ����������ָ��һ����Ч��CSeeImage����");
				// Դ�����б������ͼ������
				Assert(pSou->m_eStatus != CSISTATUS_NULL, "Load()������SEEIMAGE���ͣ���һ������ָ���CSeeImage�����б������ͼ�����ݡ�");

				// ����β�����ĺϷ���
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_NIHILITY:							// ���ޣ���������
			{
				int		width;
				int		height;
				int		bitcount;
				
				width   = va_arg(argp, int);
				height	= va_arg(argp, int);
				bitcount= va_arg(argp, int);
				cnvmark	= va_arg(argp, int);

				// ������ͼ��Ŀ�ȡ��߶Ȳ���Ϊ0
				Assert(width&&height, "Load()������NIHILITY���ͣ����ܴ���0��Ȼ�0�߶ȵ�ͼ��");
				
				// ����λ��ȺϷ���
				switch (bitcount)
				{
				case	1:
				case	4:
				case	8:
				case	16:
				case	24:
				case	32:
					break;
				default:
					Assert(0, "Load()������NIHILITY���ͣ�������������λ���ֵ�Ƿ���ֻ����1��4��8��16��24��32����֮һ����");
					break;
				}

				// ����β�����ĺϷ���
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_CLIP:								// ������
			{
				cnvmark	= va_arg(argp, int);
				
				// ����β�����ĺϷ���
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_WINDOW:								// ����
			{
				HWND	hWnd;
				
				hWnd    = va_arg(argp, HWND);
				cnvmark	= va_arg(argp, int);

				// ��������Ч��
				Assert(::IsWindow(hWnd), "Load()������WINDOW���ͣ���һ������������һ����Ч�Ĵ��ھ����");

				// ����β�����ĺϷ���
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_RESOURCE_ID:							// ��Դ����ID��ʽ������
			{
				char	*filename;						// ��Դ�ļ�ȫ·����EXE��DLL��
				int		id;
				
				filename = va_arg(argp, char*);
				id       = va_arg(argp, int);
				cnvmark	 = va_arg(argp, int);

				// ����ļ�·������
				if (filename != NULL)
				{
					Assert((lstrlen((LPCSTR)filename)<MAX_PATH)&&(filename[1]==':'), "Load()������RESOURCE_ID���ͣ���һ��������������Ч����Դ�ļ�ȫ·����");
				}

				// ���id�ĺϷ���
				Assert((id&0x7fffffff)<0xffff, "Load()������RESOURCE_ID���ͣ��ڶ���������������Ч��ID��ö��ֵ��");

				// ����β�����ĺϷ���
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_RESOURCE_NAME:						// ��Դ������Դ���ƴ���ʽ������
			{
				char	*resname;						// ��Դ��
				
				resname  = va_arg(argp, char*);
				cnvmark	 = va_arg(argp, int);

				// ��Դ���Ʋ���Ϊ��
				Assert(resname!=NULL, "Load()������RESOURCE_NAME���ͣ���һ����������ΪNULL��");

				// ����β�����ĺϷ���
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_HBITMAP:								// HBITMAP���
			{
				HBITMAP	hBmp;							// Դλͼ���
				
				hBmp     = va_arg(argp, HBITMAP);
				cnvmark	 = va_arg(argp, int);

				// ����ָ����λͼ���
				Assert(hBmp!=NULL, "Load()������HBITMAP���ͣ���һ��������λͼ�������ΪNULL��");

				// ����β�����ĺϷ���
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_HDIB:								// HDIB���
			{
				HDIB			hDib;
				
				hDib     = va_arg(argp, HDIB);
				cnvmark	 = va_arg(argp, int);
				
				// ����ָ����HDIB���
				Assert(hDib!=NULL, "Load()������HDIB���ͣ���һ��������HDIB�������ΪNULL��");
				
				// ����β�����ĺϷ���
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		default:
			Assert(0, "Load()������ָ���˷Ƿ��Ĳ������ͣ�");
			break;
		}
		break;
	case	CSIPARMCHK_DRAW:							// ���ƺ���
		switch (type)
		{
		case	CD_NON:									// ԭ�ߴ����
			{
				HDC	hdc;
				int	x, y;
				
				hdc = va_arg(argp, HDC);
				x   = va_arg(argp, int);
				y   = va_arg(argp, int);

				// ����ָ����HDC
				Assert(hdc!=NULL, "Draw()������NON���ͣ���һ��������HDC�������ΪNULL��");
			}
			break;
		case	CD_TAN:									// ͸������
			{
			}
			break;
		case	CD_STL:									// ����
			{
				HDC	hdc;
				int	x, y, w, h;
				
				hdc = va_arg(argp, HDC);
				x   = va_arg(argp, int);
				y   = va_arg(argp, int);
				w   = va_arg(argp, int);
				h   = va_arg(argp, int);

				Assert(hdc!=NULL, "Draw()������STL���ͣ���һ��������HDC�������ΪNULL��");
			}
			break;
		case	CD_CEN:									// ����
			{
				HDC	hdc;
				int	x, y, w, h;
				
				hdc = va_arg(argp, HDC);
				x   = va_arg(argp, int);
				y   = va_arg(argp, int);
				w   = va_arg(argp, int);
				h   = va_arg(argp, int);
				
				Assert(hdc!=NULL, "Draw()������CEN���ͣ���һ��������HDC�������ΪNULL��");
			}
			break;
		default:
			Assert(0, "Draw()������ָ���˷Ƿ��Ĳ������ͣ�");
			break;
		}
		break;
	case	CSIPARMCHK_SAVE:							// ���溯��
		switch (type)
		{
		case	CW_FILE:								// �����ļ�����
		case	CW_FILEFAST:							// �����ļ����ͣ����ٰ棩
			{
				char			*fname;
				CSAVETYPE		bitcount;
				int				par1;
				int				par2;
				
				fname    = va_arg(argp, char*);
				bitcount = va_arg(argp, CSAVETYPE);
				par1     = va_arg(argp, int);
				par2     = va_arg(argp, int);

				if (fname != NULL)
				{
					// �����ļ����ĺϷ���
					Assert(lstrlen((LPCSTR)fname)<MAX_PATH, "Save()������FILE���ͣ���һ����������ָ��һ����Ч���ļ�·������");
					Assert(fname[1]==':', "Save()������FILE���ͣ���һ������������һ���ļ���ȫ·������");

					// У��λ��ȱ�־�ĺϷ���
					switch (bitcount)
					{
					case	CSAVEAUTO:
					case	CSAVE1:
					case	CSAVE4:
					case	CSAVE8:
					case	CSAVE16555:
					case	CSAVE16565:
					case	CSAVE24:
					case	CSAVE32:
						break;
					default:
						Assert(0, "Save()������FILE���ͣ��ڶ�������ֵ�Ƿ���");
						break;
					}
				}
			}
			break;
		case	CW_MEMORY:								// �����ڴ�����
			{
				char			*pname;
				char			**ppaddr;
				int				*pimgsize;
				CSAVETYPE		bitcount;
				int				par1;
				int				par2;
				
				pname    = va_arg(argp, char*);
				ppaddr   = va_arg(argp, char**);
				pimgsize = va_arg(argp, int*);
				bitcount = va_arg(argp, CSAVETYPE);
				par1     = va_arg(argp, int);
				par2     = va_arg(argp, int);

				// ���������ڴ�ͼ�������Ƿ����
				Assert(pname!=NULL, "Save()������MEMORY���ͣ���һ����������ָ��һ���ڴ�ͼ�����ơ�");
				Assert(lstrlen((LPCSTR)pname)<MAX_PATH, "Save()������MEMORY���ͣ���һ����������ָ��һ�������桯��ͼ�����ơ�");

				// �����ڴ��ַ�����ȵı�������ΪNULL
				Assert(ppaddr&&pimgsize, "Save()������MEMORY���ͣ��ڶ���������������ΪNULL��");

				// У��λ��ȱ�־�ĺϷ���
				switch (bitcount)
				{
				case	CSAVEAUTO:
				case	CSAVE1:
				case	CSAVE4:
				case	CSAVE8:
				case	CSAVE16555:
				case	CSAVE16565:
				case	CSAVE24:
				case	CSAVE32:
					break;
				default:
					Assert(0, "Save()������MEMORY���ͣ����ĸ�����ֵ�Ƿ���");
					break;
				}
			}
			break;
		case	CW_CLIP:								// ������
			{
				LPRECT	prc;
				
				prc = va_arg(argp, LPRECT);

				// ��
			}
			break;
		case	CW_HBITMAP:								// HBITMAP���
			{
				HBITMAP		*phb;
				
				phb = va_arg(argp, HBITMAP *);

				// ���վ����ָ�벻��ΪNULL
				Assert(phb!=NULL, "Save()������HBITMAP���ͣ���һ��������������Ч��HBITMAPָ�롣");
			}
			break;
		case	CW_HDIB:								// HDIB���
			{
				HDIB	*phdib;
				
				phdib = va_arg(argp, HDIB *);

				// ���վ����ָ�벻��ΪNULL
				Assert(phdib!=NULL, "Save()������HDIB���ͣ���һ��������������Ч��HDIBָ�롣");
			}
			break;
		default:
			Assert(0, "Save()������ָ���˷Ƿ��Ĳ������ͣ�");
			break;
		}
		break;
	default:
		Assert(0, "##���������󣡣���");
	}

	return 0;
}


// ת����־�Ϸ��Լ��
void CSeeImage::_cnvtype_check(int cnk)
{
	switch (cnk)
	{
	case	IRWE_CNVMARK_AUTO:							/* �Զ�תΪ��ƥ��ı�׼��ʽ */
	case	IRWE_CNVMARK_1:								/* ǿ��תΪ 1 λ��ɫ��ͼ */           
	case	IRWE_CNVMARK_4:								/* ǿ��תΪ 4 λ��ɫ��ͼ */           
	case	IRWE_CNVMARK_8:								/* ǿ��תΪ 8 λ��ɫ��ͼ */           
	case	IRWE_CNVMARK_16_555:						/* ǿ��תΪ 16 λ(555)��ʽλͼ */     
	case	IRWE_CNVMARK_16_565:						/* ǿ��תΪ 16 λ(565)��ʽλͼ */     
	case	IRWE_CNVMARK_24:							/* ǿ��תΪ 24 λλͼ */              
	case	IRWE_CNVMARK_32:							/* ǿ��תΪ 32 λλͼ(��Alpha����) */ 
		break;
	default:
		Assert(0, "ת�����Ͳ��Ϸ���");					/* ��ڲ���cnk����һ���Ϸ���ת����־ */
		break;
	}
}


// ��PE��ʽ���ļ��У�EXE��DLL������DIB��Դ��ID��ʽ�ģ�
char* CSeeImage::_LoadDIBFromPE(char *fname, int id)
{
	CFile	ff;
	int		i;
	DWORD	subRva;										// RAV�������ַ��ƫ��
	short	nSection   = 0;								// ����
	DWORD	ptResource = 0;								// ��Դ�ڵ�ַ
	WORD	nResource  = 0;								// ��Դ�����
	char	*buff      = NULL;							// �����ص�DIB������
	bool	enumid     = (id<0);						// ��ʶ����

	IMAGE_DOS_HEADER					stDOSHeader;	// DOS EXE�ļ�ͷ
	IMAGE_SECTION_HEADER			stSectionHeader;	// �ڱ�
	IMAGE_NT_HEADERS32					 stPEHeader;	// NT�ļ�ͷ
	IMAGE_RESOURCE_DIRECTORY		  stResourceDir;	// ��ԴĿ¼
	IMAGE_RESOURCE_DIRECTORY_ENTRY	stResourceEntry;	// ��ԴĿ¼��
	PIMAGE_RESOURCE_DIRECTORY_ENTRY	 stResDirEntry1;	// ��һ��Ŀ¼��ڣ����飩
	IMAGE_RESOURCE_DATA_ENTRY        stResDataEntry;	// ��Դ���


	// �򿪵�����ָ����PE�ļ�
	if(!ff.Open(fname, CFile::typeBinary|CFile::shareDenyNone))
	{
		return NULL;									// �ļ�������
	}

	try
	{
		// ȡDOS�ļ�ͷ
		ff.Read(&stDOSHeader, sizeof(IMAGE_DOS_HEADER));

		// �ж��ļ��Ƿ���΢���ʽ
		if (stDOSHeader.e_magic != IMAGE_DOS_SIGNATURE)
		{
			throw 0;
		}

		// �����Ŷ�λ��PE�ļ�ͷ
		ff.Seek(stDOSHeader.e_lfanew, CFile::begin);

		// ��PE�ļ�ͷ
		ff.Read(&stPEHeader, sizeof(IMAGE_NT_HEADERS32));

		// �ж��ļ��Ƿ���PE��ʽ
		if(stPEHeader.Signature != IMAGE_NT_SIGNATURE)
		{
			throw 0;
		}
	}
	catch (...)
	{
		ff.Close();
		return NULL;									// ��PE��ʽ
	}

	stResDirEntry1 = NULL;

	try
	{
		// ȡ�ܽ���
		nSection = stPEHeader.FileHeader.NumberOfSections;

		// ��ȡ�ڱ�
		for (i=0; i<nSection; i++)
		{
			ff.Read(&stSectionHeader, sizeof(IMAGE_SECTION_HEADER));

			// �ж��Ƿ�����ԴĿ¼���Խ������жϣ�ע���˷�����δ��΢���Ͽɣ�
			if (!strcmp((LPCSTR)stSectionHeader.Name, ".rsrc\0\0"))
			{
				ptResource = stSectionHeader.PointerToRawData;
				subRva     = stSectionHeader.VirtualAddress - stSectionHeader.PointerToRawData;
				break;
			}
		}

		//�������Դ�����ȡ��
		if (ptResource != 0)
		{
			// ����Դ��Ŀ¼
			ff.Seek(ptResource, CFile::begin);
			ff.Read(&stResourceDir, sizeof(stResourceDir));

			// ID���͵ĸ���
			nResource = stResourceDir.NumberOfIdEntries;

			// ��������Ŀ¼�ֻʶ��ID��ʽ����Դ���ͣ�
			ff.Seek(sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY)*stResourceDir.NumberOfNamedEntries, CFile::current);

			// ����BMP��Դ����Դ���ͣ�
			for(i=0; i<nResource; i++)
			{
				ff.Read((void*)&stResourceEntry, sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY));

				if ((stResourceEntry.DataIsDirectory)&&(stResourceEntry.Id == 2))
				{
					break;
				}
			}

			// ���û���ҵ�BMP��Դ����ֱ�ӷ���
			if (i >= nResource)
			{
				throw 0;
			}

			ff.Seek(stResourceEntry.OffsetToDirectory+ptResource,CFile::begin);
			ff.Read((void*)&stResourceDir, sizeof(IMAGE_RESOURCE_DIRECTORY));

			nResource = (enumid) ? stResourceDir.NumberOfIdEntries+stResourceDir.NumberOfNamedEntries : stResourceDir.NumberOfIdEntries;

			// ���û��BMPͼ���򷵻�
			if (nResource == 0)
			{
				throw 0;
			}

			// �������ö����ʽ������������ֻʶ��ID��ʽ��
			if (!enumid)
			{
				ff.Seek(sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY)*stResourceDir.NumberOfNamedEntries, CFile::current);
			}
			
			stResDirEntry1 = new IMAGE_RESOURCE_DIRECTORY_ENTRY[nResource];
			
			// �����е�BMP��ԴĿ¼���Դ���ƣ�
			for (i=0; i<nResource; i++)
			{
				ff.Read(&stResDirEntry1[i], sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY));
			}

			if (!enumid)								// ����ID
			{
				for (i=0; i<nResource; i++)
				{
					if (stResDirEntry1[i].Id == (ushort)id)
					{
						break;
					}
				}
			}
			else										// ö��ID
			{
				id &= 0xffff;
				i = id;
			}

			// ��DIB
			if ((ushort)i < nResource)
			{
				ff.Seek(stResDirEntry1[i].OffsetToDirectory+ptResource, CFile::begin);
				ff.Read(&stResourceDir, sizeof(IMAGE_RESOURCE_DIRECTORY));
				
				// ��Դ���ԣ���BMP��Դû�����Բ������ֻ��һ��Ŀ¼�
				ff.Read(&stResourceEntry, sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY));
				
				// ������ƫ�Ƽ��ߴ�
				ff.Seek(stResourceEntry.OffsetToDirectory+ptResource, CFile::begin);
				ff.Read(&stResDataEntry, sizeof(IMAGE_RESOURCE_DATA_ENTRY));
				
				// ����DIB������
				buff = new char[stResDataEntry.Size];
				
				// ��DIB����
				ff.Seek(stResDataEntry.OffsetToData-subRva, CFile::begin);
				ff.Read(buff, stResDataEntry.Size);
			}

			delete stResDirEntry1;
			stResDirEntry1 = NULL;
		}

		ff.Close(); 
	}
	catch (...)
	{
		if (stResDirEntry1 != NULL)
		{
			delete []stResDirEntry1;
		}

		if (buff != NULL)
		{
			delete []buff;
		}

		ff.Close();

		return NULL;
	}

	return buff;
}


// �ӵ�ǰ�����ж���DIB��Դ
char* CSeeImage::_LoadDIBFromResource(char *lpResourceName)
{
	HMODULE		hInst = ::GetModuleHandle(NULL); 
	char		*pbuf = NULL;
	
	ASSERT(hInst);
	
	// ��ָ����BMP��Դ
	HRSRC hSrc = ::FindResource(hInst, (LPCSTR)lpResourceName, RT_BITMAP);

	if (hSrc)
	{
		HGLOBAL hResData = ::LoadResource(hInst, hSrc);
	
		if (hResData)
		{
			LPVOID lpResData = ::LockResource(hResData);

			if (lpResData)
			{
				// �������Ч��DIB��Դ������Դ����λӦ����BITMAPINFOHEADER�ṹ
				if (*((DWORD*)lpResData) == sizeof(wBITMAPINFOHEADER))
				{
					pbuf = (char*)lpResData;
				}
			}
		}
		// ��LoadResource()�����������ԴĿ�겻��Ҫ��ʾ��ɾ������Windowsϵͳ���Զ�ɾ����
		// LockResource()��������ԴĿ��Ҳ����Ҫ��ʾ�Ľ���
	}
	
	return pbuf;
}


// �����µ�רҵ���ݰ�����д��ʼ������
//
// ��ڲ�����
//	ppPack	- ָ�����רҵ����ַ��ָ�루ָ���ָ�룩
//	pbmi	- ָ��BITMAPINFO�ṹ��ָ�룬�ýṹ��Ӧ����Դͼ�����Ϣ��������ɫ�����ݣ�
//	cnvmark	- ת����־
//	sn		- �����ߵı�ʶ
//	pbits	- ָ��BITMAPINFOHEADER�ṹ�����ָ�룬����ָ���������ݵ�ָ��
//	bitmark	- 0 ��ʾpbitsָ��BITMAPINFOHEADER�ṹ���桢1 ��ʾpbitsָ����������
//	
// ����ֵ���μ�iseeirw.h�ļ��е�enum EXERESULTֵ
//	
int CSeeImage::_AllocSpecPack(LPSPEC_DATAPACK *ppPack, wBITMAPINFO* pbmi, int cnvmark, int sn, char *pbits, int bitmark)
{
	assert((ppPack != NULL)&&(pbmi != NULL));

	int				i;
	int				width    = pbmi->bmiHeader.biWidth;
	int				height   = abs(pbmi->bmiHeader.biHeight);
	int				bitcount = pbmi->bmiHeader.biBitCount;
	int				result   = ER_SUCCESS;
	LPINFOSTR		pinfo    = NULL;
	ulong	linesize = DIBSCANLINE_DWORDALIGN(width*bitcount);
	LPSPEC_DATAPACK	pPack    = NULL;
	ulong	palcnt;

	assert(cnvmark != IRWE_CNVMARK_NOT);

	// ����ͼ����Ϣ�Ƿ�Ϸ�
	if ((width == 0)||(height == 0)||(bitcount == 0))
	{
		return ER_BADIMAGE;
	}

	// ����רҵ��
	if ((pPack=isirw_alloc_SPEC_DATAPACK(ISEEIO_CONFER_LOC_FILE, CSINIHFILENAME, 0, 0, (IRWE_CNVMARK)cnvmark)) != NULL)
	{
		assert(pPack->org_img != NULL);

		pinfo = pPack->org_img;

		// �������׵�ַ����
		if ((pinfo->pp_line_addr=(void**)isirw_malloc(sizeof(char*)*height)) == NULL)
		{
			isirw_free_SPEC_DATAPACK(pPack);
			return ER_MEMORYERR;
		}
		
		// �������ػ�����
		if ((pinfo->p_bit_data=(uchar *)isirw_malloc((int)linesize*height)) == NULL)
		{
			isirw_free_SPEC_DATAPACK(pPack);
			return ER_MEMORYERR;
		}

		switch (bitcount)
		{
		case	1:
			SETMASK_8(pinfo);
			palcnt = 2;
			if (bitmark == 0)
			{
				pbits += palcnt*sizeof(wRGBQUAD);
			}
			for (i=0; i<(int)palcnt; i++)
			{
				pinfo->palette[i] = *(ulong *)&(pbmi->bmiColors[i]);
			}
			break;
		case	4:
			SETMASK_8(pinfo);
			palcnt = 16;
			if (bitmark == 0)
			{
				pbits += palcnt*sizeof(wRGBQUAD);
			}
			for (i=0; i<(int)palcnt; i++)
			{
				pinfo->palette[i] = *(ulong *)&(pbmi->bmiColors[i]);
			}
			break;
		case	8:
			SETMASK_8(pinfo);
			palcnt = 256;
			if (bitmark == 0)
			{
				pbits += palcnt*sizeof(wRGBQUAD);
			}
			for (i=0; i<(int)palcnt; i++)
			{
				pinfo->palette[i] = *(ulong *)&(pbmi->bmiColors[i]);
			}
			break;
		case	16:
			if (pbmi->bmiHeader.biCompression == BI_RGB)
			{
				pinfo->r_mask = 0x7c00;
				pinfo->g_mask = 0x3e0;
				pinfo->b_mask = 0x1f;
				pinfo->a_mask = 0x0;
			}
			else
			{
				pinfo->r_mask = 0xf800;
				pinfo->g_mask = 0x7e0;
				pinfo->b_mask = 0x1f;
				pinfo->a_mask = 0x0;
				if (bitmark == 0)
				{
					pbits += 3*sizeof(wRGBQUAD);
				}
			}
			palcnt = 0;
			break;
		case	24:
			SETMASK_24(pinfo);
			palcnt = 0;
			break;
		case	32:
			SETMASK_32(pinfo);
			palcnt = 0;
			if (pbmi->bmiHeader.biCompression == BI_BITFIELDS)
			{
				if (bitmark == 0)
				{
					pbits += 3*sizeof(wRGBQUAD);
				}
			}
			break;
		default:
			assert(0);
			isirw_free_SPEC_DATAPACK(pPack);
			return ER_NOTSUPPORT;
		}
		
		// ������������
		if (pbits != NULL)
		{
			memcpy((void*)pinfo->p_bit_data, (const void *)pbits, linesize*height);
		}
		else
		{
			memset((void*)pinfo->p_bit_data, 0xff, linesize*height);
		}

		pinfo->imgtype     = IMT_RESSTATIC;		/* ͼ���ļ����� */
		pinfo->imgformat   = IMF_BMP;			/* ͼ���ļ���ʽ�������ʶ�� */
		pinfo->compression = ICS_RGB;			/* ͼ���ļ���ѹ����ʽ */

		pinfo->width    = (ulong)width;
		pinfo->height   = (ulong)height;
		pinfo->bitcount = (ulong)bitcount;
		pinfo->order    = (pbmi->bmiHeader.biHeight > 0) ? 1 : 0;
		
		// ��д���׵�ַ����
		if (pinfo->order == 0)					/* ���� */
		{
			for (i=0;i<(int)(pinfo->height);i++)
			{
				pinfo->pp_line_addr[i] = (void *)(pinfo->p_bit_data+(i*linesize));
			}
		}
		else									/* ���� */
		{
			for (i=0;i<(int)(pinfo->height);i++)
			{
				pinfo->pp_line_addr[i] = (void *)(pinfo->p_bit_data+((pinfo->height-i-1)*linesize));
			}
		}
		
		pinfo->pal_count = palcnt;				/* ��ɫ����Ч��ĸ��� */
		
		pinfo->play_number = 0;					/* ����˳���ߴ磨��Ԫ��Ϊ��λ�� */
		pinfo->play_order  = NULL;				/* ����˳��� */
		pinfo->time        = 0;					/* ��֡ͣ��ʱ�� */
		pinfo->colorkey    = -1;				/* ��ͼ��͸��ɫ��-1��ʾ��͸��ɫ�� */
		
		pinfo->imgnumbers  = 1;					/* ���ļ���ͼ��ĸ��� */
		pinfo->psubimg     = NULL;				/* ��ͼ����������ַ */
		pinfo->data_state  = 2;					/* ���ݰ���ǰ������״̬ */
		
		pPack->command     = IRWE_SDPCOMM_LOAD;	/* ģ������� */
		pPack->data_state  = SDP_DATA_STATE_ORG_PIX|SDP_DATA_STATE_ORG_INFO|SDP_DATA_STATE_PATH;
		pPack->exec_state  = IRWE_SDPSTATE_IDLE;
		pPack->sn          = sn;
		
		if (cnvmark != IRWE_CNVMARK_NOT)
		{
			if (isirw_cnv(pPack, (IRWE_CNVMARK)cnvmark, 0) < 0)
			{
				isirw_free_SPEC_DATAPACK(pPack);
				return ER_MEMORYERR;
			}
		}

		*ppPack = pPack;
		
		result = ER_SUCCESS;
	}
	else
	{
		*ppPack = NULL;

		result = ER_MEMORYERR;
	}

	return result;
}


// ������ͼ�󱣴���ָ��Ŀ�������
//
// ��ڲ�����
//	type	- Ŀ���������
//	name	- Ŀ������
//	par1/2	- ISeeIO������������Ӳ���
//	cnvmark	- ����λ��ID
//	par		- ��������ṹ
//
// ���ڲ�����
//	ER_SUCCESS	- �ɹ�
//	����ֵʧ�ܣ�����ԭ��μ���EXERESULTö�ٱ����Ķ���
//
int CSeeImage::_SaveSpecPack(ISEEIO_CONFER_TYPE type, char *name, ulong par1, ulong par2, CSAVETYPE cnvmark, SAVESTR* par)
{
	assert(name&&par);

	// ����ԭת��������
	IRWE_CNVMARK	cnvmode    = m_pPack->cnvmark;
	LPINFOSTR		oldcnvpack = m_pPack->cnv_img;
	EXERESULT		result     = ER_SUCCESS;

	m_pPack->cnv_img = 0;
	m_pPack->cnvmark = IRWE_CNVMARK_NOT;

	if (oldcnvpack)
	{
		m_pPack->data_state &= ~SDP_DATA_STATE_CNV;
	}

	IRWE_CNVMARK	cnvid = (IRWE_CNVMARK)cnvmark;

	assert((cnvid>=CNVAUTO)&&(cnvid<=CNV32));

	// ��ͼ��ת��Ϊ�û�ָ����λ��
	if (isirw_cnv(m_pPack, cnvid, 0) >= 0)
	{
		// ����ͼ��
		if (isirw_s_write_img((ISEEIO_CONFER_TYPE)type, (const char *)name, par1, par2, m_pPack, par, 0) == 0)
		{
			// �ȴ��������
			AfxGetApp()->DoWaitCursor(1);
			result = isirw_s_wait(m_pPack, 0);
			AfxGetApp()->DoWaitCursor(0);
			
			switch ((int)result)
			{
			case	ER_SUCCESS:
				break;
			case	ER_USERBREAK:
				CFileDialogEx::DispLastErrMess(ISIRW_LASTERR_USER);
				break;
			case	ER_MEMORYERR:
				CFileDialogEx::DispLastErrMess(ISIRW_LASTERR_MEM);
				break;
			case	ER_FILERWERR:
				CFileDialogEx::DispLastErrMess(ISIRW_LASTERR_RW);
				break;
			case	ER_SYSERR:
				CFileDialogEx::DispLastErrMess(ISIRW_LASTERR_OSERR);
				break;
			case	ER_NONIMAGE:						// ָ�����ļ������Ϸ�
				break;
			case	ER_NSIMGFOR:
				ASSERT(0);								// ����������
				break;
			default:
				ASSERT(0);								// ������������ֵ
				break;
			}
		}
		else
		{
			result = CFileDialogEx::DispLastErrMess(isirw_s_get_last_err());
		}
	}
	else
	{
		result = CFileDialogEx::DispLastErrMess(ISIRW_LASTERR_MEM);
	}

	// �ͷ�ת����
	if (m_pPack->cnv_img)
	{
		isirw_free_INFOSTR(m_pPack->cnv_img);
	}

	if (oldcnvpack)
	{
		m_pPack->data_state |= SDP_DATA_STATE_CNV;
	}

	// �ָ�ԭ״̬
	m_pPack->cnv_img = oldcnvpack;
	m_pPack->cnvmark = cnvmode;

	return (int)result;
}


// ���滯�����߷�����·����
//
// ��ڲ�����
//	path	- �����߷�����·����
//	buff	- �ɵ������ṩ�ġ����ڴ�����滯��·�����Ļ��������ߴ�������MAX_PATH���ֽڣ�
//
// ����ֵ��
//	ͬ��ڲ���buff
//
// ע�ͣ�
//	1�����pathΪNULL���������ص����߳��������ľ���·��
//	2�����pathΪһ���·�����������ݵ����߳�����ת��Ϊ����·��
//	3�����������صĴ�β��Ϊ '\' �ַ�
//
char * CSeeImage::_NormalizePath(char *path, char *buff)
{
	assert(buff != NULL);

	int		iLen;
	
	// ���������û��ָ��·�������ȡ���ó�������·��
	if (path == NULL)
	{
		// ȡ��Ӧ�ó����ȫ·��
		::GetModuleFileName(NULL, (LPTSTR)buff, MAX_PATH);
		
		iLen = lstrlen((char*)buff);
		
		// ȥ��β����Ӧ�ó�����
		while ((buff[--iLen] != '\\')&&(iLen > 0))
		{
			buff[iLen] = '\0';
		}
	}
	else
	{
		// �жϵ�����ָ����·�������·�����Ǿ���·��
		if ((lstrlen(path) == 1)||(path[1] != ':'))
		{
			// ȡ��Ӧ�ó����ȫ·��
			::GetModuleFileName(NULL, (LPTSTR)buff, MAX_PATH);
			
			iLen = lstrlen((char*)buff);
			
			// ȥ��β����Ӧ�ó�����
			while ((buff[--iLen] != '\\')&&(iLen > 0))
			{
				buff[iLen] = '\0';
			}
			
			// �ϳɾ���·��
			lstrcat((LPSTR)buff, (LPCSTR)path);
			
			// ��֤��β��Ϊ '\' �ַ�
			if (buff[lstrlen((LPCSTR)buff)-1] != '\\')
			{
				lstrcat((LPSTR)buff, (LPCSTR)"\\");
			}
		}
		else
		{
			assert(path[1] == ':');
			// ���ƾ���·��
			lstrcpy((LPSTR)buff, (LPCSTR)path);

			if (buff[lstrlen((LPCSTR)buff)-1] != '\\')
			{
				lstrcat((LPSTR)buff, (LPCSTR)"\\");
			}
		}
	}

	return buff;
}


// ����ָ����ŵ���ͼ��ṹ��ַ
//
// ��ڲ�����
//	pinfo	- ��ͼ��ṹ��ַ[in]
//	num		- ����ȡ��ַ����ͼ�����[in]
//
// ����ֵ��
//	��ͼ��ṹ��ַ��ʧ�ܷ���NULL
//
SUBIMGBLOCK* CSeeImage::_CalSubImageAddr(LPINFOSTR pinfo, int num)
{
	// ͼ����Ų���Ϊ0��0����ͼ��
	assert(pinfo&&num);

	SUBIMGBLOCK*	psub = pinfo->psubimg;

	while (psub != NULL)
	{
		if (psub->number == num)
		{
			break;
		}
		else
		{
			psub = psub->next;
		}
	}

	return psub;
}


// ˢ����ʾ��Ϣ
void CSeeImage::_UpdateDispInfo()
{
	// ��������Ѿ����������ұ��뺬��ͼ��
	assert(m_pPack&&m_pPack->org_img);

	LPINFOSTR	pinfo = (m_pPack->cnv_img != NULL) ? m_pPack->cnv_img : m_pPack->org_img;

	// ��д��ͼ��ĳߴ硢��ʾ����Ϣ
	m_cDispInfo.bmi.biSize          = sizeof(wBITMAPINFOHEADER);
	m_cDispInfo.bmi.biWidth         = pinfo->width;
	m_cDispInfo.bmi.biHeight        = (pinfo->order == 0) ? -(long)pinfo->height : pinfo->height;
	m_cDispInfo.bmi.biPlanes        = 1;
	m_cDispInfo.bmi.biBitCount      = (WORD)pinfo->bitcount;
	m_cDispInfo.bmi.biCompression   = BI_RGB;
	m_cDispInfo.bmi.biSizeImage     = 0;
	m_cDispInfo.bmi.biXPelsPerMeter = 0;
	m_cDispInfo.bmi.biYPelsPerMeter = 0;
	m_cDispInfo.bmi.biClrUsed       = 0;
	m_cDispInfo.bmi.biClrImportant  = 0;
	
	memcpy((void*)m_cDispInfo.pal, (const void *)pinfo->palette, sizeof(wRGBQUAD)*pinfo->pal_count);
}


/////////////////////////////////////////////////////////////////////////////
// CFileDialogEx

IMPLEMENT_DYNAMIC(CFileDialogEx, CFileDialog)

CFileDialogEx::CFileDialogEx(LPSPEC_DATAPACK pack) : CFileDialog(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, ::AfxGetMainWnd())
{
	LPIRWMF_HEADER		pib;
	LPIRWMF_INFOBLOCK	pinfo;
	int					i;

	ASSERT(pack);

	// �ȴ����������
	isirw_s_wait(pack, 0);
	
	m_pPack       = pack;
	m_saveinfocnt = 0;
	m_ctrlcnt     = 0;

	m_ext.Empty();

	memset((void*)m_saveinfo, 0, sizeof(SAVEPLUGININFO)*ISEEIRW_MAX_PLUGIN_CNT);
	memset((void*)m_pStatic, 0, sizeof(CStatic*)*ISD_CTLCNT);
	memset((void*)m_pComboBox, 0, sizeof(CComboBox*)*ISD_CTLCNT);
	memset((void*)m_save.para_value, 0, sizeof(SAVESTR));
	
	// ���治�ܴ�������״̬
	ASSERT(isirw_is_busy() != 3);
	
	// �ȴ������������
	while (isirw_is_busy())
	{
		::Sleep(0);
	}
	
	// ����ͼ���д����
	pib = isirw_lock();
	
	pinfo = pib->puh;
	
	// �����б��湦�ܵĲ����Ϣ��ȡ�������������Ϣ������
	while (pinfo)
	{
		if (pinfo->ifo.irwp_function & IRWP_WRITE_SUPP)
		{
			m_saveinfo[m_saveinfocnt].index = m_saveinfocnt;
			m_saveinfo[m_saveinfocnt].piid  = pinfo->ifo.plug_id;
			strcpy((char*)m_saveinfo[m_saveinfocnt].str, (const char *)_strupr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)" ��ʽ (*.");
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)_strlwr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)")|");
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)"*.");
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)_strlwr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)"|");
			memcpy((void*)&(m_saveinfo[m_saveinfocnt].cont), (const void *)&(pinfo->ifo.irwp_save), sizeof(IRW_SAVE_DESC));
			strcpy((char*)m_saveinfo[m_saveinfocnt].ext, (const char *)_strlwr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			m_saveinfocnt++;
		}
		pinfo = pinfo->next;
	}
	
	isirw_unlock(pib);
	
	m_ofn.lpstrDefExt    = "";
	m_ofn.Flags			&= ~OFN_ENABLESIZING;
	m_ofn.Flags			&= ~OFN_OVERWRITEPROMPT;
	
	strcpy((char*)m_szFileName, "δ����");
	
	// ��MFC���Ĺ��˴�ת��Ϊϵͳ���
	if (m_saveinfocnt)
	{
		for (i=0, m_strFilter.Empty(); i<m_saveinfocnt; i++)
		{
			m_strFilter += (LPCTSTR)m_saveinfo[i].str;
		}

		LPTSTR pch = m_strFilter.GetBuffer(0);

		// �� '\0' �滻'|'����
		while ((pch = _tcschr(pch, '|')) != NULL)
		{
			*pch++ = '\0';
		}

		m_ofn.lpstrFilter = m_strFilter;
	}

	m_iRunMark = 0;
}


BEGIN_MESSAGE_MAP(CFileDialogEx, CFileDialog)
	//{{AFX_MSG_MAP(CFileDialogEx)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CFileDialogEx::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
	CWnd		*wndDlg = GetParent();
	CRect		Rect;
	int			i;

	// ��ʱ���б��뺬��ͼ�����������
	if (!(m_pPack->data_state&SDP_DATA_STATE_ORG_PIX))
	{
		m_iRunMark++;
	}

	for (i=0, Rect.SetRectEmpty(); i<ISD_CTLCNT; i++)
	{
		m_pStatic[i]   = new CStatic;
		m_pComboBox[i] = new CComboBox;
		
		// ���������ؼ����ڲ���������
		if (m_pStatic[i])
		{
			m_pStatic[i]->Create((LPCTSTR)"", WS_CHILD|WS_TABSTOP|SS_LEFT|SS_LEFTNOWORDWRAP, Rect, wndDlg, IDC_SAVEASCNT_STC+i);
			m_pStatic[i]->SetFont(wndDlg->GetFont());
		}
		else
		{
			m_iRunMark++;
		}

		if (m_pComboBox[i])
		{
			m_pComboBox[i]->Create(WS_CHILD|CBS_DROPDOWN|WS_VSCROLL|WS_TABSTOP|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|CBS_HASSTRINGS, Rect, wndDlg, IDC_SAVEASCNT_CON+i);
			m_pComboBox[i]->SetFont(wndDlg->GetFont());
		}
		else
		{
			m_iRunMark++;
		}
	}

	_SetFileType(0);
		
	return TRUE;
}

void CFileDialogEx::OnDestroy() 
{
	CFileDialog::OnDestroy();
	
	for (int i=0; i<ISD_CTLCNT; i++)
	{
		if (m_pStatic[i])
		{
			delete m_pStatic[i];
		}
		if (m_pComboBox[i])
		{
			delete m_pComboBox[i];
		}
	}
}


// ���ݸ������ַ������Ҳ������
int CFileDialogEx::_GetPluginIndex(LPCTSTR sou)
{
	for (int i=0; i<m_saveinfocnt; i++)
	{
		if (!memcmp((const void *)sou, (const void *)m_saveinfo[i].str, 12))
		{
			return i;
		}
	}
	return -1;
}


// �����˴�����ת��Ϊ�����Ϣ�����Ӧ������
int CFileDialogEx::_GetPluginIndexFromFilterIndex(int idx)
{
	char	*p = (char*)m_ofn.lpstrFilter;
	int		i;

	ASSERT(p);

	for (i=1; i<idx; i++)
	{
		p += strlen((const char *)p)+1;					// �ƹ����˴��ԣ�2�Σ�
		p += strlen((const char *)p)+1;
	}

	i = _GetPluginIndex((LPCTSTR)p);

	return i;
}


void CFileDialogEx::OnSize(UINT nType, int cx, int cy) 
{
	CFileDialog::OnSize(nType, cx, cy);
}


BOOL CFileDialogEx::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	OFNOTIFY			*pNotify = (OFNOTIFY*)lParam;
	
	switch(pNotify->hdr.code)
	{
	case CDN_TYPECHANGE:
		_SetFileType(pNotify->lpOFN->nFilterIndex);
		return TRUE;
	case CDN_FILEOK:
		*pResult = _OK();
		return TRUE;
	}

	return CFileDialog::OnNotify(wParam, lParam, pResult);
}


int CFileDialogEx::_SetFileType(int index)
{
	CWnd		*wndDlg = GetParent();
	int			i, j, k;
	DWORD		savebits;
	CString		stmp;

	// �����ʼ��δ�ɹ�����ִ������Ĳ���
	if (m_iRunMark)
	{
		return -1;
	}

	UpdateData(TRUE);

	static CRect	rcwnd(0,0,0,0);
	
	// �����Ի���ߴ�
	if (rcwnd.IsRectEmpty())
	{
		wndDlg->GetWindowRect(rcwnd);
	}
	
	// ȡ�õ�ǰ�������
	i = _GetPluginIndexFromFilterIndex(index);

	ASSERT(i >= 0);

	const UINT nControls = 5;	
	
	// ��ȡԭʼ�Ի�����ؿؼ����ڵĳߴ�����
	UINT  Controls[nControls] = {stc3, stc2, edt1, cmb1, lst1};
	CRect rc[nControls];							
	
	// ����ԭʼ���ڼ��ؼ����ε�����
	for (j=0; j<nControls; j++)
	{
		CWnd *wndCtrl = wndDlg->GetDlgItem(Controls[j]);
		wndCtrl->GetWindowRect(&rc[j]);
		wndDlg->ScreenToClient(&rc[j]);					// ת��Ϊ�ͻ�������
	}

	// ����һ���ؼ���������ĸ߶�
	int iExtraSize = rc[1].Height()+(rc[1].top-rc[0].bottom);
	
	// ����λ���ѡ����ƴ��ڱ���
	m_pStatic[0]->SetWindowText("λ���ѡ��:");
	// �����ǰ��λ��ѡ��
	m_pComboBox[0]->ResetContent();

	// ��ǰ���֧�ֵı����������
	savebits = m_saveinfo[i].cont.bitcount;
	
	// ���λ�������Ͽ�
	for (j=0; j<ISEEIRW_MAX_BITCOUNT; j++)
	{
		if ((savebits>>j)&1)
		{
			stmp.Format("%d λ", j+1);
			k = m_pComboBox[0]->AddString((LPCTSTR)stmp);
			m_pComboBox[0]->SetItemData(k, j+1);
		}
	}

	// ȱʡѡ��Ϊ��һ��
	m_pComboBox[0]->SetCurSel(0);

	// λ��ȿؼ��������ǿɼ���
	m_pStatic[0]->SetWindowPos(NULL, rc[1].left, rc[1].top+iExtraSize, rc[1].Width(), rc[1].Height(), 0);
	m_pStatic[0]->ShowWindow(SW_SHOW);
	m_pComboBox[0]->SetWindowPos(NULL, rc[3].left, rc[3].top+iExtraSize, rc[3].Width(), rc[3].Height()+ISD_COMBOBOXLIST_HEIGHT, 0);
	m_pComboBox[0]->ShowWindow(SW_SHOW);

	// ������ǰ�εĿؼ�����
	for (j=1; j<m_ctrlcnt+1; j++)
	{
		m_pStatic[j]->ShowWindow(SW_HIDE);
		m_pComboBox[j]->ShowWindow(SW_HIDE);
	}

	// ���������б����
	m_ctrlcnt = m_saveinfo[i].cont.count;

	// �����µĿؼ�����
	for(j=1; j<(m_ctrlcnt+1); j++)
	{
		CString strTxt;
		CRect   cr = rc[1];

		cr.OffsetRect(0, iExtraSize*(j+1));

		if(m_pStatic[j]->GetSafeHwnd())
		{
			m_pStatic[j]->SetWindowPos(NULL, cr.left, cr.top, cr.Width(), cr.Height(), 0);
			m_pStatic[j]->ShowWindow(SW_SHOW);
			strTxt = m_saveinfo[i].cont.para[j-1].desc;
			m_pStatic[j]->SetWindowText(strTxt+":");
		}
		else
		{
			strTxt = m_saveinfo[i].cont.para[j-1].desc;
			m_pStatic[j]->Create(strTxt+":", WS_CHILD|WS_VISIBLE, cr, wndDlg, IDC_SAVEASCNT_STC+j);
			m_pStatic[j]->SetFont(wndDlg->GetFont());
		}

		// ���ɻ��߸��������ؼ�
		cr = rc[3];

		cr.OffsetRect(0, iExtraSize*(j+1));
		cr.bottom += ISD_COMBOBOXLIST_HEIGHT;

		if(m_pComboBox[j]->GetSafeHwnd())
		{
			m_pComboBox[j]->SetWindowPos(NULL, cr.left, cr.top, cr.Width(), cr.Height(), 0);
			m_pComboBox[j]->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pComboBox[j]->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_TABSTOP|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|CBS_HASSTRINGS, cr, wndDlg, IDC_SAVEASCNT_CON+j);
			m_pComboBox[j]->SetFont(wndDlg->GetFont());
		}

		//��������ؼ�������
		m_pComboBox[j]->ResetContent();

		// ����ѡ�����ִ�
		for (k=0; k<m_saveinfo[i].cont.para[j-1].count; k++)
		{
			stmp.Format("%s", (LPCTSTR)m_saveinfo[i].cont.para[j-1].value_desc[k]);
			m_pComboBox[j]->AddString((LPCTSTR)stmp);
		}

		m_pComboBox[j]->SetCurSel(0);
	}

	// ��������ؼ�
	for(k=m_ctrlcnt+1; k<ISD_CTLCNT; k++)
	{
		if(m_pStatic[k]->GetSafeHwnd())
		{
			m_pStatic[k]->ShowWindow(SW_HIDE);
		}

		if(m_pComboBox[k]->GetSafeHwnd())
		{
			m_pComboBox[k]->ShowWindow(SW_HIDE);
		}
	}

	// ����ȱʡ��չ��
	m_ext = (LPCSTR)m_saveinfo[i].ext;

	// �����Ի���ߴ�
	wndDlg->SetWindowPos(NULL, 0, 0, rcwnd.Width(), rcwnd.Height()+iExtraSize*(m_ctrlcnt), SWP_NOMOVE);
	
	UpdateData(FALSE);

	return 0;
}


// �����ļ�
BOOL CFileDialogEx::_OK()
{
	// ��ʱ���б��뺬��ͼ�����������
	if (!(m_pPack->data_state&SDP_DATA_STATE_ORG_PIX))
	{
		m_iRunMark++;
	}
	
	UpdateData(TRUE);

	// ��ȡĿ���ļ�·��
	CString		strExt      = GetFileExt();
	CString		strFileName = GetFileName();
	CString		strPathName = GetPathName();
	CFileFind	fileFind;
	CString		strMsg;

	// �����ļ���չ��
	if (strExt.IsEmpty())
	{
		strPathName += ".";
		strPathName += m_ext;
		strFileName += ".";
		strFileName += m_ext;
	}
	else
	{
		if (_stricmp((const char *)strExt, (const char *)m_ext))
		{
			strPathName += ".";
			strPathName += m_ext;
			strFileName += ".";
			strFileName += m_ext;
		}
	}

	// �ж��ļ��Ƿ����
	if(fileFind.FindFile(strPathName))
	{
		strMsg.Format("�ļ� %s �Ѿ����ڣ�Ҫ�滻���ļ���", strFileName);

		if(MessageBox(strMsg, "���Ϊ", MB_ICONQUESTION|MB_YESNO) == IDNO)
		{
			UpdateData(FALSE);
			return TRUE;
		}
	}

	// �ռ��û�ѡ����Ϣ
	for(int i=0; i<m_ctrlcnt; i++)
	{
		m_save.para_value[i] = m_pComboBox[i+1]->GetCurSel();
	}

	// ��ȡĿ��ͼ��λ�����Ϣ
	int	bitcount = (int)m_pComboBox[0]->GetItemData(m_pComboBox[0]->GetCurSel());

	// ����ԭת��������
	IRWE_CNVMARK	cnvmode    = m_pPack->cnvmark;
	LPINFOSTR		oldcnvpack = m_pPack->cnv_img;

	m_pPack->cnv_img = 0;
	m_pPack->cnvmark = IRWE_CNVMARK_NOT;

	if (oldcnvpack)
	{
		m_pPack->data_state &= ~SDP_DATA_STATE_CNV;
	}

	// ��ͼ��ת��Ϊ�û�ָ����λ��
	if (isirw_cnv(m_pPack, GetStdIDFromBitCnt(bitcount), 0) >= 0)
	{
		// ����ͼ��
		if (isirw_s_write_img(ISEEIO_CONFER_LOC_FILE, (const char *)strPathName, 0, 0, m_pPack, &m_save, 0) == 0)
		{
			// �ȴ��������
			AfxGetApp()->DoWaitCursor(1);
			int retwait = isirw_s_wait(m_pPack, 0);
			AfxGetApp()->DoWaitCursor(0);
			
			switch (retwait)
			{
			case	ER_SUCCESS:
				break;
			case	ER_USERBREAK:
				DispLastErrMess(ISIRW_LASTERR_USER);
				break;
			case	ER_MEMORYERR:
				DispLastErrMess(ISIRW_LASTERR_MEM);
				break;
			case	ER_FILERWERR:
				DispLastErrMess(ISIRW_LASTERR_RW);
				break;
			case	ER_SYSERR:
				DispLastErrMess(ISIRW_LASTERR_OSERR);
				break;
			case	ER_NSIMGFOR:
				ASSERT(0);								// ����������
				break;
			default:
				ASSERT(0);								// ������������ֵ
				break;
			}
		}
		else
		{
			DispLastErrMess(isirw_s_get_last_err());
		}
	}
	else
	{
		DispLastErrMess(ISIRW_LASTERR_MEM);
	}

	// �ͷ�ת����
	if (m_pPack->cnv_img)
	{
		isirw_free_INFOSTR(m_pPack->cnv_img);
	}

	if (oldcnvpack)
	{
		m_pPack->data_state |= SDP_DATA_STATE_CNV;
	}
	
	// �ָ�ԭ״̬
	m_pPack->cnv_img = oldcnvpack;
	m_pPack->cnvmark = cnvmode;

	UpdateData(FALSE);

	return FALSE;
}


// ��λ��ֵת��Ϊ��׼��ʽID
IRWE_CNVMARK CFileDialogEx::GetStdIDFromBitCnt(int bitcnt)
{
	IRWE_CNVMARK ret = IRWE_CNVMARK_32;

	switch (bitcnt)
	{
	case	1:
		ret = IRWE_CNVMARK_1;
		break;
	case	4:
		ret = IRWE_CNVMARK_4;
		break;
	case	8:
		ret = IRWE_CNVMARK_8;
		break;
	case	16:
		ret = IRWE_CNVMARK_16_555;						// ���湦��ֻ֧��555��ʽ
		break;
	case	24:
		ret = IRWE_CNVMARK_24;
		break;
	case	32:
		ret = IRWE_CNVMARK_32;
		break;
	default:
		ASSERT(0);										// ������湦���ṩ��λ����ܳ�����׼��ʽ֮��
		break;
	}
	
	return ret;
}


// ��ʾ������Ϣ
EXERESULT CFileDialogEx::DispLastErrMess(int err)
{
	EXERESULT	ret;
	CString		str;

	str.Empty();

	switch (err)
	{
	case	ISIRW_LASTERR_SUCCESS:				// û�д���
		ret = ER_SUCCESS;
		break;
	case	ISIRW_LASTERR_SYSCLOSE:				// ����ϵͳδ����
		str = "ISeeͼ���д����ϵͳδ�������޷�����ͼ��";
		ret = ER_SUCCESS;
		break;
	case	ISIRW_LASTERR_MEM:					// �ڴ治��
		str = "�ڴ治�㣬��������޷���ɣ�";
		ret = ER_MEMORYERR;
		break;
	case	ISIRW_LASTERR_RW:					// ���ʶ�д����
		str = "�������̶�д���󣬱�������޷���ɣ�";
		ret = ER_FILERWERR;
		break;
	case	ISIRW_LASTERR_USER:					// �������û��ж�
		str = "�û��жϱ���������ò�����δ��ɡ�";
		ret = ER_USERBREAK;
		break;
	case	ISIRW_LASTERR_IMGERR:				// ͼ�����(������ͼ��)
		str = "ͼ�����(������ͼ��)��";
		ret = ER_BADIMAGE;
		break;
	case	ISIRW_LASTERR_OSERR:				// ����ϵͳ����
		str = "����ϵͳ���󣡱�������޷���ɣ�";
		ret = ER_SYSERR;
		break;
	default:
		ASSERT(0);
		str = "����쳣�������˳�����";
		ret = ER_SYSERR;
		break;
	}

	if (!str.IsEmpty())
	{
		AfxMessageBox((LPCTSTR)str);
	}

	return ret;
}



