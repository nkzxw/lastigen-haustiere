//#ifndef CONFIGMANAGER_FORWARDDECLARATION_HPP_INCLUDED
//#define CONFIGMANAGER_FORWARDDECLARATION_HPP_INCLUDED


// Locking Policies
class NoLock;
class Locking;

// Refresing Policies
template <typename T>
class RefresingPolicyBase;

template <typename T>
class NoRefresh;

template <typename T>
class StaticRefresh;

template <typename T>
class DynamicRefresh;


//template <
//	typename T, 
//	template <typename> class RefreshPolicy = Refreshing
//>
//class ConfigManager;

template <
	typename T, 
	template <typename> class RefreshPolicy
>
class ConfigManager;

//#endif // CONFIGMANAGER_FORWARDDECLARATION_HPP_INCLUDED