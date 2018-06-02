#include "algo.h"
#include "TriDimModel.h"
#include "..\��ά�궨\calibration.h"
#include "..\..\GMM_Backup\GMModel_Beta\GMModel.h"
#include "3dTrack.h"
#include "NewTrack.h"


// 1, ��ά�����궨.

/* 1.0 ������ά�����궨����. */
Calibration* CHC_CreateCbrModel(const IPoint *refPts, double refHeight, int vLineNum, int hLineNum, int SceneHeight)
{
	return new Calibration(refPts, refHeight, vLineNum, hLineNum, SceneHeight);
}

/* 1.1 ���궨���״��. */
int CHC_CheckCbrStatus(const Calibration *pCbr)
{
	return pCbr->GetStatus();
}

/* 1.2 �����߶�ʵ�ʳ�����֤. */
double CHC_ValidateLen(const Calibration *pCbr, const IPoint *begin, const IPoint *end)
{
	return pCbr->RealDist(begin, end);
}

/* 1.3 Ŀ�����ظ߶���֤. */
FPoint CHC_ValidateHeadPos(const Calibration *pCbr, const IPoint *tail, int realHeight)
{
	FPoint pixelHead;

	pCbr->ValidPos(&pixelHead, tail, realHeight);

	return pixelHead;
}

/* 1.4 ɾ����ά�����궨����. */
void CHC_ReleaseCbrModel(Calibration **pCbr)
{
	delete *pCbr; *pCbr = NULL;
}


// 2, ������άģ�ͽ���.

/* 2.0 ����������άģ�Ͷ���. */
TriDimModel* CHC_CreateTDModel(int h, 
							   int w, 
							   int subH, 
							   int subW, 
							   Calibration *pCbr, 
							   int sampNum, 
							   int rLow, 
							   int rMid, 
							   int rTop, 
							   int hLow, 
							   int hMid, 
							   int hTop)
{
	return new TriDimModel(h, w, subH, subW, pCbr->GetTMat(), pCbr->GetITMat(), sampNum, rLow, rMid, rTop, hLow, hMid, hTop);
}

/* 2.1 ɾ��������άģ�Ͷ���. */
void CHC_ReleaseTDModel(TriDimModel **pTdm)
{
	delete *pTdm; *pTdm = NULL;
}


// 3, ��˹������ģ.

/* 3.0 ������˹������ģ����. */
GMM* CHC_CreateGMModel(double afa, 
		               double thrd, 
		               double virV, 
		               double virW, 
		               double shadowp0, 
		               double shadowp1, 
		               double shadowp2, 
		               double shadowp3, 
		               int r, 
		               int c, 
		               int num, 
		               int minPts, 
		               bool isRectLabel, 
		               bool isRectShadowX, 
		               bool isRectShadowY, 
		               bool isRectShape)
{
	return new GMM(afa, thrd, virV, virW, shadowp0, shadowp1, shadowp2, shadowp3, r, c, num, minPts, isRectLabel, isRectShadowX, isRectShadowY, isRectShape);
}

/* 3.1 ��������. */
void CHC_SetupBG(GMM *pGmm, const uchar *src, int step, int num)
{
	pGmm->UpdateModel(src, step, num);
}

/* 3.2 ǰ�����. */
int CHC_ForegroundDetect(GMM *pGmm, const uchar *src, int step, const uchar *mask)
{
	return pGmm->Detect(src, step, mask);
}

/* 3.3  ɾ����˹������ģ����. */
void CHC_ReleaseGMModel(GMM** pGmm)
{
	delete *pGmm; *pGmm = NULL;
}

/* 3.4 ��ȡ����ֵ������ſ����֮��Ľ��. */
const unsigned int* CHC_GetBlobRst(GMM *pGmm)
{
	return pGmm->BlobRst();
}


// 4, ����.

/* 4.0 �������ٶ��� */
TriDimTracker* CHC_CreateTDTModel(int h, int w, int maxObjNum)
{
	return new TriDimTracker(h, w, maxObjNum);
}

/* 4.1 ɾ�����ٶ���. */
void CHC_ReleaseTDTModel(TriDimTracker **pTdt)
{
	delete *pTdt; *pTdt = NULL;
}

/* 4.2 ���ٴ���. */
void CHC_TrackProc(TriDimTracker *pTdt, const uchar *src, const GMM *pGmm, int num, const Calibration *pCbr, const TriDimModel *pTdm)
{
	pTdt->MultiTrack(src, pGmm->BlobRst(), num, pCbr, pTdm);
}

/* 4.3 �õ���Ź켣����������������λ�ø���. */
int CHC_GetCellNum(const TriDimTracker *pTdt)
{
	return pTdt->GetMaxObjNum();
}

/* 4.4 �õ���ǰ�켣����. */
int CHC_GetTraceNum(const TriDimTracker *pTdt)
{
	return pTdt->GetCurObjNum();
}

/* 4.5 �켣�����е�i��λ�����Ƿ����˹켣. */
int CHC_IsExistTrace(const TriDimTracker *pTdt, int i)
{
	return pTdt->GetTracePtr(i);
}

/* 4.6 �켣�����е�i��λ���ϵĹ켣�ı�ʶ. */
int CHC_GetTraceIdentity(const TriDimTracker *pTdt, int i)
{
	return pTdt->GetIdentity(i);
}

/* 4.7 �켣�����е�i��λ���ϵĹ켣��״̬. */
int CHC_GetTraceStatus(const TriDimTracker *pTdt, int i)
{
	return int(pTdt->GetStatus(i));
}

/* 4.8 �켣�����е�i��λ���ϵĹ켣������������Ϣ. */
void CHC_GetTraceGeoMsg(const TriDimTracker *pTdt, int i, int *msg)
{
	const Block block = pTdt->GetGeoMsg(i);

	*(msg + 0) = block.cx;
	*(msg + 1) = block.cy;

	*(msg + 2) = block.l;
	*(msg + 3) = block.t;
	*(msg + 4) = block.r;
	*(msg + 5) = block.b;
}

/* 4.9 �켣�����е�i��λ���ϵĹ켣�ķ�����Ϣ. */
int CHC_GetTraceKind(const TriDimTracker *pTdt, int i)
{
	return int(pTdt->GetKind(i));
}

/* 4.10 �켣�����е�i��λ���ϵĹ켣�ķ�����Ϣ. */
double CHC_GetTraceDirection(const TriDimTracker *pTdt, int i)
{
	return pTdt->GetDirection(i);
}

/* 4.11 �켣�����е�i��λ���ϵĹ켣���ſ����. */
int CHC_GetTraceBlockArea(const TriDimTracker *pTdt, int i)
{
	return pTdt->GetArea(i);
}

/* 4.12 �켣�����е�i��λ���ϵĹ켣����ʵ����. */
double CHC_GetTraceSpeed(const TriDimTracker *pTdt, int i)
{
	return pTdt->GetSpeed(i);
}

/* 4.13 �켣�����е�i��λ���ϵĹ켣����ʵ�߶�. */
double CHC_GetTraceHeight(const TriDimTracker *pTdt, int i)
{
	return pTdt->GetRealHeight(i);
}