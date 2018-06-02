
// http://zh.wikipedia.org/wiki/%E6%9E%81%E5%9D%90%E6%A0%87%E7%B3%BB

double RandomX(double low, double high)
{
  return ((rand() / ((double)(RAND_MAX) + 1.0)) * (high - low) + low);
}

int vcgen_rand_line(int n, DPOINT2* pt, double x1, double y1, double x2, double y2, double error)
{
  int i;
  double x;

  for (i = 0; i < n; ++i) {
    pt[i].x = x = RandomX(x1, x2);
    pt[i].y = (x - x1) * (y2 - y1) / (x2 - x1) + y1 + RandomX(-error, error);
  }

  return 0;
}

int vcgen_rand_circle(int n, DPOINT2* pt, double centrex, double centrey, double radius, double error)
{
  int i;
  double r, t;

  for (i = 0; i < n; ++i) {
    t = RandomX(0, 2 * PI);
    r = RandomX(radius - error, radius + error);
    pt[i].x = centrex + r * cos(t);
    pt[i].y = centrey + r * sin(t);
  }

  return 0;
}

int vcgen_rand_ellipse(int n, DPOINT2* pt, double centrex, double centrey, double a, double b, double error)
{
  double rotation = RandomX(0, 2 * PI);
  double sinr = sin(rotation), cosr = cos(rotation);
  int i;
  double t, x, y;

  for (i = 0; i < n; ++i) {
    t = RandomX(0, 2 * PI);
    x = RandomX(a - error, a + error) * cos(t);
    y = RandomX(b - error, b + error) * sin(t);
    pt[i].x = cosr * x - sinr * y;
    pt[i].y = sinr * x + cosr * y;
  }

  return 0;
}

//õ����  r(��) = a sin k��
//�������õ���ߣ�polar rose������ѧ�����зǳ����������ߣ�����ȥ�񻨰꣬��ֻ���ü����귽��������
int vcgen_polar_rose(int n, DPOINT2* pt, double a, int k)
{
  int i;
  double r, t;

  for (i = 0; i < n; ++i) {
    t = i * 2 * PI / n;
    r = a * sin(k * i);
    pt[i].x = r * cos(t);
    pt[i].y = r * sin(t);
  }

  return n;
}

//�����׵�����(archimedean spiral)  r(��) = a + b��
// �ı����a���ı�������״��b�������߼���룬ͨ����Ϊ�����������׵��������������ߣ�
// һ���� > 0����һ���� < 0�����������ڼ��㴦ƽ�������ӡ�������һ����ת 90��/270��õ��侵�񣬾�����һ������
int vcgen_archimedean_spiral(int n, DPOINT2* pt, double a, double b)
{
  int i;
  double r, t;

  for (i = 0; i < n; ++i) {
    t = i * 2 * PI / n;
    r = a + b * t;
    pt[i].x = r * cos(t);
    pt[i].y = r * sin(t);
  }

  return 0;
}

// ˫�����ߣ�Hyperbolic spiral���ֳƵ������ߣ�reciprocal spiral��
int vcgen_hyperbolic_spiral(int n, DPOINT2* pt, double c)
{
  int i;
  double r, t;

  for (i = 0; i < n; ++i) {
    t = i * 2 * PI / n;
    r = c / t;
    pt[i].x = r * cos(t);
    pt[i].y = r * sin(t);
  }

  return 0;
}

// �ڰ��ߣ�Բ�����ߣ�
// ���� k Ϊ��ʵ��
int vcgen_hypocycloid_spiral(int n, DPOINT2* pt, double k)
{
  int i;
  double t;

  for (i = 0; i < n; ++i) {
    t = i * 2 * PI / n;
    pt[i].x = cos(t) + cos(k * t) / k;
    pt[i].y = sin(t) - sin(k * t) / k;
  }

  return 0;
}
// �������������ߵ�һ�֣����ʽ��r^2 = ��
int vcgen_fermat_spiral(int n, DPOINT2* pt, double a, double b)
{
  int i;
  double r, t;

  for (i = 0; i < n; ++i) {
    t = i * 2 * PI / n;
    r = sqrt(t);
    pt[i].x = r * cos(t);
    pt[i].y = r * sin(t);
  }

  return 0;
}

//Բ׶����
//Բ׶���߷��̣�r = l / (1 - e cos theta)
//����l��ʾ�������ң�e��ʾ�����ʡ� ���e < 1������Ϊ��Բ�����e = 1������Ϊ�����ߣ����e > 1�����ʾ˫���ߡ�
int vcgen_conic(int n, DPOINT2* pt, double l, double e)
{
  int i;
  double r, t;

  for (i = 0; i < n; ++i) {
    t = i * 2 * PI / n;
    r = l / (1 - e * cos(t));
    pt[i].x = r * cos(t);
    pt[i].y = r * sin(t);
  }

  return 0;
}


