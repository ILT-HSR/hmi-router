#ifndef HMI_ROUTER_LISTENER_H
#define HMI_ROUTER_LISTENER_H

#include "connection.h"
#include "logging.h"
#include "stranded.h"

#include <boost/asio/io_context.hpp>

#include <functional>
#include <type_traits>
#include <vector>

namespace hmi
{
  struct listener
      : stranded
      , logging
  {
    using connection_handler_t = std::function<void(connection_ptr)>;

    listener(listener const &) noexcept(false) = default;

    listener(listener &&) noexcept(std::is_nothrow_move_constructible<logging>{} &&
                                   std::is_nothrow_move_constructible<stranded>{}) = default;

    listener(boost::asio::io_context & context, logger_ptr logger) noexcept;

    virtual ~listener() noexcept = default;

    listener & operator=(listener const &) noexcept(false) = default;

    listener & operator=(listener &&) noexcept(std::is_nothrow_move_constructible<std::vector<connection_handler_t>>{} &&
                                               std::is_nothrow_move_constructible<logging>{} &&
                                               std::is_nothrow_move_constructible<stranded>{}) = default;

    bool add_connection_listener(connection_handler_t handler) noexcept;

    void start();

    void terminate();

  protected:
    virtual void do_start() = 0;

    virtual void do_terminate() = 0;

    void alert_connection_handlers(connection_ptr connection) const noexcept;

  private:
    std::vector<connection_handler_t> m_connection_handlers;
  };
}  // namespace hmi

#endif
