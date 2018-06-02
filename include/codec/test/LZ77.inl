// lz.cpp : Defines the entry point for the console application.
//


//#include <windows.h>
#include <stdio.h>
#include <memory.h>

#ifndef _WIX_LZ77_COMPRESS_HEADER_001_
#define _WIX_LZ77_COMPRESS_HEADER_001_
#endif

// �������ڵ��ֽڴ�С
#define _MAX_WINDOW_SIZE  65536

// �Ի���������ÿһ��2�ֽڴ�����
// ������Ϊ�˽��п�������ƥ��
// ����ķ�������һ��64k��С��ָ������
// �����±����ζ�Ӧÿһ��2�ֽڴ���(00 00) (00 01) ... (01 00) (01 01) ...
// ÿһ��ָ��ָ��һ�����������еĽڵ�Ϊ��2�ֽڴ���ÿһ������λ��
typedef struct STIDXNODE {
  WORD off;    // ��src�е�ƫ��
  WORD off2;    // ���ڶ�Ӧ��2�ֽڴ�Ϊ�ظ��ֽڵĽڵ㣬ָ�� off �� off2 ����Ӧ�˸�2�ֽڴ�
  WORD next;    // ��SortHeap�е�ָ��
} STIDXNODE;

unsigned char* pWnd;
// ���ڴ�С���Ϊ 64k �����Ҳ�������
// ÿ�����ֻѹ�� 64k ���ݣ��������Է�����ļ��м俪ʼ��ѹ
// ��ǰ���ڵĳ���
int nWndSize;
  
WORD SortTable[65536];  // 256 * 256 ָ��SortHeap���±��ָ��

// ��Ϊ���ڲ�������û��ɾ���ڵ�Ĳ���������
// �ڵ������SortHeap ����������
STIDXNODE SortHeap[_MAX_WINDOW_SIZE];
int HeapPos;  // ��ǰ����λ��

  // ��ǰ���λ��(�ֽ�ƫ�Ƽ�λƫ��)
int CurByte, CurBit;

/////////////////////////////////////////////////////////
// ȡlog2(n)��upper_bound
int UpperLog2(int n)
{
  int i = 0;
  if (n > 0)
  {
    int m = 1;
    while(1)
    {
      if (m >= n)
        return i;
      m <<= 1;
      i++;
    }
  }
  else 
    return -1;
}
// UpperLog2
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// ȡlog2(n)��lower_bound
int LowerLog2(int n)
{
  int i = 0;
  if (n > 0)
  {
    int m = 1;
    while(1)
    {
      if (m == n)
        return i;
      if (m > n)
        return i - 1;
      m <<= 1;
      i++;
    }
  }
  else 
    return -1;
}
// LowerLog2
/////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// ��λָ��*piByte(�ֽ�ƫ��), *piBit(�ֽ���λƫ��)����numλ
void MovePos(int* piByte, int* piBit, int num)
{
  num += (*piBit);
  (*piByte) += num / 8;
  (*piBit) = num % 8;
}
// MovePos
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// �õ��ֽ�byte��posλ��ֵ
//    pos˳��Ϊ��λ���0����������
unsigned char GetBit(unsigned char byte, int pos)
{
  int j = 1;
  j <<= 7 - pos;
  if (byte & j)
    return 1;
  else 
    return 0;
}
// GetBit
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// ����byte�ĵ�iBitλΪaBit
//    iBit˳��Ϊ��λ���0����������
void SetBit(unsigned char* byte, int iBit, unsigned char aBit)
{
  if (aBit)
    (*byte) |= (1 << (7 - iBit));
  else
    (*byte) &= ~(1 << (7 - iBit));
}
// SetBit
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// ��DWORDֵ�Ӹ�λ�ֽڵ���λ�ֽ�����
void InvertDWord(DWORD* pDW)
{
  typedef union UDWORD { DWORD dw; unsigned char b[4]; } UDWORD;
  UDWORD* pUDW = (UDWORD*)pDW;
  unsigned char b;
  b = pUDW->b[0];  pUDW->b[0] = pUDW->b[3]; pUDW->b[3] = b;
  b = pUDW->b[1];  pUDW->b[1] = pUDW->b[2]; pUDW->b[2] = b;
}
// InvertDWord
//////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// CopyBitsInAByte : ��һ���ֽڷ�Χ�ڸ���λ��
// ��������ͬ CopyBits �Ĳ���
// ˵����
//    �˺����� CopyBits ���ã����������飬��
//    �ٶ�Ҫ���Ƶ�λ����һ���ֽڷ�Χ��
void CopyBitsInAByte(unsigned char* memDest, int nDestPos, const unsigned char* memSrc, int nSrcPos, int nBits)
{
  unsigned char b1, b2;
  b1 = *memSrc;
  b1 <<= nSrcPos; b1 >>= 8 - nBits;  // �����ø��Ƶ�λ��0
  b1 <<= 8 - nBits - nDestPos;    // ��Դ��Ŀ���ֽڶ���
  *memDest |= b1;    // ����ֵΪ1��λ
  b2 = 0xff; b2 <<= 8 - nDestPos;    // �����ø��Ƶ�λ��1
  b1 |= b2;
  b2 = 0xff; b2 >>= nDestPos + nBits;
  b1 |= b2;
  *memDest &= b1;    // ����ֵΪ0��λ
}
// CopyBitsInAByte
/////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
// CopyBits : �����ڴ��е�λ��
//    memDest - Ŀ��������
//    nDestPos - Ŀ����������һ���ֽ��е���ʼλ
//    memSrc - Դ������
//    nSrcPos - Դ��������һ���ֽڵ�����ʼλ
//    nBits - Ҫ���Ƶ�λ��
//  ˵����
//    ��ʼλ�ı�ʾԼ��Ϊ���ֽڵĸ�λ����λ���������ң�
//    ����Ϊ 0��1��... , 7
//    Ҫ���Ƶ������������������غ�
void CopyBits(unsigned char* memDest, int nDestPos, const unsigned char* memSrc, int nSrcPos, int nBits)
{
  int iByteDest = 0, iBitDest;
  int iByteSrc = 0, iBitSrc = nSrcPos;

  int nBitsToFill, nBitsCanFill;

  while (nBits > 0)
  {
    // ����Ҫ��Ŀ������ǰ�ֽ�����λ��
    nBitsToFill = MIN(nBits, iByteDest ? 8 : 8 - nDestPos);
    // Ŀ������ǰ�ֽ�Ҫ������ʼλ
    iBitDest = iByteDest ? 0 : nDestPos;
    // �������һ�δ�Դ�������и��Ƶ�λ��
    nBitsCanFill = MIN(nBitsToFill, 8 - iBitSrc);
    // �ֽ��ڸ���
    CopyBitsInAByte(memDest + iByteDest, iBitDest, memSrc + iByteSrc, iBitSrc, nBitsCanFill);    
    // �����û�и����� nBitsToFill ��
    if (nBitsToFill > nBitsCanFill)
    {
      iByteSrc++; iBitSrc = 0; iBitDest += nBitsCanFill;
      CopyBitsInAByte(memDest + iByteDest, iBitDest, 
          memSrc + iByteSrc, iBitSrc, 
          nBitsToFill - nBitsCanFill);
      iBitSrc += nBitsToFill - nBitsCanFill;
    }
    else 
    {
      iBitSrc += nBitsCanFill;
      if (iBitSrc >= 8)
      {
        iByteSrc++; iBitSrc = 0;
      }
    }

    nBits -= nBitsToFill;  // �Ѿ������nBitsToFillλ
    iByteDest++;
  }  
}
// CopyBits
/////////////////////////////////////////////////////////



// ��ʼ���������ͷ��ϴ�ѹ���õĿռ�
void LZ77_InitSortTable()
{
  memset(SortTable, 0, sizeof(WORD) * 65536);
  nWndSize = 0;
  HeapPos = 1;
}

// �����������һ��2�ֽڴ�
void LZ77_InsertIndexItem(int off)
{
  WORD q;
  unsigned char ch1, ch2;
  ch1 = pWnd[off]; ch2 = pWnd[off + 1];  
  
  if (ch1 != ch2)
  {
    // �½��ڵ�
    q = HeapPos;
    HeapPos++;
    SortHeap[q].off = off;
    SortHeap[q].next = SortTable[ch1 * 256 + ch2];
    SortTable[ch1 * 256 + ch2] = q;
  }
  else
  {
    // ���ظ�2�ֽڴ�
    // ��Ϊû������ƫ��Ҳû��ɾ��������ֻҪ�Ƚϵ�һ���ڵ�
    // �Ƿ�� off �����Ӽ���
    q = SortTable[ch1 * 256 + ch2];
    if (q != 0 && off == SortHeap[q].off2 + 1)
    {    
      // �ڵ�ϲ�
      SortHeap[q].off2 = off;
    }    
    else
    {
      // �½��ڵ�
      q = HeapPos;
      HeapPos++;
      SortHeap[q].off = off;
      SortHeap[q].off2 = off;
      SortHeap[q].next = SortTable[ch1 * 256 + ch2];
      SortTable[ch1 * 256 + ch2] = q;
    }
  }
}

//////////////////////////////////////////
// ���������һ���n���ֽ�
void LZ77_ScrollWindow(int n)
{  
  int i;
  for (i = 0; i < n; i++)
  {    
    nWndSize++;    
    if (nWndSize > 1)      
      LZ77_InsertIndexItem(nWndSize - 2);
  }
}

///////////////////////////////////////////////////////////
// �õ��Ѿ�ƥ����2���ֽڵĴ���λ��offset
// ����ƥ����ٸ��ֽ�
int LZ77_GetSameLen(const unsigned char* src, int srclen, int nSeekStart, int offset)
{
  int i = 2; // �Ѿ�ƥ����2���ֽ�
  int maxsame = MIN(srclen - nSeekStart, nWndSize - offset);
  while (i < maxsame && src[nSeekStart + i] == pWnd[offset + i]) {
    i++;
  }
  if(!(nSeekStart + i <= srclen && offset + i <= nWndSize))
    return i;
  return  0;
}

///////////////////////////////////////////////////////////
// �ڻ��������в�������
// nSeekStart - �Ӻδ���ʼƥ��
// offset, len - ���ڽ��ս������ʾ�ڻ��������ڵ�ƫ�ƺͳ���
// ����ֵ- �Ƿ�鵽����Ϊ2��2���ϵ�ƥ���ֽڴ�
BOOL LZ77_SeekPhase(const unsigned char* src, int srclen, int nSeekStart, int* offset, int* len)
{  
  int j, m, n;

  if (nSeekStart < srclen - 1)
  {
    unsigned char ch1, ch2;
    WORD p;
    ch1 = src[nSeekStart]; ch2 = src[nSeekStart + 1];
    p = SortTable[ch1 * 256 + ch2];
    if (p != 0)
    {
      m = 2; n = SortHeap[p].off;
      while (p != 0)
      {
        j = LZ77_GetSameLen(src, srclen, nSeekStart, SortHeap[p].off);
        if ( j > m )
        { 
          m = j; 
          n = SortHeap[p].off;
        }      
        p = SortHeap[p].next;
      }  
      (*offset) = n; 
      (*len) = m;
      return TRUE;    
    }  
  }
  return FALSE;
}

////////////////////////////////////////
// ���ѹ����
// code - Ҫ�������
// bits - Ҫ�����λ��(��isGamma=TRUEʱ��Ч)
// isGamma - �Ƿ����Ϊ�ñ���
void LZ77_OutCode(unsigned char* dest, DWORD code, int bits, BOOL isGamma)
{  
  if ( isGamma )
  {
    unsigned char* pb;
    DWORD out;
    // �������λ��
    int GammaCode = (int)code - 1;
    int q = LowerLog2(GammaCode);
    if (q > 0)
    {
      out = 0xffff;
      pb = (unsigned char*)&out;
      // ���q��1
      CopyBits(dest + CurByte, CurBit, 
        pb, 0, q);
      MovePos(&CurByte, &CurBit, q);
    }
    // ���һ��0
    out = 0;
    pb = (unsigned char*)&out;    
    CopyBits(dest + CurByte, CurBit, pb + 3, 7, 1);
    MovePos(&CurByte, &CurBit, 1);
    if (q > 0)
    {
      // �������, qλ
      int sh = 1;
      sh <<= q;
      out = GammaCode - sh;
      pb = (unsigned char*)&out;
      InvertDWord(&out);
      CopyBits(dest + CurByte, CurBit, 
        pb + (32 - q) / 8, (32 - q) % 8, q);
      MovePos(&CurByte, &CurBit, q);
    }
  }
  else 
  {
    DWORD dw = (DWORD)code;
    unsigned char* pb = (unsigned char*)&dw;
    InvertDWord(&dw);
    CopyBits(dest + CurByte, CurBit, 
        pb + (32 - bits) / 8, (32 - bits) % 8, bits);
    MovePos(&CurByte, &CurBit, bits);
  }
}




/////////////////////////////////////////////
// ѹ��һ���ֽ���
// src - Դ������
// srclen - Դ�������ֽڳ���
// dest - ѹ��������������ǰ����srclen+5�ֽ��ڴ�
// ����ֵ > 0 ѹ�����ݳ���
// ����ֵ = 0 �����޷�ѹ��
// ����ֵ < 0 ѹ�����쳣����
int LZ77_Encode(const void* src0, int srclen, void* dest0)
{
  int i, destlen, off, len;
  const unsigned char* src = (const unsigned char*)src0;
  unsigned char* dest = (unsigned char*)dest0;

  CurByte = 0; 
  CurBit = 0;  
  if (srclen > 65536) 
    return -1;

  pWnd = (uchar*)src;
  LZ77_InitSortTable();
  for (i = 0; i < srclen; i++)
  {    
    if (CurByte >= srclen)
      return 0;
    if (LZ77_SeekPhase(src, srclen, i, &off, &len))
    {      
      // ���ƥ������ flag(1bit) + len(�ñ���) + offset(���16bit)
      LZ77_OutCode(dest, 1, 1, FALSE);
      LZ77_OutCode(dest, len, 0, TRUE);

      // �ڴ��ڲ���64k��Сʱ������Ҫ16λ�洢ƫ��
      LZ77_OutCode(dest, off, UpperLog2(nWndSize), FALSE);
            
      LZ77_ScrollWindow(len);
      i += len - 1;
    }
    else
    {
      // ���������ƥ���ַ� 0(1bit) + char(8bit)
      LZ77_OutCode(dest, 0, 1, FALSE);
      LZ77_OutCode(dest, (DWORD)(src[i]), 8, FALSE);
      LZ77_ScrollWindow(1);
    }
  }
  destlen = CurByte + ((CurBit) ? 1 : 0);
  if (destlen >= srclen)
    return 0;
  return destlen;
}

/////////////////////////////////////////////
// ��ѹ��һ���ֽ���
// src - ����ԭʼ���ݵ��ڴ���
// srclen - Դ�������ֽڳ���
// dest - ѹ��������
// ����ֵ - �ɹ����
BOOL LZ77_Decode(const void* src0, int srclen, void* dest0)
{
  int i;
  const unsigned char* src = (const unsigned char*)src0;
  unsigned char* dest = (unsigned char*)dest0;
  CurByte = 0; CurBit = 0;
  pWnd = (uchar*)src;    // ��ʼ������
  nWndSize = 0;

  if (srclen > 65536) 
    return FALSE;
  
  for (i = 0; i < srclen; i++) {
    unsigned char b = GetBit(dest[CurByte], CurBit);
    MovePos(&CurByte, &CurBit, 1);
    if (b == 0) // �����ַ�
    {
      CopyBits(dest + CurByte, CurBit, src + i, 0, 8);
      MovePos(&CurByte, &CurBit, 8);
      nWndSize++;
    }
    else    // �����ڵ�����
    {
      int j, bits, len, off, q = -1;
      DWORD dw;
      unsigned char* pb;
      while (b != 0)
      {
        q++;
        b = GetBit(dest[CurByte], CurBit);
        MovePos(&CurByte, &CurBit, 1);        
      }
      dw = 0;
      if (q > 0)
      {        
        pb = (unsigned char*)&dw;
        CopyBits(pb + (32 - q) / 8, (32 - q) % 8, dest + CurByte, CurBit, q);
        MovePos(&CurByte, &CurBit, q);
        InvertDWord(&dw);
        len = 1;
        len <<= q;
        len += dw;
        len += 1;
      }
      else
        len = 2;

      // �ڴ��ڲ���64k��Сʱ������Ҫ16λ�洢ƫ��
      dw = 0;
      pb = (unsigned char*)&dw;
      bits = UpperLog2(nWndSize);
      CopyBits(pb + (32 - bits) / 8, (32 - bits) % 8, dest + CurByte, CurBit, bits);
      MovePos(&CurByte, &CurBit, bits);
      InvertDWord(&dw);
      off = (int)dw;
      // �������
      for (j = 0; j < len; j++) {
        if((i + j <  srclen)&&(off + j <  _MAX_WINDOW_SIZE)) {
          dest[i + j] = pWnd[off + j];
        }
      }
      nWndSize += len;
      i += len - 1;
    }
    // ��������
    if (nWndSize > _MAX_WINDOW_SIZE) {
      pWnd += nWndSize - _MAX_WINDOW_SIZE;
      nWndSize = _MAX_WINDOW_SIZE;      
    }
  }

  return TRUE;
}
int test_LZ77()
{
  long soulen;
  unsigned char soubuf[65536];
  unsigned char destbuf[65536 + 16];
  
  FILE* in;
  FILE* out;
  WORD flag1, flag2;
  if (argc != 4)
  {
    puts("Usage: ");
    printf("    Compress : %s c sourcefile destfile\n", argv[0]);
    printf("  DeLZ77_Encode : %s d sourcefile destfile\n", argv[0]);
    return 0;
  }

  //��Դ�ļ�
  in = fopen(argv[2], "rb");
  if (in == NULL)
  {
    puts("Can't open source file");
    return 0;
  }
  //��Ŀ���ļ�
  out = fopen(argv[3], "wb");
  if (out == NULL)
  {
    puts("Can't open dest file");
    fclose(in);
    return 0;
  }
  fseek(in, 0, SEEK_END);
  soulen = ftell(in);
  fseek(in, 0, SEEK_SET);

  
  if (argv[1][0] == 'c') // ��Դ�ļ�����ѹ��
  {
    int destlen, last = soulen, act;
    while ( last > 0 )
    {
      act = MIN(65536, last);
      fread(soubuf, act, 1, in);
      last -= act;
      if (act == 65536)      // ����65536�ֽ�
        flag1 = 0;    
      else          
        flag1 = act;
      fwrite(&flag1, sizeof(WORD), 1, out);

      destlen = LZ77_Encode((unsigned char*)soubuf, act, (unsigned char*)destbuf);
      if (destlen == 0)    // ����ѹ����
      {
        flag2 = flag1;
        fwrite(&flag2, sizeof(WORD), 1, out);
        fwrite(soubuf, act, 1, out);
      }
      else
      {
        flag2 = (WORD)destlen;
        fwrite(&flag2, sizeof(WORD), 1, out);        
        fwrite(destbuf, destlen, 1, out);        
      }
    }
  }
  else if (argv[1][0] == 'd') // ��ѹ��Դ�ļ�
  {
    int last = soulen, act;
    while (last > 0)
    {
      fread(&flag1, sizeof(WORD), 1, in);
      fread(&flag2, sizeof(WORD), 1, in);
      last -= 2 * sizeof(WORD);
      if (flag1 == 0)
        act = 65536;
      else
        act = flag1;
      last-= flag2 ? (flag2) : act;

      if (flag2 == flag1)
      {
        fread(soubuf, act, 1, in);        
      }
      else
      {
        fread(destbuf, flag2, 1, in);
        if (!LZ77_Decode((unsigned char*)soubuf, act, (unsigned char*)destbuf))
        {
          puts("DeLZ77_Encode error");
          fclose(in);
          fclose(out);
          return 0;
        }
      }
      fwrite((unsigned char*)soubuf, act, 1, out);        
    }
  }
  else
  {
    puts("Usage: ");
    printf("  Compress : %s c sourcefile destfile\n", argv[0]);
    printf("  DeLZ77_Encode : %s d sourcefile destfile\n", argv[0]);    
  }
  //�ر�Դ�ļ���Ŀ���ļ�
  fclose(in);
  fclose(out);
  return 0;
}

#undef _MAX_WINDOW_SIZE

