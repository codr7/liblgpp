#ifndef LGPP_ENV_HPP
#define LGPP_ENV_HPP

#include <map>
#include <string>

#include "lgpp/val.hpp"

namespace lgpp {

  using namespace std;

  struct VM;
  
  struct Env {
    Env(VM &vm): vm(vm) {}
    
    VM &vm;
    map<string, Val> bindings;
  };

  template <typename...Args>
  void let(Env& env, string id, Args&&...args) {
    env.bindings.insert(make_pair(id, Val(forward<Args>(args)...)));
  }

  inline optional<Val> find(Env& env, const string& id) {
    auto found = env.bindings.find(id);
    if (found == env.bindings.end()) { return nullopt; }
    return found->second;
  }
}

#endif
