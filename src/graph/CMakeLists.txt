cmake_minimum_required(VERSION 2.8)

#find_package(CLOOG)
#find_package(ISL)
#find_package(TCL)
find_package(GMP)
find_package(GRAPHVIZ)
find_package(PIP)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_DEBUG 1)

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

if (!Boost_FOUND) 
message("Found Boost\n")
endif()

#link_directories(${CLOOG_LINK_DIRECTORIES})
#link_directories(${ISL_LINK_DIRECTORIES})

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

SET (graphlib_SRCS ReferenceCountedObject VerilogSymbol VerilogExpression VerilogModule VerilogContext VerilogValue)
    
#SET (graphtest_SRCS Test)
SET (graphtest2_SRCS Test2)
SET (boosttest_SRCS test/test)

#add_definitions(-DLINEAR_VALUE_IS_LONGLONG -DSCOPLIB_INT_T_IS_LONGLONG)
include_directories(include)
include_directories(${GRAPHVIZ_INCLUDE_DIRS})
include_directories(${PIP_INCLUDE_DIRS})

add_library(graphform ${graphlib_SRCS})
link_directories(${GRAPHVIZ_LINK_DIRECTORIES})
link_directories(${PIP_LINK_DIRECTORIES})
link_directories(${GMP_LINK_DIRECTORIES})
link_directories(${BOOST_LINK_DIRECTORIES}) 

#add_executable(test ${graphtest_SRCS})

add_executable(test2 ${graphtest2_SRCS})

#add_executable(boost_test ${boosttest_SRCS})
#target_link_libraries(boost_test graphform)
#target_link_libraries(boost_test ${GRAPHVIZ_LIBRARIES})
#target_link_libraries(boost_test ${PIP_LIBRARIES})
#target_link_libraries(boost_test ${GMP_LIBRARIES})
#target_link_libraries(boost_test ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

#target_link_libraries(test graphform)
#target_link_libraries(test ${GRAPHVIZ_LIBRARIES})
#target_link_libraries(test ${GMP_LIBRARIES})

target_link_libraries(test2 graphform)
target_link_libraries(test2 ${GRAPHVIZ_LIBRARIES})
target_link_libraries(test2 ${PIP_LIBRARIES})
target_link_libraries(test2 ${GMP_LIBRARIES})
#include_directories(${CLOOG_INCLUDE_DIRS})
#include_directories(${ISL_INCLUDE_DIRS})
#include_directories(${GMP_INCLUDE_DIRS})



#target_link_libraries(simulator ${CLOOG_LIBRARIES})
#target_link_libraries(simulator ${ISL_LIBRARIES})
#target_link_libraries(simulator ${GMP_LIBRARIES})

#INSTALL(TARGETS test DESTINATION bin)

