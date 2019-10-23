#ifndef HMI_ROUTER_TODO_H
#define HMI_ROUTER_TODO_H

#include <exception>
#include <iosfwd>
#include <string>

namespace hmi
{
  struct todo_exception : std::exception
  {
    explicit todo_exception(std::string todo);

    [[nodiscard]] char const * what() const noexcept override;

  private:
    std::string const m_todo;
  };

  std::ostream & operator<<(std::ostream & output, todo_exception const & todo);

  [[noreturn]] void todo(std::string reason);
}  // namespace hmi

#endif
