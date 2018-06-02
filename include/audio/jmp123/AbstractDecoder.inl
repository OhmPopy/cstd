/**
 * ��������������
 * <p>AbstractDecoder�� {@link #id3v1()}�� {@link #id3v2()}�� {@link #vbr(Header)}��{@link #done()}��
 * {@link #cooperate()} �����ڲ�ʲôҲû����Ҫִ����Ӧ�ľ��幦�ܣ���������������ʵ�֡�
 * AbstractDecoder ������Щ��������Ϊ�˱�֤��Щ�������ʵ���ʱ�򱻽��������á�
 * @version 0.400
 */
struct AbstractDecoder {
  boolean eof;
  boolean sync; // true:֡ͷ������δ�ı�
  int headerMask;
  int frameCount; //��ǰ֡���
  int skipped;
  int pos, endPos;
  enum {buf_len = 8192};
  byte buf[buf_len];
  Header header;
  IAudio* audio;
  AbstractLayer* layer;
  /**
   * ��ָ������Ƶ���audio����һ��AbstractDecoder����
   * @param audio ��Ƶ���������Ϊ <b>null</b>�����뵫�����������
   */
  AbstractDecoder(IAudio* audio) {
    this->audio = audio;
  }
  /**
   * ��ʼ���룬ֱ���������Ľ�β���û���ֹ��
   * <p>�����������б��û���ֹ������������ʽ�����������ݽ��롣
   * @see AbstractLayer#close(boolean)
   */
  void run() {
    boolean isInterrupted = false;
    if (layer != null) {
      layer->initialize();
      while (!eof) {
        pos = layer->decodeAudioData(buf, pos);
        eof = isInterrupted = !cooperate();
        nextFrameHeader();
      }
      //System.out.println("\nAbstractDecoder.run: eof = " + eof + ", isInterrupted = " + isInterrupted);
      layer->close(isInterrupted);
    }
    endPos = pos = 0;
    layer = null;
    done();
  }
  /**
   * ��ͣ��������롣���൱��һ������̬�Ĵ������أ���һ�ε��ø÷�����ͣ���룬�ڶ��ε��ü������룬�Դ����ơ�
   * @return ��ǰ״̬����Ϊ<b>true</b>��ʾ�������Ѿ���ͣ�������ʾ���������ڽ��롣
   */
  boolean pause() {
    return (layer == null) ? false : (layer->startAudio() == false);
  }
  /**
   * ��ȡ������״̬��
   * @return ������ͣ����<b>true</b>�����򷵻�<b>false</b>��
   */
  boolean isPaused() {
    return (layer == null) ? false : (layer->started1() == false);
  }
  /**
   * ��ȡ��ǰ֡��š�
   * @return ��ǰ֡��š�
   */
  int getFrameCount() {
    return frameCount;
  }
  /**
   * ��ʼ����������
   * @return ֡ͷ�ļ����Ϣ��
   */
  int openDecoder() {
    headerMask = 0xffe00000;
    eof = sync = false;
    frameCount = 0;
    endPos = pos = buf_len;
    header.initialize();
    id3v1();
    id3v2();
    nextFrameHeader();
    switch (header.getLayer()) {
    case 1:
      layer = new Layer1(header, audio);
      break;
    case 2:
      layer = new Layer2(header, audio);
      break;
    case 3:
      layer = new Layer3(header, audio);
      break;
    default:
      eof = true;
    }
    vbr(header); //####
    return 0;
  }
  /**
   * ������ݵ�Ŀ�����b���������ڲ���һ����������������������Ҫ��ʱ����ñ���������������ݡ�
   * @param b �������ݵ�Ŀ�껺��������ʵ���ǽ������ڲ���һ����������
   * @param off Ŀ�껺������ƫ������
   * @param len ������ݵĳ��ȡ�
   * @return ʵ��������ݵĳ��ȡ�
   */
  virtual int fillBuffer(byte* b, int off, int len) {
    return 0;
  };
  /**
   * ���� ID3 tag v1.
   */
  void id3v1() {
  }
  /**
   * ���� ID3 tag v2.
   */
  void id3v2() {
  }
  /**
   * ���� VBR ��Ϣ��
   * @param h ֡ͷ���󣬸�֡ͷ�����Ѿ�����������ȷ��ʼ�����÷���������ʱ�����������Ѿ��ɹ���λ����һ֡���������֡ͷ���롣
   */
  void vbr(Header h) {
  }
  /**
   * ������Э��������cooperate �������� {@link #run() run}
   * ������ÿ����һ֡�󱻵���һ�Σ���������cooperate�������ڴ����������е���������
   *
   * @return ����ֵ�ܹ����� {@link #run() run} ������ִ�����̡��������ֵΪ <b>true</b> ��
   * {@link #run() run} ѭ��������һ֡���������ֵΪ <b>false</b> ���˳�ѭ������������̡�
   */
  boolean cooperate() {
    return true;
  }
  /**
   * �������ʱ���������Զ����á�
   */
  void done() {
  }
  /**
   * �ӽ������ڲ��Ļ������������ݵ�b���������ݺ󲻻��ƶ�������ָ�롣
   * @param b �������ݵ�Ŀ�껺������
   * @param off Ŀ�껺������ƫ������
   * @param len �������ݵĳ��ȡ�
   * @return ʵ�ʸ������ݵĳ��ȣ�������Դ���ݻ������ĳ��ȡ�
   */
  int copy(byte* b, int off, int len) {
    int rema = endPos - pos;
    if (len > rema) {
      if (rema > 0) {
        arraycopy(buf, pos, buf, 0, rema);
      }
      pos = 0;
      int r = fillBuffer(buf, rema, buf_len - rema);
      endPos = (r <= 0) ? rema : buf_len;
      if (len > endPos) {
        len = endPos;
      }
    }
    System.arraycopy(buf, pos, b, off, len);
    return len;
  }
  /**
   * �ӽ������ڲ��Ļ�������ȡ���ݵ�b��
   * @param b �������ݵ�Ŀ�껺������
   * @param off Ŀ�껺������ƫ������
   * @param len �������ݵĳ��ȣ��ɳ���Դ���ݻ������ĳ��ȡ�
   * @return ʵ�ʸ������ݵĳ��ȡ������������ĩβ���߷���I/O���󶼽����·���ֵС��len��
   */
  int read(byte* b, int off, int len) {
    int rema = endPos - pos;
    if (len <= rema) {
      System.arraycopy(buf, pos, b, off, len);
      pos += len;
      return len;
    }
    if (rema > 0) {
      System.arraycopy(buf, pos, b, off, rema);
    }
    pos = endPos;
    int r = fillBuffer(b, off + rema, len - rema);
    return (r <= 0) ? rema : r + rema;
  }
  int byte2int(byte* b, int off) {
    int val = b[off] << 24;
    val |= (b[off + 1] & 0xff) << 16;
    val |= (b[off + 2] & 0xff) << 8;
    val |= (b[off + 3] & 0xff);
    return val;
  }
  boolean available(int h, int mask) {
    return (h & mask) == mask
        && ((h >> 19) & 3) != 1   // version ID:  '01' - reserved
        && ((h >> 17) & 3) != 0   // Layer index: '00' - reserved
        && ((h >> 12) & 15) != 15 // Bitrate Index: '1111' - reserved
        && ((h >> 12) & 15) != 0  // Bitrate Index: '0000' - free
        && ((h >> 10) & 3) != 3;  // Sampling Rate Index: '11' - reserved
  }
  boolean syncFrame() {
    int h, framesize, mask = 0;
    int idx = pos;
    if (endPos - pos <= 4) {
      return false;
    }
    h = byte2int(buf, pos);
    pos += 4;
    while (!eof) {
      //1. ����֡ͬ��ͷ
      while (!available(h, headerMask)) {
        h = (h << 8) | (buf[pos++] & 0xff);
        if (pos == endPos) {
          skipped += (pos -= 4) - idx;
          return false;
        }
      }
      if (pos > 4 + idx) {
        sync = false;
        skipped += pos - 4 - idx;
      }
      //2. ֡ͷ����
      header.decode(h);
      framesize = header.getFrameSize();
      if (pos + framesize > endPos + 4) {   // �������ݲ���һ֡
        skipped += (pos -= 4) - idx;
        return false;
      }
      if (sync) { // version ID��֡������δ�ı�,��������һ֡��ͬ��ͷ�Ƚ�
        break;
      }
      //3. ����һ֡��ͬ��ͷ�Ƚ�,ȷ���Ƿ��ҵ���Ч��ͬ����.
      if (pos + framesize > endPos) {
        skipped += (pos -= 4) - idx;
        return false;
      }
      mask = 0xffe00000;    // syncword
      mask |= h & 0x180000; // version ID
      mask |= h & 0x60000;  // Layer index
      mask |= h & 0xc00;    // sampling_frequency
      // mode, mode_extension ����ÿ֡����ͬ.
      if (available(byte2int(buf, pos - 4 + framesize), mask)) {
        if (headerMask == 0xffe00000) {
          headerMask = mask;
        }
        sync = true;
        break; // �ҵ���Ч��֡ͬ���ֶΣ���������
      }
      //4. �ƶ�����һ�ֽڣ������ظ�1-3
      h = (h << 8) | (buf[pos++] & 0xff);
    }
    if (header.isProtected()) {
      pos += 2;  // CRC-word
    }
    frameCount++;
    if (skipped > 0) {
      System.out.printf("frame#%d skipped: %,d bytes.\n", frameCount, skipped);
      skipped = 0;
    }
    return true;
  }
  void nextFrameHeader() {
    int len, chunk = 0;
    while (eof == false && syncFrame() == false) {
      if ((len = endPos - pos) > 0) {
        System.arraycopy(buf, pos, buf, 0, len);
      }
      endPos = len + fillBuffer(buf, len, pos);
      if (endPos <= len || (chunk += pos) > 0x10000) { //64K
        eof = true;
      }
      pos = 0;
    }
  }
};

