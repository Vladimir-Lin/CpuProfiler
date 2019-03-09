cmake_minimum_required(VERSION 3.0)
project(CpuProfiler)
include (CheckIncludeFile)
file(READ "VERSION.txt" VERSION)

option(NONAMESPACE "Don't use namespace in the header" off)

string(TIMESTAMP COMPILE_DATETIME "%Y/%m/%d %H:%M:%S")

if(NONAMESPACE)
  add_definitions("-DDONT_USE_NAMESPACE=1")
endif(NONAMESPACE)

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
  # message( ${CMAKE_CXX_COMPILER} )
  add_definitions("/wd4819")
  add_definitions("/nologo")
  if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    set(TARGET_ARCH x64)
    add_definitions("-DCIOS_X64=1")
  elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    set(TARGET_ARCH x86)
    add_definitions("-DCIOS_X86=1")
  endif()
endif(MSVC)

include_directories(include)
include_directories(SRCs/Windows)

set(HEADERS include/UUIDs.hpp SRCs/Windows/PrivateUUIDs.hpp)

set(CORES
  SRCs/UUIDs.cpp
  SRCs/nDestroyer.cpp
  SRCs/nEnabler.cpp
  SRCs/Convoy.cpp
  SRCs/Windows/PrivateConvoy.cpp
  SRCs/Windows/PrivateEnabler.cpp)

# Handling pkg-config file
configure_file(UUIDs.pc.in UUIDs.pc @ONLY)

if(BUILD_SHARED_LIBS)
  add_library(UUIDs SHARED ${CORES} ${HEADERS})
  target_link_libraries(UUIDs PUBLIC kernel32)
  add_definitions("-DBUILD_SHARED_LIBS=1")
  add_definitions("-DEXPORT_UUIDS=1")
  install(TARGETS UUIDs
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
  install(FILES include/UUIDs.hpp DESTINATION include)
  install(FILES ${CMAKE_BINARY_DIR}/UUIDs.pc DESTINATION lib/packages)
else(BUILD_SHARED_LIBS)
  add_library(UUIDs STATIC ${CORES} ${HEADERS})
  target_link_libraries(UUIDs PUBLIC kernel32)
  install(TARGETS UUIDs DESTINATION lib)
  install(FILES include/UUIDs.hpp DESTINATION include)
  install(FILES ${CMAKE_BINARY_DIR}/UUIDs.pc DESTINATION lib/packages)
endif(BUILD_SHARED_LIBS)
