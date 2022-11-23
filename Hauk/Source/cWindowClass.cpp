/*--------------------------------------------------------------------------------------------------------------------------------
CWindowClass source file.

	The reason why I pass values to and from pointers and to and from struct members into a temp var is to aid in debugging.

	Written By Matthew Hajsiuk (C)opyright 2021

---------------------------------------------------------------------------------------------------------------------------------*/

#include "Hauk.h"




/*--------------------------------------------------------------------------------------------------------------------
cWindowClass() Constructor

Description:
	This function is overloaded. It set sets the basic values for sPrivateWindowClass. For this to be
	registered with the OS it needs a valid entry for lpfnWndProc as this just sets it to null. I use the class
	members to change the values so I only need to modify code in one spot.

Arguments:
	None

Returns:
	Nothing
-------------------------------------------------------------------------------------------------------------------*/
cWindowClass::cWindowClass()
{
	// Make sure we zero out the struct				
	memset(&sPrivateWindowClass, 0, sizeof(WNDCLASS));
	bIsRegisted = FALSE;

	// This needs to be set by user with their own function before this can be reigstered
	SetWindowProcedure(NULL);

	// The rest are most commonly used settings.
	SetStyle(CS_HREDRAW | CS_VREDRAW | CS_OWNDC);
	SetClassExtraMemory(0);
	SetWindowExtraMemory(0);
	SetInstanceHandle(GetModuleHandle(NULL));
	SetIconHandle(LoadIcon(NULL, IDI_APPLICATION));
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	SetBackgroundHandle((HBRUSH)COLOR_BACKGROUND + 1);
	SetMenuName(NULL);

	// Make a random name for the class using the system time.
	SYSTEMTIME sSystemTime = { 0 };
	GetSystemTime(&sSystemTime);

	DWORD dwClassNameBufferSize = sizeof(WCHAR) * CWC_MAX_STRING_LENGTH;
	dwClassNameBufferSize++;
	WCHAR *szClassName = (WCHAR *)malloc(dwClassNameBufferSize);
	memset(szClassName, 0x00, dwClassNameBufferSize);

	wsprintf(szClassName, L"Class%d%d%d%d", sSystemTime.wHour, sSystemTime.wMinute, sSystemTime.wSecond, sSystemTime.wMilliseconds);

	SetClassName(szClassName);

}




/*--------------------------------------------------------------------------------------------------------------------
cWindowClass(PWNDCLASS sWindowClass)

Description:
	Simple Constructor that forwards the address of a WNDCLASS struct to SetValue().

Arguments:
	PWNDCLASS - A Pointer to a WNDCLASS Struct.

Returns:
	Nothing.
--------------------------------------------------------------------------------------------------------------------*/
cWindowClass::cWindowClass(WNDCLASS *sWindowClass)
{
	// Not much to is here, loads the struct supplied into the sPrivateWindowClass.
	SetStyle(sWindowClass->style);
	SetWindowProcedure(sWindowClass->lpfnWndProc);
	SetClassExtraMemory(sWindowClass->cbClsExtra);
	SetWindowExtraMemory(sWindowClass->cbWndExtra);
	SetInstanceHandle(sWindowClass->hInstance);
	SetIconHandle(sWindowClass->hIcon);
	SetCursor(sWindowClass->hCursor);
	SetBackgroundHandle(sWindowClass->hbrBackground);
	SetMenuName(sWindowClass->lpszMenuName);
	SetClassName(sWindowClass->lpszClassName);
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL RegisterWindowClass()

Description:
	Simple wrapper to call the WIN32 API call RegisterClass() function. Set aPrivateWindowClassAtom on
	Success.

Arguments:
	None.

Returns:
	True on success, false on failure.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::RegisterWindowClass()
{
	DWORD dwErrorResult = 0;

	aPrivateWindowClassAtom = ::RegisterClass(&sPrivateWindowClass);
	if (aPrivateWindowClassAtom == 0)
	{
		dwErrorResult = GetLastError();
		return FALSE;
	}

	bIsRegisted = TRUE;
	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL UnRegisterWindowClass()

Description:
	Simple wrapper to call the WIN32 API call UnRegisterClass() function.

Arguments:
	None.

Returns:
	True on success, false on failure.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::UnRegisterWindowClass()
{
	WCHAR *szClassName = NULL;
	HINSTANCE hInstance = 0;
	BOOL bResult = FALSE;

	bResult = WCGetClassName(&szClassName);
	if (bResult == FALSE)
	{
		return FALSE;
	}

	bResult = GetInstanceHandle(&hInstance);
	if (bResult == FALSE)
	{
		return FALSE;
	}

	bResult = ::UnregisterClass(szClassName, hInstance);
	if (bResult == FALSE)
	{
		return FALSE;
	}

	bIsRegisted = FALSE;
	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL GetStyle(UINT *puStyle)

	Description:
		Returns the value for the sPrivateWindowClass.Style member

	Arguments:
		PINT puStyle - A pointer to the the callers UINT variable.

	Returns:
		Always retruns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::GetStyle(UINT *puStyle)
{
	UINT uStyle = sPrivateWindowClass.style;

	*puStyle = uStyle;
	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL SetStyle(UINT uStyle)

	Description:
		Set the value for the sPrivateWindowClass.Style member

	Arguments:
		INT uStyle - UINT style value i.e. CS_HREDRAW, CS_VREDRAW or CS_OWNDC.

	Returns:
		Always retruns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::SetStyle(UINT uStyle)
{
	UINT uValue = uStyle;

	sPrivateWindowClass.style = uValue;

	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL GetClassExtraMemory(INT *pnClassExtraMemory)

	Description:
		Returns the value set in sPrivateWindowClass.cbsClsExtra member.

	Arguments:
		PINT pnClassExtraMemory - A pointer to a caller variable to recieve the data

	Returns:
		Always returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::GetClassExtraMemory(INT *pnClassExtraMemory)
{
	INT nValue = sPrivateWindowClass.cbClsExtra;

	*pnClassExtraMemory = nValue;

	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL SetClassExtraMemory(int nClassExtraMemory)

	Description:
		Sets the value for the sPrivateWindowClass.cbsClsExtra member.

	Arguments:
		INT ClassExtraMemory - the value to be set in the class.

	Returns:
		Always returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::SetClassExtraMemory(INT nClassExtraMemory)
{
	INT nValue = nClassExtraMemory;

	sPrivateWindowClass.cbClsExtra;

	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL GetWindowExtraMemory(INT *pnWindowExtraMemory)

	Description:
		Returns the value set in sPrivateWindowClass.cbsWndExtra member.

	Arguments:
		PINT pnWindowExtraMemory - A pointer to a caller variable to recieve the data

	Returns:
		Always returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::GetWindowExtraMemory(INT *pnWindowExtraMemory)
{
	INT nValue = sPrivateWindowClass.cbWndExtra;

	*pnWindowExtraMemory = nValue;

	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL SetWindowExtraMemory(int nClassExtraMemory)

	Description:
		Sets the value for the sPrivateWindowClass.cbsWndExtra member.

	Arguments:
		INT WindowExtraMemory - the value to be set in the class.

	Returns:
		Always returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::SetWindowExtraMemory(INT nWindowExtraMemory)
{
	INT nValue = nWindowExtraMemory;

	sPrivateWindowClass.cbWndExtra = nValue;

	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL GetInstanceHandle(HINSTANCE *phInstance)

	Description:
		Returns the handle of the instance value set in sPrivateWindow.hInstance.

	Arguments:
		HINSTANCE *phInstance - a pointer to the callers var that will recieve the handle.

	Returns:
		Always returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::GetInstanceHandle(HINSTANCE *phInstance)
{
	HINSTANCE hValue = sPrivateWindowClass.hInstance;

	*phInstance = hValue;

	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL SetrInstanceHandle(HINSTANCE hInstance)

	Description:
		Sets the handle value in sPrivateWindowClass to the one provided in hInstance.

	Arguments:
		HINSTANCE hInstance - a valid handle value.

	Returns:
		Always returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::SetInstanceHandle(HINSTANCE hInstance)
{
	HINSTANCE hValue = hInstance;

	sPrivateWindowClass.hInstance = hValue;

	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL GetIconHandle(HICON *phIcon)

	Description:
		Gets the value set in sPrivateWindowClass.hIcon.

	Arguments:
		HICON *hIcon - a pointer to a var provided by the caller.

	Returns:

--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::GetIconHandle(HICON *phIcon)
{
	HICON hValue = sPrivateWindowClass.hIcon;

	*phIcon = hValue;

	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL SetIconHandle(HICON hIcon)

	Description:
		Sets the value in sPrivateWindowClass.hIcon to the value provied by the caller.

	Arguments:
		HICON hIcon - Value of a valid handle to an icon.

	Returns:
		Always returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::SetIconHandle(HICON hIcon)
{
	HICON hValue = hIcon;

	sPrivateWindowClass.hIcon = hValue;

	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL GetCursorHandle(HCURSOR *phCursor)

	Description:
		Gets the value set in sPrivateWindowClass.hCursor.

	Arguments:
		HCURSOR *hCursor - a pointer to a var provided by the caller.

	Returns:
		Always returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::GetCursorHandle(HCURSOR *phCursor)
{
	HCURSOR hValue = sPrivateWindowClass.hCursor;

	*phCursor = hValue;

	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL SetCursorHandle(HCURSOR hCursor)

	Description:
		Sets the value in sPrivateWindowClass.hCrusor to the value provied by the caller.

	Arguments:
		HCURSOR hCursor - a valid handle to a cursor.

	Returns:
		Always returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::SetCursorHandle(HCURSOR hCursor)
{
	HCURSOR hValue = hCursor;

	sPrivateWindowClass.hCursor = hValue;

	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL GetBackgroundHandle(HBRUSH *phbrBackground)

	Description:
		Gets the value set in sPrivateWindowClass.hbrBackground.

	Arguments:
		 HBRUSH *hbrBackground - a pointer to a var provided by the caller.

	Returns:
		Always returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::GetBackgroundHandle(HBRUSH *phbrBackground)
{
	HBRUSH hValue = sPrivateWindowClass.hbrBackground;

	*phbrBackground = hValue;

	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL SetBackgroundHandle(HBRUSH hbrBackground)

	Description:
		Sets the value in sPrivateWindowClass.hbrBackground to the value provied by the caller.

	Arguments:
		BHRUSH hbeBackground - a valid handle to a brush color.

	Returns:
		Always returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::SetBackgroundHandle(HBRUSH hbrBackground)
{
	HBRUSH hValue = hbrBackground;

	sPrivateWindowClass.hbrBackground = hValue;

	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL GetMenuName(PWACHR *szMenuName)

	Description:
		This function gets the string that is set for the sPrivateWindowClass member lpszMenuName. If it is set to
		NULL which commonly it is, the function sets the pointer to string to NULL and returns True.

	Arguments:
		PWCHAR *szMenuName - This value is a pointer to a string pointer which this function sets the string pointer

	Returns:
		If the return value is a string or NULL the return value is TRUE. otherwise FALSE is set.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::GetMenuName(PWCHAR *szMenuName)
{

	if (sPrivateWindowClass.lpszMenuName == NULL)
	{
		*szMenuName = NULL;
		return TRUE;
	}

	free(*szMenuName);

	DWORD dwStringLength = wcslen(sPrivateWindowClass.lpszMenuName);
	dwStringLength++;
	*szMenuName = (WCHAR *)malloc(sizeof(WCHAR) * dwStringLength);
	if (szMenuName == NULL)
	{
		return FALSE;
	}


	lstrcpyW(*szMenuName, sPrivateWindowClass.lpszMenuName);

	return TRUE;
}





/*-------------------------------------------------------------------------------------------------------------------
BOOL SetMenuName(PWCHAR szMenuName)

	Description:
		This function sets the name of sPrivatetWindowClass.lpszMenuName to either the string provided in szMenuName
		or NULL which is the common value for this member.

	Arguments:
		PWCHAR szMenuName - Either NULL or a pointer to a string.

	Returns:
		The only time this function returns FALSE is on memory allocation error otherwise it returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::SetMenuName(WCHAR CONST *szMenuName)
{
	if (szMenuName == NULL)
	{
		sPrivateWindowClass.lpszMenuName = NULL;
		return TRUE;
	}

	DWORD dwStringLength = wcslen(szMenuName);

	if (sPrivateWindowClass.lpszMenuName != NULL)
	{
		free((WCHAR *)sPrivateWindowClass.lpszClassName);
	}

	DWORD dwNewStringSize = sizeof(WCHAR) * dwStringLength;
	WCHAR *szNewMenuName = (WCHAR *)malloc(dwNewStringSize);

	if (szNewMenuName == NULL)
	{
		return FALSE;
	}

	lstrcpy(szNewMenuName, szMenuName);

	sPrivateWindowClass.lpszMenuName = szNewMenuName;

	return TRUE;
}





/*-------------------------------------------------------------------------------------------------------------------
BOOL GetClassName(PWACHR *szMenuName)

	Description:
		This function gets the string that is set for the sPrivateWindowClass member lpszClassName.

	Arguments:
		PWCHAR *szClassName - This value is a pointer to a string pointer which this function sets the string pointer

	Returns:
		If the is a memory allocation error the the return value is FALSE otherwise the return value is TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::WCGetClassName(PWCHAR *szClassName)
{

	if (sPrivateWindowClass.lpszClassName == NULL)		// This should be impossible but here just in case.
	{
		return FALSE;
	}

	if (*szClassName != NULL)
	{
		free(*szClassName);
	}

	DWORD dwStringLength = wcslen(sPrivateWindowClass.lpszClassName);
	dwStringLength++;
	*szClassName = (WCHAR *)malloc(sizeof(WCHAR) * dwStringLength);
	if (szClassName == NULL)
	{
		return FALSE;
	}


	lstrcpyW(*szClassName, sPrivateWindowClass.lpszClassName);

	return TRUE;
}





/*-------------------------------------------------------------------------------------------------------------------
BOOL SetMenuName(PWCHAR szMenuName)

	Description:
		This function sets the name of sPrivatetWindowClass.lpszClassName to the string provided by the pointer.

	Arguments:
		PWCHAR szClassName - A pointer to a string. If this is NULL the function will fail.

	Returns:
		The only time this function returns FALSE is on memory allocation error otherwise it returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::SetClassName(WCHAR CONST *szClassName)
{
	if (szClassName == NULL)
	{
		return FALSE;
	}

	DWORD dwStringLength = wcslen(szClassName);

	if (sPrivateWindowClass.lpszClassName != NULL)
	{
		free((WCHAR *)sPrivateWindowClass.lpszClassName);
	}

	DWORD dwNewStringSize = sizeof(WCHAR) * dwStringLength;
	WCHAR *szNewClassName = (WCHAR *)malloc(dwNewStringSize);
	if (szNewClassName == NULL)
	{
		return FALSE;
	}

	lstrcpy(szNewClassName, szClassName);

	sPrivateWindowClass.lpszClassName = szNewClassName;

	return TRUE;
}




/*-------------------------------------------------------------------------------------------------------------------
BOOL SetWindowProcedure(WNDPROC WindowProcedure);

	Description:
		Sets the address of the Window Procedure in sPrivateWindowClass.lpfnWndProc.

	Arguments:
		WNDPROC WindowProcedure - The address of an external Window Procedure.

	Returns:
		Always returns TRUE.
--------------------------------------------------------------------------------------------------------------------*/
BOOL cWindowClass::SetWindowProcedure(WNDPROC WindowProcedure)
{
	WNDPROC WindowProc = WindowProcedure;

	sPrivateWindowClass.lpfnWndProc = WindowProc;

	return TRUE;
}
