// ****************************************************************************************************
// �ļ�: box_query.cpp
// ע��:
// �����ӳ�����ѯ����ģ��, ��ѯ�����й����Լ���Ա
// ****************************************************************************************************
#include <malloc.h> // malloc, realloc
#include <stdio.h> // FILE
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
// �������ƻ�ȡƫ����(����32λ��Ա)
long fnStageIndex(void* lpVoid)
{
  return 0;
}
int fnStageEdit(PSTAGE pStage, void* lpVoid, long dwPosX, long dwPosY)
{
  // �༭��ͼ��Ԫ����ȫ������
  if (pStage == NULL) {
    fnStageCode(SEC_PARAM_NULL);
    return 0;
  }
  if (lpVoid == NULL) {
    return pStage->Matrix[dwPosY * pStage->SizeX + dwPosX]; // ��ȡ��Ԫ�ߴ�
  }
  // ���þ�������(�Զ�תΪ�½�����)
  if (dwPosX < 0 || dwPosX >= pStage->SizeX) {
    fnStageCode(SEC_PARAM_MANY);
    return -1;
  }
  if (dwPosY < 0 || dwPosY >= pStage->SizeY) {
    fnStageCode(SEC_PARAM_MANY);
    return -2;
  }
  pStage->Matrix[dwPosY * pStage->SizeX + dwPosX] = *(BYTE*)lpVoid;
  pStage->Flags = SGF_PRIME;
  return 1; // ����
}
int fnStageQuery(PSTAGE pStage, void* lpVoid, long dwPropId, UINT dwFlags)
{
  PROP prp;
  union {
    PMOVE pMove;
    PSPOT pPos;
    BYTE* pNum;
  };
  union {
    UINT dwLoop;
    int dwRet;
  };
  if (pStage == NULL) {
    fnStageCode(SEC_PARAM_NULL);
    return 0;
  }
  // ��ȡ��ǰ�ƶ�·��(�������Զ���Ѱ·�㷨)
  switch (dwPropId) {
  case SPF_SAFE_CHECK: // ���ָ��
    try {
      pMove = &pStage->Moves[pStage->Count - 1];
    }
    catch (...) { // EXCEPTION_EXECUTE_HANDLER
      return -1; // ָ����Ч, �����쳣
    }
    return 1;
  case SPF_UNIT_VALUE: // ����ָ��λ����������
    pPos = (PSPOT)lpVoid;
    dwLoop = pStage->Count;
    while (dwRet >= 0) {
      if (pStage->Stars[dwLoop].Value == pPos->Value) {
        break;
      }
      dwRet--;
    }
    return dwRet;
  case SPF_LAST_ERROR: // ���Ĵ�����
    dwLoop = fnStageCode(SEC_ERROR_DUMP);
    *(UINT*)lpVoid = dwLoop;
    return 2;
  case SPF_PREV_STAGE: // ������һ������(����, ɾ�����������Ժ�ĳ���)
    if (pStage->Prev == NULL) {
      return 0;
    }
    *(PSTAGE*)lpVoid = pStage->Prev;
    fnStageErase(pStage, 0);
    return 1;
  case SPF_NEXT_STAGE: // ������һ������(�ƶ�)
    if (pStage->Next) {
      fnStageErase(pStage->Next, 0); // ����, ���ͷ�
    }
    pStage->Next = (PSTAGE)malloc(pStage->Volume);
    if (pStage->Next == NULL) {
      return 0;
    }
    V32Copy(pStage->Next, pStage, pStage->Volume);
    pStage->Next->Prev = pStage;
    pStage->Next->Next = NULL; // ...
    *(PSTAGE*)lpVoid = pStage->Next;
    return 1;
  case SPF_HEAP_STAGE: // ѹ������, ��һ����������4K, ɾ��������������, realloc
    return 0;
  case SPF_CALC_STAGE: // ���㳡������(�½�������ȡ���λ��, ����������, ��������������)
    if (pStage->Flags & SGF_BASIC) {
      return -100; // �������������Ѿ�����
    }
    //V32Clear32(&prp, 0, sizeof(prp));
    prp.Flags = 0; // ���践����������
    prp.SizeX = pStage->SizeX;
    prp.SizeY = pStage->SizeY;
    dwRet = fnStageProp(pStage->Matrix, &prp);
    if (dwRet <= 0) {
      return dwRet; // ������Ч
    }
    pStage->Count = prp.Count;
    pStage->Value = prp.Value;
    pStage->Hash = prp.Hash;
    pStage->Position.Value = prp.Role.Value;
    pStage->Flags |= SGF_BASIC; // ��������
    return 1;
  case SPF_PUSH_STAGE: // �ƶ�����
    return fnStageProbe(pStage, *(UINT*)lpVoid);
  case SPF_PATH_STAGE: // �Զ�Ѱ·
    return fnStageStars(pStage, (PSPOT)lpVoid);
  default: // ֱ�Ӷ�λ��Ա
    if (lpVoid == NULL) {
      return -2;
    }
    pNum = (BYTE*)pStage;
    pNum += dwPropId;
    //V32Copy(lpVoid, pNum, sizeof(UINT)); // ���UINT����
    *(PUINT)lpVoid = *pNum;
    break;
  }
  return sizeof(STAGE);
}

