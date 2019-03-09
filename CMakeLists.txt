cmake_minimum_required(VERSION 3.0)
project(CpuProfiler)
include (CheckIncludeFile)
file(READ "VERSION.txt" VERSION)

string(TIMESTAMP COMPILE_DATETIME "%Y/%m/%d %H:%M:%S")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  add_definitions("-DCIOSDEBUG=1")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
else()
  set(CMAKE_BUILD_TYPE Release)
endif()

# Handling compilers in different platforms

# using Clang
if(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
endif()

# using GCC
if(CMAKE_CXX_COMPILER_ID STREQUAL GNU)
endif()

# using Intel C++
if(CMAKE_CXX_COMPILER_ID STREQUAL Intel)
endif()

# using Visual Studio C++
if(MSVC)
  if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    set(TARGET_ARCH x64)
    add_definitions("-DCIOS_X64=1")
  elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    set(TARGET_ARCH x86)
    add_definitions("-DCIOS_X86=1")
  endif()
endif(MSVC)

find_package(PkgConfig REQUIRED)
pkg_check_modules(UUIDS REQUIRED UUIDs)
pkg_check_modules(STARDATE REQUIRED libstardate)
pkg_check_modules(PARALLEL REQUIRED libparallel)

include_directories(include)
include_directories(srcs)

set(HEADERS include/CpuProfiler.hpp)
include_directories (${UUIDS_INCLUDE_DIRS})
include_directories (${STARDATE_INCLUDE_DIRS})
include_directories (${PARALLEL_INCLUDE_DIRS})
link_directories(${UUIDS_LIBRARY_DIRS})
link_directories(${STARDATE_LIBRARY_DIRS})
link_directories(${PARALLEL_LIBRARY_DIRS})

set(CORES srcs/CpuProfiler.cpp)

if(BUILD_SHARED_LIBS)
  add_executable(CpuProfiler ${CORES} ${HEADERS})
  target_link_libraries(CpuProfiler PUBLIC kernel32)
  target_link_libraries(CpuProfiler PUBLIC Psapi)
  target_link_libraries(CpuProfiler PUBLIC ${UUIDS_LIBRARIES})
  target_link_libraries(CpuProfiler PUBLIC ${STARDATE_LIBRARIES})
  target_link_libraries(CpuProfiler PUBLIC ${PARALLEL_LIBRARIES})
  target_compile_options(CpuProfiler PUBLIC ${UUIDS_CFLAGS_OTHER})
  target_compile_options(CpuProfiler PUBLIC ${STARDATE_CFLAGS_OTHER})
  target_compile_options(CpuProfiler PUBLIC ${PARALLEL_CFLAGS_OTHER})
  add_definitions("-DBUILD_SHARED_LIBS=1")
  add_definitions("-DEXPORT_UUIDS=1")
  install(TARGETS CpuProfiler RUNTIME DESTINATION bin)
else(BUILD_SHARED_LIBS)
  add_executable(CpuProfiler ${CORES} ${HEADERS})
  target_link_libraries(CpuProfiler PUBLIC kernel32)
  target_link_libraries(CpuProfiler PUBLIC Psapi)
  target_link_libraries(CpuProfiler PUBLIC ${UUIDS_LIBRARIES})
  target_link_libraries(CpuProfiler PUBLIC ${STARDATE_LIBRARIES})
  target_link_libraries(CpuProfiler PUBLIC ${PARALLEL_LIBRARIES})
  target_compile_options(CpuProfiler PUBLIC ${UUIDS_CFLAGS_OTHER})
  target_compile_options(CpuProfiler PUBLIC ${STARDATE_CFLAGS_OTHER})
  target_compile_options(CpuProfiler PUBLIC ${PARALLEL_CFLAGS_OTHER})
  install(TARGETS CpuProfiler RUNTIME DESTINATION bin)
endif(BUILD_SHARED_LIBS)
