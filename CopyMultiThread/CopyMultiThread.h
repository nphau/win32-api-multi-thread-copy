#pragma once

#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
static HINSTANCE hInst;								// current instance
static TCHAR szTitle[MAX_LOADSTRING];				// The title bar text
static TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

static HWND hEditChooseFile = NULL;
static HWND hEditSaveFile = NULL;
static HWND hComboThread = NULL;

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

	ZeroMemory(&folder, sizeof(FOLDER));

	// Initializing default value for combo box
	InitThreadComboBox(hDlg);
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	// Cast the parameter to the correct data type.
	// The pointer is known to be valid because 
	// it was checked for NULL before the thread was created.
	PFOLDER folder = (PFOLDER)lpParam;

	WCHAR src[MAX_PATH] = L"";
	WCHAR dest[MAX_PATH] = L"";

	wcscpy(src,folder->szSourceFile);

	wcscpy(dest, folder->szFolderPath);
	wcscat(dest, L"\\");
	wcscat(dest, folder->szFileName);

	if (CopyFile(src, dest, FALSE))
	{
		MessageBox(NULL, L"Copy thành công", L"ThreadFunc", MB_OK);
	}
	return 0;
}

void StartCopy()
{
	try{
		// Default Priority: TTHREAD_PRIORITY_NORMAL
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, (LPVOID)&folder, 0, 0);
	}
	catch (...)
	{
		GetLastError();
	}
}
void OnStartCopy(HWND hDlg)
{
	if (!wcscmp(folder.szSourceFile, L""))
	{
		MessageBox(hDlg, L"Bạn chưa chọn file để copy!", szTitle, MB_OK | MB_ICONHAND);
		return;
	}
	if (!wcscmp(folder.szFolderPath, L""))
	{
		MessageBox(hDlg, L"Bạn chưa chọn thư mục để lưu file!", szTitle, MB_OK | MB_ICONHAND);
		return;
	}
	StartCopy();
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
			// OnStartCopy(hDlg);
			StartCopy();
			break;
		case IDC_BUTTON_FILE:
			GetFileURL(hDlg, folder.szFileName,folder.szSourceFile, L"Chọn file cần copy");
			SetWindowText(hEditChooseFile, folder.szSourceFile);
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
