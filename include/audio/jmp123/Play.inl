package jmp123.demo;
import java.io.IOException;
import jmp123.output.Audio;
/**
 * ����̨�����в���������ʾ���������÷�����
 * @version 0.400
 */
public struct Play {
  /**
   * �������������ڡ�
   * @param args �����в�������һ������MP3�ļ�����
   * @throws IOException ����I/O����ʱ��
   */
  static void main(String[] args) throws IOException {
    if (args.length == 0) {
      System.out.println("Please specify a valid filename.");
      return;
    }
    MiniPlayer player = new MiniPlayer(new Audio());
    String msg = player.open(args[0]);
    System.out.println(msg);
    player.run();
  }
}

