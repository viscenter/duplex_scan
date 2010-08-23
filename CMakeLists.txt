cmake_minimum_required(VERSION 2.8)

PROJECT(Viz)

#platform is set corresponding to the directories 
IF(UNIX)
  IF(APPLE)
    SET(PLATFORM "osx")
  ELSE(APPLE)
    SET(PLATFORM "nix")
  ENDIF(APPLE)
ELSE(UNIX)
  IF(WIN32)
    SET(PLATFORM "win")
  ELSE(WIN32)
    SET(PLATFORM "Unknown")
  ENDIF(WIN32)
ENDIF(UNIX)


IF(DEBUG)
	MESSAGE("______Building DEBUG\(${PLATFORM}\) version______")
	SET(CMAKE_CXX_FLAGS_DEBUG "-O3 -Wall -DDEBUG -g")
	SET(CMAKE_BUILD_TYPE "Debug")
ELSE(DEBUG)
	MESSAGE("______Building RELEASE\(${PLATFORM}\) version______")
	SET(CMAKE_BUILD_TYPE "Release")
	SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")
ENDIF(DEBUG)
include(UseDoxygen OPTIONAL)

SET(CMAKE_INCLUDE_CURRENT_DIR OFF)
SET(CMAKE_VERBOSE_MAKEFILE OFF)
include_directories(${Viz_SOURCE_DIR})

add_subdirectory(external/platform/${PLATFORM})
add_subdirectory(platform/${PLATFORM})
add_subdirectory(common/platform/${PLATFORM})
add_subdirectory(common)

