#ifndef HMI_ROUTER_CONNECTION_H
#define HMI_ROUTER_CONNECTION_H

#include "logging.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/io_context_strand.hpp>
#include <boost/system/error_code.hpp>

#include <functional>
#include <string>

namespace hmi
{
  struct connection
  {
    using message_handler_t = std::function<void(std::string const &)>;
    using error_handler_t = std::function<void(boost::system::system_error const &)>;

    connection(boost::asio::io_context & context, logger_ptr logger);

    void add_error_handler(message_handler_t handler);

    void add_message_handler(message_handler_t handler);

    void send(std::string message);

    boost::system::system_error terminate();

  protected:
    virtual void do_read();

    virtual void do_send();

    virtual void do_terminate();

    boost::asio::io_context & m_context;
    boost::asio::io_context::strand const m_strand{m_context};

    logger_ptr m_logger;
  };
}  // namespace hmi

#endif