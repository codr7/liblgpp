#ifndef LGPP_TYPES_CORO_HPP
#define LGPP_TYPES_CORO_HPP

#include "lgpp/type.hpp"

namespace lgpp::types {

  template <>
  inline PC call(Type<lgpp::Coro>& type, const lgpp::Coro& imp, Thread& thread, PC return_pc, Pos pos) {
    return resume(imp, thread, return_pc, pos); 
  }

}

#endif
