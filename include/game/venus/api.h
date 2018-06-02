#ifndef __VENUS_H_
#define __VENUS_H_

//#include "v32_api.h"
#include "vrc32_api.h"    // ָ�ƿ�

#ifdef __VENUS_DLL_INC_
#define __V32_DLL_API_      // �ڲ������ʶ
#endif
#include "v32_prefix.h"   // Ԥ�����, �л����ⲿ����

// auto include: box.h require alpha.h
#include "alpha.h"
#include "box.h"

// array item address
#define ArrayItem2(a, w, x, y)    a[y * w + x]

namespace Valiant
{
namespace Gaming
{
class VFC CMaze  //public tagMaze,
{
public:
  CMaze();
  virtual ~CMaze();

  int Create(long dwSizeX, long dwSizeY, long dwMaxCost, UINT dwFlags);
  int Edit(long dwPosX, long dwPosY, void* lpValue);
  int Parse(PSPOT lpStart, PSPOT lpTarget, long* pdwStep);
  int Query(void* lpVoid, UINT dwPropId, long dwIndex);
protected:
  PMAZE m_pMaze;
};

class VFC CPorter  //public tagStage,
{
public:
  CPorter();
  CPorter(PSTAGE pStage);
  CPorter(CPorter* pPorter);
  virtual ~CPorter();

  int Open(void* lpVoid, UINT dwFlags); // ���ļ�
  int Apply(void* lpVoid, long dwSizeX, long dwSizeY);  // �����½�
  int Query(void* lpVoid, long dwPropId, UINT dwFlags); // ��ѯ����
  int Edit(void* lpVoid, long dwPosX, long dwPosY); // �༭����
  int Solve(void* lpVoid, UINT dwCount, UINT dwFlags);  // ��ⳡ��
  int Probe(void* lpVoid, UINT dwFlags);  // ����ƶ�
  int Save(void* lpVoid, UINT dwFlags); // д���ļ�
protected:
  PSTAGE m_pStage;
};
}
}

// C++ not support currently
#ifdef __cplusplus
#endif

#include "v32_subfix.h"   // Ԥ�����, �л����ⲿ����

#endif