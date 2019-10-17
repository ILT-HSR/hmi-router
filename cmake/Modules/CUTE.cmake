include("CMakeParseArguments")

macro(cute_check)
  set(OPTIONS "REQUIRED")
  set(SINGLE_VAR_ARGS "ROOT_PATH")
  cmake_parse_arguments(CUTE_CHECK "${OPTIONS}" "${SINGLE_VAR_ARGS}" "" ${ARGN})

  if(NOT IS_ABSOLUTE ${CUTE_CHECK_ROOT_PATH})
    set(CUTE_CHECK_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${CUTE_CHECK_ROOT_PATH}")
  endif()

  set(CUTE_CHECK_ROOT_PATH "${CUTE_CHECK_ROOT_PATH}" CACHE STRING "The source root of CUTE")
  mark_as_advanced(CUTE_CHECK_ROOT_PATH)

  message(STATUS "CUTE: Checking for the presence of CUTE in ${CUTE_CHECK_ROOT_PATH}")

  if(NOT EXISTS "${CUTE_CHECK_ROOT_PATH}/cute/cute.h")
    if(CUTE_CHECK_REQUIRED)
      set(MESSAGE_LEVEL "FATAL_ERROR")
    else()
      set(MESSAGE_LEVEL "STATUS")
    endif()
    set(CUTE_FOUND NO)
    message(${MESSAGE_LEVEL} "CUTE: Could not find CUTE in ${CUTE_CHECK_ROOT_PATH}")
  else()
    set(CUTE_FOUND YES)
    message(STATUS "CUTE: Found CUTE in ${CUTE_CHECK_ROOT_PATH}")
  endif()

  if(CUTE_FOUND)
    include("CTest")
    enable_testing()
  endif()

  add_library("CUTE::CUTE" INTERFACE IMPORTED)
  set_property(TARGET "CUTE::CUTE"
    PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CUTE_CHECK_ROOT_PATH}/cute"
  )
endmacro()

function(cute_driver)
  set(SINGLE_VAR_ARGS "SOURCE")
  cmake_parse_arguments(CUTE_DRIVER "" "${SINGLE_VAR_ARGS}" "" ${ARGN})

  if(NOT CUTE_DRIVER_SOURCE)
    message(FATAL_ERROR "CUTE: missing driver source")
  endif()

  set(CUTE_DRIVER_TARGET_NAME "${PROJECT_NAME}-cute_driver" CACHE STRING "The name of the CUTE test driver target")
  mark_as_advanced(CUTE_DRIVER_TARGET_NAME)

  add_library("${CUTE_DRIVER_TARGET_NAME}" OBJECT "${CUTE_DRIVER_SOURCE}" ${CUTE_SUITE_UNPARSED_ARGUMENTS})
  target_link_libraries("${CUTE_DRIVER_TARGET_NAME}" PUBLIC "CUTE::CUTE")
endfunction()

function(cute_suite)
  set(OPTIONS "SKIP_DURING_BUILD")
  set(SINGLE_VAR_ARGS "NAME")
  set(MULTI_VAR_ARGS "LIBRARIES" "SOURCES")
  cmake_parse_arguments(CUTE_SUITE "${OPTIONS}" "${SINGLE_VAR_ARGS}" "${MULTI_VAR_ARGS}" ${ARGN})

  if(NOT TARGET "${CUTE_DRIVER_TARGET_NAME}")
    message(FATAL_ERROR "CUTE: no driver has been defined")
  endif()

  if(NOT CUTE_SUITE_NAME)
    message(FATAL_ERROR "CUTE: missing suite name")
  endif()

  if(NOT CUTE_SUITE_SOURCES)
    message(FATAL_ERROR "CUTE: missing suite source files")
  endif()

  set(CUTE_SUITE_TARGET_NAME "${PROJECT_NAME}-cute_suite-${CUTE_SUITE_NAME}")

  add_executable("${CUTE_SUITE_TARGET_NAME}" "${CUTE_SUITE_SOURCES}")
  target_link_libraries("${CUTE_SUITE_TARGET_NAME}" PRIVATE
    "${CUTE_DRIVER_TARGET_NAME}"
    "${CUTE_SUITE_LIBRARIES}"
  )
  add_test(NAME "${CUTE_SUITE_TARGET_NAME}"
    COMMAND "${CUTE_SUITE_TARGET_NAME}"
  )

  if(NOT CUTE_SUITE_SKIP_DURING_BUILD)
    set(SUITE_EXECUTION_TARGET "ALL")
  endif()

  if(NOT TARGET "cute_check_all")
    add_custom_target("cute_check_all" "${SUITE_EXECUTION_TARGET}" COMMAND "${CMAKE_CTEST_COMMAND}" --verbose --schedule-random)
  endif()

  add_dependencies("cute_check_all" "${CUTE_SUITE_TARGET_NAME}")
endfunction()