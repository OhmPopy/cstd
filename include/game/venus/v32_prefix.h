// ****************************************************************************************************
// �ļ�: prefix.h
// ע��:
// �ڲ�����, ����__V32_DLL_API_�ٰ������ļ�, �ڲ���������ٰ���subfix.h
// ****************************************************************************************************
#ifndef __V32_PFX_H_
#define __V32_PFX_H_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

#ifdef __V32_SFX_H_
#undef __V32_SFX_H_
#endif

#ifdef __V32_MACRO_     // �ڲ������ĺ�(���ļ�ͷ�ڲ�ʹ��)
#error "__V32_MACRO_ is reserved for Valiant header files!\r\n"
#endif

#ifdef VFC          // Valiant foudation class
#define __V32_MACRO_ VFC  // �ݴ��
#undef VFC          // ȡ����
#endif

#ifndef __V32_DLL_API_          // �ڲ������ʶ
#define VFC    // .DLL�ⲿ����
#else
#define VFC    // .DLL�ڲ�����
#endif

#ifdef V32API
#undef V32API       // ȡ����
#endif

#ifndef V32API
#ifdef __cplusplus      // .cpp�Զ�����
#ifndef __V32_DLL_API_          // �ڲ������ʶ(VFC�ļ�ͷ�ⱻ�滻, ������ȷչ��)
#define V32API
#else
#define V32API
#endif
#else
#ifndef __V32_DLL_API_          // �ڲ������ʶ(VFC�ļ�ͷ�ⱻ�滻, ������ȷչ��)
#define V32API
#else
#define V32API
#endif
#endif
#endif

#endif
