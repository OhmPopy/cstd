
//#define Add(ARR, X)  ((ARR)[ARR##Len++]=(X))

// ��ת��
int SetObjColor(Object3d* pO, FRGBA clr)
{
  int i;

  for (i = 0; i < pO->vListLen; i++) {
    pO->vList[i].clr = clr;
  }

  return 0;
}
// ��ת��
int initRevolvingSolid(Object3d* pO, int nSlices, int nStacks, FRGBA clr)
{
  int i, j, k = 0;
  int nIndexFacet, nIndexVertex;

  for (i = 0; i < pO->vListLen; i++) {
    int j = (i) % nSlices;
    //���ȵ�����, ���ȱ�ʾ
    float fLongitude = -4.712389f + (float)((j) * 2.0 * PIE / nSlices);
    float x = (float)(pO->vList[i].coord.x * cos(fLongitude));
    float y = (float)(pO->vList[i].coord.y * sin(fLongitude));
    float z = pO->vList[i].coord.z;
    //��д�������
    V3SET(pO->vList[i].coord, x, y, z), ++k;
    pO->vList[i].clr = clr;
  }

  //i��γ��ѭ��,  j������ѭ��
  k = 0;

  //����λ�ڱ���Ȧ�ڵ���:
  for (j = 0; j < nSlices; j++) {
    int j1 = j == (nSlices - 1) ? 0 : j + 1;
    A3SET(pO->sList[k].vIndex, 0, j + 1, j1 + 1), ++k;
  }

  //��������������С��
  for (i = 0; i < (nStacks - 2); i++) {
    //С��λ����γ��֮��,
    //��γ��(ͶӰ)�� y �ύ�����
    int nIndexUp = i * nSlices + 1;
    //��γ��(ͶӰ)�� y �ύ�����
    int nIndexDown = nIndexUp + nSlices;

    //������ѭ��
    for (j = 0; j < nSlices; j++) {
      int j1 = j == (nSlices - 1) ? 0 : j + 1;
      //DefineTopology(&pO->sList[nIndexDown + j - 1], anVertices,  4);
      A3SET(pO->sList[k].vIndex, nIndexUp + j, nIndexDown + j, nIndexDown + j1), ++k;
      A3SET(pO->sList[k].vIndex, nIndexUp + j, nIndexDown + j1, nIndexUp + j1), ++k;
    }//end for j
  }//end for i;

  //�����ϼ�Ȧ�ڵ�С��
  //С����Ż���
  nIndexFacet = (nStacks - 1) * nSlices;
  //������Ż���
  nIndexVertex = nSlices * (nStacks - 2) + 1;

  for (j = 0; j < nSlices; j++) {
    int j1 = j == (nSlices - 1) ? 0 : j + 1;
    A3SET(pO->sList[k].vIndex, nIndexVertex + j, nIndexFacet  + 1, nIndexVertex + j1), ++k;
  }

  assert(k == pO->sListLen);

  return 0;
}
//float fRadius; ��İ뾶
//int nSlices;  ��ľ����������,��ƾ�����, (���������������)
//int nStacks;  ���γ�ȴ�״��������,���γ����, (γ������������ 1)
//��ʼ������ļ���(����ͷ���)
//��ʼ��С�漸��(����)
int initSphere(Object3d* pO, int n, float fRadius, int nSlices, int nStacks, FRGBA clr)
{
  int n0 = n;
  int i, j, k = 0;
  float x, y, z;
  fRadius = ABS(fRadius);
  //���ٲ���С��8, ���������
  nSlices = MAX(nSlices, 4);
  nStacks = MAX(nStacks, 4);


  for (; n--; pO++) {
    k = 0;
    // γ���Ա������ϼ�����, ����������������ֱ�γ����γ
    // ���ȴ�y������ʼ, ����ʱ�뷽������, �����ֶ���������.
    // ��������
    //��ʼ����̬����Ĵ�С.
    initObject3d(pO, (nSlices * (nStacks - 1) + 2), (2 * nSlices * (nStacks - 1)), 0);
    V3SET(pO->vList[k].coord, 0.0f, 0.0f, fRadius), ++k;

    //������ȱʡֵ��ͬ
    //��γ��ѭ��
    for (i = 0; i < (nStacks - 1); i++) {
      //γ��
      float fLatitude = (float)((i + 1) * PIE / nStacks);
      //���� z ����
      z = fRadius * (float)cos(fLatitude);
      x = fRadius * (float)sin(fLatitude);
      y = x;

      //������ѭ��
      for (j = 0; j < nSlices; j++) {
        V3SET(pO->vList[k].coord, x, y, z), ++k;
      }
    }

    // �����ϼ���
    V3SET(pO->vList[k].coord, 0.0f, 0.0f, -fRadius), ++k;
    assert(k == pO->vListLen);
    initRevolvingSolid(pO, nSlices, nStacks, clr);
  }

  return n0;
}
// Բ׵�塢Բ����
int initCylinder(Object3d* pO, int n, float fRadius1, float fRadius2, float fH, int nSlices, int nStacks, FRGBA clr)
{
  int n0 = n;
  int i, j, k = 0;
  float x, y, z;
  float zoff = 0.f;
  fRadius1 = ABS(fRadius1);
  fRadius2 = ABS(fRadius2);
  nSlices = MAX(nSlices, 3);
  nStacks = MAX(nStacks, 3);

  for (; n--; pO++) {
    //��ʼ����̬����Ĵ�С.
    initObject3d(pO, (nSlices * (nStacks - 1) + 2), (2 * nSlices * (nStacks - 1)), 0);
    k = 0;
    V3SET(pO->vList[k].coord, 0.0f, 0.0f, fH + zoff), ++k;

    for (i = 0; i < (nStacks - 1); i++) {
      float fLatitude = (float)((i + 1) * PIE / nStacks);
      z = fH * (nStacks - 2 - i) / (nStacks - 2) + zoff;
      x = (fRadius1 * (nStacks - 2 - i) + fRadius2 * i) / (nStacks - 2);
      y = x;

      for (j = 0; j < nSlices; j++) {
        V3SET(pO->vList[k].coord, x, y, z), ++k;
      }
    }

    V3SET(pO->vList[k].coord, 0.0f, 0.0f, 0.f + zoff), ++k;
    assert(k == pO->vListLen);
    initRevolvingSolid(pO, nSlices, nStacks, clr);
  }

  return n0;
}

// ��ȡoff�ļ�
int loadObject_OFF(Object3d* pO, int n, LPTSTR lpszFileName)
{
  int i, iTempB = 0, len, iAugmentNum, n0 = n, sListLen, vListLen;
  char* buf = (char*)loadfile(lpszFileName, &len);

  if (0 != strncmp("OFF", buf, 3)) {
    free(buf);
    return 0;
  }

  for (; n--; pO++) {
    float x, y, z;
    char* szTemp = buf;
    NEXTLINE(szTemp);
    sscanf(szTemp, "%d %d %d\n", &vListLen, &sListLen, &iTempB);
    NEXTLINE(szTemp);
    pO->maxRadius = 0;

    initObject3d(pO, vListLen, sListLen * 4 / 3 + 3, 0);

    for (i = 0 ; i < pO->vListLen ; i++) {
      float max_vertex;
      sscanf(szTemp, "%f %f %f\n", &x, &y, &z);
      V3SET(pO->vList[i].coord, x, y, z);
      max_vertex = MAX(ABS(x), MAX(ABS(y), ABS(z)));

      if (pO->maxRadius < max_vertex) {
        pO->maxRadius = max_vertex;
      }

      NEXTLINE(szTemp);
    }

    iAugmentNum = 0;

    for (i = 0 ; i < sListLen ; i++) {
      int iTemp = 0;
      int iT1 = 0 , iT2 = 0 , iT3 = 0 , iT4 = 0;
      sscanf(szTemp, "%d %d %d %d\n", &iTemp, &iT1, &iT2, &iT3, &iT4);

      if (iTemp == 3) {
        //����Ƭ����Ŀ����1��
        A3SET(pO->sList[iAugmentNum].vIndex, iT1, iT2, iT3);
        iAugmentNum++;
      }
      else if (iTemp == 4) {
        //��4������ɵ���Ƭ�ֳ�����3������ɵ���Ƭ,����������ģ�͵���Ƭ����
        //����Ƭ����Ŀ����2��
        A3SET(pO->sList[iAugmentNum].vIndex, iT1, iT2, iT3);
        iAugmentNum++;
        A3SET(pO->sList[iAugmentNum].vIndex, iT1, iT3, iT4);
        iAugmentNum++;
      }
      else {
        //��n������ɵ���Ƭ�ֳ�����3������ɵ���Ƭ
        //����������ģ�͵���Ƭ����
        //("�������ɵ���Ƭ�����ڻ����ܴ�������");
        assert(0);
        return FALSE;
      }

      NEXTLINE(szTemp);
    }

    //��������ģ�͵���Ƭ����
    //������Ƭ�����Ĵ洢
    pO->sListLen = iAugmentNum;
  }

  free(buf);
  return n0;
}


// ��ȡoff�ļ�
int loadObject_TM(Object3d* pO, int n, LPTSTR lpszFileName)
{
  int i, len, iAugmentNum, n0 = n;
  char* buf = (char*)loadfile(lpszFileName, &len);

  for (; n--; pO++) {
    float x, y, z;
    char* szTemp = buf;
    sscanf(szTemp, "%d %d\n", &pO->vListLen, &pO->sListLen);
    NEXTLINE(szTemp);
    pO->maxRadius = 0;

    initObject3d(pO, pO->vListLen, pO->sListLen, 0);

    for (i = 0 ; i < pO->vListLen ; i++) {
      float max_vertex;
      sscanf(szTemp, "%f %f %f\n", &x, &y, &z);
      V3SET(pO->vList[i].coord, x, y, z);
      max_vertex = MAX(ABS(x), MAX(ABS(y), ABS(z)));

      if (pO->maxRadius < max_vertex) {
        pO->maxRadius = max_vertex;
      }

      NEXTLINE(szTemp);
    }

    iAugmentNum = 0;

    for (i = 0 ; i < pO->sListLen ; i++) {
      int iT1 = 0 , iT2 = 0 , iT3 = 0 , iT4 = 0;
      sscanf(szTemp, "%d %d %d\n", &iT1, &iT2, &iT3, &iT4);
      //����Ƭ����Ŀ����1��
      A3SET(pO->sList[iAugmentNum].vIndex, iT1, iT2, iT3);
      iAugmentNum++;
      NEXTLINE(szTemp);
    }
  }

  free(buf);
  return n0;
}

