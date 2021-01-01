#ifndef LGPP_TYPE_HPP
#define LGPP_TYPE_HPP

#include "lgpp/trait.hpp"

namespace lgpp {
  template <typename T>
  struct Type: Trait {
    using Val = T;
    
    Type(VM& vm, string name, initializer_list<Trait *> parents = {}): Trait(vm, name, parents) {}
  };
}

#endif
