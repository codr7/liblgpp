#ifndef LGPP_PRIM_HPP
#define LGPP_PRIM_HPP

#include "lgpp/pos.hpp"
#include "lgpp/thread.hpp"

namespace lgpp {
  struct Prim {
    using Imp = function<void (Thread &, Pos)>;

    Prim(string name, Imp imp): name(name), imp(imp) {}

    string name;
    Imp imp;
  };
}

#endif
