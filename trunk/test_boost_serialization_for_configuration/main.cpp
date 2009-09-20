// http://landheer-cieslak.com/wordpress/?p=130


#ifndef configuration_h
#define configuration_h

//#include <boost/serialization.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/serialization/access.hpp>
 
namespace MyApp
{
	struct Configuration
	{
		enum SomethingImportant { first_possibility__, second_possibility__, };

	//, something_important_(first_possibility____)

		Configuration()
			: is_default_(true)
			, something_important_(first_possibility__)
			, something_added_later_(0)
			, something_added_even_later_(0)
		{ /* no-op */ }
 
		template < typename Archive >
		void save(Archive & ar, const unsigned int version) const
		{
			ar	& something_important_
				& something_added_later_
				& something_added_even_later_
				;
		}

		template < typename Archive >
		void load(Archive & ar, const unsigned int version)
		{
			ar	& something_important_;

			if (version > 0)
			{
				ar	& something_added_later_;
			}
			else
			{ /* use default */ }
			
			if (version > 1)
			{
				ar	& something_added_even_later_;
			}
			else
			{ /* use default */ }
			is_default_ = version < 2;
		}

		//BOOST_SERIALIZATION_SPLIT_MEMBER()
 
		SomethingImportant something_important_;
		int something_added_later_;
		int something_added_even_later_;
		bool is_default_;
	};
}

//BOOST_CLASS_VERSION(MyApp::Configuration, 2);

#endif


int main()
{
}

