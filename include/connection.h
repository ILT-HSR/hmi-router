#ifndef HMI_ROUTER_CONNECTION_H
#define HMI_ROUTER_CONNECTION_H

#include "logging.h"
#include "stranded.h"

#include <boost/system/error_code.hpp>
#include <spdlog/logger.h>

#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <type_traits>
#include <vector>

namespace hmi
{
  using connection_ptr = std::shared_ptr<struct connection>;

  struct connection
      : stranded
      , logging
      , std::enable_shared_from_this<connection>
  {
    using message_handler_t = std::function<void(std::string const &)>;
    using error_handler_t = std::function<void(boost::system::system_error const &)>;

    connection(connection const &) = delete;

    connection(connection &&) noexcept = delete;

    virtual ~connection() noexcept = default;

    connection & operator=(connection const &) = delete;

    connection & operator=(connection &&) noexcept = delete;

    bool add_error_handler(error_handler_t handler) noexcept;

    bool add_message_handler(message_handler_t handler) noexcept;

    void start() noexcept;

    void send(std::string message) noexcept(std::is_nothrow_move_constructible_v<std::string>);

    boost::system::system_error terminate() noexcept;

  protected:
    virtual void do_read() = 0;

    virtual void do_send() = 0;

    virtual boost::system::system_error do_terminate() = 0;

    std::vector<error_handler_t> & error_handlers() noexcept;

    std::vector<message_handler_t> & message_handlers() noexcept;

    std::queue<std::string> & outgoing_messages() noexcept;

  private:
    connection(boost::asio::io_context & context, logger_ptr logger) noexcept;

    std::vector<error_handler_t> m_error_handlers{};
    std::vector<message_handler_t> m_message_handlers{};

    std::queue<std::string> m_outgoing_messages{};
  };
}  // namespace hmi

#endif
