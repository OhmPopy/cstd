//����Simple Image Statistics����ͼ��ͳ��,SIS)��ͼ���ֵ���㷨��
// ���Ǹ��򵥵��㷨����ȫ�ֶ�ֵ�㷨��һ�֣��㷨ִ���ٶȿ졣
//  �㷨���̼��������£�
// ����ÿһ�����أ������´���
// 1�����㵱ǰ����ˮƽ�ʹ�ֱ������ݶȡ� ��two gradients are calculated  |I(x + 1, y) - I(x - 1, y)| and |I(x, y + 1) - I(x, y - 1)|);
// 2��ȡ�����ݶȵ����ֵ��ΪȨ�ء���weight is calculated as maximum of two gradients��;
// 3������Ȩ�صĺ͡���sum of weights is updated (weightTotal += weight)��;
// 4�����¼�Ȩ����֮�� ��sum of weighted pixel values is updated (total += weight * I(x, y))����
//   ֮�����յ���ֵȥ��Ȩ����֮�ͺ�Ȩ��֮�������ֵ��
//   ����������ݣ��һ���ϲ���Լ������ڴ棬�����Ҹ�������ֱ��ʹ��API��������������ϰ���йذɣ�����Ҳ���԰����Լ��ķ�ʽ������

byte GetSimpleStatisticsThreshold(int Width, int Height, const byte* Scan0, int Stride)
{
  int X, Y;
  int Ex, Ey;
  int Weight = 0;
  long SumWeight = 0;                 //  ���ڴ�ͼ��������ֻ������������long���͵ı���

  for (Y = 1; Y < Height - 1; Y++) {
    const byte* Pointer = Scan0 + (Y + 1) * Stride + 1;

    for (X = 1; X < Width - 1; X++) {
      Ex = *(Pointer - 1) - *(Pointer + 1);

      if (Ex < 0) {
        Ex = -Ex;
      }

      Ey = *(Pointer - Stride) - *(Pointer + Stride);

      if (Ey < 0) {
        Ey = -Ey;
      }

      if (Ex > Ey) {
        Weight += Ex;
        SumWeight += *Pointer * Ex;
      }
      else {
        Weight += Ey;
        SumWeight += *Pointer * Ey;
      }

      Pointer++;
    }
  }

  if (Weight == 0) {
    return *(Scan0);  //  ˵�����е���ɫֵ����ͬ
  }

  return (byte)(SumWeight / Weight);
}

