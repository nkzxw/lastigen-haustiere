//// CreateIoCompletionPort
//// PostQueuedCompletionStatus
//// GetQueuedCompletionStatus
//
//
//#include <iostream>
//#include <string>
//
//#include "windows.h"
//
//
//int main(int argc, char** argv)
//{
//
//	//-------------------- Win32 API Definitions
//
//	//HANDLE WINAPI CreateFile(
//	//	__in      LPCTSTR lpFileName,
//	//	__in      DWORD dwDesiredAccess,
//	//	__in      DWORD dwShareMode,
//	//	__in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
//	//	__in      DWORD dwCreationDisposition,
//	//	__in      DWORD dwFlagsAndAttributes,
//	//	__in_opt  HANDLE hTemplateFile
//	//	);
//
//	//public const int FILE_LIST_DIRECTORY = 1;
//	//public const int FILE_SHARE_DELETE = 4;
//	//public const int FILE_SHARE_READ = 1;
//	//public const int FILE_SHARE_WRITE = 2;
//	//public const int OPEN_EXISTING = 3;
//	//public const int FILE_FLAG_BACKUP_SEMANTICS = 0x2000000;
//	//public const int FILE_FLAG_OVERLAPPED = 0x40000000;
//
//	//BOOL WINAPI ReadDirectoryChangesW(
//	//	__in         HANDLE hDirectory,
//	//	__out        LPVOID lpBuffer,
//	//	__in         DWORD nBufferLength,
//	//	__in         BOOL bWatchSubtree,
//	//	__in         DWORD dwNotifyFilter,
//	//	__out_opt    LPDWORD lpBytesReturned,
//	//	__inout_opt  LPOVERLAPPED lpOverlapped,
//	//	__in_opt     LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
//	//	);
//
//	// ------------------------------------- DotNet Code 
//
//	//directoryHandle = NativeMethods.CreateFile(
//	//	directory,            // Directory name
//	//	UnsafeNativeMethods.FILE_LIST_DIRECTORY,           // access (read-write) mode
//	//	UnsafeNativeMethods.FILE_SHARE_READ |
//	//	UnsafeNativeMethods.FILE_SHARE_DELETE |
//	//	UnsafeNativeMethods.FILE_SHARE_WRITE,          // share mode 
//	//	null,                                              // security descriptor 
//	//	UnsafeNativeMethods.OPEN_EXISTING,                 // how to create
//	//	UnsafeNativeMethods.FILE_FLAG_BACKUP_SEMANTICS |
//	//	UnsafeNativeMethods.FILE_FLAG_OVERLAPPED,      // file attributes
//	//	new Microsoft.Win32.SafeHandles.SafeFileHandle(IntPtr.Zero, false)             // file with attributes to copy
//	//	);
//
//
//	//bool ok;
//
//	//ok = UnsafeNativeMethods.ReadDirectoryChangesW(
//	//	directoryHandle,
//	//	new HandleRef(this, bufferPtr),
//	//	internalBufferSize,
//	//	includeSubdirectories ? 1 : 0,
//	//	(int)notifyFilters,
//	//	out size,
//	//	overlappedPointer,
//	//	NativeMethods.NullHandleRef);
//
//	//public enum NotifyFilters
//	//{
//	//	Attributes = 4,
//	//	CreationTime = 0x40,
//	//	DirectoryName = 2,
//	//	FileName = 1,
//	//	LastAccess = 0x20,
//	//	LastWrite = 0x10,
//	//	Security = 0x100,
//	//	Size = 8
//	//}
//
//	//----------------------- C++ Sample Code
//
//	//HANDLE hDir = CreateFile(
//	//  strDirToWatch, 
//	//	FILE_LIST_DIRECTORY, 
//	//	FILE_SHARE_READ | FILE_SHARE_WRITE ,//| FILE_SHARE_DELETE, <-- removing FILE_SHARE_DELETE prevents the user or someone else from renaming or deleting the watched directory. This is a good thing to prevent.
//	//	NULL, //security attributes
//	//	OPEN_EXISTING,
//	//	FILE_FLAG_BACKUP_SEMANTICS | //<- the required priviliges for this flag are: SE_BACKUP_NAME and SE_RESTORE_NAME.  CPrivilegeEnabler takes care of that.
//	//	FILE_FLAG_OVERLAPPED, //OVERLAPPED!
//	//	NULL);
//	//if( hDir == INVALID_HANDLE_VALUE )
//	//{
//	//	DWORD dwError = GetLastError();
//	//	TRACE(_T("CDirectoryChangeWatcher::WatchDirectory() -- Couldn't open directory for monitoring. %d\n"), dwError);
//	//	::SetLastError(dwError);//who knows if TRACE will cause GetLastError() to return success...probably won't, but set it manually just for fun.
//	//	return dwError;
//	//}
//
//
//	//#define READ_DIR_CHANGE_BUFFER_SIZE 4096
//	//HANDLE      m_hDir
//	//CHAR        m_Buffer[ READ_DIR_CHANGE_BUFFER_SIZE ];//buffer for ReadDirectoryChangesW
//	//BOOL		m_bWatchSubDir;
//	//DWORD		m_dwChangeFilter;
//	//DWORD       m_dwBufLength;//length or returned data from ReadDirectoryChangesW -- ignored?...
//	//OVERLAPPED  m_Overlapped;
//
//	//if( !ReadDirectoryChangesW( 
//	//  pdi->m_hDir,
//	//	pdi->m_Buffer,//<--FILE_NOTIFY_INFORMATION records are put into this buffer
//	//	READ_DIR_CHANGE_BUFFER_SIZE,
//	//	pdi->m_bWatchSubDir,
//	//	pdi->m_dwChangeFilter,
//	//	&pdi->m_dwBufLength,//this var not set when using asynchronous mechanisms...
//	//	&pdi->m_Overlapped,
//	//	NULL) )//no completion routine!
//
//	//if( !ReadDirectoryChangesW( 
//	//  pdi->m_hDir,
//	//	pdi->m_Buffer + dwReadBuffer_Offset,//<--FILE_NOTIFY_INFORMATION records are put into this buffer 
//	//	READ_DIR_CHANGE_BUFFER_SIZE - dwReadBuffer_Offset,
//	//	pdi->m_bWatchSubDir,
//	//	pdi->m_dwChangeFilter,
//	//	&pdi->m_dwBufLength,//this var not set when using asynchronous mechanisms...
//	//	&pdi->m_Overlapped,
//	//	NULL) )//no completion routine!
//
//
//
//	//----------------------- C++ Definitive Code
//
//	//std::string directory = "C:\\Documents and Settings\\807831\\My Documents";
//	LPCTSTR directory = TEXT("C:\\Documents and Settings\\807831\\My Documents");
//
//	HANDLE directoryHandle; // private SafeFileHandle directoryHandle;
//	
//	directoryHandle = CreateFile(
//		directory,
//		FILE_LIST_DIRECTORY,
//		FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE,
//		NULL,																
//		OPEN_EXISTING, 
//		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
//		NULL // new Microsoft.Win32.SafeHandles.SafeFileHandle(IntPtr.Zero, false)             // file with attributes to copy
//		);
//
//	if( directoryHandle == INVALID_HANDLE_VALUE )
//	{
//		std::cout << "ERROR" << std::endl;
//	}
//
//
//	bool ok;
//	#define READ_DIR_CHANGE_BUFFER_SIZE 4096
//	CHAR	m_Buffer[ READ_DIR_CHANGE_BUFFER_SIZE ];	//buffer for ReadDirectoryChangesW
//	BOOL	m_bWatchSubDir = 1;
//	DWORD	m_dwChangeFilter;
//	DWORD   m_dwBufLength;
//
//	OVERLAPPED  m_Overlapped;
//	memset(&m_Overlapped, 0, sizeof(m_Overlapped));
//
//	//watcher.NotifyFilter = FPTempTest._System.IO.NotifyFilters.LastAccess | FPTempTest._System.IO.NotifyFilters.LastWrite | FPTempTest._System.IO.NotifyFilters.FileName | FPTempTest._System.IO.NotifyFilters.DirectoryName;
//	m_dwChangeFilter = FILE_NOTIFY_CHANGE_LAST_ACCESS | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME;
//
//	PostQueuedCompletionStatus(hCompPort, sizeof(this), (DWORD)this, &m_Overlapped);//make the thread waiting on GetQueuedCompletionStatus() wake up
//
//
//
//	ok = ReadDirectoryChangesW(
//		directoryHandle,
//		m_Buffer,							//new HandleRef(this, bufferPtr),
//		READ_DIR_CHANGE_BUFFER_SIZE,		//internalBufferSize,
//		m_bWatchSubDir,						//includeSubdirectories ? 1 : 0,
//		m_dwChangeFilter,					//(int)notifyFilters,
//		&m_dwBufLength,						//out size,
//		&m_Overlapped,						//overlappedPointer,
//		NULL								//NativeMethods.NullHandleRef
//		);
//
//
//
//
//
//
//
//
//
//
//
//
//
//}