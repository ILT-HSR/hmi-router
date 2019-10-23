option(CLANG_TIDY_CHECK "Check source files using clang-tidy" ON)
option(CLANG_TIDY_ERRORS "Treat clang-tidy warnings as errors" NO)

find_program(CLANG_TIDY_EXE
  NAMES "clang-tidy"
  DOC "Path to the clang-tidy executable"
)

if(CLANG_TIDY_EXE AND CLANG_TIDY_CHECK)
  message(STATUS "Clang Tidy: Enabling static source file checks")
  set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE} -extra-arg=-Wno-unknown-warning-option)
  if(CLANG_TIDY_ERRORS)
    list(APPEND CMAKE_CXX_CLANG_TIDY "--warnings-as-errors='*'")
  endif()
endif()