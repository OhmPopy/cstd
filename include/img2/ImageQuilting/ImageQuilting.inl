#include "Core.inl"
#include "Utility.inl"
///     ��д�ߣ� laviewpbt, ��дʱ�䣺 2015.9 - 10�� ��ϵQQ: 33184777
/// <summary>
///     �򵥸�Ч�ĻҶȻ��㷨��
/// </summary>
/// <param name="Src">��Ҫ�����Դͼ������ݽṹ��</param>
/// <param name="Dest">���洦����ͼ������ݽṹ��</param>
///     <remarks> Deltaδ��LAB�ռ���У�ֻ��RGB�ռ����Ĵ���,�����ٶȺ�ͼ���С��ϵ���󣬺�Level�����Թ�ϵ��</remarks>
IS_RET __stdcall ClassicDecolorization(TMatrix* Src, TMatrix* Dest)
{
  if (Src == NULL || Dest == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL || Dest->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Width != Dest->Width || Src->Height != Dest->Height || Src->Depth != Dest->Depth) {
    return IS_RET_ERR_PARAMISMATCH;
  }
  if (Src->Depth != IS_DEPTH_8U || Dest->Depth != IS_DEPTH_8U) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  if (Src->Channel != 3 || (Dest->Channel != 3 && Dest->Channel != 1)) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  int X, Y, Width = Src->Width, Height = Src->Height;
  unsigned char* LinePS, *LinePD;
  for (Y = 0; Y < Height; Y++) {
    LinePS = Src->Data + Y * Src->WidthStep;
    LinePD = Dest->Data + Y * Dest->WidthStep;
    for (X = 0; X < Width; X++) {
      LinePD[X] = (LinePS[0] + LinePS[1] + LinePS[1] + LinePS[2]) / 4;
      LinePS += 3;
    }
  }
  return IS_RET_OK;
}
/// <summary>
/// ��Ա�����д�ľ�������ת���㷨��ֻ֧�ֵ�ͨ����byte��int���;���.��2015.9.10�գ�
/// </summary>
/// <param name="Src">��Ҫ�����Դͼ������ݽṹ��</param>
/// <param name="Dest">���洦����ͼ������ݽṹ��</param>
/// <remarks> 1: Src��Dest������ͬ��</remarks>
IS_RET TransposeMatrix(TMatrix* Src, TMatrix* Dest)
{
  if (Src == NULL || Dest == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL || Dest->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Width != Dest->Height || Src->Height != Dest->Width || Src->Channel != Dest->Channel || Src->Depth != Dest->Depth) {
    return IS_RET_ERR_PARAMISMATCH;
  }
  if (Src->Depth != IS_DEPTH_8U && Src->Depth != IS_DEPTH_32S) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  if (Src->Channel != 1) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  unsigned char* LinePS, *LinePD;
  int X, Y, Width, Height, StrideS, StrideD, *LinePSI, *LinePDI;
  Width = Dest->Width, Height = Dest->Height, StrideS = Src->WidthStep;
  if (Src->Depth == IS_DEPTH_8U) {
    for (Y = 0; Y < Height; Y++) {
      LinePS = Src->Data + Y;
      LinePD = Dest->Data + Y * Dest->WidthStep;
      for (X = 0; X < Width; X++) {
        LinePD[X] = LinePS[0];
        LinePS += StrideS;
      }
    }
  }
  else {
    for (Y = 0; Y < Height; Y++) {
      LinePSI = (int*)Src->Data + Y;
      LinePDI = (int*)(Dest->Data + Y * Dest->WidthStep);
      for (X = 0; X < Width; X++) {
        LinePDI[X] = LinePSI[0];
        LinePSI += Height;
      }
    }
  }
  return IS_RET_OK;
}
/// <summary>
/// �������Ŵ�ֱ����ľֲ���С���·����ֻ֧�ֵ�ͨ�������ݾ���.��2015.9.10�գ�
/// </summary>
/// <param name="Src">��Ҫ������ȡ·��������,������int���;���</param>
/// <param name="Dest">�õ���·����0��ʾ�����Ĳ��֣�255��ʾ��Ҫ�滻���Ĳ��֣�������byte��ͼ�����ݡ�</param>
/// <remarks> 1:ԭʼ���㷨·����Ȼ���Ż������ǻ��� ��</remarks>
IS_RET MinCutVertical(TMatrix* Src, TMatrix* Dest)
{
  if (Src == NULL || Dest == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL || Dest->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Width != Dest->Width || Src->Height != Dest->Height || Src->Channel != Dest->Channel) {
    return IS_RET_ERR_PARAMISMATCH;
  }
  if (Src->Depth != IS_DEPTH_32S || Dest->Depth != IS_DEPTH_8U) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  if (Src->Channel != 1) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  IS_RET Ret = IS_RET_OK;
  int X, Y, Index, MinValue, Width, Height, *LinePL, *LinePD, *LinePS;
  unsigned char* LinePM;
  TMatrix* Energy = NULL;
  Width = Src->Width, Height = Src->Height;
  Ret = IS_CreateMatrix(Src->Width, Src->Height, IS_DEPTH_32S, 1, &Energy);                               //      ��������
  if (Ret != IS_RET_OK) {
    return Ret;
  }
  memcpy(Energy->Data, Src->Data, Src->WidthStep);                                                                                //      ��һ�е���������ԭʼ���ݵĵ�һ��
  LinePL = (int*)Energy->Data;                                                                                                                    //      ��һ�е�����
  LinePD = (int*)(Energy->Data + Energy->WidthStep);                                                                              //      ��ǰ�е��ۻ������������㣩
  for (Y = 1; Y < Height; Y++) {                                                                                                                          //      for i=2:size(E,1), �ӵڶ��п�ʼ
    LinePS = (int*)(Src->Data + Y * Src->WidthStep);                                                                        //      ��ǰ�еĵ�������ֵ
    LinePD[0] = LinePS[0] + MIN(LinePL[0] , LinePL[1]);                                                                     //      E(i,1) = X(i,1) + MIN( E(i-1,1), E(i-1,2) );
    for (X = 1; X < Width - 1; X++) {                                                                                                       //      for j=2:size(E,2)-1
      LinePD[X] = LinePS[X] + MIN(MIN(LinePL[X - 1], LinePL[X]), LinePL[X + 1]);              //      E(i,j) = X(i,j) + MIN( [E(i-1,j-1), E(i-1,j), E(i-1,j+1)] );
    }
    LinePD[Width - 1] = LinePS[Width - 1] + MIN(LinePL[Width - 2], LinePL[Width - 1]);      //      E(i,end) = X(i,end) + MIN( E(i-1,end-1), E(i-1,end) );
    LinePL += Energy->Width;
    LinePD += Energy->Width;
  }
  MinValue = MaxValueI;
  LinePD = (int*)(Energy->Data + (Height - 1) * Energy->WidthStep);
  for (X = 0; X < Width; X++) {
    if (LinePD[X] < MinValue) {                                                                                                                     //      �����һ���ڵ���С�ۻ�����ֵ
      Index = X;                                                                                                                                              //      [cost, idx] = MIN(E(end, 1:end));
      MinValue = LinePD[X];
    }
  }
  LinePM = Dest->Data + (Height - 1) * Dest->WidthStep;
  for (X = 0; X < Index; X++) {
    LinePM[X] = 0;  //      C(i, 1:idx-1) = -1;
  }
  for (X = Index; X < Width; X++) {
    LinePM[X] = 255;  //      C(i, idx+1:end) = +1;
  }
  for (Y = Height - 2; Y >= 0; Y--) {                                                                                                             //      for i=size(E,1)-1:-1:1,
    LinePD = (int*)(Energy->Data + Y * Energy->WidthStep);                                                          //      �����һ�����Ͽ�ʼ������8����ʵ����ֻ�漰����һ�е�����Ԫ��)�ľֲ���Сֵ
    if (Index > 0) {
      MinValue = MIN(LinePD[Index - 1], LinePD[Index]);                                                               //       if( idx > 1 && E(i,idx-1) == MIN(E(i,idx-1:MIN(idx+1,size(E,2))) ) )
      if (Index + 1 < Width) {
        MinValue = MIN(MinValue, LinePD[Index + 1]);
      }
      if (MinValue == LinePD[Index - 1]) {
        Index--;  //      idx = idx-1;
      }
      else if (MinValue == LinePD[Index + 1]) {
        Index++;
      }
    }
    else {
      if (LinePD[Index] > LinePD[Index + 1]) {
        Index++;
      }
    }
    LinePM = Dest->Data + Y * Dest->WidthStep;
    for (X = 0; X < Index; X++) {
      LinePM[X] = 0;  //      ��ֵΪ0��ʾ����Ҫ�ı�Ĳ��֣�Ϊ255���ʾ��Ҫ���µĿ����ɫ������
    }
    for (X = Index; X < Width; X++) {
      LinePM[X] = 255;
    } ;
  }
  IS_FreeMatrix(&Energy);
  return IS_RET_OK;
}
/// <summary>
/// ��������ˮƽ����ľֲ���С���·����ֻ֧�ֵ�ͨ�������ݾ���.��2015.9.10�գ�
/// </summary>
/// <param name="Src">��Ҫ������ȡ·��������,������int���;���</param>
/// <param name="Dest">�õ���·����0��ʾ�����Ĳ��֣�255��ʾ��Ҫ�滻���Ĳ��֣�������byte��ͼ�����ݡ�</param>
/// <remarks> 1:ֱ����ˮƽ������㷨ԭ����д����ٶȻرȽ�������Ϊ�����ַ�����漰��*Width,������ת���ٵ��ô�ֱ����ļ��㡣</remarks>
/// <remarks> 2:ת�õ��ŵ�������ת�ú����ݲ��������⣬�������ת��������˳ʱ����ת90�ȣ�����ʱ����ת90�ȣ��õ������ݻ�������з�ɫ������ܻ����ȷ��Ч����</remarks>
IS_RET MinCutHorizontal(TMatrix* Src, TMatrix* Dest)
{
  TMatrix* TransposeToVertical = NULL, *TransposeMaskToVertical = NULL;
  IS_RET Ret = IS_CreateMatrix(Src->Height, Src->Width, IS_DEPTH_32S, 1, &TransposeToVertical);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(Src->Height, Src->Width, IS_DEPTH_8U, 1, &TransposeMaskToVertical);       //      ��������
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = TransposeMatrix(Src, TransposeToVertical);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = MinCutVertical(TransposeToVertical, TransposeMaskToVertical);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = TransposeMatrix(TransposeMaskToVertical, Dest);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
Done:
  IS_FreeMatrix(&TransposeToVertical);
  IS_FreeMatrix(&TransposeMaskToVertical);
  return Ret;
}
/// <summary>
/// ����ͼ��ľֲ�ƽ���ͣ��ٶ��Ѿ��Ż���֧��1��3ͨ��ͼ�񡣣�2015.9.10�գ�
/// </summary>
/// <param name="Src">����ƽ���͵�ͼ��</param>
/// <param name="Dest">ƽ�������ݣ���Ҫʹ��int���;��󱣴档</param>
/// <param name="SizeX">��ˮƽ����ʹ�õ�ģ���С������ǰ뾶ģʽ����Ӧ����Ϊ2*Radius + 1��</param>
/// <param name="SizeY">�ڴ�ֱ����ʹ�õ�ģ���С������ǰ뾶ģʽ����Ӧ����Ϊ2*Radius + 1��</param>
/// <remarks> 1:ʹ��������BoxBlur����Ż��㷨����ʱ�Ͳ��������޹ء�</remarks>
/// <remarks> 2:Ҳ����ʹ�û���ͼʵ�֡�</remarks>
IS_RET GetLocalSquareSum(TMatrix* Src, TMatrix* Dest, int SizeX, int SizeY)
{
  if (Src == NULL || Dest == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL || Dest->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Width != Dest->Width || Src->Height != Dest->Height || Src->Channel != Dest->Channel) {
    return IS_RET_ERR_PARAMISMATCH;
  }
  if (Src->Depth != IS_DEPTH_8U || Dest->Depth != IS_DEPTH_32S) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  if (SizeX < 0 || SizeY < 0) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  IS_RET Ret = IS_RET_OK;
  int X, Y, Z, Width, Height, Left, Right, Top, Bottom;
  int SumRed, SumGreen, SumBlue, Sum, LastIndex, NextIndex;
  int* ColSum, *ColSumB, *ColSumG, *ColSumR, *LinePD;
  unsigned char* SamplePS, *LastAddress, *NextAddress;
  TMatrix* Row = NULL, *Col = NULL;
  Width = Src->Width, Height = Src->Height;
  Left = SizeX / 2, Right = SizeX - Left, Top = SizeY / 2, Bottom = SizeY - Top;
  ColSum = (int*)IS_AllocMemory(Width * Src->Channel * sizeof(int), true);
  if (ColSum == NULL) {
    Ret = IS_RET_ERR_OUTOFMEMORY;
    goto Done;
  }
  Ret = GetValidCoordinate(Width, Height, Left, Right, Top, Bottom, EdgeMode_Tile, &Row, &Col);                           //      ��ȡ����ƫ����
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  if (Src->Channel == 1) {
    for (Y = 0; Y < Height; Y++) {
      LinePD = (int*)(Dest->Data + Y * Dest->WidthStep);
      if (Y == 0) {
        for (X = 0; X < Width; X++) {
          Sum = 0;
          for (Z = -Top; Z < Bottom; Z++) {
            SamplePS = Src->Data + ((int*)Col->Data)[Z + Top] * Src->WidthStep + X;
            Sum += SamplePS[0] * SamplePS[0] ;
          }
          ColSum[X] = Sum;
        }
      }
      else {
        LastAddress = Src->Data + ((int*)Col->Data)[Y - 1] * Src->WidthStep;
        NextAddress = Src->Data + ((int*)Col->Data)[Y + SizeY - 1] * Src->WidthStep;
        for (X = 0; X < Width; X++) {
          ColSum[X] -= LastAddress[0] * LastAddress[0] - NextAddress[0] * NextAddress[0];
          LastAddress++;
          NextAddress++;
        }
      }
      for (X = 0; X < Width; X++) {
        if (X == 0) {
          Sum = 0;
          for (Z = -Left; Z < Right; Z++) {
            Sum += ColSum[((int*)Row->Data)[Z + Left]];
          }
        }
        else {
          LastIndex = ((int*)Row->Data)[X - 1];
          NextIndex = ((int*)Row->Data)[X + SizeX - 1];
          Sum -= ColSum[LastIndex] - ColSum[NextIndex];
        }
        LinePD[0] = Sum;
        LinePD++;
      }
    }
  }
  else if (Src->Channel == 3) {
    ColSumB = ColSum;
    ColSumG = ColSumB + Width;
    ColSumR = ColSumG + Width;
    for (Y = 0; Y < Height; Y++) {
      LinePD = (int*)(Dest->Data + Y * Dest->WidthStep);
      if (Y == 0) {
        for (X = 0; X < Width; X++) {
          SumBlue = 0;
          SumGreen = 0;
          SumRed = 0;
          for (Z = -Top; Z < Bottom; Z++) {
            SamplePS = Src->Data + ((int*)Col->Data)[Z + Top] * Src->WidthStep + X * 3;
            SumBlue += SamplePS[0] * SamplePS[0];
            SumGreen += SamplePS[1] * SamplePS[1];
            SumRed += SamplePS[2] * SamplePS[2];
          }
          ColSumB[X] = SumBlue;
          ColSumG[X] = SumGreen;
          ColSumR[X] = SumRed;
        }
      }
      else {
        LastAddress = Src->Data + ((int*)Col->Data)[Y - 1] * Src->WidthStep;
        NextAddress = Src->Data + ((int*)Col->Data)[Y + SizeY - 1] * Src->WidthStep;
        for (X = 0; X < Width; X++) {
          ColSumB[X] -= LastAddress[0] * LastAddress[0] - NextAddress[0] * NextAddress[0];
          ColSumG[X] -= LastAddress[1] * LastAddress[1] - NextAddress[1] * NextAddress[1];
          ColSumR[X] -= LastAddress[2] * LastAddress[2] - NextAddress[2] * NextAddress[2];
          LastAddress += 3;
          NextAddress += 3;
        }
      }
      for (X = 0; X < Width; X++) {
        if (X == 0) {
          SumBlue = 0;
          SumGreen = 0;
          SumRed = 0;                  //������������
          for (Z = -Left; Z < Right; Z++) {
            SumBlue += ColSumB[((int*)Row->Data)[Z + Left]];
            SumGreen += ColSumG[((int*)Row->Data)[Z + Left]];
            SumRed += ColSumR[((int*)Row->Data)[Z + Left]];
          }
        }
        else {
          LastIndex = ((int*)Row->Data)[X - 1];
          NextIndex = ((int*)Row->Data)[X + SizeX - 1];
          SumBlue -= ColSumB[LastIndex] - ColSumB[NextIndex];
          SumGreen -= ColSumG[LastIndex] - ColSumG[NextIndex];
          SumRed -= ColSumR[LastIndex] - ColSumR[NextIndex];
        }
        LinePD[0] = SumBlue;
        LinePD[1] = SumGreen;
        LinePD[2] = SumRed;
        LinePD += 3;
      }
    }
  }
Done:
  IS_FreeMatrix(&Row);
  IS_FreeMatrix(&Col);
  IS_FreeMemory(ColSum);
  return Ret;
}
/// <summary>
/// ����SSE���ֽ����ݵĳ˷���
/// </summary>
/// <param name="Kernel">��Ҫ����ĺ˾���</param>
/// <param name="Conv">�������</param>
/// <param name="Length">��������Ԫ�صĳ��ȡ�</param>
/// <remarks> 1: ʹ����SSE�Ż���
/// <remarks> 2: ��л��ʯ����544617183���ṩ��SSE����<��</remarks>
///     https://msdn.microsoft.com/zh-cn/library/t5h7783k(v=vs.90).aspx
int MultiplySSE(unsigned char* Kernel, unsigned char* Conv, int Length)
{
  int Y = 0, Sum = 0;
#if 0
  __m128i vsum = _mm_set1_epi32(0);
  __m128i vk0 = _mm_set1_epi8(0);
  for (Y = 0; Y <= Length - 16; Y += 16) {
    __m128i v0 = _mm_loadu_si128((__m128i*)(Kernel + Y));                           //      ��Ӧmovdquָ��,����Ҫ16�ֽڶ���
    __m128i v0l = _mm_unpacklo_epi8(v0, vk0);
    __m128i v0h = _mm_unpackhi_epi8(v0, vk0);                                                       //      ������������ǰ����Ƿֱ���ص�����128λ�Ĵ����У��������_mm_madd_epi16��16λSSE�������ã�vk0��������Ҫ�ǰѸ�8λ��0��
    __m128i v1 = _mm_loadu_si128((__m128i*)(Conv + Y));
    __m128i v1l = _mm_unpacklo_epi8(v1, vk0);
    __m128i v1h = _mm_unpackhi_epi8(v1, vk0);
    vsum = _mm_add_epi32(vsum, _mm_madd_epi16(v0l, v1l));                           //      _mm_madd_epi16 ����һ���Խ���8��16λ���ĳ˷���Ȼ�������16�Ľ���ڼ��������ŵ�һ��32���У� r0 := (a0 * b0) + (a1 * b1)����� https://msdn.microsoft.com/zh-cn/library/yht36sa6(v=vs.90).aspx
    vsum = _mm_add_epi32(vsum, _mm_madd_epi16(v0h, v1h));
  }
  for (; Y <= Length - 8; Y += 8) {
    __m128i v0 = _mm_loadl_epi64((__m128i*)(Kernel + Y));
    __m128i v0l = _mm_unpacklo_epi8(v0, vk0);
    __m128i v1 = _mm_loadl_epi64((__m128i*)(Conv + Y));
    __m128i v1l = _mm_unpacklo_epi8(v1, vk0);
    vsum = _mm_add_epi32(vsum, _mm_madd_epi16(v0l, v1l));
  }
  vsum = _mm_add_epi32(vsum, _mm_srli_si128(vsum, 8));
  vsum = _mm_add_epi32(vsum, _mm_srli_si128(vsum, 4));
  Sum = _mm_cvtsi128_si32(vsum);                                                                                  //      MOVD������Moves the least significant 32 bits of a to a 32-bit integer�� r := a0
#endif
  for (; Y < Length; Y++) {
    Sum += Kernel[Y] * Conv[Y];
  }
  return Sum;
}
/// <summary>
/// ����SSE��ͼ�����㷨������ʱ��2014.11.27
/// </summary>
/// <param name="Src">��Ҫ�����Դͼ������ݽṹ��</param>
/// <param name="Conv">�������,������ͼ�����ݡ�</param>
/// <param name="Dest">���洦���ľ����������ݽṹ��������int�;���</param>
/// <remarks> 1: ʹ����SSE�Ż���
/// <remarks> 2: ʹ���ֽ������SSE�˷�������ͨ��SSE�ٶ�Ҫ���˸��ࡣ</remarks>
IS_RET FastConv2(TMatrix* Src, TMatrix* Conv, TMatrix* Dest)
{
  if (Src == NULL || Dest == NULL || Conv == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL || Dest->Data == NULL || Conv->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Width != Dest->Width || Src->Height != Dest->Height || Src->Channel != Dest->Channel) {
    return IS_RET_ERR_PARAMISMATCH;
  }
  if (Conv->Width < 1 || Conv->Height < 1) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  if (Src->Depth != IS_DEPTH_8U || Dest->Depth != IS_DEPTH_32S) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  IS_RET Ret = IS_RET_OK;
  int IndexD, IndexE, IndexK, ExpHeight, ExpStride, *DestP;
  int X, Y, Width, Height, ConvW, ConvH, Length, Left, Top, Right, Bottom;
  unsigned char* PtExp, *CurKer;;
  TMatrix* Expand = NULL;
  Width = Src->Width, Height = Src->Height, ConvW = Conv->Width, ConvH = Conv->Height, Length = ConvW * ConvH;
  Left = ConvW / 2, Top = ConvH / 2, Right = ConvW - Left - 1, Bottom = ConvH - Top - 1, ExpHeight = Height + ConvH - 1;  //      ע��������Ǹ�Ԫ�ز�����չ������˵Ŀ��Ϊ3����ֻҪ���Ҹ���չһ�����ؾͿ�����
  Ret = GetExpandImage(Src, &Expand, Left, Right, Top, Bottom, EdgeMode_Tile);                                                           //      �õ���չ������ݣ��������ٺͷ����̣����Ƕ�ռ��һ���ڴ�
  if (Ret != IS_RET_OK) {
    IS_FreeMatrix(&Expand);
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  unsigned char* Conv16 = (unsigned char*)IS_AllocMemory(ConvW * ConvH);                                         //      ����������ȥ��ԭʼConv�п������õ����ݣ�ע�����ﲻ��16�ֽڶ���
  if (Conv16 == NULL) {
    Ret = IS_RET_ERR_OUTOFMEMORY;
    IS_FreeMatrix(&Expand);
    return IS_RET_ERR_OUTOFMEMORY;
  }
  unsigned char* Kernel = (unsigned char*)IS_AllocMemory(ConvW * ExpHeight);                                     //      ����16�ֽڶ���ľ���˾����Է���ʹ��SSE
  if (Kernel == NULL) {
    Ret = IS_RET_ERR_OUTOFMEMORY;
    IS_FreeMatrix(&Expand);
    IS_FreeMemory(Conv16);
    return IS_RET_ERR_OUTOFMEMORY;
  }
  PtExp = Expand->Data;
  ExpStride = Expand->WidthStep;
  for (Y = 0; Y < ConvH; Y++) {
    memcpy(Conv16 + Y * ConvW, Conv->Data + Y * Conv->WidthStep , ConvW);  //      ���ƾ�����������
  }
  for (Y = 0; Y < ExpHeight; Y++) {
    IndexE = Y * ExpStride;
    CurKer = Kernel + Y * ConvW;                                           //      �����һ���������ؽ�Ҫȡ���ľ��������
    for (X = 0; X < ConvW; X++) {
      CurKer[X] = PtExp[IndexE++];
    }
  }
  for (X = 0 ; X < Width ; X ++) {
    if (X != 0) {                                                                                                  //      ������ǵ�һ�У���Ҫ���¾���˵�����
      memcpy(Kernel, Kernel + 1, (ConvW * ExpHeight - 1));   //      ��ǰ�ƶ�һ������
      IndexK = ConvW - 1 ;
      IndexE = IndexK + X;
      for (Y = 0; Y < ExpHeight; Y++) {
        Kernel[IndexK] = PtExp[IndexE];                                                //      ֻҪˢ����һ��Ԫ��
        IndexK += ConvW;
        IndexE += ExpStride;
      }
    }
    CurKer = Kernel, IndexD = X, DestP = (int*)Dest->Data;
    for (Y = 0; Y < Height; Y ++) {                                                        //      ���еķ�����и���
      DestP[IndexD] = MultiplySSE(Conv16, CurKer, Length);
      CurKer += ConvW;
      IndexD += Width;
    }
  }
  IS_FreeMemory(Conv16);
  IS_FreeMemory(Kernel);
  IS_FreeMatrix(&Expand);
  return IS_RET_OK;
}
/// <summary>
/// ����ͼ����ۻ�ƽ����ٶ����Ż�����2015.9.10�գ�
/// </summary>
/// <param name="Src">Դͼ�����ǵ�ģ��ͼ���ڸ�ͼ����Ѱ���ۻ�ƽ���</param>
/// <param name="Template">ģ��ͼ�����Ⱥ͸߶ȶ�Ӧ�ñ�Դͼ��С��</param>
/// <param name="Dest">������ֵ��ʹ��int���;������С����ΪSrc->Width - Template->Width, Src->Height - Template->Height��</param>
/// <remarks> 1:�ۻ��������������ͼ���Ӧλ�����صĲ��ƽ�����ۻ��ͣ�(a-b)^2 = a^2 + b^2 - 2ab��</remarks>
/// <remarks> 2:a(ģ��)��ƽ���Ǹ���ֵ��b��Դͼ���Сͼ����ƽ�������ÿ��ٻ���ͼʵ�֣�a*b�����ÿ��پ��ʵ�֡�</remarks>
IS_RET GetSSD(TMatrix* Src, TMatrix* Template, TMatrix* Dest)
{
  if (Src == NULL || Dest == NULL || Template == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL || Dest->Data == NULL || Template->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Width <= Template->Width || Src->Height <= Template->Height || Src->Channel != Template->Channel || Src->Depth != Template->Depth) {
    return IS_RET_ERR_PARAMISMATCH;
  }
  if (Dest->Width != Src->Width - Template->Width || Dest->Height != Src->Height - Template->Height || Dest->Channel != 1) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  if (Src->Depth != IS_DEPTH_8U || Dest->Depth != IS_DEPTH_32S) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  if (Src->Channel == 1) {
    IS_RET Ret = IS_RET_OK;
    TMatrix* LocalSquareSum = NULL, *XY = NULL;
    Ret = IS_CreateMatrix(Src->Width, Src->Height, IS_DEPTH_32S, 1, &LocalSquareSum);       //      ��������
    do {
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = IS_CreateMatrix(Src->Width, Src->Height, IS_DEPTH_32S, 1, &XY);   //      ��������
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = GetLocalSquareSum(Src, LocalSquareSum, Template->Width, Template->Height);
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = FastConv2(Src, Template, XY);
      if (Ret != IS_RET_OK) {
        break;
      }
      int X, Y, Sum, *LinePP, *LinePXY, *LinePD;
      int Left = Template->Width / 2, Right = Template->Width - Left, Top = Template->Height / 2, Bottom = Template->Height - Top;
      int Width = Dest->Width, Height = Dest->Height;
      for (Y = 0, Sum = 0; Y < Template->Height * Template->WidthStep; Y++) {
        Sum += Template->Data[Y] * Template->Data[Y] ;
      }
      for (Y = 0; Y < Height; Y++) {
        LinePP = (int*)(LocalSquareSum->Data + (Y + Top) * LocalSquareSum->WidthStep) + Left;
        LinePXY = (int*)(XY->Data + (Y + Top) * XY->WidthStep) + Left;
        LinePD = (int*)(Dest->Data + Y * Dest->WidthStep);
        for (X = 0; X < Width; X++) {
          LinePD[X] = Sum + LinePP[X] - 2 * LinePXY[X];                                   //      a^2 + b^2 - 2ab
        }
      }
    }
    while (0);
    IS_FreeMatrix(&LocalSquareSum);
    IS_FreeMatrix(&XY);
    return Ret;
  }
  else {
    TMatrix* GraySrc = NULL, *GrayTemplate = NULL;
    IS_RET Ret = IS_CreateMatrix(Src->Width, Src->Height, Src->Depth, 1, &GraySrc);
    do {
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = IS_CreateMatrix(Template->Width, Template->Height, Template->Depth, 1, &GrayTemplate);
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = ClassicDecolorization(Src, GraySrc);
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = ClassicDecolorization(Template, GrayTemplate);
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = GetSSD(GraySrc, GrayTemplate, Dest);
      if (Ret != IS_RET_OK) {
        break;
      }
    }
    while (0);
    IS_FreeMatrix(&GraySrc);
    IS_FreeMatrix(&GrayTemplate);
    return Ret;
  }
}

/// <summary>
/// ����ͼ��ĺϳ��㷨����2015.9.10�գ�
/// </summary>
/// <param name="Src">Դͼ�����ǵ�ģ��ͼ���ڸ�ͼ����Ѱ���ۻ�ƽ���</param>
/// <param name="Dest">�ϳɺ��ͼ��</param>
/// <param name="TileSizeX">��Ŀ�ȡ�</param>
/// <param name="TileSizeY">��ĸ߶ȡ�</param>
/// <param name="OverlapX">�ص����ֵĿ�ȡ�</param>
/// <param name="OverlapY">�ص����ֵĸ߶ȡ�</param>
/// <remarks> 1:�ٶȻ��Ǻ������ؼ�����������һ�������Զ�����ָ�ꡣ</remarks>
IS_RET __stdcall ImageQuilting(TMatrix* Src, TMatrix* Dest, int TileSizeX, int TileSizeY, int OverlapX, int OverlapY)
{
  if (Src == NULL || Dest == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL || Dest->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Channel != Dest->Channel || Src->Depth != Dest->Depth) {
    return IS_RET_ERR_PARAMISMATCH;
  }
  if (Src->Depth != IS_DEPTH_8U || Dest->Depth != IS_DEPTH_8U) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  if (TileSizeX < 0 || TileSizeY < 0 || OverlapX < 0 || OverlapY < 0) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  if (TileSizeX >= Src->Width || TileSizeY >= Src->Height || OverlapX >= TileSizeX || OverlapY >= TileSizeY) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  int TileX = (int)ceil((double)(Dest->Width - OverlapX) / (TileSizeX - OverlapX));
  int TileY = (int)ceil((double)(Dest->Height - OverlapY) / (TileSizeY - OverlapY));                      //      ÿһ��Ĵ�С
  int NewWidth = TileX * TileSizeX - (TileX - 1) * OverlapX;
  int NewHeight = TileY * TileSizeY - (TileY - 1) * OverlapY;                     //      Ϊ�������ʱ���鷳��һ�ַ�ʽ���ǰ�ͼ��Ŀ�͸߶���չΪ������ΪTileX��TileY���С
  if ((NewWidth != Dest->Width) || (NewHeight != Dest->Height)) {         //      ���ͼ���ܱ������֣��ͽ���һ���ܹ��������ֵ�ͼ��������̴��������������
    TMatrix* Expand = NULL;
    IS_RET Ret = IS_CreateMatrix(NewWidth, NewHeight, Dest->Depth, Dest->Channel, &Expand);                //      ����һ���հ׵�ͼ��
    do {
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = ImageQuilting(Src, Expand, TileSizeX, TileSizeY, OverlapX, OverlapY);
      if (Ret != IS_RET_OK) {
        break;
      }
      for (int Y = 0; Y < Dest->Height; Y++) {               //      ������࿪ʼ����
        memcpy(Dest->Data + Y * Dest->WidthStep, Expand->Data + Y * Expand->WidthStep, Dest->WidthStep);
      }
    }
    while (0);
    IS_FreeMatrix(&Expand);
    return Ret;
  }
  int X, Y, XX, YY, Width, Height, DistW, DistH;
  int StartX, StartY, Sum, Best, Left, Top, Count, Index, Alpha;
  unsigned char* LinePS, *LinePD, *LinePP, *LinePM;
  int* LinePH, *LinePV, *LinePHV, *LinePT;
  Width = Src->Width, Height = Src->Height;
  DistW = Width - TileSizeX, DistH = Height - TileSizeY;
  TMatrix* Mask = NULL, *MaskX = NULL, *MaskY = NULL;                     //      ��С����·���ָ��Ӧ���ɰ�ͼ
  TMatrix* TileH = NULL, *TileV = NULL, *TileHV = NULL, *SmallTileH = NULL;
  TMatrix* Distance = NULL, *ImgOverlapX = NULL, *ImgOverlapY = NULL, *DistanceH = NULL, *DistanceV = NULL, *DistanceHV = NULL ;
  IS_RET Ret = IS_CreateMatrix(DistW, DistH, IS_DEPTH_32S, 1, &Distance);                                                         //      ��������
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(TileSizeX, TileSizeY, IS_DEPTH_8U, 1, &Mask);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(OverlapX, TileSizeY, IS_DEPTH_32S, 1, &ImgOverlapX);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(TileSizeX, OverlapY, IS_DEPTH_32S, 1, &ImgOverlapY);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(OverlapX, TileSizeY, IS_DEPTH_8U, 1, &MaskX);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(TileSizeX, OverlapY, IS_DEPTH_8U, 1, &MaskY);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(TileSizeX, OverlapY, IS_DEPTH_8U, Src->Channel, &TileH);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(OverlapX, TileSizeY, IS_DEPTH_8U, Src->Channel, &TileV);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(OverlapX, OverlapY, IS_DEPTH_8U, Src->Channel, &TileHV);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(Width - TileSizeX, Height - OverlapY , IS_DEPTH_32S, 1, &DistanceH);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(Width - OverlapX, Height - TileSizeY, IS_DEPTH_32S, 1, &DistanceV);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(Width - OverlapX, Height - OverlapY, IS_DEPTH_32S, 1, &DistanceHV);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  memset(Dest->Data, 0, Dest->Height * Dest->WidthStep);
  for (YY = 0; YY < TileY; YY++) {
    StartY = YY * TileSizeY - YY * OverlapY;                                //      �µĿ�����λ��
    for (XX = 0; XX < TileX; XX++) {
      StartX = XX * TileSizeX - XX * OverlapX;
      if (XX == 0 && YY == 0) {                                               //      ���ڵ�һ���飬����Ĵ�ԴͼƬ�п���һ���������
        Left = Rand() * (Width - TileSizeX);                    //      ����λ�ð��տ����ԭ�򣬲�Ҫע��Խ��
        Top = Rand() * (Height - TileSizeY);
        for (Y = 0; Y < TileSizeY; Y++) {
          memcpy(Dest->Data + Y * Dest->WidthStep, Src->Data + (Y + Top) * Src->WidthStep + Left * Src->Channel, TileSizeX * Src->Channel) ;
        }
      }
      else {
        ///     *****************************���²��ִ������������������ƵĿ�***************************************
        ///     1�� ע������Ŀ��ںܶ�����²����Ƿ��������Ŀ飬ͨ����L�εģ�
        ///     2�� ����ñ���ʵ�֣�ע������ж�Ŀ����Ƿ��Ѿ������ݣ������û�����ݵľͲ�������������������С�
        memset(Distance->Data, 0, Distance->Height * Distance->WidthStep);                      //      �Ѿ�����������
        if (YY > 0) {                                                                           //      �ڴ�ֱ�������غ���
          for (Y = 0; Y < OverlapY; Y++) {                        //      ������ֱ�����غϵ����ݵ�һ��������TileH���ݽṹ�У���Ҫ��Ϊ�˷��㣬��СTileSizeX * OverlapY
            memcpy(TileH->Data + Y * TileH->WidthStep, Dest->Data + (Y + StartY) * Dest->WidthStep + StartX * Dest->Channel, TileH->Width * TileH->Channel);
          }
          Ret = GetSSD(Src, TileH, DistanceH);                            //      �����ⲿ�ֵ�ԭͼ������ľ���
          if (Ret != IS_RET_OK) {
            goto Done;
          }
          for (Y = 0; Y < Distance->Height; Y++) {        //      �Ѽ���Ľ����ֱ�ӿ��������������
            memcpy(Distance->Data + Y * Distance->WidthStep, DistanceH->Data + Y * DistanceH->WidthStep, Distance->WidthStep);
          }
        }
        if (XX > 0) {                                                                           //      ��ˮƽ�������غ���
          for (Y = 0; Y < TileSizeY; Y++) {                       //      ����ˮƽ�����غϵ����ݵ�һ��������TileV���ݽṹ�У���Ҫ��Ϊ�˷��㣬��СOverlapX * TileSizeY
            memcpy(TileV->Data + Y * TileV->WidthStep, Dest->Data + (Y + StartY) * Dest->WidthStep + StartX * Dest->Channel, TileV->Width * TileV->Channel);
          }
          Ret = GetSSD(Src, TileV, DistanceV);
          if (Ret != IS_RET_OK) {
            goto Done;
          }
          if (YY > 0) {                                                                   //      ��ʱ�϶����ص��Ĳ�����
            for (Y = 0; Y < OverlapY; Y++) {                //      �����ص���������
              memcpy(TileHV->Data + Y * TileHV->WidthStep, Dest->Data + (Y + StartY) * Dest->WidthStep + StartX * Dest->Channel, TileHV->Width * TileHV->Channel);
            }
            Ret = GetSSD(Src, TileHV, DistanceHV);
            if (Ret != IS_RET_OK) {
              goto Done;
            }
            for (Y = 0; Y < DistH; Y++) {
              LinePT = (int*)(Distance->Data + Y * Distance->WidthStep);
              LinePV = (int*)(DistanceV->Data + Y * DistanceV->WidthStep);
              LinePHV = (int*)(DistanceHV->Data + Y * DistanceHV->WidthStep);
              for (X = 0; X < DistW; X++) {
                LinePT[X] += LinePV[X] - LinePHV[X];  //       ��Ҫ�����ص��Ĳ���
              }
            }
          }
          else {
            for (Y = 0; Y < Distance->Height; Y++) {                                                                                                //      ֱ�ӿ���
              memcpy(Distance->Data + Y * Distance->WidthStep, DistanceV->Data + Y * DistanceV->WidthStep, Distance->WidthStep);
            }
          }
        }
        //      Ѱ�������ƵĿ����㣨���Ͻǣ�������
        for (Y = 0, Best = MaxValueI; Y < DistH; Y++) {
          LinePT = (int*)(Distance->Data + Y * Distance->WidthStep);
          for (X = 0; X < DistW; X++) {
            if (LinePT[X] < Best) {
              Best = LinePT[X];                               //      ��Щ�㷨�������Ӹ��ݲ���ݲ�С�����ƥ���Ŀ������ѡȡһ����Ϊѡ�е�Patch,��Ҳδ�����ɲ��������ﻹ��ֱ��ѡ������ѿ�
              Left = X;
              Top = Y;
            }
          }
        }
        //Index = 0;
        /*for(Y = 0; Y < TileSizeY; Y++)
        {
                memcpy(Dest->Data + (Y + StartY) * Dest->WidthStep + StartX * Dest->Channel, Src->Data + (Y + Top) * Src->WidthStep + Left * Src->Channel, TileSizeX * Src->Channel) ;
        }*/
        memset(Mask->Data, 255, Mask->Height * Mask->WidthStep);
        if (YY > 0) {
          for (Y = 0; Y < OverlapY; Y++) {
            LinePT = (int*)(ImgOverlapY->Data + Y * ImgOverlapY->WidthStep);
            LinePS = Src->Data + (Y + Top) * Src->WidthStep + Left * Src->Channel;
            LinePD = Dest->Data + (Y + StartY) * Dest->WidthStep + StartX * Dest->Channel;
            for (X = 0; X < TileSizeX; X++) {
              LinePT[X] = (LinePD[0] - LinePS[0]) * (LinePD[0] - LinePS[0]) + (LinePD[1] - LinePS[1]) * (LinePD[1] - LinePS[1]) + (LinePD[2] - LinePS[2]) * (LinePD[2] - LinePS[2]);
              LinePD += Dest->Channel;
              LinePS += Src->Channel;
            }
          }
          Ret = MinCutHorizontal(ImgOverlapY, MaskY);
          if (Ret != IS_RET_OK) {
            goto Done;
          }
          for (Y = 0; Y < OverlapY; Y++) {
            memcpy(Mask->Data + Y * Mask->WidthStep, MaskY->Data + Y * MaskY->WidthStep, TileSizeX);
          }
        }
        if (XX > 0) {
          for (Y = 0; Y < TileSizeY; Y++) {
            LinePT = (int*)(ImgOverlapX->Data + Y * ImgOverlapX->WidthStep);
            LinePS = Src->Data + (Y + Top) * Src->WidthStep + Left * Src->Channel;
            LinePD = Dest->Data + (Y + StartY) * Dest->WidthStep + StartX * Dest->Channel;
            for (X = 0; X < OverlapX; X++) {
              LinePT[X] = (LinePD[0] - LinePS[0]) * (LinePD[0] - LinePS[0]) + (LinePD[1] - LinePS[1]) * (LinePD[1] - LinePS[1]) + (LinePD[2] - LinePS[2]) * (LinePD[2] - LinePS[2]);
              LinePD += Dest->Channel;
              LinePS += Src->Channel;
            }
          }
          Ret = MinCutVertical(ImgOverlapX, MaskX);
          if (Ret != IS_RET_OK) {
            goto Done;
          }
          for (Y = 0; Y < TileSizeY; Y++) {
            LinePS = MaskX->Data + Y * MaskX->WidthStep;
            LinePM = Mask->Data + Y * Mask->WidthStep;
            for (X = 0; X < OverlapX; X++) {
              LinePM[X] &= LinePS[X] ;
            }
          }
        }
        for (Y = 0; Y < TileSizeY; Y++) {
          LinePS = Src->Data + (Y + Top) * Src->WidthStep + Left * Src->Channel;
          LinePD = Dest->Data + (Y + StartY) * Dest->WidthStep + StartX * Dest->Channel;
          LinePM = Mask->Data + Y * Mask->WidthStep;
          for (X = 0; X < TileSizeX; X++) {
            Alpha = LinePM[X];
            if (Alpha == 255) {                                     //      �󲿷ֵĶ�����255��
              LinePD[0] = LinePS[0];
              LinePD[1] = LinePS[1];
              LinePD[2] = LinePS[2];
            }
            LinePD += Dest->Channel;
            LinePS += Src->Channel;
          }
        }
      }
    }
  }
Done:
  IS_FreeMatrix(&Distance);
  IS_FreeMatrix(&Mask);
  IS_FreeMatrix(&ImgOverlapX);
  IS_FreeMatrix(&ImgOverlapY);
  IS_FreeMatrix(&MaskX);
  IS_FreeMatrix(&MaskY);
  IS_FreeMatrix(&TileH);
  IS_FreeMatrix(&TileV);
  IS_FreeMatrix(&TileHV);
  IS_FreeMatrix(&DistanceH);
  IS_FreeMatrix(&DistanceV);
  IS_FreeMatrix(&DistanceHV);
  return Ret;
}

