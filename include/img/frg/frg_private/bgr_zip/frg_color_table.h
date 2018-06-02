// frg_colorTableZiper.h
// for frg_writer
// �������ɵ�ɫ��.
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
#ifndef WH_LIBFRG_FRG_COLOR_TABLE_H_
#define WH_LIBFRG_FRG_COLOR_TABLE_H_
#include "frg_color_base.h"
#include "../calc_color.h"

namespace frg{

    //�����ɫ��
    class TColorTableZiper{
    public:
        static TUInt32 getMatchColorMask(float colorQuality);
    public:        
        TColorTableZiper(float colorQuality,bool isMustFitColorTable);
        void setImageSize(int imageWidth,int imageHeight);

        bool getBestColorTable(std::vector<Color24>& out_table,const TPixels32Ref& colors,const int maxTableSize=kFrg_MaxSubTableSize)const;//������ѵ�ɫ��.
        void getBestColorIndex(std::vector<TByte>& out_indexList,const Color24* table,TInt32 tableSize,const TPixels32Ref& subColors,int subX0,int subY0);//������ɫ���.
        //void uniteColorTable(std::vector<Color24>& dstTable,const std::vector<Color24>& srcTable);
    public:
        struct TColorErrorParameter{
            TInt32       minColorError; //����������  ����ɫ�����ʱ������ɾ����ɫ������������.
            TInt32       minColorError_optimize;    //����������-�Ż�  ����ɫ���С�Ѿ����ʣ�����ɾ����ɫ������������.
            TInt32       errorDiffuse_coefficient;  //�����ɢϵ��.
            //Int32       errorDiffuse_randError;   //�����ɢ.
            TInt32       maxErrorDiffuseValue;      //�����ɢֵ.
            bool        isMustFitColorTable;
            TInt32       maxTableSize;
        };
        
        struct TColorNode {
        public:
            typedef TCalcColor<Color24,8,int> TColor;
        private:
            TColor  m_sumColor;
            TColor  m_color;
            int     m_count;
        public:
           inline void setColor(const Color24& rgb,int count){
                m_color.setWithMul(TColor(rgb), 1);
                m_sumColor.setWithMul(TColor(rgb), count);
                m_count=count;
            }
            void uniteColor(const TColorNode& n);
           inline const TColor& getColor()const{ return m_color; }
           inline int getCount()const { return m_count; }
           inline  Color24 asColor24()const { return m_color.asColor(); }
        };
        
        enum{ kColorErrorIntFloatBit=9};
        typedef TCalcColor<Color32,kColorErrorIntFloatBit,int> TErrorColor;
    private:
        float                   m_colorQuality;
        TColorErrorParameter    m_errorParameter;
        TPixelsBufferBase<TPixelsRefBase<TErrorColor> > m_errorBuffer;
    private:
        static void  getColorSet(std::vector<TColorNode>* out_colorCountsList,const TPixels32Ref& colors);
        static void  deleteColor(std::vector<TColorNode>& colorSet,int maxTableSize,const TColorErrorParameter& errorParameter);
        static void  sortColorArrayForOut(std::vector<TColorNode>& colorSet);
        static void  sortColorArrayForOut(std::vector<Color24>& colorSet);
        static void  writeTable(std::vector<Color24>& out_table,std::vector<TColorNode>& colorSet);
    };

}//end namespace frg

#endif //WH_LIBFRG_FRG_COLOR_TABLE_H_