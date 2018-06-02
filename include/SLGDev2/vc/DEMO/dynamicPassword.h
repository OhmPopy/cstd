#ifndef _DYNAMIC_PASSWORD_H_
#define _DYNAMIC_PASSWORD_H_

// ����
#define C1 52845
#define C2 22719

typedef unsigned char uchar;

int aa_encrypt(const uchar* src, int slen, int Key, uchar* dst, int maxdlen) // ���ܺ���
{
  int i;
  
  for (i = 0; i < slen; i++) {
    dst[i] = src[i] ^ (Key >> 8);
    Key = (dst[i] + Key) * C1 + C2;
  }
  
  return slen;
}

// ���ܺ���
int aa_decrypt(const uchar* src, int slen, int Key, uchar* dst, int maxdlen)
{
  int i;
  
  for (i = 0; i < slen; i++) {
    dst[i] = src[i] ^ (Key >> 8);
    Key = (src[i] + Key) * C1 + C2;
  }
  
  return slen;
}

#undef C1
#undef C2

// BKDR Hash Function
unsigned int BKDRHash(const unsigned char *str, int slen)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    int i;
 
    for (i=0; i<slen; ++i) {
        hash = hash * seed + str[i];
    }
 
    return (hash & 0x7FFFFFFF);
}

// ʱ������
typedef struct dpalg_t {
  unsigned int year, month, day, hour, minute, second;
} dpalg_t;

//����
// ����
// dp      : ʱ�� �������㣺��<16 ��<32 ʱ<24 ��<60
// data    : ������
// datalen : �������ֽڳ���
// ���
// ptt     : ָ�������������
// ����
// ʱ���ڱ�׼��Χ�ڣ�����1 ���򷵻�0
int dp_encode(const dpalg_t* dp, const uchar* data, int datalen, unsigned int* ptt)
{
  unsigned int aa=0, bb=0;
  if (dp->month<16 && dp->day<32 && dp->hour<24 && dp->minute<60) {
    bb = BKDRHash(data, datalen);
    //aa = dp->year;
    aa = aa*16 + dp->month;
    aa = aa*32 + dp->day;
    aa = aa*24 + dp->hour;
    aa = aa*60 + dp->minute;
    aa = aa*256 + (uchar)bb;
    //aa = aa*60 + dp->second;
    aa_encrypt((uchar*)&aa, 4, bb, (uchar*)ptt, 4);
    return 1;
  }
  return 0;
}

// ����
// tt      : ������
// data    : ������
// datalen : �������ֽڳ���
// ���
// dp      : ʱ��
// ����
// ������У��ɹ�����1�����򷵻�0
int dp_decode(dpalg_t* dp, const uchar* data, int datalen, unsigned int tt)
{
  unsigned int aa=0, bb=0, bb1=0;
  bb = BKDRHash(data, datalen);
  aa_decrypt((uchar*)&tt, 4, bb, (uchar*)&aa, 4);
  bb1 = aa%256, aa/=256;
  if ((uchar)bb1==(uchar)bb) {
    dp->minute = aa%60, aa/=60;
    dp->hour = aa%24, aa/=24;
    dp->day = aa%32, aa/=32;
    dp->month = aa%16, aa/=16;
    //dp->year = aa;

    return 1;
  }
  return 0;
}
/*
#ifdef _DEBUG
#include <assert.h>
#include <stdlib.h>
#include <string.h>
// ����
int test_dynamic_password()
{
  dpalg_t dp[1] = {2001, 12, 12, 12, 12, 12};
  dpalg_t dp2[1] = {0};
  char code[] = "1234ljoi1j23;l4noiuhj";
  int codelen = strlen(code);
  int i, j, k, m;
  unsigned int tt;

  dp_encode(dp, (const uchar*)code, codelen, &tt);
  dp_decode(dp2, (const uchar*)code, codelen, tt);
  for (i=0;i<12;i+=1) {
    for (j=0; j<32; j+=1) {
      for (k=0; k<24; k+=1) {
        for (m=0;m<60;m+=1) {
          dp->month = i, dp->day = j, dp->hour = k, dp->minute = m;
          dp_encode(dp, (const uchar*)code, codelen, &tt);
          if (dp_decode(dp2, (const uchar*)code, codelen, tt)) {
            if (dp->month != dp2->month || dp->day != dp2->day || dp->hour != dp2->hour || dp->minute != dp2->minute) {
              assert(0);
            }
          }
        }
      }
    }
  }
  return 0;
}

#endif
*/

#endif
