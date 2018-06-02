����˫���Բ�ֵ

// ˫���Բ�ֵ�������ڵ�ͨ��8λ�Ҷ�ͼ�񣬿���ͬʱ������С���Ŵ�
//�����ֶ���������
//1��ʹ�ÿɷ��뷽ʽ���Ⱥ����к���
//2���������������Ա����ĳЩ���ظ���ֵ��
//3��ʹ���������㣬���⸡�����㡣

�ϱ���ִ��1000�ε���ʱ�䣬���Կ���������Сʱ�ٶȱ�opencv��cvResize�൱���ڷŴ�ʱ�����ҵı�cvResize����50 % ������������⡣

typedef struct _Image {
  int w;
  int h;
  int c;
  int step;
  int type;
  void* data;
} Image;

static void _ieInterpImageBilinear8UC1_RowFilter(unsigned char* src, long* dst, int len, int* leftIdx, int* rightIdx, long* weight, int shift)
{
  int i, j;

  for (i = 0, j = 1; i < len - 2; i += 2, j += 2) {
    dst[i] = ((1 << shift) - weight[i]) * src[leftIdx[i]] + weight[i] * src[rightIdx[i]];
    dst[j] = ((1 << shift) - weight[j]) * src[leftIdx[j]] + weight[j] * src[rightIdx[j]];

  }

  for (; j < len; j++) {
    dst[j] = ((1 << shift) - weight[j]) * src[leftIdx[j]] + weight[j] * src[rightIdx[j]];
  }
}

static void _ieInterpImageBilinear8UC1(Image* src, Image* dst)
{
  int i, j;
  int sw, sh, sstep;
  int dw, dh, dstep;
  unsigned char* sdata, *ddata;
  float horScaleRatio, verScaleRatio;
  long* rowBuf1, *rowBuf2;
  long* upLinePtr, *downLinePtr, *tempPtr;
  long* horWeight;
  int* horLeftIdx, *horRightIdx;
  int preVerUpIdx, preVerDownIdx;
  int shift = 8;

  sw = src->w;
  sh = src->h;
  sstep = src->step;
  sdata = (unsigned char*)(src->tt.data);
  dw = dst->w;
  dh = dst->h;
  dstep = dst->step;
  ddata = (unsigned char*)(dst->tt.data);

  horScaleRatio = sw / (float)(dw);
  verScaleRatio = sh / (float)(dh);

  rowBuf1 = new long[dw];
  rowBuf2 = new long[dw];
  horWeight = new long[dw];
  horLeftIdx = new int[dw];
  horRightIdx = new int[dw];


  //col interpolation

  //����Ŀ��ͼ�����غ���������ھ���ţ���Ȩ�ء�
  for (i = 0; i < dw; i++) {
    float pos = (i + 0.5f) * horScaleRatio;
    horLeftIdx[i] = (int)(IET_MAX(pos - 0.5f, 0));
    horRightIdx[i] = (int)(IET_MIN(pos + 0.5f, sw - 1));
    horWeight[i] = (long)(fabs(pos - 0.5f - horLeftIdx[i]) * (1 << shift));
  }

  preVerUpIdx = -1;
  preVerDownIdx = -1;
  upLinePtr = rowBuf1;
  downLinePtr = rowBuf2;

  for (j = 0; j < dh; j++) {
    float pos = (j + 0.5f) * verScaleRatio;
    int verUpIdx = (int)(IET_MAX(pos - 0.5f, 0));
    int verDownIdx = (int)(IET_MIN(pos + 0.5f, sh - 1));
    long verWeight = (long)(fabs(pos - 0.5f - verUpIdx) * (1 << shift));

    if (verUpIdx == preVerUpIdx && verDownIdx == preVerDownIdx) {
      ;
      //do nothing
    }
    else if (verUpIdx == preVerDownIdx) {
      IET_SWAP(upLinePtr, downLinePtr, tempPtr);
      _ieInterpImageBilinear8UC1_RowFilter(sdata + sstep * verDownIdx, downLinePtr, dw, horLeftIdx, horRightIdx, horWeight, shift);
    }
    else {
      _ieInterpImageBilinear8UC1_RowFilter(sdata + sstep * verUpIdx,   upLinePtr, dw, horLeftIdx, horRightIdx, horWeight, shift);
      _ieInterpImageBilinear8UC1_RowFilter(sdata + sstep * verDownIdx, downLinePtr, dw, horLeftIdx, horRightIdx, horWeight, shift);
    }

    for (i = 0; i < dw; i++) {
      ddata[dstep * j + i] = (unsigned char)((((1 << shift) - verWeight) * upLinePtr[i] + verWeight * downLinePtr[i]) >> (2 * shift));
    }

    preVerUpIdx = verUpIdx;
    preVerDownIdx = verDownIdx;
  }

  delete []rowBuf1;
  delete []rowBuf2;
  delete []horWeight;
  delete []horLeftIdx;
  delete []horRightIdx;
}

//ʹ��������Ĵ�չ��ѭ�����ٶ�����15%+��

static void _ieInterpImageBilinear8UC1_Ver3_RowFilter(unsigned char* src, long* dst, int len, int* leftIdx, int* rightIdx, long* weight, int shift)
{
  int i;

  for (i = 0; i < len - 4; i += 4) {
    *dst++ = ((1 << shift) - weight[i]) * src[leftIdx[i]] + weight[i] * src[rightIdx[i]];
    *dst++ = ((1 << shift) - weight[i + 1]) * src[leftIdx[i + 1]] + weight[i + 1] * src[rightIdx[i + 1]];
    *dst++ = ((1 << shift) - weight[i + 2]) * src[leftIdx[i + 2]] + weight[i + 2] * src[rightIdx[i + 2]];
    *dst++ = ((1 << shift) - weight[i + 3]) * src[leftIdx[i + 3]] + weight[i + 3] * src[rightIdx[i + 3]];

  }

  for (; i < len; ++i) {
    *dst++ = ((1 << shift) - weight[i]) * src[leftIdx[i]] + weight[i] * src[rightIdx[i]];
  }
}

static void _ieInterpImageBilinear8UC1_Ver3(Image* src, Image* dst)
{
  int i, j;
  int sw, sh, sstep;
  int dw, dh, dstep;
  unsigned char* sdata, *ddata;
  float horScaleRatio, verScaleRatio;
  long* rowBuf1, *rowBuf2;
  long* upLinePtr, *downLinePtr, *tempPtr;
  long* horWeight;
  int* horLeftIdx, *horRightIdx;
  int preVerUpIdx, preVerDownIdx;
  int shift = 8;

  sw = src->w;
  sh = src->h;
  sstep = src->step;
  sdata = (unsigned char*)(src->tt.data);
  dw = dst->w;
  dh = dst->h;
  dstep = dst->step;
  ddata = (unsigned char*)(dst->tt.data);

  horScaleRatio = sw / (float)(dw);
  verScaleRatio = sh / (float)(dh);

  rowBuf1 = new long[dw];
  rowBuf2 = new long[dw];
  horWeight = new long[dw];
  horLeftIdx = new int[dw];
  horRightIdx = new int[dw];


  //col interpolation

  //����Ŀ��ͼ�����غ���������ھ���ţ���Ȩ�ء�
  for (i = 0; i < dw; i++) {
    float pos = (i + 0.5f) * horScaleRatio;
    horLeftIdx[i] = (int)(IET_MAX(pos - 0.5f, 0));
    horRightIdx[i] = (int)(IET_MIN(pos + 0.5f, sw - 1));
    horWeight[i] = (long)(fabs(pos - 0.5f - horLeftIdx[i]) * (1 << shift));
  }

  preVerUpIdx = -1;
  preVerDownIdx = -1;
  upLinePtr = rowBuf1;
  downLinePtr = rowBuf2;

  for (j = 0; j < dh; j++) {
    float pos = (j + 0.5f) * verScaleRatio;
    int verUpIdx = (int)(IET_MAX(pos - 0.5f, 0));
    int verDownIdx = (int)(IET_MIN(pos + 0.5f, sh - 1));
    long verWeight = (long)(fabs(pos - 0.5f - verUpIdx) * (1 << shift));

    if (verUpIdx == preVerUpIdx && verDownIdx == preVerDownIdx) {
      ;
      //do nothing
    }
    else if (verUpIdx == preVerDownIdx) {
      IET_SWAP(upLinePtr, downLinePtr, tempPtr);
      _ieInterpImageBilinear8UC1_Ver3_RowFilter(sdata + sstep * verDownIdx, downLinePtr, dw, horLeftIdx, horRightIdx, horWeight, shift);
    }
    else {
      _ieInterpImageBilinear8UC1_Ver3_RowFilter(sdata + sstep * verUpIdx,   upLinePtr, dw, horLeftIdx, horRightIdx, horWeight, shift);
      _ieInterpImageBilinear8UC1_Ver3_RowFilter(sdata + sstep * verDownIdx, downLinePtr, dw, horLeftIdx, horRightIdx, horWeight, shift);
    }

    unsigned char* _ptr = ddata + dstep * j;

    for (i = 0; i < dw - 4; i += 4) {
      *_ptr++ = (unsigned char)((((1 << shift) - verWeight) * upLinePtr[i] + verWeight * downLinePtr[i]) >> (2 * shift));
      *_ptr++ = (unsigned char)((((1 << shift) - verWeight) * upLinePtr[i + 1] + verWeight * downLinePtr[i + 1]) >> (2 * shift));
      *_ptr++ = (unsigned char)((((1 << shift) - verWeight) * upLinePtr[i + 2] + verWeight * downLinePtr[i + 2]) >> (2 * shift));
      *_ptr++ = (unsigned char)((((1 << shift) - verWeight) * upLinePtr[i + 3] + verWeight * downLinePtr[i + 3]) >> (2 * shift));
    }

    for (; i < dw; i++) {
      *_ptr++ = (unsigned char)((((1 << shift) - verWeight) * upLinePtr[i] + verWeight * downLinePtr[i]) >> (2 * shift));
    }

    preVerUpIdx = verUpIdx;
    preVerDownIdx = verDownIdx;
  }

  delete []rowBuf1;
  delete []rowBuf2;
  delete []horWeight;
  delete []horLeftIdx;
  delete []horRightIdx;
}
