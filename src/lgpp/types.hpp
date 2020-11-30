#ifndef LGPP_TYPES_HPP
#define LGPP_TYPES_HPP

#include "lgpp/coro.hpp"
#include "lgpp/stack.hpp"
#include "lgpp/type.hpp"

namespace lgpp::types {
  
  extern Trait Num, Seq;

  extern Type<Coro> Coro;
  
  extern Type<int> Int;

  template <>
  inline Val add(Type<int>& type, const int& x, Val y) { return Val(type, x + y.as(type)); }
  
  template <>
  inline Val sub(Type<int>& type, const int& x, Val y) { return Val(type, x - y.as(type)); }

  extern Type<lgpp::Trait *> Meta;

  extern Type<lgpp::Stack> Stack;

  extern Type<Thread::Id> Thread;

}

#endif
