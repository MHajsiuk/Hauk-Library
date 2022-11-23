#pragma once


#ifndef __HAUK_H_

#define __HAUL_H_

// Disable the warning messages from old functions 
#pragma warning(disable : 4996)
#pragma comment(lib, "hauk.lib")

// Includes
#include "Video.h"
#include "Types.h"
#include "cAnimatedWindow.h"
#include "cDrawingBuffer.h"
#include "cHighResolutionTimer.h"
#include "cWindow.h"
#include "cWindowProcedure.h"
#include "cWindowClass.h"

// defines
#define WM_DRAW_Buffer	WM_USER + 1


// Functions
char * IntegerToBinaryString(unsigned __int64 uUnsignedInteger, char *szOutputBuffer, unsigned int uUnsignedIntegerByteSize);
LRESULT CALLBACK __Draw_Buffer_Procedure(HWND hWindowHandle, UINT uMessage, WPARAM wParam, LPARAM lParam);
BOOL InitFullScreenInfoStruct(HWND hWindowHandle);
BOOL EnterFullScreen(HWND hWindowHandle);
BOOL LeaveFullScreen(HWND hWindowHandle);

#endif

