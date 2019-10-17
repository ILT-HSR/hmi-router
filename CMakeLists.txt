cmake_minimum_required(VERSION "3.9.0")

project("hmi-router"
  LANGUAGES CXX
  VERSION "1.0.0"
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

include("clang-tidy")
include("conan")
include("CUTE")

conan_check(REQUIRED)
conan_add_remote(NAME bincrafters URL "https://api.bintray.com/conan/bincrafters/public-conan")
conan_add_remote(NAME theirix URL "https://api.bintray.com/conan/theirix/conan-repo")
conan_cmake_run(CONANFILE "conanfile.txt"
  BASIC_SETUP
  CMAKE_TARGETS
  BUILD "missing"
  NO_OUTPUT_DIRS
)

# Core Object Library

add_library("${PROJECT_NAME}_objs" OBJECT
  "src/connection.cpp"
  "src/logging.cpp"
)

target_compile_options("${PROJECT_NAME}_objs" PUBLIC
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall
    -Wextra
    -Werror
    -pedantic-errors
  >
)

target_include_directories("${PROJECT_NAME}_objs"
  PUBLIC
  "include"
)

conan_target_link_libraries("${PROJECT_NAME}_objs")

get_target_property(${PROJECT_NAME}_SOURCES "${PROJECT_NAME}_objs" SOURCES)
foreach(SOURCE_FILE IN LISTS ${PROJECT_NAME}_SOURCES)
  if(SOURCE_FILE MATCHES "src/.*\.cpp")
    string(REGEX REPLACE
      "src/(.*)\.cpp"
      "include/\\1.h"
      HEADER_FILE
      "${SOURCE_FILE}"
    )
    get_filename_component(HEADER_FILE_PATH "${HEADER_FILE}" REALPATH)
    if(EXISTS "${HEADER_FILE_PATH}")
      target_sources("${PROJECT_NAME}_objs"
        INTERFACE
        "${HEADER_FILE_PATH}"
      )
    endif()
  endif()
endforeach()

# Executables

add_executable("${PROJECT_NAME}"
  "src/main.cpp"
)

target_link_libraries("${PROJECT_NAME}" PUBLIC
  "${PROJECT_NAME}_objs"
)

# Tests

cute_check(ROOT_PATH "third_party/CUTE" REQUIRED)

if(BUILD_TESTING)
  cute_driver(SOURCE "test/src/main.cpp")
  
  cute_suite(NAME "logging_helpers"
    LIBRARIES "${PROJECT_NAME}_objs"
    SOURCES "test/src/suite_logging_helpers.cpp"
  )
endif()