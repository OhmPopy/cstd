// 1������ͼ�������ƫɫ��⼰��ɫУ������ ������ , ������ , ������ , �ֿ˱� , ����ݥ
// 2��Ƥ����⼼�����о����Ľ�
// Ϊ��������⣬���ﻹ�Ǵ���Щ������ժȡЩ����Ĺ�������������
// ����RGB��ɫ�ռ�����򵥵�һ����ɫ�ռ䣬����RGB��ɫ�ռ����ľ��������ڵ���ŷ�Ͼ������̻�������ɫ֮��Ĳ���ʱ�����������������ɫ֮��ľ��޷���ȷ��������ʵ������֪������������ɫ֮�����ʵ���졣����CIE Lab��ɫ�ռ䣬�˿ռ��������������ɫ֮��ľ�����ʵ�ʸ�֪�ϵĲ�����һ�¡���ֱ��ͼ���Կ͹۵ķ�ӳͼ��ɫƫ�̶ȣ���CIE Lab�½���ƫɫͼ����Զ�����Ϊ����
//       ����������ͼ���ƫɫͼ��ķ������֣������abɫ������ƽ���ϵ�ֱ��ͼ�У�ɫ�ȷֲ�������Ϊ����ֵ�����߷ֲ���Ϊ���У���ɫ��ƽ��ֵD�ֽϴ�ʱ��һ�㶼����ɫƫ������ɫ��ƽ��ֵԽ��ɫƫԽ���ء�Ȼ���������abɫ������ƽ���ϵ�ֱ��ͼ��ɫ�ȷֲ��������ԵĶ��ֵ�����ҽ�Ϊ��ɢʱ��ͼ��ɫƫ�̶Ƚ������ᣬ����û��ɫƫ��
double GetColorCastFactor(int Width, int Height, const byte* BmpData, int Stride)
{
  int X, Y;

  int SumA = 0, SumB = 0;
  double MsqA = 0, MsqB = 0, AvgA, AvgB;
  int A, B;

  int HistA[256] = {0};
  int HistB[256] = {0};
  int LabStride = Width * 3;
  byte Pointer[4];

  for (Y = 0; Y < Height; Y++) {
    for (X = 0; X < Width; X++) {
      RGBToLAB(BmpData + Y * Stride + X * 3, Pointer, 1);
      A = *(Pointer + 1);
      B = *(Pointer + 2);
      SumA += A;
      SumB += B;
      HistA[A]++;
      HistB[B]++;
    }
  }

  AvgA = 1.*SumA / (Width * Height) - 128;                  // �����һ����[-128��,127]��Χ��
  AvgB = 1.*SumB / (Width * Height) - 128;

  for (Y = 0; Y < 256; Y++) {
    MsqA += (fabs(Y - AvgA - 128) * HistA[Y]) / (Width * Height);           // �÷���ķ�ʽ���������
    MsqB += (fabs(Y - AvgB - 128) * HistB[Y]) / (Width * Height);
  }

  return sqrt(AvgA * AvgA + AvgB * AvgB) / sqrt(MsqA * MsqA + MsqB * MsqB);
}

