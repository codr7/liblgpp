#ifndef LGPP_TYPES_BOOL_HPP
#define LGPP_TYPES_BOOL_HPP

#include "lgpp/type.hpp"

namespace lgpp::types {

  template <>
  inline void dump(Type<bool>& type, const bool& x, ostream &out) { out << (x ? 'T' : 'F'); }

  template <>
  inline bool is_true(Type<bool>& type, const bool& x) { return x; }

}

#endif
