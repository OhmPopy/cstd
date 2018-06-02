
#include "img/imgio.inl"

#include <direct.h>

int test_tif()
{
  char* szFileName;
  img_t im[1] = {0};

  _chdir("E:/pub/bin/pdf");

  szFileName = "SSS00.tif";
  szFileName = "1.tif";
  szFileName = "14-08.TIF";
  szFileName = "009.TIF";
  szFileName = "22.tif";
  szFileName = "2.tif";
  tif_load_filename(szFileName, 0, im);
  return 0;
}
#if 0
int test_tif()
{
  int nTotalFrame;
  char* dtitle;
  char* szFileName;
  TIFF* tiff = NULL;
  img_t im[1] = {0};
  int width, height;
  float resolution;
  uint16 bitspersample;
  uint16 samplesperpixel;
  uint16 bitsperpixel;
  DWORD dwBytePerLine;
  DWORD64 dwLeng;
  BYTE* pData;
  uint32* raster;
  uint32* row;
  LPBYTE bits2;
  int x, y;

  _chdir("E:/pub/bin/pdf");

  szFileName = "SSS00.tif";
  szFileName = "1.tif";
  szFileName = "14-08.TIF";
  szFileName = "009.TIF";
  szFileName = "22.tif";
  szFileName = "2.tif";
  tif_load_filename(szFileName, 0, im);
  //return 0;
  tiff = TIFFOpen(szFileName, "r");//��Tiff�ļ����õ�ָ�룬�Ժ����еĲ�����ͨ��ָ�����
  nTotalFrame = TIFFNumberOfDirectories(tiff);    //�õ�ͼ�����֡��

  //TIFFSetDirectory(tiff,0);
  //���Ǵ򿪵�һ��ͼ��Ҳ���ǵ�0֡������ǵ�1֡���ڶ�������д1���ɴ����ơ���ΪWindows��ͼ�����
  //����������BMP��ʽ���У����Ƕ�����֡��ת��BMP��ʽ��

  TIFFGetField(tiff, TIFFTAG_PAGENAME, &dtitle);
  //�õ���֡�����֣������dtitle�С�

  TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);  //�õ����
  TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);//�õ��߶�

  resolution = (float)MAX(width, height);

  bitspersample = 1;
  samplesperpixel = 1;

  TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
  //ÿ������ռ���ٻ����֣�24λͼsamplesperpixelӦ�õ���3��
  TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bitspersample);
  //ÿһ�������ֳ�������ӦΪ8��

  bitsperpixel = bitspersample * samplesperpixel;
  //���ÿ������ռ����bit��24λͼ��ֵΪ24
  dwBytePerLine = (width * bitsperpixel + 31) / 32 * 4;
  //�����漸���������ͼ��ÿ����ռ�ֽ�(BYTE)����

  dwLeng = height * dwBytePerLine; //���ڴ�������֡ͼ����������Ҫ�ĳ���
  pData = MALLOC(BYTE, dwLeng);    //Ϊ������ݷ����ڴ�ռ�


  raster = (uint32*)malloc(width * height * sizeof(uint32));
  TIFFReadRGBAImage(tiff, width, height, (uint32*)pData, 1);
  //���ϼ��ж�����֡����,���浽raster�С�

  row = &raster[0];
  bits2 = pData;

  for (y = 0; y < height; y++) {
    LPBYTE bits = bits2;

    for (x = 0; x < width; x++) {
      *bits++ = (BYTE)TIFFGetB(row[x]);
      *bits++ = (BYTE)TIFFGetG(row[x]);
      *bits++ = (BYTE)TIFFGetR(row[x]);
    }

    row += width;
    bits2 += dwBytePerLine;
  }

  _TIFFfree(raster);
#if 0
#endif
  return 0;
}
#endif
