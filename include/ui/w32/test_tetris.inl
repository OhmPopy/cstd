#if 0
C���Զ���˹���飨���װ棩

������˹����ȫ���û�ͼ����ʵ�ַ���Ļ��ƣ�û��һ����Դ�����Դ�����С�ɣ���������������Ӻã�Ҳ��10K��ɡ��ǳ�С�ɵĶ���˹���顣

���˼�룺
1������Ϸ���򻮷�Ϊ18��*10�е����̣�����һ�������͵Ķ�ά����������Ա�ʾ�����ϸ����ط��Ƿ��з��顣
2����4�����������ֲ�ͬ��״�ķ��飬һ����������Ϸ�����У��ͰѶ�Ӧ�Ĳ����Ͷ�ά���������Ϊ�棬��ʾ�÷����Ѿ��з����ˡ�
3�����������㷽���ƶ��Ƿ����ײ���жϡ�
4�������Ѿ�������һ��СBUG��
5��ѹ�����е��ļ���δ���޸ĵ�Դ���룬�˴��Ĵ���Ϊ���¡�
6���������Ϊ�ı���״����Ϊֱ���䵽�ײ���p��Ϊ��ͣ������Pause���� ��ǩ�� <��>

����Ƭ��

#endif

#include "cstd.h"
#include <Windows.h> //Ϊ��ʹ��API����
#include <time.h>	//Ϊ��ʹ�ö�ʱ��
#include <stdlib.h>  //Ϊ��ʹ�������

#define BLOCKWIDTH		20  //���������С
#define NUMLINEBLOCKS	18  //����
#define NUMCOLUMNBLOCKS	10  //����
#define ID_TIMER		1	//��ʱ��ID
#define BLOCKSTYLES		(sizeof (Blocks) / sizeof (Blocks[0]))  //�����������

//��Ϸ�������񶥵㲼��ֵ������÷����Ƿ��з���
BOOL	GameClient[NUMCOLUMNBLOCKS][NUMLINEBLOCKS];
static int		F, S, cF, cS;	//�������ͼ�ζ�Ӧ�ĵ�һ����γ
static int		Score;  //�÷�

//�����������״���Ե��ʾ
struct  
{
	POINT	pt[4];
}
Blocks[][4] = 
{
	//��7
	0, 0, 1, 0, 1, 1, 1, 2,  2, 0, 0, 1, 1, 1, 2, 1,  1, 0, 1, 1, 1, 2, 2, 2,  0, 1, 1, 1, 2, 1, 0, 2,
	//��7
	1, 0, 2, 0, 1, 1, 1, 2,  0, 1, 1, 1, 2, 1, 2, 2,  1, 0, 1, 1, 0, 2, 1, 2,  0, 0, 0, 1, 1, 1, 2, 1,
	//1
	1, 0, 1, 1, 1, 2, 1, 3,  0, 1, 1, 1, 2, 1, 3, 1,  1, 0, 1, 1, 1, 2, 1, 3,  0, 1, 1, 1, 2, 1, 3, 1,
	//Z
	0, 0, 1, 0, 1, 1, 2, 1,  2, 0, 1, 1, 2, 1, 1, 2,  0, 0, 1, 0, 1, 1, 2, 1,  2, 0, 1, 1, 2, 1, 1, 2,
	//��Z
	1, 0, 2, 0, 0, 1, 1, 1,  1, 0, 1, 1, 2, 1, 2, 2,  1, 0, 2, 0, 0, 1, 1, 1,  1, 0, 1, 1, 2, 1, 2, 2,
	//����
	0, 0, 1, 0, 0, 1, 1, 1,  0, 0, 1, 0, 0, 1, 1, 1,  0, 0, 1, 0, 0, 1, 1, 1,  0, 0, 1, 0, 0, 1, 1, 1,
	//��ͷ
	1, 0, 0, 1, 1, 1, 2, 1,  0, 0, 0, 1, 1, 1, 0, 2,  0, 0, 1, 0, 2, 0, 1, 1,  1, 0, 0, 1, 1, 1, 1, 2
};

//�жϷ����Ƿ��������
BOOL CanDown(POINT pt[])
{
  int i;
	BOOL result = TRUE;
	//���������ڸ����ȼ���ָ��Ϊ�޷���
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = FALSE;
	for(i = 0; i < 4; ++i)
	{
		//����������³����µױ߽磬����FALSE�����߼����С��������һ���Ѿ��з��飬���ΪFALSE
		if(pt[i].y + 1 == NUMLINEBLOCKS || GameClient[pt[i].x][pt[i].y + 1])
		{
			result = FALSE;
			break;
		}
	}
	//�ָ��������ڸ���Ϊ�з���
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = TRUE;
	return result;
}


//�ж��Ƿ��������
BOOL CanLeft(POINT pt[])
{
  int i;
	BOOL result = TRUE;
	//���������ڸ����ȼ���ָ��Ϊ�޷���
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = FALSE;
	for(i = 0; i < 4; ++i)
	{
		//����������Ƴ�����߽߱磬����FALSE�����߼����С��������һ���Ѿ��з��飬���ΪFALSE
		if(!pt[i].x || GameClient[pt[i].x - 1][pt[i].y])
		{
			result = FALSE;
			break;
		}
	}
	//�ָ��������ڸ���Ϊ�з���
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = TRUE;
	return result;
}

//�ж��Ƿ��������
BOOL CanRight(POINT pt[])
{
  int i;
	BOOL result = TRUE;
	//���������ڸ����ȼ���ָ��Ϊ�޷���
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = FALSE;
	for(i = 0; i < 4; ++i)
	{
		//����������Ƴ�����߽߱磬����FALSE�����߼����С��������һ���Ѿ��з��飬���ΪFALSE
		if(pt[i].x + 1 == NUMCOLUMNBLOCKS || GameClient[pt[i].x + 1][pt[i].y])
		{
			result = FALSE;
			break;
		}
	}
	//�ָ��������ڸ���Ϊ�з���
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = TRUE;
	return result;
}

//�ж��Ƿ������ת
BOOL CanChange(POINT pt[])
{
  int i, k;
	BOOL result = TRUE;
	//���������ڸ����ȼ���ָ��Ϊ�޷���
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = FALSE;
	int t = (cS + 1) % 4;
	for(k = 0; k < 4; ++k)
	{
		int x = Blocks[cF][t].pt[k].x - Blocks[cF][cS].pt[k].x,
			y = Blocks[cF][t].pt[k].y - Blocks[cF][cS].pt[k].y;
		if(GameClient[pt[k].x + x][pt[k].y + y] ||  //�÷����Ѿ��з���
			pt[k].x + x > NUMCOLUMNBLOCKS - 1 ||  //x���곬Խ���ұ߽�
			pt[k].x + x < 0 ||   //x���곬Խ����߽�
			pt[k].y + y > NUMLINEBLOCKS - 1)  //y���곬Խ���µױ߽�
		{
			result = FALSE;
			break;
		}
	}

	//�ָ��������ڸ���Ϊ�з���
	for(i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = TRUE;
	return result;
}

//ʵ����ת
void Change(POINT pt[])
{
  int i;
	int t = (cS + 1) % 4;
	for(i = 0; i < 4; ++i)
	{
		int x = Blocks[cF][t].pt[i].x - Blocks[cF][cS].pt[i].x,
			y = Blocks[cF][t].pt[i].y - Blocks[cF][cS].pt[i].y;
		GameClient[pt[i].x][pt[i].y] = FALSE;
		pt[i].x += x;
		pt[i].y += y;
		GameClient[pt[i].x][pt[i].y] = TRUE;
	}
	cS = t;
}

//ʵ������
void Right(POINT pt[])
{
  int i, k;
	for(i = 0; i < 4; ++i)
	{
		GameClient[pt[i].x][pt[i].y] = FALSE;
		++pt[i].x;
	}
	for(k = 0; k < 4; ++k)
		GameClient[pt[k].x][pt[k].y] = TRUE;
}

//ʵ������
void Left(POINT pt[])
{
  int i, k;
	for(i = 0; i < 4; ++i)
	{
		GameClient[pt[i].x][pt[i].y] = FALSE;
		--pt[i].x;
	}
	for(k = 0; k < 4; ++k)
		GameClient[pt[k].x][pt[k].y] = TRUE;
}

//ʵ�ַ��������
void Down(POINT pt[])
{
  int i, k;
	for(i = 0; i < 4; ++i)
	{
		GameClient[pt[i].x][pt[i].y] = FALSE;
		++pt[i].y;
	}
	for(k = 0; k < 4; ++k)
		GameClient[pt[k].x][pt[k].y] = TRUE;
}

//������������ư�
inline unsigned Random(int n)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	srand(st.wMilliseconds);
	return rand() % n;
}

//���д����Լ���������
void DelSqure(HWND hwnd)
{
  int x, y;
	int line = 0, temp;
	for(x = NUMLINEBLOCKS - 1; x >= 0; --x)
	{
		BOOL result = TRUE;
		for(y = 0; y < NUMCOLUMNBLOCKS; ++y)
		{
			if(!GameClient[y][x])
			{
				result = FALSE;
				break;
			}
		}
		//�ж��Ƿ��������
		if(result)
		{
			temp = x;
			++line;
			while(x > 0)
			{
				for(y = 0; y < NUMCOLUMNBLOCKS; ++y)
				{
					GameClient[y][x] = GameClient[y][x - 1];
				}
				--x;
			}
			for(y = 0; y < NUMCOLUMNBLOCKS; ++y)
				GameClient[y][0] = FALSE;
			x = temp + 1;
		}
	}
	if(line)
		Score += (line - 1) * 2 + 1;
	InvalidateRect(hwnd, NULL, TRUE);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT		TericsBorder[] = {-1, -1, 
		NUMCOLUMNBLOCKS * BLOCKWIDTH + 1, NUMLINEBLOCKS * BLOCKWIDTH + 1};
	HDC			hdc;
	PAINTSTRUCT	ps;
	TEXTMETRIC	tm;
	TCHAR		szNextTerics[] = TEXT("��һ����"), szSCore[] = TEXT("�÷֣�");
	static TCHAR szBufferScore[5];
	static int	cxChar, cyChar;
	static POINT Block[4], NextBlock[4];
	int			x, y;
  int i;
	static BOOL		pause = FALSE;  //��ͣ

	switch(message)
	{
	case WM_CREATE:
    {
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth * 2;
		cyChar = tm.tmExternalLeading + tm.tmHeight;
		SetTimer(hwnd, ID_TIMER, 600, NULL);
		//��ʼ����һ�����ֵķ���
		cS = Random(4);
		cF = Random(BLOCKSTYLES);
		for(i = 0; i < 4; ++i)
		{
			Block[i].x = Blocks[cF][cS].pt[i].x + 4;
			Block[i].y = Blocks[cF][cS].pt[i].y;
			GameClient[Block[i].x][Block[i].y] = TRUE;
		}
		S = Random(4);
		F = Random(BLOCKSTYLES);
		for(i = 0; i < 4; ++i)
		{
			NextBlock[i].x = Blocks[F][S].pt[i].x;
			NextBlock[i].y = Blocks[F][S].pt[i].y;
		}

		ReleaseDC(hwnd, hdc);
		return 0;
    }
	case WM_TIMER:
    {
		if(pause) return 0;
		if(CanDown(Block))
		{
			Down(Block);
		}
		//�������ƣ���Ҫ���������жϣ���Ϸ�����������Ҫ������һ����ʾ���͵�ǰ��ʾ�ķ���
		else
		{
			DelSqure(hwnd);
			for(i = 0; i < 4; ++i)
			{
				Block[i].x = NextBlock[i].x + 4;
				Block[i].y = NextBlock[i].y;
				if(GameClient[Block[i].x][Block[i].y])
				{
					KillTimer(hwnd, ID_TIMER);
				}
				else
					GameClient[Block[i].x][Block[i].y] = TRUE;
			}
			cS = S;  cF = F;
			S = Random(4);
			F = Random(BLOCKSTYLES);
			for(i = 0; i < 4; ++i)
			{
				NextBlock[i].x = Blocks[F][S].pt[i].x;
				NextBlock[i].y = Blocks[F][S].pt[i].y;
			}
		}
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
    }

	case WM_KEYDOWN:
		if(pause && wParam != VK_PAUSE) return 0;
		switch(wParam)
		{
		case VK_LEFT:
			if(CanLeft(Block))
				Left(Block);
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		case VK_RIGHT:
			if(CanRight(Block))
				Right(Block);
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		case VK_UP:
			if(CanChange(Block))
				Change(Block);
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		case VK_DOWN:
			while(CanDown(Block))
				Down(Block);
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case VK_PAUSE:
			pause = !pause;
			break;

		default:
			break;
		}
		return 0;

	case WM_CHAR:
		if(wParam == 'p')
			pause = !pause;
		else if(wParam == 'r')
		{
			Score = 0;
			for(x = 0; x < NUMCOLUMNBLOCKS; ++x)
			{
				for(y = 0; y < NUMLINEBLOCKS; ++y)
					GameClient[x][y] = FALSE;
			}
			cS = Random(4);
			cF = Random(BLOCKSTYLES);
			for(i = 0; i < 4; ++i)
			{
				Block[i].x = Blocks[cF][cS].pt[i].x + 4;
				Block[i].y = Blocks[cF][cS].pt[i].y;
				GameClient[Block[i].x][Block[i].y] = TRUE;
			}
			S = Random(4);
			F = Random(BLOCKSTYLES);
			for(i = 0; i < 4; ++i)
			{
				NextBlock[i].x = Blocks[F][S].pt[i].x;
				NextBlock[i].y = Blocks[F][S].pt[i].y;
			}
			pause = FALSE;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		return 0;
		
	case WM_PAINT:
		//if(pause) return 0;
		hdc = BeginPaint(hwnd, &ps);
		SetViewportOrgEx(hdc, BLOCKWIDTH, BLOCKWIDTH, NULL);
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		SelectObject(hdc, GetStockObject(BLACK_PEN));

		//������˹������Ϸ�ı߿�
		Rectangle(hdc, TericsBorder[0].x, TericsBorder[0].y,
			TericsBorder[1].x, TericsBorder[1].y);
		//�������һ�����ַ���
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, 0, szNextTerics, lstrlen(szNextTerics));

		//������÷֡��ַ���
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 5 * BLOCKWIDTH,
			szSCore, lstrlen(szSCore));
		//
		SetTextAlign(hdc, TA_RIGHT | TA_TOP);
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH + 3 * cxChar, 2 * cyChar + 5 * BLOCKWIDTH,
			szBufferScore, wsprintf(szBufferScore, TEXT("%d"), Score));
		SetTextAlign(hdc, TA_LEFT | TA_TOP);

		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		SelectObject(hdc, GetStockObject(WHITE_PEN));
		//��ʾ��Ϸ���ķ���
		for(x = 0; x < NUMCOLUMNBLOCKS; ++x)
		{
			for(y = 0; y < NUMLINEBLOCKS; ++y)
			{
				if(GameClient[x][y])
				{
					Rectangle(hdc, x * BLOCKWIDTH, y * BLOCKWIDTH,
						(x + 1) * BLOCKWIDTH, (y + 1) * BLOCKWIDTH);
				}
			}
		}
		//��ʾ��һ����������ķ���
		for(i = 0; i < 4; ++i)
		{
			Rectangle(hdc, (NextBlock[i].x + NUMCOLUMNBLOCKS + 2) * BLOCKWIDTH, NextBlock[i].y * BLOCKWIDTH + cyChar,
				(NextBlock[i].x + NUMCOLUMNBLOCKS + 3) * BLOCKWIDTH, (NextBlock[i].y + 1) * BLOCKWIDTH + cyChar);
		}

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


int test_tetris()
{
  HINSTANCE hInstance=0;
  HINSTANCE hPrevInstance=0;
  LPSTR lpCmdLine=0;
  int nCmdShow=0;
  WNDCLASS wndcls;
  TCHAR szClassName[] = TEXT("Terics"),
		  szWindowName[] = TEXT("Aka's Terics");
  static POINT Block[4];
  
  wndcls.cbClsExtra		= 0;
  wndcls.cbWndExtra		= 0;
  wndcls.hbrBackground	= static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
  wndcls.hCursor			= LoadCursor(hInstance, IDC_ARROW);
  wndcls.hIcon			= LoadIcon(hInstance, IDI_APPLICATION);
  wndcls.hInstance		= hInstance;
  wndcls.lpfnWndProc		= WndProc;
  wndcls.lpszClassName	= szClassName;
  wndcls.lpszMenuName		= NULL;
  wndcls.style			= CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wndcls);
  
  HWND hwnd = CreateWindow(szClassName, szWindowName, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
    CW_USEDEFAULT, CW_USEDEFAULT, (NUMCOLUMNBLOCKS + 10) * BLOCKWIDTH,
    (NUMLINEBLOCKS + 3) * BLOCKWIDTH,
    NULL, NULL, hInstance, NULL);
  ShowWindow(hwnd, SW_SHOWNORMAL);
  UpdateWindow(hwnd);
  
  MSG msg;
  while(GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

