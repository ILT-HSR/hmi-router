#ifndef HMI_ROUTER_WS_CONNECTION_H
#define HMI_ROUTER_WS_CONNECTION_H

#include "connection.h"
#include "logging.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/websocket/stream.hpp>
#include <boost/system/error_code.hpp>

#include <cstddef>

namespace hmi
{
  struct ws_connection : connection
  {
    void start() noexcept override;

    static connection_ptr make(boost::asio::io_context & context, boost::asio::ip::tcp::socket socket, logger_ptr logger);

  protected:
    ws_connection(boost::asio::io_context & context, boost::asio::ip::tcp::socket socket, logger_ptr logger);

    void do_read() override;

    void do_send() override;

    boost::system::error_code do_terminate() override;

  private:
    void do_accept() noexcept;

    void on_accept(boost::system::error_code error) noexcept;

    void on_read(boost::system::error_code error, std::size_t count);

    void on_send(boost::system::error_code error, std::size_t count);

    boost::beast::websocket::stream<boost::beast::tcp_stream> m_stream;

    boost::beast::flat_buffer m_incoming{};
  };
}  // namespace hmi

#endif
