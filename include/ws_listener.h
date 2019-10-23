#ifndef HMI_ROUTER_WS_LISTENER_H
#define HMI_ROUTER_WS_LISTENER_H

#include "listener.h"
#include "logging.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace hmi
{
  struct ws_listener : listener
  {
    ws_listener(boost::asio::io_context & context, boost::asio::ip::tcp::endpoint endpoint, logger_ptr logger);

  protected:
    void do_start() override;
    void do_terminate() override;

  private:
    void accept();

    boost::asio::ip::tcp::endpoint m_endpoint;
    boost::asio::ip::tcp::acceptor m_acceptor;
  };
}  // namespace hmi

#endif
