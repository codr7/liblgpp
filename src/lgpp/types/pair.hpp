#ifndef LGPP_TYPES_PAIR_HPP
#define LGPP_TYPES_PAIR_HPP

#include "lgpp/type.hpp"

namespace lgpp::types {

  template <>
  inline void dump(Type<lgpp::Pair>& type, const lgpp::Pair& x, ostream &out) {
    out << x.first << ' ' << x.second;
  }
  
}

#endif
