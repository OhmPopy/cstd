// ****************************************************************************************************
// �ļ�: debug.cpp
// ע��:
// �����ӵ��Ժ���ģ��, ������빦��, ���԰汾���������Ϣ������Ŀ¼�µ�debug.txt
// ****************************************************************************************************
#include <stdio.h> // FILE, vsprintf
#include <stdarg.h> // va_start, va_end
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
#include "debug.h"
using namespace Valiant::Gaming;
using namespace Valiant;
// globals
char g_szText[256] = {'\0'};
UINT g_dwCode = 0;
#ifdef _DEBUG
const char* g_szTitle[] = {"��", "��", "��", "��"};
FILE* pfo = NULL;
#endif
UINT fnStageCode(UINT dwCode)
{
  UINT dwNum = g_dwCode;
  if (dwCode != SEC_ERROR_DUMP) {
    g_dwCode = dwCode;
  }
  return dwNum; // ���ؾɵĴ���
}
#ifdef _DEBUG
int fnDebugInit()
{
  if (pfo) {
    fnDebugExit();
  }
  pfo = fopen("debug.txt", "wb");
  if (pfo == NULL) {
    return 1;
  }
  return 0;
}
const char* fnGetText(unsigned int dwFlags)
{
  return g_szTitle[dwFlags];
}
int fnPrint(const char* szText, ...)
{
  va_list args;
  int ret;
  va_start(args, szText); // ��һ������Ϊָ��ɱ�����ַ�ָ��ı���,�ڶ��������ǿɱ�����ĵ�һ������,ͨ������ָ���ɱ�����б��в����ĸ���
  if (pfo) {
    ret = vfprintf(pfo, szText, args);
  }
  va_end(args); // ����ſɱ�����ַ����ı������
  return ret;
}
void fnPrintBox(PSTAGE pStage)
{
  PSPOT ps = pStage->Stars;
  for (UINT dwLoop = 0; dwLoop < pStage->Count; dwLoop++) {
    fnPrint("\tBox%d=(%d, %d)", dwLoop, ps->X, ps->Y);
    ps++;
  }
  fnPrint("\r\n");
}
void fnPrintStage(PSTAGE pStage, int printTrees)
{
  long i, j;
  while (pStage) {
    fnPrint("Stage: 0x%08X:\r\n", pStage);
    for (j = 0; j < pStage->SizeY; j++) {
      for (i = 0; i < pStage->SizeX; i++) {
        //switch(pStage->Matrix[j * pStage->SizeX + i] & SMT_FILTER){
        //case SMT_UNKNOW:
        //case SMT_OBJECT:
        //case SMT_TARGET:
        //case SMT_BORDER:
        //case SMT_PORTER:
        //}
        fnPrint(" %02X", pStage->Matrix[j * pStage->SizeX + i] & SMT_FILTER);
      }
      fnPrint("\r\n");
    }
    fnPrint("\r\n");
    if (printTrees) {
      pStage = pStage->Host;
    }
  }
}
void fnOutput()
{
  if (pfo) {
    fflush(pfo);
  }
}
int fnDebugExit()
{
  if (pfo) {
    fclose(pfo);
    pfo = NULL;
  }
  return 0;
}
#endif

