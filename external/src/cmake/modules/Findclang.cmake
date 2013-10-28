# Locate libclangBasic
#
# This module defines
#  CLANG_FOUND, if false, do not try to link to clang
#  CLANG_LIBRARY, where to find clang
#  CLANG_INCLUDE_DIR, where to find yaml.h
#
# By default, the dynamic libraries of clang will be found. To find the static ones instead,
# you must set the CLANG_STATIC_LIBRARY variable to TRUE before calling find_package(YamlCpp ...).
#
# If clang is not installed in a standard path, you can use the CLANG_DIR CMake variable
# to tell CMake where clang is.

# attempt to find static library first if this is set

# find the clang include directory
find_path(CLANG_INCLUDE_DIR clang/Basic/LLVM.h
          PATH_SUFFIXES include
          PATHS
          /usr/local/include/
	  )

# find the clang library
find_library(CLANG_BASIC_LIBRARY
             NAMES libclangBasic.a clangBasic 
             PATH_SUFFIXES lib64 lib
             PATHS   /usr/local
                   )


# find the clang tooling
find_library(CLANG_TOOLING_LIBRARY
             NAMES libclangTooling.a clangTooling 
             PATH_SUFFIXES lib64 lib
             PATHS   /usr/local
                   )

find_library(CLANG_REWRITE_LIBRARY
             NAMES libclangRewriteCore.a libclangRewriteCore
             PATH_SUFFIXES lib64 lib
             PATHS   /usr/local
                   )

find_library(LLVM_LIBRARY
             NAMES LLVM-3.3
             PATH_SUFFIXES 
             PATHS   /usr/local/lib
)

# handle the QUIETLY and REQUIRED arguments and set CLANG_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CLANG DEFAULT_MSG CLANG_BASIC_LIBRARY CLANG_INCLUDE_DIR)
mark_as_advanced(CLANG_INCLUDE_DIR CLANG_LIBRARY)

set (CLANG_LIBRARIES ${CLANG_BASIC_LIBRARY} ${CLANG_TOOLING_LIBRARY} ${CLANG_REWRITE_LIBRARY} )