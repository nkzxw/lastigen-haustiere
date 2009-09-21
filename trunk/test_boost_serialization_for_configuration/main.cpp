// http://landheer-cieslak.com/wordpress/?p=130


#ifndef configuration_h
#define configuration_h

#include <fstream>
#include <iostream>

//#include <boost/serialization.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/serialization/access.hpp>

#include <boost/serialization/version.hpp>							//BOOST_CLASS_VERSION
#include <boost/serialization/split_member.hpp>						//BOOST_SERIALIZATION_SPLIT_MEMBER()



#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>




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
			//ar	& something_important_
			//	& something_added_later_
			//	& something_added_even_later_
			//	;

			ar	& BOOST_SERIALIZATION_NVP(something_important_)
				& BOOST_SERIALIZATION_NVP(something_added_later_)
				& BOOST_SERIALIZATION_NVP(something_added_even_later_)
				;


			//ar & make_nvp("id", id_);
			//BOOST_SERIALIZATION_NVP(s);


		}

		template < typename Archive >
		void load(Archive & ar, const unsigned int version)
		{
			ar	& BOOST_SERIALIZATION_NVP(something_important_);

			if (version > 0)
			{
				ar	& BOOST_SERIALIZATION_NVP(something_added_later_);
			}
			else
			{ /* use default */ }
			
			if (version > 1)
			{
				ar	& BOOST_SERIALIZATION_NVP(something_added_even_later_);
			}
			else
			{ /* use default */ }
			is_default_ = version < 2;
		}

		BOOST_SERIALIZATION_SPLIT_MEMBER()
 
		SomethingImportant something_important_;
		int something_added_later_;
		int something_added_even_later_;
		bool is_default_;
	};
}

BOOST_CLASS_VERSION(MyApp::Configuration, 3);

#endif


int main()
{

	MyApp::Configuration cfg;



	//GameData *gameData = new GameData();

	//std::string filename(boost::archive::tmpdir());
	//filename += "/demo_save.xml";
	std::string filename = "./GameConfig.xml";

	std::cout << std::endl;
	std::cout << "filename: " << filename << std::endl;

	//GameDataSerializer::saveTo(filename, *gameData);

	std::ofstream ofs(filename.c_str());
	assert(ofs.good());

	//boost::archive::xml_oarchive xml(ofs);
	////xml << boost::serialization::make_nvp("GameData", gameData);
	////xml << boost::serialization::make_nvp("cfg", cfg);
	//xml << cfg;


	// save data to archive
    {
		boost::archive::xml_oarchive xml(ofs);
        xml << BOOST_SERIALIZATION_NVP(cfg);
    }

    //{
    //    boost::archive::text_oarchive oa(ofs);
    //    oa << cfg;
    //}



	MyApp::Configuration cfgNew;
	    // open the archive
    std::ifstream ifs(filename.c_str());
    assert(ifs.good());
    boost::archive::xml_iarchive ia(ifs);

    // restore the schedule from the archive
    ia >> BOOST_SERIALIZATION_NVP(cfgNew);



	return 0;
}





/*
//TODO: ver
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/vector.hpp>

---
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		using boost::serialization::make_nvp;
		ar & make_nvp("id", id_);
		ar & make_nvp("name", name_);
		ar & make_nvp("regions", regions_);
		ar & make_nvp("color", color_);
	}

*/