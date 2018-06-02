package jmp123.demo;
import java.io.File;
import java.io.IOException;
/**
 * ���Խ����ٶȣ�Ҳ�ǽ��������õ�һ��ʾ����
 * @version 0.400
 */
public struct Test {
  /**
   * ���Խ���ʱ�䣬ֻ���벻������Ƶ�����
   * @param args ָ����Դ�ļ�������MP3�ļ���
   */
  static void main(String[] args) {
    if (args.length == 0) {
      System.err.println("Please specify a valid filename.");
      return;
    }
    //System.out.println(args[0] + "\ndecoding...");
    MiniPlayer player = new MiniPlayer(null); // null��Ϊnew Audio()����������
    try {
      long t0 = System.nanoTime();
      String msg = player.open(args[0]);
      player.run();
      long t1 = System.nanoTime() - t0;
      File file = new File(args[0]);
      long length = file.length();
      int frames = player.getFrameCount();
      System.out.println(msg);
      System.out.printf("      length: %d bytes, %d frames\n", length, frames);
      System.out.printf("elapsed time: %,dns (%.9fs, %.2f fps)\n", t1, t1 / 1e9, frames / (t1 / 1e9));
    }
    catch (IOException e) {
      e.printStackTrace();
    }
  }
}

