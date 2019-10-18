#ifndef HMI_ROUTER_LOGGING_H
#define HMI_ROUTER_LOGGING_H

#include "from_string.h"

#include <spdlog/spdlog.h>

#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

namespace hmi
{
  using logger_ptr = std::shared_ptr<spdlog::logger>;

  struct logging
  {
    explicit logging(logger_ptr logger);

    void swap(logging & other) noexcept;

  protected:
    [[nodiscard]] spdlog::logger & logger() const noexcept;

  private:
    logger_ptr m_logger;
  };

  void swap(logging & lhs, logging & rhs) noexcept;

  enum struct log_level : std::underlying_type_t<std::byte>
  {
    debug,
    info,
    warning,
    error,
  };

  template<>
  log_level from_string(std::string const & name);

  std::string to_string(log_level level);

  template<typename... SinkTypes>
  std::shared_ptr<spdlog::logger> make_logger(std::string const & name, log_level level, std::shared_ptr<SinkTypes>... sinks)
  {
    auto logger = std::make_shared<spdlog::logger>(name, spdlog::sinks_init_list{sinks...});
    auto const real_level = [&] {
      switch (level)
      {
      case log_level::debug:
        return spdlog::level::debug;
      case log_level::info:
        return spdlog::level::info;
      case log_level::warning:
        return spdlog::level::warn;
      case log_level::error:
        return spdlog::level::err;
      default:
        return spdlog::level::info;
      }
    }();
    logger->set_level(real_level);
    return logger;
  }

}  // namespace hmi

#endif
