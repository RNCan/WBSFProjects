cmake_minimum_required(VERSION 3.25)
project(WBSFProjects LANGUAGES CXX)

# Optionally enforce C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DMUPARSER_STATIC)

# Add subdirectories (order matters: core first)
add_subdirectory(WBSF/src/Basic)
add_subdirectory(WBSF/src/Geomatic)
add_subdirectory(WBSF/src/ModelBased)
add_subdirectory(WBSF/src/WeatherBased)
add_subdirectory(WBSFModels)
add_subdirectory(WBSFTools)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set_target_properties(WBSFBasic PROPERTIES FOLDER "WBSF")
  set_target_properties(WBSFGeomatic PROPERTIES FOLDER "WBSF")
  set_target_properties(WBSFModelBased PROPERTIES FOLDER "WBSF")
  set_target_properties(WBSFWeatherBased PROPERTIES FOLDER "WBSF")  
endif()