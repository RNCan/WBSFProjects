cmake_minimum_required(VERSION 3.25)

project(ClimaticLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES WetnessDuration.h WetnessDuration.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as static library
add_library(ClimaticLib STATIC
  ${SOURCE_FILES}
)

target_link_libraries(ClimaticLib PUBLIC
	WBSFBasic
)

######################  AllenWave  ######################

project(AllenWave)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES AllenWaveModel.h AllenWaveModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(AllenWave SHARED
  ${SOURCE_FILES}
)

target_link_libraries(AllenWave PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  ASCE-ETc  ######################

project(ASCE-ETc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES ETcModel.h ETcModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(ASCE-ETc SHARED
  ${SOURCE_FILES}
)

target_link_libraries(ASCE-ETc PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  ASCE-ETsz  ######################

project(ASCE-ETsz)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES ASCE-ETsz.h ASCE-ETsz.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(ASCE-ETsz SHARED
  ${SOURCE_FILES}
)

target_link_libraries(ASCE-ETsz PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  BlueStainVariables  ######################

project(BlueStainVariables)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES BlueStainVariables.h BlueStainVariables.cpp BlueStainVariablesModel.h BlueStainVariablesModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)
# Build as dynamic library
add_library(BlueStainVariables SHARED
  ${SOURCE_FILES}
)

target_link_libraries(BlueStainVariables PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  CaroleCoursolle  ######################

project(CaroleCoursolle)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES CaroleCoursolle.h CaroleCoursolle.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)
# Build as dynamic library
add_library(CaroleCoursolle SHARED
  ${SOURCE_FILES}
)

target_link_libraries(CaroleCoursolle PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  CCBio  ######################

project(CCBio)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES CCBioModel.h CCBioModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)
# Build as dynamic library
add_library(CCBio SHARED
  ${SOURCE_FILES}
)

target_link_libraries(CCBio PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  ChillingDegreeDay  ######################

project(ChillingDegreeDay)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES ChillingDegreeDayModel.h ChillingDegreeDayModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)
# Build as dynamic library
add_library(ChillingDegreeDay SHARED
  ${SOURCE_FILES}
)

target_link_libraries(ChillingDegreeDay PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  ClimateMoistureIndex  ######################

project(ClimateMoistureIndex)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES CMIModel.h CMIModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)
# Build as dynamic library
add_library(ClimateMoistureIndex SHARED
  ${SOURCE_FILES}
)

target_link_libraries(ClimateMoistureIndex PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  Climatic  ######################

project(Climatic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES Climatic.h Climatic.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(Climatic SHARED
  ${SOURCE_FILES}
)

target_link_libraries(Climatic PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  ClimateMoistureDeficit  ######################

project(ClimaticMoistureDeficit)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES ClimaticMoistureDeficit.h ClimaticMoistureDeficit.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)
# Build as dynamic library
add_library(ClimaticMoistureDeficit SHARED
  ${SOURCE_FILES}
)

target_link_libraries(ClimaticMoistureDeficit PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  ClimaticQc  ######################

project(ClimaticQc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES ClimaticQc.h ClimaticQc.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(ClimaticQc SHARED
  ${SOURCE_FILES}
)

target_link_libraries(ClimaticQc PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  ClimaticWind  ######################

project(ClimaticWind)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES ClimaticWind.h ClimaticWind.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(ClimaticWind SHARED
  ${SOURCE_FILES}
)

target_link_libraries(ClimaticWind PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  Climdex  ######################

project(Climdex)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES ClimdexModel.h ClimdexModel.cpp ClimdexVariables.h ClimdexVariables.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(Climdex SHARED
  ${SOURCE_FILES}
)

target_link_libraries(Climdex PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  CornHeatUnits  ######################

project(CornHeatUnits)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES CornHeatUnitsModel.h CornHeatUnitsModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(CornHeatUnits SHARED
  ${SOURCE_FILES}
)

target_link_libraries(CornHeatUnits PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  DailyVsHourly  ######################

project(DailyVsHourly)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES "Daily vs Hourly.h" "Daily vs Hourly.cpp")

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(DailyVsHourly SHARED
  ${SOURCE_FILES}
)

target_link_libraries(DailyVsHourly PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  DailyVsNormals  ######################

project(DailyVsNormals)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES "Daily vs Normals.h" "Daily vs Normals.cpp")

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)
# Build as dynamic library
add_library(DailyVsNormals SHARED
  ${SOURCE_FILES}
)

target_link_libraries(DailyVsNormals PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  DegreeDay  ######################

project(DegreeDay)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES DegreeDaysModel.h DegreeDaysModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(DegreeDay SHARED
  ${SOURCE_FILES}
)

target_link_libraries(DegreeDay PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  DegreeHour  ######################

project(DegreeHour)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES DegreeHoursModel.h DegreeHoursModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(DegreeHour SHARED
  ${SOURCE_FILES}
)

target_link_libraries(DegreeHour PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  DewPeriod  ######################

project(DewPeriod)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES DewPeriod.h DewPeriod.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(DewPeriod SHARED
  ${SOURCE_FILES}
)

target_link_libraries(DewPeriod PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  HourlyGenerator  ######################

project(HourlyGenerator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES HourlyGeneratorModel.h HourlyGeneratorModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(HourlyGenerator SHARED
  ${SOURCE_FILES}
)

target_link_libraries(HourlyGenerator PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  SpringFrost  ######################

project(SpringFrost)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES SpringFrostModel.h SpringFrostModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(SpringFrost SHARED
  ${SOURCE_FILES}
)

target_link_libraries(SpringFrost PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  GrowingSeason  ######################

project(GrowingSeason)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES GrowingSeasonModel.h GrowingSeasonModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(GrowingSeason SHARED
  ${SOURCE_FILES}
)

target_link_libraries(GrowingSeason PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  PotentialEvapotranspiration  ######################

project(PotentialEvapotranspiration)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES EvapotranspirationModel.h EvapotranspirationModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(PotentialEvapotranspiration SHARED
  ${SOURCE_FILES}
)

target_link_libraries(PotentialEvapotranspiration PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  ReverseDegreeDay  ######################

project(ReverseDegreeDay)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES ReverseDegreeDay.h ReverseDegreeDay.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(ReverseDegreeDay SHARED
  ${SOURCE_FILES}
)

target_link_libraries(ReverseDegreeDay PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  SnowMelt  ######################

project(SnowMelt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES SnowMelt.h SnowMelt.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(SnowMelt SHARED
  ${SOURCE_FILES}
)

target_link_libraries(SnowMelt PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  SoilTemperature  ######################

project(SoilTemperature)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES SoilTemperature.h SoilTemperature.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(SoilTemperature SHARED
  ${SOURCE_FILES}
)

target_link_libraries(SoilTemperature PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  TminTairTmax  ######################

project(TminTairTmax)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES TminTairTmax.h TminTairTmax.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(TminTairTmax SHARED
  ${SOURCE_FILES}
)

target_link_libraries(TminTairTmax PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  UnderbarkTemperature  ######################

project(UnderbarkTemperature)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES UnderbarkTemperature.h UnderbarkTemperature.cpp UnderbarkTemperatureModel.h UnderbarkTemperatureModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(UnderbarkTemperature SHARED
  ${SOURCE_FILES}
)

target_link_libraries(UnderbarkTemperature PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  VaporPressureDeficit  ######################

project(VaporPressureDeficit)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES VaporPressureDeficit.h VaporPressureDeficit.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(VaporPressureDeficit SHARED
  ${SOURCE_FILES}
)

target_link_libraries(VaporPressureDeficit PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  VHR_to_Landsat  ######################

project(VHR_to_Landsat)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES VHR_to_Landsat.h VHR_to_Landsat.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(VHR_to_Landsat SHARED
  ${SOURCE_FILES}
)

target_link_libraries(VHR_to_Landsat PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  WetnessDuration  ######################

project(WetnessDuration)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES WetnessDuration.h WetnessDuration.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)
# Build as dynamic library
add_library(WetnessDuration SHARED
  ${SOURCE_FILES}
)

target_link_libraries(WetnessDuration PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  WinterThaw  ######################

project(WinterThaw)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES WinterThawModel.h WinterThawModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(WinterThaw SHARED
  ${SOURCE_FILES}
)

target_link_libraries(WinterThaw PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  WorldClimVars  ######################

project(WorldClimVars)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES WinterThawModel.h WinterThawModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(WorldClimVars SHARED
  ${SOURCE_FILES}
)

target_link_libraries(WorldClimVars PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  BlueStainIndex  ######################

project(BlueStainIndex)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES BlueStainVariables.h BlueStainVariables.cpp BlueStainIndexModel.h BlueStainIndexModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)
# Build as dynamic library
add_library(BlueStainIndex SHARED
  ${SOURCE_FILES}
)

target_link_libraries(BlueStainIndex PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)