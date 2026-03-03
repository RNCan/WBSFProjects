cmake_minimum_required(VERSION 3.25)

project(BudBurst)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES BudBurst.h BudBurst.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(BudBurst SHARED  
  ${SOURCE_FILES}  
)

target_link_libraries(BudBurst PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)

######################  BudBurstSaintAmant  ######################

project(BudBurstSaintAmant)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES ./BudBurstSaintAmant.h ./BudBurstSaintAmant.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(BudBurstSaintAmant SHARED  
  ${SOURCE_FILES}  
)

target_link_libraries(BudBurstSaintAmant PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)

######################  BudBurstSBWHost  ######################

project(BudBurstSBWHost)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Use the GLOB command to get all the source files
set(SOURCE_FILES ./BudBurstSBWHost.h ./BudBurstSBWHost.cpp ./BudBurstSBWHostModel.h ./BudBurstSBWHostModel.cpp ./PHENO_eqs.h ./PHENO_eqs.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(BudBurstSBWHost SHARED  
  ${SOURCE_FILES}  
)

target_link_libraries(BudBurstSBWHost PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)

######################  BudBurstUlmus  ######################

project(BudBurstUlmus)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Use the GLOB command to get all the source files
set(SOURCE_FILES ./BudBurstUlmus.h ./BudBurstUlmus.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(BudBurstUlmus SHARED  
  ${SOURCE_FILES}  
)

target_link_libraries(BudBurstUlmus PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)

######################  ShootDevelopmentIndex  ######################

project(ShootDevelopmentIndex)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Use the GLOB command to get all the source files
set(SOURCE_FILES ./ShootDevelopmentIndex.h ./ShootDevelopmentIndex.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(ShootDevelopmentIndex SHARED  
  ${SOURCE_FILES}  
)

target_link_libraries(ShootDevelopmentIndex PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)