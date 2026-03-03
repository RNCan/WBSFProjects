cmake_minimum_required(VERSION 3.25)

project(WBSFTest)

find_package(GTest CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES main.cpp testARModel.cpp TestUtilTime.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_executable(WBSFTest 
  ${SOURCE_FILES}  
)

target_link_libraries(WBSFTest PUBLIC
	GTest::gtest 
	GTest::gtest_main 
	GTest::gmock 
	GTest::gmock_main	
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
	BioSIM_API
)
