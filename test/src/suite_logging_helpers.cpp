#include "logging.h"

#include <cute.h>
#include <spdlog/sinks/ostream_sink.h>

#include <memory>
#include <sstream>

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

void test_make_logger_accepts_a_single_sink()
{
  auto sink_buffer = std::ostringstream{};
  auto sink = std::make_shared<spdlog::sinks::ostream_sink_st>(sink_buffer);

  auto logger = hmi::make_logger("cute_test", hmi::log_level::info, sink);

  logger->set_pattern("%v");
  logger->info("THIS IS A TEST");

  ASSERT_EQUAL("THIS IS A TEST\n", sink_buffer.str());
}

void test_make_logger_accepts_multiple_sinks()
{
  auto first_sink_buffer = std::ostringstream{};
  auto first_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(first_sink_buffer);
  auto second_sink_buffer = std::ostringstream{};
  auto second_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(second_sink_buffer);

  auto logger = hmi::make_logger("cute_test", hmi::log_level::info, first_sink, second_sink);

  logger->set_pattern("%v");
  logger->info("THIS IS A TEST");

  ASSERT_EQUAL("THIS IS A TEST\n", first_sink_buffer.str());
  ASSERT_EQUAL("THIS IS A TEST\n", second_sink_buffer.str());
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
                       CUTE(test_make_logger_accepts_a_single_sink),
                       CUTE(test_make_logger_accepts_multiple_sinks),
                   }};
}