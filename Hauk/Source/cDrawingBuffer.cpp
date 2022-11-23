#include "Hauk.h"

cDrawingBuffer::cDrawingBuffer()
{
	this->bPrivateBufferLock = FALSE;
	this->pPrivateDrawingBufferRGB = NULL;

	this->sPrivateDrawingBufferInfo = (PDRAWING_BUFFER_INFO)malloc(sizeof(DRAWING_BUFFER_INFO));
	this->sPrivateDrawingBufferInfo->dwScale = DEFAULT_SCALE_COEFFCIENT;
	this->sPrivateDrawingBufferInfo->dwLengthX = 10;
	this->sPrivateDrawingBufferInfo->dwHeightY = 10;
	this->sPrivateDrawingBufferInfo->dwBytesPerPixel = 4;
	this->sPrivateDrawingBufferInfo->rgbBackground = 0x00000000;

	this->ResizeBuffer(this->sPrivateDrawingBufferInfo->dwLengthX, this->sPrivateDrawingBufferInfo->dwHeightY);


	SYSTEMTIME sSystemTime = { 0 };
	DWORD dwResult = 0;

	GetSystemTime(&sSystemTime);

	dwResult = sSystemTime.wDay + sSystemTime.wHour + sSystemTime.wMilliseconds + sSystemTime.wMinute + sSystemTime.wSecond;
	srand(dwResult);

}





cDrawingBuffer::~cDrawingBuffer()
{
	if (pPrivateDrawingBufferRGB != NULL)
	{
		free(this->pPrivateDrawingBufferRGB);
	}
}




BOOL cDrawingBuffer::GetBackgroundRGB(COLORREF *rgbBackground)
{
	*rgbBackground = this->sPrivateDrawingBufferInfo->rgbBackground;
	return TRUE;
}




BOOL cDrawingBuffer::SetBackgroundRGB(COLORREF rgbBackground)
{
	this->sPrivateDrawingBufferInfo->rgbBackground = rgbBackground;
	return TRUE;
}




BOOL cDrawingBuffer::SetBackgroundRGB(HBRUSH hbrBackground)
{
	LOGBRUSH sBrushInfo = { 0 };
	DWORD dwResult = 0;

	dwResult = GetObject(hbrBackground, sizeof(LOGBRUSH), &sBrushInfo);
	if (dwResult == NULL)
	{
		dwResult = GetLastError();
		return FALSE;
	}

	if ((sBrushInfo.lbStyle = BS_HATCHED) || (sBrushInfo.lbStyle = BS_SOLID))
	{
		this->SetBackgroundRGB((DWORD)sBrushInfo.lbColor);
		return TRUE;
	}

	return FALSE;
}




BOOL cDrawingBuffer::DrawLineSegment3D(DWORD dwStartX, DWORD dwStartY, DWORD dwStartZ, DWORD dwEndX, DWORD dwEndY, DWORD dwEndZ, DWORD dwColor)
{

	this->LockBuffer();

	DWORD dwFieldOfView = 90;
	DWORD dwStart2DX = 0;
	DWORD dwStart2DY = 0;
	DWORD dwEnd2DX = 0;
	DWORD dwEnd2DY = 0;

	dwStart2DX = (dwStartX)* dwFieldOfView / dwStartZ;
	dwStart2DY = (dwStartY)* dwFieldOfView / dwStartZ;
	dwEnd2DX = (dwStartX)* dwFieldOfView / dwEndZ;
	dwEnd2DY = (dwStartY)* dwFieldOfView / dwEndZ;

	this->DrawLineSegment2D(dwStart2DX, dwStart2DY, dwEnd2DX, dwEnd2DY, dwColor);

	return TRUE;
}




BOOL cDrawingBuffer::DrawPoint3D(DWORD dwX, DWORD dwY, DWORD dwZ, DWORD dwColor)
{
	this->LockBuffer();

	DWORD dwFieldOfView = 90;
	DWORD dw2DX = 0;
	DWORD dw2DY = 0;

	dw2DX = dwX / dwZ * dwFieldOfView;
	dw2DY = dwY / dwZ * dwFieldOfView;

	this->DrawPoint2D(dw2DX, dw2DY, dwColor);

	this->UnlockBuffer();

	return TRUE;
}






BOOL cDrawingBuffer::LockBuffer()
{
	//this->bPrivateBufferLock = TRUE;
	return TRUE;
}






BOOL cDrawingBuffer::UnlockBuffer()
{
	//this->bPrivateBufferLock = FALSE;
	return TRUE;
}





BOOL cDrawingBuffer::GetBufferLockStatus()
{
	return this->bPrivateBufferLock;
}





BOOL cDrawingBuffer::GetBufferAddress(PVOID *vbyteBuffer)
{
	if (this->pPrivateDrawingBufferRGB != NULL)
	{
		*vbyteBuffer = this->pPrivateDrawingBufferRGB;
		return TRUE;
	}
	return FALSE;
}



BOOL cDrawingBuffer::GetBufferInfo(DWORD *dwBufferLengthX, DWORD *dwBufferHeightY)
{

	*dwBufferHeightY = sPrivateDrawingBufferInfo->dwHeightY;
	*dwBufferLengthX = sPrivateDrawingBufferInfo->dwLengthX;

	return 0;
}




BOOL cDrawingBuffer::GetBufferInfo(PDRAWING_BUFFER_INFO pDrawingBufferInfo)
{

	pDrawingBufferInfo->dwBytesPerPixel = this->sPrivateDrawingBufferInfo->dwBytesPerPixel;
	pDrawingBufferInfo->dwHeightY = this->sPrivateDrawingBufferInfo->dwHeightY;
	pDrawingBufferInfo->dwLengthX = this->sPrivateDrawingBufferInfo->dwLengthX;
	pDrawingBufferInfo->dwScale = this->sPrivateDrawingBufferInfo->dwScale;
	pDrawingBufferInfo->hWindowHandle = this->sPrivateDrawingBufferInfo->hWindowHandle;
	pDrawingBufferInfo->rgbBackground = this->sPrivateDrawingBufferInfo->rgbBackground;


	return TRUE;
}




BOOL cDrawingBuffer::ResizeBuffer(DWORD dwLengthX, DWORD dwHeightY)
{
	// Check to see if in use.
	while (this->GetBufferLockStatus());
	this->LockBuffer();

	if (this->pPrivateDrawingBufferRGB != NULL)
	{
		free(this->pPrivateDrawingBufferRGB);
	}

	DWORD dwBufferSize = dwLengthX * dwHeightY;
	dwBufferSize = dwBufferSize * sizeof(RGBQUAD);	// Leave enough space for RGB values.

	this->pPrivateDrawingBufferRGB = (BYTE *)malloc(dwBufferSize);
	if (this->pPrivateDrawingBufferRGB == NULL)
	{
		this->UnlockBuffer();
		return FALSE;
	}

	// Update our internal data.
	this->sPrivateDrawingBufferInfo->dwLengthX = dwLengthX;
	this->sPrivateDrawingBufferInfo->dwHeightY = dwHeightY;

	FillMemory((BYTE *)this->pPrivateDrawingBufferRGB, dwBufferSize, 0x00);

	this->UnlockBuffer();
	return TRUE;

}






BOOL cDrawingBuffer::DrawBuffer(HWND hWindowHandle, DWORD dwWindowLengthX, DWORD dwWindowWidthY)
{
	this->DrawBuffer(hWindowHandle, 0, 0, dwWindowLengthX, dwWindowWidthY);

	return TRUE;
}


BOOL cDrawingBuffer::DrawBuffer(HWND hWindowHandle, DWORD dwDrawPositionX, DWORD dwDrawPositionY, DWORD dwWindowLengthX, DWORD dwWindowWidthY)
{
	// Check to see if in use.
	while (this->GetBufferLockStatus());
	this->LockBuffer();

	HDC hDeviceContext = GetDC(hWindowHandle);
	DWORD dwResult = 0;

	// First We Need a bitmap header to blit to the window.
	BITMAPINFO sBitmapInfo;
	FillMemory(&sBitmapInfo, sizeof(BITMAPINFO), 0x00);

	sBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	sBitmapInfo.bmiHeader.biPlanes = 1;
	sBitmapInfo.bmiHeader.biBitCount = 32;
	sBitmapInfo.bmiHeader.biWidth = sPrivateDrawingBufferInfo->dwLengthX;
	sBitmapInfo.bmiHeader.biHeight = sPrivateDrawingBufferInfo->dwHeightY;
	sBitmapInfo.bmiHeader.biCompression = BI_RGB;

	DWORD PX, PY;
	PX = this->sPrivateDrawingBufferInfo->dwLengthX;
	PY = this->sPrivateDrawingBufferInfo->dwHeightY;

	dwResult = StretchDIBits(hDeviceContext, dwDrawPositionX, dwDrawPositionY, dwWindowLengthX, dwWindowWidthY,
		0, 0, PX, PY,
		this->pPrivateDrawingBufferRGB, &sBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	DeleteDC(hDeviceContext);

	this->UnlockBuffer();
	return TRUE;
}


// Graphic Drawing Functions








BOOL cDrawingBuffer::DrawPoint2D(DWORD dwX, DWORD dwY, COLORREF rgbColor)
{
	PutPixel(dwX, dwY, rgbColor);
	return TRUE;
}




BOOL cDrawingBuffer::DrawLineSegment2D(DWORD dwStartX, DWORD dwStartY, DWORD dwEndX, DWORD dwEndY, COLORREF rgbColor)
{
	//Bresenham Line Algorithum

	// Calculate the difference between the two points
	INT nDifferenceX, nDifferenceY;
	INT nIncrementX, nIncrementY;
	INT nX, nY;
	INT nBalance;

	// Work out the direction and set the incrementers either + for forward & down or - for backward and up
	if (dwEndX >= dwStartX)
	{
		nDifferenceX = dwEndX - dwStartX;
		nIncrementX = 1;
	}
	else
	{
		nDifferenceX = dwStartX - dwEndX;
		nIncrementX = -1;
	}

	if (dwEndY >= dwStartY)
	{
		nDifferenceY = dwEndY - dwStartY;
		nIncrementY = 1;
	}
	else
	{
		nDifferenceY = dwStartY - dwEndY;
		nIncrementY = -1;
	}

	// Set the start position
	nX = dwStartX;
	nY = dwStartY;

	if (nDifferenceX >= nDifferenceY)
	{
		nDifferenceY <<= 1;
		nBalance = nDifferenceY - nDifferenceX;
		nDifferenceX <<= 1;

		while (nX != dwEndX)
		{
			DrawPoint2D(nX, nY, rgbColor);

			if (nBalance >= 0)
			{
				nY = nY + nIncrementY;
				nBalance = nBalance - nDifferenceX;
			}

			nBalance = nBalance + nDifferenceY;
			nX = nX + nIncrementX;
		}

		DrawPoint2D(nX, nY, rgbColor);
	}
	else
	{
		nDifferenceX <<= 1;
		nBalance = nDifferenceX - nDifferenceY;
		nDifferenceY <<= 1;

		while (nY != dwEndY)
		{
			DrawPoint2D(nX, nY, rgbColor);

			if (nBalance >= 0)
			{
				nX = nX + nIncrementX;
				nBalance = nBalance - nDifferenceY;
			}

			nBalance = nBalance + nDifferenceX;
			nY = nY + nIncrementY;
		}

		DrawPoint2D(nX, nY, rgbColor);
	}

	return TRUE;
}




BOOL cDrawingBuffer::DrawRectangle2D(DWORD dwLeftX, DWORD dwTopY, DWORD dwRightX, DWORD dwBottomY, COLORREF rgbColor)
{
	this->DrawLineSegment2D(dwLeftX, dwTopY, dwRightX, dwTopY, rgbColor);
	this->DrawLineSegment2D(dwRightX, dwTopY, dwRightX, dwBottomY, rgbColor);
	this->DrawLineSegment2D(dwRightX, dwBottomY, dwLeftX, dwBottomY, rgbColor);
	this->DrawLineSegment2D(dwLeftX, dwBottomY, dwLeftX, dwTopY, rgbColor);

	return TRUE;
}




BOOL cDrawingBuffer::DrawFilledRectangle2D(DWORD dwLeftX, DWORD dwTopY, DWORD dwRightX, DWORD dwBottomY, COLORREF rgbColor)
{
	int nDirection = 0;

	// First work out if we need to draw top down or bottom up
	if (dwTopY < dwBottomY)
	{
		nDirection = 1;
	}
	else
	{
		nDirection = -1;
	}

	for (DWORD dwPostionY = dwTopY; dwPostionY != dwBottomY; dwPostionY += nDirection)
	{
		this->DrawLineSegment2D(dwLeftX, dwPostionY, dwRightX, dwPostionY, rgbColor);
	}

	return TRUE;
}




BOOL cDrawingBuffer::DrawTriangle2D(DWORD dwStartX, DWORD dwStartY, DWORD dwMiddleX, DWORD dwMiddleY, DWORD dwEndX, DWORD dwEndY, COLORREF rgbColor)
{
	this->DrawLineSegment2D(dwStartX, dwStartY, dwMiddleX, dwMiddleY, rgbColor);
	this->DrawLineSegment2D(dwMiddleX, dwMiddleY, dwEndX, dwEndY, rgbColor);
	this->DrawLineSegment2D(dwEndX, dwEndY, dwStartX, dwStartY, rgbColor);

	return TRUE;
}





BOOL cDrawingBuffer::FillBuffer(COLORREF rgbColor)
{
	// Wait if buffer in use
	while (this->GetBufferLockStatus());

	DWORD dwBufferSize = 0;
	PVOID pvBufferRGB = NULL;
	DWORD *BufferRGB = NULL;

	BOOL bResult = FALSE;

	bResult = this->GetBufferAddress(&pvBufferRGB);
	if (bResult == FALSE)
	{
		return FALSE;
	}

	BufferRGB = (DWORD *)pvBufferRGB;

	dwBufferSize = this->sPrivateDrawingBufferInfo->dwLengthX * this->sPrivateDrawingBufferInfo->dwHeightY;

	for (DWORD dwCount = 0; dwCount < dwBufferSize; dwCount++)
	{
		*BufferRGB = rgbColor;
		BufferRGB++;
	}

	return TRUE;
}




BOOL cDrawingBuffer::ClearBuffer()
{
	DWORD dwBackgroundRGB = 0;

	this->GetBackgroundRGB(&dwBackgroundRGB);

	FillBuffer(dwBackgroundRGB);

	return TRUE;
}




BOOL cDrawingBuffer::FillBufferWithTestPattern()
{
	DWORD dwHeightY = 0;
	DWORD dwLengthX = 0;
	DWORD dwBufferLengthX = 0;
	DWORD dwBufferHeightY = 0;

	this->GetBufferInfo(&dwBufferLengthX, &dwBufferHeightY);

	for (dwHeightY = 0; dwHeightY < dwBufferHeightY; dwHeightY++)
	{
		DWORD dwRed = rand() % 255;
		DWORD dwGreen = rand() % 255;
		DWORD dwBlue = rand() % 255;

		for (dwLengthX = 0; dwLengthX < dwBufferLengthX; dwLengthX++)
		{
			this->PutPixel(dwLengthX, dwHeightY, RGB(dwRed, dwGreen, dwBlue));
		}
	}

	return TRUE;
}




BOOL cDrawingBuffer::FillBufferWithRandomPoints(DWORD dwPointCount)
{
	DWORD dwDrawingBufferLengthX = 0;
	DWORD dwDrawingBufferHeightY = 0;

	this->GetBufferInfo(&dwDrawingBufferLengthX, &dwDrawingBufferHeightY);

	for (DWORD dwCount = 0; dwCount < dwPointCount; dwCount++)
	{
		DWORD dwPositionX = rand() % dwDrawingBufferLengthX;
		DWORD dwPositionY = rand() % dwDrawingBufferHeightY;
		DWORD dwRed = rand() % 255;
		DWORD dwGreen = rand() % 255;
		DWORD dwBlue = rand() % 255;

		this->PutPixel(dwPositionX, dwPositionY, RGB(dwRed, dwGreen, dwBlue));
	}

	return TRUE;
}




BOOL cDrawingBuffer::FillBufferWithRandomLineSegments(DWORD dwLineSegmentCount)
{
	DWORD dwScreenMaxX;
	DWORD dwScreenMaxY;
	this->GetBufferInfo(&dwScreenMaxX, &dwScreenMaxY);

	for (DWORD dwCount = 0; dwCount < dwLineSegmentCount; dwCount++)
	{
		DWORD dwStartX = rand() % dwScreenMaxX;
		DWORD dwEndX = rand() % dwScreenMaxX;
		DWORD dwStartY = rand() % dwScreenMaxY;
		DWORD dwEndY = rand() % dwScreenMaxY;
		DWORD dwRed = rand() % 255;
		DWORD dwGreen = rand() % 255;
		DWORD dwBlue = rand() % 255;

		this->DrawLineSegment2D(dwStartX, dwStartY, dwEndX, dwEndY, RGB(dwRed, dwGreen, dwBlue));
	}

	return TRUE;
}




BOOL cDrawingBuffer::FillBufferWithRandomRectangles(DWORD dwRectangleCount)
{
	DWORD dwScreenMaxX;
	DWORD dwScreenMaxY;
	this->GetBufferInfo(&dwScreenMaxX, &dwScreenMaxY);

	for (DWORD dwCount = 0; dwCount < dwRectangleCount; dwCount++)
	{
		DWORD dwStartX = rand() % dwScreenMaxX;
		DWORD dwEndX = rand() % dwScreenMaxX;
		DWORD dwStartY = rand() % dwScreenMaxY;
		DWORD dwEndY = rand() % dwScreenMaxY;
		DWORD dwRed = rand() % 255;
		DWORD dwGreen = rand() % 255;
		DWORD dwBlue = rand() % 255;

		this->DrawRectangle2D(dwStartX, dwStartY, dwEndX, dwEndY, RGB(dwRed, dwGreen, dwBlue));
	}

	return TRUE;
}





BOOL cDrawingBuffer::FillBufferWithRandomFilledRectangles(DWORD dwRectangleCount)
{
	DWORD dwScreenMaxX;
	DWORD dwScreenMaxY;
	this->GetBufferInfo(&dwScreenMaxX, &dwScreenMaxY);

	for (DWORD dwCount = 0; dwCount < dwRectangleCount; dwCount++)
	{
		DWORD dwStartX = rand() % dwScreenMaxX;
		DWORD dwEndX = rand() % dwScreenMaxX;
		DWORD dwStartY = rand() % dwScreenMaxY;
		DWORD dwEndY = rand() % dwScreenMaxY;
		DWORD dwRed = rand() % 255;
		DWORD dwGreen = rand() % 255;
		DWORD dwBlue = rand() % 255;

		this->DrawFilledRectangle2D(dwStartX, dwStartY, dwEndX, dwEndY, RGB(dwRed, dwGreen, dwBlue));
	}

	return TRUE;
}





BOOL cDrawingBuffer::PutPixel(DWORD dwPositionX, DWORD dwPositionY, COLORREF rgbColor)
{
	// Wait if buffer in use
	while (this->GetBufferLockStatus());
	this->LockBuffer();

	DWORD dwRGBBufferLengthX = 0;
	DWORD dwRGBBufferHeightY = 0;

	this->GetBufferInfo(&dwRGBBufferLengthX, &dwRGBBufferHeightY);

	if (dwPositionX < 0 || dwPositionY < 0)
	{
		this->UnlockBuffer();
		return FALSE;
	}

	if (dwPositionX > (dwRGBBufferLengthX - 1) || dwPositionY > (dwRGBBufferHeightY - 1))
	{
		this->UnlockBuffer();
		return FALSE;
	}

	DWORD dwLocation = dwPositionY * dwRGBBufferLengthX;
	dwLocation = dwLocation + dwPositionX;
	dwLocation = dwLocation;

	DWORD *pBufferAddress = 0;

	this->GetBufferAddress((PVOID *)&pBufferAddress);
	pBufferAddress = pBufferAddress + dwLocation;

	*pBufferAddress = rgbColor;

	this->UnlockBuffer();
	return TRUE;
}



BOOL cDrawingBuffer::AddTVScanLines()
{
	DWORD dwScreenMaxX = 0, dwScreenMaxY = 0;
	this->GetBufferInfo(&dwScreenMaxX, &dwScreenMaxY);

	for (DWORD dwY = 0; dwY <= dwScreenMaxY; dwY += 2)
	{
		this->DrawLineSegment2D(0, dwY, dwScreenMaxX, dwY, RGB(0, 0, 0));
	}

	return TRUE;
}
	