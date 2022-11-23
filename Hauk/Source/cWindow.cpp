#include "Hauk.h"

#include <ShellScalingApi.h>		// header file and library to setup scalling mode by windows
#pragma comment(lib, "Shcore.lib")

cWindow::cWindow()
{
	// Setup generic defaults
	bIsWindowCreated = FALSE;
	bIsWindowRunning = FALSE;

	// Stop windows from messing with scaling our application.
	SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);

	WNDPROC WindowProcedureAddress = this->WindowProcedure.GetWindowProcedureAddress();
	WindowClass.SetWindowProcedure(WindowProcedureAddress);

	FillMemory(&this->sPrivateWindowParameters, sizeof(WINDOW_PARAMETERS), 0x00);
	FillMemory(&sPrivateMesssage, sizeof(MSG), 0x00);

	sPrivateWindowParameters.dwWindowStyle = WS_OVERLAPPEDWINDOW;
	sPrivateWindowParameters.hParentWindow = NULL;
	sPrivateWindowParameters.hMenu = NULL;

	WindowClass.WCGetClassName((PWCHAR *)&sPrivateWindowParameters.szClassName);
	WindowClass.GetInstanceHandle(&sPrivateWindowParameters.hInstance);

	sPrivateWindowParameters.lpParam = &this->WindowProcedure;

	sPrivateWindowParameters.nWindowHeightY = 480;
	sPrivateWindowParameters.nWindowLengthX = 640;
	sPrivateWindowParameters.nWindowPositionX = CW_USEDEFAULT;
	sPrivateWindowParameters.nWindowPositionY = CW_USEDEFAULT;
	sPrivateWindowParameters.szWindowName = L"Test Window";


}

cWindow::~cWindow()
{
}


BOOL cWindow::CreateNewWindow()
{

	BOOL bResult = FALSE;

	// Must have a class before we can create a window
	if (WindowClass.bIsRegisted != TRUE)
	{
		bResult = WindowClass.RegisterWindowClass();
		if (bResult == FALSE)
		{
			return FALSE;
		}
	}

	hPrivateWindowHandle = ::CreateWindow(
		sPrivateWindowParameters.szClassName,
		sPrivateWindowParameters.szWindowName,
		sPrivateWindowParameters.dwWindowStyle,
		sPrivateWindowParameters.nWindowPositionX,
		sPrivateWindowParameters.nWindowPositionY,
		sPrivateWindowParameters.nWindowLengthX,
		sPrivateWindowParameters.nWindowHeightY,
		sPrivateWindowParameters.hParentWindow,
		sPrivateWindowParameters.hMenu,
		sPrivateWindowParameters.hInstance,
		sPrivateWindowParameters.lpParam);

	if (hPrivateWindowHandle == NULL)
	{
		DWORD dwResult = GetLastError();
		return FALSE;
	}

	bIsWindowCreated = TRUE;

	if (sPrivateWindowParameters.dwWindowStyle == WS_BORDER)
	{
		this->SetWindowStyle(WS_BORDER);
	}

	ShowWindow(hPrivateWindowHandle, SW_SHOW);
	UpdateWindow(hPrivateWindowHandle);

	return TRUE;
}




BOOL cWindow::CreateNewWindow(DWORD dwWindowLengthX, DWORD dwWindowHeightY)
{
	this->SetWindowSize(dwWindowLengthX, dwWindowHeightY);

	this->CreateNewWindow();
	return TRUE;
}





BOOL cWindow::CreateNewWindow(DWORD dwWindowLengthX, DWORD dwWindowHeightY, WCHAR *szWindowName)
{
	WCHAR *szNewWindowName = (WCHAR *)malloc((wcslen(szWindowName) * sizeof(WCHAR)) + 2);
	if (*szNewWindowName == NULL)
	{
		return FALSE;
	}

	wcscpy(szNewWindowName, szWindowName);

	if (this->sPrivateWindowParameters.szWindowName != NULL)
	{
		free((VOID *)this->sPrivateWindowParameters.szWindowName);
	}

	this->sPrivateWindowParameters.szWindowName = szNewWindowName;
	this->SetWindowSize(dwWindowLengthX, dwWindowHeightY);
	this->CreateNewWindow();

	return TRUE;
}




BOOL cWindow::CreateNewWindow(DWORD dwWindowLengthX, DWORD dwWindowHeightY, DWORD dwWindowPositionX, DWORD dwWindowPositionY)
{
	this->SetWindowSize(dwWindowLengthX, dwWindowHeightY);
	this->SetWindowPosition(dwWindowPositionX, dwWindowPositionY);
	this->CreateNewWindow();

	return TRUE;
}




BOOL cWindow::CreateNewWindow(DWORD dwWindowLengthX, DWORD dwWindowHeightY, DWORD dwWindowPositionX, DWORD dwWindowPositionY, WCHAR *szWindowName)
{
	WCHAR *szNewWindowName = (WCHAR *)malloc((wcslen(szWindowName) * sizeof(WCHAR)) + 2);
	if (*szNewWindowName == NULL)
	{
		return FALSE;
	}

	wcscpy(szNewWindowName, szWindowName);

	if (this->sPrivateWindowParameters.szWindowName != NULL)
	{
		free((VOID *)this->sPrivateWindowParameters.szWindowName);
	}

	this->sPrivateWindowParameters.szWindowName = szNewWindowName;
	this->SetWindowSize(dwWindowLengthX, dwWindowHeightY);
	this->SetWindowPosition(dwWindowPositionX, dwWindowPositionY);
	this->CreateNewWindow();


	return TRUE;
}




BOOL cWindow::GetWindowPosition(DWORD *pdwPositionX, DWORD *pdwPositionY)
{
	*pdwPositionX = this->sPrivateWindowParameters.nWindowPositionX;
	*pdwPositionY = this->sPrivateWindowParameters.nWindowPositionY;

	return TRUE;
}





BOOL cWindow::SetWindowPosition(DWORD dwPositionX, DWORD dwPositionY)
{
	this->sPrivateWindowParameters.nWindowPositionX = dwPositionX;
	this->sPrivateWindowParameters.nWindowPositionY = dwPositionY;

	if (this->bIsWindowCreated == TRUE)
	{
		SetWindowPos(this->hPrivateWindowHandle, NULL,
			this->sPrivateWindowParameters.nWindowPositionX,
			this->sPrivateWindowParameters.nWindowPositionY,
			this->sPrivateWindowParameters.nWindowLengthX,
			this->sPrivateWindowParameters.nWindowHeightY,
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}

	return TRUE;
}






BOOL cWindow::GetWindowSize(DWORD *pdwWindowLengthX, DWORD *pdwWindowHeightY)
{
	*pdwWindowLengthX = this->sPrivateWindowParameters.nWindowLengthX;
	*pdwWindowHeightY = this->sPrivateWindowParameters.nWindowHeightY;

	return TRUE;
}




BOOL cWindow::SetWindowSize(DWORD dwWindowLengthX, DWORD dwWindowHeightY)
{
	sPrivateWindowParameters.nWindowLengthX = dwWindowLengthX;
	sPrivateWindowParameters.nWindowHeightY = dwWindowHeightY;

	if (bIsWindowCreated == TRUE)
	{
		SetWindowPos(this->hPrivateWindowHandle, NULL,
			this->sPrivateWindowParameters.nWindowPositionX,
			this->sPrivateWindowParameters.nWindowPositionY,
			this->sPrivateWindowParameters.nWindowLengthX,
			this->sPrivateWindowParameters.nWindowHeightY,
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}

	return TRUE;
}





BOOL cWindow::SetWindowClientSize(DWORD dwWindowLengthX, DWORD dwWindowHeightY)
{
	BOOL bMenu = FALSE;
	BOOL bResult = FALSE;
	DWORD dwWindowStyle = 0;
	DWORD dwResult = 0;
	RECT sWindowSize = { 0 };

	if (this->sPrivateWindowParameters.hMenu != NULL)
	{
		bMenu = TRUE;
	}

	this->GetWindowStyle(&dwWindowStyle);
	dwWindowStyle = WS_CAPTION;

	sWindowSize.bottom = dwWindowHeightY;
	sWindowSize.right = dwWindowLengthX;
	sWindowSize.top = 0;
	sWindowSize.left = 0;

	bResult = AdjustWindowRect(&sWindowSize, dwWindowStyle, bMenu);
	if (bResult == FALSE)
	{
		dwResult = GetLastError();
		return FALSE;
	}

	dwWindowLengthX = sWindowSize.right - sWindowSize.left;
	dwWindowHeightY = sWindowSize.bottom - sWindowSize.top;

	this->SetWindowSize(dwWindowLengthX, dwWindowHeightY);

	return TRUE;
}





BOOL cWindow::GetWindowStyle(DWORD *pdwStyle)
{
	if (bIsWindowCreated == TRUE)
	{
		sPrivateWindowParameters.dwWindowStyle = GetWindowLongPtr(hPrivateWindowHandle, GWL_STYLE);
	}

	*pdwStyle = sPrivateWindowParameters.dwWindowStyle;

	return TRUE;
}




BOOL cWindow::SetWindowStyle(DWORD dwStyle)
{

	this->sPrivateWindowParameters.dwWindowStyle = dwStyle;

	// If the window has been created we get the info directly from it.
	if (bIsWindowCreated == TRUE)
	{
		DWORD dwResult = SetWindowLongPtr(this->hPrivateWindowHandle, GWL_STYLE, this->sPrivateWindowParameters.dwWindowStyle);
		if (dwResult == 0)
		{
			dwResult = GetLastError();
			return FALSE;
		}
	}

	return TRUE;
}





BOOL cWindow::AddWindowStyle(DWORD dwStyle)
{
	DWORD dwCurrentStyle = 0;
	DWORD dwNewStyle = 0;

	this->GetWindowStyle(&dwCurrentStyle);

	switch (dwStyle)
	{
	case WindowStyle.TitleBar:
		dwNewStyle = dwCurrentStyle | WindowStyle.TitleBar;
		break;
	case WindowStyle.Sizeable:
		dwNewStyle = dwCurrentStyle | WindowStyle.Sizeable;
		break;
	case WindowStyle.MinimiseButton:
		dwNewStyle = dwCurrentStyle | WindowStyle.SystemMenu | WindowStyle.MinimiseButton;
		break;
	case WindowStyle.MaximiseButton:
		dwNewStyle = dwCurrentStyle | WindowStyle.SystemMenu | WindowStyle.MaximiseButton;
		break;
	case WindowStyle.SystemMenu:
		dwNewStyle = dwCurrentStyle | WindowStyle.SystemMenu;
		break;
	default:
		break;
	}

	if ((dwCurrentStyle & WS_VISIBLE) == TRUE)
	{
		dwNewStyle = dwNewStyle | WS_VISIBLE;
	}

	this->SetWindowStyle(dwNewStyle);

	return 0;
}




BOOL cWindow::RemoveWindowStyle(DWORD dwStyle)
{
	DWORD dwNewStyle = 0;
	DWORD dwCurrentStyle = 0;
	this->GetWindowStyle(&dwCurrentStyle);

	switch (dwStyle)
	{
	case WindowStyle.TitleBar:
		dwNewStyle = ~dwCurrentStyle;
		dwNewStyle = dwNewStyle | WindowStyle.TitleBar | WindowStyle.SystemMenu | WindowStyle.MaximiseButton | WindowStyle.MinimiseButton;
		dwNewStyle = ~dwNewStyle;

		break;

	case WindowStyle.Sizeable:
		dwNewStyle = ~dwCurrentStyle;
		dwNewStyle = dwNewStyle | WindowStyle.Sizeable;
		dwNewStyle = ~dwNewStyle;

		break;

	case WindowStyle.MinimiseButton:
		dwNewStyle = ~dwCurrentStyle;
		dwNewStyle = dwNewStyle | WindowStyle.MinimiseButton;
		dwNewStyle = ~dwNewStyle;

		break;

	case WindowStyle.MaximiseButton:
		dwNewStyle = ~dwCurrentStyle;
		dwNewStyle = dwNewStyle | WindowStyle.MaximiseButton;
		dwNewStyle = ~dwNewStyle;

		break;

	case WindowStyle.SystemMenu:
		dwNewStyle = ~dwCurrentStyle;
		dwNewStyle = dwNewStyle | WindowStyle.SystemMenu | WindowStyle.MaximiseButton | WindowStyle.MinimiseButton;
		dwNewStyle = ~dwNewStyle;

		break;

	default:
		break;
	}

	if ((dwCurrentStyle & WS_VISIBLE) == TRUE)
	{
		dwNewStyle = dwNewStyle | WS_VISIBLE;
	}

	this->SetWindowStyle(dwNewStyle);
	return TRUE;
}




BOOL cWindow::GetWindowParameters(CREATESTRUCT *sWindowCreateParameters)
{


	CREATESTRUCT *sWindowCreateStruct = { 0 };





	return TRUE;
}





BOOL cWindow::GetWindowHandle(HWND *hWindowHandle)
{
	if (this->bIsWindowCreated == FALSE)
	{
		return FALSE;
	}

	*hWindowHandle = this->hPrivateWindowHandle;
	return TRUE;
}




BOOL cWindow::ProcessMessageQueue()
{
	PeekMessage(&sPrivateMesssage, NULL, 0, 0, PM_REMOVE);

	// Take care of any messages that we want to here.
	switch (sPrivateMesssage.message)
	{

	case WM_QUIT:
		PostQuitMessage(0);
		return FALSE;
		break;

	default:
		break;
	}

	TranslateMessage(&sPrivateMesssage);
	DispatchMessage(&sPrivateMesssage);

	return TRUE;
}




VOID cWindow::Run()
{

	if (WindowClass.bIsRegisted == FALSE)
	{
		this->WindowClass.RegisterWindowClass();
	}

	if (bIsWindowCreated == FALSE)
	{
		this->CreateNewWindow();
	}

	// For MulitThread
	this->bIsWindowRunning = TRUE;

	while ((GetMessage(&this->sPrivateMesssage, NULL, 0, 0)) > 0)
	{
		if (this->sPrivateMesssage.message == WM_QUIT)
		{
			return;
		}

		TranslateMessage(&this->sPrivateMesssage);
		DispatchMessage(&this->sPrivateMesssage);
	}

	this->bIsWindowRunning = FALSE;
}




BOOL cWindow::IsWindowCreated()
{
	if (this->bIsWindowCreated == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}




BOOL cWindow::IsWindowRunning()
{
	return this->bIsWindowRunning;
}

