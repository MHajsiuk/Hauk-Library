#pragma once

#ifndef __HAUK_MICROSOFT_WIN32_GUI_CWINDOW_H_

#define __HAUK_MICROSOFT_WIN32_GUI_CWINDOW_H_

#include <Windows.h>
#include "cWindowClass.h"
#include "cWindowProcedure.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------

	Windows Styles

	WS_BORDER - Supposed to be just a border nothing else but windows > 10 adds the title anyway.



-----------------------------------------------------------------------------------------------------------------------------------------------*/


typedef struct tagWINDOW_PARAMETERS
{
	// Parameters
	INT	nWindowLengthX;
	INT nWindowHeightY;
	INT nWindowPositionX;
	INT nWindowPositionY;

	DWORD dwWindowStyle;

	LPCWSTR szWindowName;
	LPCWSTR szClassName;

	HWND hParentWindow;
	HMENU hMenu;
	HINSTANCE hInstance;
	LPVOID lpParam;

}WINDOW_PARAMETERS, *PWINDOW_PARAMETERS;

static struct tagWINDOW_STYLE
{
	enum Styles
	{
		TitleBar = WS_CAPTION,
		Sizeable = WS_THICKFRAME,
		MinimiseButton = WS_MINIMIZEBOX,
		MaximiseButton = WS_MAXIMIZEBOX,
		SystemMenu = WS_SYSMENU
	};

}WINDOW_STYLE, WindowStyle;


/*------------------------------------------------------------------------------------------------------------------------------*/


class cWindow
{
public:
	cWindow();
	~cWindow();

	cWindowClass WindowClass;
	cWindowProcedure WindowProcedure;

	// Management Functions
	VOID Run();
	BOOL ProcessMessageQueue();
	BOOL GetWindowParameters(CREATESTRUCT *sWindowCreateParamters);
	BOOL GetWindowHandle(HWND *hWindowHandle);
	BOOL IsWindowCreated();
	BOOL IsWindowRunning();

	// Differnet CreateNewWindow
	BOOL CreateNewWindow();
	BOOL CreateNewWindow(DWORD dwWindowLengthX, DWORD dwWindowHeightY);
	BOOL CreateNewWindow(DWORD dwWindowLengthX, DWORD dwWindowHeightY, WCHAR *szWindowName);
	BOOL CreateNewWindow(DWORD dwWindowLengthX, DWORD dwWindowHeightY, DWORD dwWindowPositionX, DWORD dwWindowPositionY);
	BOOL CreateNewWindow(DWORD dwWindowLengthX, DWORD dwWindowHeightY, DWORD dwWindowPositionX, DWORD dwWindowPositionY, WCHAR *szWindowName);

	// Window Style Functions.
	BOOL AddWindowStyle(DWORD dwStyle);
	BOOL RemoveWindowStyle(DWORD dwStyle);
	BOOL GetWindowStyle(DWORD *pdwStyles);
	BOOL SetWindowStyle(DWORD dwStyle);

	// Window Size/Postion Functions.
	BOOL GetWindowSize(DWORD *pdwWindowLengthX, DWORD *pdwWindowHeightY);
	BOOL SetWindowSize(DWORD dwWindowLengthX, DWORD dwWindowHeightY);

	BOOL SetWindowClientSize(DWORD dwWindowLengthX, DWORD dwWindowHeightY);

	BOOL GetWindowPosition(DWORD *pdwPositionX, DWORD *pdwPositionY);
	BOOL SetWindowPosition(DWORD dwPositionX, DWORD dwPositionY);




private:
	WINDOW_PARAMETERS sPrivateWindowParameters;
	MSG sPrivateMesssage;
	HWND hPrivateWindowHandle;
	BOOL bIsWindowCreated;
protected:
	BOOL bIsWindowRunning;

};


#endif