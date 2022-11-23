#include "Hauk.h"


// Global struct for Fullscreen functionality.
FULLSCREEN_INFO sFullScreenInfo;

cAnimatedWindow::cAnimatedWindow()
{
	CopyMemory(&this->sPrivateVideoMode, &VideoDisplayModes[QVGA], sizeof(VIDEO_MODE_INFO));
	this->bPrivateIsFullscreen = FALSE;
	this->DrawingBuffer.ResizeBuffer(this->sPrivateVideoMode.nLengthX, this->sPrivateVideoMode.nHeightY);

	this->SetWindowStyle(WS_CAPTION);
	this->RemoveWindowStyle(WindowStyle.Sizeable);
	this->AddWindowStyle(WindowStyle.SystemMenu);

	this->SetWindowClientSize(this->sPrivateVideoMode.nLengthX, this->sPrivateVideoMode.nHeightY);

	HBRUSH hbrBackGround = (HBRUSH)::GetStockObject(BLACK_BRUSH);
	this->WindowClass.SetBackgroundHandle(hbrBackGround);
	this->DrawingBuffer.SetBackgroundRGB(hbrBackGround);
	this->WindowProcedure.SubscribeWindowMessage(WM_SYSKEYUP, PrivateSysKeyProc);

	this->SetDrawFunction(__Draw_Buffer_Procedure);
	this->sPrivateAnimatedWindowInfo.DrawingBufferObjectAddress = &this->DrawingBuffer;
	this->sPrivateAnimatedWindowInfo.dwWindowLengthX = this->sPrivateVideoMode.nLengthX;
	this->sPrivateAnimatedWindowInfo.dwWindowHeightY = this->sPrivateVideoMode.nHeightY;

	// For Full screen Global struct.
	FillMemory(&sFullScreenInfo, sizeof(sFullScreenInfo), 0x00);
	sFullScreenInfo.bIsSet = FALSE;
	sFullScreenInfo.bIsFullScreen = FALSE;

	// Get Timer info
	this->Timer.StartTimer();
	this->sPrivateAnimatedWindowInfo.TimeInfo = this->Timer.GetTimerInfo();
	   
}




cAnimatedWindow::~cAnimatedWindow()
{


}




BOOL cAnimatedWindow::Animate()
{
	BOOL bIsWindowCreated = this->IsWindowCreated();
	BOOL bIsValidMessage = TRUE;
	HWND hWindowHandle = NULL;

	if (bIsWindowCreated == FALSE)
	{
		this->CreateNewWindow();
	}

	this->GetWindowHandle(&hWindowHandle);

	// For Multithreading
	this->bIsWindowRunning = TRUE;

	while (bIsValidMessage)
	{
		bIsValidMessage = this->ProcessMessageQueue();
		this->Timer.UpdateTimer();
		sPrivateAnimatedWindowInfo.TimeInfo = this->Timer.GetTimerInfo();
		SendMessage(hWindowHandle, WM_DRAW_Buffer, 0, (LPARAM)&this->sPrivateAnimatedWindowInfo);

		// Draw the buffer.
		if (sFullScreenInfo.bIsFullScreen)
		{
			this->DrawingBuffer.DrawBuffer(
				hWindowHandle,
				sFullScreenInfo.sDrawPosition.left,
				sFullScreenInfo.sDrawPosition.top,
				sFullScreenInfo.sDrawPosition.right,
				sFullScreenInfo.sDrawPosition.bottom);
		}

		else
		{
			this->DrawingBuffer.DrawBuffer(hWindowHandle,
				this->sPrivateAnimatedWindowInfo.dwWindowLengthX,
				this->sPrivateAnimatedWindowInfo.dwWindowHeightY);

		}


	}
	this->bIsWindowRunning = FALSE;

	return TRUE;
}




BOOL cAnimatedWindow::SetDrawFunction(WNDPROC DrawingProcedure)
{

	this->WindowProcedure.SubscribeWindowMessage(WM_DRAW_Buffer, DrawingProcedure);

	return TRUE;
}




BOOL cAnimatedWindow::SetVideoMode(VIDEO_MODE_INFO sVideoModeInfo)
{
	CopyMemory(&this->sPrivateVideoMode, &sVideoModeInfo, sizeof(VIDEO_MODE_INFO));
	DWORD dwWindowLengthX = sVideoModeInfo.nLengthX;
	DWORD dwWindowHeightY = sVideoModeInfo.nHeightY;
	BOOL bResult = FALSE;

	bResult = this->DrawingBuffer.ResizeBuffer(dwWindowLengthX, dwWindowHeightY);
	if (bResult == FALSE)
	{
		return FALSE;
	}

	bResult = this->SetWindowClientSize(dwWindowLengthX, dwWindowHeightY);
	if (bResult == FALSE)
	{
		return FALSE;
	}

	this->sPrivateAnimatedWindowInfo.dwWindowHeightY = dwWindowHeightY;
	this->sPrivateAnimatedWindowInfo.dwWindowLengthX = dwWindowLengthX;

	return TRUE;
}




LRESULT CALLBACK cAnimatedWindow::PrivateSysKeyProc(HWND hWindowHandle, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = 0;

	// Call the none member function to fill out the Global FullScreenInfoStruct
	// This only need to do this once.
	if (sFullScreenInfo.bIsSet == FALSE)
	{
		bResult = InitFullScreenInfoStruct(hWindowHandle);
		if (bResult == FALSE)
		{
			return FALSE;
		}

		sFullScreenInfo.bIsSet = bResult;
	}

	// Switch the ALT key combo.
	switch (wParam)
	{
	case VK_RETURN:

		if (sFullScreenInfo.bIsFullScreen == FALSE)
		{
			bResult = EnterFullScreen(hWindowHandle);
			sFullScreenInfo.bIsFullScreen = TRUE;
		}
		else
		{
			bResult = LeaveFullScreen(hWindowHandle);
			sFullScreenInfo.bIsFullScreen = FALSE;
		}

		UpdateWindow(hWindowHandle);
		break;
	}

	return 0;
}




BOOL LeaveFullScreen(HWND hWindowHandle)
{
	DWORD dwWindowLengthX, dwWindowHeightY;
	DWORD dwWindowPositionX, dwWindowPositionY;
	DWORD dwWindowStyle, dwWindowStyleEx;
	BOOL bResult = 0;

	dwWindowStyle = sFullScreenInfo.dwWindowStyle;
	dwWindowStyleEx = sFullScreenInfo.dwWindowStyleEx;
	bResult = SetWindowLong(hWindowHandle, GWL_STYLE, dwWindowStyle);
	bResult = SetWindowLong(hWindowHandle, GWL_EXSTYLE, dwWindowStyleEx);

	dwWindowLengthX = sFullScreenInfo.dwWindowSizeX;
	dwWindowHeightY = sFullScreenInfo.dwWindowSizeY;
	dwWindowPositionX = sFullScreenInfo.dwWindowPositionX;
	dwWindowPositionY = sFullScreenInfo.dwWindowPositionY;

	bResult = SetWindowPos(
		hWindowHandle,
		HWND_TOP,
		dwWindowPositionX,
		dwWindowPositionY,
		dwWindowLengthX,
		dwWindowHeightY,
		SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	return TRUE;
}




BOOL EnterFullScreen(HWND hWindowHandle)
{
	DWORD dwWindowLengthX = 0, dwWindowHeightY = 0;
	DWORD dwWindowStyle, dwWindowStyleEx;
	BOOL bResult = 0;
	RECT sWindowInfo;


	// Get window position as it could have changed
	FillMemory(&sWindowInfo, sizeof(RECT), 0xFF);

	GetWindowRect(hWindowHandle, &sWindowInfo);
	sFullScreenInfo.dwWindowPositionX = sWindowInfo.left;
	sFullScreenInfo.dwWindowPositionY = sWindowInfo.top;

	// Update the window style
	dwWindowStyle = sFullScreenInfo.dwWindowStyle;
	dwWindowStyleEx = sFullScreenInfo.dwWindowStyleEx;
	dwWindowStyle = dwWindowStyle & ~(WS_CAPTION | WS_THICKFRAME);
	dwWindowStyleEx = sFullScreenInfo.dwWindowStyleEx & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);

	bResult = SetWindowLong(hWindowHandle, GWL_STYLE, dwWindowStyle);
	bResult = SetWindowLong(hWindowHandle, GWL_EXSTYLE, dwWindowStyleEx);

	bResult = SetWindowPos(
		hWindowHandle,
		HWND_TOP,
		0,
		0,
		sFullScreenInfo.dwMonitorSizeX,
		sFullScreenInfo.dwMonitorSizeY,
		SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);


	return TRUE;
}




BOOL InitFullScreenInfoStruct(HWND hWindowHandle)
{
	DWORD dwDrawPositionX = 0;
	RECT sWindowInfo = { 0 };
	DWORD dwWindowLengthX = 0, dwWindowHeightY = 0;
	DWORD dwWindowStyle, dwWindowStyleEx;
	FLOAT fScratch = 0;
	HMONITOR hMonitor = 0;
	HDC hDC;
	MONITORINFO sMonitorInfo;

	hDC = GetDC(hWindowHandle);

	// Set monitor size
	FillMemory(&sMonitorInfo, sizeof(MONITORINFO), 0x00);
	sMonitorInfo.cbSize = sizeof(sMonitorInfo);

	hMonitor = MonitorFromWindow(hWindowHandle, MONITOR_DEFAULTTOPRIMARY);
	GetMonitorInfo(hMonitor, &sMonitorInfo);

	sFullScreenInfo.dwMonitorSizeX = sMonitorInfo.rcMonitor.right - sMonitorInfo.rcMonitor.left;
	sFullScreenInfo.dwMonitorSizeY = sMonitorInfo.rcMonitor.bottom - sMonitorInfo.rcMonitor.top;

	// Set the window size
	FillMemory(&sWindowInfo, sizeof(RECT), 0xFF);

	GetClientRect(hWindowHandle, &sWindowInfo);

	dwWindowLengthX = sWindowInfo.right;
	dwWindowLengthX = dwWindowLengthX - sWindowInfo.left;
	dwWindowHeightY = sWindowInfo.bottom;
	dwWindowHeightY = dwWindowHeightY - sWindowInfo.top;

	sFullScreenInfo.dwWindowSizeX = dwWindowLengthX;
	sFullScreenInfo.dwWindowSizeY = dwWindowHeightY;

	// Set the window position
	FillMemory(&sWindowInfo, sizeof(RECT), 0xFF);

	GetWindowRect(hWindowHandle, &sWindowInfo);
	sFullScreenInfo.dwWindowPositionX = sWindowInfo.left;
	sFullScreenInfo.dwWindowPositionY = sWindowInfo.top;


	// Work out the drawing position for the StartY and EndY when in fullscreen
	fScratch = (FLOAT)sFullScreenInfo.dwMonitorSizeY / (FLOAT)sFullScreenInfo.dwWindowSizeY;
	fScratch = (FLOAT)sFullScreenInfo.dwWindowSizeX * fScratch;
	dwDrawPositionX = (DWORD)fScratch;

	// Save the new window size, Y should be the same size as the monitor Y and X is the ratio of window Y into monitor Y
	sFullScreenInfo.dwFullScreenWindowSizeX = (DWORD)fScratch;
	sFullScreenInfo.dwFullScreenWindowSizeY = sFullScreenInfo.dwMonitorSizeY;

	fScratch = sFullScreenInfo.dwMonitorSizeX - fScratch;
	fScratch = fScratch / 2;

	sFullScreenInfo.sDrawPosition.left = (LONG)fScratch;
	sFullScreenInfo.sDrawPosition.right = dwDrawPositionX;
	sFullScreenInfo.sDrawPosition.top = 0;
	sFullScreenInfo.sDrawPosition.bottom = sFullScreenInfo.dwMonitorSizeY;

	// Work out the drawing position for the StartX and EndX when in fullscreen

	dwWindowStyle = GetWindowLong(hWindowHandle, GWL_STYLE);
	dwWindowStyleEx = GetWindowLong(hWindowHandle, GWL_EXSTYLE);

	sFullScreenInfo.dwWindowStyle = dwWindowStyle;
	sFullScreenInfo.dwWindowStyleEx = dwWindowStyleEx;

	sFullScreenInfo.bIsSet = TRUE;
	sFullScreenInfo.dwMonitorRefreshRate = GetDeviceCaps(hDC, VREFRESH);

	DeleteDC(hDC);

	return TRUE;
}





static LRESULT CALLBACK __Draw_Buffer_Procedure(HWND hWindowHandle, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	ANIMATED_WINDOW_INFO *sAnimateWindowInfo = (ANIMATED_WINDOW_INFO *)lParam;
	cDrawingBuffer *DrawingBuffer = sAnimateWindowInfo->DrawingBufferObjectAddress;

	DrawingBuffer->FillBufferWithRandomPoints(1000);

	//DrawingBuffer->DrawBuffer(hWindowHandle, sAnimateWindowInfo->dwWindowLengthX, sAnimateWindowInfo->dwWindowHeightY);

	return 0;
}




