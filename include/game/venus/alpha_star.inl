// ****************************************************************************************************
// �ļ�: alpha.cpp
// ע��:
// A*Ѱ·�㷨ģ��, �ж�����֮���Ƿ�ͨ��, ���õ�·���͸��ڵ��Ȩֵ
// ��չ������·������, ��ʹ��Dijkstra, �μ�http://www.2cto.com/kf/201104/87378.html
// ****************************************************************************************************
//#include <stdlib.h>
#include <malloc.h>
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
//#include "debug.h"
using namespace Valiant::Gaming;
using namespace Valiant;
// ����һ������, ����ʼ��
PMAZE fnAlphaInit(long dwSizeX, long dwSizeY, long dwMaxCost, UINT dwFlags)
{
  UINT dwSize;
  PMAZE p;
  long dwCount = dwSizeX * dwSizeY;
  dwSize = sizeof(MAZE) - sizeof(UINT);
  dwSize += dwCount * sizeof(KNOT);
  dwSize += dwCount * sizeof(SPOT);
  dwSize += dwCount * sizeof(MARK);
  dwSize += dwCount;
  p = (PMAZE)malloc(dwSize);
  if (p == NULL) {
    return NULL;
  }
  V32Clear(p, 0, dwSize); // ����ȫ������
  if (dwMaxCost <= 0) {
    dwMaxCost = dwCount * CDC_PROP_NEAR; // �Զ����ڸ�������߱�
  }
  p->Flags = dwFlags;
  p->SizeY = dwSizeY;
  p->SizeX = dwSizeX;
  p->Range = dwMaxCost; // �������(����ֵ)
  p->Count = dwCount; // �ڵ�����
  p->Value = 0; // �޽�
  p->Knots = (PKNOT)&p->Matrix[p->Count];
  p->Spots = (PSPOT)&p->Knots[p->Count];
  p->Marks = (PMARK)&p->Spots[p->Count];
  p->Scene = NULL; // ���ⲿ����
  return p;
}
// ��д��������, ����ȫ����ȡ, һ����ֵ�趨�ⲿָ��, ���������趨
int fnAlphaEdit(PMAZE pMaze, long dwPosX, long dwPosY, void* lpValue)
{
  PBYTE p;
  if (pMaze == NULL) {
    return 0;
  }
  if (dwPosX < 0 && dwPosY < 0) {
    p = pMaze->Scene;
    if (p == NULL) {
      p = pMaze->Matrix; // û���ⲿ����
    }
    *(PBYTE)lpValue = ArrayItem2(p, pMaze->SizeX, dwPosX, dwPosY);
    return 2;
  }
  if (dwPosX < 0 || dwPosY < 0) {
    pMaze->Scene = (PBYTE)lpValue; // �����ⲿ����ָ��
    return 3;
  }
  // �����ֽ���ֵ
  if (lpValue == NULL || dwPosX >= pMaze->SizeX || dwPosY >= pMaze->SizeY) {
    //V32Copy(pMaze->Matrix, lpValue, pMaze->Count);
    return -1;
  }
  p = pMaze->Scene;
  if (p == NULL) {
    p = pMaze->Matrix;
  }
  ArrayItem2(p, pMaze->SizeX, dwPosX, dwPosY) = *(PBYTE)lpValue; // д��
  return 1;
}
inline long AlphaCalc(PSPOT pSpot1, PSPOT pSpot2)
{
  // ֱ��ʹ�������ʹ������Ǻ�������
  if (pSpot1->X > pSpot2->X) {
    if (pSpot1->Y > pSpot2->Y) {
      return (pSpot1->X - pSpot2->X) + (pSpot1->Y - pSpot2->Y);
    }
    else {
      return (pSpot1->X - pSpot2->X) + (pSpot2->Y - pSpot1->Y);
    }
  }
  else {
    if (pSpot1->Y > pSpot2->Y) {
      return (pSpot2->X - pSpot1->X) + (pSpot1->Y - pSpot2->Y);
    }
    else {
      return (pSpot2->X - pSpot1->X) + (pSpot2->Y - pSpot1->Y);
    }
  }
  //return dX + dY;
}
inline void AlphaNode(PMAZE pMaze, long dwIndex, long dwPrevId, long dwCost, long dwLast, long dwPosX, long dwPosY)
{
  // ��ʼ��ѭ��������һ�Σ�dwIndex�ڵ���֮���Լӣ���ʼΪ0
  if (dwIndex >= pMaze->Count) {
    return;
  }
  //pMaze->Knots[dwIndex].Spot = *pSpot; // �Ƿ�ᴴ����ʱ�ṹ��??
  pMaze->Knots[dwIndex].Spot.X = dwPosX;
  pMaze->Knots[dwIndex].Spot.Y = dwPosY;
  pMaze->Knots[dwIndex].Prev = dwPrevId;
  if (dwPrevId != -1) {
    pMaze->Knots[dwIndex].Step = pMaze->Knots[dwPrevId].Step + 1;
  }
  else {
    pMaze->Knots[dwIndex].Step = 1;
  }
  pMaze->Knots[dwIndex].Cost = dwCost;
  pMaze->Knots[dwIndex].Last = dwLast * 10; // ÿ��dwLen���¼���õ�������
  pMaze->Knots[dwIndex].Flags = SNF_PROP_READY;
  return;
}
//inline BYTE fnAlphaUnit(PMAZE pMaze, PSPOT ps)
//{
// BYTE *pUnit = pMaze->Scene;
// if(pUnit == NULL) pUnit = pMaze->Matrix;
// return pUnit[ps->Y * pMaze->SizeX + ps->X];
//}
//inline PMARK fnAlphaStep(PMAZE pMaze, PSPOT ps)
//{
// return &pMaze->Marks[ps->Y * pMaze->SizeX + ps->X];
//}
// Ѱ·ָ������
int fnAlphaStar(PMAZE pMaze, PSPOT lpStart, PSPOT lpTarget, long* pdwStep)
{
  PMARK pMark;
  long lCurCost, nMaxCost; // �ݴ����ģ�����б�
  long i, j; // temp & looping var
  SPOT tps, cps; // test pos, current pos
  long dwValue;
  long dwIndex = 0;
  long dwNode = 0; // ��ǰNode��
  long dwLoop;
  long dwLen; // ���յ����
  // check for memory address accessable
  if (pMaze == NULL || lpStart == NULL || lpTarget == NULL) {
    return 0; // ʼĩ������Ч
  }
  dwLen = AlphaCalc(lpStart, lpTarget); // dwLen = ��X + ��Y
  if (dwLen == 0) {
    return -1; // ʼĩ������ͬ
  }
  // zero step memory(cell prop list)
  V32Clear32(pMaze->Marks, 0, pMaze->Count * sizeof(MARK));
  pMaze->Value = 0;
  if (pMaze->Scene == NULL) {
    pMaze->Scene = pMaze->Matrix;
  }
  // ��ӵ�һ����
  dwNode = 0;
  AlphaNode(pMaze, dwNode, -1, 0, dwLen, lpStart->X, lpStart->Y);
  dwNode++;
  // enter loop - check around cells
  while (1) {
    nMaxCost = pMaze->Range; // �����ܱ������
    dwIndex = -1;
    for (dwLoop = 0; dwLoop < dwNode; dwLoop++) {
      if (pMaze->Knots[dwLoop].Flags != SNF_PROP_ERROR) {
        //��δ�ر�����С·�̺͵ĵ�
        lCurCost = pMaze->Knots[dwLoop].Cost + pMaze->Knots[dwLoop].Last;
        if (lCurCost < nMaxCost) {
          nMaxCost = lCurCost; // ����������
          dwIndex = dwLoop; // ����ڵ����
          //break; // ���нڵ㶼Ҫ����
        }
      }
    }
    if (dwIndex == -1) {
      return -2; // there is no path exist!
    }
    cps.X = pMaze->Knots[dwIndex].Spot.X;
    cps.Y = pMaze->Knots[dwIndex].Spot.Y;
    if ((cps.X == lpTarget->X) && (cps.Y == lpTarget->Y)) {
      break; // ��ǰ�������յ�, ����whileѭ��
    }
    //sprintf(szText, "select best cell:[%d,%d] for check:", cps.X, cps.Y);
    for (i = -1; i <= 1; i++) {
      for (j = -1; j <= 1; j++) {
        //if(i == 0 && j == 0) continue; // �����߶Խ��ߣ�ֻҪ������ͬʱΪ��(����������)
        if (i == 0 && j == 0) {
          continue;
        }
        if (i != 0 && j != 0) {
          continue;
        } // ��ֹ�߶Խ��ߣ�������ֻ��һ��Ϊ��{[(i & j) == 0]&&[(i | j) != 0]}
        tps.X = cps.X + i;
        tps.Y = cps.Y + j;
        if (tps.X < 0) {
          continue; // ���Խ��
        }
        if (tps.X >= pMaze->SizeX) {
          continue; // �ұ�Խ��
        }
        if (tps.Y < 0) {
          continue; // ����Խ��
        }
        if (tps.Y >= pMaze->SizeY) {
          continue; // �ױ�Խ��
        }
        // �õ������ھ���Χ��
        if (ArrayItem2(pMaze->Scene, pMaze->SizeX, tps.X, tps.Y) & (BYTE)pMaze->Flags) {
          continue; // ��������λ�Ժ���Ȼ����ͨ��(��������: ������ո��Ŀ��)
        }
        pMark = &ArrayItem2(pMaze->Marks, pMaze->SizeX, tps.X, tps.Y);
        switch (pMark->Flags) {
        case SSF_PROP_UNKNOW: // it'v not been check
          dwValue = pMaze->Knots[dwIndex].Cost;
          //if(i * j != 0)
          // dwValue += CDC_PROP_AWAY; // �������궼�Ƿ���, �Խ�
          //else
          dwValue += CDC_PROP_NEAR; // �κ�һ������Ϊ��, ����
          dwLen = AlphaCalc(&tps, lpTarget); //dwLen = ��X + ��Y
          AlphaNode(pMaze, dwNode, dwIndex, dwValue, dwLen, tps.X, tps.Y);
          pMark->Flags = SSF_PROP_OPENED; // open it
          pMark->Index = dwNode++;
          // sprintf(szText, "add cell:[%d,%d] for check:", tps.X, tps.Y);
          break;
        case SSF_PROP_OPENED: // this cell is valid
          dwValue = pMark->Index;
          dwLen = pMaze->Knots[dwIndex].Cost;
          dwLen += CDC_PROP_NEAR; // ֻ������
          if (dwLen < pMaze->Knots[dwValue].Cost) {
            pMaze->Knots[dwValue].Cost = dwLen;
            pMaze->Knots[dwValue].Prev = dwIndex;
            pMaze->Knots[dwValue].Step = pMaze->Knots[dwIndex].Step + 1; //change level
          }
          //sprintf(szText, "change pID for cell:[%d,%d] to %d.", tps.X, tps.Y, nID);
          break;
        default:
          //end if(lpOGrid[tps.X][tps.Y].State..
          break;
        }
        //end if((lpCell..
        //end if((tps.X..
        //end if(((i..
      } //next j
    } //next i
    // it will continue if any path and not at target
    pMark = &ArrayItem2(pMaze->Marks, pMaze->SizeX, cps.X, cps.Y);
    pMaze->Knots[dwIndex].Flags = SNF_PROP_ERROR;
    pMark->Flags = SSF_PROP_CLOSED; // close it
    //sprintf(szText, "close cell:[%d,%d] ok.", cps.X, cps.Y);
  }
  // ����malloc()��realloc()ʱ�����Ĵ��ÿռ任ȡʱ��
  dwValue = pMaze->Knots[dwIndex].Cost + pMaze->Knots[dwIndex].Last;
  if (pdwStep == NULL) {
    // ����Ҫ�������
    return dwValue; // return cost to get here
  }
  //sprintf(szText, "best path found in cost %d.", dwValue);
  pMaze->Value = pMaze->Knots[dwIndex].Step;
  //SafeFree(pMaze->Spots); // release old path
  //pMaze->Spots = (PSPOT)malloc(pMaze->Value * sizeof(SPOT));
  //if(pMaze->Spots == NULL) return -3; // Out of memory
  // ...
  dwLoop = pMaze->Value;
  *pdwStep = dwLoop; // set the steps
  while (dwLoop > 0) { // it can also base on dwIndex
    dwLoop--;
    //pMaze->m_pStarPath[dwLoop].X = pMaze->Knots[dwIndex].Spot.X;
    //pMaze->m_pStarPath[dwLoop].Y = pMaze->Knots[dwIndex].Spot.Y;
    pMaze->Spots[dwLoop] = pMaze->Knots[dwIndex].Spot;
    dwIndex = pMaze->Knots[dwIndex].Prev; // parent id
    //sprintf(szText, "the %d cell:[%d,%d] added to path.", i, lpRoad[i].X, lpRoad[i].Y);
  }
  return dwValue; // return cost to get here
}
int fnAlphaProp(PMAZE pMaze, UINT dwPropId, long dwIndex)
{
  VAR v;
  if (pMaze == NULL) {
    return -1;
  }
  v.pValue = (void*)pMaze;
  v.Value32 += dwPropId;
  if (dwIndex < 0) {
    return (int) * v.pValue31; // ֱ�Ӷ�λ��Ա
  }
  v.Value32 = *v.pValue32;
  v.Value32 += dwIndex;
  return (int) * v.pValue31;
}
// ����һ������, �ͷ���Դ
int fnAlphaExit(PMAZE pMaze, UINT dwFlags)
{
  if (pMaze) {
    //SafeFree(pMaze->Spots);
    //pMaze->Value = 0;
    //SafeFree(pMaze->Marks);
    //pMaze->Scene = NULL; // �ⲿ����ָ��
    //SafeFree(pMaze->Matrix);
    //SafeFree(pMaze->Knots);
    //pMaze->Count = 0;
    free(pMaze);
    //pMaze = NULL;
  }
  return 1;
}

