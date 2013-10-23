# Locate libclangBasic
#
# This module defines
#  ISL_FOUND, if false, do not try to link to clang
#  ISL_LIBRARY, where to find clang
#  ISL_INCLUDE_DIR, where to find yaml.h
#
# By default, the dynamic libraries of clang will be found. To find the static ones instead,
# you must set the ISL_STATIC_LIBRARY variable to TRUE before calling find_package(YamlCpp ...).
#
# If clang is not installed in a standard path, you can use the ISL_DIR CMake variable
# to tell CMake where clang is.

# attempt to find static library first if this is set

# find the clang include directory
find_path(ISL_INCLUDE_DIR isl/ctx.h
          PATH_SUFFIXES include
          PATHS
          /usr/local/include/
	  )

# find the clang library
find_library(ISL_LIBRARY
             NAMES isl
             PATH_SUFFIXES lib64 lib
             PATHS   /usr/local
                   )

set (ISL_LIBRARIES ${ISL_LIBRARY})

# handle the QUIETLY and REQUIRED arguments and set ISL_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ISL DEFAULT_MSG ISL_LIBRARY ISL_INCLUDE_DIR)


#mark_as_advanced(ISL_INCLUDE_DIR ISL_LIBRARY)

