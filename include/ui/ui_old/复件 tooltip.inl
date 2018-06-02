

//Tooltip������ʾ�ؼ���ʹ��

//���Ϊһ���ؼ�����һ��������ʾ�ؼ�How to Create a Tooltip for a Control

// �����ؼ���Tooltip
HWND CreateToolTip(HWND hwndTool, HWND hwnd, PTSTR pszText)
{
  TOOLINFO toolInfo = {0};
  HWND hwndTip = CreateWindowEx(0,
      TOOLTIPS_CLASS,
      NULL,
      WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      hwnd,
      NULL,
      0,
      NULL);

  if (!hwndTool || !hwndTip) {
    return (HWND)NULL;
  }

  toolInfo.cbSize = sizeof(TOOLINFO);
  toolInfo.hwnd = hwnd;
  toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
  toolInfo.uId = (UINT_PTR)hwndTool;
  toolInfo.lpszText = pszText;
  SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);

  return hwndTip;
}

//3.�ڶԻ���OnInitDialog()������Ӵ��룺
//1
//      CreateToolTip(IDOK, hwnd, _T("���ǰ�ť��ToolTip���"));
//4.�������У�����Ƶ��Ի���ť�ϣ���ͣ����ƶ�����ͼ���£�

//��δ���һ����������Ĺ�����ʾ�ؼ� How to Create a Tooltip for a Rectangular Area

//1.����һ�����ڶԻ���Ĺ���TestToolTip��
//2.�ڶԻ���ʵ���ļ�д�����´��룺

// ������������ToolTip
void CreateToolTipForRect(HWND hwnd)
{
  TOOLINFO ti = {0};
  HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwnd,
      NULL,
      0,
      NULL);

  SetWindowPos(hwndTT, HWND_TOPMOST, 0, 0, 0, 0,
      SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

  ti.cbSize = sizeof(TOOLINFO);
  ti.uFlags = TTF_SUBCLASS;
  ti.hwnd = hwnd;
  ti.hinst = 0;
  ti.lpszText = _T("�������ToolTip���");

  GetWindowRect(hwnd, &ti.rect);
  //GetClientRect(hwnd, &ti.rect);

  SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
}

// �����У���9�е�����WS_POPUP��񣬵�CreateWindowExһ��Tooltipʱ�����Ǿ���WS_POPUP��WS_EX_TOOLWINDOW���
// �����ڴ�����ʱ�����Ƿ�ָ����TTS_NOPREFIXָʾ�������һ��������ʱ��ʾ������ʾ�����ܹ�����ʾ�����������Ƿ��Ǵ��ڻ״̬��
// û����������ֻ�е����ߵ����������ǻ��ʱ��Ż���ʾ������ʾ�ؼ���������ʾ��
// TTS_ALWAYSTIP�������ֹϵͳ��&�ַ���һ���ַ�����ȥ�������һ��������ʾ�ؼ�û��TTS_NOPREFIX���
// ��ϵͳ�Զ�ȥ��&�ַ�����Ӧ�ó�����ͬһ���ַ�����Ϊ�˵���͹�����ʾ�ؼ��е��ı���
// 3.�ڶԻ���OnInitDialog()������Ӵ��룺
//
//      CreateToolTipForRect(hwnd);    //�Ի�����ʾ
//     CreateToolTipForRect(GetDlgItem(IDOK)->hwnd);    //��ȷ������ť��ʾ
//
// 4.�������У�����Ƶ��Ի����ϣ���ͣ����ƶ�����ͼ���£�
//
// ���ʵ�ָ��ٹ�����ʾ�ؼ� How to Implement Tracking Tooltips
// 1.����һ�����ڶԻ���Ĺ���TestToolTip��
// 2.�ڶԻ���ʵ���ļ�д�����´��룺

TOOLINFO g_toolItem = {0};
int g_TrackingMouse = 0;
HWND g_hwndTrackingTT = 0;

HWND CreateTrackingToolTip(HWND hwnd)
{
  HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
      WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT, 0, NULL, 0,  NULL);

  if (!hwndTT) {
    return NULL;
  }

  g_hwndTrackingTT = hwndTT;

  g_toolItem.cbSize = sizeof(TOOLINFO);
  g_toolItem.uFlags = TTF_TRACK | TTF_ABSOLUTE | TTF_SUBCLASS;
  g_toolItem.hwnd = 0;
  g_toolItem.hinst = 0;
  g_toolItem.lpszText = " ";
  g_toolItem.uId = (UINT_PTR)0;

  GetClientRect(hwnd, &g_toolItem.rect);
  g_toolItem.rect.right /= 2;
  g_toolItem.rect.bottom /= 2;
  SendMessage(g_hwndTrackingTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&g_toolItem);

  //SendMessage(hwndTT, TTM_SETMAXTIPWIDTH, 0, 300);//���3
  return hwndTT;
}

// 3.�ڶԻ���OnInitDialog()������Ӵ��룺
// 1
//      g_hwndTrackingTT = CreateTrackingToolTip(IDOK, hwnd, _T("ToolTip"));
// 4.��ӶԻ����WM_MOUSELEAVE�¼����������£�
//  Description: ����Ƴ����ڣ�ͣ��ToolTip
// void CTestToolTipDlg::OnMouseLeave()
// {
//     ::SendMessage(g_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&g_toolItem);
//     g_TrackingMouse = FALSE;
//     CDialog::OnMouseLeave();
// }
// 5.��ӶԻ����WM_MOUSEMOVE�¼����������£�
//  Description: ����ƶ���ToolTip������Ϣ
int tooltip_mouse(HWND hwnd, UINT msg, POINT pt)
{
  static int oldX, oldY;

  if (WM_MOUSELEAVE == msg) {
    SendMessage(g_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&g_toolItem);
    g_TrackingMouse = 0;
    return 0;
  }
  else if (WM_MOUSEMOVE == msg) {
    if (!g_TrackingMouse) {
      TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT)};
      tme.hwndTrack = hwnd;
      tme.dwFlags = TME_LEAVE;
      TrackMouseEvent(&tme);
      g_TrackingMouse = TRUE;
    }

    if (PtInRect(&g_toolItem.rect, pt)) {
      SendMessage(g_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&g_toolItem);
    }
    else {
      SendMessage(g_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&g_toolItem);
    }

    if ((pt.x != oldX) || (pt.y != oldY)) {
      char coords[12];
      oldX = pt.x, oldY = pt.y;
      _snprintf(coords, 12, _T("%d, %d"), pt.x, pt.y);
      g_toolItem.lpszText = coords;
      SendMessage(g_hwndTrackingTT, TTM_SETTOOLINFO, 0, (LPARAM)&g_toolItem);
      ClientToScreen(hwnd, &pt);
      SendMessage(g_hwndTrackingTT, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(pt.x + 10, pt.y - 20));
    }
  }

  return 0;
}
