
#include "main.h"
#include "resource.h"

// Global variables

// The main window class name
static TCHAR szWindowClass[] = _T("Win32Project1");

// The string that appears in the application's title bar
static TCHAR szTitle[] = _T("Win32Project1 Title");

int WindowHeight = 200;
int WindowWidth = 500;

HINSTANCE hInst;
HWND hWnd;
HWND hEdit;
HWND hButtonAdd;
HWND hButtonClear;
HWND hButtonToRight;
HWND hButtonDelete;
HWND hListBox;

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

	hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		5, 5, 100, 20, hWnd, (HMENU)IDC_EDIT, hInst, NULL);

	hButtonAdd = CreateWindow(
		L"BUTTON",  
		L"Add",      
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
		5,         
		30,         
		100,       
		20,       
		hWnd,     
		(HMENU)IDC_BUTTON_ADD,      
		hInst,
		NULL);    

	hButtonClear = CreateWindow(
		L"BUTTON",  
		L"Clear",      
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  
		5,         
		55,         
		100,        
		20,        
		hWnd,     
		(HMENU)IDC_BUTTON_CLEAR,
		hInst,
		NULL);

	hButtonToRight = CreateWindow(
		L"BUTTON",  
		L"To right",      
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
		5,        
		80,         
		100,        
		20,        
		hWnd,     
		(HMENU)IDC_BUTTON_TO_RIGHT,
		hInst,
		NULL);

	hButtonDelete = CreateWindow(
		L"BUTTON",  
		L"Delete",     
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  
		5,         
		105,         
		100,        
		20,        
		hWnd,     
		(HMENU)IDC_BUTTON_DELETE,
		hInst,
		NULL);

	hListBox = CreateWindowEx(WS_EX_CLIENTEDGE,            
		L"listbox",                
		L"List box 1",              
		WS_CHILD | WS_VISIBLE,                      
		120,                      
		5,                       
		100,                     
		100,                    
		hWnd,                      
		(HMENU) IDC_LISTBOX1,
		hInst,                        
		NULL);                           //user defined info

	hListBox = CreateWindowEx(WS_EX_CLIENTEDGE,
		L"listbox",
		L"List box 2",
		WS_CHILD | WS_VISIBLE,
		230,
		5,
		100,
		100,
		hWnd,
		(HMENU)IDC_LISTBOX2,
		hInst,
		NULL);

	/*menu = CreateMenu();
	popupMenu = CreatePopupMenu();
	AppendMenu(menu, MF_STRING | MF_POPUP, (UINT)popupMenu, L"Run");
	AppendMenu(popupMenu, MF_STRING, ID_START, L"Start");
	AppendMenu(popupMenu, MF_STRING, ID_STOP, L"Stop");
	SetMenu(hWnd, menu);*/

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
		
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// End application-specific layout section.

		EndPaint(hWnd, &ps);
		break;
	
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_ADD:
		{
			// get text from edit:
			int len = GetWindowTextLength(GetDlgItem(hWnd, IDC_EDIT));
			if (len > 0)
			{
				int i;
				char* buf;

				buf = (char*)GlobalAlloc(GPTR, len + 1);
				GetDlgItemText(hWnd, IDC_EDIT, (LPWSTR)buf, len + 1);

				int index1 = SendDlgItemMessage(hWnd, IDC_LISTBOX1, LB_FINDSTRINGEXACT, -1, (LPARAM)buf);
				if (index1 == LB_ERR)
				{
					SendDlgItemMessage(hWnd, IDC_LISTBOX1, LB_ADDSTRING, NULL, (LPARAM)buf);
				}

			}
					
			break;
		}
		case IDC_BUTTON_CLEAR:
			SendDlgItemMessage(hWnd, IDC_LISTBOX1, LB_RESETCONTENT, NULL, NULL);
			SendDlgItemMessage(hWnd, IDC_LISTBOX2, LB_RESETCONTENT, NULL, NULL);
			break;
		case IDC_BUTTON_TO_RIGHT:
		{
			int selectedIndex1 = SendDlgItemMessage(hWnd, IDC_LISTBOX1, LB_GETCURSEL, NULL, NULL);
			if (selectedIndex1 == LB_ERR)
			{
				break;
			}
			char* buf = (char*)GlobalAlloc(GPTR, 255);
			SendDlgItemMessage(hWnd, IDC_LISTBOX1, LB_GETTEXT, selectedIndex1, (LPARAM)buf);
			int index2 = SendDlgItemMessage(hWnd, IDC_LISTBOX2, LB_FINDSTRINGEXACT, -1, (LPARAM)buf);
			if (index2 == LB_ERR)
			{
				SendDlgItemMessage(hWnd, IDC_LISTBOX2, LB_ADDSTRING, NULL, (LPARAM)buf);
			}
			break;
		}
		case IDC_BUTTON_DELETE:
		{
			int selectedIndex1 = SendDlgItemMessage(hWnd, IDC_LISTBOX1, LB_GETCURSEL, NULL, NULL);
			int selectedIndex2 = SendDlgItemMessage(hWnd, IDC_LISTBOX2, LB_GETCURSEL, NULL, NULL);
			if (selectedIndex1 != LB_ERR)
			{
				SendDlgItemMessage(hWnd, IDC_LISTBOX1, LB_DELETESTRING, selectedIndex1, NULL);
			}
			if (selectedIndex2 != LB_ERR)
			{
				SendDlgItemMessage(hWnd, IDC_LISTBOX2, LB_DELETESTRING, selectedIndex2, NULL);
			}
			break;
		}
		}

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

char* GetText()
{
	// get text from edit:
	int len = GetWindowTextLength(GetDlgItem(hWnd, IDC_EDIT));
	if (len > 0)
	{
		int i;
		char* buf;

		buf = (char*)GlobalAlloc(GPTR, len + 1);
		GetDlgItemText(hWnd, IDC_EDIT, (LPWSTR)buf, len + 1);

		MessageBox(hWnd, (LPCWSTR)buf, L"temp", NULL);

		//GlobalFree((HANDLE)buf);
		return buf;
	}
	return NULL;
}