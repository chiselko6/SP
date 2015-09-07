
#include "main.h"
#include "resource.h"


// Global variables

// The main window class name
static TCHAR szWindowClass[] = _T("window1");

// The string that appears in the application's title bar
static TCHAR szTitle[] = _T("Window Interaction win1");

int WindowHeight = 200;
int WindowWidth = 300;


HINSTANCE hInst;
HWND hWnd;
HWND hAnotherWnd;
HWND hCheckBox;
UINT msgWnd;
HWND hRadio1;
HWND hRadio2;
COPYDATASTRUCT* cds;

int color;
int form;
const int temp = 1001;

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

	// radioGroup1
	CreateWindow(L"button", L"Green",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		10, 10, 80, 30, hWnd, (HMENU)ID_RADIO_1_1, hInstance, NULL);
	CreateWindow(L"button", L"Red",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		10, 30, 80, 30, hWnd, (HMENU)ID_RADIO_1_2, hInstance, NULL);
	CreateWindow(L"button", L"Blue",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		10, 50, 80, 30, hWnd, (HMENU)ID_RADIO_1_3, hInstance, NULL);
	
	// radioGroup2
	CreateWindow(L"button", L"Square",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		100, 10, 80, 30, hWnd, (HMENU)ID_RADIO_2_1, hInstance, NULL);
	CreateWindow(L"button", L"Romb",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		100, 30, 80, 30, hWnd, (HMENU)ID_RADIO_2_2, hInstance, NULL);
	CreateWindow(L"button", L"Circle",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		100, 50, 80, 30, hWnd, (HMENU)ID_RADIO_2_3, hInstance, NULL);
	CreateWindow(L"button", L"Star",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		100, 70, 80, 30, hWnd, (HMENU)ID_RADIO_2_4, hInstance, NULL);

	// checkBox
	hCheckBox = CreateWindow(
		L"button",
		L"Draw",
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		50,
		110,
		60,
		20,
		hWnd,
		(HMENU)ID_CHECKBOX,
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
	SendDlgItemMessage(hWnd, ID_RADIO_1_1, BM_SETCHECK, -1, NULL);
	SendDlgItemMessage(hWnd, ID_RADIO_2_1, BM_SETCHECK, -1, NULL);
	msgWnd = RegisterWindowMessage(L"interaction");
	//hAnotherWnd = FindWindow(L"window2", NULL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HANDLE hProcess;

	if (message == msgWnd)
	{
		bool toDraw = false;
		if (BST_CHECKED == SendMessage(
			hCheckBox,
			BM_GETCHECK, // message to send 
			(WPARAM)wParam, // not used; must be zero 
			(LPARAM)lParam // not used; must be zero 
			))
			toDraw = true;
		hAnotherWnd = FindWindow(L"window2", NULL);
		PostMessage(hAnotherWnd, msgWnd, color * temp + form, toDraw);
			//BroadcastSystemMessage(BSF_POSTMESSAGE, NULL, msgWnd, color * 1001 + form, toDraw);
		return 0;
	}

	switch (message)
	{
	case WM_COMMAND:
	{
		if (ID_RADIO_1_1 <= LOWORD(wParam) && LOWORD(wParam) <= ID_RADIO_1_3)
		{
			CheckRadioButton(hWnd, ID_RADIO_1_1, ID_RADIO_1_3, LOWORD(wParam));
			color = LOWORD(wParam);
		}
		if (ID_RADIO_2_1 <= LOWORD(wParam) && LOWORD(wParam) <= ID_RADIO_2_4)
		{
			CheckRadioButton(hWnd, ID_RADIO_2_1, ID_RADIO_2_4, LOWORD(wParam));
			form = LOWORD(wParam);
		}

		/*int selectedItem = SendDlgItemMessage(hWnd, IDC_LISTBOX1, LB_GETCURSEL, NULL, NULL);
		if (selectedItem != LB_ERR)
		{
			SendDlgItemMessage(hWnd, IDC_LISTBOX2, LB_RESETCONTENT, NULL, NULL);
			PrintModuleList(IDs[selectedItem]);
		}
		switch (LOWORD(wParam))
		{
		case ID_IDLE:
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, IDs[selectedItem]);
			SetPriorityClass(hProcess, IDLE_PRIORITY_CLASS);
			break;
		case ID_BELOW_NORMAL:
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, IDs[selectedItem]);
			SetPriorityClass(hProcess, BELOW_NORMAL_PRIORITY_CLASS);
			break;
		case ID_NORMAL:
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, IDs[selectedItem]);
			SetPriorityClass(hProcess, NORMAL_PRIORITY_CLASS);
			break;
		case ID_ABOVE_NORMAL:
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, IDs[selectedItem]);
			SetPriorityClass(hProcess, ABOVE_NORMAL_PRIORITY_CLASS);
			break;
		case ID_HIGH:
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, IDs[selectedItem]);
			SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
			break;
		case ID_REALTIME:
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, IDs[selectedItem]);
			SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);
			break;
		default:
			break;
		}*/

		break;
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