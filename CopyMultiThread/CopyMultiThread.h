#pragma once

#include "resource.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
static TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
static TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

static HWND hEditChooseFile = NULL;
static HWND hEditSaveFile = NULL;
static HWND hComboThread = NULL;

void OnCreateProcessing(HWND hDlg)
{
	// Get Handle of controls
	hEditChooseFile = GetDlgItem(hDlg, IDC_EDIT_OPENFILE);
	hEditSaveFile = GetDlgItem(hDlg, IDC_EDIT_SAVE_FILE);
	hComboThread = GetDlgItem(hDlg, IDC_COMBO_THREAD);

	// .. Initializing
	SetWindowText(hEditChooseFile, L"Choosing file to copy ...");
	SetWindowText(hEditSaveFile, L"Choosing places to save file ...");
	SetWindowText(hComboThread, L"1");
}

void OnStartCopy(HWND hDlg)
{
	MessageBox(hDlg, L"Bắt đầu copy, vui lòng đợi ...", szTitle, MB_OK);
}
// Message handler for CopyDialog box.
INT_PTR CALLBACK CopyDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		OnCreateProcessing(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:

			EndDialog(hDlg, LOWORD(wParam));
			PostQuitMessage(0);
			return (INT_PTR)TRUE;
		case IDOK:
			OnStartCopy(hDlg);
			break;
		case IDC_BUTTON_FILE:
			break;
		case IDC_BUTTON_SAVE_FILE:
			break;
		}

		break;
	}
	return (INT_PTR)FALSE;
}
