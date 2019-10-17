#include "logging.h"

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <stdexcept>
#include <utility>

namespace hmi
{
  auto level_string_map = std::array{std::pair{log_level::debug, "debug"},
                                     std::pair{log_level::info, "info"},
                                     std::pair{log_level::warning, "warning"},
                                     std::pair{log_level::error, "error"}};

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