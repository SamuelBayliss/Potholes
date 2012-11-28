cmake_minimum_required(VERSION 2.6)
project(POTHOLES)

if(DEFINED ENV{BUILT_TOOLS}) 
	  
set (BUILT_TOOLS $ENV{BUILT_TOOLS})

else()
  error("BUILT_TOOLS VARIABLE NOT SET")
  return()
endif()

if(DEFINED ENV{BUILT_TOOLS_ARCH}) 
	  
set (BUILT_TOOLS_ARCH $ENV{BUILT_TOOLS_ARCH})
else()
  error("BUILT_TOOLS_ARCH VARIABLE NOT SET")
  return()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/external/src/cmake/modules")
set (CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/external/install/${CMAKE_SYSTEM_NAME})


add_subdirectory(external)
add_subdirectory(src)
