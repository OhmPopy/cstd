typedef struct _ClickPt { //�Ѽ������ĵ�
  POINT point; //�㱾��
  int fp; //���ڵ��ڱ��ṹ�е�λ��
  BOOL ckclient; //TRUEΪ����㣬FALSEΪ�Ѳ��
  int pv; //�������յ�Ĳ���
} ClickPt;
//������СPVֵ�����ؽṹ��¼
int SFMinPv(ClickPt* cpt, int n)
{
  int i, minpv = 100, minsn = 0;
  for (i = 0; i < n; i++) {
    if (cpt[i].ckclient) {
      if (cpt[i].pv < minpv) {
        minpv = cpt[i].pv;
        minsn = i;
      }
    }
  }
  return minsn;
}
//ȡ�÷���·��,�������ز���
//path ·������
//lastn �յ�ṹ������
int BackPath(POINT* backpoint, int n, ClickPt* cpt, POINT astarpoint)
{
  int i, cout = 0, lastn = n - 1;
  POINT* npoint;
  npoint = (POINT*)malloc(n * sizeof(POINT));
  for (i = 0; i < n; i++) {
    npoint[i] = cpt[lastn].point;
    lastn = cpt[lastn].fp;
    cout += 1;
    if (lastn == 0) {
      npoint[cout] = astarpoint;
      break;
    }
  }
  for (i = 0; i <= cout; i++) {
    backpoint[i] = npoint[cout - i];
  }
  free(npoint);
  return cout;
}
//�Ƚ������Ƿ����
#define CTwoPt(pt1, pt2) (((pt1).x == (pt2).x) && ((pt1).y == (pt2).y))
//����һ�����PVֵ
#define KclaterPv(pt) (abs(pt.x - aendpoint.x) + abs(pt.y - aendpoint.y))
//��һ�������ṹ��
#define FSPoint(pt, bb) { cpt[n].point = pt;cpt[n].ckclient=bb;cpt[n].fp = bn;cpt[n].pv = KclaterPv(pt); ++n;}
// A*�㷨���캯��˵����
// starpoint������ ������ʼ��
// endpoint������ A*Ѱ·�յ�
// backpoint A*�ҵ������·�����ص�����,����ͼ���ܵ��ﷵ��0
// map ��ͼ��Ϣ
// aendpoint ���
// astarpoint �յ�
// bn �����¼
// n �ṹ������
//����ֵΪ�����յ����貽��
int AstarGetWay(int h, int w, char* astarmap, int step, POINT astarpoint, POINT aendpoint, POINT* backpoint)
{
  int i, j, k, n, bn;
  ClickPt* cpt = (ClickPt*)malloc(h * w * sizeof(ClickPt));
  POINT bspt; //����
  POINT cpoint;
  int minpv = 0; //��Сpvֵ�ĵ��������
  int err;
  n = 0;
  bn = 0;
  bspt = astarpoint;
  FSPoint(astarpoint, FALSE);
  //����Ƿ��޵�ɼ�
  for (j = 0; j < h * w; ++j) {
    err = n;
    for (i = 0; i < 4; ++i) { //ѭ�������ϡ��¡������ĵ�
      switch (i) {
      case 0:
        //�������
        cpoint.x = bspt.x - 1;
        cpoint.y = bspt.y;
        break;
      case 1:
        //�����ҵ�
        cpoint.x = bspt.x + 1;
        cpoint.y = bspt.y;
        break;
      case 2:
        //�����ϵ�
        cpoint.x = bspt.x;
        cpoint.y = bspt.y - 1;
        break;
      case 3:
        //�����µ�
        cpoint.x = bspt.x;
        cpoint.y = bspt.y + 1;
        break;
      }
      //��������Ƿ��ǵ�ͼ��ɷ�Χ��
      k = cpoint.x >= 0 && cpoint.x < w && cpoint.y >= 0 && cpoint.y < h;
      if (k) {
        //����ͼ�Ƿ�����ͨ��
        k = !astarmap[cpoint.y * step + cpoint.x];
        if (k) {
          //���һ�����Ƿ��ڽṹ����
          int _i;
          k = 0;
          //��������Ƿ��ڽṹ����
          for (_i = 0; _i < n; _i++) {
            if (CTwoPt(cpoint, cpt[_i].point)) {
              k = 1;
              break;
            }
          }
          if (!k) {
            // ������ھ����
            FSPoint(cpoint, TRUE); //���ṹ��
            if (CTwoPt(cpoint, aendpoint)) { //�жϸõ��Ƿ����յ�
              k = BackPath(backpoint, n, cpt, astarpoint);
              free(cpt);
              return k;
            }
          }
        }
      }
    }
    //����Ƿ����޵�ɲ�
    if (err == n) {
      int e = -1;
      for (i = 0; i <= n; i++) {
        if (!cpt[i].ckclient) {
          e += 1;
        }
      }
      if (e == n) {
        backpoint[0] = astarpoint;
        return 0;
      }
    }
    minpv = SFMinPv(cpt, n);
    cpt[minpv].ckclient = FALSE; //��������Сpvֵ�ĵ��Ƴ������
    bspt = cpt[minpv].point; //������Ϊ����
    bn = minpv; //���û����¼
  }
  free(cpt);
  return 0;
}

