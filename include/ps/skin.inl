//1�����ζ���ʽ���ģ��
// ���ζ���ʽ���ģ��������SORIANO������˺�CHIANG�Դ˽����˸Ľ����Ľ����ģ��������R-Gƽ��Ķ��ζ���ʽ��һ��Բ���̹��ɣ�
//            l_upper(r)=-1.3767r^2 + 1.0743r+0.1452           (1)
//            l_lower(r)=-0.776r^2 + 0.5601r+0.1766            (2)
//            W(r,g) = (r-0.33)^2 + (g-0.33)^2                 (3)
//    �������������̵Ļ����ϣ���ɫ�������ͨ��һ�¹���ʵ�֣�
//     R1:g>l_lower(r) and g<l_upper(r)
//     R2:W(r,g)>=0.0004
//     R3:R>G>B
//     R4:R-G>=45
//�����㷨�Ĳο����� Adaptive skin color modeling using the skin locus.pdf
//                A novel method for detecting lips,eyes and faces in real time
//�Լ��ٶ��Ŀ�������£����ڻ�Ϸ�ɫģ�͵Ŀ�����������㷨
//     ��ʽ�У�Сдr,g,b��δ�漰��Ϊ��R/G/B(byte���͵����ݣ�0-255�����й�һ��������ݣ�����
// r=R/(R+G+B);   g=G/(R+G+B);   b=B/(R+G+B);
//  ������ʾ���㷨���漰���˲��ٵĸ������㣬�Լ������ĳ˷����������Դ֭ԭζ������д���룬�����Ч�ʿ����֪����ˣ������������㷨���Ż���
//  ���ȣ����������ĸ��ж������������ж������ǲ����Ⱥ���Ҫͬʱ����ĵط������������Ӧ�ðѼ򵥵��ж�����������ǰ���жϡ�
//  ���ȿ�����������ж�����R4������R3�е�R>G�϶����Ѿ������ģ���ֻ��Ҫ�ж�G�Ƿ����B�������Ż��ֶ�1��
//  Ȼ����������R2���Ż���Ϊ���������������Sum=R+G+B�����ж�����R2չ����
//W(r,g)>=0.0004        => (r-0.33)^2 + (g-0.33)^2>0.0004   =>
//(R/Sum-0.33)^2 + (G/Sum-0.33)^2>0.0004   => (R-0.33Sum)^2 + (G-0.33Sum)^2>0.0004*Sum*Sum   =>
//(156R-52Sum)^2 + (256G-52Sum)^2>=0.062*Sum*Sum
//     ��ʽ�����һ��ͬʱ����156, ������˵156��0.33=51.48����Ӧ��ȡ52�ģ��������0.33�������Ǹ��������ݣ�˭˵������1/3�ء�
//     ���ˣ����ǿ�����ʽ�ӵ����Ҳ໹�и�������0.0624����������������ݣ��㷨�ٶ����ɻ��д��Ӱ�죬�����о���λ�����ѿ϶���0.0625������ֺ���Ϥ��1/16=0.0625�������𣬶����𣬻�������������ɣ������ʽ�Ӻܶ඼�Ǿ��鹫ʽ����ˣ���΢�޸�һЩ�����Խ��������Ӱ��)��
//     ������������Ŀ�ģ��޷��ǽ�������������ȫ��ת��Ϊ���������㡣
//    �������ʽR1���Ż���R1ʵ����Ҳ�����������������ֿ������ֱ��ΪR11��R12������R11��ͬ��չ����
// ���ڴ󲿷ֵ�PC������32λ��ϵͳ����ˣ�ʹ��32λ�����������������ٶ������ģ���ˣ���������Ŵ�ϵ����ȡ��ͱ�����Ҫʹ�ü���ʽ���ߵ�ֵ����int.MinValue�� int.MaxValue֮�䣬������ʽ��>�������ʽ�Ŀ������ȡֵΪ10000��255��765����С��int.MaxValue���ܱ��ķ�Χ�ģ���˷Ŵ�ϵ���Ǻ���ġ�
int skin_Adaptive(int Width, int Height, const unsigned char* Scan0, int Stride, unsigned char* SkinScan0, int SkinStride, int cn)
{
  int X, Y;

  for (Y = 0; Y < Height; Y++) {
    const unsigned char* Pointer = Scan0 + Y * Stride;
    unsigned char* SkinP = SkinScan0 + Y * SkinStride;

    for (X = 0; X < Width; X++) {
      int Blue, Green, Red, Lower, Upper;
      *SkinP = 0;                                                                  // ��Ƥ������Ϊ��ɫ

      Blue = *Pointer;
      Green = *(Pointer + 1);
      Red = *(Pointer + 2);

      if (Red - Green >= 45) {                                            //  ��������R4
        if (Green > Blue) {                                             //   ��������R3
          int Sum = Red + Green + Blue;
          int T1 = 156 * Red - 52 * Sum;
          int T2 = 156 * Green - 52 * Sum;

          if (T1 * T1 + T2 * T2 >= (Sum * Sum) >> 4) {                  // ��������R2����32λϵͳҪ����������long�������ݣ�
            T1 = 10000 * Green * Sum;
            Lower = - 7760 * Red * Red + 5601 * Red * Sum + 1766 * Sum * Sum;         // ������Ĺ��õĳ˷���ȡ��������ûɶ�Ż���Ч��

            if (T1 > Lower) {                                       // ��������R11
              Upper = - 13767 * Red * Red + 10743 * Red * Sum + 1452 * Sum * Sum ;

              if (T1 < Upper) {                                   //  ��������R12
                *SkinP = 255;
              }
            }
          }
        }
      }

      Pointer += 3;
      SkinP++;
    }
  }

  return 0;
}


//2������RGB��ɫ�ռ�ļ���ֵ��ɫʶ��
//       ��human skin color clustering for face detectionһ����������¼򵥵��б���ʽ��
//            R>95 And G>40 And B>20 And R>G And R>B And Max(R,G,B)-Min(R,G,B)>15 And Abs(R-G)>15
//      �㷨�ǳ�֮�򵥣�ͬ����Ҫ�Ѹ��ӵ��ж������ŵ�����ȥ�жϣ�����Ч�Ľ��ͳ����ִ��ʱ�䣬�ο����룺
int skin_rgb(int Width, int Height, const unsigned char* Scan0, int Stride, unsigned char* SkinScan0, int SkinStride, int cn)
{
  int X, Y;

  for (Y = 0; Y < Height; Y++) {
    const unsigned char* Pointer = Scan0 + Y * Stride;
    unsigned char* SkinP = SkinScan0 + Y * SkinStride;

    for (X = 0; X < Width; X++) {
      int Blue = *Pointer;
      int Green = *(Pointer + 1);
      int Red = *(Pointer + 2);
      int Max, Min;

      if (Red > 95 && Green > 40 && Blue > 20 && Red > Blue && Red > Green && abs(Red - Green) > 15) {
        if (Blue >= Green) {
          Max = Blue;
          Min = Green;
        }
        else {
          Max = Green;
          Min = Blue;
        }

        if (Red > Max) {
          Max = Red;
        }
        else if (Red < Min) {
          Min = Red;
        }

        if (Max - Min > 15) {
          *SkinP = 255;
        }
      }

      Pointer += 3;
      SkinP++;
    }
  }

  return 0;
}
//����YCbCr��ɫ�ռ�ļ���ֵ��ɫʶ��
//    ���㷨���Ϊ�򵥣���ͼ��ת����YCbCr��ɫ�ռ䣬Ȼ����������ʽ�ж��Ƿ�����Ƥ������
//        (Cb > 77 And Cb < 127)  And (Cr > 133 And Cr < 173)
//      ����RGB��YCbCr��ɫ�ռ��ת�����Ż��㷨���ɲο�������������¡�
//      ���ڵ���д�ⷽ���ʱ��û��ע�����㷨�ĳ���������Ҳû�������ˡ�

int skin_YCbCr(int Width, int Height, const unsigned char* Scan0, int Stride, unsigned char* SkinScan0, int SkinStride, int cn)
{
  int X, Y;

  for (Y = 0; Y < Height; Y++) {
    const unsigned char* Pointer = Scan0 + Y * Stride;
    unsigned char* SkinP = SkinScan0 + Y * SkinStride;

    for (X = 0; X < Width; X++) {
      int Blue = *Pointer;
      int Green = *(Pointer + 1);
      int Red = *(Pointer + 2);
      int Cb = (-176933 * Red - 347355 * Green + 524288 * Blue + 134217728) >> 20;

      if (Cb > 77 && Cb < 127) {
        int Cr = (524288 * Red - 439026 * Green - 85262 * Blue + 134217728) >> 20;

        if (Cr > 133 && Cr < 173) {
          *SkinP = 255;
        }
      }

      Pointer += 3;
      SkinP++;
    }
  }

  return 0;
}