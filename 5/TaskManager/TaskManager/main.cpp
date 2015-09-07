
#include "main.h"
#include "resource.h"

// Global variables

// The main window class name
static TCHAR szWindowClass[] = _T("Win32Project1");

// The string that appears in the application's title bar
static TCHAR szTitle[] = _T("Win32Project1 Title");

int WindowHeight = 700;
int WindowWidth = 900;

int IDs[500];
int cnt;

HINSTANCE hInst;
HWND hWnd;
HWND hListBox1;
HWND hListBox2;
HMENU hPopupMenu;

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

	hListBox1 = CreateWindowEx(WS_EX_CLIENTEDGE,
		L"listbox",
		L"List box 1",
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
		5,
		5,
		400,
		500,
		hWnd,
		(HMENU)IDC_LISTBOX1,
		hInst,
		NULL);                          

	hListBox2 = CreateWindowEx(WS_EX_CLIENTEDGE,
		L"listbox",
		L"List box 1",
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
		450,
		5,
		400,
		500,
		hWnd,
		(HMENU)IDC_LISTBOX2,
		hInst,
		NULL);

	hPopupMenu = CreatePopupMenu();

	AppendMenu(hPopupMenu, MF_STRING, ID_IDLE, L"Idle ");
	AppendMenu(hPopupMenu, MF_STRING, ID_BELOW_NORMAL, L"Below Normal ");
	AppendMenu(hPopupMenu, MF_STRING, ID_NORMAL, L"Normal ");
	AppendMenu(hPopupMenu, MF_STRING, ID_ABOVE_NORMAL, L"Above Normal ");
	AppendMenu(hPopupMenu, MF_STRING, ID_HIGH, L"High");
	AppendMenu(hPopupMenu, MF_STRING, ID_REALTIME, L"Real time ");

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

	init2();

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//void init()
//{
//	// Get a snapshot of the thread li
//	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//	PROCESSENTRY32 pe;
//
//	if (!hSnapshot)
//		return;
//
//	SendDlgItemMessage(hListBox, IDC_LISTBOX1, LB_RESETCONTENT, NULL, NULL);
//
//	// Initialize size in structure
//	pe.dwSize = sizeof(pe);
//
//	for (int i = Process32First(hSnapshot, &pe); i; i =
//		Process32Next(hSnapshot, &pe))
//	{
//		HANDLE hModuleSnap = NULL;
//		MODULEENTRY32 me;
//
//		SendDlgItemMessage(hWnd, IDC_LISTBOX1, LB_ADDSTRING, NULL, (LPARAM) pe.szExeFile);
//
//		// Take a snapshot of all modules in the specified process
//		hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,
//			pe.th32ProcessID);
//
//		if (hModuleSnap == (HANDLE)-1)
//			return;
//
//		// Fill the size of the structure before using it
//		me.dwSize = sizeof(MODULEENTRY32);
//
//		// Walk the module list of the process, and find the module of
//		// interest. Then copy the information to the buffer pointed
//		// to by lpMe32 so that it can be returned to the caller
//		if (Module32First(hModuleSnap, &me))
//		{
//			do
//			{
//				//if (me.th32ModuleID == pe.th32ModuleID)
//				//{
//					SendDlgItemMessage(hWnd, IDC_LISTBOX1, LB_ADDSTRING, NULL, (LPARAM)me.szModule);
//					//pList->AddString(me.szExePath);
//					//break;
//				//}
//			} while (Module32Next(hModuleSnap, &me));
//		}
//
//	}
//
//	CloseHandle(hSnapshot); // Done with this snapshot. Free it
//}

VOID PrintModuleList(DWORD CONST dwProcessId)
{
	MODULEENTRY32 meModuleEntry;
	TCHAR szBuff[1024];
	DWORD dwTemp;
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(
		TH32CS_SNAPMODULE, dwProcessId);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return;
	}

	meModuleEntry.dwSize = sizeof(MODULEENTRY32);
	Module32First(hSnapshot, &meModuleEntry);
	do {
		SendDlgItemMessage(hWnd, IDC_LISTBOX2, LB_ADDSTRING, NULL, (LPARAM)meModuleEntry.szModule);
		/*wsprintf(szBuff, L"  ba: %08X, bs: %08X, %s\r\n",
			meModuleEntry.modBaseAddr, meModuleEntry.modBaseSize,
			meModuleEntry.szModule);*/
	} while (Module32Next(hSnapshot, &meModuleEntry));

	CloseHandle(hSnapshot);
}

VOID PrintProcessList()
{
	PROCESSENTRY32 peProcessEntry;
	TCHAR szBuff[1024];
	DWORD dwTemp;
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return;
	}

	peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &peProcessEntry);
	do {
		//wchar_t buf[100];
		//wcscpy_s(buf, peProcessEntry.szExeFile);
		//wcscpy_s(buf, L" : ");
		//wcscpy_s(buf, (wchar_t*) (GetThreadPriority(hSnapshot)));
		SendDlgItemMessage(hWnd, IDC_LISTBOX1, LB_ADDSTRING, NULL, (LPARAM)peProcessEntry.szExeFile);
		IDs[cnt++] = peProcessEntry.th32ProcessID;
		//PrintModuleList(peProcessEntry.th32ProcessID);
	} while (Process32Next(hSnapshot, &peProcessEntry));

	CloseHandle(hSnapshot);
}

void init2()
{
	cnt = 0;
	PrintProcessList();
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
	HANDLE hProcess;

	switch (message)
	{
	case WM_CONTEXTMENU:
	{
		int selectedItem = SendDlgItemMessage(hWnd, IDC_LISTBOX1, LB_GETCURSEL, NULL, NULL);

		if (selectedItem != LB_ERR)
		{
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, IDs[selectedItem]);
			DWORD prioritySelectedProcess = GetPriorityClass(hProcess);
			CheckMenuItem(hPopupMenu, ID_IDLE, MF_UNCHECKED);
			CheckMenuItem(hPopupMenu, ID_BELOW_NORMAL, MF_UNCHECKED);
			CheckMenuItem(hPopupMenu, ID_NORMAL, MF_UNCHECKED);
			CheckMenuItem(hPopupMenu, ID_ABOVE_NORMAL, MF_UNCHECKED);
			CheckMenuItem(hPopupMenu, ID_HIGH, MF_UNCHECKED);
			CheckMenuItem(hPopupMenu, ID_REALTIME, MF_UNCHECKED);
			switch (prioritySelectedProcess)
			{
			case IDLE_PRIORITY_CLASS:
				CheckMenuItem(hPopupMenu, ID_IDLE, MF_CHECKED);
				break;
			case BELOW_NORMAL_PRIORITY_CLASS:
				CheckMenuItem(hPopupMenu, ID_BELOW_NORMAL, MF_CHECKED);
				break;
			case NORMAL_PRIORITY_CLASS:
				CheckMenuItem(hPopupMenu, ID_NORMAL, MF_CHECKED);
				break;
			case ABOVE_NORMAL_PRIORITY_CLASS:
				CheckMenuItem(hPopupMenu, ID_ABOVE_NORMAL, MF_CHECKED);
				break;
			case HIGH_PRIORITY_CLASS:
				CheckMenuItem(hPopupMenu, ID_HIGH, MF_CHECKED);
				break;
			case REALTIME_PRIORITY_CLASS:
				CheckMenuItem(hPopupMenu, ID_REALTIME, MF_CHECKED);
				break;
			default:
				break;
			}
		}

		if (GetDlgItem(hWnd, IDC_LISTBOX1) == (HWND)wParam)
		{
			int xCursor = LOWORD(lParam);
			int yCursor = HIWORD(lParam);
			TrackPopupMenuEx(hPopupMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, xCursor, yCursor, hWnd, NULL);
		}
		break;
	}
	case WM_COMMAND:
	{
		int selectedItem = SendDlgItemMessage(hWnd, IDC_LISTBOX1, LB_GETCURSEL, NULL, NULL);
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
		}

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