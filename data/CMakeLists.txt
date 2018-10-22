# $Id: CMakeLists.txt 86065 2014-11-07 08:51:15Z gcosmo $

set(useROOT true)        # use true or false
#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(NC2)
set(useROOT true)        # use true or false

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#------------------------
# Find and include ROOT
#

find_package(ROOT 5.34 REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})
message("root libraries find under ${ROOT_LIBRARIES}")
message("root include dir found under ${ROOT_INCLUDE_DIR}")

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#if(useROOT)
#	EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
#endif(useROOT) 


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#-----------------------------
# ROOT stuff
#set(CMAKE_EXE_LINKER_FLAGS "-L/home/frederik/root/lib -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic")

#SET(CMAKE_MODULE_PATH /home/frederik/geant4/geant4.10.01/cmake/Modules/)
#find_package(ROOT)
#include_directories(/home/frederik/root/include/)

#option(ANALYSIS_HH_ "Build with ROOT" 1)
#add_definitions(-DG4UI_USE -DG4VIS_USE -DANALYSIS_HH_)

#-------------------------------------
# C++ flags
#

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#

add_executable(NC2 NC2.cc ${sources} ${headers})

#if(useROOT)
#	EXECUTE_PROCESS(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
#	set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LD_FLAGS})
#endif(useROOT)

target_link_libraries(NC2 ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ) 


#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#

add_custom_target(NC_2 DEPENDS NC2)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#

install(TARGETS NC2 DESTINATION bin)


