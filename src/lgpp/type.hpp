#ifndef LGPP_TYPE_HPP
#define LGPP_TYPE_HPP

#include "lgpp/trait.hpp"

namespace lgpp {
  template <typename T>
  struct Type: Trait {
    using Val = T;
    
    Type(string name, initializer_list<Trait *> parents = {}): Trait(name, parents) {}
  };
}

#endif
