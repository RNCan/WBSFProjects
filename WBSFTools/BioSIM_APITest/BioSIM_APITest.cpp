#include <valarray>
#include <array>
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream> 
#include <sstream>
#include <vector>

#include "BioSIM_API.h"
#include "BioSIM_APITest.h"
#include "Basic/UtilStd.h"

using namespace std;

std::vector<std::string> splitStringStream(const std::string& str, char delimiter) 
{
  std::vector<std::string> tokens;
  std::stringstream ss(str); // Create a stringstream from the input string
  std::string token;

  while (std::getline(ss, token, delimiter)) { // Read tokens until the delimiter is found
      tokens.push_back(token);
  }

  return tokens;
}

namespace BioSIM_APITest
{
  // Override this to define how to set up the environment.
  void BioSIMTestEnvironment::SetUp()
  {
    WBSF::CBioSIM_API_GlobalData global;
    std::string curpath = std::filesystem::current_path().string();
    std::string msg = global.InitGlobalData("DailyCacheSize=50&Shore=testData/Layers/Shore.ann&DEM=testData/DEM/Demo 30s(SRTM30).tif&ModelsPath=Models/");  // Note: the ModelsPath is set to "Models/" which is a relative path. 
                                                                                                                                                            // Those models are copied to the working directory by a post-build command in the CMakeLists.txt file. 
                                                                                                                                                            // If you change this path, make sure to also update the post-build command in the CMakeLists.txt file to copy the models to the correct location.
    EXPECT_EQ(msg, "Success") << "Global Data Initialization should return Success";
  }

  // Override this to define how to tear down the environment.
  void BioSIMTestEnvironment::TearDown()
  {
  }   


  // IMPORTANT
  // The following integration test suite uses testData files that must be present in the working directory.  This data is automatically copied by a post-build command in the CMakeLists.txt file.
  // Also, any model DLL that the test suite depends on must be specified as a dependency and in a post-build command in the CMakeLists.txt file to ensure it is built and copied before the test suite is run.
  // IMPORTANT
  TEST(BioSIMCoreTests, Test01_GetNormals)
	{
    std::string options = "Normals=testData/Weather/Normals/World 1991-2020.NormalsDB";
    WBSF::CWeatherGeneratorAPI weatherGen("");
    std::string msg = weatherGen.Initialize(options);
    EXPECT_EQ(msg, "Success") << "WeatherGenerator initialization should return Success";

    options = "Compress=0&Variables=TN T TX P H WS&ID=1&Name=Logan&Latitude=41.73333333&Longitude=-111.8&Elevation=120";
    WBSF::CTeleIO normals = weatherGen.GetNormals(options);
    EXPECT_EQ(normals.m_msg, "Success") << "GetNormals should return Success";
	}

  TEST(BioSIMCoreTests, Test02_WeatherGenerator)
  {
    std::string options = "Normals=testData/Weather/Normals/World 1991-2020.NormalsDB&Daily=testData/Weather/Daily/Demo 2005-2010.DailyDB";
    WBSF::CWeatherGeneratorAPI weatherGen("");
    std::string msg = weatherGen.Initialize(options);
    EXPECT_EQ(msg, "Success") << "WeatherGenerator initialization should return Success";

    options = "Compress=0&Variables=TN T TX P H WS WD R&ID=1&Name=Logan&Latitude=41.73333333&Longitude=-111.8&Elevation=120&First_year=2008&Last_year=2010&Replications=1";
    WBSF::CTeleIO normals = weatherGen.GetNormals(options);
    EXPECT_EQ(normals.m_msg, "Success") << "GetNormals should return Success";
  }

  TEST(BioSIMCoreTests, Test03_Model)
  {
    std::string options = "Normals=testData/Weather/Normals/World 1991-2020.NormalsDB&Daily=testData/Weather/Daily/Demo 2005-2010.DailyDB";
    WBSF::CWeatherGeneratorAPI weatherGen("");
    std::string msg = weatherGen.Initialize(options);
    EXPECT_EQ(msg, "Success") << "WeatherGeneratorAPI initialization should return Success";

    WBSF::CModelExecutionAPI model("");
    options = "Model=DegreeDay(Annual).mdl";
    msg = model.Initialize(options);
    EXPECT_EQ(msg, "Success") << "ModelExecutionAPIinitialization should return Success";

    std::string variables = model.GetWeatherVariablesNeeded();
    std::string parameters = model.GetDefaultParameters();
    std::string compress = "0";
    options = "Compress=" + compress + "&Variables=" + variables + "&ID=1&Name=Logan&Latitude=41.73333333&Longitude=-111.8&Elevation=120&First_year=2008&Last_year=2010&Replications=1";
    WBSF::CTeleIO WGout = weatherGen.Generate(options);
    EXPECT_EQ(WGout.m_msg, "Success") << "Generate should return Success";

    WBSF::CTeleIO modelOut = model.Execute("Compress=" + compress, WGout);
    EXPECT_EQ(modelOut.m_msg, "Success") << "Generate should return Success";
    std::string s = modelOut.m_data;
    EXPECT_TRUE(s.starts_with("Year,DD")) << "Result of DegreeDay model should start with [Year,DD]";
  }

  TEST(BioSIMCoreTests, Test04_HemlockLooperModelInit)
  {    
    WBSF::CModelExecutionAPI model("");
    std::string options = "Model=HemlockLooper.mdl";
    std::string msg = model.Initialize(options);
    EXPECT_EQ(msg, "Success") << "ModelExecutionAPIinitialization should return Success";    
  }

  TEST(BioSIMCoreTests, Test05_WeatherGenerator_CanadaUSA_1980_2020_Init)
  {
    std::string options = "Normals=testData/Weather/Normals/World 1991-2020.NormalsDB&Daily=testData/Weather/Daily/Canada-USA 1980-2020.DailyDB";
    WBSF::CWeatherGeneratorAPI weatherGen("");
    std::string msg = weatherGen.Initialize(options);
    EXPECT_EQ(msg, "Success") << "WeatherGenerator initialization should return Success";
  }

  TEST(BioSIMCoreTests, Test06_ClimaticModel_Validation)
  {
      std::string options = "Normals=testData/Weather/Normals/World 1991-2020.NormalsDB&Daily=testData/Weather/Daily/Demo 2000-2005.DailyDB";
      WBSF::CWeatherGeneratorAPI weatherGen("");
      std::string msg = weatherGen.Initialize(options);
      EXPECT_EQ(msg, "Success") << "WeatherGenerator initialization should return Success";

      options = "Latitude=46.65&Longitude=-70.25&Elevation=150&compress=0&Variables=TN+T+TX+TX+P+TD+H+R&Source=FromObservation&First_year=2000&Last_year=2003&Replications=1";
      WBSF::CTeleIO WGout = weatherGen.Generate(options);
      EXPECT_EQ(WGout.m_msg, "Success") << "Generate should return Success";

      // execute Climatic(Annual)
      //options = "model=testData/Models/Climatic(Annual).mdl";
      options = "model=Climatic(Annual).mdl";
      WBSF::CModelExecutionAPI modelClim("");
      msg = modelClim.Initialize(options);
      EXPECT_EQ(msg, "Success") << "Model initialization should return Success";

      options = "compress=0";
      WBSF::CTeleIO ModelClimOut = modelClim.Execute(options, WGout);
      EXPECT_EQ(ModelClimOut.m_msg, "Success") << "Execute should return Success";
      


      // open validation file
      std::ifstream file("testData/Validation/BioSIMTests.APIControllerTests.Test24_BioSimWeatherHappyPath.txt");
      ASSERT_TRUE(file.is_open()) << "Could not open validation file";
      std::stringstream buffer;
      buffer << file.rdbuf();
      std::string validationData = buffer.str();

      // compare results with validation data
      EXPECT_EQ(ModelClimOut.m_data, validationData) << "Generated weather data should match validation data";
  }

  TEST(BioSIMCoreTests, Test07_WeatherGenerator_Gradients)
  {    
    // Here we test that running the WG with a specific case where the number of stations is lower than expected
    std::string options = "Normals=testData/Weather/Normals/World 1991-2020.NormalsDB&Daily=testData/Weather/Daily/Demo 2000-2005.DailyDB";
    WBSF::CWeatherGeneratorAPI weatherGen("");
    std::string msg = weatherGen.Initialize(options);
    EXPECT_EQ(msg, "Success") << "WeatherGenerator initialization should return Success";

    options = "nb_nearest_neighbor=50&Latitude=49.84446273509617&Longitude=-71.92627622323562&Elevation=326.58317098134694&compress=0&Variables=TN+T+TX+P+TD+H+WS+WD+R+Z+S+SD+SWE+WS2&Source=FromObservation&First_year=2000&Last_year=2001&Replications=1";
    WBSF::CTeleIO WGout = weatherGen.Generate(options);
    EXPECT_NE(WGout.m_msg, "Success") << "Generate should not return Success";
  }

  TEST(BioSIMCoreTests, Test08_WeatherGenerator_Run_Twice_Produces_Same_Results)
  {    
    // Here we test that running the WG twice in Source=FromObservation mode with the same options produces the same results. 
    std::string options = "Normals=testData/Weather/Normals/World 1991-2020.NormalsDB&Daily=testData/Weather/Daily/Demo 2005-2010.DailyDB";
    WBSF::CWeatherGeneratorAPI weatherGen("");
    std::string msg = weatherGen.Initialize(options);
    EXPECT_EQ(msg, "Success") << "WeatherGenerator initialization should return Success";

    options = "Latitude=47&Longitude=-70&Elevation=300&compress=0&Variables=TN+T+TX+P+TD+H+WS+WD+R+Z+S+SD+SWE+WS2&Source=FromObservation&First_year=2009&Last_year=2009&Replications=1";
    WBSF::CTeleIO WGout1 = weatherGen.Generate(options);
    EXPECT_EQ(WGout1.m_msg, "Success") << "Generate should return Success";

    WBSF::CTeleIO WGout2 = weatherGen.Generate(options);
    EXPECT_EQ(WGout2.m_msg, "Success") << "Generate should return Success";

    // now compare WGout and WGout2
    EXPECT_TRUE(WGout1 == WGout2) << "WG generated data outputs should be the same";
  }

  TEST(BioSIMCoreTests, Test09_WeatherGeneratorRequest)
  {    
    // Here we test that running the WG with a specific case where the number of stations is lower than expected
    std::string options = "Normals=testData/Weather/Normals/World 1991-2020.NormalsDB&Daily=testData/Weather/Daily/Demo 2000-2005.DailyDB";
    WBSF::CWeatherGeneratorAPI weatherGen("");
    std::string msg = weatherGen.Initialize(options);
    EXPECT_EQ(msg, "Success") << "WeatherGenerator initialization should return Success";

    options = "Latitude=46&Longitude=-70&Elevation=300&compress=0&Variables=TN+T+TX+P+TD+H+WS+WD+R+Z+S+SD+SWE+WS2&Source=FromObservation&First_year=2000&Last_year=2003&Replications=1";
    WBSF::CTeleIO WGout = weatherGen.Generate(options);
    EXPECT_EQ(WGout.m_msg, "Success") << "Generate should return Success";
  }

  TEST(BioSIMCoreTests, Test10_SoilMoistureIndex_Validation)
  {
    std::string options = "Normals=testData/Weather/Normals/World 1991-2020.NormalsDB";
    WBSF::CWeatherGeneratorAPI weatherGen("");
    std::string msg = weatherGen.Initialize(options);
    EXPECT_EQ(msg, "Success") << "WeatherGenerator initialization should return Success";
    
    options = "Latitude=46.1&Longitude=-70.1&Elevation=300&compress=0&Variables=TN+T+TX+P&Source=FromNormals&nb_years=5&Replications=1";
    WBSF::CTeleIO WGout = weatherGen.Generate(options);
    EXPECT_EQ(WGout.m_msg, "Success") << "Generate should return Success";

    // execute SoilMoistureIndex(Annual).mdl
    options = "model=SoilMoistureIndex(Annual).mdl";
    WBSF::CModelExecutionAPI modelClim("");
    msg = modelClim.Initialize(options);
    EXPECT_EQ(msg, "Success") << "Model initialization should return Success";

    // here we replace the year values in the TeleIO output from the WG to replicate what the API is doing
    auto lines = splitStringStream(WGout.m_data, '\n');

    std::stringstream ss;

    for (auto line : lines)
    {
      auto fields = splitStringStream(line, ',');
      if (fields[0] != "Year")
        fields[0] = std::to_string(std::stoi(fields[0]) + 2007);

      for (auto field = fields.begin(); field != fields.end(); ++field)
      {
        ss << *field;
        if (std::next(field) == fields.end())
          ss << "\n";
        else
          ss << ",";
      }
    }

    WGout.m_data = ss.str();

    options = "compress=0";
    WBSF::CTeleIO ModelClimOut = modelClim.Execute(options, WGout);
    EXPECT_EQ(ModelClimOut.m_msg, "Success") << "Execute should return Success";      

    lines = splitStringStream(ModelClimOut.m_data, '\n');
    for (auto line : lines)
    {
      auto fields = splitStringStream(line, ',');
      if (fields[0] != "Year")
      { // this is a data line
        float SMImin = std::stof(fields[1]);
        EXPECT_TRUE(SMImin >= 0.0f && SMImin <= 100.0f) << "SMImin value " << SMImin << " outside of expected range [0-100]";

        float SMImean = std::stof(fields[2]);
        EXPECT_TRUE(SMImean >= 0.0f && SMImean <= 100.0f) << "SMImean value " << SMImean << " outside of expected range [0-100]";

        float SMImax = std::stof(fields[3]);
        EXPECT_TRUE(SMImax >= 0.0f && SMImax <= 100.0f) << "SMImax value " << SMImax << " outside of expected range [0-100]";
      }
    }
  }
}

