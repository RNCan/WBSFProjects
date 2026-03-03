#include <boost/filesystem.hpp>

#include <gtest/gtest.h>


#include "Basic/UtilMath.h"
#include "Basic/UtilTime.h"


int main(int argc, char **argv)
{

    boost::filesystem::exists("file.txt");
  testing::InitGoogleTest(&argc, argv);
  int result =  RUN_ALL_TESTS();


  return result;
}
