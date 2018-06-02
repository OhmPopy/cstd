// ****************************************************************************************************
// �ļ�: box_trapped.cpp
// ע��:
// �����ӽ����㷨ģ��, ���ɽ��־Ź�������, ���ּ�⺯��ʵ��
// ͼʾ:
// ��=��� ��=�߽� ��=���� ��=���� ��=���ӻ�߽�
// ****************************************************************************************************
//#include <malloc.h>
//#include <stdio.h>
#define __VENUS_DLL_INC_ // DLL�ڲ�����
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
int fnStageTrapped(PSQUARE ps)
{
  if (ps->Core & SMT_TARGET) {
    // *** Ŀ���ѹ�λ ***
    if (ps->Front & SMT_OBJECT) {
      // ��ǰ�и�����
      if ((ps->Front & SMT_TARGET) == 0) {
        // ����δ��λ
        if (ps->Left & SMT_MASKED) {
          if (ps->DownHill & SMT_MASKED) {
            return 1;
          }
        }
        if (ps->Right & SMT_MASKED) {
          if (ps->UpHill & SMT_MASKED) {
            return 2;
          }
        }
      }
    }
    if (ps->Left & SMT_OBJECT) {
      // ����и�����
      if ((ps->Left & SMT_TARGET) == 0) {
        // ����δ��λ
        if (ps->Front & SMT_MASKED) {
          if (ps->DownHill & SMT_MASKED) {
            return 3;
          }
        }
      }
    }
    if (ps->Right & SMT_OBJECT) {
      // �ұ��и�����
      if ((ps->Right & SMT_TARGET) == 0) {
        // ����δ��λ
        if (ps->Front & SMT_MASKED) {
          if (ps->UpHill & SMT_MASKED) {
            return 4;
          }
        }
      }
    }
    if (ps->DownHill & SMT_OBJECT) {
      // ��ǰ�и�����
      if ((ps->DownHill & SMT_TARGET) == 0) {
        // ����δ��λ
        if (ps->Front & SMT_MASKED) {
          if (ps->Left & SMT_MASKED) {
            return 5;
          }
        }
      }
    }
    if (ps->UpHill & SMT_OBJECT) {
      // ��ǰ�и�����
      if ((ps->UpHill & SMT_TARGET) == 0) {
        // ����δ��λ
        if (ps->Front & SMT_MASKED) {
          if (ps->Right & SMT_MASKED) {
            return 6;
          }
        }
      }
    }
    // û��δ��λ�ҿ�ס������
  }
  else {
    // *** Ŀ��δ��λ ***
    if (ps->Front & SMT_MASKED) {
      // ��ǰ�߽������
      if (ps->Front & SMT_BORDER) {
        // ��ǰ�߽�
        if (ps->Left & SMT_BORDER) {
          return 10; // ��ǰ��ס
        }
        if (ps->Left & SMT_OBJECT) {
          if (ps->DownHill & SMT_MASKED) {
            return 11;
          }
        }
        if (ps->Right & SMT_BORDER) {
          return 20; // ��ǰ��ס
        }
        if (ps->Right & SMT_OBJECT) {
          if (ps->UpHill & SMT_MASKED) {
            return 21;
          }
        }
      }
      else {
        // ��ǰ����
        if (ps->Left & SMT_MASKED) {
          if (ps->DownHill & SMT_MASKED) {
            return 31;
          }
        }
        if (ps->Right & SMT_MASKED) {
          if (ps->UpHill & SMT_MASKED) {
            return 32;
          }
        }
      }
    }
    // ���Լ���ǰ��
  }
  return 0;
}

