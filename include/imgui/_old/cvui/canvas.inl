#ifndef _CANVAS_INL_
#define _CANVAS_INL_
#include "cstd.h"
#include "geo.inl"
enum {
#define HTML_COLOR_DEF(NAME, CLR) HCLR_##NAME = 0xFF##CLR ,
#include "html_color.txt"
#undef HTML_COLOR_DEF
};
#define xypos_toint(x) ((int)(x))
#define RADIAN (PI/180.0)
// ����ĩ����ñ����ʽ��
enum {
  lineCap_butt, // Ĭ�ϡ���������ÿ��ĩ�����ƽֱ�ı�Ե��
  lineCap_round, // ��������ÿ��ĩ�����Բ����ñ��
  lineCap_square, // ��������ÿ��ĩ�������������ñ��
};
// �������߽���ʱ�߽ǵ�����
enum {
  lineJoin_bevel, // ����б�ǡ�
  lineJoin_round, // ����Բ��
  lineJoin_miter, // Ĭ�ϡ�������ǡ�ֵ "miter" �� miterLimit ���Ե�Ӱ�졣
};
BOOL color_parse(const char* s, int l, COLOR* pout)
{
  char str[16] = {0};
  const char* e;
  l = l > 0 ? l : strlen(s);
  e = s + l;
  for (; ' ' == *s || '\t' == *s; ++s);
  if ('#' == s[0]) {
    memset(str, 'f', 8);
    ++s;
    l = e - s;
    switch (l) {
    case 4:
      str[0] = str[1] = s[0], str[2] = str[3] = s[1], str[4] = str[5] = s[2], str[6] = str[7] = s[3];
      break;
    case 3:
      str[2] = str[3] = s[0], str[4] = str[5] = s[1], str[6] = str[7] = s[2];
      break;
    case 2:
      str[0] = str[1] = s[0], str[4] = str[5] = str[6] = str[7] = s[1];
      break;
    case 1:
      str[4] = str[5] = str[6] = str[7] = s[0];
      break;
    default:
      memcpy(str + (8 - l), s, l);
      break;
    }
    *pout = cstr_hex2int(str, 8);
    return TRUE;
  }
  else {
    char* p;
    static const char* sss = "|"
#define HTML_COLOR_DEF(NAME, CLR) #NAME"|"
#include "html_color.txt"
#undef HTML_COLOR_DEF
        ;
    static const COLOR ccc[] = {
#define HTML_COLOR_DEF(NAME, CLR) 0xFF##CLR,
#include "html_color.txt"
#undef HTML_COLOR_DEF
    };
    static int ssslen = strlen(sss);
    int k = cstr_splitfind(sss, ssslen, s, l, 1, NULL);
    if (k >= 0) {
      *pout = ccc[k];
    }
#define HTML_COLOR_DEF(NAME, CLR) if (0==cstr_icmp(s, l, #NAME , -1, 1)) { *pout = 0xFF##CLR; return TRUE; }
#include "html_color.txt"
#undef HTML_COLOR_DEF
    if (p = cstr_chr(s, l, '(')) {
      int ok = 1;
      double r = 0, g = 0, b = 0, a = 255;
      ++p;
      for (; p != e && *s != '('; ++s) {
        switch (*s) {
#define TTTT(ca, a) case ca: a = strtod(p, &p); a = a<1?(a*255+0.5):a; break
          TTTT('a', a);
          TTTT('r', r);
          TTTT('g', g);
          TTTT('b', b);
#undef TTTT
        default:
          ok = 0;
          break;
        }
        for (; ' ' == *p || ',' == *p || '\t' == *p; ++p);
      }
      if (ok) {
        *pout = _RGBA(r, g, b, a);
      }
      return ok;
    }
  }
  return FALSE;
}
typedef float xypos_t;
// Private so Surface objects can not be copied
typedef struct canvas_t canvas_t;
struct canvas_t {
  void* x;
#define canvas_release(s) s->release(s)
  int (*release)(canvas_t* s);
  // ��ɫ����ʽ
  // color|#xxxxxx
  // gradient x0 y0 x1 y1 [stop color]
  //x0 ���俪ʼ��� x ����
  //y0 ���俪ʼ��� y ����
  //x1 ���������� x ����
  //y1 ���������� y ����
  // RadialGradient
  //x0 ����Ŀ�ʼԲ�� x ����
  //y0 ����Ŀ�ʼԲ�� y ����
  //r0 ��ʼԲ�İ뾶
  //x1 ����Ľ���Բ�� x ����
  //y1 ����Ľ���Բ�� y ����
  //r1 ����Բ�İ뾶
#define canvas_fillStyle(s, clr) s->fillStyle(s, clr)
  int (*fillStyle)(canvas_t* s, const char* str); // ������ɫ��Ĭ��Ϊ��#000000������strokeStyleһ����ֵҲ��������ΪCSS��ɫֵ������������ģʽ
#define canvas_strokeStyle(s, clr) s->strokeStyle(s, clr)
  int (*strokeStyle)(canvas_t* s, const char* str); // ��������ɫ��Ĭ��Ϊ��#000000������ֵ��������ΪCSS��ɫֵ������������ģʽ���󡣶���
  // ��Ӱ
  int (*shadowColor)(canvas_t* s);// ���û򷵻�������Ӱ����ɫ
  int (*shadowBlur)(canvas_t* s);// ���û򷵻�������Ӱ��ģ������
  int (*shadowOffsetX)(canvas_t* s);// ���û򷵻���Ӱ����״��ˮƽ����
  int (*shadowOffsetY)(canvas_t* s);// ���û򷵻���Ӱ����״�Ĵ�ֱ����
  // ������ʽ
#define canvas_lineWidth(s, px) s->lineWidth(s, px)
  int (*lineWidth)(canvas_t* s, float px); // �����Ŀ�ȣ���λ�����أ�px����Ĭ��Ϊ1.0��
#define canvas_lineCap(s, type) s->lineCap(s, type)
  int (*lineCap)(canvas_t* s, int type); // �����Ķ˵���ʽ����butt���ޣ���round��Բͷ����square����ͷ���������Ϳɹ�ѡ��Ĭ��Ϊbutt��
#define canvas_lineJoin(s, type) s->lineJoin(s, type)
  int (*lineJoin)(canvas_t* s, int type); // ������ת�۴���ʽ����round��Բ�ǣ���bevel��ƽ�ǣ���miter����ǣ����֣����Ϳɹ�ѡ��Ĭ��Ϊmiter��
#define canvas_miterLimit(s, x) s->miterLimit(s, x)
  int (*miterLimit)(canvas_t* s, float x); //��������۽ǵ���������Ĭ��Ϊ10��
  // ����
#define canvas_rect(s, left, top, width, height) s->rect(s, left, top, width, height)
  int (*rect)(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height); // �������һ����֪���ϽǶ���λ���Լ���͸ߵľ��Σ������ɺ�Context�Ļ��������ƶ����þ��ε����ϽǶ��㡣������ʾ�������ϽǶ����x��y�����Լ����εĿ�͸ߡ�
#define canvas_fillRect(s, left, top, width, height) s->fillRect(s, left, top, width, height)
  int (*fillRect)(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height); // ���ơ�����䡱�ľ���
#define canvas_strokeRect(s, left, top, width, height) s->strokeRect(s, left, top, width, height)
  int (*strokeRect)(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height); // ���ƾ��Σ�����䣩
#define canvas_clearRect(s, left, top, width, height) s->clearRect(s, left, top, width, height)
  int (*clearRect)(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height); //�ڸ����ľ��������ָ��������
  // ·��
#define canvas_beginPath(s) s->beginPath(s)
  int (*beginPath)(canvas_t* s); // ��ʼһ��·���������õ�ǰ·��
#define canvas_closePath(s) s->closePath(s)
  int (*closePath)(canvas_t* s); // �����ӵ�ǰ��ص���ʼ���·��
#define canvas_clip(s) s->clip(s)
  int (*clip)(canvas_t* s); // ���ڰ������е�·���ڻ��������ü������򡣵���clip()����֮��ͼ�λ��ƴ���ֻ�Լ���������Ч������Ӱ��������Ļ�������Ĭ�ϼ�������Ϊ����Canvas����
#define canvas_fill(s) s->fill(s)
  int (*fill)(canvas_t* s); // ����ʹ�õ�ǰ������������·��������
#define canvas_stroke(s) s->stroke(s)
  int (*stroke)(canvas_t* s); // �����Ѷ����·����Ĭ����ɫ�Ǻ�ɫ��
#define canvas_moveTo(s, x, y) s->moveTo(s, x, y)
  int (*moveTo)(canvas_t* s, xypos_t x, xypos_t y); // ������ʽ��ָ��·������㡣������������Ĭ��״̬�£���һ��·��������ǻ�����(0, 0)�㣬֮����������һ��·�����յ㡣
#define canvas_lineTo(s, x, y) s->lineTo(s, x, y)
  int (*lineTo)(canvas_t* s, xypos_t x, xypos_t y); // �������һ��������ָ��λ�õ�ֱ��·���������ɺ���Ƶ������ƶ�����ָ��λ�á�
#define canvas_arcTo(s, x1, y1, x2, y2, radius) s->arcTo(s, x1, y1, x2, y2, radius)
  int (*arcTo)(canvas_t* s, xypos_t x1, xypos_t y1, xypos_t x2, xypos_t y2, xypos_t radius); // �������һ���������߶����е�Բ���������߶ηֱ��Ե�ǰContext��������(x2, y2)��Ϊ��㣬����(x1, y1)��Ϊ�յ㣬Բ���İ뾶Ϊradius�������ɺ���������ƶ�����(x2, y2)Ϊ�����߶���Բ�����е㡣
#define canvas_arc(s, x, y, radius, startAngle, endAngle, anticlockwise) s->arc(s, x, y, radius, startAngle, endAngle, anticlockwise)
  int (*arc)(canvas_t* s, xypos_t x, xypos_t y, xypos_t radius, xypos_t startAngle, xypos_t endAngle, BOOL anticlockwise); // �������һ����(x, y)��ΪԲ�ģ�radiusΪ�뾶��startAngleΪ��ʼ���ȣ�endAngleΪ��ֹ���ȵ�Բ����anticlockwiseΪ�����͵Ĳ�����true��ʾ��ʱ�룬false��ʾ˳ʱ�롣�����е�����������0��ʾ0�㣬λ����3���ӷ���Math.PIֵ��ʾ180�㣬λ����9���ӷ���
#define canvas_quadraticCurveTo(s, cpx, cpy, x, y) s->quadraticCurveTo(s, cpx, cpy, x, y)
  int (*quadraticCurveTo)(canvas_t* s, xypos_t cpx, xypos_t cpy, xypos_t x, xypos_t y); // �������α��������ߡ��Ե�ǰContext�������Ϊ��㣬(cpx,cpy)��Ϊ���Ƶ㣬(x, y)��Ϊ�յ�Ķ�����������·����
#define canvas_bezierCurveTo(s, cpx1, cpy1, cpx2, cpy2, x, y) s->bezierCurveTo(s, cpx1, cpy1, cpx2, cpy2, x, y)
  int (*bezierCurveTo)(canvas_t* s, xypos_t cpx1, xypos_t cpy1, xypos_t cpx2, xypos_t cpy2, xypos_t x, xypos_t y); // �������η����������ߡ��Ե�ǰContext�������Ϊ��㣬(cpx1,cpy1)���(cpx2, cpy2)��Ϊ�������Ƶ㣬(x, y)��Ϊ�յ�ı���������·���� ·�������ɺ���Ҫ����Context�����fill()��stroke()���������·���ͻ���·�����������ߵ���clip()����������Canvas������������������ԭ�����£�
#define canvas_isPointInPath(s, x, y) s->isPointInPath(s, x, y)
  int (*isPointInPath)(canvas_t* s, xypos_t x, xypos_t y); // ���ָ���ĵ�λ�ڵ�ǰ·���У��򷵻� true�����򷵻� false
  // ת��
#define canvas_scale(s, scalewidth, scaleheight) s->scale(s, scalewidth, scaleheight)
  int (*scale)(canvas_t* s, float scalewidth, float scaleheight); // ���ŵ�ǰ��ͼ��������С
#define canvas_rotate(s, angle) s->rotate(s, angle)
  int (*rotate)(canvas_t* s, float angle); // ��ת��ǰ��ͼ
#define canvas_translate(s, x, y) s->translate(s, x, y)
  int (*translate)(canvas_t* s, float x, float y); // ����ӳ�仭���ϵ� (0,0) λ��
  // a c e
  // b d f
  // 0 0 1
  // a ˮƽ��ת��ͼ
  // b ˮƽ��б��ͼ
  // c ��ֱ��б��ͼ
  // d ��ֱ���Ż�ͼ
  // e ˮƽ�ƶ���ͼ
  // f ��ֱ�ƶ���ͼ
#define canvas_transform(s, a, b, c, d, e, f) s->transform(s, a, b, c, d, e, f)
  int (*transform)(canvas_t* s, float a, float b, float c, float d, float e, float f); // �滻��ͼ�ĵ�ǰת������
#define canvas_setTransform(s, a, b, c, d, e, f) s->setTransform(s, a, b, c, d, e, f)
  int (*setTransform)(canvas_t* s, float a, float b, float c, float d, float e, float f); // ����ǰת������Ϊ��λ����Ȼ������ transform()
  // �ı�
  // [style][variant][weight][size][family]
  // font-style �涨������ʽ�����ܵ�?���normal|italic|oblique
  // font-variant �涨������塣���ܵ�ֵ�� normal|small-caps
  // font-weight �涨����Ĵ�ϸ�����ܵ�ֵ�� normal|bold|bolder|lighter|100|200|300|400|500|600|700|800|900
  // font-size / line-height �涨�ֺź��иߣ������ؼơ�
  // font-family �涨����ϵ�С�
#define canvas_font(s, str) s->font(s, str)
  int (*font)(canvas_t* s, const char* str); // ���û򷵻��ı����ݵĵ�ǰ��������
  //start Ĭ�ϡ��ı���ָ����λ�ÿ�ʼ��
  //end �ı���ָ����λ�ý�����
  //center �ı������ı�������ָ����λ�á�
  //left �ı�����롣
  //right �ı��Ҷ��롣
#define canvas_textAlign(s, str) s->textAlign(s, str)
  int (*textAlign)(canvas_t* s, const char* str); // ���û򷵻��ı����ݵĵ�ǰ���뷽ʽ "center|end|left|right|start";
  //alphabetic Ĭ�ϡ��ı���������ͨ����ĸ���ߡ�
  //top �ı������� em ����Ķ��ˡ���
  //hanging �ı����������һ��ߡ�
  //middle �ı������� em ��������С�
  //ideographic �ı������Ǳ�����ߡ�
  //bottom �ı������� em ����ĵ׶ˡ�
#define canvas_textBaseline(s, str) s->textBaseline(s, str)
  int (*textBaseline)(canvas_t* s, const char* str); // ���û򷵻��ڻ����ı�ʱʹ�õĵ�ǰ�ı����� "alphabetic|top|hanging|middle|ideographic|bottom";
#define canvas_fillText(s, text, textlen, x, y, maxWidth) s->fillText(s, text, textlen, x, y, maxWidth)
  int (*fillText)(canvas_t* s, const char* text, int textlen, xypos_t x, xypos_t y, xypos_t maxWidth); // �ڻ����ϻ��ơ������ġ��ı�
#define canvas_strokeText(s, text, textlen, x, y, maxWidth) s->strokeText(s, text, textlen, x, y, maxWidth)
  int (*strokeText)(canvas_t* s, const char* text, int textlen, xypos_t x, xypos_t y, xypos_t maxWidth); // �ڻ����ϻ����ı�������䣩
#define canvas_measureText(s, text, textlen) s->measureText(s, text, textlen)
  int (*measureText)(canvas_t* s, const char* text, int textlen); // ���ذ���ָ���ı���ȵĶ���
  // ͼ�����
#define canvas_drawImage(s, img, f, rcimg, x, y, sz) s->drawImage(s, img, f, rcimg, x, y, sz)
  int (*drawImage)(canvas_t* s, const img_t* img, int frame, const IRECT* rcimg, xypos_t x, xypos_t y, const ISIZE* sz); // �򻭲��ϻ���ͼ�񡢻�������Ƶ
  // ���ز���
  int (*width)(canvas_t* s); // ���� ImageData ����Ŀ��
  int (*height)(canvas_t* s); // ���� ImageData ����ĸ߶�
  int (*data)(canvas_t* s); // ����һ�����������ָ���� ImageData �����ͼ������
  int (*createImageData)(canvas_t* s); // �����µġ��հ׵� ImageData ����
  int (*getImageData)(canvas_t* s); // ���� ImageData ���󣬸ö���Ϊ������ָ���ľ��θ�����������
  int (*putImageData)(canvas_t* s); // ��ͼ�����ݣ���ָ���� ImageData ���󣩷Żػ�����
  // �ϳ�
  int (*globalAlpha)(canvas_t* s); // ���û򷵻ػ�ͼ�ĵ�ǰ alpha ��͸��ֵ
  int (*globalCompositeOperation)(canvas_t* s); // ���û򷵻���ͼ����λ��Ƶ����е�ͼ����
  // ����
  int (*save)(canvas_t* s); // ���浱ǰ������״̬
  int (*restore)(canvas_t* s); // ����֮ǰ�������·��״̬������
  int (*createEvent)(canvas_t* s); //
  int (*getContext)(canvas_t* s); //
  int (*toDataURL)(canvas_t* s); //
};
#define CANVAS_FUN(TT) \
  TT(release) \
  TT(fillStyle) \
  TT(strokeStyle) \
  TT(shadowColor) \
  TT(shadowBlur) \
  TT(shadowOffsetX) \
  TT(shadowOffsetY) \
  TT(lineWidth) \
  TT(lineCap) \
  TT(lineJoin) \
  TT(miterLimit) \
  TT(rect) \
  TT(fillRect) \
  TT(strokeRect) \
  TT(clearRect) \
  TT(beginPath) \
  TT(closePath) \
  TT(clip) \
  TT(fill) \
  TT(stroke) \
  TT(moveTo) \
  TT(lineTo) \
  TT(arcTo) \
  TT(arc) \
  TT(quadraticCurveTo) \
  TT(bezierCurveTo) \
  TT(isPointInPath) \
  TT(scale) \
  TT(rotate) \
  TT(translate) \
  TT(transform) \
  TT(setTransform) \
  TT(font) \
  TT(textAlign) \
  TT(textBaseline) \
  TT(fillText) \
  TT(strokeText) \
  TT(measureText) \
  TT(drawImage) \
  TT(width) \
  TT(height) \
  TT(data) \
  TT(createImageData) \
  TT(getImageData) \
  TT(putImageData) \
  TT(globalAlpha) \
  TT(globalCompositeOperation) \
  TT(save) \
  TT(restore) \
  TT(createEvent) \
  TT(getContext) \
  TT(toDataURL)
#endif // _CANVAS_INL_

