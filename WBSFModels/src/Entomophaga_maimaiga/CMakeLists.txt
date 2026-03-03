project(EntomophagaMaimaiga)

find_package(GSL REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES DDEVF.h DDEVF.cpp eco-climate.h eco-climate.cpp EntomophagaMaimaigaModel.h EntomophagaMaimaigaModel.cpp Inputs.h Inputs.cpp ODE_Solver.h ODE_Solver.cpp random_setup.h)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(EntomophagaMaimaiga SHARED  
  ${SOURCE_FILES}  
)

target_link_libraries(EntomophagaMaimaiga PUBLIC
	GSL::gsl 
	GSL::gslcblas
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)


