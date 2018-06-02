
// ������Ч 
// �е�app����ӰЧ�����е�app�а���Ч������ʵ������ͼ��Ϊ���ģ����ͼ���Ľǵ����Ȼ��߽������ȡ�
// Ϊͼ���Ľ�������ȵĲ��࣬����Ļ��Ǹ�ͼ�����Ӱ���Ч������ͻ��ͼ�����塣 
// �����Ǻܶ�lomo�˾������������һ�����ǡ���ô��������ôʵ�ֵ��أ����ƺܼ򵥣�
// ��������ps��ɫ��������Ȼ��ƽ�������������������Ҫһ����������Եġ��㷨������û�и���ԭ������㣬����������������ˡ�
// ���������ʾ�����루����: strength��ΧΪ[0.0f, 1.0f], ����: strength��ΧΪ[-1.0f, 0.0f]����

#define CLAMP0255(x)  BOUND(x, 0, 255)
#define SCREEN_XY(a, b)  ((a*b)>>8)
void Vignetting(int h, int w, const uchar* src, int srcstep, uchar* dst, int dststep, int cn, float strength)  
{    
  float radio = 0.0;  
  float maxLen = (float)MAX(w, h);  
  float cx = maxLen * 0.5f;  
  float cy = maxLen * 0.5f;  
  float maxDist = cx*cx + cy*cy;
  float t0, t1, t2;
  int i, j;
  
  if (strength > 0.0f)  
  {
    // ����  
    for (i = 0; i < h; i++) {
      const uchar* src1 = src + i*srcstep;
      uchar* dst1 = dst + i*dststep;
      for (j = 0; j < w; j++, src1 += cn, dst1 += cn) {
        float nx = (float)maxLen * j / w;
        float ny = (float)maxLen * i / h;
        float curDist = (nx - cx)*(nx - cx) + (ny - cy)*(ny - cy);  
        radio = curDist / maxDist;  
        // radio *= radio;
        radio *= strength;
        t0 = src1[0]*(1.0f - radio) + SCREEN_XY(src1[0], 230)*radio;
        t1 = src1[1]*(1.0f - radio) + SCREEN_XY(src1[1], 230)*radio;
        t2 = src1[2]*(1.0f - radio) + SCREEN_XY(src1[2], 230)*radio;
        dst1[0] = (uchar)CLAMP0255(t0);
        dst1[1] = (uchar)CLAMP0255(t1);
        dst1[2] = (uchar)CLAMP0255(t2);
      }  
    }  
  }
  else  
  {
    // ����  
    for (i = 0; i < h; i++) {
      const uchar* src1 = src + i*srcstep;
      uchar* dst1 = dst + i*dststep;
      for (j = 0; j < w; j++, src1 += cn, dst1 += cn) {
        float nx = (float)maxLen * j / w;  
        float ny = (float)maxLen * i / h;  
        float curDist = (nx - cx)*(nx - cx) + (ny - cy)*(ny - cy);  
        radio = curDist / maxDist;  
        // radio *= radio;  
        radio *= strength;  
        radio = 1.0f + radio;
        t0 = src1[0]*radio;
        t1 = src1[1]*radio;
        t2 = src1[2]*radio;
        dst1[0] = (uchar)CLAMP0255(t0);  
        dst1[1] = (uchar)CLAMP0255(t1);  
        dst1[2] = (uchar)CLAMP0255(t2);  
      }  
    }  
  }  
}  

