// ****************************************************************************************************
// �ļ�: box.cpp
// ע��:
// ��������ģ��, ���Ա����ʵ��
// ****************************************************************************************************
//#include <malloc.h> // malloc, realloc, free
//#include <stdio.h> // FILE
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant::Gaming;
using namespace Valiant;
CPorter::CPorter()
{
  this->m_pStage = NULL;
}
CPorter::CPorter(PSTAGE pStage)
{
  this->m_pStage = pStage;
}
CPorter::CPorter(CPorter* pPorter)
{
  this->m_pStage = pPorter->m_pStage; // ��Ա�������Է���ͬ�ౣ����Ա
}
CPorter::~CPorter()
{
  if (this->m_pStage) {
    fnStageErase(this->m_pStage, 0);
  }
}
int CPorter::Open(void* lpVoid, UINT dwFlags)
{
  PSTAGE pStage = fnStageOpen(lpVoid, dwFlags);
  if (pStage == NULL) {
    return 0;
  }
  if (m_pStage) {
    fnStageErase(m_pStage, 0);
  }
  m_pStage = pStage;
  return 1;
}
int CPorter::Apply(void* lpVoid, long dwSizeX, long dwSizeY)
{
  PSTAGE pStage;
  SPOT spt;
  spt.X = dwSizeX;
  spt.Y = dwSizeY;
  pStage = fnStageApply(lpVoid, &spt, 0); // default flags
  if (pStage == NULL) {
    return 0;
  }
  if (m_pStage) {
    fnStageErase(m_pStage, 0);
  }
  m_pStage = pStage;
  return 1;
}
int CPorter::Query(void* lpVoid, long dwPropId, UINT dwFlags)
{
  return fnStageQuery(m_pStage, lpVoid, dwPropId, dwFlags);
}
int CPorter::Edit(void* lpVoid, long dwPosX, long dwPosY)
{
  return fnStageEdit(m_pStage, lpVoid, dwPosX, dwPosY);
}
int CPorter::Solve(void* lpVoid, UINT dwCount, UINT dwFlags)
{
  return fnStageSolve(m_pStage, lpVoid, dwCount, dwFlags);
}
int CPorter::Probe(void* lpVoid, UINT dwFlags)
{
  return fnStageProbe(m_pStage, dwFlags);
}
int CPorter::Save(void* lpVoid, UINT dwFlags)
{
  return fnStageSave(m_pStage, lpVoid, dwFlags);
}

