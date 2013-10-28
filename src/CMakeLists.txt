# Potholes Library for TCL

find_package(itcl REQUIRED)
find_package(TCL REQUIRED)

find_package(clang REQUIRED)
find_package(PET REQUIRED)
find_package(isl REQUIRED)

#set(POTHOLES_FRONTEND_SOURCE potholes analysis extract options scop project consumer promote transform rewrite )

#set (POTHOLES_AST_SOURCES affine generate parallel ast statement)
set(POTHOLES_FRONTEND_SOURCE potholes)
set(POTHOLES_AST_SOURCES )

include_directories(${ITCL_INCLUDE_DIRS})

include_directories("include")

SET(CMAKE_CXX_FLAGS "-Wall -fno-rtti -O0 -g" )  

add_library(Potholes SHARED ${POTHOLES_FRONTEND_SOURCE} ${POTHOLES_AST_SOURCES})
if (ITCL_FOUND)
    target_link_libraries(Potholes ${ITCL_LIBRARY})
endif (ITCL_FOUND)

if (TCL_FOUND)
    target_link_libraries(Potholes ${TCL_LIBRARY})
endif(TCL_FOUND)

if (CLANG_FOUND)
    add_definitions( -D __STDC_LIMIT_MACROS )
    add_definitions( -D __STDC_CONSTANT_MACROS )
    include_directories(${CLANG_INCLUDE_DIR})
message(${CLANG_LIBRARIES})
    target_link_libraries(Potholes ${CLANG_LIBRARIES})
    
endif (CLANG_FOUND)

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


INSTALL(TARGETS Potholes
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)
