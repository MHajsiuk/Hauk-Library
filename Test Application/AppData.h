#pragma once

#include <Windows.h>

#define WM_TIMER_1	1
extern DWORD dwLineCount;

LRESULT CALLBACK DrawProc(HWND hWindowHandle, UINT uMessage, WPARAM wParam, LPARAM lParam);
