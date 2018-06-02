/**
 * ��������ֽ������λ����
 * <p>����Layer3�ļ����룬 BitStream �����ڶ�ȡ֡����Ϣ��Ҫ��ȡLayer3�ļ��������ݣ�����ʹ��{@link BitStreamMainData}��
 * <p>����Layer1��Layer2�ļ����룬�� BitStream ��õ�λ�������ڽ���ȫ�����ݡ�
 * @version 0.400
 */
struct BitStream {
  int bitPos;
  int bytePos;
  byte *bitReservoir;
  int endPos;   //bitReservoir��������ֽ���
  int maxOff;
  /**
   * ����һ��λ��BitStream����λ���Ļ�������Сlenָ����λ���Ļ�����β���ճ��ĳ�����extraָ����
   *
   * @param len �������ɷ��ʳ��ȡ�<br>
   *            ���������ڽ���֡����ϢʱlenΪ9��17��32��<br>
   *            ���������ڽ���������(main_data)ʱ���Ȳ�С�����֡��512+1732��
   *
   * @param extra ������β���ճ����ֽ�������ֹ����������ʱλ���д����»����������
   * β���ճ�512�ֽ�(part2_3_length���ȣ�2^12λ)��
   */
  BitStream(int len, int extra) {
    maxOff = len;
    MYREALLOC(bitReservoir, len + extra);
  }
  /**
   * �򻺳������len�ֽڡ�
   *
   * @param b Դ���ݡ�
   * @param off Դ����ƫ������
   * @param len Դ���ݳ��ȡ�
   * @return ʵ����䵽���������ֽ�����
   */
  int append(byte* b, int off, int len) {
    if (len + endPos > maxOff) {
      // ��������bytePos��֮���(δ�������)�����ƶ�����������
      arraycopy(bitReservoir, bytePos, bitReservoir, 0, endPos - bytePos);
      endPos -= bytePos;
      bitPos = bytePos = 0;
    }
    if (len + endPos > maxOff) {
      len = maxOff - endPos;
    }
    arraycopy(b, off, bitReservoir, endPos, len);
    endPos += len;
    return len;
  }
  /**
   * ������ָ��Ϊb����������ʼƫ������offָ������ {@link #append(byte*, int, int)}
   * �����������ǣ������������Դ����b�������ݡ�
   *
   * @param b Դ���ݡ�
   * @param off Դ����ƫ������
   */
  void feed(byte* b, int off) {
    bitReservoir = b;
    bytePos = off;
    bitPos = 0;
  }
  /**
   * �ӻ�������ȡһλ��
   * @return 0��1��
   */
  int get1Bit() {
    int bit = bitReservoir[bytePos] << bitPos;
    bit >>= 7;
    bit &= 0x1;
    bitPos++;
    bytePos += bitPos >> 3;
    bitPos &= 0x7;
    return bit;
  }
  /**
   * �ӻ�������ȡnλ����������ʱ�ٶȷ����ԭ������ȡ��λ��������9���뿼����{@link #getBits9(int)}��������Ч��
   * @param n ��������n=2..17ʱ���ô˷�����
   * @return nλ��ֵ��
   */
  int getBits17(int n) {
    int iret = bitReservoir[bytePos];
    iret <<= 8;
    iret |= bitReservoir[bytePos + 1] & 0xff;
    iret <<= 8;
    iret |= bitReservoir[bytePos + 2] & 0xff;
    iret <<= bitPos;
    iret &= 0xffffff; // ��8λ��0
    iret >>= 24 - n;
    bitPos += n;
    bytePos += bitPos >> 3;
    bitPos &= 0x7;
    return iret;
  }
  /**
   * �ӻ�������ȡnλ��
   * @param n ��������n=2..9ʱ���ô˷�����
   * @return nλ��ֵ��
   */
  int getBits9(int n) {
    int iret = bitReservoir[bytePos];
    iret <<= 8;
    iret |= bitReservoir[bytePos + 1] & 0xff;
    iret <<= bitPos;
    iret &= 0xffff; // ��16λ��0
    iret >>= 16 - n;
    bitPos += n;
    bytePos += bitPos >> 3;
    bitPos &= 0x7;
    return iret;
  }
  /**
   * ��ȡ�������ֽ�ָ�롣
   * @return �������ֽ�ָ�롣
   */
  int getBytePos() {
    return bytePos;
  }
  /**
   * ��ȡ�������Ѿ�������ֽ�����
   * @return �������Ѿ�������ֽ�����
   */
  int getSize() {
    return endPos;
  }
  /**
   * ����������n�ֽڣ�����������ָ�븴λ��
   * @param n �������ֽ�����
   */
  void skipBytes(int n) {
    bytePos += n;
    bitPos = 0;
  }
  /**
   * ���������������ָ�����ء�
   * @param n ��n>0����n���أ���n<0�����-n���ء�
   */
  void skipBits(int n) {
    bitPos += n;
    bytePos += bitPos >> 3;
    bitPos &= 0x7;
  }
  //----debug
  int markPos;
  void mark() {
    markPos = bytePos;
  }
  int getMark() {
    return bytePos - markPos;
  }
};

