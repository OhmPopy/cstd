//bytes_rle.h
//���ٽ�ѹ��һ��ͨ���ֽ���ѹ��rle�㷨.
/*
 Copyright (c) 2012-2013 HouSisong All Rights Reserved.
 
 Permission is hereby granted, pfree of charge, to any person
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
#ifndef WH_BYTES_RLE_H_
#define WH_BYTES_RLE_H_
#include <vector>
    
class TBytesRle{
public:
    enum TRleParameter{ kRle_size_bestSize=0, kRle_size_default=2, kRle_size_bestUnRleSpeed=32 };
    static void save(std::vector<unsigned char>& out_code,const unsigned char* src,const unsigned char* src_end,int rle_parameter=kRle_size_default);
};

#endif //WH_BYTES_RLE_H_