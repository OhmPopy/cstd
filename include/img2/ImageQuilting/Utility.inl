#include "Utility.h"
/// <summary>
/// �����ݽض���Byte���������ڣ�����ʱ�䣺2014.12.8
/// </summary>
/// <param name="Value">��Ҫ�ü���ԭʼ���ݡ�</param>
/// <remarks> �ο���http://www.cnblogs.com/zyl910/archive/2012/03/12/noifopex1.html��</remarks>
/// <remarks> ��λ���������ʹ���.�ô����������������룬��Ҫд�ɺ궨�壬����Ϊ�������Ͷ��������⣬������дϵͳ���Զ�������Ч����Ҳ�Ⱥ궨���һЩ��</remarks>
unsigned char ClampToByte(int Value)
{
  return ((Value | ((signed int)(255 - Value) >> 31)) & ~((signed int)Value >> 31));
}
/// <summary>
/// ����0��1֮��������������ʱ�䣺2014.12.8
/// </summary>
double Rand()
{
  return (double)rand() / (RAND_MAX + 1.0);
}
/// <summary>
/// ����ָ���ı�Եģʽ������չ������������ֵ
/// </summary>
/// <param name="Width">����Ŀ�ȡ�</param>
/// <param name="Height">����ĸ߶ȡ�</param>
/// <param name="Left">�����Ҫ��չ�����ꡣ</param>
/// <param name="Right">�Ҳ���Ҫ��չ�����ꡣ</param>
/// <param name="Top">������Ҫ��չ�����ꡣ</param>
/// <param name="Bottom">�ײ���Ҫ��չ�����ꡣ</param>
/// <param name="Edge">�����Ե�ķ�ʽ��</param>
/// <param name="RowPos">�����з������������ֵ��</param>
/// <param name="ColPos">�����з������������ֵ��</param>
/// <returns>���غ����Ƿ�ִ�гɹ���</returns>
IS_RET GetValidCoordinate(int Width, int Height, int Left, int Right, int Top, int Bottom, EdgeMode Edge, TMatrix** Row, TMatrix** Col)
{
  if ((Left < 0) || (Right < 0) || (Top < 0) || (Bottom < 0)) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  IS_RET Ret = IS_CreateMatrix(Width + Left + Right, 1, IS_DEPTH_32S, 1, Row);
  if (Ret != IS_RET_OK) {
    return Ret;
  }
  Ret = IS_CreateMatrix(1, Height + Top + Bottom, IS_DEPTH_32S, 1, Col);
  if (Ret != IS_RET_OK) {
    return Ret;
  }
  int X, Y, XX, YY, *RowPos = (int*)(*Row)->Data, *ColPos = (int*)(*Col)->Data;
  for (X = -Left; X < Width + Right; X++) {
    if (X < 0) {
      if (Edge == EdgeMode_Tile) {                                                   //�ظ���Ե����
        RowPos[X + Left] = 0;
      }
      else {
        XX = -X;
        while (XX >= Width) {
          XX -= Width;  // ����������
        }
        RowPos[X + Left] = XX;
      }
    }
    else if (X >= Width) {
      if (Edge == EdgeMode_Tile) {
        RowPos[X + Left] = Width - 1;
      }
      else {
        XX = Width - (X - Width + 2);
        while (XX < 0) {
          XX += Width;
        }
        RowPos[X + Left] = XX;
      }
    }
    else {
      RowPos[X + Left] = X;
    }
  }
  for (Y = -Top; Y < Height + Bottom; Y++) {
    if (Y < 0) {
      if (Edge == EdgeMode_Tile) {
        ColPos[Y + Top] = 0 ;
      }
      else {
        YY = -Y;
        while (YY >= Height) {
          YY -= Height;
        }
        ColPos[Y + Top] = YY;
      }
    }
    else if (Y >= Height) {
      if (Edge == EdgeMode_Tile) {
        ColPos[Y + Top] = Height - 1;
      }
      else {
        YY = Height - (Y - Height + 2);
        while (YY < 0) {
          YY += Height;
        }
        ColPos[Y + Top] = YY;
      }
    }
    else {
      ColPos[Y + Top] = Y;
    }
  }
  return IS_RET_OK;
}
IS_RET __stdcall GetExpandImage(TMatrix* Src, TMatrix** Dest, int Left, int Right, int Top, int Bottom, EdgeMode Edge)
{
  if (Src == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Depth != IS_DEPTH_8U || Left < 0 || Right < 0 || Top < 0 || Bottom < 0) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  int X, Y, SrcWidth, SrcHeight, SrcStride, DstWidth, DstHeight, DstStride, Channel;
  unsigned char* LinePS, *LinePD;
  SrcWidth = Src->Width;
  SrcHeight = Src->Height;
  DstWidth = SrcWidth + Left + Right;
  DstHeight = SrcHeight + Top + Bottom;
  if (*Dest != NULL) {
    if ((*Dest)->Data == NULL) {
      return IS_RET_ERR_NULLREFERENCE;
    }
    if ((*Dest)->Depth != IS_DEPTH_8U || (*Dest)->Width != DstWidth || (*Dest)->Height != DstHeight) {
      return IS_RET_ERR_ARGUMENTOUTOFRANGE;
    }
  }
  else {
    IS_RET Ret = IS_CreateMatrix(DstWidth, DstHeight, Src->Depth, Src->Channel, Dest);
    if (Ret != IS_RET_OK) {
      return Ret;
    }
  }
  SrcStride = Src->WidthStep;
  DstStride = (*Dest)->WidthStep;
  Channel = Src->Channel;
  TMatrix* Row, *Col;
  GetValidCoordinate(SrcWidth, SrcHeight, Left, Right, Top, Bottom, Edge, &Row, &Col);            //      ��ȡ����ƫ����
  int* RowPos = (int*)Row->Data, *ColPos = (int*)Col->Data;
  for (Y = 0; Y < SrcHeight; Y++) {
    LinePD = (*Dest)->Data + (Y + Top) * DstStride;
    LinePS = Src->Data + Y * SrcStride;
    for (X = 0; X < Left; X++) {
      memcpy(LinePD, LinePS + RowPos[X] * Channel, Channel);                                                          //      ��������
      LinePD += Channel;                                                                                                                                      //      �ƶ�����
    }
    memcpy(LinePD, LinePS, SrcWidth * Channel);                                                                                             //      �м������
    LinePD += SrcWidth * Channel;                                                                                                                   //      �ƶ�����
    for (X = Left + SrcWidth; X < Left + SrcWidth + Right; X++) {
      memcpy(LinePD, LinePS + RowPos[X] * Channel, Channel);                                                          //      �Ҳ������
      LinePD += Channel;
    }
  }
  for (Y = 0; Y < Top; Y++) {
    memcpy((*Dest)->Data + Y * DstStride, (*Dest)->Data + (Top + ColPos[Y]) * DstStride, DstStride);                        //      ����ֱ�ӿ���
  }
  for (Y = Top + SrcHeight; Y < Top + SrcHeight + Bottom; Y++) {
    memcpy((*Dest)->Data + Y * DstStride, (*Dest)->Data + (Top + ColPos[Y]) * DstStride, DstStride);
  }
  IS_FreeMatrix(&Row);
  IS_FreeMatrix(&Col);
  return IS_RET_OK;
}

