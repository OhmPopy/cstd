// ****************************************************************************************************
// �ļ�: box_file.cpp
// ע��:
// �������ļ������������ģ��, �Լ��½��ӿ�
// ****************************************************************************************************
#include <malloc.h> // malloc, realloc, free
#include <stdio.h> // FILE
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
// �򿪳����ļ�(�ޱ����߼�, �����ô������)
PSTAGE fnStageOpen(void* lpVoid, UINT dwFlags)
{
  PSTAGE pStage;
  PBYTE pData;
  PROP prp;
  int dwRet;
  // ���ļ�����(�ߴ���ļ���ȡ)
  pData = fnStageFile(lpVoid, &prp.Size);
  if (pData == NULL) {
    return NULL; // �ļ���
  }
  // ���㳡������
  prp.Flags = SPF_SPOT_ARRAY; // Ҫ�󷵻���������
  dwRet = fnStageProp(pData, &prp);
  if (dwRet <= 0) { // <= 1
    SafeFree(pData);
    SafeFree(prp.Spots);
    return NULL; // �����쳣
  }
  // ���볡���ڴ�(�½��հ׳����������)
  pStage = fnStageAlloc(pData, &prp); // ���г�Ա������ֵ
  SafeFree(pData);
  SafeFree(prp.Spots);
  if (pStage == NULL) {
    return NULL; // �ڴ治��
  }
  return pStage; // ����ָ��
}
// �����µĳ���(�ޱ����߼�, �����ô������)
PSTAGE fnStageApply(void* lpVoid, PSPOT pSpot, UINT dwFlags)
{
  PSTAGE pStage;
  PBYTE pData;
  PROP prp;
  int dwRet;
  // ���ڴ洴��(�ߴ������Ч)
  prp.Flags = 0; //V32Clear32(&prp, 0, sizeof(PROP));
  prp.Size.Value = pSpot->Value;
  pData = (PBYTE)lpVoid;
  if (pData != NULL) {
    prp.Flags = SPF_SPOT_ARRAY; // Ҫ�󷵻���������
  }
  // ���㳡������
  dwRet = fnStageProp(pData, &prp);
  if (dwRet <= 0) {
    //SafeFree(pData);
    SafeFree(prp.Spots);
    return NULL; // �����쳣
  }
  // ���볡���ڴ�(�½��հ׳����������)
  pStage = fnStageAlloc(pData, &prp); // ���г�Ա������ֵ
  //SafeFree(pData);
  SafeFree(prp.Spots);
  if (pStage == NULL) {
    return NULL; // �ڴ治��
  }
  return pStage; // ����ָ��
}
// �洢�������ļ�
int fnStageSave(PSTAGE pStage, void* lpVoid, UINT dwFlags)
{
  UINT dwNum;
  FILE* pfx;
  if (pStage == NULL) {
    fnStageCode(SEC_PARAM_NULL);
    return 0; // ��Ч����
  }
  if ((pStage->Flags & SGF_BASIC) == 0) {
    fnStageCode(SEC_STAGE_SAVE);
    return -1; // �½��������ܱ���
  }
  // ���泡��
  pfx = fopen((const char*)lpVoid, "wb");
  if (pfx == NULL) {
    fnStageCode(SEC_FILE_OPEN);
    return -2; // �ļ���ʧ��
  }
  dwNum = SFH_HEADER;
  fwrite(&dwNum, sizeof(UINT), 1, pfx);
  dwNum = 0;
  fwrite(&dwNum, sizeof(UINT), 1, pfx);
  fwrite(&pStage->SizeX, sizeof(UINT), 1, pfx);
  fwrite(&pStage->SizeY, sizeof(UINT), 1, pfx);
  dwNum = pStage->SizeX * pStage->SizeY;
  fwrite(pStage->Matrix, sizeof(BYTE), dwNum, pfx);
  fclose(pfx);
  return 1;
}

