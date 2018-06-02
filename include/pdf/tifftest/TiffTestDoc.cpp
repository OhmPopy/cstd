// TiffTestDoc.cpp : implementation of the CTiffTestDoc class
//

#include "stdafx.h"
#include "TiffTest.h"

#include "TiffTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTiffTestDoc

IMPLEMENT_DYNCREATE(CTiffTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CTiffTestDoc, CDocument)
	//{{AFX_MSG_MAP(CTiffTestDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTiffTestDoc construction/destruction

CTiffTestDoc::CTiffTestDoc()
{
	// TODO: add one-time construction code here
	m_hDIB = NULL;
	m_sizeDoc = 0;
	m_dwBmSize = 0;
}

CTiffTestDoc::~CTiffTestDoc()
{
	m_sizeDoc = 0;
	if(NULL != m_hDIB)
	{
		GlobalFree(m_hDIB);
		m_hDIB = NULL;
	}
}

BOOL CTiffTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTiffTestDoc serialization

void CTiffTestDoc::Serialize(CArchive& ar)
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
// CTiffTestDoc diagnostics

#ifdef _DEBUG
void CTiffTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTiffTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTiffTestDoc commands

BOOL CTiffTestDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	DWORD dwFileLength = 0;
	CString strTemp = _T("");
	WORD wDECount = 0;
	BYTE* pDIB = NULL;
	int i = 0;

	IFH ifh;
	ZeroMemory(&ifh, sizeof(IFH));

	CFile file;
	CFileException fe;

	if(0 == file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		AfxMessageBox("���ļ�ʧ��");
		return FALSE;
	}
	dwFileLength = file.GetLength();

	if(sizeof(IFH) != file.Read(&ifh, sizeof(IFH)))
	{
		AfxMessageBox("��TIF�ļ�ͷʧ��");
		return FALSE;
	}

	if(0x2a != ifh.version)
	{
		AfxMessageBox("���ļ�����TIF��ʽ�����ļ�ʧ��");
		return FALSE;
	}

	if(0x4949 != ifh.byteOrder)
	{
		AfxMessageBox("��TIF�ļ�����IBMPC�ֽ��򣬶��ļ�ʧ��");
		return FALSE;
	}

	file.Seek(ifh.offsetToIFD, CFile::begin);
	
	if(2 != file.Read(&wDECount, 2))
	{
		AfxMessageBox("�޷����TIF�ļ�Ŀ¼�������");
		return FALSE;
	}
	strTemp.Format("��TIF�ļ���%d��Ŀ¼���", wDECount);
	AfxMessageBox(strTemp);

	DE* pde = new DE[wDECount];
	DE* pTemp = pde;
	memset(pde, 0, sizeof(DE)*wDECount);
	if(sizeof(DE)*wDECount != file.ReadHuge(pde, sizeof(DE)*wDECount))
	{
		AfxMessageBox("��ͼ���ļ�Ŀ¼ʧ��");
		delete []pde;
		return FALSE;
	}

	//��ʾͼ��Ŀ¼�����Ϣ--������
	for(i=0; i<wDECount; i++)
	{
		pTemp = pde + i;
		strTemp.Format("DE[%d].tag = %d\r\nDE[%d].type = %d\r\nDE[%d].length = %d\r\nDE[%d].valueOffset = %d",
			i, pTemp->tag, i, pTemp->type, i, pTemp->length, i, pTemp->valueOffset);
		AfxMessageBox(strTemp);
	}

	for(i=0; i<wDECount; i++)
	{
		pTemp = pde + i;
		if(256 == pTemp->tag)	//ͼ����
		{
			m_sizeDoc.cx = pTemp->valueOffset;
		}
		if(257 == pTemp->tag)	//ͼ��߶�
		{
			m_sizeDoc.cy = pTemp->valueOffset;
		}
		if(273 == pTemp->tag)	//����ͼ������ռ���ֽ���
		{
			//m_dwBmSize = pTemp->valueOffset - sizeof(IFH);
			//���߰�tag=256��valueOffset����tag=257��valueOffset
			m_dwBmSize = m_sizeDoc.cx * m_sizeDoc.cy;
		}
	}

	//����BMPͼ���ڴ��
	if(!CreateBmpBuffer())
	{
		AfxMessageBox("����BMPͼ���ڴ��ʧ��");
		delete []pde;
		return FALSE;
	}

	//���BMP�ڴ��ָ��
	pDIB = (BYTE*)GlobalLock(m_hDIB);
	if(NULL == pDIB)
	{
		AfxMessageBox("���BMP�ڴ��ָ��ʧ��");
		GlobalUnlock(m_hDIB);
		delete []pde;
		return FALSE;
	}

	//����BITMAPFILEHEADER�����Ƶ�BMP�ڴ��
	BITMAPFILEHEADER bmfHdr;
	memset(&bmfHdr, 0, sizeof(BITMAPFILEHEADER));
	bmfHdr.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
					   256*sizeof(RGBQUAD);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfSize = bmfHdr.bfOffBits + m_dwBmSize;
	bmfHdr.bfType = 0x4d42;

	memmove(pDIB, &bmfHdr, sizeof(BITMAPFILEHEADER));
	
	//����BITMAPINFOHEADER�����Ƶ�BMP�ڴ��
	BITMAPINFOHEADER bmiHdr;
	memset(&bmiHdr, 0, sizeof(BITMAPINFOHEADER));
	bmiHdr.biBitCount = 8;
	bmiHdr.biClrImportant = 0;
	bmiHdr.biClrUsed = 0;
	bmiHdr.biCompression = 0;
	bmiHdr.biHeight = m_sizeDoc.cy;
	bmiHdr.biPlanes = 1;
	bmiHdr.biSize = sizeof(BITMAPINFOHEADER);
	bmiHdr.biSizeImage = 0;
	bmiHdr.biWidth = m_sizeDoc.cx;
	bmiHdr.biXPelsPerMeter = 2834;
	bmiHdr.biYPelsPerMeter = 2834;
	
	memmove((BITMAPFILEHEADER*)pDIB + 1, &bmiHdr, sizeof(BITMAPINFOHEADER));

	//����256��RGBQUAD�����Ƶ�BMP�ڴ��
	RGBQUAD* pRgbQuad = (RGBQUAD*)(pDIB + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	RGBQUAD* pOldQuad = pRgbQuad;
	RGBQUAD rgbQuad;
	memset(&rgbQuad, 0, sizeof(RGBQUAD));
	for(i=0; i<256; i++)
	{
		rgbQuad.rgbBlue = i;
		rgbQuad.rgbGreen = i;
		rgbQuad.rgbRed = i;
		rgbQuad.rgbReserved = 0;
		pRgbQuad = pOldQuad + i;
		memmove(pRgbQuad, &rgbQuad, sizeof(RGBQUAD));
	}

	//���������������, �ߵ�ͼ�����ݴ����һ�п�ʼ����
	int j = 0;
	for(i=m_sizeDoc.cy-1; i>=0; i--)
	{
		file.Seek(sizeof(IFH) + i*m_sizeDoc.cx, CFile::begin);
		file.Read((BYTE*)(pRgbQuad + 1) + j*m_sizeDoc.cx, m_sizeDoc.cx);
		j++;
	}

	//��ʼ��ר�õ�ɫ��
	BYTE buf[2+2+4*256];
	LOGPALETTE* pPal = (LOGPALETTE*)buf;
	pPal->palVersion = 0x300;
	pPal->palNumEntries = 256;
	for(i=0; i<255; i++)
	{
		pPal->palPalEntry[i].peBlue = i;
		pPal->palPalEntry[i].peFlags = 0;
		pPal->palPalEntry[i].peGreen = i;
		pPal->palPalEntry[i].peRed = i;
	}
	m_palDIB.CreatePalette(pPal);

	//����BMPͼ��
/*	CFile fileWrite("mytiff.bmp", CFile::modeCreate | CFile::modeWrite);
	fileWrite.WriteHuge(pDIB, sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD) + m_dwBmSize);
*/
	GlobalUnlock(m_hDIB);
	delete []pde;
	return TRUE;
}

BOOL CTiffTestDoc::CreateBmpBuffer()
{
	//����BMP�ڴ��
	m_hDIB = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,
		sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD) + m_dwBmSize);
	if(NULL == m_hDIB)
	{
		AfxMessageBox("����BMP�ڴ��ʧ��");
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
