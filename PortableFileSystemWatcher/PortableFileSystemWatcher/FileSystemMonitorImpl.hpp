#ifndef FileSystemMonitorImpl_h__
#define FileSystemMonitorImpl_h__

#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

#include "EventArgs.hpp"
#include "EventHandlers.hpp"
#include "NotifyFilters.hpp"
#include "Win32ApiWrapper.hpp"
#include "Win32Legacy.hpp"


class FileSystemMonitor; //Forward declaration

namespace detail
{
	class FileSystemMonitorImpl;

	struct ThreadObjectParameter
	{
		FileSystemMonitorImpl* This;
		void* CompletionPortHandle;
	};

	typedef boost::shared_ptr<boost::thread> HeapThread;


	class FileSystemMonitorImpl
	{
	public:

		friend class ::FileSystemMonitor;

		FileSystemMonitorImpl()
			: completionPortHandle_(0)
		{
			//threadObject_ = new detail::ThreadObjectParameter;
		}

		virtual ~FileSystemMonitorImpl()
		{

			// Si hubo algun error al tratar de monitorear el directorio, estos pasos no hay que hacerlos...

			PostQueuedCompletionStatus( completionPortHandle_, 0, 0, NULL );

			// Wait for the Directory thread to finish before exiting
			//WaitForSingleObject( thread_, INFINITE );
			//CloseHandle( thread_ );
			thread_->join();

			//for (int i=0; i<numDirs; ++i)
			//{
			CloseHandle( directoryInfo.directoryHandle );
			//}

			CloseHandle( completionPortHandle_ );

			//delete threadObject_;


		}

		void startMonitoring(const std::string& path) //, int notifyFilters, bool includeSubdirectories, const std::string& filter)
		{
			//this->path_ = 
			//this->notifyFilters_ = notifyFilters;
			//this->filter_ = filter;
			//this->includeSubdirectories_ = includeSubdirectories;

			//DWORD   tid;			//TODO:

			directoryInfo.directoryHandle = Win32ApiWrapper::CreateFile
				( 
				path,
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
				std::cout << "Unable to open directory " << path << ". GLE=" << GetLastError() << ". Terminating..." << std::endl; 
				return;
			}

			lstrcpy( directoryInfo.directoryName, path.c_str() );

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
				this->notifyFilters_,						// FILE_NOTIFY_CHANGE_LAST_WRITE,      // What we are watching for
				&directoryInfo.bufferLength,        // Number of bytes returned into second parameter
				&directoryInfo.overlapped,          // OVERLAPPED structure that supplies data to be used during an asynchronous operation.  If this is NULL, ReadDirectoryChangesW does not return immediately.
				NULL								// Completion routine
				);      


			//threadObject_->This = this;
			//threadObject_->CompletionPortHandle = completionPortHandle_; //TODO: dato redundante


			thread_.reset( new boost::thread( boost::bind(&FileSystemMonitorImpl::HandleDirectoryChange, this) ) );
			//thread_ = ::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE) HandleDirectoryChange, threadObject_, 0, &tid );


		}

		// Event Handlers
		FileSystemEventHandler Changed;
		FileSystemEventHandler Created;
		FileSystemEventHandler Deleted;
		RenamedEventHandler Renamed;

private:
		//static void WINAPI HandleDirectoryChange( unsigned long completionPort )
		//static void WINAPI HandleDirectoryChange( ThreadObjectParameter* threadObject )
		//static void WINAPI HandleDirectoryChange( void* tempObject )
		void HandleDirectoryChange()
		{
			//detail::ThreadObjectParameter* threadObject = static_cast<detail::ThreadObjectParameter*>(tempObject);

			unsigned long numBytes;
			unsigned long cbOffset;
			LPDIRECTORY_INFO directoryInfo;
			//DirectoryInfo* di;
			LPOVERLAPPED overlapped;
			PFILE_NOTIFY_INFORMATION notifyInformation;

			do
			{
				//threadObject->This->completionPortHandle_,

				GetQueuedCompletionStatus
					( 
					this->completionPortHandle_,
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
							//if (threadObject->This->Created)

						case FILE_ACTION_ADDED:
							if (this->Created)
							{
								std::string fileName(notifyInformation->FileName, notifyInformation->FileName + (notifyInformation->FileNameLength/sizeof(WCHAR)) ); 
								FileSystemEventArgs temp;
								temp.Name = fileName;

								//threadObject->This->Created(temp);
								this->Created(temp);
							}

							break;
						case FILE_ACTION_REMOVED:
							//std::cout << "file deleted: ";
							//if (threadObject->This->Deleted)
							if (this->Deleted)
							{
								std::string fileName(notifyInformation->FileName, notifyInformation->FileName + (notifyInformation->FileNameLength/sizeof(WCHAR)) ); 
								FileSystemEventArgs temp;
								temp.Name = fileName;

								//threadObject->This->Deleted(temp);
								this->Deleted(temp);
							}

							break;
						case FILE_ACTION_MODIFIED:
							//if (threadObject->This->Changed)
							if (this->Changed)
							{
								std::string fileName(notifyInformation->FileName, notifyInformation->FileName + (notifyInformation->FileNameLength/sizeof(WCHAR)) ); 

								FileSystemEventArgs temp;
								temp.Name = fileName;

								//threadObject->This->Changed(temp);
								this->Changed(temp);
							}

							break;
						case FILE_ACTION_RENAMED_OLD_NAME:

							//if (threadObject->This->Renamed)
							if (this->Renamed)
							{
								std::string fileName(notifyInformation->FileName, notifyInformation->FileName + (notifyInformation->FileNameLength/sizeof(WCHAR)) ); 

								RenamedEventArgs temp;
								temp.Name = fileName;

								//threadObject->This->Renamed(temp);
								this->Renamed(temp);
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


					::ReadDirectoryChangesW
						( 
						directoryInfo->directoryHandle,						
						directoryInfo->buffer,                                
						MAX_BUFFER,                               
						this->includeSubdirectories_ ? 1 : 0,		//TRUE,                                     
						this->notifyFilters_,						//FILE_NOTIFY_CHANGE_LAST_WRITE,            
						&directoryInfo->bufferLength,                        
						&directoryInfo->overlapped,                          
						NULL
						);      

				}

			} while( directoryInfo );


		}

		int notifyFilters_;									//TODO: debería ser un enum
		std::string filter_;
		bool includeSubdirectories_;
		void* completionPortHandle_;

		HeapThread thread_;
		//HANDLE  thread_;									//TODO:
		//detail::ThreadObjectParameter* threadObject_;		//TODO: shared_ptr
	};

}
#endif // FileSystemMonitorImpl_h__
