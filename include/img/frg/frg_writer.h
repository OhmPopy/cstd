//  frg_writer.h
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
#ifndef _LIBFRG_frg_writer_h
#define _LIBFRG_frg_writer_h

#include <vector>

namespace frg{
    
    struct TFrgParameter;
    struct TFrgPixels32Ref;

//save as frg 
void writeFrgImage(std::vector<unsigned char>& outFrgCode,const TFrgPixels32Ref& srcImage,const TFrgParameter& parameter);

    //��������.
    static const float kFrg_quality_max=100;    //����ѹ��,���Ƽ�.
    static const float kFrg_quality_default=80; //�Ƽ�ѹ��qualityѡ��80����(�����ʹ�С����); ����Ҳ�Ƽ�81,�Դ���������(������ʧ����Ŀ�����𱣴�).
    static const float kFrg_quality_min=0;      //���Ƽ�СkFrg_quality_default̫���ֵ,�����ϲ�.
    //��������������µ�ѹ����С����.
    static const float kFrg_size_maxUnZipSpeed=100;//����ڴ��ѹ�ٶ�,�����ɵ��ļ��ϴ�(�Ӵ��̼��ؿ��ܽ���).
    static const float kFrg_size_default=50; //�Ƽ�: �ڴ�����豸��,Ӳ�̼���ʱ��+�ڴ��ѹ��ʱ������.
    static const float kFrg_size_minSize=0; //��С�����ɵ��ļ���С,���ڴ��ѹ����.
    
    struct TFrgBGRA32{
        unsigned char b;    //blue
        unsigned char g;    //green
        unsigned char r;    //red
        unsigned char a;    //alpha     (if no alpha,must set a=255)
    };
    inline TFrgBGRA32 getFrgBGRA32(unsigned char r8,unsigned char g8,unsigned char b8,unsigned char a8) {
        TFrgBGRA32 result; result.b=b8;  result.g=g8;  result.r=r8;  result.a=a8;
        return result;
    }
    inline TFrgBGRA32 getFrgColor(unsigned char r8,unsigned char g8,unsigned char b8) {
        return getFrgBGRA32(r8,g8,b8,255);
    }
    
    struct TFrgPixels32Ref{
        TFrgBGRA32* pColor;     //ͼƬ�����ڴ���ʼ��ַ.
        int         width;      //ͼƬ��[0..65535]
        int         height;     //ͼƬ��[0..65535]
        int         byte_width; //������ʼ�����ڴ����һ����ʼ�����ڴ�ļ���ֽھ���.
    };
    
    struct TFrgParameter{
        //frg֧�ֵļ���ѹ�����ڲ���. 
        float   quality;                //��������,ֵ��[0--100].
        bool    isMustFitColorTable;    //������ɫ�϶�Ŀ�,�Ƿ�Ҳ����ʹ�õ�ɫ��(�����Ļ��ļ����С���������ܱ��).
        bool    isAlphaDataUseBytesZip; //�Ƿ��alphaͨ����rle�����һ��ʹ��bytesZipѹ��.
        bool    isRGBDataUseBytesZip;   //�Ƿ��rgbͨ���ı����һ��ʹ��bytesZipѹ��.
        int     rle_parameter;          //alphaͨ����rleѹ������(�����rleѹ���㷨��һ��Ϊalphaͨ�������Ż����г�ѹ���㷨,ѹ���ͽ�ѹ���ܿ�);ֵ��[0--),һ��С��32,����ֵ2,һ��ֵԽСѹ����Խ��. 
        int     zip_parameter;          //bytesZip��ѹ������(bytesZip��һ��Ϊ�Ż���ѹ�ٶ�ΪĿ����Ƶ�ѹ���㷨);ֵ��[0--),һ��С��32,����ֵ4,һ��ֵԽСѹ����Խ��.
        bool    isDeleteEmptyColor;     //ȫ͸��������,��ɫ�ᱻ����,����Ϊtrue
        
        //setCtrlParameter��frg����Ķ�����Ʋ���ӳ�䵽���������Ŀ��Ʋ�����(������ѹ����С;�ú�����ʵ���ǰ����ߵľ����趨��ϵ��ֵ,��Ҳ�����ֹ�������Ŀ��Ʋ���).
        explicit TFrgParameter(float _quality=kFrg_quality_default,float _compressSizeParameter=kFrg_size_default){
            setParameter(_quality,_compressSizeParameter);
        }
        
        void setParameter(float _quality,float _compressSizeParameter);
    };
    
}//end namespace frg

#endif
