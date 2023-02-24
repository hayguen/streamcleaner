cmake_minimum_required(VERSION 3.15)

# required to enable set_property(TARGET clsocket PROPERTY  MSVC_RUNTIME_LIBRARY
cmake_policy(SET CMP0091 NEW)
# set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)

project(streamcleaner)


########################################################################
# select the release build type by default to get optimization flags
########################################################################
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
   message(STATUS "Build type not specified: defaulting to release.")
endif(NOT CMAKE_BUILD_TYPE)

# set the reported version
add_definitions("-DSTREAMCLEANER_VERSION=\"0.0.1\"")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(PkgConfig)

# pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)

find_package(FFTW3F)
if (NOT FFTW3F_FOUND)
    message(FATAL_ERROR "please install FFTW3F")
endif()


if ( CMAKE_C_COMPILER_ID MATCHES "MSVC" )
  # using Visual Studio C++
  message(STATUS "INFO: detected MSVC: will not link math lib m")
  set(MATHLIB "")

  add_definitions("/D_CRT_SECURE_NO_WARNINGS")
  set(MSVC_DISABLED_WARNINGS_LIST "C4996")

else()
  message(STATUS "INFO: detected NO MSVC: ${CMAKE_C_COMPILER_ID}: will link math lib m")
  set(MATHLIB "m")
endif()


add_executable(streamcleaner cleanup.cpp)

# -Wsign-compare
target_compile_options(streamcleaner PRIVATE -Wall -Wextra -fvisibility=hidden)

if ( CMAKE_C_COMPILER_ID MATCHES "MSVC" )
    # make a static compilation with MSVC
    set_property(TARGET streamcleaner  PROPERTY  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()


target_include_directories(streamcleaner PRIVATE
    ${FFTW_INCLUDE_DIRS}  # PkgConfig::FFTW
    # ${THREADS_PTHREADS_INCLUDE_DIR}
)

target_link_libraries(streamcleaner PRIVATE
    ${FFTW3F_LIBRARIES}  # PkgConfig::FFTW
    # ${CMAKE_THREAD_LIBS_INIT}
    ${MATHLIB}
)


install(TARGETS streamcleaner RUNTIME DESTINATION bin)


########################################################################
# Create uninstall target
########################################################################
configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
@ONLY)

add_custom_target(uninstall
    ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)
