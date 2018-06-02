// ****************************************************************************************************
// �ļ�: box_scan.cpp
// ע��:
// ɨ��������㷨ģ��, ��ѭ������ݹ�, �ӽ�ɫλ�ÿ�ʼ��䳡����ͨ�е�Ԫ
// ****************************************************************************************************
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
// ɨ�������(��ѭ������ݹ�, ��ұ����ڱ߽��յ�������)
int fnStageScan(PQUEUE pQueue, PSTAGE pStage)
{
  UINT x0, xl, xr, y0, xid;
  UINT flag, c;
  PSTACK s;
  PSPOT p;
  union {
    UINT* pData;
    BYTE* pNum;
  };
  UINT X, Y;
  int i;
  //if((pStage->Flags & SGF_BASIC) == 0)
  //{
  // return 0; // �½����������޷����(�ⲿ����)
  //}
  // �������������λ
  Y = pStage->SizeX * pStage->SizeY;
  X = Y % 4;
  pNum = pStage->Matrix;
  while (X--) {
    *pNum++ &= SMT_FILTER; // �����������Ϣ
  }
  Y /= 4;
  while (Y--) {
    *pData++ &= SMT_MATRIX; // �����������Ϣ
  }
  // ��ն�ջ, ������ջ
  s = pQueue->Stacks;
  p = s->Stars;
  p->X = pStage->PosX;
  p->Y = pStage->PosY;
  c = 1; // ��ջʹ��һ��
  while (c) {
    X = p->X;
    Y = p->Y;
    p--;
    c--;
    pNum = &pStage->Matrix[Y * pStage->SizeX + X];
    *pNum |= SMT_OPENED; // Me.PSet (x0, Y), newvalue
    x0 = X + 1;
    pNum++;
    // ����ұ߲�������Ҳ���Ǳ߽�ĵ�Ԫ
    while ((*pNum & SMT_MASKED) == 0) { // Me.Point(x0, Y) <> boundaryvalue
      //if(x0 >= pStage->SizeX) break; // �����ұ�(��ͼ����)
      *pNum |= SMT_OPENED;
      pNum++;
      x0++;
    }
    xr = x0 - 1; // ��������
    x0 = X - 1;
    pNum = &pStage->Matrix[Y * pStage->SizeX + x0];
    // �����߲�������Ҳ���Ǳ߽�ĵ�Ԫ
    while ((*pNum & SMT_MASKED) == 0) { // Me.Point(x0, Y) <> boundaryvalue
      //if(x0 < 0) break; // �������(��ͼ����)
      *pNum |= SMT_OPENED;
      pNum--;
      x0--;
    }
    xl = x0 + 1; // ��������
    // �����һ��ɨ���ߺ���һ��ɨ���ߣ������ڷǱ߽���δ�������أ���ѡȡ������������������������ջ��
    y0 = Y;
    for (i = 1; i >= -1; i -= 2) {
      x0 = xr;
      Y = y0 + i;
      while (x0 >= xl) {
        flag = 0; // ���󴫵�δ���ĵ�ֱ���߽�, ��¼���һ�����X����
        pNum = &pStage->Matrix[Y * pStage->SizeX + x0]; // c = Me.Point(x0, Y)
        //while(((*pNum & SMT_MASKED) == 0) && ((*pNum & SMT_OPENED) == 0) && (x0 >= xl))
        while (((*pNum & SMT_OPNMSK) == 0) && (x0 >= xl)) {
          // (c <> boundaryvalue) And (c <> newvalue) And (x0 >= xl)
          if (flag == 0) {
            flag = 1;
            xid = x0;
          }
          pNum--; // c = Me.Point(x0, Y)
          x0--;
        }
        // �����Ҳ���������ѹ��ջ��, if(c >= s.Count)
        if (flag == 1) {
          p++;
          p->X = xid;
          p->Y = Y;
          c++; // s.push(Point(xid,y));
          flag = 0;
        }
        // ��鵱ǰ������Ƿ��жϣ������жϣ�Ѱ���󷽵�һ�����������
        pNum = &pStage->Matrix[Y * pStage->SizeX + x0]; // c = Me.Point(x0, Y)
        while (*pNum & SMT_OPNMSK) {
          // (c = boundaryvalue) Or (c = newvalue) '�жϵ�ǰ���Ƿ�Ϊ�߽������ �� �жϵ�ǰ���Ƿ�Ϊ������
          if (x0 == 0) {
            break; // �������(...)
          }
          pNum--;
          x0--; // ����ǰ��Ϊ�߽��������㣬����ǰ����жϣ���ǰ���Ȼδ������߽磬��ǰ�������ƶ�
        }
      } // loop while(x0 >= xl)
    } // next for(i = 1; i >= -1; i -= 2)
  } // loop while(!s.isempty())
  return 1;
}

