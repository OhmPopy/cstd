// ****************************************************************************************************
// �ļ�: prefix.h
// ע��:
// �ڲ�����, ����__V32_DLL_API_�ٰ������ļ�, �ڲ���������ٰ���subfix.h
// ****************************************************************************************************
#ifndef __V32_SFX_H_
#define __V32_SFX_H_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

#ifdef __V32_PFX_H_
#undef __V32_PFX_H_
#endif

#ifdef __V32_DLL_API_
#undef __V32_DLL_API_   // ȡ���ڲ������ʶ
#endif

#ifdef __V32_MACRO_     // VFC���ļ����ж���, �ָ���
#undef VFC          // ȡ����
#define VFC __V32_MACRO_  // �ָ���
#undef __V32_MACRO_     // ȡ����
#else
#undef VFC          // VFC���ļ����޶���, ȡ����
#endif

#endif