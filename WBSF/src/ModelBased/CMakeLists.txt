cmake_minimum_required(VERSION 3.25)
project(WBSFModelBased LANGUAGES CXX)

find_package(Boost CONFIG REQUIRED)
find_package(GDAL CONFIG REQUIRED)

file(GLOB_RECURSE WBSF_MODELBASED_SRC *.cpp *.h *.hpp)

add_library(WBSFModelBased STATIC
    ${WBSF_MODELBASED_SRC}
)

target_link_libraries(WBSFModelBased PUBLIC Boost::boost GDAL::GDAL)
target_include_directories(WBSFModelBased PUBLIC ${PROJECT_SOURCE_DIR}/..)
