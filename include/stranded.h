#ifndef HMI_ROUTER_STRANDED_H
#define HMI_ROUTER_STRANDED_H

#include <boost/asio/bind_executor.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/post.hpp>

#include <functional>
#include <memory>
#include <type_traits>

namespace hmi
{
  struct stranded
  {
    explicit stranded(boost::asio::io_context & context) noexcept;

    void swap(stranded & other) noexcept;

  protected:
    [[nodiscard]] boost::asio::io_context & context() noexcept;

    [[nodiscard]] boost::asio::io_context::strand const & strand() const noexcept;

    template<typename CallableType>
    std::void_t<> dispatch(CallableType callable) const;

    template<typename CallableType>
    std::void_t<> post(CallableType callable) const;

  private:
    std::reference_wrapper<boost::asio::io_context> m_context;
    std::unique_ptr<boost::asio::io_context::strand> m_strand;
  };

  template<typename CallableType>
  inline void stranded::dispatch(CallableType callable) const
  {
    boost::asio::dispatch(boost::asio::bind_executor(strand(), callable));
  }

  template<typename CallableType>
  inline void stranded::post(CallableType callable) const
  {
    boost::asio::post(boost::asio::bind_executor(strand(), callable));
  }

  void swap(stranded & lhs, stranded & rhs) noexcept;

}  // namespace hmi

#endif
