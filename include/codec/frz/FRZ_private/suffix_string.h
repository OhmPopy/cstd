//suffix_string.h
//��׺�ַ�����һ��ʵ��.
/*
 Copyright (c) 2012-2013 HouSisong All Rights Reserved.
 
 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:
 
 The above copyright notice and this permission notice shall be
 included in all copies of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef WH_SUFFIX_STRING_H_
#define WH_SUFFIX_STRING_H_
#include <vector>
#include "string.h" //for strlen


typedef int TSuffixIndex;

class TSuffixString{
public:
    typedef signed int      TInt;
    typedef unsigned short  TUShort;
    
    TSuffixString(const char* src_begin,const char* src_end);
    void clear();
    void resetString(const char* src_begin,const char* src_end);

    typedef std::vector<TSuffixIndex>   TSuffixArray;

    const char*         ssbegin;//ԭ�ַ���.
    const char*         ssend;
    TSuffixArray        SA;     //�ź���ĺ�׺�ַ�������.
    inline TInt size()const { return (TInt)(ssend-ssbegin); }
    TInt lower_bound(const char* str,const char* str_end)const;//return index in SA
    inline const TInt lower_bound(const char* c_str)const { return lower_bound(c_str,c_str+strlen(c_str)); }

    void R_create();
    std::vector<TInt>   R;          //Rank ��׺�ַ�������.
    inline const TInt lower_bound_withR(TSuffixIndex curString)const { return R[curString]; }

    void LCP_create_withR(); //must R_create();
    void LCP_create_withOutR();//slow; not need R_create();
    std::vector<TInt>   LCP;        //lcp(i,i+1)  ���ں�׺�ַ���֮��������ǰ׺.
    //todo:inline const Int32 getEqualLength_withLCP(TSuffixIndex aString,TSuffixIndex bString)const;
    
    void LCPLite_create_withR(); //must R_create();
    void LCPLite_create_withOutR();//slow; not need R_create();
    std::vector<TUShort>   LCPLite;  //���LCP��Լһ���ڴ�.
};

#include "suffix_string.inl"

#endif //WH_SUFFIX_STRING_H_
