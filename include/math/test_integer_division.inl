//��������
//���ࣺ Algorithms 2008-12-19 23:41 1212���Ķ� ����(0) �ղ� �ٱ�
//iostream
//ֱ������������е㲻�����ż�����ģ������Ҳ������ôһ�ΰɣ�
//n���ֵ��������n1<=m�Ļ��ָ���
int q(int n, int m)
{
  if (n < 1 || m < 1) {
    return 0;
  }
  if (n == 1 || m == 1) {
    return 1;
  }
  if (n < m) {
    return q(n, n);
  }
  if (n == m) {
    return q(n, n - 1) + 1;
  }
  return q(n, m - 1) + q(n - m, m);
}
//����������
int q2(int n, int m)
{
  if (n < 1 || m < 1) {
    return 0;
  }
  if (n == 1 || m == 1) {
    return 1;
  }
  if (n < m) {
    return q2(n, n);
  }
  if (n == m && n % 2 == 1) {
    return q2(n, n - 1) + 1;
  }
  if (n == m && n % 2 == 0) {
    return q2(n, n - 1);
  }
  //if(n>m && n%2==1)
  return q2(n, m - 2) + q2(n - m, m);
  //return q2(n,m-2)+q2(n-m,m);
}
//��ͬ����������(��n���ֳɲ�ͬ���������)
int q3(int n, int m)
{
  if (n == 1 && m == 1) {
    return 1;
  }
  if (n < 0 || m <= 1) {
    return 0;
  }
  if (n < m) {
    return q3(n, n);
  }
  if (n == m) {
    return q3(n, n - 1) + 1;
  }
  return q3(n, m - 1) + q3(n - m, m - 1);
}
int test_integer_division()
{
  int i, n, m;
  for (i = 0; i < 10; ++i) {
    n = rand() % 20;
    m = rand() % 20;
    n = i + 5;
    m = 2;
    printf("%4d %4d %4d %4d %4d %4d\n", n, m, q(n, n), q(n, m), q2(n, n), q3(n, n));
  }
  return 0;
}

