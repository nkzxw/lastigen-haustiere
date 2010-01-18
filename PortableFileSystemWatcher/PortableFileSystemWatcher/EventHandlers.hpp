#ifndef EventHandlers_h__
#define EventHandlers_h__

//Similar a un C# Delegate
//TODO: falta el object sender que podría ser implementado mediante boost::any
typedef boost::function<void (FileSystemEventArgs e)> FileSystemEventHandler;
typedef boost::function<void (RenamedEventArgs e)> RenamedEventHandler;

#endif // EventHandlers_h__