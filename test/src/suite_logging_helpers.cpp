#include "logging.h"

#include <cute.h>

void test_log_level_debug_to_string()
{
  ASSERT_EQUAL("debug", hmi::to_string(hmi::log_level::debug));
}

void test_log_level_info_to_string()
{
  ASSERT_EQUAL("info", hmi::to_string(hmi::log_level::info));
}

void test_log_level_warning_to_string()
{
  ASSERT_EQUAL("warning", hmi::to_string(hmi::log_level::warning));
}

void test_log_level_error_to_string()
{
  ASSERT_EQUAL("error", hmi::to_string(hmi::log_level::error));
}

void test_log_level_debug_from_string()
{
  ASSERT_EQUAL(hmi::log_level::debug, hmi::from_string<hmi::log_level>("debug"));
}

void test_log_level_info_from_string()
{
  ASSERT_EQUAL(hmi::log_level::info, hmi::from_string<hmi::log_level>("info"));
}

void test_log_level_warning_from_string()
{
  ASSERT_EQUAL(hmi::log_level::warning, hmi::from_string<hmi::log_level>("warning"));
}

void test_log_level_error_from_string()
{
  ASSERT_EQUAL(hmi::log_level::error, hmi::from_string<hmi::log_level>("error"));
}

std::pair<std::string, cute::suite> get_suite()
{
  return std::pair{"Logging Helpers",
                   cute::suite{
                       CUTE(test_log_level_debug_to_string),
                       CUTE(test_log_level_info_to_string),
                       CUTE(test_log_level_warning_to_string),
                       CUTE(test_log_level_error_to_string),
                       CUTE(test_log_level_debug_from_string),
                       CUTE(test_log_level_info_from_string),
                       CUTE(test_log_level_warning_from_string),
                       CUTE(test_log_level_error_from_string),
                   }};
}