#pragma once

#include "resource.h"
#include <commdlg.h>

#define MAX_LOADSTRING 100

// Global Variables:
static HINSTANCE hInst;								// current instance
static TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
static TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

static HWND hEditChooseFile = NULL;
static HWND hEditSaveFile = NULL;
static HWND hComboThread = NULL;

static WCHAR szOpenfile[MAX_PATH] = L"";

void InitThreadComboBox(HWND hDlg)
{
	// .. Initializing

	WCHAR numThreads[5][2] =
	{
		TEXT("1"), TEXT("2"), TEXT("3"), TEXT("4"),
		TEXT("5")
	};

	TCHAR A[16];
	int  k = 0;

	memset(&A, 0, sizeof(A));
	for (k = 0; k < 5; k++)
	{
		wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)numThreads[k]);

		// Add string to combobox.
		SendMessage(hComboThread, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
	}
	SendMessage(hComboThread, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
}
void OnCreateProcessing(HWND hDlg)
{
	// Get Handle of controls
	hEditChooseFile = GetDlgItem(hDlg, IDC_EDIT_OPENFILE);
	hEditSaveFile = GetDlgItem(hDlg, IDC_EDIT_SAVE_FILE);
	hComboThread = GetDlgItem(hDlg, IDC_COMBO_THREAD);

	// Initializing default value for combo box
	InitThreadComboBox(hDlg);
}

void OnStartCopy(HWND hDlg)
{
	MessageBox(hDlg, L"Bắt đầu copy, vui lòng đợi ...", szTitle, MB_OK);
}

void GetFileURL(HWND hWnd, WCHAR path[], WCHAR szDlgTitle[])
{
	OPENFILENAME ofn;

	WCHAR szFile[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"All file (*.*)\0*.*\0";
	ofn.lpstrFile = szFile;

	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.lpstrTitle = szDlgTitle;
	ofn.nMaxFile = MAX_PATH;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = L"txt";

	if (GetOpenFileName(&ofn))
	{
		_tcscpy(path, szFile);
	}
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
			GetFileURL(hDlg, szOpenfile, L"Chọn file cần copy");
			SetWindowText(hEditChooseFile, szOpenfile);
			break;
		case IDC_BUTTON_SAVE_FILE:
			if (BrowseFolder(hDlg))
				SetWindowText(hEditSaveFile, folder.szFolderPath);
			break;
		}

		break;
	}
	return (INT_PTR)FALSE;
}
