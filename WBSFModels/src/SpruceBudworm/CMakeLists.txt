cmake_minimum_required(VERSION 3.25)

project(SpruceBudwormLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES 
  SpruceBudworm.cpp
  SpruceBudworm.h
  SpruceBudwormEquations.cpp
  SpruceBudwormEquations.h
  )

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(SpruceBudwormLib STATIC
  ${SOURCE_FILES}
)

target_link_libraries(SpruceBudwormLib PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  SpruceBudwormBiology  ######################

project(SpruceBudwormBiology)

find_package(Boost CONFIG REQUIRED COMPONENTS thread chrono filesystem serialization process locale iostreams timer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES 
  SpruceBudwormModel.cpp
  SpruceBudwormModel.h
  )

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(SpruceBudwormBiology SHARED    
  ${SOURCE_FILES}
)

target_link_libraries(SpruceBudwormBiology PUBLIC	
	SpruceBudwormLib
)

######################  SpruceBudwormDispersal  ######################

project(SpruceBudwormDispersal)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES 
  SpruceBudwormDispersal.cpp
  SpruceBudwormDispersal.h
  )

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(SpruceBudwormDispersal SHARED
  ${SOURCE_FILES}
)

target_link_libraries(SpruceBudwormDispersal PUBLIC	
	SpruceBudwormLib
)

######################  SpruceBudwormLaboratory  ######################

project(SpruceBudwormLaboratory)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES 
  SpruceBudwormLaboratory.cpp
  SpruceBudwormLaboratory.h
  SpruceBudwormLaboratoryEquations.cpp
  SpruceBudwormLaboratoryEquations.h
  )

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(SpruceBudwormLaboratory SHARED
  ${SOURCE_FILES}
)

target_link_libraries(SpruceBudwormLaboratory PUBLIC
	SpruceBudwormLib
)

######################  SpruceBudwormEldon  ######################

project(SpruceBudwormEldon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES 
  SBW-Eldon-Model.cpp
  SBW-Eldon-Model.h
  )

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(SpruceBudwormEldon SHARED
  ${SOURCE_FILES}
)

target_link_libraries(SpruceBudwormEldon PUBLIC
	SpruceBudwormLib
)

######################  SpruceBudwormLysyk  ######################

project(SpruceBudwormLysyk)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES 
  SBW-Lysyk-Model.cpp
  SBW-Lysyk-Model.h
  )

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(SpruceBudwormLysyk SHARED
  ${SOURCE_FILES}
)

target_link_libraries(SpruceBudwormLysyk PUBLIC
	SpruceBudwormLib
)

######################  SpruceBudwormManitoba  ######################

project(SpruceBudwormManitoba)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES 
  SBWManitobaModel.cpp
  SBWManitobaModel.h
  )

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(SpruceBudwormManitoba SHARED
  ${SOURCE_FILES}
)

target_link_libraries(SpruceBudwormManitoba PUBLIC
	SpruceBudwormLib
)