
// PhotoShop�㷨ԭ�����ϵ�� - ���ػ�---����Ƭ
// ��ͼ�񴴽��ĸ��໥ƫ�Ƶĸ���������������Ӱ��Ч��
// ƫ�Ƶ����ľ�����ÿ������Ϊ���ģ�4��ƫ�Ʒֱ������ĶԳƣ�б45�Ⱦ���Բ�ܲ��ã�
// ˮƽ�ʹ�ֱƫ�Ƹ�45�ȣ�ƫ����4�����ء�
int fragment(int h, int w, const byte* A, int al, byte* B, int bl, int ai)
{
  int x, y, z, xx, yy;
  int Speed, Index;
  int SumR, SumG, SumB;

  // ÿ�����ƫ����
  int OffsetX[] = { 4, -4, -4, 4 };
  int OffsetY[] = { -4, -4, 4, 4 };

  ASSERT(ai >= 3);

  for (y = 0; y < h; y++) {
    Speed = y * bl;

    for (x = 0; x < w; x++) {
      SumB = 0;
      SumG = 0;
      SumR = 0;

      for (z = 0; z < 4; z++) {
        //  �ۻ�ȡ�����ȡ����
        xx = x + OffsetX[z], yy = y + OffsetY[z];
        // ע��Խ��
        xx = CLAMP(xx, 0, w - 1), yy = CLAMP(yy, 0, h - 1);
        Index = yy * al + xx * 3;
        SumB += A[Index + 0];
        SumG += A[Index + 1];
        SumR += A[Index + 2];
      }

      //  ��ƽ��ֵ(Sum+2)/4,ΪʲôҪ+2����Ϊ���������롣�������������Ϊ108.6����ȡ����109��Ϊ����
      B[Speed + 0] = ((SumB + 2) >> 2);
      B[Speed + 1] = ((SumG + 2) >> 2);
      B[Speed + 2] = ((SumR + 2) >> 2);
      //  ������һ������
      Speed += ai;
    }
  }

  return 0;
}
