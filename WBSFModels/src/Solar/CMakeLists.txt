cmake_minimum_required(VERSION 3.25)

project(Solar)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
file(GLOB SOURCE_FILES ./*.h ./*.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(Solar SHARED
  ${SOURCE_FILES}
)

target_link_libraries(Solar PUBLIC
  WBSFBasic
  WBSFModelBased
  WBSFWeatherBased
)


