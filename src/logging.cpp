#include "logging.h"

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <stdexcept>
#include <string>
#include <utility>

namespace hmi
{
  auto constexpr level_string_map = std::array{std::pair{log_level::debug, "debug"},
                                               std::pair{log_level::info, "info"},
                                               std::pair{log_level::warning, "warning"},
                                               std::pair{log_level::error, "error"}};

  logging::logging(logger_ptr logger)
      : m_logger{std::move(logger)}
  {
    assert(logger);
  }

  void logging::swap(logging & other) noexcept
  {
    using std::swap;
    swap(m_logger, other.m_logger);
  }

  spdlog::logger & logging::logger() const noexcept
  {
    return *m_logger;
  }

  void swap(logging & lhs, logging & rhs) noexcept
  {
    lhs.swap(rhs);
  }

  template<>
  log_level from_string(std::string const & name)
  {
    auto mapping =
        find_if(level_string_map.cbegin(), level_string_map.cend(), [&](auto entry) { return name == entry.second; });
    if (mapping != level_string_map.cend())
    {
      return mapping->first;
    }
    throw std::invalid_argument{fmt::format("Unknown logging level {0}", name)};
  }

  std::string to_string(log_level level)
  {
    auto mapping =
        find_if(level_string_map.cbegin(), level_string_map.cend(), [&](auto entry) { return level == entry.first; });
    if (mapping != level_string_map.cend())
    {
      return mapping->second;
    }
    throw std::invalid_argument{
        fmt::format("Unknown logging level {0}", static_cast<std::underlying_type_t<log_level>>(level))};
  }

}  // namespace hmi
