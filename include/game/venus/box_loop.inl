// ****************************************************************************************************
// �ļ�: box_loop.cpp
// ע��:
// �����ظ�����㷨ģ��, �ڶ��к�ʧ���߷���Ѱ����ͬ����, ����ɫ�Ƿ�ͨ��, ��ͨ��������ظ�
// ****************************************************************************************************
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
// globals
PMAZE g_pScene = NULL;
void fnFreeScene()
{
  if (g_pScene) {
    fnAlphaExit(g_pScene, 0);
    g_pScene = NULL;
  }
}
inline void fnStageScene(PSTAGE pStage)
{
  if (g_pScene == NULL) {
_labCreate:
    g_pScene = fnAlphaInit(pStage->SizeX, pStage->SizeY, 0, SMT_MASKED);
    goto _labCheck;
  }
  if (g_pScene->SizeY != pStage->SizeY || g_pScene->SizeX != pStage->SizeX) {
    fnAlphaExit(g_pScene, 0);
    goto _labCreate;
  }
_labCheck:
  // �ⲿ����ָ��(X��Yһ��Ϊ��)
  fnAlphaEdit(g_pScene, -1, 0, pStage->Matrix);
}
int fnStageStars(PSTAGE pStage, PSPOT pStars)
{
  int dwRet;
  long dwStep;
  if ((pStage->Flags & SGF_BASIC) == 0) {
    return 0; // �½����������޷�����
  }
  fnStageScene(pStage); // �趨�ⲿ����
  dwRet = fnAlphaStar(g_pScene, &pStage->Position, pStars, &dwStep); // ֱ��Ѱ·(lpVoidҪ���㹻����������)
  if (dwRet <= 0 || dwStep <= 0) {
    return dwRet; // �޷�����
  }
  V32Copy(pStars, g_pScene->Spots, sizeof(SPOT) * dwStep);
  return dwStep;
}
int fnStageLoopHash(PQUEUE pQueue, PSTAGE pStage)
{
  union {
    PSTAGE pNode; // Ҫ�Ƚϵĳ���ָ��
    PSNAP pSnap; // Ҫ�ȽϵĿ���ָ��
    BYTE* pNum; // ���ڵݽ�����ָ��
  };
  PSHOT pShot;
  UINT dwSize; // ��λ�����ڴ��С
  UINT dwLoop; // ѭ������
  //int dwRet; // ����ֵ
  //if((pStage->Flags & SGF_READY) != SGF_READY) return -1; // �ⲿ���
  pNode = pQueue->Active;
  //if(pStage->Flags & SGF_CRC32) // ******** ʹ�ù�ϣ�Ƚ� ********
  // 1. ������ж��г���, �������������������׷��
  while (pNode) {
    //if(pNode != pStage) // ���Լ����Ƚ�(������δ�������)
    if (pNode->Hash == pStage->Hash) {
      // ����λ��һ��, ����ɫͨ��
      //dwRet = AlphaStar(g_pScene, &pStage->Position, &pNode->Position, NULL);
      //if(dwRet > 0)
      if (GetMatrix(pStage, pNode->PosX, pNode->PosY) & SMT_OPENED) {
        return 1; // �����߷��ظ�, Ӧ����ֹ
      }
    }
    pNode = pNode->Next; // �����¸�����
  }
  // 2. ���ʧ�ܿ���, ����Ѿ�ʧ�ܹ��Ͳ�Ҫ�ص�����
  pShot = pQueue->Shots;
  dwSize = pShot->Size;
  pSnap = pShot->Snaps;
  dwLoop = pShot->Value;
  while (dwLoop--) {
    if (pSnap->Hash == pStage->Hash) {
      // ����λ��һ��, ����ɫͨ��
      //dwRet = AlphaStar(g_pScene, &pStage->Position, &pNode->Position, NULL);
      //if(dwRet > 0)
      if (GetMatrix(pStage, pSnap->PosX, pSnap->PosY) & SMT_OPENED) {
        return 2; // ʧ���߷�����, Ӧ����ֹ
      }
    }
    pNum += dwSize; // pSnap++;
  }
  return 0;
}
int fnStageLoopSpot(PQUEUE pQueue, PSTAGE pStage)
{
  union {
    PSTAGE pNode; // Ҫ�Ƚϵĳ���ָ��
    PSNAP pSnap; // Ҫ�ȽϵĿ���ָ��
    BYTE* pNum; // ���ڵݽ�����ָ��
  };
  PSHOT pShot;
  UINT dwSize; // ��λ�����ڴ��С
  UINT dwLoop; // ѭ������
  int dwRet; // ����ֵ
  //if((pStage->Flags & SGF_READY) != SGF_READY) return -1; // �ⲿ���
  pNode = pQueue->Active;
  //if((pStage->Flags & SGF_CRC32) == 0) // ******** ��������Ƚ� ********
  // 1. ������ж��г���, ��ʱpStage->Series�����������
  while (pNode) {
    //if(pNode != pStage) // ���Լ����Ƚ�(������δ�������)
    // �Ƚ�pNode�����б��뵱ǰ���������б�
    dwRet = fnStageSort(pNode->Series, pStage->Series, NULL, pStage->Count);
    if (dwRet == 2) {
      // ����λ��һ��, ����ɫͨ��
      //dwRet = AlphaStar(g_pScene, &pStage->Position, &pNode->Position, NULL);
      //if(dwRet > 0)
      if (GetMatrix(pStage, pNode->PosX, pNode->PosY) & SMT_OPENED) {
        return 1; // �����߷��ظ�, Ӧ����ֹ
      }
    }
#ifdef _DEBUG
    else if (pNode->Hash == pStage->Hash) {
      fnPrint("[����]���ֲ�ͬ, ����ϣֵ��ͬ!"); // ���Զϵ�
    }
#endif
    pNode = pNode->Next; // �����¸�����
  }
  // 2. ���ʧ�ܿ���, ����Ѿ�ʧ�ܹ��Ͳ�Ҫ�ص�����
  pShot = pQueue->Shots;
  dwSize = pShot->Size;
  pSnap = pShot->Snaps;
  dwLoop = pShot->Value;
  while (dwLoop--) {
    // �Ƚ�ʧ�ܾ��������(������洢)�뵱ǰ��������
    dwRet = fnStageSort(pSnap->Stars, pStage->Series, NULL, pStage->Count);
    if (dwRet == 2) {
      // ����λ��һ��, ����ɫͨ��
      //dwRet = AlphaStar(g_pScene, &pStage->Position, &pNode->Position, NULL);
      //if(dwRet > 0)
      if (GetMatrix(pStage, pSnap->PosX, pSnap->PosY) & SMT_OPENED) {
        return 2; // ʧ���߷�����, Ӧ����ֹ
      }
    }
#ifdef _DEBUG
    else if (pSnap->Hash == pStage->Hash) {
      fnPrint("[����]���ֲ�ͬ, ����ϣֵ��ͬ!"); // ���Զϵ�
    }
#endif
    pNum += dwSize; // pSnap++;
  }
  return 0;
}
// �жϾ����ظ�(��ʱ����������ɨ��, ���Ѿ���)
int fnStageLoop(PQUEUE pQueue, PSTAGE pStage)
{
  if (pStage->Flags & SGF_CRC32) {
    // ******** ʹ�ù�ϣ�Ƚ� ********
    return fnStageLoopHash(pQueue, pStage);
  }
  // ******** ��������Ƚ� ********
  return fnStageLoopSpot(pQueue, pStage);
}

