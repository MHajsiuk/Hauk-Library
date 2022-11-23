#pragma once

#ifndef __HAUK_MICROSOFT_WIN32_GUI_CWINDOWPROCEDURE_H_

#define __HAUK_MICROSOFT_WIN32_GUI_CWINDOWPROCEDURE_H_

#include <Windows.h>

// Defines for setting up sPrivateSubWindowProcedureArray
#define WPC_EXTRA_USER_WINDOW_MESSAGES			100		
#define WPC_MAX_SUB_WINDOW_PROCEDURES			(WM_USER + WPC_EXTRA_USER_WINDOW_MESSAGES)	


// Modes for ModifyWindowMessageSubscription()
#define MWMS_SUBSCRIBE		0x001
#define MWMS_REMOVE			0x000

// Ued to initalise the array of function pointers that the window procedure use and the user can subscribe too.
typedef struct tagSUB_WINDOW_PROCEDURE_INFO
{
	BOOL bIsSubFuncionSubscribed;
	WNDPROC lpfnSubWindowProcedure;
	BOOL bIsLocked;

}SUB_WINDOW_PROCEDURE_INFO, *PSUB_WINDOW_PROCEDURE_INFO;


class cWindowProcedure
{
public:
	cWindowProcedure();
	~cWindowProcedure();

	// Getter and setter for the window procedure
	WNDPROC GetWindowProcedureAddress();
	BOOL SetWindowProcedureAddress(WNDPROC WindowProcedure);

	// Getter and setter for the Window Procedure subscribe functions.
	BOOL SubscribeWindowMessage(UINT uMessage, WNDPROC WindowProcedure);
	BOOL RemoveWindowMessageSubscription(UINT uMessage);

private:
	static LRESULT CALLBACK StaticWindowProcedure(HWND hWindowHandle, UINT uMessage, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK PrivateWindowProcedure(HWND hWindowHandle, UINT uMessage, WPARAM wParam, LPARAM lParam);
	BOOL ModifyWindowMessageSubscription(UINT uWindowMessage, WNDPROC WindowProcedure, UINT uMode);
	BOOL LockWindowMessageSubscription(UINT uWindowMessage);
	BOOL UnLockWindowMessageSubscription(UINT uWindowMessage);

	WNDPROC lpfnPrivateStaticWindowProcedureAddress;
	PSUB_WINDOW_PROCEDURE_INFO sPrivateSubWindowProcedureArray = NULL;
	BOOL bIsSubArrayIntialised = FALSE;
};



#endif