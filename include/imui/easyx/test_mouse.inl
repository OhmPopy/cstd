// �������ƣ���������ʾ
// ���뻷����Visual C++ 6.0��EasyX 2011���ݰ�
// �����£�2009-11-23
//

#include <conio.h>

void test_mouse()
{
  // ��ʼ��ͼ�δ���
  initgraph(640, 480);
  
  MOUSEMSG m;		// ���������Ϣ
  
  while(true)
  {
    // ��ȡһ�������Ϣ
    m = GetMouseMsg();
    
    switch(m.uMsg)
    {
    case WM_MOUSEMOVE:
      // ����ƶ���ʱ�򻭺�ɫ��С��
      putpixel(m.x, m.y, RED);
      break;
      
    case WM_LBUTTONDOWN:
      // ����������ͬʱ������ Ctrl ��
      if (m.mkCtrl)
        // ��һ���󷽿�
        rectangle(m.x-10, m.y-10, m.x+10, m.y+10);
      else
        // ��һ��С����
        rectangle(m.x-5, m.y-5, m.x+5, m.y+5);
      break;
      
      case WM_RBUTTONUP:
        return;	// ������Ҽ��˳�����
    }
  }
  
  // �ر�ͼ�δ���
  closegraph();
}