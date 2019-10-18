#include "connection.h"

#include <boost/asio/bind_executor.hpp>
#include <boost/asio/dispatch.hpp>

#include <stdexcept>
#include <type_traits>
#include <utility>

namespace asio = boost::asio;

namespace hmi
{
  connection::connection(asio::io_context & context, logger_ptr logger) noexcept
      : m_context{context}
      , m_strand{m_context}
      , m_logger{logger ? std::move(logger) : throw std::invalid_argument{"Null logger!"}}
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

  boost::system::system_error connection::terminate() noexcept
  {
    return do_terminate();
  }

  asio::io_context & connection::context() noexcept
  {
    return m_context;
  }

  asio::io_context::strand const & connection::strand() const noexcept
  {
    return m_strand;
  }

  spdlog::logger & connection::logger() const noexcept
  {
    return *m_logger;
  }

  std::vector<connection::error_handler_t> & connection::error_handlers() noexcept
  {
    return m_error_handlers;
  }

  std::vector<connection::message_handler_t> & connection::message_handlers() noexcept
  {
    return m_message_handlers;
  }

  std::queue<std::string> & connection::outgoing_messages() noexcept
  {
    return m_outgoing_messages;
  }

  void connection::dispatch(std::function<void()> callable) noexcept
  {
    asio::dispatch(asio::bind_executor(m_strand, callable));
  }

}  // namespace hmi