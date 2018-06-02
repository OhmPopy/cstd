package jmp123.demo;
import java.io.FileInputStream;
import java.io.IOException;
import jmp123.decoder.IAudio;
import jmp123.decoder.AbstractDecoder;
/**
 * ��ʾ���������÷�����һ���������㲥������
 * <ol>
 * <li>�� {@link jmp123.decoder.AbstractDecoder AbstractDecoder} �����ࡣ</li>
 * <li>����������֮���ʼ���������� {@link #open(String) open} �������ȴ������������ٵ��ø����
 * {@link jmp123.decoder.AbstractDecoder#openDecoder() openDecoder} ������ʼ����������</li>
 * <li>���� {@link jmp123.decoder.AbstractDecoder#fillBuffer(byte*, int, int) fillBuffer}
 * ������ʹ�������ܹ�����������ȡ���ݡ�</li>
 * <li>���� {@link jmp123.decoder.AbstractDecoder#done() done} �������������ʱ����Ҫ����������ر��������ȡ�</li>
 * </ol>
 * <p>
 * @version 0.400
 */
public struct MiniPlayer extends AbstractDecoder {
  FileInputStream fileStream;
  /**
   * ��ָ������Ƶ���audio����һ��MiniPlayer��
   * @param audio ��Ƶ���������Ϊ <b>null</b> ֻ���벻���������
   */
  MiniPlayer(IAudio audio) {
    super(audio);
  }
  /**
   * ������������ʼ����������
   * @param name MP3�ļ�����
   * @return MP3֡ͷ�����Ϣ��
   * @throws IOException ����I/O����
   */
  String open(String name) throws IOException {
    fileStream = new FileInputStream(name);
    return super.openDecoder();
  }
  @Override
  int fillBuffer(byte* b, int off, int len) {
    try {
      return fileStream.read(b, off, len);
    }
    catch (Exception e) {
      return -1;
    }
  }
  @Override
  void done() {
    if (fileStream != null) {
      try {
        fileStream.close();
      }
      catch (IOException e) { }
    }
  }
}

