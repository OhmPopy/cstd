#define GDIPARAM(s) (0)
// ��ɫ����ʽ
int nul_fillStyle(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  return 0;
}
int nul_strokeStyle(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  return 0;
}
// ��Ӱ
int nul_shadowColor(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int nul_shadowBlur(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int nul_shadowOffsetX(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int nul_shadowOffsetY(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
// ������ʽ
int nul_lineWidth(canvas_t* s, float px)
{
  GDIPARAM(s);
  return 0;
}
int nul_lineCap(canvas_t* s, int type)
{
  GDIPARAM(s);
  return 0;
}
int nul_lineJoin(canvas_t* s, int type)
{
  GDIPARAM(s);
  return 0;
}
int nul_miterLimit(canvas_t* s, float x)
{
  GDIPARAM(s);
  return 0;
}
// ����
int nul_rect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  return 0;
}
int nul_fillRect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  return 0;
}
int nul_strokeRect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  return 0;
}
int nul_clearRect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  return 0;
}
// ·��
int nul_beginPath(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int nul_closePath(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int nul_clip(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int nul_fill(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int nul_stroke(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int nul_moveTo(canvas_t* s, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  return 0;
}
int nul_lineTo(canvas_t* s, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  return 0;
}
int nul_arcTo(canvas_t* s, xypos_t x1, xypos_t y1, xypos_t x2, xypos_t y2, xypos_t radius)
{
  GDIPARAM(s);
  return 0;
}
int nul_arc(canvas_t* s, xypos_t x, xypos_t y, xypos_t radius, xypos_t startAngle, xypos_t endAngle, BOOL anticlockwise)
{
  GDIPARAM(s);
  return 0;
}
int nul_quadraticCurveTo(canvas_t* s, xypos_t cpx, xypos_t cpy, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  return 0;
}
int nul_bezierCurveTo(canvas_t* s, xypos_t cpx1, xypos_t cpy1, xypos_t cpx2, xypos_t cpy2, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  return 0;
}
int nul_isPointInPath(canvas_t* s, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  return 0; // ���ָ���ĵ�λ�ڵ�ǰ·���У��򷵻� true�����򷵻� false
}
// ת��
int nul_scale(canvas_t* s, float scalewidth, float scaleheight)
{
  GDIPARAM(s);
  return 0;
}
int nul_rotate(canvas_t* s, float angle)
{
  GDIPARAM(s);
  return 0;
}
int nul_translate(canvas_t* s, float x, float y)
{
  GDIPARAM(s);
  return 0;
}
int nul_transform(canvas_t* s, float a, float b, float c, float d, float e, float f)
{
  GDIPARAM(s);
  return 0;
}
int nul_setTransform(canvas_t* s, float a, float b, float c, float d, float e, float f)
{
  GDIPARAM(s);
  return 0;
}
// �ı�
int nul_font(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  return 0;
}
int nul_textAlign(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  return 0;
}
int nul_textBaseline(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  return 0;
}
int nul_fillText(canvas_t* s, const char* text, int textlen, xypos_t x, xypos_t y, xypos_t maxWidth)
{
  GDIPARAM(s);
  return 0;
}
int nul_strokeText(canvas_t* s, const char* text, int textlen, xypos_t x, xypos_t y, xypos_t maxWidth)
{
  GDIPARAM(s);
  return 0;
}
int nul_measureText(canvas_t* s, const char* text, int textlen)
{
  GDIPARAM(s);
  return 0;
}
// ͼ�����
int nul_drawImage(canvas_t* s, const img_t* img, int frame, const IRECT* rcimg, xypos_t x, xypos_t y, const ISIZE* sz)
{
  GDIPARAM(s);
  return 0;
}
// ���ز���
int nul_width(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int nul_height(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int nul_data(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int nul_createImageData(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int nul_getImageData(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int nul_putImageData(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
// �ϳ�
int nul_globalAlpha(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int nul_globalCompositeOperation(canvas_t* s)
{
  GDIPARAM(s);
  return 0; // ���û򷵻���ͼ����λ��Ƶ����е�ͼ����
}
// ����
int nul_save(canvas_t* s)
{
  GDIPARAM(s);
  return 0; // ���浱ǰ������״̬
}
int nul_restore(canvas_t* s)
{
  GDIPARAM(s);
  return 0; // ����֮ǰ�������·��״̬������
}
int nul_createEvent(canvas_t* s)
{
  GDIPARAM(s);
  return 0; //
}
int nul_getContext(canvas_t* s)
{
  GDIPARAM(s);
  return 0; //
}
int nul_toDataURL(canvas_t* s)
{
  GDIPARAM(s);
  return 0; //
}
int nul_release(canvas_t* s)
{
  GDIPARAM(s);
  FREE(s->x);
  s->x = NULL;
  return 0;
}
#define NUL_FUNSET(name) s->name = nul_##name;
int canvas_nul_set(canvas_t* s, HDC hDC)
{
  GDIPARAM(s);
  CANVAS_FUN(NUL_FUNSET);
  return 0;
}
#undef NUL_FUNSET
#undef GDIPARAM

