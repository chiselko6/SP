
#include "main.h"
#include "resource.h"

// Global variables

// The main window class name
static TCHAR szWindowClass[] = _T("Win32Project1");

// The string that appears in the application's title bar
static TCHAR szTitle[] = _T("Win32Project1 Title");

// The string that is moved through the window
TCHAR fluidText[] = _T("This is fluid text");
int fluidTextLength = 100;
int fluidTextSize = 10;

// Start point of the fluidText message
const int LPXInitial = 500;
const int LPYInitial = 100;
int LPX = LPXInitial;
int LPY = LPYInitial;
bool XFlag = true;
bool YFlag = true;

// Timer's parameters
int TimerPeriod = 5;
bool TimerFlag = true;

// Initial window size
const int WindowHeight = 200;
const int WindowWidth = 500;

HINSTANCE hInst;
HWND hWnd;

// Forward declarations of functions included in this code module:
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	wcex.hCursor = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	//wcex.hbrBackground = CreateSolidBrush(RGB(0, 255, 179));

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

	menu = CreateMenu();
	popupMenu = CreatePopupMenu();
	AppendMenu(menu, MF_STRING | MF_POPUP, (UINT)popupMenu, L"Run");
	AppendMenu(popupMenu, MF_STRING, ID_START, L"Start");
	AppendMenu(popupMenu, MF_STRING, ID_STOP, L"Stop");
	SetMenu(hWnd, menu);

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

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, World!");

	switch (message)
	{
		//case WM_LBUTTONDOWN:
		//	// BEGIN NEW CODE
		//{
		//	char szFileName[MAX_PATH];
		//	HINSTANCE hInstance = GetModuleHandle(NULL);

		//	GetModuleFileName(hInstance, (LPWSTR)szFileName, MAX_PATH);
		//	MessageBox(hWnd, (LPCWSTR)szFileName, L"This program is:", MB_OK | MB_ICONINFORMATION);

		//}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// Here your application is laid out.
		// For this introduction, we just print out "Hello, World!"
		// in the top left corner.
		TextOut(hdc,
			LPX, LPY,
			fluidText, _tcslen(fluidText));

		// End application-specific layout section.

		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		LPX--;
		if (LPX <= -fluidTextLength)
		{
			LPX = LPXInitial;
		}
		if (TimerFlag)
			TimerPeriod--;
		else
			TimerPeriod++;
		if (TimerPeriod <= 0 || TimerPeriod >= 40)
		{
			if (TimerFlag)
				TimerPeriod++;
			else
				TimerPeriod--;
			TimerFlag = !TimerFlag;
		}
		KillTimer(hWnd, MY_TIMER);
		SetTimer(hWnd, MY_TIMER, TimerPeriod, NULL);
		InvalidateRect(hWnd, 0, true);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_START:
			SetTimer(hWnd, MY_TIMER, TimerPeriod, NULL);
			break;
		case ID_STOP:
			KillTimer(hWnd, MY_TIMER);
			break;
		}
		
		SetWindowText(hWnd, fluidText);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}