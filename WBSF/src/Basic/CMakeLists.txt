cmake_minimum_required(VERSION 3.25)
project(WBSFBasic LANGUAGES CXX)

find_package(Boost CONFIG REQUIRED COMPONENTS process serialization locale dynamic_bitset exception throw_exception)

file(GLOB_RECURSE WBSF_BASIC_SRC *.cpp *.h *.hpp)

add_library(WBSFBasic STATIC
    ${WBSF_BASIC_SRC}
)

target_include_directories(WBSFBasic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(WBSFBasic PUBLIC Boost::process Boost::serialization Boost::locale Boost::dynamic_bitset Boost::exception Boost::throw_exception)
