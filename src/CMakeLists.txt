
find_package(ISL REQUIRED)

set(ANALYSIS_SOURCE_FILES analysis main)

include_directories(include)

include_directories(${ISL_INCLUDE_DIR})


add_executable(Analysis ${ANALYSIS_SOURCE_FILES})
target_link_libraries(Analysis ${ISL_LIBRARY})
