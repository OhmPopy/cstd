#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <assert.h>
#include <string.h>
//typedef CvRect IRECT;
typedef unsigned int myCvSubdiv2DEdge;
// ԭʼ�Ͷ�ż���ֵ�
//typedef CvPoint2D32f CvPoint2D32f;
/* �ķ���Ե�ṹ����ƽ�滮�ֵĻ�Ԫ�����а����ĸ���Ե */
/* quad-edge �е�һ����Ե������λ��ʾ�ñ�Ե�������ţ�������λ��ʾ��Եָ�롣 */
typedef struct myCvQuadEdge2D {
  CvPoint2D32f* pt[ 4 ];
  myCvSubdiv2DEdge next[ 4 ];
}
myCvQuadEdge2D;
// ƽ�滮��
// ƽ�滮���ǽ�һ��ƽ��ָ�Ϊһ�黥���ص����ܹ���������ƽ�������P(facets)��
// ����ṹ�����˽����� 2D �㼯�ϵĻ��ֽṹ�����е㼯�������Ӳ��ҹ���ƽ��ͼ�Σ�
// ��ͼ��ͨ�����һЩ���������ⲿ���ֵ�(��Ϊ͹�ε�)�ı�Ե����һ��ƽ���ñ߰�����
// ��Ե���ֳɺܶ�С����(facets)��
// ����ÿһ�����ֲ���������һ����ż������֮��Ӧ����ż����˼��С����͵�
// (���ֵĶ���)�任��ɫ�����ڶ�ż�����У�С���򱻵���һ������(���³�֮Ϊ�����)��
// ��ԭʼ�Ļ��ֶ��㱻����С������������ʾ��ͼ���У�ԭʼ�Ļ�����ʵ������ʾ��
// ����ż�����õ�������ʾ��
// Delaunay's �㷨��ƽ��ָ��С�����������򡣷ָ��ʵ��ͨ����һ���ٶ���
// ������(��������ȷ���������еķָ��)��ʼ���ϵ�������ɡ�����������£�
// ��ż���־��������2d�㼯�� Voronoiͼ�����ֻ��ֿ������ڶ�һ��ƽ���3d�ֶα任��
// ��̬�任��ƽ���Ŀ��ٶ�λ�Լ������ض���ͼ�ṹ (���� NNG,RNG�ȵ�)��
typedef struct myCvSubdiv2D {
  int is_geometry_valid;
  myCvSubdiv2DEdge recent_edge;
  CvPoint2D32f topleft;
  CvPoint2D32f bottomright;
  myCvQuadEdge2D* edges;
  CvPoint2D32f* points;
  int edge_num;
  int point_num;
  int edge_maxnum;
  int point_maxnum;
}
myCvSubdiv2D;
/* get the nextedgewith the same origin point (counterwise) */
#undef CC_SUBDIV2D_NEXT_EDGE
#define CC_SUBDIV2D_NEXT_EDGE(edge) (((myCvQuadEdge2D*)((edge) & ~3))->next[(edge) & 3])
// CC_SUBDIV2D_ROTATE_EDGE ����ͬһ���ķ���Ե�ṹ�е���һ����Ե
// edge ���ֵı�Ե (�������ķ���Ե�ṹ)
// type ȷ��������������ı�Ե����ͬһ�ķ���Ե�ṹ�е�������Ե�������漸��֮һ:
// 0 - �����Ե (��ͼ�е�e�����e�������Ե)
// 1 - ��ת��Ե (eRot)
// 2 - ���Ե ( e�ķ����Ե)
// 3 - ��ת��Ե�ķ����Ե(eRot�ķ����Ե�� ͼ����ɫ)
// CC_SUBDIV2D_ROTATE_EDGE ��������ı�Ե�����ķ���Ե�ṹ�е�һ����Ե
#define CC_SUBDIV2D_ROTATE_EDGE(edge, rotate) ((((myCvSubdiv2DEdge)(edge)) & ~3) + ((((myCvSubdiv2DEdge)(edge)) + rotate) & 3))
#define CC_SUBDIV2D_SYM_EDGE(edge) (((myCvSubdiv2DEdge)(edge))^2)
// CC_SUBDIV2D_EDGE_ORG ���ر�Ե��ԭ��
// CC_SUBDIV2D_EDGE_DST ���ر�Ե���յ�
// ����ñ�Ե�ǴӶ�ż���ֵõ�����������껹û�м��������
// ���ܷ��ؿ�ָ�롣�������ú�����cvCalcSubdivVoronoi2D���㡣
#define CC_SUBDIV2D_EDGE_ORG(edge) ((CvPoint2D32f*)(((myCvQuadEdge2D*)(((myCvSubdiv2DEdge)(edge)) & ~3))->pt)[((myCvSubdiv2DEdge)(edge)) & 3])
#define CC_SUBDIV2D_EDGE_DST(edge) ((CvPoint2D32f*)(((myCvQuadEdge2D*)(((myCvSubdiv2DEdge)(edge)) & ~3))->pt)[(((myCvSubdiv2DEdge)(edge)) + 2) & 3])
// �����������Ե��صı�Ե
// edge ���ֵı�Ե (�������ķ���Ե�ṹ)
// type ȷ����������������ر�Ե�������漸��֮һ��
// CC_NEXT_AROUND_ORG - ��Եԭ�����һ�� (eOnext on the picture above if e is the input edge)
// CC_NEXT_AROUND_DST - ��Ե�������һ�� (eDnext)
// CC_PREV_AROUND_ORG - ��Եԭ���ǰһ�� (reversed eRnext)
// CC_PREV_AROUND_DST - ��Ե�յ��ǰһ�� (reversed eLnext)
// CC_NEXT_AROUND_LEFT - ���������һ�� (eLnext)
// CC_NEXT_AROUND_RIGHT - ���������һ�� (eRnext)
// CC_PREV_AROUND_LEFT - �������ǰһ�� (reversed eOnext)
// CC_PREV_AROUND_RIGHT - �������ǰһ�� (reversed eDnext)
myCvSubdiv2DEdge cvSubdiv2DGetEdge(myCvSubdiv2DEdge edge, CvNextEdgeType type);
myCvSubdiv2D* CreateSubdivDelaunay2D(int vtx_size, IRECT rect);
void cvClearSubdivVoronoi2D(myCvSubdiv2D* subdiv);
CvPoint2D32f* cvSubdivDelaunay2DInsert(myCvSubdiv2D* subdiv, CvPoint2D32f pt);
void cvCalcSubdivVoronoi2D(myCvSubdiv2D* subdiv);
typedef struct tagEDGEREF {
  CvPoint2D32f* s;
  CvPoint2D32f* e;
}
EDGEREF, *PEDGEREF;
#define cvSubdiv2DNextEdge( edge ) cvSubdiv2DGetEdge( edge, CC_NEXT_AROUND_ORG )
#define cvTriangleArea(a, b, c) (((b).x - (a).x) * ((c).y - (a).y) - ((b).y - (a).y) * ((c).x - (a).x))
myCvSubdiv2DEdge cvSubdiv2DGetEdge(myCvSubdiv2DEdge edge, CvNextEdgeType type)
{
  edge = ((myCvQuadEdge2D*)(edge & ~3)) ->next[(edge + (int) type) & 3 ];
  return (edge & ~3) + ((edge + ((int) type >> 4)) & 3);
}
myCvSubdiv2D* cvCreateSubdiv2D(int vtx_size, int quadedge_size)
{
  myCvSubdiv2D* subdiv = 0;
  subdiv = (myCvSubdiv2D*) pmalloc(sizeof(myCvSubdiv2D));
  memset(subdiv, 0, sizeof(myCvSubdiv2D));
  vtx_size = MAX(1, vtx_size);
  quadedge_size = MAX(1, quadedge_size);
  subdiv->points = (FPOINT*) pmalloc(vtx_size * sizeof(FPOINT));
  subdiv->edges = (myCvQuadEdge2D*) pmalloc(quadedge_size * sizeof(myCvQuadEdge2D));
  subdiv->point_maxnum = vtx_size;
  subdiv->edge_maxnum = quadedge_size;
  return subdiv;
}
void cvClearSubdivVoronoi2D(myCvSubdiv2D* subdiv)
{
  FREE(subdiv->points);
  FREE(subdiv->edges);
  FREE(subdiv);
}
myCvSubdiv2DEdge cvSubdiv2DMakeEdge(myCvSubdiv2D* subdiv)
{
  myCvQuadEdge2D* edge = subdiv->edges;
  myCvSubdiv2DEdge edgehandle = 0;
#if 0
  if (subdiv->edge_num >= subdiv->edge_maxnum) {
    subdiv->edge_maxnum += 1;
    edge = (myCvQuadEdge2D*) pmalloc((subdiv->edge_maxnum) * sizeof(myCvQuadEdge2D));
    memcpy(edge, subdiv->edges, (subdiv->edge_num) * sizeof(myCvQuadEdge2D));
    pfree(subdiv->edges);
    subdiv->edges = edge;
  }
#endif
  edge += subdiv->edge_num;
  memset(edge->pt, 0, sizeof(edge->pt));
  edgehandle = (myCvSubdiv2DEdge) edge;
  edge->next[ 0 ] = edgehandle;
  edge->next[ 1 ] = edgehandle + 3;
  edge->next[ 2 ] = edgehandle + 2;
  edge->next[ 3 ] = edgehandle + 1;
  subdiv->edge_num++;
  return edgehandle;
}
#define CC_SUBDIV2D_VIRTUAL_POINT_FLAG (1 << 30)
FPOINT* cvSubdiv2DAddPoint(myCvSubdiv2D* subdiv, FPOINT pt, int is_virtual)
{
  FPOINT* subdiv_point = subdiv->points + subdiv->point_num;
  subdiv_point[ 0 ] = pt;
  //subdiv_point->flags |= is_virtual ? CC_SUBDIV2D_VIRTUAL_POINT_FLAG : 0;
  subdiv->point_num++;
  return subdiv_point;
}
void cvSubdiv2DSplice(myCvSubdiv2DEdge edgeA, myCvSubdiv2DEdge edgeB)
{
  myCvSubdiv2DEdge* a_next = &CC_SUBDIV2D_NEXT_EDGE(edgeA);
  myCvSubdiv2DEdge* b_next = &CC_SUBDIV2D_NEXT_EDGE(edgeB);
  myCvSubdiv2DEdge a_rot = CC_SUBDIV2D_ROTATE_EDGE(*a_next, 1);
  myCvSubdiv2DEdge b_rot = CC_SUBDIV2D_ROTATE_EDGE(*b_next, 1);
  myCvSubdiv2DEdge* a_rot_next = &CC_SUBDIV2D_NEXT_EDGE(a_rot);
  myCvSubdiv2DEdge* b_rot_next = &CC_SUBDIV2D_NEXT_EDGE(b_rot);
  myCvSubdiv2DEdge t;
  CC_SWAP(*a_next, *b_next, t);
  CC_SWAP(*a_rot_next, *b_rot_next, t);
}
void cvSubdiv2DSetEdgePoints(myCvSubdiv2DEdge edge,
    FPOINT* org_pt, FPOINT* dst_pt)
{
  myCvQuadEdge2D* quadedge = (myCvQuadEdge2D*)(edge & ~3);
  if (!quadedge) {
    CC_ERROR(CC_StsNullPtr, "");
  }
  quadedge->pt[ edge & 3 ] = org_pt;
  quadedge->pt[(edge + 2) & 3 ] = dst_pt;
}
void cvSubdiv2DDeleteEdge(myCvSubdiv2D* subdiv, myCvSubdiv2DEdge edge)
{
  myCvQuadEdge2D* quadedge = (myCvQuadEdge2D*)(edge & ~3);
  if (!subdiv || !quadedge) {
    CC_ERROR(CC_StsNullPtr, "");
  }
  cvSubdiv2DSplice(edge, cvSubdiv2DGetEdge(edge, CC_PREV_AROUND_ORG));
  {
    myCvSubdiv2DEdge sym_edge = CC_SUBDIV2D_SYM_EDGE(edge);
    cvSubdiv2DSplice(sym_edge, cvSubdiv2DGetEdge(sym_edge, CC_PREV_AROUND_ORG));
  }
  {
    subdiv->edge_num--;
    *quadedge = subdiv->edges[ subdiv->edge_num ];
  }
}
myCvSubdiv2DEdge cvSubdiv2DConnectEdges(myCvSubdiv2D* subdiv, myCvSubdiv2DEdge edgeA, myCvSubdiv2DEdge edgeB)
{
  myCvSubdiv2DEdge new_edge = 0;
  FPOINT* orgB, *dstA;
  if (!subdiv) {
    CC_ERROR(CC_StsNullPtr, "");
  }
  new_edge = cvSubdiv2DMakeEdge(subdiv);
  cvSubdiv2DSplice(new_edge, cvSubdiv2DGetEdge(edgeA, CC_NEXT_AROUND_LEFT));
  cvSubdiv2DSplice(CC_SUBDIV2D_SYM_EDGE(new_edge), edgeB);
  dstA = CC_SUBDIV2D_EDGE_DST(edgeA);
  orgB = CC_SUBDIV2D_EDGE_ORG(edgeB);
  cvSubdiv2DSetEdgePoints(new_edge, dstA, orgB);
  return new_edge;
}
void cvSubdiv2DSwapEdges(myCvSubdiv2DEdge edge)
{
  myCvSubdiv2DEdge sym_edge = CC_SUBDIV2D_SYM_EDGE(edge);
  myCvSubdiv2DEdge a = cvSubdiv2DGetEdge(edge, CC_PREV_AROUND_ORG);
  myCvSubdiv2DEdge b = cvSubdiv2DGetEdge(sym_edge, CC_PREV_AROUND_ORG);
  FPOINT* dstB, *dstA;
  cvSubdiv2DSplice(edge, a);
  cvSubdiv2DSplice(sym_edge, b);
  dstA = CC_SUBDIV2D_EDGE_DST(a);
  dstB = CC_SUBDIV2D_EDGE_DST(b);
  cvSubdiv2DSetEdgePoints(edge, dstA, dstB);
  cvSubdiv2DSplice(edge, cvSubdiv2DGetEdge(a, CC_NEXT_AROUND_LEFT));
  cvSubdiv2DSplice(sym_edge, cvSubdiv2DGetEdge(b, CC_NEXT_AROUND_LEFT));
}
int icvIsRightOf(FPOINT pt, myCvSubdiv2DEdge edge)
{
  FPOINT* org = CC_SUBDIV2D_EDGE_ORG(edge), *dst = CC_SUBDIV2D_EDGE_DST(edge);
  Cv32suf cw_area;
  cw_area.f = (float) cvTriangleArea(pt, dst[ 0 ], org[ 0 ]);
  return (cw_area.i > 0) * 2 - (cw_area.i * 2 != 0);
}
// �� Delaunay���ǲ����ж�λ�����
// subdiv Delaunay �����������ָ�ṹ.
// pt ����λ�������
// edge ��������Ӧ�������Ե(�������ϻ�������)
// vertex ��������Ӧ�������������(ָ��double����)����ѡ��
// ���� cvSubdiv2DLocate �ڻ����ж�λ����㣬����5������:
// ���������ĳС�����ڡ� �������ز�����CC_PTLOC_INSIDE ��*edge �а���С����ı�Ե֮һ��
// �������p�ڱ�Ե֮�ϡ� �������ز��� CC_PTLOC_ON_EDGE �� *edge �����˱�Ե��
// ������뻮�ֵĶ���֮һ���Ӧ�� �������ز��� CC_PTLOC_VERTEX �� *vertex �а���ָ��ö����ָ�룻
// ��������ڻ��ֵĲο�����֮�⡣ �������ز��� CC_PTLOC_OUTSIDE_RECT�Ҳ���д�κ�ָ�롣
// �������֮һ���󡣺��������д���(����Ѿ�ѡ���˳�Ĭ���߸�ĸ����ģʽ����������CC_PTLOC_ERROR) ��
int cvSubdiv2DLocate(myCvSubdiv2D* subdiv, FPOINT pt,
    myCvSubdiv2DEdge* _edge, FPOINT** _point)
{
  myCvSubdiv2DEdge edge = 0;
  FPOINT* point = 0;
  int location = CC_PTLOC_ERROR;
  int i, max_edges;
  int right_of_curr = 0;
  if (!subdiv) {
    CC_ERROR(CC_StsNullPtr, "");
  }
  max_edges = subdiv->edge_num * 4;
  edge = subdiv->recent_edge;
  if (max_edges == 0) {
    return 0;
  }
  if (!edge) {
    return 0;
  }
  location = CC_PTLOC_OUTSIDE_RECT;
  if (pt.x < subdiv->topleft.x || pt.y < subdiv->topleft.y ||
      pt.x >= subdiv->bottomright.x || pt.y >= subdiv->bottomright.y) {
    return 0;
  }
  location = CC_PTLOC_ERROR;
  right_of_curr = icvIsRightOf(pt, edge);
  if (right_of_curr > 0) {
    edge = CC_SUBDIV2D_SYM_EDGE(edge);
    right_of_curr = -right_of_curr;
  }
  for (i = 0; i < max_edges; i++) {
    myCvSubdiv2DEdge onext_edge = CC_SUBDIV2D_NEXT_EDGE(edge);
    myCvSubdiv2DEdge dprev_edge = cvSubdiv2DGetEdge(edge, CC_PREV_AROUND_DST);
    int right_of_onext = icvIsRightOf(pt, onext_edge);
    int right_of_dprev = icvIsRightOf(pt, dprev_edge);
    if (right_of_dprev > 0) {
      if (right_of_onext > 0 || right_of_onext == 0 && right_of_curr == 0) {
        location = CC_PTLOC_INSIDE;
        break;
      }
      else {
        right_of_curr = right_of_onext;
        edge = onext_edge;
      }
    }
    else {
      if (right_of_onext > 0) {
        if (right_of_dprev == 0 && right_of_curr == 0) {
          location = CC_PTLOC_INSIDE;
          break;
        }
        else {
          right_of_curr = right_of_dprev;
          edge = dprev_edge;
        }
      }
      else if (right_of_curr == 0 &&
          icvIsRightOf(CC_SUBDIV2D_EDGE_DST(onext_edge) [ 0 ], edge) >= 0) {
        edge = CC_SUBDIV2D_SYM_EDGE(edge);
      }
      else {
        right_of_curr = right_of_onext;
        edge = onext_edge;
      }
    }
  }
  //__END__;
  subdiv->recent_edge = edge;
  if (location == CC_PTLOC_INSIDE) {
    double t1, t2, t3;
    FPOINT org_pt = CC_SUBDIV2D_EDGE_ORG(edge) [ 0 ];
    FPOINT dst_pt = CC_SUBDIV2D_EDGE_DST(edge) [ 0 ];
    t1 = fabs(pt.x - org_pt.x);
    t1 += fabs(pt.y - org_pt.y);
    t2 = fabs(pt.x - dst_pt.x);
    t2 += fabs(pt.y - dst_pt.y);
    t3 = fabs(org_pt.x - dst_pt.x);
    t3 += fabs(org_pt.y - dst_pt.y);
    if (t1 < FLT_EPSILON) {
      location = CC_PTLOC_VERTEX;
      point = CC_SUBDIV2D_EDGE_ORG(edge);
      edge = 0;
    }
    else if (t2 < FLT_EPSILON) {
      location = CC_PTLOC_VERTEX;
      point = CC_SUBDIV2D_EDGE_DST(edge);
      edge = 0;
    }
    else if ((t1 < t3 || t2 < t3) &&
        fabs(cvTriangleArea(pt, org_pt, dst_pt)) < FLT_EPSILON) {
      location = CC_PTLOC_ON_EDGE;
      point = 0;
    }
  }
  if (location == CC_PTLOC_ERROR) {
    edge = 0;
    point = 0;
  }
  if (_edge) {
    * _edge = edge;
  }
  if (_point) {
    * _point = point;
  }
  return location;
}
int icvIsPtInCircle3(FPOINT pt, FPOINT a, FPOINT b, FPOINT c)
{
  double val = (a.x * a.x + a.y * a.y) * cvTriangleArea(b, c, pt);
  val -= (b.x * b.x + b.y * b.y) * cvTriangleArea(a, c, pt);
  val += (c.x * c.x + c.y * c.y) * cvTriangleArea(a, b, pt);
  val -= (pt.x * pt.x + pt.y * pt.y) * cvTriangleArea(a, b, c);
  return val > FLT_EPSILON ? 1 : val < -FLT_EPSILON ? -1 : 0;
}
// �� Delaunay���ǲ����в���һ����
// FPOINT* cvSubdivDelaunay2DInsert( myCvSubdiv2D* subdiv, FPOINT pt);
// subdiv ͨ������ cvCreateSubdivDelaunay2D.���ɵ�Delaunay����
// pt ������ĵ�
// ���� cvSubdivDelaunay2DInsert �򻮷ֵĽṹ�в���һ���㲢����ȷ�ظı仮�ֵ����ӽṹ��
// ������ֽṹ���Ѿ�����һ����ͬ������㣬�򲻻����µ���롣
// �ú�������ָ���Ѳ�����ָ�롣������ضϣ��������κ�������ꡣ
FPOINT* cvSubdivDelaunay2DInsert(myCvSubdiv2D* subdiv, FPOINT pt)
{
  FPOINT* point = 0;
  int location = CC_PTLOC_ERROR;
  FPOINT* curr_point = 0, *first_point = 0;
  myCvSubdiv2DEdge curr_edge = 0, deleted_edge = 0, base_edge = 0;
  int i, max_edges;
  if (!subdiv) {
    CC_ERROR(CC_StsNullPtr, "");
  }
  location = cvSubdiv2DLocate(subdiv, pt, &curr_edge, &curr_point);
  switch (location) {
  case CC_PTLOC_ERROR:
    CC_ERROR_FROM_STATUS(CC_BADSIZE_ERR);
  case CC_PTLOC_OUTSIDE_RECT:
    CC_ERROR_FROM_STATUS(CC_BADRANGE_ERR);
  case CC_PTLOC_VERTEX:
    point = curr_point;
    break;
  case CC_PTLOC_ON_EDGE:
    deleted_edge = curr_edge;
    subdiv->recent_edge = curr_edge = cvSubdiv2DGetEdge(curr_edge, CC_PREV_AROUND_ORG);
    cvSubdiv2DDeleteEdge(subdiv, deleted_edge);
    /* no break */
  case CC_PTLOC_INSIDE:
    assert(curr_edge != 0);
    subdiv->is_geometry_valid = 0;
    curr_point = cvSubdiv2DAddPoint(subdiv, pt, 0);
    base_edge = cvSubdiv2DMakeEdge(subdiv);
    first_point = CC_SUBDIV2D_EDGE_ORG(curr_edge);
    cvSubdiv2DSetEdgePoints(base_edge, first_point, curr_point);
    cvSubdiv2DSplice(base_edge, curr_edge);
    do {
      base_edge = cvSubdiv2DConnectEdges(subdiv, curr_edge,
          CC_SUBDIV2D_SYM_EDGE(base_edge));
      curr_edge = cvSubdiv2DGetEdge(base_edge, CC_PREV_AROUND_ORG);
    }
    while (CC_SUBDIV2D_EDGE_DST(curr_edge) != first_point);
    curr_edge = cvSubdiv2DGetEdge(base_edge, CC_PREV_AROUND_ORG);
    max_edges = subdiv->edge_num * 4;
    for (i = 0; i < max_edges; i++) {
      FPOINT* temp_dst = 0, *curr_org = 0, *curr_dst = 0;
      myCvSubdiv2DEdge temp_edge = cvSubdiv2DGetEdge(curr_edge, CC_PREV_AROUND_ORG);
      temp_dst = CC_SUBDIV2D_EDGE_DST(temp_edge);
      curr_org = CC_SUBDIV2D_EDGE_ORG(curr_edge);
      curr_dst = CC_SUBDIV2D_EDGE_DST(curr_edge);
      if (icvIsRightOf(temp_dst[ 0 ], curr_edge) > 0 &&
          icvIsPtInCircle3(curr_org[ 0 ], temp_dst[ 0 ],
              curr_dst[ 0 ], curr_point[ 0 ]) < 0) {
        cvSubdiv2DSwapEdges(curr_edge);
        curr_edge = cvSubdiv2DGetEdge(curr_edge, CC_PREV_AROUND_ORG);
      }
      else if (curr_org == first_point) {
        break;
      }
      else {
        curr_edge = cvSubdiv2DGetEdge(CC_SUBDIV2D_NEXT_EDGE(curr_edge),
            CC_PREV_AROUND_LEFT);
      }
    }
    break;
  default:
    assert(0);
    CC_ERROR_FROM_STATUS(CC_NOTDEFINED_ERR);
  }
  point = curr_point;
  //icvSubdiv2DCheck( subdiv );
  return point;
}
void cvInitSubdivDelaunay2D(myCvSubdiv2D* subdiv, IRECT rect)
{
  float big_coord = 3.f * MAX(RCW(&rect), RCH(&rect));
  FPOINT ppA, ppB, ppC;
  FPOINT* pA, *pB, *pC;
  myCvSubdiv2DEdge edge_AB, edge_BC, edge_CA;
  float rx = (float) rect.l;
  float ry = (float) rect.t;
  if (!subdiv) {
    CC_ERROR(CC_StsNullPtr, "");
  }
  //cvClearSet( (CvSet *) (subdiv->edges) );
  //cvClearSet( (CvSet *) subdiv );
  subdiv->edge_num = 0;
  subdiv->recent_edge = 0;
  subdiv->is_geometry_valid = 0;
  subdiv->topleft = fPOINT(rx, ry);
  subdiv->bottomright = fPOINT(rect.r, rect.b);
  ppA = fPOINT(rx + big_coord, ry);
  ppB = fPOINT(rx, ry + big_coord);
  ppC = fPOINT(rx - big_coord, ry - big_coord);
  pA = cvSubdiv2DAddPoint(subdiv, ppA, 0);
  pB = cvSubdiv2DAddPoint(subdiv, ppB, 0);
  pC = cvSubdiv2DAddPoint(subdiv, ppC, 0);
  edge_AB = cvSubdiv2DMakeEdge(subdiv);
  edge_BC = cvSubdiv2DMakeEdge(subdiv);
  edge_CA = cvSubdiv2DMakeEdge(subdiv);
  cvSubdiv2DSetEdgePoints(edge_AB, pA, pB);
  cvSubdiv2DSetEdgePoints(edge_BC, pB, pC);
  cvSubdiv2DSetEdgePoints(edge_CA, pC, pA);
  cvSubdiv2DSplice(edge_AB, CC_SUBDIV2D_SYM_EDGE(edge_CA));
  cvSubdiv2DSplice(edge_BC, CC_SUBDIV2D_SYM_EDGE(edge_AB));
  cvSubdiv2DSplice(edge_CA, CC_SUBDIV2D_SYM_EDGE(edge_BC));
  subdiv->recent_edge = edge_AB;
}
void
icvCreateCenterNormalLine(myCvSubdiv2DEdge edge, double* _a, double* _b, double* _c)
{
  FPOINT org = *CC_SUBDIV2D_EDGE_ORG(edge);
  FPOINT dst = *CC_SUBDIV2D_EDGE_DST(edge);
  double a = dst.x - org.x;
  double b = dst.y - org.y;
  double c = -(a * (dst.x + org.x) + b * (dst.y + org.y));
  *_a = a + a;
  *_b = b + b;
  *_c = c;
}
void
icvIntersectLines3(double* a0, double* b0, double* c0,
    double* a1, double* b1, double* c1, FPOINT* point)
{
  double det = a0[ 0 ] * b1[ 0 ] - a1[ 0 ] * b0[ 0 ];
  if (det != 0) {
    det = 1. / det;
    point->x = (float)((b0[ 0 ] * c1[ 0 ] - b1[ 0 ] * c0[ 0 ]) * det);
    point->y = (float)((a1[ 0 ] * c0[ 0 ] - a0[ 0 ] * c1[ 0 ]) * det);
  }
  else {
    point->x = point->y = FLT_MAX;
  }
}
// ����Voronoiͼ���ϸ���ṹ
// void cvCalcSubdivVoronoi2D( myCvSubdiv2D* subdiv );
// subdiv Delaunay ����,�������еĵ��Ѿ���� ��
// ���� cvCalcSubdivVoronoi2D �����������꣬������ԭ�����е�
// ĳ�������Ӧ������γ���(�������໥����ʱ)�ö����Voronoi ϸ���ı߽硣
void cvCalcSubdivVoronoi2D(myCvSubdiv2D* subdiv)
{
  // CvSeqReader reader;
  int i, total;
  if (!subdiv) {
    CC_ERROR(CC_StsNullPtr, "");
  }
  /* check if it is already calculated */
  if (subdiv->is_geometry_valid) {
    return ;
  }
  total = subdiv->edge_num;
  //cvClearSubdivVoronoi2D( subdiv );
  //cvStartReadSeq( (CvSeq *) (subdiv->edges), &reader, 0 );
  if (total <= 3) {
    return ;
  }
  /* skip first three edges (bounding triangle) */
  //for( i = 0; i < 3; i++ )
  // CC_NEXT_SEQ_ELEM( elem_size, reader );
  /* loop through all quad-edges */
  for (i = 3 ; i < total; i++) {
    myCvQuadEdge2D* quadedge = subdiv->edges + i;
    //if( CC_IS_SET_ELEM( quadedge ))
    {
      myCvSubdiv2DEdge edge0 = (myCvSubdiv2DEdge) quadedge, edge1, edge2;
      double a0, b0, c0, a1, b1, c1;
      FPOINT virt_point;
      FPOINT* voronoi_point;
      if (!quadedge->pt[ 3 ]) {
        edge1 = cvSubdiv2DGetEdge(edge0, CC_NEXT_AROUND_LEFT);
        edge2 = cvSubdiv2DGetEdge(edge1, CC_NEXT_AROUND_LEFT);
        icvCreateCenterNormalLine(edge0, &a0, &b0, &c0);
        icvCreateCenterNormalLine(edge1, &a1, &b1, &c1);
        icvIntersectLines3(&a0, &b0, &c0, &a1, &b1, &c1, &virt_point);
        if (fabs(virt_point.x) < FLT_MAX * 0.5 &&
            fabs(virt_point.y) < FLT_MAX * 0.5) {
          voronoi_point = cvSubdiv2DAddPoint(subdiv, virt_point, 1);
          quadedge->pt[ 3 ] =
              ((myCvQuadEdge2D*)(edge1 & ~3)) ->pt[ 3 - (edge1 & 2) ] =
                  ((myCvQuadEdge2D*)(edge2 & ~3)) ->pt[ 3 - (edge2 & 2) ] = voronoi_point;
        }
      }
      if (!quadedge->pt[ 1 ]) {
        edge1 = cvSubdiv2DGetEdge(edge0, CC_NEXT_AROUND_RIGHT);
        edge2 = cvSubdiv2DGetEdge(edge1, CC_NEXT_AROUND_RIGHT);
        icvCreateCenterNormalLine(edge0, &a0, &b0, &c0);
        icvCreateCenterNormalLine(edge1, &a1, &b1, &c1);
        icvIntersectLines3(&a0, &b0, &c0, &a1, &b1, &c1, &virt_point);
        if (fabs(virt_point.x) < FLT_MAX * 0.5 &&
            fabs(virt_point.y) < FLT_MAX * 0.5) {
          voronoi_point = cvSubdiv2DAddPoint(subdiv, virt_point, 1);
          quadedge->pt[ 1 ] =
              ((myCvQuadEdge2D*)(edge1 & ~3)) ->pt[ 1 + (edge1 & 2) ] =
                  ((myCvQuadEdge2D*)(edge2 & ~3)) ->pt[ 1 + (edge2 & 2) ] = voronoi_point;
        }
      }
    }
  }
  subdiv->is_geometry_valid = 1;
}
int
icvIsRightOf2(const FPOINT pt, const FPOINT org, const FPOINT diff)
{
  Cv32suf cw_area;
  cw_area.f = (org.x - pt.x) * diff.y - (org.y - pt.y) * diff.x;
  return (cw_area.i > 0) * 2 - (cw_area.i * 2 != 0);
}
// ��������㣬�ҵ�������Ļ��ֶ���
// subdiv Delaunay�����������ַ�ʽ
// pt �����
// ���� cvFindNearestPoint2D ����һ����λ�����ĺ�����
// �ú����ҵ�������������ֶ��㡣���ܻ��ֳ���С����(facet)��������Ϊ��ʼ�㣬
// ��������㲻һ���ǵ��������ҵ��Ķ������ڵ�С����֮�ڡ�
// �ú�������ָ���ҵ��Ļ��ֶ����ָ�롣
FPOINT* cvFindNearestPoint2D(myCvSubdiv2D* subdiv, FPOINT pt)
{
  FPOINT* point = 0;
  FPOINT start;
  FPOINT diff;
  int loc;
  myCvSubdiv2DEdge edge;
  int i, total = subdiv->point_num;
  if (!subdiv) {
    CC_ERROR(CC_StsNullPtr, "");
  }
  if (!subdiv->is_geometry_valid) {
    cvCalcSubdivVoronoi2D(subdiv);
  }
  loc = cvSubdiv2DLocate(subdiv, pt, &edge, &point);
  switch (loc) {
  case CC_PTLOC_ON_EDGE:
  case CC_PTLOC_INSIDE:
    break;
  default:
    return point;
  }
  point = 0;
  start = CC_SUBDIV2D_EDGE_ORG(edge) [ 0 ];
  diff.x = pt.x - start.x;
  diff.y = pt.y - start.y;
  edge = CC_SUBDIV2D_ROTATE_EDGE(edge, 1);
  for (i = 0; i < total; i++) {
    FPOINT t;
    for (;;) {
      assert(CC_SUBDIV2D_EDGE_DST(edge));
      t = CC_SUBDIV2D_EDGE_DST(edge) [ 0 ];
      if (icvIsRightOf2(t, start, diff) >= 0) {
        break;
      }
      edge = cvSubdiv2DGetEdge(edge, CC_NEXT_AROUND_LEFT);
    }
    for (;;) {
      assert(CC_SUBDIV2D_EDGE_ORG(edge));
      t = CC_SUBDIV2D_EDGE_ORG(edge) [ 0 ];
      if (icvIsRightOf2(t, start, diff) < 0) {
        break;
      }
      edge = cvSubdiv2DGetEdge(edge, CC_PREV_AROUND_LEFT);
    }
    {
      FPOINT tempDiff = CC_SUBDIV2D_EDGE_DST(edge) [ 0 ];
      t = CC_SUBDIV2D_EDGE_ORG(edge) [ 0 ];
      tempDiff.x -= t.x;
      tempDiff.y -= t.y;
      if (icvIsRightOf2(pt, t, tempDiff) >= 0) {
        point = CC_SUBDIV2D_EDGE_ORG(CC_SUBDIV2D_ROTATE_EDGE(edge, 3));
        break;
      }
    }
    edge = CC_SUBDIV2D_SYM_EDGE(edge);
  }
  return point;
}
myCvSubdiv2D* CreateSubdivDelaunay2D(int vtx_size, IRECT rect)
{
  myCvSubdiv2D* subdiv = cvCreateSubdiv2D(vtx_size, 3 * vtx_size + 3);
  cvInitSubdivDelaunay2D(subdiv, rect);
  return subdiv;
}
#if 0
// �����������ʷ�
int QuickSubdiv2D_(int total, const FPOINT* pointset, PEDGEREF out)
{
  typedef FPOINT T;
  int sp = 0;
  struct {
    T* lb;
    T* ub;
  }
  stack[ 48 ];
  stack[ 0 ].lb = pointset;
  stack[ 0 ].ub = pointset + total;
  while (sp >= 0) {
    T* left = stack[ sp ].lb;
    T* right = stack[ sp-- ].ub;
    for (;;) {}
  }
  return 0;
}
int FindMinyCH(int chtatal, FPOINT** ch)
{
  int id_min = 0, i;
  for (i = 0; i < chtatal; ++i) {
    if (ch[ i ] ->y < ch[ id_min ] ->y) {
      id_min = 0;
    }
  }
  return id_min;
}
void QuickSubdiv2D(int total, const FPOINT* pointset, FPOINT** ch, PEDGEREF out, int* chlen, int* outlen)
{
  int i;
  if (2 == total) {
    if (pointset[ 0 ].y > pointset[ 1 ].y) {
      ch[ 0 ] = pointset;
      ch[ 1 ] = pointset + 1;
    }
    else {
      ch[ 0 ] = pointset + 1;
      ch[ 1 ] = pointset;
    }
  }
  else if (1 == total) {
    ch[ 0 ] = pointset;
  }
  else if (2 < total) {
    int left_total = total / 2;
    int right_total = total - left_total;
    int left_chlen, right_chlen;
    int left_outlen, right_outlen;
    int left_miny, right_miny;
    QuickSubdiv2D(left_total, pointset, ch, out, &left_chlen, &left_outlen);
    QuickSubdiv2D(right_total, pointset + left_total, ch + left_total, out + left_outlen, &right_chlen, &right_outlen);
    left_miny = FindMinyCH(left_chlen, ch);
    right_miny = FindMinyCH(right_chlen, ch + left_total);
  }
}
#endif
int IsCurrentEdgeAlreadyIn(const int* edges, int edge_num, int ind1, int ind2)
{
  int i;
  for (i = 0; i < edge_num; i++) {
    if ((edges[ i * 2 + 0 ] == ind1 && edges[ i * 2 + 1 ] == ind2) ||
        (edges[ i * 2 + 0 ] == ind2 && edges[ i * 2 + 1 ] == ind1)) {
      return 1;
    }
  }
  return 0;
}
int VectorIsNotInFormerTriangles(const int* one_tri, const int* tris, int tri_num)
{
  int i;
  for (i = 0; i < tri_num; ++i, tris += 3) {
    if ((tris[ 0 ] == one_tri[ 0 ]) && (tris[ 1 ] == one_tri[ 1 ]) && (tris[ 2 ] == one_tri[ 2 ])) {
      return 0;
    }
  }
  return 1;
}
typedef size_t CvSubdiv2DEdge;
typedef FPOINT CvPoint2D32f;
#define CC_SUBDIV2D_VIRTUAL_POINT_FLAG (1 << 30)
typedef struct CvSubdiv2DPoint {
  int flags;
  CvSubdiv2DEdge first;
  CvPoint2D32f pt;
}
CvSubdiv2DPoint;
typedef struct CvQuadEdge2D {
  CvSubdiv2DPoint* pt[4];
  CvSubdiv2DEdge next[4];
}
CvQuadEdge2D;
int Delaunay(int nPoints, const FPOINT* pt, int* edges, int* pedge_num, int* tris, int* ptri_num)
{
  int i, j;
  int edge_num = 0, tri_num = 0;
  IRECT rect;
  //CvSubdiv2D* subdiv;
  myCvSubdiv2D* subdiv;
  double minx, miny, maxx, maxy;
  if (nPoints < 2) {
    return 0;
  }
  minx = pt[0].x, miny = pt[0].y, maxx = pt[0].x, maxy = pt[0].y;
  for (i = 1; i < nPoints; ++i) {
    minx = MIN(minx, pt[i].x);
    miny = MIN(miny, pt[i].y);
    maxx = MAX(maxx, pt[i].x);
    maxy = MAX(maxy, pt[i].y);
  }
  rect.l = (int)minx - 5;
  rect.r = (int)maxx + 5;
  rect.t = (int)miny - 5;
  rect.b = (int)maxy + 5;
  subdiv = CreateSubdivDelaunay2D(nPoints * 2, rect);
  for (i = 0; i < nPoints; ++i) {
    cvSubdivDelaunay2DInsert(subdiv, pt[i]);
  }
  if (edges) {
    for (i = 0; i < subdiv->edge_num; i++) {
      int org_pt, dst_pt;
      CvQuadEdge2D* e = (CvQuadEdge2D*)(subdiv->edges + i);
      org_pt = CC_SUBDIV2D_EDGE_ORG(e) - subdiv->points - 3;
      dst_pt = CC_SUBDIV2D_EDGE_DST(e) - subdiv->points - 3;
      if (org_pt >= 0 && dst_pt >= 0) {
        edges[ edge_num * 2 + 0 ] = org_pt;
        edges[ edge_num * 2 + 1 ] = dst_pt;
        ++edge_num;
      }
    }
    if (pedge_num) {
      *pedge_num = edge_num;
    }
    if (tris) {
      int one_tri[ 3 ], t;
      // secondly we start to build triangles
      for (i = 0; i < edge_num; i++) {
        int ind1 = edges[ i * 2 + 0 ];
        int ind2 = edges[ i * 2 + 1 ];
        for (j = 0; j < nPoints; j++) {
          // At most, there are only 2 triangles that can be added
          if (IsCurrentEdgeAlreadyIn(edges, edge_num, ind1, j) &&
              IsCurrentEdgeAlreadyIn(edges, edge_num, ind2, j)) {
            one_tri[ 0 ] = ind1;
            one_tri[ 1 ] = ind2;
            one_tri[ 2 ] = j;
            SORT3(one_tri, LESS, t);
            if (VectorIsNotInFormerTriangles(one_tri, tris, tri_num)) {
              //tris.push_back(one_tri);
              tris[ tri_num * 3 + 0 ] = one_tri[ 0 ];
              tris[ tri_num * 3 + 1 ] = one_tri[ 1 ];
              tris[ tri_num * 3 + 2 ] = one_tri[ 2 ];
              ++tri_num;
            }
          }
        }
      }
      if (ptri_num) {
        *ptri_num = tri_num;
      }
    }
  }
  cvClearSubdivVoronoi2D(subdiv);
  return edge_num;
}
int delaunay_memsize(delaunay_t* d)
{
  d->buflen = 2 * (d->point_num * 6 + 20) * sizeof(int);
  return d->buflen;
}
int delaunay_process(delaunay_t* d)
{
  BUFUSEBEGIN(d->buf, d->buflen);
  int n = d->point_num * 6 + 20;
  d->edges = BUFMALLOC(int, n);
  d->tris = BUFMALLOC(int, n);
  Delaunay(d->point_num, d->pt, d->edges, &d->edge_num, d->tris, &d->tri_num);
  BUFUSEEND();
  return 0;
}

