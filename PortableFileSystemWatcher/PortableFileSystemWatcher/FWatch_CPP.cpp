//http://www.experts-exchange.com/Programming/Languages/CPP/Q_22507220.html

//TODO: NULL


// FWatch_CPP.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <windows.h>

#include "Win32ApiWrapper.hpp"

//Main source file for sample demonstrating use of file change
//notification APIs.
//
//Functions:
//
//CheckChangedFile()      - Gets and displays change information
//HandleDirectoryChange() - Watch function
//WatchDirectories()      - Starts the watch
//main()                  - Program main
//
//
//Written by Microsoft Product Support Services, Windows Developer Support.
//Copyright 1996-1997 Microsoft Corporation. All rights reserved.


#define MAX_DIRS    10
#define MAX_FILES   255
#define MAX_BUFFER  4096


// this is the all purpose structure that contains  
//// the interesting directory information and provides
//// the input buffer that is filled with file change data
//typedef struct _DIRECTORY_INFO 
//{
//      HANDLE      directoryHandle;
//      TCHAR       directoryName[MAX_PATH];
//      CHAR        buffer[MAX_BUFFER];
//      DWORD       bufferLength;
//      OVERLAPPED  overlapped;
//} DIRECTORY_INFO, *PDIRECTORY_INFO, *LPDIRECTORY_INFO;
//DIRECTORY_INFO  directoryInfo[MAX_DIRS];        // Buffer for all of the directories
////TCHAR           FileList[MAX_FILES*MAX_PATH];   // Buffer for all of the files

DWORD           numDirs;	// TODO: global... hay que sacarla...





struct DirectoryInfo
{
	void* directoryHandle;				//      hDir;
	std::string directoryName;			// TCHAR       lpszDirName[MAX_PATH];
	char buffer[MAX_BUFFER];			//CHAR        lpBuffer[MAX_BUFFER];
	unsigned long bufferLength;			//DWORD       dwBufLength;
	OVERLAPPED  overlapped;				//OVERLAPPED  Overlapped;
};
//std::vector<DirectoryInfo> directoryInfo;
std::vector<DirectoryInfo*> directoryInfo;

/**********************************************************************
CheckChangedFile()

Purpose:
This function prints out information when one of the files we are watching is changed.

Parameters:

LPDIRECTORY_INFO lpdi - Information about the watched directory
PFILE_NOTIFY_INFORMATION lpfni - Information about modified file


Return Value:
None

Comments:

********************************************************************/
//void WINAPI CheckChangedFile( LPDIRECTORY_INFO lpdi, PFILE_NOTIFY_INFORMATION lpfni)
//{
//      TCHAR      szFullPathName[MAX_PATH];
//      TCHAR      *p;
//      HANDLE     hFile;
//      FILETIME   LocalFileTime;
//      SYSTEMTIME SystemTime;
//      BY_HANDLE_FILE_INFORMATION FileInfo;
//
//      p = FileList;
//
//      while(*p && lstrcmpi(p, (char*)lpfni->FileName))
//	  {
//            p+=(lstrlen(p)+1);
//	  }
//
//      if (*p)
//      {
//            lstrcpy( szFullPathName, lpdi->lpszDirName );
//			lstrcat( szFullPathName, "\\" );
//            lstrcat( szFullPathName, (LPSTR)lpfni->FileName );
//
//            // we assume that the file was changed, since  that is the only thing we look for in this sample
//            //wprintf( L"%s changed,", szFullPathName );
//			std::cout << szFullPathName << " changed,";
//
//            hFile=CreateFile( szFullPathName,
//                  GENERIC_READ,
//                  FILE_SHARE_READ,
//                  NULL,
//                  OPEN_EXISTING,
//                  FILE_FLAG_SEQUENTIAL_SCAN,
//                  0);
//
//            GetFileInformationByHandle( hFile, &FileInfo );
//
//            FileTimeToLocalFileTime( &(FileInfo.ftLastWriteTime), &LocalFileTime );
//
//            FileTimeToSystemTime( &LocalFileTime, &SystemTime );
//
//            //wprintf( L" Size = %d bytes,", FileInfo.nFileSizeLow );
//			std::cout << " Size = " << FileInfo.nFileSizeLow << " bytes,";
//
//			//TODO:
//            wprintf( L" Last Access = %02d/%02d/%02d %02d:%02d:%02d",
//                  SystemTime.wMonth,
//                  SystemTime.wDay,
//                  SystemTime.wYear,
//                  SystemTime.wHour,
//                  SystemTime.wMinute,
//                  SystemTime.wSecond );
//
//            CloseHandle( hFile );
//
//            //wprintf( L"\n" );
//			std::cout << std::endl;
//      }
//}


/**********************************************************************
HandleDirectoryChange()

Purpose:
This function receives notification of directory changes and calls CheckChangedFile() to display the actual changes. After
notification and processing, this function calls ReadDirectoryChangesW to reestablish the watch.

Parameters:

HANDLE completionPortHandle - Handle for completion port


Return Value:
None

Comments:

********************************************************************/
void WINAPI HandleDirectoryChange( unsigned long completionPort )
{
      DWORD numBytes;
      DWORD cbOffset;
      //LPDIRECTORY_INFO di;
	  DirectoryInfo* di;
      LPOVERLAPPED lpOverlapped;
      PFILE_NOTIFY_INFORMATION fni;

      do
      {
            // Retrieve the directory info for this directory through the completion key
            GetQueuedCompletionStatus
				( 
				  (HANDLE) completionPort,
                  &numBytes,
                  (LPDWORD) &di,      // This is the DIRECTORY_INFO structure that was passed in the call to CreateIoCompletionPort below.
                  &lpOverlapped,
                  INFINITE
				 );

            if ( di )
            {
				std::cout << "Notify..." << std::endl;

                  //fni = (PFILE_NOTIFY_INFORMATION)di->lpBuffer;
				  fni = (PFILE_NOTIFY_INFORMATION)di->buffer;

                  do
                  {
                        cbOffset = fni->NextEntryOffset;

                        //if( fni->Action == FILE_ACTION_MODIFIED )
                        //      CheckChangedFile( di, fni );

                        //
                        switch ( fni->Action )
                        {
							case FILE_ACTION_ADDED:
								  //wprintf(L"file added: ");
								std::cout << "file added: ";
								break;
							case FILE_ACTION_REMOVED:
								//wprintf(L"file deleted: ");
								std::cout << "file deleted: ";
								break;
							case FILE_ACTION_MODIFIED:
								std::cout << "time stamp or attribute changed: ";
								break;
							case FILE_ACTION_RENAMED_OLD_NAME:
								//wprintf(L"file name changed - old name: ");
								std::cout << "file name changed - old name: ";
								break;
							case FILE_ACTION_RENAMED_NEW_NAME:
								//wprintf(L"file name changed - new name: ");
								std::cout << "file name changed - new name: ";
								break;
							default: 
								std::cout << "unknown event: ";
								break;
                        }

						std::string fileName(fni->FileName, fni->FileName + (fni->FileNameLength/sizeof(WCHAR)) ); 
						std::cout << fileName << std::endl;

                        fni = (PFILE_NOTIFY_INFORMATION)((LPBYTE) fni + cbOffset);

                  } while( cbOffset );

                  // Reissue the watch command
      //            ReadDirectoryChangesW
					 // ( 
						//di->hDir,
						//di->lpBuffer,
      //                  MAX_BUFFER,
      //                  TRUE,
      //                  FILE_NOTIFY_CHANGE_LAST_WRITE,
      //                  &di->dwBufLength,
      //                  &di->Overlapped,
      //                  NULL);

				  ReadDirectoryChangesW
					  ( 
					  di->directoryHandle,						
					  di->buffer,                                
					  MAX_BUFFER,                               
					  TRUE,                                     
					  FILE_NOTIFY_CHANGE_LAST_WRITE,            
					  &di->bufferLength,                        
					  &di->overlapped,                          
					  NULL);                                    

            }

      } while( di );


}


/**********************************************************************
WatchDirectories()

Purpose:
This function implements the ReadDirectoryChangesW API for
indicated directories. For each directory watched, a thread
is created which waits for changes to the directory. This
function waits for the user to input 'q', then cleans up and
quits.

Parameters:

HANDLE completionPortHandle - Handle for completion port


Return Value:
None

********************************************************************/
void WINAPI WatchDirectories( HANDLE completionPortHandle )
{
      //DWORD   i;
      DWORD   tid;
      HANDLE  hThread;


      // Start watching each of the directories of interest

      for ( int i=0; i<numDirs; ++i )
      {
		  //DirInfo[i].hDir,						 // HANDLE TO DIRECTORY
			
		  

            ReadDirectoryChangesW
				( 
					directoryInfo[i]->directoryHandle,						 // HANDLE TO DIRECTORY
					directoryInfo[i]->buffer,                                                      // Formatted buffer into which read results are returned.  This is a
                  MAX_BUFFER,                                                            // Length of previous parameter, in bytes
                  TRUE,                                                                              // Monitor sub trees?
                  FILE_NOTIFY_CHANGE_LAST_WRITE,            // What we are watching for
                  &directoryInfo[i]->bufferLength,                                    // Number of bytes returned into second parameter
                  &directoryInfo[i]->overlapped,                                          // OVERLAPPED structure that supplies data to be used during an asynchronous operation.  If this is NULL, ReadDirectoryChangesW does not return immediately.
                  NULL);                                                                        // Completion routine
      }

      // Create a thread to sit on the directory changes

      hThread = CreateThread( NULL,
            0,
            (LPTHREAD_START_ROUTINE) HandleDirectoryChange,
            completionPortHandle,
            0,
            &tid);

      // Just loop and wait for the user to quit

	  char chr;
      /*while (getch() != 'q');*/

		do
		{
			std::cin.sync();
			std::cin.get( chr );
		} while (chr != 'q' );

      // The user has quit - clean up

      PostQueuedCompletionStatus( completionPortHandle, 0, 0, NULL );

      // Wait for the Directory thread to finish before exiting

      WaitForSingleObject( hThread, INFINITE );

      CloseHandle( hThread );
}


/**********************************************************************
main()

Purpose:
Main entry-point for fwatch sample. This function reads an .ini
file (fwatch.ini) to determine which directories and files to
watch. For each directory watched some information is gathered
and stored.

Return Value:  (see errors.h for description)
None

Comments:

********************************************************************/
int main(int argc, char** argv)
{
	//HANDLE  hCompPort=NULL;                 // Handle To a Completion Port
	void* completionPortHandle = 0;
	std::vector<std::string> dirList;

	  //----------------------------------------------------------------------------------------

	  //TCHAR   IniFile[MAX_PATH];
	  //TCHAR   FilePath[MAX_PATH];
	  //HANDLE  hFile;

	  //   GetCurrentDirectory( MAX_PATH, IniFile );

	  //lstrcat( IniFile, "\\fwatch.ini" );

	  //GetPrivateProfileString( "Directories",
		 // NULL,
		 // NULL,
		 // DirList,
		 // MAX_DIRS*MAX_PATH,
		 // IniFile );
	  //GetPrivateProfileString( "Files",
		 // NULL,
		 // NULL,
		 // FileList,
		 // MAX_FILES*MAX_PATH,
		 // IniFile );

	  dirList.push_back("C:\\temp1");
	  dirList.push_back("C:\\temp2");
	  numDirs = dirList.size();
	  //----------------------------------------------------------------------------------------



      //wprintf( L"Watching these directories:\n" );
	  std::cout << "Watching these directories:" << std::endl;

      // First, walk through the raw list and count items, creating  an array of handles for each directory
   //   for ( p=DirList; *p!='\0'; numDirs++, p+=(lstrlen(p) + 1) )    // Each string is appended with a NULL, so we skip this with the +1
   //   {

   //         if( CreateDirectory( p, NULL ) )
			//{
			//	//wprintf( L"Directory %s created\n", p );
			//	std::cout << "Directory " << p << " created" << std::endl;
			//}
   //         else
			//{
   //               //wprintf( L"Directory %s exists\n", p );
			//	  std::cout << "Directory " << p << " exists" << std::endl;
			//}

   //         // Get a handle to the directory
   //         DirInfo[numDirs].hDir = CreateFile( p,
   //               FILE_LIST_DIRECTORY,
   //               FILE_SHARE_READ |
   //               FILE_SHARE_WRITE |
   //               FILE_SHARE_DELETE,
   //               NULL,
   //               OPEN_EXISTING,
   //               FILE_FLAG_BACKUP_SEMANTICS |
   //               FILE_FLAG_OVERLAPPED,
   //               NULL);

   //         if( DirInfo[numDirs].hDir == INVALID_HANDLE_VALUE )
   //         {
			//	//wprintf( L"Unable to open directory %s. GLE=%d. Terminating...\n", p, GetLastError() );
			//	std::cout << "Unable to open directory " << p << ". GLE=" << GetLastError() << ". Terminating..." << std::endl; 
			//	exit( 0 );
   //         }

   //         for (q=FileList;*q!='\0';q+=(lstrlen(q)+1))      // Each string is appended with a NULL, so we skip this with the +1
   //         {
   //               lstrcpy( FilePath, p );
   //               //lstrcat( FilePath, L"\\" );
			//	  lstrcat( FilePath, "\\" );
   //               lstrcat( FilePath, q );

   //               if( hFile = CreateFile( FilePath,
   //                     GENERIC_WRITE,
   //                     FILE_SHARE_READ |
   //                     FILE_SHARE_WRITE |
   //                     FILE_SHARE_DELETE,
   //                     NULL,
   //                     CREATE_ALWAYS,
   //                     FILE_ATTRIBUTE_NORMAL,
   //                     NULL) )
   //               {
			//			//wprintf( L"  File %s created\n", FilePath );
			//			std::cout << "  File " << FilePath << " created" << std::endl;
			//			CloseHandle( hFile );
   //               }
   //               else
			//	  {
   //                     wprintf( L"  File %s could not be created\n", FilePath );
			//			std::cout << "  File " << FilePath << " could not be created" << std::endl;
			//	  }

   //         }

   //         lstrcpy( DirInfo[numDirs].lpszDirName, p );

   //         // Set up a key(directory info) for each directory
   //         completionPortHandle=CreateIoCompletionPort( DirInfo[numDirs].hDir,
   //               completionPortHandle,
   //               (DWORD) &DirInfo[numDirs],
   //               0);

   //   }

	  int i = 0;
	  for ( std::vector<std::string>::const_iterator it = dirList.begin(); it < dirList.end(); ++it, ++i )
	  {
		  //TODO: usar boost::filesystem
		  if ( Win32ApiWrapper::CreateDirectory( *it, NULL ) )
		  {
			  std::cout << "Directory " << *it << " created" << std::endl;
		  }
		  else
		  {
			  std::cout << "Directory " << *it << " exists" << std::endl;
		  }

		  // Get a handle to the directory

		//DirectoryInfo tempDi;
		DirectoryInfo* tempDi = new DirectoryInfo;
		  directoryInfo.push_back(tempDi);



		//tempDi.directoryHandle =  Win32ApiWrapper::CreateFile
		directoryInfo[i]->directoryHandle = Win32ApiWrapper::CreateFile
			  ( 
			  *it,
			  FILE_LIST_DIRECTORY,
			  FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			  NULL,
			  OPEN_EXISTING,
			  FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
			  NULL
			  );

		//if ( tempDi.directoryHandle == INVALID_HANDLE_VALUE )
		if ( directoryInfo[i]->directoryHandle == INVALID_HANDLE_VALUE )
		{
			std::cout << "Unable to open directory " << *it << ". GLE=" << GetLastError() << ". Terminating..." << std::endl; 
			return -1; //exit( 0 );
		}

		  tempDi->directoryName = *it;
		  //lstrcpy( directoryInfo[i].directoryName, (*it).c_str() );



		  //unsigned long addr = (unsigned long) &directoryInfo[i];
		  unsigned long addr = (unsigned long) directoryInfo[i];
		  addr = (unsigned long) tempDi;



		  // Set up a key(directory info) for each directory
		  completionPortHandle=CreateIoCompletionPort
			  ( 
			  directoryInfo[i]->directoryHandle,
			  completionPortHandle,
			  (unsigned long) addr,
			  0
			  );

		  //(unsigned long) addr,

		  

		  //completionPortHandle=CreateIoCompletionPort
			 // ( 
			 // DirInfo[i].hDir,
			 // completionPortHandle,
			 // (DWORD) &DirInfo[i],
			 // 0
			 // );



	  }



      //wprintf( L"\n\nPress <q> to exit\n" );
	  std::cout << std::endl << std::endl << "Press <q> to exit" << std::endl;

      // Start watching
	WatchDirectories( completionPortHandle );

	//for (int i=0; i<numDirs; ++i)
	//{
	//	CloseHandle( directoryInfo[i].directoryHandle );
	//}

	for ( std::vector<DirectoryInfo*>::const_iterator it = directoryInfo.begin(); it != directoryInfo.end(); ++it )
	{
		::CloseHandle( (*it)->directoryHandle );
		// delete (*it);
		//::CloseHandle( it->directoryHandle );
	}

	CloseHandle( completionPortHandle );

}
