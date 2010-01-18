// http://www.linuxjournal.com/article/8478
// http://www.ibm.com/developerworks/linux/library/l-ubuntu-inotify/index.html?ca=drs-


// TODO: boost::asio::posix::stream_descriptor

// watcher-select.c

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int main( int argc, char **argv ) 
{
	int length;
	int i = 0;
	int fd;
	int wd;
	char buffer[BUF_LEN];

	fd = inotify_init();

	if ( fd < 0 ) 
	{
		perror( "inotify_init" );
	}
/*
#define IN_ACCESS        0x00000001     // File was accessed.
#define IN_MODIFY        0x00000002     // File was modified.
#define IN_ATTRIB        0x00000004     // Metadata changed.
#define IN_CLOSE_WRITE   0x00000008     // Writtable file was closed.
#define IN_CLOSE_NOWRITE 0x00000010     // Unwrittable file closed.
#define IN_CLOSE         (IN_CLOSE_WRITE | IN_CLOSE_NOWRITE) // Close.
#define IN_OPEN          0x00000020     // File was opened.
#define IN_MOVED_FROM    0x00000040     // File was moved from X.
#define IN_MOVED_TO      0x00000080     // File was moved to Y.
#define IN_MOVE          (IN_MOVED_FROM | IN_MOVED_TO) // Moves.
#define IN_CREATE        0x00000100     // Subfile was created.
#define IN_DELETE        0x00000200     // Subfile was deleted.
#define IN_DELETE_SELF   0x00000400     // Self was deleted.
#define IN_MOVE_SELF     0x00000800     // Self was moved.
*/

	wd = inotify_add_watch( fd, "/home/fernando/temp1", IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVE );


	//length = read( fd, buffer, BUF_LEN );  




	struct timeval time;
	fd_set rfds;
	int ret;

	// timeout after five seconds
	time.tv_sec = 15;
	time.tv_usec = 0;

	// zero-out the fd_set
	FD_ZERO (&rfds);

	// add the inotify fd to the fd_set -- of course, your application will probably want to add other file descriptors here, too

	FD_SET (fd, &rfds);

	ret = select (fd + 1, &rfds, NULL, NULL, &time);
	if (ret < 0)
	{
		perror ("select");
	}
	else if (!ret)
	{
		printf( "timed out!\n");       
	}
	else if (FD_ISSET (fd, &rfds))
	{
		printf( "inotify events are available!\n"); 
		length = read( fd, buffer, BUF_LEN );        
	}






	if ( length < 0 ) 
	{
		perror( "read" );
	}  

	while ( i < length ) 
	{
		struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];

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

			else if ( event->mask & IN_MOVED_FROM ) 
			{
				if ( event->mask & IN_ISDIR ) 
				{
					printf( "The directory '%s' was moved from ... .\n", event->name );
				}
				else 
				{
					printf( "The file '%s' was moved from... .\n", event->name );
				}
			}

			else if ( event->mask & IN_MOVED_TO ) 
			{
				if ( event->mask & IN_ISDIR ) 
				{
					printf( "The directory '%s' was moved to... .\n", event->name );
				}
				else 
				{
					printf( "The file '%s' was moved to... .\n", event->name );
				}
			}

		}

		i += EVENT_SIZE + event->len;
	}

	( void ) inotify_rm_watch( fd, wd );
	( void ) close( fd );

	exit( 0 );
}
