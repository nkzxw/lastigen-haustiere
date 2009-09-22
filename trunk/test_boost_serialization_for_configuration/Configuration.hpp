// http://landheer-cieslak.com/wordpress/?p=130


#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

//#include <fstream>
//#include <iostream>

#include <boost/filesystem/path.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/version.hpp>							//BOOST_CLASS_VERSION
#include <boost/serialization/split_member.hpp>						//BOOST_SERIALIZATION_SPLIT_MEMBER()

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>


class Registration
{
public:
	Registration()
		: interface_(""), dllPath_("")
	{}

	friend class boost::serialization::access;

	template < typename Archive >
	void save(Archive & ar, const unsigned int version) const
	{
		ar	& BOOST_SERIALIZATION_NVP(interface_)
			& BOOST_SERIALIZATION_NVP(dllPath_)
			;

		//ar & make_nvp("id", id_);
	}

	template < typename Archive >
	void load(Archive & ar, const unsigned int version)
	{
		ar	& BOOST_SERIALIZATION_NVP(interface_)
			& BOOST_SERIALIZATION_NVP(dllPath_);

		//ar	& BOOST_SERIALIZATION_NVP(dllPath_);

		//isDefault_ = version < 2;
	}


	BOOST_SERIALIZATION_SPLIT_MEMBER()

	std::string interface_;
	std::string dllPath_;
};


class Configuration
{
public:
	//enum SomethingImportant { first_possibility__, second_possibility__, };

	Configuration()
		: isDefault_(true)
		//, dllPath_("")
	{ 
		/* no-op */ 
	}


	//template < typename Archive >
	//void save(Archive & ar, const unsigned int version) const
	//{
	//	//ar	& something_important_
	//	//	& something_added_later_
	//	//	& something_added_even_later_
	//	//	;

	//	ar	& BOOST_SERIALIZATION_NVP(something_important_)
	//		& BOOST_SERIALIZATION_NVP(something_added_later_)
	//		& BOOST_SERIALIZATION_NVP(something_added_even_later_)
	//		;
	//	//ar & make_nvp("id", id_);
	//	//BOOST_SERIALIZATION_NVP(s);
	//}

	//template < typename Archive >
	//void load(Archive & ar, const unsigned int version)
	//{
	//	ar	& BOOST_SERIALIZATION_NVP(something_important_);

	//	if (version > 0)
	//	{
	//		ar	& BOOST_SERIALIZATION_NVP(something_added_later_);
	//	}
	//	else
	//	{ /* use default */ }
	//	
	//	if (version > 1)
	//	{
	//		ar	& BOOST_SERIALIZATION_NVP(something_added_even_later_);
	//	}
	//	else
	//	{ /* use default */ }
	//	isDefault_ = version < 2;
	//}

	friend class boost::serialization::access;

	template < typename Archive >
	void save(Archive & ar, const unsigned int version) const
	{
		ar	& BOOST_SERIALIZATION_NVP(registration_)
			;

		//ar	& BOOST_SERIALIZATION_NVP(dllPath_)
		//	;

		//ar & make_nvp("id", id_);
	}

	template < typename Archive >
	void load(Archive & ar, const unsigned int version)
	{
		ar	& BOOST_SERIALIZATION_NVP(registration_);
		//ar	& BOOST_SERIALIZATION_NVP(dllPath_);

		isDefault_ = version < 2;
	}


	BOOST_SERIALIZATION_SPLIT_MEMBER()

protected:


public: //protected:

	//SomethingImportant something_important_;
	//int something_added_later_;
	//int something_added_even_later_;

	Registration registration_;
	//std::string dllPath_;
	bool isDefault_;
};


BOOST_CLASS_VERSION(Configuration, 0);

#endif // CONFIGURATION_HPP
