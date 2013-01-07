add_subdirectory(test)

include_directories(include)

set (GRAVEL_SOURCE ReferenceCountedObject)

add_library(gravel ${GRAVEL_SOURCE})