#ifndef LGPP_ENV_HPP
#define LGPP_ENV_HPP

#include <map>
#include <string>

namespace lgpp {
  struct Val;
  
  using Env = map<string, Val>;

  template <typename...Args>
  Env &set(Env &env, string id, Args&&...args) {
    env.insert(make_pair(id, Val(forward<Args>(args)...)));
    return env;
  }
}

#endif
