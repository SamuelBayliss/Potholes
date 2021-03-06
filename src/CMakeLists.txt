# Potholes Library for TCL



find_package(Itcl REQUIRED)
find_package(TCL REQUIRED)

find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)

find_package(PET REQUIRED)
find_package(ISL REQUIRED)


set (POTHOLES_AST_SOURCES affine generate parallel ast statement)

set(POTHOLES_FRONTEND_SOURCE potholes analysis extract options scop project consumer promote transform rewrite )



include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

SET(CMAKE_CXX_FLAGS "-Wall -fno-rtti -O0 -g " )  

if (CMAKE_HOST_APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS " -fvisibility=hidden") 
else()
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,--exclude-libs=ALL -Wl,-z,defs -fvisibility=hidden")
endif()


add_library(Potholes SHARED ${POTHOLES_FRONTEND_SOURCE} ${POTHOLES_AST_SOURCES})
if (ITCL_FOUND)
    include_directories(${ITCL_INCLUDE_DIR})
    target_link_libraries(Potholes ${ITCL_LIBRARY})
endif (ITCL_FOUND)

if (TCL_FOUND)
    target_link_libraries(Potholes ${TCL_LIBRARY})
endif(TCL_FOUND)

if (CLANG_FOUND)
    add_definitions( -D __STDC_LIMIT_MACROS )
    add_definitions( -D __STDC_CONSTANT_MACROS )
    include_directories(${CLANG_INCLUDE_DIR})
    target_link_libraries(Potholes ${CLANG_LIBRARIES})   
endif (CLANG_FOUND)

if (LLVM_FOUND) 
   target_link_libraries(Potholes ${REQ_LLVM_LIBRARIES})
endif (LLVM_FOUND)

if(Boost_FOUND) 
    include_directories(${BOOST_INCLUDE_DIR})
    link_directories(${Boost_LIBRARY_DIR})
     target_link_libraries(Potholes ${Boost_LIBRARIES})
endif (Boost_FOUND)

if (ISL_FOUND)
    include_directories(${ISL_INCLUDE_DIR})
    link_directories(${ISL_LIBRARY_DIR})
    target_link_libraries(Potholes ${ISL_LIBRARIES})
    message(${ISL_LIBRARIES})
endif (ISL_FOUND)

if (PET_FOUND)
    include_directories(${PET_INCLUDE_DIR})
    link_directories(${PET_LIBRARY_DIR})
    target_link_libraries(Potholes ${PET_LIBRARIES})
    message(${PET_LIBRARIES})
endif (PET_FOUND)

GET_TARGET_PROPERTY(POTHOLES_BIN_NAME Potholes LOCATION)
add_custom_command(TARGET Potholes POST_BUILD COMMAND strip -x ${POTHOLES_BIN_NAME})

INSTALL(TARGETS Potholes
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)
