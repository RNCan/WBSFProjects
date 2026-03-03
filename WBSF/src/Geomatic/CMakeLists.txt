cmake_minimum_required(VERSION 3.25)
project(WBSFGeomatic LANGUAGES CXX)

find_package(GDAL CONFIG REQUIRED)

file(GLOB_RECURSE WBSF_GEOMATIC_SRC *.cpp *.h *.hpp)
list(FILTER WBSF_GEOMATIC_SRC EXCLUDE REGEX "Projection.cpp$")	#exclude Projection.cpp

add_library(WBSFGeomatic STATIC
    ${WBSF_GEOMATIC_SRC}
)

target_include_directories(WBSFGeomatic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(WBSFGeomatic PUBLIC WBSFBasic GDAL::GDAL)
