
find_package(ISL REQUIRED)

add_subdirectory(gravel)

set(ANALYSIS_SOURCE_FILES analysis main)

set(POTHOLES_SOURCE_FILES map)

include_directories(include)

include_directories(${ISL_INCLUDE_DIR})


add_executable(Analysis ${ANALYSIS_SOURCE_FILES})

add_library(Potholes ${POTHOLES_SOURCE_FILES})

target_link_libraries(Analysis ${ISL_LIBRARY})