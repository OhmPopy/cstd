//  frg_reader.cpp
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
#include "frg_reader.h"
#include "string.h" //memset memcpy
#include "assert.h" //assert
#include "frg_draw.h"
#include "FRZ1_decompress.h" //source code: https://github.com/sisong/FRZ 

    inline static TUInt32 readUInt32(const void* _codeData){
        const TByte* codeData=(const TByte*)_codeData;
        TUInt32 result=codeData[0]|(codeData[1]<<8)|(codeData[2]<<16)|(codeData[3]<<24);
        return result;
    }

    inline static TUInt32 readUInt32(const TByte** code){
        const TByte* codeData=*code;
        TUInt32 result=readUInt32(codeData);
        *code=codeData+4;
        return result;
    }

    inline static const void* toAlign4(const void* pointer){
        return (const TByte*)0+( ((const TByte*)pointer-(const TByte*)0+3)>>2<<2 );
    }

int FRG_READER_EXPORT_API getFrgHeadSize(){
    return kFrgFileHeadSize;
}

#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
    const int kSafeColorTable_extendMemSize=kFrg_MaxSubTableSize*4;
#endif
frg_BOOL FRG_READER_EXPORT_API readFrgImageInfo(const TByte* frgCode_begin,const TByte* frgCode_end,struct frg_TFrgImageInfo* out_frgImageInfo){
    if (frgCode_end-frgCode_begin<kFrgFileHeadSize) return frg_FALSE;
    const struct TFrgFileHead& fhead=*(const struct TFrgFileHead*)frgCode_begin;
    if (  readUInt32(&fhead.frgTagAndVersion[0])!=readUInt32(&kFrgTagAndVersion[0]))
        return frg_FALSE;
    if (out_frgImageInfo==0) return frg_TRUE;
    
    out_frgImageInfo->imageFileSize=readUInt32(&fhead.imageFileSize);
    out_frgImageInfo->imageWidth=readUInt32(&fhead.headInfo.imageWidth);
    out_frgImageInfo->imageHeight=readUInt32(&fhead.headInfo.imageHeight);
    out_frgImageInfo->decoder_tempMemoryByteSize=readUInt32(&fhead.headInfo.decoder_tempMemoryByteSize);
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
    out_frgImageInfo->decoder_tempMemoryByteSize+=kSafeColorTable_extendMemSize;
#endif
    //����ʵ�ֵĽ��������Զ�decoder_tempMemoryByteSize�ͻ�0,���Լ����������Ҫ���ڴ�;���߷����Լ���Ҫ���ڴ�(������).

    return frg_TRUE;
}

////
static frg_BOOL _bytesRle_load(TByte* out_data,TByte* out_dataEnd,const TByte* rle_code,const TByte* rle_code_end);//rle����.
static frg_BOOL _colorUnZiper_loadColor(const struct frg_TPixelsRef* dst_image,const TByte* code,const TByte* code_end,const TByte* alpha,int alpha_byte_width,TByte* tempMemory,TByte* tempMemory_end);

#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
#   define _FRZ1_decompress_NAME FRZ1_decompress_safe
#else
#   define _FRZ1_decompress_NAME FRZ1_decompress
#endif

frg_BOOL FRG_READER_EXPORT_API readFrgImage(const TByte* frgCode_begin,const TByte* frgCode_end,const struct frg_TPixelsRef* dst_image,TByte* tempMemory,TByte* tempMemory_end){
    struct frg_TFrgImageInfo frgInfo;
    if (!readFrgImageInfo(frgCode_begin,frgCode_end,&frgInfo))
        return frg_FALSE;
    if ((frgInfo.imageWidth!=dst_image->width)||(frgInfo.imageHeight!=dst_image->height)||((tempMemory_end-tempMemory)<frgInfo.decoder_tempMemoryByteSize)) return frg_FALSE;

    if (dst_image->colorType!=kFrg_ColorType_32bit_A8R8G8B8) return frg_FALSE;
    if ((dst_image->width<=0)||(dst_image->height<=0)) return frg_TRUE;
    
    const struct TFrgFileHead& fhead=*(const struct TFrgFileHead*)frgCode_begin;
    if (fhead.headInfo.encodingFormat!=kEncodingFormat_stream) return frg_FALSE;
    if (fhead.headInfo.savedColorFormat!=kSavedColorFormat_A8R8G8B8) return frg_FALSE;
    const TUInt32 headInfoCodeSize=readUInt32(&fhead.headInfo.headInfoCodeSize);
    const TByte _colorInfo=fhead.headInfo.colorInfo;
    const bool isSingleAlpha=(_colorInfo&kColorInfo_isSingleAlpha)!=0;
    const bool isSingleBGR=(_colorInfo&kColorInfo_isSingleBGR)!=0;
    const bool isAlphaDataUseBytesZip=(_colorInfo&kColorInfo_isAlphaDataUseBytesZip)!=0;
    const bool isRGBDataUseBytesZip=(_colorInfo&kColorInfo_isRGBDataUseBytesZip)!=0;
    
    const TByte* singleBGRA=&(fhead.headInfo.singleColor_b); //b,g,r,a
    const TByte* frgCodeData=frgCode_begin+(sizeof(struct TFrgFileHead)-sizeof(struct TFrgHeadInfo))
                            +sizeof(headInfoCodeSize)+headInfoCodeSize;//skip head code

    //single color
    if (isSingleAlpha&&isSingleBGR){
        frg_fillPixels_32bit(dst_image,singleBGRA);
        return frg_TRUE;
    }

    //read alpha
    TByte* alphaBuf=0;
    int alpha_byte_width;
    if (isSingleAlpha){
        alpha_byte_width=0;
        const int alphaBufSize=dst_image->width;
        if (tempMemory_end-tempMemory<alphaBufSize) return frg_FALSE;
        alphaBuf=tempMemory; tempMemory+=alphaBufSize;
        memset(alphaBuf,singleBGRA[3],alphaBufSize);
    }else{
        if (frgCode_end-frgCodeData<4) return frg_FALSE;
        const TUInt32 codeSize=readUInt32(&frgCodeData);
        if (frgCode_end-frgCodeData<codeSize) return frg_FALSE;
        const TByte* code=frgCodeData;
        frgCodeData+=codeSize;
        const TByte* code_end=frgCodeData;

        alpha_byte_width=dst_image->width;
        const int alphaBufSize=alpha_byte_width*dst_image->height;
        if (tempMemory_end-tempMemory<alphaBufSize) return frg_FALSE;
        alphaBuf=tempMemory; tempMemory+=alphaBufSize;

        TByte* _tempMemory_back=tempMemory;
        if (isAlphaDataUseBytesZip){
            if (codeSize<4) return frg_FALSE;
            const TUInt32 alpha_code_size=readUInt32(&code);
            if ((TUInt32)(tempMemory_end-tempMemory)<alpha_code_size) return frg_FALSE;
            TByte* _alpha_code_buf=tempMemory; tempMemory+=alpha_code_size;
            if (!_FRZ1_decompress_NAME(_alpha_code_buf,_alpha_code_buf+alpha_code_size,code,code_end))
                return frg_FALSE;
            code=_alpha_code_buf;
            code_end=_alpha_code_buf+alpha_code_size;
        }
        if (!_bytesRle_load(alphaBuf,alphaBuf+alphaBufSize,code,code_end))
            return frg_FALSE;
        tempMemory=_tempMemory_back;
    }

    //read bgr
    if (isSingleBGR) {
        frg_fillPixels_32bit_withAlpha(dst_image,singleBGRA,alphaBuf,alpha_byte_width);
    }else{
        if (frgCode_end-frgCodeData<4) return frg_FALSE;
        const TUInt32 codeSize=readUInt32(&frgCodeData);
        if ((TUInt32)(frgCode_end-frgCodeData)<codeSize) return frg_FALSE;
        const TByte* code=frgCodeData;
        frgCodeData+=codeSize;
        const TByte* code_end=frgCodeData;

        //TByte* _tempMemory_back=tempMemory;
        if (isRGBDataUseBytesZip){
            if (codeSize<4) return frg_FALSE;
            const TUInt32 bgr_code_size=readUInt32(&code);
            if ((TUInt32)(tempMemory_end-tempMemory)<bgr_code_size) return frg_FALSE;
            TByte* _bgr_code_buf=tempMemory; tempMemory+=bgr_code_size;
            if (!_FRZ1_decompress_NAME(_bgr_code_buf,_bgr_code_buf+bgr_code_size,code,code_end))
                return frg_FALSE;
            code=_bgr_code_buf;
            code_end=_bgr_code_buf+bgr_code_size;
        }
        if (!_colorUnZiper_loadColor(dst_image,code,code_end,alphaBuf,alpha_byte_width,tempMemory,tempMemory_end))
            return frg_FALSE;
        //tempMemory=_tempMemory_back;
    }
    return frg_TRUE;
}

/////////

//�䳤32bit���������뷽��(x bit�������ͱ�־λ,x<=3),�Ӹ�λ��ʼ���1-5byte:
// x0*  7-x bit
// x1* 0*  7+7-x bit
// x1* 1* 0*  7+7+7-x bit
// x1* 1* 1* 0*  7+7+7+7-x bit
// x1* 1* 1* 1* 0*  7+7+7+7+7-x bit
static TUInt32 unpack32BitWithTag(const TByte** src_code,const TByte* src_code_end,const int kTagBit){//����������ǰ��ָ��.
    const TByte* pcode;
    TUInt32 value;
    TByte   code;
    pcode=*src_code;
    
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
    if (src_code_end-pcode<=0) return 0;
#endif
    code=*pcode; ++pcode;
    value=code&((1<<(7-kTagBit))-1);
    if ((code&(1<<(7-kTagBit)))!=0){
        do {
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
            //assert((value>>(sizeof(value)*8-7))==0);
            if (src_code_end==pcode) break;
#endif
            code=*pcode; ++pcode;
            value=(value<<7) | (code&((1<<7)-1));
        } while ((code&(1<<7))!=0);
    }
    (*src_code)=pcode;
    return value;
}

static inline TUInt32 unpack32Bit(const TByte** src_code,const TByte* src_code_end){
    return unpack32BitWithTag(src_code, src_code_end, 0);
}

static frg_BOOL _colorUnZiper_loadColor(const struct frg_TPixelsRef* dst_image,const TByte* code,const TByte* code_end,const TByte* alpha,int alpha_byte_width,TByte* tempMemory,TByte* tempMemory_end){
    //assert((dst_image->width>0)&&(dst_image->height>0));

    const TUInt32 nodeCount=unpack32Bit(&code,code_end);
    const int nodeWidth=(dst_image->width+kFrg_ClipWidth-1)/kFrg_ClipWidth;
    const int nodeHeight=(dst_image->height+kFrg_ClipHeight-1)/kFrg_ClipHeight;
    assert(nodeCount==nodeWidth*nodeHeight);
    const TUInt32 tableSize=unpack32Bit(&code,code_end);
    const TUInt32 indexCodeSize=unpack32Bit(&code,code_end);
    const TUInt32 matchCount=unpack32Bit(&code,code_end);

#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
    if (nodeCount>(TUInt32)(code_end-code)) return frg_FALSE;
#endif
    const TByte* nodeInfoList=code;   code+=nodeCount;
    const TByte* nodeInfoList_end=code;
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
    if ( ((tableSize>>(32-2))!=0) || (tableSize*3>(TUInt32)(code_end-code)) ) return frg_FALSE;
#endif
    const TByte* color24Table=code;   code+=tableSize*3;

#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
    if (indexCodeSize>(TUInt32)(code_end-code)) return frg_FALSE;
#endif
    const TByte* indexCodeList=code; code+=indexCodeSize;
    const TByte* indexCodeList_end=code;

#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
    if ( ((matchCount>>(32-2))!=0) || (matchCount*4>(TUInt32)(code_end-code)) ) return frg_FALSE;
#endif
    const TByte* matchXYList=code; code+=matchCount*4;
    const TByte* matchXYList_end=code;
    if (code!=code_end) return frg_FALSE;

    //colorTable
    if ((TUInt32)(tempMemory_end-tempMemory)<(TUInt32)(tableSize*4+3)) return frg_FALSE;
    TUInt32* colorTable=(TUInt32*)tempMemory; //tempMemory+=...;
    colorTable=(TUInt32*)toAlign4(colorTable);
    const TUInt32* colorTable_end=colorTable+tableSize;
    frg_table_BGR24_to_32bit(colorTable,color24Table,tableSize);
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
    const TUInt32* _colorTable_begin=colorTable;
    if ((TUInt32)(tempMemory_end-tempMemory-(tableSize*4+3))<(TUInt32)kSafeColorTable_extendMemSize) return frg_FALSE;
    memset(&colorTable[tableSize],0,kSafeColorTable_extendMemSize);
#endif


    int lastNodeWidth=((TUInt32)dst_image->width)%kFrg_ClipWidth;
    if (lastNodeWidth==0) lastNodeWidth=kFrg_ClipWidth;
    int lastNodeHeight=((TUInt32)dst_image->height)%kFrg_ClipHeight;
    if (lastNodeHeight==0) lastNodeHeight=kFrg_ClipHeight;
    struct frg_TPixelsRef subRef;
    subRef.byte_width=dst_image->byte_width;
    subRef.colorType=dst_image->colorType;
    subRef.height=kFrg_ClipHeight;

    for (int ny=0; ny<nodeHeight;++ny,alpha+=alpha_byte_width*kFrg_ClipHeight){
        subRef.pColor=(TByte*)dst_image->pColor+ny*kFrg_ClipHeight*dst_image->byte_width;
        subRef.width=kFrg_ClipWidth;
        if (ny==nodeHeight-1)
            subRef.height=lastNodeHeight;
        const TByte* curAlpha=alpha;
        for (int nx=0; nx<nodeWidth;++nx,curAlpha+=kFrg_ClipWidth,subRef.pColor=(TUInt32*)( ((TByte*)subRef.pColor)+kFrg_ClipWidth*kFrg_outColor32_size )){
            if (nx==nodeWidth-1)
                subRef.width=lastNodeWidth;

            TByte nodeInfo=*nodeInfoList; ++nodeInfoList;
            //assert(nodeInfoList<=nodeInfoList_end);
            const enum frg_TClipType nodeType=(enum frg_TClipType)(nodeInfo>>4);
            switch (nodeType) { //type
                case kFrg_ClipType_index_single_a_w8:{
                    int tableLength=1+( nodeInfo&15 );
                    const int bit=kFrg_SubTableSize_to_indexBit[tableLength];
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
                    if ((TUInt32)tableLength>(TUInt32)(colorTable_end-colorTable)) return frg_FALSE;
                    if ((TUInt32)(subRef.height*bit)>(TUInt32)(indexCodeList_end-indexCodeList)) return frg_FALSE;
#endif
                    frg_copyPixels_32bit_index_single_a_w8_xbit[bit](&subRef,colorTable,indexCodeList,*curAlpha);
                    colorTable+=tableLength;
                    indexCodeList+=subRef.height*bit;//*kFrg_ClipWidth/8
                } break;
                case kFrg_ClipType_index:{
                    int tableLength=1+( nodeInfo&15 );
                    const int bit=kFrg_SubTableSize_to_indexBit[tableLength];
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
                    if ((TUInt32)tableLength>(TUInt32)(colorTable_end-colorTable)) return frg_FALSE;
                    if ((TUInt32)((subRef.width*subRef.height*bit+7)>>3)>(TUInt32)(indexCodeList_end-indexCodeList)) return frg_FALSE;
#endif
                    frg_copyPixels_32bit_index_xbit[bit](&subRef,colorTable,indexCodeList,curAlpha,alpha_byte_width);
                    colorTable+=tableLength;
                    indexCodeList+=(subRef.width*subRef.height*bit+7)>>3;
                } break;
                case kFrg_ClipType_single_bgra_w8:{
                    int tableForwardLength=nodeInfo&15;
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
                    if ((TUInt32)tableForwardLength>(TUInt32)(colorTable-_colorTable_begin)) return frg_FALSE;
                    //kSafeColorTable_extendMemSize�Ѿ��������𻵵�tableForwardLengthʹcolorTable-tableForwardLength+(1<<4)>colorTable_end����ɵ�Խ��.
#endif
                    frg_copyPixels_32bit_single_bgra_w8(&subRef,colorTable[-tableForwardLength],*curAlpha);
                    if (tableForwardLength==0){
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
                        if (colorTable>=colorTable_end) return frg_FALSE;
#endif
                        ++colorTable;
                    }
                } break;
                case kFrg_ClipType_single_bgr:{
                    int tableForwardLength=nodeInfo&15;
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
                    if ((TUInt32)tableForwardLength>(TUInt32)(colorTable-_colorTable_begin)) return frg_FALSE;
#endif
                    frg_copyPixels_32bit_single_bgr(&subRef,colorTable[-tableForwardLength],curAlpha,alpha_byte_width);
                    if (tableForwardLength==0){
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
                        if (colorTable>=colorTable_end) return frg_FALSE;
#endif
                        ++colorTable;
                    }
                } break;
                case kFrg_ClipType_match_table_single_a_w8:{
                    const int bit=(nodeInfo&3)+1;
                    int tableForwardLength=unpack32Bit(&indexCodeList,indexCodeList_end);
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
                    if ((TUInt32)tableForwardLength>(TUInt32)(colorTable-_colorTable_begin)) return frg_FALSE;
                    if ((TUInt32)(subRef.height*bit)>(TUInt32)(indexCodeList_end-indexCodeList)) return frg_FALSE;
#endif
                    const TUInt32* subTable=&colorTable[-tableForwardLength];
                    frg_copyPixels_32bit_index_single_a_w8_xbit[bit](&subRef,subTable,indexCodeList,*curAlpha);
                    indexCodeList+=subRef.height*bit;//*kFrg_ClipWidth/8
                } break;
                case kFrg_ClipType_match_table:{
                    const int bit=(nodeInfo&3)+1;
                    int tableForwardLength=unpack32Bit(&indexCodeList,indexCodeList_end);
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
                    if ((TUInt32)tableForwardLength>(TUInt32)(colorTable-_colorTable_begin)) return frg_FALSE;
                    if ((TUInt32)((subRef.width*subRef.height*bit+7)>>3)>(TUInt32)(indexCodeList_end-indexCodeList)) return frg_FALSE;
#endif
                    const TUInt32* subTable=&colorTable[-tableForwardLength];
                    frg_copyPixels_32bit_index_xbit[bit](&subRef,subTable,indexCodeList,curAlpha,alpha_byte_width);
                    indexCodeList+=(subRef.width*subRef.height*bit+7)>>3;
                } break;
                case kFrg_ClipType_match_image:{
                    enum frg_TMatchType matchType=(enum frg_TMatchType)(nodeInfo&((1<<3)-1)); //3bit
                    frg_copyPixels_32bit_match(&subRef,dst_image->pColor,matchType,matchXYList,curAlpha,alpha_byte_width);
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
                    if (4>(TUInt32)(matchXYList_end-matchXYList)) return frg_FALSE;
#endif
                    matchXYList+=4;
                } break;
                case kFrg_ClipType_directColor:{
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
                    if ((TUInt32)(subRef.width*subRef.height)>(TUInt32)(colorTable_end-colorTable)) return frg_FALSE;
#endif
                    if (nodeInfo&1)
                        frg_copyPixels_32bit_directColor_single_a_w8(&subRef,colorTable,(*curAlpha));
                    else
                        frg_copyPixels_32bit_directColor(&subRef,colorTable,curAlpha,alpha_byte_width);
                    colorTable+=subRef.width*subRef.height;
                } break;
            }//end case
        }//for x
    }//for y
    return (nodeInfoList==nodeInfoList_end)&&(colorTable==colorTable_end)&&(indexCodeList==indexCodeList_end)&&(matchXYList==matchXYList_end);
}

/////////////

static frg_BOOL _bytesRle_load(TByte* out_data,TByte* out_dataEnd,const TByte* rle_code,const TByte* rle_code_end){
    TUInt32 ctrlSize,length;
    const TByte* ctrlBuf,*ctrlBuf_end;
    enum TByteRleType type;
    
    ctrlSize= unpack32Bit(&rle_code,rle_code_end);
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
    if (ctrlSize>(TUInt32)(rle_code_end-rle_code)) return frg_FALSE;
#endif
    ctrlBuf=rle_code;
    rle_code+=ctrlSize;
    ctrlBuf_end=rle_code;
    while (ctrlBuf_end-ctrlBuf>0){
        type=(enum TByteRleType)((*ctrlBuf)>>(8-kByteRleType_bit));
        length= 1 + unpack32BitWithTag(&ctrlBuf,ctrlBuf_end,kByteRleType_bit);
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
        if (length>(TUInt32)(out_dataEnd-out_data)) return frg_FALSE;
#endif
        switch (type){
            case kByteRleType_rle0:{
                memset(out_data,0,length);
                out_data+=length;
            }break;
            case kByteRleType_rle255:{
                memset(out_data,255,length);
                out_data+=length;
            }break;
            case kByteRleType_rle:{
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
                if (rle_code_end-rle_code<1) return frg_FALSE;
#endif
                memset(out_data,*rle_code,length);
                ++rle_code;
                out_data+=length;
            }break;
            case kByteRleType_unrle:{
#ifdef FRG_READER_RUN_MEM_SAFE_CHECK
                if (length>(TUInt32)(rle_code_end-rle_code)) return frg_FALSE;
#endif
                memcpy(out_data,rle_code,length);
                rle_code+=length;
                out_data+=length;
            }break;
        }
    }
    return (ctrlBuf==ctrlBuf_end)&&(rle_code==rle_code_end)&&(out_data==out_dataEnd);
}
