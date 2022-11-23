#pragma once

#ifndef __HAUK_MICROSOFT_WIN32_GUI_CANIMATEDWINDOW_H_

#define __HAUK_MICROSOFT_WIN32_GUI_CANIMATEDWINDOW_H_

#include <Windows.h>
#include "cWindow.h"
#include "cHighResolutionTimer.h"
#include "cDrawingBuffer.h"
#include "Video.h"


// This is used for fullscreen procedure and is made global until I can find away to pass info to the static function.
typedef struct tagFULLSCREEN_INFO
{
	BOOL bIsSet;
	BOOL bIsFullScreen;

	DWORD dwWindowSizeX;
	DWORD dwWindowSizeY;
	DWORD dwWindowPositionX;
	DWORD dwWindowPositionY;
	DWORD dwWindowStyle;
	DWORD dwWindowStyleEx;

	DWORD dwFullScreenWindowSizeX;
	DWORD dwFullScreenWindowSizeY;

	DWORD dwMonitorSizeX;
	DWORD dwMonitorSizeY;
	DWORD dwMonitorRefreshRate;

	RECT sDrawPosition;

}FULLSCREEN_INFO, *PFULLSCREEN_INFO;


extern FULLSCREEN_INFO sFullScreenInfo;


typedef struct tagANIMATED_WINDOW_INFO
{
	cDrawingBuffer *DrawingBufferObjectAddress;
	DWORD dwWindowLengthX;
	DWORD dwWindowHeightY;
	TIMER_INFO TimeInfo;

}ANIMATED_WINDOW_INFO, *PANIMATED_WINDOW_INFO;


class cAnimatedWindow : protected cWindow
{
public:
	cAnimatedWindow();
	~cAnimatedWindow();

	BOOL Animate();
	BOOL SetDrawFunction(WNDPROC);
	BOOL SetVideoMode(VIDEO_MODE_INFO);
	cHighResolutionTimer Timer;
	cDrawingBuffer DrawingBuffer;

private:
	// Keyboard Processing Routine
	static LRESULT CALLBACK PrivateSysKeyProc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);

	BOOL bPrivateIsFullscreen;
	VIDEO_MODE_INFO sPrivateVideoMode;
	ANIMATED_WINDOW_INFO sPrivateAnimatedWindowInfo;

};


#endif