#ifndef _TRI_DIM_TRACK_
#define _TRI_DIM_TRACK_
#ifdef __cplusplus
extern "C" {;
#endif
// ���͵�, �����͵�����.
struct IPoint {
  int x;
  int y;
};
struct FPoint {
  double x;
  double y;
};
// �궨����ǰ������.
struct Calibration;
struct GMM;
struct TriDimModel;
struct TriDimTracker;
typedef unsigned char uchar;
//  1, ��ά�����궨.  1.0 ������ά�����궨����. ��������һ��ָ����ά�����궨�����ָ��, ����ʧ���򷵻ؿ�ָ��.
// * �ú��������Ķ���ʹ����Ϻ������ú���ReleaseCbrModelɾ���ö���.
// * refPts    : �궨�߶ζ˵�ͼ�������. һ��Ϊ5����ֱ�ߺ�2������ϵ�ƽ���߹�18����.
// * refHeight : �궨�������ʵ�ʸ߶�, ��λΪ����(cm).
// * vLineNum  : ��ֱ������.
// * hLineNum  : ˮƽ������.
// * SceneWidth: ͼ�����ؿ��.
Calibration* CHC_CreateCbrModel( const IPoint * refPts, double refHeight, int vLineNum, int hLineNum, int SceneHeight );
//  1.1 ���궨���״��. ��������0��ʾ�궨�ɹ�.
// * ����ֵ˵��
// *  0: �궨�ɹ�.
// * -1: �ڴ����ʧ��.
// * -2: ��ֱ���г���ƽ�����.
// * -3: ������ֱֱ�ߵĽ����y���궼С��0, ȫ��������.
// * -4: ˮƽ�߶��г���ƽ�����.
// * -5: �����г��ֶԸ������������.
// * pCbr: ָ�򳡾��궨�����ָ��.
int CHC_CheckCbrStatus( const Calibration * pCbr );
//  1.2 �����߶�ʵ�ʳ�����֤, ��������������ʵ�ʾ���.
// * ���������������ͼ���ϵ���������, �����������ʵ�ʾ���.
// * pCbr : ָ����ά�����궨�����ָ��.
// * begin: �����ϵ��߶ε������ͼ���ϵ���������.
// * end  : �����ϵ��߶ε��յ���ͼ���ϵ���������.
double CHC_ValidateLen( const Calibration * pCbr, const IPoint * begin, const IPoint * end );
//  1.3 Ŀ�����ظ߶���֤. ��������Ŀ�궥����������.
// * ����Ŀ���ʵ�ʸ߶Ⱥ�Ŀ��ĵײ�(��)��ͼ���ϵ���������, ����Ŀ��Ķ���(ͷ)��ͼ���ϵ���������.
// * pCbr      : ָ����ά�����궨�����ָ��.
// * tail      : Ŀ��ײ���ͼ���ϵ���������.
// * realHeight: Ŀ���ʵ�ʸ߶�.
FPoint CHC_ValidateHeadPos( const Calibration * pCbr, const IPoint * tail, int realHeight );
//  1.4 ɾ����ά�����궨����.
// * ע��������ָ��궨�����ָ���ָ��.
// * pCbr: ָ��궨����ָ���ָ��.
void CHC_ReleaseCbrModel( Calibration **pCbr );
// 2, ������άģ�ͽ���. 
// 2.0 ����������άģ�Ͷ���. �����Ķ�����ʹ��ʱ������ú���CHC_ReleaseTDModel�ͷ�.
// * ����������άģ�Ͷ���, ���ڸ��ݸ��ٶ���Ĵ�С�ж�������(��, ��..), �ж�ϸ���Ѿ���װ����ά��������.
// * h      : �������ظ߶�.
// * w      : �������ؿ��.
// * subH   : ����ƽ���ָ�ʱ, ��ֱ������ƽ�����ֵ��ӿ���Ŀ(������ƽ������ΪsubH * sumW�鲢�ڸ����ӿ齨����Ӧ����ά����ģ��).
// * subW   : ����ƽ���ָ�ʱ, ˮƽ������ƽ�����ֵ��ӿ���Ŀ.
// * pCbr   : ��ά�궨����ָ��(��ָ��ָ��Ķ����Ѿ��ڵ�һ���д���).
// * ���²����������ʵ�������������ֵ.
// * sampNum: ģ��Բ���ϵĳ�����ĸ���.
// * rLow   : �Ȳ�Բ��ģ�Ͱ뾶.
// * rMid   : ����Բ��ģ�Ͱ뾶.
// * rTop   : ͷ��Բ��ģ�Ͱ뾶.
// * hLow   : �Ȳ����ظ߶�.
// * hMid   : �������ظ߶�.
// * hTop   : ͷ�����ظ߶�.
TriDimModel* CHC_CreateTDModel( int h,int w,int subH,int subW,Calibration * pCbr,int sampNum,int rLow,int rMid,int rTop,int hLow,int hMid,int hTop );
// 2.1 ����������άģ�Ͷ���.
// pTdm: ָ����ά����ģ�Ͷ����ָ���ָ��.
void CHC_ReleaseTDModel( TriDimModel **pTdm );
//  4, ����.  4.0 �������ٶ���. ����ʧ��ʱ���ؿ�ָ��.
// * ���ٶ�������Ҫʹ��ʱ������ú���CHC_ReleaseTDTModel�ͷŸö���.
// * h        : �������ظ߶�.
// * w        : �������ؿ��.
// * maxObjNum: ���켣��������.
TriDimTracker* CHC_CreateTDTModel( int h, int w, int maxObjNum );
//  4.1 ɾ�����ٶ���.
// * ע�⴫��Ĳ�����ָ����ٶ���ָ���ָ��.
// * pTdt: ָ����ٶ���ָ���ָ��.
void CHC_ReleaseTDTModel( TriDimTracker **pTdt );
//  4.2 ���ٴ���.
// * ����˵��
// * pTdt: ָ����ٶ����ָ��.
// * src : ��Ҫ�����ٴ����ԭʼRGBͼ������.
// * pGmm: ��˹������ģ����ָ��.
// * num : ��ǰ֡�ſ���������, ��ֵ�ɺ���CHC_ForegroundDetect����.
// * pCbr: ��ά�궨����ָ��.
// * pTdm: ������άģ�Ͷ���ָ��.
void CHC_TrackProc( TriDimTracker * pTdt, const uchar * src, const GMM * pGmm, int num, const Calibration * pCbr, const TriDimModel * pTdm );
//  4.3 �õ���Ź켣����������������λ�ø���.
// * �õ���ά���ٶ����д�Ź켣������Ĵ�С, �������Դ��(����)�������켣, �봴����ά���ٶ���ʱ��maxObjNumֵ���.
// * pTdt: ָ����ٶ����ָ��.
int CHC_GetCellNum( const TriDimTracker * pTdt );
//  4.4 �õ���ǰ�켣����. �����������ڱ������еĹ켣������(����ֵ <= maxObjNum).
// * ����˵��
// * pTdt: ָ����ٶ����ָ��.
int CHC_GetTraceNum( const TriDimTracker * pTdt );
//  4.5 �켣�����е�i��λ�����Ƿ����˹켣.
// * ���켣�����е�i��λ�����Ƿ��������ڱ�����Ĺ켣, ����˹켣����1, ���򷵻�0.
// * pTdt: ָ����ά���ٶ����ָ��.
// * i   : �켣λ������.
int CHC_IsExistTrace( const TriDimTracker * pTdt, int i );
//  4.6 �켣�����е�i��λ���ϵĹ켣�ı�ʶ.
// * ʹ�ú���CHC_IsExistTrace�ж���i��λ���ϴ��ڹ켣֮��, ���øú�����ȡ��λ���ϵĹ켣�ı�ʶ, ��ʶʹ��һ����������ʾ.
// * ͬ����CHC_IsExistTrace.
int CHC_GetTraceIdentity( const TriDimTracker * pTdt, int i );
//  4.7 �켣�����е�i��λ���ϵĹ켣��״̬.
// * ʹ�ú���CHC_IsExistTrace�ж���i��λ���ϴ��ڹ켣֮��, ���øú�����ȡ��λ���ϵĹ켣��״̬.
// * ͬ����CHC_IsExistTrace.
// * ����ֵ˵��
// * 1: �³��ֵĹ켣.
// * 2: ���������еĹ켣.
// * 3: �����ϲ��Ĺ켣.
// * 4: �������ѵĹ켣.
// * 5: ������ʧ״̬�Ĺ켣.
int CHC_GetTraceStatus( const TriDimTracker * pTdt, int i );
//  4.8 �켣�����е�i��λ���ϵĹ켣������������Ϣ.
// * ʹ�ú���CHC_IsExistTrace�ж���i��λ���ϴ��ڹ켣֮��, ���øú�����ȡ��λ���ϵĹ켣�����ĺ������Ϣ.
// * pTdt: ָ����ά���ٶ����ָ��.
// * i   : �켣λ������.
// * msg : ���6�����α���������ָ��, ����ռ����ⲿ����.
//     ���ݴ��˳��Ϊ����x����, ����y����, ������Ͻ�x����, ���Ͻ�y����, ���½�x����, ���½�y����.
void CHC_GetTraceGeoMsg( const TriDimTracker * pTdt, int i, int * msg );
//  4.9 �켣�����е�i��λ���ϵĹ켣�ķ�����Ϣ.
// * ʹ�ú���CHC_IsExistTrace�ж���i��λ���ϴ���˹켣֮��, ���øú�����ȡ��λ���ϵĹ켣�����.
// * ͬ����CHC_IsExistTrace.
// * ����ֵ˵��
// * 1: ����.
// * 2: ����.
// * 4: ����.
// * 8: δ֪.
int CHC_GetTraceKind( const TriDimTracker * pTdt, int i );
//  4.10 �켣�����е�i��λ���ϵĹ켣�ķ�����Ϣ.
//  * ʹ�ú���CHC_IsExistTrace�ж���i��λ���ϴ���˹켣֮��, ���øú�����ȡ��λ���ϵĹ켣�ķ���.
//  * ����ֵ˵��
//  * ����ֵΪ��Χ��[0, 360)����ĸ�����, ��Ŀ�����ĺ���һ֡���û�иı�ʱ, ���ص�ֵҲ��0.
double CHC_GetTraceDirection( const TriDimTracker * pTdt, int i );
//  4.11 �켣�����е�i��λ���ϵĹ켣����Ӧ�ſ�����(���ص����).
//  * ʹ�ú���CHC_IsExistTrace�ж���i��λ���ϴ���˹켣֮��, ���øú�����ȡ��λ���ϵĹ켣����Ӧ�ſ�����ص����.
int CHC_GetTraceBlockArea( const TriDimTracker * pTdt, int i );
//  4.12 �켣�����е�i��λ���ϵĹ켣���ƶ���ʵ����.
//  * ʹ�ú���CHC_IsExistTrace�ж���i��λ���ϴ���˹켣֮��, ���øú�����ȡ��λ���ϵĹ켣����ʵ�����е�����.
double CHC_GetTraceSpeed( const TriDimTracker * pTdt, int i );
//  4.12 �켣�����е�i��λ���ϵĹ켣����ʵ�߶�.
//  * ʹ�ú���CHC_IsExistTrace�ж���i��λ���ϴ���˹켣֮��, ���øú�����ȡ��λ���ϵĹ켣����ʵ�����еĸ߶�.
double CHC_GetTraceHeight( const TriDimTracker * pTdt, int i );
#ifdef __cplusplus
}
#endif
#endif
