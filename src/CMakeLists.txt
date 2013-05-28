
find_package(ISL REQUIRED)

find_package(PET REQUIRED)

find_package(Boost 1.52 COMPONENTS program_options system filesystem REQUIRED)


if (APPLE) 
    set (CLANG_PREFIX ${BUILT_TOOLS_ARCH})
    set (GMP_PREFIX ${MACPORTS_PREFIX})
elseif(APPLE) 
    set (CLANG_PREFIX /cas/apps/llvm-3.0)
    set (GMP_PREFIX ${BUILT_TOOLS_ARCH}) 
endif(APPLE)

add_subdirectory(ASTProcessing)
add_subdirectory(gravel)

set(ANALYSIS_SOURCE_FILES  main )

set(POTHOLES_SOURCE_FILES map analysis frontend context scop prune)
set(BOOST_USE_STATIC_LIBS OFF)
set(BOOST_USE_MULTITHREADED ON)
set(BOOST_USE_STATIC_RUNTIME OFF)
set(BOOST_ALL_DYN_LINK ON)

include_directories(include)

include_directories(${ISL_INCLUDE_DIR})

include_directories(${PET_INCLUDE_DIR})
include_directories(${CLANG_PREFIX}/include)

add_executable(Analysis ${ANALYSIS_SOURCE_FILES})

if (Boost_FOUND)
include_directories(${BOOST_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
target_link_libraries(Analysis ${Boost_LIBRARIES})
endif()



add_library(Potholes ${POTHOLES_SOURCE_FILES})

target_link_libraries(Analysis Potholes)

target_link_libraries(Analysis ${ISL_LIBRARY})
target_link_libraries(Analysis ${PET_LIBRARY})