
#include "main.h"
#include "resource.h"


// Global variables

// The main window class name
static TCHAR szWindowClass[] = _T("window2");

// The string that appears in the application's title bar
static TCHAR szTitle[] = _T("Window Interaction win2");

int WindowHeight = 500;
int WindowWidth = 500;


HINSTANCE hInst;
HWND hWnd;
HWND hAnotherWnd;
UINT msgWnd;
COPYDATASTRUCT* cds;
const int temp = 1001;
POINT p;

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;
	HMENU menu, popupMenu;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance; // Store instance handle in our global variable

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_SYSMENU: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// TODO: // NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WindowWidth, WindowHeight,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	init();

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

void init()
{
	//hAnotherWnd = FindWindow(L"window1", NULL);
	msgWnd = RegisterWindowMessage(L"interaction");
}

void Draw(int color, int form)
{
	PAINTSTRUCT ps;
	HPEN hPen;
	HBRUSH hBrush;

	HDC hdc = GetDC(hWnd);
	COLORREF _color;

	switch (color)
	{
	case ID_RADIO_1_1:
	{
		_color = RGB(0, 255, 0);
		break;
	}case ID_RADIO_1_2:
	{
		_color = RGB(255, 0, 0);
		break;
	}case ID_RADIO_1_3:
	{
		_color = RGB(0, 0, 255);
		break;
	}
	}

	hPen = CreatePen(PS_SOLID, 1, _color); SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(_color); SelectObject(hdc, hBrush);
	// head
	int circleRadius = 10;
	int squareSize = 30;
	int rhombusSize = 30;
	int starSize = 30;
	switch (form)
	{
	case ID_RADIO_2_1:
	{
		Rectangle(hdc, p.x - squareSize / 2, p.y - squareSize / 2, p.x + squareSize / 2, p.y + squareSize / 2);
		break;
	}
	case ID_RADIO_2_2:
	{
		POINT points[4];
		points[0] = { p.x - rhombusSize / 2, p.y };
		points[1] = { p.x, p.y - rhombusSize / 2 };
		points[2] = { p.x + rhombusSize / 2, p.y };
		points[3] = { p.x, p.y + rhombusSize / 2 };
		Polygon(hdc, points, 4);
		break;
	}
	case ID_RADIO_2_3:
	{
		Ellipse(hdc, p.x - circleRadius, p.y - circleRadius, p.x + circleRadius, p.y + circleRadius);
		break;
	}
	case ID_RADIO_2_4:
	{
		POINT points[8];
		points[0] = { p.x - starSize / 2, p.y };
		points[1] = { p.x - starSize / 9, p.y - starSize / 9 };
		points[2] = { p.x, p.y - starSize / 2 };
		points[3] = { p.x + starSize / 9, p.y - starSize / 9 };
		points[4] = { p.x + starSize / 2, p.y };
		points[5] = { p.x + starSize / 9, p.y + starSize / 9 };
		points[6] = { p.x, p.y + starSize / 2 };
		points[7] = { p.x - starSize / 9, p.y + starSize / 9 };
		Polygon(hdc, points, 8);
		break;
	}
	}
	
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HANDLE hProcess;

	if (message == msgWnd)
	{

		int color = (int)wParam / temp;
		int form = (int)wParam % temp;
		if (lParam)
			Draw(color, form);
	}

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		hAnotherWnd = FindWindow(L"window1", NULL);
		p.x = LOWORD(lParam);
		p.y = HIWORD(lParam);
		PostMessage(hAnotherWnd, msgWnd, NULL, NULL);

		//BroadcastSystemMessage(BSF_POSTMESSAGE, NULL, msgWnd, NULL, NULL);
		break;
	}
	case WM_COMMAND:
	{
		
		break;
	}
	case WM_COPYDATA:
	{
		cds = (COPYDATASTRUCT*)lParam;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}