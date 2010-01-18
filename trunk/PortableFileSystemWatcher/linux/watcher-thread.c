// http://www.linuxjournal.com/article/8478
// http://www.ibm.com/developerworks/linux/library/l-ubuntu-inotify/index.html?ca=drs-
// http://www.yolinux.com/TUTORIALS/LinuxTutorialPosixThreads.html
// http://www.xgc.com/manuals/xgclib/x4709.html

// TODO: boost::asio::posix::stream_descriptor

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/inotify.h>

void *print_message_function( void *ptr );

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int fd; // file descriptor
int wd;
int closing;

int main( int argc, char **argv ) 
{
	closing = 0;

	pthread_t thread1;
	char *message1 = "Thread 1";
	int  iret1;

	/* Create independent threads each of which will execute function */
	iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);










	int length;
	int i = 0;
	char buffer[BUF_LEN];

	fd = inotify_init();

	if ( fd < 0 ) 
	{
		perror( "inotify_init" );
	}

	wd = inotify_add_watch( fd, "/home/fernando/temp1", IN_MODIFY | IN_CREATE | IN_DELETE );
	
	
	printf("reading in file descriptor\n");
	length = read( fd, buffer, BUF_LEN );
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

	//( void ) inotify_rm_watch( fd, wd );
	//( void ) close( fd );

	int retRMWatch = inotify_rm_watch( fd, wd );
	printf("retRMWatch: %d\n", retRMWatch);
	int retClose =  close( fd );
	printf("retClose: %d\n", retClose);



	pthread_join( thread1, NULL);

	//printf("Thread 1 returns: %d\n", iret1);

	exit(0);
}

void *print_message_function( void *ptr )
{
	printf("waiting 5 seconds\n");

	sleep ( 5 );

	closing = 1;

	printf("removing watch...\n");
	int retRMWatch = inotify_rm_watch( fd, wd );
	printf("retRMWatch: %d\n", retRMWatch);
	printf("closing file descriptor...\n");
	int retClose =  close( fd );
	printf("retClose: %d\n", retClose);



	//char *message;
	//message = (char *) ptr;
	//printf("%s \n", message);
}
