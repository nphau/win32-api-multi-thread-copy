#pragma once

#include "resource.h"
#include <shlobj.h>
#include <commdlg.h>
#include <Shlwapi.h>
#include <windowsx.h>

/* Store infomation of file which is copied */
typedef struct MYFILE
{
	DWORD		dwFileSize;					// File size
	HANDLE		hFileOld;					// Handle of the old file
	HANDLE		hFileNew;					// Handle of the new file
	WCHAR		szFileName[MAX_PATH];		// Only file nane and extension
	WCHAR		szFilePath[MAX_PATH];		// Full file path
	WCHAR		szFolderPath[MAX_PATH];		// Full folder path
	WCHAR		szFolderName[MAX_PATH];		// Only folder name
	WCHAR		szNewFilePath[MAX_PATH];		// Only folder name

} *PMYFILE, MYFILE;

static MYFILE myFile;

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
		wcscpy(myFile.szFolderName, L"");
		wcscpy(myFile.szFolderPath, L"");
		break;
	case BFFM_SELCHANGED:
		SHGetPathFromIDListW((ITEMIDLIST*)lParam, myFile.szFolderPath);
		break;
	}
	return 0;
}

/*
* Choose myFile to save file
*/
bool BrowseFolder(HWND hDlg)
{
	BROWSEINFOW browinfo;
	ZeroMemory(&browinfo, sizeof(browinfo));

	browinfo.hwndOwner = hDlg; // A handle to the owner window for the dialog box.
	browinfo.pidlRoot = NULL; // The location of the root myFile

	// Pointer to a buffer to receive the display name of the myFile selected by the user.
	// The size of this buffer is assumed to be MAX_PATH characters.
	browinfo.pszDisplayName = myFile.szFolderName;

	// Pointer to a null-terminated string that is displayed above the tree view control in the dialog box.
	// This string can be used to specify instructions to the user.
	browinfo.lpszTitle = _T("Vui lòng chọn thư mục mà bạn muốn lưu file ...");

	/* BIF_RETURNONLYFSDIRS: Only return file system directories. If the user selects myFiles that are not part of the file system, the OK button is grayed
	* BIF_DONTGOBELOWDOMAIN: Do not include network myFiles below the domain level in the dialog box's tree view control.
	* BIF_STATUSTEXT: Include a status area in the dialog box. The callback function can set the status text by sending messages to the dialog box. This flag is not supported when BIF_NEWDIALOGSTYLE is specified
	* BIF_NEWDIALOGSTYLE: Use the new user interface. Setting this flag provides the user with a larger dialog box that can be resized. The dialog box has several new capabilities, including: drag-and-drop capability within the dialog box, reordering, shortcut menus, new myFiles, delete, and other shortcut menu commands.
	* BIF_EDITBOX: Include an edit control in the browse dialog box that allows the user to type the name of an item.
	* BIF_BROWSEINCLUDEFILES: The browse dialog box displays files as well as myFiles.
	*/
	browinfo.ulFlags = BIF_NONEWFOLDERBUTTON | BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_USENEWUI;

	// Pointer to an application-defined function that the dialog box calls when an event occurs.
	browinfo.lpfn = BrowseCallbackProc;

	// Displays the selected myFile
	if (SHBrowseForFolderW(&browinfo) != NULL)
	{
		return true;
	}
	else
	{
		wcscpy(myFile.szFolderPath, L"");
		return false;
	}
}

/* The function gets file path */
bool GetFileURL(HWND hWnd, WCHAR fileTitle[], WCHAR filePath[], WCHAR szDlgTitle[])
{
	OPENFILENAME ofn;

	WCHAR szFilePath[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"All file (*.*)\0*.*\0";
	ofn.lpstrFile = szFilePath;

	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.lpstrTitle = szDlgTitle;	// Dialog title
	ofn.lpstrFileTitle = NULL;		// File name and extension
	ofn.nMaxFile = MAX_PATH;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = L"txt";

	if (GetOpenFileName(&ofn))
	{
		wcscpy(filePath, szFilePath);
		wcscpy(fileTitle, szFilePath);
		PathStripPathW(fileTitle);
		return true;
	}
	return false;
}
