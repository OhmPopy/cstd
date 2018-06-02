
//���� z-buffer �� color-buffer �� Bresenham ֱ�������㷨
//����һ��ֱ��,ֱ�ߵĿ��Ϊ1,ֱ�ߵ���ɫ��ֱ�������˵����ɫ��ͬ����
//ע��,������ɫ����ЧֵΪ[0,1]�����ڵĸ���С��
//��Windows���Ӧ,���㷨Ҳ���������һ����(x2,y2)
//z1,z2Ϊ��ǰ������ֵ
//hDC,pZB�ֱ�Ϊ��ɫ�������Ȼ���
//ע��,��(x1,y1)--(x2,y2)��(x2,y2)--(x1,y1)����Щ��ͬ.
//����:�ú������Ƶ�ֱ��(����λ��)��Windows��LineTo()�������ƵĽ������ȫһ�µ�
void CBLine_Draw(HDDC hDC, ZBuffer* pZB, Vertex3d* v1, Vertex3d* v2)
{
  POINT pt1 = v1->PtOnScrn, pt2 = v2->PtOnScrn;
  float z1 = v1->vInView.z, z2 = v2->vInView.z;
  FRGBA clr1 = v1->clr, clr2 = v2->clr;
  int x1 = pt1.x, y1 = pt1.y, x2 = pt2.x, y2 = pt2.y;
  //���˵���ˮƽƫ�����ʹ�ֱƫ����
  int nDx = x2 - x1;
  int nDy = y2 - y1;
  //���˵���ˮƽ����ʹ�ֱ����
  int nIx = ABS(nDx);
  int nIy = ABS(nDy);
  //��㲽��(������ֵ)
  int nInc = MAX(nIx, nIy);
  //ֱ�ߵ�ǰ�����ɫ
  float fPlotR = clr1.r, fPlotG = clr1.g, fPlotB = clr1.b, fPlotA = clr1.a;
  //��Ҫ������,��ʼ
  //�������е����
  float zInBuffer = GetDepth(pZB, x1, y1);
  //��ǰ���
  float z = z1;
  int i, nTwoIx, nTwoIy, x, y, nJudgeX, nJudgeY;
  float fIncR, fIncG, fIncB, fIncA, fIncZ;

  //�����ǰ���z����С����Ȼ������е���ͬλ��z������
  if (z < zInBuffer) {
    //����ɫ�������л��Ƶ�һ����
    CBSetPixel(hDC, x1, y1, fPlotR, fPlotG, fPlotB, fPlotA);
    //������Ȼ�������ֵ
    SetDepth(pZB, x1, y1, z);
  }

  if (nInc < 2) {
    return;
  }

  //��ֱ�ߵ��������
  fIncZ = (z2 - z1) / nInc;
  //����
  //�����ж��Ƿ���nJudgeX,nJudgeY��������ǰ��
  nJudgeX = -nIy, nJudgeY = -nIx;
  //ͨ����������õ��ĵ�ǰ��
  x = x1, y = y1;
  //��ʼ������������
  //����Bresenham�㷨.������,���ǽ��㷨���иĽ�,
  //ʹ���������б�ʵ�ֱ�߶��ܻ���,���һ��Ʒ���ʼ��Ϊ�������Ƶ��յ�
  //��Windows���Ӧ,���㷨Ҳ���������һ����(x2,y2)
  nInc--;
  //ע��,������2�ڼ�����в�����λʵ��
  //Bresenham�㷨
  nTwoIx = 2 * nIx, nTwoIy = 2 * nIy;
  //��ɫ����
  fIncR = (clr2.r - fPlotR) / nInc;
  fIncG = (clr2.g - fPlotG) / nInc;
  fIncB = (clr2.b - fPlotB) / nInc;
  fIncA = (clr2.a - fPlotA) / nInc;

  //��ʼ��������
  for (i = 0; i < nInc; i++) {
    //ͨ������������ĵ�ǰ���Ƿ�����ֱ���ϵĵ�
    BOOL bPlot = FALSE;
    nJudgeX += nTwoIx;
    nJudgeY += nTwoIy;

    //��� nJudgeX ����
    if (nJudgeX >= 0) {
      bPlot = TRUE;
      nJudgeX -= nTwoIy;

      //�����������ֱ��ͳһ����
      if (nDx > 0) {
        x++;
      }
      else if (nDx < 0) {
        x--;
      }
    }

    //��� nJudgeY ����
    if (nJudgeY >= 0) {
      bPlot = TRUE;
      nJudgeY -= nTwoIx;

      //�����������ֱ��ͳһ����
      if (nDy > 0) {
        y++;
      }
      else if (nDy < 0) {
        y--;
      }
    }

    //�����ǰ����ֱ����
    if (bPlot) {
      fPlotR += fIncR;
      fPlotG += fIncG;
      fPlotB += fIncB;
      fPlotA += fIncA;
      //��Ҫ������,��ʼ
      z += fIncZ;
      zInBuffer = GetDepth(pZB, x, y);

      //�����ǰ���z����С����Ȼ������е���ͬλ��z������
      if (z < zInBuffer) {
        //����ɫ�������л��Ƶ�һ����
        CBSetPixel(hDC, x, y, fPlotR, fPlotG, fPlotB, fPlotA);
        //������Ȼ�������ֵ
        SetDepth(pZB, x, y, z);
      }

      //����
    }//end if
  }//end for
}
//����Z��������������ɨ��ת�������Ʋ���ȡ����Ϣ
//��ȡֱ�߽߱��ϵĵ��x����,��ɫ�����
//x1,y1,x2,y2    ֱ�����˵�����
//z1,z2          �˵����ֵ
//clr1,clr2      ֱ�������յ���ɫ
//pnCoord        ��¼ɨ������ߵĽ����x���꣬
//pnCoord        pnCoord����ĳ���Ӧ����y�����ľ���ֵ��1
//pColor         ��¼���������ɫ��
//pfDepth        ��¼����������ֵ
//hDC,pZB        �ֱ�Ϊ��ɫ�������Ȼ���
//dwFeature      ��һ���߽��¼������������ΪTRIANGLE_AREA_ON_LINE_RIGHTʱ����ʾ������ֱ���Ҳࣻ����ΪTRIANGLE_AREA_ON_LINE_LEFTʱ,��ʾ����������ࡣ
//����ȱʡ����Ϊ�����ڱߵ��Ҳࡣ
void CBTriangle_DealEdge(int x1, int y1, int x2, int y2, float z1, float z2,
    FRGBA clr1, FRGBA clr2, HDDC hDC, ZBuffer* pZB,
    int* pnCoord, FRGBA* pColor, float* pfDepth, DWORD dwFeature/*=G3D_TRIANGLE_ON_LINE_RIGHT*/
                        )
{
  //������ĳ���Ӧ����y�����ľ���ֵ��1,
  ASSERT(pnCoord);
  ASSERT(pColor);
  ASSERT(pfDepth);

  //����һ��ˮƽֱ�ߣ���򵥵ط��ص�һ������������ɫ
  //����һ��ˮƽֱ�ߣ����ǲ�������������������ҹ�ϵ
  if (y1 == y2) {
    pnCoord[0] = x1;
    pColor[0] = clr1;
    pfDepth[0] = z1;
    return;
  }

  //���濼��ֱ��б�ʲ�Ϊ0�����:
  {
    int i;
    //���˵���ˮƽƫ�����ʹ�ֱƫ����
    int nDx = x2 - x1;
    int nDy = y2 - y1;
    //���˵���ˮƽ����ʹ�ֱ����
    int nIx = ABS(nDx);
    int nIy = ABS(nDy);
    //��㲽��(������ֵ)
    int nInc = MAX(nIx, nIy);
    //�����ж��Ƿ���nJudgeX,nJudgeY��������ǰ��
    int nJudgeX = -nIy, nJudgeY = -nIx;
    //ͨ����������õ��ĵ�ǰ��
    int x = x1, y = y1;
    //Bresenham�㷨
    int nTwoIx = 2 * nIx, nTwoIy = 2 * nIy;
    //ֱ�ߵ�ǰ�����ɫ
    float fPlotR = clr1.r, fPlotG = clr1.g, fPlotB = clr1.b;
    float fPlotA = clr1.a;
    //��ɫ����
    float fIncR = (clr2.r - fPlotR) / nInc;
    float fIncG = (clr2.g - fPlotG) / nInc;
    float fIncB = (clr2.b - fPlotB) / nInc;
    float fIncA = (clr2.a - fPlotA) / nInc;
    //ˮƽ�߽�������յ�
    FRGBA clrStart = {fPlotR, fPlotG, fPlotB, fPlotA};
    FRGBA clrEnd = clrStart;
    //�����±꣬ˮƽ�߽�������յ�� x ����
    int nIndex = 0, nStartX = x1, nEndX = x1;
    //ˮƽ�߽�������յ�� z ����,��ǰ��� z ����
    //�Լ� z ��������
    float fStartZ = z1, fEndZ = z1, z = z1, fIncZ = (z2 - z1) / nInc;
    //�������ֵ���Ƶ�һ����
    //�������е����
    float zInBuffer = GetDepth(pZB, x1, y1);

    //�����ǰ���z����С����Ȼ������е���ͬλ��z������
    if (z < zInBuffer) {
      //����ɫ�������л��Ƶ�һ����
      CBSetPixel(hDC, x1, y1, fPlotR, fPlotG, fPlotB, fPlotA);
      //������Ȼ�������ֵ
      SetDepth(pZB, x1, y1, z);
    }

    //��ʼ��������
    for (i = 0; i <= nInc; i++) {
      //ͨ������������ĵ�ǰ���Ƿ�����ֱ���ϵĵ�
      BOOL bRecord = FALSE;
      //��ǰ���Ƿ�Ϊֱ���ϵĵ�
      BOOL bPlot = FALSE;
      nJudgeX += nTwoIx;
      nJudgeY += nTwoIy;

      //�ȼ�� y ����
      if (nJudgeY >= 0) {
        bPlot = TRUE;
        bRecord = TRUE;
        nJudgeY -= nTwoIx;

        //�����������ֱ��ͳһ����
        if (nDy > 0) {
          nEndX = x;
          y++;
          //ˮƽ�߽���յ����ɫ
          clrEnd.r = fPlotR;
          clrEnd.g = fPlotG;
          clrEnd.b = fPlotB;
          clrEnd.a = fPlotA;
          //ˮƽ�߽���յ�����(�ر߽��в�ֵ��������)
          fEndZ = z;
        }
        else if (nDy < 0) {
          nEndX = x;
          y--;
          //ˮƽ�߽���յ����ɫ
          clrEnd.r = fPlotR;
          clrEnd.g = fPlotG;
          clrEnd.b = fPlotB;
          clrEnd.a = fPlotA;
          //ˮƽ�߽���յ�����(�ر߽��в�ֵ��������)
          fEndZ = z;
        }
      }//end if

      //���� x ����
      if (nJudgeX >= 0) {
        bPlot = TRUE;
        nJudgeX -= nTwoIy;

        //�����������ֱ��ͳһ����
        if (nDx > 0) {
          x++;
        }
        else if (nDx < 0) {
          x--;
        }
      }

      if (bPlot) {
        fPlotR += fIncR;
        fPlotG += fIncG;
        fPlotB += fIncB;
        fPlotA += fIncA;
        z += fIncZ;

        if (i < (nInc - 1)) {
          //�������ֵ���Ƶ�һ����
          zInBuffer = GetDepth(pZB, x, y);

          //�����ǰ���z����С����Ȼ������е���ͬλ��z������
          if (z < zInBuffer) {
            //����ɫ�������л��Ƶ�һ����
            CBSetPixel(hDC, x, y, fPlotR, fPlotG, fPlotB, fPlotA);
            //������Ȼ�������ֵ
            SetDepth(pZB, x, y, z);
          }
        }//end if(i < (nInc - 1))
      }//end if(bPlot)

      //�����ǰ����ֱ���ϣ����¼��ǰ��
      if (bRecord) {
        //ȡ����ߵ�,���������
        if (dwFeature == G3D_TRIANGLE_ON_LINE_LEFT) {
          //ȡx������С��
          if (nStartX < nEndX) {
            pnCoord[nIndex] = nStartX;
            pColor[nIndex] = clrStart;
            pfDepth[nIndex] = fStartZ;
          }
          else {
            pnCoord[nIndex] = nEndX;
            pColor[nIndex] = clrEnd;
            pfDepth[nIndex] = fEndZ;
          }
        }//end if
        //ȡ���ұߵ㣬�������Ҳ�
        else {
          //ȡx���������
          if (nStartX < nEndX) {
            pnCoord[nIndex] = nEndX;
            pColor[nIndex] = clrEnd;
            pfDepth[nIndex] = fEndZ;
          }
          else {
            pnCoord[nIndex] = nStartX;
            pColor[nIndex] = clrStart;
            pfDepth[nIndex] = fStartZ;
          }
        }//end else

        nIndex++;
        //��һ�����
        nStartX = x;
        //��һ������ z ����
        fStartZ = z;
        //��һ��������ɫ
        clrStart.r = fPlotR;
        clrStart.g = fPlotG;
        clrStart.b = fPlotB;
        clrStart.a = fPlotA;
      }//end if(bRecord)
    }//end for

    //��б�ʵĺ�Сʱ,y������������,��ʱ��Ҫ�ر���
    if (nIndex < (ABS(y2 - y1) + 1)) {
      //ȡ����ߵ�,���������
      if (dwFeature == G3D_TRIANGLE_ON_LINE_LEFT) {
        //ȡx������С��
        if (nStartX < x2) {
          pnCoord[nIndex] = nStartX;
          pColor[nIndex] = clrStart;
          pfDepth[nIndex] = fStartZ;
        }
        else {
          pnCoord[nIndex] = x2;
          pColor[nIndex] = clr2;
          pfDepth[nIndex] = z2;
        }
      }//end if
      //ȡ���ұߵ�,�������Ҳ�
      else {
        //ȡx���������
        if (nStartX < x2) {
          pnCoord[nIndex] = x2;
          pColor[nIndex] = clr2;
          pfDepth[nIndex] = z2;
        }
        else {
          pnCoord[nIndex] = nStartX;
          pColor[nIndex] = clrStart;
          pfDepth[nIndex] = fStartZ;
        }
      }//end else
    }//end if(nIndex < (ABS(y2 - y1) + 1))
  }
}
//��ɫһ��ɨ����,ɨ�������x����Ϊx1��x2, y����ΪnScanLine,
//z1,z2         -�˵����ֵ
//clr1��clr2�ֱ�Ϊ�����յ����ɫ.
//hDC,pZB       �ֱ�Ϊ��ɫ�������Ȼ���
void CBTriangle_Scan(int x1, int x2, int nScanLine, float z1, float z2, FRGBA clr1, FRGBA clr2, HDDC hDC, ZBuffer* pZB)
{
  //������ظ���
  int nFillPixels = ABS(x2 - x1);

  //���Ϊͬһ�����������Ϊ0,�򷵻�;
  if (nFillPixels < 2) {
    return;
  }

  {
    int i;
    //��ɫ����(��ɫ��ֵ / ����)
    float fIncR = (clr2.r - clr1.r) / nFillPixels;
    float fIncG = (clr2.g - clr1.g) / nFillPixels;
    float fIncB = (clr2.b - clr1.b) / nFillPixels;
    float fIncA = (clr2.a - clr1.a) / nFillPixels;
    //��ǰ��ɫ
    float fRed = clr1.r + fIncR;
    float fGreen = clr1.g + fIncG;
    float fBlue = clr1.b + fIncB;
    float fAlpha = clr1.a + fIncA;
    //��䷽��
    int nDirection = (x1 < x2) ? 1 : -1;
    //��ǰx����
    int x = x1 + nDirection;
    //��ǰ z ����, z��������,
    //ע�� ���ǲ�û�в���ƽ��ϵ�����������
    float z = z1, fIncZ = (z2 - z1) / nFillPixels;
    z += fIncZ;
    nFillPixels--;

    //��ʼ���
    for (i = 0; i < nFillPixels ; i++) {
      float zInBuffer = GetDepth(pZB, x, nScanLine);

      //�����ǰ���z����С����Ȼ������е���ͬλ��z������
      if (z < zInBuffer) {
        //����ɫ�������л��Ƶ�һ����
        CBSetPixel(hDC, x, nScanLine, fRed, fGreen, fBlue, fAlpha);
        //������Ȼ�������ֵ
        SetDepth(pZB, x, nScanLine, z);
      }

      //��������
      x += nDirection;
      z += fIncZ;
      fRed += fIncR;
      fGreen += fIncG;
      fBlue += fIncB;
      fAlpha += fIncA;
    }
  }
}

//����Gouraud������ɫ�������������
//ɨ��ת��������
//pt1,pt2,pt3      �����ζ�������
//z1, z2,z3      --�����ζ������
//clr1,clr2,clr3   �����ζ�����ɫ
//hDC,pZB         -��ɫ����������Ȼ�����
void CBTriangle_Draw(HDDC hDC, ZBuffer* pZB, Vertex3d* v1, Vertex3d* v2, Vertex3d* v3)
{
  POINT pt1 = v1->PtOnScrn, pt2 = v2->PtOnScrn, pt3 = v3->PtOnScrn;
  float z1 = v1->vInView.z, z2 = v2->vInView.z, z3 = v3->vInView.z;
  FRGBA clr1 = v1->clr, clr2 = v2->clr, clr3 = v3->clr;
  float zInBuffer;
  POINT point1, point2, point3;
  FRGBA color1, color2, color3;
  float fz1, fz2, fz3;
  int nDy12, nDy13, nDy32, *pnEdge12, *pnEdge13, *pnEdge32, nDelta;
  FRGBA* pClrEdge12, *pClrEdge13, *pClrEdge32;
  float* pfzEdge12, *pfzEdge13, *pfzEdge32;
  int y, nPlumbScan;
  int nPlumbScan32;
  ASSERT(hDC);
  ASSERT(pZB);

  //��һ���ִ���:�����˻�����е�һ�����
  //�����˻����(������һ��ˮƽֱ����)
  //���� z-buffer�㷨�����˳��һ����������޹ص�,
  //����,��������һ��ɨ������ʱ,��Ȼ�б�Ҫ��������
  if ((pt1.y == pt2.y) && (pt2.y == pt3.y)) {
    //pt1λ���м�(����ʸ����������ж�)
    if ((pt2.x - pt1.x) * (pt3.x - pt1.x) <= 0) {
      CBTriangle_Scan(pt1.x, pt2.x, pt1.y, z1, z2, clr1, clr2, hDC, pZB);
      CBTriangle_Scan(pt1.x, pt3.x, pt1.y, z1, z3, clr1, clr3, hDC, pZB);
    }
    //pt2λ���м�
    else if ((pt1.x - pt2.x) * (pt3.x - pt2.x) <= 0) {
      CBTriangle_Scan(pt2.x, pt1.x, pt1.y, z2, z1, clr2, clr1, hDC, pZB);
      CBTriangle_Scan(pt2.x, pt3.x, pt1.y, z2, z3, clr2, clr3, hDC, pZB);
    }
    //pt3λ���м�
    else {
      CBTriangle_Scan(pt3.x, pt1.x, pt1.y, z3, z1, clr3, clr1, hDC, pZB);
      CBTriangle_Scan(pt3.x, pt2.x, pt1.y, z3, z2, clr3, clr2, hDC, pZB);
    }

    //���������˵�
    zInBuffer = GetDepth(pZB, pt1.x, pt1.y);

    //�����ǰ���z����С����Ȼ������е���ͬλ��z������
    if (z1 < zInBuffer) {
      //����ɫ�������л��Ƶ�һ����
      CBSetPixel(hDC, pt1.x, pt1.y, clr1.r, clr1.g, clr1.b, clr1.a);
      //������Ȼ�������ֵ
      SetDepth(pZB, pt1.x, pt1.y, z1);
    }

    //
    zInBuffer = GetDepth(pZB, pt2.x, pt2.y);

    //�����ǰ���z����С����Ȼ������е���ͬλ��z������
    if (z2 < zInBuffer) {
      //����ɫ�������л��Ƶ�һ����
      CBSetPixel(hDC, pt2.x, pt2.y, clr2.r, clr2.g, clr2.b, clr2.a);
      //������Ȼ�������ֵ
      SetDepth(pZB, pt2.x, pt2.y, z2);
    }

    zInBuffer = GetDepth(pZB, pt3.x, pt3.y);

    //�����ǰ���z����С����Ȼ������е���ͬλ��z������
    if (z3 < zInBuffer) {
      //����ɫ�������л��Ƶ�һ����
      CBSetPixel(hDC, pt3.x, pt3.y, clr3.r, clr3.g, clr3.b, clr3.a);
      //������Ȼ�������ֵ
      SetDepth(pZB, pt3.x, pt3.y, z3);
    }

    return;
  }

  //�ڶ����ִ���:����һ�����
  //��һ��,����:
  //�������:��һ�����y������С,�ڶ������y�������,����������λ�����ǵ��м�
  //�������ɫ����
  point1 = pt1, point2 = pt2, point3 = pt3;
  color1 = clr1, color2 = clr2, color3 = clr3;
  fz1 = z1, fz2 = z2, fz3 = z3;

  //point1��¼y������С��
  if (point1.y > point2.y) {
    point1 = pt2;
    point2 = pt1;
    color1 = clr2;
    color2 = clr1;
    fz1 = z2;
    fz2 = z1;
  }

  if (point1.y > point3.y) {
    POINT ptTemp = point1;
    FRGBA clrTemp = color1;
    float zTemp = fz1;
    point1 = point3;
    color1 = color3;
    fz1 = fz3;
    point3 = ptTemp;
    color3 = clrTemp;
    fz3 = zTemp;
  }

  //point2��¼y���������
  if (point2.y < point3.y) {
    POINT ptTemp = point2;
    FRGBA clrTemp = color2;
    float zTemp = fz2;
    point2 = point3;
    color2 = color3;
    fz2 = fz3;
    point3 = ptTemp;
    color3 = clrTemp;
    fz3 = zTemp;
  }

  //�ڶ���,��ȡ�߽��� x ����,����߽���ɫ
  //12     1 to 2, 13    - 1 to 3, 32    - 3 to 2
  //y����֮��ľ���
  nDy12 = (point2.y - point1.y) + 1;
  nDy13 = (point3.y - point1.y) + 1;
  nDy32 = (point2.y - point3.y) + 1;
  //��ȡɨ���ߵ� x ����
  pnEdge12 = MALLOC(int, nDy12);
  pnEdge13 = MALLOC(int, nDy13);
  pnEdge32 = MALLOC(int, nDy32);
  //��ȡ�߽���ɫ
  //ע��,����֧��͸������,�����ɫ����������
  pClrEdge12 = MALLOC(FRGBA, nDy12);
  pClrEdge13 = MALLOC(FRGBA, nDy13);
  pClrEdge32 = MALLOC(FRGBA, nDy32);
  //��¼�߽����
  pfzEdge12 = MALLOC(float, nDy12);
  pfzEdge13 = MALLOC(float, nDy13);
  pfzEdge32 = MALLOC(float, nDy32);
  //��ȡ�߽缰����ɫ
  //����ʸ�����(ֻ��Ҫ����Z����),���ж������ε�λ�ù�ϵ
  nDelta = ((point3.x - point1.x) * (point2.y - point3.y) - (point2.x - point3.x) * (point3.y - point1.y));

  if (nDelta > 0) {
    CBTriangle_DealEdge(point1.x, point1.y, point2.x, point2.y, fz1, fz2, color1, color2, hDC, pZB, pnEdge12, pClrEdge12, pfzEdge12, G3D_TRIANGLE_ON_LINE_RIGHT);
    CBTriangle_DealEdge(point1.x, point1.y, point3.x, point3.y, fz1, fz3, color1, color3, hDC, pZB, pnEdge13, pClrEdge13, pfzEdge13, G3D_TRIANGLE_ON_LINE_LEFT);
    CBTriangle_DealEdge(point3.x, point3.y, point2.x, point2.y, fz3, fz2, color3, color2, hDC, pZB, pnEdge32, pClrEdge32, pfzEdge32, G3D_TRIANGLE_ON_LINE_LEFT);
  }
  else {
    CBTriangle_DealEdge(point1.x, point1.y, point2.x, point2.y, fz1, fz2, color1, color2, hDC, pZB, pnEdge12, pClrEdge12, pfzEdge12, G3D_TRIANGLE_ON_LINE_LEFT);
    CBTriangle_DealEdge(point1.x, point1.y, point3.x, point3.y, fz1, fz3, color1, color3, hDC, pZB, pnEdge13, pClrEdge13, pfzEdge13, G3D_TRIANGLE_ON_LINE_RIGHT);
    CBTriangle_DealEdge(point3.x, point3.y, point2.x, point2.y, fz3, fz2, color3, color2, hDC, pZB, pnEdge32, pClrEdge32, pfzEdge32, G3D_TRIANGLE_ON_LINE_RIGHT);
  }

  //��֤���һ����Ҳ�ܻ���
  //������Ϊ,CPLine��Ԫ��Windows����ֱ�ߵķ�ʽ��һ�µ�,�����������һ����
  zInBuffer = GetDepth(pZB, point2.x, point2.y);

  //�����ǰ���z����С����Ȼ������е���ͬλ��z������
  if (fz2 < zInBuffer) {
    //����ɫ�������л��Ƶ�һ����
    CBSetPixel(hDC, point2.x, point2.y, color2.r, color2.g, color2.b, color2.a);
    //������Ȼ�������ֵ
    SetDepth(pZB, point2.x, point2.y, fz2);
  }

  //����������
  if (point3.y == point2.y) {
    zInBuffer = GetDepth(pZB, point3.x, point3.y);

    //�����ǰ���z����С����Ȼ������е���ͬλ��z������
    if (fz3 < zInBuffer) {
      //����ɫ�������л��Ƶ�һ����
      CBSetPixel(hDC, point3.x, point3.y, color3.r, color3.g, color3.b, color3.a);
      //������Ȼ�������ֵ
      SetDepth(pZB, point3.x, point3.y, fz3);
    }
  }

  //������,����ɨ�����
  //ɨ���Ϊ�����׶�:
  //��һ,ɨ���һ����������֮�������
  //�ڶ�,ɨ����������ڶ���֮�������
  //��ǰɨ���ߵ�y���꼰ɨ�������
  nPlumbScan = 0;

  if (point1.y < point3.y) {
    for (y = point1.y; y < point3.y; y++) {
      //һ��ɨ���ߵ�x����
      int x12 = pnEdge12[nPlumbScan];
      int x13 = pnEdge13[nPlumbScan];
      //һ��ɨ���ߵ���ȱ仯
      float z12 = pfzEdge12[nPlumbScan];
      float z13 = pfzEdge13[nPlumbScan];
      //��ɫ
      FRGBA clrEdge12 = pClrEdge12[nPlumbScan];
      FRGBA clrEdge13 = pClrEdge13[nPlumbScan];
      //���
      CBTriangle_Scan(x12, x13, y, z12, z13, clrEdge12, clrEdge13, hDC, pZB);
      nPlumbScan++;
    }//end for y
  }//end (point1.y < point3.y)

  //�ڶ��׶�ɨ��
  //׼������
  //�ر�3-2�ļ�����
  nPlumbScan32 = 0;

  for (y = point3.y; y <= point2.y; y++) {
    //һ��ɨ���ߵ�x����
    int x12 = pnEdge12[nPlumbScan];
    int x32 = pnEdge32[nPlumbScan32];
    //һ��ɨ���ߵ���ȱ仯
    float z12 = pfzEdge12[nPlumbScan];
    float z32 = pfzEdge32[nPlumbScan32];
    //��ɫ
    FRGBA clrEdge12 = pClrEdge12[nPlumbScan];
    FRGBA clrEdge32 = pClrEdge32[nPlumbScan32];
    //���
    CBTriangle_Scan(x12, x32, y, z12, z32, clrEdge12, clrEdge32, hDC, pZB);
    nPlumbScan++;
    nPlumbScan32++;
  }//end for y

  free(pnEdge12);
  free(pnEdge13);
  free(pnEdge32);
  free(pClrEdge12);
  free(pClrEdge13);
  free(pClrEdge32);
  free(pfzEdge12);
  free(pfzEdge13);
  free(pfzEdge32);
}
//�����߿�ģ��
//pO  ��ά�������
//hDC   ��ɫ����
//pZB   ��Ȼ���
void FlatWirize(HDDC hDC, ZBuffer* pZB, Object3d* pO)
{
  int i, j;

  //���ƹ���:ֱ�߻�Ԫ
  //���Ƶ�ɫģ��
  for (i = 0; i < pO->sListLen; i++) {
    //��ȡ�ߵĶ�������
    for (j = 0; j < 3; ++j) {
      int j1 = (j == 2) ? 0 : (j + 1);
      int n1 = pO->sList[i].vIndex[j];
      int n2 = pO->sList[i].vIndex[j1];
      Vertex3d v1 = pO->vList[n1];
      Vertex3d v2 = pO->vList[n2];

      if (n1 > n2) {
        continue;
      }

      if (pO->bAutoColor) {
        //��ȡ��һ���������ɫ���������ߵ���ɫ
        v1.clr = v2.clr = v1.clr;
      }
      else {
        v1.clr = v2.clr = pO->sList[i].clr;
      }

      //����
      CBLine_Draw(hDC, pZB, &v1, &v2);
    }
  }//end for
}
//����ʵ��ģ��, Flatģ��
//pO          ��ά�������
//hDC         ��ɫ����
//pZB         ��Ȼ���
void Flat(HDDC hDC, ZBuffer* pZB, Object3d* pO)
{
  int i;

  //����ƹ���: �����λ�Ԫ
  //�ɶ������С�����ɫ, �������ⲿ����С�����ɫ
  //����, ��ѭ��
  for (i = 0; i < pO->sListLen; i++) {
    //��ȡ����������
    int n1 = pO->sList[i].vIndex[0];
    int n2 = pO->sList[i].vIndex[1];
    int n3 = pO->sList[i].vIndex[2];
    Vertex3d v1 = pO->vList[n1];
    Vertex3d v2 = pO->vList[n2];
    Vertex3d v3 = pO->vList[n3];

    //С����ɫ
    FRGBA rgba;

    //��ȡ������ɫ
    if (pO->bAutoColor) {
      //һ���, ���õ�һ���������ɫ����ʾС�����ɫ
      //���ģ�͵Ķ��㰲��Ҫ��Ƚϸ�
      //���, ���ǲ�������������ɫ��ƽ��ֵ����ʾС�����ɫ
      //�ڹ���������, ��������Ҳ�ǿ��Խ��ܵ�
      //������ɫ��һ��Ӧ�����ⲿ����
      rgba.r = (v1.clr.r + v2.clr.r + v3.clr.r) / 3.0f;
      rgba.g = (v1.clr.g + v2.clr.g + v3.clr.g) / 3.0f;
      rgba.b = (v1.clr.b + v2.clr.b + v3.clr.b) / 3.0f;
      //͸����, ����Ҳ�򵥵ش����ͬһֵ
      rgba.a = (v1.clr.a + v2.clr.a + v3.clr.a) / 3.0f;
    }
    //��ȡС����ɫ
    else {
      rgba = pO->sList[i].clr;
    }

    v1.clr = v2.clr = v3.clr = rgba;
    //����
    CBTriangle_Draw(hDC, pZB, &v1, &v2, &v3);

  }//end for
}
//����ʵ��ģ��, Gouraudģ��
//pO          ��ά�������
//hDC         ��ɫ����
//pZB         ��Ȼ���
int Gouraud1(HDDC hDC, ZBuffer* pZB, Object3d* pO)
{
  int i;

  //����ƹ���: �����λ�Ԫ
  //���ƣ���ѭ��
  for (i = 0; i < pO->sListLen; i++) {
    //��ȡ����������
    int n1 = pO->sList[i].vIndex[0];
    int n2 = pO->sList[i].vIndex[1];
    int n3 = pO->sList[i].vIndex[2];
    //����
    CBTriangle_Draw(hDC, pZB, pO->vList + n1, pO->vList + n2, pO->vList + n3);
  }//end for

  return 0;
}