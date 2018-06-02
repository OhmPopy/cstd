const double INFINITY = 1e10;
typedef struct point_ii {
  double x, y, t1, t2;
  bool isIntersection, visited;
  struct point_ii* next, * equal;
} point_ii;
int dblcmp1(double d)
{
  if (fabs(d) < 0.0000001) {
    return 0;
  }
  return (d > 0) ? 1 : -1;
}
double det(double x1, double y1, double x2, double y2)
{
  return x1 * y2 - x2 * y1;
}
double cross(point_ii* a, point_ii* b, point_ii* c) //����ba��ca�Ĳ��
{
  return det(b->x - a->x, b->y - a->y, c->x - a->x, c->y - a->y);
}
double cos_a(point_ii* a, point_ii* b, point_ii* c) //����ba��ca�ļн�����ֵ
{
  double x1 = b->x - a->x, y1 = b->y - a->y, x2 = c->x - a->x, y2 = c->y - a->y;
  return (x1 * x2 + y1 * y2) / (sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2));
}
bool isIntersected(point_ii* a, point_ii* b, point_ii* c, point_ii* d) //�ж��߶��Ƿ��ཻ
{
  int c1 = dblcmp(cross(a, c, d)), c2 = dblcmp(cross(b, c, d)), c3 = dblcmp(cross(c, a, b)), c4 = dblcmp(cross(d, a, b));
  if (((c1 ^ c2) == -2 || ((c1 & c2) == 0 && (c1 | c2) != 0)) && ((c3 ^ c4) == -2 || ((c3 & c4) == 0 && (c1 | c2) != 0))) {
    return 1;
  }
  return 0;
}
bool IsOnline(point_ii* p, point_ii* p1, point_ii* p2) //�жϵ��Ƿ����߶���
{
  return(!dblcmp(cross(p, p1, p2)) && ((p->x - p1->x) * (p->x - p2->x) <= 0) && ((p->y - p1->y) * (p->y - p2->y) <= 0));
}
bool segcross(point_ii* a, point_ii* b, point_ii* c, point_ii* d, point_ii* i)
{
  if (isIntersected(a, b, c, d)) { //���ཻ��ʹ�ò��������󽻵�����
    double dx1 = b->x - a->x, dx2 = d->x - c->x, dx3 = c->x - a->x, dy1 = b->y - a->y, dy2 = d->y - c->y, dy3 = c->y - a->y;
    double dx = dy1 * dx2 - dx1 * dy2;
    i->t1 = (dx2 * dy3 - dx3 * dy2) / dx;
    i->t2 = (dx1 * dy3 - dx3 * dy1) / dx;
    i->x = a->x + i->t1 * dx1;
    i->y = a->y + i->t1 * dy1;
    i->isIntersection = 1;
    i->visited = 0;
    i->next = i->equal = NULL;
    return 1;
  }
  return 0;
}
point_ii* add(point_ii* p, point_ii* q, point_ii* i, int type) //�����㰴˳������p���q֮��
{
  point_ii* newpoint = MALLOC(point_ii, 1), * r = p;
  *(newpoint) = *i;
  if (type == 1) {
    while (r->next != q && (r->next)->t1 <= i->t1) {
      r = r->next;
    }
  }
  else {
    while (r->next != q && (r->next)->t2 <= i->t2) {
      r = r->next;
    }
  }
  newpoint->next = r->next;
  r->next = newpoint;
  return newpoint;
}
void Intersect(point_ii* p, point_ii* q, point_ii* main) //���ü�����εı�pq�Ƿ�����������ཻ�����ཻ�������㰴˳�������������εĶ������
{
  point_ii* j = main, * k, * e1, * e2;
  point_ii i[1] = {0};
  while (j) {
    k = j->next;
    while (k->isIntersection) {
      k = k->next;
    }
    if (j->equal && IsOnline(j, p, q)) { //jΪһ��������ε�һ����㣬�ҵ�j��pq����
      j = k;
      continue;
    }
    if (IsOnline(k, p, q)) { //��k����pq����
      point_ii* temp = k->next;
      if (k->equal) { //kΪ������ε�һ���յ�
        temp = (k->equal)->next;
      }
      while (temp->isIntersection) { //Ѱ��jk�ߵ���һ����
        temp = temp->next;
      }
      *i = *k;
      if (q->x != p->x) {
        i->t1 = (k->x - p->x) / (q->x - p->x);
      }
      else {
        i->t1 = (k->y - p->y) / (q->y - p->y);
      }
      i->isIntersection = 1;
      i->next = i->equal = NULL;
      if (cos_a(k, j, p) < cos_a(k, temp, p)) { //������������˳��
        i->t2 = 0;
        e1 = add(p, q, i, 1);
        if (k->equal) {
          e2 = add(k->equal, temp, i, 2);
        }
        else {
          e2 = add(k, temp, i, 2);
        }
        e1->equal = e2;
        e2->equal = e1;
        i->t2 = 1;
        e1 = add(p, q, i, 1);
        e2 = add(j, k, i, 2);
        e1->equal = e2;
        e2->equal = e1;
      }
      else {
        i->t2 = 1;
        e1 = add(p, q, i, 1);
        e2 = add(j, k, i, 2);
        e1->equal = e2;
        e2->equal = e1;
        i->t2 = 0;
        e1 = add(p, q, i, 1);
        if (k->equal) {
          e2 = add(k->equal, temp, i, 2);
        }
        else {
          e2 = add(k, temp, i, 2);
        }
        e1->equal = e2;
        e2->equal = e1;
      }
      if (k->equal) {
        j = k->next; //��kΪ������ε�һ���յ㣬��j��Ϊ��һ�����
      }
      else {
        j = temp;
        if (j->equal) {
          j = j->next; //��jΪ�յ㣬��j��Ϊ��һ�����
        }
      }
      continue;
    }
    if (segcross(p, q, j, k, i)) { //�淶�ཻ
      e1 = add(p, q, i, 1);
      e2 = add(j, k, i, 2);
      e1->equal = e2;
      e2->equal = e1;
    }
    j = k;
    if (j->equal) {
      j = j->next; //��jΪ�յ㣬��j��Ϊ��һ�����
    }
  }
}
void getVertex(DPOINT* main, int mainlength, DPOINT* clip, int cliplength, point_ii* mainhead, point_ii* cliphead) //�����������ü�����εĶ����
{
  int i;
  point_ii* p, * r, * start;
  bool flag = 0;
  mainhead->x = main[0].x;
  mainhead->y = main[0].y;
  mainhead->isIntersection = mainhead->visited = false;
  mainhead->next = mainhead->equal = NULL;
  start = r = mainhead; //��ʼ��������εĶ����
  for (i = 1; i < mainlength; i++) {
    p = MALLOC(point_ii, 1);
    p->x = main[i].x;
    p->y = main[i].y;
    p->isIntersection = p->visited = false;
    p->next = p->equal = NULL;
    if (flag) {
      start = p;
      flag = 0;
    }
    if (p->x == start->x && p->y == start->y) {
      p->equal = start;
      start->equal = p;
      flag = 1;
    }
    r->next = p;
    r = p;
  }
  p->equal = start;
  start->equal = p;
  flag = 1;
  cliphead->x = clip[0].x; //��ʼ���ü�����εĶ����
  cliphead->y = clip[0].y;
  cliphead->isIntersection = cliphead->visited = false;
  cliphead->next = cliphead->equal = NULL;
  r = cliphead;
  for (i = 1; i < cliplength; i++) {
    p = MALLOC(point_ii, 1);
    p->x = clip[i].x;
    p->y = clip[i].y;
    p->isIntersection = p->visited = false;
    p->next = p->equal = NULL;
    r->next = p;
    r = p;
  }
  start = r = cliphead; //���ζԲü�����εĸ�������
  while (r) {
    p = r->next;
    Intersect(r, p, mainhead);
    r = p;
    if (r->x == start->x && r->y == start->y) {
      r->equal = start;
      start->equal = r;
      r = r->next;
      start = r;
    }
  }
}
bool isIntheclip(point_ii* p, DPOINT* clip, int cliplength) //ʹ�����߷��жϵ�p�Ƿ��ڲü��������
{
  int count = 0, i, start = 0;
  point_ii p1[1] = {0}, p2[1] = {0}, l[1] = {0};
  l->y = p->y;
  l->x = INFINITY;
  for (i = 0; i < cliplength - 1; i++) {
    p1->x = clip[i].x;
    p1->y = clip[i].y;
    p2->x = clip[i + 1].x;
    p2->y = clip[i + 1].y;
    if (IsOnline(p, p1, p2)) {
      return 1;
    }
    if (clip[i + 1].x == clip[start].x && clip[i + 1].y == clip[start].y) {
      i++;
      start = i + 1;
    }
    if (!dblcmp(p1->y - p2->y)) {
      continue;
    }
    if (IsOnline(p1, p, l)) {
      if (p1->y > p2->y) {
        count++;
      }
    }
    else if (IsOnline(p2, p, l)) {
      if (p2->y > p1->y) {
        count++;
      }
    }
    else if (isIntersected(p, l, p1, p2)) {
      count++;
    }
  }
  if (count % 2 == 1) {
    return 1;
  }
  return 0;
}
void display(DPOINT* pointarray, int length) //����ü���Ķ����
{
  int i;
  for (i = 0; i < length; i++) {
    printf("(%f,%f)", pointarray[i].x, pointarray[i].y);
  }
  printf("\n");
  printf("\n");
}
void print(point_ii* head) //��������б�
{
  point_ii* r = head;
  while (r) {
    printf("(%f,%f) ", r->x, r->y);
    r = r->next;
  }
  printf("\n");
  printf("\n");
}
void output(point_ii* main, point_ii* clip, DPOINT* clipArray, int cliplength, poly2d_t* po) //�ü�
{
  point_ii* r = main, * p;
  DPOINT* outArray = MALLOC(DPOINT, 3000);
  int outlength = 0;
  bool type = 0;
  while (r) {
    while (r && (!r->isIntersection || r->visited)) { //��������εĶ������Ѱ�ҵ�һ��δ���ʹ��Ľ���
      p = r;
      r = r->next;
    }
    if (!r) {
      break;
    }
    //����ǰһ����Ϊ������εĶ��㣬�����ڲü�������ڣ���ý���Ϊ���㣬���ߵ����ת���ü��������
    if (!p->isIntersection && isIntheclip(p, clipArray, cliplength) && ((r->x != p->x) || (r->y != p->y))) {
      r = r->equal;
      type = 1;
    }
    outArray[outlength].x = r->x;
    outArray[outlength++].y = r->y;
    r->visited = 1;
    r = r->next;
    while (r) {
      while (!r->isIntersection && !r->visited) { //��rΪ����εĶ��㣬��ֱ�����������������
        outArray[outlength].x = r->x;
        outArray[outlength++].y = r->y;
        r->visited = 1;
        if (r->equal) {
          r = r->equal; //��rΪ����ε�һ���յ㣬��ת����֮��ͬ����㴦
        }
        r = r->next;
      }
      if (r->visited) {
        break;
      }
      if (r->x != outArray[outlength - 1].x || r->y != outArray[outlength - 1].y) {
        outArray[outlength].x = r->x;
        outArray[outlength++].y = r->y;
      }
      r->visited = 1;
      (r->equal)->visited = 1; //ת������һ����εĶ������
      r = (r->equal)->next;
    }
    //�ü�����һ���������Σ��������
    //display(outArray, outlength);
    poly2d_addpts(po, outlength, outArray);
    outlength = 0;
    if (type) { //����ʱrΪ�ü�����ζ�����еĵ㣬����ת��Ϊ��Ӧ����������еĵ�
      r = r->equal;
      type = 0;
    }
    p = r;
    r = r->next;
  }
  r = main;
  while (r) {
    p = r;
    r = r->next;
    pfree(p);
  }
  r = clip;
  while (r) {
    p = r;
    r = r->next;
    pfree(p);
  }
  pfree(outArray);
}
void polygonClip(DPOINT* main, int mainlength, DPOINT* clip, int cliplength, poly2d_t* po) //������յ�������κͲü�����εĶ������飬�Զ���ν��вü�
{
  point_ii* mainhead = MALLOC(point_ii, 1), * cliphead = MALLOC(point_ii, 1);
  getVertex(main, mainlength, clip, cliplength, mainhead, cliphead); //���������κͲü�����εĶ����
  output(mainhead, cliphead, clip, cliplength, po); //����ü����
}
int test_Weiler_Atherton()
{
  poly2d_t po[1] = {0};
  DPOINT main[] = {{1, 3.5}, {2, 2.5}, {1, 0}, {5, 2}, {2.7, 2.7}, {3, 4}, {1, 3.5}}, clip[] = {{1, 4}, {1, 1}, {4, 1}, {4, 4}, {1, 4}, {2, 3}, {3, 3}, {3, 2}, {2, 2}, {2, 3}};
  int mainlen = countof(main), cliplen = countof(clip);
  polygonClip(main, mainlen, clip, cliplen, po);
  free_poly2d(po);
  return 0;
}

