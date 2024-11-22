#pragma once
#include "SugilMable.h"
#include "framework.h"
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <functional>

#define MAX_LOADSTRING 100

HWND CreateIngameWindow(HWND hParentWnd); // 함수 선언
ATOM MyRegisterIngameClass(HINSTANCE hInstance); // 함수 선언
LRESULT CALLBACK IngameWndProc(HWND, UINT, WPARAM, LPARAM);


class IngameMain {

	public:
		



	private:
		




};
