#pragma once

#include "resource.h"
#include <vector>
#include "StrLib.h"
#include "ThreadLib.h"

#define MAX_LOADSTRING 100

// Global Variables:
static HINSTANCE hInst;								// current instance
static TCHAR szTitle[MAX_LOADSTRING];				// The title bar text
static TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

static HWND hEditChooseFile = NULL;
static HWND hEditSaveFile = NULL;
static HWND hComboThread = NULL;

static HWND hDialog = NULL;

static std::vector<HWND> hThreadUI;

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
	SendMessage(hComboThread, CB_SETCURSEL, (WPARAM)1, (LPARAM)0);
}

void InitThreadUIProgress(HWND hDlg)
{

	hThreadUI.push_back(GetDlgItem(hDlg, IDC_Pro1));
	SendDlgItemMessage(hDlg, IDC_Pro1, PBM_SETPOS, 1, 0);
	SendDlgItemMessage(hDlg, IDC_Pro1, PBM_SETBARCOLOR, 0, RGB(255, 0, 0));

	hThreadUI.push_back(GetDlgItem(hDlg, IDC_Pro2));
	SendDlgItemMessage(hDlg, IDC_Pro2, PBM_SETPOS, 1, 0);
	SendDlgItemMessage(hDlg, IDC_Pro2, PBM_SETBARCOLOR, 0, RGB(0, 255, 0));


	hThreadUI.push_back(GetDlgItem(hDlg, IDC_Pro3));
	SendDlgItemMessage(hDlg, IDC_Pro3, PBM_SETPOS, 1, 0);
	SendDlgItemMessage(hDlg, IDC_Pro3, PBM_SETBARCOLOR, 0, RGB(0, 0, 255));

	hThreadUI.push_back(GetDlgItem(hDlg, IDC_Pro4));
	SendDlgItemMessage(hDlg, IDC_Pro4, PBM_SETPOS, 1, 0);
	SendDlgItemMessage(hDlg, IDC_Pro4, PBM_SETBARCOLOR, 0, RGB(255, 255, 0));

	hThreadUI.push_back(GetDlgItem(hDlg, IDC_Pro5));
	SendDlgItemMessage(hDlg, IDC_Pro5, PBM_SETPOS, 1, 0);
	SendDlgItemMessage(hDlg, IDC_Pro5, PBM_SETBARCOLOR, 0, RGB(255, 0, 255));

}
void OnCreate(HWND hDlg)
{

	// Get Handle of controls
	hEditChooseFile = GetDlgItem(hDlg, IDC_EDIT_OPENFILE);
	hEditSaveFile = GetDlgItem(hDlg, IDC_EDIT_SAVE_FILE);
	hComboThread = GetDlgItem(hDlg, IDC_COMBO_THREAD);

	ZeroMemory(&myFile, sizeof(MYFILE));

	// Initializing default value for combo box
	InitThreadComboBox(hDlg);

	//
	InitThreadUIProgress(hDlg);
}
/**/
bool OnStartCopy(HWND hDlg)
{
	if (!wcscmp(myFile.szFilePath, L""))
	{
		MessageBox(hDlg, L"Bạn chưa chọn file để copy!", szTitle, MB_OK | MB_ICONHAND);
		return false;
	}
	if (!wcscmp(myFile.szFolderPath, L""))
	{
		MessageBox(hDlg, L"Bạn chưa chọn thư mục để lưu file!", szTitle, MB_OK | MB_ICONHAND);
		return false;
	}

	// Kiểm tra dung lượng đĩa hiện tại
	DWORD dwSectPerClust = 0, dwBytesPerSect = 0, dwFreeClusters = 0, dwTotalClusters = 0;
	if (GetDiskFreeSpace(myFile.szFolderPath,
		&dwSectPerClust,
		&dwBytesPerSect,
		&dwFreeClusters,
		&dwTotalClusters))
	{
		DWORD i64FreeBytes = (__int64)dwFreeClusters * dwSectPerClust * dwBytesPerSect;
		__int64 iDelta = i64FreeBytes - myFile.dwFileSize;
		if (iDelta <= 0)
		{
			MessageBox(hDlg, 
				L"Đĩa không còn trống. Vui lòng xoá bớt file để copy", szTitle, MB_OK);
			return false;
		}
	}

	//
	HANDLE hThread = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)InitCopyThreadsProc,
		NULL, 0, 0);

	return true;
}

/**/
bool OnSelectFileToCopy(HWND hDlg)
{
	if (GetFileURL(hDlg, myFile.szFileName, myFile.szFilePath,
		L"Chọn file cần copy"))
	{
		SetWindowText(hEditChooseFile, myFile.szFilePath);
		if (GetMyFileInfo())
		{
			SetWindowText(GetDlgItem(hDlg, IDC_FSIZE), GetWcharFrom(L"Size: ", myFile.dwFileSize, L" byte(s)"));
			SetWindowText(GetDlgItem(hDlg, IDC_FNAME), GetWcharFrom(L"Filename: ", myFile.szFileName, L""));
		}
	}
	return true;
}
/**/
bool OnSelectFolderToSaveFile(HWND hDlg)
{
	if (BrowseFolder(hDlg))
	{
		SetWindowText(hEditSaveFile, myFile.szFolderPath);

		WCHAR dest[MAX_PATH] = L"";
		wcscpy(myFile.szNewFilePath, myFile.szFolderPath);
		wcscat(myFile.szNewFilePath, L"\\");
		wcscat(myFile.szNewFilePath, myFile.szFileName);

	}
	return true;
}