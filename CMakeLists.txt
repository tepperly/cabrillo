cmake_minimum_required(VERSION 3.8)
project(cabrillo)

if (DEFINED BLT_SOURCE_DIR)
    # Support having a shared BLT outside of the repository if given a BLT_SOURCE_DIR
    if (NOT EXISTS ${BLT_SOURCE_DIR}/SetupBLT.cmake)
        message(FATAL_ERROR "Given BLT_SOURCE_DIR does not contain SetupBLT.cmake")
    endif()
else()
    # Use internal BLT if no BLT_SOURCE_DIR is given
    set(BLT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/blt" CACHE PATH "")
    if (NOT EXISTS ${BLT_SOURCE_DIR}/SetupBLT.cmake)
        message(FATAL_ERROR
            "The BLT git submodule is not present. "
            "Either run the following two commands in your git repository: \n"
            "    git submodule init\n"
            "    git submodule update\n"
            "Or add -DBLT_SOURCE_DIR=/path/to/blt to your CMake command." )
    endif()
endif()

# Default to C++11 if not set so GTest/GMock can build
if (NOT BLT_CXX_STD)
    set(BLT_CXX_STD "c++14" CACHE STRING "")
endif()

include(${BLT_SOURCE_DIR}/SetupBLT.cmake)
set(CAB_LIBRARY_SRCS stringreg.cpp tabletext.cpp)
set(CAB_LIBRARY_HDRS stringreg.h tabletext.h)

blt_add_library(NAME cabrillo
		HEADERS ${CAB_LIBRARY_HDRS}
		SOURCES ${CAB_LIBRARY_SRCS})

set(CAB_TEST_SRCS cabtests.cpp)

blt_add_executable(NAME cabtests
                   SOURCES ${CAB_TEST_SRCS}
		   DEPENDS_ON cabrillo gtest)

blt_add_test(NAME cabtests
             COMMAND cabtests)

blt_add_code_checks(PREFIX cabrillo
  SOURCES ${CAB_LIBRARY_HDRS} ${CAB_LIBRARY_SRCS} ${CAB_TEST_SRCS}
  ASTYLE_CFG_FILE ${CMAKE_SOURCE_DIR}/.astyle)
