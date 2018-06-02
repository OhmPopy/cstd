// ****************************************************************************************************
// �ļ�: box_motion.cpp
// ע��:
// �������ƶ��㷨ģ��, �жϻ�ִ��ĳ�����ӵ��ƶ�, ���ٳ���, ����������������ʵ��
// Ѱ·��һ�ָ��ŷ�����, ���뻺��ʱ����ÿ����Ԫ, Ѱ·ֻҪ�жϸõ�Ԫ�Ƿ�򿪼���
// ****************************************************************************************************
//#include <malloc.h>
//#include <stdio.h>
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
int fnStagePush(PSTAGE pStage, PMOVE pMove, UINT dwFlags)
{
  SQUARE squ;
  PSPOT ps;
  //if((pStage->Flags & SGF_READY) != SGF_READY)
  //{
  // return 0; // ����δ����������Ч����(�ⲿ����)
  //}
  ps = &pStage->Stars[pMove->Index]; // ��Ҫֱ�Ӳ������ָ��
  if (dwFlags == SMF_MOVE_NONE) { // Ӧ�ö���
    pStage->Position = pMove->Porter;
    pStage->Automation = pMove->Movement;
    *ps = pMove->Object;
    // Ӧ�ö������ɨ��, �Զ����������λ(ע�����!!)
#ifdef _DEBUG
    //if(pMove->PortX == 3 && pMove->PortY == 5)
    //{
    // squ.Value = 0; // ���Զϵ�
    //}
#endif
    //pStage->Matrix[pMove->RoleId] = pMove->Role; // ��ɫ���뿪(����Ŀ����������ǽ�ɫλ��)
    //pStage->Matrix[pMove->CubeId] = pMove->Cube; // ���ӱ�����(��ɫ����)
    //pStage->Matrix[pMove->DestId] = pMove->Dest; // ���ӽ�Ŀ��
    pStage->Matrix[pMove->RoleId] ^= SMT_PORTER;
    pStage->Matrix[pMove->CubeId] |= SMT_PORTER;
    pStage->Matrix[pMove->CubeId] ^= SMT_OBJECT;
    pStage->Matrix[pMove->DestId] |= SMT_OBJECT;
    pStage->Value = pMove->Value;
    return 2; //break;
  }
  pMove->CubeId = ps->Y * pStage->SizeX + ps->X; // ���ӵĵ�Ԫ����
#ifdef _DEBUG
  //if((pStage->Matrix[pMove->CubeId] & SMT_OBJECT) == 0)
  //{
  // fnPrint("[����]Ŀ�����Ӳ�����!\r\n");
  // fnPrintStage(pStage, 1);
  // fnOutput();
  //}
#endif
  pMove->Porter = *ps; // �ƶ����ɫ������
  squ.Value = 0;
  switch (dwFlags) {
  case SMF_MOVE_RISE: // ��
    pMove->MoveX = ps->X;
    pMove->MoveY = ps->Y + 1;
    if (pStage->Position.Value == pMove->Movement.Value) {
      pMove->Movement.Value = 0; // ��ɫ�������ƶ�λ����
    }
    pMove->Object.X = ps->X; // �ƶ������ӵ�����
    pMove->Object.Y = ps->Y - 1;
    pMove->DestId = pMove->CubeId - pStage->SizeX;
    pMove->Move = pStage->Matrix[pMove->CubeId + pStage->SizeX]; // �ƶ�λ��
    squ.Left = pStage->Matrix[pMove->DestId - 1];
    squ.Right = pStage->Matrix[pMove->DestId + 1];
    squ.DownHill = pStage->Matrix[pMove->DestId - pStage->SizeX - 1];
    squ.Front = pStage->Matrix[pMove->DestId - pStage->SizeX];
    squ.UpHill = pStage->Matrix[pMove->DestId - pStage->SizeX + 1];
    break;
  case SMF_MOVE_DROP: // ��
    pMove->MoveX = ps->X;
    pMove->MoveY = ps->Y - 1;
    if (pStage->Position.Value == pMove->Movement.Value) {
      pMove->Movement.Value = 0; // ��ɫ�������ƶ�λ����
    }
    pMove->Object.X = ps->X; // �ƶ������ӵ�����
    pMove->Object.Y = ps->Y + 1;
    pMove->DestId = pMove->CubeId + pStage->SizeX;
    pMove->Move = pStage->Matrix[pMove->CubeId - pStage->SizeX]; // �ƶ�λ��
    squ.Left = pStage->Matrix[pMove->DestId + 1];
    squ.Right = pStage->Matrix[pMove->DestId - 1];
    squ.DownHill = pStage->Matrix[pMove->DestId + pStage->SizeX + 1];
    squ.Front = pStage->Matrix[pMove->DestId + pStage->SizeX];
    squ.UpHill = pStage->Matrix[pMove->DestId + pStage->SizeX - 1];
    break;
  case SMF_MOVE_BACK: // ��
    pMove->MoveX = ps->X + 1;
    pMove->MoveY = ps->Y;
    if (pStage->Position.Value == pMove->Movement.Value) {
      pMove->Movement.Value = 0; // ��ɫ�������ƶ�λ����
    }
    pMove->Object.X = ps->X - 1; // �ƶ������ӵ�����
    pMove->Object.Y = ps->Y;
    pMove->DestId = pMove->CubeId - 1;
    pMove->Move = pStage->Matrix[pMove->CubeId + 1]; // �ƶ�λ��
    squ.Left = pStage->Matrix[pMove->DestId + pStage->SizeX];
    squ.Right = pStage->Matrix[pMove->DestId - pStage->SizeX];
    squ.DownHill = pStage->Matrix[pMove->DestId + pStage->SizeX - 1];
    squ.Front = pStage->Matrix[pMove->DestId - 1];
    squ.UpHill = pStage->Matrix[pMove->DestId - pStage->SizeX - 1];
    break;
  case SMF_MOVE_FORE: // ��
    pMove->MoveX = ps->X - 1;
    pMove->MoveY = ps->Y;
    if (pStage->Position.Value == pMove->Movement.Value) {
      pMove->Movement.Value = 0; // ��ɫ�������ƶ�λ����
    }
    pMove->Object.X = ps->X + 1; // �ƶ������ӵ�����
    pMove->Object.Y = ps->Y;
    pMove->DestId = pMove->CubeId + 1;
    pMove->Move = pStage->Matrix[pMove->CubeId - 1]; // �ƶ�λ��
    squ.Left = pStage->Matrix[pMove->DestId - pStage->SizeX];
    squ.Right = pStage->Matrix[pMove->DestId + pStage->SizeX];
    squ.DownHill = pStage->Matrix[pMove->DestId - pStage->SizeX + 1];
    squ.Front = pStage->Matrix[pMove->DestId + 1];
    squ.UpHill = pStage->Matrix[pMove->DestId + pStage->SizeX + 1];
    break;
  default:
    return 0; //break;
  }
  if (pMove->Move & SMT_MASKED) {
    return -1; // �ƶ�λ�������ӻ�߽�
  }
  if (pStage->Matrix[pMove->DestId] & SMT_MASKED) {
    return -2; // Ŀ��λ�������ӻ�߽�
  }
  if ((pMove->Move & SMT_OPENED) == 0) {
    return -3; // �ƶ�λ�ò���Ѱ·����
  }
  squ.Core = pStage->Matrix[pMove->DestId]; // �ƶ�������λ��
  if (fnStageTrapped(&squ) != 0) {
    return -4; // �����߷�������ֽ���(���ı�squ����)
  }
  pMove->RoleId = pStage->PosY * pStage->SizeX + pStage->PosX; // �ƶ�ǰ��ɫ�ĵ�Ԫ����
  // �趨�������
  pMove->Dest = pStage->Matrix[pMove->DestId]; // squ.Core
  pMove->Dest |= SMT_OBJECT; // ���ӽ���
  pMove->Cube = pStage->Matrix[pMove->CubeId];
  pMove->Cube ^= SMT_OBJECT; // �����뿪
  pMove->Cube |= SMT_PORTER; // ��ɫ����
  //pMove->Role = pStage->Matrix[pMove->RoleId];
  //pMove->Role ^= SMT_PORTER; // ��ɫ�뿪
  pMove->Value = pStage->Value; // �ƶ�ǰ�Ĺ�λ����
  if (pMove->Cube & SMT_TARGET) {
    pMove->Value--;
  }
  if (pMove->Dest & SMT_TARGET) {
    pMove->Value++;
  }
  return 1;
}
// ********** ���º�����Ϸר�� ********
// ��Ϸ�ƶ�
int fnStageProbe(PSTAGE pStage, UINT dwFlags)
{
  //int dwRet;
  BYTE* pNum;
  if ((pStage->Flags & SGF_BASIC) == 0) {
    fnStageCode(SEC_STAGE_GAME);
    return -1; // �½�����������Ч����
  }
  pNum = &GetMatrix(pStage, pStage->PosX, pStage->PosY);
  if (dwFlags & SMF_MOVE_TEST) {
    // �����ƶ�
    switch (dwFlags & SMF_MOVE_MASK) {
    case SMF_MOVE_RISE: // ��
      pNum -= pStage->SizeX;
      if (*pNum & SMT_BORDER) {
        return 0; // ��ס�߽�
      }
      if (*pNum & SMT_OBJECT) {
        pNum -= pStage->SizeX;
        if (*pNum & SMT_MASKED) {
          return -1; // ����
        }
        return 2; // �ƶ�������
      }
      break;
    case SMF_MOVE_DROP: // ��
      pNum += pStage->SizeX;
      if (*pNum & SMT_BORDER) {
        return 0; // ��ס�߽�
      }
      if (*pNum & SMT_OBJECT) {
        pNum += pStage->SizeX;
        if (*pNum & SMT_MASKED) {
          return -1; // ����
        }
        return 2; // �ƶ�������
      }
      break;
    case SMF_MOVE_BACK: // ��
      pNum--;
      if (*pNum & SMT_BORDER) {
        return 0; // ��ס�߽�
      }
      if (*pNum & SMT_OBJECT) {
        pNum--;
        if (*pNum & SMT_MASKED) {
          return -1; // ����
        }
        return 2; // �ƶ�������
      }
      break;
    case SMF_MOVE_FORE: // ��
      pNum++;
      if (*pNum & SMT_BORDER) {
        return 0; // ��ס�߽�
      }
      if (*pNum & SMT_OBJECT) {
        pNum++;
        if (*pNum & SMT_MASKED) {
          return -1; // ����
        }
        return 2; // �ƶ�������
      }
      break;
    default:
      return -1;
    }
    return 1; // ����
  }
  // ��ʽ�ƶ�(�����ټ��һ��, ����ֱ�ӵݹ�)
  *pNum ^= SMT_PORTER; // ��ɫ�뿪
  switch (dwFlags & SMF_MOVE_MASK) {
  case SMF_MOVE_RISE: // ��
    pStage->PosY--;
    pNum -= pStage->SizeX;
    *pNum |= SMT_PORTER; // ��ɫ����
    if (*pNum & SMT_OBJECT) {
      *pNum ^= SMT_OBJECT; // �����뿪
      if (*pNum & SMT_TARGET) {
        pStage->Value--;
      }
      pNum -= pStage->SizeX;
      *pNum |= SMT_OBJECT; // ���ӽ���
      if (*pNum & SMT_TARGET) {
        pStage->Value++;
      }
      return 2; // �ƶ�������
    }
    break;
  case SMF_MOVE_DROP: // ��
    pStage->PosY++;
    pNum += pStage->SizeX;
    *pNum |= SMT_PORTER; // ��ɫ����
    if (*pNum & SMT_OBJECT) {
      *pNum ^= SMT_OBJECT; // �����뿪
      if (*pNum & SMT_TARGET) {
        pStage->Value--;
      }
      pNum += pStage->SizeX;
      *pNum |= SMT_OBJECT; // ���ӽ���
      if (*pNum & SMT_TARGET) {
        pStage->Value++;
      }
      return 2; // �ƶ�������
    }
    break;
  case SMF_MOVE_BACK: // ��
    pStage->PosX--;
    pNum--;
    *pNum |= SMT_PORTER; // ��ɫ����
    if (*pNum & SMT_OBJECT) {
      *pNum ^= SMT_OBJECT; // �����뿪
      if (*pNum & SMT_TARGET) {
        pStage->Value--;
      }
      pNum--;
      *pNum |= SMT_OBJECT; // ���ӽ���
      if (*pNum & SMT_TARGET) {
        pStage->Value++;
      }
      return 2; // �ƶ�������
    }
    break;
  case SMF_MOVE_FORE: // ��
    pStage->PosX++;
    pNum++;
    *pNum |= SMT_PORTER; // ��ɫ����
    if (*pNum & SMT_OBJECT) {
      *pNum ^= SMT_OBJECT; // �����뿪
      if (*pNum & SMT_TARGET) {
        pStage->Value--;
      }
      pNum++;
      *pNum |= SMT_OBJECT; // ���ӽ���
      if (*pNum & SMT_TARGET) {
        pStage->Value++;
      }
      return 2; // �ƶ�������
    }
    break;
  default:
    return -1;
  }
  return 1; // ����
}

