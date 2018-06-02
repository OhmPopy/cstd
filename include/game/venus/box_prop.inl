// ****************************************************************************************************
// �ļ�: box_hash.cpp
// ע��:
// ���������Լ���ģ��, ���ݳ����������ָ�����ڵĽṹ������
// ****************************************************************************************************
#include <malloc.h> // malloc, realloc, free
//#include <stdio.h> // FILE
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
#include "debug.h"
using namespace Valiant::Cipher;
// globals
CRC32 g_Hash = {{0}, 0};
UINT fnStageHash(PSPOT pStar, PSPOT pSort, UINT dwCount)
{
  //if(pStar == NULL || pSort == NULL || dwCount == 0)
  //{
  // return 0; // �ⲿ����ָ����Ч
  //}
  // ����pStar��pSort(ָ����ͬ��ֱ������)
  fnStageSort(pStar, NULL, pSort, dwCount);
  if (g_Hash.Seed == 0) {
    // �Զ���ʼ��CRC��ʽ��
    InitCyclicTable(g_Hash.Table, CRC_INIT_SEED);
    g_Hash.Seed = CRC_INIT_SEED;
  }
  return CalcCyclicValue(g_Hash.Table, pSort, sizeof(SPOT) * dwCount);
}
int fnStageProp(PBYTE pMatrix, PPROP pProp)
{
  UINT dwLoopX;
  UINT dwLoopY;
  long dwRoles;
  UINT dwNum1; // ��������
  UINT dwNum2; // Ŀ������
  UINT dwNum3; // ��λ����
  PSPOT pStars, ps;
  BYTE bValue;
  if (pProp == NULL) {
    fnStageCode(SEC_PARAM_NULL);
    return 0; // ��Ч����
  }
  // ��ʼ��û�г�������
  pProp->Count = 0;
  pProp->Value = 0;
  pProp->Hash = 0;
  pProp->RoleX = 0;
  pProp->RoleY = 0;
  pProp->Spots = NULL;
  if (pMatrix == NULL) {
    return 1; // û�г�������, �������(�������)
  }
  // ��ʼ��������
  dwNum1 = dwNum2 = dwNum3 = dwRoles = 0;
  pStars = NULL;
  for (dwLoopY = 0; dwLoopY < pProp->SizeY; dwLoopY++) {
    // ÿһ��
    for (dwLoopX = 0; dwLoopX < pProp->SizeX; dwLoopX++) {
      // ÿһ��
      bValue = pMatrix[dwLoopY * pProp->SizeX + dwLoopX];
      if (bValue & SMT_PORTER) {
        // ��ɫλ��
        if (dwRoles > 0) {
          SafeFree(pStars);
          fnStageCode(SEC_STAGE_ROLE);
          return -1; // ���ֶ����ɫ(����������������)
        }
        pProp->RoleX = dwLoopX;
        pProp->RoleY = dwLoopY;
        dwRoles++;
        if (bValue & SMT_TARGET) {
          dwNum2++; // ��ɫĿ���ص�
        }
        continue; // ��ɫ�������������ص�
      }
      if (bValue & SMT_OBJECT) {
        // ����λ��
        if (pStars == NULL) {
          // ��һ������
          ps = (PSPOT)malloc(sizeof(SPOT) * 2);
          if (ps == NULL) {
            //SafeFree(pStars);
            fnStageCode(SEC_CACHE_NULL);
            return -2; // �ڴ治��(�״�����, �����ͷ�)
          }
        }
        else {
          // ����������
          ps = (PSPOT)realloc(pStars, sizeof(SPOT) * (dwNum1 + 1) * 2);
          if (ps == NULL) {
            SafeFree(pStars);
            fnStageCode(SEC_CACHE_NULL);
            return -3; // �ڴ治��(���Ժϲ�)
          }
        }
        ps[dwNum1].X = dwLoopX;
        ps[dwNum1].Y = dwLoopY;
        pStars = ps; // Update
        // ����
        dwNum1++; // ������������
        if (bValue & SMT_TARGET) {
          dwNum2++; // ����Ŀ������
          dwNum3++; // ���ӹ�λ����
          continue; // ��λ����Ŀ��, ���ټ�ⵥ��Ŀ��
        }
      }
      if (bValue & SMT_TARGET) {
        dwNum2++; // ����Ŀ������
      }
    }
  }
  if (dwNum1 != dwNum2 || dwNum1 <= 0) {
    SafeFree(pStars);
    fnStageCode(SEC_STAGE_VARY);
    return -4; // ���Ӻ�Ŀ�����������
  }
  // ��ֵ��������(��ɫ�����Ѿ���ֵ)
  pProp->Count = dwNum1; // dwNum2;
  pProp->Value = dwNum3;
  pProp->Hash = fnStageHash(pStars, &pStars[dwNum1], dwNum1); // Hash of sorted box position
  if ((pProp->Flags & SPF_SPOT_ARRAY) == 0) {
    SafeFree(pStars); // ��������������
    return 2;
  }
  pProp->Spots = pStars;
  return 3;
}

