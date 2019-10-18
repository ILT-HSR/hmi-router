#ifndef HMI_ROUTER_LISTENER_H
#define HMI_ROUTER_LISTENER_H

#include "connection.h"
#include "logging.h"
#include "stranded.h"

#include <boost/asio/io_context.hpp>

#include <functional>
#include <vector>

namespace hmi
{
  struct listener
      : stranded
      , logging
  {
    using connection_factory_t = std::function<connection_ptr(boost::asio::io_context &, logger_ptr)>;

    using connection_handler_t = std::function<void(connection_ptr)>;

    listener(boost::asio::io_context & context, logger_ptr logger, connection_factory_t connection_factory) noexcept;

    bool add_connection_listener(connection_handler_t handler) noexcept;

    void start();

    void terminate();

  protected:
    virtual void do_start() = 0;

    virtual void do_terminate() = 0;

    void alert_connection_handlers(connection_ptr connection) const noexcept;

  private:
    connection_factory_t m_connection_factory;

    std::vector<connection_handler_t> m_connection_handlers;
  };
}  // namespace hmi

#endif
