#ifndef LGPP_ENV_HPP
#define LGPP_ENV_HPP

#include <map>
#include <string>

namespace lgpp {
  struct Val;
  
  using Env = map<string, Val>;
}

#endif
