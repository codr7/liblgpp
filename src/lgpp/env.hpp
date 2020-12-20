#ifndef LGPP_ENV_HPP
#define LGPP_ENV_HPP

#include <map>
#include <string>

#include "lgpp/val.hpp"

namespace lgpp {

  using namespace std;
  
  using Env = map<string, Val>;

  template <typename...Args>
  void set(Env& env, string id, Args&&...args) {
    env.insert(make_pair(id, Val(forward<Args>(args)...)));
  }

  inline Val *find(Env& env, const string& id) {
    auto found = env.find(id);
    if (found == env.end()) { return nullptr; }
    return &found->second;
  }
}

#endif
