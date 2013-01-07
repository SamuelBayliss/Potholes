
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost)


set(GRAVEL_TEST_SOURCE_FILES Test)

include_directories(../include)
include_directories(${BOOST_INCLUDE_DIRS})
include_directories(${ISL_INCLUDE_DIR})


add_executable(Test ${GRAVEL_TEST_SOURCE_FILES})
target_link_libraries(Test gravel)