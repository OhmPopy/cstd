// ****************************************************************************************************
// �ļ�: api.cpp
// ע��:
// ���ӿ����ģ��, �����������ʵ��
// ****************************************************************************************************
#include <windows.h>
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
#include "debug.h"
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
    break;
  case DLL_PROCESS_DETACH:
    fnFreeScene(); // �ͷſ��ܴ�����Ѱ·����(��������Ϸ)
    break;
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break; // multiple label same address
  }
  return TRUE;
}
// ****************************************************************************************************
// Alpha star path finding
// ****************************************************************************************************
V32API PMAZE AlphaInit(long dwSizeX, long dwSizeY, long dwMaxCost, UINT dwFlags)
{
  return fnAlphaInit(dwSizeX, dwSizeY, dwMaxCost, dwFlags);
}
V32API int AlphaEdit(PMAZE pMaze, long dwPosX, long dwPosY, void* lpValue)
{
  return fnAlphaEdit(pMaze, dwPosX, dwPosY, lpValue);
}
V32API int AlphaStar(PMAZE pMaze, PSPOT lpStart, PSPOT lpTarget, long* pdwStep)
{
  return fnAlphaStar(pMaze, lpStart, lpTarget, pdwStep);
}
V32API int AlphaProp(PMAZE pMaze, UINT dwPropId, long dwIndex)
{
  return fnAlphaProp(pMaze, dwPropId, dwIndex);
}
V32API int AlphaExit(PMAZE pMaze, UINT dwFlags)
{
  return fnAlphaExit(pMaze, dwFlags);
}
// ****************************************************************************************************
// Sokoban Automatic Solver
// ****************************************************************************************************
V32API PSTAGE StageInit(void* lpVoid, PSPOT pSpot, UINT dwFlags)
{
  if (pSpot == NULL) {
    return fnStageOpen(lpVoid, dwFlags); // ���ļ�����(�ߴ���ļ���ȡ)
  }
  return fnStageApply(lpVoid, pSpot, dwFlags); // ���ڴ洴��(�ߴ������Ч)
}
V32API int StageEdit(PSTAGE pStage, void* lpVoid, long dwPosX, long dwPosY)
{
  return fnStageEdit(pStage, lpVoid, dwPosX, dwPosY);
}
V32API int StageMove(PSTAGE pStage, void* lpVoid, UINT dwCount, UINT dwFlags)
{
  return fnStageSolve(pStage, lpVoid, dwCount, dwFlags);
}
V32API int StageProp(PSTAGE pStage, void* lpVoid, long dwPropId, UINT dwFlags)
{
  return fnStageQuery(pStage, lpVoid, dwPropId, dwFlags);
}
V32API int StageSave(PSTAGE pStage, void* lpVoid, UINT dwFlags)
{
  return fnStageSave(pStage, lpVoid, dwFlags);
}
V32API int StageExit(PSTAGE pStage, UINT dwFlags)
{
  return fnStageErase(pStage, dwFlags);
}

