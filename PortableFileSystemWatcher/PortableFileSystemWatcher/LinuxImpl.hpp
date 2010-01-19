#ifndef LinuxImpl_h__
#define LinuxImpl_h__

#include <string>

#include <cstdio>		//<stdio.h>
#include <cstdlib>		//<stdlib.h>
#include <cerrno>		//<errno.h>
//#include <pthread.h>	// Threads
#include <sys/types.h>
#include <sys/inotify.h>

#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>



#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

namespace detail
{

typedef boost::shared_ptr<boost::thread> HeapThread;

class LinuxImpl
{
public:

	~LinuxImpl()
	{
		closing_ = true;

		printf("removing watch...\n");
		int retRMWatch = inotify_rm_watch( fileDescriptor_, watchDescriptor_ );
		printf("retRMWatch: %d\n", retRMWatch);
		printf("closing file descriptor...\n");
		int retClose =  close( fileDescriptor_ );
		printf("retClose: %d\n", retClose);
	
	}

	void startMonitoring(const std::string& path)
	{
		fileDescriptor_ = inotify_init();

		if ( fileDescriptor_ < 0 ) 
		{
			perror( "inotify_init" );
		}

		
		//watchDescriptor_ = inotify_add_watch( fileDescriptor_, "/home/fernando/temp1", IN_MODIFY | IN_CREATE | IN_DELETE );
		watchDescriptor_ = inotify_add_watch( fileDescriptor_, path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE );


		thread_.reset( new boost::thread( boost::bind(&LinuxImpl::HandleDirectoryChange, this) ) );
	}
private:
	void HandleDirectoryChange() //TODO: rename
	{
		int i = 0;
		char buffer[BUF_LEN];

		printf("reading in file descriptor\n");
		int length = read( fileDescriptor_, buffer, BUF_LEN );
		printf("end reading on file descriptor\n");

		printf("length: %d\n", length);

		if ( length < 0 ) 
		{
			perror( "read" );
		}  


		//int j;
		//for (j = 0; j<length; j++)
		//{
		//	printf("buffer[j]: %d\n", buffer[j]);
		//}

		printf("i: %d\n", i);
		while ( i < length ) 
		{
			printf("inside the 'while'\n");

			struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ]; //TODO:

			printf("event: %d\n", (void*)event);
			printf("event->len: %d\n", event->len);

			if ( event->len ) 
			{
				if ( event->mask & IN_CREATE ) 
				{
					if ( event->mask & IN_ISDIR ) 
					{
						printf( "The directory '%s' was created.\n", event->name );       
					}
					else 
					{
						printf( "The file '%s' was created.\n", event->name );
					}
				}
				else if ( event->mask & IN_DELETE ) 
				{
					if ( event->mask & IN_ISDIR ) 
					{
						printf( "The directory '%s' was deleted.\n", event->name );       
					}
					else 
					{
						printf( "The file '%s' was deleted.\n", event->name );
					}
				}
				else if ( event->mask & IN_MODIFY ) 
				{
					if ( event->mask & IN_ISDIR ) 
					{
						printf( "The directory '%s' was modified.\n", event->name );
					}
					else 
					{
						printf( "The file '%s' was modified.\n", event->name );
					}
				}
			}

			i += EVENT_SIZE + event->len;

			printf("i: %d\n", i);

		}

		//( void ) inotify_rm_watch( fileDescriptor_, wd );
		//( void ) close( fileDescriptor_ );

		int retRMWatch = inotify_rm_watch( fileDescriptor_, watchDescriptor_ );
		printf("retRMWatch: %d\n", retRMWatch);
		int retClose =  close( fileDescriptor_ );
		printf("retClose: %d\n", retClose);



		pthread_join( thread1, NULL);

		//printf("Thread 1 returns: %d\n", iret1);

		exit(0);
	}

	HeapThread thread_;

	int fileDescriptor_; // file descriptor
	int watchDescriptor_;
	bool closing_;

};

typedef LinuxImpl PlatformImpl;

}

#endif // LinuxImpl_h__
