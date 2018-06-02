//
// ����/�Աȶȵ���
//
// Photoshopͼ������/�Աȶȵ�����ԭ��
//
//     һ���Աȶ��㷨��ʽ��
//
//     Photoshop���ڶԱȶ��������ǰ�����ֵ�������ֱ���ġ�
//
//     �����newRGB��ʾͼ�������µ�R��G��B������RGB��ʾͼ������R��G��B������ThresholdΪ�����ķ�ֵ��ContrastΪ�Աȶ���������Contrast����0ʱ��
//
//          1) newRGB = RGB + (RGB - Threshold) * (1 / (1 - Contrast / 255) - 1)
//
//     ���У���Contrast����255ʱ(RGB - Threshold) * (1 / (1 - Contrast / 255) - 1)Ϊ����(��)������RGB�����Сֵ�ֱ�Ϊ255��0����ˣ�ֻ�ܰ�Threshold��ȷ��newRGB����newRGB = RGB >= Threshold? 255 : 0����ʵ�ʾ�������ͼ��ֵ��ͼ������������ɫ��ɣ����졢�ơ��̡��ࡢ�����ϼ�����ף��ڻҶ�ͼ��Ҳֻ�����8���ߡ�
//
//     ��ContrastС��0ʱ��
//
//         2) newRGB = RGB + (RGB - Threshold) * Contrast / 255
//
//     ���У���Contrast����-255ʱ��ͼ��RGB�����������ڷ�ֵ��ͼ���ȫ��ɫ���Ҷ�ͼ��ֻ��1���ߣ�����ֵ�Ҷȡ�
//
//     ����ͼ�����ȵ��������Ĳ��õ�����õķ��������ȵ�����Phoposhop CS3���°汾Ҳ���������ȵ�����ʽ��CS3�����ϰ汾Ҳ�����˸����ȵ�����ʽ��ѡ���
//
//     ����ͼ������/�Աȶ��ۺϵ����㷨������ܼ򵥣������ȡ��Աȶ�ͬʱ����ʱ������Աȶȴ���0���ֵ������ȣ��ٵ����Աȶȣ����Աȶ�С��0ʱ�����෴���ȵ����Աȶȣ��ٵ������ȡ�
//
//     ��������BCB2007��GDI+λͼ����д��Photoshopͼ������/�Աȶȵ������룬�������Ӵ��룺
//
// ������/�Աȶȵ�������BrightAndContrast�У����Ȱ�ǰ����ܵ�ԭ��������һ��256��Ԫ�ش�С�Ĳ��ұ�Ȼ���ͼ�����������ذ�R��G��B����ֵ�ڲ��ұ���ȡ�õ���������ݣ���˴����ٶ��൱�졣
// ���������ӳ������н����ͼ���Աȶ�255ʱ����


//---------------------------------------------------------------------------
INT CheckValue(INT value)
{
  return (value & ~0xff) == 0 ? value : value > 255 ? 255 : 0;
}
//---------------------------------------------------------------------------

// ����/�Աȶȵ���
int BrightAndContrast(const img_t* im, INT bright, INT contrast, BYTE threshold, img_t* im2)
{
  BYTE values[256] = {0};
  FLOAT cv;
  INT i, x, y;

  if (bright == 0 && contrast == 0) {
    return 0;
  }

  cv = contrast <= -255 ? -1.0f : contrast / 255.0f;

  if (contrast > 0 && contrast < 255) {
    cv = 1.0f / (1.0f - cv) - 1.0f;
  }


  for (i = 0; i < 256; i ++) {
    INT v = contrast > 0 ? CheckValue(i + bright) : i;

    if (contrast >= 255) {
      v = v >= threshold ? 255 : 0;
    }
    else {
      v = CheckValue(v + (INT)((v - threshold) * cv + 0.5f));
    }

    values[i] = contrast <= 0 ? CheckValue(v + bright) : v;
  }

  imsetsamesize(im2, im);

  switch (im->c) {
  case 1:
    for (y = 0; y < im->h; ++y) {
      const uchar* p = im->tt.data + y * im->s;
      uchar* p2 = im2->tt.data + y * im2->s;

      for (x = 0; x < im->w; ++x, p += im->c, p2 += im->c, p2 += im2->c) {
        p2[0] = values[p[0]];
      }
    }

    break;

  case 3:
  case 4:
    for (y = 0; y < im->h; ++y) {
      const uchar* p = im->tt.data + y * im->s;
      uchar* p2 = im2->tt.data + y * im2->s;

      for (x = 0; x < im->w; ++x, p += im->c, p2 += im2->c) {
        p2[0] = values[p[0]];
        p2[1] = values[p[1]];
        p2[2] = values[p[2]];
      }
    }

    break;
  }

  return 0;
}
//---------------------------------------------------------------------------

int test_BrightAndContrast()
{
  img_t im[3] = {0};
  img_t* im1 = im + 1;
  img_t* im2 = im + 2;
  const char* name = "����/�Աȶȵ��� ";
  int ch = 0, bright = 0, contrast = 255, threshold = 121;
  _chdir("D:/pub/pic");
  imread("source1.jpg", 3, 1, im);
  imshow2(name, im);

  cvCreateTrackbar("����", name, &bright, 256, 0);
  cvCreateTrackbar("�Աȶ�", name, &contrast, 256, 0);
  cvCreateTrackbar("��ֵ", name, &threshold, 256, 0);

  for (; 'q' != ch && 'Q' != ch;) {
    BrightAndContrast(im, bright, contrast, threshold, im1);
    imcath(2, im, im2);
    imshow2(name, im1);
    ch = cvWaitKey(50);
  }

  imfree(im);
  return 0;
}
//---------------------------------------------------------------------------

