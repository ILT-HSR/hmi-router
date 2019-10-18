#include "stranded.h"

#include <memory>
#include <utility>

namespace hmi
{
  stranded::stranded(boost::asio::io_context & context) noexcept
      : m_context{context}
      , m_strand{std::make_unique<boost::asio::io_context::strand>(m_context)}
  {
  }

  boost::asio::io_context & stranded::context() noexcept
  {
    return m_context;
  }

  boost::asio::io_context::strand const & stranded::strand() const noexcept
  {
    return *m_strand;
  }

  void stranded::swap(stranded & other) noexcept
  {
    using std::swap;
    swap(m_context, other.m_context);
    swap(m_strand, other.m_strand);
  }

  void swap(stranded & lhs, stranded & rhs) noexcept
  {
    lhs.swap(rhs);
  }
}  // namespace hmi
