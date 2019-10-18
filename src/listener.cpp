#include "listener.h"

#include <algorithm>
#include <cassert>
#include <execution>
#include <iterator>
#include <utility>

namespace hmi
{
  listener::listener(boost::asio::io_context & context, logger_ptr logger) noexcept
      : stranded(context)
      , logging(std::move(logger))
  {
    assert(logger);
  }

  bool listener::add_connection_listener(connection_handler_t handler) noexcept
  {
    try
    {
      m_connection_handlers.push_back(std::move(handler));
    }
    catch (...)
    {
      return false;
    }

    return true;
  }

  void listener::start()
  {
    post([this] { do_start(); });
  }

  void listener::terminate()
  {
    post([this] { do_terminate(); });
  }

  void listener::alert_connection_handlers(connection_ptr connection) const noexcept
  {
    for_each(std::execution::par_unseq, cbegin(m_connection_handlers), cend(m_connection_handlers), [&](auto const handler) {
      handler(connection);
    });
  }

}  // namespace hmi
