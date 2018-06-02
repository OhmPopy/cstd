int ctrl_iszoomed(ctrl_t* c)
{
  return 0;
}
static int ctrl_hittest(ctrl_t* c, int x, int y)
{
  //myScreenToClient(hwnd, &x, &y);
  if (iPtInRect(&c->rc, x, y)) {
    IRECT rcc = c->rc;
    int CaptionHeight;
    int bw = getcxdlgframe();
    IRECT bd = iRECT(bw, bw, bw, bw);
    //ctrl_get_rect(c, c->rc, 0, &rcc, 0);
    if (y < rcc.t + bd.t) {
      if (x < rcc.l + bd.l) {
        return HTTOPLEFT;
      }
      if (x > rcc.r - bd.r) {
        return HTTOPRIGHT;
      }
      return HTTOP;
    }
    else if (y > rcc.b - bd.b) {
      if (x < rcc.l + bd.l) {
        return HTBOTTOMLEFT;
      }
      if (x > rcc.r - bd.r) {
        return HTBOTTOMRIGHT;
      }
      return HTBOTTOM;
    }
    if (x < rcc.l + bd.l) {
      return HTLEFT;
    }
    if (x > rcc.r - bd.r) {
      return HTRIGHT;
    }
    CaptionHeight = getcycaption();
    if (y < rcc.t + CaptionHeight) {
      return HTCAPTION;
    }
    return HTCLIENT;
  }
  return HTNOWHERE;
}
int ctrl_has_boder(const ctrl_t* c)
{
  return c->style & WS_BORDER && !(c->style & WS_MAXIMIZE);
}
int ctrl_has_cycaption(const ctrl_t* c)
{
  return c->style & WS_DLGFRAME;
}
static int frame_cmd_proc_max(frame_t* s, ctrl_t* c)
{
  ctrl_modify_style(c, 0, 0, WS_MAXIMIZE);
  return 0;
}
static int frame_hitrect(const frame_t* s, const ctrl_t* c, IRECT rect, xpos_t* prc)
{
  IRECT rc = rect;
  int has_cycaption = ctrl_has_cycaption(c);
  int has_bordr = ctrl_has_boder(c);
  //int cxicon = 16;
  //int cxsize = GetSystemMetrics(SM_CXSIZE);
  //int cysize = GetSystemMetrics(SM_CYSIZE);
  //int cxborder = GetSystemMetrics(SM_CXBORDER);
  //int cyborder = GetSystemMetrics(SM_CYBORDER);
  int cxdlgframe = getcxdlgframe();
  int cycaption = getcycaption();
  int d = has_cycaption ? cycaption : 0, b = has_bordr ? cxdlgframe : 0, cc = 0, t = 2, tt;
  memset(prc, 0, sizeof(xpos_t)*HITRECTNUM);
  if (has_bordr) {
    rc = rect;
    t = b;
    tt = t;
    //iRectOffset(rc, -t, -t, t, t, &rc);
    iRectCutT(&rc, tt, 0, &prc[HTTOP].rc);
    iRectCutT(&rc, -tt, 0, &prc[HTBOTTOM].rc);
    iRectCutL(&rc, tt, 0, &prc[HTLEFT].rc);
    iRectCutL(&rc, -tt, 0, &prc[HTRIGHT].rc);
    iRectCutL(&prc[HTTOP].rc, tt, 0, &prc[HTTOPLEFT].rc);
    iRectCutL(&prc[HTTOP].rc, -tt, 0, &prc[HTTOPRIGHT].rc);
    iRectCutL(&prc[HTBOTTOM].rc, tt, 0, &prc[HTBOTTOMLEFT].rc);
    iRectCutL(&prc[HTBOTTOM].rc, -tt, 0, &prc[HTBOTTOMRIGHT].rc);
    iRectOffset(&rc, b, b, -b, -b);
  }
  prc[HTCLOSE].z = prc[HTMAXBUTTON].z = prc[HTMINBUTTON].z = prc[HTSYSMENU].z = 1;
  if (d > 0) {
    t = 2;
    rc = rect;
    iRectOffset(&rc, b, b, -b, -b);
    iRectCutT(&rc, d, 0, &prc[HTCAPTION].rc);
    prc[HTCLIENT].rc = rc;
    rc = prc[HTCAPTION].rc;
    iRectOffset(&rc, t, t, -t, -1);
    //c = RCH(&rc);
    //iRectCutL(&rc, c, 0, &prc[HTSYSMENU].rc);
    iRectAlign(rc, 16, 16, DT_LEFT | DT_VCENTER, &prc[HTSYSMENU].rc);
    cc = RCH(&rc) + 2;
    iRectCutL(&rc, -cc, 0, &prc[HTCLOSE].rc);
    iRectCutL(&rc, -2, 0, &prc[HTMAXBUTTON].rc);
    iRectCutL(&rc, -cc, 0, &prc[HTMAXBUTTON].rc);
    iRectCutL(&rc, -cc, 0, &prc[HTMINBUTTON].rc);
  }
  else {
    prc[HTCLIENT].rc = rc;
  }
  return 0;
}
static int frame_hittest(const frame_t* s, const ctrl_t* c, int x, int y)
{
  int i = 0, ret = -1, z = -1, n = HITRECTNUM;
  xpos_t pos[HITRECTNUM];
  frame_hitrect(s, c, c->rc, pos);
  //x -= s->c->rc.l; y -= s->c->rc.t;
  for (; i < n; ++i) {
    if (pos[i].z > z && iPtInRect(&pos[i].rc, x, y)) {
      z = pos[i].z;
      ret = i;
    }
  }
  if (0) {
#define XWINHITDEF(a, b) if (ret==a) printf("%s\n", b);
    //XWINHITDEF(HITERROR,"��Ļ�����򴰿ڼ�ķֽ����ϣ���HTNOWHERE��ͬ����������DefWindowProc��������beep�Ĵ����λ�ã�")
    XWINHITDEF(HTTRANSPARENT, "�ڱ�ͬһ�̵߳���һ�����ڸ��ǵĴ�����")
    XWINHITDEF(HTBORDER, "���ܸı��С�Ĵ��ڵı߿�")
    XWINHITDEF(HTCAPTION, "������")
    XWINHITDEF(HTCLIENT, "�ͻ�����")
    XWINHITDEF(HTGROWBOX, "���Ÿ�(ͬHTSIZE)")
    XWINHITDEF(HTMENU, "�˵���")
    XWINHITDEF(HTHELP, "������ť")
    XWINHITDEF(HTMAXBUTTON, "��󻯰�ť")
    XWINHITDEF(HTMINBUTTON, "��С����ť")
    XWINHITDEF(HTREDUCE, "��С����ť")
    XWINHITDEF(HTCLOSE, "�رհ�ť")
    XWINHITDEF(HTNOWHERE, "��Ļ�����򴰿ڼ�ķֽ�����")
    XWINHITDEF(HTLEFT, "���ڵ���߿�")
    XWINHITDEF(HTRIGHT, "���ڵ��ұ߿�")
    XWINHITDEF(HTSIZE, "���Ÿ�(ͬHTGROWBOX)")
    XWINHITDEF(HTTOP, "���ڵ��ϱ߿�")
    XWINHITDEF(HTBOTTOM, "�±߿�")
    XWINHITDEF(HTTOPLEFT, "�߿�����Ͻ�")
    XWINHITDEF(HTTOPRIGHT, "�߿�����Ͻ�")
    XWINHITDEF(HTBOTTOMLEFT, "�߿�����½�")
    XWINHITDEF(HTBOTTOMRIGHT, "�߿�����½�")
    XWINHITDEF(HTVSCROLL, "��ֱ������")
    XWINHITDEF(HTHSCROLL, "ˮƽ������")
    XWINHITDEF(HTZOOM, "��󻯰�ť")
    XWINHITDEF(HTSYSMENU, "ϵͳ�˵����Ӵ���Ĺرհ�ť��")
#undef XWINHITDEF
  }
  return ret;
}
static LRESULT frame_proc(frame_t* s, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  //IRECT rc = c->rc;
  IRECT rc1 = c->rc;
  int hit = 0, x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  int ret = 0;
  xpos_t pos[HITRECTNUM];
  //TraceMsg(hwnd, uMsg, wParam, lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    //s->has_cap = 1;
    //s->has_bdr = 1;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
#if 0
  case WM_NCHITTEST:
    myScreenToClient(hwnd, &x, &y);
    if (iPtInRect(&rc1, x, y)) {
      hit = frame_hittest(s, x, y);
      if (hit >= 0) {
        return hit;
      }
    }
    break;
#endif
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    ISIZE sz = font_text_size(c->fo, c->text, -1, 0);
    if (c->sub) {
      c->sub->ctrlproc(0, c->sub, WM_GETMINMAXINFO, 0, (LPARAM)mminfo);
    }
    mminfo->ptMinTrackSize.x += sz.w + sz.h;
    mminfo->ptMinTrackSize.y += sz.h + sz.h;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_NCLBUTTONUP:
  case WM_LBUTTONUP:
    if (s) {
      if (HTCLOSE == s->hit || HTMAXBUTTON == s->hit || HTMINBUTTON == s->hit) {
        hit = frame_hittest(s, c, x, y);
        if (s->hit == hit) {
          if (HTCLOSE == hit) {
            sendmsg(c, c->listener, WM_COMMAND, XCMD_CLOSE, (LPARAM)c);
          }
          if (HTMAXBUTTON == hit) {
            frame_cmd_proc_max(s, c);
          }
          if (HTMINBUTTON == hit) {
            //PostMessage(sys->hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
          }
        }
        force_redraw(c);
      }
      s->hit = 0;
    }
    break;
  case WM_LBUTTONDBLCLK:
    if (s) {
      hit = frame_hittest(s, c, x, y);
      if (HTCAPTION == hit) {
        frame_cmd_proc_max(s, c);
        force_redraw(c);
      }
    }
    break;
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc1, x, y)) {
      s->x = x, s->y = y;
      s->rc1 = c->rc;
      s->hit = hit = frame_hittest(s, c, x, y);
      ctrl_setcursor(c, hit);
      if (HTMAXBUTTON == hit || HTMINBUTTON == hit || HTCLOSE == hit) {
        force_redraw(c);
      }
      if (c->father) {
        ctrl_t* fa = c->father;
        ctrl_remove(c);
        ctrl_addsub_tail(fa, c);
      }
      force_redraw(c);
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_MOUSEMOVE:
    //myScreenToClient(hwnd, &x, &y);
    if (c->sys->ldown) {
      int hit = s->hit;
      if (!(c->style & WS_MAXIMIZE)) {
        IRECT rcoff = {0};
        int dx, dy, donul = 0;
        dx = x - s->x, dy = y - s->y;
        //s->hot = hit;
        switch (hit) {
        case HTCAPTION:
          rcoff.l = rcoff.r = dx;
          rcoff.t = rcoff.b = dy;
          break;
        case HTLEFT:
          rcoff.l = dx;
          break;
        case HTRIGHT:
          rcoff.r = dx;
          break;
        case HTTOP:
          rcoff.t = dy;
          break;
        case HTBOTTOM:
          rcoff.b = dy;
          break;
        case HTTOPLEFT:
          rcoff.t = dy;
          rcoff.l = dx;
          break;
        case HTTOPRIGHT:
          rcoff.t = dy;
          rcoff.r = dx;
          break;
        case HTBOTTOMLEFT:
          rcoff.b = dy;
          rcoff.l = dx;
          break;
        case HTBOTTOMRIGHT:
          rcoff.b = dy;
          rcoff.r = dx;
          break;
        default:
          donul = 1;
          hit = frame_hittest(s, c, x, y);
          break;
        }
        if (!donul) {
          rc1 = s->rc1;
          iRectAdd(&rc1, &rcoff);
          rc1 = ctrl_calc_rect_impl(c, rc1);
          ctrl_setrect(c, rc1);
          //xMoveWindow(hwnd, rc1, 1, 1);
          //printf("%d %d %d %d %d %d %s\n", rc1.l, rc1.t, rcoff.l, rcoff.t, rcoff.r, rcoff.b, c->text);
          //c->rc = rc1;
          //s->x = x;
          //s->y = y;
          force_redraw(c);
        }
        //printf("%d %d %d %d\n", x, y, rc.l, rc.t);
      }
      return 1;
    }
    else {
      hit = frame_hittest(s, c, x, y);
      if (ctrl_has_boder(c)) {
        ctrl_setcursor(c, hit);
      }
      if (s->hot != hit) {
        if (HTCLOSE == s->hot || HTMAXBUTTON == s->hot || HTMINBUTTON == s->hot) {
          force_redraw(c);
        }
        s->hot = hit;
        if (HTCLOSE == s->hot || HTMAXBUTTON == s->hot || HTMINBUTTON == s->hot) {
          force_redraw(c);
        }
      }
    }
    break;
  case WM_PAINT:
    frame_hitrect(s, c, c->rc, pos);
    if (c->recalcsize) {
      ctrl_setrect(c->sub, pos[HTCLIENT].rc);
    }
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int i;
      COLOR clr = 0;
      static const int g_hitbtn[3] = {HTCLOSE, HTMAXBUTTON, HTMINBUTTON};
      int tt[] = {IdClose, (c->style & WS_MAXIMIZE) ? IdRestore : IdMaximize, IdMinimize};
      IRECT rc1;
      IRECT rc2 = c->rc;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      rc1 = rc;
      rc2 = rc;
      frame_hitrect(s, c, rc, pos);
      //iRectOffset(&rc2, -c->rc.l, -c->rc.t, -c->rc.l, -c->rc.t);
      //printf("WM_PAINT\n");
      //frame_hitrect(rc2, s->rcs, s->has_cap, s->has_bdr && !s->ismax);
      if (1) {
        imdraw_border_syscolor(im, &rcclip, rc2, BDS_HIGH, ctrl_has_boder(c) ? BDM_ALL : 0);
        if (ctrl_has_cycaption(c)) {
          COLOR clr1 = ctrl_isfocus(c) ? _RGB(10, 36, 106) : _RGB(106, 106, 106);
          COLOR clr2 = ctrl_isfocus(c) ? _RGB(166, 202, 240) : _RGB(240, 240, 240);
          imdraw_rect_gradient(im, &rcclip, pos[HTCAPTION].rc, clr1, clr2, 1);
          for (i = 0; i < 3; ++i) {
            int pushed = s->hit == g_hitbtn[i];
            IRECT rc1 = pos[g_hitbtn[i]].rc;
            imdraw_border_syscolor(im, &rcclip, rc1, pushed ? BDS_SUNKEN : BDS_HIGH, BDM_ALL);
            rc1.l += 2 * pushed - 1;
            imdraw_menuimg(im, &rcclip, rc1, tt[i], Color_Black);
          }
        }
      }
      else {
        COLOR clrs[] = {Color_Blue, Color_Blue, Color_Blue};
        imdraw_border_n(im, &rcclip, rc2, Color_White, countof(clrs), clrs, clrs, clrs, clrs);
        if (ctrl_has_cycaption(c)) {
          //imdraw_rect_gradient(im, pos[HTCAPTION].rc, _RGB(10, 36, 106), _RGB(166, 202, 240), 1);
          imdraw_rect(im, &rcclip, pos[HTCAPTION].rc, Color_Green, 0, 0, 0);
          for (i = 0; i < 3; ++i) {
            int hoted = s->hot == g_hitbtn[i];
            int pushed = s->hit == g_hitbtn[i];
            IRECT rc1 = pos[g_hitbtn[i]].rc;
            imdraw_rect(im, &rcclip, rc1, pushed ? Color_Gray : (hoted ? Color_Khaki : Color_White), 0, 0, 0);
            rc1.l += 2 * pushed - 1;
            imdraw_menuimg(im, &rcclip, rc1, tt[i], Color_Black);
          }
        }
      }
      if (ctrl_has_cycaption(c)) {
        rc1 = pos[HTCAPTION].rc;
        rc1.l = pos[HTSYSMENU].rc.r;
        rcclip.r = pos[HTMINBUTTON].rc.l;
        imdraw_text(im, &rcclip, rc1, c->text, -1, c->fo, Color_White, 0, DT_VCENTER);
      }
      c->redraw = 0;
    }
    break;
  }
  return 0;
}
#if 0
CC_INLINE int frame_set(frame_t* s, ctrl_t* li, sys_t* sys, const char* text, IRECT rc)
{
  ctrl_set(frame_proc, s, s->c, li, text, sys);
  s->hit = 0;
  rc = ctrl_calc_rect_impl(s->c, rc);
  ctrl_setrect(s->c, rc);
  ctrl_setfocus(s->c);
  return 0;
}
#endif

