# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
add_compile_definitions(DEBUG_MODE=${DEBUG_MODE})
if(${DEBUG_MODE})
  set(SDKCONFIG_DEFAULTS sdkconfig)
message("debug")
else()
  set(SDKCONFIG_DEFAULTS sdkconfig.release)
message("release")
endif()
project(gme_carel)

