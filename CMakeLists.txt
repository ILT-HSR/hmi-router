cmake_minimum_required(VERSION "3.9.0")

project("hmi-router"
  LANGUAGES CXX
  VERSION "1.0.0"
)

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

add_executable("${PROJECT_NAME}"
  "src/main.cpp"
)

target_include_directories("${PROJECT_NAME}"
  PRIVATE
  "include"
)

conan_target_link_libraries("${PROJECT_NAME}")