// DibMerger.cpp: implementation of the CDibMerger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Texture.h"
#include "Dib.h"
#include "DibMerger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDibMerger::CDibMerger()
{

}

CDibMerger::~CDibMerger()
{

}

/*
BOOL CDibMerger::SetComparDIB(CDib *bmp1, CDib *bmp2)
{
   
	//������
    m_iMAXarea=0;
    //�ӵڶ���ͼ��������Ͻǿ�ʼ���ӵ�һ��ͼ��������Ͻǿ�ʼ���ȵ���бȿ�
	for(int rightbmp_h=0;rightbmp_h<bmp2->GetHeight()/3;rightbmp_h++)
	    for(int leftbmp_w=bmp1->GetWidth();leftbmp_w>=bmp1->GetWidth()/2;leftbmp_w--)
		    for(unsigned int leftbmp_h=0;leftbmp_h<bmp1->GetHeight();leftbmp_h++)
		{
			bmp2->Pix_X=0;                   
			bmp2->Pix_Y=rightbmp_h;
			bmp1->Pix_X=leftbmp_w;
			bmp1->Pix_Y=leftbmp_h;
			if(IsSamePix(bmp1,bmp2,bmp1->Pix_X,bmp1->Pix_Y,bmp2->Pix_X,bmp2->Pix_Y))
				if(IsSameCol(bmp1,bmp2,bmp1->Pix_X,bmp1->Pix_Y,bmp2->Pix_X,bmp2->Pix_Y))
				if(IsSameRect(bmp1,bmp2))
						break;
					else 
						continue;
		}

	//�����ͬ��������Ϊ����ƴ��
     if(m_iMAXarea!=0)
	 {
		 MergeDIB(bmp1,bmp2);
		 return TRUE;
	 }
	 else
	 {
		 MessageBox("û���ҵ��ϲ��㣬������ϲ�");
		 return FALSE;
	 }
}

long CDibMerger::GetPixOffset(unsigned int X, unsigned int Y, CDib *bmp)
{

   //�������ͼ���е�λ��
	if((X>=0&&X<bmp->GetWidth())&&(Y>=0&&Y<bmp->GetHeight()))
		return (Y*bmp->GetDibSaveDim().cx+X*(bmp->GetBiBitCount()/8));//Ҫ��CDib�ж���bmp_line_w��bmp_biBitCount
	return 0;
}


BOOL CDibMerger::IsSamePix(CDib *bmp1, CDib *bmp2, int x1, int y1, int x2, int y2)
{


	BYTE *p,*q;
	p=bmp1->m_pDibBits+GetPixOffset(x1,y1,bmp1);
	q=bmp2->m_pDibBits+GetPixOffset(x2,y2,bmp2);
	for(int i=0;i<bmp1->GetBiBitCount()/8;i++)
		if(abs(p[i]-q[i])>50)

			return FALSE;
		//OR if(abs(p[i]-q[i])!=0)return FALSE;  //m_pDibBits
		else 
			return TRUE;
		return TRUE;
}


BOOL CDibMerger::IsSameCol(CDib *bmp1, CDib *bmp2, int x1, int y1, int x2, int y2)
{

    //���㵱ǰ���ص㵽ͼ��1�ײ��ĸ߶�
	unsigned int below_h1=bmp1->GetHeight()-y1;
	//����ͼ��2��ǰ���ص㵽�ײ��ĸ߶�
	unsigned int below_h2=bmp2->GetHeight()-y2;
	unsigned int h=(below_h1>below_h2)?below_h2:below_h1;

	for(unsigned int i=0;i<h;i++)
	{
		//�ж��Ƿ�����ͬ�����ص㣬����ǣ�����ͬһ�������±Ƚ�
		if(!IsSamePix(bmp1,bmp2,x1,y1,x2,y2))
			return FALSE ;
		y1++;
		y2++;
	}
	return TRUE;
}

BOOL CDibMerger::IsSameRect(CDib *bmp1, CDib *bmp2)
{


	unsigned int x1,y1,x2,y2;
	unsigned int w1=bmp1->GetWidth()-bmp1->Pix_X;
	unsigned int w2=bmp2->GetWidth()-bmp2->Pix_X;
	unsigned int w=(w1>w2)?w2:w1;
	
	x1=bmp1->Pix_X;y1=bmp1->Pix_Y;
	x2=bmp2->Pix_X;y2=bmp2->Pix_Y;
	for(unsigned int i=0;i<w;i++)
	{     
		 //�ж��Ƿ�����ͬ���У�������������ƶ������Ƚ��Ƿ�����ͬ�Ŀ�
		if(!IsSameCol(bmp1,bmp2,x1,y1,x2,y2))
			return FALSE;
		x1++;
		x2++;
	}

	unsigned int h=(bmp1->GetHeight()-bmp1->Pix_Y)<(bmp2->GetHeight()-bmp2->Pix_Y)?
		(bmp1->GetHeight()-bmp1->Pix_Y):(bmp2->GetHeight()-bmp2->Pix_Y);
    //�������������ǰ������������ȡ��֮
	if(i*h>m_iMAXarea)
	{
		m_iMAXarea=i*h;

		left_starX=bmp1->Pix_X;
	    left_starY=bmp1->Pix_Y;
		left_endX=i+bmp1->Pix_X;
		left_endY=h+bmp1->Pix_Y;
		right_starX=bmp2->Pix_X;
		right_starY=bmp2->Pix_Y;
		right_endX=i+bmp2->Pix_X;
		right_endY=h+bmp2->Pix_Y;
	}
	return TRUE;
}

CDib * CDibMerger::MergeDIB(CDib *bmp1, CDib *bmp2)
{
    CDib *m_pDibResult;
	//
	BYTE* unitbuff;
	unitbuff=NULL;


	unsigned int w1=bmp1->GetHeight();
	unsigned int w2=bmp2->GetHeight();


	//��ͼ��Ŀ�
	unsigned int unit_w=bmp2->GetWidth()+left_starX;
	//�ص����ֵ��ϲ��߶�
	unsigned int unit_up=(left_starY>right_starY)?left_starY:right_starY;
	//�ص����ֵ��²��߶�
	unsigned int unit_down=((w1-left_endY)>(w2-right_endY))?(w1-left_endY):(w2-right_endY);
	//��ͼ��ĸ�
	unsigned int unit_h=unit_up+(left_endY-left_starY) +unit_down;


	//�õ�ÿ��ʵ��ռ���ֽ���
	int byte_w=unit_w*(bmp1->GetBiBitCount()/8);
	if(byte_w%4)
		byte_w=byte_w+(4-byte_w%4);

	//�õ���ͼ������ݴ�С
	int imagedata=byte_w*unit_h;


		// ����ͼ����CDib���װ
	m_pDibResult = new CDib(CSize(unit_w,unit_h), 8);

	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();
	
	
	// �����ڴ���ϲ����ͼ��
	LPBYTE lpImgResult;
	lpImgResult = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

	BYTE *p1,*p2,*p3;
	p1=bmp1->m_pDibBits+left_starY*bmp1->GetDibSaveDim().cx;  //����1��ʼ��ַ
	p2=bmp2->m_pDibBits+right_starY*bmp2->GetDibSaveDim().cx; //����2��ʼ��ַ
	p3=lpImgResult;

	//����ͼ������ݣ��Ȱѵ�һ��ͼ��ĵ�һ�д��ȥ���ٴ�ڶ���ͼ��ĵ�һ�У�Ȼ���ٴ�
	//��һ��ͼ��ĵڶ��У��ٴ�ڶ���ͼ��ĵڶ��У��Դ�����
	for(unsigned int i=0;i<unit_h;i++)
	{
		
		memcpy(p3,p1,left_starX*(bmp1->GetBiBitCount()/8));
		p3+=left_starX*(bmp1->GetBiBitCount()/8);
		p1+=bmp1->GetDibSaveDim().cx;
		memcpy(p3,p2,bmp2->GetWidth()*(bmp2->GetBiBitCount()/8));
		p3+=bmp2->GetWidth()*(bmp2->GetBiBitCount()/8);
		p2+=bmp2->GetDibSaveDim().cx;
        //�������4�ı���������Ϊ4�ı���
		if((unsigned int)(p3-unitbuff)%4!=0)
			p3+=4-(unsigned int)((p3-unitbuff)%4);
	}
	// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_pDibBits = lpImgResult;

    //������ʾƴ�Ӻ�ͼ��ĶԻ���
	CDlgMosaicRes* pDlg;
	pDlg = new CDlgMosaicRes(NULL, 	m_pDibResult);
	pDlg->DoModal();
	
	// ɾ������
	delete pDlg;


	return ;
	

}

*/
