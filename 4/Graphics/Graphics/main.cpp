
#include "main.h"
#include "resource.h"

// The main window class name
static TCHAR szWindowClass[] = _T("Win32Project1");

// The string that appears in the application's title bar
static TCHAR szTitle[] = _T("Win32Project1 Title");

int WindowHeight = 500;
int WindowWidth = 500;

HINSTANCE hInst;
HWND hWnd;
HWND hButtonRefresh;
HWND hButtonClear;

struct HandRect 
{
	HWND hwnd;
	int handRectWidth;
	int handRectHeight;
};

HandRect hRect1;
HandRect hRect2;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {

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

	hButtonRefresh = CreateWindow(L"button", NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 32, 32, hWnd, (HMENU)IDC_BUTTON_REFRESH, hInst, NULL);
	hButtonClear = CreateWindow(L"button", NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 32, 32, 32, hWnd, (HMENU)IDC_BUTTON_CLEAR, hInst, NULL);

	init();

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

void init()
{
	hRect1.handRectHeight = 50;
	hRect1.handRectWidth = 100;

	hRect2.handRectHeight = 50;
	hRect2.handRectWidth = 100;

	hRect1.hwnd = hRect2.hwnd = hWnd;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

	static DRAWITEMSTRUCT* pdis;

	switch (msg) {
	/*case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		break;*/
	case WM_DRAWITEM:
		pdis = (DRAWITEMSTRUCT*)lParam;
		switch (pdis->CtlID)
		{
		case IDC_BUTTON_REFRESH:
		{
			RECT rect = pdis->rcItem;
			DrawIconEx(pdis->hDC, (int) 0.5 * (rect.right - rect.left - 32), (int) 0.5 * (rect.bottom - rect.top - 32),
				(HICON)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)), 32, 32, 0, NULL, DI_NORMAL);
			break;
		}
		case IDC_BUTTON_CLEAR:
		{
			RECT rect = pdis->rcItem;
			DrawIconEx(pdis->hDC, (int) 0.5 * (rect.right - rect.left - 32), (int) 0.5 * (rect.bottom - rect.top - 32),
				(HICON)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON2)), 32, 32, 0, NULL, DI_NORMAL);
			break;
		}
		default:
			break;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_REFRESH:
		{
			Draw(hWnd, { 50, 50 });
			break;
		}
		case IDC_BUTTON_CLEAR:
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
			break;
		}
		break;
	case WM_PAINT:
		//Draw(hWnd, { 50, 50 });
		break;
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void Draw(HWND hwnd, POINT p1)
{
	PAINTSTRUCT ps;
	HPEN hPen;
	HBRUSH hBrush;
	HDC hdc = GetDC(hwnd);

	hPen = CreatePen(PS_SOLID, 1, RGB(221, 243, 20)); SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(RGB(221, 243, 20)); SelectObject(hdc, hBrush);
	// head
	int headRadius = 50;
	int xOffset = 100;
	int yOffset = 50;
	POINT headCenter = { (int)p1.x + xOffset + headRadius, (int)p1.y + yOffset + headRadius };
	Ellipse(hdc, headCenter.x - headRadius, headCenter.y - headRadius, headCenter.x + headRadius, headCenter.y + headRadius);
		// hair 
	const int hairsCount = 15;
	int hairRectWidth = headRadius * 0.1;
	int hairRectHeight = headRadius * 0.4;
	POINT pointsHair[hairsCount];
	pointsHair[0] = { (int)(double)headCenter.x - (double)headRadius * 0.65, (int)(double)headCenter.y - (double)headRadius * 0.4 };
	pointsHair[1] = { (int)pointsHair[0].x + hairRectWidth, (int) pointsHair[0].y - hairRectHeight};
	for (int i = 2; i < hairsCount; i++)
	{
		pointsHair[i] = { (int)pointsHair[i - 2].x + 2 * hairRectWidth, (int)pointsHair[i - 2].y };
	}
	hPen = CreatePen(PS_SOLID, 2, RGB(129, 90, 22)); SelectObject(hdc, hPen);
	Polyline(hdc, pointsHair, hairsCount);
		// ----
		// eyes
	int eyeHeight = 10;
	int eyeWidth = 20;
	POINT eyeCenter = { (int)headCenter.x - headRadius / 2, (int)headCenter.y - eyeHeight / 2 };
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); SelectObject(hdc, hPen);
	Arc(hdc, eyeCenter.x - eyeWidth / 2, eyeCenter.y - eyeHeight / 2, eyeCenter.x + eyeWidth / 2, eyeCenter.y + eyeHeight / 2, eyeCenter.x - eyeWidth / 2, eyeCenter.y, eyeCenter.x + eyeWidth / 2, eyeCenter.y);
	Arc(hdc, eyeCenter.x - eyeWidth / 2, eyeCenter.y - eyeHeight / 2, eyeCenter.x + eyeWidth / 2, eyeCenter.y + eyeHeight / 2, eyeCenter.x + eyeWidth / 2, eyeCenter.y, eyeCenter.x - eyeWidth / 2, eyeCenter.y);
	eyeCenter = { headCenter.x + headRadius / 2, headCenter.y - eyeHeight / 2 };
	Arc(hdc, eyeCenter.x - eyeWidth / 2, eyeCenter.y - eyeHeight / 2, eyeCenter.x + eyeWidth / 2, eyeCenter.y + eyeHeight / 2, eyeCenter.x - eyeWidth / 2, eyeCenter.y, eyeCenter.x + eyeWidth / 2, eyeCenter.y);
	Arc(hdc, eyeCenter.x - eyeWidth / 2, eyeCenter.y - eyeHeight / 2, eyeCenter.x + eyeWidth / 2, eyeCenter.y + eyeHeight / 2, eyeCenter.x + eyeWidth / 2, eyeCenter.y, eyeCenter.x - eyeWidth / 2, eyeCenter.y);
		// ----
		// nose
	int noseOffset = 5;
	POINT pointsNose[3] = { 
		{ (int) p1.x + xOffset + headRadius, (int) p1.y + yOffset + headRadius},
		{ (int)p1.x + xOffset + headRadius - noseOffset, (int)p1.y + yOffset + headRadius + 2 * noseOffset },
		{ (int)p1.x + xOffset + headRadius + noseOffset, (int)p1.y + yOffset + headRadius + 2 * noseOffset }
	};
	Polyline(hdc, pointsNose, 3);
		// ----
		// mouth
	int mouthRadius = (int)(double)headRadius * 0.45;
	POINT mouthCenter = headCenter;
	int NOTUSED = 5;
	Arc(hdc, (int)(double) mouthCenter.x - (double) mouthRadius * 1.42, (int)(double) mouthCenter.y - (double) mouthRadius * 1.42,
		(int)mouthCenter.x + mouthRadius * 1.42, (int)mouthCenter.y + mouthRadius * 1.42,
		(int)headCenter.x - 5, (int)headCenter.y + 5,
		(int)headCenter.x + 5, (int)headCenter.y + 5);
		// ----
	// ----
	// neck
	hPen = CreatePen(PS_SOLID, 1, RGB(221, 243, 20)); SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(RGB(221, 243, 20)); SelectObject(hdc, hBrush);
	int neckWidth = 15;
	int neckHeight = 20;
	// 4!!!
	Rectangle(hdc, (int)headCenter.x - neckWidth / 2, (int)headCenter.y + headRadius - 4,
		(int)headCenter.x + neckWidth / 2, (int)headCenter.y + headRadius + neckHeight);
	// ----
	// body
	int bodyWidth = 100;
	int bodyHeight = 160;
	// 4!!!!
	POINT bodyCenter = { (int)headCenter.x, (int)headCenter.y + headRadius + neckHeight + bodyHeight / 2 - 4 };
	Ellipse(hdc, (int)bodyCenter.x - bodyWidth / 2, (int)bodyCenter.y - bodyHeight / 2,
		(int)bodyCenter.x + bodyWidth / 2, (int)bodyCenter.y + bodyHeight / 2);
		// hands
	hPen = CreatePen(PS_SOLID, 15, RGB(221, 243, 20)); SelectObject(hdc, hPen);
	POINT handsCenter = { (int)bodyCenter.x, (int)(double)bodyCenter.y - (double)bodyHeight / 2 * 0.7 };
	//int handRectWidth = 100;
	//int handRectHeight = 50;
	MoveToEx(hdc, (int)handsCenter.x, (int)handsCenter.y, NULL);
	LineTo(hdc, (int)handsCenter.x - hRect1.handRectWidth, (int)handsCenter.y + hRect1.handRectHeight);
	MoveToEx(hdc, (int)handsCenter.x, (int)handsCenter.y, NULL);
	LineTo(hdc, (int)handsCenter.x + hRect2.handRectWidth, (int)handsCenter.y + hRect2.handRectHeight);
			// flagpole
	int flagpoleWidth = 8;
	int flagpoleHeight = 100;
	POINT flagCenter = { (int)handsCenter.x + hRect2.handRectWidth, (int)handsCenter.y + hRect2.handRectHeight - flagpoleHeight / 2 };
	hPen = CreatePen(PS_SOLID, 2, RGB(21, 54, 183)); SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(RGB(21, 54, 183)); SelectObject(hdc, hBrush);
	Rectangle(hdc, (int) flagCenter.x - flagpoleWidth / 2, (int) flagCenter.y - flagpoleHeight / 2, (int) flagCenter.x + flagpoleWidth / 2, (int) flagCenter.y + flagpoleHeight / 2);
				// flag
	int flagWidth = 60;
	int flagHeight = flagpoleHeight * 0.4;
	POINT pointsFlag[3] = {
		{ (int)flagCenter.x - flagpoleWidth / 2, (int)flagCenter.y - flagpoleHeight / 2 },
		{ (int)flagCenter.x - flagpoleWidth / 2 + flagWidth, (int)flagCenter.y - flagpoleHeight / 2 + flagHeight / 2 },
		{ (int)flagCenter.x - flagpoleWidth / 2, (int)flagCenter.y - flagpoleHeight / 2 + flagHeight }
	};
	hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(RGB(255, 0, 0)); SelectObject(hdc, hBrush);
	Polygon(hdc, pointsFlag, 3);
				// ---------
			// ----
		// -----
		// legs
	POINT legsCenter = { (int)bodyCenter.x, (int)bodyCenter.y + bodyHeight / 2 * 0.5 };
	int legRectWidth = 50;
	int legRectHeight = 80;
	hPen = CreatePen(PS_SOLID, 15, RGB(221, 243, 20)); SelectObject(hdc, hPen);
	MoveToEx(hdc, (int)legsCenter.x, (int)legsCenter.y, NULL);
	LineTo(hdc, (int)legsCenter.x - legRectWidth, (int)legsCenter.y + legRectHeight);
	MoveToEx(hdc, (int)legsCenter.x, (int)legsCenter.y, NULL);
	LineTo(hdc, (int)legsCenter.x + legRectWidth, (int)legsCenter.y + legRectHeight);
		// ----
	// ----

	DeleteObject(hPen);
	DeleteObject(hBrush);
	//EndPaint(hwnd, &ps);
}