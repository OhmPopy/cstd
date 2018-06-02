// ****************************************************************************************************
// �ļ�: VRC32\\api.h
// ע��:
// �ӿڶ���ģ��, �����������ͺ����ӿ�
// ****************************************************************************************************
#ifndef __VRC32_H_
#define __VRC32_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// auto include
#include "v32_api.h"

#ifdef __VRC32_DLL_INC_
#define __V32_DLL_API_   // �ڲ������ʶ
#endif
#include "v32_prefix.h" // Ԥ�����, �趨�ڲ�VFC��

// *** �����ռ�ΪC++����, �������������� ***
#pragma pack(1)
typedef struct tagHash {
  UINT Flags;
  union {
    struct { // CRC32
      UINT Seed;  // ����, ����������ʽ��
      UINT Value;  // ��ֵ, ��ʼ����Ӧ����
      UINT Table[256]; // ��ʽ��
    };
    struct { // Base64
      BYTE Base[64];
      BYTE Tail;
    };
    struct { // MDA5
      UINT Count;  // ����, �Ѽ�����ֽ���
      //UINT A, B, C, D; // ��ֵ, �Ƚ�ֵ
      UINT A;
      UINT B;
      UINT C;
      UINT D;
      //UINT a, b, c, d; // ���, �Ĵ���
      UINT a;
      UINT b;
      UINT c;
      UINT d;
      union {
        UINT Size[2];
        __int64 Volume; // ��λ����(��������ִ��������)
      };
      union {
        BYTE Data[64]; // ������
        UINT Dummy[16];
      };
    };
    struct { // Random
      UINT Prime;  // �Ѿ��������һ��������(Ԥ��δ��)
      UINT RndSeed; // f(0) = seed, f(i) = (f(i - 1) * a + b) % c
      UINT RndValue;
      UINT RndSlope; // k, a
      UINT RndOffset; // Intercept, b
      UINT RndModulo; // c
    };
  };
} HASH, *PHASH;
#pragma pack()

// Hash struct flags
#define HSF_FLAG_NONE 0x00000000  // ...
#define HSF_FLAG_INIT 0x00000001  // CRC struct initialized
#define HSF_FLAG_MASK 0x000000FF  // mask

#define HSF_CALC_INIT 0x00000100  // ��ʼ��
#define HSF_CALC_COPY 0x00000200  // д����
#define HSF_CALC_HASH 0x00000400  // ����ֵ
#define HSF_CALC_TEST 0x00000800  // �Ƚ�ֵ
#define HSF_CALC_CODE 0x00001000  // ����
#define HSF_CALC_TEXT 0x00002000  // ����
#define HSF_CALC_MASK 0x0000FF00  // ����

#define HSF_TYPE_ADLER  0x00000000  // Adler32 struct
#define HSF_TYPE_CRC32  0x01000000  // CRC32 struct
#define HSF_TYPE_MDA5 0x02000000  // MD5 struct
#define HSF_TYPE_BASE 0x03000000  // Base64 struct
#define HSF_TYPE_MASK 0xFF000000  // mask

#define HSF_EDGE_ADLER  0x0000FFF1  // 65521

#define HSF_SEED_ADLER  0x00000001  // Default last value of Adler32
#define HSF_SEED_HASH 0xEDB88320  // Default CRC32 seed, ZMODEM, PKZIP
#define HSF_SEED_MPEG 0x04C11DB7  // AUTODIN II, Ethernet, FDDI, mpeg, same as 0xEDB88320 after flip, different algorithm

// Adler & CRC32 functions
V32API UINT  CRCAdler(PHASH pHash, void* pData, UINT dwSize);
V32API void  CRCMakeMpeg(void* p, UINT dwSeed);
V32API void  CRCMakeFast(void* p, UINT dwSeed);
V32API void  CRCMakeFlip(void* p, UINT dwSeed);
V32API UINT  CRCSSH(PHASH pHash, void* pData, UINT dwSize);
V32API UINT  CRCText(PHASH pHash, void* pData);
V32API UINT  CRCMPG(PHASH pHash, void* pData, UINT dwSize);
V32API UINT  CRC32(PHASH pHash, void* pData, UINT dwSize);

// Base64 functions
V32API int  BaseMake(PHASH pHash, void* pData, UINT dwFlags);
V32API int  BaseCodeEx(PHASH pHash, BYTE* pText, BYTE* pCode, UINT dwSize);
V32API int  BaseTextEx(PHASH pHash, BYTE* pCode, BYTE* pText, UINT dwSize);
V32API int  BaseCode(BYTE* pText, BYTE* pCode, UINT dwSize);
V32API int  BaseText(BYTE* pCode, BYTE* pText, UINT dwSize);
V32API int  Base64(PHASH pHash, BYTE* pImport, BYTE* pExport, UINT dwFlags);

// MD5 functions
V32API void  MDALoop(PHASH p);
V32API int  MDA5(PHASH pHash, BYTE* pData, UINT dwSize, UINT dwFlags);

// Random functions
V32API UINT  RndLoop(PHASH pHash, UINT dwSpike);
V32API UINT  RndMake(PHASH pHash, UINT dwSeed, UINT dwSlope, UINT dwOffset, UINT dwModulo);

namespace Valiant
{
namespace Cipher
{
class VFC CHash : public HASH  // Dummy class
{
public:
  CHash();
  CHash(UINT dwFlags); // Init with type, default is HSF_TYPE_ADLER
  virtual ~CHash();

  virtual int Calculate(void* lpData, void* lpArgs, UINT dwFlags);
};
}
}

#include "v32_subfix.h" // Ԥ�����, �޸��ⲿVFC��

#endif