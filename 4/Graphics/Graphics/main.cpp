
#include "main.h"
#include "resource.h"

// The main window class name
static TCHAR szWindowClass[] = _T("Win32Project1");

// The string that appears in the application's title bar
static TCHAR szTitle[] = _T("Win32Project1 Title");

int WindowHeight = 500;
int WindowWidth = 500;

const double PI = 3.14159265;

HINSTANCE hInst;
HWND hWnd;
HWND hWndHands;
HWND hButtonStart;
HWND hButtonStop;

HandRect hRect1;
HandRect hRect2;

HANDLE thread1;
HANDLE thread2;

bool isRun;

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

	hWndHands = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
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

	hButtonStart = CreateWindow(L"button", NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 32, 32, hWnd, (HMENU)IDC_BUTTON_START, hInst, NULL);
	hButtonStop = CreateWindow(L"button", NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 40, 32, 32, hWnd, (HMENU)IDC_BUTTON_STOP, hInst, NULL);

	init();

	thread1 = CreateThread(NULL, 0, ThreadProc, &hRect1, CREATE_SUSPENDED, NULL);
	thread2 = CreateThread(NULL, 0, ThreadProc, &hRect2, CREATE_SUSPENDED, NULL);

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);
	//ShowWindow(hWndHands,
	//	nCmdShow);
	UpdateWindow(hWnd);
	//UpdateWindow(hWndHands);


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
	hRect1.radius = hRect2.radius = 150;
	hRect1.hwnd = hRect2.hwnd = hWnd;
	hRect1.deltaAngle = PI / 100 + 0.01;
	hRect2.deltaAngle = PI / 100;

	hRect1.angle = PI * 4 / 3 + PI;
	hRect2.angle = PI * 4 / 3;
	
	isRun = false;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

	static DRAWITEMSTRUCT* pdis;
	PAINTSTRUCT ps;

	switch (msg) {
	/*case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		break;*/
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_START:
		{
			ResumeThread(thread1);
			ResumeThread(thread2);
			isRun = true;
			break;
		}
		case IDC_BUTTON_STOP:
			if (!isRun)
				break;
			SuspendThread(thread1);
			SuspendThread(thread2);
			//DrawHands({ 150, 150 }, GetDC(hRect1.hwnd));
			isRun = false;
			break;
		}
		break;
	case WM_PAINT:
	{
		
		HDC hdc = BeginPaint(hWnd, &ps);
		Draw(hWnd, { 50, 50 }, hdc);
		//DrawHands({ 150, 150 }, hdc);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

DWORD WINAPI ThreadProc(LPVOID args)
{
	HandRect* hRect = (HandRect*)args;

	while (true)
	{
		hRect->angle += hRect->deltaAngle;

		Sleep(15);
		InvalidateRect(hRect->hwnd, 0, TRUE);
	}

	return 0;
}

void Draw(HWND hwnd, POINT p1, HDC hdc)
{
	PAINTSTRUCT ps;
	HPEN hPen;
	HBRUSH hBrush;

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
	hdc = GetDC(hRect1.hwnd);
	POINT handsCenter = { (int)bodyCenter.x, (int)(double)bodyCenter.y - (double)bodyHeight / 2 * 0.7 };
	DrawHands(handsCenter, hdc);
		// -----
		// legs
	hdc = GetDC(hWnd);
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

POINT GetEndPoint(HandRect hRect)
{
	POINT res = {
		(int)((double)hRect.center.x + (double)hRect.radius * cos(hRect.angle)),
		(int)((double)hRect.center.y + (double)hRect.radius * sin(hRect.angle))
	};
	return res;
}

void DrawHands(POINT handsCenter, HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 15, RGB(221, 243, 20)); SelectObject(hdc, hPen);
	//POINT handsCenter = { (int)bodyCenter.x, (int)(double)bodyCenter.y - (double)bodyHeight / 2 * 0.7 };
	hRect1.center = hRect2.center = handsCenter;
	MoveToEx(hdc, (int)handsCenter.x, (int)handsCenter.y, NULL);
	POINT endPoint = GetEndPoint(hRect1);
	LineTo(hdc, endPoint.x, endPoint.y);
	MoveToEx(hdc, (int)handsCenter.x, (int)handsCenter.y, NULL);
	endPoint = GetEndPoint(hRect2);
	LineTo(hdc, endPoint.x, endPoint.y);
	// flagpole
	int flagpoleWidth = 8;
	int flagpoleHeight = 100;
	POINT flagCenter = { endPoint.x, endPoint.y - flagpoleHeight / 2 };
	hPen = CreatePen(PS_SOLID, 2, RGB(21, 54, 183)); SelectObject(hdc, hPen);
	HBRUSH hBrush = CreateSolidBrush(RGB(21, 54, 183)); SelectObject(hdc, hBrush);
	Rectangle(hdc, (int)flagCenter.x - flagpoleWidth / 2, (int)flagCenter.y - flagpoleHeight / 2, (int)flagCenter.x + flagpoleWidth / 2, (int)flagCenter.y + flagpoleHeight / 2);
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
}