// �Լ�����дC���Ը�ʽ���������
// ���ࣺ C/C++ 2011-12-22 13:32 3331���Ķ� ����(6) �ղ� �ٱ�
// ����c������bufferwindows���ݷ���
//
// printfϵ�к���������fprintf��sprintf�����ȣ��书���ǽ�C���Ե����л����������Ͱ��û�Ҫ����и�ʽ�������
//
// printf��������������ѧϰC���Ե��˽Ӵ����ĵ�һ����������C���Ա�׼��ʹ��Ƶ����ߵĺ�����
//
// printf������C���Ա�׼�������������Ŀɱ��������������printf�������������������C���Ե�˵��һ��Ҳ�����֣���ˣ�����˵��C���Ա�׼�����е���߱�־�Եĺ�����
//
// printfϵ�к�������DOS�����£���һϵ���������������PC�������õ�����������豸������printfϵ�к���Ҳ��C��������ӵĺ�����
//
// ��Ȼ������DOSʱ���Ľ���������printfϵ�к��������ü����ˣ�����C���Ա���Ҳ��ѹ��������С��Ӧ������
//
// ����д��sprintfA������Ҳ��Ӧһ��С��Ҫ��д�ļ�������֮һ�����������췢���ġ��Լ�����дC���Ը�����ת���ַ����������е�FloatToStr������������ѧϰ�õġ�֮����ȡ��ΪsprintfA������������ϵͳ�����sprintf������ͬʱҲ��Ϊ��Windows�£�A��ʾ���Ǵ�ͳ��ANSI��������Ϊ��Windows�£�printfϵ�к���Ҳ����ʱ������ˣ���wprintf�Ⱦ����ڿ��ַ������µ������������������sprintfA������ʹ����Windows�Ŀ��ַ�ת����������˸ú���ֻ������Windows������
//
// ����sprintfA��������Ƚϳ�������Ϊ��ƪ���·��������Լ�����дC���Ը�����ת���ַ���������һ���еĴ���ҲӦ����һƪ��
//
// һ�����ݶ��壺
typedef struct {
  INT type; // ���ݳ�������
  INT width; // ������С���
  INT precision; // ���ݾ���
  BOOL left; // �Ƿ����
  BOOL zero; // �Ƿ�ǰ����
  INT decimals; // ������: 1ǿ��С��λ; 16����: -1: 0x, 1: 0X
  INT negative; // ���ţ�-1: '-'; 1: '+'
  LPSTR param; // ����ָ��
} FormatRec;
typedef __int64 LLONG, *PLLONG;
typedef unsigned __int64 ULLONG, *PULLONG;
#define TYPE_CHAR 0
#define TYPE_SHORT 1
#define TYPE_GENERAL 2
#define TYPE_LONG 3
#define TYPE_LLONG 4
#define PTR_SIZE sizeof(VOID*)
#define TypeSize(size) (((size + PTR_SIZE - 1) / PTR_SIZE) * PTR_SIZE)
#define TS_PTR PTR_SIZE
#define TS_CHAR TypeSize(sizeof(CHAR))
#define TS_WCHAR TypeSize(sizeof(WCHAR))
#define TS_SHORT TypeSize(sizeof(SHORT))
#define TS_INT TypeSize(sizeof(INT))
#define TS_LONG TypeSize(sizeof(LONG))
#define TS_LLONG TypeSize(sizeof(LLONG))
#define TS_FLOAT TypeSize(sizeof(FLOAT))
#define TS_DOUBLE TypeSize(sizeof(double))
#define TS_EXTENDED TypeSize(sizeof(EXTENDED))
#define CHAR_SPACE ' '
#define CHAR_ZERO '0'
#define CHAR_POS '+'
#define CHAR_NEG '-'
#define HEX_PREFIX_U "0X"
#define HEX_PREFIX_L "0x"
#define MAX_DIGITS_SIZE 40
//
// FormatRec��һ�����ݸ�ʽ���ṹ����������sprintfA��ʽ��������������Ļ�����Ϣ��
//
// TYPE_XXXX���������ͱ�ǣ���Ӧ��FormatRec.type�ֶΡ�
//
// TS_XXXX�Ǹ�������������sprintfA�ɱ��������ʱ��ռ��ջ�ֽڳ��ȡ���ָ�����ͺ�INT���ͳ���ֱ����sizeof�ؼ���ȷ��ջ�ֽڳ����⣬��������������ռջ��������TypeSize����ϼ���ȡ�ã�������ʹ����Щ������ռջ�ֽڳ����ڸ��ֻ����¶�����ȷ�ģ������ַ��ͳ���Ϊ1�ֽڣ�TypeSizesizeof(CHAR))����32λ���뻷��ʱ����4����64λ���뻷��ʱ�����8��
//
// ���ڴ��������Ϳɱ�����ĺ�����˵��ʵ���������͵�ջ�ֽڳ�����ȷ�����ȫȡ���ڳ���Ա��������sprintfA�ĸ�ʽ�����ж�����%Ld��Ӧ���Ǹ�64λ�������ͣ����ڶ�Ӧ�Ŀɱ��������ȴ����һ��int���ͣ���32λ�����������£��ʹ���2������һ���������Ͳ���ȷ������ջ�ֽڳ��Ȳ�ƥ�䣬64λ��������Ϊ8�ֽڣ���INT�ĳ���ȴֻ��4�ֽڣ�����������������Լ������������ݶ�����ִ������ʾ������������п�����ɳ����������Ҳ��һЩC���Գ�ѧ����ʹ��printfϵ�к���ʱ�����׷��Ĵ������ǻ�����һ�㺯������ɱ�����������õ����� ����һ���C�������βε����������ǹ̶��ģ��ڵ���ʱ�����ʵ�����β��������Ͳ�ƥ�䣬��������������������󡢾������ת���ȴ��������ڲ�ͬ���ȵ���ͬ�������ͣ��������󶼻��Զ�������չ��ضϣ������ô��ɱ��������ʱ�����ں���ԭ�͵��β�˵������Ϊ��...������������û����int��չΪ_int64�ˡ�
//
// ���⣬�����йظ��������ֵ����ݶ����ڡ��Լ�����дC���Ը�����ת���ַ�����������
//
// �����������塣
// ��ȡ�ַ����е����֡��������ַ���������ָ�롣�����ַ��������һ������λ��
static LPSTR GetControlNum(LPCSTR s, INT* value)
{
  register LPCSTR p = s;
  register INT v;
  for (v = 0; *p >= '0' && *p <= '9'; p ++) {
    v = v * 10 + (*p - '0');
  }
  *value = v;
  return (LPSTR)(p - 1);
}
// sprintfA���������岿�־���һ���򵥵Ľ�������ͨ��һ����ѭ�������ַ�������format���ַ��������½�����
//
// 1������������ݸ�ʽǰ׺�ַ�'%'��ֱ�ӿ��������������buffer��
//
// 2�����'%'�����һ��'%'�ַ������ʾҪ����������'%'��
//
// 3��������'%'����ģ�Ӧ�������ݸ�ʽǰ���ַ�������4��ǰ���ַ���
//
// 1��'0'��ǰ�����־��������ݱ���ʽ����ĳ���С�ڹ涨�ĸ�ʽ����ȣ����ڱ���ʽ���������ǰ��0��
//
// 2��'-'��������ǡ�
//
// 3��'+'���������������ǡ�������������ʽ���ʱ���������ʡ���˵ģ�'+'���ʾҪ���������ţ�
//
// 4��'#'���Ը�����������ǿ��С���㣨'.'�������ǡ������������û��С�����֣�������������С��λ���ţ���������ʮ���������������ʮ������ǰ׺��0x����0X�������ǡ�
//
// ǰ���ַ����Ǳ���ģ�Ҳ���ж��ǰ����ͬʱ������'%'���棬��'0'�����������һ��������˳������⡣
//
// 4���������������Ⱥ;��ȡ������ָ�������ʱ����ﵽ���ֽ����������ʽ��������ݳ���С�ڿ�ȣ�Ӧ�ÿո�����㲹�룻������������Ҫ���ʽ���ĳ��ȣ����������Ͳ�ͬ�����������縡������ָС�����ֵĳ��ȣ���������������ָȫ�����ݸ�ʽ�����ȣ����ھ��ȵ������Ǳ������ǽضϣ�С�ھ����Ǻ��Ի��ǲ��루���ո񣩣������漰������������ʱ��˵����
//
// ��Ⱥ;���һ����'.'Ϊ�ָ���������ǿ�ȣ��ұ��Ǿ��ȣ����ֻ�п����'.'�ɺ��ԡ���Ⱥ;��ȿ��ù̶����ֱ�ʾ���硰10.6����Ҳ���ÿɱ���ʽ��*.*����ʾ���ɱ���ʽ�Ŀ�Ⱥ;��ȱ�����sprintf�Ŀɱ�������������Ӧ������ʵ�Ρ�
//
// ��Ⱥ;��Ȳ���Ҳ���Ǳ���ġ�
//
// 5�������������;����ַ�����C�����У���ͬ���͵Ļ������ݿ����в�ͬ�ľ��ȣ��������г���֮�֣��������о���֮�֣����ַ���ANSI��UNICODE֮�ֵȵȡ���sprintfA�У��ǿ��������;����ַ���ȡ�õġ��ַ�'l'��'h'�ֱ��ʾ�����ݺͶ����ݣ���16λ�����������£�һ��'l'��'h'�͹��ˣ���32λ�����ϱ������У��������ݾ��ȵ���ߣ����뿿������;����ַ����ܱ�ʾ������Ϊ�ˣ�Ҳ�����ַ�'L'��'H'�ֱ��ʾ�������͵���󾫶Ⱥ���С���ȡ�sprintfA���������;��ȷ����нϸߵ��ݴ���������������������;����ַ���
//
// ���;����ַ�Ҳ���Ǳ���ģ�ȱʡ����£���һ�����;��ȴ���
//
// 6���������������ַ������������ַ���������2����һ��ȷ����Ҫ������������ͣ���x����������e�Ǹ������ȣ�����ȷ��Ҫ�������ʽ��x����Сдʮ�����������������e������ָ����ʽ�����������
//
// ���������ַ��Ǳ���ġ����������ַ�������ϣ�������Ϣд��FormatRec�ṹ�����ž��Ǿ���ĸ������ݵĸ�ʽ�������ˣ�����뽫�ں��������
//
// 7�������������'%'�ַ��󣬳������������ַ�������ַ����������������ַ�����˳����󣬾���Ҫ���д�����printfϵ�к����Ĵ������ڲ�ͬ�ı������еĴ���ʽ�ǲ�һ���ģ���Ҫ��2�ִ���ʽ��һ�Ǻ��Ա������ݷ�����formatָ���˻ص�'%'֮�󣬼���ѭ����'%'����ַ���һ���ַ����������ǲ�����������ֱ�ӽ�'%'��������ַ������buffer���˳�����������sprintfA���������˺�һ�ִ���ʽ��ǰһ�ִ���ʽ�ں���������Ҳ���ҵ������Ǳ�ע���˵���䡣
//
// ���û�д�����ص�1����������һ���ݷ�����
//
// ������ʽ���ַ����ַ�����
// ���ַ���ת��ANSI�ַ�����������ANSI�ַ��������ַ�����ת���ַ���(0��ת��)��
// ����ʵ��ת���ַ�����
static INT WStrToStr(LPSTR dst, LPCWSTR src, INT count)
{
  return WideCharToMultiByte(CP_THREAD_ACP, 0, src, -1,
      dst, count > 0 ? count + 1 : 0, NULL, NULL) - 1;
}
// ��ʽ���ַ�������������������ʽ��¼�����ػ�����βƫ��
static LPSTR FormatCharA(LPSTR buffer, FormatRec* rec)
{
  INT len, spaces;
  LPSTR p = buffer;
  if (rec->type == TYPE_LONG) {
    len = WStrToStr(NULL, (LPCWSTR)rec->param, 0);
    if (len == 0) {
      len = sizeof(CHAR);
    }
  }
  else {
    len = sizeof(CHAR);
  }
  spaces = rec->width - len;
  if (rec->left == FALSE && spaces > 0) {
    memset(p, CHAR_SPACE, spaces);
    p += spaces;
  }
  if (rec->type == TYPE_LONG) {
    WStrToStr(p, (LPCWSTR)rec->param, len);
    p += len;
  }
  else {
    *p ++ = *(LPCSTR)rec->param;
  }
  if (rec->left == TRUE && spaces > 0) {
    memset(p, CHAR_SPACE, spaces);
    p += spaces;
  }
  rec->param += rec->type == TYPE_LONG ? TS_WCHAR : TS_CHAR;
  return p;
}
// ��ʽ���ַ���������������������ʽ��¼�����ػ�����βƫ��
static LPSTR FormatStrA(LPSTR buffer, FormatRec* rec)
{
  INT len, spaces;
  LPSTR p = buffer;
  if (rec->type == TYPE_LONG) {
    len = WStrToStr(NULL, *(LPCWSTR*)rec->param, 0);
  }
  else {
    len = lstrlenA(*(LPCSTR*)rec->param);
  }
  if (rec->precision >= 0 && len > rec->precision) {
    len = rec->precision;
  }
  spaces = rec->width - len;
  if (rec->left == FALSE && spaces > 0) {
    memset(p, CHAR_SPACE, spaces);
    p += spaces;
  }
  if (rec->type == TYPE_LONG) {
    WStrToStr(p, *(LPCWSTR*)rec->param, len);
  }
  else {
    memcpy(p, *(LPCSTR*)rec->param, len);
  }
  p += len;
  if (rec->left == TRUE && spaces > 0) {
    memset(p, CHAR_SPACE, spaces);
    p += spaces;
  }
  rec->param += TS_PTR;
  return p;
}
//
// ������漰���ַ�����ʽ���ַ����ַ����Ǻܼ򵥵ġ�
//
// �����ַ����ַ�����"%lc"��"%ls"��ʾ���ַ��Ϳ��ַ������������;���ȫ����ΪĬ��ֵ����ANSI�ַ���ANSI�ַ�����
//
// ���ַ���ת������WStrToStr��������ɵģ���WStrToStr���ǵ��õ�Windows API����WideCharToMultiByte��
//
// �ڸ�ʽ���ַ�0ʱ��C���Ե�printf��sprintf������ͬ��ǰ�����ÿո�����ġ����磺printf("%s%c456", "123", 0)����ʾ�����ǡ�123 456"����sprintf(s, "%s%c456", "123", 0)��s="123"����ˣ�sprintfAҲ����s="123"��
//
// �ġ���ʽ����������
// ��ʽ�����ִ�������������������ʽ��¼�����ִ������ִ����ȡ����ػ�����βƫ��
static LPSTR FormatDigitsA(LPSTR buffer, FormatRec* rec, LPCSTR digits, INT len)
{
  LPSTR p = buffer;
  INT spaces;
  if (rec->precision >= 0) {
    rec->zero = FALSE;
  }
  rec->precision -= len;
  if (rec->precision < 0) {
    rec->precision = 0;
  }
  spaces = rec->width - len - rec->precision;
  if (rec->negative) {
    spaces --;
    if (rec->left || rec->zero) {
      *p ++ = (rec->negative == -1 ? CHAR_NEG : CHAR_POS);
    }
  }
  if (rec->left == FALSE) {
    if (spaces > 0) {
      memset(p, rec->zero ? CHAR_ZERO : CHAR_SPACE, spaces);
      p += spaces;
    }
    if (rec->negative && !rec->zero && !rec->decimals) {
      *p ++ = (rec->negative == -1 ? CHAR_NEG : CHAR_POS);
    }
  }
  if (rec->precision != 0) {
    memset(p, CHAR_ZERO, rec->precision);
    p += rec->precision;
  }
  memcpy(p, digits, len);
  p += len;
  if (rec->left == TRUE && spaces > 0) {
    memset(p, CHAR_SPACE, spaces);
    p += spaces;
  }
  return p;
}
// ������ת��Ϊ���ִ������������ִ������������Ƿ��޷�������
static INT IntToDigits(LPSTR digits, LONG src, BOOL* isUnsigned)
{
  ULONG v;
  LPSTR p = digits + MAX_DIGITS_SIZE;
  if (*isUnsigned == FALSE && src < 0) {
    src = -src;
  }
  else {
    *isUnsigned = TRUE;
  }
  v = (ULONG)src;
  do {
    *(-- p) = (CHAR)(v % 10 + '0');
    v /= 10;
  }
  while (v);
  return (INT)(MAX_DIGITS_SIZE - (p - digits));
}
static INT LLongToDigits(LPSTR digits, LLONG src, BOOL* isUnsigned)
{
  ULLONG v;
  LPSTR p = digits + MAX_DIGITS_SIZE;
  if (*isUnsigned == FALSE && src < 0) {
    src = -src;
  }
  else {
    *isUnsigned = TRUE;
  }
  v = (ULLONG)src;
  do {
    *(-- p) = (CHAR)(v % 10 + '0');
    v /= 10;
  }
  while (v);
  return (INT)(MAX_DIGITS_SIZE - (p - digits));
}
static INT numSizes[] = {sizeof(CHAR), sizeof(SHORT), sizeof(INT), sizeof(LONG), sizeof(LLONG)};
// ��ʽ��������������������������ʽ��¼���Ƿ��޷������������ػ�����βƫ��
static LPSTR FormatIntA(LPSTR buffer, FormatRec* rec, BOOL isUnsigned)
{
  ULONG value;
  INT len;
  CHAR digits[MAX_DIGITS_SIZE];
  if (isUnsigned) {
    rec->negative = 0;
  }
  if (numSizes[rec->type] <= TS_PTR) {
    value = *(PULONG)rec->param;
    if (isUnsigned) {
      value &= ((ULONG)(-1) >> ((TS_PTR - numSizes[rec->type]) << 3));
    }
    len = IntToDigits(digits, value, &isUnsigned);
  }
  else {
    len = LLongToDigits(digits, *(PLLONG)rec->param, &isUnsigned);
  }
  if (!isUnsigned) {
    rec->negative = -1;
  }
  rec->param += TypeSize(numSizes[rec->type]);
  rec->decimals = 0;
  return FormatDigitsA(buffer, rec, &digits[MAX_DIGITS_SIZE - len], len);
}
//
// ��C�Ļ��������У��������ı�ﷶΧ�����ʱ������ġ�16λ������ʱ��int��2�ֽڣ�longΪ4�ֽڣ���32�������£�int��long�������4�ֽڣ�������˸�8�ֽڵ�_int64���ͣ�64λ�������£�int��Ȼ��4�ֽڣ�long����8�ֽڣ��Ƿ���и�16�ֽڵ�_int128����û�ù�64λ����������֪���������ж�����һ��LLONG���ͣ���д��2��������ת���ַ�������������С�ڻ����ָ�볤�ȷ�Χ����������ʹ��IntToDigits����������ʹ��LLongToDigits�������ӱ��濴����2����������������?�ͬ�⣬�����һ���ģ��������ǰ�ߵ��ٶ�Ҫ�졣�����д���õĺ��������黹��ʹ�ò��������ת������Ϊ���ֻ��һ���������Ϳɵĵ��̺����������߼��������?��������
//
// ��ЩC���Ը�ʽ�����������������ת��ʱ���Ǻ���hh������H�����ȵģ�Ҳ����˵������ת������С����Ϊsizeof(SHORT)����sprintfA������������С����Ϊsizeof(CHAR)������"%hhu", -123��ǰ�������65413��������ȴ��133������Ѵ�����numSizes����ĵ�һ��Ԫ�ظ�Ϊsizeof(SHORT)��sprintfAҲ�����hh������H�����ȡ�
//
// �塢��������ʽ��Ϊʮ�����ƺͰ˽������ִ���
static CHAR hexDigitsU[] = "0123456789ABCDEF";
static CHAR hexDigitsL[] = "0123456789abcdef";
// ������ת��Ϊʮ�����ƴ���������ʮ�����ƴ������������ֽڳ��ȣ�ת�����ȣ��Ƿ��д
static INT NumberToHexA(LPSTR hex, LPCVOID lpNumber, INT bytes, INT precision, BOOL upper)
{
  LPSTR ph = hex;
  LPBYTE pn = (LPBYTE)lpNumber;
  LPSTR hexDigits;
  INT len;
  for (bytes --; bytes > 0 && pn[bytes] == 0; bytes --);
  pn += bytes;
  bytes ++;
  len = bytes * 2;
  if ((*pn & 0xf0) == 0) {
    len --;
  }
  if (hex == NULL) {
    return precision > len ? precision : len;
  }
  for (precision -= len; precision > 0; *ph ++ = '0', precision --);
  hexDigits = upper ? hexDigitsU : hexDigitsL;
  if ((*pn & 0xf0) == 0) {
    *ph ++ = hexDigits[*pn -- & 0x0f];
    bytes --;
  }
  for (; bytes > 0; bytes --, pn --) {
    *ph ++ = hexDigits[*pn >> 4];
    *ph ++ = hexDigits[*pn & 0x0f];
  }
  return (INT)(ph - hex);
}
// ��ʮ�����Ƹ�ʽ��������������������������ʽ��¼�������ַ�(x or X)
static LPSTR FormatHexA(LPSTR buffer, FormatRec* rec, CHAR hexChar)
{
  LPSTR p = buffer;
  INT spaces, len, pix;
  BOOL upper = hexChar == 'X';
  if (rec->precision >= 0) {
    rec->zero = FALSE;
  }
  pix = rec->decimals ? 2 : 0;
  rec->precision -= pix;
  len = NumberToHexA(NULL, rec->param, numSizes[rec->type], rec->precision, upper);
  spaces = rec->width - len - pix;
  if (rec->decimals && (rec->left || rec->zero)) {
    memcpy(p, rec->decimals > 0 ? HEX_PREFIX_U : HEX_PREFIX_L, 2);
    p += 2;
  }
  if (rec->left == FALSE) {
    if (spaces > 0) {
      memset(p, rec->zero ? CHAR_ZERO : CHAR_SPACE, spaces);
      p += spaces;
    }
    if (rec->decimals && !rec->zero) {
      memcpy(p, rec->decimals > 0 ? HEX_PREFIX_U : HEX_PREFIX_L, 2);
      p += 2;
    }
  }
  p += NumberToHexA(p, rec->param, numSizes[rec->type], rec->precision, upper);
  if (rec->left == TRUE && spaces > 0) {
    memset(p, CHAR_SPACE, spaces);
    p += spaces;
  }
  rec->param += TypeSize(numSizes[rec->type]);
  return p;
}
// ������ת��Ϊ�˽��ƴ����������˽��ƴ������������ֽڳ���
static INT NumberToOtcalA(LPSTR otcal, LPCVOID lpNumber, INT bytes)
{
  LPSTR p = otcal + MAX_DIGITS_SIZE;
  ULLONG v = 0;
  memcpy(&v, lpNumber, bytes);
  do {
    *(-- p) = (CHAR)((v & 7) + '0');
    v >>= 3;
  }
  while (v);
  return (INT)(MAX_DIGITS_SIZE - (p - otcal));
}
// ���˽��Ƹ�ʽ��������������������������ʽ��¼
static LPSTR FormatOctalA(LPSTR buffer, FormatRec* rec)
{
  CHAR otcal[MAX_DIGITS_SIZE];
  INT len = NumberToOtcalA(otcal, rec->param, numSizes[rec->type]);
  rec->param += TypeSize(numSizes[rec->type]);
  rec->negative = 0;
  return FormatDigitsA(buffer, rec, &otcal[MAX_DIGITS_SIZE - len], len);
}
//
// ������ת��Ϊʮ�����ƻ��߰˽������ִ������˽��Ʋ�ͬ��������ǰ��������ת��Ϊ10��������һ���ġ�
//
// ������ʽ��ָ�롣
// ��ʮ�����Ƹ�ʽ��ָ�롣����������������ʽ��¼
static LPSTR FormatPointerA(LPSTR buffer, FormatRec* rec)
{
  INT prec = PTR_SIZE << 1;
  CHAR tmp[PTR_SIZE * 2];
  NumberToHexA(tmp, rec->param, TS_PTR, prec, TRUE);
  rec->precision = -1; // ���Ծ���
  return FormatDigitsA(buffer, rec, tmp, prec);
}
//
// ��Ϊָ���ַͬ��Ҳ�Ǹ�������������ָ��ĸ�ʽ����������ת��Ϊʮ���������ִ���һ���ģ�ֻ���������ǹ̶��ģ�32λ��������Ϊ8λʮ����������64λ����������Ϊ16λʮ����������
//
// �ߡ���ȡ��������ǰλ���ֽ�����
// ��ȡ��������ǰλ���ֽ��������������������������׵�ַ����ʽ��¼
static VOID GetPosSizeA(LPSTR buffer, LPSTR buffer0, FormatRec* rec)
{
  LLONG size = buffer - buffer0;
  memcpy((LPVOID) * (PLONG*)rec->param, &size, numSizes[rec->type]);
  rec->param += TS_PTR;
}
//
// ���Ǹ�ʽ����?������������������������ǰ�ĳ��������ֵ����������������ǰ������������ǰλ�õĳ��������ĳ���������������������ָ����ʽ�ġ?
//
// ͬ������ת��Ϊ���ִ�һ����sprintfAȷ�ϵ���С���ݾ���Ϊsizeof(CHAR)��Ҳ���Ըı�Ϊsizeof(SHORT)��
//
// �ˡ���ʽ�����������йظ����������ݶ���͵ײ������ת�����������Լ�����дC���Ը���?�ת���ַ���������һ�ģ��?
// ת����������Ϣ����������¼fRec����������ʽ��¼����ʽ��ʽ��ǣ���������¼
static void GetFloatRec(FormatRec* rec, INT flag, FloatRec* fRec)
{
  EXTENDED value;
  if (rec->precision < 0) {
    rec->precision = F_DEFDECIMALS;
  }
  else if (rec->precision > F_MAXDECIMALS) {
    rec->precision = F_MAXDECIMALS;
  }
  if (rec->type == TYPE_LLONG) {
    value = *(PEXTENDED)rec->param;
    rec->param += TS_EXTENDED;
  }
  else {
    value = *(double*)rec->param;
    rec->param += TS_DOUBLE;
  }
  switch (flag) {
  case 0: // %f
    FloatResolve(&value, F_MAXPRECISION, rec->precision, fRec);
    break;
  case 1: // %e or %E
    FloatResolve(&value, rec->precision + 1, 9999, fRec);
    break;
  case 2: // %g or %G
    FloatResolve(&value, rec->precision, 9999, fRec);
  }
  if (fRec->negative) {
    rec->negative = -1;
  }
}
// ��ʽ��С���ִ�������������������ʽ��¼�����ִ������ִ����ȡ����ػ�����βƫ��
static LPSTR FormatDecimalA(LPSTR buffer, FormatRec* rec, LPCSTR str, INT strLen)
{
  LPSTR p = buffer;
  INT spaces = rec->width - strLen;
  if (rec->negative) {
    spaces --;
    if (rec->left || rec->zero) {
      *p ++ = (rec->negative == -1 ? CHAR_NEG : CHAR_POS);
    }
  }
  if (rec->left == FALSE) {
    if (spaces > 0) {
      memset(p, rec->zero ? CHAR_ZERO : CHAR_SPACE, spaces);
      p += spaces;
    }
    if (rec->negative && !rec->zero) {
      *p ++ = (rec->negative == -1 ? CHAR_NEG : CHAR_POS);
    }
  }
  memcpy(p, str, strLen);
  p += strLen;
  if (rec->left && spaces > 0) {
    memset(p, CHAR_SPACE, spaces);
    p += spaces;
  }
  return p;
}
#define F_MAXEXPONENT 45
#define F_MINEXPONENT -45
// ���ָ���ַ�����buffer������ָ���ַ�������
INT PutExponent1(LPSTR buffer, CONST FloatRec* rec)
{
  LPSTR p = buffer;
  INT e, exp = rec->digits[0] ? rec->exponent - 1 : 0;
  *p ++ = rec->negative & 0x80 ? 'E' : 'e';
  if (exp < 0) {
    exp = -exp;
    *p ++ = '-';
  }
  else {
    *p ++ = '+';
  }
  if ((e = (exp / 1000)) != 0) {
    *p ++ = e + 0x30;
    exp %= 1000;
  }
  *p ++ = exp / 100 + 0x30;
  exp %= 100;
  *(PUSHORT)p = (((exp % 10) << 8) | (exp / 10)) + 0x3030;
  return (INT)(p - buffer + 2);
}
// ����������¼��Ϣת��Ϊָ����ʽ���ִ���
// ����������������������¼��ת�����ȣ��Ƿ�ǿ��С��λ
static INT FloatExponentA(LPSTR buffer, CONST FloatRec* rec, INT precision, BOOL decPoint)
{
  LPSTR p = buffer;
  LPCSTR digits = rec->digits;
  if (*digits) {
    *p ++ = *digits ++;
  }
  else {
    *p ++ = '0';
  }
  if (precision > 0 || decPoint) {
    for (*p ++ = '.'; precision > 0 && *digits; *p ++ = *digits ++, precision --);
    for (; precision > 0; *p ++ = '0', precision --);
  }
  p += PutExponent(p, rec);
  return (INT)(p - buffer);
}
// ����������¼��Ϣת��ΪС����ʽ���ִ���
// ����������������������¼��ת�����ȣ��Ƿ�ǿ��С��λ
static INT FloatDecimalA(LPSTR buffer, CONST FloatRec* rec, INT precision, BOOL decPoint)
{
  LPSTR p;
  LPCSTR digits;
  INT exp = rec->exponent;
  if (exp > F_MAXEXPONENT || exp < F_MINEXPONENT) {
    return FloatExponentA(buffer, rec, precision, decPoint);
  }
  p = buffer;
  digits = rec->digits;
  if (exp > 0) {
    for (; exp > 0 && *digits; *p ++ = *digits ++, exp --);
    for (; exp > 0; *p ++ = '0', exp --);
    if (decPoint || precision > 0) {
      *p ++ = '.';
    }
  }
  else {
    exp = -exp;
    precision -= exp;
    if (precision < 0) {
      exp += precision;
      precision = 0;
    }
    *p ++ = '0';
    if (exp > 0 || decPoint || precision > 0) {
      *p ++ = '.';
      for (; exp > 0; *p ++ = '0', exp --);
    }
  }
  for (; precision > 0 && *digits; *p ++ = *digits ++, precision --);
  for (; precision > 0; *p ++ = '0', precision --);
  return (INT)(p - buffer);
}
// ��������ʽ��ΪС����������������������ʽ��¼�����ػ�����βƫ��
static LPSTR FormatFloatFA(LPSTR buffer, FormatRec* rec)
{
  FloatRec fRec;
  INT len;
  CHAR tmp[F_MAXDECIMALS + 48];
  GetFloatRec(rec, 0, &fRec);
  if (fRec.digits[0] > '9') { // nan or inf
    return FormatDecimalA(buffer, rec, fRec.digits, 3);
  }
  len = FloatDecimalA(tmp, &fRec, rec->precision, rec->decimals);
  return FormatDecimalA(buffer, rec, tmp, len);
}
// ��������ʽ��Ϊָ����������������������ʽ��¼�����ػ�����βƫ��
static LPSTR FormatFloatEA(LPSTR buffer, FormatRec* rec, CHAR expChar)
{
  FloatRec fRec;
  INT len;
  CHAR tmp[F_MAXDECIMALS + 8];
  GetFloatRec(rec, 1, &fRec);
  if (fRec.digits[0] > '9') { // nan or inf
    return FormatDecimalA(buffer, rec, fRec.digits, 3);
  }
  if (expChar == 'E') {
    fRec.negative |= 0x80; // ��λ��1����д
  }
  len = FloatExponentA(tmp, &fRec, rec->precision, rec->decimals);
  return FormatDecimalA(buffer, rec, tmp, len);
}
// ��������ʽ��ΪС��������ָ����������������������ʽ��¼�����ػ�����βƫ��
static LPSTR FormatFloatGA(LPSTR buffer, FormatRec* rec, CHAR expChar)
{
  FloatRec fRec;
  INT len, precision;
  CHAR tmp[F_MAXDECIMALS + 48];
  GetFloatRec(rec, 2, &fRec);
  if (fRec.digits[0] > '9') { // nan or inf
    return FormatDecimalA(buffer, rec, fRec.digits, 3);
  }
  if (expChar == 'G') {
    fRec.negative |= 0x80; // ��λ��1����д
  }
  if (fRec.exponent > rec->precision || fRec.exponent < -3) {
    precision = rec->decimals ? rec->precision - 1 : lstrlenA(fRec.digits) - 1;
    len = FloatExponentA(tmp, &fRec, precision, rec->decimals);
  }
  else {
    precision = rec->decimals ? rec->precision - fRec.exponent : lstrlenA(fRec.digits) - fRec.exponent;
    if (precision < 0) {
      precision = 0;
    }
    len = FloatDecimalA(tmp, &fRec, precision, rec->decimals);
  }
  return FormatDecimalA(buffer, rec, tmp, len);
}
//
// ��sprintfA�����У��������ĸ�ʽ������ӵġ���������2�ֱ�����ʽ����С����ʽ��ָ����ʽ���ֱ���"%f"��"%e"��ʽ��ʾ�����⻹�и�"%g"��ʽ�����Ǹ��Զ���ʽ����sprintfAͨ�����������о�������������ʽ��
//
// ����С����ʽ�ĸ�ʽ���У������ݵĸ�ʽ���и����޳��ȣ���Ȼ������չ���ȸ������£���Щ���������ȿɴﵽ��5000λ����ʹ��˫���ȸ���������߳���Ҳ��300�ࡣ��printfϵ�к����У�������޳������������ͬ����ͬ���еĽ����ֵ��Ϊ100���еĶ�Ϊ�����ȸ���������������ʽ����38�ȣ��������������Ϊ������45λ�������ݳ���������޳��ȣ����Զ�����ָ����ʽ����ʽ�������ˡ�
//
// �ڽ���sprintfA���ݶ���ʱ��˵��������sprintfA�Ŀɱ��������û�в���ԭ�͹����������գ����������븡��������ʱҪע�����ʽ�ַ����ж�Ӧ�ĸ���������ƥ�䣬32λ�������ĸ�����ȱʡ������64λ˫����������ʹ����Ĳ��������Ǹ�����������Ҳ����չΪ˫���������������������long double������ʹ�õı�����֧��80λ��չ���ȸ�����ʱ�����ݵ���80λ��չ������������Ҳ��˫����������������һ���������������ǲ����Զ�ת���ġ�������ڲ���λ��������ǳ����͸�Ӧ��ע���ˣ�123��123L��123.0f��123.0��123.0L�⼸�ֳ�����ʽ�ǲ�ͬ�ģ�LҲ����Сд�����ֱ����������������������ȸ�������˫���ȸ���������չ���ȸ������������������֧�֣�Ҳ��˫�������������ԣ���32λ�����ϱ������и�ʽ%f��%lf�ǵ�ͬ�ģ���Ȼ���ڲ�֧����չ���ȸ������ı������У�%llf��%Lf��Ҳ��ͬ��%f��
//
// ���ĸ�ʽ��������ʱ�õ���FloatResolve�����Լ��й����ݶ�������Լ�����дC���Ը�����ת���ַ�����������
//
// �й�sprintfA�����Ľ��ܾ�ȫ����ϡ����´���û�����ϸ�Ĳ��ԡ�
LPSTR _sprintfA(LPSTR buffer, LPCSTR format, ...)
{
  FormatRec rec;
  BOOL flag;
  CHAR c;
  LPCSTR psave; // ?
  register LPCSTR pf = format;
  register LPSTR pb = buffer;
  va_list paramList;
  va_start(paramList, format);
  rec.param = (LPSTR)paramList;
  while (TRUE) {
    while (*pf && *pf != '%') {
      *pb ++ = *pf ++;
    }
    if (*pf == 0) {
      break;
    }
    if (*(pf + 1) == '%') { // ����%%
      *pb ++ = '%';
      pf += 2;
      continue;
    }
    psave = pf; // ?
    rec.width = rec.decimals = rec.negative = 0;
    rec.left = rec.zero = FALSE;
    rec.type = TYPE_GENERAL;
    rec.precision = -1;
    // ����ǰ������
    flag = TRUE;
    while (flag) {
      pf ++;
      switch (*pf) {
      case '0':
        rec.zero = TRUE;
        flag = FALSE;
        break;
      case '-':
        rec.left = TRUE;
        break;
      case '+':
        rec.negative = 1;
        break;
      case '#':
        rec.decimals = 1;
        break;
      default:
        pf --;
        flag = FALSE;
        break;
      }
    }
    // ���������Ⱥ;���
    flag = TRUE;
    while (flag) {
      pf ++;
      switch (*pf) {
      case '.': // ��С�����Ϊ'*','0' - '9'���������ȺͿ��
        rec.precision = 0;
        c = *(pf + 1);
        flag = (c == '*' || (c >= '0' && c <= '9'));
        break;
      case '*': // ����'*'��ʾ�Ŀ�Ȳ����;��Ȳ���
        if (*(pf - 1) == '.') {
          rec.precision = *(PINT)rec.param;
          flag = FALSE;
        }
        else {
          rec.width = *(PINT)rec.param;
          flag = *(pf + 1) == '.';
        }
        rec.param += TS_PTR;
        break;
      default: // �����ʽ�������ֱ�ʾ�Ŀ�Ⱥ;���
        if (*(pf - 1) == '.') {
          pf = GetControlNum(pf, &rec.precision);
          flag = FALSE;
        }
        else {
          pf = GetControlNum(pf, &rec.width);
          flag = *(pf + 1) == '.';
        }
      }
    }
    // �����������;���
    flag = TRUE;
    while (flag) {
      pf ++;
      switch (*pf) {
      case 'L':
        rec.type = TYPE_LLONG;
        break;
      case 'l':
        if (rec.type < TYPE_LLONG) {
          rec.type ++;
        }
        break;
      case 'H':
        rec.type = TYPE_CHAR;
        break;
      case 'h':
        if (rec.type > TYPE_CHAR) {
          rec.type --;
        }
        break;
      default:
        flag = FALSE;
      }
    }
    // �����������ͣ�����ʽ��
    c = *pf ++;
    switch (c) {
    case 's':
      pb = FormatStrA(pb, &rec);
      break;
    case 'c':
      pb = FormatCharA(pb, &rec);
      break;
    case 'd':
    case 'i':
    case 'u':
      pb = FormatIntA(pb, &rec, c == 'u');
      break;
    case 'f':
      pb = FormatFloatFA(pb, &rec);
      break;
    case 'e':
    case 'E':
      pb = FormatFloatEA(pb, &rec, c);
      break;
    case 'g':
    case 'G':
      pb = FormatFloatGA(pb, &rec, c);
      break;
    case 'x':
      if (rec.decimals) {
        rec.decimals = -1;
      }
    case 'X':
      pb = FormatHexA(pb, &rec, c);
      break;
    case 'o':
      pb = FormatOctalA(pb, &rec);
      break;
    case 'p':
      pb = FormatPointerA(pb, &rec);
      break;
    case 'n':
      GetPosSizeA(pb, buffer, &rec);
      break;
    default: // ���󣺿���formatʣ���ַ�������
      // pf = psave + 1; // ? Ҳ�ɴ���Ϊ���Ժ����
      // break; // ?
      lstrcpyA(pb, psave);
      return buffer;
    }
  }
  va_end(paramList);
  *pb = 0;
  return buffer;
}

