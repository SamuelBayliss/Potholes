include(ExternalProject)

if (APPLE) 
    set (CLANG_PREFIX ${BUILT_TOOLS_ARCH})
    set (GMP_PREFIX ${MACPORTS_PREFIX})
elseif(APPLE) 
    set (CLANG_PREFIX /cas/apps/llvm-3.0)
    set (GMP_PREFIX ${BUILT_TOOLS_ARCH}) 
endif(APPLE)




ExternalProject_Add(
pet
#DOWNLOAD_COMMAND ""
URL ${CMAKE_CURRENT_SOURCE_DIR}/pet.tar.gz
PREFIX ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SYSTEM_NAME}
INSTALL_DIR ${CMAKE_SOURCE_DIR}/external/install/${CMAKE_SYSTEM_NAME}
CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --with-clang-prefix=${CLANG_PREFIX} --with-isl=bundled --with-gmp=system --with-gmp-prefix=${GMP_PREFIX} --with-libyaml-prefix=${BUILT_TOOLS_ARCH}
)

ExternalProject_Add_Step(pet autogen
COMMAND cd <SOURCE_DIR> && <SOURCE_DIR>/autogen.sh
DEPENDEES configure_isl
DEPENDERS configure
)

ExternalProject_Add_Step(pet configure_isl
COMMAND cd <SOURCE_DIR> && git submodule init && git submodule update
DEPENDEES download
DEPENDERS configure
)

ExternalProject_Add(
barvinok
#DOWNLOAD_COMMAND ""
URL ${CMAKE_CURRENT_SOURCE_DIR}/barvinok.tar.gz
PREFIX ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SYSTEM_NAME}
INSTALL_DIR ${CMAKE_SOURCE_DIR}/external/install/${CMAKE_SYSTEM_NAME}
CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --with-isl=system --with-isl-prefix=${CMAKE_SOURCE_DIR}/external/install/${CMAKE_SYSTEM_NAME} --with-libgmp=${GMP_PREFIX} --with-ntl=${BUILT_TOOLS_ARCH}
)

 