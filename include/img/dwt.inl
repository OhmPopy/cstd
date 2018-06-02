// С���任�ĺ���

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

#define ASSERT assert

// С���任����ԭ��
// Daubechies��������С����
// ��֧ͬ�����䳤���µ��˲���ϵ������
const Float hCoef[ 10 ][ 20 ] = { //
  {
    .707106781187, .707106781187
  },

  { .482962913145, .836516303738, .224143868042, -.129409522551 },

  { .332670552950, .806891509311, .459877502118, -.135011020010, -.085441273882, .035226291882 },

  {
    .230377813309, .714846570553, .630880767930, -.027983769417,
    -.187034811719, .030841381836, .032883011667, -.010597401785
  },

  {
    .160102397974, .603829269797, .724308528438, .138428145901, -.242294887066,
    -.032244869585, .077571493840, -.006241490213, -.012580751999, .003335725285
  },

  {
    .111540743350, .494623890398, .751133908021, .315250351709, -.226264693965,
    -.129766867567, .097501605587, .027522865530, -.031582039318, .000553842201,
    .004777257511, -.001077301085
  },

  {
    .077852054085, .396539319482, .729132090846, .469782287405, -.143906003929,
    -.224036184994, .071309219267, .080612609151, -.038029936935, -.016574541631,
    .012550998556, .000429577973, -.001801640704, .000353713800
  },

  {
    .054415842243, .312871590914, .675630736297, .585354683654, -.015829105256,
    -.284015542962, .000472484574, .128747426620, -.017369301002, -.044088253931,
    .013981027917, .008746094047, -.004870352993, -.000391740373, .000675449406,
    -.000117476784
  },

  {
    .038077947364, .243834674613, .604823123690, .657288078051, .133197385825,
    -.293273783279, -.096840783223, .148540749338, .030725681479, -.067632829061,
    .000250947115, .022361662124, -.004723204758, -.004281503682, .001847646883,
    .000230385764, -.000251963189, .000039347320
  },

  {
    .026670057901, .188176800078, .527201188932, .688459039454, .281172343661,
    -.249846424327, -.195946274377, .127369340336, .093057364604, -.071394147166,
    -.029457536822, .033212674059, .003606553567, -.010733175483, .001395351747,
    .001992405295, -.000685856695, -.000116466855, .000093588670, -.000013264203
  }
};

/*************************************************************************
 *\�������ƣ�
 *  FloatToChar()
 *
 *\�������:
 *  Float  f            - ����˫���ȱ���
 *
 *\����ֵ:
 *  Char                 - �����ַ�����
 *
 *\˵��:
 *  �ú����������˫���ȱ���ת��ΪChar�͵ı���
 *************************************************************************
 */
char FloatToChar(Float f)
{
  if (f >= 0)
    if (f >= 127.0) {
      return (char) 127;
    }
    else {
      return (char)(f + 0.5);
    }
  else if (f <= -128) {
    return (char) - 128;
  }
  else {
    return -(char)(-f + 0.5);
  }
}

/*************************************************************************
 *\�������ƣ�
 *  FloatToByte()
 *
 *\�������:
 *  Float  f            - ����˫���ȱ���
 *
 *\����ֵ:
 *  BYTE            - ���ر����ͱ���
 *
 *\˵��:
 *  �ú����������˫���ȱ���ת��ΪBYTE�͵ı���
 *************************************************************************
 */
BYTE FloatToByte(Float f)
{
  if (f <= 0) {
    return (BYTE) 0;
  }
  else if (f >= 255) {
    return (BYTE) 255;
  }
  else {
    return (BYTE)(f + 0.5);
  }
}

/*************************************************************************
 *
 *\�������ƣ�
 *  DWTStep_1D()
 *
 *\�������:
 *  Float *pDbSrc  - ָ��Դ���ݵ�ָ��
 *  int nCurLevel  - ��ǰ�ֽ�Ĳ���
 *  int nInv    - �Ƿ�ΪDWT��1��ʾΪIDWT��0��ʾDWT
 *  int nStep   - ��ǰ�ļ������
 *  int nSupp   - С�����Ľ�֧���ĳ���
 *
 *\����ֵ:
 *  BOOL   - �ɹ��򷵻�TRUE�����򷵻�FALSE
 *
 *\˵��:
 *  �ú����öԴ����pDBSrc�е����ݽ���һ���һάDWT����IDWT�����У�nInvΪ��ʾ����
 *  DWT����IDWT�ı�־��nCurLevelΪ��ǰ��Ҫ���зֽ�Ĳ�����nStepΪ�Ѿ��ֽ�Ĳ���
 *  ����������Դ����pDbSrc��
 *
 *************************************************************************
 */
BOOL DWTStep_1D(Float* pDbSrc, int nCurLevel, int nInv, int nStep, int nSupp)
{
  int i, j, Index1, Index2;
  Float* ptemp, s1, s2, s = sqrt(2);
  // ���С������ָ��
  Float* h = (Float*) hCoef[ nSupp - 1 ];

  // ���㵱ǰ�����ĳ���
  int CurN = 1 << nCurLevel;

  if (nInv) {
    CurN <<= 1;
  }

  // ȷ�ϵ�ǰ������Ч
  ASSERT(nCurLevel >= 0);

  // ȷ����ѡ���С�����͵�ǰ�����ĳ�����Ч
  if (nSupp < 1 || nSupp > 10 || CurN < 2 * nSupp) {
    return FALSE;
  }

  // ������ʱ�ڴ����ڴ�Ž��
  ptemp = MALLOC(Float, CurN);

  if (!ptemp) {
    return FALSE;
  }


  // �ж��ǽ���DWT����IDWT
  if (!nInv) {   // DWT
    Index1 = 0;
    Index2 = 2 * nSupp - 1;

    // ���о��������s1Ϊ��Ƶ���֣�s2Ϊ��Ƶ���ֵĽ��
    for (i = 0; i < CurN / 2; i++) {
      Float t = -1;
      s1 = s2 = 0;

      for (j = 0; j < 2 * nSupp; j++, t = -t) {
        s1 += h[ j ] * pDbSrc[(Index1 & CurN - 1) * nStep ];
        s2 += t * h[ j ] * pDbSrc[(Index2 & CurN - 1) * nStep ];

        Index1++;
        Index2--;
      }

      // ������������ʱ�ڴ���
      ptemp[ i ] = s1 / s;
      ptemp[ i + CurN / 2 ] = s2 / s;

      Index1 -= 2 * nSupp;
      Index2 += 2 * nSupp;
      Index1 += 2;
      Index2 += 2;
    }
  }

  // �������IDWT
  else { // IDWT
    Index1 = CurN / 2;
    Index2 = CurN / 2 - nSupp + 1;

    // ���о������������s1Ϊ��Ƶ���֣�s2Ϊ��Ƶ���ֵĽ��
    for (i = 0; i < CurN / 2; i++) {
      int Index3 = 0;
      s1 = s2 = 0;

      for (j = 0; j < nSupp; j++) {
        double sss = pDbSrc[(Index1 & CurN / 2 - 1) * nStep ];
        s1 += h[ Index3 ] * pDbSrc[(Index1 & CurN / 2 - 1) * nStep ]
            + h[ Index3 + 1 ] * pDbSrc[((Index2 + CurN / 2) & CurN / 2 - 1) * nStep ];
        s2 += h[ Index3 + 1 ] * pDbSrc[(Index1 & CurN / 2 - 1) * nStep ]
            - h[ Index3 ] * pDbSrc[((Index2 + CurN / 2) & CurN / 2 - 1) * nStep ];

        Index3 += 2;
        Index1--;
        Index2++;
      }

      // �����������ʱ�ڴ�
      ptemp[ 2 * i ] = s1 * s;
      ptemp[ 2 * i + 1 ] = s2 * s;

      Index1 += nSupp;
      Index2 -= nSupp;
      Index1++;
      Index2++;
    }
  }

  // ���������Դͼ����
  for (i = 0; i < CurN; i++) {
    pDbSrc[ i * nStep ] = ptemp[ i ];
  }

  // �ͷ���ʱ�ڴ棬������
  SAFEFREE(ptemp);
  return TRUE;
}

/*************************************************************************
 *
 *\�������ƣ�
 *  DWTStep_2D()
 *
 *\�������:
 *  Float *pDbSrc  - ָ��Դ���ݵ�ָ��
 *  int nCurWLevel  - X�����ϵ�ǰ�ֽ�Ĳ���
 *  int nCurHLevel  - Y�����ϵ�ǰ�ֽ�Ĳ���
 *  int nMaxWLevel  - X���������ɷֽ�Ĳ���
 *  int nMaxHLevel  - Y���������ɷֽ�Ĳ���
 *  int nInv   - �Ƿ�ΪDWT��1��ʾΪIDWT��0��ʾDWT
 *  int nStep   - ��ǰ�ļ������
 *  int nSupp   - С�����Ľ�֧���ĳ���
 *
 *\����ֵ:
 *  BOOL   - �ɹ��򷵻�TRUE�����򷵻�FALSE
 *
 *\˵��:
 *  �ú����öԴ����pDBSrc�е����ݽ���һ��Ķ�άDWT����IDWT��
 *  ����������Դ����pDbSrc��
 *
 *************************************************************************
 */
BOOL DWTStep_2D(Float* pDbSrc, int nCurWLevel, int nCurHLevel,
    int nMaxWLevel, int nMaxHLevel, int nInv, int nStep, int nSupp)
{
  // ����ͼ��ĳ��ȺͿ�ȣ�2���ݶ��룩
  int i, W = 1 << nMaxWLevel, H = 1 << nMaxHLevel;

  // ���㵱ǰ�ֽ��ͼ��ĳ��ȺͿ��
  int CurW = 1 << nCurWLevel, CurH = 1 << nCurHLevel;

  // �ж��ǽ���DWT����IDWT
  if (!nInv) {   // ���н���һάDWT
    for (i = 0; i < CurH; i++)
      if (!DWTStep_1D(pDbSrc + (int) i * W * nStep, nCurWLevel, nInv, nStep, nSupp)) {
        return FALSE;
      }

    // ���н���һάDWT
    for (i = 0; i < CurW; i++)
      if (!DWTStep_1D(pDbSrc + i * nStep, nCurHLevel, nInv, W * nStep, nSupp)) {
        return FALSE;
      }
  }

  // �������IDWT
  else {
    // ���㵱ǰ�任��ͼ��ĳ��ȺͿ��
    CurW <<= 1;
    CurH <<= 1;

    // ���н���IDWT
    for (i = 0; i < CurW; i++)
      if (!DWTStep_1D(pDbSrc + i * nStep, nCurHLevel, nInv, W * nStep, nSupp)) {
        return FALSE;
      }

    // ���н���IDWT
    for (i = 0; i < CurH; i++)
      if (!DWTStep_1D(pDbSrc + (int) i * W * nStep, nCurWLevel, nInv, nStep, nSupp)) {
        return FALSE;
      }
  }

  // ����
  return TRUE;
}

/*************************************************************************
 *\�������ƣ�
 *  DWT_1D()
 *
 *\�������:
 *  Float *pDbSrc  - ָ��Դ���ݵ�ָ��
 *  int nMaxLevel  - ���ɷֽ�Ĳ���
 *  int nDWTSteps  - ��Ҫ�ֽ�Ĳ���
 *  int nInv   - �Ƿ�ΪDWT��1��ʾΪIDWT��0��ʾDWT
 *  int nStep   - ��ǰ�ļ������
 *  int nSupp   - С�����Ľ�֧���ĳ���
 *
 *\����ֵ:
 *  BOOL   - �ɹ��򷵻�TRUE�����򷵻�FALSE
 *
 *\˵��:
 *  �ú����öԴ����pDBSrc�е����ݽ���һάDWT����IDWT�����У�nInvΪ��ʾ����
 *  DWT����IDWT�ı�־��nStepΪ��ǰ�Ѿ��ֽ�Ĳ���������������Դ����pDbSrc��
 **************************************************************************/
BOOL DWT_1D(Float* pDbSrc, int nMaxLevel,
    int nDWTSteps, int nInv, int nStep, int nSupp)
{
  // ������С�ɷֽ�Ĳ���
  int MinLevel = nMaxLevel - nDWTSteps;

  // �ж��Ƿ�ΪDWT
  if (!nInv) {   // DWT
    int n = nMaxLevel;

    while (n > MinLevel)

      // ����DWTStep_1D���е�n���DWT
      if (!DWTStep_1D(pDbSrc, n--, nInv, nStep, nSupp)) {
        return FALSE;
      }
  }

  // nInvΪ1�����IDWT
  else { // IDWT
    int n = MinLevel;

    while (n < nMaxLevel)

      // ����DWTStep_1D���е�n���IDWT
      if (!DWTStep_1D(pDbSrc, n++, nInv, nStep, nSupp)) {
        return FALSE;
      }
  }

  return TRUE;
}

/*************************************************************************
 *
 *\�������ƣ�
 *  DWT_2D()
 *
 *\�������:
 *  Float *pDbSrc  - ָ��Դ���ݵ�ָ��
 *  int nMaxWLevel  - X�������ɷֽ�Ĳ���
 *  int nMaxHLevel  - Y�������ɷֽ�Ĳ���
 *  int nDWTSteps  - ��Ҫ�ֽ�Ĳ���
 *  int nInv   - �Ƿ�ΪDWT��1��ʾΪIDWT��0��ʾDWT
 *  int nStep   - ��ǰ�ļ������
 *  int nSupp   - С�����Ľ�֧���ĳ���
 *
 *\����ֵ:
 *  BOOL   - �ɹ��򷵻�TRUE�����򷵻�FALSE
 *
 *\˵��:
 *  �ú����öԴ����pDBSrc�еĶ�ά���ݽ��ж�άDWT����IDWT�����У�nDWTSteps��ʾ
 *  ��Ҫ�ֽ�Ĳ�����nInvΪ��ʾ����DWT����IDWT�ı�־��nStepΪ��ǰ�Ѿ��ֽ�Ĳ���
 *  ����������Դ����pDbSrc��
 *
 *************************************************************************
 */
BOOL DWT_2D(Float* pDbSrc, int nMaxWLevel, int nMaxHLevel,
    int nDWTSteps, int nInv, int nStep, int nSupp)
{
  // ����X��Y��������С�ķֽ����
  int MinWLevel = nMaxWLevel - nDWTSteps;
  int MinHLevel = nMaxHLevel - nDWTSteps;

  // �ж��ǽ���DWT������IDWT
  if (!nInv) {   // DWT
    int n = nMaxWLevel, m = nMaxHLevel;

    // ����DWTStep_2D���зֽ⣬�ֽ�Ĳ���ΪnDWTSteps
    while (n > MinWLevel)
      if (!DWTStep_2D(pDbSrc, n--, m--, nMaxWLevel, nMaxHLevel, nInv, nStep, nSupp)) {
        return FALSE;
      }
  }

  // �������IDWT
  else { // IDWT
    int n = MinWLevel, m = MinHLevel;

    // ����DWTStep_2D����IDWT�����лָ��Ĳ���ΪnDWTSteps
    while (n < nMaxWLevel)
      if (!DWTStep_2D(pDbSrc, n++, m++, nMaxWLevel, nMaxHLevel, nInv, nStep, nSupp)) {
        return FALSE;
      }
  }

  // ����
  return TRUE;
}

/*************************************************************************
 * \�������ƣ�  Log2()
 * \�������:
 *   int n            - �������ͱ���
 *
 * \����ֵ:
 *  int           - ������������Ķ���
 *
 * \˵��:
 *    �ú�����ȡ�����������2Ϊ�׵Ķ�������ת��Ϊ���������
 *************************************************************************
 */
int Log2(int n)
{
  int rsl = 0;

  while (n >>= 1) {
    rsl++;
  }

  return rsl;
}

BOOL DIBDWTStep(LPBYTE lpDIBBits, Float* m_pDbImage, int lWidth, int lHeight, int nInv, int m_nDWTCurDepth, int m_nSupp)
{
  // ѭ������
  int i, j;

  // ��ȡ�任��������
  int nMaxWLevel = Log2(lWidth);
  int nMaxHLevel = Log2(lHeight);
  int nMaxLevel, lfw, lfh;
  // ��ʱ����
  Float* pDbTemp;
  BYTE* pBits;

  if (lWidth == 1 << nMaxWLevel && lHeight == 1 << nMaxHLevel) {
    nMaxLevel = MIN(nMaxWLevel, nMaxHLevel);
  }

  // ���С���任�Ĵ洢�ڴ滹û�з��䣬�������ڴ�
  if (!m_pDbImage) {
    m_pDbImage = MALLOC(Float, lWidth * lHeight);

    if (!m_pDbImage) {
      return FALSE;
    }

    // ��ͼ�����ݷ���m_pDbImage��
    for (j = 0; j < lHeight; j++) {
      pDbTemp = m_pDbImage + j * lWidth;
      pBits = (LPBYTE) lpDIBBits + (lHeight - 1 - j) * lWidth;

      for (i = 0; i < lWidth; i++) {
        pDbTemp[ i ] = pBits[ i ];
      }
    }
  }

  // ����С���任���򷴱任��
  if (!DWTStep_2D(m_pDbImage, nMaxWLevel - m_nDWTCurDepth, nMaxHLevel - m_nDWTCurDepth,
      nMaxWLevel, nMaxHLevel, nInv, 1, m_nSupp)) {
    return FALSE;
  }

  // ����Ƿ��任����ǰ������1
  if (nInv) {
    m_nDWTCurDepth --;
  }
  // �����1
  else {
    m_nDWTCurDepth ++;
  }

  // Ȼ�󣬽����ݿ�����ԭCDib�У���������Ӧ������ת��
  lfw = lWidth >> m_nDWTCurDepth;
  lfh = lHeight >> m_nDWTCurDepth;

  for (j = 0; j < lHeight; j++) {
    pDbTemp = m_pDbImage + j * lWidth;
    pBits = (LPBYTE) lpDIBBits + (lHeight - 1 - j) * lWidth;

    for (i = 0; i < lWidth; i++) {
      if (j < lfh && i < lfw) {
        pBits[ i ] = FloatToByte(pDbTemp[ i ]);
      }
      else {
        pBits[ i ] = (BYTE)(FloatToChar(pDbTemp[ i ]) ^ 0x80);
      }
    }
  }

  // ����
  return TRUE;
}


/*************************************************************************
 *
 * \�������ƣ�
 *    ImageDWT()
 *
 * \�������:
 *    CDib *pDibSrc  - ָ��Դ���ݵ�ָ��
 *    int nMaxWLevel  - X���������ɷֽ�Ĳ���
 *    int nMaxHLevel  - Y���������ɷֽ�Ĳ���
 *    int nDWTSteps  - ��Ҫ���б任�Ĳ���
 *    int nInv   - �Ƿ�ΪDWT��1��ʾΪIDWT��0��ʾDWT
 *    int nStep   - ��ǰ�ļ������
 *    int nSupp   - С�����Ľ�֧���ĳ���
 *
 * \����ֵ:
 *    BOOL   - �ɹ��򷵻�TRUE�����򷵻�FALSE
 *
 * \˵��:
 *    �ú����öԴ����pDBSrc�е����ݽ���һ��Ķ�άDWT����IDWT��
 *    ����������Դ����pDbSrc��
 *
 **************************************************************************/
BOOL ImageDWT(LPBYTE lpImage, int nMaxWLevel, int nMaxHLevel,
    int nDWTSteps, int nInv, int nStep, int nSupp)
{
  // ���X��Y�����ϵ������������2���ݶ��룩
  int W = 1 << nMaxWLevel, H = 1 << nMaxHLevel;

  // ���X��Y�����ϱ任ʱ��С��������
  int minW = W >> nDWTSteps, minH = H >> nDWTSteps;

  int i, j, index;

  // ������ʱ�ڴ��Ž��
  Float* pDbTemp;

  // �жϱ任�Ĳ����Լ���ǰ�����Ƿ���Ч
  if (nDWTSteps > nMaxWLevel || nDWTSteps > nMaxHLevel || nStep <= 0) {
    return FALSE;
  }

  pDbTemp = MALLOC(Float, W * H);

  if (!pDbTemp) {
    return FALSE;
  }

  // �ж��ǽ���DWT����IDWT��Ȼ�����ݴ�ŵ���ʱ�ڴ��У���Ҫע����ǣ���Ҫ���в���
  if (!nInv)    // DWT
    for (index = 0; index < W * H; index++) {
      pDbTemp[ index ] = lpImage[ index * nStep ];
    }
  else { // IDWT
    index = 0;

    for (i = 0; i < minH; i++) {
      for (j = 0; j < minW; j++, index++) {
        pDbTemp[ index ] = lpImage[ index * nStep ];
      }

      for (; j < W; j++, index++) {
        pDbTemp[ index ] = (char) lpImage[ index * nStep ];
      }
    }

    for (; index < W * H; index++) {
      pDbTemp[ index ] = (char) lpImage[ index * nStep ];
    }
  }

  // ����DWT_2D����С���任
  if (!DWT_2D(pDbTemp, nMaxWLevel, nMaxHLevel, nDWTSteps, nInv, nStep, nSupp)) {
    SAFEFREE(pDbTemp);
    return FALSE;
  }

  // �����ݴ���ԭʼ���ڴ��У���Ҫע����ǣ��洢ʱ��Ҫ��������ת��
  if (!nInv) {   // DWT
    index = 0;

    for (i = 0; i < minH; i++) {
      for (j = 0; j < minW; j++, index++) {
        lpImage[ index * nStep ] = FloatToByte(pDbTemp[ index ]);
      }

      for (; j < W; j++, index++) {
        lpImage[ index * nStep ] = (BYTE) FloatToChar(pDbTemp[ index ]);
      }

      // lpImage[index*nStep] = (BYTE)FloatToByte(pDbTemp[index]);
    }

    for (; index < W * H; index++) {
      lpImage[ index * nStep ] = (BYTE) FloatToChar(pDbTemp[ index ]);
    }

    //lpImage[index*nStep] = (BYTE)FloatToByte(pDbTemp[index]);
  }
  else    // IDWT
    for (index = 0; index < W * H; index++) {
      lpImage[ index * nStep ] = FloatToByte(pDbTemp[ index ]);
    }

  // �ͷ��ڴ�
  SAFEFREE(pDbTemp);

  // ����
  return TRUE;
}

// int lHeight, int lWidth ͼ��ĳ��ȺͿ��
BOOL DWTStep(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nInv)
{
  // ѭ������
  int i, j;
  // ��ʱ����
  double* m_pDbImage = NULL, * pDbTemp;
  BYTE* pBits;
  int lfw, lfh;
  int m_nSupp;
  int m_nDWTCurDepth;
  int lDbLineBytes;

  // ��ȡ�任��������
  int nMaxWLevel = Log2(lWidth);
  int nMaxHLevel = Log2(lHeight);
  int nMaxLevel;

  if (lWidth == 1 << nMaxWLevel && lHeight == 1 << nMaxHLevel) {
    nMaxLevel = MIN(nMaxWLevel, nMaxHLevel);
  }

  // ���õ�ǰ����
  m_nDWTCurDepth = 0;

  // ����С������֧������
  m_nSupp = 1;

  // ���С���任�Ĵ洢�ڴ滹û�з��䣬�������ڴ�
  //if(!m_pDbImage)
  {
    if (nInv) {
      lDbLineBytes = 2 * lWidth;
      m_pDbImage = MALLOC(double, 4 * lWidth * lHeight);
    }
    else {
      lDbLineBytes = lWidth;
      m_pDbImage = MALLOC(double, lWidth * lHeight);
    }

    if (!m_pDbImage) {
      return FALSE;
    }

    // ��ͼ�����ݷ���m_pDbImage��
    for (j = 0; j < lHeight; j++) {
      pDbTemp = m_pDbImage + j * lDbLineBytes;
      pBits = lpDIBBits + (lHeight - 1 - j) * lLineBytes;

      for (i = 0; i < lWidth; i++) {
        pDbTemp[ i ] = pBits[ i ];
      }
    }
  }

  // ����С���任���򷴱任��
  if (!DWTStep_2D(m_pDbImage, nMaxWLevel - m_nDWTCurDepth, nMaxHLevel - m_nDWTCurDepth,
      nMaxWLevel, nMaxHLevel, nInv, 1, m_nSupp)) {
    return FALSE;
  }

  // ����Ƿ��任����ǰ������1
  if (nInv) {
    m_nDWTCurDepth --;
  }
  // �����1
  else {
    m_nDWTCurDepth ++;
  }

  // Ȼ�󣬽����ݿ�����ԭCDib�У���������Ӧ������ת��
  lfw = lWidth >> m_nDWTCurDepth;
  lfh = lHeight >> m_nDWTCurDepth;

  for (j = 0; j < lHeight; j++) {
    pDbTemp = m_pDbImage + j * lDbLineBytes;
    pBits = lpDIBBits + (lHeight - 1 - j) * lLineBytes;

    for (i = 0; i < lWidth; i++) {
      if (j < lfh && i < lfw) {
        pBits[ i ] = FloatToByte(pDbTemp[ i ]);
      }
      else {
        pBits[ i ] = (BYTE)(FloatToChar(pDbTemp[ i ]) ^ 0x80);
      }
    }
  }

  SAFEFREE(m_pDbImage);
  // ����
  return TRUE;
}
