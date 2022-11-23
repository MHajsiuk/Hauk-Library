#pragma once

#ifndef __HAUK_MICROSOFT_WIN32_GUI_CWINDOWCLASS_H_

#define __HAUK_MICROSOFT_WIN32_GUI_CWINDOWCLASS_H_

#include <Windows.h>

#define CWC_MAX_STRING_LENGTH	255

class cWindowClass
{
public:
	cWindowClass();

	// Overloaded constructor using WNDCLASS struct to intialise
	cWindowClass(WNDCLASS *psWindowClass);

	// Getter and Setter for single member values.
	BOOL GetStyle(UINT *puStyle);
	BOOL SetStyle(UINT uStyle);

	BOOL GetClassExtraMemory(INT *pnClassExtraMemory);
	BOOL SetClassExtraMemory(INT nClassExtraMemory);


	BOOL GetWindowExtraMemory(INT *pnWindowExtraMemory);
	BOOL SetWindowExtraMemory(INT nWindowExtraMemory);

	BOOL GetInstanceHandle(HINSTANCE *phInstance);
	BOOL SetInstanceHandle(HINSTANCE hInstance);

	BOOL GetIconHandle(HICON *phIcon);
	BOOL SetIconHandle(HICON hIcon);

	BOOL GetCursorHandle(HCURSOR *phCursor);
	BOOL SetCursorHandle(HCURSOR hCursor);

	BOOL GetBackgroundHandle(HBRUSH *phbrBackground);
	BOOL SetBackgroundHandle(HBRUSH hbrBackground);

	BOOL GetMenuName(PWCHAR *szMenuName);
	BOOL SetMenuName(WCHAR CONST *szMenuName);

	BOOL WCGetClassName(PWCHAR *szClassName);
	BOOL SetClassName(WCHAR CONST *szClassName);

	// Functions
	BOOL RegisterWindowClass();
	BOOL UnRegisterWindowClass();
	BOOL SetWindowProcedure(WNDPROC WindowProcedure);

	BOOL bIsRegisted;

private:
	WNDCLASS sPrivateWindowClass;
	ATOM aPrivateWindowClassAtom;


};

#endif