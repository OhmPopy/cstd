//#include "img/color.inl"
// Seam Carving �㷨��2007��siggraph�ϵ�һƪpaper��
// ��Ϊ��Seam Carving for Content-Aware Image Resizing������ƪpaper�����һ�����ݸ�֪��ͼ�������㷨������ƪpaper��˼�룬
//����������siggraph����Ҳ�кü�ƪ�������ݸ�֪��ͼ�������㷨�����ݸ�֪��ͼ�������㷨һ������ͼ��Ĳü��������е�ʱ��
//�����һ����Ƭ�е����ϣ�������ü���СһЩ�����������뱣����Ƭ�е����壬���ʱ���Ҫ�õ����ݸ�֪��ͻ�̰������㷨�ˡ�
//�ÿ�ʼѧϰ����㷨��ʱ�򣬿���������ܸ���Ī������ӣ����ۿ���������ţ�ƺ�壬ʲô��������̬�滮���������ŵ��ˣ�������۵Ķ�����ʵ�����ܲ����ˣ����ǹ���ֱ�ӽ�����룬���˴����Ժ󣬲�֪������㷨��ʵ�ǳ��򵥣��ѶȾͱ���ͨ��ͼ����������ôһ��㣬��ʵ�㷨��ԭ��ܼ򵥡��ϻ�����˵�����ֻ������С�㷨������ͼ������С���㷨������ͼ��߶���С��ֻ���ͼ��ת�ô�����֮paper���������ܵ�ʵ�֣���ͬС�죬��������˵�ļ�һ�����ͨ��Ѱ��һ���ߣ��������ͼ1��ʾ���ҵ������Ժ󣬰��������ϵ����ص�ȫ��ɾ�����Ӷ�ʵ����С������
//�ù�����ͨ������ʵ�ֵģ�ÿ��ֻ���Ƴ�һ�У�����ֱ���������Ҫ���������Ѱ���Ƴ������ߵķ�������Ҫע����ǣ�ÿ���Ƴ�һ�У���Ҫ���¼�������ͼ��Ҳ����˵���ݶ�ͼҪ���¼��㣬���ܽ�����һ�ֵ��Ƴ�������
// http://www.faculty.idc.ac.il/arik/SCWeb/imret/
// http://www.inf.ufrgs.br/~eslgastal/DomainTransform/
#include "img/imgopt.inl"
//#include "img/imedge.inl"
int imSobel_mag(const img_t* im, img_t* im2)
{
  img_t im1[1] = {0};
  imreplicate_border(im, im1, iRECT(1, 1, 1, 1));
  im = im1;
  {
    int x, y, w = im->w - 2, h = im->h - 2, s = im->s;
    imsetsize(im2, h, w, sizeof(ushort), 1);
    for (y = 0; y < h; ++y) {
      const uchar* src = im->tt.data + (y + 1) * im->s + 1;
      ushort* dst = (ushort*)(im2->tt.data + y * im2->s);
      for (x = 0; x < w; ++x, ++src, ++dst) {
        int gx = (src[1] << 1) + src[-s + 1] + src[s + 1] - (src[-1] << 1) - src[-s - 1] - src[s - 1];
        int gy = (src[s] << 1) + src[s - 1] + src[s + 1] - (src[-s] << 1) - src[-s - 1] - src[-s + 1];
        *dst = (IABS(gx) + IABS(gy));
      }
    }
  }
  imfree(im1);
  return 0;
}
//�㷨����
int SeamCarving(img_t* im, int new_w)
{
  //���ҳ�ͼƬ��ײ���һ���У�������С�����ص���Ϊ��ʼ��
  int x, y, w = im->w, h = im->h;
  int c = im->c, i = 0;
  img_t ims[10] = {0};
  img_t* gry = ims + i++;
  img_t* energy = ims + i++;
  int* Path = NULL;
  MYREALLOC(Path, h);
  if (new_w > im->w) {
    imreplicate_border(im, im, iRECT(0, 0, new_w - im->w, 0));
    im->w = w;
    //imshow(im);cvWaitKey(-1);
  }
  while (im->w != new_w) {
    im2gry(im, gry);
    //1���㷨��һ��������ͼ������ͼ
    //����ͼһ����ͼ�����ص��ݶ�ģֵ��Ϊ�˼򻯼������ת���ɻҶ�ͼ��Ȼ��ֱ�Ӳ������¹�ʽ(ֱ����x��y�����ϵĲ��ȡ����ֵ��Ȼ�����)����ʵ��һ�������൱�ڱ�Ե����㷨һ����
    imSobel_mag(gry, energy);
    //imshowfmt(energy, PF_16bppGray);//cvWaitKey(-1);
    //2���㷨�ڶ�����Ѱ����С������
    //��С������ָ������Ҫ���Ƴ�����һ�У�������Ҫ��ͼ���һ�л����һ��Ϊ��ʼ�н��е���������ʵ�ֵ�Ϊ��ͼ�����һ�п�ʼ�����ϵ�����
    //min_x��ͼƬ��ײ���һ�е�����ֵ��С�����ص� ��˱�����ʱ��Ҫ����Ѱ��
    {
      int min_x = 0;
      int minx, Maxy;
      ushort* en = (ushort*)(energy->tt.data + (h - 1) * energy->s);
      for (x = 0; x < w; x++) {
        if (en[x] < en[min_x]) {
          min_x = x;
        }
      }
      Path[h - 1] = min_x;
      x = min_x;//��ʼ�е�������С��
      //�ӵײ�����Ѱ����С������
      for (y = h - 2; y >= 0; y--) { //��������
        ushort* en = (ushort*)(energy->tt.data + (y) * energy->s);
        minx = x; //�׸������������
        //������һ���� �����ڽ�����������ֵ��С�����ص� ��Ϊ��һ�е�������С��
        Maxy = en[x];
        if (en[x - 1] < Maxy) { //
          minx = x - 1;
        }
        if (en[x + 1] < Maxy) { //up-right
          minx = x + 1;
        }
        Path[y] = minx;
        x = minx;
      }
    }
    //3���㷨���������Ƴ��õ�����С�����ߣ���ͼƬ�Ŀ����Сһ������
    // �Ƴ���С�����ߣ�ͬʱ����λ����С�������ұߵ����ص�����һ����λ���Ӷ�ʵ��ͼ����С�����Сһ����λ��
    // �Ƴ���ʱ�� Ϊ����ͼ��������Ȼ����Ҫ���Ƴ����ߵĵط�����ƽ���������Ƴ�����ΪP(x,y),��ô�Ƴ���P(x-1,y)������ֵΪP(x-1,y)��P(x,y)������ֵ��ƽ����P(x+1,y)������ֵΪP(x-1,y)��P(x,y)������ֵ��ƽ����Ȼ����ܰ�P(x+1,y)�ƶ���P(x,y)��λ�á�
    if (im->w > new_w) {
      for (y = 0; y < h; ++y) {
        int k = BOUND(Path[y], 1, w - 2);
        //�Ƴ���ʱ�� Ϊ����ͼ��������Ȼ����Ҫ���Ƴ����ߵĵط�����ƽ���������Ƴ�����ΪP(x,y),��ô
        //�Ƴ���P(x-1,y)������ֵΪP(x-1,y)��P(x,y)������ֵ��ƽ��
        //P(x+1,y)������ֵΪP(x-1,y)��P(x,y)������ֵ��ƽ����Ȼ����ܰ�P(x+1,y)�ƶ���P(x,y)��λ��
        uchar* src = im->tt.data + y * im->s + k * im->c;
        for (i = 0; i < im->c; ++i) {
          src[-c + i] = (src[-c + i] + src[i]) >> 1;
          src[i] = (src[i] + src[c + i]) >> 1;
        }
        memmove(src + c, src + 2 * c, (w - k - 3)*c);
      }
      im->w--;
    }
    else {
      //����ͼ��ķŴ��㷨ԭ��һ�������ҵ���С�����ߣ����������ϵ������ΪP(x,y),����P(x,y)��P(x+1,y)����λ�ò����µ����أ�����ֵΪP(x,y)��P(x+1,y)��ƽ����
      for (y = 0; y < h; ++y) {
        int k = BOUND(Path[y], 1, w - 2);
        uchar* src = im->tt.data + y * im->s + k * im->c;
        memmove(src + 2 * c, src + c, (w - k - 1)*c);
        for (i = 0; i < im->c; ++i) {
          src[c + i] = (src[i] + src[c + i]) >> 1;
        }
      }
      im->w++;
    }
    //imshow(gry); imshow(im);cvWaitKey(-1);
  }
  FREE(Path);
  imfrees(ims, 10);
  return 0;
}

