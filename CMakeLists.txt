cmake_minimum_required (VERSION 2.6)
project (meterreader)

set(SYSROOTPATH "~/sysroot" CACHE STRING "give pat h to sysroot directory")
#option(FCGIDBG "set to wait for debugger connection" FALSE)

# The version number.
set (meterreader_VERSION_MAJOR 1)
set (meterreader_VERSION_MINOR 0)

set (PROJECT_INCLUDE_DIR "Code/inc")
set (PROJECT_SOURCE_DIR "Code/src")

#if (${FCGIDBG} EQUAL TRUE)
#    add_definitions( -DFCGIDEBUG=TRUE )
#endif(${FCGIDBG} EQUAL TRUE)

LINK_DIRECTORIES(${SYSROOTPATH}/usr/lib)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_INCLUDE_DIR}/meterreader_version.h.in"
  "${PROJECT_INCLUDE_DIR}/meterreader_version.h"
  )

include_directories("${SYSROOTPATH}/usr/include")
include_directories("${CMAKE_SOURCE_DIR}/Code/inc")

FILE(GLOB_RECURSE TARGET_SRC  "${PROJECT_SOURCE_DIR}/*.c")
FILE(GLOB_RECURSE TARGET_INC  "${PROJECT_INCLUDE_DIR}/*.h")

add_executable(meterreader ${TARGET_SRC} ${TARGET_INC})

TARGET_LINK_LIBRARIES(meterreader)
