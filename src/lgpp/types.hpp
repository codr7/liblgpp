#ifndef LGPP_TYPES_HPP
#define LGPP_TYPES_HPP

#include "lgpp/coro.hpp"
#include "lgpp/type.hpp"

namespace lgpp::types {
  
  extern Type<Coro> Coro;
  
  extern Type<int> Int;

  template <>
  inline Val add(Type<int>& type, const int& x, Val y) { return Val(type, x + y.as(type)); }
  
  template <>
  inline Val sub(Type<int>& type, const int& x, Val y) { return Val(type, x - y.as(type)); }

  extern Type<Thread::Id> Thread;

}

#endif
