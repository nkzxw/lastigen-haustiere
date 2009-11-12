//#ifndef CONFIGMANAGER_FORWARDDECLARATION_HPP_INCLUDED
//#define CONFIGMANAGER_FORWARDDECLARATION_HPP_INCLUDED


// Locking Policies
class NoLock;
class Locking;

// Refresing Policies
class NoRefresh;

template <typename T>
class Refreshing;


//template <
//	typename T, 
//	typename RefreshPolicy = Refreshing
//>
//class ConfigManager;


template <
	typename T, 
	template <typename> class RefreshPolicy = Refreshing
>
class ConfigManager;

//#endif // CONFIGMANAGER_FORWARDDECLARATION_HPP_INCLUDED