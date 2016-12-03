cmake_minimum_required(VERSION 3.0)

project(coconut)

include("cmake.cfg")

set(CMAKE_MODULE_PATH "${EXTENSIONS_DIR}/cmake" ${CMAKE_MODULE_PATH})
include("executable-module")
include("library-module")

set(coconut_VERSION_MAJOR 1)
set(coconut_VERSION_MINOR 0)
set(coconut_VERSION_BUGFIX 0)
set(coconut_VERSION_EXTRA "-SNAPSHOT")
set(coconut_VERSION
    "${coconut_VERSION_MAJOR}.${coconut_VERSION_MINOR}.${coconut_VERSION_BUGFIX}${coconut_VERSION_EXTRA}"
    )

option(BUILD_STATIC_LIBS "Whether to build static libs" ON)
option(COMPILER_IS_DEBUG_BUILD "Is debug build" ON)
option(COMPILER_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
option(COMPILER_BOOST_USE_STATIC_LIBS "Use static boost libs" OFF)

if(${BUILD_STATIC_LIBS})
  set(LIBRARY_TYPE LINK_STATIC)
else()
  set(LIBRARY_TYPE LINK_DYNAMIC)
endif()

if(${COMPILER_IS_DEBUG_BUILD})
    set(CMAKE_BUILD_TYPE "Debug")
else()
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(${CMAKE_COMPILER_IS_GNUCXX})
    if(${COMPILER_WARNINGS_AS_ERRORS})
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -O0")
elseif(${MSVC})
	# set(CMAKE_CXX_FLAGS "/std:c++latest")
    if(${COMPILER_WARNINGS_AS_ERRORS})
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_SCL_SECURE_NO_WARNINGS /DUNICODE")
else()
    message(SEND_ERROR "Unrecognised compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

enable_testing()

# find boost
set(Boost_USE_STATIC_LIBS ${COMPILER_BOOST_USE_STATIC_LIBS})
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS system thread unit_test_framework filesystem regex REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# find gmock
find_package(GMock REQUIRED)
include_directories(${GMOCK_INCLUDE_DIR})

# find coconut tools
set(CoconutTools_INCLUDE_HINT ${EXTENSIONS_DIR})
set(CoconutTools_LIBRARY_HINT ${EXTENSIONS_DIR})
find_package(CoconutTools REQUIRED)
include_directories(${CoconutTools_INCLUDE_DIR})
link_directories(${CoconutTools_LIBRARY_DIR})

find_package(Threads REQUIRED)

set(MODULES
    "coconut-milk-graphics-${MODULE_RENDERER}"
    "coconut-milk-math-${MODULE_MATHS}"
    "coconut-milk-system-${MODULE_SYSTEM}"
    "coconut-milk-utils"
    "coconut-pulp-model"
    "coconut-pulp-renderer"
    "coconut-shell"
    )

add_modules("${MODULES}")
