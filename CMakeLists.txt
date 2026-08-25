# SPDX-License-Identifier: GPL-2.0-or-later

cmake_minimum_required(VERSION 3.20)
project(MouseInjectorRA VERSION 0.1.0 LANGUAGES C)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
set(RCHEEVOS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/rcheevos" CACHE PATH "rcheevos source")

file(GLOB RC CONFIGURE_DEPENDS
  "${RCHEEVOS_DIR}/src/*.c"
  "${RCHEEVOS_DIR}/src/rapi/*.c"
  "${RCHEEVOS_DIR}/src/rcheevos/*.c"
  "${RCHEEVOS_DIR}/src/rhash/*.c")
list(FILTER RC EXCLUDE REGEX "(rc_client_(external|raintegration)|rc_libretro)\\.c$")

add_library(Mouse_Injector_RA SHARED
  src/proxy.c
  src/proxy_ra.c
  src/proxy.def
  ${RC})

target_include_directories(Mouse_Injector_RA PRIVATE
  "${RCHEEVOS_DIR}/include"
  "${RCHEEVOS_DIR}/src")
target_compile_definitions(Mouse_Injector_RA PRIVATE
  RC_DISABLE_LUA
  RC_CLIENT_SUPPORTS_HASH
  WIN32_LEAN_AND_MEAN)
target_compile_options(Mouse_Injector_RA PRIVATE /O2 /W3 /wd4996)
target_link_libraries(Mouse_Injector_RA PRIVATE winhttp)
set_target_properties(Mouse_Injector_RA PROPERTIES
  PREFIX ""
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/dist")
