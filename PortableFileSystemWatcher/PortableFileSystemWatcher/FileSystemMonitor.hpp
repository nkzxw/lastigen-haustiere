#ifndef FileSystemMonitor_h__ //TODO:cambiar
#define FileSystemMonitor_h__

#include <string>

#include <boost/function.hpp>

#include "Win32ApiWrapper.hpp"





//enum NotifyFilters
namespace NotifyFilters
{
	static const int FileName		= 0x00000001;  //1;
	static const int DirectoryName	= 0x00000002;  //2;
	static const int Attributes		= 0x00000004;  //4;
	static const int Size			= 0x00000008;  //8;
	static const int LastWrite		= 0x00000010;  //16;
	static const int LastAccess		= 0x00000020;  //32;
	static const int CreationTime	= 0x00000040;  //64;
	static const int Security		= 0x00000100;  //256;
}


//TODO;
struct FileSystemEventArgs
{
	std::string Name;
};


//TODO;
struct RenamedEventArgs
{
	std::string Name;
};

//Similar a un C# Delegate
//TODO: falta el object sender que podría ser implementado mediante boost::any
typedef boost::function<void (FileSystemEventArgs e)> FileSystemEventHandler;
typedef boost::function<void (RenamedEventArgs e)> RenamedEventHandler;



//----------------------------------------------------------------------------------------------------------------
// TODO: Legacy-Code, me gustaría reemplazarlo por código C++
// WIN32

#define MAX_BUFFER  4096

typedef struct _DIRECTORY_INFO 
{
	HANDLE      directoryHandle;
	TCHAR       directoryName[MAX_PATH];
	CHAR        buffer[MAX_BUFFER];
	DWORD       bufferLength;
	OVERLAPPED  overlapped;
} DIRECTORY_INFO, *PDIRECTORY_INFO, *LPDIRECTORY_INFO;

//DIRECTORY_INFO  directoryInfo[MAX_DIRS];        // Buffer for all of the directories
DIRECTORY_INFO  directoryInfo;        // Buffer for all of the directories
//TCHAR           FileList[MAX_FILES*MAX_PATH];   // Buffer for all of the files

DWORD           numDirs;	// TODO: global... hay que sacarla...




//----------------------------------------------------------------------------------------------------------------

class FileSystemMonitor; //Forward declaration

namespace internal
{
	struct ThreadObjectParameter
	{
		FileSystemMonitor* This;
		void* CompletionPortHandle;
	};
}
	

class FileSystemMonitor
{
public:
	FileSystemMonitor()
		: completionPortHandle_(0)
	{
		threadObject_ = new internal::ThreadObjectParameter;
	}

	virtual ~FileSystemMonitor()
	{
		PostQueuedCompletionStatus( completionPortHandle_, 0, 0, NULL );

		// Wait for the Directory thread to finish before exiting
		WaitForSingleObject( thread_, INFINITE );

		CloseHandle( thread_ );

		//for (int i=0; i<numDirs; ++i)
		//{
			CloseHandle( directoryInfo.directoryHandle );
		//}
	
		CloseHandle( completionPortHandle_ );

		delete threadObject_;
		

	}

	std::string getPath() const //TODO: evitar copias
	{ 
		return path_; 
	}
	void setPath (const std::string& val) 
	{ 
		//TODO: debe chequear el Path que exista en el FileSystem...
		path_ = val; 
	}

	int getNotifyFilters() const //TODO: evitar copias
	{ 
		return notifyFilters_; 
	}
	void setNotifyFilters(int val) 
	{ 
		notifyFilters_ = val; 
	}

	std::string getFilter() const //TODO: evitar copias
	{ 
		return filter_; 
	}
	void setFilter(const std::string& val) 
	{ 
		filter_ = val; 
	}

	bool getEnableRaisingEvents() const 
	{ 
		return enableRaisingEvents_; 
	}
	void setEnableRaisingEvents(bool val) 
	{ 
		enableRaisingEvents_ = val; 


		//TODO: acá va el código
	}





	void startMonitoring()
	{
		DWORD   tid;			//TODO:

		directoryInfo.directoryHandle = Win32ApiWrapper::CreateFile
									( 
										path_,
										FILE_LIST_DIRECTORY,
										FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
										NULL,
										OPEN_EXISTING,
										FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
										NULL
									);

		if ( directoryInfo.directoryHandle == INVALID_HANDLE_VALUE )
		{
			//TODO: manejo de excepciones
			std::cout << "Unable to open directory " << path_ << ". GLE=" << GetLastError() << ". Terminating..." << std::endl; 
			return;
		}

		lstrcpy( directoryInfo.directoryName, path_.c_str() );

		unsigned long addr = (unsigned long) &directoryInfo;

		// Set up a key(directory info) for each directory
		completionPortHandle_ = CreateIoCompletionPort
										( 
											directoryInfo.directoryHandle,
											completionPortHandle_,
											(unsigned long) addr,
											0
										);


		ReadDirectoryChangesW
										( 
											directoryInfo.directoryHandle,		// HANDLE TO DIRECTORY
											directoryInfo.buffer,               // Formatted buffer into which read results are returned.  This is a
											MAX_BUFFER,                         // Length of previous parameter, in bytes
											this->includeSubdirectories_ ? 1 : 0,	//TRUE,  // Monitor sub trees?
											notifyFilters_,						// FILE_NOTIFY_CHANGE_LAST_WRITE,      // What we are watching for
											&directoryInfo.bufferLength,        // Number of bytes returned into second parameter
											&directoryInfo.overlapped,          // OVERLAPPED structure that supplies data to be used during an asynchronous operation.  If this is NULL, ReadDirectoryChangesW does not return immediately.
											NULL								// Completion routine
											);      





		//TODO: boost::threads

		threadObject_->This = this;
		threadObject_->CompletionPortHandle = completionPortHandle_; //TODO: dato redundante

		thread_ = ::CreateThread
			( 
			NULL,
			0,
			(LPTHREAD_START_ROUTINE) HandleDirectoryChange,
			threadObject_,
			0,
			&tid
			);


	}

	// Event Handlers
	FileSystemEventHandler Changed;
	FileSystemEventHandler Created;
	FileSystemEventHandler Deleted;
	RenamedEventHandler Renamed;








private:

	//static void WINAPI HandleDirectoryChange( unsigned long completionPort )
	//static void WINAPI HandleDirectoryChange( ThreadObjectParameter* threadObject )
	static void WINAPI HandleDirectoryChange( void* tempObject )
	{

		internal::ThreadObjectParameter* threadObject = static_cast<internal::ThreadObjectParameter*>(tempObject);

		unsigned long numBytes;
		unsigned long cbOffset;
		LPDIRECTORY_INFO directoryInfo;
		//DirectoryInfo* di;
		LPOVERLAPPED overlapped;
		PFILE_NOTIFY_INFORMATION notifyInformation;


		
		do
		{
			GetQueuedCompletionStatus
				( 
				threadObject->This->completionPortHandle_,
				&numBytes,
				(LPDWORD) &directoryInfo,      // This is the DIRECTORY_INFO structure that was passed in the call to CreateIoCompletionPort below.
				&overlapped,
				INFINITE
				);

			if ( directoryInfo )
			{
				//fni = (PFILE_NOTIFY_INFORMATION)di->lpBuffer;
				//notifyInformation = static_cast<PFILE_NOTIFY_INFORMATION>(directoryInfo->buffer);
				notifyInformation = (PFILE_NOTIFY_INFORMATION)directoryInfo->buffer;

				do
				{
					cbOffset = notifyInformation->NextEntryOffset;

					//if( fni->Action == FILE_ACTION_MODIFIED )
					//      CheckChangedFile( di, fni );

					//
					switch ( notifyInformation->Action )
					{
						case FILE_ACTION_ADDED:
							if (threadObject->This->Created)
							{
								std::string fileName(notifyInformation->FileName, notifyInformation->FileName + (notifyInformation->FileNameLength/sizeof(WCHAR)) ); 
								FileSystemEventArgs temp;
								temp.Name = fileName;

								threadObject->This->Created(temp);
							}

							break;
						case FILE_ACTION_REMOVED:
							//std::cout << "file deleted: ";
							if (threadObject->This->Deleted)
							{
								std::string fileName(notifyInformation->FileName, notifyInformation->FileName + (notifyInformation->FileNameLength/sizeof(WCHAR)) ); 
								FileSystemEventArgs temp;
								temp.Name = fileName;

								threadObject->This->Deleted(temp);
							}

							break;
						case FILE_ACTION_MODIFIED:
							if (threadObject->This->Changed)
							{
								std::string fileName(notifyInformation->FileName, notifyInformation->FileName + (notifyInformation->FileNameLength/sizeof(WCHAR)) ); 

								FileSystemEventArgs temp;
								temp.Name = fileName;

								threadObject->This->Changed(temp);
							}

							break;
						case FILE_ACTION_RENAMED_OLD_NAME:
							
							if (threadObject->This->Renamed)
							{
								std::string fileName(notifyInformation->FileName, notifyInformation->FileName + (notifyInformation->FileNameLength/sizeof(WCHAR)) ); 

								RenamedEventArgs temp;
								temp.Name = fileName;

								threadObject->This->Renamed(temp);
							}

							break;
						case FILE_ACTION_RENAMED_NEW_NAME:
							break;
						default: 
							std::cout << "unknown event: ";
							break;
					}

					//std::string fileName(notifyInformation->FileName, notifyInformation->FileName + (notifyInformation->FileNameLength/sizeof(WCHAR)) ); 
					//std::cout << fileName << std::endl;

					notifyInformation = (PFILE_NOTIFY_INFORMATION)((LPBYTE) notifyInformation + cbOffset);

				} while( cbOffset );

	
	
				ReadDirectoryChangesW
				( 
					directoryInfo->directoryHandle,						
					directoryInfo->buffer,                                
					MAX_BUFFER,                               
					threadObject->This->includeSubdirectories_ ? 1 : 0,		//TRUE,                                     
					threadObject->This->notifyFilters_,						//FILE_NOTIFY_CHANGE_LAST_WRITE,            
					&directoryInfo->bufferLength,                        
					&directoryInfo->overlapped,                          
					NULL
				);      

			}

		} while( directoryInfo );


	}



	std::string path_;
	int notifyFilters_;									//TODO: debería ser un enum
	std::string filter_;
	bool enableRaisingEvents_;
	bool includeSubdirectories_;

	void* completionPortHandle_;
	HANDLE  thread_;									//TODO:
	internal::ThreadObjectParameter* threadObject_;		//TODO: shared_ptr




};

#endif // FileSystemMonitor_h__