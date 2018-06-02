//  frg_reader_base.h
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
#ifndef _LIBFRG_frg_base_h
#define _LIBFRG_frg_base_h

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char   TByte;
typedef signed   int    TInt32;
typedef unsigned int    TUInt32;
typedef unsigned short  TUInt16;


/*// frg image file struct:
  head    : sizeof(TFrgFileHead) byte
  --if (!isSingleAlpha){
    --alpha code
      alpha_code_size       : 4byte
    --if (!isAlphaDataUseBytesZip){
        alpha rle code      : alpha_code_size byte
    --}else{
        alpha_rle_code_size : 4byte
        alpha BytesZip code : alpha_code_size-4 byte
    --}
  --}
  --if (!isSingleBGR){
    --bgr code
        bgr_code_size       : 4byte
    --if (!isRGBDataUseBytesZip){
        bgr data code       : bgr_code_size byte
    --}else{
        bgr_data_size       : 4byte
        bgr BytesZip code   : bgr_code_size-4 byte
    --}
  --}
*/

struct TFrgHeadInfo{
    TUInt32         headInfoCodeSize; //==size(TFrgHeadInfo)-4
    TUInt32         imageWidth;
    TUInt32         imageHeight;
    TUInt32         decoder_tempMemoryByteSize;
    TByte           encodingFormat;     //TEncodingFormat
    TByte           savedColorFormat;   //TSavedColorFormat
    TByte           colorInfo;          //TFrgHeadColorInfo
    TByte           _reserved;          //set 0
    TByte           singleColor_b;
    TByte           singleColor_g;
    TByte           singleColor_r;
    TByte           singleColor_a;
};

struct TFrgFileHead{ //frg�ļ�ͷ���ݲ���(С�˸�ʽ��д).
    char                frgTagAndVersion[4];
    TUInt32             imageFileSize;
    struct TFrgHeadInfo headInfo;
};
static const int kFrgFileHeadSize=sizeof(struct TFrgFileHead);

static const int  kFrgTagAndVersionSize =4;
static const char kFrgTagAndVersion[kFrgTagAndVersionSize]={'F','R','G',13};

//8bit �洢��ɫ�������Ϣ.
enum TFrgHeadColorInfo{
    kColorInfo_isSingleAlpha             = 1<<0,
    kColorInfo_isSingleBGR               = 1<<1,
    kColorInfo_isAlphaDataUseBytesZip    = 1<<2,
    kColorInfo_isRGBDataUseBytesZip      = 1<<3
};

//8bit �������ݴ��淽ʽ.
enum TEncodingFormat{
    kEncodingFormat_stream =1  //�ֽ���.
    //todo:֧������ֱ��ӳ�䵽�ڴ���Դ�Ĵ����ʽ kEncodingFormat_memoryMapping? .
};

//8bit �������ɫ�ĸ�ʽ��λ��.
enum TSavedColorFormat{
    kSavedColorFormat_A8R8G8B8 =32
    //todo:֧�� kSavedColorFormat_ xxx ? .
};

    
//------


//�ֿ��С.
static const int kFrg_ClipWidth=8;
static const int kFrg_ClipHeight=8;

//��ɫ������.  ռ�ø�4bit
enum frg_TClipType{
    kFrg_ClipType_index_single_a_w8         = 0, //��ɫ��(��������� ��Alpha,8���ؿ�) +4bit(�ֲ���ɫ�峤��-1)
    kFrg_ClipType_index                     = 1, //��ɫ��(���������) +4bit(�ֲ���ɫ�峤��-1)
    kFrg_ClipType_single_bgra_w8            = 2, //��ɫBGRA(8���ؿ�) +4bit��ǰƥ���λ�ô���ɫ��ͬ(����0��ʾ�Լ�������һ����ɫ����ɫ��)
    kFrg_ClipType_single_bgr                = 3, //��ɫBGR +4bit��ǰƥ���λ�ô���ɫ��ͬ(����0��ʾ�Լ�������һ����ɫ����ɫ��)
    kFrg_ClipType_match_table_single_a_w8   = 4, //��ɫ��ƥ��(��Alpha,8���ؿ�) +2bitδ��+2bit(�ֲ���ɫ��bit��-1) (��������һ����ǰƥ��λ��(�䳤1-5byte))
    kFrg_ClipType_match_table               = 5, //��ɫ��ƥ�� +2bitδ��+2bit(�ֲ���ɫ��bit��-1) (��������һ����ǰƥ��λ��(�䳤1-5byte))
    kFrg_ClipType_match_image               = 6, //֡��Ԥ�� +1bitδ��+3bitƥ������ (��������һ����ǰƥ��xy����λ��(2+2byte))
    kFrg_ClipType_directColor               = 7, //����ѹ��(���ֿ��ڵ�������ɫ��˳������ڵ�ɫ����) +3bitδ�� +1bit(�Ƿ��ǵ�Alpha����8���ؿ�)
};

static const int kFrg_MaxSubTableSize=(1<<4); //���ֲ���ɫ���С.
static const TByte kFrg_SubTableSize_to_indexBit[kFrg_MaxSubTableSize+1]={ 0,0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4 };//�ֲ���ɫ���С���õ������Ҫ��bit��.
static const int kFrg_MaxForwardLength=(1<<4)-1; //����ʣ���4bit�ռ��ܴ���������ǰƥ��λ����ֵ.

//kFrg_ClipType_match_image���Կ�ƥ������. ռ��3bit
enum frg_TMatchType{
    kFrg_MatchType_move_bgra_w8         = 0,    //ƽ��BGRA(8���ؿ�)
    kFrg_MatchType_move_bgr             = 1,    //ƽ��BGR
    kFrg_MatchType_left_right_bgra_w8   = 2,    //���Ҿ���BGRA(8���ؿ�)
    kFrg_MatchType_left_right_bgr       = 3,    //���Ҿ���BGR
    kFrg_MatchType_up_down_bgra_w8      = 4,    //���¾���BGRA(8���ؿ�)
    kFrg_MatchType_up_down_bgr          = 5,    //���¾���BGR
};

inline static int unpackMatchX(TUInt32 xy){
    return xy&((1<<16)-1);
}
inline static int unpackMatchY(TUInt32 xy){
    return xy>>16;
}

//---
    
//������rleѹ����İ�����2bit
enum TByteRleType{
    kByteRleType_rle0  = 0,    //00��ʾ������ѹ��0    (���в���Ҫ�ֽ�����)
    kByteRleType_rle255= 1,    //01��ʾ������ѹ��255  (���в���Ҫ�ֽ�����)
    kByteRleType_rle   = 2,    //10��ʾ������ѹ������  (����ֻ�财��һ���ֽ�����)
    kByteRleType_unrle = 3     //11��ʾ������δѹ������ (���������������ֽ�����)
};
static const int kByteRleType_bit=2;
    
#ifdef __cplusplus
}
#endif

#endif
