#include "Hauk.h"

/*--------------------------------------------------------------------------------------------------------------------
cWindowProcedure() Constructor

	Deccription:
		Creates the array of WNDPROC pointers and sets it to 0x00 so that all of the bIsSubscribed are equal to FALSE.
		Also sets the address of the static window procedure.

	Arguments:

	Returns:

--------------------------------------------------------------------------------------------------------------------*/
cWindowProcedure::cWindowProcedure()
{
	lpfnPrivateStaticWindowProcedureAddress = StaticWindowProcedure;

	// Create the Sub Window Procedure array, It is only done here once.
	DWORD dwPrivateSubWindowProcedureArraySize = WPC_MAX_SUB_WINDOW_PROCEDURES * sizeof(SUB_WINDOW_PROCEDURE_INFO);
	sPrivateSubWindowProcedureArray = (PSUB_WINDOW_PROCEDURE_INFO)malloc(dwPrivateSubWindowProcedureArraySize);

	FillMemory(sPrivateSubWindowProcedureArray, dwPrivateSubWindowProcedureArraySize, 0x00);
}




/*--------------------------------------------------------------------------------------------------------------------
~WindowProcedure() Destructor

	Deccription:
		This cleans up the array of Subscribed window procedure pointers.

	Arguments:

	Returns:

--------------------------------------------------------------------------------------------------------------------*/
cWindowProcedure::~cWindowProcedure()
{

	free(sPrivateSubWindowProcedureArray);
}




/*-------------------------------------------------------------------------------------------------------------------
WNDPROC GetWindowProcAddress()

	Description:
		Getter for the WindowProc address.

	Arguments:
		None

	Returns:
		The address of the currently assigned window procedure
--------------------------------------------------------------------------------------------------------------------*/
WNDPROC cWindowProcedure::GetWindowProcedureAddress()
{

	return lpfnPrivateStaticWindowProcedureAddress;
}




/*-------------------------------------------------------------------------------------------------------------------
WNDPROC GetWindowProcAddress()

	Description:
		Setter for the WindowProc address.

	Arguments:
		WNDPROC WindowProcedure - The address of a user specified window procedure.

	Returns:
		This function always returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowProcedure::SetWindowProcedureAddress(WNDPROC WindowProcedure)
{
	lpfnPrivateStaticWindowProcedureAddress = WindowProcedure;
	return TRUE;
}





/*-------------------------------------------------------------------------------------------------------------------
BOOL LockWindowMessageSubscription(UINT uMessage);

	Description:
		This function puts a lock on any of the Window Message sub procedures subscribed in
		sPrivateSubWindowProcedureArray so that it can't be modified. The sub procedure does not need to be
		subscribed which means the def window proc will always handle the WM_XXXX.

	Arguments:
		UINT uWindowMessage - one of the valid Window Messages (WM_XXXXXX) that the WIN32 API has.

	Returns:
		Always TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowProcedure::LockWindowMessageSubscription(UINT uWindowMessage)
{
	sPrivateSubWindowProcedureArray[uWindowMessage].bIsLocked = TRUE;
	return TRUE;
}





/*-------------------------------------------------------------------------------------------------------------------
BOOL UnLockWindowMessageSubscription(UINT uMessage);

	Description:
		This function is the opposite of LockWindowMessageSubscription and removes the lock from a subscription so
		that it can be modified.

	Arguments:
		UINT uWindowMessage - one of the valid Window Messages (WM_XXXXXX) that the WIN32 API has.

	Returns:
		Always TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowProcedure::UnLockWindowMessageSubscription(UINT uWindowMessage)
{
	sPrivateSubWindowProcedureArray[uWindowMessage].bIsLocked = FALSE;
	return TRUE;
}





/*-------------------------------------------------------------------------------------------------------------------
BOOL SubscribeWindowMessage(UINT uMessage, WNDPROC WindowProcedure);

	Description:
		Public setter funcion for odifyWindowMessageSubscription()

	Arguments:
		UINT uWindowMessage - one of the valid Window Messages (WM_XXXXXX) that the WIN32 API has.

		WNDPROC WindowProcedure = The addess of a valid window procedure to call. Ignored if it is a MWMS_REMOVE.

	Returns:
		TRUE if Successful, FALSE on failure.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowProcedure::SubscribeWindowMessage(UINT uMessage, WNDPROC WindowProcedure)
{
	return ModifyWindowMessageSubscription(uMessage, WindowProcedure, MWMS_SUBSCRIBE);
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL RemoveWindowMessageSubscription(UINT uMessage);

	Description:
		Public getter funcion for ModifyWindowMessageSubscription()

	Arguments:
		UINT uWindowMessage - one of the valid Window Messages (WM_XXXXXX) that the WIN32 API has.

	Returns:
		TRUE if Successful, FALSE on failure.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowProcedure::RemoveWindowMessageSubscription(UINT uMessage)
{
	return ModifyWindowMessageSubscription(uMessage, NULL, MWMS_REMOVE);
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL ModifyWindowMessageSubscription(UINT uWindowMessage, WNDPROC WindowProcedure, UINT uMode)

	Description:
		This function adds or removes an function pointer entry in function pointer array for whatever windows
		message.

	Arguments:
		UINT uWindowMessage - one of the valid Window Messages (WM_XXXXXX) that the WIN32 API has.

		WNDPROC WindowProcedure = The addess of a valid window procedure to call. Ignored if it is a MWMS_REMOVE.

		UINT uMode - either MWMS_SUBSCRIBE or MWMSS_REMOVE which is one of the 2 modes this function supports.

	Returns:
		TRUE if Successful, FALSE on failure.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowProcedure::ModifyWindowMessageSubscription(UINT uWindowMessage, WNDPROC WindowProcedure, UINT uMode)
{
	// Are we outside the range of usable subscriptions.
	if (uWindowMessage > WPC_MAX_SUB_WINDOW_PROCEDURES)
	{
		return FALSE;
	}

	// Are we locked.
	if (sPrivateSubWindowProcedureArray[uWindowMessage].bIsLocked == TRUE)
	{
		return FALSE;
	}

	if (sPrivateSubWindowProcedureArray != NULL)
	{
		if (uMode == MWMS_SUBSCRIBE)
		{
			sPrivateSubWindowProcedureArray[uWindowMessage].lpfnSubWindowProcedure = WindowProcedure;
			sPrivateSubWindowProcedureArray[uWindowMessage].bIsSubFuncionSubscribed = TRUE;
		}
		else if (uMode == MWMS_REMOVE)
		{
			sPrivateSubWindowProcedureArray[uWindowMessage].lpfnSubWindowProcedure = NULL;
			sPrivateSubWindowProcedureArray[uWindowMessage].bIsSubFuncionSubscribed = FALSE;
		}
		else
		{
			return FALSE;
		}

		return TRUE;
	}
	return FALSE;
}




/*--------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK StaticWindowProcedure(HWND hWindowHandle, UINT uMessage, WPARAM wParam, LPARAM lParam)

	Deccription:
		This is a tricky one, as we cant call a class member function without it exsiting first we need to get the
		address of the object and set it into the window data area so it can be retrieved each time the function gets
		called. The address is initally passed by CreateWindow() LPARAM parameter. Then once that has been done, every
		time the function is called, it can retieve that object pointer	and call our member function.

	Arguments:
		HWND hWindowHandle - HAndle to the window that called the function.

		UINT uMessage - the WM_MESSAGE that needs to be processed.

		WPARAM wParam - Depends on the WM_MESSAGE.

		LPARAM lParam - Depends on the WM_MESSAGE.

	Returns:
		LRESULT - See MSDN.
--------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK cWindowProcedure::StaticWindowProcedure(HWND hWindowHandle, UINT uMessage, WPARAM wParam, LPARAM lParam)
{

	cWindowProcedure *pWindowProcedureObjectAddress = NULL;

	if (uMessage == WM_CLOSE)
	{
		PostQuitMessage(0);
	}

	if (uMessage == WM_NCCREATE)
	{

		// Get the address that was passed in from CreateWindow.
		LONG_PTR pWindowProcedureObjectAddress = (LONG_PTR)LPCREATESTRUCT(lParam)->lpCreateParams;

		// Save the address in our window data.
		SetWindowLongPtr(hWindowHandle, GWLP_USERDATA, pWindowProcedureObjectAddress);

		return DefWindowProc(hWindowHandle, uMessage, wParam, lParam);
	}

	if (uMessage <= WPC_MAX_SUB_WINDOW_PROCEDURES)
	{
		// Get the address we saved 
		pWindowProcedureObjectAddress = (cWindowProcedure *)GetWindowLongPtr(hWindowHandle, GWLP_USERDATA);

		if (pWindowProcedureObjectAddress != NULL)
		{

			return pWindowProcedureObjectAddress->PrivateWindowProcedure(hWindowHandle, uMessage, wParam, lParam);

		}
	}



	return DefWindowProc(hWindowHandle, uMessage, wParam, lParam);

}




/*-------------------------------------------------------------------------------------------------------------------
WNDPROC PrivateWindowProcAddress()

	Description:
		This member function gets called from the static window procedure once it determines the address. This
		then checks to see if the uMessage has been subscribed and passes control to that WNDPROC handler.

	Arguments:
		HWND hWindowHandle - Handle to the window that called the function.

		UINT uMessage - the WM_MESSAGE that needs to be processed.

		WPARAM wParam - Depends on the WM_MESSAGE.

		LPARAM lParam - Depends on the WM_MESSAGE.

	Returns:
		LRESULT - See MSDN.
--------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK cWindowProcedure::PrivateWindowProcedure(HWND hWindowHandle, UINT uMessage, WPARAM wParam, LPARAM lParam)
{


	if (sPrivateSubWindowProcedureArray[uMessage].bIsSubFuncionSubscribed == TRUE)
	{
		WNDPROC SubWindowProcedure = sPrivateSubWindowProcedureArray[uMessage].lpfnSubWindowProcedure;

		SubWindowProcedure(hWindowHandle, uMessage, wParam, lParam);
	}

	return DefWindowProc(hWindowHandle, uMessage, wParam, lParam);
}
