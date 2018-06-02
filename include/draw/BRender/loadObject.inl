
#define NEXTLINE(szText)  {szText=strchr(szText, '\n'); if (NULL==szText++) return 0;}
int loadObject_PLG(Object3d* pO, int n, LPTSTR lpszFileName)
{
  int i, vertex, len, n0 = n;
  char* buf = (char*)loadfile(lpszFileName, &len);

  for (; n--; ++pO) {
    char* szText = buf;
    // Read header information
    i = _stscanf(szText, _T("%s %d %d"), pO->szName, &pO->vListLen, &pO->sListLen);
    NEXTLINE(szText);
    //pO->world_pos = pos;
    // Read vertex information
    initObject3d(pO, pO->vListLen, pO->sListLen, 0);
    pO->maxRadius = 0;

    for (i = 0; i < pO->vListLen; i++) {
      float max_vertex;
      _stscanf(szText, _T("%f %f %f"), &pO->vList[i].coord.x, &pO->vList[i].coord.y, &pO->vList[i].coord.z);
      pO->vList[i].coord.w = 1.0f;
      max_vertex = MAX(ABS(pO->vList[i].coord.x), MAX(ABS(pO->vList[i].coord.y), ABS(pO->vList[i].coord.z)));
      NEXTLINE(szText);

      if (pO->maxRadius < max_vertex) {
        pO->maxRadius = max_vertex;
      }
    }

    for (i = 0; i < pO->sListLen; i++) {
      _TCHAR szTemp[16];
      int number_vertices;
      int poly_surface_descr = 0;
      BYTE shade;
      _stscanf(szText, _T("%s %d %d %d %d"), szTemp, &number_vertices, &pO->sList[i].vIndex[0], &pO->sList[i].vIndex[1], &pO->sList[i].vIndex[2]);
      NEXTLINE(szText);

      if ((szTemp[0] == '0') && (_totupper(szTemp[1]) == 'X')) {
        _stscanf(szTemp, _T("%X"), &poly_surface_descr);
      }
      else {
        poly_surface_descr = _ttoi(szTemp);
      }

      if (poly_surface_descr & 0x8000) {
        BYTE red = (poly_surface_descr & 0x0F00) >> 8;
        BYTE green = (poly_surface_descr & 0x00F0) >> 4;
        BYTE blue = poly_surface_descr & 0x000F;
        FRGBSET(pO->sList[i].clr, red << 4, green << 4, blue << 4);
      }
      else {
        DWORD clr = poly_surface_descr & 0x00FF;
        FRGBSET(pO->sList[i].clr, GetRV(clr), GetGV(clr), GetBV(clr));
      }

      shade = (poly_surface_descr & 0x6000) >> 13;
    }

    for (vertex = 0; vertex < pO->vListLen; vertex++) {
      pO->vList[vertex].coord.x /= pO->maxRadius * 2;
      pO->vList[vertex].coord.y /= pO->maxRadius * 2;
      pO->vList[vertex].coord.z /= pO->maxRadius * 2;
    }
  }

  free(buf);
  return n0;
}

typedef struct __MD2HEADER {
  int identifier;
  int version;
  int skin_width;
  int skin_height;
  int framesize;
  int num_skins;
  int num_verts;
  int num_textcoords;
  int num_polys;
  int num_openGLcmds;
  int num_frames;
  int offset_skins;
  int offset_textcoords;
  int offset_polys;
  int offset_frames;
  int offset_openGlcmds;
  int offset_end;
} _MD2HEADER;

typedef struct __MD2POLY {
  unsigned short vindex[3];
  unsigned short tindex[3];
} _MD2POLY;

typedef struct __MD2TEXTCOORD {
  short u;
  short v;
} _MD2TEXTCOORD;

typedef struct __MD2POINT {
  unsigned char v[3];
  unsigned char normal_index;
} _MD2POINT;

typedef struct __MD2FRAME {
  float scale[3];
  float translate[3];
  char name[16];
  _MD2POINT vlist[1];
} _MD2FRAME;

typedef enum __TEXTURE_FILTERING {
  _TEXTURE_FILTERING_NONE = 0x0000,
  _TEXTURE_FILTERING_BILINEAR = 0x0001
} _TEXTURE_FILTERING;

#define MD2_MAGIC_NUM ('I' + ('D' << 8) + ('P' << 16) + ('2' << 24))  // MD2 file magic number
#define MD2_VERSION   8

int extractFrame_MD2(Object3d* pO, int n, _MD2HEADER* md2_header, int md2_curr_frame)
{
  int vindex, pindex, tindex, n0 = n;
  uchar* lpData = (uchar*)md2_header;
  int frame = md2_curr_frame % md2_header->num_frames;
  _MD2FRAME* md2_frame = (_MD2FRAME*)(lpData + md2_header->offset_frames + md2_header->framesize * frame);
  _MD2POLY* md2_poly = (_MD2POLY*)(lpData + md2_header->offset_polys);
  float sx = md2_frame->scale[0];
  float sy = md2_frame->scale[1];
  float sz = md2_frame->scale[2];
  float tx = md2_frame->translate[0];
  float ty = md2_frame->translate[1];
  float tz = md2_frame->translate[2];

  if ((md2_header->identifier != MD2_MAGIC_NUM) || (md2_header->version != MD2_VERSION)) {
    return 0;
  }

  for (; n--; ++pO) {
    pO->maxRadius = 0;
    initObject3d(pO, md2_header->num_verts, md2_header->num_polys, md2_header->num_textcoords);

    for (tindex = 0; tindex < pO->tListLen; tindex++) {
      pO->tList[tindex].x = ((_MD2TEXTCOORD*)(lpData + md2_header->offset_textcoords))[tindex].u;
      pO->tList[tindex].y = ((_MD2TEXTCOORD*)(lpData + md2_header->offset_textcoords))[tindex].v;
    }

    for (vindex = 0; vindex < pO->vListLen; vindex++) {
      VECTOR3D v;
      float max_vertex, temp;
      v.x = (float)(md2_frame->vlist[vindex].v[0]) * sx + tx;
      v.y = (float)(md2_frame->vlist[vindex].v[1]) * sy + ty;
      v.z = (float)(md2_frame->vlist[vindex].v[2]) * sz + tz;
      v.w = 1.0f;

      SWAP(v.y, v.z, temp);

      pO->vList[vindex].coord = v;

      max_vertex = MAX(ABS(pO->vList[vindex].coord.x), MAX(ABS(pO->vList[vindex].coord.y), ABS(pO->vList[vindex].coord.z)));

      if (pO->maxRadius < max_vertex) {
        pO->maxRadius = max_vertex;
      }
    }

    for (pindex = 0; pindex < pO->sListLen; pindex++) {
      pO->sList[pindex].vIndex[0] = md2_poly[pindex].vindex[0];
      pO->sList[pindex].vIndex[1] = md2_poly[pindex].vindex[1];
      pO->sList[pindex].vIndex[2] = md2_poly[pindex].vindex[2];

      pO->sList[pindex].tIndex[0] = md2_poly[pindex].tindex[0];
      pO->sList[pindex].tIndex[1] = md2_poly[pindex].tindex[1];
      pO->sList[pindex].tIndex[2] = md2_poly[pindex].tindex[2];
    }
  }

  return n0;
}

//----------------------------------------------------
//Function Name : Read3DSTextureChunk
//Desc          : �ӵ�ǰλ�ÿ�ʼ��ȡ�����
//Input         ��file       : ���ڶ�λ��fstream�ļ����������
//              : dwLength  : ���С
//              : lpData    : ����������ݵĶ���
//Output        : �ɹ�����true�����򷵻�false
//Notice        : ��ǰ�ļ�ָ�����ָ�����������ݲ��֣���������ͷ��
//                file��lpData���걸����ʹ���߱�֤
//----------------------------------------------------
int Read3DSTextureChunk(tMatInfo* pMaterial, tChunk* pPreChunk, char* buf)
{
  char* ebuf = buf + pPreChunk->length - sizeof(tChunk);
  tChunk*       m_CurrentChunk;             /**< ��ȡ�����е�ǰ�� */

  /** ����������Щ�� */
  while (buf < ebuf) {
    /** ������һ�� */
    m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

    /** �ж϶������ʲô�� */
    switch (m_CurrentChunk->ID) {
    case CHUNK_MAPFILENAME:            /**< �����ļ������� */
      /** ������ʵ��ļ����� */
      memcpy(pMaterial->strTexture, buf, m_CurrentChunk->length - sizeof(tChunk));
      break;

    case CHUNK_OFFSETU:     //����Ϊ�ֲ�����ӳ�����
      memcpy((char*) & (pMaterial->fOffsetU), buf, sizeof(float));
      break;

    case CHUNK_OFFSETV:
      memcpy((char*) & (pMaterial->fOffsetV), buf, sizeof(float));
      pMaterial->fOffsetV = -(pMaterial->fOffsetV);
      break;

    case CHUNK_TILINGU:
      memcpy((char*) & (pMaterial->fTilingU), buf, sizeof(float));
      break;

    case CHUNK_TILINGV:
      memcpy((char*) & (pMaterial->fTilingV), buf, sizeof(float));
      break;

    case CHUNK_ROTATEW:
      memcpy((char*) & (pMaterial->fRotationW), buf, sizeof(float));
      pMaterial->fRotationW = -(pMaterial->fRotationW);

    default:
      /** ��������Ҫ����Ŀ� */
      printf("id = %4x\n", m_CurrentChunk->ID);
      break;
    }

    buf += m_CurrentChunk->length - sizeof(tChunk);
  }

  return 0;
}

//----------------------------------------------------
//Function Name : Read3DSColorChunk
//Desc          : �ӵ�ǰλ�ÿ�ʼ��ȡ��ɫ��
//Input         ��file       : ���ڶ�λ��fstream�ļ����������
//              : wID       :
//Output        : �ɹ�����true�����򷵻�false
//              : r         : ��ɫ������float��
//              : g         : �̡�����
//              : b         : ��������
//Notice        : ��ǰ�ļ�ָ�����ָ����ɫ�����ݲ��֣���������ͷ��
//                file���걸����ʹ���߱�֤
//----------------------------------------------------
int Read3DSColorChunk(float* rgb, char* buf)
{
  tChunk*       m_CurrentChunk;             /**< ��ȡ�����е�ǰ�� */
  m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

  if (m_CurrentChunk->ID == CHUNK_RGB1) {         //3 floats of rgb
    memcpy(rgb, buf, 3 * sizeof(float));
  }
  else if (m_CurrentChunk->ID == CHUNK_RGB2) {
    //3 bytes of rgb
    BYTE byRGB[4];
    memcpy(byRGB, buf, 3);
    rgb[0] = (float)byRGB[0] / 255.0f;
    rgb[1] = (float)byRGB[1] / 255.0f;
    rgb[2] = (float)byRGB[2] / 255.0f;
  }
  else {
    return 0;
  }

  return 1;
}
/** �������еĲ�����Ϣ */
int ReadNextMatChunk(tMatInfo* pMaterial, tChunk* pPreChunk, char* buf)
{
  char* ebuf = buf + pPreChunk->length - sizeof(tChunk);
  tChunk*       m_CurrentChunk;             /**< ��ȡ�����е�ǰ�� */

  /** ����������Щ�� */
  while (buf < ebuf) {
    /** ������һ�� */
    m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

    /** �ж϶������ʲô�� */
    switch (m_CurrentChunk->ID) {
    case CHUNK_MATNAME:             /**< ���ʵ����� */
      /** ������ʵ����� */
      memcpy(pMaterial->strName, buf, m_CurrentChunk->length - sizeof(tChunk));
      break;

    case CHUNK_AMBIENT:     //���ʲ�������ͬ
      //ÿ������������RGB���ɵ�
      Read3DSColorChunk(pMaterial->pAmbient, buf);
      break;

    case CHUNK_DIFFUSE:            /**< �����R G B��ɫ */
      Read3DSColorChunk(pMaterial->pDiffuse, buf);
      break;

    case CHUNK_SPECULAR:
      Read3DSColorChunk(pMaterial->pSpecular, buf);
      break;

    case CHUNK_TEXTURE:              /**< ������Ϣ��ͷ�� */
      /** ��һ�����ʿ���Ϣ */
      Read3DSTextureChunk(pMaterial, m_CurrentChunk, buf);
      break;

    default:
      /** ��������Ҫ����Ŀ� */
      //printf("id = %4x\n", m_CurrentChunk->ID);
      break;
    }

    buf += m_CurrentChunk->length - sizeof(tChunk);
  }

  return 0;
}

/** �������е��ļ��ж������Ϣ */
int ReadNextTrimesh(t3DObject* pObject, tChunk* pPreChunk, char* buf)
{
  char* ebuf = buf + pPreChunk->length - sizeof(tChunk);
  tChunk*       m_CurrentChunk = (tChunk*)buf;           /**< ��ȡ�����е�ǰ�� */

  /** ��������������ֱ�����ӿ���� */
  while (buf < ebuf) {
    /** ������һ���� */
    m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

    /** ������������ֿ� */
    switch (m_CurrentChunk->ID) {
    case CHUNK_VERTLIST: {      /**< �����Ƕ��󶥵� */
      /** ���붥�����Ŀ */
      pObject->numOfVerts = *(unsigned short*)(buf);
      /** ���붥������ */
      pObject->pVerts = (Vector3*)(buf + 2);
    }

    break;

    case CHUNK_FACELIST: {       /**< ������Ƕ������ */
      /** ����ö����������Ŀ */
      pObject->numOfFaces = *(unsigned short*)(buf);
      /** ����������Ĵ洢�ռ䣬����ʼ���ṹ */
      pObject->pFaces = (tFace*)(buf + 2);
    }

    break;

    case CHUNK_MAPLIST: {          /**< ��������UV�������� */
      /** ����UV��������� */
      pObject->numTexVertex = *(unsigned short*)(buf);
      /** ������������ */
      pObject->pTexVerts = (Vector2*)(buf + 2);
    }
    break;


    case CHUNK_FACEMAT: {      /**< ������Ƕ���Ĳ������� */
      char* strMaterial;      /**< �����������Ĳ������� */
      /** ���븳�赱ǰ����Ĳ������� */
      strMaterial = buf;
    }

    break;

    case CHUNK_TRMATRIX:
      //ȡ��������������꣨Ŀǰû���ô���
      ASSERT(54 == m_CurrentChunk->length);
      memcpy(&pObject->vtxCenter, buf + 9 * sizeof(float), 3 * sizeof(float));
      break;

    default:
      /** �Թ�����Ҫ����Ŀ� */
      //ASSERT(0);
      //printf("ReadNextObjChunk id = %4x\n", m_CurrentChunk->ID);
      break;
    }

    buf += m_CurrentChunk->length - sizeof(tChunk);
  }

  return 0;
}
/** �������е��ļ��ж������Ϣ */
int ReadNextObjChunk(t3DObject* pObject, tChunk* pPreChunk, char* buf)
{
  char* ebuf = buf + pPreChunk->length - sizeof(tChunk);
  tChunk*       m_CurrentChunk = (tChunk*)buf;           /**< ��ȡ�����е�ǰ�� */

  /** ��ò������������ƣ�Ȼ�����Ӷ�����ֽ��� */
  strcpy(pObject->strName, buf);
  buf += strlen(buf) + 1;

  /** ��������������ֱ�����ӿ���� */
  while (buf < ebuf) {
    /** ������һ���� */
    m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

    /** ������������ֿ� */
    switch (m_CurrentChunk->ID) {
    case CHUNK_TRIMESH:          /**< ���������һ���¿� */
      /** ʹ�õݹ麯�����ã�������¿� */
      ReadNextTrimesh(pObject, m_CurrentChunk, buf);
      break;

    default:
      /** �Թ�����Ҫ����Ŀ� */
      //ASSERT(0);
      //printf("ReadNextObjChunk id = %4x\n", m_CurrentChunk->ID);
      break;
    }

    buf += m_CurrentChunk->length - sizeof(tChunk);
  }

  return 0;
}
/** ����3ds�ļ�����Ҫ���� */
int ReadNextChunk(t3DModel* pModel, tChunk* pPreChunk, char* buf)
{
  unsigned int version = 0;         /**< �����ļ��汾 */
  char* ebuf = buf + pPreChunk->length - sizeof(tChunk);
  tChunk*       m_CurrentChunk;             /**< ��ȡ�����е�ǰ�� */

  /** ���������ӿ� */
  while (buf < ebuf) {
    /** ������һ���� */
    m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

    /** �жϿ��ID�� */
    switch (m_CurrentChunk->ID) {
    case 0x3d3e:
      /** ������һ���� */
      /** �������İ汾�� */
      memcpy(&version, buf, m_CurrentChunk->length - sizeof(tChunk));
      break;

      /** �ļ��汾�� */
      /** ������Ϣ */
    case CHUNK_MATERIAL:
      /** �������װ�뺯�� */
      ASSERT(pModel->numOfMaterials < MAX_MATERIALS);
      ReadNextMatChunk(pModel->pMaterials + pModel->numOfMaterials, m_CurrentChunk, buf);
      pModel->numOfMaterials++;
      break;

      /** �������� */
    case CHUNK_OBJBLOCK:
      ASSERT(pModel->numOfMaterials < MAX_OBJECTS);
      /** ��������Ķ�����Ϣ�Ķ��� */
      ReadNextObjChunk(pModel->pObject + pModel->numOfObjects, m_CurrentChunk, buf);
      pModel->numOfObjects++;
      break;

      /** �ؼ�֡ */
    case CHUNK_KEYFRAMER:
      break;

    default:
      //ASSERT(0);
      //printf("ReadNextChunk id = %4x\n", m_CurrentChunk->ID);
      break;
    }

    buf += m_CurrentChunk->length - sizeof(tChunk);
  }

  return 0;
}
int load3DSTex(t3DModel* pModel, char* path)
{
  int i;
  char buf[256];

  for (i = 0; i < pModel->numOfMaterials; ++i) {
    if (path) {
      _snprintf(buf, 256, "%s/%s", path, pModel->pMaterials[i].strTexture);
      loadbmpfile(buf, 32, 1, pModel->pMaterials[i].tex);
    }
    else {
      if (pModel->pMaterials[i].tex) {
        free(pModel->pMaterials[i].tex);
      }
    }
  }

  return 0;
}
/** ����3DS�ļ���ģ�ͽṹ�� */
int load3DS(t3DModel* pModel, char* buf)
{
  int n0 = 0;
  tChunk* m_CurrentChunk;
  memset(pModel, 0, sizeof(t3DModel));

  /**< ����ļ�ָ�� */
  if (!buf) {
    return 0;
  }

  /** ���ļ��ĵ�һ��������ж��Ƿ���3ds�ļ� */
  m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

  /** ȷ����3ds�ļ� */
  if (m_CurrentChunk->ID != CHUNK_MAIN) {
    MessageBox(NULL, "���ܼ�������!", "Error", MB_OK);
    return 0;
  }

  m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

  if (CHUNK_VERSION == m_CurrentChunk->ID) {
    /** �����ļ��İ汾�� */
    unsigned int version = *(unsigned int*)(buf);
    memcpy(&version, buf, m_CurrentChunk->length - sizeof(tChunk));

    /** ����ļ��汾�Ŵ���3������һ��������Ϣ */
    if (version > 0x03) {
      MessageBox(NULL, " ��3DS�ļ��汾����3.0,���ܲ�����ȷ��ȡ", "����", MB_OK);
    }
  }

  buf += m_CurrentChunk->length - sizeof(tChunk);
  m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

  if (CHUNK_OBJMESH == m_CurrentChunk->ID) {
    ReadNextChunk(pModel, m_CurrentChunk, buf);
  }

  return n0;
}
