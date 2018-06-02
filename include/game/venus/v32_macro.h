// ****************************************************************************************************
// �ļ�: macro.h
// ע��:
// ͨ�ú궨��ģ��, ���滻��������, ��������������������
// .def�ļ���ʹ��extern "C"(����C++�������������, �����������, �����C++����ĺ�����)
// .cpp����__V32_DLL_INC_ʹ�ò�ͬ���η�(���ظ�����)
// �궨����ȫ�ֵ�, �ļ�������һ��, ���Ѷ������ٶ���, ���������ʾ��Ϣ
// ****************************************************************************************************
#ifndef __V32_MACRO_H_
#define __V32_MACRO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// *** �������η��궨�� ***

#ifndef NAKED
#define NAKED
#else
#endif

// *** ��ȫ�ͷŷ��궨�� ***

#ifndef SafeDelete
#define SafeDelete(p)   if(p){delete p; p = NULL;}
#else
#endif

#ifndef SafeErase
#define SafeErase(p)    if(p){delete[] p; p = NULL;}
#else
#endif

#ifndef SafeFree
#define SafeFree(p)     if(p){free(p); p = NULL;}
#else
#endif

#ifndef SafeRelease
#define SafeRelease(p)    if(p){p->Release(); p = NULL;}
#else
#endif

#ifndef SafeDeleteObject
#define SafeDeleteObject(p) if(p){DeleteObject(p); p = NULL;}
#else
#endif

#ifndef SafeCloseHandle
#define SafeCloseHandle(p)  if(p){CloseHandle(p); p = NULL;}
#else
#endif

// *** �������궨�� ***

// *** �ṹ����غ궨�� ***
#define V32_CACHE_FIXED   0x00000001  // cache is fixed

#define V32_CACHE_ARRAY   0x01000000  // cache is array
#define V32_CACHE_CHAR    0x00000100  // cache array is 8 bit
#define V32_CACHE_WORD    0x00000200  // cache array is 16 bit
#define V32_CACHE_UINT    0x00000300  // cache array is 32 bit
#define V32_CACHE_QWORD   0x00000400  // cache array is 64 bit, reserved

// *** �����궨��, ��Щ�겻���ظ����� ***
#define V32_PROCESS_ATTACH  1 // DLL_PROCESS_ATTACH
#define V32_THREAD_ATTACH 2 // DLL_THREAD_ATTACH
#define V32_THREAD_DETACH 3 // DLL_THREAD_DETACH
#define V32_PROCESS_DETACH  0 // DLL_PROCESS_DETACH

#define V32_MAJOR_VERSION 2 // major
#define V32_MINOR_VERSION 0 // minor
#define V32_BUILD_VERSION 10  // build
#define V32_DEBUG_VERSION 13  // update

#endif