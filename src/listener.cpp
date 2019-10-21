#include "listener.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <utility>

namespace hmi
{
  listener::listener(boost::asio::io_context & context, logger_ptr logger, connection_factory_t connection_factory) noexcept
      : stranded(context)
      , logging(std::move(logger))
      , m_connection_factory(std::move(connection_factory))
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
    for_each(cbegin(m_connection_handlers), cend(m_connection_handlers), [&](auto const handler) { handler(connection); });
  }

}  // namespace hmi
