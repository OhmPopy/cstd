// ****************************************************************************************************
// �ļ�: box_stage.cpp
// ע��:
// �����ӻ�������ģ��, �����ߴ����, ָ������, ��������, �ļ����ؼ��ڴ������ͷ�
// ****************************************************************************************************
#include <malloc.h> // malloc, realloc
#include <stdio.h> // FILE
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
UINT fnStageSize(UINT dwCount, UINT dwSizeX, UINT dwSizeY)
{
  UINT dwSize;
  dwSize = sizeof(STAGE) - sizeof(UINT); // �ṹ�ߴ�
  dwSize += dwSizeX * dwSizeY; // ��Ԫ����
  dwSize += sizeof(SPOT) * dwCount * 2; // ��������
  dwSize += sizeof(MOVE) * dwCount * 4; // �����߷�
  return dwSize;
}
void fnStagePtr(PSTAGE pStage)
{
  UINT dwNum = pStage->SizeX * pStage->SizeY;
  // �����ڲ�ָ��
  pStage->Stars = (PSPOT)&pStage->Matrix[dwNum]; // ��������, ָ���ڲ�
  dwNum = pStage->Count;
  pStage->Series = &pStage->Stars[dwNum]; // ��������, ָ���ڲ�
  pStage->Moves = (PMOVE)&pStage->Series[dwNum]; // �����߷�, ָ���ڲ�
  return;
}
int fnStageSort(PSPOT pStar1, PSPOT pStar2, PSPOT pCache, UINT dwCount)
{
  __int64 nValue;
  union {
    UINT dwLoop1;
    int dwRet;
  };
  UINT dwLoop2;
  //if(dwCount == 0) return 0;
  // ���������������Ƿ���ͬ(������洢pStar1��pCache)
  if (pStar2 == NULL) {
    //if(pCache == NULL) return -1;
    if ((UINT)pStar1 != (UINT)pCache) {
      // ָ�벻ͬ, �ȸ�������
      V32Copy(pCache, pStar1, dwCount * sizeof(SPOT));
    }
    // ����pStar1���洢
    for (dwLoop1 = 0; dwLoop1 < dwCount - 1; dwLoop1++) {
      for (dwLoop2 = dwLoop1 + 1; dwLoop2 < dwCount; dwLoop2++) {
        // ��С��������
        nValue = pCache[dwLoop2].Value;
        if (nValue < pCache[dwLoop1].Value) {
          pCache[dwLoop2].Value = pCache[dwLoop1].Value;
          pCache[dwLoop1].Value = nValue;
        }
      }
    }
    return 1;
  }
  // �Ƚ�pStar1��pStar2
  if (pCache == NULL) {
    // ֱ�ӱȽ�
    dwLoop1 = 0;
    while (dwLoop1 < dwCount) {
      if (pStar1->Value != pStar2->Value) {
        return -2; // �˴˲���
      }
      dwLoop1++;
      pStar1++; // ʹ��ָ��ע��ݽ�
      pStar2++;
    }
    return 2; // ��ȫһ��
  }
  // ����pStar1��pCache����pStar2�Ƚ�
  dwRet = fnStageSort(pStar1, NULL, pCache, dwCount);
  if (dwRet <= 0) {
    return -3;
  }
  return fnStageSort(pCache, pStar2, NULL, dwCount);
}
PBYTE fnStageFile(void* lpVoid, PSPOT pSpot)
{
  BYTE* pData;
  FILE* pfx;
  UINT dwNum;
  int dwRet;
  pfx = fopen((const char*)lpVoid, "rb");
  if (pfx == NULL) {
    fnStageCode(SEC_FILE_OPEN);
    return NULL; // �ļ���ʧ��
  }
  fread(&dwNum, sizeof(UINT), 1, pfx);
  if (dwNum != SFH_HEADER) {
    fclose(pfx);
    fnStageCode(SEC_FILE_SIGN);
    return NULL; // .box�ļ�ͷ��
  }
  dwRet = fread(&dwNum, sizeof(UINT), 1, pfx); // �汾(Ԥ��)
  dwRet = fread(&pSpot->X, sizeof(long), 1, pfx);
  dwRet = fread(&pSpot->Y, sizeof(long), 1, pfx);
  dwNum = pSpot->X * pSpot->Y;
  if (dwNum <= 16 || pSpot->X < 0) {
    fclose(pfx);
    fnStageCode(SEC_FILE_SIZE);
    return NULL; // �������м����ڵĵ�ͼ������
  }
  pData = (PBYTE)malloc(dwNum);
  if (pData == NULL) {
    fclose(pfx);
    fnStageCode(SEC_CACHE_NULL);
    return NULL; // �ڴ治��
  }
  dwRet = fread(pData, sizeof(BYTE), dwNum, pfx);
  fclose(pfx); // �ر��ļ�
  if ((UINT)dwRet != dwNum) {
    SafeFree(pData);
    fnStageCode(SEC_FILE_SIZE);
    return NULL; // ������
  }
  return pData; // pSpot���ֳ����ߴ�
}
// Ҫ���ṩ��Чƥ��ĳ�������
PSTAGE fnStageAlloc(PBYTE pData, PPROP pProp)
{
  PSTAGE pStage;
  UINT dwCount;
  UINT dwSize;
  UINT dwFlags = SGF_PRIME; // ��ʼ����
  if (pProp == NULL) {
    fnStageCode(SEC_PARAM_NULL);
    return NULL; // ��Ч����
  }
  dwCount = 0; // û��ʵ�����굱��û������
  if (pProp->Spots != NULL) {
    dwCount = pProp->Count;
    dwFlags |= SGF_READY; // �����Ѿ���: SGF_BASIC | SGF_EXTRA
  }
  dwSize = fnStageSize(dwCount, pProp->SizeX, pProp->SizeY); // �ڴ��С
  pStage = (PSTAGE)malloc(dwSize);
  if (pStage == NULL) {
    fnStageCode(SEC_CACHE_NULL);
    return NULL; // �ڴ治��
  }
  //V32Clear(pStage, 0, sizeof(STAGE)); // memset, �Զ�����DWORD����(ֻ����ṹ��)
  pStage->Volume = dwSize;
  pStage->Flags = dwFlags;
  pStage->Stars = NULL; // �Ⱥ�����
  pStage->Series = NULL; // �Ⱥ�����
  pStage->Count = dwCount; // ��������
  pStage->Value = pProp->Value;
  pStage->Hash = pProp->Hash;
  pStage->Prev = NULL;
  pStage->Next = NULL;
  pStage->Host = NULL;
  pStage->SizeX = pProp->SizeX;
  pStage->SizeY = pProp->SizeY;
  pStage->PosX = pProp->RoleX;
  pStage->PosY = pProp->RoleY;
  pStage->AutoX = 0;
  pStage->AutoY = 0;
  pStage->Moves = NULL; // �Ⱥ�����
  pStage->Range = 0; // ���޿����߷�
  pStage->Index = 0;
  pStage->Slaves = 0; // ���ӳ���
  pStage->Layer = 0; // ��ʼһ��
  //pStage->Data = 0; // ��������
  if (pData != NULL) {
    V32Copy(pStage->Matrix, pData, pProp->SizeX * pProp->SizeY); // ���ƾ�������
  }
  else {
    V32Clear(pStage->Matrix, 0, pProp->SizeX * pProp->SizeY); // �����������
  }
  if (dwCount != 0) {
    fnStagePtr(pStage); // �����ڲ�ָ��
    V32Copy(pStage->Stars, pProp->Spots, sizeof(SPOT) * dwCount * 2); // ������������(���������)
  }
  return pStage; // ����ָ��
}
int fnStageErase(PSTAGE pStage, UINT dwFlags)
{
  if (pStage == NULL) {
    fnStageCode(SEC_PARAM_NULL);
    return 0;
  }
  // ��ⳡ��
  if (pStage->Flags & SGF_ARRAY) {
    fnStageCode(SEC_STAGE_FLAG);
    return -1; // �Ӽ����������ͷ�
  }
  // �ݹ��ͷ��ӳ���
  while (pStage->Next) {
    fnStageErase(pStage->Next, 0);
    //pStage->Next = NULL;
  }
  // �Ͽ�����
  if (pStage->Prev) {
    pStage->Prev->Next = NULL;
  }
  // �ͷų���
  SafeFree(pStage);
  return 1;
}

