//  frg_color_tools.cpp
//  for frg_writer
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
#include "frg_color_tools.h"

namespace frg{

void pixelsFill(const TPixels32Ref& dst,TBGRA32 color){
    for (int y=0;y<dst.height;++y){
        for (int x=0;x<dst.width;++x){
            dst.pixels(x, y)=color;
        }
    }
}

void pixelsCopy(const TPixels32Ref& dst,const TPixels32Ref& src){
    for (int y=0;y<std::min(dst.height,src.height);++y){
        for (int x=0;x<std::min(dst.width,src.width);++x){
            dst.pixels(x, y)=src.pixels(x, y);
        }
    }
}

static bool findFirstNotEmptyAlpha(const TPixels32Ref& src,int* out_x,int* out_y){
    for (int y=0;y<src.height;++y){
        for (int x=0;x<src.width;++x){
            if (src.pixels(x, y).a!=0){
                *out_x=x;
                *out_y=y;
                return true;
            }
        }
    }
    return false;
}

bool getIsSigleRGBColor(const TPixels32Ref& src,TBGRA32* out_BGR){
    int aX,aY;
    if (!findFirstNotEmptyAlpha(src,&aX,&aY)){
        *out_BGR=TBGRA32(0,0,0,0);
        return true;
    }

    TBGRA32 BGR=src.pixels(aX,aY);
    BGR.a=0;
    for (int y=aY;y<src.height;++y){
        for (int x=0;x<src.width;++x){
            if ((src.pixels(x,y).getBGR()!=BGR.getBGR())&&(src.pixels(x,y).a!=0)){
                return false;
            }
        }
    }
    *out_BGR=BGR;
    return true;
}

bool getIsSigleAlphaColor(const TPixels32Ref& src,TByte* out_Alpha){
    TUInt32 A=0;
    if ((src.width>0)&&(src.height>0))
        A=src.pixels(0,0).a;

    for (int y=0;y<src.height;++y){
        for (int x=0;x<src.width;++x){
            if (src.pixels(x,y).a!=A){
                return false;
            }
        }
    }
    *out_Alpha=A;
    return true;
}


void delEmptyColor(const TPixels32Ref& dst){
    TByte A;
    if (getIsSigleAlphaColor(dst,&A)){ //��ͬalpha
        if (A==0)
            pixelsFill(dst,TBGRA32(0,0,0,0)); //Alpha = 0
        return;
    }
    TBGRA32 rgb;
    if (getIsSigleRGBColor(dst,&rgb)){//��ͬrgb
        rgb.a=0;
        for (int y=0;y<dst.height;++y){
            for (int x=0;x<dst.width;++x){
                if (dst.pixels(x,y).a==0){
                    dst.pixels(x,y)=rgb;
                }
            }
        }
        return;
    }

    /*ȫ͸������ֱ�����0
     for (int y=0;y<dst.height;++y){
         for (int x=0;x<dst.width;++x){
             if (dst.pixels(x,y).a==0){
                 dst.pixels(x,y)={0};
             }
         }
     }
    //*/
    //*�Ż����ŵ�ʱ�ı߽�����,������ڱ�.
    const int kBorder=1;
    for (int y=0;y<dst.height;++y){
        for (int x=0;x<dst.width;++x){
            if (dst.pixels(x,y).a==0){
                std::map<TUInt32,TUInt32> set;
                for (int dy=-kBorder;dy<=kBorder;++dy){
                    for (int dx=-kBorder;dx<=kBorder;++dx){
                        TBGRA32 c;
                        if (dst.getPixels(x+dx,y+dy,&c)){
                            if (c.a>0){
                                set[c.getBGR()]+=c.a;
                            }
                        }
                    }
                }
                if (set.empty())
                    dst.pixels(x,y)=TBGRA32(0,0,0,0);
                else{
                    TUInt32 maxCount=0;
                    TUInt32 bestColor=0;
                    for (std::map<TUInt32,TUInt32>::const_iterator it(set.begin());it!=set.end();++it){
                        if (it->second>maxCount){
                            maxCount=it->second;
                            bestColor=it->first;
                        }
                    }
                    dst.pixels(x,y)=TBGRA32((TByte)bestColor,(TByte)(bestColor>>8),(TByte)(bestColor>>16),0);
                }
            }
        }
    }
    //*/
}

}//end namespace frg
