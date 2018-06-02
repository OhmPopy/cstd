#include "Core.h"



void* IS_AllocMemory(unsigned int Size, bool ZeroMemory)
{
  void* Ptr = malloc(Size);               //      ������ֻ���������������뽫�˺�����Ϊmalloc֮��ģ���PC��������Ҫ��ʵ���ڴ������ֽڶ��롣
  if (Ptr != NULL)
    if (ZeroMemory == true) {
      memset(Ptr, 0, Size);
    }
  return Ptr;
}
void IS_FreeMemory(void* Ptr)
{
  if (Ptr != NULL) {
    free(Ptr);
  }
}
/// <summary>
/// ���ݾ���Ԫ�ص���������ȡһ��Ԫ��ʵ��ռ�õ��ֽ���
/// </summary>
/// <param name="Depth">���ݾ���Ԫ�ص����͡�</param>
/// <remarks> ֻ��Ϊ�ڲ�ʹ�á�</remarks>
int IS_ELEMENT_SIZE(int Depth)
{
  int Size;
  switch (Depth) {
  case IS_DEPTH_8U:
    Size = sizeof(unsigned char);
    break;
  case IS_DEPTH_8S:
    Size = sizeof(char);
    break;
  case IS_DEPTH_16S:
    Size = sizeof(short);
    break;
  case IS_DEPTH_32S:
    Size = sizeof(int);
    break;
  case IS_DEPTH_32F:
    Size = sizeof(float);
    break;
  case IS_DEPTH_64F:
    Size = sizeof(double);
    break;
  default:
    Size = 0;
    break;
  }
  return Size;
}
/// <summary>
/// �����µľ������ݡ�
/// </summary>
/// <param name="Width">����Ŀ�ȡ�</param>
/// <param name="Height">����ĸ߶ȡ�</param>
/// <param name="Depth">�����Ԫ�����͡�</param>
/// <param name="Channel">�����ͨ������</param>
/// <param name="Matrix">���صľ������</param>
/// <returns>����ֵΪ0��ʾ�ɹ���������ʾʧ�ܡ�</returns>
IS_RET IS_CreateMatrix(int Width, int Height, int Depth, int Channel, TMatrix** Matrix)
{
  if (Width < 1 || Height < 1) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  if (Depth != IS_DEPTH_8U && Depth != IS_DEPTH_8S && Depth != IS_DEPTH_16S && Depth != IS_DEPTH_32S && Depth != IS_DEPTH_32F && Depth != IS_DEPTH_64F) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  if (Channel != 1 && Channel != 2 && Channel != 3 && Channel != 4) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  *Matrix = (TMatrix*)IS_AllocMemory(sizeof(TMatrix));                                    //      ��Ĳ���������������з��䲻�ɹ���ʱ��
  (*Matrix)->Width = Width;
  (*Matrix)->Height = Height;
  (*Matrix)->Depth = Depth;
  (*Matrix)->Channel = Channel;
  (*Matrix)->WidthStep = WIDTHBYTES(Width * Channel * IS_ELEMENT_SIZE(Depth));
  (*Matrix)->Data = (unsigned char*)IS_AllocMemory((*Matrix)->Height * (*Matrix)->WidthStep, true);
  if ((*Matrix)->Data == NULL) {
    IS_FreeMemory(*Matrix);
    return IS_RET_ERR_OUTOFMEMORY;
  }
  (*Matrix)->Reserved = 0;
  return IS_RET_OK;
}
/// <summary>
/// �ͷŴ����ľ���
/// </summary>
/// <param name="Matrix">��Ҫ�ͷŵľ������</param>
/// <returns>����ֵΪ0��ʾ�ɹ���������ʾʧ�ܡ�</returns>
IS_RET IS_FreeMatrix(TMatrix** Matrix)
{
  if ((*Matrix) == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if ((*Matrix)->Data == NULL) {
    IS_FreeMemory((*Matrix));
    return IS_RET_ERR_OUTOFMEMORY;
  }
  else {
    IS_FreeMemory((*Matrix)->Data);                 //      ע���ͷŵ�˳��
    IS_FreeMemory((*Matrix));
    return IS_RET_OK;
  }
}
/// <summary>
/// ��¡���еľ���
/// </summary>
/// <param name="Src">����¡�ľ������</param>
/// <param name="Dest">��¡��ľ������</param>
/// <returns>����ֵΪ0��ʾ�ɹ���������ʾʧ�ܡ�</returns>
IS_RET IS_CloneMatrix(TMatrix* Src, TMatrix** Dest)
{
  if (Src == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  IS_RET Ret = IS_CreateMatrix(Src->Width, Src->Height, Src->Depth, Src->Channel, Dest);
  if (Ret == IS_RET_OK) {
    memcpy((*Dest)->Data, Src->Data, (*Dest)->Height * (*Dest)->WidthStep);
  }
  return Ret;
}

