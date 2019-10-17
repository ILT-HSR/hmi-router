#ifndef HMI_ROUTER_FROM_STRING_H
#define HMI_ROUTER_FROM_STRING_H

#include <string>

namespace hmi
{
  template<typename T>
  T from_string(std::string const & name);
}

#endif