
//�������ʼ��ɫ:��ɫ
#define INIT_SUBObject3d(pS)  FRGBASET(pS->clr, 1.0f, 1.0f, 1.0f, 1.0f)
#define OBJ_VISABEL   0x0001  //����ɼ�
typedef struct _Vertex3d {
  //DWORD stage;
  VECTOR3D coord;   // ��������
  VECTOR3D normal;  // ���㷨��
  VECTOR3D scrn;   // ����͸�ӵ���Ļ�ϵĵ�
  FRGBA clr;//��������ɫ:������ɫ,RGBA��ɫģ��
} Vertex3d;
typedef struct _Vertex3d2 {
  //DWORD stage;
  FRGBA clr;//��������ɫ:������ɫ,RGBA��ɫģ��
  VECTOR3D coord;   // ��������
} Vertex3d2;
void initVertex3d(Vertex3d* pV, int n)
{
  for (; n--; ++pV) {
    INIT_SUBObject3d(pV);
    //��������:����ԭ��
    V3SET(pV->coord, 0.0f, 0.0f, 0.0f);
    //���㷨��:ָ��Z��
    V3SET(pV->normal, 0.0f, 0.0f, 1.0f);
  }
}
void initVertex3d2(Vertex3d2* pV, int n)
{
  for (; n--; ++pV) {
    INIT_SUBObject3d(pV);
    //��������:����ԭ��
    V3SET(pV->coord, 0.0f, 0.0f, 0.0f);
  }
}
float MaxRadius(HMC hV)
{
  int i, j;
  float mr, d;
  GetMatElem(mr, hV, 0, 0, float);
  mr = ABS(mr);

  for (i = 0; i < hV->h; ++i) {
    for (j = 0; j < hV->w; ++j) {
      d = _PixelT(hV, i, j, float);
      d = ABS(d);
      mr = MAX(mr, d);
    }
  }

  return mr;
}
// ���Ļ�
void Centralization(int vListLen, Vertex3d2* vList, float sc)
{
  int i;
  VECTOR3D maxv = vList[0].coord, minv = vList[0].coord, v;
  float r, invr;

  for (i = 1; i < vListLen; ++i) {
    maxv.x = MAX(maxv.x, vList[i].coord.x), minv.x = MIN(minv.x, vList[i].coord.x);
    maxv.y = MAX(maxv.y, vList[i].coord.y), minv.y = MIN(minv.y, vList[i].coord.y);
    maxv.z = MAX(maxv.z, vList[i].coord.z), minv.z = MIN(minv.z, vList[i].coord.z);
  }

  v.x = (maxv.x + minv.x) / 2;
  v.y = (maxv.y + minv.y) / 2;
  v.z = (maxv.z + minv.z) / 2;

  for (i = 0; i < vListLen; ++i) {
    vList[i].coord.x -= v.x;
    vList[i].coord.y -= v.y;
    vList[i].coord.z -= v.z;
  }

  v.x = (maxv.x - minv.x) / 2;
  v.y = (maxv.y - minv.y) / 2;
  v.z = (maxv.z - minv.z) / 2;
  r = MAX(v.x, v.y);
  r = MAX(r, v.z);
  invr = sc / r;

  for (i = 0; i < vListLen; ++i) {
    vList[i].coord.x *= invr;
    vList[i].coord.y *= invr;
    vList[i].coord.z *= invr;
  }

  return ;
}

typedef struct _CFacet {
  FRGBA clr;//��������ɫ:������ɫ,RGBA��ɫģ��
  //С�淨��
  //VECTOR3D normal;
  //��ͼ�δ�����,С��һ��Ϊ�����λ��ı���
  //С�涥�����
#define A3SET(A, A0, A1, A2)  ((A)[0]=A0, (A)[1]=A1, (A)[2]=A2)
  int vIndex[3];
  int tIndex[3];
  //int vIndexLen;
} CFacet;
void initCFacet(CFacet* pF, int n)
{
  for (; n--; pF++) {
    INIT_SUBObject3d(pF);
    //С�淨��:ָ��Z��
    //V3SET(pF->normal, 0.0f, 0.0f, 1.0f);
  }
}
//��ά����,�����任����������ϵ
// ȱʡ����ϵ
// X    ˮƽ�������� [-1.0, 1.0]
// Y    ��ֱ���¶��� [-1.0, 1.0]
// Z    ˮƽ�������� [-1.0, 1.0]
//��ҪӦ�ø�������ݵ�����: CPRender��,����������Щ��Ϊ������Ѻ���,Ȼ�����ӱ�����ʩ���ṩ�������ݵĻ���.
//Ϊ�˷������,���Ǽ򵥵���������Ϊ���г�Ա����
typedef struct _Object3d {
  char szName[256];
  //�����
  Vertex3d2* vList;
  int vListLen;
  //���
  CFacet* sList;
  int sListLen;
  //���
  POINT* tList;
  int tListLen;

  //�任����
  //MATRIX3D mGeomTrans;
  //�Զ���ɫ,���Ǹ�ֵ��ɫ
  //�Զ���ɫ��ָ�ɶ�����ɫ����С��ͱߵ���ɫ
  //��ֵ��ɫ��ָ���ⲿ��С��ͱ߸���ɫֵ
  //����,������ɫ������Ҫ��ֵ��ͨ�����ռ���
  BOOL bAutoColor;
  float maxRadius; // ���뾶
  Image* tex;
} Object3d;
int SwapXYZ(Object3d* pO, int xIndex, int yIndex, int zIndex)
{
  int i, t, index[3] = {xIndex, yIndex, zIndex};
  uchar* u = (uchar*)(&pO->vList->coord);
  int bw = sizeof(Vertex3d2);

  for (i = 0; i < pO->vListLen; ++i) {
    float* f = (float*)(u + bw * i);
    float x = f[index[0]], y = f[index[1]], z = f[index[2]];
    f[0] = x, f[1] = y, f[2] = z;
  }

  i = (xIndex > yIndex) + (yIndex > zIndex) + (xIndex > zIndex);

  if (0 == i % 2) {
    return 0;
  }

  for (i = 0; i < pO->sListLen; ++i) {
    SWAP(pO->sList[i].vIndex[0], pO->sList[i].vIndex[1], t);
    SWAP(pO->sList[i].tIndex[0], pO->sList[i].tIndex[1], t);
  }

  return 0;
}
void initObject3d(Object3d* pO, int nV, int nF, int nT)
{
  memset(pO, 0, sizeof(Object3d));
  pO->vListLen = nV;
  pO->sListLen = nF;
  pO->tListLen = nT;

  pO->bAutoColor = (TRUE);
  //M3IDENTITY(pO->mGeomTrans);
  pO->vList = MALLOC(Vertex3d2, pO->vListLen);
  pO->sList = MALLOC(CFacet, pO->sListLen);

  if (pO->tListLen) {
    pO->tList = MALLOC(POINT, pO->tListLen);
  }

  initCFacet(pO->sList, pO->sListLen);
  initVertex3d2(pO->vList, pO->vListLen);
}
void delObject3d(Object3d* pO, int n)
{
  for (; n--; ++pO) {
    if (pO->vList) {
      free(pO->vList);
      pO->vListLen = 0;
    }

    if (pO->sList) {
      free(pO->sList);
      pO->sListLen = 0;
    }

    if (pO->tList) {
      free(pO->tList);
      pO->tList = 0;
    }

    if (pO->tex) {
      free(pO->tex);
      pO->tex = 0;
    }
  }
}
//��¡
Object3d* cloneObject3D(Object3d* pO)
{
  Object3d* pCloned = (Object3d*)malloc(sizeof(Object3d));
  //��¡���μ�����
  *pCloned = *pO;
  pCloned->vList = MEMDUP(Vertex3d2, pO->vList, pO->vListLen);
  pCloned->sList = MEMDUP(CFacet, pO->sList, pO->sListLen);
  return pCloned;
}
// ���㶥�㡢С�淨��
VECTOR3D CalcSingleFacetNoraml(const VECTOR3D* v1,  const VECTOR3D* v2,  const VECTOR3D* v3)
{
  //���һ���������а�������Զ��, �����������һ������������,
  float w1 = v1->w,  w2 = v2->w,  w3 = v3->w;

  if (ABS(w1) <= 0.00001f) {
    w1 = 1.0f;
  }

  if (ABS(w2) <= 0.00001f) {
    w2 = 1.0f;
  }

  if (ABS(w3) <= 0.00001f) {
    w3 = 1.0f;
  }

  {
    float x1 = v1->x / w1,  y1 = v1->y / w1,  z1 = v1->z / w1;
    float x2 = v2->x / w2,  y2 = v2->y / w2,  z2 = v2->z / w2;
    float x3 = v3->x / w3,  y3 = v3->y / w3,  z3 = v3->z / w3;
    VECTOR3D vector1 = {(x2 - x1), (y2 - y1), (z2 - z1)};
    VECTOR3D vector2 = {(x3 - x1), (y3 - y1), (z3 - z1)};
    VECTOR3D noraml;
    V3CROSS(noraml, vector1, vector2);
    return noraml;
  }
}

#define UNITIZE(V)  {float fMag = (float)sqrt(V.x * V.x + V.y * V.y + V.z * V.z);if (fMag < 0.0001f) {fMag = 1.0f;}V.x /= fMag;V.y /= fMag;V.z /= fMag;}


//�����Ƕ���һЩ���ID��
#define CHUNK_VERSION        0x0002  // 3ds�ļ��İ汾
#define CHUNK_RGB1           0x0010  // 3 floats of RGB
#define CHUNK_RGB2           0x0011  // 3 bytes of RGB
#define CHUNK_RGB3           0x0012  //?
#define CHUNK_AMOUNT         0x0030  // ������(Primary Chunk)��λ���ļ��Ŀ�ʼ
#define CHUNK_MAIN           0x4D4D  //����(==�����ļ�)
#define CHUNK_OBJMESH        0x3D3D  //�������İ汾��
#define CHUNK_BKGCOLOR       0x1200  //
#define CHUNK_AMBCOLOR       0x2100  //
#define CHUNK_OBJBLOCK       0x4000  //����飬������桢�������Ϣ
#define CHUNK_TRIMESH        0x4100  //�µ��������
#define CHUNK_VERTLIST       0x4110  //�����
#define CHUNK_FACELIST       0x4120  //���б��
#define CHUNK_FACEMAT        0x4130  //����ʿ�
#define CHUNK_MAPLIST        0x4140  //�����UV��������
#define CHUNK_SMOOLIST       0x4150  //
#define CHUNK_TRMATRIX       0x4160  //
#define CHUNK_LIGHT          0x4600  //
#define CHUNK_SPOTLIGHT      0x4610  //
#define CHUNK_CAMERA         0x4700  //
#define CHUNK_MATERIAL       0xAFFF  //������Ϣ
#define CHUNK_MATNAME        0xA000  //��������
#define CHUNK_AMBIENT        0xA010  //
#define CHUNK_DIFFUSE        0xA020  //������ʵ���ɫ
#define CHUNK_SPECULAR       0xA030  //
#define CHUNK_SHININESS      0xA040  //
#define CHUNK_SHINE_STRENGTH 0xA041  //
#define CHUNK_DOUBLESIDED    0xA081  //
#define CHUNK_TEXTURE        0xA200  //�²��ʵ�ͷ��
#define CHUNK_BUMPMAP        0xA230  //
#define CHUNK_MAPFILENAME    0xA300  //����������ļ���
#define CHUNK_MAPOPTIONS     0xA351  //
#define CHUNK_TILINGU        0xA354  //U��������ϵ��
#define CHUNK_TILINGV        0xA356  //V��������ϵ��
#define CHUNK_OFFSETU        0xA358  //U����ƫ��ϵ��
#define CHUNK_OFFSETV        0xA35A  //V����ƫ��ϵ��
#define CHUNK_ROTATEW        0xA35C  //W������ת��
#define CHUNK_KEYFRAMER      0xB000  //���йؼ�֡��Ϣ��ͷ��

#pragma pack(1)
/** ��Ľṹ���� */
typedef struct _tFace {
  unsigned short vertIndex[4];         /**< �������� */
} tFace;
/** ������Ϣ�ṹ�� */
typedef struct _tMatInfo {
  char  strName[255];        /**< �������� */
  char  strTexture[255];        /**< �����ļ����� */
  int   texureId;          /**< ����ID */
  float uTile;           /**< u �ظ� */
  float vTile;           /**< v �ظ� */
  float uOffset;             /**< u ����ƫ�� */
  float vOffset;           /**< v ����ƫ�� */
  Image tex[1];

  float   pAmbient[4];
  float   pDiffuse[4]; // �����RGB��ɫ
  float   pSpecular[4];
  float   fOffsetU;
  float   fOffsetV;
  float   fTilingU;
  float   fTilingV;
  float   fRotationW;

} tMatInfo;
/** ������Ϣ�ṹ�� */
typedef struct _t3DObject {
  int  numOfVerts;          /**<  ģ���ж������Ŀ */
  int  numOfFaces;        /**< ģ���������Ŀ */
  int  numTexVertex;        /**< ģ���������������Ŀ */
  int  materialID;        /**< ����ID */
  BOOL bHasTexture;       /**< �Ƿ��������ӳ�� */
  Vector3  vtxCenter;       /**< ������������� */
  char strName[255];        /**< ��������� */
  Vector3*  pVerts;       /**< ����Ķ��� */
  //Vector3*  pNormals;         /**< ����ķ����� */
  Vector2*  pTexVerts;      /**< ����UV���� */
  tFace* pFaces;          /**< ���������Ϣ */
  Vertex3d* pV;
} t3DObject;

#define MAX_MATERIALS  1000
#define MAX_OBJECTS  1000
/** ģ����Ϣ�ṹ�� */
typedef struct _t3DModel {
  int numOfObjects;       /**< ģ���ж������Ŀ */
  int numOfMaterials;     /**< ģ���в��ʵ���Ŀ */
  tMatInfo pMaterials[MAX_MATERIALS];   /**< ����������Ϣ */
  t3DObject pObject[MAX_OBJECTS];     /**< ģ���ж���������Ϣ */
} t3DModel;

/** ����Ϣ�Ľṹ */
typedef struct _tChunk {
  unsigned short ID;      /**< ���ID */
  unsigned int length;      /**< ��ĳ��� */
} tChunk;
#pragma pack()






#include "loadObject.inl"
