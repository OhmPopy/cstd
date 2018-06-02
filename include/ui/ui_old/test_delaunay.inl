
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // for time
#include "cstd.h"

//#include "geometry.inl"
//#include "geo/subdiv.h"
//#include "geo/subdiv.inl"
#include "ui/window.inl"




#include "cv/geometry.inl"
#include "cv/subdivision2d.inl"


//typedef myCvSubdiv2D CvSubdiv2D;

//adapted by the example of leanring opencv by crazy_007
//adapted by the OpenCV2.0\samples\c\delaunay.c
//2010-4-22

int draw_subdiv_edge(img_t* img, CvSubdiv2DEdge edge, COLOR color);   //Ϊ�˲鿴���뷽�㣬��Ȼ��������ú���˳��

/* the script demostrates iterative construction of
   delaunay triangulation and voronoi tesselation */


//draw subdiv point
int draw_subdiv_point(img_t* img, CvPoint2D32f fp, COLOR color)
{
  DrawCircle(IMARG(img), cvRound(fp.x), cvRound(fp.y), 5, color, 0, 0);  //��fpΪԲ�ģ�5Ϊ�뾶��ʵ��Բ��ʾdelaunay����
  return 0;
}
//use an external point to locate an edge or vertex or step around the edges of a delaunay tirangle
//����delaunay ����
int locate_point(CvSubdiv2D* subdiv, CvPoint2D32f fp, img_t* img,
    COLOR active_color)
{
  CvSubdiv2DEdge e;
  CvSubdiv2DEdge e0 = 0;
  CvSubdiv2DPoint* p = 0;

  cvSubdiv2DLocate(subdiv, fp, &e0, &p);  //ʹ��һ���ⲿ�ĵ㶨λ��Ե�򶥵�

  //�ú�����������εı�Ե�Ͷ���������õ������ڵ�Voronoi��
  if (e0) {
    e = e0;

    do {
      draw_subdiv_edge(img, e, active_color);  //�������溯����������ɫֱ��
      e = cvSubdiv2DGetEdge(e, CV_NEXT_AROUND_LEFT); //����Delaunayͼ���������������һ���ı�Ե
    }
    while (e != e0);
  }

  draw_subdiv_point(img, fp, active_color);  //��������ĺ�����ʵ���ڸõ㴦���뾶Ϊ5��Բ
  return 0;
}




/*************                 �ָ���                ************************************/
//draw subdiv edge
int draw_subdiv_edge(img_t* img, CvSubdiv2DEdge edge, COLOR color)
{
  CvSubdiv2DPoint* org_pt;
  CvSubdiv2DPoint* dst_pt;
  CvPoint2D32f org;
  CvPoint2D32f dst;
  IPOINT iorg, idst;

  org_pt = cvSubdiv2DEdgeOrg(edge);//Delaunay����Voronoi��Ե��ԭʼ��
  dst_pt = cvSubdiv2DEdgeDst(edge);//Delaunay����Voronoi��Ե���յ�

  if (org_pt && dst_pt) {
    org = org_pt->pt;
    dst = dst_pt->pt;

    iorg = iPOINT(cvRound(org.x), cvRound(org.y));
    idst = iPOINT(cvRound(dst.x), cvRound(dst.y));

    DrawLine(IMARG(img), iorg, idst, color, 1);  //����ɫֱ��
  }

  return 0;
}

//draw subdiv:�������е�Delaunay��
int draw_subdiv(img_t* img, CvSubdiv2D* subdiv,
    COLOR delaunay_color, COLOR voronoi_color)
{
  int i, total = subdiv->edge_num;

  for (i = 0; i < total; i++) { //total�Ǳ���Ŀ
    CvQuadEdge2D* edge = subdiv->edges + i;
    //CvQuadEdge2Dƽ�滮���е�Quad-edge(�ķ���Ե�ṹ):�ĸ���Ե (e, eRot(��ɫ) �Լ����ǵ��棨��ɫ��

    {
      draw_subdiv_edge(img, (CvSubdiv2DEdge)edge + 1, voronoi_color);   //��֪������(CvSubdiv2DEdge)edge + 1
      //����P346��voronoi_edge=(CvSubdiv2DEdge)edge + 1
      //ֱ�Ӳ�������λ�Ʒ����и��ֱߵĶ�Ӧ��(��edge+1),
      //cvSubdiv2DRotateEdge((CvSubdiv2DEdge)edge,1)=(CvSubdiv2DEdge)edge+1
      //�ο���ַΪ��http://tech.ddvip.com/2007-12/119897724239781.html
      draw_subdiv_edge(img, (CvSubdiv2DEdge)edge, delaunay_color);   //����������Ӻ���
    }

  }

  return 0;
}


//draw the voronoi facet:����Voronoi��
int draw_subdiv_facet(img_t* img, CvSubdiv2DEdge edge)
{
  CvSubdiv2DEdge t = edge;
  int i, count = 0;
  IPOINT* buf = 0;

  // count number of edges in facet
  do {
    count++;
    t = cvSubdiv2DGetEdge(t, CV_NEXT_AROUND_LEFT);  //�������������һ���ı�Ե
  }
  while (t != edge);

  buf = (IPOINT*)malloc(count * sizeof(buf[0]));

  // gather points
  t = edge;

  for (i = 0; i < count; i++) {
    CvSubdiv2DPoint* pt = cvSubdiv2DEdgeOrg(t);  //��ñ�Ե�����

    if (!pt) {
      break;
    }

    buf[i] = iPOINT(cvRound(pt->pt.x), cvRound(pt->pt.y)); //���¼��buf��
    t = cvSubdiv2DGetEdge(t, CV_NEXT_AROUND_LEFT);
  }

  if (i == count) {
    CvSubdiv2DPoint* pt = cvSubdiv2DEdgeDst(cvSubdiv2DRotateEdge(edge, 1));   //��ñ�Ե���յ�
    FillConvexPoly(IMARG(img), buf, count, _RGB(rand() & 255, rand() & 255, rand() & 255)); //һ��ֻ�ܻ�һ������Σ�����ֻ�ܻ�͹�����
    PolyLine(IMARG(img), buf, &count, 1, 1, _RGB(0, 0, 0), 1); //һ�ε����л��ƶ�������
    draw_subdiv_point(img, pt->pt, CV_RGB(0, 0, 0)); //��Բ
  }

  free(buf);
  return 0;
}
//draw & paint voronoi graph
int paint_voronoi(CvSubdiv2D* subdiv, img_t* img)
{
  int i, total = subdiv->edge_num;

  cvCalcSubdivVoronoi2D(subdiv);  //����Voronoiͼ���ϸ���ṹ

  for (i = 0; i < total; i++) {
    CvQuadEdge2D* edge = subdiv->edges + i;

    {
      CvSubdiv2DEdge e = (CvSubdiv2DEdge)edge;
      // left
      draw_subdiv_facet(img, cvSubdiv2DRotateEdge(e, 1));   //����������Ӻ���

      // right
      draw_subdiv_facet(img, cvSubdiv2DRotateEdge(e, 3));
    }

  }

  return 0;
}


int test_delaunay()
{
  char win[] = "source";
  int i;
  CvRect rect = { 0, 0, 600, 600 };//���߽���δ�С
  CvSubdiv2D subdiv[1] = {0};//ϸ��
  img_t img[1] = {0};
  COLOR active_facet_color, delaunay_color, voronoi_color, bkgnd_color;

  active_facet_color = CV_RGB(255, 0, 0);
  delaunay_color = CV_RGB(0, 0, 0);
  voronoi_color = CV_RGB(0, 180, 0);
  bkgnd_color = CV_RGB(255, 255, 255);

  imsetsize(img, rect.h, rect.w, 3, 1);  //������ɫ������ͼ��
  imdraw_fill(IMARG(img), bkgnd_color);

  cvNamedWindow(win, 1);  //��������Ϊ"source"

  //initialization convenience function for delaunay subdivision�������Ӻ���ȷ�����α߽�
  cvInitSubdivDelaunay2D(subdiv, 100, 100, rect);  //����ȷ���ı߽�

  for (i = 0; i < 20; i++) {
    CvPoint2D32f fp = cvPoint2D32f((float)(rand() % (rect.w - 10) + 5),
        (float)(rand() % (rect.h - 10) + 5)); //This is our point holder

    locate_point(subdiv, fp, img, active_facet_color);  //���ú���
    //cvShowImage( win, (img)->h, (img)->w, (img)->tt.data, (img)->s, (img)->c );

    //�ȴ�600ms
    //if( cvWaitKey( 100 ) >= 0 )  break;

    cvSubdivDelaunay2DInsert(subdiv, fp);  //��Delaunay���ǲ����в���һ����
    //cvCalcSubdivVoronoi2D( subdiv );//����Voronoiͼ���ϸ���ṹ
    //imdraw_fill(IMARG(img), bkgnd_color);
    // ��һ������ȫ��Ԫ�ظ�ֵ:int cvSet( CvArr* arr, COLOR value, const CvArr* mask=NULL );
    //draw_subdiv( img, subdiv, delaunay_color, voronoi_color );//�����Ӻ�����cvSeqReader�𲽱����������ϸ�ֽṹ
    //cvShowImage( win, (img)->h, (img)->w, (img)->tt.data, (img)->s, (img)->c );

    //if( cvWaitKey(100) >= 0 ) break;
  }

  cvCalcSubdivVoronoi2D(subdiv);  //����Voronoiͼ���ϸ���ṹ

  imdraw_fill(IMARG(img), bkgnd_color);
  paint_voronoi(subdiv, img);  //�����Ӻ���
  cvShowImage(win, (img)->h, (img)->w, (img)->tt.data, (img)->s, (img)->c);

  cvWaitKey(0);

  cvDestroyWindow(win);
  return 0;
}

