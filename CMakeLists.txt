cmake_minimum_required(VERSION 2.6)
project(POTHOLES)

#if(DEFINED ENV{BUILT_TOOLS})	  
#    set (BUILT_TOOLS $ENV{BUILT_TOOLS} )
#else()
#        message(FATAL_ERROR "BUILT_TOOLS VARIABLE NOT SET")
#endif()

#if(DEFINED ENV{BUILT_TOOLS_ARCH}) 
	  
#set (BUILT_TOOLS_ARCH $ENV{BUILT_TOOLS_ARCH})
#else()
#    message(FATAL_ERROR "BUILT_TOOLS_ARCH VARIABLE NOT SET")
#endif()

#if (APPLE)
#    if (DEFINED ENV{MACPORTS_PREFIX}) 
#        set (MACPORTS_PREFIX $ENV{MACPORTS_PREFIX} )
#    else()
#        message(FATAL_ERROR "MACPORTS_PREFIX must be set for apple platform")
#    endif()
#endif(APPLE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/external/src/cmake/modules")
set (CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/external/platforms/${CMAKE_SYSTEM_NAME})

set(CMAKE_CXX_COMPILER c++)
set(CMAKE_C_COMPILER cc)


add_subdirectory(src)
add_subdirectory(scripts)