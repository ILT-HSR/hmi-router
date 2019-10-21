#include "logging.h"
#include "todo.h"
#include "ws_listener.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <exception>
#include <memory>

auto constexpr default_port = 2222;

int main()
{
  auto logger_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  auto logger = hmi::make_logger("hmi-router", hmi::log_level::info, logger_sink);

  logger->info("HMI Router starting up...");

  try
  {
    auto context = boost::asio::io_context{};
    auto endpoint = boost::asio::ip::tcp::endpoint{boost::asio::ip::make_address("127.0.0.1"), default_port};

    logger->info("Starting WebSocket listener on: {}", endpoint);
    auto listener = hmi::ws_listener{context, endpoint, logger};
    listener.start();

    context.run();
  }
  catch (hmi::todo_exception const & todo)
  {
    logger->critical("TODO: {}", todo);
  }
  catch (std::exception const & e)
  {
    logger->critical("Unexpected exception: {}", e.what());
  }
  catch (...)
  {
    logger->critical("An unhandled exception occurred");
  }
}
