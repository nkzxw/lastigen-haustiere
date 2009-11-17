// http://cboard.cprogramming.com/windows-programming/77061-readdirectorychangesw.html


#define _WIN32_WINNT 0x0400
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <shlwapi.h>

#if defined(_MSC_VER)
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "ole32.lib")
#endif

HINSTANCE g_hinst;


typedef void (CALLBACK *FileChangeCallback)(LPTSTR, DWORD, LPARAM);

typedef struct tagDIR_MONITOR
{
	OVERLAPPED ol;
	HANDLE     hDir;
	BYTE       buffer[32 * 1024];
	LPARAM     lParam;
	DWORD      notifyFilter;
	BOOL       fStop;
	FileChangeCallback callback;
} *HDIR_MONITOR;

/* 
 * Unpacks events and passes them to a user defined callback.
 */
VOID CALLBACK MonitorCallback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
	TCHAR                    szFile[MAX_PATH];
	PFILE_NOTIFY_INFORMATION pNotify;
	HDIR_MONITOR             pMonitor  = (HDIR_MONITOR) lpOverlapped;
	size_t                   offset    =  0;
	BOOL RefreshMonitoring(HDIR_MONITOR pMonitor);

	if (dwErrorCode == ERROR_SUCCESS)
	{
		do
		{
			pNotify = (PFILE_NOTIFY_INFORMATION) &pMonitor->buffer[offset];
			offset += pNotify->NextEntryOffset;

#			if defined(UNICODE)
			{
			    lstrcpynW(szFile, pNotify->FileName,
			                min(MAX_PATH, pNotify->FileNameLength / sizeof(WCHAR) + 1));
			}
#			else
			{
			    int count = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName,
			                                    pNotify->FileNameLength / sizeof(WCHAR),
			                                    szFile, MAX_PATH - 1, NULL, NULL);
			    szFile[count] = TEXT('\0');
			}
#			endif

			pMonitor->callback(szFile, pNotify->Action, pMonitor->lParam);

		} while (pNotify->NextEntryOffset != 0);
	}

	if (!pMonitor->fStop)
	{
		RefreshMonitoring(pMonitor);
	}
}

/*
 * Refreshes the directory monitoring.
 */
BOOL RefreshMonitoring(HDIR_MONITOR pMonitor)
{
	//return ReadDirectoryChangesW(pMonitor->hDir, pMonitor->buffer, sizeof(pMonitor->buffer), FALSE, pMonitor->notifyFilter, NULL, &pMonitor->ol, MonitorCallback);
	return ReadDirectoryChangesW(pMonitor->hDir, pMonitor->buffer, sizeof(pMonitor->buffer), FALSE, pMonitor->notifyFilter, NULL, &pMonitor->ol, NULL);
}

/*
 * Stops monitoring a directory.
 */
void StopMonitoring(HDIR_MONITOR pMonitor)
{
	if (pMonitor)
	{
		pMonitor->fStop = TRUE;

		CancelIo(pMonitor->hDir);

		if (!HasOverlappedIoCompleted(&pMonitor->ol))
		{
			SleepEx(5, TRUE);
		}

		CloseHandle(pMonitor->ol.hEvent);
		CloseHandle(pMonitor->hDir);
		HeapFree(GetProcessHeap(), 0, pMonitor);
	}
}

/*
 * Starts monitoring a directory.
 */
HDIR_MONITOR StartMonitoring(LPCTSTR szDirectory, DWORD notifyFilter, FileChangeCallback callback)
{
	HDIR_MONITOR pMonitor = (HDIR_MONITOR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*pMonitor));

	//pMonitor->hDir = CreateFile(szDirectory, FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
	pMonitor->hDir = CreateFile(szDirectory, 1, 7, NULL, 3, 0x42000000, NULL);

	//pMonitor->hDir = CreateFile(strDirToWatch, 
	//							FILE_LIST_DIRECTORY, 
	//							FILE_SHARE_READ | FILE_SHARE_WRITE ,//| FILE_SHARE_DELETE, <-- removing FILE_SHARE_DELETE prevents the user or someone else from renaming or deleting the watched directory. This is a good thing to prevent.
	//							NULL, //security attributes
	//							OPEN_EXISTING,
	//							FILE_FLAG_BACKUP_SEMANTICS | //<- the required priviliges for this flag are: SE_BACKUP_NAME and SE_RESTORE_NAME.  CPrivilegeEnabler takes care of that.
 //                               FILE_FLAG_OVERLAPPED, //OVERLAPPED!
	//							NULL);


	if (pMonitor->hDir != INVALID_HANDLE_VALUE)
	{
		pMonitor->ol.hEvent    = CreateEvent(NULL, TRUE, FALSE, NULL);
		pMonitor->notifyFilter = notifyFilter;
		pMonitor->callback     = callback;

		if (RefreshMonitoring(pMonitor))
		{
			return pMonitor;
		}
		else
		{
			CloseHandle(pMonitor->ol.hEvent);
			CloseHandle(pMonitor->hDir);
		}
	}

	HeapFree(GetProcessHeap(), 0, pMonitor);
	return NULL;
}

/*
 * Runs a message loop that allows APCs to be despatched.
 */
int RunAPCMessageLoop(void)
{
	BOOL done = FALSE;
	MSG  msg  = { 0 };

	while (!done)
	{
		/* Wait for either an APC or a message. */
		while (WAIT_IO_COMPLETION == 
		        MsgWaitForMultipleObjectsEx(0, NULL, INFINITE, QS_ALLINPUT, MWMO_ALERTABLE)) /* Do nothing */;

		/* One or more messages have arrived. */
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = TRUE;
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}


  
void CALLBACK FileCallback(LPTSTR szFile, DWORD action, LPARAM lParam)
{
	/* Shouldn't call MessageBox as it will block new notifications coming in. */
	MessageBox(NULL, szFile, NULL, 0);
}

HDIR_MONITOR h;

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpcs)
{
	h = StartMonitoring(TEXT("C:\\Documents and Settings\\USER\\My Documents"), FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_FILE_NAME, FileCallback);

	return TRUE;
}

void OnDestroy(HWND hwnd)
{
	if (h) StopMonitoring(h);

	PostQuitMessage(0);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uiMsg) 
    {
        HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
    }

    return DefWindowProc(hwnd, uiMsg, wParam, lParam);
}

BOOL InitApp(void)
{
    WNDCLASS wc = { 0 };

    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = g_hinst;
    wc.hIcon         = NULL;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = TEXT("Scratch");

    if (!RegisterClass(&wc)) return FALSE;

    return TRUE;
}


  
int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hinstPrev,
                   LPSTR lpCmdLine, int nShowCmd)
{
    HWND hwnd;
    int  result;

    g_hinst = hinst;

    if (!InitApp()) return 0;

    hwnd = CreateWindow(
            TEXT("Scratch"),                      /* Class Name */
            TEXT("Scratch"),                      /* Title */
            WS_OVERLAPPEDWINDOW,                  /* Style */
            CW_USEDEFAULT, CW_USEDEFAULT,         /* Position */
            CW_USEDEFAULT, CW_USEDEFAULT,         /* Size */
            NULL,                                 /* Parent */
            NULL,                                 /* No menu */
            hinst,                                /* Instance */
            0);                                   /* No special parameters */

    ShowWindow(hwnd, nShowCmd);

    result = RunAPCMessageLoop();

    return result;
}
