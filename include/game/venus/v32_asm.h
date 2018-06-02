// ****************************************************************************************************
// �ļ�: asm.h
// ע��:
// ��ຯ��ģ��, C����û�к�������,  "C" ��Ч
// ****************************************************************************************************
#ifndef __V32_ASM_H_
#define __V32_ASM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CRITICAL_SECTION cs;     // ȫ�ֱ������

CBase** g_ppObjects;     // ȫ�ֶ���ָ���
UINT g_dwObjects;

// __declspec(naked) for implementation only

// v32.cpp
void fnTableInit();
void fnTableLock();
void fnTableLoop();
void fnTableExit();

// asm_index.cpp

// asm_strlen.cpp

// asm_unique.cpp
int  fnArrayUniqueDummy(void* lpVoid, UINT dwSize, void* lpRefer);

// asm_memcpy.cpp

// asm_memset.cpp

// asm_sort.cpp
int  fnArraySortDummy(void* lpVoid, UINT dwSize, void* lpIndex);

#endif