//����Ϊ�ҵ����
#include<math.h>
struct Point_Link {
  unsigned char num; //���
  double k; //б��
  unsigned char flag; //���õ��б��Ϊ������ʱ��flagȡ1����Ϊ0
  double x; //���X����
  double y; //���Y ����
  struct Point_Link* p_pre; //ǰ��ָ��
  struct Point_Link* p_succ; //��ָ��
};
struct Point_Link* p_head, *p_temp;
struct Edge_Link {
  unsigned char n0; //�ߵ�ǰһ������
  unsigned char n1; //�ߵĺ�һ������
  double d; //����
  struct Edge_Link* e_pre; //ǰ��ָ��
  struct Edge_Link* e_succ; //��ָ��
};
struct Draw_Point {
  double x;
  double y;
  int num;
  unsigned char d_flag; //��ʾͨ���õ��б���Ƿ�Ϊ�����
  double d_k; //��ʾͨ���õ��б��;
  struct Draw_Point* next;
};
struct Draw_Point* d_Point_one;
bool m_pLMouseDown;
int vertex = 0;
//����б�ʣ������������㣩
double Caculate_X_L(struct Point_Link* point_A, struct Point_Link* point_B,
    struct Point_Link* point_C)
{
  double k_BC, k_BA;
  double k = 0, k1, k2;
  if (point_B->x == point_C->x) {
    if (point_B->y == point_A->y) {
      return (k = 1.0);
    }
    else {
      k_BC = 0.0;
      k_BA = (point_A->x - point_B->x) / (point_A->y - point_B->y);
      if (k_BA > 0.0) {
        k = (double)((sqrt(1 + k_BA * k_BA) - 1) / k_BA);
      }
      else {
        k = (double)((- sqrt(1 + k_BA * k_BA) - 1) / k_BA);
      }
      return (1 / k);
    }
  }
  if (point_A->x == point_B->x) {
    if (point_C->y == point_B->y) {
      return (k = -1.0);
    }
    else {
      k_BA = 0.0; //��ʱk_BA��k_BC��б��Ϊ��y���б��
      k_BC = (point_C->x - point_B->x) / (point_C->y - point_B->y);
      if (k_BC > 0) {
        k = (double)((-1 - sqrt(1 + k_BC * k_BC)) / k_BC);
      }
      else {
        k = (double)((sqrt(1 + k_BC * k_BC) - 1) / k_BC);
      }
      return (1 / k); //���Ҫȡ������
    }
  }
  if ((point_A->x != point_B->x) && (point_B->x != point_C->x)) {
    k_BA = (point_A->y - point_B->y) / (point_A->x - point_B->x);
    k_BC = (point_C->y - point_B->y) / (point_C->x - point_B->x);
    if (k_BA + k_BC == 0) { //��ʱ����б�ʿ���Ϊ0��Ҳ����Ϊ�����
      if (((point_B->x > point_A->x) && (point_B->x > point_C->x))
          || ((point_B->x < point_A->x) && (point_B->x < point_C->x))) {
        k = 0.0;
        return k;
      }
      else {
        point_B->flag = 1; //������ʱ���ܷ���ʲô�أ�
        //return ('s');
      }
    }
    else {
      k1 = (double)((k_BA * k_BC - 1 + sqrt(1 + k_BA * k_BA + k_BC * k_BC + k_BA * k_BA * k_BC * k_BC))
          / (k_BA + k_BC));
      k2 = (double)((k_BA * k_BC - 1 - sqrt(1 + k_BA * k_BA + k_BC * k_BC + k_BA * k_BA * k_BC * k_BC))
          / (k_BA + k_BC));
      if (k_BA >= 0 && k_BC >= 0) {
        if ((point_B->x < point_C->x && point_B->x < point_A->x)
            || (point_B->x > point_C->x && point_B->x > point_A->x)) {
          k = (k1 > k2) ? k1 : k2;
          return k;
        }
        else {
          k = (k1 > k2) ? k2 : k1;
          return k;
        }
      }
      if (k_BA <= 0 && k_BC <= 0) {
        if ((point_B->x > point_C->x && point_B->x > point_A->x)
            || (point_B->x < point_C->x && point_B->x < point_A->x)) {
          k = (k1 > k2) ? k2 : k1;
          return k;
        }
        else {
          k = (k1 > k2) ? k1 : k2;
          return k;
        }
      }
      if (k_BA >= 0 && k_BC <= 0) {
        if ((k1 > k_BA && k1 < k_BC) || (k1 < k_BA && k1 > k_BC)) {
          k = k2;
          return k;
        }
        else {
          k = k1;
          return k;
        }
      }
      if (k_BA <= 0 && k_BC >= 0) {
        if ((k1 > k_BA && k1 < k_BC) || (k1 < k_BA && k1 > k_BC)) {
          k = k1;
          return k;
        }
        else {
          k = k2;
          return k;
        }
      }
    }
  }
  return k;
}
//�������
double Caculate_D(struct Point_Link* point_D, struct Point_Link* point_E)
{
  double d = 0;
  double x, y, x1, y1;
  double k1, k2;
  if (point_D->flag == 1) { //б��Ϊ�����
    x = point_D->x;
    y = point_E->y + point_E->k * (x - point_E->x);
    k1 = (point_E->y - point_D->y) / (point_E->x - point_D->x);
    k2 = -1 / k1;
    x1 = (point_D->y - y + k2 * x - k1 * point_D->x) / (k2 - k1);
    y1 = (x - point_D->x + k1 * y - k2 * point_D->y) / (k1 - k2);
    d = (double)sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
    return d;
  }
  if (point_E->flag == 1) {
    x = point_E->x;
    y = point_D->y + point_D->k * (x - point_D->x);
    k1 = (point_E->y - point_D->y) / (point_E->x - point_D->x);
    k2 = -1 / k1;
    x1 = (point_D->y - y + k2 * x - k1 * point_D->x) / (k2 - k1);
    y1 = (x - point_D->x + k1 * y - k2 * point_D->y) / (k1 - k2);
    d = (double)sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
    return d;
  }
  if (point_D->flag != 1 && point_E->flag != 1) {
    x = (point_E->y - point_D->y + point_D->k * point_D->x
        - point_E->k * point_E->x) / (point_D->k - point_E->k);
    y = (point_E->k * point_D->k * (point_E->x - point_D->x)
        + point_D->y * point_E->k - point_E->y * point_D->k)
        / (point_E->k - point_D->k);
    if (point_D->x == point_E->x) {
      y1 = y;
      x1 = point_D->x;
      d = (double)sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
      return d;
    }
    if (point_D->y == point_E->y) {
      x1 = x;
      y1 = point_D->y;
      d = (double)sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
      return d;
    }
    if ((point_D->x != point_E->x) && (point_D->y != point_E->y)) {
      k1 = (point_E->y - point_D->y) / (point_E->x - point_D->x);
      k2 = -1 / k1;
      x1 = (point_D->y - y + k2 * x - k1 * point_D->x) / (k2 - k1);
      y1 = (x - point_D->x + k1 * y - k2 * point_D->y) / (k1 - k2);
      d = (double)sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
      return d;
    }
  }
  return d;
}
//����б�ʣ������ĸ����㣩
int Caculate_Fourpoints_X_L(struct Point_Link* point_F, struct Point_Link* point_G,
    struct Point_Link* point_H, struct Point_Link* point_I)
{
  double u, v, k;
  double k1, k2, k3, k4;
  if ((point_F->x == point_G->x) && (point_H->x == point_I->x)) {
    d_Point_one->d_flag = 1;
  }
  if ((point_F->x == point_G->x) && (point_H->x != point_I->x)) {
    u = point_F->x;
    k = (point_I->y - point_H->y) / (point_I->x - point_H->x);
    v = point_H->y + k * (u - point_H->x);
    d_Point_one->d_k = (d_Point_one->y - v) / (d_Point_one->x - u);
  }
  if ((point_F->x != point_G->x) && (point_H->x == point_I->x)) {
    v = point_H->x;
    k = (point_F->y - point_G->y) / (point_F->x - point_G->x);
    v = point_F->y + k * (u - point_F->x);
    d_Point_one->d_k = (d_Point_one->y - v) / (d_Point_one->x - u);
  }
  if ((point_F->x != point_G->x) && (point_H->x != point_I->x)) {
    k1 = (point_F->y - point_G->y) / (point_F->x - point_G->x);
    k2 = (point_I->y - point_H->y) / (point_I->x - point_H->x);
    if ((k1 + k2 == 0) && (k1 != 0)) { //�෴����Ϊ0
      if ((point_F->x > point_G->x && point_I->x < point_H->x)
          || (point_F->x < point_G->x && point_I->x > point_H->x)) {
        d_Point_one->d_flag = 1;
      }
      else {
        d_Point_one->d_k = 0.0;
      }
    }
    else {
      if (k1 == k2) { //���
        d_Point_one->d_k = k1;
      }
      else {
        k3 = (double)((k1 * k2 - 1 + sqrt(1 + k1 * k1 + k2 * k2 + k1 * k1 * k2 * k2))
            / (k1 + k2));
        k4 = (double)((k1 * k2 - 1 - sqrt(1 + k1 * k1 + k2 * k2 + k1 * k1 * k2 * k2))
            / (k1 + k2));
        if (k1 >= 0 && k2 >= 0) { //
          if ((point_F->x > point_G->x && point_I->x > point_H->x)
              || (point_F->x < point_G->x && point_I->x < point_H->x)) {
            d_Point_one->d_k = (k3 > k4) ? k3 : k4;
          }
          else {
            d_Point_one->d_k = (k3 > k4) ? k4 : k3;
          }
        }
        if (k1 <= 0 && k2 <= 0) { //
          if ((point_F->x > point_G->x && point_I->x < point_H->x)
              || (point_F->x < point_G->x && point_I->x > point_H->x)) {
            d_Point_one->d_k = (k3 > k4) ? k3 : k4;
          }
          else {
            d_Point_one->d_k = (k3 > k4) ? k4 : k3;
          }
        }
        if (k1 >= 0 && k2 <= 0) {
          if ((point_F->x > point_G->x && point_I->x < point_H->x)
              || (point_F->x < point_G->x && point_I->x > point_H->x)) {
            if ((k3 > k1 && k3 < k2) || (k3 < k1 && k3 > k2)) { //ȡ����!
              d_Point_one->d_k = k4;
            }
            else {
              d_Point_one->d_k = k3;
            }
          }
          else { //ȡ�м�
            if ((k3 > k1 && k3 < k2) || (k3 < k1 && k3 > k2)) {
              d_Point_one->d_k = k3;
            }
            else {
              d_Point_one->d_k = k4;
            }
          }
        }
        if (k1 <= 0 && k2 >= 0) {
          if ((point_F->x > point_G->x && point_I->x < point_H->x)
              || (point_F->x < point_G->x && point_I->x > point_H->x)) {
            if ((k3 > k1 && k3 < k2) || (k3 < k1 && k3 > k2)) { //ȡ����!
              d_Point_one->d_k = k4;
            }
            else {
              d_Point_one->d_k = k3;
            }
          }
          else {
            if ((k3 > k1 && k3 < k2) || (k3 < k1 && k3 > k2)) { //ȡ�м�
              d_Point_one->d_k = k3;
            }
            else {
              d_Point_one->d_k = k4;
            }
          }
        }
      }
    }
  }
  return 0;
}
//�����һ����
int Caculate_First_Point(struct Draw_Point* d_Point_temp, struct Point_Link* point_P,
    struct Point_Link* point_Q)
{
  if (point_P->flag != 1 && point_Q->flag != 1) {
    d_Point_temp->x = (point_Q->y - point_P->y + point_P->k * point_P->x
        - point_Q->k * point_Q->x)
        / (point_P->k - point_Q->k);
    d_Point_temp->y = (point_P->k * point_Q->k * (point_Q->x - point_P->x)
        + point_Q->k * point_P->y - point_P->k * point_Q->y)
        / (point_Q->k - point_P->k);
  }
  if (point_P->flag == 1) { //����õ��б��Ϊ�����
    d_Point_temp->x = point_P->x;
    d_Point_temp->y = point_Q->k * (d_Point_temp->x - point_Q->x) + point_Q->y;
  }
  if (point_Q->flag == 1) {
    d_Point_temp->x = point_Q->x;
    d_Point_temp->y = point_P->k * (d_Point_temp->x - point_P->x) + point_P->y;
  }
  return 0;
}
// �Ƚ�Զ��
void Compare_Distance(struct Draw_Point* d_Point_tail, struct Point_Link* point_J,
    struct Point_Link* point_K)
{
  struct Draw_Point* d_Point_temp;
  double x1, y1, x2, y2; //�����ཻ��������
  double d1, d2;
  d_Point_temp = (struct Draw_Point*)pmalloc(sizeof(struct Draw_Point));
  d_Point_one->next = d_Point_temp;
  d_Point_temp->d_flag = 0;
  d_Point_temp->next = NULL;
  if (d_Point_tail->d_flag != 1 && point_J->flag != 1) {
    x1 = (d_Point_tail->y - point_J->y + point_J->k * point_J->x
        - d_Point_tail->d_k * d_Point_tail->x) / (point_J->k - d_Point_tail->d_k);
    y1 = (point_J->k * d_Point_tail->d_k * (d_Point_tail->x - point_J->x)
        + d_Point_tail->d_k * point_J->y - point_J->k * d_Point_tail->y)
        / (d_Point_tail->d_k - point_J->k);
  }
  if (d_Point_tail->d_flag == 1 && point_J->flag != 1) {
    x1 = d_Point_tail->x;
    y1 = point_J->k * (x1 - point_J->x) + point_J->y;
  }
  if (d_Point_tail->d_flag != 1 && point_J->flag == 1) {
    x1 = point_J->x;
    y1 = d_Point_tail->d_k * (x1 - d_Point_tail->x) + d_Point_tail->y;
  }
  if (d_Point_tail->d_flag != 1 && point_K->flag != 1) {
    x2 = (d_Point_tail->y - point_K->y + point_K->k * point_K->x
        - d_Point_tail->d_k * d_Point_tail->x) / (point_K->k - d_Point_tail->d_k);
    y2 = (point_K->k * d_Point_tail->d_k * (d_Point_tail->x - point_K->x)
        + d_Point_tail->d_k * point_K->y - point_K->k * d_Point_tail->y)
        / (d_Point_tail->d_k - point_K->k);
  }
  if (d_Point_tail->d_flag == 1 && point_K->flag != 1) {
    x2 = d_Point_tail->x;
    y2 = point_K->k * (x2 - point_K->x) + point_K->y;
  }
  if (d_Point_tail->d_flag != 1 && point_K->flag == 1) {
    x2 = point_K->x;
    y2 = d_Point_tail->d_k * (x2 - d_Point_tail->x) + d_Point_tail->y;
  }
  //�ֱ����point_J �� point_K ��d_Point_temp �ľ��룬ȡ��С�ߣ�����
  d1 = (double)sqrt((d_Point_tail->x - x1) * (d_Point_tail->x - x1) +
      (d_Point_tail->y - y1) * (d_Point_tail->y - y1));
  d2 = (double)sqrt((d_Point_tail->x - x2) * (d_Point_tail->x - x2) +
      (d_Point_tail->y - y2) * (d_Point_tail->y - y2));
  if (d1 <= d2) {
    d_Point_one->num = point_J->num;
    d_Point_temp->x = x1;
    d_Point_temp->y = y1;
  }
  else {
    d_Point_one->num = point_K->num;
    d_Point_temp->x = x2;
    d_Point_temp->y = y2;
  }
}
//�жϷ��ٱ��Ƿ��ཻ
bool Cross_Or_Not(struct Point_Link* point_L)
{
  struct Point_Link* first_Pointer, *second_Pointer, *third_Pointer, *forth_Pointer;
  first_Pointer = point_L;
  second_Pointer = first_Pointer->p_succ;
  third_Pointer = second_Pointer->p_succ;
  forth_Pointer = third_Pointer->p_succ;
  while (second_Pointer != point_L->p_pre) {
    while (forth_Pointer != first_Pointer) {
      double Delta = (second_Pointer->x - first_Pointer->x) * (third_Pointer->y - forth_Pointer->y)
          + (second_Pointer->y - first_Pointer->y) * (forth_Pointer->x - third_Pointer->x);
      if (Delta == 0) {
      }
      else {
        double s = ((third_Pointer->x - first_Pointer->x) * (third_Pointer->y - forth_Pointer->y)
            + (third_Pointer->y - first_Pointer->y) * (forth_Pointer->x - third_Pointer->x))
            / Delta;
        double t = ((second_Pointer->x - first_Pointer->x) * (third_Pointer->y - first_Pointer->y)
            - (second_Pointer->y - first_Pointer->y) * (third_Pointer->x - first_Pointer->x))
            / Delta;
        if ((s >= 0 && s <= 1) && (t >= 0 && t <= 1)) {
          return 1; //�н�����������
        }
        else {
        }
      }
      third_Pointer = third_Pointer->p_succ;
      forth_Pointer = third_Pointer->p_succ;
    }
    first_Pointer = first_Pointer->p_succ;
    second_Pointer = first_Pointer->p_succ;
    third_Pointer = second_Pointer->p_succ;
    forth_Pointer = third_Pointer->p_succ;
  }
  return 0;
}
// ͹�����ж�
char Tu_Ao_Caculate(struct Point_Link* point_L, struct Point_Link* point_M,
    struct Point_Link* point_N)
{
  double s;
  s = -(point_L->x * (point_M->y - point_N->y)
      - point_M->x * (point_L->y - point_N->y)
      + point_N->x * (point_L->y - point_M->y));
  if (s > 0) {
    return('t');
  }
  else {
    return('a');
  }
}
//��ʱ������
BOOL Clockwise(struct Point_Link* point_M)
{
  struct Point_Link* first_Pointer, *second_Pointer, *third_Pointer;
  first_Pointer = point_M;
  second_Pointer = first_Pointer->p_succ;
  third_Pointer = second_Pointer->p_succ;
  if (second_Pointer->x == first_Pointer->x) {
    if ((second_Pointer->y > first_Pointer->y && third_Pointer->x < first_Pointer->x)
        || (second_Pointer->y < first_Pointer->y && third_Pointer->x > first_Pointer->x)) { //˳ʱ��
      return 1;
    }
  }
  else {
    if (second_Pointer->x > first_Pointer->x &&
        (second_Pointer->y - first_Pointer->y) * (third_Pointer->x - first_Pointer->x)
        / (second_Pointer->x - first_Pointer->x) + first_Pointer->y - third_Pointer->y < 0) {
      return 1;
    }
    else {
      if (second_Pointer->x < first_Pointer->x &&
          (second_Pointer->y - first_Pointer->y) * (third_Pointer->x - first_Pointer->x)
          / (second_Pointer->x - first_Pointer->x) + first_Pointer->y - third_Pointer->y > 0) {
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  return 0;
}
int OnExecute(int n, POINT* pt)
{
  int i;
  for (i = 0; i < n; ++i) {
    POINT point = pt[i];
    struct Point_Link* p_tail;
    vertex++;
    p_tail = (struct Point_Link*)pmalloc(sizeof(struct Point_Link));
    p_tail->x = point.x;
    p_tail->y = point.y;
    p_tail->num = vertex - 1;
    p_tail->flag = 0;
    p_temp->p_succ = p_tail;
    p_tail->p_pre = p_temp;
    p_temp = p_tail;
    p_tail = NULL;
  }
  // TODO: Add your command handler code here
  p_temp->p_succ = p_head;
  p_head->p_pre = p_temp;
  //�б���ٱ��Ƿ��ཻ
  if (Cross_Or_Not(p_head)) {
    printf("���ٱ��н���!");
    return 0;
    //��������ִ��
  }
  //������ʱ������
  if (Clockwise(p_head)) {
    printf("����ʱ�����룡");
    return 0;
  }
  //�б��Ƿ���͹��
  p_temp = p_head;
  while (p_temp != p_head->p_pre) {
    if (Tu_Ao_Caculate(p_temp->p_pre, p_temp, p_temp->p_succ) == 'a') {
      printf("�˶�����а��㣡");
      return 0;
      //��������ִ��
    }
    else {
      p_temp = p_temp->p_succ;
    }
  }
  if (Tu_Ao_Caculate(p_temp->p_pre, p_temp, p_temp->p_succ) == 'a') {
    printf("�˶�����а��㣡");
    return 0;
    //��������ִ��
  }
  //����߱�ͷָ��
  int a = 1;
  struct Point_Link* pointer;
  pointer = p_head;
  p_temp = p_head->p_succ;
  //�߱�Ĵ���
  struct Edge_Link* e_head, *e_tail, *e_temp;
  e_temp = (struct Edge_Link*)pmalloc(sizeof(struct Edge_Link));
  e_temp->n0 = 0;
  e_temp->n1 = 1;
  e_head = e_temp;
  e_tail = e_temp;
  e_temp = NULL;
  while (p_temp != p_head->p_pre) {
    pointer = p_temp;
    p_temp = p_temp->p_succ;
    e_temp = (struct Edge_Link*)pmalloc(sizeof(struct Edge_Link));
    e_temp->n0 = a;
    e_temp->n1 = a + 1;
    e_tail->e_succ = e_temp;
    e_temp->e_pre = e_tail;
    e_tail = e_temp;
    e_temp = NULL;
    a++;
  }
  e_temp = (struct Edge_Link*)pmalloc(sizeof(struct Edge_Link));
  e_temp->n0 = a;
  e_temp->n1 = 0;
  e_tail->e_succ = e_temp;
  e_temp->e_pre = e_tail;
  e_tail = e_temp;
  e_temp = NULL;
  e_tail->e_succ = e_head;
  e_head->e_pre = e_tail;
  //����ÿ����Ľ�ƽ���ߵ�б��
  p_temp = p_head;
  while (p_temp != p_head->p_pre) {
    p_temp->k = Caculate_X_L(p_temp->p_pre, p_temp, p_temp->p_succ);
    p_temp = p_temp->p_succ;
  }
  p_temp->k = Caculate_X_L(p_temp->p_pre, p_temp, p_temp->p_succ);
  p_temp = NULL;
  //�������ٶ����ƽ���ߵĽ��㼰���㵽������ľ���
  e_temp = e_head;
  p_temp = p_head;
  pointer = p_temp->p_succ;
  while (e_temp != e_head->e_pre) {
    e_temp->d = Caculate_D(p_temp, pointer);
    e_temp = e_temp->e_succ;
    p_temp = pointer;
    pointer = p_temp->p_succ;
  }
  e_temp->d = Caculate_D(p_temp, pointer);
  p_temp = NULL;
  pointer = NULL;
  e_temp = e_head;
  //�Ƚ�d��ֵ�������С��d��Ӧ�ı�
  double b = e_temp->d;
  int c = e_temp->n0;
  e_temp = e_head->e_succ;
  while (e_temp != e_head) {
    if (e_temp->d < b) {
      b = e_temp->d;
      c = e_temp->n0;
    }
    e_temp = e_temp->e_succ;
  }
  //�ƶ���ͷָ��p_head �� e_head ��c��
  while (p_head->num != c) {
    p_head = p_head->p_succ;
  }
  while (e_head->n0 != c) {
    e_head = e_head->e_succ;
  }
  //�������
  struct Edge_Link* e_First_Pointer, *e_Second_Pointer;
  e_First_Pointer = e_head;
  p_temp = p_head;
  pointer = p_head->p_succ;
  //�����һ����ҳ��� ������d_Point_one��
  d_Point_one = (struct Draw_Point*)pmalloc(sizeof(struct Draw_Point));
  d_Point_one->num = 0;
  d_Point_one->d_flag = 0;
  d_Point_one->next = NULL;
  Caculate_First_Point(d_Point_one, p_temp, pointer);
  //����������ָ�룬�����ƶ�
  e_First_Pointer = e_head->e_pre;
  e_Second_Pointer = e_head->e_succ;
  struct Point_Link* first_pointer, *second_pointer, *third_pointer, *forth_pointer;
  first_pointer = p_head;
  second_pointer = p_head->p_succ;
  third_pointer = first_pointer->p_pre;
  forth_pointer = second_pointer->p_succ;
  struct Draw_Point* d_point_temp; //�ڻ��ߵ�ʱ���õ��������ṹDraw_Point ָ�룺d_Point_one �� d_point_temp
  while (third_pointer->num != forth_pointer->num) {
    //double i,j;
    Caculate_Fourpoints_X_L(third_pointer, first_pointer,
        second_pointer, forth_pointer);
    Compare_Distance(d_Point_one, third_pointer, forth_pointer);
    if (third_pointer->num == d_Point_one->num) {
      first_pointer = first_pointer->p_pre;
      third_pointer = first_pointer->p_pre;
    }
    else {
      second_pointer = second_pointer->p_succ;
      forth_pointer = second_pointer->p_succ;
    }
    d_point_temp = d_Point_one->next;
    d_Point_one->next = NULL;
    d_Point_one = d_point_temp;
  }
  return 0;
}

