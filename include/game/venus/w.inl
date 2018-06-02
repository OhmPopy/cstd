// ****************************************************************************************************
// �ļ�: alpha.cpp
// ע��:
// A*Ѱ·�㷨��ģ��, ���Ա����ʵ��
// ****************************************************************************************************
//#include <stdlib.h>
//#include <malloc.h>
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
#include "debug.h"
using namespace Valiant::Gaming;
using namespace Valiant;
CMaze::CMaze()
{
  this->m_pMaze = NULL;
}
CMaze::~CMaze()
{
  if (this->m_pMaze) {
    fnAlphaExit(m_pMaze, 0);
  }
}
int CMaze::Create(long dwSizeX, long dwSizeY, long dwMaxCost, UINT dwFlags)
{
  PMAZE pMaze = fnAlphaInit(dwSizeX, dwSizeY, dwMaxCost, dwFlags);
  if (pMaze == NULL) {
    return 0;
  }
  if (m_pMaze) {
    fnAlphaExit(m_pMaze, 0);
  }
  m_pMaze = pMaze;
  return 1;
}
int CMaze::Edit(long dwPosX, long dwPosY, void* lpValue)
{
  return fnAlphaEdit(m_pMaze, dwPosX, dwPosY, lpValue);
}
int CMaze::Parse(PSPOT lpStart, PSPOT lpTarget, long* pdwStep)
{
  return fnAlphaStar(m_pMaze, lpStart, lpTarget, pdwStep);
}
int CMaze::Query(void* lpVoid, UINT dwPropId, long dwIndex)
{
  return fnAlphaProp(m_pMaze, dwPropId, dwIndex);
}

