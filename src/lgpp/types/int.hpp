#ifndef LGPP_TYPES_INT_HPP
#define LGPP_TYPES_INT_HPP

#include "lgpp/type.hpp"

namespace lgpp::types {

  template <>
  inline void dump(Type<int>& type, const int& x, ostream &out) { out << x; }

  template <>
  inline bool is_true(Type<int>& type, const int& x) { return x; }

  template <>
  inline Val add(Type<int>& type, const int& x, Val y) { return Val(type, x + y.as(type)); }
  
  template <>
  inline Val sub(Type<int>& type, const int& x, Val y) { return Val(type, x - y.as(type)); }

}

#endif
