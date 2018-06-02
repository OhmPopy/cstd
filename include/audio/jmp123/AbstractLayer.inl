/**
 * ����1֡MPEG-1/MPEG-2/MPEG-2.5 Audio Layer��/��/��  �������Ƶ�����
 * <p>Դ�����أ� http://jmp123.sf.net/
 * @version 0.400
 */
struct AbstractLayer {
  //public static boolean debug1; //----debug
  int channels;
  /*
   * ������Ƶ�������ͣ/����
   */
  boolean started;
  /*
   * Layer1, Layer2, Layer3 ����һ���˲�����
   */
  Synthesis filter;
  /*
   * ��Ƶ���������һ�����ñ��������ⲿ��ʼ�����롣
   */
  IAudio* audio;
  /*
   * ����PCM��������5������
   */
  byte* pcmbuf;
  int pcmbuf_len;
  int size;     // һ������Ƶ������pcmbufд��ĳ���
  int writeCursor[2];  // ����������pcmbufд������ʱʹ�õ�ƫ����
  int readCursor;   // ��ȡpcmbufʱʹ�õ�ƫ����
  int remainder;    // δ���ƻ������Ĳ�����PCM����
  /**
   * ��ָ��֡ͷh����Ƶ���audio���󴴽�һ��֡��������
   * @param h �Ѿ�����֡ͷ��Ϣ��֡ͷ����
   * @param audio ��Ƶ����������ָ��Ϊ <b>null</b> ����� {@link #outputPCM()}
   * ��������������ƶ�PCM������ָ�루��ʾ�������Ѿ�ȡ�ߣ�������д�����ݣ���
   */
  AbstractLayer() {}
  void set(Header h, IAudio* audio) {
    this->audio = audio;
    started = true;
    size = 4 * h.getPcmSize();  //#### �������ֲ��ܸ���
    channels = h.getChannels();
    filter.set_nth(channels);
    writeCursor[0] = 0;
    writeCursor[1] = 2;       //####
    pcmbuf_len = size * 4;
    MYREALLOC(pcmbuf, pcmbuf_len);  //####
    if (audio != NULL) {
      audio->open(h.getSamplingRate(), channels, size * 4);  //####
    }
  }
  ~AbstractLayer() {
    FREE(pcmbuf);
  }
  /**
   * �Ӵ˻�����b�и���ƫ����off����ʼ����һ֡����֡ͷ��ģ��������ݡ�
   * @param b Դ���ݻ�������
   * @param off Դ���ݻ�������ƫ������
   * @return Դ���ݻ������µ�ƫ���������ڼ��������һ֡���ݵĿ�ʼλ����Դ���ݻ�������ƫ������
   */
  virtual int decodeAudioData(byte* b, int off) {
    return 0;
  }
  /**
   * һ���Ӵ�����ϳ����ˡ�
   * @param samples �����32������ֵ��
   * @param ch ��ǰ������0��ʾ��������1��ʾ��������
   */
  void synthesisSubBand(float* samples, int ch) {
    writeCursor[ch] = filter.synthesisSubBand(samples, ch, pcmbuf, writeCursor[ch]);
  }
  /**
   * �Ƿ񵽴�PCM������ĩβ��������ﻺ����ĩβ���ڻ�������ջ���ȡ�߲�������֮ǰ����д�롣
   * @param ch ������0��ʾ��������1��ʾ��������
   * @return ����<b>true</b>��ʾ���ﻺ����ĩβ�����򷵻�<b>false</b>��
   */
  boolean isEndOfBuffer(int ch) {
    return writeCursor[ch] >= pcmbuf_len;
  }
  /**
   * ��ʼ����<p>�������ᱻ��������ִ�н���ѭ��ǰ���ã���������Ҫ���ĳЩ�ض��ĳ�ʼ�������ر�������
   */
  void initialize() {
  }
  /**
   * ���������������
   * <p>����ϳ��˲������PCM����д�뻺�������������������������4֡�õ���PCM���ݲŲ���һ�������
   * �������߲�����Ҫ֪����ǰ�������Ƿ��Ѿ������㹻���ݡ� ��ֹ�����������ÿ�������4֡Ӧ���ñ�����1�Σ�
   * ��ȻҲ����ÿ����1֡�͵��ñ�����1�Ρ�
   * <p>��������Ƶ����������ӻ�����ȡ�߽���4֡�õ���PCM���ݡ�
   * <p><b>���ܲ���������</b>����Ƶ����Ѿ�ֹͣ���Ա������ĵ��ý���������ֱ����ʼ��Ƶ�������������������꣬�������Զ������
   * @see #startAudio()
   */
  int outputPCM() {
      //if (debug1)
      //System.out.printf("outputPCM(1): thread=%s, remainder=%d\n",Thread.currentThread().getName(), remainder);
      int w;
      while (remainder > 0) {
        if ((w = audio->write(pcmbuf, readCursor, remainder)) == 0) {
          return 0;
        }
        remainder -= w;
        if ((readCursor += w) >= pcmbuf_len) {
          resetOffset();
        }
        if (remainder == 0) {
          return size;  // ĳһ������������
        }
      }
      if (writeCursor[0] < size + readCursor || (channels == 2 && writeCursor[1] < size + readCursor)) {
        return 0;
      }
      w = (audio == NULL) ? size : audio->write(pcmbuf, readCursor, size);
      remainder = size - w;
      if ((readCursor += w) >= pcmbuf_len) {
        resetOffset();
      }
      //if (debug1)
      //System.out.printf("outputPCM(2), thread=%s, w=%d\n",Thread.currentThread().getName(), w);
      return (w != size) ? 0 : size;
  }
  /**
   * ��ʼ��ֹͣ��Ƶ�����
   * @return ��ʼ��Ƶ������� <b>true</b>�����򷵻� <b>false</b>��
   */
  boolean startAudio() {
      started = (!started);
      audio->start(started);
      if (started) {
        //audioLock.notifyAll();
      }
      //if(debug1 == false)
      //debug1 = true;
      //System.out.println("\nAbstractLayer.startAudio ret: "+ started);
      return started;
  }
  /**
   * ��ȡ��Ƶ���״̬��
   * @return �Ѿ���ʼ��Ƶ�������<b>true</b>������ֹͣ��Ƶ����򷵻�<b>false</b>��
   */
  boolean started1() {
    return this->started;
  }
  /**
   * �����������ݽ��룬����PCM������ָ�븴λ�������Ϊ��Ƶ����Ѿ�ֹͣ������ {@link #outputPCM()} ���������������������
   * @param interrupted �Ƿ���Ҫ��������ʽ�رա���Ϊ
   * <b>false</b>���ȴ���Ƶ�����������ȫ������ˢ����Ƶ�������󷵻أ���Ϊ <b>true</b> ��������ʽ���ء�
   */
  void close(boolean interrupted) {
    if (audio != null && started) {
      if (!interrupted) {
        if (writeCursor[0] > readCursor) {
          audio->write(pcmbuf, 0, writeCursor[0] - readCursor);
        }
        audio->drain();
      }
      audio->close();
    }
    resetOffset();
    started = true;
  }
  // ������pcmbufָ�븴λ
  void resetOffset() {
    writeCursor[0] = readCursor = 0;
    writeCursor[1] = 2;
  }
};

