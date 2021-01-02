#ifndef LGPP_TYPES_PAIR_HPP
#define LGPP_TYPES_PAIR_HPP

#include "lgpp/type.hpp"

namespace lgpp::types {

  template <>
  inline void dump(Type<lgpp::Pair>& type, const lgpp::Pair& x, ostream &out) {
    out << '<';
    dump(x.first, out);
    out << ' ';
    dump(x.second, out);
    out << '>';
  }

  template <>
  inline void say(Type<lgpp::Pair>& type, const lgpp::Pair& x, ostream &out) {
    say(x.first, out);
    say(x.second, out);
  }

}

#endif
