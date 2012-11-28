include(ExternalProject)

ExternalProject_Add(
pet
#DOWNLOAD_COMMAND ""
URL ${CMAKE_CURRENT_SOURCE_DIR}/pet.tar.gz
PREFIX ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SYSTEM_NAME}
INSTALL_DIR ${CMAKE_SOURCE_DIR}/external/install/${CMAKE_SYSTEM_NAME}
CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --with-clang-prefix=/cas/apps/llvm-3.0 --with-isl=bundled --with-gmp=system --with-gmp-prefix=${BUILT_TOOLS_ARCH} --with-libyaml-prefix=${BUILT_TOOLS_ARCH}
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
