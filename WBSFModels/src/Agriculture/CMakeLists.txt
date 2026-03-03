cmake_minimum_required(VERSION 3.25)

project(InsectDevelopmentDatabaseII)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES "Insect Development Database II.h" "Insect Development Database II.cpp")

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(InsectDevelopmentDatabaseII SHARED
  ${SOURCE_FILES}
)

target_link_libraries(InsectDevelopmentDatabaseII PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  InsectDevelopmentDatabaseIII  ######################

project(InsectDevelopmentDatabaseIII)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES "Insect Development Database III.h" "Insect Development Database III.cpp")

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(InsectDevelopmentDatabaseIII SHARED
  ${SOURCE_FILES}
)

target_link_libraries(InsectDevelopmentDatabaseIII PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  MagareyInfection  ######################

project(MagareyInfection)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES MagareyInfection.h MagareyInfection.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(MagareyInfection SHARED
  ${SOURCE_FILES}
)

target_link_libraries(MagareyInfection PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
	ClimaticLib
)