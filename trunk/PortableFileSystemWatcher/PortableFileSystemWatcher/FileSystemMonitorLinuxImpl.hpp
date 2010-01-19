// TODO: LEER: URGENTE
// http://linux.die.net/man/7/inotify

// TODO: UBUNTU:
//			http://old-releases.ubuntu.com/releases/5.10/
//			http://old-releases.ubuntu.com/releases/
//			http://en.wikipedia.org/wiki/List_of_Ubuntu_releases

// LINUX OLD VERSIONS: dnotify 
//			http://en.wikipedia.org/wiki/Dnotify


// WINDOWS 3.11: http://www.taringa.net/posts/downloads/1035822/Windows-3_11.html
// WINDOWS 98 SE: http://wiki.taringa.net/posts/downloads/2174431/Windows-98--SE-Booteable.html
//                http://wiki.taringa.net/posts/downloads/1955565/Windows-98-_Equot;Segunda-edicion_Equot;.html
//				  http://www.taringa.net/posts/downloads/1491446/Windows-98-SE-Actualizado-hasta-2007.html
//				  http://www.taringa.net/posts/downloads/995032/Pack-para-windows-98-(editado).html




#ifndef FileSystemMonitorLinuxImpl_h__
#define FileSystemMonitorLinuxImpl_h__


//Inotify was merged into the 2.6.13 Linux kernel. The required library interfaces were added to glibc in version 2.4. 
//Bugs
//In kernels before 2.6.16, the IN_ONESHOT mask flag does not work. 


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
	typedef boost::shared_ptr<boost::thread> HeapThread; //TODO: repetido

	class FileSystemMonitorLinuxImpl
	{
	public:

		friend class ::FileSystemMonitor;

		FileSystemMonitorLinuxImpl()
			: completionPortHandle_(0)
		{
		}

		virtual ~FileSystemMonitorLinuxImpl()
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


			thread_.reset( new boost::thread( boost::bind(&FileSystemMonitorLinuxImpl::HandleDirectoryChange, this) ) );
			//thread_ = ::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE) HandleDirectoryChange, threadObject_, 0, &tid );


		}

		// Event Handlers
		FileSystemEventHandler Changed;
		FileSystemEventHandler Created;
		FileSystemEventHandler Deleted;
		RenamedEventHandler Renamed;

	private:
		void HandleDirectoryChange()
		{
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
	};

}
#endif // FileSystemMonitorLinuxImpl_h__
