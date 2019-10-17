cmake_minimum_required(VERSION "3.9.0")

project("hmi-router"
  LANGUAGES CXX
  VERSION "1.0.0"
)

if(POLICY CMP0076)
  cmake_policy(SET CMP0076 NEW)
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("conan")

conan_check(REQUIRED)
conan_add_remote(NAME bincrafters URL "https://api.bintray.com/conan/bincrafters/public-conan")
conan_add_remote(NAME theirix URL "https://api.bintray.com/conan/theirix/conan-repo")
conan_cmake_run(CONANFILE "conanfile.txt"
  BASIC_SETUP
  CMAKE_TARGETS
  BUILD "missing"
  NO_OUTPUT_DIRS
)

include("CUTE")

# General Targets

add_library("${PROJECT_NAME}_objs" OBJECT
  "src/logging.cpp"
)

target_sources("${PROJECT_NAME}_objs"
  PUBLIC
  "include/logging.h"
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