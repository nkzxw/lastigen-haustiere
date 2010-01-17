//TODO: IMPORTANTE: LEER: Making Pimpl Easy http://www.ddj.com/cpp/205918714;jsessionid=ZHK0ZRNRQYCJHQE1GHOSKH4ATMY32JVN?pgno=2

#ifndef FileSystemMonitor_h__ //TODO:cambiar
#define FileSystemMonitor_h__

#include <exception>
#include <string>


#include <boost/function.hpp>
//TODO: boost::any
//TODO: boost::config

#include "EventArgs.hpp"
#include "EventHandlers.hpp"
#include "FileSystemMonitorImpl.hpp"
#include "NotifyFilters.hpp"
#include "Utils.hpp"
//#include "Win32ApiWrapper.hpp"
#include "Win32Legacy.hpp"


//class FileSystemMonitor; //Forward declaration

class FileSystemMonitor
{
public:

	FileSystemMonitor()
		//: notifyFilters_(NotifyFilters::LastWrite | NotifyFilters::DirectoryName | NotifyFilters::FileName)
		//, directory_("")
		//, filter_("*.*")
		////, impl_(new detail::FileSystemMonitorImpl)
	{
		//: completionPortHandle_(0)
		//threadObject_ = new detail::ThreadObjectParameter;

		initialize();
	}

	FileSystemMonitor(const std::string& path)
		//: this(path, "*.*")
	{
		initialize(path, "*.*");
	}


	

	//    public FSW(string path)
	//        : this(path, "*.*")
	//    {
	//    }
	//
	//    public FSW(string path, string filter)
	//    {
	//        this.notifyFilters = NotifyFilters.LastWrite | NotifyFilters.DirectoryName | NotifyFilters.FileName;
	//        this.internalBufferSize = 0x2000;
	//        if (path == null)
	//        {
	//            throw new ArgumentNullException("path");
	//        }
	//        if (filter == null)
	//        {
	//            throw new ArgumentNullException("filter");
	//        }
	//        if ((path.Length == 0) || !Directory.Exists(path))
	//        {
	//            throw new ArgumentException(SR.GetString("InvalidDirName", new object[] { path }));
	//        }
	//        this.directory = path;
	//        this.filter = filter;
	//    }

	//    [DefaultValue("*.*"), IODescription("FSW_Filter"), TypeConverter("System.Diagnostics.Design.StringValueConverter, System.Design, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a"), RecommendedAsConfigurable(true)]
	//    public string Filter
	//    {
	//        get
	//        {
	//            return this.filter;
	//        }
	//        set
	//        {
	//            if ((value == null) || (value == string.Empty))
	//            {
	//                value = "*.*";
	//            }
	//            if (string.Compare(this.filter, value, StringComparison.OrdinalIgnoreCase) != 0)
	//            {
	//                this.filter = value;
	//            }
	//        }
	//    }

	//    [IODescription("FSW_ChangedFilter"), DefaultValue(0x13)]
	//    public NotifyFilters NotifyFilter
	//    {
	//        get
	//        {
	//            return this.notifyFilters;
	//        }
	//        set
	//        {
	//            if ((value & ~notifyFiltersValidMask) != 0)
	//            {
	//                throw new InvalidEnumArgumentException("value", (int)value, typeof(NotifyFilters));
	//            }
	//            if (this.notifyFilters != value)
	//            {
	//                this.notifyFilters = value;
	//                this.Restart();
	//            }
	//        }
	//    }

	//    [TypeConverter("System.Diagnostics.Design.StringValueConverter, System.Design, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a"), IODescription("FSW_Path"), Editor("System.Diagnostics.Design.FSWPathEditor, System.Design, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a", "System.Drawing.Design.UITypeEditor, System.Drawing, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a"), DefaultValue(""), RecommendedAsConfigurable(true)]
	//    public string Path
	//    {
	//        get
	//        {
	//            return this.directory;
	//        }
	//        set
	//        {
	//            value = (value == null) ? string.Empty : value;
	//            if (string.Compare(this.directory, value, StringComparison.OrdinalIgnoreCase) != 0)
	//            {
	//                if (base.DesignMode)
	//                {
	//                    if ((value.IndexOfAny(wildcards) != -1) || (value.IndexOfAny(Path.GetInvalidPathChars()) != -1))
	//                    {
	//                        throw new ArgumentException(SR.GetString("InvalidDirName", new object[] { value }));
	//                    }
	//                }
	//                else if (!Directory.Exists(value))
	//                {
	//                    throw new ArgumentException(SR.GetString("InvalidDirName", new object[] { value }));
	//                }
	//                this.directory = value;
	//                this.readGranted = false;
	//                this.Restart();
	//            }
	//        }
	//    }


	//    private bool MatchPattern(string relativePath)
	//    {
	//        string fileName = System.IO.Path.GetFileName(relativePath);
	//        return ((fileName != null) && System.IO.PatternMatcher.StrictMatchPattern(this.filter.ToUpper(CultureInfo.InvariantCulture), fileName.ToUpper(CultureInfo.InvariantCulture)));
	//    }

	//    private void NotifyFileSystemEventArgs(int action, string name)
	//    {
	//        if (this.MatchPattern(name))
	//        {
	//            switch (action)
	//            {
	//                case 1:
	//                    this.OnCreated(new FileSystemEventArgs(WatcherChangeTypes.Created, this.directory, name));
	//                    return;
	//
	//                case 2:
	//                    this.OnDeleted(new FileSystemEventArgs(WatcherChangeTypes.Deleted, this.directory, name));
	//                    return;
	//
	//                case 3:
	//                    this.OnChanged(new FileSystemEventArgs(WatcherChangeTypes.Changed, this.directory, name));
	//                    return;
	//            }
	//        }
	//    }

	//    private void NotifyRenameEventArgs(WatcherChangeTypes action, string name, string oldName)
	//    {
	//        if (this.MatchPattern(name) || this.MatchPattern(oldName))
	//        {
	//            RenamedEventArgs e = new RenamedEventArgs(action, this.directory, name, oldName);
	//            this.OnRenamed(e);
	//        }
	//    }


	virtual ~FileSystemMonitor()
	{
	}

	std::string getPath() const //TODO: evitar copias
	{ 
		return this->directory_; 
	}
	void setPath (const std::string& val) 
	{ 
		//TODO: debe chequear el Path que exista en el FileSystem...
		this->directory_ = val; 
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
		impl_->startMonitoring(directory_, notifyFilters_, includeSubdirectories_);
	}

	// Event Handlers
	FileSystemEventHandler Changed;
	FileSystemEventHandler Created;
	FileSystemEventHandler Deleted;
	RenamedEventHandler Renamed;


private:

	inline void initialize()
	{
		initialize("", "*.*");
	}

	inline void initialize(const std::string& path, const std::string& filter)
	{
		this->notifyFilters_ = NotifyFilters::LastWrite | NotifyFilters::DirectoryName | NotifyFilters::FileName;
		this->impl_ = new detail::FileSystemMonitorImpl;  //TODO: heap o stack????
		this->filter_ = filter;

		//std::runtime_error

		if ( path.size() == 0 || !utils::directoryExists(path) )
		{
			throw std::runtime_error("InvalidDirName");
			//throw new ArgumentException(SR.GetString("InvalidDirName", new object[] { path }));
		}

		this->directory_ = path;
	}


	std::string directory_;
	int notifyFilters_;									//TODO: debería ser un enum
	std::string filter_;
	bool enableRaisingEvents_;
	bool includeSubdirectories_;

	//void* completionPortHandle_;
	//HANDLE  thread_;									//TODO:
	//detail::ThreadObjectParameter* threadObject_;		//TODO: shared_ptr

	detail::FileSystemMonitorImpl* impl_;				//TODO:

};

#endif // FileSystemMonitor_h__