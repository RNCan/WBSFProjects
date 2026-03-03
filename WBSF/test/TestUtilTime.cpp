#include <iostream>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>

#include <gtest/gtest.h>

#include "Basic/UtilTime.h"
#include "Basic/UtilMath.h"
#include "Basic/UtilStd.h"

#include "Basic/Statistic.h"

using namespace std;



namespace WBSF
{

	// C++ stream interface
	class TestCout : public std::stringstream
	{
	public:
		~TestCout()
		{
			std::cout << "\u001b[32m[          ] \u001b[33m" << str() << "\u001b[0m" << std::flush;
		}
	};

#define TEST_COUT  TestCout()






	class CStatisticTest : public testing::Test
	{
	protected:
		// You can remove any or all of the following functions if their bodies would
		// be empty.

		CStatisticTest()
		{
			// You can do set-up work for each test here.
		}

		~CStatisticTest() override
		{
			// You can do clean-up work that doesn't throw exceptions here.
		}

		// If the constructor and destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:

		void SetUp() override
		{
			// Code here will be called immediately after the constructor (right
			// before each test).
			//shared_value=0;
			stat = 5;
			stat += 15;
		}

		void TearDown() override
		{
			// Code here will be called immediately after each test (right
			// before the destructor).
		}

		// Class members declared here can be used by all tests in the test suite
		// for Foo.

		// Members used by tests in this fixture
		CStatistic stat;
	};


	TEST_F(CStatisticTest, TestSomething)
	{
		// Access shared_value or other members of FooTest
		//ASSERT_EQ(shared_value, 0);
		//ASSERT_TRUE(n != nullptr);
		EXPECT_EQ(stat[SUM], 20);
		EXPECT_EQ(stat[MEAN], 10);
		//EXPECT_NE(stat, 2);
		//EXPECT_LE(stat, 0);
		//EXPECT_LT(stat, 1);
		//EXPECT_GE(stat, 0);
		//EXPECT_GT(stat, -1);
		//EXPECT_TRUE
		//EXPECT_FALSE
	}


	TEST(UtilTime, GetMonthName)
	{

		EXPECT_STREQ(GetMonthName(JANUARY, true), "January");
		EXPECT_STREQ(GetMonthName(JANUARY, false), "Jan");
		EXPECT_STREQ(GetMonthTitle(DECEMBER, true), "December");
		EXPECT_STREQ(GetMonthTitle(DECEMBER, false), "Dec");
	}



	TEST(UtilTime, IsLeap)
	{
		EXPECT_FALSE(IsLeap(YEAR_NOT_INIT));
		EXPECT_FALSE(IsLeap(-998));
		EXPECT_FALSE(IsLeap(-4));
		EXPECT_FALSE(IsLeap(-1));
		EXPECT_FALSE(IsLeap(0));
		EXPECT_TRUE(IsLeap(1600));
		EXPECT_FALSE(IsLeap(1700));
		EXPECT_FALSE(IsLeap(1800));
		EXPECT_FALSE(IsLeap(1900));
		EXPECT_TRUE(IsLeap(1904));
		EXPECT_TRUE(IsLeap(2000));
		EXPECT_FALSE(IsLeap(2001));
	}



	TEST(UtilTime, GetNbDays)
	{
		EXPECT_EQ(GetNbDaysPerYear(YEAR_NOT_INIT), 365);
		EXPECT_EQ(GetNbDaysPerYear(-4), 365);
		EXPECT_EQ(GetNbDaysPerYear(0), 365);
		EXPECT_EQ(GetNbDaysPerYear(4), 366);
		EXPECT_EQ(GetNbDaysPerYear(1600), 366);
		EXPECT_EQ(GetNbDaysPerYear(1900), 365);
		EXPECT_EQ(GetNbDaysPerYear(2000), 366);
		EXPECT_EQ(GetNbDaysPerYear(2002), 365);
		EXPECT_EQ(GetNbDaysPerYear(2004), 366);
		EXPECT_EQ(GetNbDaysPerYear(2100), 365);

		EXPECT_EQ(GetNbDaysPerMonth(YEAR_NOT_INIT, JANUARY), 31);
		EXPECT_EQ(GetNbDaysPerMonth(-4, NOVEMBER), 30);
		EXPECT_EQ(GetNbDaysPerMonth(0, OCTOBER), 31);
		EXPECT_EQ(GetNbDaysPerMonth(4, SEPTEMBER), 30);
		EXPECT_EQ(GetNbDaysPerMonth(2002, AUGUST), 31);
		EXPECT_EQ(GetNbDaysPerMonth(2004, JULY), 31);
		EXPECT_EQ(GetNbDaysPerMonth(JUNE), 30);

		EXPECT_EQ(GetNbDaysPerMonth(-4, FEBRUARY), 28);
		EXPECT_EQ(GetNbDaysPerMonth(0, FEBRUARY), 28);
		EXPECT_EQ(GetNbDaysPerMonth(4, FEBRUARY), 29);
		EXPECT_EQ(GetNbDaysPerMonth(2002, FEBRUARY), 28);
		EXPECT_EQ(GetNbDaysPerMonth(2004, FEBRUARY), 29);
		EXPECT_EQ(GetNbDaysPerMonth(FEBRUARY), 28);
	}

	TEST(UtilTime, GetDOY)
	{
		EXPECT_EQ(GetDOY(2000, FEBRUARY, DAY_29), 59);
		EXPECT_EQ(GetDOY(2000, MARCH, DAY_01), 60);
		EXPECT_EQ(GetDOY(2001, MARCH, DAY_01), 59);
		EXPECT_EQ(GetDOY(2000, APRIL, DAY_15), 105);


		EXPECT_EQ(GetFirstDOY(JANUARY), 0);
		EXPECT_EQ(GetLastDOY(DECEMBER), 364);

		EXPECT_EQ(GetLastDOY(FEBRUARY), 58);
		EXPECT_EQ(GetFirstDOY(MARCH), 59);

		EXPECT_EQ(GetLastDOY(2002, FEBRUARY), 58);
		EXPECT_EQ(GetFirstDOY(2002, MARCH), 59);

		EXPECT_EQ(GetLastDOY(2000, FEBRUARY), 59);
		EXPECT_EQ(GetFirstDOY(2000, MARCH), 60);
		EXPECT_EQ(GetFirstDOY(2000, JULY), GetDOY(2000, JULY, DAY_01));
		EXPECT_EQ(GetFirstDOY(2000, AUGUST), GetDOY(2000, AUGUST, DAY_01));
		EXPECT_EQ(GetLastDOY(2004, AUGUST), GetDOY(2004, AUGUST, DAY_31));

		EXPECT_EQ(GetMonth(2000, GetDOY(2000, FEBRUARY, DAY_29)), FEBRUARY);
		EXPECT_EQ(GetMonth(2000, GetDOY(2000, MARCH, DAY_01)), MARCH);
		EXPECT_EQ(GetMonth(1974, GetDOY(1974, APRIL, DAY_15)), APRIL);
		EXPECT_EQ(GetDayOfTheMonth(2000, GetDOY(2000, FEBRUARY, DAY_29)), DAY_29);
		EXPECT_EQ(GetDayOfTheMonth(2000, GetDOY(2000, MARCH, DAY_01)), DAY_01);
		EXPECT_EQ(GetDayOfTheMonth(1974, GetDOY(1974, APRIL, DAY_15)), DAY_15);
	}



	TEST(UtilTime, GetMonth)
	{
		EXPECT_EQ(GetMonth("blabla"), NOT_INIT);


		int32_t Uref = GetRef(2020, SEPTEMBER, DAY_22);
		EXPECT_EQ(Uref, 737689);

		int year = 0;
		size_t m = 0;
		size_t d = 0;
		SetRef(Uref, year, m, d);

		EXPECT_EQ(year, 2020);
		EXPECT_EQ(m, SEPTEMBER);
		EXPECT_EQ(d, DAY_22);
	}



	class CTMTest : public testing::Test
	{
	protected:

		// If the constructor and destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:

		void SetUp() override
		{
			// Code here will be called immediately after the constructor (right
			// before each test).
			TM1 = CTM(CTM::MONTHLY);
			TM2 = CTM(CTM::MONTHLY, CTM::OVERALL_YEARS);
			TM3 = CTM(CTM::ANNUAL, CTM::FOR_EACH_YEAR);
		}

		void TearDown() override
		{
			// Code here will be called immediately after each test (right
			// before the destructor).
		}

		// Class members declared here can be used by all tests in the test suite
		// for Foo.

		// Members used by tests in this fixture
		CTM TM1;
		CTM TM2;
		CTM TM3;
	};







	TEST_F(CTMTest, GetTypeName)
	{

		EXPECT_STREQ(CTM::GetTypeName(CTM::ANNUAL), "Annual");
		EXPECT_STREQ(CTM::GetTypeName(CTM::ATEMPORAL), "Atemporal");
		EXPECT_STREQ(CTM::GetModeName(CTM::FOR_EACH_YEAR), "For each year");
		EXPECT_STREQ(CTM::GetModeName(CTM::OVERALL_YEARS), "Overall years");

		EXPECT_TRUE(CTM::IsAvailable(CTM(CTM::DAILY), CTM(CTM::MONTHLY)));
		EXPECT_FALSE(CTM::IsAvailable(CTM(CTM::MONTHLY), CTM(CTM::DAILY)));
		EXPECT_TRUE(CTM::IsAvailable(CTM(CTM::DAILY), CTM(CTM::DAILY, CTM::OVERALL_YEARS)));

		EXPECT_EQ(TM1.GetTypeName(), "Monthly");
		EXPECT_EQ(TM1.GetModeName(), "For each year");
	}

	TEST_F(CTMTest, Operator)
	{
		EXPECT_TRUE(TM1 != TM2);
		EXPECT_TRUE(TM1 != TM3);

		TM1.Type(CTM::ANNUAL);
		EXPECT_TRUE(TM1 == TM3);
		EXPECT_EQ(TM1, TM3);
	}


	//****************************************************************************************************

	TEST(UtilTime, CTRefFormat)
	{
		EXPECT_STREQ(CTRefFormat::GetDefaultFormat(CTM::DAILY), "%Y-%m-%d");
	}

	//****************************************************************************************************

	class CTRefTest : public testing::Test
	{
	protected:

		// If the constructor and destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:

		void SetUp() override
		{
			// Code here will be called immediately after the constructor (right
			// before each test).
			//TRef1 = CTRef(CTM::MONTHLY);
			//TRef2 = CTRef(CTM::MONTHLY, CTM::OVERALL_YEARS);
			//TRef3 = CTRef(CTM::ANNUAL, CTM::FOR_EACH_YEAR);
		}

		void TearDown() override
		{
			// Code here will be called immediately after each test (right
			// before the destructor).
		}

		// Class members declared here can be used by all tests in the test suite
		// for Foo.

		// Members used by tests in this fixture
		CTRef TRef1;
		CTRef TRef2;
		CTRef TRef3;
	};




	//TEST_F( CTRefTest, Constructor )
	//{
	//
	//    struct STest
	//    {
	//        int32_t m_year : 16;   // -65535..65536  (16 bits)
	//        uint32_t m_month : 4;    // 0..16 (4 bits)
	//        uint32_t m_day : 5;    // 0..31 (5 bits)
	//        uint32_t m_hour : 5;    // 0..31 (5 bits)
	//        uint32_t m_mode: 2;    // 0..4 (2 bits)
	//    };
	//
	//    STest test1 = {2005, JULY, DAY_15, 0, 0};
	//    STest test2 = {2005, OCTOBER, DAY_12, 0, 0};
	//    //bool brep = int32_t( *(void*)(&test1) ) < int32_t( *(void*)(&test2));
	//
	//    const void* p_bit1 = &test1;
	//    int32_t i1 = *((int32_t*) p_bit1);
	//
	//    void* p_bit2 = &test2;
	//    int32_t i2 = *((int32_t*) p_bit2);
	//
	//    EXPECT_EQ( i1 < i2 );
	//}

	TEST_F(CTRefTest, is_valid)
	{
		EXPECT_TRUE(CTRef::is_valid(0, 0, 0, 0, CTM::ANNUAL));
		EXPECT_TRUE(CTRef::is_valid(2000, 0, 0, 0, CTM::ANNUAL));
		EXPECT_TRUE(CTRef::is_valid(YEAR_NOT_INIT, 0, 0, 0, CTM::ANNUAL, CTM::OVERALL_YEARS));
		EXPECT_TRUE(CTRef::is_valid(2000, JANUARY, 0, 0, CTM::MONTHLY));
		EXPECT_TRUE(CTRef::is_valid(YEAR_NOT_INIT, JANUARY, 0, 0, CTM::MONTHLY, CTM::OVERALL_YEARS));
		EXPECT_TRUE(CTRef::is_valid(2000, JANUARY, DAY_01, 0, CTM::DAILY));
		EXPECT_TRUE(CTRef::is_valid(YEAR_NOT_INIT, JANUARY, DAY_01, 0, CTM::DAILY, CTM::OVERALL_YEARS));
		EXPECT_TRUE(CTRef::is_valid(2000, FEBRUARY, DAY_29, 0, CTM::DAILY));
		EXPECT_TRUE(CTRef::is_valid(2000, JANUARY, DAY_01, 0, CTM::HOURLY));
		EXPECT_TRUE(CTRef::is_valid(YEAR_NOT_INIT, JANUARY, DAY_01, 0, CTM::HOURLY, CTM::OVERALL_YEARS));
		EXPECT_TRUE(CTRef::is_valid(412, 0, 0, 0, CTM::ATEMPORAL));


		EXPECT_FALSE(CTRef::is_valid(2000, DECEMBER, DAY_31, 25, CTM::HOURLY));
		EXPECT_FALSE(CTRef::is_valid(2000, DECEMBER, DAY_31, -1, CTM::HOURLY));
		EXPECT_FALSE(CTRef::is_valid(2000, JANUARY, DAY_01 - 1, 0, CTM::DAILY));
		EXPECT_FALSE(CTRef::is_valid(2000, DECEMBER, DAY_31 + 1, 0, CTM::DAILY));
		EXPECT_FALSE(CTRef::is_valid(YEAR_NOT_INIT, FEBRUARY, DAY_29, 0, CTM::DAILY, CTM::OVERALL_YEARS));
		EXPECT_FALSE(CTRef::is_valid(2001, FEBRUARY, DAY_29, 0, CTM::DAILY));
		EXPECT_FALSE(CTRef::is_valid(2000, JANUARY - 1, 0, 0, CTM::MONTHLY));
		EXPECT_FALSE(CTRef::is_valid(2000, DECEMBER + 1, 0, 0, CTM::MONTHLY));
		EXPECT_FALSE(CTRef::is_valid(4000, 0, 0, 0, CTM::ANNUAL));
		EXPECT_FALSE(CTRef::is_valid(0, 0, 0, 0, CTM::ATEMPORAL, CTM::OVERALL_REF));


	}



	TEST_F(CTRefTest, Constructor)
	{
		//cout << "Constructor" << endl;
		CTRef TRefA1(2010);
		EXPECT_EQ(TRefA1.TM().Type(), CTM::ANNUAL);
		EXPECT_EQ(TRefA1.TM().Mode(), CTM::FOR_EACH_YEAR);
		EXPECT_EQ(TRefA1.m_year, 2010);


		CTRef TRefM1(2022, MARCH);
		EXPECT_EQ(TRefM1.TM().Type(), CTM::MONTHLY);
		EXPECT_EQ(TRefM1.TM().Mode(), CTM::FOR_EACH_YEAR);
		EXPECT_EQ(TRefM1.m_year, 2022);
		EXPECT_EQ(TRefM1.m_month, MARCH);


		CTRef TRefD1(2004, FEBRUARY, DAY_29);
		EXPECT_EQ(TRefD1.TM().Type(), CTM::DAILY);
		EXPECT_EQ(TRefD1.TM().Mode(), CTM::FOR_EACH_YEAR);
		EXPECT_EQ(TRefD1.m_year, 2004);
		EXPECT_EQ(TRefD1.m_month, FEBRUARY);
		EXPECT_EQ(TRefD1.m_day, DAY_29);

		CTRef TRefH1(2004, FEBRUARY, DAY_29, 16ull);
		EXPECT_EQ(TRefH1.TM().Type(), CTM::HOURLY);
		EXPECT_EQ(TRefH1.TM().Mode(), CTM::FOR_EACH_YEAR);
		EXPECT_EQ(TRefH1.m_year, 2004);
		EXPECT_EQ(TRefH1.m_month, FEBRUARY);
		EXPECT_EQ(TRefH1.m_day, DAY_29);
		EXPECT_EQ(TRefH1.m_hour, 16);

		CTRef TRefR1(12345, 0, 0, 0, CTM::ATEMPORAL);
		EXPECT_EQ(TRefR1.TM().Type(), CTM::ATEMPORAL);
		EXPECT_EQ(TRefR1.TM().Mode(), CTM::FOR_EACH_REF);
		EXPECT_EQ(TRefR1.m_ref, 12345);

		CTRef TRefD2("2004-06-07");
		EXPECT_EQ(TRefD2.TM().Type(), CTM::DAILY);
		EXPECT_EQ(TRefD2.TM().Mode(), CTM::FOR_EACH_YEAR);
		EXPECT_EQ(TRefD2.m_year, 2004);
		EXPECT_EQ(TRefD2.m_month, JUNE);
		EXPECT_EQ(TRefD2.m_day, DAY_07);

		CTRef TRefH2("1804-11-22 18:00:00");
		EXPECT_EQ(TRefH2.TM().Type(), CTM::HOURLY);
		EXPECT_EQ(TRefH2.TM().Mode(), CTM::FOR_EACH_YEAR);
		EXPECT_EQ(TRefH2.m_year, 1804);
		EXPECT_EQ(TRefH2.m_month, NOVEMBER);
		EXPECT_EQ(TRefH2.m_day, DAY_22);
		EXPECT_EQ(TRefH2.m_hour, 18);


		CTRef TRefA2("01");
		EXPECT_EQ(TRefA2.TM().Type(), CTM::ANNUAL);
		EXPECT_EQ(TRefA2.TM().Mode(), CTM::FOR_EACH_YEAR);
		EXPECT_EQ(TRefA2.m_year, 1);


		CTRef TRefM2("9999-12");
		EXPECT_EQ(TRefM2.TM().Type(), CTM::MONTHLY);
		EXPECT_EQ(TRefM2.TM().Mode(), CTM::OVERALL_YEARS);
		EXPECT_EQ(TRefM2.m_month, DECEMBER);

		CTRef TRefA3("9999");
		EXPECT_EQ(TRefA3.TM().Type(), CTM::ANNUAL);
		EXPECT_EQ(TRefA3.TM().Mode(), CTM::OVERALL_YEARS);

		CTRef TRef;
		EXPECT_FALSE(TRef.is_init());
		TRef = CTRef(2010, SEPTEMBER, DAY_12);
		EXPECT_TRUE(TRef.is_init());
		EXPECT_TRUE(TRef.is_valid());
		TRef.clear();
		EXPECT_FALSE(TRef.is_init());
		EXPECT_TRUE(TRef.is_valid());
	}

	TEST_F(CTRefTest, GetSet)
	{
		CTRef TRef;

		TRef.Set(2020, OCTOBER, DAY_20, 0, CTM::DAILY);
		EXPECT_EQ(TRef, CTRef("2020-10-20"));

		EXPECT_EQ(TRef.GetDOY(), GetDOY(2020, OCTOBER, DAY_20));

		TRef.SetDOY(2020, GetDOY(2020, OCTOBER, DAY_21));
		EXPECT_EQ(TRef, CTRef("2020-10-21"));
	}

	TEST_F(CTRefTest, Operator)
	{
		CTRef TRef1("2010-12-25");
		CTRef TRef2("2011-11-11");

		EXPECT_TRUE(TRef2 != TRef1);
		EXPECT_TRUE(TRef2 > TRef1);
		EXPECT_TRUE(TRef2 >= TRef1);
		EXPECT_TRUE(TRef1 < TRef2);
		EXPECT_TRUE(TRef1 <= TRef2);


		TRef2 = TRef1;
		EXPECT_TRUE(TRef2 == TRef1);
		EXPECT_TRUE(TRef2 >= TRef1);
		EXPECT_TRUE(TRef2 <= TRef1);

		TRef2--;
		EXPECT_EQ(TRef2, CTRef("2010-12-24"));

		EXPECT_EQ(TRef2 - TRef1, -1);
		EXPECT_EQ(TRef1 - TRef2, 1);

		TRef2++;
		EXPECT_EQ(TRef2, CTRef("2010-12-25"));


		TRef2 += 365;
		EXPECT_EQ(TRef2, CTRef("2011-12-25"));

		CTRef TRef3(2011, JANUARY);
		TRef3 -= 2;
		EXPECT_EQ(TRef3, CTRef("2010-11"));

		EXPECT_EQ(TRef3 + 12, CTRef("2011-11"));
		EXPECT_EQ(TRef3 - 12, CTRef("2009-11"));
	}

	TEST_F(CTRefTest, as)
	{
		CTRef TRef("1111-11-11 11:00:00");
		EXPECT_EQ(TRef.as(CTM(CTM::HOURLY, CTM::FOR_EACH_YEAR)), CTRef("1111-11-11 11:00:00"));
		EXPECT_EQ(TRef.as(CTM(CTM::HOURLY, CTM::OVERALL_YEARS)), CTRef("9999-11-11 11:00:00"));
		EXPECT_EQ(TRef.as(CTM(CTM::DAILY, CTM::FOR_EACH_YEAR)), CTRef("1111-11-11"));
		EXPECT_EQ(TRef.as(CTM(CTM::DAILY, CTM::OVERALL_YEARS)), CTRef("9999-11-11"));
		EXPECT_EQ(TRef.as(CTM(CTM::MONTHLY, CTM::FOR_EACH_YEAR)), CTRef("1111-11"));
		EXPECT_EQ(TRef.as(CTM(CTM::MONTHLY, CTM::OVERALL_YEARS)), CTRef("9999-11"));
		EXPECT_EQ(TRef.as(CTM(CTM::ANNUAL, CTM::FOR_EACH_YEAR)), CTRef("1111"));
		EXPECT_EQ(TRef.as(CTM(CTM::ANNUAL, CTM::OVERALL_YEARS)), CTRef("9999"));

		TRef = CTRef("1111");
		EXPECT_EQ(TRef.as(CTM(CTM::MONTHLY, CTM::FOR_EACH_YEAR), CTM::FIRST_TREF), CTRef("1111-01"));
		EXPECT_EQ(TRef.as(CTM(CTM::MONTHLY, CTM::FOR_EACH_YEAR), CTM::MID_TREF), CTRef("1111-06"));
		EXPECT_EQ(TRef.as(CTM(CTM::MONTHLY, CTM::FOR_EACH_YEAR), CTM::LAST_TREF), CTRef("1111-12"));
		EXPECT_EQ(TRef.as(CTM(CTM::MONTHLY, CTM::OVERALL_YEARS), CTM::FIRST_TREF), CTRef("9999-01"));
		EXPECT_EQ(TRef.as(CTM(CTM::MONTHLY, CTM::OVERALL_YEARS), CTM::MID_TREF), CTRef("9999-06"));
		EXPECT_EQ(TRef.as(CTM(CTM::MONTHLY, CTM::OVERALL_YEARS), CTM::LAST_TREF), CTRef("9999-12"));

		TRef = CTRef("1111-11");
		EXPECT_EQ(TRef.as(CTM(CTM::DAILY, CTM::FOR_EACH_YEAR), CTM::FIRST_TREF), CTRef("1111-11-01"));
		EXPECT_EQ(TRef.as(CTM(CTM::DAILY, CTM::FOR_EACH_YEAR), CTM::MID_TREF), CTRef("1111-11-15"));
		EXPECT_EQ(TRef.as(CTM(CTM::DAILY, CTM::FOR_EACH_YEAR), CTM::LAST_TREF), CTRef("1111-11-30"));
		EXPECT_EQ(TRef.as(CTM(CTM::DAILY, CTM::OVERALL_YEARS), CTM::FIRST_TREF), CTRef("9999-11-01"));
		EXPECT_EQ(TRef.as(CTM(CTM::DAILY, CTM::OVERALL_YEARS), CTM::MID_TREF), CTRef("9999-11-15"));
		EXPECT_EQ(TRef.as(CTM(CTM::DAILY, CTM::OVERALL_YEARS), CTM::LAST_TREF), CTRef("9999-11-30"));

		TRef = CTRef("1111-11-11");
		EXPECT_EQ(TRef.as(CTM(CTM::HOURLY, CTM::FOR_EACH_YEAR), CTM::FIRST_TREF), CTRef("1111-11-11 00:00:00"));
		EXPECT_EQ(TRef.as(CTM(CTM::HOURLY, CTM::FOR_EACH_YEAR), CTM::MID_TREF), CTRef("1111-11-11 11:00:00"));
		EXPECT_EQ(TRef.as(CTM(CTM::HOURLY, CTM::FOR_EACH_YEAR), CTM::LAST_TREF), CTRef("1111-11-11 23:00:00"));
		EXPECT_EQ(TRef.as(CTM(CTM::HOURLY, CTM::OVERALL_YEARS), CTM::FIRST_TREF), CTRef("9999-11-11 00:00:00"));
		EXPECT_EQ(TRef.as(CTM(CTM::HOURLY, CTM::OVERALL_YEARS), CTM::MID_TREF), CTRef("9999-11-11 11:00:00"));
		EXPECT_EQ(TRef.as(CTM(CTM::HOURLY, CTM::OVERALL_YEARS), CTM::LAST_TREF), CTRef("9999-11-11 23:00:00"));
	}

	TEST_F(CTRefTest, TemporalAtemporal)
	{
		EXPECT_TRUE(CTRef("2008-11-11 23:00:00").IsTemporal());
		EXPECT_FALSE(CTRef("9999-11").IsAtemporal());
		EXPECT_TRUE(CARef(0).IsAtemporal());
	}

	TEST_F(CTRefTest, String)
	{
		CTRef Tref("2024-10-16");
		EXPECT_EQ(Tref, CTRef(2024, OCTOBER, DAY_16));
		EXPECT_STREQ(Tref.to_string().c_str(), "2024-10-16");

		Tref = CTRef("2024-10-16 04:11:00");
		EXPECT_EQ(Tref, CTRef(2024, OCTOBER, DAY_16, 4));
		EXPECT_STREQ(Tref.to_string().c_str(), "2024-10-16 04:00:00");//minutes, seconds not supported
	}

	TEST_F(CTRefTest, SerializeText)
	{
		CTRef Tref1("2024-10-16");
		CTRef Tref2;


		std::stringstream memory;
		try
		{
			//unsigned int flags = boost::archive::no_header;

			boost::archive::text_oarchive oa(memory);
			oa << boost::serialization::make_nvp("CTRef", Tref1);
		}
		catch (boost::archive::archive_exception& e)
		{
			cout << "Error writing: " << e.what() << endl;
		}
		catch (...)
		{
			cout << "Error writing " << endl;
		}


		try
		{
			std::string test = memory.str();
			TEST_COUT << "Text: " << test << std::endl;


			boost::archive::text_iarchive ia(memory);
			ia >> BOOST_SERIALIZATION_NVP(Tref2);
		}
		catch (boost::archive::archive_exception& e)
		{
			cout << "Error reading: " << e.what() << endl;
		}
		catch (...)
		{
			cout << "Error reading " << endl;
		}

		EXPECT_EQ(Tref2, Tref1);

	}

	TEST_F(CTRefTest, SerializeXml)
	{
		CTRef Tref1("2024-10-16");
		CTRef Tref2;
		CTRef Tref3;


		std::stringstream memory;
		try
		{
			// unsigned int flags = boost::archive::no_header;
			 //boost::archive::xml_oarchive oa(memory, boost::archive::no_header);
			boost::archive::xml_oarchive oa(memory);
			oa.register_type(static_cast<CTRef*>(NULL));

			oa& boost::serialization::make_nvp("obj1", Tref1);
			oa& boost::serialization::make_nvp("obj2", Tref1);
		}
		catch (boost::archive::archive_exception& e)
		{
			cout << "Error writing: " << e.what() << endl;
		}
		catch (...)
		{
			cout << "Error writing " << endl;
		}


		try
		{
			std::string test = memory.str();
			TEST_COUT << "Text: " << test << std::endl;


			boost::archive::xml_iarchive ia(memory);
			ia& BOOST_SERIALIZATION_NVP(Tref2);
		}
		catch (boost::archive::archive_exception& e)
		{
			cout << "Error reading: " << e.what() << endl;
		}
		catch (...)
		{
			cout << "Error reading " << endl;
		}

		EXPECT_EQ(Tref2, Tref1);

	}


	//****************************************************************************************************

	TEST(UtilTime, CDOYRef)
	{
		CTRef Tref("2019-09-09");
		CDOYRef DOYref(Tref.GetYear(), Tref.GetDOY());

		EXPECT_EQ(DOYref, Tref);
	}

	//****************************************************************************************************

	TEST(UtilTime, CTimeRef)
	{

		CTRef Tref1("2019-09-09");
		std::time_t time64 = Tref1.get_time64();

		CTimeRef Tref2(time64);

		EXPECT_EQ(Tref2, Tref1);

	}



	//****************************************************************************************************
	class CTPeriodTest : public testing::Test
	{
	protected:

		// If the constructor and destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:

		void SetUp() override
		{
			// Code here will be called immediately after the constructor (right
			// before each test).
			//TRef1 = CTRef(CTM::MONTHLY);
			//TRef2 = CTRef(CTM::MONTHLY, CTM::OVERALL_YEARS);
			//TRef3 = CTRef(CTM::ANNUAL, CTM::FOR_EACH_YEAR);
		}

		void TearDown() override
		{
			// Code here will be called immediately after each test (right
			// before the destructor).
		}

		// Class members declared here can be used by all tests in the test suite
		// for Foo.

		// Members used by tests in this fixture
		CTPeriod p1;
		CTPeriod p2;

	};




	TEST_F(CTPeriodTest, is_valid)
	{
		EXPECT_TRUE(CTPeriod::is_valid(CTRef("2024-10-16"), CTRef("2024-10-16")));
		EXPECT_TRUE(CTPeriod::is_valid(CTRef("2024-10-16"), CTRef("2024-10-20")));
		EXPECT_FALSE(CTPeriod::is_valid(CTRef("2024-10-20"), CTRef("2024-10-16")));
		EXPECT_FALSE(CTPeriod::is_valid(CTRef("2024-10-20"), CTRef("2024-11")));
	}

	TEST_F(CTPeriodTest, String)
	{
		CTPeriod p("2004-02-29|2004-06-07");
		EXPECT_EQ(p.begin(), CTRef(2004, FEBRUARY, DAY_29));
		EXPECT_EQ(p.end(), CTRef(2004, JUNE, DAY_07));
		EXPECT_EQ(p, CTPeriod("2004-02-29|2004-06-07"));
		EXPECT_EQ(p, CTPeriod("2004-02-29", "2004-06-07"));
		EXPECT_EQ(p, CTPeriod(CTRef("2004-02-29"), CTRef("2004-06-07")));
	}

	TEST_F(CTPeriodTest, Constructor)
	{
		CTRef TRefA1(2010);
		CTRef TRefA2("2020");

		CTRef TRefM1(2022, MARCH);
		CTRef TRefM2("2024-12");


		CTRef TRefD1(2004, FEBRUARY, DAY_29);
		CTRef TRefD2("2004-06-07");

		CTRef TRefH1(2004, FEBRUARY, DAY_29, 16);
		CTRef TRefH2("2004-11-22 18:00:00");

		CTRef TRefR1(12345, 0, 0, 0, CTM::ATEMPORAL);
		CTRef TRefR2(23456, 0, 0, 0, CTM::ATEMPORAL);


		CTPeriod pA1(TRefA1, TRefA2);
		CTPeriod pA2("2010", "2020");
		EXPECT_EQ(pA1.begin(), TRefA1);
		EXPECT_EQ(pA1.end(), TRefA2);
		EXPECT_EQ(pA1, pA2);
		EXPECT_EQ(pA1.size(), 11);


		CTPeriod pM1(TRefM1, TRefM2);
		CTPeriod pM2("2022-03", "2024-12");
		EXPECT_EQ(pM1.begin(), TRefM1);
		EXPECT_EQ(pM1.end(), TRefM2);
		EXPECT_EQ(pM1, pM2);

		CTPeriod pD1(TRefD1, TRefD2);
		CTPeriod pD2("2004-02-29", "2004-06-07");
		EXPECT_EQ(pD1.begin(), TRefD1);
		EXPECT_EQ(pD1.end(), TRefD2);
		EXPECT_EQ(pD1, pD2);

		CTPeriod pH1(TRefH1, TRefH2);
		CTPeriod pH2("2004-02-29 16:00:00", "2004-11-22 18:00:00");
		EXPECT_EQ(pH1.begin(), TRefH1);
		EXPECT_EQ(pH1.end(), TRefH2);
		EXPECT_EQ(pH1, pH2);

		CTPeriod pR1(TRefR1, TRefR2);
		CTPeriod pR2(CARef(12345), CARef(23456));
		EXPECT_EQ(pR1.begin(), TRefR1);
		EXPECT_EQ(pR1.end(), TRefR2);
		EXPECT_EQ(pR1, pR2);

		CTPeriod pH3("2004-02-29 16:00:00|2004-11-22 18:00:00");
		EXPECT_EQ(pH3, pH1);

		CTPeriod pD3("2004-02-29|2004-06-07");
		EXPECT_EQ(pD3, pD1);


		CTPeriod pM3(pM2);
		EXPECT_EQ(pM3, pM1);

		pA2 = pA1;
		EXPECT_EQ(pA2, pA1);

		pA2.clear();
		EXPECT_TRUE(pA2 != pA1);
		EXPECT_EQ(pA2.size(), 0);
	}


	TEST_F(CTPeriodTest, Operator)
	{
		CTPeriod p1(CTRef("2010-12-25"), CTRef("2011-01-11"));
		CTPeriod p2 = p1;

		EXPECT_TRUE(p2.is_inside(CTRef("2010-12-31")));
		EXPECT_TRUE(p2.is_inside(p2));
		EXPECT_TRUE(p2.is_intersect(p1));

		p2 += CTRef("2010-12-31");
		EXPECT_EQ(p2, p1);

		EXPECT_FALSE(p2.is_inside(CTRef("2010-12-20")));
		p2 += CTRef("2010-12-20");
		EXPECT_TRUE(p2 != p1);
		EXPECT_EQ(p2.begin(), CTRef(2010, DECEMBER, DAY_20));
		EXPECT_EQ(p2.end(), p1.end());
		EXPECT_TRUE(p2.is_intersect(p1));

		EXPECT_FALSE(p2.is_inside(CTRef("2011-01-12")));
		p2 += CTRef("2011-01-20");
		EXPECT_EQ(p2.begin(), CTRef(2010, DECEMBER, DAY_20));
		EXPECT_EQ(p2.end(), CTRef(2011, JANUARY, DAY_20));
		EXPECT_TRUE(p2.is_intersect(p1));

		p2 = p1;

		p2.inflate(CTRef("2010-12-31"));
		EXPECT_EQ(p2, p1);

		p2.inflate(CTRef("2010-12-20"));
		EXPECT_TRUE(p2 != p1);
		EXPECT_EQ(p2.begin(), CTRef(2010, DECEMBER, DAY_20));
		EXPECT_EQ(p2.end(), p1.end());

		p2.inflate(CTRef("2011-01-20"));
		EXPECT_EQ(p2.begin(), CTRef(2010, DECEMBER, DAY_20));
		EXPECT_EQ(p2.end(), CTRef(2011, JANUARY, DAY_20));

		EXPECT_FALSE(p2.is_intersect(CTPeriod("2010-01-01|2010-12-19")));
		EXPECT_TRUE(p2.is_intersect(CTPeriod("2010-01-01|2010-12-20")));
		EXPECT_TRUE(p2.is_intersect(CTPeriod("2011-01-20|2011-01-20")));
		EXPECT_FALSE(p2.is_intersect(CTPeriod("2011-01-21|2011-01-28")));

		EXPECT_EQ(CTPeriod::unions(CTPeriod("2010-01-01|2010-01-10"), CTPeriod("2010-01-20|2010-01-31")), CTPeriod("2010-01-01|2010-01-31"));
		EXPECT_EQ(CTPeriod::unions(CTPeriod("2010-01-01|2010-12-31"), CTPeriod("2010-07-01|2010-07-31")), CTPeriod("2010-01-01|2010-12-31"));
		EXPECT_EQ(CTPeriod::unions(CTPeriod("2010-01-01|2010-01-10"), CTRef("2010-01-31")), CTPeriod("2010-01-01|2010-01-31"));

		EXPECT_FALSE(CTPeriod::intersect(CTPeriod("2010-01-01|2010-01-10"), CTPeriod("2010-01-20|2010-01-31")).is_init());
		EXPECT_FALSE(CTPeriod::intersect(CTPeriod("2010-01-01|2010-01-10"), CTRef("2010-01-31")).is_init());
		EXPECT_EQ(CTPeriod::intersect(CTPeriod("2010-01-01|2010-01-31"), CTPeriod("2010-01-15|2010-02-15")), CTPeriod("2010-01-15|2010-01-31"));
		EXPECT_EQ(CTPeriod::intersect(CTPeriod("2010-01-15|2010-01-15"), CTPeriod("2010-01-15|2010-01-15")), CTPeriod("2010-01-15|2010-01-15"));
		EXPECT_EQ(CTPeriod::intersect(CTPeriod("2010-01-01|2010-01-31"), CTPeriod("2010-01-15|2010-01-17")), CTPeriod("2010-01-15|2010-01-17"));
		EXPECT_EQ(CTPeriod::intersect(CTPeriod("2010-01-15|2010-01-31"), CTPeriod("2010-01-01|2010-01-15")), CTPeriod("2010-01-15|2010-01-15"));
		EXPECT_EQ(CTPeriod::intersect(CTPeriod("2010-01-01|2010-01-10"), CTRef("2010-01-05")), CTPeriod("2010-01-05|2010-01-05"));


		p1 += p2;
		EXPECT_EQ(p1, p2);

		EXPECT_EQ(p1[4], CTRef("2010-12-24"));
		EXPECT_FALSE(p1[-1].is_init());
		EXPECT_FALSE(p1[p1.size() + 1].is_init());
		EXPECT_EQ(p1[0], p1.begin());
		EXPECT_EQ(p1[p1.size() - 1], p1.end());


		EXPECT_EQ(p1[CTRef("2010-12-24")], 4);
		EXPECT_EQ(p1[p1.begin()], 0);
		EXPECT_EQ(p1[p1.end()], p1.size() - 1);
		EXPECT_EQ(p1[p1.begin() - 1], NOT_INIT);
		EXPECT_EQ(p1[p1.end() + 1], NOT_INIT);
	}

	TEST_F(CTPeriodTest, Others)
	{
		CTPeriod p1("2000-12", "2010-01");

		EXPECT_EQ(p1.GetFirstYear(), 2000);
		EXPECT_EQ(p1.GetLastYear(), 2010);
		EXPECT_EQ(p1.GetNbYears(), 11);
		EXPECT_EQ(p1.TM(), CTM::MONTHLY);

		p1.TM(CTM(CTM::MONTHLY, CTM::OVERALL_YEARS));
		EXPECT_EQ(p1.GetFirstYear(), 9999);
		EXPECT_EQ(p1.GetLastYear(), 9999);
		EXPECT_EQ(p1.GetNbYears(), 1);
		EXPECT_TRUE(p1.is_countinious());
	}

	TEST_F(CTPeriodTest, as)
	{
		CTPeriod p("1111-11-11 11:00:00", "1212-12-12 12:00:00");

		EXPECT_EQ(p.as(CTM(CTM::HOURLY, CTM::FOR_EACH_YEAR)), CTPeriod("1111-11-11 11|1212-12-12 12"));
		EXPECT_EQ(p.as(CTM(CTM::HOURLY, CTM::OVERALL_YEARS)), CTPeriod("9999-11-11 11|9999-12-12 12"));
		EXPECT_EQ(p.as(CTM(CTM::DAILY, CTM::FOR_EACH_YEAR)), CTPeriod("1111-11-11|1212-12-12"));
		EXPECT_EQ(p.as(CTM(CTM::DAILY, CTM::OVERALL_YEARS)), CTPeriod("9999-11-11|9999-12-12"));
		EXPECT_EQ(p.as(CTM(CTM::MONTHLY, CTM::FOR_EACH_YEAR)), CTPeriod("1111-11|1212-12"));
		EXPECT_EQ(p.as(CTM(CTM::MONTHLY, CTM::OVERALL_YEARS)), CTPeriod("9999-11|9999-12"));
		EXPECT_EQ(p.as(CTM(CTM::ANNUAL, CTM::FOR_EACH_YEAR)), CTPeriod("1111|1212"));
		EXPECT_EQ(p.as(CTM(CTM::ANNUAL, CTM::OVERALL_YEARS)), CTPeriod("9999", "9999"));

		p = CTPeriod("2000-12-01", "2001-01-31");
		EXPECT_EQ(p.as(CTM(CTM::DAILY, CTM::OVERALL_YEARS)), CTPeriod("9999-01-31|9999-12-01"));//month is revert
	}



	TEST_F(CTPeriodTest, Serialize)
	{

		//        CTPeriod p1("2010-10-10","2012-12-12");
		//
		//        cout << sizeof(p1) << endl;
		//        //EXPECT_EQ( sizeof(p1) , );
		//        using namespace boost::interprocess;
		//        managed_shared_memory segment(create_only,
		//                "MySharedMemory",  //segment name
		//                65536);
		//
		//        //Allocate a buffer in shared memory to write data
		//        char *my_cstring = segment.construct<char>("MyCString")[sizeof(p1)+1](0);
		//        bufferstream io(my_cstring, sizeof(p1)+1);
		//
		//        io << p1;
		//
		//
		//          //Check there was no overflow attempt
		//        assert(io.good());
		//
		//
		//        CTPeriod p2;
		//
		//        io.seekp(0, std::ios::beg);
		//        io >> p2;
		//
		//        io.clear();
		//
		//        EXPECT_EQ( p2 , p1 );

	}



	TEST(UtilTime, CTReferencedVector)
	{
		CTPeriod p("2024-10-16|2024-10-20");
		CTReferencedVector<float> v(p);
		EXPECT_TRUE(v.is_init());
		EXPECT_EQ(v.size(), p.size());
	}





}