// MPEG-1/2/2.5 Audio Layer I/II/III ֡ͬ����֡ͷ��Ϣ����
/**
 * ֡ͬ����֡ͷ��Ϣ���롣<p>Դ�����н���֡ͷ32λ�õ��ı��������壺
 * <table border="2" bordercolor="#000000" cellpadding="8" style="border-collapse:collapse">
 * <tr><th>ƫ����</th><th>����</th><th>������</th><th>����</th></tr>
 * <tr><td>0</td><td>11</td><td>֡ͬ��ʱֱ�ӽ���</td><td>11λȫ����Ϊ'1'��֡ͬ����</td></tr>
 * <tr><td>11</td><td>2</td><td>verID</td><td>MPEG�汾</td></tr>
 * <tr><td>13</td><td>2</td><td>layer</td><td>MPEGѹ����</td></tr>
 * <tr><td>15</td><td>1</td><td>protection_bit</td><td>�Ƿ�CRC</td></tr>
 * <tr><td>16</td><td>4</td><td>bitrate_index</td><td>λ������</td></tr>
 * <tr><td>20</td><td>2</td><td>sampling_frequency</td><td>����������</td></tr>
 * <tr><td>22</td><td>1</td><td>padding</td><td>��ǰ֡�Ƿ񸽼����һ������</td></tr>
 * <tr><td>23</td><td>1</td><td>δ����</td><td>��֪�Ƿ�˽��</td></tr>
 * <tr><td>24</td><td>2</td><td>mode</td><td>����ģʽ</td></tr>
 * <tr><td>26</td><td>2</td><td>mode_extension</td><td>������չģʽ</td></tr>
 * <tr><td>28</td><td>1</td><td>δ����</td><td>��֪�Ƿ��а�Ȩ</td></tr>
 * <tr><td>29</td><td>1</td><td>δ����</td><td>��֪�Ƿ�Ϊԭ��</td></tr>
 * <tr><td>30</td><td>2</td><td>�����ã�δ����</td><td>Ԥ����</td></tr>
 * </table>
 * <p>
 * @version 0.400
 */
/**
 * MPEG�汾MPEG-1��
 */
static const int MPEG1 = 3;
/**
 * MPEG�汾MPEG-2��
 */
static const int MPEG2 = 2;
/**
 * MPEG�汾MPEG-2.5���ǹٷ��汾����
 */
static const int MPEG25 = 0;
//static int MAX_FRAMESIZE = 1732;
/*
 * ��lsf,layer,bitrate_index��������: bitrate[lsf][layer-1][bitrate_index]
*/
static const int bitrate[2][3][15] = {
  {
    //MPEG-1
    //Layer I
    {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448},
    //Layer II
    {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384},
    //Layer III
    {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320}
  },
  {
    //MPEG-2/2.5
    //Layer I
    {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256},
    //Layer II
    {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160},
    //Layer III
    {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160}
  }
};
/*
 * samplingRate[verID][sampling_frequency]
 */
static const int samplingRate[4][4] = {
  {11025 , 12000 , 8000, 0}, //MPEG-2.5
  {0, 0, 0, 0,},      //reserved
  {22050, 24000, 16000 , 0}, //MPEG-2 (ISO/IEC 13818-3)
  {44100, 48000, 32000, 0}  //MPEG-1 (ISO/IEC 11172-3)
};
typedef struct Header {
  /*
   * verID: 2-bit
   * '00'  MPEG-2.5 (unofficial extension of MPEG-2);
   * '01'  reserved;
   * '10'  MPEG-2 (ISO/IEC 13818-3);
   * '11'  MPEG-1 (ISO/IEC 11172-3).
   */
  int verID;
  /*
   * layer: 2-bit
   * '11'  Layer I
   * '10'  Layer II
   * '01'  Layer III
   * '00'  reserved
   *
   * �ѻ���layer=4-layer: 1--Layer I; 2--Layer II; 3--Layer III; 4--reserved
   */
  int layer;
  /*
   * protection_bit: 1-bit
   * '1'  no CRC;
   * '0'  by 16-bit CRC following header.
   */
  int protection_bit;
  /*
   * bitrate_index: 4-bit
   */
  int bitrate_index;
  /*
   * sampling_frequency: 2-bit
   * '00'  44.1kHz
   * '01'  48kHz
   * '10'  32kHz
   * '11'  reserved
   */
  int sampling_frequency;
  int padding;
  /*
   * mode: 2-bit
   * '00'  Stereo;
   * '01'  Joint Stereo (Stereo);
   * '10'  Dual channel (Two mono channels);
   * '11'  Single channel (Mono).
   */
  int mode;
  /*
   * mode_extension: 2-bit
   *     intensity_stereo MS_stereo
   * '00'  off        off
   * '01'  on         off
   * '10'  off        on
   * '11'  on         on
   */
  int mode_extension;
  int framesize;
  int maindatasize;
  int sideinfosize;
  int lsf;
  BOOL isMS, isIntensityStereo;
} Header;
/**
 * ֡ͷ���롣
 * @param h ֡ͷ����ͷ���ϵ�4�ֽ�(32λ)������
 */
void Header_decode(int h, Header* s)
{
  s->verID = (h >> 19) & 3;
  s->layer = 4 - ((h >> 17) & 3);
  s->protection_bit = (h >> 16) & 0x1;
  s->bitrate_index = (h >> 12) & 0xF;
  s->sampling_frequency = (h >> 10) & 3;
  s->padding = (h >> 9) & 0x1;
  s->mode = (h >> 6) & 3;
  s->mode_extension = (h >> 4) & 3;
  s->isMS = s->mode == 1 && (s->mode_extension & 2) != 0;
  s->isIntensityStereo = s->mode == 1 && (s->mode_extension & 0x1) != 0;
  s->lsf = (s->verID == MPEG1) ? 0 : 1;
  switch (s->layer) {
  case 1:
    s->framesize = bitrate[s->lsf][0][s->bitrate_index] * 12000;
    s->framesize /= samplingRate[s->verID][s->sampling_frequency];
    s->framesize += s->padding;
    s->framesize <<= 2; // 1-slot = 4-byte
    break;
  case 2:
    s->framesize  = bitrate[s->lsf][1][s->bitrate_index] * 144000;
    s->framesize /= samplingRate[s->verID][s->sampling_frequency];
    s->framesize += s->padding;
    break;
  case 3:
    s->framesize = bitrate[s->lsf][2][s->bitrate_index] * 144000;
    s->framesize /= samplingRate[s->verID][s->sampling_frequency] << s->lsf;
    s->framesize += s->padding;
    //����֡����Ϣ����
    if (s->verID == MPEG1) {
      s->sideinfosize = (s->mode == 3) ? 17 : 32;
    }
    else {
      s->sideinfosize = (s->mode == 3) ? 9 : 17;
    }
    break;
  }
  //���������ݳ���
  s->maindatasize = s->framesize - 4 - s->sideinfosize;
  if (s->protection_bit == 0) {
    s->maindatasize -= 2;  //CRC-word
  }
}
/**
 * �Ƿ���ѭ������У���롣
 * @return ����true��ʾ��ѭ������У���룬֡ͷ֮���ڽ���2�ֽڵ���������CRC��
 */
#define Header_isProtected(h) (h->protection_bit == 0)
/**
 * ��ȡ����ģʽ�Ƿ�Ϊ��/����������Mid/Side stereo��ģʽ��
 *
 * isMS true��ʾ����/��������ģʽ��
*/
/**
 * ��ȡ����ģʽ�Ƿ�Ϊǿ����������Intensity Stereo��ģʽ��
 *
 * isIntensityStereo true��ʾ��ǿ��������ģʽ��
 */
/**
 * ��ȡ��ǰ֡��λ�ʡ�
 *
 * @return ��ǰ֡��λ�ʣ���λΪ��ǧλÿ�루Kbps������
 */
#define Header_getBitrate(h, j) bitrate[h->lsf][h->layer - 1][h->bitrate_index];
/**
 * ��ȡ��ǰ֡��λ�ʵ�����ֵ��
 *
 * @return ��ǰ֡��λ�ʵ�����ֵ��λ�ʵ�����ֵ��Χ��1��14��ĳһ������
 */
#define Header_getBitrateIndex(h)  h->bitrate_index
/**
 * ��ȡ��������
 *
 * @return ��������1��2��
 */
#define Header_getChannels(h)  ((h->mode == 3) ? 1 : 2)
/**
 * ��ȡ����ģʽ��
 *
 * @return ����ģʽ����ֵ��ʾ�ĺ��壺
 * <table border="1" bordercolor="#000000" cellpadding="8" style="border-collapse:collapse">
 * <tr><th>����ֵ</th><th>����ģʽ</th></tr>
 * <tr><td>0</td><td>��������stereo��</td></tr>
 * <tr><td>1</td><td>������������joint stereo��</td></tr>
 * <tr><td>2</td><td>˫������dual channel��</td></tr>
 * <tr><td>3</td><td>��������mono channel��</td></tr>
 * </table>
 * @see #getModeExtension()
 */
/**
 * ��ȡ������չģʽ��
 *
 * @return ������չģʽ����ֵ��ʾ��ǰ����ʹ�õ����������뷽ʽ��
 * <table border="1" bordercolor="#000000" cellpadding="8" style="border-collapse:collapse">
 * <tr><th>����ֵ</th><th>ǿ��������</th><th>��/��������</th></tr>
 * <tr><td>0</td><td>off</td><td>off</td></tr>
 * <tr><td>1</td><td>on</td><td>off</td></tr>
 * <tr><td>2</td><td>off</td><td>on</td></tr>
 * <tr><td>3</td><td>on</td><td>on</td></tr>
 * </table>
 * @see #getMode()
 */
#define Header_getModeExtension(h) h->mode_extension
/**
 * ��ȡMPEG�汾��
 *
 * @return MPEG�汾��{@link #MPEG1}�� {@link #MPEG2} �� {@link #MPEG25} ��
 */
#define Header_getVersion(h)  h->verID
/**
 * ��ȡMPEG����㡣
 *
 * @return MPEG����㣺����ֵ1��ʾLayer��2��ʾLayer��3��ʾLayer��
 */
/**
 * ��ȡPCM���������ʵ�����ֵ��
 *
 * @return PCM���������ʵ�����ֵ��
 */
#define Header_getSamplingFrequency(h)  h->sampling_frequency
/**
 * ��ȡPCM���������ʡ�
 *
 * @return ��ȡPCM���������ʣ���λ�����ȣ�Hz����
 */
#define Header_getSamplingRate(h)   samplingRate[h->verID][h->sampling_frequency]
/**
 * ��ȡ�����ݳ��ȡ�
 *
 * @return ��ǰ֡�������ݳ��ȣ���λ���ֽڡ���
 */
#define Header_getMainDataSize(h)   h->maindatasize
/**
 * ��ȡ����Ϣ���ȡ�
 *
 * @return ��ǰ֡����Ϣ���ȣ���λ���ֽڡ���
 */
#define Header_getSideInfoSize(h)   h->sideinfosize
/**
 * ��ȡ֡���ȡ�<p>֡�ĳ��� = 4�ֽ�֡ͷ + CRC������еĻ���2�ֽڣ� + �������ݳ��ȡ�
 * <br>�����������ݳ��� = ����Ϣ���� + �����ݳ��ȡ�
 * <p>�����ǿɱ�λ�ʣ�VBR��������ļ����ǹ̶�λ�ʣ�CBR��������ļ���ÿ֡�ĳ��Ȳ�һ��ͬ��
 *
 * @return ��ǰ֡�ĳ��ȣ���λ���ֽڡ���
 */
#define Header_getFrameSize(h) h->framesize
/**
 * ��ȡ��ǰ֡�����õ���PCM�������ȡ�ͨ�������ͬһ�ļ�ÿһ֡�����õ���PCM������������ͬ�ġ�
 *
 * @return ��ǰ֡�����õ���PCM�������ȣ���λ���ֽڡ���
 */
int Header_getPcmSize(Header* h)
{
  int pcmsize = (h->verID == MPEG1) ? 4608 : 2304;
  if (h->mode == 3) { // if channels == 1
    pcmsize >>= 1;
  }
  return pcmsize;
}
/**
 * ��ȡ��ǰ�ļ�һ֡�Ĳ���ʱ�䳤�ȡ�
 *
 * @return ��ǰ�ļ�һ֡�Ĳ���ʱ�䳤�ȣ���λ���롱��
 */
#define Header_getFrameDuration(h) (1152.f / (Header_getSamplingRate(h) << h->lsf))
/**
 * ��ȡ֡ͷ�ļ����Ϣ��
 * @return ֡ͷ�ļ����Ϣ��
 */
int Header_toString(const Header* s, char* sbuf, int len)
{
  int l2 = 0;
  if (len <= 0 || NULL == sbuf) {
    return 0;
  }
  sbuf[0] = 0;
  if (s->verID == MPEG25) {
    l2 += cstr_cat1(sbuf, len, "MPEG-2.5");
  }
  else if (s->verID == MPEG2) {
    l2 += cstr_cat1(sbuf, len, "MPEG-2");
  }
  else if (s->verID == MPEG1) {
    l2 += cstr_cat1(sbuf, len, "MPEG-1");
  }
  else {
    cstr_cpy(sbuf, len, "Let me tell you gently\nThe header is unavailable", -1);
    return 0;
  }
  l2 += cstr_cat1(sbuf, len, ", Layer ");
  cstr_cat_int32(sbuf, len, s->layer);
  l2 += cstr_cat1(sbuf, len, ", ");
  cstr_cat_int32(sbuf, len, Header_getSamplingRate(s));
  l2 += cstr_cat1(sbuf, len, "Hz, ");
  if (s->mode == 0) {
    l2 += cstr_cat1(sbuf, len, "Stereo");
  }
  else if (s->mode == 1) {
    l2 += cstr_cat1(sbuf, len, "Joint Stereo");
  }
  else if (s->mode == 2) {
    l2 += cstr_cat1(sbuf, len, "Dual channel");
  }
  else if (s->mode == 3) {
    l2 += cstr_cat1(sbuf, len, "Mono");
  }
  if (s->mode_extension == 1) {
    l2 += cstr_cat1(sbuf, len, "(I/S)");
  }
  else if (s->mode_extension == 2) {
    l2 += cstr_cat1(sbuf, len, "(M/S)");
  }
  else if (s->mode_extension == 3) {
    l2 += cstr_cat1(sbuf, len, "(I/S & M/S)");
  }
  return 0;
}

