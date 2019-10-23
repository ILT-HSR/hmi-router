#include "connection.h"

#include <boost/asio/bind_executor.hpp>
#include <boost/asio/dispatch.hpp>

#include <algorithm>
#include <iterator>
#include <queue>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace asio = boost::asio;

namespace hmi
{
  connection::connection(asio::io_context & context, logger_ptr logger) noexcept
      : stranded(context)
      , logging(std::move(logger))
  {
  }

  bool connection::add_error_handler(error_handler_t handler) noexcept
  {
    try
    {
      m_error_handlers.push_back(std::move(handler));
    }
    catch (...)
    {
      return false;
    }
    return true;
  }

  bool connection::add_message_handler(message_handler_t handler) noexcept
  {
    try
    {
      m_message_handlers.push_back(std::move(handler));
    }
    catch (...)
    {
      return false;
    }
    return true;
  }

  void connection::start() noexcept
  {
    dispatch([this, lifeline = shared_from_this()] { do_read(); });
  }

  void connection::send(std::string message) noexcept(std::is_nothrow_move_constructible_v<std::string>)
  {
    dispatch([this, lifeline = shared_from_this(), message = std::move(message)] {
      outgoing_messages().push(std::move(message));
      if (outgoing_messages().size() == 1)
      {
        do_send();
      }
    });
  }

  boost::system::error_code connection::terminate() noexcept
  {
    return do_terminate();
  }

  void connection::alert_error_handlers(boost::system::error_code error) noexcept
  {
    for_each(
        cbegin(m_error_handlers), cend(m_error_handlers), [&](auto const & handler) { handler(shared_from_this(), error); });
  }

  void connection::alert_message_handlers(std::string message) noexcept
  {
    for_each(cbegin(m_message_handlers), cend(m_message_handlers), [&](auto const & handler) {
      handler(shared_from_this(), message);
    });
  }

  std::queue<std::string> & connection::outgoing_messages() noexcept
  {
    return m_outgoing_messages;
  }

}  // namespace hmi
