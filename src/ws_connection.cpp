#include "ws_connection.h"

#include <boost/asio/buffer.hpp>
#include <boost/beast/core/bind_handler.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/role.hpp>
#include <boost/beast/http/field.hpp>
#include <boost/beast/websocket/error.hpp>
#include <boost/beast/websocket/stream.hpp>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/ostr.h>

#include <cstddef>
#include <functional>
#include <memory>
#include <utility>

namespace asio = boost::asio;
namespace beast = boost::beast;

namespace
{
  struct ws_connection_ctor_proxy : hmi::ws_connection
  {
    template<typename... Args>
    explicit ws_connection_ctor_proxy(Args &&... args)
        : ws_connection(std::forward<Args>(args)...)
    {
    }
  };
}  // namespace

namespace hmi
{
  void ws_connection::start() noexcept
  {
    do_accept();
  }

  connection_ptr ws_connection::make(boost::asio::io_context & context, boost::asio::ip::tcp::socket socket, logger_ptr logger)
  {
    return std::make_shared<ws_connection_ctor_proxy>(context, std::move(socket), logger);
  }

  ws_connection::ws_connection(asio::io_context & context, asio::ip::tcp::socket socket, logger_ptr logger)
      : connection(context, std::move(logger))
      , m_stream(std::move(socket))
  {
    m_stream.set_option(beast::websocket::stream_base::timeout::suggested(beast::role_type::server));
    m_stream.set_option(beast::websocket::stream_base::decorator{[](auto & response) {
      response.set(beast::http::field::server, "hmi-router");
    }});
    m_stream.text(true);
  }

  void ws_connection::do_read()
  {
    m_stream.async_read(
        m_incoming,
        boost::beast::bind_front_handler(&ws_connection::on_read, std::static_pointer_cast<ws_connection>(shared_from_this())));
  }

  void ws_connection::do_send()
  {
    m_stream.async_write(
        asio::buffer(outgoing_messages().front()),
        boost::beast::bind_front_handler(&ws_connection::on_send, std::static_pointer_cast<ws_connection>(shared_from_this())));
  }

  boost::system::error_code ws_connection::do_terminate()
  {
    return {};
  }

  void ws_connection::do_accept() noexcept
  {
    m_stream.async_accept(boost::beast::bind_front_handler(&ws_connection::on_accept,
                                                           std::static_pointer_cast<ws_connection>(shared_from_this())));
  }

  void ws_connection::on_accept(boost::system::error_code error) noexcept
  {
    if (error)
    {
      alert_error_handlers(error);
      return;
    }
    connection::start();
  }

  void ws_connection::on_read(boost::system::error_code error, std::size_t count)
  {
    if (error == boost::beast::websocket::error::closed)
    {
      logger().info("Connection '{}' was closed while reading", fmt::ptr(this));
      return;
    }

    if (error)
    {
      logger().error("Connection '{}' encountered an error while reading: {}", fmt::ptr(this), error.message());
      alert_error_handlers(error);
      auto close_error = beast::error_code{};
      m_stream.close(boost::beast::websocket::close_code::protocol_error, close_error);
      if (close_error)
      {
        logger().warn("Error while closing connection '{}': {}", fmt::ptr(this), close_error.message());
      }
      return;
    }

    if (m_stream.got_binary())
    {
      logger().error("Received binary data on connection '{}'. Closing the connection due to protocol violation.",
                     fmt::ptr(this));
      m_stream.close(boost::beast::websocket::close_code::unknown_data);
      return;
    }

    alert_message_handlers(boost::beast::buffers_to_string(m_incoming.cdata()));
    m_incoming.consume(count);
    do_read();
  }

  void ws_connection::on_send(boost::system::error_code error, std::size_t count)
  {
    if (error == boost::beast::websocket::error::closed)
    {
      logger().info("Connection '{}' was closed while writing", fmt::ptr(this));
      return;
    }

    if (error)
    {
      logger().error("Connection '{}' encountered an error while writing: {}", fmt::ptr(this), error.message());
      auto close_error = beast::error_code{};
      m_stream.close(boost::beast::websocket::close_code::protocol_error, close_error);
      if (close_error)
      {
        logger().warn("Error while closing connection '{}': {}", fmt::ptr(this), close_error.message());
      }
      return;
    }

    if (count < outgoing_messages().front().size())
    {
      logger().error(
          "Incomplete write of {} vs. {} bytes on connection '{}'", count, outgoing_messages().front().size(), fmt::ptr(this));
      m_stream.close(boost::beast::websocket::close_code::internal_error);
      return;
    }

    outgoing_messages().pop();
    if (outgoing_messages().size())
    {
      dispatch(std::bind(&ws_connection::do_send, std::static_pointer_cast<ws_connection>(shared_from_this())));
    }
  }

}  // namespace hmi
