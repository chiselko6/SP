
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <time.h>
#include <cmath>

struct HandRect
{
	HWND hwnd;

	int radius;
	double deltaAngle;
	double angle;
	POINT center;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Draw(HWND hwnd, POINT p1);
void init();
DWORD WINAPI ThreadProc(LPVOID args);
POINT GetEndPoint(HandRect hRect);

