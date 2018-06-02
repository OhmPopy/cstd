// ****************************************************************************************************
// �ļ�: box_queue.cpp
// ע��:
// �Զ������ģ��, �ö��н��ѭ������ݹ�, ���з�����: �Ƚ��ȳ�, ��֤���Ͳ��������ȱ�����
// ****************************************************************************************************
#include <malloc.h> // malloc, realloc
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
PQUEUE fnQueueInit(PSTAGE pStage, PSTACK pStack, PSHOT pShot, UINT dwSize, UINT dwCount, UINT dwFlags)
{
  PQUEUE pQueue;
  union {
    UINT dwVolume;
    int dwRet;
  };
  if (dwSize == 0) {
    dwSize = pStage->Volume; // ��������
  }
  dwVolume = dwSize * dwCount; // ���и���
  dwVolume += sizeof(QUEUE) - sizeof(UINT); // �����ײ�
  pQueue = (PQUEUE)malloc(dwVolume);
  if (pQueue == NULL) {
    fnStageCode(SEC_CACHE_NULL);
    SafeFree(pStack->Stars);
    return NULL; // �ڴ治��
  }
  V32Copy(pQueue->Dummy, pStage, pStage->Volume);
  pStage = (PSTAGE)pQueue->Dummy; // ����ָ��
  pStage->Volume = dwSize; // ���³����ߴ�
  pStage->Flags |= SGF_EXTRA; // ��������
  pStage->Flags |= SGF_ARRAY; // ����Ԫ��(��ʶ�����ͷ�)
  if (dwFlags & SMF_FLAG_TEST) {
    pStage->Flags |= SGF_DEBUG;
  }
  else {
    pStage->Flags &= ~SGF_DEBUG;
  }
  pStage->Prev = NULL; // �������(�������Ϸ��, ����������)
  pStage->Next = NULL;
  pStage->Host = NULL;
  fnStagePtr(pStage); // �����ڲ�ָ��
  if (pStack->Stars != NULL) {
    // ������������(���������)
    V32Copy(pStage->Stars, pStack->Stars, sizeof(SPOT) * pStage->Count * 2);
  }
  SafeFree(pStack->Stars);
  // ��ʼ����ջ�Ϳ���(��Ч����)
  pStack->Count = pStage->SizeX * pStage->SizeY;
  pStack->Stars = (PSPOT)malloc(sizeof(SPOT) * pStack->Count);
  if (pStack->Stars == NULL) {
    SafeFree(pQueue);
    fnStageCode(SEC_CACHE_NULL);
    return NULL; // �ڴ治��
  }
  pShot->Count = 0;
  pShot->Value = 0;
  if (dwFlags & SMF_FLAG_HASH) {
    pShot->Size = sizeof(SNAP) - sizeof(SPOT); // ʹ�ù�ϣ, û����������
    pStage->Flags |= SGF_CRC32;
  }
  else {
    pShot->Size = sizeof(SNAP) + sizeof(SPOT) * (pStage->Count - 1); // ���ԱȽ�
    pStage->Flags &= ~SGF_CRC32;
  }
  pShot->Snaps = NULL;
  // ��ʼ������
  pQueue->Volume = dwVolume; // ���гߴ�
  pQueue->Size = dwSize; // Ԫ�سߴ�
  pQueue->Count = dwCount; // Ԫ������
  pQueue->Value = 1;
  pQueue->Used = 1;
  pQueue->Active = pStage;
  pQueue->Backup = pStage;
  pQueue->Stages = NULL;
  pQueue->Shots = pShot; // ��Ч����
  pQueue->Stacks = pStack; // ɨ���ջ
  dwRet = fnStageScan(pQueue, pStage); // ɨ���һ������
  return pQueue;
}
// ���һ��Ԫ���Ƿ��ڻ�������Χ��
int fnQueueRange(PQUEUE pQueue, PSTAGE pStage)
{
  union {
    PSTAGE pStage1;
    BYTE* pDummy1;
    UINT dwNum1;
  };
  union {
    PSTAGE pStage2;
    BYTE* pDummy2;
    UINT dwNum2;
  };
  union {
    PSTAGE pStage3;
    BYTE* pDummy3;
    UINT dwNum3;
  };
  pStage1 = pStage;
  pDummy2 = pQueue->Dummy;
  pDummy3 = pDummy2 + pQueue->Size * pQueue->Count;
  if (dwNum1 < dwNum2 || dwNum1 >= dwNum3) {
    return 0; // ���뻺������Ԫ��ָ��
  }
  pStage2 = pQueue->Active;
  while (pStage2) {
    if (pStage2 == pStage) {
      return 1;
    }
    pStage2 = pStage2->Next;
  }
  return -1;
}
// ����һ��Ԫ��
PSTAGE fnQueuePop(PQUEUE pQueue)
{
  PSTAGE pStage = pQueue->Active;
  if (pStage) {
    // ��һ��Ԫ����Ч, ָ����һ��
    pQueue->Active = pStage->Next;
  }
  return pStage;
}
// ׷��һ��Ԫ��
int fnQueuePush(PQUEUE pQueue, PSTAGE pStage)
{
  pStage->Prev = pQueue->Backup;
  pStage->Next = NULL;
  if (pQueue->Backup) {
    pQueue->Backup->Next = pStage;
  }
  pQueue->Backup = pStage; // �ӵ�ĩβ
  return 1;
}
// ����һ��Ԫ��
PSTAGE fnQueueApply(PQUEUE pQueue)
{
  union {
    PSTAGE pStage;
    BYTE* pDummy;
  };
  pStage = pQueue->Stages;
  if (pStage) {
#ifdef _DEBUG
    fnPrint("���ճ���=0x%08X.\r\n", pStage);
#endif
    // ���ڹ��ڳ���, ʹ�ù��ڳ���
    pQueue->Stages = pStage->Next; // ��һ��ǰ��
    if (pStage->Next) {
      pStage->Next->Prev = NULL;
    }
  }
  else {
    // ����һ��Ԫ��
    if ((pQueue->Value + 1) >= pQueue->Count) {
      return NULL; // ��ջ�ľ�
    }
    pDummy = pQueue->Dummy;
    pDummy += pQueue->Size * pQueue->Value; // ÿ��Ԫ�ر�����ͬ�ߴ�
    pQueue->Value++; // ����һ��
  }
  pQueue->Used++; // ���ø�������һ��
  return pStage;
}
// ɾ��һ��Ԫ��, ���������¸�Ԫ��
PSTAGE fnQueueRemove(PQUEUE pQueue, PSTAGE pStage)
{
  union {
    PSTAGE pPrev;
    PSTAGE pStages;
  };
  PSTAGE pNext = pStage->Next;
  //if(!fnQueueRange(pQueue, pStage))
  //{
  // return NULL;
  //}
  // ������β
  pPrev = pStage->Prev;
  if (pPrev) {
    pPrev->Next = pNext;
  }
  if (pNext) {
    pNext->Prev = pPrev;
  }
  // ���ָ��
  if (pStage == pQueue->Active) {
    pQueue->Active = pNext;
  }
  if (pStage == pQueue->Backup) {
    pQueue->Backup = pPrev;
  }
  // �ӵ�����
  pStages = pQueue->Stages;
  pStage->Prev = NULL;
  pStage->Next = pStages;
  //pStage->Host = NULL;
  if (pStages) {
    pStages->Prev = pStage;
  }
  pQueue->Stages = pStage;
  pQueue->Used--; // ���ø�������һ��
  return pNext;
}

