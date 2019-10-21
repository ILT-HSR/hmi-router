#include "todo.h"

#include <ostream>
#include <string>
#include <utility>

namespace hmi
{
  todo_exception::todo_exception(std::string todo)
      : m_todo(std::move(todo))
  {
  }

  char const * todo_exception::what() const noexcept
  {
    return m_todo.c_str();
  }

  std::ostream & operator<<(std::ostream & out, todo_exception const & todo)
  {
    return out << todo.what();
  }

  [[noreturn]] void todo(std::string reason)
  {
    throw todo_exception{std::move(reason)};
  }
}  // namespace hmi
