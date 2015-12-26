#pragma once

#include "resource.h"
#include <windowsx.h>
#include <shlobj.h>	// BROWSEINFO

/*
 * Save infomation of folder
 */
struct FOLDER
{
	WCHAR szFolderPath[MAX_PATH]; // Full path
	WCHAR szFolderName[MAX_PATH]; // Only folder name
};

static FOLDER folder;

// This will set the font of the controls
void SetFont(HWND hwnd, LPTSTR FontName, int FontSize)
{
	HFONT hf;
	LOGFONT lf = { 0 };
	HDC hdc = GetDC(hwnd);

	GetObject(GetWindowFont(hwnd), sizeof(lf), &lf);
	lf.lfWeight = FW_REGULAR;
	lf.lfHeight = (LONG)FontSize;
	lstrcpy(lf.lfFaceName, FontName);
	hf = CreateFontIndirect(&lf);
	SetBkMode(hdc, OPAQUE);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hf, TRUE);
	ReleaseDC(hwnd, hdc);

}

/*
 * BFFCALLBACK function pointer
 * Browse dialog callback procedure
 * hWnd: The window handle of the browse dialog box.
 * uMsg: The dialog box event that generated the message. One of the following values.
 * BFFM_INITIALIZED: The dialog box has finished initializing.
 * BFFM_SELCHANGED: The selection has changed in the dialog box.
 * lParam: A value whose meaning depends on the event specified in uMsg
 */
int __stdcall BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{

	case BFFM_INITIALIZED:
		wcscpy(folder.szFolderName, L"");
		wcscpy(folder.szFolderPath, L"");
		break;
		// Selection change message
	case BFFM_SELCHANGED:
		SHGetPathFromIDListW((ITEMIDLIST*)lParam, folder.szFolderPath);
		break;
	}
	return 0;
}

/*
* Choose folder to save file
*/
bool BrowseFolder(HWND hDlg)
{
	BROWSEINFOW browinfo;
	ZeroMemory(&browinfo, sizeof(browinfo));

	browinfo.hwndOwner = hDlg; // A handle to the owner window for the dialog box.
	browinfo.pidlRoot = NULL; // The location of the root folder

	// Pointer to a buffer to receive the display name of the folder selected by the user.
	// The size of this buffer is assumed to be MAX_PATH characters.
	browinfo.pszDisplayName = folder.szFolderName;

	// Pointer to a null-terminated string that is displayed above the tree view control in the dialog box.
	// This string can be used to specify instructions to the user.
	browinfo.lpszTitle = _T("Vui lòng chọn thư mục mà bạn muốn lưu file ...");

	/* BIF_RETURNONLYFSDIRS: Only return file system directories. If the user selects folders that are not part of the file system, the OK button is grayed
	* BIF_DONTGOBELOWDOMAIN: Do not include network folders below the domain level in the dialog box's tree view control.
	* BIF_STATUSTEXT: Include a status area in the dialog box. The callback function can set the status text by sending messages to the dialog box. This flag is not supported when BIF_NEWDIALOGSTYLE is specified
	* BIF_NEWDIALOGSTYLE: Use the new user interface. Setting this flag provides the user with a larger dialog box that can be resized. The dialog box has several new capabilities, including: drag-and-drop capability within the dialog box, reordering, shortcut menus, new folders, delete, and other shortcut menu commands.
	* BIF_EDITBOX: Include an edit control in the browse dialog box that allows the user to type the name of an item.
	* BIF_BROWSEINCLUDEFILES: The browse dialog box displays files as well as folders.
	*/
	browinfo.ulFlags = BIF_NONEWFOLDERBUTTON | BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN
		| BIF_USENEWUI;

	// Pointer to an application-defined function that the dialog box calls when an event occurs.
	browinfo.lpfn = BrowseCallbackProc;

	// Displays the selected folder
	if (SHBrowseForFolderW(&browinfo) != NULL)
	{
		return true;
	}
	else
	{
		wcscpy(folder.szFolderPath, L"");
		return false;
	}
}