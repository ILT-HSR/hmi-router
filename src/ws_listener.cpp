#include "ws_listener.h"

#include "todo.h"

#include <boost/asio/socket_base.hpp>
#include <boost/system/error_code.hpp>
#include <spdlog/fmt/ostr.h>

#include <utility>

namespace asio = boost::asio;

namespace hmi
{
  ws_listener::ws_listener(asio::io_context & context, asio::ip::tcp::endpoint endpoint, logger_ptr logger)
      : listener(context, std::move(logger))
      , m_endpoint(std::move(endpoint))
      , m_acceptor(context)
  {
  }

  void ws_listener::do_start()
  {
    m_acceptor.open(m_endpoint.protocol());
    m_acceptor.set_option(asio::socket_base::reuse_address(true));
    m_acceptor.bind(m_endpoint);
    m_acceptor.listen(asio::socket_base::max_listen_connections);

    accept();
  }

  void ws_listener::do_terminate()
  {
    m_acceptor.cancel();
    m_acceptor.close();
  }

  void ws_listener::accept()
  {
    m_acceptor.async_accept(strand(), [&](auto error, auto socket) {
      if (error)
      {
        logger().error("Error accepting new connection: {}", error);
        return;
      }

      logger().debug("Accepted new connection from: {}", socket.remote_endpoint());

      todo("ws_listener::accept: Implement WebSocket connection class and creation");
    });
  }

}  // namespace hmi
