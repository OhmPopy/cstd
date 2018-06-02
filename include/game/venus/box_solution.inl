// ****************************************************************************************************
// �ļ�: box_solution.cpp
// ע��:
// �����ӹؿ��ٷ��㷨ģ��, �Զ������������Ϸ�ؿ����߼���������ʵ��
// ****************************************************************************************************
#include <malloc.h> // malloc, realloc
//#include <stdio.h>
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
// ���ɿ����߷��б�
UINT fnStageMoves(PSTAGE pStage)
{
  UINT dwFlags;
  UINT dwMoves;
  UINT dwLoop;
  PMOVE pMove = pStage->Moves;
  dwMoves = 0; // ����߷�
  dwLoop = 0;
  while (dwLoop < pStage->Count) {
    dwFlags = SMF_MOVE_RISE; // ��ʼ����
    pMove->Index = dwLoop;
    while (dwFlags <= SMF_MOVE_FORE) {
      if (fnStagePush(pStage, pMove, dwFlags) > 0) { // �����ƶ�
        // �����ƶ�, ������Ч�ⷨ����, ָ���¸��߷�
        pMove->Flags = SMF_MOVE_NONE; // ��δʹ��
        pMove++;
        dwMoves++;
        pMove->Index = dwLoop;
      }
      dwFlags++; // ��һ������
    }
    dwLoop++; // ��һ������
  }
  return dwMoves;
}
// �Ӷ���������һ������, ���Ե�ǰ�������, ɨ������ظ�, ��Ч��׷�ӵ�����
PSTAGE fnStageNext(PQUEUE pQueue, PSTAGE pStage, int* pdwCode)
{
  PSTAGE pNext; // ������һ������
  PMOVE pMove;
  int dwRet;
  pNext = fnQueueApply(pQueue);
  if (pNext == NULL) {
    if (pdwCode) {
      *pdwCode = 0; // ���кľ�
    }
    fnStageCode(SEC_CACHE_NULL);
    return NULL;
  }
  // �����ϼ�����, ����ָ��
  V32Copy(pNext, pStage, pStage->Volume);
  pNext->Host = pStage; // .Prev��.Next�ڶ���ǰ��������ʱ��ֵ
  fnStagePtr(pNext); // �����ڲ�ָ��
  // ���ݵ�ǰ����, �ƶ�����
  pMove = &pStage->Moves[pStage->Index];
#ifdef _DEBUG
  //fnPrint("��ǰ����=0x%08X, ��������=0x%08X, ���=(%d, %d), ����:\r\n", pStage, pStage->Host, pStage->PosX, pStage->PosY);
  //fnPrintBox(pStage);
  //fnPrint("��ǰ����: ����%d����(%d, %d), �������(%d, %d), Ѱ·����Ϊ(%d, %d).\r\n\r\n",
  // pMove->Index, pMove->ObjX, pMove->ObjY, pMove->PortX, pMove->PortY, pMove->MoveX, pMove->MoveY);
#endif
  fnStagePush(pNext, pMove, SMF_MOVE_NONE); // Ӧ���߷�
  pNext->Range = 0; // û���߷�
  pNext->Index = 0; // ...
  pNext->Layer++; // ����
  // ɨ��������ͨ�е�Ԫ
  dwRet = fnStageScan(pQueue, pNext);
  // ��������ظ�
  pNext->Hash = fnStageHash(pNext->Stars, pNext->Series, pNext->Count); // ��������ϣ
  dwRet = fnStageLoop(pQueue, pNext);
  if (dwRet != 0) {
#ifdef _DEBUG
    fnPrint("�����ظ�����=0x%08X.\r\n", pNext);
#endif
    pNext->Prev = NULL; // ����, ��ֹ����ɾ��(������δ�������, ֻ׷�ӵ���������)
    pNext->Next = NULL;
    fnQueueRemove(pQueue, pNext); // �Ƴ�����
    if (pdwCode) {
      *pdwCode = -1; // �ظ�����
    }
    fnStageCode(SEC_ERROR_NONE); // �������
    return NULL;
  }
  // ��������
  if (pdwCode) {
    *pdwCode = 1;
  }
  return pNext;
}
// ��¼ʧ�ܳ�������
int fnStageSnap(PQUEUE pQueue, PSTAGE pStage)
{
  union {
    PSNAP pSnap; // ����ָ��
    BYTE* pDummy;
  };
  PSHOT pShot;
  UINT dwCount;
  int dwRet;
  //#if 1
  // return 1;
  //#endif
  pShot = pQueue->Shots;
  dwCount = pShot->Count;
  if (pShot->Value >= dwCount) {
    if (dwCount) {
      dwCount *= 2; // ��������, �����ط��仺����
      pSnap = (PSNAP)realloc(pShot->Snaps, pShot->Size * dwCount);
      if (pSnap == NULL) {
        goto _labCacheNull;
      }
    }
    else {
      dwCount = pQueue->Count; // �������Ԫ�����޸���
      pSnap = (PSNAP)malloc(pShot->Size * dwCount);
      if (pSnap == NULL) {
        goto _labCacheNull;
      }
      //pShot->Value = 0; // ʹ�õ�һ��(���г�ʼ��ʱ��ֵ)
    }
    pShot->Count = dwCount; // ���¼���
    pShot->Snaps = pSnap; // ����ָ��
  }
  else {
    pSnap = pShot->Snaps;
  }
  //pSnap = &pSnap[pShot->Value]; // Ԫ�ز�����С
  pDummy += pShot->Size * pShot->Value;
  pShot->Value++; // ����ʧ�ܾ�������
  pSnap->Position = pStage->Position;
  pSnap->Hash = pStage->Hash; // ����ָ��
  if ((pStage->Flags & SGF_CRC32) == 0) {
    // ����洢��������(ֻҪ����λ, ���᳷���ص����, ��������)
    //fnStageSort(pNext->Stars, NULL, pSnap->Stars, pNext->Count);
    V32Copy(pSnap->Stars, pStage->Series, sizeof(SPOT) * pStage->Count);
  }
  // ���ϵݹ鸸������, ���д������ӳ�������Ϊ��Ķ�����¼��ʧ���б���������
  pStage = pStage->Host;
  if (pStage != NULL) {
#ifdef _DEBUG
    fnPrint("��������=0x%08X, ʣ���Ӽ���������=%d.\r\n", pStage, pStage->Slaves);
    dwRet = fnQueueRange(pQueue, pStage);
    if (dwRet <= 0) {
      fnPrint("[����]����0x%08X������Ч������!\r\n", pStage);
      return 1;
    }
    if (pStage->Slaves == 0) {
      fnPrint("[����]����0x%08X�Ѿ�û���ӳ���!\r\n", pStage);
      return 1;
    }
#endif
    pStage->Slaves--; // ������ɿ���ȷ������Ϊ��
    if (pStage->Slaves == 0) {
#ifdef _DEBUG
      fnPrint("��¼��������=0x%08X��ʧ�ܼ���.\r\n", pStage);
#endif
      dwRet = fnStageSnap(pQueue, pStage);
      if (dwRet <= 0) {
        return dwRet - 1;
      }
#ifdef _DEBUG
      fnPrint("�Ƴ���������=0x%08X.\r\n", pStage);
#endif
      fnQueueRemove(pQueue, pStage);
      return dwRet + 1; // ���ؼ���
    }
  }
  return 1;
_labCacheNull:
  fnStageCode(SEC_CACHE_NULL);
  return 0; // �ڴ治��, ���ݲ���
}
// ѡ������λ�����߷�(����ٵ��ಽ���߼������ظ�, �����˺���)
void fnStageBest(PSTAGE pStage, UINT dwMoves)
{
  UINT dwValue; // ����λ
  PMOVE pMove = pStage->Moves; // �׸��߷�
  // �ҳ���һ����Ч�߷�
  while (dwMoves) {
    if (pMove->Flags == SMF_MOVE_NONE) {
      dwValue = pMove->Value;
      pStage->Index = ((UINT)pMove - (UINT)pStage->Moves) / sizeof(MOVE); // ��������
      dwMoves--; // ����������ݼ�
      break;
    }
    pMove++;
  }
  // �ҳ����߹�λ���߷�
  while (dwMoves) {
    if (pMove->Flags == SMF_MOVE_NONE) {
      // ����߷���û����
      if (pMove->Value > dwValue) {
        dwValue = pMove->Value;
        pStage->Index = ((UINT)pMove - (UINT)pStage->Moves) / sizeof(MOVE); // ��������
      }
      dwMoves--; // ����������ݼ�
    }
    pMove++;
  }
}
void fnStagePath(PQUEUE pQueue, PSTAGE pStage, PSPOT pPath)
{
  UINT dwStep = pStage->Layer * 2;
  // �洢�߷�����(��������һ������.X�洢��ߴ�)
  if (dwStep > (UINT)pPath->X) {
    return; // �ⷨ��������������
  }
  pPath = &pPath[dwStep - 1]; // ��������ʼ����
  pQueue->Step = pStage->Layer;
  // ����λ��(��������ʼ����)
  while (pStage->Host) {
#ifdef _DEBUG
    //fnPrint("\r\n��ǰ����: ���=(%d, %d), ����:", pStage->PosX, pStage->PosY);
    //fnPrintBox(pStage);
#endif
    //if(pStage->Automation.Value != 0)
    *pPath-- = pStage->Position; // �ƶ�λ��
    *pPath-- = pStage->Automation; // �Զ�λ��
    pStage = pStage->Host; // ��һ������
  }
  return;
}
// �����ӳ����б��������
UINT fnStageSlaves(PQUEUE pQueue, PSTAGE pStage, void* lpVoid, int* pdwCode)
{
  PSTAGE pNext;
  UINT dwStage;
  int dwRet;
  UINT dwMoves = pStage->Range;
  dwStage = 0;
  while (dwMoves) {
#if 0 // *** ����λ�߷���С��������һ���ٶ�, �ڴ濪֧���� ***
    fnStageBest(pStage, dwMoves); // ѡ������λ���߷�
#else
    pStage->Index = pStage->Range - dwMoves;
#endif
    pNext = fnStageNext(pQueue, pStage, &dwRet); // ��������: .Index������
    if (dwRet == 0) {
      if (pdwCode) {
        *pdwCode = -1;
      }
      return 0; // ���кľ�
    }
    pStage->Moves[pStage->Index].Flags |= SMF_MOVE_USED; // ��ʶ�Ѿ�����
    if (pNext != NULL) {
      // ��һ���������ظ�, ׷�ӵ�����
      dwRet = fnQueuePush(pQueue, pNext);
      // ����Ѿ���ȫ��λ, �򷵻�(��ʱpNext������, ���׼���ⷨ)
      if (pNext->Value == pNext->Count) {
        // ******** ������λ(��Ϊ�����߷�) ********
        fnStagePath(pQueue, pNext, (PSPOT)lpVoid); // ���ɽⷨ����
        if (pdwCode) {
          *pdwCode = 1;
        }
        return 0;
      }
      dwStage++; // ������һ���ӳ���
    }
    dwMoves--; // �߷������ݼ�
  }
  if (pdwCode) {
    *pdwCode = 0;
  }
  return dwStage;
}
// *** �˺�������ǰӦ��֤���г����Ǿ������� ***
// λ����(3,1), �ƶ���(4,3), ֱ�����ϼ���, ����ѭ��źͷ���˳��, id=0�����ظ�, ����id=1����
// ���: ׷����������, ������ѡ���λ�����������Ӻͷ���(�Ȳ�������ѡ���λ�������ٵ��߷�)
// �ȳ��ԺͿ��ٳ����õ������߷������λ�����б�, ���ΰ���λ�����Ӵ�С����������ݹ�
// Ϊ��������λ����δ��λ��������, �õ��ⷨ��, �Դ���Ϊ��������, ���������и��Žⷨ
// ���: �ӵ�ǰ����������Ч�߷������ĳ���, ��λ�򷵻�, �����������, һ����������
// �õ���һ���ⷨ�������Žⷨ, 100��ÿ��16���߷�����16^100������, �ڴ濪���޴�
// ���д���ݹ�, ��������֤���Ͳ������ȱ�����, ʧЧ���ظ���������, ��������ڴ�����
// ���: ���ɵ��Ӽ�����, ��������Ч��, ��Ч���˵��´�����Ӽ���ʱ��, ��Ϊ��߲����ݽ����߼�
// ��������ٳ�����������, ���з������������߲���, �Ӷ������ظ����ѭ������
int fnStageCalc(PQUEUE pQueue, void* lpVoid)
{
  PSTAGE pStage;
  int dwRet;
  // ѭ������ݹ�(�ڱ��������ʼ������)
  pStage = pQueue->Active; // fnQueuePop()���Ƴ�Ԫ��
  // ��һ�������Ѿ�ɨ��
  while (pStage != NULL) {
    pStage->Range = fnStageMoves(pStage); // ���ɿ����߷��б�
    if (pStage->Range == 0) {
      // û�п����߷�
#ifdef _DEBUG
      fnPrint("û�п����߷�, ��¼����=0x%08X��ʧ�ܼ���, �¸�����=0x%08X.\r\n\r\n", pStage, pStage->Next);
#endif
      dwRet = fnStageSnap(pQueue, pStage); // ��¼��ʧ�ܶ���
      if (dwRet <= 0) {
        return -1;
      }
      pStage = fnQueueRemove(pQueue, pStage); // �Ƴ�����, ���¼ӵ�δ��, ָ��������һ������
      continue;
    }
    // �Ӹ����λ���߷���ʼ, ���η���
    pStage->Slaves = fnStageSlaves(pQueue, pStage, lpVoid, &dwRet);
    if (dwRet > 0) {
      fnStageCode(SEC_ERROR_NONE); // ����������
      return 1; // �õ����Žⷨ
    }
    else if (dwRet < 0) {
      return -2;
    }
    if (pStage->Slaves == 0) {
#ifdef _DEBUG
      fnPrint("û���Ӽ�����, ��¼����=0x%08X��ʧ�ܼ���, �¸�����=0x%08X.\r\n\r\n", pStage, pStage->Next);
#endif
      dwRet = fnStageSnap(pQueue, pStage); // ��¼��ʧ�ܶ���
      if (dwRet <= 0) {
        return -3;
      }
      pStage = fnQueueRemove(pQueue, pStage); // �Ƴ�����, ���¼ӵ�δ��, ָ��������һ������
      continue;
    }
    pStage = pStage->Next; // ��ǰ���ӳ���, ����Ԫ��, ������������
  }
  return 0; // �����޽�
}
int fnStageSolve(PSTAGE pStage, void* lpVoid, UINT dwCount, UINT dwFlags)
{
  PROP prp; // ����, ���ڼ������ݲ������ĳ���
  PQUEUE pQueue; // ����ָ��
  STACK stk; // ɨ���ջ
  SHOT sht; // ʧ�ܿ���
  union {
    UINT dwSize; // �����ڴ��С, �洢�������
    int dwStep;
  };
  int dwRet; // ����ֵ
  if (pStage == NULL) {
    fnStageCode(SEC_PARAM_NULL);
    return 0; // ��Чָ��
  }
  if (dwCount <= 1) {
    fnStageCode(SEC_PARAM_MANY);
    return -1; // ��������С
  }
  dwSize = 0; // �Զ��ߴ�
  V32Clear32(&prp, 0, sizeof(prp)); // ��������
  if ((pStage->Flags & SGF_READY) != SGF_READY) {
    // �½�����, ���㳡������(ֻ�л������ݵĳ���ҲҪ��������)
    prp.SizeX = pStage->SizeX;
    prp.SizeY = pStage->SizeY;
    prp.Flags = SPF_SPOT_ARRAY; // Ҫ�󷵻���������
    dwRet = fnStageProp(pStage->Matrix, &prp);
    pStage->Count = prp.Count;
    pStage->Position.Value = prp.Role.Value;
    pStage->Flags |= SGF_BASIC; // ����������ɼ���, ����������û�б���
    if (dwRet <= 0) {
      fnStageCode(SEC_STAGE_NULL);
      return -2; // ������Ч
    }
    dwSize = fnStageSize(pStage->Count, pStage->SizeX, pStage->SizeY);
  }
  if (pStage->Value == pStage->Count) {
    fnStageCode(SEC_SOLVE_DONE);
    return -3; // �Ѿ���λ
  }
  // ��ʼ����������
  stk.Stars = prp.Spots;
  pQueue = fnQueueInit(pStage, &stk, &sht, dwSize, dwCount, dwFlags);
  if (pQueue == NULL) {
    return -4; // �ڴ治��(����������Ӻ�����ֵ)
  }
#ifdef _DEBUG
  dwRet = fnDebugInit();
  prp.Flags = ((PSPOT)lpVoid)->X;
  fnPrint("��ջ��С=%d, ���гߴ�=%d, �ⷨ�ߴ�=%d, ��ʼ���...\r\n", stk.Count, dwCount, prp.Flags);
#endif
  // ������(����ɹ�, �ⷨ���걣�浽lpVoid, �������浽pQueue->Step)
  dwRet = fnStageCalc(pQueue, lpVoid);
  dwStep = pQueue->Step;
#ifdef _DEBUG
  prp.Value = pQueue->Value;
  prp.Hash = pQueue->Used;
#endif
  SafeFree(sht.Snaps); // �ͷ���Ч����
  SafeFree(stk.Stars); // �ͷ�ɨ���ջ
  SafeFree(pQueue); // �ͷŶ���
  if (dwRet <= 0) {
#ifdef _DEBUG
    fnPrint("���ʧ��, ����ֵ=%d, ���������=%d!\r\n", dwRet, fnStageCode(SEC_ERROR_DUMP));
    fnDebugExit();
#endif
    return -6; // �����޽�(�����Ǵ������)
  }
#ifdef _DEBUG
  fnPrint("�ؿ����ɹ�, ����ʹ�÷�ֵ=%d, ����ʣ�����=%d, ���շ�ֵ=%d, ��������=%d!\r\n", prp.Value, prp.Hash, sht.Count, sht.Value);
  stk.Count = dwSize; // ���������ƶ�, union
  if (stk.Count > 0) {
    stk.Stars = (PSPOT)lpVoid; // �������ָ��
    prp.Count = 0; // ����Ѱ·����
    while (stk.Count > 0) {
      stk.Count--;
      if (stk.Stars->Value != 0) {
        fnPrint("(%d, %d)\r\n", stk.Stars->X, stk.Stars->Y);
        prp.Count++;
      }
      stk.Stars++;
      fnPrint("<%d, %d>\r\n", stk.Stars->X, stk.Stars->Y);
      stk.Stars++;
    }
    fnPrint("���Žⷨ�ƶ� %d ��, Ѱ· %d ��, �ϼ����� %d ��!\r\n", dwSize, prp.Count, dwSize + prp.Count);
  }
  fnDebugExit();
#endif
  return dwStep;
}

