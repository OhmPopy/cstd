//  frg_writer_base.h
/*
 This is the frg copyright.
 
 Copyright (c) 2012-2013 HouSisong All Rights Reserved.
 (The MIT License)
 
 Permission is hereby granted, pfree of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:
 
 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef _LIBFRG_frg_writer_base_h
#define _LIBFRG_frg_writer_base_h

#include "frg_reader_base.h"
#include "assert.h" //assert
#include "string.h" //memset
#include <vector>
#include <algorithm>
#include <map>

#ifndef PACKED

//PACKED �������ݸ�ʽ  ���ں��ļ�������ص�����.
#ifdef _MSC_VER
#define PACKED __declspec(align(1))
#else
#ifdef __GNUC__
#define PACKED __attribute__((packed))
#else
#define PACKED
#endif
#endif

#endif

namespace frg {

inline static TUInt32 packMatchXY(TUInt32 x,TUInt32 y){
    assert((x|y)<(1<<16));
    return x|(y<<16);
}

inline static void writeUInt16(std::vector<TByte>& out_code,TUInt32 value){
    assert((value>>16)==0);
    out_code.push_back(value);
    out_code.push_back(value>>8);
}

inline static void writeUInt32(std::vector<TByte>& out_code,TUInt32 value){
    out_code.push_back(value);
    out_code.push_back(value>>8);
    out_code.push_back(value>>16);
    out_code.push_back(value>>24);
}

//�䳤32bit���������뷽��(x bit�������ͱ�־λ,x<=3),�Ӹ�λ��ʼ���1-5byte:
// x0*  7-x bit
// x1* 0*  7+7-x bit
// x1* 1* 0*  7+7+7-x bit
// x1* 1* 1* 0*  7+7+7+7-x bit
// x1* 1* 1* 1* 0*  7+7+7+7+7-x bit

static void pack32BitWithTag(std::vector<TByte>& out_code,TUInt32 iValue,int highBit,const int kTagBit){//д�벢ǰ��ָ��.
    const int kMaxPack32BitTagBit=3;
    assert((0<=kTagBit)&&(kTagBit<=kMaxPack32BitTagBit));
    assert((highBit>>kTagBit)==0);
    const int kMaxPack32BitSize=5;
    const unsigned int kMaxValueWithTag=(1<<(7-kTagBit))-1;

    TByte codeBuf[kMaxPack32BitSize];
    TByte* codeEnd=codeBuf;
    while (iValue>kMaxValueWithTag) {
        *codeEnd=iValue&((1<<7)-1); ++codeEnd;
        iValue>>=7;
    }
    out_code.push_back( (highBit<<(8-kTagBit)) | iValue | (((codeBuf!=codeEnd)?1:0)<<(7-kTagBit)));
    while (codeBuf!=codeEnd) {
        --codeEnd;
        out_code.push_back((*codeEnd) | (((codeBuf!=codeEnd)?1:0)<<7));
    }
}

inline static int pack32BitWithTagOutSize(TUInt32 iValue,int kTagBit){//����pack���ֽڴ�С.
    const unsigned int kMaxValueWithTag=(1<<(7-kTagBit))-1;
    int result=0;
    while (iValue>kMaxValueWithTag) {
        ++result;
        iValue>>=7;
    }
    return (result+1);
}

static inline void pack32Bit(std::vector<TByte>& out_code,TUInt32 iValue){
    pack32BitWithTag(out_code, iValue, 0, 0);
}
static inline int pack32BitOutSize(TUInt32 iValue){
    return pack32BitWithTagOutSize(iValue, 0);
}

}//end namespace frg
#endif
