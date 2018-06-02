//  frg_reader.h
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
#ifndef _LIBFRG_frg_reader_h
#define _LIBFRG_frg_reader_h

#if defined(__BCPLUSPLUS__) || defined( _BORLANDC_ )
#define _BCC32_OBJ_FOR_DELPHI
//uses bcc32 compile out ".obj" file for link with Delphi App.
#pragma option push -V?-
#define FRG_READER_EXPORT_API __stdcall
#else
#define FRG_READER_EXPORT_API 
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
//���������ڴ����Խ����,���Ա���frg�𻵻���������빥��;�ٶ�Ӱ���С(�򿪿�����3%).
//#define FRG_READER_RUN_MEM_SAFE_CHECK

enum frg_TColorType {
    kFrg_ColorType_32bit_A8R8G8B8  =1
    //todo: other ?  8bit_A8 24bit_R8G8B8 24bit_B8G8R8 32bit_X8B8G8R8 32bit_A8B8G8R8 32bit_X8R8G8B8 32bit_R8G8B8A8
};

//������޸�kFrg_outColor32_*��4��ֵ��֧������Ҫ��rgba32bit��ɫ�����ʽ(��Ҫ���±��������Դ����).
//outColor32 = (b8<<blue_shl) | (g8<<green_shl) | (r8<<red_shl) | (a8<<alpha_shl);
enum {
    kFrg_outColor32_blue_shl    =0,
    kFrg_outColor32_green_shl   =8,
    kFrg_outColor32_red_shl     =16,
    kFrg_outColor32_alpha_shl   =24
};
const int kFrg_outColor32_size =4;

struct frg_TPixelsRef{
    void*    pColor;        //ͼƬ�����ڴ���ʼ��ַ.
    int      width;         //ͼƬ��.
    int      height;        //ͼƬ��.
    int      byte_width;    //������ʼ�����ڴ����һ����ʼ�����ڴ�ļ���ֽھ���.
    enum frg_TColorType  colorType; //now must kFrg_ColorType_32bit_A8R8G8B8
};

    
#define frg_BOOL    int
#define frg_FALSE   0
#define frg_TRUE    (!frg_FALSE)

    
struct frg_TFrgImageInfo{
    int imageFileSize;
    int imageWidth;
    int imageHeight;
    int decoder_tempMemoryByteSize;
};
    
//���ػ��frg����������Ϣ�Ĵ�С. //����ĳЩʱ��Ͳ��ö�ȡ�����ļ���������.
int FRG_READER_EXPORT_API getFrgHeadSize();

//���frgͼƬ������Ϣ. //must(frgCode_end-frgCode_begin>=frgHeadSize) ;  out_frgImageInfo can null;
frg_BOOL FRG_READER_EXPORT_API readFrgImageInfo(const unsigned char* frgCode_begin,const unsigned char* frgCode_end,struct frg_TFrgImageInfo* out_frgImageInfo);

//����frg��ʽ��ͼƬ����.
frg_BOOL FRG_READER_EXPORT_API readFrgImage(const unsigned char* frgCode_begin,const unsigned char* frgCode_end,const struct frg_TPixelsRef* dst_image,unsigned char* tempMemory,unsigned char* tempMemory_end);

#ifdef __cplusplus
}
#endif

#endif

