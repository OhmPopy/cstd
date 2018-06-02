/********************************************************************
 pct.c
 ���ļ���;�� Apple PICTͼ���дģ��ʵ���ļ�
 ��ȡ���ܣ��ɶ�ȡ8��32λPCTͼ�󣬵����е�ʸ���ɷݽ�������
 ���湦�ܣ��ݲ�֧��
 ���ļ���д�ˣ�
 YZ yzfree##yeah.net
 ���ļ��汾�� 30623
 ����޸��ڣ� 2003-06-23
 ----------------------------------------------------------------
 ������ʷ��
 2003-6 ����16λPICTͼ�Ľ���
 2003-6 ���Ӷ�PICT�汾1��֧��
 ����֧��PCTDEF_DIRBITSRGN��PCTDEF_BITSRECT��PCTDEF_BITSRGN��PCTDEF_PACKBITSRGN��ǩ��ͼ��Ķ�ȡ
 ����֧�ֶ�ȡȫ��λ���ͼ��(���� mssoft#sina.com)
 2003-4 ֧��ͼ���д�����ṩ�����ػ���������
 2002-8 ����ӿ�������2.2�������ڴ�������
 2002-5 ��һ�η������°棩
********************************************************************/


/* PICT ΢�����붨�壨�����ڰ汾2����չ�汾2�� */
#define PCTDEF_NOP      0x0000      /* No operation 0 */
#define PCTDEF_CLIP     0x0001      /* Clipping region Region size */
#define PCTDEF_BKPAT    0x0002      /* BkPat Background pattern 8 */
#define PCTDEF_TXFONT   0x0003      /* TxFont Font number for text (Integer) 2 */
#define PCTDEF_TXFACE   0x0004      /* TxFace Text's font style (0..255) 1 */
#define PCTDEF_TXMODE   0x0005      /* TxMode Source mode (Integer) 2 */
#define PCTDEF_SPEXTRA    0x0006      /* SpExtra Extra space (Fixed) 4 */
#define PCTDEF_PNSIZE   0x0007      /* PnSize Pen size (Point) 4 */
#define PCTDEF_PNMODE   0x0008      /* PnMode Pen mode (Integer) 2 */
#define PCTDEF_PNPAT    0x0009      /* PnPat Pen pattern 8 */
#define PCTDEF_FILLPAT    0x000A      /* FillPat Fill pattern 8 */
#define PCTDEF_OVSIZE   0x000B      /* OvSize Oval size (Point) 4 */
#define PCTDEF_ORIGIN   0x000C      /* Origin dh, dv (Integer) 4 */
#define PCTDEF_TXSIZE   0x000D      /* TxSize Text size (Integer) 2 */
#define PCTDEF_FGCOLOR    0x000E      /* FgColor Foreground color (Long) 4 */
#define PCTDEF_BKCOLOR    0x000F      /* BkColor Background color (Long) 4 */
#define PCTDEF_TXRATIO    0x0010      /* TxRatio Numerator (Point), denominator (Point) 8 */
#define PCTDEF_VERSIONOP  0x0011      /* VersionOp Version (0..255) 1 */
#define PCTDEF_BKPIXPAT   0x0012      /* BkPixPat Background pixel pattern Variable; see Listing A-1 on page A-17 */
#define PCTDEF_PNPIXPAT   0x0013      /* PnPixPat Pen pixel pattern Variable; see Listing A-1 on page A-17 */
#define PCTDEF_FILLPIXPAT 0x0014      /* FillPixPat Fill pixel pattern Variable; see Listing A-1 on page A-17 */
#define PCTDEF_PNLOCHFRAC 0x0015      /* PnLocHFrac Fractional pen position (Integer�low word of Fixed); 2*/
#define PCTDEF_CHEXTRA    0x0016      /* ChExtra Added width for nonspace characters (Integer) 2*/
#define PCTDEF_REV0017    0x0017      /* Reserved for Apple use Not determined */
#define PCTDEF_REV0018    0x0018      /* Reserved for Apple use Not determined */
#define PCTDEF_REV0019    0x0019      /* Reserved for Apple use Not determined */
#define PCTDEF_RGBFGCOL   0x001A      /* RGBFgCol Foreground color (RGBColor) 6 */
#define PCTDEF_RGBBKCOL   0x001B      /* RGBBkCol Background color (RGBColor) 6 */
#define PCTDEF_HILITEMODE 0x001C      /* HiliteMode Highlight mode flag: no data; this opcode is sent before a drawing operation that uses the highlight mode 0 */
#define PCTDEF_HILITECOLOR  0x001D      /* HiliteColor Highlight color (RGBColor) 6 */
#define PCTDEF_DEFHILITE  0x001E      /* DefHilite Use default highlight color; no data; set highlight to default (from low memory) 0 */
#define PCTDEF_OPCOLOR    0x001F      /* OpColor Opcolor (RGBColor) 6 */
#define PCTDEF_LINE     0x0020      /* Line pnLoc (Point), newPt (Point) 8 */
#define PCTDEF_LINEFROM   0x0021      /* LineFrom newPt (Point) 4 */
#define PCTDEF_SHORTLINE  0x0022      /* ShortLine pnLoc (Point), dh (?28..127), dv (?28..127) 6 */
#define PCTDEF_SHORTLINEFR  0x0023      /* ShortLineFrom dh (?28..127), dv (?28..127) 2 */
#define PCTDEF_REV0024    0x0024      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_REV0025    0x0025      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_REV0026    0x0026      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_REV0027    0x0027      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_LONGTEXT   0x0028      /* LongText txLoc (Point), count (0..255), text 5 + text */
#define PCTDEF_DHTEXT   0x0029      /* DHText dh (0..255), count (0..255), text 2 + text */
#define PCTDEF_DVTEXT   0x002A      /* DVText dv (0..255), count (0..255), text 2 + text */
#define PCTDEF_DHDVTEXT   0x002B      /* DHDVText dh (0..255), dv (0..255), count (0..255), text 3 + text */
#define PCTDEF_FONTNAME   0x002C      /* fontName Data length (Integer), old font ID (Integer), name length (0..255), font name* 5 + name length */
#define PCTDEF_LINEJUSTIFY  0x002D      /* lineJustify Operand data length (Integer), intercharacter spacing (Fixed), total extra space for justification (Fixed) 10 */
#define PCTDEF_GLYPHSTATE 0x002E      /* glyphState Data length (word), followed by these 1-byte Boolean values: outline preferred, preserve glyph, fractional widths, scaling disabled 8 */
#define PCTDEF_REV002F    0x002F      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_FRAMERECT  0x0030      /* frameRect Rectangle (Rect) 8 */
#define PCTDEF_PAINTRECT  0x0031      /* paintRect Rectangle (Rect) 8 */
#define PCTDEF_ERASERECT  0x0032      /* eraseRect Rectangle (Rect) 8 */
#define PCTDEF_INVERTRECT 0x0033      /* invertRect Rectangle (Rect) 8 */
#define PCTDEF_FILLRECT   0x0034      /* fillRect Rectangle (Rect) 8 */
#define PCTDEF_REV0035    0x0035      /* Reserved for Apple use 8 bytes of data 8 */
#define PCTDEF_REV0036    0x0036      /* Reserved for Apple use 8 bytes of data 8 */
#define PCTDEF_REV0037    0x0037      /* Reserved for Apple use 8 bytes of data 8 */
#define PCTDEF_FRAMESAMERT  0x0038      /* frameSameRect Rectangle (Rect) 0 */
#define PCTDEF_PAINTSAMERT  0x0039      /* paintSameRect Rectangle (Rect) 0 */
#define PCTDEF_ERASESAMERT  0x003A      /* eraseSameRect Rectangle (Rect) 0 */
#define PCTDEF_INVERTSAMERT 0x003B      /* invertSameRect Rectangle (Rect) 0 */
#define PCTDEF_FILLSAMERECT 0x003C      /* fillSameRect Rectangle (Rect) 0 */
#define PCTDEF_REV003D    0x003D      /* Reserved for Apple use 0 */
#define PCTDEF_REV003E    0x003E      /* Reserved for Apple use 0 */
#define PCTDEF_REV003F    0x003F      /* Reserved for Apple use 0 */
#define PCTDEF_FRAMERRECT 0x0040      /* frameRRect Rectangle (Rect)?8 */
#define PCTDEF_PAINTRRECT 0x0041      /* paintRRect Rectangle (Rect)?8 */
#define PCTDEF_ERASERRECT 0x0042      /* eraseRRect Rectangle (Rect)?8 */
#define PCTDEF_INVERTRRECT  0x0043      /* invertRRect Rectangle (Rect)?8 */
#define PCTDEF_FILLRRECT  0x0044      /* fillRRect Rectangle (Rect)?8 */
#define PCTDEF_REV0045    0x0045      /* Reserved for Apple use 8 bytes of data 8 */
#define PCTDEF_REV0046    0x0046      /* Reserved for Apple use 8 bytes of data 8 */
#define PCTDEF_REV0047    0x0047      /* Reserved for Apple use 8 bytes of data 8 */
#define PCTDEF_FRAMESAMERRT 0x0048      /* frameSameRRect Rectangle (Rect) 0 */
#define PCTDEF_PAINTSAMERRT 0x0049      /* paintSameRRect Rectangle (Rect) 0 */
#define PCTDEF_ERASESAMERRT 0x004A      /* eraseSameRRect Rectangle (Rect) 0 */
#define PCTDEF_INVSAMERRT 0x004B      /* invertSameRRect Rectangle (Rect) 0 */
#define PCTDEF_FILLSAMERRT  0x004C      /* fillSameRRect Rectangle (Rect) 0 */
#define PCTDEF_REV004D    0x004D      /* Reserved for Apple use 0 */
#define PCTDEF_REV004E    0x004E      /* Reserved for Apple use 0 */
#define PCTDEF_REV004F    0x004F      /* Reserved for Apple use 0 */
#define PCTDEF_FRAMEOVAL  0x0050      /* frameOval Rectangle (Rect) 8 */
#define PCTDEF_PAINTOVAL  0x0051      /* paintOval Rectangle (Rect) 8 */
#define PCTDEF_ERASEOVAL  0x0052      /* eraseOval Rectangle (Rect) 8 */
#define PCTDEF_INVERTOVAL 0x0053      /* invertOval Rectangle (Rect) 8 */
#define PCTDEF_FILLOVAL   0x0054      /* fillOval Rectangle (Rect) 8 */
#define PCTDEF_REV0055    0x0055      /* Reserved for Apple use 8 bytes of data 8 */
#define PCTDEF_REV0056    0x0056      /* Reserved for Apple use 8 bytes of data 8 */
#define PCTDEF_REV0057    0x0057      /* Reserved for Apple use 8 bytes of data 8 */
#define PCTDEF_FRAMESAMEOV  0x0058      /* frameSameOval Rectangle (Rect) 0 */
#define PCTDEF_PAINTSAMEOV  0x0059      /* paintSameOval Rectangle (Rect) 0 */
#define PCTDEF_ERASESAMEOV  0x005A      /* eraseSameOval Rectangle (Rect) 0 */
#define PCTDEF_INVERTSAMEOV 0x005B      /* invertSameOval Rectangle (Rect) 0 */
#define PCTDEF_FILLSAMEOV 0x005C      /* fillSameOval Rectangle (Rect) 0 */
#define PCTDEF_REV005D    0x005D      /* Reserved for Apple use 0 */
#define PCTDEF_REV005E    0x005E      /* Reserved for Apple use 0 */
#define PCTDEF_REV005F    0x005F      /* Reserved for Apple use 0 */
#define PCTDEF_FRAMEARC   0x0060      /* frameArc Rectangle (Rect), startAngle, arcAngle 12 */
#define PCTDEF_PAINTARC   0x0061      /* paintArc Rectangle (Rect), startAngle, arcAngle 12 */
#define PCTDEF_ERASEARC   0x0062      /* eraseArc Rectangle (Rect), startAngle, arcAngle 12 */
#define PCTDEF_INVERARC   0x0063      /* invertArc Rectangle (Rect), startAngle,arcAngle 12 */
#define PCTDEF_FILLARC    0x0064      /* fillArc Rectangle (Rect), startAngle, arcAngle 12 */
#define PCTDEF_REV0065    0x0065      /* Reserved for Apple use 12 bytes of data 12 */
#define PCTDEF_REV0066    0x0066      /* Reserved for Apple use 12 bytes of data 12 */
#define PCTDEF_REV0067    0x0067      /* Reserved for Apple use 12 bytes of data 12 */
#define PCTDEF_FRAMESAMEARC 0x0068      /* frameSameArc Rectangle (Rect) 4 */
#define PCTDEF_PAINTSAMEARC 0x0069      /* paintSameArc Rectangle (Rect) 4 */
#define PCTDEF_ERASESAMEARC 0x006A      /* eraseSameArc Rectangle (Rect) 4 */
#define PCTDEF_INVSAMEARC 0x006B      /* invertSameArc Rectangle (Rect) 4 */
#define PCTDEF_FILLSAMEARC  0x006C      /* fillSameArc Rectangle (Rect) 4 */
#define PCTDEF_REV006D    0x006D      /* Reserved for Apple use 4 bytes of data 4 */
#define PCTDEF_REV006E    0x006E      /* Reserved for Apple use 4 bytes of data 4 */
#define PCTDEF_REV006F    0x006F      /* Reserved for Apple use 4 bytes of data 4 */
#define PCTDEF_FRAMEPOLY  0x0070      /* framePoly Polygon (Poly) Polygon size */
#define PCTDEF_PAINTPOLY  0x0071      /* paintPoly Polygon (Poly) Polygon size */
#define PCTDEF_ERASEPOLY  0x0072      /* erasePoly Polygon (Poly) Polygon size */
#define PCTDEF_INVERTPOLY 0x0073      /* invertPoly Polygon (Poly) Polygon size */
#define PCTDEF_FILLPOLY   0x0074      /* fillPoly Polygon (Poly) Polygon size */
#define PCTDEF_REV0075    0x0075      /* Reserved for Apple use Polygon (Poly) Polygon size */
#define PCTDEF_REV0076    0x0076      /* Reserved for Apple use Polygon (Poly) Polygon size */
#define PCTDEF_REV0077    0x0077      /* Reserved for Apple use Polygon (Poly) Polygon size */
#define PCTDEF_FRAMESAMEPL  0x0078      /* frameSamePoly (Not yet implemented) 0 */
#define PCTDEF_PAINTSAMEPL  0x0079      /* paintSamePoly (Not yet implemented) 0 */
#define PCTDEF_ERASESAMEPL  0x007A      /* eraseSamePoly (Not yet implemented) 0 */
#define PCTDEF_INVERTSAMEPL 0x007B      /* invertSamePoly (Not yet implemented) 0 */
#define PCTDEF_FILLSAMEPL 0x007C      /* fillSamePoly (Not yet implemented) 0 */
#define PCTDEF_REV007D    0x007D      /* Reserved for Apple use 0 */
#define PCTDEF_REV007E    0x007E      /* Reserved for Apple use 0 */
#define PCTDEF_REV007F    0x007F      /* Reserved for Apple use 0 */
#define PCTDEF_FRAMERGN   0x0080      /* frameRgn Region (Rgn) Region size */
#define PCTDEF_PAINTRGN   0x0081      /* paintRgn Region (Rgn) Region size */
#define PCTDEF_ERASERGN   0x0082      /* eraseRgn Region (Rgn) Region size */
#define PCTDEF_INVERTRGN  0x0083      /* invertRgn Region (Rgn) Region size */
#define PCTDEF_FILLRGN    0x0084      /* fillRgn Region (Rgn) Region size */
#define PCTDEF_REV0085    0x0085      /* Reserved for Apple use Region (Rgn) Region size */
#define PCTDEF_REV0086    0x0086      /* Reserved for Apple use Region (Rgn) Region size */
#define PCTDEF_REV0087    0x0087      /* Reserved for Apple use Region (Rgn) Region size */
#define PCTDEF_FRAMESAMERGN 0x0088      /* frameSameRgn (Not yet implemented) 0 */
#define PCTDEF_PAINTSAMERGN 0x0089      /* paintSameRgn (Not yet implemented) 0 */
#define PCTDEF_ERASESAMERGN 0x008A      /* eraseSameRgn (Not yet implemented) 0 */
#define PCTDEF_INVSAMERGN 0x008B      /* invertSameRgn (Not yet implemented) 0 */
#define PCTDEF_FILLSAMERGN  0x008C      /* fillSameRgn (Not yet implemented) 0 */
#define PCTDEF_REV008D    0x008D      /* Reserved for Apple use 0 */
#define PCTDEF_REV008E    0x008E      /* Reserved for Apple use 0 */
#define PCTDEF_REV008F    0x008F      /* Reserved for Apple use 0 */
#define PCTDEF_BITSRECT   0x0090      /* BitsRect CopyBits with clipped rectangle Variable��; see Listing A-2 on page A-17 */
#define PCTDEF_BITSRGN    0x0091      /* BitsRgn CopyBits with clipped region Variable��; see Listing A-3 on page A-18 */
#define PCTDEF_REV0092    0x0092      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_REV0093    0x0093      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_REV0094    0x0094      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_REV0095    0x0095      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_REV0096    0x0096      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_REV0097    0x0097      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_PACKBITSRECT 0x0098      /* PackBitsRect Packed CopyBits with clipped rectangle Variable? see Listing A-2 on page A-17 */
#define PCTDEF_PACKBITSRGN  0x0099      /* PackBitsRgn Packed CopyBits with clipped rectangle Variable? see Listing A-3 on page A-18 */
#define PCTDEF_DIRBITSRT  0x009A      /* DirectBitsRect PixMap, srcRect, dstRect, mode (Integer), PixData Variable */
#define PCTDEF_DIRBITSRGN 0x009B      /* DirectBitsRgn PixMap, srcRect, dstRect, mode (Integer), maskRgn, PixData Variable */
#define PCTDEF_REV009C    0x009C      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_REV009D    0x009D      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_REV009E    0x009E      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_REV009F    0x009F      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_SHORTCOMMENT 0x00A0      /* ShortComment Kind (Integer) 2 */
#define PCTDEF_LONGCOMMENT  0x00A1      /* LongComment Kind (Integer), size (Integer), data 4 + data */
#define PCTDEF_REV00A2    0x00A2      /* Reserved for Apple use Data length (Integer), data 2 + data length */
#define PCTDEF_REV00A3    0x00A3
#define PCTDEF_REV00A4    0x00A4
#define PCTDEF_REV00A5    0x00A5
#define PCTDEF_REV00A6    0x00A6
#define PCTDEF_REV00A7    0x00A7
#define PCTDEF_REV00A8    0x00A8
#define PCTDEF_REV00A9    0x00A9
#define PCTDEF_REV00AA    0x00AA
#define PCTDEF_REV00AB    0x00AB
#define PCTDEF_REV00AC    0x00AC
#define PCTDEF_REV00AD    0x00AD
#define PCTDEF_REV00AE    0x00AE
#define PCTDEF_REV00AF    0x00AF
#define PCTDEF_REV00B0    0x00B0      /* Reserved for Apple use 0 */
/*
. . . .
. . . . ===== Reserved for Apple use 0
. . . .
*/
#define PCTDEF_REV00CF    0x00CF
#define PCTDEF_REV00D0    0x00D0      /* Reserved for Apple use Data length (Long), data 4 + data length */
/*
. . . .
. . . . ===== Reserved for Apple use Data length (Long), data 4 + data length
. . . .
*/
#define PCTDEF_REV00FE    0x00FE
#define PCTDEF_OPENDPIC   0x00FF      /* OpEndPic End of picture 2 */
#define PCTDEF_REV0100    0x0100      /* Reserved for Apple use 2 bytes of data 2 */
/*
. . . .
. . . . ===== Reserved for Apple use 2 bytes of data 2
. . . .
*/
#define PCTDEF_REV01FF    0x01FF      /* Reserved for Apple use 2 bytes of data 2 */
#define PCTDEF_REV0200    0x0200      /* Reserved for Apple use 4 bytes of data 4 */
#define PCTDEF_VERSION    0x02FF      /* Version Version number of picture 2 */
/*
. . . .
. . . . ===== 0xnnXX  COUNT = nn*2
. . . .
*/
#define PCTDEF_REV0BFF    0x0BFF      /* Reserved for Apple use 22 bytes of data 22 */
#define PCTDEF_HEADEROP   0x0C00      /* HeaderOp For extended version 2: version (Integer), reserved (Integer), hRes, vRes (Fixed), srcRect, reserved (Long); for version 2: opcode 24 */
#define PCTDEF_REV0C01    0x0C01      /* Reserved for Apple use 24 bytes of data 24 */
/*
. . . .
. . . . Reserved for Apple use 24 bytes of data 24, 0xnnXX  COUNT = nn*2
. . . .
*/
#define PCTDEF_REV7F00    0x7F00      /* Reserved for Apple use 254 bytes of data 254 */
/*
. . . .
. . . . Reserved for Apple use 254 bytes of data 254
. . . .
*/
#define PCTDEF_REV7FFF    0x7FFF      /* Reserved for Apple use 254 bytes of data 254 */
#define PCTDEF_REV8000    0x8000      /* Reserved for Apple use 0 */
/*
. . . .
. . . . Reserved for Apple use 0
. . . .
*/
#define PCTDEF_REV80FF    0x80FF      /* Reserved for Apple use 0 */
#define PCTDEF_REV8100    0x8100      /* Reserved for Apple use Data length (Long), data 4 + data length */
/*
. . . .
. . . . Reserved for Apple use Data length (Long), data 4 + data length
. . . .
*/
#define PCTDEF_COMPQT   0x8200      /* CompressedQuickTime Data length (Long), data (private to QuickTime) 4 + data length */
#define PCTDEF_UNCOMPQT   0x8201      /* UncompressedQuickTime Data length (Long), data (private to QuickTime) 4 + data length */
#define PCTDEF_REVFFFF    0xFFFF      /* Reserved for Apple use Data length (Long), data 4 + data length */



/* PICT ͷ */
typedef struct {
  short size;
  short top, left, bottom, right;
} PCT_MAGIC, *LPPCT_MAGIC;


/* PICT �汾2 ͼ��ͷ�ṹ */
typedef struct {
  long  ver;            /* �汾�ţ�����-1��0xffff��*/
  short top, rev0, left, rev1, bottom, rev2, right, rev3;
  short rev4, rev5;
} PCT_HEADER, *LPPCT_HEADER;


/* PICT ��չ�汾2 ͼ��ͷ�ṹ */
typedef struct {
  short ver;            /* �汾�ţ�����-2��0xfffe��*/
  short rev0;
  short hdpi;           /* ��ѷֱ������ݣ���ˮƽ����ֱ�� */
  short rev1;
  short vdpi;
  short rev2;
  short top, left, bottom, right; /* ��ѡ��Դ�������� */
  short rev3, rev4;
} PCT_EX_HEADER, *LPPCT_EX_HEADER;


/* PICT ������ͼԪ�ؽṹ���� */
typedef struct {
  short y, x;       /* ���������� */
} PCT_POINT, *LPPCT_POINT;


typedef struct {
  short top, left, bottom, right;
} PCT_RECT, *LPPCT_RECT;


typedef struct {
  short   size;     /* �������ݳߴ磨�ֽڵ�λ��*/
  PCT_RECT  enrect;     /* ����߽� */
} PCT_REGION, *LPPCT_REGION;


typedef struct {
  long    pos;      /* λ���ݵ�ַ */
  short   width;      /* �п�ȣ��ֽڵ�λ�������ص�λ�� */
  PCT_RECT  bound;      /* ͼ��߽� */
} PCT_BITMAP, *LPPCT_BITMAP;


typedef struct {
  short   pmVersion;    /* PixMap �汾�� */
  short   packType;   /* ѹ������ */
  long    packSize;   /* ѹ����ĳߴ磨���ֵû�б�ʹ�ã�����Ϊ0�� */
  long    hRes;     /* ˮƽ�ֱ��� */
  long    vRes;     /* ��ֱ�ֱ��� */
  short   pixelType;    /* ���ظ�ʽ��0������ͼ��16��RGBͼ�� */
  short   pixelSize;    /* ÿ���ص�����λ�� */
  short   cmpCount;   /* ÿ���ص��߼���� */
  short   cmpSize;    /* ÿ��ֵ��߼�λ�� */
  long    planeBytes;   /* ��һ������ƫ�� */
  long    pmTable;    /* ��ɫ��ṹ�ľ�� */
  long    pmReserved;   /* ����������Ϊ0 */
} PCT_PIXMAP, *LPPCT_PIXMAP;


typedef struct {
  ushort red;
  ushort green;
  ushort blue;
} PCT_RGBCOLOR, *LPPCT_RGBCOLOR;

typedef struct {
  short     value;      /* ����ֵ */
  PCT_RGBCOLOR  rgb;      /* ������� */
} PCT_COLORSPEC, *LPPCT_COLORSPEC;


typedef struct {
  long      ctSeed;     /* ���Ψһ��ʶ */
  short     ctFlags;    /* ��λ��0 = PixMap; 1 = device */
  short     ctSize;     /* ��һ��������ĸ��� */
  PCT_COLORSPEC ctTable[1];   /* ��ɫ�ռ��� */
} PCT_COLORTABLE, *LPPCT_COLORTABLE;


typedef struct {
  ushort  red;
  ushort  green;
  ushort  blue;
  long      matchData;
} PCT_MATCHRECT, *LPPCT_MATCHRECT;


typedef struct {
  short     patType;
  long      patMap;
  long      patData;
  long      patXData;
  short     patXValid;
  long      patXMap;
  long      pat1Data;
} PCT_PIXPAT, *LPPCT_PIXPAT;


typedef struct {
  short     polySize;   /* �ֽ��� */
  PCT_RECT    polyBBox;   /* �߽����� */
  PCT_POINT   polyPoints[1];  /* �������꼯 */
} PCT_POLYGON, *LPPCT_POLYGON;


typedef struct {
  uchar pat[8];
} PCT_PATTERN, *LPPCT_PATTERN;


typedef struct {
  PCT_POINT   pnLoc;      /* λ�� */
  PCT_POINT   pnSize;     /* �ʳߴ� */
  short     pnMode;     /* �ʵ�����ģʽ */
  PCT_PATTERN   pnPat;      /* ������ */
} PCT_PENSTATE, *LPPCT_PENSTATE;


typedef struct {
  PCT_PIXMAP    pixmap;
  PCT_RECT    src_rect;   /* Դ�������� */
  PCT_RECT    dec_rect;   /* Ŀ��������� */
  short     mode;     /* ת��ģʽ */
} PCT_DIRECT_BITS_RECT, *LPPCT_DIRECT_BITS_RECT;


typedef struct {
  PCT_PIXMAP    pixmap;
  PCT_RECT    src_rect;   /* Դ�������� */
  PCT_RECT    dec_rect;   /* Ŀ��������� */
  short     mode;     /* ת��ģʽ */
  PCT_REGION    mask_rgn;   /* �������� */
} PCT_DIRECT_BITS_RGB, *LPPCT_DIRECT_BITS_RGN;

#define MAX_PALETTE_COUNT 256

/* �ڲ����ֺ��� */
static int _skip_ign_data(ISFILE* pfile, short opcode);
static int _read_dword(ISFILE* pfile, long* ps);
static int _read_word(ISFILE* pfile, short* ps);
static int _read_byte(ISFILE* pfile, char* ps);
static int _skip_and_skip(ISFILE* pfile, int n);
static int _read_pictrect(ISFILE* pfile, LPPCT_RECT prc);
static int _decomp_rle8(uchar*, int, uchar*, int);
static int _find_index(LPPCT_COLORSPEC pcol, int len, int index);
static enum EXERESULT _read_header(ISFILE* pfile, int* pversion);
static int _expand_buffer(uchar* psrc, int src_len, uchar* pdec, const int bits_per_pixel);

/* ��ȡͼ����Ϣ */
EXERESULT pct_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  PCT_PIXMAP pixmap;
  PCT_RECT frame;
  short code, stmp, bytes_per_line, bits_per_pixel;
  int version, mark = 0;
  unsigned int i, j;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ������ݰ���������ͼ��λ���ݣ������ٸı���е�ͼ����Ϣ */

    do {
      /* ����ײ�������Ϣ�����Ա�������õ���Ϣ:��*/
      if ((b_status = _read_header(pfile, &version)) != ER_SUCCESS) {
        break;
      }
      memset(&pixmap, 0, sizeof(pixmap));
      while (mark == 0) {
        // ��ȡ������
        if (version == 1 || isio_tell(pfile) % 2 != 0) {
          stmp = 0;
          if (_read_byte(pfile, (char*)&stmp)) {
            b_status = ER_FILERWERR;
            break;
          }
        }
        else {
          if (_read_word(pfile, &stmp)) {
            b_status = ER_FILERWERR;
            break;
          }
        }
        if (stmp == 0xffff || stmp == 0xff) { // ������
          break;
        }
        switch ((int)(unsigned int)(ushort)stmp) {
        case PCTDEF_BITSRECT:
        case PCTDEF_BITSRGN:
        case PCTDEF_PACKBITSRECT:
        case PCTDEF_PACKBITSRGN:
        case PCTDEF_DIRBITSRT:
        case PCTDEF_DIRBITSRGN:
          code = stmp;
          if ((code != PCTDEF_DIRBITSRT) && (code != PCTDEF_DIRBITSRGN)) {
            if (_read_word(pfile, &stmp)) {
              b_status = ER_FILERWERR;
              break;
            }
            bytes_per_line = (ushort)stmp;
          }
          else {
            if (_skip_and_skip(pfile, 6) != 0) {
              b_status = ER_FILERWERR;
              break;
            }
          }
          /* �߽�������� */
          if (_read_pictrect(pfile, &frame)) {
            b_status = ER_FILERWERR;
            break;
          }
          /* ��PICTͼ��ṹ */
          if (code == PCTDEF_DIRBITSRT || code == PCTDEF_DIRBITSRGN ||
              (bytes_per_line & 0x8000)) {
            if (isio_read((void*)&pixmap, sizeof(PCT_PIXMAP), 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
            }
            pixmap.pmVersion = EXCHANGE_WORD((pixmap.pmVersion)); /* PixMap �汾�� */
            pixmap.packType = EXCHANGE_WORD((pixmap.packType)); /* ѹ������ */
            pixmap.pixelType = EXCHANGE_WORD((pixmap.pixelType)); /* ���ظ�ʽ */
            pixmap.pixelSize = EXCHANGE_WORD((pixmap.pixelSize)); /* ÿ���ص�����λ�� */
            pixmap.cmpCount = EXCHANGE_WORD((pixmap.cmpCount)); /* ÿ���ص��߼���� */
            pixmap.cmpSize = EXCHANGE_WORD((pixmap.cmpSize)); /* ÿ��ֵ��߼�λ�� */
            pixmap.packSize = EXCHANGE_DWORD((pixmap.packSize)); /* ѹ����ĳߴ� */
            pixmap.hRes = EXCHANGE_DWORD((pixmap.hRes)); /* ˮƽ�ֱ��� */
            pixmap.vRes = EXCHANGE_DWORD((pixmap.vRes)); /* ��ֱ�ֱ��� */
            pixmap.planeBytes = EXCHANGE_DWORD((pixmap.planeBytes)); /* ��һ������ƫ�� */
            pixmap.pmTable = EXCHANGE_DWORD((pixmap.pmTable)); /* ��ɫ��ṹ�ľ�� */
            pixmap.pmReserved = EXCHANGE_DWORD((pixmap.pmReserved)); /* ����������Ϊ0 */
          }
          /* ��ʼ����ɫ�� */
          if ((code != PCTDEF_DIRBITSRT) && (code != PCTDEF_DIRBITSRGN)) {
            short flags;
            wRGBQUAD* pRgb;
            PCT_COLORSPEC pctcolor;
            pRgb = (wRGBQUAD*)pinfo_str->palette;
            if (bytes_per_line & 0x8000) {
              /* ����־ */
              if (_skip_and_skip(pfile, 4) != 0 || _read_word(pfile, &flags)) {
                b_status = ER_FILERWERR;
                break;
              }
              /* ����ɫ���� */
              if (_read_word(pfile, &stmp)) {
                b_status = ER_FILERWERR;
                break;
              }
              pinfo_str->pal_count = stmp + 1;
              if (pinfo_str->pal_count > 256) { /* ��������ɫ���� */
                b_status = ER_BADIMAGE;
                break;
              }
              for (i = 0; i < pinfo_str->pal_count; ++i) {
                if (isio_read(&pctcolor, sizeof(PCT_COLORSPEC), 1, pfile) == 0) {
                  b_status = ER_FILERWERR;
                  break;
                }
                j = EXCHANGE_WORD(pctcolor.value) % pinfo_str->pal_count;
                if (flags & 0x8000) {
                  j = i;
                }
                pRgb[j].rgbRed = EXCHANGE_WORD(pctcolor.rgb.red) >> 8;
                pRgb[j].rgbGreen = EXCHANGE_WORD(pctcolor.rgb.green) >> 8;
                pRgb[j].rgbBlue = EXCHANGE_WORD(pctcolor.rgb.blue) >> 8;
              }
            }
            else { /* 2λ��ɫ�� */
              pinfo_str->pal_count = 2;
              pinfo_str->palette[0] = 0xffffffff;
              pinfo_str->palette[1] = 0x0;
            }
          }
          mark = 1;
          break;
        default:
          /* ʸ����ͼ�����뽫������ */
          if (_skip_ign_data(pfile, stmp)) {
            b_status = ER_FILERWERR;
            break;
          }
          break;
        }
      }
      /* PCT��û���ҵ��ɶ���λͼ */
      if (mark == 0) {
        b_status = ER_BADIMAGE;
        break;
      }
      if ((code != PCTDEF_DIRBITSRT) && (code != PCTDEF_DIRBITSRGN) && (bytes_per_line & 0x8000) == 0) {
        bits_per_pixel = 1;
      }
      else {
        bits_per_pixel = pixmap.pixelSize;
      }
      if (bits_per_pixel <= 8) {
        bytes_per_line &= 0x7FFF;
      }
      if (bits_per_pixel == 32) {
        if (pixmap.cmpCount != 4) {
          bits_per_pixel = 24;
        }
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
      pinfo_str->imgformat = IMF_PCT; /* ͼ���ļ���ʽ����׺���� */
      pinfo_str->compression = ICS_RLE8;
      pinfo_str->width = (ulong)(frame.right - frame.left);
      pinfo_str->height = (ulong)(frame.bottom - frame.top);
      pinfo_str->order = 0;
      pinfo_str->bitcount = bits_per_pixel;
      switch (pinfo_str->bitcount) {
      case 16:
        pinfo_str->r_mask = 0x7c00;
        pinfo_str->g_mask = 0x3e0;
        pinfo_str->b_mask = 0x1f;
        pinfo_str->a_mask = 0x0;
        break;
      case 24:
      case 32:
        pinfo_str->b_mask = 0xff;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->a_mask = 0x0;
        break;
      default:
        pinfo_str->b_mask = 0x0;
        pinfo_str->g_mask = 0x0;
        pinfo_str->r_mask = 0x0;
        pinfo_str->a_mask = 0x0;
        break;
      }
      /* �趨���ݰ�״̬ */
      pinfo_str->data_state = 1;
    }
    while (0);
  return b_status;
}
/* ��ȡͼ��λ���� */
EXERESULT pct_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  PCT_PIXMAP pixmap;
  PCT_RECT frame, srrect, dcrect;
  short stmp, code;
  uchar* pbuf = 0, *pdec = 0;
  int mark = 0, linesize;
  unsigned int i, j;
  int version, bits_per_pixel;
  int bytes_per_line = 0;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* ���ݰ��в��ܴ���ͼ��λ���� */

    do {
      /* ����ײ�������Ϣ�����Ա�������õ���Ϣ:��*/
      if ((b_status = _read_header(pfile, &version)) != ER_SUCCESS) {
        break;
      }
      //////////////////////////////////////////////////////////////////////////
      memset(&pixmap, 0, sizeof(pixmap));
      while (mark == 0) {
        // ��ȡ������
        if (version == 1 || isio_tell(pfile) % 2 != 0) {
          stmp = 0;
          if (_read_byte(pfile, (char*)&stmp)) {
            b_status = ER_FILERWERR;
            break;
          }
        }
        else {
          if (_read_word(pfile, &stmp)) {
            b_status = ER_FILERWERR;
            break;
          }
        }
        if (stmp == 0xffff || stmp == 0xff) { // ������
          break;
        }
        switch ((int)(unsigned int)(ushort)stmp) {
        case PCTDEF_BITSRECT:
        case PCTDEF_BITSRGN:
        case PCTDEF_PACKBITSRECT:
        case PCTDEF_PACKBITSRGN:
        case PCTDEF_DIRBITSRT:
        case PCTDEF_DIRBITSRGN:
          code = stmp;
          if ((code != PCTDEF_DIRBITSRT) && (code != PCTDEF_DIRBITSRGN)) {
            if (_read_word(pfile, &stmp)) {
              b_status = ER_FILERWERR;
              break;
            }
            bytes_per_line = (ushort)stmp;
          }
          else {
            if (_skip_and_skip(pfile, 6) != 0) {
              b_status = ER_FILERWERR;
              break;
            }
          }
          /* �߽�������� */
          if (_read_pictrect(pfile, &frame)) {
            b_status = ER_FILERWERR;
            break;
          }
          /* ��PICTͼ��ṹ */
          if (code == PCTDEF_DIRBITSRT || code == PCTDEF_DIRBITSRGN ||
              (bytes_per_line & 0x8000)) {
            if (isio_read((void*)&pixmap, sizeof(PCT_PIXMAP), 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
            }
            pixmap.pmVersion = EXCHANGE_WORD((pixmap.pmVersion)); /* PixMap �汾�� */
            pixmap.packType = EXCHANGE_WORD((pixmap.packType)); /* ѹ������ */
            pixmap.pixelType = EXCHANGE_WORD((pixmap.pixelType)); /* ���ظ�ʽ */
            pixmap.pixelSize = EXCHANGE_WORD((pixmap.pixelSize)); /* ÿ���ص�����λ�� */
            pixmap.cmpCount = EXCHANGE_WORD((pixmap.cmpCount)); /* ÿ���ص��߼���� */
            pixmap.cmpSize = EXCHANGE_WORD((pixmap.cmpSize)); /* ÿ��ֵ��߼�λ�� */
            pixmap.packSize = EXCHANGE_DWORD((pixmap.packSize)); /* ѹ����ĳߴ� */
            pixmap.hRes = EXCHANGE_DWORD((pixmap.hRes)); /* ˮƽ�ֱ��� */
            pixmap.vRes = EXCHANGE_DWORD((pixmap.vRes)); /* ��ֱ�ֱ��� */
            pixmap.planeBytes = EXCHANGE_DWORD((pixmap.planeBytes)); /* ��һ������ƫ�� */
            pixmap.pmTable = EXCHANGE_DWORD((pixmap.pmTable)); /* ��ɫ��ṹ�ľ�� */
            pixmap.pmReserved = EXCHANGE_DWORD((pixmap.pmReserved)); /* ����������Ϊ0 */
          }
          else {
            /* Ϊ��������Ĵ���������Ҳ��дһЩpixmap��ֵ */
            pixmap.packType = 0;
            pixmap.packSize = 0;
            pixmap.pixelType = 0;
            pixmap.pixelSize = 1;
            pixmap.cmpCount = 1;
            pixmap.cmpSize = 1;
          }
          /* ��ʼ����ɫ�� */
          if ((code != PCTDEF_DIRBITSRT) && (code != PCTDEF_DIRBITSRGN)) {
            short flags;
            wRGBQUAD* pRgb;
            PCT_COLORSPEC pctcolor;
            pRgb = (wRGBQUAD*)pinfo_str->palette;
            if (bytes_per_line & 0x8000) {
              assert(pixmap.packType == 0);
              /* ����־ */
              if (_skip_and_skip(pfile, 4) != 0 || _read_word(pfile, &flags)) {
                b_status = ER_FILERWERR;
                break;
              }
              /* ����ɫ���� */
              if (_read_word(pfile, &stmp)) {
                b_status = ER_FILERWERR;
                break;
              }
              pinfo_str->pal_count = stmp + 1;
              if (pinfo_str->pal_count > 256) { /* ��������ɫ���� */
                b_status = ER_BADIMAGE;
                break;
              }
              for (i = 0; i < pinfo_str->pal_count; ++i) {
                if (isio_read(&pctcolor, sizeof(PCT_COLORSPEC), 1, pfile) == 0) {
                  b_status = ER_FILERWERR;
                  break;
                }
                j = EXCHANGE_WORD(pctcolor.value) % pinfo_str->pal_count;
                if (flags & 0x8000) {
                  j = i;
                }
                pRgb[j].rgbRed = EXCHANGE_WORD(pctcolor.rgb.red) >> 8;
                pRgb[j].rgbGreen = EXCHANGE_WORD(pctcolor.rgb.green) >> 8;
                pRgb[j].rgbBlue = EXCHANGE_WORD(pctcolor.rgb.blue) >> 8;
              }
            }
            else { /* 2λ��ɫ�� */
              pinfo_str->pal_count = 2;
              pinfo_str->palette[0] = 0xffffffff;
              pinfo_str->palette[1] = 0x0;
            }
          }
          if (_read_pictrect(pfile, &srrect)) {
            b_status = ER_FILERWERR;
            break;
          }
          if (_read_pictrect(pfile, &dcrect)) {
            b_status = ER_FILERWERR;
            break;
          }
          /* ����ģʽ�� */
          if (_skip_and_skip(pfile, 2) != 0) {
            b_status = ER_FILERWERR;
            break;
          }
          /* ������������ */
          if ((code == PCTDEF_BITSRGN) || (code == PCTDEF_PACKBITSRGN) || (code == PCTDEF_DIRBITSRGN)) {
            if (_read_word(pfile, &stmp)
                || _skip_and_skip(pfile, stmp - 2) != 0) {
              b_status = ER_FILERWERR;
              break;
            }
          }
          mark = 1;
          break;
        default:
          /* ʸ����ͼ�����뽫������ */
          if (_skip_ign_data(pfile, stmp)) {
            b_status = ER_FILERWERR;
            break;
          }
          break;
        }
      }
      /* PCT��û��λͼ */
      if (mark == 0) {
        b_status = ER_BADIMAGE;
        break;
      }
      //////////////////////////////////////////////////////////////////////////
      if ((code != PCTDEF_DIRBITSRT) && (code != PCTDEF_DIRBITSRGN) && (bytes_per_line & 0x8000) == 0) {
        bits_per_pixel = 1;
      }
      else {
        bits_per_pixel = pixmap.pixelSize;
      }
      if (bits_per_pixel <= 8) {
        bytes_per_line &= 0x7FFF;
      }
      if (bytes_per_line == 0) {
        bytes_per_line = (frame.right - frame.left);
      }
      /* �����32λͼ����ֻ��3��ͨ������ô��24λ���� */
      if (bits_per_pixel == 32 && pixmap.cmpCount != 4) {
        bits_per_pixel = 24;
      }
      /* ����һ���Ƿ�������Ը */
#ifdef _DEBUG
      switch (bits_per_pixel) {
      case 1:
        assert(pixmap.pixelType == 0);
        assert(pixmap.pixelSize == 1);
        assert(pixmap.cmpCount == 1);
        assert(pixmap.cmpSize == 1);
        break;
      case 2:
        assert(pixmap.pixelType == 0);
        assert(pixmap.pixelSize == 2);
        assert(pixmap.cmpCount == 1);
        assert(pixmap.cmpSize == 2);
        break;
      case 4:
        assert(pixmap.pixelType == 0);
        assert(pixmap.pixelSize == 4);
        assert(pixmap.cmpCount == 1);
        assert(pixmap.cmpSize == 4);
        break;
      case 8:
        assert(pixmap.pixelType == 0);
        assert(pixmap.pixelSize == 8);
        assert(pixmap.cmpCount == 1);
        assert(pixmap.cmpSize == 8);
        break;
      case 16:
        assert(pixmap.packType == 3);
        assert(pixmap.pixelType == 16);
        assert(pixmap.pixelSize == 16);
        assert(pixmap.cmpCount == 3);
        assert(pixmap.cmpSize == 5);
        break;
      case 24:
        assert(pixmap.packType == 4);
        assert(pixmap.pixelType == 16);
        assert(pixmap.pixelSize == 32);
        assert(pixmap.cmpCount == 3);
        assert(pixmap.cmpSize == 8);
        break;
      case 32:
        assert(pixmap.packType == 4);
        assert(pixmap.pixelType == 16);
        assert(pixmap.pixelSize == 32);
        assert(pixmap.cmpCount == 4);
        assert(pixmap.cmpSize == 8);
        break;
      default:
        assert(0);
        break;
      }
#endif
      //////////////////////////////////////////////////////////////////////////
      /* �����ͼ��δ���ù���ȡ��Ϣ����������дͼ����Ϣ */
      if (pinfo_str->data_state == 0) {
        pinfo_str->imgtype = IMT_RESSTATIC; /* ͼ���ļ����� */
        pinfo_str->imgformat = IMF_PCT; /* ͼ���ļ�?�ʽ����׺���?*/
        pinfo_str->compression = ICS_RLE8;
        pinfo_str->width = (ulong)(frame.right - frame.left);
        pinfo_str->height = (ulong)(frame.bottom - frame.top);
        pinfo_str->order = 0;
        pinfo_str->bitcount = bits_per_pixel;
        switch (pinfo_str->bitcount) {
        case 16:
          pinfo_str->r_mask = 0x7c00;
          pinfo_str->g_mask = 0x3e0;
          pinfo_str->b_mask = 0x1f;
          pinfo_str->a_mask = 0x0;
          break;
        case 24:
        case 32:
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0x0;
          break;
        default:
          pinfo_str->b_mask = 0x0;
          pinfo_str->g_mask = 0x0;
          pinfo_str->r_mask = 0x0;
          pinfo_str->a_mask = 0x0;
          break;
        }
        /* �趨���ݰ�״̬ */
        pinfo_str->data_state = 1;
      }
      /* ȡ��ɨ���гߴ� */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      /* ����ͼ����� */
      pinfo_str->imgnumbers = 1;
      pinfo_str->psubimg = (SUBIMGBLOCK*)0;
      assert(pinfo_str->p_bit_data == (uchar*)0);
      /* ����Ŀ��ͼ���ڴ�飨+4 �� β������4�ֽڻ������� */
      pinfo_str->p_bit_data = (uchar*)isirw_malloc(linesize * pinfo_str->height + 4);
      if (!pinfo_str->p_bit_data) {
        b_status = ER_MEMORYERR;
        break;
      }
      assert(pinfo_str->pp_line_addr == NULL);
      /* �������׵�ַ���� */
      pinfo_str->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * pinfo_str->height);
      if (!pinfo_str->pp_line_addr) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* ��д���׵�ַ���� */
      for (i = 0; i < pinfo_str->height; i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
      }
      /* �����ѹ������ */
      if ((pbuf = (uchar*)isirw_malloc(linesize * 2 + 4)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      if ((pdec = (uchar*)isirw_malloc(linesize + 4)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      i = (unsigned int)isio_tell(pfile);
      /* ���ж����������� */
      for (i = 0; i < pinfo_str->height; i++) {
        uchar* q = pdec;
        /* Pixels are already uncompressed. */
        if (bytes_per_line < 8) {
          if (isio_read((void*)q, bytes_per_line, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
        else { /* Uncompress RLE pixels into uncompressed pixel buffer. */
          /* �����г��� */
          ushort uscount = 0;
          if ((bytes_per_line > 250) || (bits_per_pixel > 8)) {
            _read_word(pfile, (short*)&uscount);
          }
          else {
            _read_byte(pfile, (char*)&uscount);
          }
          if (isio_read((void*)pbuf, uscount, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
          for (j = 0; j < uscount;) {
            int slen;
            if ((pbuf[j] & 0x80) == 0) {
              slen = ((pbuf[j++] & 0xff) + 1) * 1;
              while (slen-- > 0) {
                *q++ = pbuf[j++];
                if (bits_per_pixel == 16) { /* һ���������ֽ�RLE16 */
                  *q++ = pbuf[j++];
                }
              }
            }
            else {
              slen = ((pbuf[j++] ^ 0xff) & 0xff) + 2;
              while (slen-- > 0) {
                *q++ = pbuf[j];
                if (bits_per_pixel == 16) { /* һ���������ֽ�RLE16 */
                  *q++ = pbuf[j + 1];
                }
              }
              if (bits_per_pixel == 16) {
                j++;
              }
              j++;
            }
          }
          assert(q - pdec <= linesize);
          /* �ϳ����ص����ݰ� */
          {
            uchar* pr, *pg, *pb;
            uchar* p = (uchar*)pinfo_str->pp_line_addr[i];
            switch (pinfo_str->bitcount) {
            case 1:
            case 4:
            case 8:
              memcpy(p, pdec, linesize);
              break;
            case 2: /* ����˳�� */
              for (j = 0; j < (unsigned int)linesize; ++j) {
                uchar c = pdec[j];
                CVT_BITS2(c);
                *p++ = c;
              }
              break;
            case 16:
              for (j = 0; j < pinfo_str->width; ++j) {
                *((short*)p) = EXCHANGE_WORD(((short*)pdec)[j]);
                p += sizeof(short);
              }
              break;
              /* 24/32 λͼ������aͨ�� */
            case 24:
              pb = pdec;
              pg = pb + pinfo_str->width;
              pr = pg + pinfo_str->width;
              for (j = 0; j < pinfo_str->width; ++j) {
                *p++ = *pr++;
                *p++ = *pg++;
                *p++ = *pb++;
              }
              break;
            case 32:
              pb = pdec + pinfo_str->width;
              pg = pb + pinfo_str->width;
              pr = pg + pinfo_str->width;
              for (j = 0; j < pinfo_str->width; ++j) {
                *p++ = *pr++;
                *p++ = *pg++;
                *p++ = *pb++;
                *p++ = 0;
              }
              break;
            default:
              assert(0);
              break;
            }
          }
        }
      }
      pinfo_str->data_state = 2;
    }
    while (0);
    
      if (pdec) {
        isirw_free(pdec);
      }
      if (pbuf) {
        isirw_free(pbuf);
      }
      if (b_status != ER_SUCCESS) {
        if (pinfo_str->p_bit_data) {
          isirw_free(pinfo_str->p_bit_data);
          pinfo_str->p_bit_data = (uchar*)0;
        }
        if (pinfo_str->pp_line_addr) {
          isirw_free(pinfo_str->pp_line_addr);
          pinfo_str->pp_line_addr = (uchar**)0;
        }
        if (pinfo_str->data_state == 2) {
          pinfo_str->data_state = 1; /* �Զ����� */
        }
        
      }
      else {
        if (b_status != ER_SUCCESS) {
          if (pinfo_str->p_bit_data) {
            isirw_free(pinfo_str->p_bit_data);
            pinfo_str->p_bit_data = (uchar*)0;
          }
          if (pinfo_str->pp_line_addr) {
            isirw_free(pinfo_str->pp_line_addr);
            pinfo_str->pp_line_addr = (uchar**)0;
          }
          if (pinfo_str->data_state == 2) {
            pinfo_str->data_state = 1; /* �Զ����� */
          }
          
        }
      }
  return b_status;
}
/* ����ͼ�� */
EXERESULT pct_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  // PICT��ʽ����ݲ�ʵ�ֱ��湦�ܣ�PS���Խ�ͼ�󱣴�ΪPCT��ʽ��
  return ER_NOTSUPPORT;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* �ڲ��������� */

const int codesize[] = {
  /* 0x00 NOP */ 0, /* nop */
  /* 0x01 Clip */ 0, /* clip */
  /* 0x02 BkPat */ 8, /* background pattern */
  /* 0x03 TxFont */ 2, /* text font (word) */
  /* 0x04 TxFace */ 1, /* text face (byte) */
  /* 0x05 TxMode */ 2, /* text mode (word) */
  /* 0x06 SpExtra */ 4, /* space extra (fixed point) */
  /* 0x07 PnSize */ 4, /* pen size (point) */
  /* 0x08 PnMode */ 2, /* pen mode (word) */
  /* 0x09 PnPat */ 8, /* pen pattern */
  /* 0x0a FillPat */ 8, /* fill pattern */
  /* 0x0b OvSize */ 4, /* oval size (point) */
  /* 0x0c Origin */ 4, /* dh, dv (word) */
  /* 0x0d TxSize */ 2, /* text size (word) */
  /* 0x0e FgColor */ 4, /* foreground color (longword) */
  /* 0x0f BkColor */ 4, /* background color (longword) */
  /* 0x10 TxRatio */ 8, /* numerator (point), denominator (point) */
  /* 0x11 Version */ 1, /* version (byte) */
  /* 0x12 BkPixPat */ 0, /* color background pattern */
  /* 0x13 PnPixPat */ 0, /* color pen pattern */
  /* 0x14 FillPixPat */ 0, /* color fill pattern */
  /* 0x15 PnLocHFrac */ 2, /* fractional pen position */
  /* 0x16 ChExtra */ 2, /* extra for each character */
  /* 0x17 reserved */ 0, /* reserved for Apple use */
  /* 0x18 reserved */ 0, /* reserved for Apple use */
  /* 0x19 reserved */ 0, /* reserved for Apple use */
  /* 0x1a RGBFgCol */ 6, /* RGB foreColor */
  /* 0x1b RGBBkCol */ 6, /* RGB backColor */
  /* 0x1c HiliteMode */ 0, /* hilite mode flag */
  /* 0x1d HiliteColor */ 6, /* RGB hilite color */
  /* 0x1e DefHilite */ 0, /* Use default hilite color */
  /* 0x1f OpColor */ 6, /* RGB OpColor for arithmetic modes */
  /* 0x20 Line */ 8, /* pnLoc (point), newPt (point) */
  /* 0x21 LineFrom */ 4, /* newPt (point) */
  /* 0x22 ShortLine */ 6, /* pnLoc (point, dh, dv (-128 .. 127)) */
  /* 0x23 ShortLineFrom */ 2, /* dh, dv (-128 .. 127) */
  /* 0x24 reserved */ -1, /* reserved for Apple use */
  /* 0x25 reserved */ -1, /* reserved for Apple use */
  /* 0x26 reserved */ -1, /* reserved for Apple use */
  /* 0x27 reserved */ -1, /* reserved for Apple use */
  /* 0x28 LongText */ 0, /* txLoc (point), count (0..255), text */
  /* 0x29 DHText */ 0, /* dh (0..255), count (0..255), text */
  /* 0x2a DVText */ 0, /* dv (0..255), count (0..255), text */
  /* 0x2b DHDVText */ 0, /* dh, dv (0..255), count (0..255), text */
  /* 0x2c reserved */ -1, /* reserved for Apple use */
  /* 0x2d reserved */ -1, /* reserved for Apple use */
  /* 0x2e reserved */ -1, /* reserved for Apple use */
  /* 0x2f reserved */ -1, /* reserved for Apple use */
  /* 0x30 frameRect */ 8, /* rect */
  /* 0x31 paintRect */ 8, /* rect */
  /* 0x32 eraseRect */ 8, /* rect */
  /* 0x33 invertRect */ 8, /* rect */
  /* 0x34 fillRect */ 8, /* rect */
  /* 0x35 reserved */ 8, /* reserved for Apple use */
  /* 0x36 reserved */ 8, /* reserved for Apple use */
  /* 0x37 reserved */ 8, /* reserved for Apple use */
  /* 0x38 frameSameRect */ 0, /* rect */
  /* 0x39 paintSameRect */ 0, /* rect */
  /* 0x3a eraseSameRect */ 0, /* rect */
  /* 0x3b invertSameRect */ 0, /* rect */
  /* 0x3c fillSameRect */ 0, /* rect */
  /* 0x3d reserved */ 0, /* reserved for Apple use */
  /* 0x3e reserved */ 0, /* reserved for Apple use */
  /* 0x3f reserved */ 0, /* reserved for Apple use */
  /* 0x40 frameRRect */ 8, /* rect */
  /* 0x41 paintRRect */ 8, /* rect */
  /* 0x42 eraseRRect */ 8, /* rect */
  /* 0x43 invertRRect */ 8, /* rect */
  /* 0x44 fillRRrect */ 8, /* rect */
  /* 0x45 reserved */ 8, /* reserved for Apple use */
  /* 0x46 reserved */ 8, /* reserved for Apple use */
  /* 0x47 reserved */ 8, /* reserved for Apple use */
  /* 0x48 frameSameRRect */ 0, /* rect */
  /* 0x49 paintSameRRect */ 0, /* rect */
  /* 0x4a eraseSameRRect */ 0, /* rect */
  /* 0x4b invertSameRRect */ 0, /* rect */
  /* 0x4c fillSameRRect */ 0, /* rect */
  /* 0x4d reserved */ 0, /* reserved for Apple use */
  /* 0x4e reserved */ 0, /* reserved for Apple use */
  /* 0x4f reserved */ 0, /* reserved for Apple use */
  /* 0x50 frameOval */ 8, /* rect */
  /* 0x51 paintOval */ 8, /* rect */
  /* 0x52 eraseOval */ 8, /* rect */
  /* 0x53 invertOval */ 8, /* rect */
  /* 0x54 fillOval */ 8, /* rect */
  /* 0x55 reserved */ 8, /* reserved for Apple use */
  /* 0x56 reserved */ 8, /* reserved for Apple use */
  /* 0x57 reserved */ 8, /* reserved for Apple use */
  /* 0x58 frameSameOval */ 0, /* rect */
  /* 0x59 paintSameOval */ 0, /* rect */
  /* 0x5a eraseSameOval */ 0, /* rect */
  /* 0x5b invertSameOval */ 0, /* rect */
  /* 0x5c fillSameOval */ 0, /* rect */
  /* 0x5d reserved */ 0, /* reserved for Apple use */
  /* 0x5e reserved */ 0, /* reserved for Apple use */
  /* 0x5f reserved */ 0, /* reserved for Apple use */
  /* 0x60 frameArc */ 12, /* rect, startAngle, arcAngle */
  /* 0x61 paintArc */ 12, /* rect, startAngle, arcAngle */
  /* 0x62 eraseArc */ 12, /* rect, startAngle, arcAngle */
  /* 0x63 invertArc */ 12, /* rect, startAngle, arcAngle */
  /* 0x64 fillArc */ 12, /* rect, startAngle, arcAngle */
  /* 0x65 reserved */ 12, /* reserved for Apple use */
  /* 0x66 reserved */ 12, /* reserved for Apple use */
  /* 0x67 reserved */ 12, /* reserved for Apple use */
  /* 0x68 frameSameArc */ 4, /* rect, startAngle, arcAngle */
  /* 0x69 paintSameArc */ 4, /* rect, startAngle, arcAngle */
  /* 0x6a eraseSameArc */ 4, /* rect, startAngle, arcAngle */
  /* 0x6b invertSameArc */ 4, /* rect, startAngle, arcAngle */
  /* 0x6c fillSameArc */ 4, /* rect, startAngle, arcAngle */
  /* 0x6d reserved */ 4, /* reserved for Apple use */
  /* 0x6e reserved */ 4, /* reserved for Apple use */
  /* 0x6f reserved */ 4, /* reserved for Apple use */
  /* 0x70 framePoly */ 0, /* poly */
  /* 0x71 paintPoly */ 0, /* poly */
  /* 0x72 erasePoly */ 0, /* poly */
  /* 0x73 invertPoly */ 0, /* poly */
  /* 0x74 fillPoly */ 0, /* poly */
  /* 0x75 reserved */ 0, /* reserved for Apple use */
  /* 0x76 reserved */ 0, /* reserved for Apple use */
  /* 0x77 reserved */ 0, /* reserved for Apple use */
  /* 0x78 frameSamePoly */ 0, /* poly (NYI) */
  /* 0x79 paintSamePoly */ 0, /* poly (NYI) */
  /* 0x7a eraseSamePoly */ 0, /* poly (NYI) */
  /* 0x7b invertSamePoly */ 0, /* poly (NYI) */
  /* 0x7c fillSamePoly */ 0, /* poly (NYI) */
  /* 0x7d reserved */ 0, /* reserved for Apple use */
  /* 0x7e reserved */ 0, /* reserved for Apple use */
  /* 0x7f reserved */ 0, /* reserved for Apple use */
  /* 0x80 frameRgn */ 0, /* region */
  /* 0x81 paintRgn */ 0, /* region */
  /* 0x82 eraseRgn */ 0, /* region */
  /* 0x83 invertRgn */ 0, /* region */
  /* 0x84 fillRgn */ 0, /* region */
  /* 0x85 reserved */ 0, /* reserved for Apple use */
  /* 0x86 reserved */ 0, /* reserved for Apple use */
  /* 0x87 reserved */ 0, /* reserved for Apple use */
  /* 0x88 frameSameRgn */ 0, /* region (NYI) */
  /* 0x89 paintSameRgn */ 0, /* region (NYI) */
  /* 0x8a eraseSameRgn */ 0, /* region (NYI) */
  /* 0x8b invertSameRgn */ 0, /* region (NYI) */
  /* 0x8c fillSameRgn */ 0, /* region (NYI) */
  /* 0x8d reserved */ 0, /* reserved for Apple use */
  /* 0x8e reserved */ 0, /* reserved for Apple use */
  /* 0x8f reserved */ 0, /* reserved for Apple use */
  /* 0x90 BitsRect */ 0, /* copybits, rect clipped */
  /* 0x91 BitsRgn */ 0, /* copybits, rgn clipped */
  /* 0x92 reserved */ -1, /* reserved for Apple use */
  /* 0x93 reserved */ -1, /* reserved for Apple use */
  /* 0x94 reserved */ -1, /* reserved for Apple use */
  /* 0x95 reserved */ -1, /* reserved for Apple use */
  /* 0x96 reserved */ -1, /* reserved for Apple use */
  /* 0x97 reserved */ -1, /* reserved for Apple use */
  /* 0x98 PackBitsRect */ 0, /* packed copybits, rect clipped */
  /* 0x99 PackBitsRgn */ 0, /* packed copybits, rgn clipped */
  /* 0x9a DirectBitsRect */ 0, /* PixMap, srcRect, dstRect, mode, PixData */
  /* 0x9b DirectBitsRgn */ 0, /* PixMap, srcRect, dstRect, mode, maskRgn, PixData */
  /* 0x9c reserved */ -1, /* reserved for Apple use */
  /* 0x9d reserved */ -1, /* reserved for Apple use */
  /* 0x9e reserved */ -1, /* reserved for Apple use */
  /* 0x9f reserved */ -1, /* reserved for Apple use */
  /* 0xa0 ShortComment */ 2, /* kind (word) */
  /* 0xa1 LongComment */ 0, /* kind (word), size (word), data */
};
#if 1
/* �����޷�����Ĳ����� */
static int _skip_ign_data(ISFILE* pfile, short opcode)
{
  PCT_RECT rc;
  short stmp;
  long ltmp;
  int result = 0;
  do {
    if (opcode <= 0xa1) {
      switch (opcode) {
      case PCTDEF_CLIP:
        if (_read_word(pfile, &stmp)) {
          break;
        }
        if (stmp != 0x0a) {
          if (_skip_and_skip(pfile, stmp - 2) != 0) {
            break;
          }
          break;
        }
        if (_read_pictrect(pfile, &rc)) {
          break;
        }
        break;
      case PCTDEF_BKPIXPAT:
      case PCTDEF_PNPIXPAT:
      case PCTDEF_FILLPIXPAT:
        assert(0); /* δʵ�֣�����������Ҳ�Ǻ��鷳���� */
        break;
      case PCTDEF_FRAMEPOLY:
      case PCTDEF_PAINTPOLY:
      case PCTDEF_ERASEPOLY:
      case PCTDEF_INVERTPOLY:
      case PCTDEF_FILLPOLY:
      case PCTDEF_REV0075:
      case PCTDEF_REV0076:
      case PCTDEF_REV0077:
        if (_read_word(pfile, &stmp)) {
          break;
        }
        if (_skip_and_skip(pfile, stmp - 2) != 0) {
          break;
        }
        break;
      case PCTDEF_LONGCOMMENT:
        if (_read_word(pfile, &stmp) || _read_word(pfile, &stmp)) {
          break;
        }
        if (stmp > 0)
          if (_skip_and_skip(pfile, stmp) != 0) {
            break;
          }
        break;
      default:
        if (codesize[opcode] == -1) {
          if (_skip_and_skip(pfile, 2) != 0) {
            break;
          }
        }
        else {
          if (_skip_and_skip(pfile, codesize[opcode]) != 0) {
            break;
          }
        }
        break;
      }
      return result;
    }
    if (opcode == PCTDEF_HEADEROP) {
      _skip_and_skip(pfile, 24);
      return result;
    }
    if (((opcode >= 0xb0) && (opcode <= 0xcf)) ||
        ((opcode >= 0x8000) && (opcode <= 0x80ff))) {
      return result;
    }
    if (((opcode >= 0xd0) && (opcode <= 0xfe)) ||
        ((opcode >= 0x8100) && (opcode <= 0xffff))) {
      if (_read_word(pfile, &stmp)) {
        break;
      }
      if (_skip_and_skip(pfile, stmp - 2) != 0) {
        break;
      }
      return result;
    }
    if (opcode == PCTDEF_COMPQT) {
      if (_read_dword(pfile, &ltmp)) {
        break;
      }
      if (_skip_and_skip(pfile, (int)ltmp) != 0) {
        break;
      }
      return result;
    }
    if ((opcode >= 0x100) && (opcode <= 0x7fff)) {
      // Skip reserved.
      stmp = (opcode >> 7) & 0xff;
      if (_skip_and_skip(pfile, stmp) != 0) {
        break;
      }
      return result;
    }
  }
  while (0);
  
    if (1) {
      result = -1; /* ����д�� */
    }
  
  return result;
}
/* �����޷�����Ĳ����� */
/* ��֪�������д����������д�̫���ˣ������ۻ� */
#else
static int _skip_ign_data(ISFILE* pfile, short opcode)
{
  int result = 0;
  long ltmp;
  short stmp;
  char btmp;
  do {
    switch ((int)(unsigned int)(ushort)opcode) {
    case PCTDEF_NOP: /* No operation 0 */
    case PCTDEF_REV0017: /* Reserved for Apple use Not determined */
    case PCTDEF_REV0018: /* Reserved for Apple use Not determined */
    case PCTDEF_REV0019: /* Reserved for Apple use Not determined */
    case PCTDEF_HILITEMODE: /* HiliteMode Highlight mode flag: no data; this opcode is sent before a drawing operation that uses the highlight mode 0 */
    case PCTDEF_DEFHILITE: /* DefHilite Use default highlight color; no data; set highlight to default (from low memory) 0 */
    case PCTDEF_FRAMESAMERT: /* frameSameRect Rectangle (Rect) 0 */
    case PCTDEF_PAINTSAMERT: /* paintSameRect Rectangle (Rect) 0 */
    case PCTDEF_ERASESAMERT: /* eraseSameRect Rectangle (Rect) 0 */
    case PCTDEF_INVERTSAMERT: /* invertSameRect Rectangle (Rect) 0 */
    case PCTDEF_FILLSAMERECT: /* fillSameRect Rectangle (Rect) 0 */
    case PCTDEF_REV003D: /* Reserved for Apple use 0 */
    case PCTDEF_REV003E: /* Reserved for Apple use 0 */
    case PCTDEF_REV003F: /* Reserved for Apple use 0 */
    case PCTDEF_FRAMESAMERRT: /* frameSameRRect Rectangle (Rect) 0 */
    case PCTDEF_PAINTSAMERRT: /* paintSameRRect Rectangle (Rect) 0 */
    case PCTDEF_ERASESAMERRT: /* eraseSameRRect Rectangle (Rect) 0 */
    case PCTDEF_INVSAMERRT: /* invertSameRRect Rectangle (Rect) 0 */
    case PCTDEF_FILLSAMERRT: /* fillSameRRect Rectangle (Rect) 0 */
    case PCTDEF_REV004D: /* Reserved for Apple use 0 */
    case PCTDEF_REV004E: /* Reserved for Apple use 0 */
    case PCTDEF_REV004F: /* Reserved for Apple use 0 */
    case PCTDEF_FRAMESAMEOV: /* frameSameOval Rectangle (Rect) 0 */
    case PCTDEF_PAINTSAMEOV: /* paintSameOval Rectangle (Rect) 0 */
    case PCTDEF_ERASESAMEOV: /* eraseSameOval Rectangle (Rect) 0 */
    case PCTDEF_INVERTSAMEOV: /* invertSameOval Rectangle (Rect) 0 */
    case PCTDEF_FILLSAMEOV: /* fillSameOval Rectangle (Rect) 0 */
    case PCTDEF_REV005D: /* Reserved for Apple use 0 */
    case PCTDEF_REV005E: /* Reserved for Apple use 0 */
    case PCTDEF_REV005F: /* Reserved for Apple use 0 */
    case PCTDEF_FRAMESAMEPL: /* frameSamePoly (Not yet implemented) 0 */
    case PCTDEF_PAINTSAMEPL: /* paintSamePoly (Not yet implemented) 0 */
    case PCTDEF_ERASESAMEPL: /* eraseSamePoly (Not yet implemented) 0 */
    case PCTDEF_INVERTSAMEPL: /* invertSamePoly (Not yet implemented) 0 */
    case PCTDEF_FILLSAMEPL: /* fillSamePoly (Not yet implemented) 0 */
    case PCTDEF_REV007D: /* Reserved for Apple use 0 */
    case PCTDEF_REV007E: /* Reserved for Apple use 0 */
    case PCTDEF_REV007F: /* Reserved for Apple use 0 */
    case PCTDEF_FRAMESAMERGN: /* frameSameRgn (Not yet implemented) 0 */
    case PCTDEF_PAINTSAMERGN: /* paintSameRgn (Not yet implemented) 0 */
    case PCTDEF_ERASESAMERGN: /* eraseSameRgn (Not yet implemented) 0 */
    case PCTDEF_INVSAMERGN: /* invertSameRgn (Not yet implemented) 0 */
    case PCTDEF_FILLSAMERGN: /* fillSameRgn (Not yet implemented) 0 */
    case PCTDEF_REV008D: /* Reserved for Apple use 0 */
    case PCTDEF_REV008E: /* Reserved for Apple use 0 */
    case PCTDEF_REV008F: /* Reserved for Apple use 0 */
      break;
    case PCTDEF_CLIP: /* Clipping region Region size */
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _skip_and_skip(pfile, (int)(unsigned int)(ushort)stmp - 2)) != 0) {
        break;
      }
      break;
    case PCTDEF_BKPAT: /* BkPat Background pattern 8 */
    case PCTDEF_PNPAT: /* PnPat Pen pattern 8 */
    case PCTDEF_FILLPAT: /* FillPat Fill pattern 8 */
    case PCTDEF_TXRATIO: /* TxRatio Numerator (Point), denominator (Point) 8 */
    case PCTDEF_LINE: /* Line pnLoc (Point), newPt (Point) 8 */
    case PCTDEF_GLYPHSTATE: /* glyphState Data length (word), followed by these 1-byte Boolean values: outline preferred, preserve glyph, fractional widths, scaling disabled 8 */
    case PCTDEF_FRAMERECT: /* frameRect Rectangle (Rect) 8 */
    case PCTDEF_PAINTRECT: /* paintRect Rectangle (Rect) 8 */
    case PCTDEF_ERASERECT: /* eraseRect Rectangle (Rect) 8 */
    case PCTDEF_INVERTRECT: /* invertRect Rectangle (Rect) 8 */
    case PCTDEF_FILLRECT: /* fillRect Rectangle (Rect) 8 */
    case PCTDEF_REV0035: /* Reserved for Apple use 8 bytes of data 8 */
    case PCTDEF_REV0036: /* Reserved for Apple use 8 bytes of data 8 */
    case PCTDEF_REV0037: /* Reserved for Apple use 8 bytes of data 8 */
    case PCTDEF_FRAMERRECT: /* frameRRect Rectangle (Rect)?8 */
    case PCTDEF_PAINTRRECT: /* paintRRect Rectangle (Rect)?8 */
    case PCTDEF_ERASERRECT: /* eraseRRect Rectangle (Rect)?8 */
    case PCTDEF_INVERTRRECT: /* invertRRect Rectangle (Rect)?8 */
    case PCTDEF_FILLRRECT: /* fillRRect Rectangle (Rect)?8 */
    case PCTDEF_REV0045: /* Reserved for Apple use 8 bytes of data 8 */
    case PCTDEF_REV0046: /* Reserved for Apple use 8 bytes of data 8 */
    case PCTDEF_REV0047: /* Reserved for Apple use 8 bytes of data 8 */
    case PCTDEF_FRAMEOVAL: /* frameOval Rectangle (Rect) 8 */
    case PCTDEF_PAINTOVAL: /* paintOval Rectangle (Rect) 8 */
    case PCTDEF_ERASEOVAL: /* eraseOval Rectangle (Rect) 8 */
    case PCTDEF_INVERTOVAL: /* invertOval Rectangle (Rect) 8 */
    case PCTDEF_FILLOVAL: /* fillOval Rectangle (Rect) 8 */
    case PCTDEF_REV0055: /* Reserved for Apple use 8 bytes of data 8 */
    case PCTDEF_REV0056: /* Reserved for Apple use 8 bytes of data 8 */
    case PCTDEF_REV0057: /* Reserved for Apple use 8 bytes of data 8 */
      if ((result = _skip_and_skip(pfile, 8)) != 0) {
        break;
      }
      break;
    case PCTDEF_HILITECOLOR: /* HiliteColor Highlight color (RGBColor) 6 */
    case PCTDEF_RGBBKCOL: /* RGBBkCol Background color (RGBColor) 6 */
    case PCTDEF_OPCOLOR: /* OpColor Opcolor (RGBColor) 6 */
    case PCTDEF_SHORTLINE: /* ShortLine pnLoc (Point), dh (?28..127), dv (?28..127) 6 */
    case PCTDEF_RGBFGCOL: /* RGBFgCol Foreground color (RGBColor) 6 */
      if ((result = _skip_and_skip(pfile, 6)) != 0) {
        break;
      }
      break;
    case PCTDEF_SPEXTRA: /* SpExtra Extra space (Fixed) 4 */
    case PCTDEF_PNSIZE: /* PnSize Pen size (Point) 4 */
    case PCTDEF_OVSIZE: /* OvSize Oval size (Point) 4 */
    case PCTDEF_ORIGIN: /* Origin dh, dv (Integer) 4 */
    case PCTDEF_FGCOLOR: /* FgColor Foreground color (Long) 4 */
    case PCTDEF_BKCOLOR: /* BkColor Background color (Long) 4 */
    case PCTDEF_LINEFROM: /* LineFrom newPt (Point) 4 */
    case PCTDEF_FRAMESAMEARC: /* frameSameArc Rectangle (Rect) 4 */
    case PCTDEF_PAINTSAMEARC: /* paintSameArc Rectangle (Rect) 4 */
    case PCTDEF_ERASESAMEARC: /* eraseSameArc Rectangle (Rect) 4 */
    case PCTDEF_INVSAMEARC: /* invertSameArc Rectangle (Rect) 4 */
    case PCTDEF_FILLSAMEARC: /* fillSameArc Rectangle (Rect) 4 */
    case PCTDEF_REV006D: /* Reserved for Apple use 4 bytes of data 4 */
    case PCTDEF_REV006E: /* Reserved for Apple use 4 bytes of data 4 */
    case PCTDEF_REV006F: /* Reserved for Apple use 4 bytes of data 4 */
      if ((result = _skip_and_skip(pfile, 4)) != 0) {
        break;
      }
      break;
    case PCTDEF_TXFONT: /* TxFont Font number for text (Integer) 2 */
    case PCTDEF_TXMODE: /* TxMode Source mode (Integer) 2 */
    case PCTDEF_PNMODE: /* PnMode Pen mode (Integer) 2 */
    case PCTDEF_TXSIZE: /* TxSize Text size (Integer) 2 */
    case PCTDEF_PNLOCHFRAC: /* PnLocHFrac Fractional pen position (Integer�low word of Fixed); 2*/
    case PCTDEF_CHEXTRA: /* ChExtra Added width for nonspace characters (Integer) 2*/
    case PCTDEF_SHORTLINEFR: /* ShortLineFrom dh (?28..127), dv (?28..127) 2 */
    case PCTDEF_SHORTCOMMENT: /* ShortComment Kind (Integer) 2 */
    case PCTDEF_VERSION: /* Version Version number of picture 2 */
      if ((result = _skip_and_skip(pfile, 2)) != 0) {
        break;
      }
      break;
    case PCTDEF_TXFACE: /* TxFace Text's font style (0..255) 1 */
    case PCTDEF_VERSIONOP: /* VersionOp Version (0..255) 1 */
      if ((result = _skip_and_skip(pfile, 1)) != 0) {
        break;
      }
      break;
    case PCTDEF_BKPIXPAT: /* BkPixPat Background pixel pattern Variable; see Listing A-1 on page A-17 */
      break;
    case PCTDEF_PNPIXPAT: /* PnPixPat Pen pixel pattern Variable; see Listing A-1 on page A-17 */
      break;
    case PCTDEF_FILLPIXPAT: /* FillPixPat Fill pixel pattern Variable; see Listing A-1 on page A-17 */
      break;
    case PCTDEF_REV0024: /* Reserved for Apple use Data length (Integer), data 2 + data length */
    case PCTDEF_REV0025: /* Reserved for Apple use Data length (Integer), data 2 + data length */
    case PCTDEF_REV0026: /* Reserved for Apple use Data length (Integer), data 2 + data length */
    case PCTDEF_REV0027: /* Reserved for Apple use Data length (Integer), data 2 + data length */
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _skip_and_skip(pfile, stmp)) != 0) {
        break;
      }
      break;
    case PCTDEF_LONGTEXT: /* LongText txLoc (Point), count (0..255), text 5 + text */
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _read_byte(pfile, &btmp)) != 0) {
        break;
      }
      if ((result = _skip_and_skip(pfile, (int)(unsigned int)(uchar)btmp)) != 0) {
        break;
      }
      break;
    case PCTDEF_DHTEXT: /* DHText dh (0..255), count (0..255), text 2 + text */
    case PCTDEF_DVTEXT: /* DVText dv (0..255), count (0..255), text 2 + text */
      if ((result = _read_byte(pfile, &btmp)) != 0) {
        break;
      }
      if ((result = _read_byte(pfile, &btmp)) != 0) {
        break;
      }
      if ((result = _skip_and_skip(pfile, (int)(unsigned int)(uchar)btmp)) != 0) {
        break;
      }
      break;
    case PCTDEF_DHDVTEXT: /* DHDVText dh (0..255), dv (0..255), count (0..255), text 3 + text */
      if ((result = _read_byte(pfile, &btmp)) != 0) {
        break;
      }
      if ((result = _read_byte(pfile, &btmp)) != 0) {
        break;
      }
      if ((result = _read_byte(pfile, &btmp)) != 0) {
        break;
      }
      if ((result = _skip_and_skip(pfile, (int)(unsigned int)(uchar)btmp)) != 0) {
        break;
      }
      break;
    case PCTDEF_FONTNAME: /* fontName Data length (Integer), old font ID (Integer), name length (0..255), font name* 5 + name length */
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _read_byte(pfile, &btmp)) != 0) {
        break;
      }
      if ((result = _skip_and_skip(pfile, (int)(unsigned int)(uchar)btmp)) != 0) {
        break;
      }
      break;
    case PCTDEF_LINEJUSTIFY: /* lineJustify Operand data length (Integer), intercharacter spacing (Fixed), total extra space for justification (Fixed) 10 */
      if ((result = _skip_and_skip(pfile, 10)) != 0) {
        break;
      }
      break;
    case PCTDEF_REV002F: /* Reserved for Apple use Data length (Integer), data 2 + data length */
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _skip_and_skip(pfile, (int)(unsigned int)(ushort)stmp)) != 0) {
        break;
      }
      break;
    case PCTDEF_FRAMEARC: /* frameArc Rectangle (Rect), startAngle, arcAngle 12 */
    case PCTDEF_PAINTARC: /* paintArc Rectangle (Rect), startAngle, arcAngle 12 */
    case PCTDEF_ERASEARC: /* eraseArc Rectangle (Rect), startAngle, arcAngle 12 */
    case PCTDEF_INVERARC: /* invertArc Rectangle (Rect), startAngle,arcAngle 12 */
    case PCTDEF_FILLARC: /* fillArc Rectangle (Rect), startAngle, arcAngle 12 */
    case PCTDEF_REV0065: /* Reserved for Apple use 12 bytes of data 12 */
    case PCTDEF_REV0066: /* Reserved for Apple use 12 bytes of data 12 */
    case PCTDEF_REV0067: /* Reserved for Apple use 12 bytes of data 12 */
      if ((result = _skip_and_skip(pfile, 12)) != 0) {
        break;
      }
      break;
    case PCTDEF_FRAMEPOLY: /* framePoly Polygon (Poly) Polygon size */
    case PCTDEF_PAINTPOLY: /* paintPoly Polygon (Poly) Polygon size */
    case PCTDEF_ERASEPOLY: /* erasePoly Polygon (Poly) Polygon size */
    case PCTDEF_INVERTPOLY: /* invertPoly Polygon (Poly) Polygon size */
    case PCTDEF_FILLPOLY: /* fillPoly Polygon (Poly) Polygon size */
    case PCTDEF_REV0075: /* Reserved for Apple use Polygon (Poly) Polygon size */
    case PCTDEF_REV0076: /* Reserved for Apple use Polygon (Poly) Polygon size */
    case PCTDEF_REV0077: /* Reserved for Apple use Polygon (Poly) Polygon size */
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      ltmp = (long)(int)(unsigned int)(ushort)stmp;
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _skip_and_skip(pfile, (int)ltmp)) != 0) {
        break;
      }
      break;
    case PCTDEF_FRAMERGN: /* frameRgn Region (Rgn) Region size */
    case PCTDEF_PAINTRGN: /* paintRgn Region (Rgn) Region size */
    case PCTDEF_ERASERGN: /* eraseRgn Region (Rgn) Region size */
    case PCTDEF_INVERTRGN: /* invertRgn Region (Rgn) Region size */
    case PCTDEF_FILLRGN: /* fillRgn Region (Rgn) Region size */
    case PCTDEF_REV0085: /* Reserved for Apple use Region (Rgn) Region size */
    case PCTDEF_REV0086: /* Reserved for Apple use Region (Rgn) Region size */
    case PCTDEF_REV0087: /* Reserved for Apple use Region (Rgn) Region size */
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      ltmp = (long)(int)(unsigned int)(ushort)stmp;
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _skip_and_skip(pfile, (int)ltmp)) != 0) {
        break;
      }
      break;
    case PCTDEF_BITSRECT: /* 0x0090 - BitsRect CopyBits with clipped rectangle Variable��; see Listing A-2 on page A-17 */
    case PCTDEF_PACKBITSRECT: /* 0x0098 - PackBitsRect Packed CopyBits with clipped rectangle Variable? see Listing A-2 on page A-17 */
    case PCTDEF_BITSRGN: /* 0x0091 - BitsRgn CopyBits with clipped region Variable��; see Listing A-3 on page A-18 */
    case PCTDEF_PACKBITSRGN: /* 0x0099 - PackBitsRgn Packed CopyBits with clipped rectangle Variable? see Listing A-3 on page A-18 */
    case PCTDEF_DIRBITSRT: /* 0x009A - DirectBitsRect PixMap, srcRect, dstRect, mode (Integer), PixData Variable */
    case PCTDEF_DIRBITSRGN: /* 0x009B - DirectBitsRgn PixMap, srcRect, dstRect, mode (Integer), maskRgn, PixData Variable */
      assert(0); /* ��Щ������Ӧ�ɵ�����ȥ��������Ӧ�ó��������� */
      break;
    case PCTDEF_REV0092: /* Reserved for Apple use Data length (Integer), data 2 + data length */
    case PCTDEF_REV0093:
    case PCTDEF_REV0094:
    case PCTDEF_REV0095:
    case PCTDEF_REV0096:
    case PCTDEF_REV0097:
    case PCTDEF_REV009C:
    case PCTDEF_REV009D:
    case PCTDEF_REV009E:
    case PCTDEF_REV009F:
    case PCTDEF_REV00A2:
    case PCTDEF_REV00A3:
    case PCTDEF_REV00A4:
    case PCTDEF_REV00A5:
    case PCTDEF_REV00A6:
    case PCTDEF_REV00A7:
    case PCTDEF_REV00A8:
    case PCTDEF_REV00A9:
    case PCTDEF_REV00AA:
    case PCTDEF_REV00AB:
    case PCTDEF_REV00AC:
    case PCTDEF_REV00AD:
    case PCTDEF_REV00AE:
    case PCTDEF_REV00AF:
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _skip_and_skip(pfile, (int)(unsigned int)(ushort)stmp)) != 0) {
        break;
      }
      break;
    case PCTDEF_LONGCOMMENT: /* LongComment Kind (Integer), size (Integer), data 4 + data */
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _read_word(pfile, &stmp)) != 0) {
        break;
      }
      if ((result = _skip_and_skip(pfile, (int)(unsigned int)(ushort)stmp)) != 0) {
        break;
      }
      break;
    case PCTDEF_OPENDPIC: /* OpEndPic End of picture 2 */
      /* �����뽫�������� */
      break;
    case PCTDEF_HEADEROP: /* HeaderOp For extended version 2: version (Integer), reserved (Integer), hRes, vRes (Fixed), srcRect, reserved (Long); for version 2: opcode 24 */
      if ((result = _skip_and_skip(pfile, 24)) != 0) {
        break;
      }
      break;
    case PCTDEF_COMPQT: /* CompressedQuickTime Data length (Long), data (private to QuickTime) 4 + data length */
      if ((result = _read_dword(pfile, &ltmp)) != 0) {
        break;
      }
      if ((result = _skip_and_skip(pfile, (int)ltmp)) != 0) {
        break;
      }
      break;
    case PCTDEF_UNCOMPQT: /* UncompressedQuickTime Data length (Long), data (private to QuickTime) 4 + data length */
      if ((result = _read_dword(pfile, &ltmp)) != 0) {
        break;
      }
      if ((result = _skip_and_skip(pfile, (int)ltmp)) != 0) {
        break;
      }
      break;
    default: {
      if (((int)(unsigned int)(ushort)opcode >= PCTDEF_REV00B0) && \
          ((int)(unsigned int)(ushort)opcode <= PCTDEF_REV00CF)) {
        break;
      }
      else if (((int)(unsigned int)(ushort)opcode >= PCTDEF_REV00D0) && \
          ((int)(unsigned int)(ushort)opcode <= PCTDEF_REV00FE)) {
        if ((result = _read_dword(pfile, &ltmp)) != 0) {
          break;
        }
        if ((result = _skip_and_skip(pfile, (int)ltmp)) != 0) {
          break;
        }
        break;
      }
      else if (((int)(unsigned int)(ushort)opcode >= PCTDEF_REV0100) && \
          ((int)(unsigned int)(ushort)opcode <= PCTDEF_REV01FF)) {
        if ((result = _skip_and_skip(pfile, 2)) != 0) {
          break;
        }
        break;
      }
      else if (((int)(unsigned int)(ushort)opcode >= PCTDEF_REV0200) && \
          ((int)(unsigned int)(ushort)opcode <= PCTDEF_REV7FFF)) {
        if ((result = _skip_and_skip(pfile, (((int)(unsigned int)(ushort)opcode) >> 8) * 2)) != 0) {
          break;
        }
        break;
      }
      else if (((int)(unsigned int)(ushort)opcode >= (int)(unsigned int)(ushort)PCTDEF_REV8000) && \
          ((int)(unsigned int)(ushort)opcode <= (int)(unsigned int)(ushort)PCTDEF_REV80FF)) {
        break;
      }
      else if (((int)(unsigned int)(ushort)opcode >= (int)(unsigned int)(ushort)PCTDEF_REV8100) && \
          ((int)(unsigned int)(ushort)opcode <= (int)(unsigned int)(ushort)PCTDEF_REVFFFF)) {
        if ((result = _read_dword(pfile, &ltmp)) != 0) {
          break;
        }
        if ((result = _skip_and_skip(pfile, (int)ltmp)) != 0) {
          break;
        }
        break;
      }
    }
    }
  }
  while (0);
  
    if (1) {
      result = -1; /* ����д�� */
    }
  
  return result;
}
#endif
/* ��ȡһ��˫�֣�dword����תΪX86����*/
static int _read_dword(ISFILE* pfile, long* ps)
{
  if (isio_read((void*)ps, 4, 1, pfile) == 0) {
    return -1; /* ��д�� */
  }
  *ps = EXCHANGE_DWORD((*ps));
  return 0; /* ��ȡ�ɹ� */
}
/* ��ȡһ���֣�word����תΪX86����*/
static int _read_word(ISFILE* pfile, short* ps)
{
  if (isio_read((void*)ps, 2, 1, pfile) == 0) {
    return -1; /* ��д�� */
  }
  *ps = EXCHANGE_WORD((*ps));
  return 0; /* ��ȡ�ɹ� */
}
/* ��ȡһ���ֽڣ�byte��*/
static int _read_byte(ISFILE* pfile, char* ps)
{
  if (isio_read((void*)ps, 1, 1, pfile) == 0) {
    return -1; /* ��д�� */
  }
  return 0; /* ��ȡ�ɹ� */
}
/* ����дλ������ n ���ֽ� */
static int _skip_and_skip(ISFILE* pfile, int n)
{
  if (isio_seek(pfile, n, SEEK_CUR) == -1) {
    return -1; /* ��λ���� */
  }
  return 0;
}
/* ��һ������ */
static int _read_pictrect(ISFILE* pfile, LPPCT_RECT prc)
{
  PCT_RECT rc;
  if (isio_read(&rc, sizeof(PCT_RECT), 1, pfile) == 0) {
    return -1;
  }
  prc->left = EXCHANGE_WORD(rc.left);
  prc->top = EXCHANGE_WORD(rc.top);
  prc->right = EXCHANGE_WORD(rc.right);
  prc->bottom = EXCHANGE_WORD(rc.bottom);
  return 0;
}
/* ���PCTͼ���ײ���������Ϣ�� */
static enum EXERESULT _read_header(ISFILE* pfile, int* pversion)
{
  PCT_MAGIC magic;
  short stmp;
  char c;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile);
  do {
    /* �����ͷ���հ�����PS������PCTͼ����д˿հ���������Ϊ512�ֽ�
    * ������Ϊȫ0������Apple���ĵ��в�û�б��������һ������������
    * �ĵ��Բ��ϵ�����ڻ��кܶࡣ
    * (��������ר����Ӧ�ó������Ϣ menghui)
    */
    if (isio_seek(pfile, 0x200, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* ��ȡͷ�ṹ */
    if (isio_read((void*)&magic, sizeof(PCT_MAGIC), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* תΪX86���� */
    /* size������2.0�������ڱ�ʾ����64K�ĵ�16λ */
    magic.size = EXCHANGE_WORD((magic.size));
    magic.top = EXCHANGE_WORD((magic.top));
    magic.left = EXCHANGE_WORD((magic.left));
    magic.bottom = EXCHANGE_WORD((magic.bottom));
    magic.right = EXCHANGE_WORD((magic.right));
    /* ����0x11 Ϊ�汾2��0x1101Ϊ�汾1 */
    if (_read_word(pfile, &stmp)) {
      b_status = ER_BADIMAGE;
      break;
    }
    if (stmp != 0x11 && stmp != 0x1101) {
      b_status = ER_BADIMAGE;
      break;
    }
    /*
    * ����˵�����Ϊ�汾2���汾ռһ��word��Ϊ0x02ff
    * ���Ϊ�汾1���汾�Ž�ռһ�ֽڣ�Ϊ0x01
    * (����δ�ҵ�PICT Version 1��ͼ���޷���֤)
    * menghui 2003.06.18
    */
    if (_read_byte(pfile, &c)) {
      b_status = ER_BADIMAGE;
      break;
    }
    if (c == 0x2) {
      if (_read_byte(pfile, &c) && c != 0xff) {
        b_status = ER_BADIMAGE;
        break;
      }
      *pversion = 2;
    }
    else if (c == 0x1) {
      *pversion = 1;
    }
    else {
      b_status = ER_BADIMAGE;
      break;
    }
  }
  while (0);
  {
    if (1) {
      b_status = ER_FILERWERR;
    }
  }
  return b_status;
}

