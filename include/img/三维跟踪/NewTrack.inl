#include "algo.h"
#include "TriDimModel.h"
#include "calibration.h"
#include "GMModel.h"
#include "3dTrack.h"
#include "NewTrack.h"



// 2, ������άģ�ͽ���.

/* 2.0 ����������άģ�Ͷ���. */
TriDimModel* CHC_CreateTDModel( int h,
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
                                int hTop ) {
  return new TriDimModel( h, w, subH, subW, pCbr->GetTMat(), pCbr->GetITMat(), sampNum, rLow, rMid, rTop, hLow, hMid, hTop );
}

/* 2.1 ɾ��������άģ�Ͷ���. */
void CHC_ReleaseTDModel( TriDimModel **pTdm ) {
  delete * pTdm;
  *pTdm = NULL;
}



// 4, ����.

/* 4.0 �������ٶ��� */
TriDimTracker* CHC_CreateTDTModel( int h, int w, int maxObjNum ) {
  return new TriDimTracker( h, w, maxObjNum );
}

/* 4.1 ɾ�����ٶ���. */
void CHC_ReleaseTDTModel( TriDimTracker **pTdt ) {
  delete * pTdt;
  *pTdt = NULL;
}

/* 4.2 ���ٴ���. */
void CHC_TrackProc( TriDimTracker *pTdt, const uchar *src, const GMM *pGmm, int num, const Calibration *pCbr, const TriDimModel *pTdm ) {
  pTdt->MultiTrack( src, pGmm->BlobRst(), num, pCbr, pTdm );
}

/* 4.3 �õ���Ź켣����������������λ�ø���. */
int CHC_GetCellNum( const TriDimTracker *pTdt ) {
  return pTdt->GetMaxObjNum();
}

/* 4.4 �õ���ǰ�켣����. */
int CHC_GetTraceNum( const TriDimTracker *pTdt ) {
  return pTdt->GetCurObjNum();
}

/* 4.5 �켣�����е�i��λ�����Ƿ����˹켣. */
int CHC_IsExistTrace( const TriDimTracker *pTdt, int i ) {
  return pTdt->GetTracePtr( i );
}

/* 4.6 �켣�����е�i��λ���ϵĹ켣�ı�ʶ. */
int CHC_GetTraceIdentity( const TriDimTracker *pTdt, int i ) {
  return pTdt->GetIdentity( i );
}

/* 4.7 �켣�����е�i��λ���ϵĹ켣��״̬. */
int CHC_GetTraceStatus( const TriDimTracker *pTdt, int i ) {
  return int( pTdt->GetStatus( i ) );
}

/* 4.8 �켣�����е�i��λ���ϵĹ켣������������Ϣ. */
void CHC_GetTraceGeoMsg( const TriDimTracker *pTdt, int i, int *msg ) {
  const Block block = pTdt->GetGeoMsg( i );
  
  *( msg + 0 ) = block.w;
  *( msg + 1 ) = block.h;
  
  *( msg + 2 ) = block.l;
  *( msg + 3 ) = block.t;
  *( msg + 4 ) = block.r;
  *( msg + 5 ) = block.b;
}

/* 4.9 �켣�����е�i��λ���ϵĹ켣�ķ�����Ϣ. */
int CHC_GetTraceKind( const TriDimTracker *pTdt, int i ) {
  return int( pTdt->GetKind( i ) );
}

/* 4.10 �켣�����е�i��λ���ϵĹ켣�ķ�����Ϣ. */
double CHC_GetTraceDirection( const TriDimTracker *pTdt, int i ) {
  return pTdt->GetDirection( i );
}

/* 4.11 �켣�����е�i��λ���ϵĹ켣���ſ����. */
int CHC_GetTraceBlockArea( const TriDimTracker *pTdt, int i ) {
  return pTdt->GetArea( i );
}

/* 4.12 �켣�����е�i��λ���ϵĹ켣����ʵ����. */
double CHC_GetTraceSpeed( const TriDimTracker *pTdt, int i ) {
  return pTdt->GetSpeed( i );
}

/* 4.13 �켣�����е�i��λ���ϵĹ켣����ʵ�߶�. */
double CHC_GetTraceHeight( const TriDimTracker *pTdt, int i ) {
  return pTdt->GetRealHeight( i );
}
