cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# ---- Project ----

project(simdjson_proxy LANGUAGES CXX)

option(SIMDJSON_EXAMPLES "whether or not to build the examples" ON)

# ---- Fetch CPM ----

set(CPM_DOWNLOAD_VERSION 0.27.2)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake v${CPM_DOWNLOAD_VERSION}")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})

# ---- Add dependencies via CPM ----
# See https://github.com/TheLartians/CPM.cmake for details and examples

CPMAddPackage(
  NAME simdjson
  VERSION 1.0.0  
  # not using the repo as it takes longer to clone
  URL https://github.com/simdjson/simdjson/archive/refs/tags/v1.0.0.zip
  URL_HASH SHA256=93e465fee14b53086fb5284e6e992b1d1ca4c8d9ed441d6bcbb009b34e7de305
)

if(simdjson_ADDED)
  add_library(simdjson INTERFACE)
  target_include_directories(simdjson INTERFACE ${simdjson_SOURCE_DIR})
endif()

CPMAddPackage(
  NAME doctest
  GITHUB_REPOSITORY sthagen/doctest
  GIT_TAG 2020.12
)

# ---- Create binary ----

add_executable(simdjson_test main.cpp)
target_link_libraries(simdjson_test doctest simdjson)
set_target_properties(simdjson_test PROPERTIES CXX_STANDARD 17)

# ---- Enable testing ----

enable_testing() 
add_test(simdjson_test simdjson_test)

# ---- Create the examples ----

message(STATUS "Note the fetched includes are taken from below: ${FETCHCONTENT_BASE_DIR}")
if (SIDJSON_EXAMPLES)
    macro(simdjson_examples EXAMPLENAME)
        add_executable(${EXAMPLENAME} ${ARGN})
        set_target_properties(${EXAMPLENAME} PROPERTIES FOLDER examples)
        set_target_properties(${EXAMPLENAME} PROPERTIES CXX_STANDARD 17)
        target_include_directories(${EXAMPLENAME} PUBLIC "${FETCHCONTENT_BASE_DIR}/simdjson-src/")
        add_test(NAME ${EXAMPLENAME} COMMAND ${EXAMPLENAME})
    endmacro()
    add_subdirectory(examples)
endif()
