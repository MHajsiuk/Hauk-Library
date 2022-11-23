#pragma once

#ifndef __HAUK_GENERIC_GRAPHICS_CDRAWINGBUFFER_H_

#define __HAUK_GENERIC_GRAPHICS_CDRAWINGBUFER_H_

#include "Hauk.h"


typedef struct tagDRAWING_BUFFER_INFO
{
	DWORD dwLengthX;
	DWORD dwHeightY;
	DWORD dwScale;
	DWORD dwBytesPerPixel;
	COLORREF rgbBackground;
	HWND hWindowHandle;

}DRAWING_BUFFER_INFO, *PDRAWING_BUFFER_INFO;

class cDrawingBuffer
{
public:
	cDrawingBuffer();
	~cDrawingBuffer();

	// Getters and Setters
	BOOL GetBackgroundRGB(COLORREF *prgbBackgroundColor);
	BOOL SetBackgroundRGB(COLORREF rgbBackgroundColor);
	BOOL SetBackgroundRGB(HBRUSH hbrBackground);

	// Buffer managment functions
	BOOL LockBuffer();
	BOOL UnlockBuffer();
	BOOL GetBufferLockStatus();
	BOOL GetBufferAddress(PVOID *vbyteBuffer);
	BOOL GetBufferInfo(DWORD *dwBufferLengthX, DWORD *dwBufferHeightY);
	BOOL GetBufferInfo(PDRAWING_BUFFER_INFO pDrawingBufferInfo);
	BOOL ResizeBuffer(DWORD dwLengthX, DWORD dwHeightY);
	BOOL DrawBuffer(HWND hWindowHandle, DWORD dwWindowLengthX, DWORD dwWindowWidthY);
	BOOL DrawBuffer(HWND hWindowHandle, DWORD dwDrawPositionX, DWORD dwDrawPositionY, DWORD dwWindowLengthX, DWORD dwWindowWidthY);
	BOOL ClearBuffer();

	// Test functions
	BOOL FillBufferWithTestPattern();
	BOOL FillBufferWithRandomPoints(DWORD dwPointCount);
	BOOL FillBufferWithRandomLineSegments(DWORD dwLineSegmentCount);
	BOOL FillBufferWithRandomRectangles(DWORD dwRectangleCount);
	BOOL FillBufferWithRandomFilledRectangles(DWORD dwRectangleCount);
	BOOL FillBuffer(COLORREF rgbColor);


	// 2D Graphic functions
	BOOL DrawPoint2D(DWORD dwX, DWORD dwY, COLORREF rgbColor);
	BOOL DrawLineSegment2D(DWORD dwStartX, DWORD dwStartY, DWORD dwEndX, DWORD dwEndY, COLORREF rgbColor);
	BOOL DrawRectangle2D(DWORD dwLeftX, DWORD dwTopY, DWORD dwRightX, DWORD dwBottomY, COLORREF rgbColor);
	BOOL DrawFilledRectangle2D(DWORD dwLeftX, DWORD dwTopY, DWORD dwRightX, DWORD dwBottomY, COLORREF rgbColor);
	BOOL DrawTriangle2D(DWORD dwStartX, DWORD dwStartY, DWORD dwMiddleX, DWORD dwMiddleY, DWORD dwEndX, DWORD dwEndY, COLORREF rgbColor);


	// 3D Graphics functions
	BOOL DrawPoint3D(DWORD dwX, DWORD dwY, DWORD dwZ, DWORD dwColor);
	BOOL DrawLineSegment3D(DWORD dwStartX, DWORD dwStartY, DWORD dwStartZ, DWORD dwEndX, DWORD dwEndY, DWORD dwEndZ, DWORD dwColor);
	// Effects functions
	BOOL AddTVScanLines();


private:
	BOOL PutPixel(DWORD dwX, DWORD dwY, COLORREF rgbColor);
	BOOL bPrivateBufferLock;
	PDRAWING_BUFFER_INFO sPrivateDrawingBufferInfo;
	VOID *pPrivateDrawingBufferRGB;

};

#endif