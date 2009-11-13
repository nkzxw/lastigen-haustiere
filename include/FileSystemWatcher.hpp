// http://md5deep.sourceforge.net/

// http://api.kde.org/4.2-api/kdepim-apidocs/kleopatra/html/filesystemwatcher_8cpp_source.html

// http://discuss.fogcreek.com/joelonsoftware5/default.asp?cmd=show&ixPost=146691&ixReplies=14

// http://msdn.microsoft.com/en-us/library/system.io.filesystemwatcher%28VS.71%29.aspx

// http://support.microsoft.com/default.aspx?scid=kb;en-us;841290

// http://msdn.microsoft.com/es-es/library/system.io.filesystemwatcher%28VS.80%29.aspx



// WIN32

	// FileSystemWatcher usa: ReadDirectoryChangesW (debuguear dentro de esta API a ver como lo implementa)
		// Ver si hay un equivalente en POSIX
		// http://msdn.microsoft.com/en-us/library/aa365465%28VS.85%29.aspx

		// Aparente esta API está implementada en Samba: https://bugzilla.redhat.com/show_bug.cgi?id=247447


	// Obtaining Directory Change Notifications:
		// http://msdn.microsoft.com/en-us/library/aa365261%28VS.85%29.aspx


// POSIX General
	// http://mark.heily.com/pnotify/

// LINUX

	// inotify: Linux kernel notifications
		// http://en.wikipedia.org/wiki/Inotify
		// http://www.ibm.com/developerworks/linux/library/l-inotify.html
		// http://sourceforge.net/projects/iwatch/
		// http://inotify.aiken.cz/?section=inotify-cxx&page=about&lang=en

// FreeBSD (MacOS)
	// kqueue
	// http://www.ibm.com/developerworks/linux/library/l-ubuntu-inotify/index.html
	// http://thoughtsonrails.wordpress.com/2007/02/28/beagle-on-pcbsd-freebsd-it-works/

// ????
	// sgi FAM:
		// http://oss.sgi.com/projects/fam/faq.html