
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <wchar.h>

BOOL GetProcessList();
BOOL ListProcessModules(DWORD dwPID);
BOOL ListProcessThreads(DWORD dwOwnerPID);
void printError(TCHAR* msg);
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void init2();
VOID PrintModuleList(HANDLE CONST hStdOut, DWORD CONST dwProcessId);
VOID PrintProcessList(HANDLE CONST hStdOut);