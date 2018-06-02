typedef struct Material {
  //���ʶԻ�����ķ�����(reflectivity)
  FRGBA refAmb;
  //���ʶ��������ķ�����
  FRGBA refDif;
  //���ʶԾ���߹�ķ�����
  FRGBA refSpe;
  //���ʵ���������ɫ(Emission)
  FRGBA clrEmi;
  //�������ȣ��߹�ָ����
  float fShininess;
  //�������,ǰ��,����,����ǰ������(˫��)
  DWORD dwFaceType;
} Material;
int initMaterial(Material* pM, int n)
{
  for (; n-- > 0; ++pM) {
    //ȱʡ����
    FRGBASET(pM->refAmb, 0.2f, 0.2f, 0.2f, 1.0f);
    FRGBASET(pM->refDif, 0.8f, 0.8f, 0.8f, 1.0f);
    FRGBASET(pM->refSpe, 0.0f, 0.0f, 0.0f, 1.0f);
    FRGBASET(pM->clrEmi, 0.0f, 0.0f, 0.0f, 1.0f);
    pM->fShininess = 128.0f;
    pM->dwFaceType = G3D_FRONT;
  }

  return 0;
}
//���ù�����
//dwFace-------������, ȡö��ֵ
//dwProperty---����, ȡ G3D_SHININESS Ϊ��Ч
//fParam-------�������ֵ
void Materialf(Material* pM, DWORD dwFace,  DWORD dwProperty,  float fParam)
{
  if (dwProperty == G3D_SHININESS) {
    pM->dwFaceType = dwFace;
    pM->fShininess = (fParam < 0.0f) ? 0.0f : ((fParam > 128.0f) ? 128.0f : fParam);
  }
}
//������������
//dwFace-------������, ȡö��ֵ
//dwProperty---����,
//pfParams-----�������ֵ
void Materialfv(Material* pM, DWORD dwFace,  DWORD dwProperty,  const float* pfParams)
{
  ASSERT(pfParams);
  pM->dwFaceType = dwFace;

  switch (dwProperty) {
    //�����ⷴ��ϵ��
  case G3D_AMBIENT:
    FRGBASETv(pM->refAmb, pfParams);
    break;

    //������ⷴ��ϵ��
  case G3D_DIFFUSE:
    FRGBASETv(pM->refDif, pfParams);
    break;

    //����߹ⷴ��ϵ��
  case G3D_SPECULAR:
    FRGBASETv(pM->refSpe, pfParams);
    break;

    //������������ϵ����ͬ
  case G3D_AMBIENT_AND_DIFFUSE:
    FRGBASETv(pM->refAmb, pfParams);
    FRGBASETv(pM->refDif, pfParams);
    break;

    //��������ɫ
  case G3D_EMISSION:
    FRGBASETv(pM->clrEmi, pfParams);
    break;
  }
}
typedef struct _LightObj {
  //��Ļ�����ɫ
  FRGBA clrAmb;
  //�����������ɫ
  FRGBA clrDif;
  //��ľ���߹���ɫ
  FRGBA clrSpe;
  //��Դλ��
  VECTOR3D hcPosition;
  //�۹����䷽��:
  VECTOR3D  spotDirection;
  //�۹�ָ��
  float fSpotExp;
  //�۹ⷢɢ���
  float fSpotCutoff;
  //����˥��ϵ��
  float fAttCoef0;
  //����˥��ϵ��
  float fAttCoef1;
  //����˥��ϵ��
  float fAttCoef2;
  //���Ƿ���
  BOOL bOnOff;
} LightObj;
int initLightObj(LightObj* pLO, int n)
{
  for (; n > 0; --n, ++pLO) {
    //ȱʡ��ԴΪһȫ���ɫ���Դ
    FRGBASET(pLO->clrAmb, 0.0f, 0.0f, 0.0f, 1.0f);
    FRGBASET(pLO->clrDif, 1.0f, 1.0f, 1.0f, 1.0f);
    FRGBASET(pLO->clrSpe, 1.0f, 1.0f, 1.0f, 1.0f);
    pLO->hcPosition.x = 0.0f;
    pLO->hcPosition.y = 0.0f;
    pLO->hcPosition.z = 8.0f;
    pLO->hcPosition.w = 1.0f;
    pLO->spotDirection.x = 0.0f;
    pLO->spotDirection.y = 0.0f;
    pLO->spotDirection.z = -1.0f;
    pLO->fSpotExp = 0.0f;
    pLO->fSpotCutoff = 180.0f;
    pLO->fAttCoef0 = 1.0f;
    pLO->fAttCoef1 = 0.0f;
    pLO->fAttCoef2 = 0.0f;
    pLO->bOnOff = TRUE;
  }

  return 0;
}
//���Ƿ�Ϊ����״̬
//���ù�Դ�Ƿ���Կ���
//��ʽ��ͳһ, ��һ������ΪG3D_LIGHT_ON_OFF��Ч
void Lightb(LightObj* pLO, DWORD dwProperty,  BOOL bOnOff)
{
  if (dwProperty == G3D_LIGHT_ON_OFF) {
    pLO->bOnOff = bOnOff;
  }
}
//���ñ�������
//dwProperty----����
//fParam------����ֵ, ����
void Lightf(LightObj* pLO, DWORD dwProperty,  float fParam)
{
  switch (dwProperty) {
    //�۹�ָ��
  case G3D_SPOT_EXPONENT:
    //��ɢָ����[0, 128]֮��
    pLO->fSpotExp = (fParam < 0.0f) ? 0.0f : ((fParam > 128.0f) ? 128.0f : fParam);
    break;

    //��ɢ���
  case G3D_SPOT_CUTOFF:

    //��ɢ���ȡ [0, 90] �� 180
    if (ABS(fParam - 180.0f) < 0.001f) {
      pLO->fSpotCutoff = 180.0f;
    }
    else {
      pLO->fSpotCutoff = (fParam < 0.0f) ? 0.0f : ((fParam > 90.0f) ? 90.0f : fParam);
    }

    break;

    //����˥������
  case G3D_CONSTANT_ATTENUATION:
    pLO->fAttCoef0 = ABS(fParam);
    break;

    //����˥������
  case G3D_LINEAR_ATTENUATION:
    pLO->fAttCoef1 = ABS(fParam);
    break;

    //����˥������
  case G3D_QUADRATIC_ATTENUATION:
    pLO->fAttCoef2 = ABS(fParam);
    break;
  }
}
//������������
//��������, ����ֵ(һά����)
//dwProperty----����
//pfParams------����ֵ, �� RGBA ˳��� XYZW ˳����֯����
#define Lightfv(pLO, dwProperty, pfParams)  Lightff(pLO, dwProperty, pfParams[0], pfParams[1], pfParams[2], pfParams[3])
int Lightff(LightObj* pLO, DWORD dwProperty, float param0, float param1, float param2, float param3)
{
  switch (dwProperty) {
    //������
  case G3D_AMBIENT:
    FRGBASET(pLO->clrAmb, param0, param1, param2, param3);
    break;

    //�������
  case G3D_DIFFUSE:
    FRGBASET(pLO->clrDif, param0, param1, param2, param3);
    break;

    //����߹�
  case G3D_SPECULAR:
    FRGBASET(pLO->clrSpe, param0, param1, param2, param3);
    break;

    //���λ��
  case G3D_POSITION:
    V4SET(pLO->hcPosition, param0, param1, param2, param3);
    break;

    //�۹����䷽��(���������)
  case G3D_SPOT_DIRECTION: {
    //ֱ�ӵ�λ��
    float fMag = (float)sqrt(param0 * param0 + param1 * param1 + param2 * param2);

    //�����˻�����, �����䷽������ z ��
    if (fMag < 0.0001f) {
      V3SET(pLO->spotDirection, 0.0f, 0.0f, -1.0f);
    }
    else {
      V3SET(pLO->spotDirection, param0 / fMag, param1 / fMag, param2 / fMag);
    }
  }
  break;
  }

  return 0;
}

typedef struct _RenderDC {
  TDrawDC hDC[1]; // ��ɫ����
  HMC hZB; // ��Ȼ���
  Material material[1]; // �����׼����
  MATRIX3D m[1]; // ģ�ͱ任����
  VECTOR3D viewer;//�ӵ㣬���ڹ��ռ��㣬����������������ϵ
  VIEWFINDER pVF[1]; // ȡ����
  LightObj lights[4]; // ���Դ
  int nNumLight; // ��Դ����
  FRGBA clrGlobalAmb; //ȫ�ֻ�����
  BOOL bLocalViewer; // ��ֲ��ӵ� 0:����Զ�ӵ�ģ��
  BOOL bTwoSides; // �Ƿ�˫����� 0:ֻ����ǰ��
  DWORD dwRop; // ��ɫģʽ
  Image* tex;
} RenderDC;
int initRenderDC(RenderDC* rDC, HDDC hDC)
{
  Material defMaterial = {{1.f, 1.f, 1.f, 1.0f}, {1.f, 1.f, 1.f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.f, 1.f, 1.f, 1.0f}, 50.0f, G3D_FRONT};
  VIEWFINDER ViewFinder = {100.0f, 16.0f, 16.0f, {hDC->w / 2, hDC->h / 2}};
  memset(rDC, 0, sizeof(RenderDC));
  initLightObj(rDC->lights, 4);
  initMaterial(rDC->material, 1);
  rDC->bLocalViewer = 1;
  M3IDENTITY(*rDC->m);
  FRGBASET(rDC->clrGlobalAmb, 0.2f, 0.2f, 0.2f, 1.0f);
  V3SET(rDC->viewer, 0.0f, 0.0f, 8.0f);
  *rDC->pVF = ViewFinder;
  //��Ȼ�����, ����ɫ��������С����һ��
  rDC->hZB = newZBuffer(hDC->w, hDC->h);
  *rDC->hDC = *hDC;
  //*rDC->material = defMaterial;
  initAllDepth(rDC->hZB, 500.0f);
  return 0;
}
int delRenderDC(RenderDC* rDC)
{
  free(rDC->hZB);
  return 0;
}
int LightingVertex(Vertex3d* pVertex, RenderDC* rDC)
{
  int j; // ����Դ��������ѭ��
  Material* material = rDC->material;
  VIEWFINDER* pVF = rDC->pVF;
  const LightObj* lights = rDC->lights;
  int nNumLight = rDC->nNumLight;
  BOOL bLocalViewer = rDC->bLocalViewer;
  BOOL bTwoSides = rDC->bTwoSides;

  //ע:   �ù��ռ�������������ϵ�����
  //��ʽ��ʾ�ӵ�, ������������������ϵ
  VECTOR3D Viewer = rDC->viewer;
  //����һ������Զ�ӵ�۲�����,
  //������ռ���ģ�Ͳ�������Զ�ӵ�, ���������������, ����������
  //�ڴ������, ���ж���ָ���ӵ������������ͬ��.
  VECTOR3D vLocalView = rDC->viewer;
  //�������߹�ʱ��������Զ�ӵ�, �����ھ͵�λ��, �⽫���ٴ����ļ���
  //Ϊ��ͨ��, �����䶥���ǿɼ���, ���ǲ��ɼ���, ��������ж��㶼���м���
  //���ڸù���ϵͳ����������ϵ�н���, ����Ŀɼ�����Ϣ����֪��.
  //���ڸ��ӵĳ���, ����ƽ�湫ʽ�����������Ѿ������������.�����ıߺ�С��Ŀɼ��Ժ�ģ��.
  //��, ��һ����, �ߡ�С�����ɫ�������ڶ������ɫ
  //��ȡ����ĵ� i ������:
  VECTOR3D Vertex = pVertex->coord;
  FRGBA ff, fLast;

  //���Ĳ�, �ù۲�����ϵ�е� z ������������Ļ����ϵ�µ� z ����
  //����һ�����ο�Խ��������������ȡʵ�����,
  //ע��, CViewFinder_PerspectiveH()�����������º������ѭ����ͬ�ĳ�������;
  //�ڼ�������Щ�ظ�, ���۲������ڸ��Ӹ��ӵ�ϵͳ�л���������Ӧ��.
  //���, ���ǲ���ͨ��CViewFinder���Perspective()�������ѹ۲�����任Ϊ�ȼ۵���ά����
  if (ABS(pVertex->scrn.w) < 0.00001f) {
    pVertex->scrn.z = 100000.0f;
  }
  else {
    pVertex->scrn.z /= pVertex->scrn.w;
  }

  if (!bLocalViewer) {
    Unitize(&vLocalView);
  }

  //���������ת��Ϊ��ʵ����
  //����Զ��, ���Ƶ��� 0.0001 ������(�����������10000���ĵ�����������Զ�����ɫ)
  //���ؿ��� wVertex ������ 0 ʱ�ķ���
  if (ABS(Vertex.w) < 0.0001f) {
    Vertex.w = 0.0001f;
  }

  V3OPTv(Vertex, /= Vertex.w);
  //����������ɫ
  //��һ��, ���������ɫ, ����ֱ�ӹ�����
  fLast = material->clrEmi;
  //�ڶ���, ȫ�ֻ�����Ĺ���
  //ȫ�ֻ����� * ���ʶԻ�����ķ���ϵ�� -----
  //������Ӧ��˺�, �ۻ���������ɫ֮��.
  FRGBOPTvvv(fLast, += , rDC->clrGlobalAmb, *, material->refAmb);

  //�����ڹ�Դ�Ĺ��ף�
  for (j = 0; j < nNumLight; j++) {
    //�����Դ�ǿ�����
    if (lights[j].bOnOff) {
      //��һ, ����˥��ϵ��
      //˥��ϵ��.����1.0��˥��
      float fAttenuCoef = 1.0f;
      float fCos, fSpotFactor;
      float fInsideCone;
      //��ȡ�� j ����Դλ��
      VECTOR3D Light = lights[j].hcPosition;
      VECTOR3D vVertexToLight;

      //ֻ�е��Դ���ǲ���Ϊ���ǿ�˥����, ��ʱ, w = 1.0f.
      //�����Դ��ƽ�й�Դ, �������Դ, ��˥��, ʱʱ, w = 0.0f
      //��λ��Դ
      if (ABS(Light.w) > 0.0001f) {
        float fc1, fc2;
        //��ʵλ��, ������������������
        V3OPTv(Light, /= Light.w);
        //����˥��ϵ��
        fAttenuCoef = lights[j].fAttCoef0;
        //һ��˥��ϵ�������˥��ϵ��
        fc1 = lights[j].fAttCoef1;
        fc2 = lights[j].fAttCoef2;

        if ((fc1 > 0.0001f) || (fc2 > 0.0001f)) {
          //�󶥵�����Դ�ľ���
          float fDist = V3DIST(Light, Vertex);
          //=(float)sqrt((Light.x - Vertex.x) * (Light.x - Vertex.x) + (Light.y - Vertex.y) * (Light.y - Vertex.y) + (Light.z - Vertex.z) * (Light.z - Vertex.z));
          //����һ�κͶ�������
          fAttenuCoef += (fc1 * fDist + fc2 * fDist * fDist);
        }

        if (fAttenuCoef < 0.0001f) {
          fAttenuCoef = 0.0001f;
        }

        fAttenuCoef = 1.0f / fAttenuCoef;
        //˥��ϵ�����ô���1.0
        fAttenuCoef = MIN(1.0f, fAttenuCoef);
      }//end if ---- for calculating the coefficient of attenuation

      //���, ����۹�����
      //�۹�����, һ����Դ�ľ۹�����Ϊ 1.0f, (��ɢ���Ϊ180��)
      fSpotFactor = 1.0f;

      //�۹����������:��һ, ��Ϊ��λ��Դ; �ڶ�, ��ķ�ɢ���С�ڻ����90��
      if ((ABS(Light.w) > 0.0001f) && (lights[j].fSpotCutoff < 90.0001f)) {
        //��Դ����ʵλ���Ѿ�Ϊ(Light.x, Light.y, Light.z),
        //�����Դ�������۹�Ч��
        //����: �۹�λ��ָ�����䶥��
        VECTOR3D vLightToVertex;// = {(Vertex.x - Light.x), (Vertex.y - Light.y), (Vertex.z - Light.z)};
        V3OPTvvv(vLightToVertex, = , Vertex, -, Light);
        //��λ��
        Unitize(&vLightToVertex);
        //�۹����䷽��(�Ѿ���һ����λ����) �� ���� vLightToVertex �нǵ�����
        fInsideCone = V3DOT(vLightToVertex, lights[j].spotDirection);

        //�������λ�ڹ�׶֮��, �򲻻��о۹�������䵽������
        if (fInsideCone < (float)cos((lights[j].fSpotCutoff * PIE) / 180.0)) {
          fSpotFactor = 0.0f;
        }
        else {
          //���þ۹�ָ�����м���
          fSpotFactor = (float)pow(fInsideCone, lights[j].fSpotExp);
        }
      }//enf if ---- for calculating spot factor

      //����, �������Թ�Դ�Ĺ���(�����Ѿ����㹻��������)
      //����, ���뻷�����䲿��:
      FRGBOPTvvv(ff, = , material->refAmb, *, lights[j].clrAmb);
      //���, ���������䲿��
      //����ָ���Դ������
      V3SET(vVertexToLight, (Light.x - Vertex.x), (Light.y - Vertex.y), (Light.z - Vertex.z));

      //�����ԴΪƽ�й�Դ(��λ��Դ)
      if ((ABS(Light.w) <= 0.0001f)) {
        //��Դ��λ�þ������䷽��, �����������Դ���������ǹ�Դλ���������෴����
        V3OPTvv(vVertexToLight, = -, Light);
      }

      //��λ��
      Unitize(&vVertexToLight);
      //���㷨�������� vVertexToLight �����ļнǵ�����
      //���㷨��Ӧ�ǵ�λ����, ���ڽ�ģʱ�Ѿ�����뱣֤��
      fCos = V3DOT(vVertexToLight, pVertex->normal);
      fCos = (fCos < 0.0f) ? 0.0f : fCos;
      //���������䲿�ֵĹ���
      FRGBOPTvvv(ff, += fCos*, material->refDif, *, lights[j].clrDif);

      //���, ����߹ⲿ��
      //������㷨���붥������Դ�������нǴ���90��, ��û�и߹����
      //����Ͳ���
      if (fCos > 0.0f) {
        //���㶥��ָ���ӵ�ĵ�λ����:
        VECTOR3D vVertexToViewer;
        float fShine, fShineFactor;

        //����Զ�ӵ�ģ��
        if (!bLocalViewer) {
          vVertexToViewer = vLocalView;
        }
        //�ֲ��ӵ�ģ��
        else {
          V3OPTvvv(vVertexToViewer, = , Viewer, -, Vertex);
          //��λ��:
          Unitize(&vVertexToViewer);
        }

        //���϶���ָ���Դ�ĵ�λ����:
        //vVertexToViewer += vVertexToLight;
        V3ADD(vVertexToViewer, vVertexToViewer, vVertexToLight);
        //�ٴε�λ��:
        Unitize(&vVertexToViewer);
        //������ӻ���:�붥�㷨�������
        fShine = V3DOT(vVertexToViewer, pVertex->normal);
        fShine = (fShine < 0.0f) ? 0.0f : fShine;
        fShineFactor = (float)pow(fShine, material->fShininess);
        //����߹ⲿ�ֵĹ���
        FRGBOPTvvv(ff, += fShineFactor*, material->refSpe, *, lights[j].clrSpe);
      }// end if(fCos > 0.0f)

      //������˥���;۹�����
      //����ǵ� j ���������ĵڸ����������:
      FRGBOPTv(ff, *= fAttenuCoef * fSpotFactor);
      //�ۼ��������ɫ:
      FRGBOPTvv(fLast, += , ff);
    }// end if(lights[0].bOnOff)
  }//end j (number of lights)

  //����ɫ��һ������:
  FRGBROUND01(fLast);
  //��ɫ��ֵ
  FRGBOPTvv(pVertex->clr, *= , fLast);
  //����alpha, ��������򵥵��ò�������������������
  //��ֻ�Ǽ򻯴���, ����͸��Ч��, ��Ӧ����������ģ��.
  pVertex->clr.a = material->refDif.a;
  return 0;
}
