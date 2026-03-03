cmake_minimum_required(VERSION 3.25)
project(WBSFWeatherBased LANGUAGES CXX)

find_package(Boost CONFIG REQUIRED COMPONENTS iostreams)
find_package(GDAL CONFIG REQUIRED)

file(GLOB_RECURSE WBSF_WEATHERBASED_SRC *.cpp *.h *.hpp)

add_library(WBSFWeatherBased STATIC
    ${WBSF_WEATHERBASED_SRC}
)

target_link_libraries(WBSFWeatherBased PUBLIC WBSFBasic Boost::iostreams GDAL::GDAL)
target_include_directories(WBSFWeatherBased PUBLIC ${PROJECT_SOURCE_DIR}/..)
